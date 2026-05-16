# Neighborhood HV/LV support logic

This IP block contains small CORE-ET neighborhood high-voltage support leaves
used by the later `neigh_hi_voltage_cross`, `neigh_lo_voltage_cross`,
`neigh_channel`, and `neigh_top` translations.  Each RTL module is a direct
SystemVerilog translation of one original `rtl/shire/neigh/neigh_hv_logic_*.v`
leaf.

## Modules

| Module | Original module | Function |
|--------|-----------------|----------|
| `neigh_hv_logic_apb_req` | `neigh_hv_logic_apb_req` | Generates an APB request FIFO push and suppresses repeated pushes while waiting for `pready`. |
| `neigh_hv_logic_apb_rsp` | `neigh_hv_logic_apb_rsp` | Gates an APB response payload with the response FIFO valid bit. |
| `neigh_hv_logic_bpam` | `neigh_hv_logic_bpam` | Pushes BPAM run-control updates when the run-control word changes or a debug write occurs. |
| `neigh_hv_logic_bpam_rc_tbox_ack` | `neigh_hv_logic_bpam_rc_tbox_ack` | Registers the TBOX BPAM halt/resume acknowledge payload in the HV domain. |
| `neigh_hv_logic_clock_feedback` | `neigh_hv_logic_clock_feedback` | Provides clock-feedback combinational cells: inverted shire clock, buffered neighborhood clock. |
| `neigh_hv_logic_constants` | `neigh_hv_logic_constants` | Drives a constant `1'b1`. |
| `neigh_hv_logic_icache_err` | `neigh_hv_logic_icache_err` | Toggle-enable flop that captures ICache error indication changes. |
| `neigh_hv_logic_ipi` | `neigh_hv_logic_ipi` | OR-reduces neighborhood thread IPI trigger bits into a FIFO push. |
| `neigh_hv_logic_l2_bank` | `neigh_hv_logic_l2_bank` | Converts 256-bit bank FIFO records into 512-bit Shire Cache requests, including two-half evict assembly. |
| `neigh_hv_logic_neigh_sc` | `neigh_hv_logic_neigh_sc` | Ready-qualified round-robin mux from bank/UC requests to the Shire Cache request vector. |
| `neigh_hv_logic_pwr_ctrl_tdr_isolate` | `neigh_hv_logic_pwr_ctrl_tdr_isolate` | Masks power-isolation control low when the DFT TDR control bit is asserted. |
| `neigh_hv_logic_uc_fcc` | `neigh_hv_logic_uc_fcc` | OR-reduces UC FCC bits into a FIFO push. |

The package `neigh_hv_logic_pkg` defines the small support-only types that were
previously pulled from `soc.vh`: `apb_from_neigh_t`, `bpam_rc_tbox_ack_t`, and
sizing constants for minions, threads, banks, and UC data.

## Parameters

| Parameter | Module(s) | Default | Meaning |
|-----------|-----------|---------|---------|
| `BpamRcSize` | `neigh_hv_logic_bpam` | `neigh_hv_logic_pkg::BpamRcSize` (4) | Width of the BPAM run-control word. |
| `ThreadsPerN` | `neigh_hv_logic_ipi` | `neigh_hv_logic_pkg::ThreadsPerN` (16) | Number of IPI trigger bits in one neighborhood. |
| `MinPerN` | `neigh_hv_logic_uc_fcc` | `neigh_hv_logic_pkg::MinPerN` (8) | Number of minion FCC bits in one neighborhood. |
| `ScBanks` | `neigh_hv_logic_neigh_sc` | `neigh_hv_logic_pkg::ScBanks` (4) | Number of Shire Cache bank request sources. |

## Port interface

All translated input ports use `_i`, output ports use `_o`, and original resets
are active-low async reset inputs (`rst_*_ni`) where present.

- APB leaves use `neigh_hv_logic_pkg::apb_from_neigh_t`, whose packed field
  order matches original `APB_from_neigh_t`: `apb_prdata`, `apb_pready`,
  `apb_pslverr`.
