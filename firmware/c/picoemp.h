#pragma once

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

extern const uint32_t PIN_LED_HV;
extern const uint32_t PIN_LED_STATUS;
extern const uint32_t PIN_BTN_PULSE;
extern const uint32_t PIN_OUT_HVPULSE;
extern const uint32_t PIN_IN_CHARGED;
extern const uint32_t PIN_OUT_HVPWM;
extern const uint32_t PIN_LED_CHARGE_ON;
extern const uint32_t PIN_BTN_ARM;

void picoemp_enable_pwm(float duty_frac);
void picoemp_disable_pwm();
void picoemp_pulse(uint32_t pulse_time);
void picoemp_configure_pulse_output();
void picoemp_configure_pulse_external();
void picoemp_init();
