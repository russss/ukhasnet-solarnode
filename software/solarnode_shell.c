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
#include "solarnode_version.h"

static THD_WORKING_AREA(shellWorkingArea, 1024);
static thread_t *shelltp = NULL;

static void cmd_version(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: version\r\n");
        return;
    }

    char *id = (char*)UNIQUE_ID_ADDRESS;
    chprintf(chp, "Solarnode Software\n");
    chprintf(chp, "Version:      %s\n", SOLARNODE_VERSION);
    chprintf(chp, "Git revision: %s\n", SOLARNODE_GIT_REV);
    chprintf(chp, "Build date:   %s\n", SOLARNODE_BUILD_DATE);
    chprintf(chp, "Chip serial:  %02x%02x%02x%02x:%02x%02x%02x%02x:%02x%02x%02x%02x\n",
             id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], id[8], id[9], id[10], id[11]);
}

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
    chprintf(chp, "%10s|%8s|%8s|%5s|%5s|%9s\n",
                    "name", "addr", "stack", "prio", "refs", "state");
    chprintf(chp, "--------------------------------------------------\n");
    tp = chRegFirstThread();
    do {
      chprintf(chp, "%10s|%08lx|%08lx|%5lu|%5lu|%9s\r\n",
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
    float ow_result;
    uint8_t ow_error;
    ow_error = oneWireTempRead(&ow_result);
    if (ow_error == OW_SUCCESS) {
        chprintf(chp, "1-wire temperature: %.3f\n", ow_result);
    } else {
        chprintf(chp, "1-wire read: FAILED (error %i)\n", ow_error);
    }
}

static void cmd_config(BaseSequentialStream *chp, int argc, char *argv[]) {
    if (argc == 2) {
        if (strcmp(argv[0], "name") == 0) {
            // TODO: validate name meets requirements
            strncpy(node_config.name, argv[1], sizeof(node_config.name));
        }
        if (strcmp(argv[0], "position") == 0) {
            strncpy(node_config.position, argv[1], sizeof(node_config.position));
        }
        if (strcmp(argv[0], "repeat_count") == 0) {
            node_config.repeat_count = atoi(argv[1]);
        }
        if (strcmp(argv[0], "tx_interval") == 0) {
            node_config.tx_interval = atoi(argv[1]);
        }
        if (strcmp(argv[0], "tx_interval_low") == 0) {
            node_config.tx_interval_low = atoi(argv[1]);
        }
        if (strcmp(argv[0], "low_power_threshold") == 0) {
            node_config.low_power_threshold = atoi(argv[1]);
        }
        if (strcmp(argv[0], "zombie") == 0) {
            node_config.zombie = (argv[1][0] == '1');
        }
        if (strcmp(argv[0], "output_power") == 0) {
            int8_t pwr = atoi(argv[1]);
            if (pwr < -18) {
                chprintf(chp, "Error: Output power must be greater than -18dBm\n");
            } else if (node_config.rfm69h && pwr > 20) {
                chprintf(chp, "Error: Output power must be less than 20dBm for the RFM69H[W]\n");
            } else if (!node_config.rfm69h && pwr > 13) {
                chprintf(chp, "Error: Output power must be less than 13dBm for the RFM69[W]\n");
            } else {
                node_config.output_power = pwr;
            }
        }
        if (strcmp(argv[0], "rfm69h") == 0) {
            node_config.rfm69h = (argv[1][0] == '1');
        }
        int ret = ConfigSave();
        if (ret != FLASH_OK) {
            chprintf(chp, "Error: Flash write failed with status: %i\n", ret);
        }
    }
    chprintf(chp, "Node Configuration\n");
    chprintf(chp, "------------------\n");
    chprintf(chp, "name:                    %s\n",  node_config.name);
    chprintf(chp, "position:                %s\n",  node_config.position);
    chprintf(chp, "repeat_count:            %i\n",  node_config.repeat_count);
    chprintf(chp, "tx_interval:             %i seconds\n",  node_config.tx_interval);
    chprintf(chp, "tx_interval_low:         %i seconds\n",  node_config.tx_interval_low);
    chprintf(chp, "low_power_threshold:     %i mV\n",  node_config.low_power_threshold);
    chprintf(chp, "zombie:                  %i\n", node_config.zombie);
    chprintf(chp, "output_power:            %i dBmW\n", node_config.output_power);
    chprintf(chp, "rfm69h:                  %i\n", node_config.rfm69h);
}

static void cmd_upgrade(BaseSequentialStream *chp, int argc, char *argv[]) {
    (void)argv;
    if (argc > 0) {
        chprintf(chp, "Usage: upgrade\r\n");
        return;
    }

    chprintf(chp, "Starting bootloader...");
    RTC->BKP0R = 0x42;
    NVIC_SystemReset();
}

static const ShellCommand commands[] = {
    {"mem", cmd_mem},
    {"test", cmd_test},
    {"threads", cmd_threads},
    {"config", cmd_config},
    {"version", cmd_version},
    {"upgrade", cmd_upgrade},
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
