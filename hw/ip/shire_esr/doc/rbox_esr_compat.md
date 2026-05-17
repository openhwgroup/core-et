# RBOX ESR/APB compatibility audit

This audit covers the current native `rbox_top` / `rbox_esr` APB path against the original shire-channel RBOX APB routing.

## Original shire-channel behavior

Evidence:

- `rtl/shire/shire_channel/orig/shire_channel.v:325-332` defines APB slave ordering:
  - banks: `[0, NUM_BANKS)`;
  - shire-level ESR (`esr_shire_other`): `NUM_BANKS`;
  - RBOX: `NUM_BANKS + 1` through `NUM_BANKS + NUM_RBOX`;
  - ICache memory APB after RBOX.
- `rtl/shire/shire_channel/orig/shire_channel.v:386-395` places `apb_esr_ff` between the parent APB fanout and every shire ESR slave lane.
- `rtl/shire/shire_channel/orig/shire_channel.v:441-449` adapts the selected RBOX lane by passing through `pwrite`, `psel`, `penable`, and `pwdata`, truncating address to ``RBOX_SHIRE_APB_ADDR_WIDTH``.
- `rtl/shire/rbox/rbox_dbg_ram.v:63,218-219` uses the top RBOX-local address bit (original bit 16) to route debug-RAM accesses versus ESR accesses.
- `rtl/shire/esr/esr_rbox.v:96-134` implements the RBOX ESR APB slave as a two-phase block with `pready=1`, setup-phase read capture, setup-phase write-data capture, and `pslverr=!s1_addr_valid`.
- `rtl/shire/esr/esr_rbox.v:155-179` generates one-cycle write-enable outputs one cycle after a non-zero write to each writable RBOX ESR.

## Native implementation audit

| Item | Native status |
|------|---------------|
| Shire APB lane ordering | The compute-shire contract keeps `rbox_top` as a separate APB slave behind the shire APB fanout. `apb_esr_ff` preserves the original per-lane request/response delay before downstream adapters. |
| RBOX-local ESR register map | `hw/ip/rbox/rtl/rbox_esr.sv` keeps addresses `0x0` through `0x7` for config, input/output buffer, status, start, and consume registers. |
| RBOX ESR APB timing | `rbox_esr` captures read data and write data in setup phase, returns captured read data in access phase, keeps `pready=1`, and drives `pslverr` from the captured setup decode result. |
| Write-enable pulses | `rbox_esr` emits the same delayed, non-zero-write-qualified enables as the original custom statements. |
| RBOX top response qualification | `hw/ip/rbox/rtl/rbox_top.sv:182-190` qualifies the ESR response `pready` with `psel && penable` before feeding `rbox_dbg_ram`, matching the original top-level mux expectation. |
| Debug-RAM versus ESR address split | Native `rbox_dbg_ram` uses the high bit of `apb_pkg::req_t.paddr` (`DefaultAddrWidth-1`, bit 15) as the debug flag because the project APB struct is 16 bits wide. The original used bit 16 of a 17-bit RBOX-local address. This is an existing documented native-address-width difference at the RBOX boundary. |
| Reset domains | Native `rbox_top` preserves original RBOX domain separation: debug reset for the debug/run-control path and warm reset for the ESR/control path. |

## Conclusion

No shire-ESR RTL fix is required by this audit. The retained RBOX ESR address map, APB setup/access timing, response semantics, and delayed write-enable pulse semantics match the original behavior used by the shire-channel APB fanout.

The only noted difference is the existing native RBOX debug-RAM address-width compression (`apb_pkg::DefaultAddrWidth == 16` versus original ``RBOX_SHIRE_APB_ADDR_WIDTH == 17``). That difference is already localized inside `rbox_dbg_ram` and does not affect the low-address RBOX ESR register map audited for this task. A future compute-shire top-level integration that needs original bit-16 debug-RAM addresses should either widen `apb_pkg::req_t.paddr` for the RBOX path or add an explicit shire-channel adapter; no such change is needed for the ESR/APB blocks delivered here.
