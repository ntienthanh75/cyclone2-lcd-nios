# lcd_nios synthesis experiment

Target: Waveshare/CoreEP2C5, Cyclone II `EP2C5T144C8`.

This is a 50 MHz constrained compile of the LCD/Nios hardware, excluding the separate `lcd_photo` application. Quartus II 13.0 SP1 completed fit, assembly, and TimeQuest with 0 errors.

| Result | Value |
|---|---:|
| Logic elements | 4,282 / 4,608 (93%) |
| Pins | 78 / 89 (88%) |
| Memory bits | 33,792 / 119,808 (28%) |
| Slow setup slack | +5.287 ns |
| Slow hold slack | +0.499 ns |
| Fast setup slack | +8.516 ns |
| Fast hold slack | +0.215 ns |
| Clocks found | 4 |

The experiment uses [`lcd_nios.sdc`](lcd_nios.sdc) with the 50 MHz input clock, derived PLL clocks, and the asynchronous touch IRQ excluded. Reports and the SOF are under [`experiments/50MHz`](experiments/50MHz). External I/O delays remain unspecified, so TimeQuest still labels the design as not fully constrained for board-level I/O.
