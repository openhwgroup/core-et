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

## Checked-in coverage updates

CI uses checked-in compressed LCOV snapshots under `dv/rtlcosim/coverage/*.info.gz`.
The top-level Makefile decompresses them into raw `.info` files under
`build/coverage/` for reports and dashboard generation. Generated `build/`
outputs and raw cosim `.info` snapshots are local artifacts and must not be
committed.

For a full refresh, run every cosim before copying coverage snapshots:

```bash
make -C dv/rtlcosim test
make update-cosim-coverage
```

`make update-cosim-coverage` writes deterministic gzip files with `gzip -9n` and
removes raw `dv/rtlcosim/coverage/coverage_*_cosim.info` snapshots.

For a task-scoped targeted update, running `make update-cosim-coverage` after
`make -C dv/rtlcosim clean` plus only selected cosims can create targeted-only
snapshots. Do not commit those files as replacements for the global checked-in
coverage snapshots. Preserve the compressed baseline
`dv/rtlcosim/coverage/*.info.gz`, save the targeted
`dv/rtlcosim/build/coverage/coverage_*_cosim.info` output, merge or append only
the targeted module records into decompressed working copies, then recompress the
final snapshots with `gzip -9n`. The final raw decompressed diff must be scoped
to the targeted cosims (insertion-only for new cosims); the checked-in
`*.info.gz` files are binary. If that safe merge cannot be produced or audited,
run the full refresh flow instead.

## Adding a new co-simulation

1. Create `dv/rtlcosim/<module>/`
2. Write a `*_cosim_tb.sv` that instantiates both old and new modules with an adapter for reset polarity and tied-off ASIC ports
3. Write a `*_cosim_test.cc` that drives stimulus and uses `CosimCtrl<>` from `dv/rtlcosim/common/cosim_ctrl.h` with `sim.compare()` to compare outputs
4. Create a `Makefile` following the pattern in existing cosim tests
5. If the cosim adds new untracked files, remember that plain `git diff --check`
   will not inspect them. Binary gzip snapshots do not have whitespace to check,
   but mark approved coverage snapshot paths as intent-to-add for status, and run
   a whitespace check on the new text files, for example:

   ```bash
   git add -N dv/rtlcosim/<module> dv/rtlcosim/coverage/<approved-files>.info.gz
   git diff --check -- dv/rtlcosim/<module>
   ```

   Alternatively, stage only the approved cosim/coverage paths, run
   `git diff --cached --check`, then `git reset` those paths if the review
   handoff should remain unstaged. Do not stage generated `build/` outputs or
   unrelated local changes just to run the whitespace check.
