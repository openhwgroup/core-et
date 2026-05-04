<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# icache_tlb_array

`icache_tlb_array` is the standalone-Minion I-cache TLB wrapper from the original
CORE-ET shell. It adapts the I-cache request/response structs used by the
standalone frontend to the shared translated `minion_tlb` implementation.

The wrapper:

- latches pseudo-static SATP and MATP configuration from the Minion side
- latches the I-cache page-size ESR
- converts `icache_tlb_req_t` into the shared `tlb_req`
- hardwires the lookup as an instruction fetch
- converts the shared `tlb_resp` back into `icache_tlb_resp_t`

## Parameters

- `Entries`
  Number of TLB entries passed through to `minion_tlb`.
- `NrMinions`
  Number of Minion contexts sharing this wrapper.

## Interface

### System

- `clk_i`, `rst_ni`
  Functional clock and active-low async reset.

### ESR / request side

- `esr_vmspagesize_i`
  Current I-cache page-size mode.
- `esr_shire_coop_mode_i`
  Cooperative TLB mode.
- `req_min_id_i`
  Minion context issuing the lookup.
- `req_data_i`, `req_valid_i`
  I-cache TLB lookup request. The request status, VPN, passthrough flag, and
  top-bit address error are forwarded to the shared TLB.

### Response side

- `resp_data_o`
  I-cache-facing translation result. Only the original I-cache-visible fields
  are exposed: `fill_pending`, `miss`, `ppn`, `xcpt_if`, and `access_fault`.

### PTW / invalidate control

- `satp_info_i`, `matp_info_i`
  Per-Minion SATP and MATP configuration.
- `tlb_invalidate_i`
  Per-Minion invalidate request.
- `ptw_req_data_o`, `ptw_req_valid_o`, `ptw_req_ready_i`
  PTW request handshake forwarded from the shared TLB.
- `ptw_invalidate_o`
  Shared invalidate pulse towards the PTW.
- `ptw_resp_valid_i`, `ptw_resp_data_i`
  PTW response returned to the shared TLB.

## Integration notes

- The wrapper keeps the original instruction-fetch semantics by always driving
  `instruction=1` and `store=0` into `minion_tlb`.
- The page-size ESR and SATP/MATP arrays are flopped exactly once before being
  presented to the shared TLB, matching the original standalone shell.

## Intentional differences from the original

| Original | Translation |
|----------|-------------|
| `clock`, active-high `reset` | `clk_i`, active-low `rst_ni` |
| Local `tlb_top` instantiation | Shared translated `minion_tlb` |
| `soc.vh` types (`icache_tlb_req`, `icache_tlb_resp`) | package-based `icache_tlb_req_t`, `icache_tlb_resp_t` |
| CORE-ET macro flops (`RST_FF`, `RST_EN_FF`) | explicit `always_ff` state |

No intentional functional differences are introduced.
