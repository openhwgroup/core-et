# Minion VPU

Vector / graphics coprocessor subtree for `minion_top`.

This block is being translated bottom-up from the original CORE-ET
`rtl/shire/minion/vpu/` tree. The minion-facing boundary already exists in
`minion_pkg`, `core_top`, and `minion_top`; this subtree holds the internal
VPU-only types and datapath blocks that sit behind that boundary.

## Current scope

| Module | File | Original | Status |
|--------|------|----------|--------|
| `vpu_pkg` | `rtl/vpu_pkg.sv` | `vpu_types.vh` + `trans_types.vh` internal types/constants | Done |
| `vpu_defs_pkg` | `rtl/vpu_defs_pkg.sv` | selected constants/aliases from the original VPU include stack | RTL present for TXFMA top-half support |
| `vpu_bypass` | `rtl/vpu_bypass.sv` | `vpu_bypass.v` | Done |
| `vpu_mask` | `rtl/vpu_mask.sv` | `vpu_mask.v` | Done |
| `vpu_rf` | `rtl/vpu_rf.sv` | `vpu_rf.v` | Done |
| `vpu_sh_sw` | `rtl/vpu_sh_sw.sv` | `vpu_sh_sw.v` | Done |
| `txfma_csa` | `rtl/txfma_csa.sv` | `txfma_csa.v` | Done |
| `txfma_4_2_compressor` | `rtl/txfma_4_2_compressor.sv` | `txfma_4_2_compressor.v` | Done |
| `txfma_4_2_compressor_array` | `rtl/txfma_4_2_compressor_array.sv` | `txfma_4_2_compressor_array.v` | Done |
| `txfma_booth_ppg_32r4_norm` | `rtl/txfma_booth_ppg_32r4_norm.sv` | `txfma_booth_ppg_32r4_norm.v` | Done |
| `txfma_booth_ppg_32r4_msb` | `rtl/txfma_booth_ppg_32r4_msb.sv` | `txfma_booth_ppg_32r4_msb.v` | Done |
| `txfma_booth_ppg_32r4` | `rtl/txfma_booth_ppg_32r4.sv` | `txfma_booth_ppg_32r4.v` | Done |
| `txfma_wallace1` | `rtl/txfma_wallace1.sv` | `txfma_wallace1.v` | Done |
| `txfma_wallace2` | `rtl/txfma_wallace2.sv` | `txfma_wallace2.v` | Done |
| `txfma_c0` | `rtl/txfma_c0.sv` | `txfma_c0.v` | RTL present, covered through `txfma_top`/`txfmaexp_top` cosim |
| `txfma_c1` | `rtl/txfma_c1.sv` | `txfma_c1.v` | RTL present, covered through `txfma_top`/`txfmaexp_top` cosim |
| `txfma_c2` | `rtl/txfma_c2.sv` | `txfma_c2.v` | RTL present, covered through `txfma_top`/`txfmaexp_top` cosim |
| `txfma_c3` | `rtl/txfma_c3.sv` | `txfma_c3.v` | RTL present, covered through `txfma_top`/`txfmaexp_top` cosim |
| `txfma_c4` | `rtl/txfma_c4.sv` | `txfma_c4.v` | RTL present, covered through `txfma_top`/`txfmaexp_top` cosim |
| `txfma_c5` | `rtl/txfma_c5.sv` | `txfma_c5.v` | RTL present, covered through `txfma_top`/`txfmaexp_top` cosim |
| `txfma_c6` | `rtl/txfma_c6.sv` | `txfma_c6.v` | RTL present, covered through `txfma_top`/`txfmaexp_top` cosim |
| `txfma_e1` | `rtl/txfma_e1.sv` | `txfma_e1.v` | RTL present, covered through `txfmaexp_top` cosim |
| `txfma_e2` | `rtl/txfma_e2.sv` | `txfma_e2.v` | RTL present, covered through `txfmaexp_top` cosim |
| `txfma_e4` | `rtl/txfma_e4.sv` | `txfma_e4.v` | RTL present, covered through `txfmaexp_top` cosim |
| `txfma_e5` | `rtl/txfma_e5.sv` | `txfma_e5.v` | RTL present, covered through `txfmaexp_top` cosim |
| `txfma_e6` | `rtl/txfma_e6.sv` | `txfma_e6.v` | RTL present, covered through `txfmaexp_top` cosim |
| `txfma_ediff_opdorder_logic` | `rtl/txfma_ediff_opdorder_logic.sv` | `txfma_ediff_opdorder_logic.v` | RTL present, covered through `txfmaexp_top` cosim |
| `txfma_exp_special_detect` | `rtl/txfma_exp_special_detect.sv` | `txfma_exp_special_detect.v` | RTL present, standalone DV/cosim pending |
| `txfma_f0` | `rtl/txfma_f0.sv` | `txfma_f0.v` | RTL present, covered through `txfma_top` cosim |
| `txfma_f1` | `rtl/txfma_f1.sv` | `txfma_f1.v` | RTL present, covered through `txfma_top` cosim |
| `txfma_f2` | `rtl/txfma_f2.sv` | `txfma_f2.v` | RTL present, covered through `txfma_top` cosim |
| `txfma_f3` | `rtl/txfma_f3.sv` | `txfma_f3.v` | RTL present, covered through `txfma_top` cosim |
| `txfma_f4` | `rtl/txfma_f4.sv` | `txfma_f4.v` | RTL present, covered through `txfma_top` cosim |
| `txfma_f5` | `rtl/txfma_f5.sv` | `txfma_f5.v` | RTL present, covered through `txfma_top` cosim |
| `txfma_f6` | `rtl/txfma_f6.sv` | `txfma_f6.v` | RTL present, covered through `txfma_top` cosim |
| `txfma_frac_zero_detect` | `rtl/txfma_frac_zero_detect.sv` | `txfma_frac_zero_detect.v` | RTL present, covered through `txfma_top` cosim |
| `txfma_rnd_adder` | `rtl/txfma_rnd_adder.sv` | `txfma_rnd_adder.v` | RTL present, covered through `txfma_top` cosim |
| `txfmactl_top` | `rtl/txfmactl_top.sv` | `txfmactl_top.v` | Done |
| `txfmaexp_top` | `rtl/txfmaexp_top.sv` | `txfmaexp_top.v` | Done |
| `txfmafrac_top` | `rtl/txfmafrac_top.sv` | `txfmafrac_top.v` | Done |
| `txfma_top` | `rtl/txfma_top.sv` | `txfma_top.v` | Done |
| `txfma_top_fake` | `rtl/txfma_top_fake.sv` | `txfma_top_fake.v` | Done |
| `txfma_lxd` | `rtl/txfma_lxd.sv` | `txfma_lxd.v` | Done |
| `txfma_trz` | `rtl/txfma_trz.sv` | `txfma_trz.v` | Done |
| `txfma_trz2` | `rtl/txfma_trz2.sv` | `txfma_trz2.v` | Done |
| `txfma_adder` | `rtl/txfma_adder.sv` | `txfma_adder.v` | Done |
| `txfma_align_shf` | `rtl/txfma_align_shf.sv` | `txfma_align_shf.v` | Done |
| `txfma_norm_shf` | `rtl/txfma_norm_shf.sv` | `txfma_norm_shf.v` | Done |
| `tima_adder` | `rtl/tima_adder.sv` | `tima_adder.v` | Done |
| `tima_top` | `rtl/tima_top.sv` | `tima_top.v` | Done |
| `trans_exp_rom_case_c0` | `rtl/trans_exp_rom_case_c0.sv` | `trans_exp_rom_case_c0.v` | Done (cosim only) |
| `trans_exp_rom_case_c1c2` | `rtl/trans_exp_rom_case_c1c2.sv` | `trans_exp_rom_case_c1c2.v` | Done (cosim only) |
| `trans_log_rom_case_c0` | `rtl/trans_log_rom_case_c0.sv` | `trans_log_rom_case_c0.v` | Done (cosim only) |
| `trans_log_rom_case_c1c2` | `rtl/trans_log_rom_case_c1c2.sv` | `trans_log_rom_case_c1c2.v` | Done (cosim only) |
| `trans_rcp_rom_case_c0` | `rtl/trans_rcp_rom_case_c0.sv` | `trans_rcp_rom_case_c0.v` | Done (cosim only) |
| `trans_rcp_rom_case_c1c2` | `rtl/trans_rcp_rom_case_c1c2.sv` | `trans_rcp_rom_case_c1c2.v` | Done (cosim only) |
| `trans_rsqrt_rom_case_c0` | `rtl/trans_rsqrt_rom_case_c0.sv` | `trans_rsqrt_rom_case_c0.v` | Done (cosim only) |
| `trans_rsqrt_rom_case_c1c2` | `rtl/trans_rsqrt_rom_case_c1c2.sv` | `trans_rsqrt_rom_case_c1c2.v` | Done (cosim only) |
| `trans_sin_rom_case_c0` | `rtl/trans_sin_rom_case_c0.sv` | `trans_sin_rom_case_c0.v` | Done (cosim only) |
| `trans_sin_rom_case_c1c2` | `rtl/trans_sin_rom_case_c1c2.sv` | `trans_sin_rom_case_c1c2.v` | Done (cosim only) |
| `trans_top` | `rtl/trans_top.sv` | `trans_top.v` | Done |
| `vpu_trans` | `rtl/vpu_trans.sv` | `vpu_trans.v` | Done |
| `vpu_tensora_rf` | `rtl/vpu_tensora_rf.sv` | `vpu_tensora_rf.v` | Done |
| `vpu_tensorb_rf` | `rtl/vpu_tensorb_rf.sv` | `vpu_tensorb_rf.v` | Done |
| `vpu_tensorc_rf` | `rtl/vpu_tensorc_rf.sv` | `vpu_tensorc_rf.v` | Done |
| `vpu_tensortmp_rf` | `rtl/vpu_tensortmp_rf.sv` | `vpu_tensortmp_rf.v` | Done |
| `vpu_lane_tima` | `rtl/vpu_lane_tima.sv` | `vpu_lane_tima.v` | Done |
| `vpu_uinst_decoder` | `rtl/vpu_uinst_decoder.sv` | `vpu_uinst_decoder.v` | Done |
| `vpu_tensorreduce` | `rtl/vpu_tensorreduce.sv` | `vpu_tensorreduce.v` | Done |
| `vpu_txfma_trans_top` | `rtl/vpu_txfma_trans_top.sv` | `vpu_txfma_trans_top.v` | Done (`UseFakeTxfma=0/1`) |
| `vpu_tensorfma` | `rtl/vpu_tensorfma.sv` | `vpu_tensorfma.v` | Done |
| `vpu_tensorquant` | `rtl/vpu_tensorquant.sv` | `vpu_tensorquant.v` | Done |
| `vpu_ml` | `rtl/vpu_ml.sv` | `vpu_ml.v` | Done |
| `vpu_ctrl` | `rtl/vpu_ctrl.sv` | `vpu_ctrl.v` | Done |
| `vpu_lane` | `rtl/vpu_lane.sv` | `vpu_lane.v` | Done |
| `vpu_top` | `rtl/vpu_top.sv` | `vpu_top.v` | Done (standalone VPU-local DV/cosim; not connected to `minion_top` yet) |

