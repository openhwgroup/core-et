// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# shirecache original-repository bugs

## Original repository bug: dual-port tag-state generic RAM ignores `dft__ram_rei`

- **Original module:** `rtl/shire/shire_cache/shire_cache_pipe_tag_state_ram_wrap.v`
- **Reimplemented module:** `hw/ip/shirecache/rtl/shirecache_pipe_tag_state_ram_wrap.sv`
- **Symptom:** In the original non-ASIC/generic dual-port tag-state RAM path, asserting `esr_shire_cache_ram_cfg.dft__ram_rei` does not inhibit the read port. A read with `rd_en=1` still updates `rd_data` from `rd_addr`.
- **Root cause:** The generic `gen_mem2p` instantiation wires `.enB(rd_en)` at original lines 1094-1098 and omits `!esr_shire_cache_ram_cfg.dft__ram_rei`. The single-port generic path uses `.en(1'b1 && !...dft__ram_rei)` at lines 1068-1074, and the ASIC dual-port macro path gates both `.MEA` and `.MEB` with `!dft__ram_rei` at lines 142-147.
- **Reachability:** Reachable only in the original non-ASIC/generic dual-port tag-state RAM simulation configuration with `SC_TAG_STATE_RAM_SINGLE_PORT` undefined and `ET_USE_ASIC_MEMS` undefined. The taped-out ASIC macro path has the inhibit on both ports.
- **System impact:** DFT RAM read-enable inhibit behavior in generic simulation can differ from the ASIC SRAM wrapper behavior for tag-state reads. Normal functional cache traffic is unaffected because `dft__ram_rei` is a DFT/test control.
- **Current reimplementation behavior:** The reimplementation intentionally diverges from the original generic simulation bug and preserves the ASIC/DFT contract by gating `rd_en_gated = rd_en_i & ~dft_i.ram_rei` and connecting it to `prim_ram_2p.b_req_i` (new lines 94-112). Standalone cosim keeps active `ram_rei` reads on a held address so all visible outputs remain comparable while still toggling the inhibit path; tag/data RAM wrappers and `shirecache_pipe_sub_bank_mem` also exercise `ram_rei`.
