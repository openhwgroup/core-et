# prim_fifo_semisync_hiv

Generic/default implementation of `prim_fifo_semisync_hiv`.

## System role

`prim_fifo_semisync_hiv` is the semi-synchronous voltage-crossing FIFO for transfers written in the high-voltage domain and consumed in the low-voltage domain. It replaces CORE-ET `vcfifo_wr_hiv_ss` sites in the neighborhood crossing logic.

## Contract

- Write-side `push_i`, `wdata_i`, and `ready_o` are synchronous to `clk_wr_i`.
- Read-side `pop_i`, `valid_o`, and `rdata_o` are synchronous to `clk_rd_i`.
- The write and read clocks are expected to be 1:1 frequency clocks with a controlled phase relationship, matching the original `_ss` FIFO contract.
- `Depth` is the original `NUM_ELEMS` parameter. The internal credit/destination FIFO depth is `Depth + PushRelay + PopRelay`.
- `PushRelay` and `PopRelay` preserve the optional original relay stages. The current neighborhood uses `Depth=3`, `PushRelay=0`, and `PopRelay=0`.
- Functional reset of either side resets both sides through per-domain reset synchronizers. In scan mode, `dft_hv_i` controls the write/HV side reset bypass and `dft_lv_i` controls the read/LV side reset bypass.
- The generic model is logical RTL only. It does not model physical level shifter delay or metastability.

## Why this is separate from `prim_fifo_async_hiv`

The CORE-ET `_ss` FIFO is not a Gray-pointer asynchronous FIFO. It transfers one-cycle push/pop pulses across a controlled 1:1 clock relationship and uses a destination `gen_fifo_reg` plus write-side credits. Its latency and reset-visible handshake differ from `vcfifo_wr_hiv_gcd` / `prim_fifo_async_hiv`, so it has a separate primitive seam.

RTL: `rtl/prim_fifo_semisync_hiv.sv`