The remaining VPU top-half integration RTL is now present locally. The current
standalone closure covers `vpu_tensorreduce`, `vpu_tensorfma`,
`vpu_tensorquant`, `vpu_ml`, both real and fake `vpu_txfma_trans_top`
configurations, the standalone TXFMA control/fraction subtops `txfmactl_top`
and `txfmafrac_top`, plus the `vpu_ctrl`, `vpu_lane`, and `vpu_top`
integration modules. The real `vpu_top` is still not connected to `minion_top`;
that replacement remains a later integration step after the standalone VPU-local
closure.

## What lives here

- internal VPU-only structs and enums that should not stay in `minion_pkg`
- VPU leaf datapath blocks
- later, the integration blocks `vpu_ml`, `vpu_trans`, `vpu_lane`, `vpu_ctrl`,
  and `vpu_top`

The shared minion-facing request/response bundles remain in `minion_pkg`
because they are already used by `core_top`, `minion_top`, and `null_vpu`.

The imported TXFMA top-half RTL uses `vpu_defs_pkg` as a narrow compatibility
package for constants and aliases that were originally spread through the global
VPU include stack. The existing `vpu_pkg` MMI compatibility defaults remain in
place; this stage does not overwrite the target-specific VPU RF/MMI policy.

## Porting strategy

