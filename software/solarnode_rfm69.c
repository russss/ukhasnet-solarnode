#include "ch.h"
#include "hal.h"
#include "solarnode_rfm69.h"
#include "rfm69_config.h"

bool radio_ok = false;

static THD_WORKING_AREA(rfmWorkingArea, 512);

static const SPIConfig spi_cfg = {
    NULL,
    GPIOB,
    12,
    SPI_CR1_BR_2,
    0
};

static void rfm69_reset(void) {
    palWritePad(GPIOA, GPIOA_RFM_RST, 1);
    chThdSleepMilliseconds(5);
    palWritePad(GPIOA, GPIOA_RFM_RST, 0);
    chThdSleepMilliseconds(100);
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

static void rfm69_config(SPIDriver* SPID) {
    int i;
    for (i = 0; RFM69_CONFIG[i][0] != 255; i++) {
        rfm69_register_write(SPID, RFM69_CONFIG[i][0], RFM69_CONFIG[i][1]);
    }
}

static THD_FUNCTION(rfm69_thread, arg) {
    (void) arg;
    chRegSetThreadName("RFM69");
    rfm69_reset();
    while (rfm69_register_read(&SPID1, RFM69_REG_10_VERSION) != 0x24) {
        chThdSleepMilliseconds(100);
        rfm69_reset();
    }
    radio_ok = true;
    rfm69_config(&SPID1);

    while(TRUE) {
        chThdSleepMilliseconds(100);
    }
}

void rfm69Init() {
    spiStart(&SPID1, &spi_cfg);
    chThdCreateStatic(rfmWorkingArea, sizeof(rfmWorkingArea), NORMALPRIO, rfm69_thread, NULL);
}
