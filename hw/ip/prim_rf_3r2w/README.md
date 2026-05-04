# prim_rf_3r2w

Latch-timed 3R2W register-file primitive used by translated CORE-ET VPU
blocks.

Replaces: `rf_latch_3r_2w` from the CORE-ET source tree.

Implementation selection:
- `mk/prim.mk` exposes this primitive as `PRIM_RF_3R2W`
- Current implementations live under `tech_generic/prim_rf_3r2w` and
  `tech_ice40/prim_rf_3r2w` and `tech_xilinx/prim_rf_3r2w`

## Contract

- Each write port is phase-split: falling-edge data capture, rising-edge
  commit.
- Read ports A, B, and C are combinational.
- If both write ports target the same entry on the same rising edge, write
  port A wins, matching the original `rf_latch_3r_2w` ordering.
- RF contents are not reset.

## Current users

- `hw/ip/minion/vpu/rtl/vpu_rf.sv` (next translated VPU dependency)

## Differences from CORE-ET

| Aspect | Original | Reimplementation | Rationale |
|--------|----------|------------------|-----------|
| Module name | `rf_latch_3r_2w` | `prim_rf_3r2w` | Repository primitive naming |
| `test_en` | Explicit port | Removed from public contract | DFT/clocking detail belongs in the target-specific implementation |
| Storage body | Latch array with gated clocks | Technology-selected RF primitive body | Keeps the visible 3R2W latch-timed contract while allowing target-specific remapping |

## Verification

- Unit test: `hw/ip/prim_rf_3r2w/dv/`
- RTL co-simulation: `dv/rtlcosim/prim_rf_3r2w/`