The original VPU tree is large and deeply pipelined. The translation order is:

1. extract internal types to `vpu_pkg`
2. port leaf modules with standalone unit tests and standalone cosims
3. port integration modules on top of those leaves
4. replace `null_vpu` in `minion_top` with a faithful translated `vpu_top`

## Configuration parameters

The translated VPU keeps original feature and implementation knobs as explicit
parameters/package defaults instead of synthesizable `` `ifdef `` gates.

| Knob | Default | Used by | Meaning |
|------|---------|---------|---------|
| `EnableExtraTransDefault` | `0` | `trans_top`, `vpu_trans`, `vpu_uinst_decoder`, `vpu_ctrl` | Enables the extra transcendental RSQRT/SIN flow beyond the base RCP/LOG/EXP flow |
| `UseFakeTxfmaDefault` | `0` | `vpu_txfma_trans_top`, `vpu_top` | Selects the translated fake-TXFMA model for verification |
| `EnableRcg2Default` | `1` | `vpu_lane` | Enables the translated per-submodule clock-gating path in each lane |
| `UseMmi*Default` | target-specific | VPU RF/TIMA/TXFMA leaves | Preserves the target's existing explicit MMI compatibility policy |

## Scripts

### `scripts/extract_trans_rom.py`

This helper reconstructs the original transcendental ROM contents directly from
the checked-in CORE-ET `trans_*_rom_case_*.v` files. It emits both:

- raw `rom_data` dumps
- packed `trans_fma_coefficients` dumps in the original packed-struct order

  `{exc, mask[6:0], taylor, c2[16:0], c0[31:0]}`

The script is intentionally source-of-truth driven: it parses the original
case tables and `trans_defines.vh` instead of regenerating coefficients from an
approximation flow that is not present in the repository.

Usage example:

```bash
python3 hw/ip/minion/vpu/scripts/extract_trans_rom.py \
  --orig-root ~/ainekko/etcore-soc \
  --out-dir build/trans_rom_extract
```

Useful options:

- `--module trans_rcp_rom_case_c0` to dump one ROM module
- `--view raw|packed|all` to select raw `rom_data` vs packed coefficients
- `--format hex|bin|json|all` to control the emitted file types

### `scripts/gen_trans_rom_sv.py`

This helper takes the packed `.hex` files produced by
`extract_trans_rom.py` and emits SystemVerilog for the original helper-module
boundaries, for example:

- `trans_rcp_rom_case_c0.packed.hex` -> `trans_rcp_rom_case_c0.sv`
- `trans_sin_rom_case_c1c2.packed.hex` -> `trans_sin_rom_case_c1c2.sv`

The emitted modules preserve the original module granularity and packed output
behavior, but use repo-native package types and `_i`/`_o` port naming.

Usage example:

```bash
python3 hw/ip/minion/vpu/scripts/gen_trans_rom_sv.py \
  --hex-dir hw/ip/minion/vpu/data/trans_rom \
  --out-dir /tmp/trans_rom_sv
