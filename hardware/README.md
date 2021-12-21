# PicoEMP Build Details

*This page is a work in progress, hardware source files not yet uploaded*

## BOM

Due to the horribleness that is the 2021 Supply Chain Issues, this "bom explainer" is included and
is split into several parts.
This helps you find alternate parts that you will require because the part we used originally is
not available anywhere.

Part numbers are given in this format (which can be uploaded to e.g., Digi-Key)
```
QUANTITY,PART NUMBER,REF DES
```

Be sure to pick up all parts in the following **4** subsections:

### BOM PART 1: Specific Parts

The following parts are fairly specific and very hard to sub and core to the design:

```
2,ATB322524-0110-T000,T1 T2
1,MM3Z18VB,D7
1,LDA111STR,Q1
1,1551BTRD,SHIELD
1,SC0915,U1
```

NOTES:
* If you can't find ATB322524 basically all is lost. Some distributors just use the `ATB322524` part
number instead of the full one given above, so try the shorter number on Octopart or similar.
* If you can't find MM3Z18VB (D7) try another Zener in ~15V to 20V range. But this diode drastically
affects the drive waveform, so try to avoid subing it.
* The opto LDA111STR is very low-current, and other ones are less likely to work with
the circuit as-is. The feedback isn't "required" per-say so you can omit that circuitry.
* If you can't find `1551BTRD` a 3D printed solution is available, but it will be about
50% less cool-looking.

If you somehow can't get the RPi Pico all is not lost - the pin headers on the board can be driven
with the required signals. If you have another RP2040 board this should be easy, but you could even
drive the signals from an Arduino or similar.

### BOM PART 2: Slightly Generic Parts

The following parts are a little generic, but you try to get the suggested ones if in stock:

```
1,RGT16BM65DTL,Q2
1,TL3301AF160QJ,SW3
1,MURA160T3G,D2
4,SM4005PL-TP,D1 D3 D4 D5
1,KRM55TR72J474MH01K,C3
1,CRMA2010AF20M0FKEF,R1
1,3522300KFT,R3
1,BG306-02-A-2-0400-L-G,J3
1,0732511150,J1
```

KNOWN SUBS:
* D2: US1J-13-F (NOT TESTED YET)
* Q3/Q4: PMV37ENEAR
* J1: 142-0701-801

NOTES:
* Q2 is IGBT, 500V+ rating in TO-252 ('DPAK'). Watch a sub here as gate charge may be different which will affect drive
  waveform. In theory you can use a MOSFET instead too (pinout "should" be compatable).
* Q3/Q4 are logic level N-Channel MOSFETs, I use higher voltage rating ones due to potential for inductive kick.
  Watch the gate turn-on voltage, don't just rely on "logic level". Sub a lower Vds rating FET if required.
  *WARNING*: These MOSFETs seem to be surprisingly hard to source sometimes.
* SW3 is a 'discharge' switch and is optional, it should be 5mm high to provide clearance.
* J3 is optional (used for calibrating charge voltage, not used normally).
* D1 D3 D4 D5 are Schottky diodes. Suggested ones have 600V rating which is only used in case iolation
breakdown occurs. In circuit they will be exposed to 4V max ever. Almost any diode can be sub'd OK.
* C3 is 630V rated, 0.47uF capacitor. Can sub anything in ~0.1uF to 1uF range with same voltage rating & case size
  (watch height! Must be below 5mm to fit shield).
* R1 is 20M-ohm, 2010 size. 1% to 5% is all OK, this part isn't very specific. At least 100mW rating, ideally
  at least 500V rating. Probably will need to change value if you change Q1 (or due to variation of Q1).
* R2 is 300K-ohm, 2510 size. Any value in 120K to 470K is fine. Rated part is 3W which is way overkill, anything 1/4W
  or higher is fine. Dissipation only occurs when you press discharge button.
* J1 is Edge mount SMA. You can sub another connector here or anything else you want (such as screw terminals).

### BOM PART 3: Very Generic Parts

The following parts are not very specific to the design, and you may be better off selecting parts available
at the time. For some of our builds we used existing stock we had that fit the requirements, so there is no
"validated/preferred" part number (unlike Part 1 & Part 2).

```
2,KSC741J LFS,SW1 SW2
2,10R 0603 RESISTOR,R3 R7
1,75R 0805 RESISTOR,R4
5,1K 0603 RESISTOR,R5 R10 R11 R12 R13
1,2K 0603 RESISTOR,R9
1,22K 0603 RESISTOR,R6
2,4.7uF 50V 0805 CERAMIC CAPACITOR,C1 C2
1,100nF 0603 CERAMIC CAPACITOR,C5
2,APT1608SRCPRV,D6 D9
1,APT1608CGCK,D8
```

