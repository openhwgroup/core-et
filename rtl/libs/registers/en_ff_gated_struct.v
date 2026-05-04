// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module en_ff_gated_struct #(
   parameter WIDTH = 8,
   parameter gating_type_t GATING_TYPE = gating_type_default
) (
  input  logic clock,
  input  logic en,
  output logic [WIDTH-1:0] Q,
  input  logic [WIDTH-1:0] D
);
`ifdef UNIT_TEST
   `EN_FF(clock, en, Q, D)
`else
   generate
      case (GATING_TYPE)
         // sc_xbar_req_t
         gating_type_sc_xbar_req: begin : g_sc_xbar_req_t
            sc_xbar_req_t d_struct;
            sc_xbar_req_t q_struct;
            assign d_struct = D;
            always @(posedge clock) begin
               if(en) begin
                  q_struct.port_id            <= d_struct.port_id;
                  q_struct.trans_id           <= d_struct.trans_id;
                  q_struct.req_info.id        <= d_struct.req_info.id;
                  q_struct.req_info.source    <= d_struct.req_info.source;
                  q_struct.req_info.wdata     <= d_struct.req_info.wdata;
                  q_struct.req_info.opcode    <= d_struct.req_info.opcode;
                  q_struct.req_info.address   <= d_struct.req_info.address;
                  q_struct.req_info.size      <= d_struct.req_info.size;
                  q_struct.req_info.qwen      <= d_struct.req_info.qwen;
                  q_struct.req_info.subopcode <= d_struct.req_info.subopcode;
               end
               for (integer ii = 0; ii < `ET_LINK_QWEN_SIZE; ii++) begin
                  if(en & d_struct.req_info.wdata & d_struct.req_info.qwen[ii]) begin
                     q_struct.req_info.data[ii*128+:128]  <= d_struct.req_info.data[ii*128+:128];
                  end
               end
            end
            assign Q = q_struct;
         end
         // sc_xbar_rsp_t
         gating_type_sc_xbar_rsp: begin : g_sc_xbar_rsp_t
            sc_xbar_rsp_t d_struct;
            sc_xbar_rsp_t q_struct;
            assign d_struct = D;
            always @(posedge clock) begin
               if(en) begin
                  q_struct.port_id            <= d_struct.port_id;
                  q_struct.trans_id           <= d_struct.trans_id;
                  q_struct.src_is_uc          <= d_struct.src_is_uc;
                  q_struct.rsp_info.id        <= d_struct.rsp_info.id;
                  q_struct.rsp_info.dest      <= d_struct.rsp_info.dest;
                  q_struct.rsp_info.wdata     <= d_struct.rsp_info.wdata;
                  q_struct.rsp_info.opcode    <= d_struct.rsp_info.opcode;
                  q_struct.rsp_info.size      <= d_struct.rsp_info.size;
                  q_struct.rsp_info.qwen      <= d_struct.rsp_info.qwen;
               end
               for (integer ii = 0; ii < `ET_LINK_QWEN_SIZE; ii++) begin
                  if(en & d_struct.rsp_info.wdata & d_struct.rsp_info.qwen[ii]) begin
                     q_struct.rsp_info.data[ii*128+:128] <= d_struct.rsp_info.data[ii*128+:128];
                  end
               end
            end
            assign Q = q_struct;
         end
         // et_link_req_info_t
         gating_type_et_link_req: begin : g_et_link_req_info_t
            et_link_req_info_t d_struct;
            et_link_req_info_t q_struct;
            assign d_struct = D;
            always @(posedge clock) begin
               if(en) begin
                  q_struct.id        <= d_struct.id;
                  q_struct.source    <= d_struct.source;
                  q_struct.wdata     <= d_struct.wdata;
                  q_struct.opcode    <= d_struct.opcode;
                  q_struct.address   <= d_struct.address;
                  q_struct.size      <= d_struct.size;
                  q_struct.qwen      <= d_struct.qwen;
                  q_struct.subopcode <= d_struct.subopcode;
               end
               for (integer ii = 0; ii < `ET_LINK_QWEN_SIZE; ii++) begin
                  if(en & d_struct.wdata & d_struct.qwen[ii]) begin
                     q_struct.data[ii*128+:128]  <= d_struct.data[ii*128+:128];
                  end
               end
            end
            assign Q = q_struct;
         end
         // et_link_rsp_info_t
         gating_type_et_link_rsp: begin : g_et_link_rsp_info_t
            et_link_rsp_info_t d_struct;
            et_link_rsp_info_t q_struct;
            assign d_struct = D;
            always @(posedge clock) begin
               if(en) begin
                  q_struct.id        <= d_struct.id;
                  q_struct.dest      <= d_struct.dest;
                  q_struct.wdata     <= d_struct.wdata;
                  q_struct.opcode    <= d_struct.opcode;
                  q_struct.size      <= d_struct.size;
                  q_struct.qwen      <= d_struct.qwen;
               end
               for (integer ii = 0; ii < `ET_LINK_QWEN_SIZE; ii++) begin
                  if(en & d_struct.wdata & d_struct.qwen[ii]) q_struct.data[ii*128+:128] <= d_struct.data[ii*128+:128];
               end
            end
            assign Q = q_struct;
         end
         // sc_dataq_wr_t
         gating_type_sc_dataq_wr: begin : g_sc_dataq_wr_info_t
            sc_dataq_wr_t d_struct;
            sc_dataq_wr_t q_struct;
            assign d_struct = D;
            always @(posedge clock) begin
               if(en) begin
                  q_struct.trans_id  <= d_struct.trans_id;
                  q_struct.reqq_id   <= d_struct.reqq_id;
                  q_struct.dataq_id  <= d_struct.dataq_id;
                  q_struct.qwen      <= d_struct.qwen;
                  q_struct.wr_ben    <= d_struct.wr_ben;
               end
               for (integer ii = 0; ii < `ET_LINK_QWEN_SIZE; ii++) begin
                  if(en & d_struct.qwen[ii]) begin
                      q_struct.data[ii*128+:128] <= d_struct.data[ii*128+:128];
                  end
               end
               if(en & d_struct.wr_ben) begin
                      q_struct.ben       <= d_struct.ben;
               end
            end
            assign Q = q_struct;
         end
         // sc_mesh_master_req_t
         gating_type_sc_mesh_master_req: begin : g_sc_mesh_master_req_t
            localparam STRUCT_SIZE = $bits(sc_mesh_master_req_t);
            localparam TRANSP_DATA_ENABLE = (WIDTH>STRUCT_SIZE) ? 1 : 0;
            localparam TRANSP_DATA_WIDTH  = TRANSP_DATA_ENABLE ? WIDTH - STRUCT_SIZE : 1;
            sc_mesh_master_req_t d_struct;
            sc_mesh_master_req_t q_struct;
            logic [TRANSP_DATA_WIDTH-1:0] d_transp_data;
            logic [TRANSP_DATA_WIDTH-1:0] q_transp_data;
            if (TRANSP_DATA_ENABLE) begin : transp_data_d
               assign {d_transp_data, d_struct} = D;
            end
            else begin : no_transp_data_d
               assign d_transp_data = '0;
               assign d_struct = D;
            end
            always @(posedge clock) begin
               if(en) begin
                  q_transp_data      <= d_transp_data;
                  q_struct.trans_id  <= d_struct.trans_id;
                  q_struct.id        <= d_struct.id;
                  q_struct.port_id   <= d_struct.port_id;
                  q_struct.source    <= d_struct.source;
                  q_struct.opcode    <= d_struct.opcode;
                  q_struct.address   <= d_struct.address;
                  q_struct.size      <= d_struct.size;
                  q_struct.qos       <= d_struct.qos;
                  q_struct.wdata     <= d_struct.wdata;
                  q_struct.qwen      <= d_struct.qwen;
                  q_struct.ben_en    <= d_struct.ben_en;
               end
               for (integer ii = 0; ii < `SC_LINE_QW_SIZE; ii++) begin
                  if(en & d_struct.wdata & d_struct.qwen[ii]) begin
                      q_struct.data[ii*128+:128] <= d_struct.data[ii*128+:128];
                  end
               end
               if(en & d_struct.ben_en) begin
                  q_struct.ben       <= d_struct.ben;
               end
            end
            if (TRANSP_DATA_ENABLE) begin : transp_data_q
               assign Q = {q_transp_data, q_struct};
            end
            else begin : no_transp_data_q
               assign Q = q_struct;
               wire  unused_ok = &{
                  `ifndef ET_ASCENT_LINT
                  1'b0,
                  `endif
                  q_transp_data,
                  q_transp_data
               };
            end
         end
         // sc_mesh_slave_req_t
         gating_type_sc_mesh_slave_req: begin : g_sc_mesh_slave_req_t
            localparam STRUCT_SIZE = $bits(sc_mesh_slave_req_t);
            localparam TRANSP_DATA_ENABLE = (WIDTH>STRUCT_SIZE) ? 1 : 0;
            localparam TRANSP_DATA_WIDTH  = TRANSP_DATA_ENABLE ? WIDTH - STRUCT_SIZE : 1;
            sc_mesh_slave_req_t d_struct;
            sc_mesh_slave_req_t q_struct;
            logic [TRANSP_DATA_WIDTH-1:0] d_transp_data;
            logic [TRANSP_DATA_WIDTH-1:0] q_transp_data;
            if (TRANSP_DATA_ENABLE) begin : transp_data_d
               assign {d_transp_data, d_struct} = D;
            end
            else begin : no_transp_data_d
               assign d_transp_data = '0;
               assign d_struct = D;
            end
            always @(posedge clock) begin
               if(en) begin
                  q_transp_data      <= d_transp_data;
                  q_struct.trans_id  <= d_struct.trans_id;
                  q_struct.id        <= d_struct.id;
                  q_struct.port_id   <= d_struct.port_id;
                  q_struct.source    <= d_struct.source;
                  q_struct.opcode    <= d_struct.opcode;
                  q_struct.address   <= d_struct.address;
                  q_struct.size      <= d_struct.size;
                  q_struct.qos       <= d_struct.qos;
                  q_struct.wdata     <= d_struct.wdata;
                  q_struct.qwen      <= d_struct.qwen;
                  q_struct.ben_en    <= d_struct.ben_en;
               end
               for (integer ii = 0; ii < `SC_LINE_QW_SIZE; ii++) begin
                  if(en & d_struct.wdata & d_struct.qwen[ii]) begin
                     q_struct.data[ii*128+:128] <= d_struct.data[ii*128+:128];
                  end
               end
               if(en & d_struct.ben_en) begin
                  q_struct.ben       <= d_struct.ben;
               end
            end
            if (TRANSP_DATA_ENABLE) begin : transp_data_q
               assign Q = {q_transp_data, q_struct};
            end
            else begin : no_transp_data_q
               assign Q = q_struct;
               wire  unused_ok = &{
                  `ifndef ET_ASCENT_LINT
                  1'b0,
                  `endif
                  q_transp_data,
                  q_transp_data
               };
            end
         end
         // sc_mesh_master_rsp_t
         gating_type_sc_mesh_master_rsp: begin : g_sc_mesh_master_rsp_t
            localparam STRUCT_SIZE = $bits(sc_mesh_master_rsp_t);
            localparam TRANSP_DATA_ENABLE = (WIDTH>STRUCT_SIZE) ? 1 : 0;
            localparam TRANSP_DATA_WIDTH  = TRANSP_DATA_ENABLE ? WIDTH - STRUCT_SIZE : 1;
            sc_mesh_master_rsp_t d_struct;
            sc_mesh_master_rsp_t q_struct;
            logic [TRANSP_DATA_WIDTH-1:0] d_transp_data;
            logic [TRANSP_DATA_WIDTH-1:0] q_transp_data;
            if (TRANSP_DATA_ENABLE) begin : transp_data_d
               assign {d_transp_data, d_struct} = D;
            end
            else begin : no_transp_data_d
               assign d_transp_data = '0;
               assign d_struct = D;
            end
            always @(posedge clock) begin
               if(en) begin
                  q_transp_data      <= d_transp_data;
                  q_struct.trans_id  <= d_struct.trans_id;
                  q_struct.id        <= d_struct.id;
                  q_struct.port_id   <= d_struct.port_id;
                  q_struct.source    <= d_struct.source;
                  q_struct.err       <= d_struct.err;
                  q_struct.wdata     <= d_struct.wdata;
               end
               if(en & d_struct.wdata) begin
                  q_struct.data      <= d_struct.data;
               end
            end
            if (TRANSP_DATA_ENABLE) begin : transp_data_q
               assign Q = {q_transp_data, q_struct};
            end
            else begin : no_transp_data_q
               assign Q = q_struct;
               wire  unused_ok = &{
                  `ifndef ET_ASCENT_LINT
                  1'b0,
                  `endif
                  q_transp_data,
                  q_transp_data
               };
            end
         end
         // sc_mesh_slave_rsp_t
         gating_type_sc_mesh_slave_rsp: begin : g_sc_mesh_slave_rsp_t
            localparam STRUCT_SIZE = $bits(sc_mesh_slave_rsp_t);
            localparam TRANSP_DATA_ENABLE = (WIDTH>STRUCT_SIZE) ? 1 : 0;
            localparam TRANSP_DATA_WIDTH  = TRANSP_DATA_ENABLE ? WIDTH - STRUCT_SIZE : 1;
            sc_mesh_slave_rsp_t d_struct;   // ri lint_check_waive NOT_READ  // .trans_id not used by RTL
            sc_mesh_slave_rsp_t q_struct;
            logic [TRANSP_DATA_WIDTH-1:0] d_transp_data;
            logic [TRANSP_DATA_WIDTH-1:0] q_transp_data;
            if (TRANSP_DATA_ENABLE) begin : transp_data_d
               assign {d_transp_data, d_struct} = D;
            end
            else begin : no_transp_data_d
               assign d_transp_data = '0;
               assign d_struct = D;
            end
            always @(posedge clock) begin
               if(en) begin
                  q_transp_data      <= d_transp_data;
                  q_struct.trans_id  <= d_struct.trans_id;
                  q_struct.id        <= d_struct.id;
                  q_struct.port_id   <= d_struct.port_id;
                  q_struct.source    <= d_struct.source;
                  q_struct.err       <= d_struct.err;
                  q_struct.wdata     <= d_struct.wdata;
               end
               if(en & d_struct.wdata) begin
                  q_struct.data      <= d_struct.data;
               end
            end
            if (TRANSP_DATA_ENABLE) begin : transp_data_q
               assign Q = {q_transp_data, q_struct};
            end
            else begin : no_transp_data_q
               assign Q = q_struct;
               wire  unused_ok = &{
                  `ifndef ET_ASCENT_LINT
                  1'b0,
                  `endif
                  q_transp_data,
                  q_transp_data
               };
            end
         end
         // sc_reqq_mesh_req_t
         gating_type_sc_reqq_mesh_req: begin : g_sc_reqq_mesh_req_info_t
            sc_reqq_mesh_req_t d_struct;
            sc_reqq_mesh_req_t q_struct;
            assign d_struct = D;
            always @(posedge clock) begin
               if(en) begin
                  q_struct.trans_id  <= d_struct.trans_id;
                  q_struct.reqq_id   <= d_struct.reqq_id;
                  q_struct.dataq_id  <= d_struct.dataq_id;
                  q_struct.with_data <= d_struct.with_data;
                  q_struct.rd_ben    <= d_struct.rd_ben;
                  q_struct.qwen      <= d_struct.qwen;
               end
               if(en & d_struct.with_data) begin
                  q_struct.data      <= d_struct.data;
               end
               if(en & d_struct.rd_ben) begin
                  q_struct.ben       <= d_struct.ben;
               end
            end
            assign Q = q_struct;
         end
         // sc_rspmux
         gating_type_sc_rspmux: begin : g_sc_rspmux_t
            sc_rspmux_t d_struct;
            sc_rspmux_t q_struct;
            assign d_struct = D;
            always @(posedge clock) begin
               if(en) begin
                  q_struct.trans_id  <= d_struct.trans_id;
                  q_struct.reqq_id   <= d_struct.reqq_id;
                  q_struct.with_data <= d_struct.with_data;
                  q_struct.zero_data <= d_struct.zero_data;
                  q_struct.qwen      <= d_struct.qwen;
               end
               if(en & d_struct.with_data) begin
                  q_struct.data      <= d_struct.data;
               end
            end
            assign Q = q_struct;
         end
         // sc_rspmux_long
         gating_type_sc_rspmux_long: begin : g_sc_rspmux_long_t
            sc_rspmux_long_t d_struct;
            sc_rspmux_long_t q_struct;
            assign d_struct = D;
            always @(posedge clock) begin
               if(en) begin
                  q_struct.trans_id  <= d_struct.trans_id;
                  q_struct.reqq_id   <= d_struct.reqq_id;
                  q_struct.dest_hot  <= d_struct.dest_hot;
                  q_struct.port_id   <= d_struct.port_id;
                  q_struct.traced    <= d_struct.traced;
                  q_struct.with_data <= d_struct.with_data;
                  if(en & d_struct.with_data) begin
                     q_struct.data <= d_struct.data;
                  end
               end
               if(en) begin
                  q_struct.xbar_info.port_id            <= d_struct.xbar_info.port_id;
                  q_struct.xbar_info.trans_id           <= d_struct.xbar_info.trans_id;
                  q_struct.xbar_info.src_is_uc          <= d_struct.xbar_info.src_is_uc;
                  q_struct.xbar_info.rsp_info.id        <= d_struct.xbar_info.rsp_info.id;
                  q_struct.xbar_info.rsp_info.dest      <= d_struct.xbar_info.rsp_info.dest;
                  q_struct.xbar_info.rsp_info.wdata     <= d_struct.xbar_info.rsp_info.wdata;
                  q_struct.xbar_info.rsp_info.opcode    <= d_struct.xbar_info.rsp_info.opcode;
                  q_struct.xbar_info.rsp_info.size      <= d_struct.xbar_info.rsp_info.size;
                  q_struct.xbar_info.rsp_info.qwen      <= d_struct.xbar_info.rsp_info.qwen;
               end
               for (integer ii = 0; ii < `ET_LINK_QWEN_SIZE; ii++) begin
                  if(en & d_struct.xbar_info.rsp_info.wdata & d_struct.xbar_info.rsp_info.qwen[ii]) begin
                     q_struct.xbar_info.rsp_info.data[ii*128+:128] <= d_struct.xbar_info.rsp_info.data[ii*128+:128];
                  end
               end
            end
            assign Q = q_struct;
         end
         gating_type_default: begin : gt_default
            `EN_FF(clock, en, Q, D)
         end
         default: begin : default_block
            `EN_FF(clock, en, Q, D)
         end
      endcase
   endgenerate
`endif
endmodule
