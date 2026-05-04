# prim_ram_2p

iCE40-specific implementation of `prim_ram_2p`.

Why this exists
- The generic symmetric two-port model does not infer iCE40 EBRs under Yosys.
- In the current system, every real `prim_ram_2p` user is a single-clock 1W1R RAM, so the iCE40 body should match that hardware shape directly.

How this implementation realizes the contract
- Port A is the write side and port B is the read side.
- The storage array carries `(* ram_style = "block" *)` so Yosys can infer `SB_RAM40_4K` tiles from the simple-dual-port pattern.
- Port B read data is registered and holds its previous value when `b_req_i=0`, matching the current wrapper contract used by the system.
- Port A readback is tied off because the iCE40 body does not implement the unused symmetric read capability.

Why it is implemented this way
- iCE40 EBR is a simple-dual-port block RAM: one read port and one write port, not a general symmetric TDP macro.
- Matching that shape in the primitive is the simplest way to keep the current minion, shirecache, rbox, and FIFO-SRAM users on hard RAM.
- Unsupported port usage is called out explicitly with simulation-time assertions instead of pretending the target can realize a broader contract efficiently.

RTL: `rtl/prim_ram_2p.sv`
