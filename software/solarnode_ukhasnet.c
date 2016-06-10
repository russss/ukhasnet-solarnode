#include <string.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "solarnode_ukhasnet.h"
#include "solarnode_rfm69.h"

static THD_WORKING_AREA(transmitWorkingArea, 128);

static void transmitPacket(const char counter) {
    char sendbuf[MAX_MESSAGE];
    char pos = 0;

    pos += chsnprintf((char *)(&sendbuf + pos), MAX_MESSAGE - pos, "%i%c", 1, counter);

    chsnprintf(((char *)&sendbuf) + pos, MAX_MESSAGE - pos, "[%s]", "RUSSTEST");
    rfm69Send(sendbuf);
}

static THD_FUNCTION(transmit_thread, arg) {
    char counter = 'a';
    (void)arg;
    chRegSetThreadName("Transmit");
    // Wait for the radio to initialise
    while (!rfm69_ok) {
        chThdSleepMilliseconds(10);
    }
    transmitPacket(counter);

    while(true) {
        chThdSleepSeconds(20); // TODO: config
        if (counter < 'z') {
            counter++;
        } else {
            counter = 'b';
        }
        transmitPacket(counter);
    }
}

void ukhasnetInit() {
    chThdCreateStatic(transmitWorkingArea, sizeof(transmitWorkingArea), NORMALPRIO, transmit_thread, NULL);
}
