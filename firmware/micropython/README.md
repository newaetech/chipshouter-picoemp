# MicroPython Firmware

## Loading Files

Your raw Raspberry Pi Pico will need to be loaded with:

* MicroPython Interpreter
* The selected file version (below), saved as `main.py` on the R-Pi Pico.

You can find various guides to using the Raspberry Pi Pico. The main steps you need to do are:

1. [Install Thonny](https://projects.raspberrypi.org/en/projects/getting-started-with-the-pico/2)
2. [Install MicroPython Firmware](https://projects.raspberrypi.org/en/projects/getting-started-with-the-pico/3)
3. [Open the selected file and save as `main.py` on the R-Pi Pico](https://projects.raspberrypi.org/en/projects/getting-started-with-the-pico/9)

## Versions

Currently there are the following versions:

* `cspico_simple.py`: Most basic driver example

## `cspico_simple.py`

This version includes only the most basic required functionality. To use this version:

1. Apply power (via MicroUSB or power input).
2. Press 'Arm' switch.
3. Wait for HV light to come on.
4. Press 'pulse' switch.

The default file turns off the high voltage after some time to save battery power. You can comment out this section if you
don't want that:

```
    # Turn off HV if no activity after 60 seconds
    if enabled:
        if utime.ticks_diff(utime.ticks_ms(), timeout_start) > 60000:
            enabled = False
            pwm_off()
            ledArm.off()
```

The default code keeps charging past the 'charged' pin going high. You can improve power consumption by toggling the charging
on/off, but this hurts the already-slow recovery time from pulses being inserted.