- BPAM TBOX acknowledge uses `neigh_hv_logic_pkg::bpam_rc_tbox_ack_t` with
  `halt_ack` and `resume_ack`.
- L2/SC request leaves use `etlink_pkg::req_nodata_t` for original
  `et_link_nodata_req_info_t` and `etlink_pkg::req_t` for original
  `et_link_req_info_t`.
- `neigh_hv_logic_neigh_sc` preserves the original ready-qualified grant
  contract: request bids are `valid & ready`, only one ready/valid bit is
  asserted by the round-robin arbiter, and `neigh_sc_req_info_o` is zero when
  no request is granted.

## Integration notes

Use these leaves inside the voltage-crossing wrappers rather than re-spelling
the original helper logic.  The modules do not instantiate voltage-crossing
FIFOs directly; crossing wrappers are responsible for connecting them to
`prim_fifo_semisync_*` / `prim_fifo_async_*` primitives as appropriate.

`neigh_hv_logic_clock_feedback` intentionally remains combinational.  It
translates original `et_combo_inv` / `et_combo_buf` cells into shared RTL
assignments; technology-specific don't-touch buffering belongs at integration or
backend constraint level, not inside these pure support leaves.

## Excluded original leaf: `neigh_hv_logic_fcc`

`rtl/shire/neigh/neigh_hv_logic_fcc.v` is not translated in this block because
it is not instantiated by the audited neighborhood hierarchy.  The relevant
filelists are:

- `rtl/shire/neigh/neigh_top_rtl.f`, which includes `neigh_channel_rtl.f`
- `rtl/shire/neigh/neigh_channel_rtl.f`, which uses `-y $RTLROOT/shire/neigh`

A source audit finds the live FCC support instantiation in
`rtl/shire/neigh/neigh_hi_voltage_cross.v` at the `neigh_hv_logic_uc_fcc`
instance (`neigh_hv_logic_uc_fcc_pd_hv`).  No instantiation of
`neigh_hv_logic_fcc` is present under `rtl/shire/neigh` or `rtl/shire/esr`.
The translated live leaf is therefore `neigh_hv_logic_uc_fcc`, which has the
same OR-reduction behavior but preserves the original live port names expected
by the crossing wrapper.

## Differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| Active-high reset ports (`reset_push`, `reset_hv`, `reset_pop`, `reset_shire_ndm`) are translated to active-low async reset ports (`rst_*_ni`). | Repository reset convention; each original reset domain remains separate. |
| Original `clock_*` ports are renamed to `clk_*_i`. | lowRISC naming convention. |
| `soc.vh` macro types are replaced by packages (`etlink_pkg`, `minion_pkg`, `shirecache_pkg`, `neigh_hv_logic_pkg`). | Project rule: packages instead of synthesizable `define`/include dependencies. |
| Original `arb_rr` in `neigh_hv_logic_neigh_sc` is translated to `prim_arb_rr`; the output request info is explicitly zeroed when no grant is active. | Standard primitive mapping while preserving the original non-held info mux behavior. |
| Original `et_combo_inv` / `et_combo_buf` in `neigh_hv_logic_clock_feedback` are plain combinational assignments. | These cells behave as inverter/buffer in non-foundry RTL; backend don't-touch behavior is not a functional shared-RTL difference. |
| `neigh_hv_logic_fcc` is excluded; `neigh_hv_logic_uc_fcc` is translated. | `neigh_hv_logic_fcc` is unreachable in the audited filelists; see the exclusion note above. |

## Verification

The grouped DV test `hw/ip/neigh_hv_logic/dv` instantiates every translated leaf
and checks directed APB, BPAM, clock-feedback, ICache-error, IPI/FCC,
power-isolation, L2-bank, and Shire-Cache mux behavior.

Each translated module also has an individual standalone RTL cosim under
`dv/rtlcosim/<module>/` comparing all outputs against the original CORE-ET leaf.
