create_clock -name clk_in -period 25.000 [get_ports {CLK}]
derive_pll_clocks
derive_clock_uncertainty
set_false_path -from [get_ports {in_port_to_the_touch_irq}]
