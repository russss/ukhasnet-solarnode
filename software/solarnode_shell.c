#include <string.h>
#include <stdlib.h>
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
    chprintf(chp, "Charging: %i, OK: %i\n", !palReadPad(GPIOF, GPIOF_BATT_CHARGE),
                                            !palReadPad(GPIOF, GPIOF_BATT_OK));
    if (rfm69_ok) {
        chprintf(chp, "Radio OK. RSSI threshold: -%.1f, last reset %i seconds ago.\n",
                rfm69_rssi_threshold / 2.0,
                ST2S(chVTTimeElapsedSinceX(rfm69_last_reset)));
    } else {
        chprintf(chp, "Radio FAILED.\n");
    }
}

static void cmd_config(BaseSequentialStream *chp, int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[0], "name") == 0) {
            strncpy(node_config.name, argv[1], sizeof(node_config.name));
        }
        if (strcmp(argv[0], "position") == 0) {
            strncpy(node_config.position, argv[1], sizeof(node_config.position));
        }
        if (strcmp(argv[0], "repeat_count") == 0) {
            node_config.repeat_count = atoi(argv[1]);
        }
        if (strcmp(argv[0], "txinterval") == 0) {
            node_config.tx_interval = atoi(argv[1]);
        }
        if (strcmp(argv[0], "txinterval_low") == 0) {
            node_config.tx_interval_low = atoi(argv[1]);
        }
        if (strcmp(argv[0], "low_power_threshold") == 0) {
            node_config.low_power_threshold = atoi(argv[1]);
        }
        int ret = ConfigSave();
        if (ret != FLASH_OK) {
            chprintf(chp, "Flash write failed with status: %i\n", ret);
        }
    }
    chprintf(chp, "Node Configuration\n");
    chprintf(chp, "------------------\n");
    chprintf(chp, "name:                    %s\n",  node_config.name);
    chprintf(chp, "position:                %s\n",  node_config.position);
    chprintf(chp, "repeat_count:            %i\n",  node_config.repeat_count);
    chprintf(chp, "tx_interval:             %i  seconds\n",  node_config.tx_interval);
    chprintf(chp, "tx_interval_low:         %i  seconds\n",  node_config.tx_interval_low);
    chprintf(chp, "low_power_threshold:     %i  mV\n",  node_config.low_power_threshold);
}


static const ShellCommand commands[] = {
    {"mem", cmd_mem},
    {"test", cmd_test},
    {"threads", cmd_threads},
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
