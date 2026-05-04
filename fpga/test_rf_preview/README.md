# test_rf_preview

ULX3S-ready focused bring-up project for the repository's preview and
preview-plus-storage primitives.

## What It Does

This project isolates the recurring preview and preview-plus-storage patterns
that matter for the Minion cache and RF paths:

1. `prim_rf_1r1w_preview`
2. `prim_rf_1r1w_dec_preview`
3. `prim_rf_1r1w_par_preview`
4. `prim_rf_1r1w_diff_preview`
5. `prim_rf_1r1w_reg_preview`
6. `prim_rf_2r1w_preview`
7. `prim_rf_3r2w_preview`
8. `prim_rf_1r1w_par_preview`
9. `prim_rf_single_1r1w_par_preview`
10. direct chained `prim_write_preview_en`
11. Width/depth variants of the preview RF primitives used by cache metadata
    and data storage seams
12. Paired 128-entry `prim_rf_1r1w_reg_preview` instances used as a direct
    primitive contract check for line-storage-style timing
    (`B0`-`B4` explicitly check registered-read read/write collision behavior,
    held-read-address write-through, and split low/high half writes).
13. D-cache data-array debug-style write/read and scalar core-style read through
    `minion_dcache_data_array`
14. D-cache leaf-array access patterns using actual
    `minion_dcache_metadata_array` valid/tag hit, all-bank line write/read,
    low-only and high-only lane writes, and address/way isolation
15. D-cache-shaped leaf hit timing: metadata valid/tag/coherency and data-array
    read launch together and are checked one cycle later, without the full
    dcache top-level controller.
16. Registered-read RF enable timing: a disabled read with a changed address
    must hold the previous read address, then a re-enabled read must advance.
17. D-cache S1/S2/S3 timing seam: metadata tag hit is captured from an S1-like
    launch into S2-like state, data-array output is checked with the read enable
    dropped, and the S3-like response is checked one cycle later.
18. D-cache data-array read-enable hold/resume: after an enabled read, changed
    per-bank request fields must not advance the registered read address while
    the top-level array read enable is low, then must advance when re-enabled.
19. D-cache miss-suppressed S3 capture: metadata miss and data-array output are
    observed together, and the S3-like response must remain invalid/zero.
20. D-cache back-to-back read latency: two consecutive enabled reads to different
    way/address pairs must return the previous launch one cycle later.
21. D-cache leaf-array sweep: eight row/way combinations are written through
    the preview/commit path across all four banks and read back one cycle later.
22. D-cache metadata S0/S1/S2 pipeline seam: an S0-like launch loads a
    target set/tag into an S1-like stage, metadata valid/tag/state are read and
    captured into S2-like flops on the next edge, matching the path that a
    dcache load hit depends on.
23. D-cache top-level tag-hit qualifier seam: a raw S2 tag match is checked
    through the same message/AMO/bypass/DC-bypass qualifier mask used by
    `minion_dcache_top`, then a bypass-shaped request must suppress the raw
    match without destroying the previously proven hit path.
24. D-cache hostile timing seam: decoy metadata is written into the non-hit ways,
    then an S0/S1/S2/S3-shaped read must observe all four ways valid, match only
    the target way, capture state-hit in S2, and return the expected bank data in
    S3.
25. A hostile preview-clock contract check: after a valid seed write, the
    preview clock is held in the non-update phase while a blocked write is
    attempted; the stored value must not change.
26. A full preview-RF dual-clock contract sweep (`Y0`-`Y2`) that repeats the
    seed, blocked-preview, and rearmed-write sequence across all preview RF
    primitive shapes, including the frontend-sized mixed-width RF.

The harness runs a built-in self-check and reports a compact UART summary. That
makes it useful both as a fast Verilator loop and as a small ULX3S hardware
check while debugging ECP5-specific preview timing issues.

The built-in checks cover:

1. same-width preview reuse and split preview/commit timing
2. decoded-address preview reuse and multi-target write timing
3. parallel-readback preview reuse across multiple entries
4. mixed-width preview reuse, partial-lane preservation, and split timing
5. registered-read preview reuse and split preview/commit timing
6. 2R1W preview reuse, zero-entry handling, and parallel-read taps
7. 3R2W dual-preview reuse and same-address write ordering
8. parallel-readback preview reuse across replay-style queue writes
9. single-entry preview reuse across tensor-load-style staging writes
10. raw two-stage `prim_write_preview_en` chaining as used by preview-only paths
11. cache-sized metadata-width preview RF update timing with preview data
    separated from commit address
12. registered-read 128-entry primitive fill/store readback plus a sentinel
    check for stray delayed writes
13. registered-read 128-entry primitive collision behavior through `B0`-`B4`:
    same-cycle same-address read/write must return the new staged data,
    same-cycle different-address read/write must return the addressed old data,
    `rd_en=0` must hold the registered read address while a write to that held
    entry becomes visible, and low/high split instances must preserve the
    untouched half during same-address collisions
14. the wrapped dcache data-array address/valid shape used by APB debug fill and scalar cache-hit reads
15. actual dcache metadata-array valid/tag hit behavior, all four dcache data
    banks, half-line write masks, and way/address independence
