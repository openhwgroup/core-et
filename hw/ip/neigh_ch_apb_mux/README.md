# neigh_ch_apb_mux

`neigh_ch_apb_mux` is the translated CORE-ET neighborhood debug/APB mux leaf.
It routes the registered BPAM/shire APB request either to a selected minion via
`bpam2minions`, to the neighborhood ESR block, to the shared I-cache APB port,
or to the TBOX APB port.

The IP block contains:

- `neigh_ch_apb_mux_pkg` — APB request/response structs and address-decode
  constants translated from `soc.vh`/`debug_defines.vh`.
- `bpam2minions` — registered APB bridge from a neighborhood APB request to one
  of the minion APB ports.
- `neigh_ch_apb_mux` — top APB mux leaf that registers the shire request,
  minion responses, TBOX request, and shire response exactly like the original.

## Parameters

`bpam2minions` exposes the same sizing parameters and defaults as the original
module with lowRISC-style names. `neigh_ch_apb_mux` overrides them to the live
neighborhood values (`17`/`13`/`64`/`8`), exactly like the original mux.

| Parameter | Default | Meaning |
|-----------|---------|---------|
| `AdInWidth` | `19` | Incoming APB address width default from original leaf |
| `AdOutWidth` | `10` | Outgoing APB address width default from original leaf |
| `DataWidth` | `32` | APB data width default from original leaf |
| `NrMinions` | `ShireCoopMinMaskSize` (`8`) | Minions in the neighborhood |

`neigh_ch_apb_mux` uses package constants and has no module parameters.

## Address decode

The translated package preserves the original `soc.vh` address fields:

- minion select: `apb_paddr[12:10]`
- minion register: `apb_paddr[8:0]`
- minion thread: `apb_paddr[9]`
- minion PP: `apb_paddr[14:13]`
- minion destination/debug bit: `apb_paddr[16]`
- minion-resource class: `apb_paddr[16:15] == 2'b00` or `2'b10`
- TBOX select inside neighborhood resources: `apb_paddr[14]`

The minion encoded address is `{dest, pp, thread, reg}`.

## Ports and protocol

`bpam2minions` takes a standard APB setup/access request from the BPAM side and
forwards it to a one-hot minion APB port. `req_apb_psel_i &&
!req_apb_penable_i` starts a request. The selected minion `psel`, encoded
address, write bit, and write data are registered; `penable` is asserted after
`minion_start_q`; and the BPAM response is registered when the selected minion
reports ready. `rsp_apb_pready_o` pulses for one cycle after minion completion.

`neigh_ch_apb_mux` uses packed structs matching original APB field names for
shire/ESR/Icache/TBOX APB traffic. It registers:

- the incoming shire request (`apb_req_from_shire_i`)
- the shire response (`apb_rsp_to_shire_o`)
- the outgoing TBOX request (`apb_req_struct_tbox_o`)
- the one-cycle ESR response-valid gate
- all minion response inputs before the internal `bpam2minions`

## Integration notes

List sources in this order. `etlink_pkg.sv` must be compiled before
`minion_pkg.sv` because `minion_pkg` references `etlink_pkg::SourceSize`:

1. `hw/ip/etlink/rtl/etlink_pkg.sv`
2. `hw/ip/minion/rtl/minion_pkg.sv`
3. `hw/ip/esr/rtl/esr_pkg.sv`
4. `hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux_pkg.sv`
5. `hw/ip/neigh_ch_apb_mux/rtl/bpam2minions.sv`
6. `hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux.sv`

The TBOX request remains registered, while ESR and I-cache requests are purely
combinational from the registered shire request, matching the original module.

## Differences from original CORE-ET

| Difference | Rationale |
|------------|-----------|
| `clock`/`reset` become `clk_i`/active-low `rst_ni` | Repository reset/naming convention; the original reset domain is preserved |
| `soc.vh` macros become package constants and typedefs | Avoid synthesizable preprocessor constants and document address fields |
| Original APB typedef names become lower-snake package typedefs | lowRISC naming convention while preserving field names/order and widths |
| Flip-flop macros become explicit `always_ff` blocks | Repository style requirement |
| Original bitwise-OR minion-resource decode is preserved | Strict equivalence; no silent rewrite to a different expression |

No intentional functional divergence from the original `bpam2minions` or
`neigh_ch_apb_mux` is introduced.
