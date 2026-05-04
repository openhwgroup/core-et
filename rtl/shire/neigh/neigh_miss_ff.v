// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module neigh_miss_ff (
  // System signals
  input  logic                                                      clock,
  input  logic                                                      reset,
  // Miss request from minion
  input  et_link_minion_miss_req_info_t                             l2_dcache_req,
  input  logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]                      l2_dcache_req_valid,
  output logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]                      l2_dcache_req_ready,
  // Data to L2
  output et_link_neigh_req_info_t                                   miss_req,
  output logic                                                      miss_valid,
  input  logic                                                      miss_ready,
  // Data to cooperative tload module
  output et_link_minion_miss_req_info_t [`SHIRE_COOP_MISS_REQS-1:0] coop_miss_req,
  output logic [`SHIRE_COOP_MISS_REQS-1:0]                          coop_miss_valid,
  input  logic [`SHIRE_COOP_MISS_REQS-1:0]                          coop_miss_ready,
  // Output debug signals going to Status Monitor
  output logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]                      dbg_sm_signals
  );

  // //////////////////////////////////////////////////////////////////////////////
  // This unit stores all the outstanding misses for a given minion pending to
  // be sent to the L2 and already out of the minion. There are 3 different
  // outstanding miss requests per minion:
  // - Regular misses
  // - Cooperative misses for TLoads going to TenB
  // - Cooperative misses for TLoads going to SCP
  // Regular misses go to the L2 arbiter
  // Cooperative TLoads go to the neigh_coop_tload unit
  // //////////////////////////////////////////////////////////////////////////////
  logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]                          miss_ff_available; // Stores if FF is avaiable
  logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]                          miss_ff_available_next;
  logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]                          miss_ff_in_accepted; // Miss request being accepted this cycle
  et_link_minion_miss_req_info_t [`DCACHE_L2_MISS_REQ_PORTS-1:0] miss_ff_req;
  logic [`DCACHE_L2_MISS_REQ_PORTS-1:0]                          miss_ff_out_accepted;

  // Send ready signals to dcache
  assign l2_dcache_req_ready = miss_ff_available;

  // //////////////////////////////////////////////////////////////////////////////
  // Generate the three miss FFs
  // //////////////////////////////////////////////////////////////////////////////

  for (genvar miss_ff_idx = 0; miss_ff_idx < `DCACHE_L2_MISS_REQ_PORTS; miss_ff_idx++) begin : MISS_FF
    // CLK    RST    EN                                DOUT                            DIN                                  DEF
    `RST_FF(clock, reset,                                   miss_ff_available[miss_ff_idx], miss_ff_available_next[miss_ff_idx], 1'b1)
    `EN_FF (clock,        miss_ff_in_accepted[miss_ff_idx], miss_ff_req[miss_ff_idx],       l2_dcache_req)

    assign miss_ff_in_accepted[miss_ff_idx] = l2_dcache_req_valid[miss_ff_idx] & miss_ff_available[miss_ff_idx];

    always_comb begin
      // If FF still available
      if (miss_ff_available[miss_ff_idx])
      // Not available anymore when input is valid
      miss_ff_available_next[miss_ff_idx] = ~l2_dcache_req_valid[miss_ff_idx];
      // Not available, wait for arbiter grant
      else
      // Grant received, FF available again
      miss_ff_available_next[miss_ff_idx] = miss_ff_out_accepted[miss_ff_idx];
    end
  end


  // //////////////////////////////////////////////////////////////////////////////
  // Send regular requests to L2 arbiter
  // //////////////////////////////////////////////////////////////////////////////

  // Check when output is accepted
  assign miss_ff_out_accepted[`DCACHE_L2_MISS_IDX_COMM] = miss_ready;

  // Send request
  assign miss_req.id        = (`ET_LINK_ID_SIZE)'(miss_ff_req[`DCACHE_L2_MISS_IDX_COMM].id);
  assign miss_req.source    = '0; // Unused (assigned at neighborhood channel)
  assign miss_req.wdata     = miss_ff_req[`DCACHE_L2_MISS_IDX_COMM].wdata;
  assign miss_req.opcode    = miss_ff_req[`DCACHE_L2_MISS_IDX_COMM].opcode;
  assign miss_req.address   = miss_ff_req[`DCACHE_L2_MISS_IDX_COMM].address;
  assign miss_req.data      = (`NEIGH_DATA_SIZE)'(miss_ff_req[`DCACHE_L2_MISS_IDX_COMM].data);
  assign miss_req.size      = miss_ff_req[`DCACHE_L2_MISS_IDX_COMM].size;
  assign miss_req.qwen      = miss_ff_req[`DCACHE_L2_MISS_IDX_COMM].qwen;
  assign miss_req.subopcode = miss_ff_req[`DCACHE_L2_MISS_IDX_COMM].subopcode;
  assign miss_valid         = ~miss_ff_available[`DCACHE_L2_MISS_IDX_COMM];

  // //////////////////////////////////////////////////////////////////////////////
  // Send cooperative requests to cooperative module
  // //////////////////////////////////////////////////////////////////////////////

  for (genvar miss_ff_idx = 0; miss_ff_idx < `SHIRE_COOP_MISS_REQS; miss_ff_idx++) begin : COOP_MISS_REQS
    // Check when output is accepted
    assign miss_ff_out_accepted[miss_ff_idx] = coop_miss_ready[miss_ff_idx];

    // Send request
    assign coop_miss_req[miss_ff_idx]   = miss_ff_req[miss_ff_idx];
    assign coop_miss_valid[miss_ff_idx] = ~miss_ff_available[miss_ff_idx];
  end
  // //////////////////////////////////////////////////////////////////////////////
  // Connect output debug signals
  // //////////////////////////////////////////////////////////////////////////////
  assign dbg_sm_signals = miss_ff_available;

endmodule
