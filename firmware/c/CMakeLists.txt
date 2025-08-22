cmake_minimum_required(VERSION 3.13)
# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)
project(picoemp)
 
pico_sdk_init()

add_executable(picoemp)

pico_generate_pio_header(picoemp ${CMAKE_CURRENT_LIST_DIR}/trigger_basic.pio)


target_sources(picoemp PRIVATE
        main.c
        picoemp.c
        serial.c
        )

target_link_libraries(picoemp PRIVATE pico_stdlib hardware_pwm hardware_pio pico_multicore)
pico_add_extra_outputs(picoemp)

# enable usb output, disable uart output
pico_enable_stdio_usb(picoemp 1)
pico_enable_stdio_uart(picoemp 0)

# Use 200MHz clock for pio, for 5ns precision in fast trigger mode
# RP2040 is certified to run at 200MHz, see https://github.com/raspberrypi/pico-sdk/releases/tag/2.1.1
add_definitions(-DSYS_CLK_MHZ=200)
