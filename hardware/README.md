# PicoEMP Build Details

*This page is a work in progress*

## BOM

## PCBs

The PCB gerbers can be found in the [gerbers](gerbers) folder. There are two versions:

* Regular Version
* Bantam Mill Version

### Regular Version

The regular gerbers have very little special about them. They should be accepted by your fav
PCB fab (OSHPark, JLCPCB, PCBWay, AllPCB, Advanced Circuits, etc).

The board can be hand-built if you have done a few SMD builds before. The smallest parts are 0603
so there is nothing very crazy. Watch the build order on some parts.

### Bantam Version

The 'bantam mill version' is optimized for milling on a Bantam PCB mill, but could be done with
a classic PCB etch or other PCB mill. It removes most of the throuh-holes from the NC-Drill file
and only includes the copper layers.

This file should work well using the `0.005" PCB Engraving Bit` and `1/32 end mill`.

See the file [bantam_jumpers.png](gerbers/rev04_img/bantam_jumpers.png) which annotates a required jumper.

## Build Process

### PCBA

A assembly drawing is available in the file [BUILD-DRAWING-REV04.PDF](BUILD-DRAWING-REV04.PDF)
which includes the location of parts. Lines indicate the cathods of dioeds. The angled ends of
the LEDs indicates their cathods.

Watch transformer T1 and T2 have a dot that needs to be matched to the assembly drawing/silkscreen.

Q1 has pin 1 marked on the silkscreen with a line above pin 1 - check this carefully. See the photo
of assembled boards for a better reference.

### Shields

The safety shield should "just fit" if the PCB was made correctly with the milled out slots. Use
#2 screws to fit the safety shield - you can cut the ones provided with the Hammond Enclosure in
half with a large pair of side cutters.

A piece of heat shrink will fit over the connector J1 to ensure only one "pin" is exposed. The
pyramid-like flare on J1 is designed to help the heatshrink fit snugly onto J1.