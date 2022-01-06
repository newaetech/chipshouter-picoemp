#pragma once

// FIFO commands
#define cmd_arm 0
#define cmd_disarm 1
#define cmd_pulse 2
#define cmd_status 3
#define cmd_enable_timeout 4
#define cmd_disable_timeout 5
#define cmd_fast_trigger 6
#define return_ok 0
#define return_failed 1

void serial_console();
