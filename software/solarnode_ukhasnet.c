#include <string.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "solarnode_ukhasnet.h"
#include "solarnode_config.h"
#include "solarnode_adc.h"
#include "solarnode_rfm69.h"

static THD_WORKING_AREA(transmitWorkingArea, 256);

static void transmitPacket(const char counter, const adc_values_t values) {
    char sendbuf[MAX_MESSAGE];
    char pos = 0;

    pos += chsnprintf(((char *)&sendbuf) + pos, MAX_MESSAGE - pos, "%i%c",
                       node_config.repeat_count, counter);

    if (counter == 'a' && node_config.position[0] != 0) {
        // First packet, add location
        pos += chsnprintf(((char *)&sendbuf) + pos, MAX_MESSAGE - pos, "L%s",
                            node_config.position);
    } else {
        // Normal packet
        pos += chsnprintf(((char *)&sendbuf) + pos, MAX_MESSAGE - pos,
                          "T%.1fV%.1f,%.1f,%.1fI%.2fX%i,%i", values.internal_temp,
                          values.supply_voltage, values.batt_voltage, values.vdda_voltage,
                          values.charge_current,
                          !palReadPad(GPIOF, GPIOF_BATT_CHARGE), !palReadPad(GPIOF, GPIOF_BATT_OK));
    }

    chsnprintf(((char *)&sendbuf) + pos, MAX_MESSAGE - pos, "[%s]", node_config.name);
    rfm69Send(sendbuf);
}

static THD_FUNCTION(transmit_thread, arg) {
    adc_values_t adc_values;
    char counter = 'a';
    (void)arg;
    chRegSetThreadName("Transmit");
    // Wait for the radio to initialise
    while (!rfm69_ok) {
        chThdSleepMilliseconds(10);
    }
    transmitPacket(counter, adc_values);

    while(true) {
        chThdSleepSeconds(node_config.tx_interval); // TODO: low-batt mode
        adc_values = readADC();
        if (counter < 'z') {
            counter++;
        } else {
            counter = 'b';
        }
        transmitPacket(counter, adc_values);
    }
}

void ukhasnetInit() {
    chThdCreateStatic(transmitWorkingArea, sizeof(transmitWorkingArea), NORMALPRIO, transmit_thread, NULL);
}
