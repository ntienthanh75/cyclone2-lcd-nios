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

## Nios flower demo

The current application is a simple Nios II LCD test that draws a flower: five colored petals, a yellow center, a green stem/leaves, and the text `Nios flower` on a black background. The source file is:

```text
D:\fpga\lcd_nios\software\lcd_nios_5\main.c
```

The file was created as a replacement for the previous test application. It initializes the LCD, enables the backlight, draws the flower with `LCD_SetPoint`, `LCD_DrawLine`, and `GUI_Text`, then stays in a loop so the picture remains visible. Touch calibration and the old number/image demo are not called, because they could block the display test.

The compiled application is:

```text
D:\fpga\lcd_nios\software\lcd_nios_5\lcd_nios_5.elf
```

### Build on Windows

Quartus II 13.0 SP1's Nios makefiles require its bundled Cygwin shell. The following command uses only the installation on `D:` and disables the obsolete stack-report helper, which is incompatible with the installed line endings:

```powershell
& 'D:\Program\altera\13.0sp1\quartus\bin\cygwin\bin\bash.exe' -lc 'export SOPC_KIT_NIOS2=/cygdrive/d/Program/altera/13.0sp1/nios2eds; export QUARTUS_ROOTDIR=/cygdrive/d/Program/altera/13.0sp1/quartus; export PATH="$SOPC_KIT_NIOS2/bin:$SOPC_KIT_NIOS2/sdk2/bin:$SOPC_KIT_NIOS2/bin/gnu/H-i686-mingw32/bin:$QUARTUS_ROOTDIR/bin:$QUARTUS_ROOTDIR/sopc_builder/bin:$PATH"; cd /cygdrive/d/fpga/lcd_nios/software/lcd_nios_5; make -j1 DISABLE_STACKREPORT=1'
```

### Program the board and download the flower

The hardware image used for this test is the existing time-limited Nios/LCD image:

```text
D:\fpga\lcd_nios\lcd_nios_time_limited.sof
```

First verify the USB-Blaster and program the FPGA:

```powershell
& 'D:\Program\altera\13.0sp1\quartus\bin64\jtagconfig.exe'
& 'D:\Program\altera\13.0sp1\quartus\bin64\quartus_pgm.exe' -m JTAG -c 'USB-Blaster [USB-0]' -o 'p;D:\fpga\lcd_nios\lcd_nios_time_limited.sof'
```

Then download and start the ELF through the Nios JTAG debug module. The line-ending conversion is needed for the legacy shell script shipped with Quartus 13.0 SP1:

```powershell
& 'D:\Program\altera\13.0sp1\quartus\bin\cygwin\bin\bash.exe' -lc 'export SOPC_KIT_NIOS2=/cygdrive/d/Program/altera/13.0sp1/nios2eds; export QUARTUS_ROOTDIR=/cygdrive/d/Program/altera/13.0sp1/quartus; export PATH="$SOPC_KIT_NIOS2/bin:$SOPC_KIT_NIOS2/sdk2/bin:$SOPC_KIT_NIOS2/bin/gnu/H-i686-mingw32/bin:$QUARTUS_ROOTDIR/bin:$QUARTUS_ROOTDIR/sopc_builder/bin:$PATH"; cd /cygdrive/d/fpga/lcd_nios/software/lcd_nios_5; tr -d "\r" < "$SOPC_KIT_NIOS2/bin/nios2-download" | bash -s -- -g lcd_nios_5.elf'
```

The successful test used cable `USB-Blaster [USB-0]`, device `EP2C5`, and reported `Downloaded 77KB`, `Verified OK`, and `Starting processor at address 0x040001B4`. The `.sof` image is temporary and must be programmed again after power-off; the ELF is downloaded into the Nios instruction/data memory at runtime.
