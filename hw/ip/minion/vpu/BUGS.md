# Minion VPU original repository bugs

## Original repository bug: `vpu_txfma_trans_top` fake-TXFMA branch does not elaborate

Affected original modules:
- `rtl/shire/minion/vpu/vpu_txfma_trans_top.v`
- `rtl/shire/minion/vpu/txfma_top_fake.v`

Affected reimplemented modules:
- `hw/ip/minion/vpu/rtl/txfma_top_fake.sv`
- A future translated `vpu_txfma_trans_top.sv` parent will also need the same
  fake-TXFMA boundary decision when it is imported.

Symptom:
- In the original fake-TXFMA build branch, `vpu_txfma_trans_top.v` instantiates
  `txfma_top_fake` with real-TXFMA-style `.out_data_res(...)` and
  `.out_comp_res(...)` connections.
- The original `txfma_top_fake.v` exposes only `output vpu_output_t out_data`
  and has no compare-result output.
- The original fake-TXFMA branch therefore does not elaborate as written.

Root cause:
- `rtl/shire/minion/vpu/vpu_txfma_trans_top.v` shares one named-port
  instantiation template between the real `txfma_top` and fake
  `txfma_top_fake` implementations.
- `rtl/shire/minion/vpu/txfma_top_fake.v` kept the older single-output fake
  model boundary instead of the real top's `out_data_res` / `out_comp_res`
  contract.

Reachability:
- Reachable whenever the original build enables the fake-TXFMA configuration.
- The default real-TXFMA path is not affected, so the bug is latent unless that
  alternate fake branch is compiled or cosimmed.

System impact:
- The original fake-TXFMA top-level integration path cannot be used as a
  buildable reference configuration without adapting the boundary.
- Direct cosim of a future translated fake `vpu_txfma_trans_top` against the
  original parent branch will require an adapted original composition instead of
  instantiating the broken original parent as-is.

Reimplementation decision:
- The current reimplementation intentionally diverges for the imported fake leaf
  boundary: `txfma_top_fake.sv` uses the real-TXFMA-style
  `out_data_res` / `out_comp_res` port contract, with `out_comp_res` hardwired
  low.
- The standalone `txfma_top_fake` cosim compares all current translated outputs
  against the original fake leaf by adapting the original single `out_data`
  output and treating the missing original compare output as constant zero.
