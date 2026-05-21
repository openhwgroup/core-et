# dll_dly_est

DLL delay estimator translated from CORE-ET `dll_dly_est` and
`dll_dly_est_core`.

## What it does

`dll_dly_est_core` sends an 8-bit LFSR sequence through a delay-code loopback,
checks the returned sequence after the original fixed valid delay, counts
mismatches, and reports completion through ESR status fields. `dll_dly_est`
wraps the core with the original two-clock Shire/HV to Neighborhood/LV register
loopback and return crossing.

## Parameters

| Module | Parameter | Default | Meaning |
|--------|-----------|---------|---------|
| `dll_dly_est_core` | `Width` | 8 | Error-counter width. The ESR status carries the low/zero-extended 8 bits, matching the original packed status field. |
| `dll_dly_est_core` | `SyncStages` | 3 | Number of core-local valid/end delay flops expected between TX write and RX compare. |
| `dll_dly_est_core` | `PolyInit` | 1 | Initial value for the x^8+x^4+x^3+x^2+1 LFSR. |
| `dll_dly_est` | `Width` | 8 | Width of the inlined semisynchronous loopback register crossings only; the wrapped `dll_dly_est_core` intentionally keeps its original default `Width=8`, `SyncStages=3`, and `PolyInit=1`. |

## Ports and protocol

### `dll_dly_est_core`

- `clk_i`, `rst_ni`: Shire/HV clock and active-low reset.
- `dll_dly_est_ctl_i`: `esr_pkg::esr_dll_dly_est_ctl_t` control:
  - `ctl_enable`: enables internal counters/LFSRs/status updates.
  - `ctl_init`: initializes TX/RX LFSRs and clears status.
  - `ctl_start`: starts a measurement and captures `ctl_txn`.
  - `ctl_txn[1:0]`: selects the TX counter terminal bit (5/11/17/23).
    `ctl_txn[7:2] != 0` preserves the original behavior where RX completion is
    not asserted and software must reinitialize/abort the run.
- `dll_dly_est_sts_o`: `esr_pkg::esr_dll_dly_est_sts_t` status:
  - `sts_errn`: saturating mismatch count.
  - `sts_done`: RX completion flag.
- `tx_dly_est_write_o`, `tx_dly_est_code_o`: generated write pulse and LFSR code
  to the loopback crossing.
- `rx_dly_est_code_i`: returned code sampled when the internal delayed valid is
  asserted.

### `dll_dly_est`

- `clk_shire_i`, `rst_shire_ni`: Shire/HV core and return-read domain.
- `clk_neigh_i`, `rst_neigh_ni`: Neighborhood/LV loopback domain.
- `dll_dly_est_ctl_i`, `dll_dly_est_sts_o`: same ESR control/status protocol as
  the core.

The wrapper keeps the original two reset domains separate. The return crossing's
`rd_valid` remains unused, matching the original wrapper; the core derives the
expected compare-valid delay internally. Changing the wrapper `Width` parameter
only changes the payload width of the two semisynchronous register crossings;
it does not change the core error-counter width.

## Integration notes

Software should pulse `ctl_init` while enabled to reset the estimator, then pulse
`ctl_start` with the desired `ctl_txn`. For the short measurement mode,
`ctl_txn[1:0] == 2'b00`, completion occurs after the delayed final TX write
returns through the loopback. The wrapper assumes the Shire and Neighborhood
clocks have the original semi-synchronous 1:1 DLL relationship.

## Differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| Active-high `reset`/`reset_shire`/`reset_neigh` became active-low `rst_ni`/`rst_shire_ni`/`rst_neigh_ni`. | Project reset convention; reset domains remain separate. |
| Ports use `_i`/`_o` names and lowRISC-style parameter names. | Project SystemVerilog style. |
| ESR structs come from `esr_pkg` instead of `soc.vh`/`shire_types.vh`. | Package-based type sharing replaces textual includes. |
| CORE-ET flip-flop macros are explicit `always_ff` blocks. | Project coding style; reset/enable ordering is preserved. |
| Wrapper semisynchronous register crossings are inlined rather than replaced by `_ss` FIFO primitives. | The original uses one-word register crossings (`semisync_reg_wr_hiv/lov`), not FIFO contracts; the inlined logic preserves the local wr/rd register behavior and keeps the unused return `rd_valid` unused. |
| The wrapper instantiates `dll_dly_est_core` without parameter overrides even when wrapper `Width` is changed. | Matches the original `dll_dly_est`, whose `WIDTH` parameter sizes only `wr_data`, `rd_data`, `data_loopback`, and the two semisynchronous register crossings. |

## Verification notes

The wrapper cosim is parameterized so non-default wrapper crossing widths can be
checked against the original. For example, from the repository root with
`ORIG_ROOT` pointing at the CORE-ET checkout:

```bash
ORIG_ROOT=/home/glguida/ainekko/et-soc make -C dv/rtlcosim/dll_dly_est clean test EXTRA_FLAGS='-DET_ASSERT_OFF -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SYMRSVDWORD -GWidth=4'
```

This `Width=4` regression exercises the original wrapper `WIDTH` semantics while
leaving the internal core parameters at their original defaults.

No intentional functional divergence from the original is known.
