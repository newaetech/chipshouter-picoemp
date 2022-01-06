# PicoEMP C firmware

This is a basic firmware for Colin O'Flynn's [PicoEMP](https://picoemp.com/).

It currently supports:
- Manual arming + pulsing
- Serial interface for arming/disarming/pulsing
- Automatic disarm after 60 seconds
- Fast-trigger via GPIO0 (uses PIO for very fast and consistent triggering)

## Building

```
export PICO_SDK_PATH=.. path to pico SDK ..
mkdir build
cd build
cmake ..
make
```
