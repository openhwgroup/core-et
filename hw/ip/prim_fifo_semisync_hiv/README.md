# prim_fifo_semisync_hiv / prim_fifo_semisync_lov

Semi-synchronous voltage-crossing FIFOs for the neighborhood HV/LV boundary.

These are **technology primitives**. Implementations live under `hw/ip/tech_*` and are selected with `mk/prim.mk`.

| Module | Write domain | Read domain | Replaces |
|--------|--------------|-------------|----------|
| `prim_fifo_semisync_hiv` | High-voltage | Low-voltage | `vcfifo_wr_hiv_ss` |
| `prim_fifo_semisync_lov` | Low-voltage | High-voltage | `vcfifo_wr_lov_ss` |

## Behavior

The primitive preserves the CORE-ET `_ss` FIFO contract: it is for 1:1 write/read clocks with a controlled phase relationship, not an arbitrary asynchronous CDC FIFO. It forwards accepted write pulses and payloads across the voltage boundary, buffers them in a read-domain registered FIFO, and returns pop pulses as write-side credits.

The neighborhood currently uses this primitive for all original `_ss` instances with `Depth=3`, `PushRelay=0`, and `PopRelay=0`.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `Width` | 32 | Payload width (`ELEM_SIZE` in the original). |
| `Depth` | 3 | Original `NUM_ELEMS`; minimum original neighborhood value is 3. |
| `PushRelay` | 0 | Optional original extra relay stage on the push side. |
| `PopRelay` | 0 | Optional original extra relay stage on the pop side. |

Effective internal depth is `Depth + PushRelay + PopRelay`, matching the original `vcfifo_wr_pd_ss` / `vcfifo_rd_pd_ss` implementation.

## Ports

| Port | Direction | Description |
|------|-----------|-------------|
| `clk_wr_i`, `rst_wr_ni` | input | Write-domain clock and active-low functional reset. |
| `push_i`, `wdata_i`, `ready_o` | input/input/output | Write interface. Data is accepted when `push_i && ready_o`. |
| `clk_rd_i`, `rst_rd_ni` | input | Read-domain clock and active-low functional reset. |
| `pop_i`, `rdata_o`, `valid_o` | input/output/output | Read interface. Data is consumed when `pop_i && valid_o`. |
| `dft_hv_i`, `dft_lv_i` | input | Per-voltage-domain DFT reset-bypass controls. |

Functional reset of either side resets both internal domains through the same two-stage asynchronous-assert/synchronous-deassert reset synchronizer used by the original `sync_rst`. In scan mode the domain-local DFT reset bypass controls only that domain, matching the original `dft__reset_byp_*` / `dft__reset_*` ports.

## Differences from the original CORE-ET modules

| Original | Reimplementation | Rationale |
|----------|------------------|-----------|
| Active-high `reset_push` / `reset_pop` | Active-low `rst_wr_ni` / `rst_rd_ni` | Project reset naming convention. |
| Individual `dft__reset_byp_push`, `dft__reset_push`, `dft__reset_byp_pop`, `dft__reset_pop` ports | `dft_pkg::dft_t dft_hv_i` / `dft_lv_i` | Consolidated DFT surface; scan reset polarity remains active-low inside `dft_t`. |
| Physical `vclevel_shft_h2l` / `vclevel_shft_l2h` wrappers | Technology primitive boundary | Generic/FPGA models are logical transports; ASIC implementations are the level-shifter insertion point. |
| `vcfifo_wr_*_ss` plus private `vcfifo_wr_pd_ss` / `vcfifo_rd_pd_ss` submodules | Single self-contained primitive | Keeps the translated block boundary narrow while preserving the same pulse, credit, FIFO, reset, and DFT behavior. |

## Usage notes

Use these primitives only for the original `_ss` neighborhood crossings or another path with the same controlled 1:1 clock relationship. Use `prim_fifo_async_hiv` / `prim_fifo_async_lov` for GCD-style arbitrary asynchronous crossings.

The signed-off neighborhood shape is `Depth=3`, `PushRelay=0`, and `PopRelay=0`. The relay parameters are preserved for interface compatibility with the original modules; enabling them in a future translated site requires extending the primitive unit tests and standalone cosims for that configuration.

## Tests

The shared DV directory builds unit tests for both variants:

```bash
make -C hw/ip/prim_fifo_semisync_hiv/dv test
```

Standalone RTL cosims live under:

- `dv/rtlcosim/prim_fifo_semisync_hiv/`
- `dv/rtlcosim/prim_fifo_semisync_lov/`

They compare `ready_o`, `valid_o`, and `rdata_o` against `vcfifo_wr_hiv_ss` / `vcfifo_wr_lov_ss` every cycle.