```

This is intentionally a generation step, not a runtime build step. The
canonical packed ROM data and the generated helper RTL are checked in, and
tracked cosim directories exist under `dv/rtlcosim/<module>/`. Each ROM helper
is checked against the original with an exhaustive sweep over all 256
`trans_rom_index` values.

### `scripts/gen_trans_rom_cosim.py`

This helper materializes the tracked `dv/rtlcosim/<module>/` directories for
the generated trans ROM helper modules. It reads the canonical ROM manifest and
emits:

- `dv/rtlcosim/<module>/Makefile`
- `dv/rtlcosim/<module>/cosim_<module>_tb.sv`
- `dv/rtlcosim/<module>/cosim_<module>_test.cc`

The generated cosim exhaustively sweeps all 256 `trans_rom_index` values and
compares the packed coefficient output cycle-by-cycle against the original RTL.

## Module notes

### `vpu_pkg`

`vpu_pkg` contains the original VPU-internal types from `vpu_types.vh` and
`trans_types.vh`, plus small compatibility aliases for the original
`minion_reg_dest` / `minion_mreg_dest` names. It also carries explicit
RTL-visible defaults for the original VPU feature / implementation knobs
(`EnableExtraTrans`, MMI RF/TIMA/TXFMA selection, fake TXFMA, and `VPU_EN_RCG_2`)
so translated modules do not need new preprocessor guards. The minion-facing
VPU boundary types are intentionally not duplicated here.

### `vpu_defs_pkg`

`vpu_defs_pkg` carries TXFMA/top-half constants and compatibility aliases that
were originally provided by the global VPU include stack. It imports no behavior;
it only centralizes widths, instruction encodings, TXFMA control-bit positions,
and aliases such as `vpu_minion_id_ctrl` and `reduce_tensorstore_control` that
are needed by the translated top-half RTL.

### `vpu_bypass`

`vpu_bypass` resolves register-file bypassing for one VPU lane and pipelines
short-latency `sh/sw` results forward into the later F4-F8 stages.

Ports:

| Port group | Description |
|------------|-------------|
| EX/F1 inputs | RF read addresses/data, force-bypass controls, from-int path, tensor RF bypass data |
| F3-F8 inputs | staged RF writeback addresses/masks plus `sh/sw` and TXFMA results |
| EX output | forwarded lane request with `in1`/`in2`/`in3` replaced by selected bypass data |
| F3 output | `maskop` bit extracted from the staged short-path data |
| F8 outputs | selected writeback flags/data and the raw `sh/sw` writeback data |

### `vpu_sh_sw`

`vpu_sh_sw` is the short-latency integer/graphics leaf that implements the
lane-local shift, compare, swizzle, cube-face, sign-injection, saturation, and
simple add/sub datapaths behind the VPU decode boundary.

Ports:

| Port group | Description |
|------------|-------------|
| EX inputs | command, dtype, immediates, mask bit, and the three source operands for one lane |
| Swizzle sideband | `f2_swizz_in*` inputs consumed during the F2->F3 capture cycle for `fswizz` / `packrep*` paths |
| F2 output | `f2_fswizz_rdata_o` forwards the latched `in1` operand for later swizzle staging |
| F3 output | `f3_out_bits_o` returns the selected data result plus exception flags |

### `vpu_mask`

`vpu_mask` is the VPU mask-register file leaf. It owns the per-thread mask RF,
the mask-operation datapath (`mand`, `mor`, `mxor`, `mnot`, `mova.{m,x}`), the
mask-to-int reductions (`mpopc*`), and the staged mask-write scoreboard that
tracks short-path and TXFMA compare writes across F3-F8.

Ports:

| Port group | Description |
|------------|-------------|
| EX inputs | mask RF read addresses, command, immediate, integer source image, thread id, ignore-mask override |
| F2 inputs | stage-local write-enable / mask-op / to-int / destination control |
| F3/F4 inputs | short-compare and TXFMA compare write steering |
| F5-F8 inputs | stage thread ids and final compare-result bits |
| EX/F2/F8 outputs | forwarded mask values, to-int result, and F8 regmask-store bits |
| WB outputs | four-slot mask scoreboard (`wb_mask_valid_o`, `wb_mask_dest_o`) for the minion hazard logic |

### `vpu_rf`

`vpu_rf` is the lane-local scalar/vector floating-point register file wrapper.
It preserves both original implementations behind an explicit parameter:
the default latch-backed path with low-phase write-data capture, and the
original MMI-model path that samples write data directly on the rising edge.

Parameters:

| Parameter | Default | Description |
|-----------|---------|-------------|
| `UseMmi` | `vpu_pkg::UseMmiVpuRfDefault` (`0`) | Selects the original MMI-style RF behavior instead of the latch-backed RF path |
| `UseMmiNetlist` | `vpu_pkg::UseMmiVpuRfNetlistDefault` (`0`) | Reserved explicit knob for the future hard-macro netlist replacement |

Ports:

| Port group | Description |
|------------|-------------|
| Read | Three read-enable / thread-id / address inputs feeding three flopped RF read addresses and three combinational read-data outputs |
| Write control | Two write-enable ports plus per-port thread id, mask, address, and data |
| Write-preview seam | `wr_en_early_i` is honored only in the latch-backed path, now carried through the coarser `prim_rf_3r2w_preview` seam |

Behavior notes:

| Mode | Original source | Key timing |
|------|-----------------|------------|
| `UseMmi=0` | `vpu_rf.v` latch path + `rf_latch_3r_2w` | write data captured in the low phase, committed on the next rising edge; write port 0 wins same-address collisions |
| `UseMmi=1` | `vpu_rf.v` MMI path + `etcust_64x32_3r2w_vpurf_model.v` | write data sampled directly on the rising edge; write port 1 wins same-address collisions |

### `txfma_csa`

`txfma_csa` is the one-bit/bit-vector carry-save adder leaf used throughout the
TXFMA multiplier-reduction tree.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | three same-width carry-save operands `a_i`, `b_i`, and `cin_i` |
| Outputs | same-width carry-save outputs `cout_o` and `sum_o` |

### `txfma_4_2_compressor`

`txfma_4_2_compressor` is the scalar 4:2 compressor leaf built from two
single-bit `txfma_csa` instances, preserving the original intermediate carry
chain exactly.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | bit inputs `x0_i`..`x3_i` plus the incoming carry `cin_i` |
| Outputs | outgoing compressor carry `cout_o`, local carry `c_o`, and local sum `s_o` |

### `txfma_4_2_compressor_array`

`txfma_4_2_compressor_array` is the width-parameterized TXFMA reduction leaf.
It ripples the `cout` output from each bit position into the next bit’s
incoming carry exactly as in the original `txfma_7s` tree.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | four `Width`-bit input vectors `x0_i`..`x3_i` |
| Outputs | `Width`-bit compressor carry vector `c_o` and sum vector `s_o` |

### `txfma_booth_ppg_32r4_norm`

`txfma_booth_ppg_32r4_norm` is the regular radix-4 Booth partial-product
generator leaf. It maps the three-bit recode input to `0`, `+x`, `-x`, `+2x`,
or `-2x` exactly as in the original TXFMA tree, with the same 33-bit signed
extension behavior.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | 32-bit multiplicand `mulcand_i`, Booth recode bits `r4input_i`, and `sign_i` selecting signed vs unsigned multiplicand extension |
| Outputs | 33-bit Booth partial product `pp_o` |

### `txfma_booth_ppg_32r4_msb`

`txfma_booth_ppg_32r4_msb` is the special final partial-product helper used at
the top of the radix-4 tree. It preserves the original “unsigned extra MSB
only” behavior exactly.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | 32-bit multiplicand `mulcand_i`, final multiplier MSB `msb_i`, and `sign_i` |
| Outputs | 33-bit final partial product `pp_o` |

### `txfma_booth_ppg_32r4`

`txfma_booth_ppg_32r4` is the full 17-lane radix-4 Booth front end for the
TXFMA multiplier. It expands the multiplier into 17 recode groups, instantiates
the normal/MSB helper leaves, and emits the original `sxt`/`cin` sideband
vectors used by the downstream compressor tree.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | 32-bit multiplicand `mpcnd_i`, 32-bit multiplier `mplr_i`, signed/unsigned select `sign_i`, and phase mode `mode_i` |
| Outputs | `sxt_o`, `cin_o`, and the 17 34-bit partial-product outputs `pp1_o`..`pp17_o` |

### `txfma_wallace1`

`txfma_wallace1` is the first two levels of the TXFMA Wallace reduction tree.
It widens the first sixteen Booth partial products into the 64-bit accumulator
space, injects the original mode-dependent “magic 1” bits, and reduces those
products down to the four intermediate carry/sum vectors consumed by the next
stage.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | partial products `pp1_i`..`pp16_i` plus the mode select `mode_i` |
| Outputs | 64-bit intermediate carry/sum vectors `c21_o`, `c22_o`, `s21_o`, and `s22_o` |

### `txfma_wallace2`

`txfma_wallace2` is the final Wallace reduction stage ahead of the later TXFMA
adder/normalization pipeline. It combines the Booth sideband vectors
`{sxt_i, cin_i}`, the final Booth partial product, and the two intermediate
carry/sum pairs from `txfma_wallace1` into the final 64-bit carry/sum pair.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | Booth sideband vectors `sxt_i`/`cin_i`, final partial product `pp17_i`, and the intermediate vectors `c21_i`, `c22_i`, `s21_i`, `s22_i` |
| Outputs | final 64-bit carry/sum pair `c41_o` and `s41_o` |

### `txfma_adder`

`txfma_adder` is the small carry-propagate adder leaf used by the later TXFMA
alignment / rounding stages. It preserves the original widened temporary sum
and the intentional drop of the final carry-out bit.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | `a_i`, `b_i`, and `cin_i` form the parameterized-width add input |
| Output | `sum_o` returns the low `Width` bits of the widened sum, matching the original carry-drop behavior |

### `txfma_align_shf`

`txfma_align_shf` is the TXFMA alignment right-shifter leaf. It takes the
incoming datapath window and shifts it right by the exponent-sized alignment
amount, preserving the original parameter defaults used by the TXFMA stages.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | `op_i` is the parameterized datapath window and `shfamnt_i` is the logical right-shift amount |
| Output | `op_align_o` returns the full-width aligned datapath window |

### `txfma_norm_shf`

`txfma_norm_shf` is the TXFMA normalization shift leaf. It preserves the
original signed shift convention: negative `shfamnt_i` values shift right,
non-negative values shift left, and only the normalized output slice is kept.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | `op_i` is the pre-normalized datapath window and `shfamnt_i` is the signed normalization shift amount |
| Output | `op_norm_o` returns the original normalized output slice from the shifted intermediate |

### `txfma_trz`

`txfma_trz` is the small trailing-zero leaf used by the early TXFMA operand
prep path. It scans the fraction window from LSB significance upward and
returns the index of the lowest asserted bit, or `Max+1` when the input is all
zero, matching the original default-zero behavior exactly.

Ports:

| Port group | Description |
|------------|-------------|
| Input | `frac_i` is the parameterized fraction window under inspection |
| Output | `trz_o` returns the original trailing-zero index encoding, with the all-zero case mapped to `Max+1` |

### `txfma_trz2`

`txfma_trz2` is the later TXFMA trailing-zero anticipator used on the combined
adder inputs. It preserves the original propagate/kill indicator chain and the
same default “all mantissas are zero” result when no indicator bit asserts.

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | `in1_i`, `in2_i`, and `add_cin_i` form the original adder-side propagate/kill inputs |
| Output | `trz_o` returns the lowest asserted indicator position, or `InWidth` when none are set |

### `txfma_lxd`

`txfma_lxd` is the PS-side leading-distance anticipator used in the later
TXFMA combine stage. It preserves the original generate/propagate/kill
construction, the hard-wired `indicator[0]` base case, and the ascending scan
that lets the highest asserted indicator index win.

Parameters:

| Parameter | Default | Description |
|-----------|---------|-------------|
| `InWidth` | `vpu_pkg::TxfmaPsDpathSz` | PS-side combined datapath width |
| `AftA0BitsNum` | `vpu_pkg::TxfmaA3 - vpu_pkg::TxfmaA0 + 1` | number of bits above `A0` removed from the final distance |
| `OutWidth` | `vpu_pkg::TxfmaLxdPsSz` | encoded leading-distance width |

Ports:

| Port group | Description |
|------------|-------------|
| Inputs | `in1_i`, `in2_i`, and `sub_op_i` form the original PS-side anticipator inputs |
| Output | `lxd_o` returns the encoded leading distance used by the later TXFMA normalization logic |

### TXFMA staged helpers (`txfma_c*`, `txfma_e*`, `txfma_f*`)

The imported staged TXFMA helpers preserve the original split between control
(`txfma_c0`..`txfma_c6`), exponent (`txfma_e1`, `txfma_e2`, `txfma_e4`,
`txfma_e5`, `txfma_e6`), and fraction/result (`txfma_f0`..`txfma_f6`) pipeline
logic. Smaller helpers such as `txfma_ediff_opdorder_logic`,
`txfma_exp_special_detect`, `txfma_frac_zero_detect`, and `txfma_rnd_adder`
keep the original leaf boundaries. Most of these are exercised through the
imported TXFMA top cosims; `txfma_exp_special_detect` is imported for boundary
completeness but still needs standalone DV/cosim coverage.

### `txfmactl_top`, `txfmaexp_top`, and `txfmafrac_top`

`txfmactl_top` owns the staged TXFMA control decode and valid/exception control
pipe. `txfmaexp_top` owns the exponent-side difference, sticky, overflow, and
underflow decisions. `txfmafrac_top` owns the fraction datapath, local clock-gating seams, and later
rounding/normalization stages. All three top-level TXFMA subblocks now have
standalone unit tests and all-output cosims in addition to parent `txfma_top`
coverage.

### `txfma_top`

`txfma_top` ties the imported control, exponent, and fraction TXFMA subtrees
together behind the original VPU lane request/response contract. It keeps the
original active-high `reset` and `clock` names at this translated boundary
because the surrounding imported TXFMA files are still mechanical top-half
translations. The standalone unit test provides smoke checks, while the cosim
compares data, exception flags, and compare-result outputs against the original
real-TXFMA top.

### `txfma_top_fake`

`txfma_top_fake` is the legacy fake TXFMA model used for verification-only
bring-up. The translated fake leaf intentionally uses the same
`out_data_res`/`out_comp_res` port contract as `txfma_top`; `out_comp_res` is
hardwired low. This intentionally diverges from the original fake leaf's older
single-output port contract to avoid the original fake-branch elaboration bug
tracked in [BUGS.md](BUGS.md).

### `tima_adder`

`tima_adder` is the leaf integer adder used by the TIMA datapath. It is a
pure combinational helper that preserves the original 32-bit wraparound
addition semantics.

Ports:

| Port | Description |
|------|-------------|
| `a_i`, `b_i` | 32-bit adder operands |
| `sum_o` | 32-bit wraparound sum |

### `tima_top`

`tima_top` is the three-stage TIMA datapath leaf. It captures per-byte
signedness in F1, computes the four-lane byte products in F2, and adds the
sign-extended product sum to the optional tensor-C operand in F3 through
`tima_adder`.

Ports:

| Port group | Description |
|------------|-------------|
| F1 inputs | multiply enable, two 32-bit operands, and per-operand signedness controls |
| F2 inputs | tensor-C operand, operand-3 zap control, and the independent F2 capture enables |
| F3 output | 32-bit wraparound TIMA result |

### Tensor RF leaves

`vpu_tensora_rf`, `vpu_tensorb_rf`, `vpu_tensorc_rf`, and `vpu_tensortmp_rf`
are the VPU tensor register-file wrappers. They preserve the original two-part
write protocol: `wr_en_early_i` captures the write payload during the low
phase, then `wr_en_i` commits that latched payload into the RF on the rising
edge. This means a late `wr_en_i` without a fresh `wr_en_early_i` intentionally
reuses the previously latched write data, matching the original CORE-ET RF
implementation. In the translated tree this contract is expressed with the
named `prim_rf_1r1w_preview` seam rather than an open-coded raw latch.

Ports:

| Port group | Description |
|------------|-------------|
| Read | `rd_addr_i` selects one synchronous read port and returns `rd_data_o` |
| Low-phase write capture | `wr_en_early_i`, `wr_addr_i`, `wr_data_i` latch the pending write payload |
| Rising-edge commit | `wr_en_i` commits the previously latched write payload into the RF |

### `vpu_lane_tima`

`vpu_lane_tima` is the lane-local TIMA wrapper. It binds the tensor-B and
tensor-C RF leaves to the gated `tima_top` datapath and preserves the original
feedback path where the tensor-C write port captures the current TIMA result.

Ports:

| Port group | Description |
|------------|-------------|
| System | `clk_i`, `tima_clk_en_i`, and the preserved-but-unused `te_i` interface bit from the original wrapper |
| Tensor A/B/C | tensor-A read data input, tensor-B/C RF read/write controls, and tensor-B read data output |
| TIMA controls | F1 multiply enable / signedness plus the F2 TIMA capture controls |
| Output | `tima_out_o` returns the lane-local TIMA result |

### `vpu_uinst_decoder`

`vpu_uinst_decoder` is the internal micro-op decoder used by the VPU transcendental
pipeline. It decodes the synthetic `TR_*` instruction stream emitted by
`vpu_trans` plus the packed/tensor helper instructions that never cross the
integer decode boundary.

Parameters:

| Parameter | Default | Description |
|-----------|---------|-------------|
| `EnableExtraTrans` | `vpu_pkg::EnableExtraTransDefault` (`0`) | Enables the original `ENABLE_EXTRA_TRANS` micro-op decode cases (`TR_*_RSQRT`, `TR_*_SIN`) |

### `trans_top`

`trans_top` is the transcendental ROM/pipeline leaf used by the VPU trans path.
It owns the original F2-F8 response pipeline, ROM-bank selection, EXP exception
carry, and the sine-specific Taylor handling on top of the generated
`trans_*_rom_case_*` helper modules.

Parameters:

| Parameter | Default | Description |
|-----------|---------|-------------|
| `EnableExtraTrans` | `vpu_pkg::EnableExtraTransDefault` (`0`) | Enables the original `ENABLE_EXTRA_TRANS` RSQRT/SIN ROM cases and command handling |

### `vpu_trans`

`vpu_trans` is the transcendental micro-op sequencer. It detects the external
RCP/RSQRT/LOG/SIN/EXP start instructions, advances the original seven-stage
ring of `trans_state` entries, injects the internal `TR_*` micro-ops back into
ID, and maintains the six-slot transpose scoreboard plus the F6/F8 write
control pipe that feeds the minion dirty-state path.

Parameters:

| Parameter | Default | Description |
|-----------|---------|-------------|
| `EnableExtraTrans` | `vpu_pkg::EnableExtraTransDefault` (`0`) | Enables the original `ENABLE_EXTRA_TRANS` RSQRT/SIN start-op handling, state transitions, and internal micro-op generation |

Ports:

| Port group | Description |
|------------|-------------|
| EX inputs | `ex_valid_i`, `ex_in_bits_i.sigs.cmd`, and `ex_in_bits_i.in2` drive the original trans request/address generation logic |
| TXFMA flags | `ex_txfma_exc_i` carries the exception bits that the EXP path injects into the F8 packed response |
| F8 output | `f8_rom_response_o` returns the packed `trans_fma_coefficients_t` response used by the later VPU trans/TXFMA stages |

Behavior notes:

| Path | Original behavior preserved |
|------|-----------------------------|
| `SIN` Taylor mode | `f2_sin_taylor` is derived from the exponent bucket in `in2[30:23]`, not from the ROM helper output |
| `SIN_P1` | Taylor mode overrides `c0` with the original hard-coded constant `32'hc90fdb00` |
| `SIN_P2` / `TRANS_ROM_SIN3_ID` | Only the packed `taylor` bit is updated; the other coefficient fields intentionally carry the previously selected response |
| `EXP` | `exc` is re-sourced from `ex_txfma_exc_i` and carried through the original F2->F8 pipe |

