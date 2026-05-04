# prim_mul_div

ECP5 ownership point for `prim_mul_div`.

Current status:
- ECP5 now has its own RTL body at `rtl/prim_mul_div.sv`
- the ECP5 body is intentionally FF-based and does not reuse the generic
  latch/phase-pair implementation

Contract:
- keeps the public `intpipe_mul_div_top` request/response interface unchanged
- preserves request-side backpressure through `req_ready`
- preserves one-cycle-early writeback reservation through `resp_valid_early`
- preserves destination/data return through `resp_dest` and `resp_data`

Intentional divergence from generic/original:
- the ECP5 body replaces the original local gated-clock / two-phase latch
  microarchitecture with a simple edge-triggered implementation
- the ECP5 body fully squashes killed operations instead of preserving the
  original stale-response-after-kill bug

Why:
- this isolates FPGA bring-up from the latch-heavy mul/div timing problem
- it lets ECP5 use an implementation that is structurally appropriate for the
  OSS ECP5 flow while keeping the surrounding intpipe contract stable
