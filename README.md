# sf2_m3_bootloader

All credit to Antti Lukats for designing this bootloader
https://github.com/AnttiLukats

This code will configure the SmartFusion2's M3 processor to load data from the eNVM to other memory (such as eSRAM and external LSRAM).

The MSS must be configured as follows:

[ ] Remap eNVM to 0x00000000
[ ] Enable MMUART0

Load these two files into a new SoftConsole project. Ensure that the linker script is used by adding the file to GNU ARM Cross C Linker -> General -> Script files.
