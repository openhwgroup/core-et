# prim_clk_gate

Xilinx-specific implementation of `prim_clk_gate`.

- Replaces the generic latch-based model with a negedge FF + AND structure that maps cleanly on 7-series and Ultrascale+ fabric.

RTL: `rtl/prim_clk_gate.sv`
