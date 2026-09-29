# Timing constraints for the Waveshare/CoreEP2C5 LCD/Nios hardware.
# The board oscillator is 50 MHz on top-level port CLK.
create_clock -name clk_in -period 20.000 [get_ports {CLK}]

# Constrain the generated PLL clocks from the block-design instance.
# c0 is the 50 MHz system clock; c1 is the phase-shifted SDRAM clock.
derive_pll_clocks
derive_clock_uncertainty

# The touch IRQ is an asynchronous external signal. It is sampled by the
# Nios system and is not a synchronous timing endpoint.
set_false_path -from [get_ports {in_port_to_the_touch_irq}]
