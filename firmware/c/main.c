#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "picoemp.h"
#include "serial.h"

#include "trigger.pio.h"

static bool armed = false;
static bool timeout_active = true;
static absolute_time_t timeout_time;

void arm() {
    gpio_put(PIN_LED_CHARGE_ON, true);
    armed = true;
}

void disarm() {
    gpio_put(PIN_LED_CHARGE_ON, false);
    armed = false;
    picoemp_disable_pwm();
}

uint32_t get_status() {
    uint32_t result = 0;
    if(armed) {
        result |= 0b1;
    }
    if(gpio_get(PIN_IN_CHARGED)) {
        result |= 0b10;
    }
    if(timeout_active) {
        result |= 0b100;
    }
    return result;
}

void update_timeout() {
    timeout_time = delayed_by_ms(get_absolute_time(), 60 * 1000);
}

void fast_trigger() {
    // Choose which PIO instance to use (there are two instances)
    PIO pio = pio0;

    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember this location!
    uint offset = pio_add_program(pio, &trigger_program);

    // Find a free state machine on our chosen PIO (erroring if there are
    // none). Configure it to run our program, and start it, using the
    // helper function we included in our .pio file.
    uint sm = 0;
    trigger_program_init(pio, sm, offset, 0, PIN_OUT_HVPULSE);
    pio_sm_put_blocking(pio, sm, 3000);
}

int main() {
    // Initialize USB-UART as STDIO
    stdio_init_all();

    picoemp_init();

    // Run serial-console on second core
    multicore_launch_core1(serial_console);

    while(1) {
        gpio_put(PIN_LED_HV, gpio_get(PIN_IN_CHARGED));

        // Handle serial commands (if any)
        while(multicore_fifo_rvalid()) {
            uint32_t command = multicore_fifo_pop_blocking();
            switch(command) {
                case cmd_arm:
                    arm();
                    update_timeout();
                    multicore_fifo_push_blocking(return_ok);
                    break;
                case cmd_disarm:
                    disarm();
                    multicore_fifo_push_blocking(return_ok);
                    break;
                case cmd_pulse:
                    picoemp_pulse();
                    update_timeout();
                    multicore_fifo_push_blocking(return_ok);
                    break;
                case cmd_status:
                    multicore_fifo_push_blocking(return_ok);
                    multicore_fifo_push_blocking(get_status());
                    break;
                case cmd_enable_timeout:
                    timeout_active = true;
                    update_timeout();
                    multicore_fifo_push_blocking(return_ok);
                    break;
                case cmd_disable_timeout:
                    timeout_active = false;
                    multicore_fifo_push_blocking(return_ok);
                    break;
                case cmd_fast_trigger:
                    fast_trigger();
                    multicore_fifo_push_blocking(return_ok);
                    while(!pio_interrupt_get(pio0, 0));
                    multicore_fifo_push_blocking(return_ok);
                    pio_sm_set_enabled(pio0, 0, false);
                    picoemp_configure_pulse_output();
                    break;

            }
        }

        // Pulse
        if(gpio_get(PIN_BTN_PULSE)) {
            update_timeout();
            picoemp_pulse();
        }

        if(gpio_get(PIN_BTN_ARM)) {
            update_timeout();
            if(!armed) {
                arm();
            } else {
                disarm();
            }
            // YOLO debouncing
            while(gpio_get(PIN_BTN_ARM));
            sleep_ms(100);
        }

        if(!gpio_get(PIN_IN_CHARGED) && armed) {
            picoemp_enable_pwm();
        }

        if(timeout_active && (get_absolute_time() > timeout_time) && armed) {
            disarm();
        }
    }
    
    return 0;
}
