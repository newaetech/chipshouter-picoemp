# ChipSHOUTER-PicoEMP

[![CC BY-SA 3.0][cc-by-sa-shield]][cc-by-sa]

![](hardware/picoemp.jpeg)

The PicoEMP is a low-cost Electromagnetic Fault Injection (EMFI) tool, designed *specifically* for self-study and hobbiest research.

## Background

The [ChipSHOUTER](http://www.chipshouter.com) is a high-end Electromagnetic Fault Injection (EMFI) tool designed by Colin
at [NewAE Technology](http://www.newae.com). While not the first commercially available EMFI tool, ChipSHOUTER was the first
"easily purchasable" (even if expensive) tool with extensive open documentation. The tool was *not* open-source, but it
did contain a variety of detailed description of the design and architecture in the
[User Manual](https://github.com/newaetech/ChipSHOUTER/tree/master/documentation). The ChipSHOUTER design optimization focused in rough order on (1) safe operation, (2) high performance, (3) usability, and finally (4) cost. This results in a tool that covers many use-cases, but may be overkill (and too costly) for many. In additional, acquiring the safety testing/certification is not cheap, and must be accounted for in the product sale price.

The PicoEMP tries to fill in the gap that ChipSHOUTER leaves at the lower end of the spectrum. This PicoEMP project is *not* the
ChipSHOUTER. Instead it's designed to present a "bare bones" tool that has a design optimization focused in rough order of (1) safe
operation, (2) cost, (3) usability, (4) performance. Despite the focus on safety and low-cost, it works *suprisingly* well. It is also
*not* sold as a complete product - you are responsible for building it, ensuring it meets any relevant safety requirements/certifications,
and we completely disclaim all liability for what happens next. Please **only** use PicoEMP where you are building and controlling it
yourself, with total understanding of the operation and risks. It is *not* designed to be used in professional or educational environments,
where tools are expected to meet safety certifications (ChipSHOUTER was designed for these use-cases).

## Building a PicoEMP

The PicoEMP uses a Raspberry Pi Pico as the controller. You could alternatively use an Arduino or another microcontroller. You basically just need a few things:

1. PWM output to drive HV transformer.
2. Pulse pin to generate a pulse.
3. Status pin to monitor the HV status.

You have two options for building the PicoEMP: (1) total scratch build, or (2) easy-assemble build.

### Scratch Build

The PCB is *mostly* one layer. Original versions of it were milled on a Bantam PCB mill, and the final 'production' version is designed
to still allow this simple milling process. You can find details in the [gerbers](hardware/gerbers) folder, including Bantam-optimized files
which remove some of the smaller vias (used for the mounting holes), and require you to surface-mount the Raspberry Pi Pico. Here was
'rev3' of the PCB with a few hacked up tests:

![](hardware/design_notes/img/proto_rev3_hackedup.jpeg)

If you've got time you can order the "real" PCBs from the [gerbers](hardware/gerbers) as well.

The BOM and build details are described in the [hardware](hardware) folder. If you cannot find the plastic shield (the upper half of Hammond
1551BTRD is used), you can find a simple 3D-printable shield as well. The official shield is low-cost and available from Digikey/Mouser/
Newark so you can purchase alongside everything else you need.

**IMPORTANT**: The plastic shield is critical for safe operation. While the output itself is isolated from the input connections, you will still **easily shock yourself** on the exposed high-voltage capacitor and circuitry. **NEVER** operate the device without the shield.

### Easy-Assemble Build

The Easy-Assembly build uses a "mostly complete" SMD board, which you need to solder a Raspberry Pi Pico, switches, and through-hole headers.

### Programming the PicoEMP

You'll need to program the PicoEMP with the firmware in the [firmware](firmware) directory. You can run other tasks on the microcontroller
as well.

### Building the EM Injection Tip (Probe / Coil)

You will also need an "injection tip", typically made with a ferrite core and some wires wrapped around it. You can see examples of such cores in the ChipSHOUTER kit.

An easy way to make them is with a SMA connector TODO.

See the [injection_tips](hardware/injection_tips) folder for more examples, including user-submitted examples.

*Reader Note: Please submit your own examples with a pull-request to this repo!*

You can find additional examples of homemade cores in research papers such as:

* A. Cui, R. Housley, "BADFET: Defeating Modern Secure Boot Using Second-Order Pulsed Electromagnetic Fault Injection," USENIX Workshop on Offensive Technologies (WOOT 17), 2017.  [Paper Link.](https://www.usenix.org/conference/woot17/workshop-program/presentation/cui) [Slides Link.](https://github.com/RedBalloonShenanigans/BADFET)
* J. Balasch, D. Arumí and S. Manich, "Design and validation of a platform for electromagnetic fault injection," 2017 32nd Conference on Design of Circuits and Integrated Systems (DCIS), 2017, pp. 1-6. [Paper Link.](https://upcommons.upc.edu/bitstream/handle/2117/116688/bare_conf.pdf)
* J. Toulemont, G. Chancel, J. M. Galliere, F. Mailly, P. Nouet and P. Maurine, "On the scaling of EMFI probes," 2021 Workshop on Fault Detection and Tolerance in Cryptography (FDTC), 2021. [Paper Link.](https://ieeexplore.ieee.org/abstract/document/9565575) [Slides Link.](https://jaif.io/2021/media/JAIF2021%20-%20Toulemont.pdf)
* LimitedResults. "Enter the Gecko," 2021. [Blog Link](https://limitedresults.com/2021/06/enter-the-efm32-gecko/)

## Using the PicoEMP

The general usage of the PicoEMP is as follows:

1. Press the "ARM" button. The red "ARMING" led will come on instantly telling you it's trying to charge the high voltage.
2. The red "HV" led will come on after a few seconds saying it is charged to "some voltage".
3. Place the probe tip overtop of the target.
4. Press the "Pulse" button.

You can see more examples of this in the video (TODO).

**WARNING**: The high voltage will be applied across the SMA connector. If an injection tip (coil) is present, it will absorb most of the power. If you leave the SMA connector open, you will present a high voltage pulse across this SMA and could shock yourself. Do NOT touch the output SMA tip as a general "best practice", and treat the output as if it has a high voltage present.

The full ChipSHOUTER detects the missing connector tip and refuses to power up the high voltage, the PicoEMP does not have this failsafe!

## About the High Voltage Isolation

Most EMFI tools generate high voltages (similar to a camera flash). Many previous designs of open-source EMFI tools would work well, but [exposed the user to high voltages](https://github.com/RedBalloonShenanigans/BADFET). This was fine provided you use the tool correctly, but of course there is always a risk of grabbing the electrically "hot" tool! This common design choice happens because the easiest way to design an EMFI tool is with "low-side switching" (there is a very short mention of these design choices as well in my [book](https://www.nostarch.com/hardwarehacking) if you are curious). With low-side switching the output connector is always "hot", which presents a serious shock hazard.

PicoEMP gets around this problem by floating the high-voltage side, meaning there is no electrical path between the EMFI probe output and the input voltage ground. With the isolated high voltage output we can use the simple "low-side switching" in a safe manner. Some current will still flow due to the high-frequency spikes, so this isn't *perfect*, but it works well enough in practice (well enough you will shock yourself less often).

The caveat here is for this to work you also need to isolate your gate drive. There are a variety of [solutions to this](https://www.analog.com/en/technical-articles/powering-the-isolated-side-of-your-half-bridge-configuration.html), with the simplist being a gate drive transformer (GDT). The PicoEMP uses the transformer architecture, with some simplifications to further reduce BOM count.

More details of the design are available in the [hardware](hardware) folder.

### Hipot Testing for Validating Isolation

Easy-assemble builds have been subject to a hipot test. This test validates the isolation exists, and has not been compromised by things like leftover flux on the PCB.

This test applies a high voltage (1000V) from the SMA connector pads to the low-voltage signals shorted together. The test is done both with AC and DC, with test passing if LESS than 1 uA of current flows at DC, and 100uA at AC, with the test voltage held for 60 seconds for both tests. Note these limits are *far* lower than most industry standard limits (which would fail at 500 to 3000uA).

### Technical Differences between ChipSHOUTER and PicoEMP

The main differences from a technical standpoint:

* ChipSHOUTER uses a much more powerful high voltage circuit and transformer (up to ~30W vs ~0.2W) that gives it
  almost unlimited glitch delivery, typically limited by your probe tip. The PicoEMP is slower to recover, typically ~1 to 4 seconds between
  glitches.

* ChipSHOUTER has a larger internal energy storage & more powerful output drivers.

* ChipSHOUTER has a controlled high-voltage setting from 150V to 500V. PicoEMP generates ~250V, there is some feedback but it's uncalibrated.
  **NOTE**: The PicoEMP allows some control of output pulse size by instead controlling the drive signal. This is less reliable (more variability
  in the output), but meets the goal of using the lowest-cost control method.

## License

This work is licensed under a [Creative Commons Attribution-ShareAlike 3.0 International License][cc-by-sa].

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/3.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/3.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%203.0-lightgrey.svg
