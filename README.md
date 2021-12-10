# ChipSHOUTER-PicoEMP

[![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

The PicoEMP is a low-cost Electromagnetic Fault Injection (EMFI) tool, designed *specifically* for self-study and hobbiest research.

## Background

The [ChipSHOUTER](www.chipshouter.com) is a high-end Electromagnetic Fault Injection (EMFI) tool designed by Colin at [NewAE Technology](www.newae.com). While not the first
commercially available EMFI tool, ChipSHOUTER was the first "openly available" tool with extension open documentation. The tool was *not* open-source, but it
did contain a variety of detailed description of the design and architecture in the [User Manual](https://github.com/newaetech/ChipSHOUTER/tree/master/documentation). The ChipSHOUTER design optimization focused in rough order on (1) safe operation, (2) high performance, (3) usability, and finally (4) cost. This results in a tool that covers many use-cases, but may be overkill (and too costly) for a subset of those use-cases. In additional, acquiring the safety testing/certification is not cheap, and must be accounted for in the product sale price.

The PicoEMP tries to fill in the gap that ChipSHOUTER leaves, where cost is a critical aspect. This PicoEMP project is *not* the ChipSHOUTER. Instead it's designed to present a "bare bones" tool that has a design optimization focused in rough order of (1) safe operation, (2) cost, (3) usability, (4) performance. Despite the focus on safety and low-cost, it works *suprisingly* well. It is also *not* sold as a complete product - you are responsible for building it, ensuring it meets any relevant safety requirements/certifications, and we completely disclaim all liability for what happens next. Please **only** use PicoEMP where you are building and controlling it yourself, with total understanding of the operation and risks. It is *not* designed to be used in professional or educational environments, where tools are expected to meet safety certifications (ChipSHOUTER was designed for these use-cases).

## Building a PicoEMP

The PicoEMP uses a Raspberry Pi Pico as the controller. You could alternatively use an Arduino or another microcontroller. You basically just need a few things:

1. PWM output to drive HV transformer.
2. Pulse pin to generate a pulse.
3. Status pin to monitor the HV status.

You have two options for building the PicoEMP: (1) total scratch build, or (2) easy-assemble build.

### Scratch Build

The PCB is *mostly* one layer. Original versions of it were milled on a Bantam PCB mill, and the final 'production' version is designed to still allow this simple milling process.
You can find details in the [gerbers](hardware/gerbers) folder, including Bantam-optimized files which remove some of the smaller vias (used for the mounting holes).

If you've got time you can order the "real" PCBs from the [gerbers](hardware/gerbers) as well.

The BOM and build details are described in the [hardware](hardware) folder. If you cannot find the plastic shield (the upper half of Hammond 1551BTRD is used), you can find a simple 3D-printable shield as well.

**IMPORTANT**: The plastic shield is critical for safe operation. While the output itself is isolated from the input connections, you will still **easily shock yourself** on the exposed high-voltage capacitor and circuitry. **NEVER** operate the device without the shield.

### Easy-Assemble Build

The Easy-Assembly build uses a mostly complete 

### Building the EM Injection Tip (Probe / Coil)

You will also need an "injection tip", typically made with a ferrite core and some wires wrapped around it. You can see examples of such cores in the ChipSHOUTER kit.

An easy way to make them is with a SMA connector TODO.

See the [injection_tips](hardware/injection_tips) folder for more examples, including user-submitted examples.

*Reader Note: Please submit your own examples with a pull-request to this repo!*

You can find additional examples of homemade cores in:

* TODO PAPER1
* TODO PAPER2
* TODO PAPER3

## Using the PicoEMP

TODO

**WARNING**: The high voltage will be applied across the SMA connector. If an injection tip (coil) is present, it will absorb most of the power. If you leave the SMA connector open, you will present a high voltage pulse across this SMA and could shock yourself. Do NOT touch the output SMA tip as a general "best practice", and treat the output as if it has a high voltage present.

## About the High Voltage Isolation

Most EMFI tools generate high voltages (similar to a camera flash). Many previous designs of open-source EMFI tools would work well, but [exposed the user to high voltages](https://github.com/RedBalloonShenanigans/BADFET). This was fine provided you use the tool correctly, but of course there is always a risk of grabbing the electrically "hot" tool! This common design choice happens because the easiest way to design an EMFI tool is with "low-side switching" (there is a very short mention of these design choices as well in my [book](https://www.nostarch.com/hardwarehacking) if you are curious). With low-side switching the output connector is always "hot", which presents a serious shock hazard.

PicoEMP gets around this problem by floating the high-voltage side, meaning there is no electrical path between the EMFI probe output and the input voltage ground. With the isolated high voltage output we can use the simple "low-side switching" in a safe manner. Some current will still flow due to the high-frequency spikes, so this isn't *perfect*, but it works well enough in practice (well enough you will shock yourself less often).

The caveat here is for this to work you also need to isolate your gate drive. There are a variety of [solutions to this](https://www.analog.com/en/technical-articles/powering-the-isolated-side-of-your-half-bridge-configuration.html), with the simplist being a gate drive transformer (GDT). The PicoEMP uses the transformer architecture, with some simplifications to further reduce BOM count.

More details of the design are available in the [hardware](hardware) folder.

### Hipot Testing 

Easy-assemble builds have been subject to a hipot test. This test validates the isolation exists, and has not been compromised by things like leftover flux on the PCB.

This test applies a high voltage (XXXV in this case) from the SMA connector to the USB ground. The test passes if less than XX mA of current flows, with the voltage held for XX seconds.

## License

This work is licensed under a [Creative Commons Attribution-ShareAlike 3.0 International License][cc-by-sa].

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/3.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/3.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%203.0-lightgrey.svg
