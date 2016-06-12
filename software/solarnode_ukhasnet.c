#include <string.h>
#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "solarnode_ukhasnet.h"
#include "solarnode_config.h"
#include "solarnode_adc.h"
#include "solarnode_rfm69.h"

volatile char node_state = STATE_REPEATER;

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
                          "T%.1fV%.1f,%.1f,%.1fI%.2fZ%iX%i", values.internal_temp,
                          values.supply_voltage, values.batt_voltage, values.vdda_voltage,
                          values.charge_current,
                          node_state == STATE_ZOMBIE,
                          (!palReadPad(GPIOF, GPIOF_BATT_CHARGE)) | (!palReadPad(GPIOF, GPIOF_BATT_OK)) << 1
                          );
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

    // Transmit initial packet
    adc_values = readADC();
    transmitPacket(counter, adc_values);

    while(true) {
        // Set node state depending on battery voltage
        if (adc_values.batt_voltage < (node_config.low_power_threshold / 1000.0) || node_config.zombie) {
            node_state = STATE_ZOMBIE;
        } else if (adc_values.batt_voltage > (node_config.low_power_threshold / 1000.0 + 0.05)) {
            node_state = STATE_REPEATER;
        }

        if (node_state == STATE_ZOMBIE) {
            chThdSleepSeconds(node_config.tx_interval_low);
        } else {
            chThdSleepSeconds(node_config.tx_interval);
        }

        adc_values = readADC();
        if (counter < 'z') {
            counter++;
        } else {
            counter = 'b';
        }

        transmitPacket(counter, adc_values);
    }
}

char repeatPacket(uint8_t *packet, uint8_t packet_len) {
    uint8_t name_len = strlen(node_config.name);
    if (*packet < 49 || *packet > 57 || *(packet + packet_len - 1) != ']') {
        // Invalid packet format (or first byte is 0, so we drop)
        return PACKET_DROP;
    }
    if (strstr((const char *)packet, (const char *)&node_config.name)) {
        // We already repeated
        return PACKET_DROP;
    }
    if (packet_len + name_len + 1 > MAX_MESSAGE) {
        // Too long to repeat
        return PACKET_DROP;
    }

    (*packet)--;
    *(packet + packet_len - 1) = ',';
    memcpy((void*)(packet + packet_len), (void *)&node_config.name, name_len);
    *(packet + packet_len + name_len) = ']';
    return PACKET_REPEAT;
}

void ukhasnetInit() {
    chThdCreateStatic(transmitWorkingArea, sizeof(transmitWorkingArea), NORMALPRIO, transmit_thread, NULL);
}
