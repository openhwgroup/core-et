# prim_fifo_async_hiv / prim_fifo_async_lov

Asynchronous CDC FIFOs for clock domain crossing between voltage domains.

These are **technology primitives** — implementations live in per-module technology directories such as `hw/ip/tech_generic/prim_fifo_async_hiv/rtl/` and will differ per target technology (ASIC implementations use foundry-specific level shifters and CDC cells).

| Module | Write domain | Read domain | Replaces |
|--------|-------------|-------------|----------|
| `prim_fifo_async_hiv` | High-voltage (e.g. shire cache) | Low-voltage (e.g. NOC) | `vcfifo_wr_hiv_gcd` |
| `prim_fifo_async_lov` | Low-voltage (e.g. NOC) | High-voltage (e.g. shire cache) | `vcfifo_wr_lov_gcd` |

The only functional difference is which DFT domain signal (`dft_hv_i` vs `dft_lv_i`) maps to the write vs read reset bypass. On ASIC, the level shifter placement also differs.

## Parameters

| Parameter | Default | Description |
|-----------|---------|-------------|
| `Width` | 32 | Data width |
| `Depth` | 8 | Number of entries |
| `SyncStages` | 2 | CDC synchronizer depth |

## Ports

| Port | Direction | Description |
|------|-----------|-------------|
| `clk_wr_i` / `rst_wr_ni` | input | Write clock and reset |
| `push_i` / `wdata_i` / `ready_o` | in/in/out | Write interface |
| `clk_rd_i` / `rst_rd_ni` | input | Read clock and reset |
| `pop_i` / `rdata_o` / `valid_o` | in/out/out | Read interface |
| `dft_hv_i` / `dft_lv_i` | input | Per-domain DFT control |

## Differences from the original CORE-ET modules

| Original | Reimplementation | Rationale |
|----------|------------------|-----------|
| Active-high `reset_push` / `reset_pop` | Active-low `rst_wr_ni` / `rst_rd_ni` | Project reset naming/style convention |
| Individual `dft__reset_byp_*` / `dft__reset_*` ports | `dft_pkg::dft_t dft_hv_i` / `dft_lv_i` | Consolidated DFT surface; scan reset remains per voltage domain |
| Physical level shifter wrappers | Technology primitive boundary | Generic/FPGA models are logical CDC transports; ASIC implementation is the level-shifter insertion point |
| CORE-ET GCD counter wrappers | Portable Gray-pointer FIFO in generic/ECP5 RTL | Preserves show-ahead FIFO behavior, reset masking, and DFT reset-bypass contract in open-source simulation/FPGA flows |

During a functional or scan reset of a domain, `ready_o` and `valid_o` are
masked low to match the original reset-visible handshake behavior. `rdata_o` is
show-ahead storage data and is only meaningful when `valid_o=1`.

## Tests

The shared DV directory builds standalone unit tests for both variants:

```bash
make -C hw/ip/prim_fifo_async_hiv/dv test
```

Current standalone unit coverage:

| Variant | Unit checks | Notes |
|---------|-------------|-------|
| `prim_fifo_async_hiv` | 30 | Reset, fill/drain, and interleaved push/pop |
| `prim_fifo_async_lov` | 63 | Reset masking, independent clocks, fill/drain, interleaved push/pop, and HV/LV DFT reset-bypass mapping |

Standalone RTL cosims live under `dv/rtlcosim/prim_fifo_async_hiv/` and
`dv/rtlcosim/prim_fifo_async_lov/` and compare `ready_o`, `valid_o`, and
`rdata_o` against `vcfifo_wr_hiv_gcd` / `vcfifo_wr_lov_gcd`.
