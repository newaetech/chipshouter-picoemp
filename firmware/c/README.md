# PicoEMP C firmware

This is a basic firmware for Colin O'Flynn's [PicoEMP](https://picoemp.com/).

It currently supports:
- Manual arming + pulsing
- Serial interface for arming/disarming/pulsing
- Automatic disarm after 60 seconds
- Fast-trigger via GPIO0 (uses PIO for very fast and consistent triggering)
- External HVP mode: use an external pulse generator (e.g. ChipWhisperer) to control EM pulse insertion

## Serial command documentation:
```
* [h]elp → displays list of commands
* [a]rm → arms device  
* [d]isarm → disarms device
* [p]ulse → sends out single, relatively long pulse of length configured using "c" command (no delay parameter)
* [en]able_timeout → disarms device after certain timeout period
* [di]sable_timeout → disables disarming timeout, i.e. device stays armed unless disarmed manually
* [f]ast_trigger → device triggers when getting a signal on GPIO0, emits pulse of configured length after configured delay
* [fa]st_trigger_configure → sets configuration parameters (# of delay cycles and pulse cycles) for fast_trigger
* [in]ternal_hvp → sets hvp_internal=true
* [ex]ternal_hvp → sets hvp_internal=false
* [c]onfigure → sets pulse power and pulse time (for manual pulse command)
* [t]oggle_gp1 → toggles GPIO pin 1 (not sure what for)
* [s]tatus → print device status
* [r]eset → reboot & reset all settings
```

## Building

```
export PICO_SDK_PATH=.. path to pico SDK ..
mkdir build
cd build
cmake ..
make
```
