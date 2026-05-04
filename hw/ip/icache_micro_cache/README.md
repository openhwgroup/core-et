<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# icache_micro_cache

`icache_micro_cache` is the standalone-Minion L0 I-cache controller from the
original CORE-ET shell. It sits in front of the L1 I-cache SRAM/tag arrays and
implements:

- the per-requester fetch pipeline (`F0` through `F5`)
- the standalone I-cache prefetch FSM
- the L0 miss FSM and refill handshake towards the shared L1 path
- TLB/PTW translation through `icache_tlb_array`
- PMA permission checking through `icache_pma_unit`
- the micro-tag and micro-data arrays
- the bypass-on-fill path used when the micro-cache is disabled
- the local APB debug window for micro-tag/data/error state

## Parameters

- `Id`
  Requester slot index written into the debug/status-monitor bundle.
- `NrMinions`
  Number of Minion contexts sharing the I-cache requester.

## Interface

### System / ESRs

- `clk_i`, `rst_ni`
  Functional clock and active-low async reset.
- `ioshire_i`
  Indicates whether this requester belongs to the IOShire address map.
- `esr_prefetch_conf_i`, `esr_prefetch_start_i`, `esr_prefetch_done_o`
  Prefetch-service configuration and completion pulse.
- `esr_mprot_i`
  Memory-protection ESRs forwarded to the PMA check.
- `esr_vmspagesize_i`, `esr_shire_coop_mode_i`
  TLB page-size and cooperative-mode ESRs.
- `esr_bypass_icache_i`
  Bypass the micro-cache and serve hits from the refill bypass register.

### Frontend side

- `f0_req_valid_i`, `f0_req_i`, `f0_req_min_id_i`, `f0_req_ready_o`
  Request input at `F0`.
- `f4_resp_valid_o`, `f4_resp_miss_o`, `f4_resp_o`
  Fetch response returned to the requester.
- `f5_resp_fill_done_o`
  Delayed fill-done pulse matching the original pipeline timing.
- `f0_flush_data_i`
  Invalidates micro-cache state.

### L1 miss/refill side

- `f0_l1_miss_req_ready_i`, `f0_l1_miss_req_valid_o`, `f0_l1_miss_req_addr_o`
  Miss request handshake towards the shared L1 I-cache.
- `f0_l1_miss_resp_early_valid_i`, `f0_l1_miss_resp_valid_i`,
  `f0_l1_miss_resp_data_i`, `f0_l1_miss_resp_ecc_err_i`,
  `f0_l1_miss_resp_l2_err_i`
  Refill return path from the shared L1 side.

### TLB / PTW

- `satp_info_i`, `matp_info_i`, `tlb_invalidate_i`
  Per-Minion VM state and invalidate requests.
- `ptw_req_data_o`, `ptw_req_valid_o`, `ptw_req_ready_i`,
  `ptw_invalidate_o`, `ptw_resp_valid_i`, `ptw_resp_data_i`
  PTW request/response interface forwarded through `icache_tlb_array`.

### APB debug / status monitor

- `apb_*`
  Micro-cache-local APB debug access for micro-tag/data/error state.
- `dbg_sm_signals_o`
  Partial standalone-Icache debug/status-monitor bundle.

## Integration notes

- The translated block keeps the original multi-stage fetch timing and refill
  behavior. It is not a redesign of the fetch path.
- The prefetch request generator still injects requests only when the requester
  is otherwise idle, matching the original standalone shell.
- The debug/status output currently carries the fields driven by the original
  micro-cache block itself. Higher-level aggregation fields are completed by
  `icache_top`.

## Intentional differences from the original

| Original | Translation |
|----------|-------------|
| `clock`, active-high `reset` | `clk_i`, active-low `rst_ni` |
| `soc.vh` macros/types | package constants and packed structs |
| macro flops (`RST_FF`, `EN_FF`, `RST_EN_FF`) | explicit `always_ff` state |
| original `icache_tlb_array` / `icache_pma_unit` naming | translated package-style ports |

No intentional functional differences are introduced.