### `vpu_tensorreduce`

`vpu_tensorreduce` sequences tensor reduce and tensor-store requests into the
VPU pipeline. It preserves the original active-high reset, pending-register
counter, sticky wait bits for scoreboard/TensorFMA/TensorQuant dependencies,
and dcache-driven `send_reg` / `exec_op` instruction stuffing.

Ports:

| Port group | Description |
|------------|-------------|
| Request inputs | `reduce_start`, `tensorstore_start`, `tensorfma_start`, `tensorquant_start`, and packed `reduce_ctrl` select the new tensor operation |
| Dependency inputs | `scoreboard_pend`, `tensorfma_pend`, and `tensorquant_pend` hold off reduce/store issue until older VPU writes clear |
| Dcache control | `dcache_reduce_ctrl` advances send/execute/nothing actions from the dcache reduce unit |
| Outputs | `enabled`, `reduce_wait`, packed load control, the next injected instruction, and debug status |

### `vpu_txfma_trans_top`

`vpu_txfma_trans_top` binds the transcendental ROM pipe to either the real
seven-stage TXFMA or the translated fake-TXFMA model. The real and fake paths
share the same parent-visible output contract in this repo; the fake path's
original port-contract bug is documented in [BUGS.md](BUGS.md).

Parameters:

| Parameter | Default | Description |
|-----------|---------|-------------|
| `UseFakeTxfma` | `vpu_pkg::UseFakeTxfmaDefault` (`0`) | Selects `txfma_top_fake` instead of the real `txfma_top` implementation |

