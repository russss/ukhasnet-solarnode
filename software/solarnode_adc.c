#include "ch.h"
#include "hal.h"
#include "solarnode_adc.h"
#include "solarnode_hardware.h"

// PA0 - ADC_IN0 - Vsuppmon
// PA1 - ADC_IN1 - Icharge
// PB1 - ADC_IN9 - Vbattmon
// ADC_IN16 - internal temperature
// ADC_IN17 - Vrefint

// Raw data acquired at a temperature of 30 °C (± 5 °C), VDDA= 3.3 V (± 10 mV)
#define VREFINT_CAL (adcsample_t *)0x1FFFF7BA
// TS ADC raw data acquired at a temperature of 30 °C (± 5 °C), VDDA= 3.3 V (± 10 mV)
#define TS_CAL1     (adcsample_t *)0x1FFFF7B8
// TS ADC raw data acquired at a temperature of 110 °C (± 5 °C), VDDA= 3.3 V (± 10 mV)
#define TS_CAL2     (adcsample_t *)0x1FFFF7C2

#define ADC_RES             4096
#define ADC_NUM_CHANNELS    5

static adcsample_t samples[ADC_NUM_CHANNELS];

static ADCConfig adccfg = {};
static ADCConversionGroup adcgrpcfg = {
      FALSE,
      ADC_NUM_CHANNELS,
      NULL,
      NULL,
      ADC_CFGR1_RES_12BIT,
      ADC_TR(0, 0),
      ADC_SMPR_SMP_239P5,
      ADC_CHSELR_CHSEL0 | ADC_CHSELR_CHSEL1 | ADC_CHSELR_CHSEL9 | \
          ADC_CHSELR_CHSEL16 | ADC_CHSELR_CHSEL17
};

static float get_vdda_voltage(adcsample_t vrefint) {
     return 3.3 * *VREFINT_CAL / vrefint;
}

static float get_temp(adcsample_t ts_data) {
    return (110.0 - 30.0)/(*TS_CAL2 - *TS_CAL1) * (ts_data - *TS_CAL1) + 30;
}

adc_values_t readADC(void) {
    adc_values_t values;
    adcStart(&ADCD1, &adccfg);
    adcSTM32SetCCR(ADC_CCR_TSEN | ADC_CCR_VREFEN);
    adcStartConversion(&ADCD1, &adcgrpcfg, samples, 1);
    chThdSleepMilliseconds(500);
    adcStop(&ADCD1);

    values.vdda_voltage = get_vdda_voltage(samples[4]);
    values.supply_voltage = ((float)samples[0] / ADC_RES * values.vdda_voltage) / VSUPPMON_DIVIDER;
    values.batt_voltage = ((float)samples[2] / ADC_RES * values.vdda_voltage) / VBATTMON_DIVIDER;
    values.charge_current = ((float)samples[1] / ADC_RES * values.vdda_voltage) / ICHARGE_DIVIDER;
    values.internal_temp = get_temp(samples[3]);
    return values;
}
