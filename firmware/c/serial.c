#include "serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/watchdog.h"

static char serial_buffer[256];
static char last_command[256];

#define PULSE_TIME_DEFAULT 5
#define PULSE_POWER_DEFAULT 0.0122
static uint32_t pulse_time;
static union float_union {float f; uint32_t ui32;} pulse_power;

void read_line() {
    memset(serial_buffer, 0, sizeof(serial_buffer));
    while(1) {
        int c = getchar();
        if(c == EOF) {
            return;
        }

        putchar(c);

        if(c == '\r') {
            return;
        }
        if(c == '\n') {
            continue;
        }

        // buffer full, just return.
        if(strlen(serial_buffer) >= 255) {
            return;
        }

        serial_buffer[strlen(serial_buffer)] = (char)c;
    }
}

void print_status(uint32_t status) {
    bool armed = (status >> 0) & 1;
    bool charged = (status >> 1) & 1;
    bool timeout_active = (status >> 2) & 1;
    bool hvp_mode = (status >> 3) & 1;
    printf("Status:\n");
    if(armed) {
        printf("- Armed\n");
    } else {
        printf("- Disarmed\n");
    }
    if(charged) {
        printf("- Charged\n");
    } else {
        printf("- Not charged\n");
    }
    if(timeout_active) {
        printf("- Timeout active\n");
    } else {
        printf("- Timeout disabled\n");
    }
    if(hvp_mode) {
        printf("- HVP internal\n");
    } else {
        printf("- HVP external\n");
    }
}

bool handle_command(char *command) {
    if (command[0] == 0 && last_command[0] != 0) {
        printf("Repeat previous command (%s)\n", last_command);
        return handle_command(last_command);
    } else {
        strcpy(last_command, command);
    }

    if(strcmp(command, "h") == 0 || strcmp(command, "help") == 0)
        return false;

    if(strcmp(command, "a") == 0 || strcmp(command, "arm") == 0) {
        multicore_fifo_push_blocking(cmd_arm);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Device armed!\n");
        } else {
            printf("Arming failed!\n");
        }
        return true;
    }
    if(strcmp(command, "d") == 0 || strcmp(command, "disarm") == 0) {
        multicore_fifo_push_blocking(cmd_disarm);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Device disarmed!\n");
        } else {
            printf("Disarming failed!\n");
        }
        return true;
    }
    if(strcmp(command, "p") == 0 || strcmp(command, "pulse") == 0) {
        multicore_fifo_push_blocking(cmd_pulse);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Pulsed!\n");
        } else {
            printf("Pulse failed!\n");
        }
        return true;
    }
    if(strcmp(command, "s") == 0 || strcmp(command, "status") == 0) {
        multicore_fifo_push_blocking(cmd_status);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            print_status(multicore_fifo_pop_blocking());
        } else {
            printf("Getting status failed!\n");
        }
        return true;
    }
    if(strcmp(command, "en") == 0 || strcmp(command, "enable_timeout") == 0) {
        multicore_fifo_push_blocking(cmd_enable_timeout);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Timeout enabled!\n");
        } else {
            printf("Enabling timeout failed!\n");
        }
        return true;
    }
    if(strcmp(command, "di") == 0 || strcmp(command, "disable_timeout") == 0) {
        multicore_fifo_push_blocking(cmd_disable_timeout);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Timeout disabled!\n");
        } else {
            printf("Disabling timeout failed!\n");
        }
        return true;
    }
    if(strcmp(command, "f") == 0 || strcmp(command, "fast_trigger") == 0) {
        multicore_fifo_push_blocking(cmd_fast_trigger);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Fast trigger active...\n");
            multicore_fifo_pop_blocking();
            printf("Triggered!\n");
        } else {
            printf("Setting up fast trigger failed.");
        }
        return true;
    }
    if(strcmp(command, "in") == 0 || strcmp(command, "internal_hvp") == 0) {
        multicore_fifo_push_blocking(cmd_internal_hvp);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Internal HVP mode active!\n");
        } else {
            printf("Setting up internal HVP mode failed.");
        }
        return true;
    }
    if(strcmp(command, "ex") == 0 || strcmp(command, "external_hvp") == 0) {
        multicore_fifo_push_blocking(cmd_external_hvp);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("External HVP mode active!\n");
        } else {
            printf("Setting up external HVP mode failed.");
        }
        return true;
    }

    if(strcmp(command, "c") == 0 || strcmp(command, "configure") == 0) {
        char **unused;
        printf(" pulse_time (current: %d, default: %d)?\n> ", pulse_time, PULSE_TIME_DEFAULT);
        read_line();
        printf("\n");
        if (serial_buffer[0] == 0)
            printf("Using default\n");
        else
            pulse_time = strtoul(serial_buffer, unused, 10);

        printf(" pulse_power (current: %f, default: %f)?\n> ", pulse_power, PULSE_POWER_DEFAULT);
        read_line();
        printf("\n");
        if (serial_buffer[0] == 0)
            printf("Using default");
        else
            pulse_power.f = strtof(serial_buffer, unused);

        multicore_fifo_push_blocking(cmd_config_pulse_time);
        multicore_fifo_push_blocking(pulse_time);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result != return_ok) {
            printf("Config pulse_time failed.");
        }

        multicore_fifo_push_blocking(cmd_config_pulse_power);
        multicore_fifo_push_blocking(pulse_power.ui32);
        result = multicore_fifo_pop_blocking();
        if(result != return_ok) {
            printf("Config pulse_power failed.");
        }

        printf("pulse_time=%d, pulse_power=%f\n", pulse_time, pulse_power.f);

        return true;
    }

    if(strcmp(command, "t") == 0 || strcmp(command, "toggle_gp1") == 0) {
        multicore_fifo_push_blocking(cmd_toggle_gp1);
        
        uint32_t result = multicore_fifo_pop_blocking();
        if(result != return_ok) {
            printf("target_reset failed.");
        }

        return true;
    }

    if(strcmp(command, "r") == 0 || strcmp(command, "reset") == 0) {
        watchdog_enable(1, 1);
        while(1);
    }

    return false;
}

void serial_console() {
    multicore_fifo_drain();

    memset(last_command, 0, sizeof(last_command));

    pulse_time = PULSE_TIME_DEFAULT;
    pulse_power.f = PULSE_POWER_DEFAULT;
    
    while(1) {
        read_line();
        printf("\n");
        if(!handle_command(serial_buffer)) {
            printf("PicoEMP Commands:\n");
            printf("- <empty to repeat last command>\n");
            printf("- [h]elp\n");
            printf("- [a]rm\n");
            printf("- [d]isarm\n");
            printf("- [p]ulse\n");
            printf("- [en]able_timeout\n");
            printf("- [di]sable_timeout\n");
            printf("- [f]ast_trigger\n");
            printf("- [in]ternal_hvp\n");
            printf("- [ex]ternal_hvp\n");
            printf("- [c]onfigure: pulse_time=%d, pulse_power=%f\n", pulse_time, pulse_power.f);
            printf("- [t]oggle_gp1\n");
            printf("- [s]tatus\n");
            printf("- [r]eset\n");
        }
        printf("\n");
        
        if (last_command[0] != 0) {
            printf("[%s] > ", last_command);
        } else {
            printf(" > ");
        }
    }
}