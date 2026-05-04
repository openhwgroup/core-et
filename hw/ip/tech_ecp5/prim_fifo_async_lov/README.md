# prim_fifo_async_lov

ECP5-specific implementation of `prim_fifo_async_lov`.

Why this exists
- The project keeps the LV/HV CDC seam explicit even on FPGA.
- ECP5 still needs its own implementation ownership point so targets do not
  fall back implicitly to another technology.

How this implementation realizes the contract
- The FIFO body is the same Gray-pointer asynchronous FIFO structure as the
  generic model.
- `dft_lv_i` and `dft_hv_i` still provide per-domain reset bypass.

Why it is implemented this way
- No ECP5-specific hard macro is required here today.
- The separate tech implementation keeps the contract explicit and leaves room
  for future target-specific refinements without changing functional RTL.

RTL: `rtl/prim_fifo_async_lov.sv`
