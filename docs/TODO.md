# TODO

## Minion / Intpipe

- Add focused unit tests under `hw/ip/minion/intpipe/dv/`.
  Finding: the intpipe leaf modules and `intpipe_top` have standalone cosims in
  `dv/rtlcosim/`, but there is no local unit-test subtree for fast,
  expected-value checks. Good first targets:
  - `intpipe_alu`
  - `intpipe_int_scoreboard`
  - `intpipe_fp_scoreboard`
  - `intpipe_mask_scoreboard`
  - `debug_breakpoint`
  - `intpipe_csr_replay`
  - `intpipe_csr_msgs`

## Shire Cache

- Add the intended free-running clock integration at `shirecache_top` and route
  it to `shirecache_bank.clk_free_i`.
  Finding: `shirecache_top` currently ties `clk_free_i` to `clk_i`, while the
  bank-level trace/perfmon path keeps a separate free-running-clock contract.

- Add the shire cache DFT SRAM clock hookup at `shirecache_top` and route it to
  `shirecache_bank.dft_sram_clk_i`.
  Finding: the original `shire_cache` exposed `dft__sram_clock` plus
  `dft__clk_override`, and the translated RAM wrappers already consume
  `dft_sram_clk_i` with `dft_i.sram_clk_override`.

- Add the shire cache DFT MBIST enable hookup at `shirecache_top` and route it
  to `shirecache_bank.dft_mbist_en_i`.
  Finding: the original `shire_cache` exposed `dft__mbist_en`, and the
  translated `shirecache_bist_wrapper` still consumes the corresponding enable.
