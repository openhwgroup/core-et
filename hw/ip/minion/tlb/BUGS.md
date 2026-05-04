# Minion TLB Bugs

## Original Repository Bug: `tlb_top` can cache stale translation data on a 1-cycle PTW response

### Scope

- Original module: `rtl/shire/minion/tlb_top.v` in the CORE-ET repository
- Reimplemented module: `hw/ip/minion/tlb/rtl/minion_tlb.sv`
- Affected wrapper/integration point: `rtl/shire/minion/dcache/dcache_tlb_array.v` and dcache consumers of TLB translations

### Summary

The original `tlb_top` can write a TLB entry with the correct tag for the current translation miss but stale payload data from an older PTW response or from uninitialized RF contents. This becomes reachable when the PTW returns a one-cycle `ptw_resp_valid` pulse, which the original PTW path does.

### Severity

High. This is a silent wrong-translation risk, not just a test artifact. The worst case is a store using the wrong physical address.

### Original-Code Evidence

- The original PTW emits a one-cycle response-valid pulse:
  - `rtl/shire/neigh/ptw_top.v`
  - `resp_valid_next = (state == PTW_S_DONE);`
  - flopped into `resp_valid` with `EN_FF` / `RST_FF`
- The shared PTW wrapper does not stretch the response:
  - `rtl/shire/neigh/neigh_shared_ptw.v`
  - `resp_valid[i] = ptw_resp_valid && (req_dest_reg == i);`
- In the original TLB, the RF early-capture path is keyed off raw `ptw_resp_valid`:
  - `rtl/shire/minion/tlb_top.v`
  - `assign rf_latch_wr_en_early = ... & ptw_resp_valid & ...;`
- But the RF write payload is built from registered `ptw_resp_data_reg`:
  - `rtl/shire/minion/tlb_top.v`
  - `EN_FF(clock, ptw_resp_valid, ptw_resp_data_reg, ptw_resp_data)`
  - `assign tlb_cache_wdata = '{..., ppn: ptw_resp_data_reg.ppn[...]};`
- The original RF primitive captures write data in phase 2 / low-clock timing:
  - `rtl/libs/rf_latches/rf_latch_1r_1w.v`
  - `if (wr_data_a_en_1p) wr_data_a_del <= wr_data_a;`

### Failure Mechanism

1. A TLB miss saves the current request identity (`req_min_id_reg`, `req_vpn_reg`, `req_tag_reg`).
2. The PTW returns `ptw_resp_valid = 1` for one cycle with new `ptw_resp_data`.
3. During the low phase of that cycle, the RF early-data latch opens because `rf_latch_wr_en_early` depends on raw `ptw_resp_valid`.
4. At that point, `ptw_resp_data_reg` is still old because it only updates on the next rising edge.
5. On the next rising edge, `ptw_resp_valid_reg` becomes `1`, so the TLB:
   - writes the RF entry
   - sets the corresponding valid bit
6. The write address/tag come from the current miss (`req_*_reg`), but the payload can be stale because the early-data latch captured old `ptw_resp_data_reg`.

In short: the original can create `tag(B) + payload(A)`.

### System Impact

The stale entry can cause:

- silent wrong-load data
- silent wrong-store data to an unrelated physical page
- false page/access faults
- spurious misses/retries if stale mode bits prevent a hit

On the dcache side, the translated PPN is used to form `s1_phys_addr`, so a stale-but-hitting TLB entry can directly drive the wrong physical address into the cache pipeline.

### Reachability

This is reachable with the original PTW path. The original PTW producer does not hold `resp_valid` for two cycles and there is no `resp_ready` handshake on the TLB response interface.

### Verification Status In This Repo

- `make -C hw/ip/minion/dcache/dv test-xrand-tlb_array` exposes the issue in the dcache wrapper test.
- The failing xrand check initially assumed the stale PPN would be zero; randomized initialization shows the stale contents are not deterministic.
- The standalone `minion_tlb` unit test currently avoids the hazard by holding the PTW response valid for two cycles in its directed refill helper.

### Guidance

- Do not silently "fix" this in the reimplementation if cycle-accurate equivalence to the original is required.
- If correctness is preferred over equivalence, the fix must align early RF capture timing with the PTW data source and delay `tlb_cache_valid` assertion until the correctly captured payload is written.
- Any intentional divergence from the original should be documented explicitly in the module README and cosim expectations.