Ports:

| Port group | Description |
|------------|-------------|
| System | Separate `clock_txfma`, `clock_trans_rom`, and active-high `reset` inputs preserve the original boundary |
| EX inputs | `ex_rom_valid`, `ex_txfma_valid`, and packed `ex_in_bits` feed the ROM and TXFMA paths |
| F8 outputs | `f8_txfma_res`, `f8_txfma_comp_res`, and two debug bits from the packed ROM response |

### Tensor datapath integration blocks

`vpu_tensorfma` sequences tensor FMA requests, scratchpad reads, tensor A/B load
control, TIMA/TENC/TMP addressing, and TensorFMA instruction injection. Its
standalone unit test covers hazard waiting, FP32/FP16/INT8 setup, convolution
mask waiting, scratchpad fill sequencing, and tensor-temp/TIMA side effects; the
standalone cosim compares every output against `vpu_tensorfma.v`.

`vpu_tensorquant` translates TensorQuant transform lists into injected VPU
instructions and scratchpad/TENA read controls. Its standalone unit test covers
empty transform suppression, scoreboard/TensorFMA/TensorStore hazards,
FCVT/ReLU/add/mul/saturation transforms, scratchpad availability waiting, and
TENA/ex read pipelining; the standalone cosim compares every output against
`vpu_tensorquant.v`.

