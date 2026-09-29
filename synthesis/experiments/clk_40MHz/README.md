# Lcd_touch synthesis

This folder contains the dedicated Quartus hardware project and the synthesis/timing review for the LCD touch experiment.

Target: Waveshare/CoreEP2C5, Cyclone II `EP2C5T144C8`.

The original design fitted successfully, but its TimeQuest report had an unconstrained top-level `CLK`, missing PLL generated clocks, and many unconstrained I/O paths. This copy adds `lcd_nios.sdc` with:

- a 50 MHz (`20 ns`) clock on `CLK`;
- `derive_pll_clocks` for the two PLL outputs;
- `derive_clock_uncertainty`;
- a false path for asynchronous `TP_IRQ`.

The external LCD, touch SPI, SDRAM, and backlight ports are intentionally not assigned guessed input/output delays. Their real board timing should be added from the peripheral datasheets before calling timing fully closed.

## Constrained compile result

The compile completed successfully on 29 September 2026 with Quartus II 13.0 SP1:

- Fitter: successful, `4,282 / 4,608` logic elements (93%).
- Pins: `78 / 89` (88%); memory bits 28%; one of two PLLs; four of 26 DSP elements.
- TimeQuest found four clocks: JTAG TCK, the 50 MHz input clock, and both PLL-generated clocks.
- Slow-model worst-case slack: setup `5.287 ns`, hold `0.499 ns`, recovery `6.069 ns`, removal `3.348 ns`.
- Fast-model worst-case slack: setup `8.516 ns`, hold `0.215 ns`, recovery `8.247 ns`, removal `1.497 ns`.
- Total negative slack: `0.000 ns` in the reported setup/hold/recovery/removal analyses.
- Unconstrained clocks: `0`.

This resolves the original missing-clock and missing-PLL warnings. TimeQuest still reports the design as not fully constrained because external I/O delays are not specified: 33 input ports and 73 output ports remain unconstrained. That part cannot be resolved honestly without the LCD, touch, SDRAM, and board interconnect timing requirements. The compile also reports the expected Cyclone II limitations: no jitter analysis support, no exact location for three unused/auxiliary pins, and no output-load assignments for external pins.

## Comparison with the LCD flower case

The flower application is `D:\fpga\lcd_nios\software\lcd_nios_5\main.c`. It uses the same 173,412-byte SOF, the same `SOPC.sopcinfo`, and the same `EP2C5T144C8` hardware. Consequently, its FPGA synthesis result is the same; only the Nios software changes.

| Item | Flower | Lcd_touch |
|---|---|---|
| Main behavior | Draws a flower on the LCD | Reads touch IRQ/SPI and sends JTAG-UART events |
| Nios ELF size | 1,013,746 bytes | 950,862 bytes |
| FPGA resource result | Same shared hardware | Same shared hardware |
| Timing constraints | Original report had 1 unconstrained clock | Dedicated copy now has 0 unconstrained clocks |

The corrected constrained project is a validation build under this folder. The existing time-limited SOF remains the previously fitted image used by the running experiment; the newly generated constrained SOF is not programmed automatically.

## Clock-sweep summary

The following table summarizes the synthesis experiments in [`experiments\clock_sweep.md`](experiments/clock_sweep.md). The board design uses the 50 MHz row; the other rows are what-if timing constraints for comparison.

| Clock constraint | Fit | Logic usage | Slow setup slack | Slow hold slack | Timing verdict |
|---:|:---:|---:|---:|---:|:---|
| 40 MHz | PASS | 4,273/4,608 (93%) | +8.046 ns | +0.499 ns | Pass |
| 50 MHz | PASS | 4,282/4,608 (93%) | +5.287 ns | +0.499 ns | Pass; board default |
| 60 MHz | PASS | 4,277/4,608 (93%) | +2.887 ns | +0.499 ns | Pass |
| 75 MHz | PASS | 4,296/4,608 (93%) | +1.618 ns | +0.499 ns | Pass, small margin |
| 100 MHz | PASS fit | 4,295/4,608 (93%) | -0.999 ns | +0.499 ns | Timing fail |

The 100 MHz build fits the FPGA but fails setup timing, so it must not be used as a programming image. The 50 MHz constrained build is the recommended result for the board.

The multi-clock statistic is in [`experiments\clock_sweep.md`](experiments/clock_sweep.md).

## Build

Run from a Quartus II 13.0 SP1 command shell:

```powershell
Set-Location D:\fpga\Lcd_touch\synthesis
& 'D:\Program\altera\13.0sp1\quartus\bin64\quartus_sh.exe' --flow compile lcd_nios
```

Reports are generated beside this README and under `output_files`.
