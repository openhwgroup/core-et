// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_evict_ff_unit
  import minion_pkg::*;
  import minion_dcache_pkg::*;
(
  input  logic                                        clk_i,
  input  logic                                        rst_ni,
  input  logic [NumShireIdsBits-1:0]                  shire_id_i,
  input  logic [1:0]                                  neigh_id_i,
  /* verilator lint_off UNUSEDSIGNAL */  // The original evict FF ignores the input source bit and stores only data/control fields used downstream.
  input  et_link_minion_evict_req_info_t              l2_dcache_req_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  logic                                        l2_dcache_req_valid_i,
  output logic                                        l2_dcache_req_ready_o,
  input  logic [2:0]                                  local_channel_mask_i,
  input  logic [2:0][2:0]                             local_dest_id_i,
  input  logic                                        local_message_ready_i,
  output logic [2:0]                                  local_message_valid_o,
  output etlink_pkg::neigh_rsp_t                      local_message_info_o,
  output etlink_pkg::neigh_req_t                      evict_req_o,
  output logic                                        evict_valid_o,
  input  logic                                        evict_ready_i,
  output logic                                        dbg_sm_signals_o
);

  localparam int unsigned MaxLocalConnect      = 3;
  localparam int unsigned LocalDestIdBits      = 3;
  localparam int unsigned AdEsrRegionStart     = 32;
  localparam int unsigned AdEsrRegionSize      = etlink_pkg::PaSize - AdEsrRegionStart;
  localparam logic [AdEsrRegionSize-1:0] AdEsrRegion = 8'h01;
  localparam int unsigned AdEsrMessageIdStart    = 3;
  localparam int unsigned AdEsrMessageIdSize     = 8;
  localparam int unsigned AdEsrNeighHartIdStart  = 12;
  localparam int unsigned AdEsrNeighHartIdSize   = 4;
  localparam int unsigned AdEsrShireMinIdStart   = 13;
  localparam int unsigned AdEsrShireMinIdSize    = 7;
  localparam int unsigned AdEsrAgentTypeStart    = 20;
  localparam int unsigned AdEsrAgentTypeSize     = 2;
  localparam logic [AdEsrAgentTypeSize-1:0] AdEsrAgentTypeMinion = '0;
  localparam int unsigned AdEsrShireIdStart      = 22;
  localparam int unsigned AdEsrShireIdSize       = 8;
  localparam logic [AdEsrShireIdSize-1:0] LocalShireId = 8'hff;
  localparam int unsigned NumNeighBits           = 2;
  localparam int unsigned LocalMinIdPrefixBits   =
      AdEsrShireMinIdSize - NumNeighBits - LocalDestIdBits;
  localparam int unsigned InputPassCountWidth    =
      (CoreL2BlockExtPasses > 1) ? $clog2(CoreL2BlockExtPasses) : 1;
  localparam int unsigned OutputPassCountWidth   =
      ((etlink_pkg::DataSize / etlink_pkg::NeighDataSize) > 1)
          ? $clog2(etlink_pkg::DataSize / etlink_pkg::NeighDataSize)
          : 1;
  localparam logic [InputPassCountWidth-1:0] InputLastPassLine =
      {{(InputPassCountWidth-1){1'b0}}, 1'b1};
  localparam logic [InputPassCountWidth-1:0] InputLastPassHLine =
      '0;
  localparam logic [OutputPassCountWidth-1:0] OutputLastPassLine =
      {{(OutputPassCountWidth-1){1'b0}}, 1'b1};
  localparam logic [OutputPassCountWidth-1:0] OutputLastPassHLine =
      '0;

  logic                                        evict_ff_available_q;
  logic                                        evict_ff_available_d;
  logic                                        evict_ff_in_accepted;
  logic                                        evict_ff_in_update;
  logic [InputPassCountWidth-1:0]              evict_ff_in_counter_q;
  logic [InputPassCountWidth-1:0]              evict_ff_in_counter_d;
  logic [InputPassCountWidth-1:0]              evict_ff_last_in_pass;
  logic                                        is_last_in_pass;
  etlink_pkg::req_nodata_t                     evict_ff_req_q;
  etlink_pkg::req_nodata_t                     evict_ff_req_d;
  logic [CoreL2BlockExtPasses-1:0][CoreL2BlockExtSize-1:0] evict_ff_data_q;
  logic [CoreL2BlockExtPasses-1:0]             evict_ff_data_en;
  logic                                        evict_ff_out_accepted;
  logic [OutputPassCountWidth-1:0]             evict_ff_out_counter_q;
  logic [OutputPassCountWidth-1:0]             evict_ff_out_counter_d;
  logic [OutputPassCountWidth-1:0]             evict_ff_last_out_pass;
  logic                                        is_last_out_pass;
  logic [MaxLocalConnect-1:0]                  evict_goes_to_local;
  logic                                        out_ready;

  always_comb begin
    evict_ff_req_d = '0;
    evict_ff_req_d.id = {{(etlink_pkg::IdSize-$bits(l2_dcache_req_i.id)){1'b0}}, l2_dcache_req_i.id};
    evict_ff_req_d.source = '0;
    evict_ff_req_d.wdata = 1'b1;
    evict_ff_req_d.opcode = etlink_pkg::req_opcode_e'(l2_dcache_req_i.opcode);
    evict_ff_req_d.address = l2_dcache_req_i.address;
    evict_ff_req_d.size = etlink_pkg::size_e'(l2_dcache_req_i.size);
    evict_ff_req_d.qwen = l2_dcache_req_i.qwen;
    evict_ff_req_d.subopcode = l2_dcache_req_i.subopcode;
  end

  assign l2_dcache_req_ready_o = evict_ff_available_q;
  assign evict_ff_in_accepted = l2_dcache_req_valid_i && evict_ff_available_q;
  assign evict_ff_in_update = evict_ff_in_accepted && (evict_ff_in_counter_q == '0);
  assign evict_ff_in_counter_d = evict_ff_in_counter_q + 1'b1;
  assign evict_ff_out_counter_d = evict_ff_out_counter_q + 1'b1;

  always_comb begin
    unique case (l2_dcache_req_i.size)
      EtLinkLine: evict_ff_last_in_pass = InputLastPassLine;
      default:    evict_ff_last_in_pass = InputLastPassHLine;
    endcase
  end

  assign is_last_in_pass = (evict_ff_in_counter_q == evict_ff_last_in_pass);

  always_comb begin
    if (evict_ff_available_q) begin
      evict_ff_available_d = !(l2_dcache_req_valid_i && is_last_in_pass);
    end else begin
      evict_ff_available_d = out_ready && is_last_out_pass;
    end
  end

  assign evict_ff_out_accepted = !evict_ff_available_q && out_ready;

  always_comb begin
    unique case (evict_ff_req_q.size)
      etlink_pkg::SizeLine: evict_ff_last_out_pass = OutputLastPassLine;
      default:              evict_ff_last_out_pass = OutputLastPassHLine;
    endcase
  end

  assign is_last_out_pass = (evict_ff_out_counter_q == evict_ff_last_out_pass);

  always_comb begin
    evict_req_o = '0;
    evict_req_o.id = evict_ff_req_q.id;
    evict_req_o.source = evict_ff_req_q.source;
    evict_req_o.wdata = evict_ff_req_q.wdata;
    evict_req_o.opcode = evict_ff_req_q.opcode;
    evict_req_o.address = evict_ff_req_q.address;
    evict_req_o.data = evict_ff_data_q[evict_ff_out_counter_q];
    evict_req_o.size = evict_ff_req_q.size;
    evict_req_o.qwen = evict_ff_req_q.qwen;
    evict_req_o.subopcode = evict_ff_req_q.subopcode;

    local_message_info_o = '0;
    local_message_info_o.id = evict_ff_req_q.address[AdEsrMessageIdStart +: AdEsrMessageIdSize];
    local_message_info_o.dest = evict_ff_req_q.address[AdEsrNeighHartIdStart +: AdEsrNeighHartIdSize];
    local_message_info_o.wdata = 1'b1;
    local_message_info_o.opcode = etlink_pkg::RspMsgRcvData;
    local_message_info_o.data = evict_ff_data_q[0];
    local_message_info_o.size = evict_ff_req_q.size;
    local_message_info_o.qwen = evict_ff_req_q.qwen;
  end

  always_comb begin
    for (int unsigned i = 0; i < MaxLocalConnect; i++) begin
      evict_goes_to_local[i] = local_channel_mask_i[i]
          && (evict_ff_req_q.opcode == etlink_pkg::ReqMsgSendData)
          && (evict_ff_req_q.address[AdEsrRegionStart +: AdEsrRegionSize] == AdEsrRegion)
          && ((evict_ff_req_q.address[AdEsrShireIdStart +: AdEsrShireIdSize] == shire_id_i)
              || (evict_ff_req_q.address[AdEsrShireIdStart +: AdEsrShireIdSize] == LocalShireId))
          && (evict_ff_req_q.address[AdEsrAgentTypeStart +: AdEsrAgentTypeSize] == AdEsrAgentTypeMinion)
          && (evict_ff_req_q.address[AdEsrShireMinIdStart +: AdEsrShireMinIdSize]
              == {{LocalMinIdPrefixBits{1'b0}}, neigh_id_i, local_dest_id_i[i]})
          && (evict_ff_req_q.size <= etlink_pkg::SizeHLine);
      local_message_valid_o[i] = !evict_ff_available_q && evict_goes_to_local[i];
    end
  end

  assign out_ready = (|evict_goes_to_local) ? local_message_ready_i : evict_ready_i;
  assign evict_valid_o = !evict_ff_available_q && !(|evict_goes_to_local);
  assign dbg_sm_signals_o = evict_ff_available_q;

  for (genvar i = 0; i < CoreL2BlockExtPasses; i++) begin : gen_evict_ff_data_en
    localparam logic [InputPassCountWidth-1:0] PassIdx = i;
    assign evict_ff_data_en[i] = evict_ff_in_accepted && (evict_ff_in_counter_q == PassIdx);
  end

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      evict_ff_available_q   <= 1'b1;
      evict_ff_in_counter_q  <= '0;
      evict_ff_req_q         <= '0;
      evict_ff_out_counter_q <= '0;
      evict_ff_data_q        <= '0;
    end else begin
      evict_ff_available_q <= evict_ff_available_d;

      if (!evict_ff_available_q) begin
        evict_ff_in_counter_q <= '0;
      end else if (evict_ff_in_accepted) begin
        evict_ff_in_counter_q <= evict_ff_in_counter_d;
      end

      if (evict_ff_available_q) begin
        evict_ff_out_counter_q <= '0;
      end else if (evict_ff_out_accepted) begin
        evict_ff_out_counter_q <= evict_ff_out_counter_d;
      end

      if (evict_ff_in_update) begin
        evict_ff_req_q <= evict_ff_req_d;
      end

      for (int unsigned i = 0; i < CoreL2BlockExtPasses; i++) begin
        if (evict_ff_data_en[i]) begin
          evict_ff_data_q[i] <= l2_dcache_req_i.data;
        end
      end
    end
  end

endmodule
