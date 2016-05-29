#include <string.h>
#include "ch.h"
#include "hal.h"
#include "hal_community.h"
#include "solarnode_onewire.h"
#include "solarnode_hardware.h"

#define ONEWIRE_MASTER_CHANNEL        2
#define ONEWIRE_SAMPLE_CHANNEL        3
#define ONEWIRE_PAD_MODE_ACTIVE       (PAL_MODE_ALTERNATE(1) | PAL_STM32_OTYPE_OPENDRAIN)

static PWMConfig pwm_cfg = {
    0,
    0,
    NULL,
    {
     {PWM_OUTPUT_DISABLED, NULL},
     {PWM_OUTPUT_DISABLED, NULL},
     {PWM_OUTPUT_DISABLED, NULL},
     {PWM_OUTPUT_DISABLED, NULL}
    },
    0,
#if STM32_PWM_USE_ADVANCED
    0,
#endif
    0
};

static const onewireConfig ow_cfg = {
    &PWMD1,
    &pwm_cfg,
    PWM_OUTPUT_ACTIVE_LOW,
    ONEWIRE_MASTER_CHANNEL,
    ONEWIRE_SAMPLE_CHANNEL,
    ONEWIRE_PORT,
    ONEWIRE_PIN,
#if defined(STM32F1XX)
    ONEWIRE_PAD_MODE_IDLE,
#endif
    ONEWIRE_PAD_MODE_ACTIVE,
};


void oneWireInit() {
    onewireObjectInit(&OWD1);
}

char oneWireTempRead() {
    char temp = -127;
    uint8_t rombuf[24];
    uint8_t testbuf[12];
    size_t devices_on_bus = 0;

    onewireStart(&OWD1, &ow_cfg);
    /*
    while(onewireReset(&OWD1) == FALSE) {
        chThdSleepMilliseconds(1);
    }
    memset(rombuf, 0x55, sizeof(rombuf));
    devices_on_bus = onewireSearchRom(&OWD1, rombuf, 3);
    if (devices_on_bus == 1) {
        testbuf[0] = ONEWIRE_CMD_READ_ROM;
        onewireWrite(&OWD1, testbuf, 1, 0);
        onewireRead(&OWD1, testbuf, 8);
        osalDbgCheck(testbuf[7] == onewireCRC(testbuf, 7));
        osalDbgCheck(0 == memcmp(rombuf, testbuf, 8));
    }
    onewireStop(&OWD1);
    */
    return temp;
}

