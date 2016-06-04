#include "ch.h"
#include "hal.h"
#include "solarnode_rfm69.h"

static THD_WORKING_AREA(rfmWorkingArea, 1024);

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
    chThdSleepMilliseconds(20);
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

static THD_FUNCTION(rfm69_thread, arg) {
    (void) arg;
    chRegSetThreadName("RFM69");

    while(TRUE) {
        chThdSleepMilliseconds(100);
    }
}

static bool rfm69_test() {
    if (rfm69_register_read(&SPID1, 0x10) != 0x24) {
        return false;
    }
    return true;
}

bool rfm69Init() {
    rfm69_reset();
    spiStart(&SPID1, &spi_cfg);
    if (!rfm69_test()) {
        return false;
    }
    chThdCreateStatic(rfmWorkingArea, sizeof(rfmWorkingArea), NORMALPRIO, rfm69_thread, NULL);
    return true;
}
