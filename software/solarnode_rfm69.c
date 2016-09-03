// Some of this code derived from CUSF's avionics:
// https://github.com/cuspaceflight/avionics14/blob/master/firmware/rfm69.c
#include <string.h>
#include "ch.h"
#include "hal.h"
#include "solarnode_rfm69.h"
#include "solarnode_config.h"
#include "solarnode_ukhasnet.h"
#include "rfm69_config.h"
#include "rfm69_rdl.h"

#define MAILBOX_SIZE 2
#define RSSI_SAMPLES 10

volatile bool rfm69_ok = false;
volatile systime_t rfm69_last_reset = 0;
volatile char rfm69_rssi_threshold = 0;

static THD_WORKING_AREA(rfmWorkingArea, 256);

static volatile char rfm69_mp_b[MAILBOX_SIZE * MAX_MESSAGE];
MEMORYPOOL_DECL(rfm69_mp, MAX_MESSAGE, NULL);

static volatile msg_t rfm69_tx_mailbox_q[MAILBOX_SIZE];
MAILBOX_DECL(rfm69_tx_mailbox, rfm69_tx_mailbox_q, MAILBOX_SIZE);


static const SPIConfig spi_cfg = {
    NULL,
    GPIOA,
    GPIOA_NSS,
    SPI_CR1_BR_2 | SPI_CR1_BR_0,
    SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

static void rfm69_reset(void) {
    rfm69_last_reset = chVTGetSystemTime();
    palWritePad(GPIOA, GPIOA_RFM_RST, 1);
    chThdSleepMilliseconds(1);
    palWritePad(GPIOA, GPIOA_RFM_RST, 0);
    chThdSleepMilliseconds(50);
}

static uint8_t rfm69_spi_transfer_byte(SPIDriver* SPID, uint8_t out) {
    uint8_t val;
    spiExchange(SPID, 1, &out, &val);
    return val;
}

static uint8_t rfm69_register_read(SPIDriver* SPID, uint8_t reg_addr) {
    spiSelect(SPID);
    rfm69_spi_transfer_byte(SPID, reg_addr);
    uint8_t reg_value = rfm69_spi_transfer_byte(SPID, 0xff);
    spiUnselect(SPID);
    return reg_value;
}

static uint8_t rfm69_fifo_read(SPIDriver *SPID, uint8_t* buf) {
    uint8_t length;
    spiSelect(SPID);
    rfm69_spi_transfer_byte(SPID, RFM69_FIFO & ~0x80);
    length = rfm69_spi_transfer_byte(SPID, 0xff);
    spiReceive(SPID, length, buf);
    spiUnselect(SPID);
    return length;
}

static void rfm69_fifo_write(SPIDriver *SPID, uint8_t* buf, uint8_t length) {
    spiSelect(SPID);
    rfm69_spi_transfer_byte(SPID, RFM69_FIFO | 0x80);
    rfm69_spi_transfer_byte(SPID, length);
    spiSend(SPID, length, buf);
    spiUnselect(SPID);
}

static void rfm69_register_write (SPIDriver* SPID, uint8_t reg_addr, uint8_t reg_value) {
    spiSelect(SPID);
    rfm69_spi_transfer_byte (SPID, reg_addr | 0x80);
    rfm69_spi_transfer_byte (SPID, reg_value);
    spiUnselect(SPID);
}

int rfm69_wait_for_bit_high (SPIDriver* SPID, uint8_t reg_addr, uint8_t mask) {
    int niter = 60;
    while ((rfm69_register_read(SPID, reg_addr) & mask) == 0) {
        chThdSleepMilliseconds(30);
        if (--niter == 0) {
            return -1;
        }
    }
    return 0;
}

int rfm69_mode (SPIDriver* SPID, uint8_t mode) {
    uint8_t regVal = rfm69_register_read(SPID, RFM69_OPMODE);
    regVal &= ~RFM69_OPMODE_Mode_MASK;
    regVal |= RFM69_OPMODE_Mode_VALUE(mode);
    rfm69_register_write(SPID, RFM69_OPMODE,regVal);

    return rfm69_wait_for_bit_high(SPID, RFM69_IRQFLAGS1, RFM69_IRQFLAGS1_ModeReady);
}

void rfm69_restart_rx(SPIDriver* SPID) {
    uint8_t regVal = rfm69_register_read(SPID, RFM69_PACKETCONFIG2);
    regVal |= RFM69_PACKETCONFIG2_RestartRx;
    rfm69_register_write(SPID, RFM69_PACKETCONFIG2, regVal);
}

/* Set RFM69 output power in dBm.
 * NOTE: if the power is set to greater than +17 dBm, it must be reduced to below this
 * level after transmission.
 */
static bool rfm69_set_power(SPIDriver* SPID, int8_t power, bool rfm69h) {
    uint8_t RegPaLevel = 0;
    if (power < -18 || (power > 20 && rfm69h) || (power > 13 && !rfm69h)) {
        return false;
    }

    if (power < -2 || !rfm69h) {
        RegPaLevel = RFM69_PALEVEL_Pa0On | (power + 18);
    } else if (power < 14) {
        RegPaLevel = RFM69_PALEVEL_Pa1On | (power + 18);
    } else if (power < 18) {
        RegPaLevel = RFM69_PALEVEL_Pa1On | RFM69_PALEVEL_Pa2On | (power + 14);
    } else {
        RegPaLevel = RFM69_PALEVEL_Pa1On | RFM69_PALEVEL_Pa2On | (power + 11);
    }

    if (power > 17) {
        // Disable overcurrent protection
        rfm69_register_write(SPID, RFM69_OCP, 0x0F);
        // High power settings 18-20dBm (RFM69HW manual section 3.3.7)
        rfm69_register_write(SPID, 0x5A, 0x5D);
        rfm69_register_write(SPID, 0x5C, 0x7C);
    } else {
        rfm69_register_write(SPID, RFM69_OCP, 0x1A);
        rfm69_register_write(SPID, 0x5A, 0x55);
        rfm69_register_write(SPID, 0x5C, 0x70);
    }

    rfm69_register_write(SPID, RFM69_PALEVEL, RegPaLevel);
    return true;
}


int rfm69_frame_tx(SPIDriver* SPID, uint8_t *buf, int len) {
    // Set transmit power
    if (!rfm69_set_power(&SPID1, node_config.output_power, node_config.rfm69h)) {
        return -1;
    }

    // Power up TX and wait for state to stabilise
    if (rfm69_mode(SPID, RFM69_OPMODE_Mode_TX) != 0) {
        return -1;
    }
    if (rfm69_wait_for_bit_high(SPID, RFM69_IRQFLAGS1, RFM69_IRQFLAGS1_TxReady) != 0) {
        return -1;
    }

    rfm69_fifo_write(SPID, buf, len);

    // TX packet
    if (rfm69_wait_for_bit_high(SPID, RFM69_IRQFLAGS2, RFM69_IRQFLAGS2_PacketSent) != 0 ) {
        return -1;
    }

    // Return power to a safe level for receive
    if (!rfm69_set_power(&SPID1, 13, node_config.rfm69h)) {
        return -1;
    }
    // Back to standby
    rfm69_mode(SPID, RFM69_OPMODE_Mode_STDBY);
    return 0;
}

static bool rfm69_config(SPIDriver* SPID) {
    int i;
    for (i = 0; RFM69_CONFIG[i][0] != 255; i++) {
        rfm69_register_write(SPID, RFM69_CONFIG[i][0], RFM69_CONFIG[i][1]);
    }
    return true;
}


static bool calibrate_rssi(SPIDriver* SPID) {
    char i;
    int sum = 0;

    rfm69_register_write(SPID, RFM69_RSSITHRESH, 0xff);
    if (rfm69_wait_for_bit_high(SPID, RFM69_IRQFLAGS1, RFM69_IRQFLAGS1_Rssi) == -1) {
        return false;
    }

    for (i = 0; i < RSSI_SAMPLES; i++) {
        sum += rfm69_register_read(SPID, RFM69_RSSIVALUE);
        chThdSleepMilliseconds(200);
    }

    rfm69_rssi_threshold = sum/RSSI_SAMPLES;
    rfm69_register_write(SPID, RFM69_RSSITHRESH, rfm69_rssi_threshold);
    return true;
}

static int set_idle_mode(SPIDriver* SPID) {
    if (node_state == STATE_ZOMBIE) {
        return rfm69_mode(SPID, RFM69_OPMODE_Mode_SLEEP);
    } else {
        return rfm69_mode(SPID, RFM69_OPMODE_Mode_RX);
    }
}

static void check_received_packet(SPIDriver *SPID) {
    if ((rfm69_register_read(SPID, RFM69_IRQFLAGS2) & RFM69_IRQFLAGS2_PayloadReady) != 0) {
        uint8_t packet_len;
        uint8_t rfm69_packet_buf[MAX_MESSAGE];
        memset((void *)&rfm69_packet_buf, 0, MAX_MESSAGE);
        packet_len = rfm69_fifo_read(SPID, (uint8_t *)&rfm69_packet_buf);
        if (repeatPacket((uint8_t *)&rfm69_packet_buf, packet_len) == PACKET_REPEAT) {
            rfm69Send((char *)&rfm69_packet_buf);
        }
    } else if ((rfm69_register_read(SPID, RFM69_IRQFLAGS1) & RFM69_IRQFLAGS1_Timeout) != 0) {
        rfm69_restart_rx(SPID);
    }
}


void radio_loop(SPIDriver* SPID) {
    msg_t status;
    intptr_t msgp;
    systime_t last_rssi_calibration = 0;

    while(true) {
        // TODO: check wraparound?
        if (node_state != STATE_ZOMBIE &&
                (last_rssi_calibration == 0 || ST2S(chVTTimeElapsedSinceX(last_rssi_calibration)) > 600)) {
            if (!calibrate_rssi(SPID)) {
                return;
            }
            last_rssi_calibration = chVTGetSystemTime();
        }

        if (node_state != STATE_ZOMBIE) {
            check_received_packet(SPID);
        }

        // Sleep waiting for something to transmit
        status = chMBFetch(&rfm69_tx_mailbox, (msg_t*)&msgp, MS2ST(200));
        if (status != MSG_OK || msgp == 0) {
            // No messages to send, loop around
            continue;
        }
        if (rfm69_frame_tx(SPID, (uint8_t *) msgp, strlen((char *)msgp)) != 0) {
            return;
        }
        chPoolFree(&rfm69_mp, (void *)msgp);

        if (set_idle_mode(SPID) != 0) {
            return;
        }
    }
}

static THD_FUNCTION(rfm69_thread, arg) {
    (void) arg;
    chRegSetThreadName("RFM69");
    chThdSleepMilliseconds(500);

    spiStart(&SPID1, &spi_cfg);

    while(true) {
        rfm69_reset();
        while (rfm69_register_read(&SPID1, RFM69_VERSION) != 0x24) {
            chThdSleepMilliseconds(250);
            rfm69_reset();
        }
        if (!rfm69_config(&SPID1)) {
            continue;
        }
        if (set_idle_mode(&SPID1) != 0) {
            continue;
        }

        rfm69_ok = true;
        radio_loop(&SPID1);
        rfm69_ok = false;
        chThdSleepMilliseconds(100);
    }
}

void rfm69Init() {
    chPoolLoadArray(&rfm69_mp, (void *)rfm69_mp_b, MAILBOX_SIZE);
    chThdCreateStatic(rfmWorkingArea, sizeof(rfmWorkingArea), NORMALPRIO, rfm69_thread, NULL);
}

void rfm69Send(const char *packet) {
    char *msg = (char *)chPoolAlloc(&rfm69_mp);
    if (msg == NULL) {
        return;
    }
    strncpy(msg, packet, MAX_MESSAGE);
    if (chMBPost(&rfm69_tx_mailbox, (msg_t)msg, TIME_IMMEDIATE) != MSG_OK) {
        chPoolFree(&rfm69_mp, msg);
    }
}
