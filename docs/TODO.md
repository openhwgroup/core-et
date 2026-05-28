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
