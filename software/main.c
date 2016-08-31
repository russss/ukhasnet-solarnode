#include <string.h>
#include "ch.h"
#include "hal.h"
#include "test.h"
#include "chprintf.h"
#include "shell.h"

#include "solarnode_usb.h"
#include "solarnode_shell.h"
#include "solarnode_config.h"
#include "solarnode_rfm69.h"
#include "solarnode_onewire.h"
#include "solarnode_led.h"
#include "solarnode_ukhasnet.h"

// Configure watchdog: 1000 * 64 / 40kHz = 1.6 seconds
static const WDGConfig wdgcfg = {
  STM32_IWDG_PR_64,
  STM32_IWDG_RL(1000),
  STM32_IWDG_WIN_DISABLED
};

static void hardware_config(void) {
    // Remap pins PA11 and PA12 to enable USB
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_PA12_RMP;
    // Stop watchdog when core is halted for debugging
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;
}

int main(void) {
    halInit();
    chSysInit();
    hardware_config();
    ledInit();
    ConfigInit();
    USBInit();
    shellInit();
    oneWireInit();
    rfm69Init();
    ukhasnetInit();
#ifndef _DEBUG
    wdgStart(&WDGD1, &wdgcfg);
#endif

    while (true) {
        checkShell();
        chThdSleepMilliseconds(1000);
#ifndef _DEBUG
        wdgReset(&WDGD1);
#endif
    }
}


