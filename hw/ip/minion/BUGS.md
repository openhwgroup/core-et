# BUGS

## Original Repository Bug: `intpipe_mul_div_top` kill does not fully squash the response path

### Affected modules

- Original RTL (`ORIG_ROOT` relative):
  - `rtl/shire/minion/intpipe/intpipe_mul_div_top.v`
  - `rtl/shire/minion/intpipe/intpipe_mul_div_ctl.v`
- Reimplementation:
  - [intpipe_mul_div_top.sv](intpipe/rtl/intpipe_mul_div_top.sv)
  - [tech_generic/prim_mul_div.sv](../tech_generic/prim_mul_div/rtl/prim_mul_div.sv)

### Symptom

A killed in-flight mul/div operation can still produce a later `resp_valid`
pulse before the next recovery operation completes.

This is reproducible in the primitive-based leaf harness
[test_mul_div.sv](../../../fpga/test_mul_div/rtl/test_mul_div.sv),
where the generic/default body reports:

```text
K15 F=10 W=1 P=0 G=000000000000004D E=000000000000004D D=33 N=0
TEST MUL DIV FAILED
```

`G` and `E` match the recovery multiply result (`7 * 11 = 77`), but `N=0`
shows that a response from the killed operation escaped during the kill window.

### Root cause

In both the original and translated control RTL, `kill` only feeds the
loop/phase early-exit path:

- Original: `early_out_2d = kill || ...` at `intpipe_mul_div_ctl.v:164`
- Reimplementation: `early_out_2d = kill || ...` at
  [intpipe_mul_div_ctl.sv:147](../tech_generic/prim_mul_div/rtl/intpipe_mul_div_ctl.sv#L147)

That early-out is used to clear the loop counter and stop divide/multiply phase
progression:

- Original loop counter clear at `intpipe_mul_div_ctl.v:172`
- Reimplementation loop counter clear at
  [intpipe_mul_div_ctl.sv:156](../tech_generic/prim_mul_div/rtl/intpipe_mul_div_ctl.sv#L156)

But the response-valid pipeline is not explicitly killed:

- Original response generation at `intpipe_mul_div_ctl.v:473-482`
- Reimplementation response generation at
  [intpipe_mul_div_ctl.sv:587](../tech_generic/prim_mul_div/rtl/intpipe_mul_div_ctl.sv#L587)
  through
  [intpipe_mul_div_ctl.sv:598](../tech_generic/prim_mul_div/rtl/intpipe_mul_div_ctl.sv#L598)

`resp_valid_early` is derived from `mul_op_2p`, `loop_cnt_2p`,
`rem_adj_ph_2p`, and held `resp_valid_2q`, but not gated by `kill`, and
`resp_valid_2q` is only cleared by reset. As a result, a late response from
the killed operation can still be observed after `kill` is asserted.

### Reachability

Reachable with a direct leaf sequence:

1. Issue a long-running mul/div operation.
2. Let it advance for several cycles.
3. Assert `kill`.
4. Wait for `req_ready`.
5. Issue a recovery multiply.

The Ainekko generic/default reimplementation matches the original behavior on
this sequence.

### System impact

Pipeline kill/flush logic cannot rely on `kill` to fully squash an in-flight
mul/div response. A downstream consumer can observe a ghost result from an
operation that was architecturally killed.

### Reimplementation status

The generic/default reimplementation intentionally preserves the original
behavior for equivalence.

The ECP5-specific
[prim_mul_div](../tech_ecp5/prim_mul_div/rtl/prim_mul_div.sv)
intentionally diverges: it uses an FF-based replacement and fully squashes
killed operations instead of preserving the stale-response-after-kill bug.
