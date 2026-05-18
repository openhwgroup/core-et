// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module uncacheable_noc
  import axi_pkg::*;
  import etlink_pkg::*;
  import shirecache_pkg::*;
  import shire_esr_pkg::*;
  import shire_uncached_pkg::*;
#(
  parameter int unsigned ReqqSize = UcReqqSize,
  parameter int unsigned NumPortsP = NumNeigh,
  localparam int unsigned ReqqIndexWidth = $clog2(ReqqSize)
) (
  input  logic                       clk_i,
  input  logic                       rst_ni,
  input  logic [NumShireIdsBits-1:0] shire_id_i,
  input  esr_uc_config_t             uc_config_i,

  output logic                       neigh_uc_req_ready_o,
  input  logic                       neigh_uc_req_valid_i,
  input  shirecache_pkg::xbar_req_t  neigh_uc_req_info_i,

  input  logic [NumPortsP-1:0]       neigh_uc_min_rsp_ready_i,
  output logic [NumPortsP-1:0]       neigh_uc_min_rsp_valid_o,
  output shirecache_pkg::xbar_rsp_t  neigh_uc_min_rsp_info_o,

  input  logic                       credit_i,
  output logic                       credit_return_o,

  input  sys_slave_aw_t              from_sys_aw_i,
  input  logic                       from_sys_aw_valid_i,
  output logic                       from_sys_aw_ready_o,
  input  sys_slave_w_t               from_sys_w_i,
  input  logic                       from_sys_w_valid_i,
  output logic                       from_sys_w_ready_o,
  output sys_slave_b_t               from_sys_b_o,
  output logic                       from_sys_b_valid_o,
  input  logic                       from_sys_b_ready_i,

  input  logic                       l3_enabled_i,
  input  logic                       remote_scp_enabled_i,
  output sc_master_ar_t              to_l3_ar_o,
  output logic                       to_l3_ar_valid_o,
  input  logic                       to_l3_ar_ready_i,
  output sc_master_aw_t              to_l3_aw_o,
  output logic                       to_l3_aw_valid_o,
  input  logic                       to_l3_aw_ready_i,
  output sc_master_w_t               to_l3_w_o,
  output logic                       to_l3_w_valid_o,
  input  logic                       to_l3_w_ready_i,
  input  sc_master_b_t               to_l3_b_i,
  input  logic                       to_l3_b_valid_i,
  output logic                       to_l3_b_ready_o,
  input  sc_master_r_t               to_l3_r_i,
  input  logic                       to_l3_r_valid_i,
  output logic                       to_l3_r_ready_o,

  output sc_master_ar_t              to_sys_ar_o,
  output logic                       to_sys_ar_valid_o,
  input  logic                       to_sys_ar_ready_i,
  output sc_master_aw_t              to_sys_aw_o,
  output logic                       to_sys_aw_valid_o,
  input  logic                       to_sys_aw_ready_i,
  output sc_master_w_t               to_sys_w_o,
  output logic                       to_sys_w_valid_o,
  input  logic                       to_sys_w_ready_i,
  input  sc_master_b_t               to_sys_b_i,
  input  logic                       to_sys_b_valid_i,
  output logic                       to_sys_b_ready_o,
  input  sc_master_r_t               to_sys_r_i,
  input  logic                       to_sys_r_valid_i,
  output logic                       to_sys_r_ready_o
);

  localparam int unsigned AxiFifoIdx = 0;
  localparam int unsigned ErrorFifoIdx = 1;
  localparam int unsigned PortRspIndexWidth = $clog2(NumPortsP);

  logic rd_req_fifo_push;
  logic rd_req_fifo_pop;
  logic rd_req_fifo_full;
  logic rd_req_fifo_not_empty;
  logic [ReqqIndexWidth-1:0] rd_req_index_push;
  logic [ReqqIndexWidth-1:0] rd_req_index_pop;
  logic [ReqqSize:0] rd_req_fifo_level_unused;

  logic wr_req_fifo_push;
  logic wr_req_fifo_pop;
  logic wr_req_fifo_full;
  logic wr_req_fifo_not_empty;
  logic [ReqqIndexWidth-1:0] wr_req_index_push;
  logic [ReqqIndexWidth-1:0] wr_req_index_pop;
  logic [ReqqSize:0] wr_req_fifo_level_unused;

  logic error_decode_rsp_fifo_push;
  logic error_decode_rsp_fifo_pop;
  logic error_decode_rsp_fifo_full;
  uc_decode_rsp_t error_decode_rsp_info_push;
  uc_decode_rsp_t error_decode_rsp_info_pop;
  logic [ReqqSize:0] error_decode_rsp_fifo_level_unused;

  logic broadcast_new_send;
  logic broadcast_new_ack;
  logic [BcastSendsSize-1:0] broadcast_sends_q;
  logic [BcastSendsSize-1:0] broadcast_sends_d;
  logic [BcastSendsSize-1:0] broadcast_total_sends_q;
  logic [BcastSendsSize-1:0] broadcast_acks_q;
  logic [BcastSendsSize-1:0] broadcast_acks_d;
  logic [BcastMaskWidth-1:0] broadcast_mask_q;
  logic [BcastMaskWidth-1:0] broadcast_mask_d;
  logic [ReqqIndexWidth-1:0] broadcast_req_index_q;
  logic [ReqqIndexWidth-1:0] broadcast_req_index_d;
  logic broadcast_req_infly_q;
  logic broadcast_new_req;
  logic broadcast_req_done;
  logic broadcast_new_req_error;
  logic broadcast_rsp_is_error_q;
  logic broadcast_rsp_is_error;
  localparam logic [AdEsrShireIdSize-1:0] MemshireEsrBaseId = 8'd16;
  localparam logic [AdEsrShireIdSize-1:0] NumAvailableMinShiresId = 8'd16;
  localparam logic [axi_pkg::ScMasterIdSize-1:0] BcastAxiIdStartId = 9'd480;

  logic [AdEsrShireIdSize-1:0] broadcast_memshire_addr;
  logic [AdEsrShireIdSize-1:0] first_dest_ext;
  logic [axi_pkg::ScMasterIdSize-1:0] broadcast_axi_offset;
  logic [$clog2(BcastMaskWidth)-1:0] first_dest;

  uc_esr_broadcast_t esr_broadcast_data_q;
  uc_esr_broadcast_t esr_broadcast_data_d;
  logic esr_broadcast_data_valid_q;
  logic esr_broadcast_data_en;

  logic new_req;
  logic req_needs_storage;
  logic wr_req_needs_wait;
  logic wr_req_needs_push;
  logic [EsrBroadcastDataSize-1:0] neigh_uc_req_info_broadcast_data;
  logic [UcReqDataSize-1:0] neigh_uc_req_info_atomic_data;
  logic [UcReqDataSize-1:0] neigh_uc_req_info_unalign_data;

  logic error_req_invalid_size;
  logic error_l2scp_disabled;
  logic error_decode_broadcast_esr;
  logic error_decode_broadcast;
  logic error_decode;
  uc_decode_rsp_t error_rsp_info;
  uc_decode_rsp_t esr_rsp_info;

  uc_reqq_entry_t req_buffer_next;
  uc_reqq_valid_t [ReqqSize-1:0] req_buffer_valid_q;
  uc_reqq_valid_t [ReqqSize-1:0] req_buffer_valid_d;
  logic [ReqqSize-1:0] req_buffer_valid_entries;
  logic [ReqqSize-1:0] req_buffer_valid_enable;
  logic set_valid_new_req_en;
  uc_reqq_valid_t reqq_axi_clear;
  uc_reqq_valid_t [ReqqSize-1:0] clear_valid;
  uc_reqq_valid_t [ReqqSize-1:0] set_valid_new_req;
  logic [ReqqIndexWidth:0] first_free_ret;
  logic [ReqqIndexWidth-1:0] first_free;
  logic [ReqqIndexWidth-1:0] reqq_axi_rsp_idx;
  logic wr_reqq;
  uc_reqq_entry_t reqq_entry_axi;

  uc_reqq_entry_t wr_req_mux;
  uc_reqq_entry_t rd_req_mux;
  logic [ReqqIndexWidth-1:0] wr_req_index;
  logic wr_req_axi_ready_tosys;
  logic wr_req_axi_ready_tol3;
  logic wr_req_axi_ready;
  logic wr_req_axi_valid;
  uc_noc_master_req_t wr_req_axi_info;

  logic rd_req_axi_ready_tosys;
  logic rd_req_axi_ready_tol3;
  logic rd_req_axi_ready;
  logic rd_req_axi_valid;
  uc_noc_master_req_nodata_t rd_req_axi_info;

  logic [UcRspArbClients-1:0] pending_neigh_rsp;
  shirecache_pkg::xbar_rsp_t [UcRspArbClients-1:0] pending_neigh_rsp_data;
  shirecache_pkg::xbar_rsp_t rsp_axi_data;
  logic neigh_uc_min_rsp_accepted;
  logic [UcRspArbClients-1:0] arb_req;
  logic [UcRspArbClients-1:0] arb_selection;
  logic [$clog2(UcRspArbClients)-1:0] arb_winner;
  logic rsp_wait;
  logic rsp_axi_fifo_pop;

  assign new_req = neigh_uc_req_ready_o && neigh_uc_req_valid_i;
  assign neigh_uc_req_info_unalign_data = uc_unaligned_data(
      neigh_uc_req_info_i.req_info.data,
      neigh_uc_req_info_i.req_info.address[UcAlignBitsEnd:UcAlignBitsStart]);
  assign neigh_uc_req_info_broadcast_data = neigh_uc_req_info_unalign_data[EsrBroadcastDataSize-1:0];

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      broadcast_sends_q <= '0;
      broadcast_total_sends_q <= '0;
      broadcast_acks_q <= '0;
      broadcast_req_infly_q <= 1'b0;
      broadcast_rsp_is_error_q <= 1'b0;
      esr_broadcast_data_q <= '0;
      esr_broadcast_data_valid_q <= 1'b0;
      for (int unsigned i = 0; i < ReqqSize; i++) begin
        req_buffer_valid_q[i] <= '0;
      end
    end else begin
      if (broadcast_new_send || broadcast_new_req) begin
        broadcast_sends_q <= broadcast_sends_d;
      end
      if (broadcast_new_req) begin
        broadcast_mask_q <= broadcast_mask_d;
        broadcast_total_sends_q <= broadcast_acks_d;
        broadcast_req_index_q <= broadcast_req_index_d;
      end
      if (broadcast_new_ack || broadcast_new_req) begin
        broadcast_acks_q <= broadcast_acks_d;
      end
      if (broadcast_new_req || broadcast_req_done) begin
        broadcast_req_infly_q <= broadcast_new_req ? 1'b1 : 1'b0;
      end
      if (broadcast_rsp_is_error || broadcast_req_done) begin
        broadcast_rsp_is_error_q <= broadcast_req_done ? 1'b0 : 1'b1;
      end
      if (esr_broadcast_data_en) begin
        esr_broadcast_data_q <= esr_broadcast_data_d;
        esr_broadcast_data_valid_q <= 1'b1;
      end
      for (int unsigned i = 0; i < ReqqSize; i++) begin
        if (req_buffer_valid_enable[i]) begin
          req_buffer_valid_q[i] <= req_buffer_valid_d[i];
        end
      end
    end
  end

  assign neigh_uc_req_ready_o = first_free_ret[ReqqIndexWidth] && !error_decode_rsp_fifo_full &&
                                !rd_req_fifo_full && !wr_req_fifo_full;
  assign first_free_ret = get_free_entry(req_buffer_valid_entries);
  assign first_free = first_free_ret[ReqqIndexWidth-1:0];
  assign wr_reqq = new_req && req_needs_storage && !error_decode;

  always_comb begin
    error_rsp_info = '0;
    error_rsp_info.port_id = neigh_uc_req_info_i.port_id;
    error_rsp_info.trans_id = neigh_uc_req_info_i.trans_id;
    error_rsp_info.id = neigh_uc_req_info_i.req_info.id;
    error_rsp_info.size = neigh_uc_req_info_i.req_info.size;
    error_rsp_info.dest = neigh_uc_req_info_i.req_info.source;
    error_rsp_info.opcode = etlink_pkg::RspErr;

    error_decode_broadcast_esr = uc_is_broadcast_esr(neigh_uc_req_info_i.req_info.address) &&
                                 (neigh_uc_req_info_i.req_info.size != etlink_pkg::SizeDWord);
    error_decode_broadcast = uc_is_broadcast(neigh_uc_req_info_i.req_info.address) &&
        ((neigh_uc_req_info_i.req_info.opcode != etlink_pkg::ReqWrite) ||
         (esr_broadcast_data_q.source != neigh_uc_req_info_i.req_info.source) ||
         (esr_broadcast_data_q.port_id != neigh_uc_req_info_i.port_id) ||
         !esr_broadcast_data_valid_q ||
         (neigh_uc_req_info_i.req_info.size != etlink_pkg::SizeDWord) ||
         (neigh_uc_req_info_broadcast_data[etlink_pkg::PaSize-1:0] == '0));
    broadcast_new_req_error = new_req && (error_decode_broadcast_esr || error_decode_broadcast);
    error_req_invalid_size = neigh_uc_req_info_i.req_info.size == etlink_pkg::SizeLine;
    error_l2scp_disabled = uc_is_scp(neigh_uc_req_info_i.req_info.address) && !remote_scp_enabled_i;
    error_decode = new_req && (error_decode_broadcast_esr || error_decode_broadcast ||
                               error_req_invalid_size || error_l2scp_disabled);
  end

  always_comb begin
    req_buffer_next = '0;
    set_valid_new_req = '0;
    req_needs_storage = !uc_is_broadcast_esr(neigh_uc_req_info_i.req_info.address);
    esr_broadcast_data_en = new_req && uc_is_broadcast_esr(neigh_uc_req_info_i.req_info.address);

    req_buffer_next.id      = neigh_uc_req_info_i.req_info.id;
    req_buffer_next.opcode  = neigh_uc_req_info_i.req_info.opcode;
    req_buffer_next.resp    = !(neigh_uc_req_info_i.req_info.opcode == etlink_pkg::ReqAtomic);
    req_buffer_next.source  = neigh_uc_req_info_i.req_info.source;
    req_buffer_next.use_l3  = ((neigh_uc_req_info_i.req_info.address[etlink_pkg::PaSize-1:AdDdrRegionStart] == AdDdrRegion) && l3_enabled_i) ||
                              (uc_is_scp(neigh_uc_req_info_i.req_info.address) && remote_scp_enabled_i);
    req_buffer_next.port_id = neigh_uc_req_info_i.port_id;
    req_buffer_next.trans_id = neigh_uc_req_info_i.trans_id;
    req_buffer_next.size = neigh_uc_req_info_i.req_info.size;
    req_buffer_next.bcast = uc_is_broadcast(neigh_uc_req_info_i.req_info.address);

    set_valid_new_req_en = new_req && !error_decode && first_free_ret[ReqqIndexWidth] && req_needs_storage;
    set_valid_new_req[first_free].wait_ack = set_valid_new_req_en;
    set_valid_new_req[first_free].wait_from_sys = set_valid_new_req_en &&
                                                  uc_is_atomic(neigh_uc_req_info_i.req_info.opcode);

    neigh_uc_req_info_atomic_data = '0;
    neigh_uc_req_info_atomic_data[255:0] = neigh_uc_req_info_i.req_info.data[255:0];
    neigh_uc_req_info_atomic_data[UcReqAtmDataSubopLsb +: etlink_pkg::SubopcodeSize] =
        neigh_uc_req_info_i.req_info.subopcode;
    neigh_uc_req_info_atomic_data[UcReqAtmDataShireIdLsb +: NumShireIdsBits] = shire_id_i;
    neigh_uc_req_info_atomic_data[UcReqAtmDataAlgnBitsLsb +: UcAlignBitsSize] =
        neigh_uc_req_info_i.req_info.address[UcAlignBitsEnd:UcAlignBitsStart];
    neigh_uc_req_info_atomic_data[UcReqAtmDataReqqIdxLsb +: ReqqIndexWidth] = first_free;

    if (uc_is_broadcast(neigh_uc_req_info_i.req_info.address)) begin
      req_buffer_next.data = '0;
      req_buffer_next.data[EsrBroadcastDataSize-1:0] = esr_broadcast_data_q.data;
    end else if (uc_is_msg(neigh_uc_req_info_i.req_info.opcode)) begin
      req_buffer_next.data = neigh_uc_req_info_i.req_info.data[UcReqDataSize-1:0];
    end else if (uc_is_atomic(neigh_uc_req_info_i.req_info.opcode)) begin
      req_buffer_next.data = neigh_uc_req_info_atomic_data;
    end else begin
      req_buffer_next.data = neigh_uc_req_info_unalign_data;
    end

    if (uc_is_broadcast(neigh_uc_req_info_i.req_info.address)) begin
      req_buffer_next.address = '0;
      req_buffer_next.address[21:0] = {neigh_uc_req_info_broadcast_data[58:40], 3'b000};
      req_buffer_next.address[AdEsrProtBitsStart +: AdEsrProtBitsSize] =
          neigh_uc_req_info_i.req_info.address[AdEsrProtBitsStart +: AdEsrProtBitsSize];
      req_buffer_next.address[etlink_pkg::PaSize-1:AdEsrRegionStart] = AdEsrRegion;
      req_buffer_next.data[BcastMaskStoreLo +: BcastMaskWidth] =
          neigh_uc_req_info_broadcast_data[BcastMaskHi:BcastMaskLo];
    end else begin
      req_buffer_next.address = neigh_uc_req_info_i.req_info.address;
      req_buffer_next.address[UcAlignBitsEnd:UcAlignBitsStart] =
          uc_is_atomic(neigh_uc_req_info_i.req_info.opcode) ? '0 :
          neigh_uc_req_info_i.req_info.address[UcAlignBitsEnd:UcAlignBitsStart];
      if ((neigh_uc_req_info_i.req_info.address[etlink_pkg::PaSize-1:AdEsrRegionStart] == AdEsrRegion) &&
          (neigh_uc_req_info_i.req_info.address[AdEsrShireIdStart +: AdEsrShireIdSize] == LocalShireId)) begin
        req_buffer_next.address[AdEsrShireIdStart +: AdEsrShireIdSize] = shire_id_i;
      end
    end

    wr_req_index_push = first_free;
    wr_req_needs_push = req_needs_storage;
    wr_req_fifo_push = new_req && wr_req_needs_push && !error_decode && neigh_uc_req_info_i.req_info.wdata;
    rd_req_index_push = first_free;
    rd_req_fifo_push = new_req && !error_decode && !neigh_uc_req_info_i.req_info.wdata;
  end

  always_comb begin
    esr_broadcast_data_d = '0;
    esr_broadcast_data_d.source = neigh_uc_req_info_i.req_info.source;
    esr_broadcast_data_d.port_id = neigh_uc_req_info_i.port_id;
    esr_broadcast_data_d.data = neigh_uc_req_info_broadcast_data[EsrBroadcastDataSize-1:0];
  end

  always_comb begin
    esr_rsp_info = '0;
    esr_rsp_info.port_id = neigh_uc_req_info_i.port_id;
    esr_rsp_info.trans_id = neigh_uc_req_info_i.trans_id;
    esr_rsp_info.id = neigh_uc_req_info_i.req_info.id;
    esr_rsp_info.size = neigh_uc_req_info_i.req_info.size;
    esr_rsp_info.dest = neigh_uc_req_info_i.req_info.source;
    esr_rsp_info.opcode = etlink_pkg::RspAck;
    error_decode_rsp_info_push = error_decode ? error_rsp_info : esr_rsp_info;
    error_decode_rsp_fifo_push = error_decode || (esr_broadcast_data_en && !broadcast_new_req_error);
  end

  always_comb begin
    clear_valid = '0;
    clear_valid[reqq_axi_rsp_idx] = reqq_axi_clear;
    for (int unsigned i = 0; i < ReqqSize; i++) begin
      req_buffer_valid_d[i] = (req_buffer_valid_q[i] & ~clear_valid[i]) | set_valid_new_req[i];
      req_buffer_valid_entries[i] = |req_buffer_valid_q[i];
      req_buffer_valid_enable[i] = (|clear_valid[i]) || (|set_valid_new_req[i]);
    end
  end

  always_comb begin
    wr_req_index = wr_req_index_pop;
    wr_req_axi_ready = !wr_req_fifo_not_empty ||
                       (wr_req_axi_ready_tol3 && wr_req_mux.use_l3) ||
                       (wr_req_axi_ready_tosys && !wr_req_mux.use_l3);
    wr_req_needs_wait = broadcast_req_infly_q && wr_req_mux.bcast &&
                        (broadcast_req_index_q != wr_req_index);
    wr_req_fifo_pop = (wr_req_axi_ready && wr_req_fifo_not_empty && !wr_req_mux.bcast) ||
                      (wr_req_axi_ready && wr_req_fifo_not_empty && wr_req_mux.bcast &&
                       broadcast_req_infly_q && (broadcast_sends_q == {{BcastSendsSize-1{1'b0}}, 1'b1})) ||
                      (wr_req_axi_ready && wr_req_fifo_not_empty && wr_req_mux.bcast &&
                       !broadcast_req_infly_q && (broadcast_sends_d == '0));
    wr_req_axi_valid = wr_req_axi_ready && !wr_req_needs_wait && wr_req_fifo_not_empty;

    wr_req_axi_info = '0;
    wr_req_axi_info.size = wr_req_mux.size;
    wr_req_axi_info.axi_id = {{(axi_pkg::ScMasterIdSize-ReqqIndexWidth){1'b0}}, wr_req_index};
    wr_req_axi_info.opcode = wr_req_mux.opcode;
    wr_req_axi_info.data = '0;
    wr_req_axi_info.data[UcReqDataSize-1:0] = wr_req_mux.data;
    wr_req_axi_info.address = wr_req_mux.address;
    wr_req_axi_info.l3_en = wr_req_mux.use_l3;

    broadcast_new_send = wr_req_axi_valid && wr_req_axi_ready && wr_req_mux.bcast;
    broadcast_new_req = wr_req_mux.bcast && !broadcast_req_infly_q && wr_req_fifo_not_empty;
    broadcast_req_index_d = wr_req_index_pop;
    broadcast_mask_d = wr_req_mux.data[BcastMaskStoreLo +: BcastMaskWidth];
    first_dest_ext = {{(AdEsrShireIdSize-$bits(first_dest)){1'b0}}, first_dest};
    broadcast_memshire_addr = MemshireEsrBaseId + first_dest_ext;
    broadcast_axi_offset = {{(axi_pkg::ScMasterIdSize-BcastSendsSize){1'b0}},
                            (broadcast_total_sends_q - broadcast_sends_q)};

    if (wr_req_mux.bcast) begin
      wr_req_axi_info.address[AdEsrShireIdStart +: AdEsrShireIdSize] =
          (first_dest_ext < NumAvailableMinShiresId) ? first_dest_ext : broadcast_memshire_addr;
      wr_req_axi_info.axi_id = broadcast_req_infly_q ?
          (BcastAxiIdStartId + broadcast_axi_offset) : BcastAxiIdStartId;
    end

    rd_req_axi_ready = !rd_req_fifo_not_empty ||
                       (rd_req_axi_ready_tol3 && rd_req_mux.use_l3) ||
                       (rd_req_axi_ready_tosys && !rd_req_mux.use_l3);
    rd_req_fifo_pop = rd_req_axi_ready && rd_req_fifo_not_empty;
    rd_req_axi_valid = rd_req_axi_ready && rd_req_fifo_not_empty;
    rd_req_axi_info = '0;
    rd_req_axi_info.axi_id = {{(axi_pkg::ScMasterIdSize-ReqqIndexWidth){1'b0}}, rd_req_index_pop};
    rd_req_axi_info.address = rd_req_mux.address;
    rd_req_axi_info.size = rd_req_mux.size;
    rd_req_axi_info.opcode = rd_req_mux.opcode;
    rd_req_axi_info.l3_en = rd_req_mux.use_l3;
  end

  always_comb begin
    if (broadcast_req_infly_q) begin
      broadcast_sends_d = broadcast_sends_q - {{BcastSendsSize-1{1'b0}}, 1'b1};
      broadcast_acks_d = broadcast_acks_q - {{BcastSendsSize-1{1'b0}}, 1'b1};
      first_dest = first_destination(broadcast_mask_q, broadcast_sends_q);
    end else begin
      broadcast_acks_d = broadcast_count_destinations(broadcast_mask_d);
      broadcast_sends_d = broadcast_acks_d - {{BcastSendsSize-1{1'b0}}, broadcast_new_send};
      first_dest = first_destination(broadcast_mask_d, broadcast_sends_d + {{BcastSendsSize-1{1'b0}}, broadcast_new_send});
    end
  end

  always_comb begin
    pending_neigh_rsp_data[AxiFifoIdx] = rsp_axi_data;
    pending_neigh_rsp_data[ErrorFifoIdx] = '0;
    pending_neigh_rsp_data[ErrorFifoIdx].port_id = error_decode_rsp_info_pop.port_id;
    pending_neigh_rsp_data[ErrorFifoIdx].trans_id = error_decode_rsp_info_pop.trans_id;
    pending_neigh_rsp_data[ErrorFifoIdx].src_is_uc = 1'b0;
    pending_neigh_rsp_data[ErrorFifoIdx].rsp_info.opcode = error_decode_rsp_info_pop.opcode;
    pending_neigh_rsp_data[ErrorFifoIdx].rsp_info.id = error_decode_rsp_info_pop.id;
    pending_neigh_rsp_data[ErrorFifoIdx].rsp_info.size = error_decode_rsp_info_pop.size;
    pending_neigh_rsp_data[ErrorFifoIdx].rsp_info.dest = error_decode_rsp_info_pop.dest;
    pending_neigh_rsp_data[ErrorFifoIdx].rsp_info.wdata = 1'b0;
    pending_neigh_rsp_data[ErrorFifoIdx].rsp_info.qwen = 4'b0000;
    pending_neigh_rsp_data[ErrorFifoIdx].rsp_info.data = '0;
  end

  always_comb begin
    neigh_uc_min_rsp_valid_o = '0;
    neigh_uc_min_rsp_valid_o[neigh_uc_min_rsp_info_o.port_id[PortRspIndexWidth-1:0]] = arb_selection[arb_winner];
  end

  assign neigh_uc_min_rsp_accepted = |(neigh_uc_min_rsp_valid_o & neigh_uc_min_rsp_ready_i);
  assign error_decode_rsp_fifo_pop = neigh_uc_min_rsp_accepted && arb_selection[ErrorFifoIdx];
  assign rsp_axi_fifo_pop = neigh_uc_min_rsp_accepted && arb_selection[AxiFifoIdx];
  assign arb_req = pending_neigh_rsp;
  assign neigh_uc_min_rsp_info_o = pending_neigh_rsp_data[arb_winner];
  assign rsp_wait = |neigh_uc_min_rsp_valid_o && !neigh_uc_min_rsp_accepted;

  prim_fifo_reg #(.Width(ReqqIndexWidth), .Depth(ReqqSize)) u_read_req_index_fifo (
    .clk_i(clk_i), .rst_ni(rst_ni), .push_i(rd_req_fifo_push), .wdata_i(rd_req_index_push),
    .full_o(rd_req_fifo_full), .pop_i(rd_req_fifo_pop), .valid_o(rd_req_fifo_not_empty),
    .rdata_o(rd_req_index_pop), .level_o(rd_req_fifo_level_unused));

  prim_fifo_reg #(.Width(ReqqIndexWidth), .Depth(ReqqSize)) u_write_req_index_fifo (
    .clk_i(clk_i), .rst_ni(rst_ni), .push_i(wr_req_fifo_push), .wdata_i(wr_req_index_push),
    .full_o(wr_req_fifo_full), .pop_i(wr_req_fifo_pop), .valid_o(wr_req_fifo_not_empty),
    .rdata_o(wr_req_index_pop), .level_o(wr_req_fifo_level_unused));

  prim_fifo_reg #(.Width($bits(uc_decode_rsp_t)), .Depth(ReqqSize)) u_decode_rsp_fifo (
    .clk_i(clk_i), .rst_ni(rst_ni), .push_i(error_decode_rsp_fifo_push),
    .wdata_i(error_decode_rsp_info_push), .full_o(error_decode_rsp_fifo_full),
    .pop_i(error_decode_rsp_fifo_pop), .valid_o(pending_neigh_rsp[ErrorFifoIdx]),
    .rdata_o(error_decode_rsp_info_pop), .level_o(error_decode_rsp_fifo_level_unused));

  prim_rf_3r2w #(.Width($bits(uc_reqq_entry_t)), .Entries(ReqqSize), .Level2CkGate(1)) u_uc_reqq (
    .clk_i(clk_i),
    .rd_addr_a_i(wr_req_index_pop), .rd_data_a_o(wr_req_mux),
    .rd_addr_b_i(rd_req_index_pop), .rd_data_b_o(rd_req_mux),
    .rd_addr_c_i(reqq_axi_rsp_idx), .rd_data_c_o(reqq_entry_axi),
    .wr_en_a_i(wr_reqq), .wr_addr_a_i(first_free), .wr_data_a_en_1p_i(1'b1), .wr_data_a_i(req_buffer_next),
    .wr_en_b_i(1'b0), .wr_addr_b_i('0), .wr_data_b_en_1p_i(1'b0), .wr_data_b_i('0)
  );

  uncacheable_noc_rsp #(.ReqqSize(ReqqSize)) u_axi_rsp_handlers (
    .clk_i(clk_i), .rst_ni(rst_ni),
    .rsp_data_o(rsp_axi_data),
    .rsp_valid_o(pending_neigh_rsp[AxiFifoIdx]),
    .rsp_send_i(rsp_axi_fifo_pop),
    .reqq_entry_i(reqq_entry_axi),
    .reqq_index_o(reqq_axi_rsp_idx),
    .reqq_entry_valid_i(|req_buffer_valid_q[reqq_axi_rsp_idx]),
    .reqq_entry_clear_o(reqq_axi_clear),
    .broadcast_new_ack_o(broadcast_new_ack),
    .broadcast_req_done_o(broadcast_req_done),
    .broadcast_rsp_is_error_o(broadcast_rsp_is_error),
    .broadcast_rsp_is_error_reg_i(broadcast_rsp_is_error_q || broadcast_rsp_is_error),
    .broadcast_acks_i(broadcast_acks_q),
    .broadcast_req_index_i(broadcast_req_index_q),
    .credit_i(credit_i),
    .credit_return_o(credit_return_o),
    .from_sys_aw_i(from_sys_aw_i),
    .from_sys_aw_valid_i(from_sys_aw_valid_i),
    .from_sys_aw_ready_o(from_sys_aw_ready_o),
    .from_sys_w_i(from_sys_w_i),
    .from_sys_w_valid_i(from_sys_w_valid_i),
    .from_sys_w_ready_o(from_sys_w_ready_o),
    .from_sys_b_o(from_sys_b_o),
    .from_sys_b_valid_o(from_sys_b_valid_o),
    .from_sys_b_ready_i(from_sys_b_ready_i),
    .remote_scp_enabled_i(remote_scp_enabled_i),
    .l3_enabled_i(l3_enabled_i),
    .to_l3_b_i(to_l3_b_i),
    .to_l3_b_valid_i(to_l3_b_valid_i),
    .to_l3_b_ready_o(to_l3_b_ready_o),
    .to_l3_r_i(to_l3_r_i),
    .to_l3_r_valid_i(to_l3_r_valid_i),
    .to_l3_r_ready_o(to_l3_r_ready_o),
    .to_sys_b_i(to_sys_b_i),
    .to_sys_b_valid_i(to_sys_b_valid_i),
    .to_sys_b_ready_o(to_sys_b_ready_o),
    .to_sys_r_i(to_sys_r_i),
    .to_sys_r_valid_i(to_sys_r_valid_i),
    .to_sys_r_ready_o(to_sys_r_ready_o)
  );

  uncached_arb_lru_grant #(.NumClients(UcRspArbClients)) u_minion_response_arb (
    .clk_i(clk_i), .rst_ni(rst_ni), .bid_i(arb_req), .stall_i(rsp_wait),
    .grant_o(arb_selection), .winner_o(arb_winner));

  noc_etlink_to_axi u_noc_etlink_to_axi (
    .uc_config_i(uc_config_i),
    .wr_req_axi_ready_tol3_o(wr_req_axi_ready_tol3),
    .wr_req_axi_ready_tosys_o(wr_req_axi_ready_tosys),
    .wr_req_axi_valid_i(wr_req_axi_valid),
    .wr_req_axi_info_i(wr_req_axi_info),
    .rd_req_axi_ready_tol3_o(rd_req_axi_ready_tol3),
    .rd_req_axi_ready_tosys_o(rd_req_axi_ready_tosys),
    .rd_req_axi_valid_i(rd_req_axi_valid),
    .rd_req_axi_info_i(rd_req_axi_info),
    .to_sys_ar_o(to_sys_ar_o), .to_sys_ar_valid_o(to_sys_ar_valid_o), .to_sys_ar_ready_i(to_sys_ar_ready_i),
    .to_sys_aw_o(to_sys_aw_o), .to_sys_aw_valid_o(to_sys_aw_valid_o), .to_sys_aw_ready_i(to_sys_aw_ready_i),
    .to_sys_w_o(to_sys_w_o), .to_sys_w_valid_o(to_sys_w_valid_o), .to_sys_w_ready_i(to_sys_w_ready_i),
    .to_l3_ar_o(to_l3_ar_o), .to_l3_ar_valid_o(to_l3_ar_valid_o), .to_l3_ar_ready_i(to_l3_ar_ready_i),
    .to_l3_aw_o(to_l3_aw_o), .to_l3_aw_valid_o(to_l3_aw_valid_o), .to_l3_aw_ready_i(to_l3_aw_ready_i),
    .to_l3_w_o(to_l3_w_o), .to_l3_w_valid_o(to_l3_w_valid_o), .to_l3_w_ready_i(to_l3_w_ready_i)
  );

  function automatic logic [ReqqIndexWidth:0] get_free_entry(input logic [ReqqSize-1:0] valid_entries);
    logic found;
    begin
      get_free_entry = '0;
      found = 1'b0;
      for (int unsigned it = 0; it < ReqqSize; it++) begin
        if (!found && (valid_entries[it] == 1'b0)) begin
          get_free_entry[ReqqIndexWidth-1:0] = it[ReqqIndexWidth-1:0];
          get_free_entry[ReqqIndexWidth] = 1'b1;
          found = 1'b1;
        end
      end
    end
  endfunction

  function automatic logic [$clog2(BcastMaskWidth)-1:0] first_destination(
    input logic [BcastMaskWidth-1:0] mask,
    input logic [$clog2(BcastMaskWidth)-1:0] counter
  );
    logic [$clog2(BcastMaskWidth)-1:0] counter_skip;
    begin
      first_destination = '0;
      counter_skip = counter;
      for (int it = BcastMaskWidth - 1; it >= 0; it--) begin
        if (mask[it]) begin
          counter_skip = counter_skip - {{($clog2(BcastMaskWidth)-1){1'b0}}, 1'b1};
          if (counter_skip == '0) begin
            first_destination = it[$clog2(BcastMaskWidth)-1:0];
            break;
          end
        end
      end
    end
  endfunction

  function automatic logic [BcastSendsSize-1:0] broadcast_count_destinations(
    input logic [BcastMaskWidth-1:0] broadcast_mask
  );
    logic [BcastSendsSize-1:0] counter;
    begin
      counter = '0;
      for (int unsigned i = BcastMaskLo; i <= BcastMaskHi; i++) begin
        counter = counter + {{BcastSendsSize-1{1'b0}}, broadcast_mask[i]};
      end
      broadcast_count_destinations = counter;
    end
  endfunction

  /* verilator lint_off UNUSEDSIGNAL */
  logic unused_bits;
  assign unused_bits = ^{rd_req_fifo_level_unused, wr_req_fifo_level_unused,
                         error_decode_rsp_fifo_level_unused, rd_req_mux,
                         wr_req_mux.id, wr_req_mux.source, wr_req_mux.port_id,
                         wr_req_mux.trans_id, wr_req_mux.resp, reqq_entry_axi,
                         from_sys_aw_i.cache, from_sys_aw_i.lock, from_sys_aw_i.prot,
                         from_sys_aw_i.qos, from_sys_w_i.last, from_sys_w_i.strb,
                         neigh_uc_req_info_i.req_info.qwen, rsp_wait};
  /* verilator lint_on UNUSEDSIGNAL */

endmodule : uncacheable_noc
