# CSPico Minimal MicroPython Driver Program
# Copyright (C) Colin O'Flynn, 2021
# CC-SA 3.0 License

from machine import Pin, PWM, Signal
import utime

def pwm_off():
    """Turn HV Transformer Off"""
    hvpwm = Pin(20, Pin.OUT)
    hvpwm.low()

def pwm_on():
    """Turn HV Transformer On"""
    hvpwm = PWM(Pin(20))
    # The duty cycle & frequency have been emperically tuned
    # here to maximize the HV charge. This results in around
    # 250V on the HV capacitor. Setting duty cycle higher generally
    # just causes more current flow/waste in the HV transformer.
    hvpwm.freq(2500)
    hvpwm.duty_u16(800) #range 0 to 65535, 800 = 1.22% duty cycle

#hvpwm pin drives the HV transformer
pwm_off()

# Status LEDs:
ledHv = Signal(Pin(6, Pin.OUT)) #HV 'on' LED (based on feedback)
ledArm = Signal(Pin(27, Pin.OUT)) #Arm 'on' LED
ledStatus = Signal(Pin(7, Pin.OUT)) #Simple status LED
ledStatus.on()

# Due to original PCB being single-layer milled board, these buttons
# used different "active" status to simplify the board routing. This
# was left the same for final PCB.
buttonArm = Signal(Pin(28,  Pin.IN, pull=Pin.PULL_DOWN))
buttonPulse = Signal(Pin(11, Pin.IN, pull=Pin.PULL_UP), invert=True)

# The 'charged' input routes to two pins, one of them is an ADC pin.
# Technically could just use the ADC pin as digital input, but again
# for 'technical debt' reasons left as two pins.
charged = Signal(Pin(18,  Pin.IN), invert=True)

# The 'pulseOut' pin drives the gate of the switch via transformer.
pulse_out_pin = 14
pulseOut = Pin(pulse_out_pin, Pin.OUT)

# Originally 'pulseOut' directly drove the transformer so we increased
# slew rate & drive. This wasn't enough so MOSFET was added to design,
# but the high slew & drive is left set. Potentially we could modulate
# the drive signal slightly by adjusting drive strength?
machine.mem32[0x4 + 0x04*pulse_out_pin + 0x4001c000] = 0b1110011
pulseOut.low()

enabled = False
oldButtonArm = False

timeout_start = utime.ticks_ms()

while True:
    # Arm button 'toggles' output on/off
    if buttonArm.value() and oldButtonArm == False:
        oldButtonArm = True

        if enabled == False:
            enabled = True
            ledArm.on()
            timeout_start = utime.ticks_ms()
        else:
            enabled = False
            ledArm.off()
            pwm_off()
            timeout_start = utime.ticks_ms()

    if buttonArm.value() == False:
        oldButtonArm = False

    if buttonPulse.value():
        pulseOut.high()
        utime.sleep_us(5)
        pulseOut.low()

        # Used to sleep HV
        timeout_start = utime.ticks_ms()

        # Force delay between pulses, allowing you to
        # hold down button. If you want one-shot operation
        # add code to check previous value like with 'arm'
        # button. Note HV circuit takes ~2-3 seconds to
        # recover so 2nd and later pulses not as strong.
        utime.sleep_ms(250)

    if charged.value() == False:
        ledHv.off()

        if enabled == True:
            pwm_on()

    else:
        ledHv.on()

    # Turn off HV if no activity after 60 seconds
    if enabled:
        if utime.ticks_diff(utime.ticks_ms(), timeout_start) > 60000:
            enabled = False
            pwm_off()
            ledArm.off()

