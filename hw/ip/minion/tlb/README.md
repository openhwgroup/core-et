# Minion TLB

Address translation lookaside buffer for the minion core.

Replaces: `tlb_top` from the CORE-ET source tree.

## Modules

| Module | File | Replaces |
|--------|------|----------|
| `minion_tlb` | `rtl/minion_tlb.sv` | `tlb_top` |
| `tlb_pkg` | `rtl/tlb_pkg.sv` | TLB-local cache-entry / FSM types from `soc.vh` |

## What It Does

- Accepts minion translation requests (`tlb_req`) from dcache or icache wrappers.
- Checks the local TLB array for a matching translation.
- Issues PTW requests on misses and refills the cache from PTW responses.
- Applies privilege, MXR, SUM, access, dirty, and malformed-address checks.
- Handles per-minion and cooperative invalidation.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `Entries` | 8 | Number of TLB entries |
| `NrMinions` | 1 | Number of minions sharing this TLB instance |

## Port Interface

| Port | Direction | Type | Description |
|------|-----------|------|-------------|
| `clk_i` | input | logic | Functional clock |
| `rst_ni` | input | logic | Active-low async reset |
| `dft_i` | input | `dft_t` | DFT bundle for the gated FSM clock |
| `vmspagesize_i` | input | `tlb_entry_type` | Minimum enabled page size |
| `coop_mode_i` | input | logic | Cooperative shared-TLB mode |
| `req_min_id_i` | input | logic | Requesting minion ID |
| `req_data_i` | input | `tlb_req` | Translation request bundle |
| `req_valid_i` | input | logic | Request valid |
| `resp_data_o` | output | `tlb_resp` | Translation hit/miss/exception response |
| `satp_info_i` | input | `minion_satp_info [NrMinions]` | Per-minion SATP mode + PPN |
| `matp_info_i` | input | `minion_satp_info [NrMinions]` | Per-minion MATP mode + PPN |
| `tlb_invalidate_i` | input | logic [NrMinions-1:0] | Per-minion invalidation request |
| `ptw_req_data_o` | output | `minion_ptw_req` | PTW request bundle |
| `ptw_req_valid_o` | output | logic | PTW request valid |
| `ptw_req_ready_i` | input | logic | PTW accepts request |
| `ptw_invalidate_o` | output | logic | Invalidate the shared PTW path |
| `ptw_resp_data_i` | input | `minion_ptw_pte` | PTW response PTE |
| `ptw_resp_valid_i` | input | logic | PTW response valid |
| `vm_enabled_o` | output | logic | Translation active for the current request |

## Integration Notes

- `minion_tlb` is a standalone minion sub-IP under `hw/ip/minion/tlb/`.
- Shared VM/PTW request and response types stay in `minion_pkg` because they
  are also consumed by dcache, icache, and PTW wrappers.
- `tlb_pkg` only contains TLB-local state and cache-entry types.
- The cache array uses `prim_rf_1r1w`, matching the original `rf_latch_1r_1w`
  access model rather than the SRAM-style `prim_ram_*` path.

## Differences From Original

| Aspect | Original | Reimplementation | Rationale |
|--------|----------|------------------|-----------|
| Reset | Active-high synchronous `reset` | Active-low async-assert/sync-deassert `rst_ni` | Project reset convention |
| Naming | `clock`, `reset`, bare port names | lowRISC `_i` / `_o` names | Project naming convention |
| Types | `include "soc.vh"` globals | `minion_pkg` + `tlb_pkg` packages | Scoped ownership |
| Clock gating | `et_clk_gate` | `prim_clk_gate` + `dft_t` | Technology abstraction |
| RF preview seam | open-coded low-phase preview latch + `rf_latch_1r_1w` | `prim_rf_1r1w_preview` | Named semantic seam for exact generic / FPGA-safe tech mapping |

No intentional functional changes. The miss/refill FSM, PTW handshake, tag/index
functions, invalidation behavior, and permission checks match the original
cycle-by-cycle behavior.

## Verification

| Check | Result |
|-------|--------|
| Block lint | `make -C hw/ip/minion/tlb/dv lint` |
| Unit test | `123` checks, `0` failures |
| RTL cosim | `517,146` comparisons, `0` mismatches |
