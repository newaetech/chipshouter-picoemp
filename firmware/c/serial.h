#pragma once

// FIFO commands
#define cmd_arm 0
#define cmd_disarm 1
#define cmd_pulse 2
#define cmd_status 3
#define cmd_enable_timeout 4
#define cmd_disable_timeout 5
#define cmd_fast_trigger 6
#define cmd_internal_hvp 7
#define cmd_external_hvp 8
#define cmd_config_pulse_time 9
#define cmd_config_pulse_power 10
#define cmd_toggle_gp1 11
#define cmd_config_pulse_delay_cycles 12
#define cmd_config_pulse_time_cycles 13

#define return_ok 0
#define return_failed 1

void serial_console();
