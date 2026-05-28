// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

# neigh_channel_routing BUGS

## Original repository bug: `neigh_tbox_router` routes a TBOX neighborhood request using stale FIFO data on empty-to-nonempty transitions

- **Original module:** `/home/glguida/ainekko/et-soc/rtl/shire/neigh/neigh_tbox_router.v`
- **Reimplemented module:** `hw/ip/neigh_channel_routing/rtl/neigh_tbox_router.sv`
- **Symptom:** The first `tbox_neigh_req_info` entry accepted after the request FIFO has been empty can assert `tbox_sc_req_valid` or `tbox_min_rsp_valid` according to the previously exposed `tbox_neigh_req_pop_data.opcode`, while the corresponding output payload fields (`tbox_sc_req_info` or `tbox_min_rsp_info`) reflect the newly accepted request. In a directed sequence where a non-message request is followed by a `ET_LINK_REQ_MsgSendData` request, both the original and the reimplementation assert the SC-request valid for the message entry.
- **Root cause:** The original computes `tbox_neigh_req_goes_to_sc` directly from `tbox_neigh_req_pop_data.opcode` and uses that combinational route selector in the valid-state next logic:
  - `assign tbox_neigh_req_goes_to_sc = tbox_neigh_req_pop_data.opcode != ET_LINK_REQ_MsgSendData;`
  - `if (tbox_neigh_req_pop_valid && tbox_neigh_req_goes_to_sc && !tbox_sc_req_disable_next) tbox_sc_req_valid_next = 1'b1;`
  - `if (tbox_neigh_req_pop_valid && !tbox_neigh_req_goes_to_sc) tbox_min_rsp_valid_next = 1'b1;`
  The `tbox_neigh_req_fifo` is a `gen_fifo` with `USE_LATCHES=0`; its `valid` flag and `pop_data` can expose different phase/cycle views around an empty-to-nonempty transition, so the route valid can be selected from stale data while the unregistered output payload follows the current FIFO read data.
- **Reachability:** Reachable with legal valid/ready traffic. The standalone `neigh_tbox_router` cosim drives non-message and message request sequences and observes zero mismatches, confirming the behavior is present in the original RTL under Verilator. The unit test includes a directed check named `TBOX preserves original stale-route behavior for the first message after an SC request`.
- **System impact:** A message request can be presented on the SC request interface, or the reverse can occur after a stale message opcode. Downstream behavior depends on surrounding traffic assumptions and whether higher-level integration keeps the route-select FIFO from entering the empty-to-nonempty/stale-data pattern.
- **Reimplementation decision:** The current reimplementation intentionally **preserves** the original behavior for cycle-by-cycle equivalence. It does not insert an extra route register or change the FIFO timing. Any future correctness fix must be treated as an intentional divergence and reflected in cosim expectations.
