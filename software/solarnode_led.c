#include "ch.h"
#include "hal.h"
#include <solarnode_led.h>
#include <solarnode_ukhasnet.h>
#include <solarnode_rfm69.h>

static THD_WORKING_AREA(ledWorkingArea, 32);

static void flashLED(char number) {
    char i;
    for (i = 0; i < number; i++) {
        palWritePad(GPIOB, GPIOB_LED, 1);
        chThdSleepMilliseconds(5);
        palWritePad(GPIOB, GPIOB_LED, 0);
        if (i < number - 1) {
            chThdSleepMilliseconds(300);
        }
    }
}

static THD_FUNCTION(led_thread, arg) {
    (void)arg;
    chRegSetThreadName("LED");
    // Wait for the radio to initialise to avoid reporting a false error.
    chThdSleepMilliseconds(2000);
    while(true) {
        if (rfm69_ok) {
            if (node_state == STATE_ZOMBIE) {
                flashLED(1);
            } else {
                flashLED(2);
            }
            chThdSleepMilliseconds(10000);
        } else {
            flashLED(4);
            chThdSleepMilliseconds(500);
        }
    }
}

void ledInit() {
    chThdCreateStatic(ledWorkingArea, sizeof(ledWorkingArea), LOWPRIO, led_thread, NULL);
}
