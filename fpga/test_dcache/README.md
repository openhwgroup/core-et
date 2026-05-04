# test_dcache

ULX3S-ready focused bring-up project for the repo's `minion_dcache_top`.

## What it does

This project reuses the existing synthesizable `minion_dcache_top_tb` wrapper
and runs a built-in scalar-only self-check over UART. The harness exercises
the most useful standalone dcache functional surfaces:

1. reset/idle bring-up (`id_core_ready_o`, idle error/event outputs)
2. stage-1 breakpoint sideband classification for a physical load request
3. SATP-programmed virtual request translation reaching the PTW request port
4. APB-debug cache prefill followed by scalar load, store, and load-back hits

The current project is intentionally narrow:

- the Verilator and ULX3S heads both instantiate the same `test_dcache` project
  top with `DebugApbEn=1`, seed metadata/data through the public dcache APB
  debug window, then issue normal scalar requests against the prefilled line
- scalar requests are issued only after `id_core_ready_o`, matching the
  core/intpipe contract instead of relying on a fixed-cycle post-APB delay
- `DebugMonEn=1` in the Verilator and ULX3S heads so the harness can print a
  compact `HX`/`HZ` hit-path decode for the APB-prefilled scalar load
- `VpuEn=0` by default to keep the FPGA target focused on the scalar cache/MMU
  path

The expected UART terminates with:

```text
TEST DCACHE PASSED
```

## Debug Interfaces Covered

`minion_dcache_top` exposes three distinct debug/observability surfaces. This
project intentionally enables APB debug as a deterministic standalone prefill
path, then checks the scalar dcache path through normal request traffic.

1. APB debug window
   - `apb_*`
   - metadata debug read/write
   - data-array debug read/write

2. Request-side observability
   - `s1_bp_conf_valid_o`
   - `s1_bp_conf_o`
   - `io_events_o`
   - `bus_err_o`

3. Raw debug monitor outputs
   - `csr_debug_sigs_o`
   - `sm_match_debug_signals_*`
   - `sm_filter_debug_signals_*`
   - `sm_data_debug_signals_*`

Both simulation and ULX3S configurations enable the APB debug window only for
prefill. The monitor surface is decoded only for the scalar hit/miss decision
line printed as `HX`: S1 tag match, S2 qualified tag match, S1/S2 validity,
S2 hit, miss request, nack-miss, cacheability, TLB miss/fill, request type,
S2 command, S1/S2 physical-address bits, and uncacheable-load second-pass bit.
The following `HZ` line narrows the same path further by printing raw S2 tag
match, the qualifier kill mask, S2 state-hit/state, S2 valid/masked bits, and
the pre-misaligned nack bit. `HY` checks that S1 tag match overlaps S1/S2 valid
and S2 qualified-hit conditions. `HG` checks the S1-to-S2 capture gate directly:
`V`, `C`, `M`, and `D` are S1 tag match masked by `s1_valid_to_s2`, `s1_clk_en`,
`s1_md_read_en`, and `s1_da_read_en`, respectively.

## IPs Used

- `minion_dcache_top` — dcache under test
- `minion_dcache_top_tb` — existing standalone wrapper reused as the FPGA-facing
  contract
- `minion_tlb` — translated shared TLB under the dcache wrapper
- `ulx3s_uart_tx` — local UART transmitter reused from ULX3S bring-up

## Project Structure

```text
test_dcache/
  rtl/
    test_dcache.sv
  ulx3s/
    Makefile
    test_dcache_ulx3s.sv
    test_dcache_ulx3s_pll.sv
    ulx3s_v20.lpf
  verilator/
    Makefile
    test_dcache_test.cc
  srcs.mk
```

## Usage

```bash
# Verilator test
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

The ULX3S head uses the same conservative board bring-up pattern as the other
projects: reset stays asserted until the PLL locks, then remains asserted for
about one second before the self-check starts.
