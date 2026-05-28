# prim_fifo_semisync_hiv

ECP5-specific implementation of `prim_fifo_semisync_hiv`.

Why this exists
- The project keeps the HV/LV semi-synchronous FIFO seam explicit even on FPGA.
- ECP5 targets should not fall back implicitly to another technology tree.

How this implementation realizes the contract
- The body is the same logical pulse/credit/read-domain FIFO structure as the generic model.
- It preserves the original 1:1-clock semi-synchronous contract and optional relay parameters.
- `dft_hv_i` controls the write/HV reset bypass and `dft_lv_i` controls the read/LV reset bypass.

Why it is implemented this way
- No ECP5 hard macro is required for this FIFO today.
- Keeping an ECP5-owned implementation point leaves room for future FPGA-specific mapping while keeping all approximations behind the primitive seam.

RTL: `rtl/prim_fifo_semisync_hiv.sv`
