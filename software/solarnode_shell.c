#include <string.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "shell.h"

#include "solarnode_shell.h"
#include "solarnode_config.h"
#include "solarnode_adc.h"
#include "solarnode_onewire.h"
#include "solarnode_usb.h"
#include "solarnode_rfm69.h"

static THD_WORKING_AREA(shellWorkingArea, 1024);
static thread_t *shelltp = NULL;

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  size_t n, size;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
    static const char *states[] = {CH_STATE_NAMES};
    thread_t *tp;

    (void)argv;
    if (argc > 0) {
      chprintf(chp, "Usage: threads\r\n");
      return;
    }
    chprintf(chp, "    addr    stack prio refs     state\r\n");
    tp = chRegFirstThread();
    do {
      chprintf(chp, "%10s %08lx %08lx %4lu %4lu %9s %lu\r\n",
               tp->p_name, (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
               (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
               states[tp->p_state]);
      tp = chRegNextThread(tp);
    } while (tp != NULL);
}

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: test\r\n");
        return;
    }
    adc_values_t values = readADC();
    chprintf(chp, "Vsupp:                %.3fV\n", values.supply_voltage);
    chprintf(chp, "Vbatt:                %.3fV\n", values.batt_voltage);
    chprintf(chp, "Icharge:              %.3fA\n", values.charge_current);
    chprintf(chp, "Tsen:                 %.2f\n", values.internal_temp);
    chprintf(chp, "Vdda:                 %.3fV\n", values.vdda_voltage);
    chprintf(chp, "---------------------------\n");
    chprintf(chp, "Charging: %i, OK: %i\n", !palReadPad(GPIOF, 0), !palReadPad(GPIOF, 1));
    chprintf(chp, "Radio state: %s, last reset %i seconds ago.\n",
            radio_ok?"OK":"Failed",
            ST2S(chVTTimeElapsedSinceX(radio_last_reset)));
}

static void cmd_config(BaseSequentialStream *chp, int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[0], "name") == 0) {
            strcpy(node_config.name, argv[1]);
        }

        ConfigSave();
    }
    chprintf(chp, "Name:                 %s\n",  node_config.name);
}


static const ShellCommand commands[] = {
    {"mem", cmd_mem},
    {"threads", cmd_threads},
    {"test", cmd_test},
    {"config", cmd_config},
    {NULL, NULL}
};

const ShellConfig shell_cfg = {
  (BaseSequentialStream *)&SDU1,
  commands
};

void checkShell() {
    if (!shelltp && (SDU1.config->usbp->state == USB_ACTIVE)) {
        shelltp = shellCreateStatic(&shell_cfg, shellWorkingArea, sizeof(shellWorkingArea), NORMALPRIO);
    } else if (chThdTerminatedX(shelltp)) {
        chThdRelease(shelltp);
        shelltp = NULL;
    }
}
