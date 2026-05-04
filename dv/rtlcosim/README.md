# RTL Co-simulation

Cycle-by-cycle comparison of original CORE-ET modules against their clean reimplementations.

## How it works

Each co-simulation test instantiates both the original module (from the
original CORE-ET RTL tree) and the new module (from this repo) inside a
single Verilator testbench. Both receive identical stimulus on every cycle.
The C++ test compares all outputs and reports any divergence.

## Prerequisites

Set `ORIG_ROOT` to point to the original CORE-ET RTL tree. The path must
contain `rtl/inc`, `rtl/libs`, and `rtl/shire`.

```bash
export ORIG_ROOT=~/ainekko/etcore-soc
# or
make -C dv/rtlcosim/prim_fifo_sram test ORIG_ROOT=~/ainekko/etcore-soc
```

If your checkout lives elsewhere, adjust `ORIG_ROOT` accordingly. If
`ORIG_ROOT` is not set, `mk/rtlcosim.mk` falls back to its configured relative
original-RTL checkout path.

## Running

```bash
# Run all co-simulations
make -C dv/rtlcosim test

# Run a specific module
make -C dv/rtlcosim/prim_fifo_sram test

# Run with VCD trace for debugging mismatches
make -C dv/rtlcosim/prim_fifo_sram test-trace
```

## Adding a new co-simulation

1. Create `dv/rtlcosim/<module>/`
2. Write a `*_cosim_tb.sv` that instantiates both old and new modules with an adapter for reset polarity and tied-off ASIC ports
3. Write a `*_cosim_test.cc` that drives stimulus and uses `CosimCtrl<>` from `dv/rtlcosim/common/cosim_ctrl.h` with `sim.compare()` to compare outputs
4. Create a `Makefile` following the pattern in existing cosim tests