16. dcache-shaped metadata/data leaf hit timing through `Z5`
17. registered-read enable hold/resume behavior through `Z6`
18. dcache S1-to-S2-to-S3 timing through `Z7`, matching the visible contract
    that `test_dcache` depends on for cache-hit completion
19. data-array read-enable hold/resume through `Z8`
20. miss-suppressed S3 capture through `Z9`, where data is visible but metadata
    says no hit
21. back-to-back one-cycle data-array read timing through `ZA`
22. multi-bank, multi-way, multi-row dcache data-array sweep through `ZB`
23. metadata-array S0/S1/S2 tag-hit capture through `ZC`, checking that a
    D-cache-shaped set/tag read is capturable at the pipeline boundary rather
    than only eventually readable
24. top-level S2 tag-match qualification through `ZD`, checking both the raw
    matched-load hit path and the bypass-suppressed raw-match path
25. decoy-way D-cache timing stress through `ZE`, checking a full four-way
    metadata read, target-only S2 capture, and S3 data capture in the primitive
    FPGA project before running the slower `test_dcache`
26. preview-clock observability: `P3` must preserve the previous value when the
    preview phase is intentionally blocked, catching implementations that ignore
    `preview_clk_i`
27. canonical `prim_rf_1r1w_preview` protocol mask: `P4` checks preview-only
    no-commit behavior, commit-only use of staged data, commit data not being
    bypassed, delayed commit, commit-address selection, blocked preview clock,
    latest-preview-wins, and address independence
28. full preview-RF dual-clock protocol mask: `Y0` checks seeded writes with a
    live preview clock, `Y1` checks that a held-high preview clock blocks all
    writes, and `Y2` checks that writes resume after the preview clock is
    released across all preview RF primitive shapes

The expected UART terminates with:

```text
TEST RF PREVIEW PASSED
```

## Why This Exists

`minion_dcache_top` relies on these primitive and leaf-array contracts in its
metadata and data-array paths. This project does not replace full dcache
testing, but it now includes a minimal S1/S2/S3 timing seam so a one-cycle
tag/data transfer failure is caught in the fast FPGA primitive test before
running the much slower `test_dcache` project.

Relevant D-cache users include:

- [minion_dcache_metadata_array.sv](../../hw/ip/minion/dcache/rtl/minion_dcache_metadata_array.sv)
- [minion_dcache_buffer_array.sv](../../hw/ip/minion/dcache/rtl/minion_dcache_buffer_array.sv)
- [minion_dcache_128x64_1r1w_lram.sv](../../hw/ip/minion/dcache/rtl/minion_dcache_128x64_1r1w_lram.sv)
- [minion_dcache_128x72_1r1w_lram.sv](../../hw/ip/minion/dcache/rtl/minion_dcache_128x72_1r1w_lram.sv)
- [minion_dcache_replay_queue.sv](../../hw/ip/minion/dcache/rtl/minion_dcache_replay_queue.sv)
- [minion_dcache_tensor_load.sv](../../hw/ip/minion/dcache/rtl/minion_dcache_tensor_load.sv)
- [icache_micro_data_array.sv](../../hw/ip/icache_micro_data_array/rtl/icache_micro_data_array.sv)
- [icache_micro_tag_array.sv](../../hw/ip/icache_micro_tag_array/rtl/icache_micro_tag_array.sv)
- [icache_tag_array.sv](../../hw/ip/icache_tag_array/rtl/icache_tag_array.sv)
- [intpipe_rf.sv](../../hw/ip/minion/intpipe/rtl/intpipe_rf.sv)
- [vpu_rf.sv](../../hw/ip/minion/vpu/rtl/vpu_rf.sv)

## IPs Used

- `prim_write_preview_en`
- `prim_rf_1r1w`
- `prim_rf_1r1w_dec`
- `prim_rf_1r1w_dec_preview`
- `prim_rf_1r1w_par`
- `prim_rf_1r1w_par_preview`
- `prim_rf_1r1w_preview`
- `prim_rf_1r1w_diff`
- `prim_rf_1r1w_diff_preview`
- `prim_rf_1r1w_reg`
- `prim_rf_1r1w_reg_preview`
- `prim_rf_single_1r1w_par`
- `prim_rf_single_1r1w_par_preview`
- `prim_rf_2r1w`
- `prim_rf_2r1w_preview`
- `prim_rf_3r2w`
- `prim_rf_3r2w_preview`
- `minion_dcache_metadata_array`
- `minion_dcache_data_array`
- `minion_dcache_128x64_1r1w_lram`
- `ulx3s_uart_tx`

## Project Structure

```text
test_rf_preview/
  rtl/
    test_rf_preview.sv
  ulx3s/
    Makefile
    test_rf_preview_ulx3s.sv
    test_rf_preview_ulx3s_pll.sv
    ulx3s_v20.lpf
  verilator/
    Makefile
    test_rf_preview_test.cc
  srcs.mk
```

## Usage

```bash
# Verilator test with generic primitives
make -C verilator test

# Build ULX3S bitstream
make -C ulx3s bit

# Program ULX3S SRAM
make -C ulx3s prog
```

On the host:

```bash
stty -F /dev/ttyUSB0 115200 raw -echo
cat /dev/ttyUSB0
```
