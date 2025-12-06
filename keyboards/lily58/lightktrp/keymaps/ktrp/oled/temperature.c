#include "progmem.h"
#include "timer.h"
#include "analog.h"
#include "oled_driver.h"

#include "temperature.h"

#ifndef TEMPERATURE_UNITS
/* Choose 'C' for Celsius or 'F' for Fahrenheit. */
#    define TEMPERATURE_UNITS 'C'
#endif

#ifndef TEMP_SENSOR_THROTTLE
#    define TEMP_SENSOR_THROTTLE 2000
#endif

float read_onboard_temperature(const char unit) {
    const float ref_voltage      = 3.3f;
    const float conversionFactor = ref_voltage / (1 << 10);

    adcAcquireBus(&ADCD1);
    adcRPEnableTS(&ADCD1);

    float adc = (float)adc_read(TO_MUX(4, 0)) * conversionFactor;

    adcRPDisableTS(&ADCD1);
    adcReleaseBus(&ADCD1);

    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    if (unit == 'C') {
        return tempC;
    } else if (unit == 'F') {
        return tempC * 9 / 5 + 32;
    }

    return -1.0f;
}

uint32_t last_templ_sampled_at = 0;
char     temp_as_chars[5];

void updateTemperatureSample(void) {
    if (last_templ_sampled_at == 0 || (timer_elapsed32(last_templ_sampled_at) > TEMP_SENSOR_THROTTLE)) {
        last_templ_sampled_at  = timer_read32();
        float temperature_read = read_onboard_temperature(TEMPERATURE_UNITS);

        if (temperature_read > 999) {
            temperature_read = 999.0f;
        } else if (temperature_read < 0) {
            temperature_read = 0.0f;
        }
        int temperature = ((temperature_read) >= 0 ? (int)((temperature_read) + 0.5) : (int)((temperature_read)-0.5));

        temp_as_chars[4] = '\0';
        temp_as_chars[3] = TEMPERATURE_UNITS;
        temp_as_chars[2] = '0' + temperature % 10;
        temp_as_chars[1] = (temperature /= 10) % 10 ? '0' + (temperature) % 10 : (temperature / 10) % 10 ? '0' : ' ';
        temp_as_chars[0] = temperature / 10 ? '0' + temperature / 10 : ' ';
    }
}

void init_temp_sensor(void) {
    adcInit();
    // dummy read for initialization
    adc_read(TO_MUX(3, 0));
}

void render_temp(void) {
    updateTemperatureSample();

    oled_write_ln_P(PSTR("TEMP"), false);
    oled_write(temp_as_chars, false);
    oled_write_P(PSTR(" "), false);
}
