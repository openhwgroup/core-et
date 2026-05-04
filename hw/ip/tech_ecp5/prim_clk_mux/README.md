# prim_clk_mux

ECP5-specific implementation of `prim_clk_mux`.

Why this exists
- Clock muxing is a real ECP5 technology seam.
- A behavioral mux is the generic contract reference, but not the right FPGA
  clock-path implementation.

How this implementation realizes the contract
- When `UseTechClockMux=1`, instantiates the ECP5 `DCSC` dynamic clock select
  primitive.
- Drives `SEL0 = ~sel_i` and `SEL1 = sel_i`, so `sel_i=0` selects `clk0_i`
  and `sel_i=1` selects `clk1_i`.
- Uses `MODESEL=0` and `DCSMODE="POS"` for glitchless rising-edge switching.
- When `UseTechClockMux=0` (the default), forwards `clk0_i` without consuming a
  `DCSC`. This mode is only for tied-off FPGA DFT paths that never select
  `clk1_i`.

Integration constraints
- Both source clocks must be oscillating while switching in glitchless mode.
- `DCSC` is a scarce ECP5 clocking resource. Projects that instantiate many
  live clock muxes must budget these cells explicitly.

Implications
- `UseTechClockMux=1` matches the generic primitive contract on real clock
  routing rather than forwarding one input or using a fabric clock mux.
- The ULX3S `test_clk_prims` project opts into `UseTechClockMux=1` so it
  exercises the real `DCSC` path.

RTL: `rtl/prim_clk_mux.sv`
