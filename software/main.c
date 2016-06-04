#include <string.h>
#include "ch.h"
#include "hal.h"
#include "test.h"
#include "chprintf.h"
#include "shell.h"

#include "solarnode_usb.h"
#include "solarnode_shell.h"
#include "solarnode_debug.h"
#include "solarnode_config.h"
#include "solarnode_rfm69.h"
#include "solarnode_onewire.h"

static void flashLED(char number) {
    char i;
    for (i = 0; i < number; i++) {
        palWritePad(GPIOB, GPIOB_LED, 1);
        chThdSleepMilliseconds(10);
        palWritePad(GPIOB, GPIOB_LED, 0);
        if (i < number - 1) {
            chThdSleepMilliseconds(400);
        }
    }
}

int main(void) {
#ifdef _DEBUG
    initialise_monitor_handles();
#endif
    halInit();
    chSysInit();
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_PA12_RMP;
    ConfigInit();
    USBInit();
    shellInit();
    if (!rfm69Init()) {
        // TODO: handle RFM69 init failure
    }
    oneWireInit();

    while (true) {
        checkShell();
        chThdSleepMilliseconds(1000);
        flashLED(1);
    }
}
