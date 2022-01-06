#include "serial.h"

#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/watchdog.h"

static char serial_buffer[256];

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
}

bool handle_command(char *command) {
    if(strcmp(command, "arm") == 0) {
        multicore_fifo_push_blocking(cmd_arm);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Device armed!\n");
        } else {
            printf("Arming failed!\n");
        }
        return true;
    }
    if(strcmp(command, "disarm") == 0) {
        multicore_fifo_push_blocking(cmd_disarm);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Device disarmed!\n");
        } else {
            printf("Disarming failed!\n");
        }
        return true;
    }
    if(strcmp(command, "pulse") == 0) {
        multicore_fifo_push_blocking(cmd_pulse);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Pulsed!\n");
        } else {
            printf("Pulse failed!\n");
        }
        return true;
    }
    if(strcmp(command, "status") == 0) {
        multicore_fifo_push_blocking(cmd_status);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            print_status(multicore_fifo_pop_blocking());
        } else {
            printf("Getting status failed!\n");
        }
        return true;
    }
    if(strcmp(command, "enable_timeout") == 0) {
        multicore_fifo_push_blocking(cmd_enable_timeout);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Timeout enabled!\n");
        } else {
            printf("Enabling timeout failed!\n");
        }
        return true;
    }
    if(strcmp(command, "disable_timeout") == 0) {
        multicore_fifo_push_blocking(cmd_disable_timeout);
        uint32_t result = multicore_fifo_pop_blocking();
        if(result == return_ok) {
            printf("Timeout disabled!\n");
        } else {
            printf("Disabling timeout failed!\n");
        }
        return true;
    }
    if(strcmp(command, "fast_trigger") == 0) {
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

    if(strcmp(command, "reset") == 0) {
        watchdog_enable(1, 1);
        while(1);
    }
    return false;
}

void serial_console() {
    multicore_fifo_drain();
    while(1) {
        read_line();
        printf("\n");
        if(!handle_command(serial_buffer)) {
            printf("PicoEMP Commands:\n");
            printf("- arm\n");
            printf("- disarm\n");
            printf("- pulse\n");
            printf("- enable_timeout\n");
            printf("- disable_timeout\n");
            printf("- fast_trigger\n");
            printf("- status\n");
            printf("- reset\n");
        }
        printf("\n");
        
        printf("> ");
    }
}