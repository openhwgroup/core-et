# prim_phase_pair_lo_hi

Named composite for a low-phase capture followed by a high-phase capture when
both phase outputs remain live in shared RTL.

Use this instead of repeating a raw `rlatchn` -> `rlatch` chain when the
owning block is really expressing a semantic two-phase handoff rather than an
isolated helper latch. The enable ports preserve the original phase-local gate
plus latch behavior directly inside this self-contained primitive; tying both
enables high gives the always-open direct pair case.

Typical examples:

- a two-phase loop counter
- a phase-local status handoff
- any repeated low-to-high pair where both intermediate and final values are
  architecturally live inside the translated block

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 1 | Data width |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Root clock |
| `lo_en_i` | input | 1 | Enable for the low-phase stage |
| `hi_en_i` | input | 1 | Enable for the high-phase stage |
| `d_i` | input | `Width` | Input to the low-phase stage |
| `q_lo_o` | output | `Width` | Low-phase stage output |
| `q_hi_o` | output | `Width` | High-phase stage output |

## Tests

```bash
make -C dv test
ORIG_ROOT=/path/to/etcore-soc make -C ../../../../dv/rtlcosim/prim_phase_pair_lo_hi test
```
