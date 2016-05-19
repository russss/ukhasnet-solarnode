#ifndef __SOLARNODE_ADC_H__
#define __SOLARNODE_ADC_H__


typedef struct {
    float vdda_voltage;
    float supply_voltage;
    float batt_voltage;
    float charge_current;
    float internal_temp;
} adc_values_t;

adc_values_t readADC(void);

#endif
