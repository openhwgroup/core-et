<!--
Copyright (c) 2026 Ainekko
SPDX-License-Identifier: Apache-2.0
-->

# icache_pma_unit

`icache_pma_unit` is the standalone Minion I-cache physical-memory-attributes
checker. It classifies fetch addresses as cacheable or faulting based on the
physical address map, the ESR memory-protection settings, and the PTW-provided
privilege level.

## Interface

- `ioshire_i`
  Selects IOShire vs regular Shire address-permission behavior.
- `paddr_i`
  Physical fetch address.
- `mprot_i`
  ESR memory-protection control struct.
- `ptw_prv_i`
  Privilege mode coming back from the PTW/TLB path.
- `resp_data_o`
  `{access_fault, cacheable}` response.

## Intentional differences from the original

| Original | Translation |
|----------|-------------|
| `esr_mprot_t` from ESR headers | `minion_dcache_pkg::esr_mprot_t` |
| `icache_pma_resp` | `icache_pkg::icache_pma_resp_t` |

No intentional functional differences are introduced.
