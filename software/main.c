#include <string.h>
#include "ch.h"
#include "hal.h"
#include "test.h"
#include "chprintf.h"
#include "shell.h"

#include "solarnode_usb.h"
#include "solarnode_shell.h"
#include "solarnode_debug.h"

int main(void) {
#ifdef _DEBUG
    initialise_monitor_handles();
#endif
    halInit();
    chSysInit();

    SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_PA12_RMP;

    USBInit();
    shellInit();

    while (true) {
        checkShell();
        chThdSleepMilliseconds(1000);
    }
}
