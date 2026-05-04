// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_miss_ff
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                                                      clk_i,
  input  logic                                                      rst_ni,
  input  et_link_minion_miss_req_info_t                             l2_dcache_req_i,
  input  logic [DcacheL2MissReqPorts-1:0]                           l2_dcache_req_valid_i,
  output logic [DcacheL2MissReqPorts-1:0]                           l2_dcache_req_ready_o,
  output etlink_pkg::neigh_req_t                                    miss_req_o,
  output logic                                                      miss_valid_o,
  input  logic                                                      miss_ready_i,
  output et_link_minion_miss_req_info_t [ShireCoopMissReqs-1:0]     coop_miss_req_o,
  output logic [ShireCoopMissReqs-1:0]                              coop_miss_valid_o,
  input  logic [ShireCoopMissReqs-1:0]                              coop_miss_ready_i,
  output logic [DcacheL2MissReqPorts-1:0]                           dbg_sm_signals_o
);

  localparam int unsigned ShireCoopMissReqs = 2;
  localparam int unsigned DcacheL2MissIdxTenb = 0;
  localparam int unsigned DcacheL2MissIdxScp  = 1;
  localparam int unsigned DcacheL2MissIdxComm = 2;

  logic [DcacheL2MissReqPorts-1:0]              miss_ff_available_q;
  logic [DcacheL2MissReqPorts-1:0]              miss_ff_available_d;
  logic [DcacheL2MissReqPorts-1:0]              miss_ff_in_accepted;
  logic [DcacheL2MissReqPorts-1:0]              miss_ff_out_accepted;
  et_link_minion_miss_req_info_t                miss_ff_req_q [DcacheL2MissReqPorts-1:0];

  always_comb begin
    miss_ff_out_accepted = '0;
    miss_ff_out_accepted[DcacheL2MissIdxComm] = miss_ready_i;
    miss_ff_out_accepted[DcacheL2MissIdxTenb] = coop_miss_ready_i[DcacheL2MissIdxTenb];
    miss_ff_out_accepted[DcacheL2MissIdxScp]  = coop_miss_ready_i[DcacheL2MissIdxScp];

    l2_dcache_req_ready_o = miss_ff_available_q;

    for (int unsigned i = 0; i < DcacheL2MissReqPorts; i++) begin
      miss_ff_in_accepted[i] = l2_dcache_req_valid_i[i] && miss_ff_available_q[i];
      if (miss_ff_available_q[i]) begin
        miss_ff_available_d[i] = !l2_dcache_req_valid_i[i];
      end else begin
        miss_ff_available_d[i] = miss_ff_out_accepted[i];
      end
    end

    miss_req_o = '0;
    miss_req_o.id        = {{(etlink_pkg::IdSize-5){1'b0}},
                            miss_ff_req_q[DcacheL2MissIdxComm].id};
    miss_req_o.source    = '0;
    miss_req_o.wdata     = miss_ff_req_q[DcacheL2MissIdxComm].wdata;
    miss_req_o.opcode    = etlink_pkg::req_opcode_e'(miss_ff_req_q[DcacheL2MissIdxComm].opcode);
    miss_req_o.address   = miss_ff_req_q[DcacheL2MissIdxComm].address;
    miss_req_o.data      = {{(etlink_pkg::NeighDataSize-$bits(tensorcoop_control)){1'b0}},
                            miss_ff_req_q[DcacheL2MissIdxComm].data};
    miss_req_o.size      = etlink_pkg::size_e'(miss_ff_req_q[DcacheL2MissIdxComm].size);
    miss_req_o.qwen      = miss_ff_req_q[DcacheL2MissIdxComm].qwen;
    miss_req_o.subopcode = miss_ff_req_q[DcacheL2MissIdxComm].subopcode;
    miss_valid_o         = !miss_ff_available_q[DcacheL2MissIdxComm];

    for (int unsigned i = 0; i < ShireCoopMissReqs; i++) begin
      coop_miss_req_o[i]   = miss_ff_req_q[i];
      coop_miss_valid_o[i] = !miss_ff_available_q[i];
    end

    dbg_sm_signals_o = miss_ff_available_q;
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      for (int unsigned i = 0; i < DcacheL2MissReqPorts; i++) begin
        miss_ff_available_q[i] <= 1'b1;
        miss_ff_req_q[i]       <= '0;
      end
    end else begin
      for (int unsigned i = 0; i < DcacheL2MissReqPorts; i++) begin
        miss_ff_available_q[i] <= miss_ff_available_d[i];
        if (miss_ff_in_accepted[i]) begin
          miss_ff_req_q[i] <= l2_dcache_req_i;
        end
      end
    end
  end

endmodule
