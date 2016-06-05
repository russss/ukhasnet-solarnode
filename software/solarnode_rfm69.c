// Some of this code derived from CUSF's avionics:
// https://github.com/cuspaceflight/avionics14/blob/master/firmware/rfm69.c
#include <string.h>
#include "ch.h"
#include "hal.h"
#include "solarnode_rfm69.h"
#include "solarnode_debug.h"
#include "rfm69_config.h"
#include "rfm69_rdl.h"

#define MAX_MESSAGE 64
#define MAILBOX_SIZE 4

volatile bool radio_ok = false;
static THD_WORKING_AREA(rfmWorkingArea, 512);

MEMORYPOOL_DECL(rfm69_mp, MAILBOX_SIZE * MAX_MESSAGE, NULL);
static volatile msg_t rfm69_tx_mailbox_q[MAILBOX_SIZE];
MAILBOX_DECL(rfm69_tx_mailbox, rfm69_tx_mailbox_q, MAILBOX_SIZE);

void radio_loop(void);


static const SPIConfig spi_cfg = {
    NULL,
    GPIOB,
    12,
    SPI_CR1_BR_2,
    0
};

static void rfm69_reset(void) {
    palWritePad(GPIOA, GPIOA_RFM_RST, 1);
    chThdSleepMilliseconds(25);
    palWritePad(GPIOA, GPIOA_RFM_RST, 0);
    chThdSleepMilliseconds(150);
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

int rfm69_frame_tx(SPIDriver* SPID, uint8_t *buf, int len) {
    // Power up TX
    if (rfm69_mode(SPID, RFM69_OPMODE_Mode_TX) != 0) {
        return -1;
    }
    if (rfm69_wait_for_bit_high(SPID, RFM69_IRQFLAGS1, RFM69_IRQFLAGS1_TxReady) != 0) {
        return -1;
    }
    // Write frame to FIFO
    spiSelect(SPID);
    rfm69_spi_transfer_byte(SPID, RFM69_FIFO | 0x80);
    // packet length
    rfm69_spi_transfer_byte(SPID, len);

    int i;
    for (i = 0; i < len; i++) {
        rfm69_spi_transfer_byte(SPID, buf[i]);
    }
    spiUnselect(SPID);

    // TX packet
    if (rfm69_wait_for_bit_high(SPID, RFM69_IRQFLAGS2, RFM69_IRQFLAGS2_PacketSent) != 0 ) {
        return -1;
    }
    // Back to standby
    rfm69_mode(SPID, RFM69_OPMODE_Mode_STDBY);
    return 0;
}

static void rfm69_config(SPIDriver* SPID) {
    int i;
    for (i = 0; RFM69_CONFIG[i][0] != 255; i++) {
        rfm69_register_write(SPID, RFM69_CONFIG[i][0], RFM69_CONFIG[i][1]);
    }
}

void radio_loop() {
    msg_t status;
    intptr_t msgp;

    while(true) {
        /*
        status = chMBFetch(&rfm69_tx_mailbox, (msg_t*)&msgp, TIME_INFINITE);
        if (status != MSG_OK || msgp == 0) {
            chThdSleepMilliseconds(1);
            continue;
        }*/
        char test[] = "1a[RUSSTEST]";
        msgp = &test;
        if (rfm69_frame_tx(&SPID1, (uint8_t *) msgp, strlen((char *)msgp)) != 0) {
            return;
        }
        chThdSleepMilliseconds(2000);
        //chPoolFree(&rfm69_mp, (void *)msgp);
    }
}

static THD_FUNCTION(rfm69_thread, arg) {
    (void) arg;

    chRegSetThreadName("RFM69");

    while(true) {
        rfm69_reset();
        while (rfm69_register_read(&SPID1, RFM69_VERSION) != 0x24) {
            chThdSleepMilliseconds(100);
            rfm69_reset();
        }
        rfm69_config(&SPID1);
        if (rfm69_mode(&SPID1, RFM69_OPMODE_Mode_STDBY) != 0) {
            continue;
        }

        radio_ok = true;
        radio_loop();
        radio_ok = false;

        chThdSleepMilliseconds(500);
    }
}

void rfm69Init() {
    spiStart(&SPID1, &spi_cfg);
    chThdCreateStatic(rfmWorkingArea, sizeof(rfmWorkingArea), NORMALPRIO, rfm69_thread, NULL);
}

void rfm69Send(const char *packet) {
    msg_t *msg = (msg_t *)chPoolAlloc(&rfm69_mp);
    if (msg == NULL) {
        return;
    }
    strncpy((char *)msg, packet, MAX_MESSAGE);
    chMBPost(&rfm69_tx_mailbox, *msg, TIME_IMMEDIATE);
}
