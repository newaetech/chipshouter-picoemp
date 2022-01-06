#include "picoemp.h"

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "pico/stdlib.h"

const uint32_t PIN_LED_HV = 6;
const uint32_t PIN_LED_STATUS = 7;
const uint32_t PIN_BTN_PULSE = 11;
const uint32_t PIN_OUT_HVPULSE = 14;
const uint32_t PIN_IN_CHARGED = 18;
const uint32_t PIN_OUT_HVPWM = 20;
const uint32_t PIN_LED_CHARGE_ON = 27;
const uint32_t PIN_BTN_ARM = 28;

static bool pwm_enabled = false;

// Code from https://www.i-programmer.info/programming/hardware/14849-the-pico-in-c-basic-pwm.html?start=2
uint32_t pwm_set_freq_duty(uint slice_num,
       uint chan,uint32_t f, float d)
{
    uint32_t clock = clock_get_hz(clk_sys);
    uint32_t divider16 = clock / f / 4096 + 
                            (clock % (f * 4096) != 0);
    if (divider16 / 16 == 0)
    divider16 = 16;
    uint32_t wrap = clock * 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16/16,
                                        divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, (int)((float)wrap * d));
    return wrap;
}

void picoemp_enable_pwm() {
    if(pwm_enabled) {
        return;
    }

    // Get PWM slice
    uint32_t slice = pwm_gpio_to_slice_num(PIN_OUT_HVPWM);
    gpio_set_function(PIN_OUT_HVPWM, GPIO_FUNC_PWM);
    
    // Set up clock divider
    float target_frequency = 25;
    float divider = clock_get_hz(clk_sys) / target_frequency;
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, divider);
    pwm_config_set_wrap(&config, UINT16_MAX);

    // Init PWM, but don't start it yet
    pwm_init(slice, &config, false);
    pwm_set_chan_level(slice, PWM_CHAN_A, 800);
    pwm_set_freq_duty(slice, PWM_CHAN_A, 2500, 0.0122);
    pwm_set_enabled(slice, true);
    pwm_enabled = true;
}

void picoemp_disable_pwm() {
    pwm_enabled = false;
    gpio_init(PIN_OUT_HVPWM);
    gpio_set_dir(PIN_OUT_HVPWM, GPIO_OUT);
    gpio_put(PIN_OUT_HVPWM, false);
}

void picoemp_pulse() {
    gpio_put(PIN_OUT_HVPULSE, true);
    sleep_us(5);
    gpio_put(PIN_OUT_HVPULSE, false);
    sleep_ms(250);
}

void picoemp_configure_pulse_output() {
    // Configure pulse output
    gpio_init(PIN_OUT_HVPULSE);
    gpio_set_dir(PIN_OUT_HVPULSE, GPIO_OUT);
    // No pullups & pulldowns
    gpio_set_pulls(PIN_OUT_HVPULSE, false, false);
    // Adjust slew-rate & drive strength. 
    gpio_set_drive_strength(PIN_OUT_HVPULSE, GPIO_DRIVE_STRENGTH_12MA);
    gpio_set_slew_rate(PIN_OUT_HVPULSE, GPIO_SLEW_RATE_FAST);
    gpio_put(PIN_OUT_HVPULSE, false);
}

void picoemp_init() {
    // Initialize LED GPIOs
    gpio_init(PIN_LED_HV);
    gpio_init(PIN_LED_STATUS);
    gpio_init(PIN_LED_CHARGE_ON);
    gpio_set_dir(PIN_LED_HV, GPIO_OUT);
    gpio_set_dir(PIN_LED_STATUS, GPIO_OUT);
    gpio_set_dir(PIN_LED_CHARGE_ON, GPIO_OUT);
    // Enable status LED
    gpio_put(PIN_LED_STATUS, true);

    // Initialize button GPIOs
    gpio_init(PIN_BTN_ARM);
    gpio_init(PIN_BTN_PULSE);
    gpio_set_dir(PIN_BTN_ARM, GPIO_IN);
    gpio_set_dir(PIN_BTN_PULSE, GPIO_IN);
    // ARM is active high, PULSE is active low
    // Enable ARM pulldown
    gpio_set_pulls(PIN_BTN_ARM, false, true);
    // Enable PULSE pullup
    gpio_set_pulls(PIN_BTN_PULSE, true, false);
    // Invert PULSE input so we can read ARM & PULSE both as active-high
    gpio_set_inover(PIN_BTN_PULSE, GPIO_OVERRIDE_INVERT);


    // Charge-detection coming from HV side
    gpio_init(PIN_IN_CHARGED);
    gpio_set_dir(PIN_IN_CHARGED, GPIO_IN);
    gpio_set_pulls(PIN_IN_CHARGED, false, false);
    // Charge is active-low, so we invert it to make the code seem
    // more logical
    gpio_set_inover(PIN_IN_CHARGED, GPIO_OVERRIDE_INVERT);

    // Configure pulse output
    picoemp_configure_pulse_output();

    // Configure PWM pin
    picoemp_disable_pwm();
}
