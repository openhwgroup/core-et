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

- Add the shire cache DFT MBIST enable hookup at `shirecache_top` and route it
  to `shirecache_bank.dft_mbist_en_i`.
  Finding: the original `shire_cache` exposed `dft__mbist_en`, and the
  translated `shirecache_bist_wrapper` still consumes the corresponding enable.