NOTES:
* SW1 SW2: You can use the same PN as SW3 (the footprint is the same). The suggested switches are kinda
  squishy which I like the feel of more. If you are mounting R-Pi Pico with pins you probably want higher
  switches (like SW3).
* Resistors were 1% in test builds due to our stock, but values not critical. 5% tol is fine.
* C1/C2 were 50V in test builds (50V rating shared with parts from BasicBBI). PicoEMP does not need
  such high rating, but still suggest 25V or higher to avoid biasing voltage reducing effective capacitance.
* C5 was a 50V cap in test builds, but anything > 16V should be fine.
* LEDs: These are just some part numbers we used for one build, but in practice any 0603 red/green LED
  should work.

### BOM PART 4: Other Useful Parts

In practice it's handy having the battery input, which is designed to use the following:

```
1,S2B-XH-A(LF)(SN),J2
1,PRT-09925,BATTERY HOLDER
```

PRT-09925 is a Sparkfun AA battery holder with power switch, terminated in 2.5mm (0.1") JST-XH connector.
This goes right to the R-Pi Pico power input, see the R-Pi Pico documentation for allowed input voltage
ranges.

You will also want some SMA connectors & ferrites to actually use your tool! So pick up for example
at least the following for a minimal-fuss option:
```
1,CONSMA013.062
1,PCV-0-472-03L
```

The SMA connector is to mate with the SMA on the output. You might want to pickup several of them.

### Minimum Build

If you run into supply chain issues, the *absolute minimum parts* needed are:

* Shield (Hammond 1551BTRD)
* T1, T2 (Transformers)
* D2 (High Voltage Rectifier)
* Q2 (High Voltage Switch)
* Q3, Q4 (Low voltage logic-level MOSFET)
* C1, C2 (Caps on HV transformer)
* C3 (Cap on gate drive transformer)
* R3--R7, R9--R13 (various 0603 & 0805 resistors)
* D1, D3--D5 (generic schottky diodes) - they can be replaced with jumpers.
* D7 is *highly recommended* but technically optional, you may immediatly blow up Q2 without it.

This minimum build dumps some features (such as 'charge OK' feedback) and may require more
tuning.

## PCBs

The PCB gerbers can be found in the [gerbers](gerbers) folder. There are two versions:

* Regular Version
* Bantam Mill Version

### Regular Version

The regular gerbers have very little special about them. They should be accepted by your fav
PCB fab (OSHPark, JLCPCB, PCBWay, AllPCB, Advanced Circuits, etc).

The board can be hand-built if you have done a few SMD builds before. The smallest parts are 0603
so there is nothing very crazy. Watch the build order on some parts to make soldering easier:

* Mount J3 (if using) before the nearby SMD parts.
* Mount SW1 & SW2 after mounting R-Pi Pico if SMD mounting the R-Pi Pico.
* Mount Q1 (6-pin optoisolator) after parts around it.

### Bantam Version

The 'bantam mill version' is optimized for milling on a Bantam PCB mill, but could be done with
a classic PCB etch or other PCB mill. It removes most of the throuh-holes from the NC-Drill file
and only includes the copper layers.

This file should work well using both the `0.005" PCB Engraving Bit` and `1/32 end mill` combination.

See the [gerbers/rev04_bantam/README.md](gerbers/rev04_bantam/README.md) file which annotates using this & the required jumper wire!

## Build Process

### PCBA

A assembly drawing is available in the file [BUILD-DRAWING-REV04.PDF](BUILD-DRAWING-REV04.PDF)
which includes the location of parts. Lines indicate the cathods of diodes. The angled ends of
the LEDs indicates their cathods. Some specific notes:

* Watch transformer T1 and T2 have a dot that needs to be matched to the assembly drawing/silkscreen.

* Q1 (6-pin optoisolator) has pin 1 marked on the silkscreen with a line above pin 1 - check this carefully.
  See the photo of assembled boards for a better reference.

* Check D7 cathod carefully - if mounted backwards (or there is an open solder joint on it) you may
  blow up Q2 (the large MOSFET). Depending on your equipment on-hand Q2 may be difficult to replace.

### Shields

The safety shield should "just fit" if the PCB was made correctly with the milled out slots. Use
#2 screws to fit the safety shield - you can cut the ones provided with the Hammond Enclosure in
half with a large pair of side cutters. If your shield doesn't fit, you can use the "other side"
of the Hammond Enclosure (which doesn't have the protrusions) and simply hot glue it to the PCB.

A piece of heat shrink will fit over the connector J1 to ensure only one "pin" is exposed. The
pyramid-like flare on J1 is designed to help the heatshrink fit snugly onto J1.