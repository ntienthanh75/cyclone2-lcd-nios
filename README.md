# Cyclone II LCD Nios II Repository

This repository contains one design in the shared Cyclone II FPGA Board project. It targets the `EP2C5T144C8` board and combines the Nios II system with the LCD/touch hardware.

## Target board

- FPGA: Altera/Intel Cyclone II `EP2C5T144C8`
- Package: `T144`
- I/O standard: `3.3-V LVTTL`
- System clock: FPGA pin `17`
- Reset: FPGA pin `88`
- Programming: USB-Blaster through JTAG

The related LED/joystick project documents the shared board connections in the [Cyclone II board specification](https://github.com/ntienthanh75/fpga-cyclone2-5led/blob/main/docs/board-spec.md).

## Main board connections

This repository uses the board SDRAM and LCD/touch interface defined in `lcd_nios.qsf`. The LCD data and control pins are assigned there; do not reuse those pins for joystick or LED signals while this design is running.

The LED outputs remain on pins `8`, `9`, `24`, and `25`. The buzzer is active-low; drive its output high to keep it muted.

## Build and download

Open `lcd_nios.qpf` in Quartus II 13.0 SP1, compile the repository design, and program the generated `.sof` with:

```powershell
& 'D:\Program\altera\13.0sp1\quartus\bin64\jtagconfig.exe'
& 'D:\Program\altera\13.0sp1\quartus\bin64\quartus_pgm.exe' -c 'USB-Blaster [USB-0]' -m JTAG -o 'p;D:\fpga\lcd_nios\output_files\lcd_nios.sof'
```

The `.sof` configuration is temporary and is lost after power-off.

## Related projects

- [Cyclone II 5LED and joystick projects](https://github.com/ntienthanh75/fpga-cyclone2-5led)
- [Adder with timing constraints](https://github.com/ntienthanh75/adders_time_constrains)

This repository runs software on the Nios II core and uses the 3.2-inch touch LCD, four embedded LEDs, and the board buzzer.