`vpu_ml` arbitrates the tensor FMA, TensorQuant, and tensor reduce/store child
blocks. Its standalone unit test covers isolated TensorFMA, TensorQuant, reduce,
tensorstore, and simultaneous-start paths plus dcache/control muxing; the
standalone cosim compares every top-level output against `vpu_ml.v` while also
building the original child modules.

### Lane/control integration

`vpu_lane` integrates the lane RF/bypass, short-path, TXFMA/trans, and TIMA
children. Its standalone unit test covers reset/default behavior, RF
read/write/bypass flow, short-path valid/clock-valid behavior, TXFMA/trans and
ROM clock-valid paths, TIMA/tensor RF controls, and lane-local chicken-bit
clock-enable inputs. The standalone cosim drives every aggregate input bit both
low and high and compares all eight declared outputs against `vpu_lane.v`.

`vpu_ctrl` integrates decode, scoreboard/stage control, mask, transpose/ML, and
tensor child controls. Its standalone unit test covers reset/default behavior,
normal decoded VPU instruction flow, register read/thread ID generation, swap
variants, from-int propagation, short/TXFMA/ROM clock-valid paths, and dcache
writeback response handling. The standalone cosim drives every aggregate input
bit both low and high, including the TIMA/ML/kill/debug boundary fields, and
compares all 85 declared outputs against `vpu_ctrl.v`.

The lane/control cosims target the default translated configurations
(`EnableRcg2=1`, `UseFakeTxfma=0`, `EnableExtraTrans=0`) to match the original
standalone modules used here. `UseFakeTxfma=1` remains covered at the
`vpu_txfma_trans_top` boundary because the original fake-TXFMA child has the
documented port-contract bug noted below. `EnableExtraTrans=1` remains covered by
the standalone `vpu_uinst_decoder`, `trans_top`, and `vpu_trans` extra-trans
variants; the `vpu_ctrl` all-output cosim keeps the original default
`ENABLE_EXTRA_TRANS`-off configuration.

### Top integration standalone closure

`vpu_top` integrates the translated VPU control and lane blocks behind the
minion-facing request/control boundary. Its standalone unit test covers
reset/default behavior, decoded short-path/TXFMA/ROM/TIMA paths, dcache response
propagation, kill/chicken-bit controls, thread/debug fields, and output sanity.
The standalone cosim drives every aggregate input bit both low and high across
directed and random phases and compares all 10 declared top-level outputs against
`vpu_top.v` on every sampled cycle. The translated `vpu_top` remains disconnected
from `minion_top`; the integer-only `null_vpu` substitution is left unchanged for
a later top-level integration job.

