# Firmware

This directory includes the firmware loaded onto the Raspberry Pi Pico.

## C

The C firmware in the [c](c) directory was started by @nezza, and is the most robust/future-proof firmware option!
This includes a serial console similar to ChipSHOUTER, and also allows usage of an external
trigger input.

## Micropython

The micropython firmware is a basic Micropython example. See the [micropython](micropython)
directory for that version and instructions on loading.

This firmware allows only manual usage by the button, and does not include a serial console.