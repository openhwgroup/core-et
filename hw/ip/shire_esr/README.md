# Shire ESR/APB

`shire_esr` contains the compute-shire system-register and APB fanout blocks that sit at the shire-channel boundary. The block family translates the original CORE-ET shire ESRs into project-native SystemVerilog and typed APB/control structs.

## Modules

| Module | Original CORE-ET block | Purpose |
|--------|-------------------------|---------|
| `apb_esr_ff` | `apb_esr_ff` + `apb_ff` | Per-slave APB request/response skid/flop stage preserving the original one-transaction state machine. |
| `esr_cache_bank` | `esr_cache_bank` | APB register block for one shire-cache bank; drives `shirecache_pkg::bank_esr_ctl_t` and consumes `bank_esr_status_t`. |
| `esr_shire_other` | `esr_shire_other` | Shire-level APB register block for minion/thread controls, interrupts, power controls, PLL/DLL controls, cache RAM config, ICache prefetch, debug run-control, and ECO fields. |
| `shire_esr_pkg` | original ESR generated constants/types | Address constants and packed native structs shared by the shire ESR modules. |

## Parameters

`apb_esr_ff`:

- `NumInterfaces`: number of independent APB slave lanes. Defaults to the compute-shire shire ESR fanout width.

`esr_cache_bank`:

- `AdWidth`: APB address width presented to this bank ESR block.
- `SetsPerSubBank`: cache geometry used to compute original default set ranges and masks.
- `NumReqqEntries`: request-queue depth used to compute original default L3 REQQ allocation.

`esr_shire_other`:

- `NumBanks`: number of shire-cache banks whose error/status bits are reported.
- `NumNeigh`: number of neighborhoods / ICache prefetch lanes / TBOX run-control lanes.
- `AdWidth`: APB address width presented to this shire-level ESR block.

## Port protocol

All three blocks use the original CORE-ET two-phase APB timing:

1. setup phase: `psel=1`, `penable=0`; read data/address-valid are captured for the following access phase;
2. access phase: `psel=1`, `penable=1`; writes update state and reads return the setup-phase value;
3. `pready` is combinationally `1` for the ESR blocks, while `apb_esr_ff` preserves the original request/response delay state machine.

Reset domains are preserved from the original:

- `rst_c_ni`: cold/control reset for APB address-valid and selected shire-level state.
- `rst_w_ni`: warm reset for most programmable ESR state.
- `rst_d_ni`: debug reset for debug/run-control state where the original used `reset_d`.
- In `esr_cache_bank`, the trace address-enable and trace-control registers reset from `rst_d_ni`; the trace address-value register intentionally has no reset, matching the original generated ESR write sequence.

## Integration notes

- Functional RTL should instantiate `esr_cache_bank` once per cache bank and connect `esr_ctl_o`/`esr_status_i` directly to `shirecache_pkg::bank_esr_ctl_t` / `bank_esr_status_t`.
- `esr_shire_other` owns the APB-programmable RAM configuration and emits `ram_cfg_pkg::ram_cfg_t` for cache/RBOX/ICache RAM wrappers. Normal simulation/FPGA operation uses the reset value unless firmware programs timing fields.
- DFT control is consolidated into `dft_pkg::dft_t`; RAM read/write inhibits are ORed with the programmed RAM config path before driving `ram_cfg_o`-consuming wrappers.
- Use `apb_esr_ff` between the shire APB fanout and individual ESR slaves when matching the original shire-channel timing.

## Intentional differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| Active-low `rst_*_ni` ports replace active-high `reset_*`. | Project reset convention; reset-domain separation is preserved. |
| `dft_pkg::dft_t` replaces scattered DFT/control wires. | Project DFT abstraction. |
| `ram_cfg_pkg::ram_cfg_t` replaces `esr_shire_cache_ram_cfg_t`. | Project RAM wrapper contract for FPGA/ASIC portability. |
| Vendor/debug/sensor-only surfaces are omitted or tied to native zero/status values. | The compute-shire contract excludes third-party UltraSoC/sensor/hard-macro surfaces. |
| Cache-bank trace source comparison uses the native `shirecache_pkg::ReqqSourceSize` low bits. | Native cache request-source width is narrower than the original generated ESR field. |
| `esr_shire_other` minion feature output uses `minion_pkg::esr_minion_features_t`. | Native packed struct type while preserving reset/write bits observed in cosim. |

See `doc/rbox_esr_compat.md` for the RBOX APB compatibility audit required by the compute-shire ESR/APB task.

## Verification

Unit tests live in `hw/ip/shire_esr/dv/` and are auto-discovered by top-level `make test`.

Targeted commands:

```sh
make -C hw/ip/shire_esr/dv test
ORIG_ROOT=/path/to/et-soc make -C dv/rtlcosim/apb_esr_ff test
ORIG_ROOT=/path/to/et-soc make -C dv/rtlcosim/esr_cache_bank test
ORIG_ROOT=/path/to/et-soc make -C dv/rtlcosim/esr_shire_other test
```
