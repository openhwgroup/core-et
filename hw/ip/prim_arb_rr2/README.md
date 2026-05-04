# prim_arb_rr2

Dual-grant round-robin arbiter with registered outputs.

Selects **two distinct entries** from a set of requestors using round-robin
fairness. Used by `reqq_alloc` to select primary + paired entries
simultaneously.

## Parameters

| Name     | Default | Description             |
|----------|---------|-------------------------|
| NumReqs  | 64      | Number of requestors    |

## Ports

| Port          | Direction | Width              | Description                        |
|---------------|-----------|--------------------|------------------------------------|
| clk_i         | input     | 1                  | Clock                              |
| rst_ni        | input     | 1                  | Active-low async reset             |
| reqs_i        | input     | NumReqs            | Free entries (request bitmap)      |
| alloc_i       | input     | 1                  | Advance RR pointer                 |
| esr_wr_i      | input     | 1                  | Re-evaluate grants                 |
| valid_o       | output    | 1                  | At least 1 free entry              |
| valid2_o      | output    | 1                  | At least 2 free entries            |
| grants_o      | output    | NumReqs            | Primary grant (one-hot, registered)|
| grants2_o     | output    | NumReqs            | Secondary grant (one-hot, reg.)    |
| grants_id_o   | output    | clog2(NumReqs)     | Primary binary ID (combinational)  |
| grants2_id_o  | output    | clog2(NumReqs)     | Secondary binary ID (combinational)|

## Behavior

1. Maintains a round-robin pointer via an internal `prim_arb_rr` instance.
2. Filters `reqs_i` to exclude currently granted entries, producing `reqs_valid`.
3. The primary grant is the RR winner from `reqs_valid`.
4. The secondary grant is chosen by LSB/MSB priority encoding: the MSB-priority
   result is preferred when it differs from the primary, otherwise the
   LSB-priority result is used.
5. Registered outputs update when `alloc_i | ~valid_o | ~valid2_o | esr_wr_i`.
6. Binary IDs are combinational one-hot-to-binary of the registered grant vectors.

## Origin

Replaces `arb_rr2` + `arb_grants2` + `hot2bin` + `arb_rr` + `onehot_mux`
from the CORE-ET source tree.