## Differences from original

| Aspect | Original | Reimplementation | Rationale |
|--------|----------|------------------|-----------|
| Internal types | `include "soc.vh"` | `vpu_pkg` + `minion_pkg` packages | repo-native package ownership |
| Naming | `clock`, `ex_req_lane`, `f8_wdata` | `clk_i`, `ex_req_lane_i`, `f8_wdata_o` | lowRISC convention |
| FF macros | `EN_FF` | explicit `always_ff` | standard SV |
| Functional `ifdef`s | `` `ifdef ENABLE_EXTRA_TRANS `` / original MMI selection defines | explicit RTL-visible parameters / package defaults, while the target keeps its VPU RF/MMI compatibility knobs | repo policy: functional feature selection must not stay in the preprocessor |
| TXFMA clock gates | `et_clk_gate` in the fraction/top pipeline | `prim_clk_gate` | technology-swappable clock-gating seam |
| Fake TXFMA parent/leaf output contract | `txfma_top_fake` has only `out_data`; original fake `vpu_txfma_trans_top` branch tries to connect `out_data_res`/`out_comp_res` | translated `txfma_top_fake` and `vpu_txfma_trans_top` expose a consistent `out_data_res` plus hardwired-low compare result contract | intentional divergence for a documented original repository bug; see [BUGS.md](BUGS.md) |
| Reset | VPU top-half modules use active-high `reset` where present | preserved active-high `reset` at imported VPU top-half boundaries | exact original top-half behavior preserved |

No functional changes are intended except for the documented fake-TXFMA port-contract divergence.

## Verification

| Module | Unit test | Cosim |
|--------|-----------|-------|
| `vpu_bypass` | 25 checks | 98,412 comparisons |
| `vpu_mask` | 15 checks | 57,827 comparisons |
| `vpu_rf` | 12,391 checks (`UseMmi=0`) + 12,391 checks (`UseMmi=1`) | 49,251 comparisons (`UseMmi=0`) + 49,251 comparisons (`UseMmi=1`) |
| `vpu_sh_sw` | 87 checks | 30,697 comparisons |
| `txfma_csa` | 522 checks | 50,266 comparisons |
| `txfma_4_2_compressor` | 3,168 checks | 12,384 comparisons |
| `txfma_4_2_compressor_array` | 524 checks | 16,396 comparisons |
| `txfma_booth_ppg_32r4_norm` | 608 checks | 25,608 comparisons |
| `txfma_booth_ppg_32r4_msb` | 536 checks | 25,152 comparisons |
| `txfma_booth_ppg_32r4` | 9,842 checks | 155,762 comparisons |
| `txfma_wallace1` | 8,216 checks | 25,288 comparisons |
| `txfma_wallace2` | 8,200 checks | 25,074 comparisons |
| `txfmactl_top` | 79 checks | 996,194 comparisons |
| `txfmaexp_top` | 31 checks | 508,152 comparisons |
| `txfmafrac_top` | 43 checks | 390,468 comparisons |
| `txfma_top` | 9 checks | 15,024 comparisons |
| `txfma_top_fake` | 18 checks | 15,024 comparisons |
| `txfma_lxd` | 4,102 checks | 25,162 comparisons |
| `txfma_trz` | 4,102 checks | 25,054 comparisons |
| `txfma_trz2` | 4,102 checks | 25,162 comparisons |
| `txfma_adder` | 4,101 checks | 25,197 comparisons |
| `txfma_align_shf` | 4,102 checks | 25,566 comparisons |
| `txfma_norm_shf` | 4,104 checks | 25,234 comparisons |
| `tima_adder` | 69 checks | 25,068 comparisons |
| `tima_top` | 8,205 checks | 50,018 comparisons |
| generated `trans_*_rom_case_*` helpers | — | 10 x 257 comparisons |
| `trans_top` | 8,344 checks (`EnableExtraTrans=0`) + 8,372 checks (`EnableExtraTrans=1`) | 65,626 comparisons (`EnableExtraTrans=0`) + 65,659 comparisons (`EnableExtraTrans=1`) |
| `vpu_trans` | 49 checks (`EnableExtraTrans=0`) + 66 checks (`EnableExtraTrans=1`) | 900,990 comparisons (`EnableExtraTrans=0`) + 900,990 comparisons (`EnableExtraTrans=1`) |
| `vpu_tensora_rf` | 5 checks | 50,010 comparisons |
| `vpu_tensorb_rf` | 5 checks | 50,010 comparisons |
| `vpu_tensorc_rf` | 5 checks | 50,010 comparisons |
| `vpu_tensortmp_rf` | 5 checks | 50,010 comparisons |
| `vpu_lane_tima` | 200,066 checks | 200,040 comparisons |
| `vpu_uinst_decoder` | 19 checks (`EnableExtraTrans=0`) + 19 checks (`EnableExtraTrans=1`) | 50,294 comparisons (`EnableExtraTrans=0`) + 50,294 comparisons (`EnableExtraTrans=1`) |
| `vpu_tensorreduce` | 27 checks | 15,030 comparisons |
| `vpu_txfma_trans_top` | 16 checks (`UseFakeTxfma=0`) + 13 checks (`UseFakeTxfma=1`) | 524,588 comparisons (`UseFakeTxfma=0`) + 262,444 comparisons (`UseFakeTxfma=1`) |
| `vpu_tensorfma` | 24 checks | 551,854 comparisons |
| `vpu_tensorquant` | 41 checks | 259,600 comparisons |
| `vpu_ml` | 26 checks | 602,896 comparisons |
| `vpu_ctrl` | 25 checks | 2,019,808 comparisons |
| `vpu_lane` | 16 checks | 780,108 comparisons |
| `vpu_top` | 19 checks | 4,823,376 comparisons |
