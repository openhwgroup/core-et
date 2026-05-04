# prim_phase_pair_hi_lo

Named composite for a high-phase capture followed by a low-phase capture when
both phase outputs remain live in shared RTL.

Use this instead of repeating a raw `rlatch` -> `rlatchn` chain when the
owning block is really expressing a semantic two-phase handoff rather than an
isolated helper latch. The enable ports preserve the original phase-local gate
plus latch behavior directly inside this self-contained primitive; tying both
enables high gives the always-open direct pair case.

Typical examples:

- a ping-pong accumulator state pair
- a high-to-low status handoff
- any repeated high-to-low pair where both intermediate and final values are
  architecturally live inside the translated block

## Parameters

| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `Width` | `int unsigned` | 1 | Data width |

## Ports

| Port | Direction | Width | Description |
|------|-----------|-------|-------------|
| `clk_i` | input | 1 | Root clock |
| `hi_en_i` | input | 1 | Enable for the high-phase stage |
| `lo_en_i` | input | 1 | Enable for the low-phase stage |
| `d_i` | input | `Width` | Input to the high-phase stage |
| `q_hi_o` | output | `Width` | High-phase stage output |
| `q_lo_o` | output | `Width` | Low-phase stage output |

## Tests

```bash
make -C dv test
ORIG_ROOT=/path/to/etcore-soc make -C ../../../../dv/rtlcosim/prim_phase_pair_hi_lo test
```
