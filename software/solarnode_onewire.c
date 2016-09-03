#include <string.h>
#include "ch.h"
#include "hal.h"
#include "solarnode_onewire.h"
#include "solarnode_hardware.h"

#define  READ_ROM               0x33
#define  SKIP_ROM               0xcc
#define  READ_SCRATCHPAD        0xbe
#define  CONVERT_TEMP           0x44

#define TEMP_LSB                0
#define TEMP_MSB                1

#define owInput()        palSetPadMode(GPIOA, GPIOA_ONEWIRE, PAL_MODE_INPUT)
#define owOutput()       palSetPadMode(GPIOA, GPIOA_ONEWIRE, PAL_MODE_OUTPUT_PUSHPULL)
#define owHigh()         palSetPad(GPIOA, GPIOA_ONEWIRE)
#define owLow()          palClearPad(GPIOA, GPIOA_ONEWIRE)
#define owReadBit()      palReadPad(GPIOA, GPIOA_ONEWIRE)
#define owDelayus(delay) gptPolledDelay(&GPTD3, delay);

static const GPTConfig gpt3cfg = {
  1000000, // 1 MHz timer clock.
  NULL, // No callback
  0, 0
};

void oneWireInit() {
    gptStart(&GPTD3, &gpt3cfg);
}

static void SendByte(uint8_t val) {
    uint8_t n;
    owHigh();
    owOutput();
    for (n=0; n<8; n++) {
        owLow();
        if (val & 1) {
            owDelayus(6);
            owHigh();
            owDelayus(64);
        } else {
            owDelayus(60);
            owHigh();
            owDelayus(10);
        }
        val = val >> 1;
    }
}


static uint8_t ReadByte(void) {
    uint8_t n;
    uint8_t val = 0;
    for (n=0; n<8; n++) {
        val = val >> 1;
        owLow();
        owOutput();
        owDelayus(6);
        owHigh();
        owInput();
        owDelayus(9);
        if (owReadBit()) {
            val = val | 0x80;
        }
        owDelayus(55);
    }
    return  val;
}

static bool InitBus(void) {
    bool res;
    owLow();
    owOutput();
    chThdSleepMicroseconds(500);
    // The following polled delay appears to be necessary to wake the timer up,
    // so the next delay is accurate.
    owDelayus(10);
    owHigh();
    owInput();
    owDelayus(80);
    res = owReadBit();
    chThdSleepMicroseconds(410);
    return !res;
}

uint8_t ow_crc8(const uint8_t *addr, uint8_t len) {
    uint8_t inbyte, i, mix;
    uint8_t crc = 0;
    while (len--) {
        inbyte = *addr++;
        for (i = 8; i; i--) {
            mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if (mix) {
                crc ^= 0x8C;
            }
            inbyte >>= 1;
        }
    }
    return crc;
}

uint8_t oneWireTempRead(float* value) {
    uint8_t n;
    uint8_t pad[9];
    if (!InitBus()) {
        return OW_NO_DEVICES;
    }
    SendByte(SKIP_ROM);
    SendByte(CONVERT_TEMP);
    // Conversion time at 12-bit resolution == 750ms
    chThdSleepMilliseconds(760);
    if (!InitBus()) {
        return OW_ERROR;
    }
    SendByte(SKIP_ROM);
    SendByte(READ_SCRATCHPAD);
    for (n=0; n<9; n++) {
        pad[n] = ReadByte();
    }
    if (ow_crc8((const uint8_t *)&pad, 8) != pad[8]) {
        return OW_CRC_ERROR;
    }
    if (pad[TEMP_LSB] == 0x50 && pad[TEMP_MSB] == 0x05) {
        // Default values for temperature, assume error.
        return OW_ERROR;
    }
    *value = (int16_t)((pad[TEMP_MSB] << 8) | pad[TEMP_LSB]) / 16.0;
    return OW_SUCCESS;
}

uint8_t oneWireTempReadRetry(float* value) {
    uint8_t i, err;
    for (i = 0; i < 3; i++) {
        err = oneWireTempRead(value);
        if (err == OW_SUCCESS) {
            return err;
        }
        chThdSleepMilliseconds(50);
    }
    return err;
}

