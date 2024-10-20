# LedHex #

This repository contains several programs to run on an Arduino
and drive a display made from LEDs.

The display was made as a small-volume low-cost PCB using a design drawn in KiCad.

## KiCadV3Pcb ##

CAD files for the schematic and PCB,
designed in 2014 using KiCad V3.
This version of KiCad is so old now that it's not practical to update the files.
At the time of writing,
KiCad is at V8.0.5 and uses a completely different set of PCB footprints from V3.
So although it's possible to update the schematic files to use the latest format
and the latest symbols,
it's difficult to map those symbols to the new footprints.
This means that the PCB layout cannot be resynchronised with the schematic.
Probably the easiest way to fix this would be to re-draw in KiCad V8.

The schematic is shown as a PDF for reference when writing software
to drive the display.

## LedHexClock ##

Very simple four-digit clock with flashing colon.
So simple, in fact,
that there's no way to set the time.

TODO: add a way to set the time.

TODO: add 12/24 hour switch.

TODO: add other types of time display, TBD.

## LedHexCount4 ##

Display a four-digit counter on four LedHex PCBs connected in a chain.
Range is 0000-FFFF (0-65535).

## LedHexLampTest4 ##

Test a four-digit chain of LedHex PCBs by lighting up all the segments
in sequence.

## LedHexTest ##

Simply count 0-9, A-F repeatedly on a single digit display.

## LedHexTest4 ##

Test a four-digit chain of LedHex PCBs by lighting up all the segments
in sequence.
This is also a test of the LED power supply!

