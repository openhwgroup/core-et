`include "rtl/inc/soc.vh"

`ifndef __EVL_SHIRE_CACHE_PKG
   `include "dv/shire_common/shire_cache/shire_cache_vc/evl_shire_cache_pkg.sv"
`endif

`include "dv/common/apb/apb_vc/evl_apb_pkg.sv"

//--------------------------------------------------------------------------------------------------
//
// This file is intended to be included in the Shire Cache RTL top-level module.  It includes
// DV-specific objects and methods for the Esperanto Verification Library.
//
module evl_shire_cache_interface #(parameter int NUM_BANKS           = 1,
                                   parameter int NUM_NEIGH           = `NUM_NEIGH,
                                   parameter int NUM_L3_MASTER_PORTS = `SC_L3_MASTER_PORTS) (
      input reset,
      input clock
   );

   import uvm_pkg::*;
   import evl_base_pkg::*;
   import evl_shire_cache_types_pkg::evl_sc_reqq_opcode_t;
   import evl_shire_cache_types_pkg::evl_sc_reqq_orig_opcode_t;
   import evl_shire_cache_types_pkg::evl_sc_pipe_err_rsp_t;
   import evl_shire_cache_pkg::*;
   typedef my_shire_cache_rtl_port;

   localparam int NUM_NEIGH_ID_MAX = $clog2(NUM_NEIGH);
   localparam int L3_PORT_1        = (NUM_L3_MASTER_PORTS > 1) ? 1 : 0;
   localparam int L3_PORT_2        = (NUM_L3_MASTER_PORTS > 2) ? 2 : 0;
   localparam int L3_PORT_3        = (NUM_L3_MASTER_PORTS > 3) ? 3 : 0;


   // PEEK #3: TC
   string                      abstract_name          = "";
   int                         m_sc_sb_en             = 1;
   reg                         start_monitor          = 1'b0;
   reg                         mon_reset_on           = 1'b0;
   reg                         mon_reset_off          = 1'b0;
   reg                         mon_pipe_signal_done   = 1'b0;
   reg  [NUM_BANKS-1:0]        mon_pipe_tc_signals    = {NUM_BANKS{1'b0}};
   wire [NUM_BANKS-1:0]        mon_pipe_signal_hi;
   wire [NUM_BANKS-1:0]        mon_pipe_signal_lo;
   wire [NUM_BANKS-1:0]        mon_pipe_tc_req_valid;
   wire [NUM_BANKS-1:0]        mon_pipe_tc_rsp_valid;
   wire [NUM_BANKS-1:0]        mon_pipe_idx_cop_l2_busy;
   wire [NUM_BANKS-1:0]        mon_pipe_idx_cop_l3_busy;
   wire [NUM_BANKS-1:0]        mon_pipe_idx_cop_req_ready;
   wire [NUM_BANKS-1:0]        mon_pipe_idx_cop_req_valid;
   wire [NUM_BANKS-1:0]        mon_pipe_idx_cop_cb_inv_start;

   wire sc_reqq_pipe_req_t     mon_pipe_tc_pipe_req_info          [NUM_BANKS];
   wire sc_pipe_info_t         mon_pipe_tc_pipe_info              [NUM_BANKS];
   wire sc_pipe_tag_rsp_t      mon_pipe_tc_rsp_info               [NUM_BANKS];
   wire sc_pipe_idx_cop_req_t  mon_pipe_idx_cop_req_info          [NUM_BANKS];
   wire [`SC_REQQ_DEPTH-1:0]   mon_dealloc_ag                     [NUM_BANKS];
   wire [`SC_REQQ_DEPTH-1:0]   mon_reqq_l3_entries                [NUM_BANKS];
   wire                        mon_alloc_l2_valid_ag              [NUM_BANKS];
   wire                        mon_alloc_l3_valid_ag              [NUM_BANKS];
   wire [`SC_REQQ_DEPTH-1:0]   mon_alloc_l2_1hot_ag               [NUM_BANKS];
   wire [`SC_REQQ_DEPTH-1:0]   mon_alloc_l3_1hot_ag               [NUM_BANKS];
   reg  [`SC_REQQ_DEPTH-1:0]   mon_alloc_scp_valid_r              [NUM_BANKS];
   reg  [`SC_REQQ_DEPTH-1:0]   mon_alloc_l3_idx_cop_valid_r       [NUM_BANKS];
   wire sc_reqq_alloc_t        mon_alloc_l2_info_ag               [NUM_BANKS];
   wire sc_reqq_alloc_t        mon_alloc_l3_info_ag               [NUM_BANKS];
   wire sc_reqq_alloc_t        mon_alloc_l2_info_paired_ag        [NUM_BANKS];
   wire sc_reqq_alloc_t        mon_alloc_l3_info_paired_ag        [NUM_BANKS];
   wire                        mon_alloc_l2_alloc_valid_paired_ag [NUM_BANKS];
   wire                        mon_alloc_l3_alloc_valid_paired_ag [NUM_BANKS];
   wire                        mon_alloc_l2_2nd_half_needed_ad    [NUM_BANKS];
   wire                        mon_alloc_l3_2nd_half_needed_ad    [NUM_BANKS];
   wire sc_reqq_bank_req_t     mon_alloc_l2_bank_info_ag          [NUM_BANKS];
   wire sc_reqq_bank_req_t     mon_alloc_l3_bank_info_ag          [NUM_BANKS];
   wire sc_bank_esr_ctl_t      mon_esr_ctl                        [NUM_BANKS];
        sc_bank_esr_ctl_t      mon_esr_ctl_r                      [NUM_BANKS];
   wire [NUM_NEIGH_ID_MAX-1:0] mon_neigh_id_0;
   wire [NUM_NEIGH_ID_MAX-1:0] mon_neigh_id_1;
   wire [NUM_NEIGH_ID_MAX-1:0] mon_neigh_id_2;
   wire [NUM_NEIGH_ID_MAX-1:0] mon_neigh_id_3;
   bit  [NUM_NEIGH_ID_MAX-1:0] mon_neigh_id_0_r;
   bit  [NUM_NEIGH_ID_MAX-1:0] mon_neigh_id_1_r;
   bit  [NUM_NEIGH_ID_MAX-1:0] mon_neigh_id_2_r;
   bit  [NUM_NEIGH_ID_MAX-1:0] mon_neigh_id_3_r;
   wire esr_shire_config_t     mon_esr_shire_config;
   wire [NUM_BANKS-1:0]        mon_pipe_rbuf_rd_req_valid;
   wire [NUM_BANKS-1:0]        mon_pipe_rbuf_rd_req_ready;
   wire sc_pipe_rbuf_rd_req_t  mon_pipe_rbuf_rd_req_info          [NUM_BANKS];
   wire [NUM_BANKS-1:0]        mon_cbuf_matches_tag_victim;
   wire [NUM_BANKS-1:0]        mon_pipe_cbuf_inv_req_valid;
   wire [NUM_BANKS-1:0]        mon_pipe_cbuf_req_valid;
   wire sc_pipe_cbuf_req_t     mon_pipe_cbuf_req_info             [NUM_BANKS];
   wire [NUM_BANKS-1:0]        mon_pipe_cbuf_rsp_valid;
   wire sc_pipe_cbuf_rsp_t     mon_pipe_cbuf_rsp_info             [NUM_BANKS];

   bit  [31:0]                 m_cycle_count      = 32'h00000000;
   reg  [NUM_BANKS-1:0]        m_send_cbuf_status = {NUM_BANKS{1'b0}};


   //
   // EVL Proxy Code
   //
   bit                     m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), SHIRE_CACHE_RTL_AGENT, ~evl_agent_id_t'(0), "evl_shire_cache_env", "", "", "", { 1 });
   my_shire_cache_rtl_port m_rtl_port    = new($sformatf("%m"), { 1 });
   evl_cache_index_config  m_index_config[int];


   //
   // This small block of code is used to prevent any evaluation at time 0.  Because an x->1
   // transition is considered a positive edge in Verilog, this block forces us to see a ->0
   // transition first (this only applies if the clock is asserted at time 0).
   //
   initial begin
      if (`et_value_plusargs("sc_sb_en=%d", m_sc_sb_en) == 0) begin
         m_sc_sb_en = 1;
      end
      @(negedge clock);
      m_index_config[int'(SC_RAM_TYPE_SC)] = new();
      m_index_config[int'(SC_RAM_TYPE_L2)] = new();
      m_index_config[int'(SC_RAM_TYPE_L3)] = new();
      start_monitor <= 1'b1;
      @(posedge clock);
      abstract_name = m_rtl_port.get_abstract_name();
   end


   //
   // Monitor the signals here and send appropriate info to the RTL port proxy.
   //
   genvar gen_it, gen_in;

   //ESR neigh0/1/2/3_id have dissapeared!
   //assign mon_neigh_id_0 = shire_cache.esr_shire_config.neigh0_id;
   //assign mon_neigh_id_1 = shire_cache.esr_shire_config.neigh1_id;
   //assign mon_neigh_id_2 = shire_cache.esr_shire_config.neigh2_id;
   //assign mon_neigh_id_3 = shire_cache.esr_shire_config.neigh3_id;
   assign mon_neigh_id_0 = 2'd0;
   assign mon_neigh_id_1 = 2'd1;
   assign mon_neigh_id_2 = 2'd2;
   assign mon_neigh_id_3 = 2'd3;

   bit                     dv_reset_esr_ctl_captured_r = 1'b0;

   wire [NUM_BANKS-1:0]      mon_atomic_valid;
   wire [`EVL_PADDR_MSB:0]   mon_atomic_addr[NUM_BANKS];
   wire [5:0]                mon_atomic_opcode[NUM_BANKS];
   wire [5:0]                mon_atomic_reqq_id[NUM_BANKS];
   wire [NUM_BANKS-1:0]      mon_atomic_l2;
   wire [NUM_BANKS-1:0]      mon_atomic_l3;
   wire [7:0]                mon_atomic_l3_source[NUM_BANKS];
   wire [4:0]                mon_atomic_l3_trid[NUM_BANKS];
   wire [5:0]                mon_rsp_reqq_id[NUM_BANKS];
   wire [NUM_BANKS-1:0]      mon_rsp_reqq_id_vld;
   wire [7:0]                mon_rsp_dst_pop[NUM_BANKS];

   localparam int lp_bank_1    = (`SC_BANKS > 1) ? 1 : 0;
   localparam int lp_bank_2    = (`SC_BANKS > 2) ? 2 : 0;
   localparam int lp_bank_3    = (`SC_BANKS > 3) ? 3 : 0;
   localparam int lp_bank_4    = (`SC_BANKS > 4) ? 4 : 0;
   localparam int lp_bank_5    = (`SC_BANKS > 5) ? 5 : 0;
   localparam int lp_bank_6    = (`SC_BANKS > 6) ? 6 : 0;
   localparam int lp_bank_7    = (`SC_BANKS > 7) ? 7 : 0;

   localparam int lp_subbank_1 = (`SC_SUB_BANKS > 1) ? 1 : 0;
   localparam int lp_subbank_2 = (`SC_SUB_BANKS > 2) ? 2 : 0;
   localparam int lp_subbank_3 = (`SC_SUB_BANKS > 3) ? 3 : 0;
   localparam int lp_subbank_4 = (`SC_SUB_BANKS > 4) ? 4 : 0;
   localparam int lp_subbank_5 = (`SC_SUB_BANKS > 5) ? 5 : 0;
   localparam int lp_subbank_6 = (`SC_SUB_BANKS > 6) ? 6 : 0;
   localparam int lp_subbank_7 = (`SC_SUB_BANKS > 7) ? 7 : 0;

   localparam int lp_way_1     = (`SC_WAYS > 1) ? 1 : 0;
   localparam int lp_way_2     = (`SC_WAYS > 2) ? 2 : 0;
   localparam int lp_way_3     = (`SC_WAYS > 3) ? 3 : 0;
   localparam int lp_way_4     = (`SC_WAYS > 4) ? 4 : 0;
   localparam int lp_way_5     = (`SC_WAYS > 5) ? 5 : 0;
   localparam int lp_way_6     = (`SC_WAYS > 6) ? 6 : 0;
   localparam int lp_way_7     = (`SC_WAYS > 7) ? 7 : 0;

   generate
      `ifndef ET_USE_BIST_RTL
         for (gen_it = 0; gen_it < NUM_BANKS; gen_it++) begin : pipe_atomic
            assign mon_atomic_valid     [gen_it] = shire_cache.banks[ gen_it].bank.pipe.sub_bank.atomic.pipe_valid_d0;
            assign mon_atomic_addr      [gen_it] = shire_cache.banks[ gen_it].bank.pipe.sub_bank.atomic.pipe_req_info.address;
            assign mon_atomic_opcode    [gen_it] = shire_cache.banks[ gen_it].bank.pipe.sub_bank.atomic.pipe_req_info.opcode;
            assign mon_atomic_reqq_id   [gen_it] = shire_cache.banks[ gen_it].bank.pipe.sub_bank.atomic.pipe_req_info.reqq_id;
            assign mon_atomic_l2        [gen_it] = (mon_atomic_opcode[gen_it] == int'(SC_L2_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3        [gen_it] = (mon_atomic_opcode[gen_it] == int'(SC_L3_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3_source [gen_it] = shire_cache.banks[ gen_it].bank.pipe.sub_bank.atomic.atomic_routing_d0[20:13];
            assign mon_atomic_l3_trid   [gen_it] = shire_cache.banks[ gen_it].bank.pipe.sub_bank.atomic.atomic_routing_d0[25:21];

            assign mon_rsp_reqq_id      [gen_it] = shire_cache.banks[ gen_it].bank.rspmux.bank_rsp_info_full.reqq_id;
            assign mon_rsp_reqq_id_vld  [gen_it] = shire_cache.banks[ gen_it].bank.rspmux.rspmux_trace_sent.valid;
            for (gen_in = 0; gen_in < 8; gen_in++) begin : dst_pop
               if (gen_in < NUM_NEIGH) begin : dst_pop_a
                  assign mon_rsp_dst_pop[gen_it][gen_in] = shire_cache.rsp_xbar.dest_fifo_pop[gen_in][gen_it];
               end
               else begin : dst_pop_b
                  assign mon_rsp_dst_pop[gen_it][gen_in] = 1'b0;
               end
            end
         end
      `else
         assign mon_atomic_valid     [0] = shire_cache.\banks[0] .bank.pipe.sub_bank.atomic.pipe_valid_d0;
         assign mon_atomic_addr      [0] = shire_cache.\banks[0] .bank.pipe.sub_bank.atomic.pipe_req_info.address;
         assign mon_atomic_opcode    [0] = shire_cache.\banks[0] .bank.pipe.sub_bank.atomic.pipe_req_info.opcode;
         assign mon_atomic_reqq_id   [0] = shire_cache.\banks[0] .bank.pipe.sub_bank.atomic.pipe_req_info.reqq_id;
         assign mon_atomic_l2        [0] = (mon_atomic_opcode[0] == int'(SC_L2_Atomic)) ? 1'b1 : 1'b0;
         assign mon_atomic_l3        [0] = (mon_atomic_opcode[0] == int'(SC_L3_Atomic)) ? 1'b1 : 1'b0;
         assign mon_atomic_l3_source [0] = shire_cache.\banks[0] .bank.pipe.sub_bank.atomic.atomic_routing_d0[20:13];
         assign mon_atomic_l3_trid   [0] = shire_cache.\banks[0] .bank.pipe.sub_bank.atomic.atomic_routing_d0[25:21];

         assign mon_rsp_reqq_id      [0] = shire_cache.\banks[0] .bank.rspmux.bank_rsp_info_full.reqq_id;
         assign mon_rsp_reqq_id_vld  [0] = shire_cache.\banks[0] .bank.rspmux.rspmux_trace_sent.valid;
         
         assign mon_atomic_valid     [1] = shire_cache.\banks[1] .bank.pipe.sub_bank.atomic.pipe_valid_d0;
         assign mon_atomic_addr      [1] = shire_cache.\banks[1] .bank.pipe.sub_bank.atomic.pipe_req_info.address;
         assign mon_atomic_opcode    [1] = shire_cache.\banks[1] .bank.pipe.sub_bank.atomic.pipe_req_info.opcode;
         assign mon_atomic_reqq_id   [1] = shire_cache.\banks[1] .bank.pipe.sub_bank.atomic.pipe_req_info.reqq_id;
         assign mon_atomic_l2        [1] = (mon_atomic_opcode[1] == int'(SC_L2_Atomic)) ? 1'b1 : 1'b0;
         assign mon_atomic_l3        [1] = (mon_atomic_opcode[1] == int'(SC_L3_Atomic)) ? 1'b1 : 1'b0;
         assign mon_atomic_l3_source [1] = shire_cache.\banks[1] .bank.pipe.sub_bank.atomic.atomic_routing_d0[20:13];
         assign mon_atomic_l3_trid   [1] = shire_cache.\banks[1] .bank.pipe.sub_bank.atomic.atomic_routing_d0[25:21];

         assign mon_rsp_reqq_id      [1] = shire_cache.\banks[1] .bank.rspmux.bank_rsp_info_full.reqq_id;
         assign mon_rsp_reqq_id_vld  [1] = shire_cache.\banks[1] .bank.rspmux.rspmux_trace_sent.valid;
                                                               
         assign mon_atomic_valid     [2] = shire_cache.\banks[2] .bank.pipe.sub_bank.atomic.pipe_valid_d0;
         assign mon_atomic_addr      [2] = shire_cache.\banks[2] .bank.pipe.sub_bank.atomic.pipe_req_info.address;
         assign mon_atomic_opcode    [2] = shire_cache.\banks[2] .bank.pipe.sub_bank.atomic.pipe_req_info.opcode;
         assign mon_atomic_reqq_id   [2] = shire_cache.\banks[2] .bank.pipe.sub_bank.atomic.pipe_req_info.reqq_id;
         assign mon_atomic_l2        [2] = (mon_atomic_opcode[2] == int'(SC_L2_Atomic)) ? 1'b1 : 1'b0;
         assign mon_atomic_l3        [2] = (mon_atomic_opcode[2] == int'(SC_L3_Atomic)) ? 1'b1 : 1'b0;
         assign mon_atomic_l3_source [2] = shire_cache.\banks[2] .bank.pipe.sub_bank.atomic.atomic_routing_d0[20:13];
         assign mon_atomic_l3_trid   [2] = shire_cache.\banks[2] .bank.pipe.sub_bank.atomic.atomic_routing_d0[25:21];

         assign mon_rsp_reqq_id      [2] = shire_cache.\banks[2] .bank.rspmux.bank_rsp_info_full.reqq_id;
         assign mon_rsp_reqq_id_vld  [2] = shire_cache.\banks[2] .bank.rspmux.rspmux_trace_sent.valid;
                                                               
         assign mon_atomic_valid     [3] = shire_cache.\banks[3] .bank.pipe.sub_bank.atomic.pipe_valid_d0;
         assign mon_atomic_addr      [3] = shire_cache.\banks[3] .bank.pipe.sub_bank.atomic.pipe_req_info.address;
         assign mon_atomic_opcode    [3] = shire_cache.\banks[3] .bank.pipe.sub_bank.atomic.pipe_req_info.opcode;
         assign mon_atomic_reqq_id   [3] = shire_cache.\banks[3] .bank.pipe.sub_bank.atomic.pipe_req_info.reqq_id;
         assign mon_atomic_l2        [3] = (mon_atomic_opcode[3] == int'(SC_L2_Atomic)) ? 1'b1 : 1'b0;
         assign mon_atomic_l3        [3] = (mon_atomic_opcode[3] == int'(SC_L3_Atomic)) ? 1'b1 : 1'b0;
         assign mon_atomic_l3_source [3] = shire_cache.\banks[3] .bank.pipe.sub_bank.atomic.atomic_routing_d0[20:13];
         assign mon_atomic_l3_trid   [3] = shire_cache.\banks[3] .bank.pipe.sub_bank.atomic.atomic_routing_d0[25:21];

         assign mon_rsp_reqq_id      [3] = shire_cache.\banks[3] .bank.rspmux.bank_rsp_info_full.reqq_id;
         assign mon_rsp_reqq_id_vld  [3] = shire_cache.\banks[3] .bank.rspmux.rspmux_trace_sent.valid;
                                                               
         if (NUM_BANKS == 8) begin : b8_atomic
            assign mon_atomic_valid     [4] = shire_cache.\banks[4] .bank.pipe.sub_bank.atomic.pipe_valid_d0;
            assign mon_atomic_addr      [4] = shire_cache.\banks[4] .bank.pipe.sub_bank.atomic.pipe_req_info.address;
            assign mon_atomic_opcode    [4] = shire_cache.\banks[4] .bank.pipe.sub_bank.atomic.pipe_req_info.opcode;
            assign mon_atomic_reqq_id   [4] = shire_cache.\banks[4] .bank.pipe.sub_bank.atomic.pipe_req_info.reqq_id;
            assign mon_atomic_l2        [4] = (mon_atomic_opcode[4] == int'(SC_L2_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3        [4] = (mon_atomic_opcode[4] == int'(SC_L3_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3_source [4] = shire_cache.\banks[4] .bank.pipe.sub_bank.atomic.atomic_routing_d0[20:13];
            assign mon_atomic_l3_trid   [4] = shire_cache.\banks[4] .bank.pipe.sub_bank.atomic.atomic_routing_d0[25:21];

            assign mon_rsp_reqq_id      [4] = shire_cache.\banks[4] .bank.rspmux.bank_rsp_info_full.reqq_id;
            assign mon_rsp_reqq_id_vld  [4] = shire_cache.\banks[4] .bank.rspmux.rspmux_trace_sent.valid;
                                                                  
            assign mon_atomic_valid     [5] = shire_cache.\banks[5] .bank.pipe.sub_bank.atomic.pipe_valid_d0;
            assign mon_atomic_addr      [5] = shire_cache.\banks[5] .bank.pipe.sub_bank.atomic.pipe_req_info.address;
            assign mon_atomic_opcode    [5] = shire_cache.\banks[5] .bank.pipe.sub_bank.atomic.pipe_req_info.opcode;
            assign mon_atomic_reqq_id   [5] = shire_cache.\banks[5] .bank.pipe.sub_bank.atomic.pipe_req_info.reqq_id;
            assign mon_atomic_l2        [5] = (mon_atomic_opcode[5] == int'(SC_L2_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3        [5] = (mon_atomic_opcode[5] == int'(SC_L3_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3_source [5] = shire_cache.\banks[5] .bank.pipe.sub_bank.atomic.atomic_routing_d0[20:13];
            assign mon_atomic_l3_trid   [5] = shire_cache.\banks[5] .bank.pipe.sub_bank.atomic.atomic_routing_d0[25:21];

            assign mon_rsp_reqq_id      [5] = shire_cache.\banks[5] .bank.rspmux.bank_rsp_info_full.reqq_id;
            assign mon_rsp_reqq_id_vld  [5] = shire_cache.\banks[5] .bank.rspmux.rspmux_trace_sent.valid;
                                                                  
            assign mon_atomic_valid     [6] = shire_cache.\banks[6] .bank.pipe.sub_bank.atomic.pipe_valid_d0;
            assign mon_atomic_addr      [6] = shire_cache.\banks[6] .bank.pipe.sub_bank.atomic.pipe_req_info.address;
            assign mon_atomic_opcode    [6] = shire_cache.\banks[6] .bank.pipe.sub_bank.atomic.pipe_req_info.opcode;
            assign mon_atomic_reqq_id   [6] = shire_cache.\banks[6] .bank.pipe.sub_bank.atomic.pipe_req_info.reqq_id;
            assign mon_atomic_l2        [6] = (mon_atomic_opcode[6] == int'(SC_L2_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3        [6] = (mon_atomic_opcode[6] == int'(SC_L3_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3_source [6] = shire_cache.\banks[6] .bank.pipe.sub_bank.atomic.atomic_routing_d0[20:13];
            assign mon_atomic_l3_trid   [6] = shire_cache.\banks[6] .bank.pipe.sub_bank.atomic.atomic_routing_d0[25:21];

            assign mon_rsp_reqq_id      [6] = shire_cache.\banks[6] .bank.rspmux.bank_rsp_info_full.reqq_id;
            assign mon_rsp_reqq_id_vld  [6] = shire_cache.\banks[6] .bank.rspmux.rspmux_trace_sent.valid;
                                                                  
            assign mon_atomic_valid     [7] = shire_cache.\banks[7] .bank.pipe.sub_bank.atomic.pipe_valid_d0;
            assign mon_atomic_addr      [7] = shire_cache.\banks[7] .bank.pipe.sub_bank.atomic.pipe_req_info.address;
            assign mon_atomic_opcode    [7] = shire_cache.\banks[7] .bank.pipe.sub_bank.atomic.pipe_req_info.opcode;
            assign mon_atomic_reqq_id   [7] = shire_cache.\banks[7] .bank.pipe.sub_bank.atomic.pipe_req_info.reqq_id;
            assign mon_atomic_l2        [7] = (mon_atomic_opcode[7] == int'(SC_L2_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3        [7] = (mon_atomic_opcode[7] == int'(SC_L3_Atomic)) ? 1'b1 : 1'b0;
            assign mon_atomic_l3_source [7] = shire_cache.\banks[7] .bank.pipe.sub_bank.atomic.atomic_routing_d0[20:13];
            assign mon_atomic_l3_trid   [7] = shire_cache.\banks[7] .bank.pipe.sub_bank.atomic.atomic_routing_d0[25:21];

            assign mon_rsp_reqq_id      [7] = shire_cache.\banks[7] .bank.rspmux.bank_rsp_info_full.reqq_id;
            assign mon_rsp_reqq_id_vld  [7] = shire_cache.\banks[7] .bank.rspmux.rspmux_trace_sent.valid;
            for (gen_in = 0; gen_in < 8; gen_in++) begin : dst_pop
               if (gen_in < NUM_NEIGH) begin : dst_pop_a
                  assign mon_rsp_dst_pop[gen_it][gen_in] = shire_cache.rsp_xbar.dest_fifo_pop[gen_in][gen_it];
               end
               else begin : dst_pop_b
                  assign mon_rsp_dst_pop[gen_it][gen_in] = 1'b0;
               end
            end
         end
      `endif


      always @(posedge clock) begin
         if (start_monitor === 1'b1) begin
            if (reset === 1'b1) begin
               // do something here.
            end
            else begin
               // do something here.
            end
         end
      end
   endgenerate

   generate
      `ifndef ET_USE_BIST_RTL
         for (gen_it = 0; gen_it < NUM_BANKS; gen_it++) begin : pipe_tc
            assign mon_pipe_tc_req_valid             [gen_it] = shire_cache.banks[gen_it].bank.pipe.sub_bank.pipe_valid_tc;
            assign mon_pipe_tc_pipe_req_info         [gen_it] = shire_cache.banks[gen_it].bank.pipe.sub_bank.pipe_req_info_tc;
            assign mon_pipe_tc_pipe_info             [gen_it] = shire_cache.banks[gen_it].bank.pipe.sub_bank.pipe_info_tc;
            assign mon_pipe_tc_rsp_valid             [gen_it] = shire_cache.banks[gen_it].bank.pipe_tag_rsp_valid;
            assign mon_pipe_tc_rsp_info              [gen_it] = shire_cache.banks[gen_it].bank.pipe_tag_rsp_info;
            assign mon_dealloc_ag                    [gen_it] = shire_cache.banks[gen_it].bank.reqq.reqq_entries_dealloc_hot;
            assign mon_reqq_l3_entries               [gen_it] = shire_cache.banks[gen_it].bank.reqq.reqq_l3_entries;
            assign mon_alloc_l2_valid_ag             [gen_it] = shire_cache.banks[gen_it].bank.reqq.alloc_l2_valid_ag;
            assign mon_alloc_l3_valid_ag             [gen_it] = shire_cache.banks[gen_it].bank.reqq.alloc_l3_valid_ag;
            assign mon_alloc_l2_1hot_ag              [gen_it] = shire_cache.banks[gen_it].bank.reqq.alloc_l2_1hot_ag;
            assign mon_alloc_l3_1hot_ag              [gen_it] = shire_cache.banks[gen_it].bank.reqq.alloc_l3_1hot_ag;
            assign mon_alloc_l2_info_ag              [gen_it] = shire_cache.banks[gen_it].bank.reqq.alloc_l2_info_ag;
            assign mon_alloc_l3_info_ag              [gen_it] = shire_cache.banks[gen_it].bank.reqq.alloc_l3_info_ag;
            assign mon_alloc_l2_info_paired_ag       [gen_it] = shire_cache.banks[gen_it].bank.reqq.alloc_l2_info_paired_ag;
            assign mon_alloc_l3_info_paired_ag       [gen_it] = shire_cache.banks[gen_it].bank.reqq.alloc_l3_info_paired_ag;
            assign mon_alloc_l2_alloc_valid_paired_ag[gen_it] = shire_cache.banks[gen_it].bank.reqq.allocs[0].alloc.alloc_valid_paired_ag;
            assign mon_alloc_l3_alloc_valid_paired_ag[gen_it] = shire_cache.banks[gen_it].bank.reqq.allocs[1].alloc.alloc_valid_paired_ag;
            assign mon_alloc_l2_2nd_half_needed_ad   [gen_it] = shire_cache.banks[gen_it].bank.reqq.allocs[0].alloc.alloc_2nd_half_needed_ad;
            assign mon_alloc_l3_2nd_half_needed_ad   [gen_it] = shire_cache.banks[gen_it].bank.reqq.allocs[1].alloc.alloc_2nd_half_needed_ad;
            assign mon_alloc_l2_bank_info_ag         [gen_it] = shire_cache.banks[gen_it].bank.reqq.bank_l2_info;
            assign mon_alloc_l3_bank_info_ag         [gen_it] = shire_cache.banks[gen_it].bank.reqq.bank_l3_info;
            assign mon_pipe_rbuf_rd_req_valid        [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_rbuf_rd_req_valid;
            assign mon_pipe_rbuf_rd_req_ready        [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_rbuf_rd_req_ready;
            assign mon_pipe_rbuf_rd_req_info         [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_rbuf_rd_req_info;
            assign mon_esr_ctl                       [gen_it] = shire_cache.banks[gen_it].bank.esr_ctl;
            assign mon_cbuf_matches_tag_victim       [gen_it] = shire_cache.banks[gen_it].bank.reqq.coalesce_victim_matches_tag_victim;
            assign mon_pipe_cbuf_inv_req_valid       [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_cbuf_invalidate_req_valid;
            assign mon_pipe_cbuf_req_valid           [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_cbuf_req_valid;
            assign mon_pipe_cbuf_req_info            [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_cbuf_req_info;
            assign mon_pipe_cbuf_rsp_valid           [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_cbuf_rsp_valid;
            assign mon_pipe_cbuf_rsp_info            [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_cbuf_rsp_info;
            assign mon_pipe_idx_cop_l2_busy          [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_idx_cop_l2_busy;
            assign mon_pipe_idx_cop_l3_busy          [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_idx_cop_l3_busy;
            assign mon_pipe_idx_cop_req_ready        [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_idx_cop_req_ready;
            assign mon_pipe_idx_cop_req_valid        [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_idx_cop_req_valid;
            assign mon_pipe_idx_cop_cb_inv_start     [gen_it] = shire_cache.banks[gen_it].bank.pipe.idx_cop_sm.idx_cop_sm_load & shire_cache.banks[gen_it].bank.pipe.idx_cop_sm.cb_inv_op_xx_next;
            assign mon_pipe_idx_cop_req_info         [gen_it] = shire_cache.banks[gen_it].bank.pipe.pipe_idx_cop_req_info;
            `ifndef ET_ASSERT_OFF
               assign shire_cache.banks[gen_it].bank.pipe.dv_reset_esr_ctl_captured = dv_reset_esr_ctl_captured_r;
            `endif // ifndef ET_ASSERT_OFF
         end
      `else
         assign mon_pipe_tc_req_valid             [0]        = shire_cache.\banks[0] .bank.pipe.sub_bank.pipe_valid_tc;
         assign mon_pipe_tc_pipe_req_info         [0]        = shire_cache.\banks[0] .bank.pipe.sub_bank.pipe_req_info_tc;
         assign mon_pipe_tc_pipe_info             [0]        = shire_cache.\banks[0] .bank.pipe.sub_bank.pipe_info_tc;
         assign mon_pipe_tc_rsp_valid             [0]        = shire_cache.\banks[0] .bank.pipe_tag_rsp_valid;
         assign mon_pipe_tc_rsp_info              [0]        = shire_cache.\banks[0] .bank.pipe_tag_rsp_info;
         assign mon_dealloc_ag                    [0]        = shire_cache.\banks[0] .bank.reqq.reqq_entries_dealloc_hot;
         assign mon_reqq_l3_entries               [0]        = shire_cache.\banks[0] .bank.reqq.reqq_l3_entries;
         assign mon_alloc_l2_valid_ag             [0]        = shire_cache.\banks[0] .bank.reqq.alloc_l2_valid_ag;
         assign mon_alloc_l3_valid_ag             [0]        = shire_cache.\banks[0] .bank.reqq.alloc_l3_valid_ag;
         assign mon_alloc_l2_1hot_ag              [0]        = shire_cache.\banks[0] .bank.reqq.alloc_l2_1hot_ag;
         assign mon_alloc_l3_1hot_ag              [0]        = shire_cache.\banks[0] .bank.reqq.alloc_l3_1hot_ag;
         assign mon_alloc_l2_info_ag              [0]        = shire_cache.\banks[0] .bank.reqq.alloc_l2_info_ag;
         assign mon_alloc_l3_info_ag              [0]        = shire_cache.\banks[0] .bank.reqq.alloc_l3_info_ag;
         assign mon_alloc_l2_info_paired_ag       [0]        = shire_cache.\banks[0] .bank.reqq.alloc_l2_info_paired_ag;
         assign mon_alloc_l3_info_paired_ag       [0]        = shire_cache.\banks[0] .bank.reqq.alloc_l3_info_paired_ag;
         assign mon_alloc_l2_alloc_valid_paired_ag[0]        = shire_cache.\banks[0] .bank.reqq.allocs[0].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l3_alloc_valid_paired_ag[0]        = shire_cache.\banks[0] .bank.reqq.allocs[1].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l2_2nd_half_needed_ad   [0]        = shire_cache.\banks[0] .bank.reqq.allocs[0].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l3_2nd_half_needed_ad   [0]        = shire_cache.\banks[0] .bank.reqq.allocs[1].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l2_bank_info_ag         [0]        = shire_cache.\banks[0] .bank.reqq.bank_l2_info;
         assign mon_alloc_l3_bank_info_ag         [0]        = shire_cache.\banks[0] .bank.reqq.bank_l3_info;
         assign mon_pipe_rbuf_rd_req_valid        [0]        = shire_cache.\banks[0] .bank.pipe.pipe_rbuf_rd_req_valid;
         assign mon_pipe_rbuf_rd_req_ready        [0]        = shire_cache.\banks[0] .bank.pipe.pipe_rbuf_rd_req_ready;
         assign mon_pipe_rbuf_rd_req_info         [0]        = shire_cache.\banks[0] .bank.pipe.pipe_rbuf_rd_req_info;
         assign mon_esr_ctl                       [0]        = shire_cache.\banks[0] .bank.esr_ctl;
         assign mon_cbuf_matches_tag_victim       [0]        = shire_cache.\banks[0] .bank.reqq.coalesce_victim_matches_tag_victim;
         assign mon_pipe_cbuf_inv_req_valid       [0]        = shire_cache.\banks[0] .bank.pipe.pipe_cbuf_invalidate_req_valid;
         assign mon_pipe_cbuf_req_valid           [0]        = shire_cache.\banks[0] .bank.pipe.pipe_cbuf_req_valid;
         assign mon_pipe_cbuf_req_info            [0]        = shire_cache.\banks[0] .bank.pipe.pipe_cbuf_req_info;
         assign mon_pipe_cbuf_rsp_valid           [0]        = shire_cache.\banks[0] .bank.pipe.pipe_cbuf_rsp_valid;
         assign mon_pipe_cbuf_rsp_info            [0]        = shire_cache.\banks[0] .bank.pipe.pipe_cbuf_rsp_info;
         assign mon_pipe_idx_cop_l2_busy          [0]        = shire_cache.\banks[0] .bank.pipe.pipe_idx_cop_l2_busy;
         assign mon_pipe_idx_cop_l3_busy          [0]        = shire_cache.\banks[0] .bank.pipe.pipe_idx_cop_l3_busy;
         assign mon_pipe_idx_cop_req_ready        [0]        = shire_cache.\banks[0] .bank.pipe.pipe_idx_cop_req_ready;
         assign mon_pipe_idx_cop_req_valid        [0]        = shire_cache.\banks[0] .bank.pipe.pipe_idx_cop_req_valid;
         assign mon_pipe_idx_cop_cb_inv_start     [0]        = shire_cache.\banks[0] .bank.pipe.idx_cop_sm.idx_cop_sm_load & shire_cache.\banks[0] .bank.pipe.idx_cop_sm.cb_inv_op_xx_next;
         assign mon_pipe_idx_cop_req_info         [0]        = shire_cache.\banks[0] .bank.pipe.pipe_idx_cop_req_info;
         `ifndef ET_ASSERT_OFF
            assign shire_cache.\banks[0] .bank.pipe.dv_reset_esr_ctl_captured = dv_reset_esr_ctl_captured_r;
         `endif // ifndef ET_ASSERT_OFF
                                                                                   
         assign mon_pipe_tc_req_valid             [1]        = shire_cache.\banks[1] .bank.pipe.sub_bank.pipe_valid_tc;
         assign mon_pipe_tc_pipe_req_info         [1]        = shire_cache.\banks[1] .bank.pipe.sub_bank.pipe_req_info_tc;
         assign mon_pipe_tc_pipe_info             [1]        = shire_cache.\banks[1] .bank.pipe.sub_bank.pipe_info_tc;
         assign mon_pipe_tc_rsp_valid             [1]        = shire_cache.\banks[1] .bank.pipe_tag_rsp_valid;
         assign mon_pipe_tc_rsp_info              [1]        = shire_cache.\banks[1] .bank.pipe_tag_rsp_info;
         assign mon_dealloc_ag                    [1]        = shire_cache.\banks[1] .bank.reqq.reqq_entries_dealloc_hot;
         assign mon_reqq_l3_entries               [1]        = shire_cache.\banks[1] .bank.reqq.reqq_l3_entries;
         assign mon_alloc_l2_valid_ag             [1]        = shire_cache.\banks[1] .bank.reqq.alloc_l2_valid_ag;
         assign mon_alloc_l3_valid_ag             [1]        = shire_cache.\banks[1] .bank.reqq.alloc_l3_valid_ag;
         assign mon_alloc_l2_1hot_ag              [1]        = shire_cache.\banks[1] .bank.reqq.alloc_l2_1hot_ag;
         assign mon_alloc_l3_1hot_ag              [1]        = shire_cache.\banks[1] .bank.reqq.alloc_l3_1hot_ag;
         assign mon_alloc_l2_info_ag              [1]        = shire_cache.\banks[1] .bank.reqq.alloc_l2_info_ag;
         assign mon_alloc_l3_info_ag              [1]        = shire_cache.\banks[1] .bank.reqq.alloc_l3_info_ag;
         assign mon_alloc_l2_info_paired_ag       [1]        = shire_cache.\banks[1] .bank.reqq.alloc_l2_info_paired_ag;
         assign mon_alloc_l3_info_paired_ag       [1]        = shire_cache.\banks[1] .bank.reqq.alloc_l3_info_paired_ag;
         assign mon_alloc_l2_alloc_valid_paired_ag[1]        = shire_cache.\banks[1] .bank.reqq.allocs[0].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l3_alloc_valid_paired_ag[1]        = shire_cache.\banks[1] .bank.reqq.allocs[1].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l2_2nd_half_needed_ad   [1]        = shire_cache.\banks[1] .bank.reqq.allocs[0].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l3_2nd_half_needed_ad   [1]        = shire_cache.\banks[1] .bank.reqq.allocs[1].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l2_bank_info_ag         [1]        = shire_cache.\banks[1] .bank.reqq.bank_l2_info;
         assign mon_alloc_l3_bank_info_ag         [1]        = shire_cache.\banks[1] .bank.reqq.bank_l3_info;
         assign mon_pipe_rbuf_rd_req_valid        [1]        = shire_cache.\banks[1] .bank.pipe.pipe_rbuf_rd_req_valid;
         assign mon_pipe_rbuf_rd_req_ready        [1]        = shire_cache.\banks[1] .bank.pipe.pipe_rbuf_rd_req_ready;
         assign mon_pipe_rbuf_rd_req_info         [1]        = shire_cache.\banks[1] .bank.pipe.pipe_rbuf_rd_req_info;
         assign mon_esr_ctl                       [1]        = shire_cache.\banks[1] .bank.esr_ctl;
         assign mon_cbuf_matches_tag_victim       [1]        = shire_cache.\banks[1] .bank.reqq.coalesce_victim_matches_tag_victim;
         assign mon_pipe_cbuf_inv_req_valid       [1]        = shire_cache.\banks[1] .bank.pipe.pipe_cbuf_invalidate_req_valid;
         assign mon_pipe_cbuf_req_valid           [1]        = shire_cache.\banks[1] .bank.pipe.pipe_cbuf_req_valid;
         assign mon_pipe_cbuf_req_info            [1]        = shire_cache.\banks[1] .bank.pipe.pipe_cbuf_req_info;
         assign mon_pipe_cbuf_rsp_valid           [1]        = shire_cache.\banks[1] .bank.pipe.pipe_cbuf_rsp_valid;
         assign mon_pipe_cbuf_rsp_info            [1]        = shire_cache.\banks[1] .bank.pipe.pipe_cbuf_rsp_info;
         assign mon_pipe_idx_cop_l2_busy          [1]        = shire_cache.\banks[1] .bank.pipe.pipe_idx_cop_l2_busy;
         assign mon_pipe_idx_cop_l3_busy          [1]        = shire_cache.\banks[1] .bank.pipe.pipe_idx_cop_l3_busy;
         assign mon_pipe_idx_cop_req_ready        [1]        = shire_cache.\banks[1] .bank.pipe.pipe_idx_cop_req_ready;
         assign mon_pipe_idx_cop_req_valid        [1]        = shire_cache.\banks[1] .bank.pipe.pipe_idx_cop_req_valid;
         assign mon_pipe_idx_cop_cb_inv_start     [1]        = shire_cache.\banks[1] .bank.pipe.idx_cop_sm.idx_cop_sm_load & shire_cache.\banks[1] .bank.pipe.idx_cop_sm.cb_inv_op_xx_next;
         assign mon_pipe_idx_cop_req_info         [1]        = shire_cache.\banks[1] .bank.pipe.pipe_idx_cop_req_info;
         `ifndef ET_ASSERT_OFF
            assign shire_cache.\banks[1] .bank.pipe.dv_reset_esr_ctl_captured = dv_reset_esr_ctl_captured_r;
         `endif // ifndef ET_ASSERT_OFF
                                                                                   
         assign mon_pipe_tc_req_valid             [2]        = shire_cache.\banks[2] .bank.pipe.sub_bank.pipe_valid_tc;
         assign mon_pipe_tc_pipe_req_info         [2]        = shire_cache.\banks[2] .bank.pipe.sub_bank.pipe_req_info_tc;
         assign mon_pipe_tc_pipe_info             [2]        = shire_cache.\banks[2] .bank.pipe.sub_bank.pipe_info_tc;
         assign mon_pipe_tc_rsp_valid             [2]        = shire_cache.\banks[2] .bank.pipe_tag_rsp_valid;
         assign mon_pipe_tc_rsp_info              [2]        = shire_cache.\banks[2] .bank.pipe_tag_rsp_info;
         assign mon_dealloc_ag                    [2]        = shire_cache.\banks[2] .bank.reqq.reqq_entries_dealloc_hot;
         assign mon_reqq_l3_entries               [2]        = shire_cache.\banks[2] .bank.reqq.reqq_l3_entries;
         assign mon_alloc_l2_valid_ag             [2]        = shire_cache.\banks[2] .bank.reqq.alloc_l2_valid_ag;
         assign mon_alloc_l3_valid_ag             [2]        = shire_cache.\banks[2] .bank.reqq.alloc_l3_valid_ag;
         assign mon_alloc_l2_1hot_ag              [2]        = shire_cache.\banks[2] .bank.reqq.alloc_l2_1hot_ag;
         assign mon_alloc_l3_1hot_ag              [2]        = shire_cache.\banks[2] .bank.reqq.alloc_l3_1hot_ag;
         assign mon_alloc_l2_info_ag              [2]        = shire_cache.\banks[2] .bank.reqq.alloc_l2_info_ag;
         assign mon_alloc_l3_info_ag              [2]        = shire_cache.\banks[2] .bank.reqq.alloc_l3_info_ag;
         assign mon_alloc_l2_info_paired_ag       [2]        = shire_cache.\banks[2] .bank.reqq.alloc_l2_info_paired_ag;
         assign mon_alloc_l3_info_paired_ag       [2]        = shire_cache.\banks[2] .bank.reqq.alloc_l3_info_paired_ag;
         assign mon_alloc_l2_alloc_valid_paired_ag[2]        = shire_cache.\banks[2] .bank.reqq.allocs[0].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l3_alloc_valid_paired_ag[2]        = shire_cache.\banks[2] .bank.reqq.allocs[1].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l2_2nd_half_needed_ad   [2]        = shire_cache.\banks[2] .bank.reqq.allocs[0].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l3_2nd_half_needed_ad   [2]        = shire_cache.\banks[2] .bank.reqq.allocs[1].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l2_bank_info_ag         [2]        = shire_cache.\banks[2] .bank.reqq.bank_l2_info;
         assign mon_alloc_l3_bank_info_ag         [2]        = shire_cache.\banks[2] .bank.reqq.bank_l3_info;
         assign mon_pipe_rbuf_rd_req_valid        [2]        = shire_cache.\banks[2] .bank.pipe.pipe_rbuf_rd_req_valid;
         assign mon_pipe_rbuf_rd_req_ready        [2]        = shire_cache.\banks[2] .bank.pipe.pipe_rbuf_rd_req_ready;
         assign mon_pipe_rbuf_rd_req_info         [2]        = shire_cache.\banks[2] .bank.pipe.pipe_rbuf_rd_req_info;
         assign mon_esr_ctl                       [2]        = shire_cache.\banks[2] .bank.esr_ctl;
         assign mon_cbuf_matches_tag_victim       [2]        = shire_cache.\banks[2] .bank.reqq.coalesce_victim_matches_tag_victim;
         assign mon_pipe_cbuf_inv_req_valid       [2]        = shire_cache.\banks[2] .bank.pipe.pipe_cbuf_invalidate_req_valid;
         assign mon_pipe_cbuf_req_valid           [2]        = shire_cache.\banks[2] .bank.pipe.pipe_cbuf_req_valid;
         assign mon_pipe_cbuf_req_info            [2]        = shire_cache.\banks[2] .bank.pipe.pipe_cbuf_req_info;
         assign mon_pipe_cbuf_rsp_valid           [2]        = shire_cache.\banks[2] .bank.pipe.pipe_cbuf_rsp_valid;
         assign mon_pipe_cbuf_rsp_info            [2]        = shire_cache.\banks[2] .bank.pipe.pipe_cbuf_rsp_info;
         assign mon_pipe_idx_cop_l2_busy          [2]        = shire_cache.\banks[2] .bank.pipe.pipe_idx_cop_l2_busy;
         assign mon_pipe_idx_cop_l3_busy          [2]        = shire_cache.\banks[2] .bank.pipe.pipe_idx_cop_l3_busy;
         assign mon_pipe_idx_cop_req_ready        [2]        = shire_cache.\banks[2] .bank.pipe.pipe_idx_cop_req_ready;
         assign mon_pipe_idx_cop_req_valid        [2]        = shire_cache.\banks[2] .bank.pipe.pipe_idx_cop_req_valid;
         assign mon_pipe_idx_cop_cb_inv_start     [2]        = shire_cache.\banks[2] .bank.pipe.idx_cop_sm.idx_cop_sm_load & shire_cache.\banks[2] .bank.pipe.idx_cop_sm.cb_inv_op_xx_next;
         assign mon_pipe_idx_cop_req_info         [2]        = shire_cache.\banks[2] .bank.pipe.pipe_idx_cop_req_info;
         `ifndef ET_ASSERT_OFF
            assign shire_cache.\banks[2] .bank.pipe.dv_reset_esr_ctl_captured = dv_reset_esr_ctl_captured_r;
         `endif // ifndef ET_ASSERT_OFF
                                                                                   
         assign mon_pipe_tc_req_valid             [3]        = shire_cache.\banks[3] .bank.pipe.sub_bank.pipe_valid_tc;
         assign mon_pipe_tc_pipe_req_info         [3]        = shire_cache.\banks[3] .bank.pipe.sub_bank.pipe_req_info_tc;
         assign mon_pipe_tc_pipe_info             [3]        = shire_cache.\banks[3] .bank.pipe.sub_bank.pipe_info_tc;
         assign mon_pipe_tc_rsp_valid             [3]        = shire_cache.\banks[3] .bank.pipe_tag_rsp_valid;
         assign mon_pipe_tc_rsp_info              [3]        = shire_cache.\banks[3] .bank.pipe_tag_rsp_info;
         assign mon_dealloc_ag                    [3]        = shire_cache.\banks[3] .bank.reqq.reqq_entries_dealloc_hot;
         assign mon_reqq_l3_entries               [3]        = shire_cache.\banks[3] .bank.reqq.reqq_l3_entries;
         assign mon_alloc_l2_valid_ag             [3]        = shire_cache.\banks[3] .bank.reqq.alloc_l2_valid_ag;
         assign mon_alloc_l3_valid_ag             [3]        = shire_cache.\banks[3] .bank.reqq.alloc_l3_valid_ag;
         assign mon_alloc_l2_1hot_ag              [3]        = shire_cache.\banks[3] .bank.reqq.alloc_l2_1hot_ag;
         assign mon_alloc_l3_1hot_ag              [3]        = shire_cache.\banks[3] .bank.reqq.alloc_l3_1hot_ag;
         assign mon_alloc_l2_info_ag              [3]        = shire_cache.\banks[3] .bank.reqq.alloc_l2_info_ag;
         assign mon_alloc_l3_info_ag              [3]        = shire_cache.\banks[3] .bank.reqq.alloc_l3_info_ag;
         assign mon_alloc_l2_info_paired_ag       [3]        = shire_cache.\banks[3] .bank.reqq.alloc_l2_info_paired_ag;
         assign mon_alloc_l3_info_paired_ag       [3]        = shire_cache.\banks[3] .bank.reqq.alloc_l3_info_paired_ag;
         assign mon_alloc_l2_alloc_valid_paired_ag[3]        = shire_cache.\banks[3] .bank.reqq.allocs[0].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l3_alloc_valid_paired_ag[3]        = shire_cache.\banks[3] .bank.reqq.allocs[1].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l2_2nd_half_needed_ad   [3]        = shire_cache.\banks[3] .bank.reqq.allocs[0].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l3_2nd_half_needed_ad   [3]        = shire_cache.\banks[3] .bank.reqq.allocs[1].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l2_bank_info_ag         [3]        = shire_cache.\banks[3] .bank.reqq.bank_l2_info;
         assign mon_alloc_l3_bank_info_ag         [3]        = shire_cache.\banks[3] .bank.reqq.bank_l3_info;
         assign mon_pipe_rbuf_rd_req_valid        [3]        = shire_cache.\banks[3] .bank.pipe.pipe_rbuf_rd_req_valid;
         assign mon_pipe_rbuf_rd_req_ready        [3]        = shire_cache.\banks[3] .bank.pipe.pipe_rbuf_rd_req_ready;
         assign mon_pipe_rbuf_rd_req_info         [3]        = shire_cache.\banks[3] .bank.pipe.pipe_rbuf_rd_req_info;
         assign mon_esr_ctl                       [3]        = shire_cache.\banks[3] .bank.esr_ctl;
         assign mon_cbuf_matches_tag_victim       [3]        = shire_cache.\banks[3] .bank.reqq.coalesce_victim_matches_tag_victim;
         assign mon_pipe_cbuf_inv_req_valid       [3]        = shire_cache.\banks[3] .bank.pipe.pipe_cbuf_invalidate_req_valid;
         assign mon_pipe_cbuf_req_valid           [3]        = shire_cache.\banks[3] .bank.pipe.pipe_cbuf_req_valid;
         assign mon_pipe_cbuf_req_info            [3]        = shire_cache.\banks[3] .bank.pipe.pipe_cbuf_req_info;
         assign mon_pipe_cbuf_rsp_valid           [3]        = shire_cache.\banks[3] .bank.pipe.pipe_cbuf_rsp_valid;
         assign mon_pipe_cbuf_rsp_info            [3]        = shire_cache.\banks[3] .bank.pipe.pipe_cbuf_rsp_info;
         assign mon_pipe_idx_cop_l2_busy          [3]        = shire_cache.\banks[3] .bank.pipe.pipe_idx_cop_l2_busy;
         assign mon_pipe_idx_cop_l3_busy          [3]        = shire_cache.\banks[3] .bank.pipe.pipe_idx_cop_l3_busy;
         assign mon_pipe_idx_cop_req_ready        [3]        = shire_cache.\banks[3] .bank.pipe.pipe_idx_cop_req_ready;
         assign mon_pipe_idx_cop_req_valid        [3]        = shire_cache.\banks[3] .bank.pipe.pipe_idx_cop_req_valid;
         assign mon_pipe_idx_cop_cb_inv_start     [3]        = shire_cache.\banks[3] .bank.pipe.idx_cop_sm.idx_cop_sm_load & shire_cache.\banks[3] .bank.pipe.idx_cop_sm.cb_inv_op_xx_next;
         assign mon_pipe_idx_cop_req_info         [3]        = shire_cache.\banks[3] .bank.pipe.pipe_idx_cop_req_info;
         `ifndef ET_ASSERT_OFF
            assign shire_cache.\banks[3] .bank.pipe.dv_reset_esr_ctl_captured = dv_reset_esr_ctl_captured_r;
         `endif // ifndef ET_ASSERT_OFF
                                                                                   
         if (NUM_BANKS == 8) begin : b8_tc
         assign mon_pipe_tc_req_valid             [4]        = shire_cache.\banks[4] .bank.pipe.sub_bank.pipe_valid_tc;
         assign mon_pipe_tc_pipe_req_info         [4]        = shire_cache.\banks[4] .bank.pipe.sub_bank.pipe_req_info_tc;
         assign mon_pipe_tc_pipe_info             [4]        = shire_cache.\banks[4] .bank.pipe.sub_bank.pipe_info_tc;
         assign mon_pipe_tc_rsp_valid             [4]        = shire_cache.\banks[4] .bank.pipe_tag_rsp_valid;
         assign mon_pipe_tc_rsp_info              [4]        = shire_cache.\banks[4] .bank.pipe_tag_rsp_info;
         assign mon_dealloc_ag                    [4]        = shire_cache.\banks[4] .bank.reqq.reqq_entries_dealloc_hot;
         assign mon_reqq_l3_entries               [4]        = shire_cache.\banks[4] .bank.reqq.reqq_l3_entries;
         assign mon_alloc_l2_valid_ag             [4]        = shire_cache.\banks[4] .bank.reqq.alloc_l2_valid_ag;
         assign mon_alloc_l3_valid_ag             [4]        = shire_cache.\banks[4] .bank.reqq.alloc_l3_valid_ag;
         assign mon_alloc_l2_1hot_ag              [4]        = shire_cache.\banks[4] .bank.reqq.alloc_l2_1hot_ag;
         assign mon_alloc_l3_1hot_ag              [4]        = shire_cache.\banks[4] .bank.reqq.alloc_l3_1hot_ag;
         assign mon_alloc_l2_info_ag              [4]        = shire_cache.\banks[4] .bank.reqq.alloc_l2_info_ag;
         assign mon_alloc_l3_info_ag              [4]        = shire_cache.\banks[4] .bank.reqq.alloc_l3_info_ag;
         assign mon_alloc_l2_info_paired_ag       [4]        = shire_cache.\banks[4] .bank.reqq.alloc_l2_info_paired_ag;
         assign mon_alloc_l3_info_paired_ag       [4]        = shire_cache.\banks[4] .bank.reqq.alloc_l3_info_paired_ag;
         assign mon_alloc_l2_alloc_valid_paired_ag[4]        = shire_cache.\banks[4] .bank.reqq.allocs[0].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l3_alloc_valid_paired_ag[4]        = shire_cache.\banks[4] .bank.reqq.allocs[1].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l2_2nd_half_needed_ad   [4]        = shire_cache.\banks[4] .bank.reqq.allocs[0].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l3_2nd_half_needed_ad   [4]        = shire_cache.\banks[4] .bank.reqq.allocs[1].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l2_bank_info_ag         [4]        = shire_cache.\banks[4] .bank.reqq.bank_l2_info;
         assign mon_alloc_l3_bank_info_ag         [4]        = shire_cache.\banks[4] .bank.reqq.bank_l3_info;
         assign mon_pipe_rbuf_rd_req_valid        [4]        = shire_cache.\banks[4] .bank.pipe.pipe_rbuf_rd_req_valid;
         assign mon_pipe_rbuf_rd_req_ready        [4]        = shire_cache.\banks[4] .bank.pipe.pipe_rbuf_rd_req_ready;
         assign mon_pipe_rbuf_rd_req_info         [4]        = shire_cache.\banks[4] .bank.pipe.pipe_rbuf_rd_req_info;
         assign mon_esr_ctl                       [4]        = shire_cache.\banks[4] .bank.esr_ctl;
         assign mon_cbuf_matches_tag_victim       [4]        = shire_cache.\banks[4] .bank.reqq.coalesce_victim_matches_tag_victim;
         assign mon_pipe_cbuf_inv_req_valid       [4]        = shire_cache.\banks[4] .bank.pipe.pipe_cbuf_invalidate_req_valid;
         assign mon_pipe_cbuf_req_valid           [4]        = shire_cache.\banks[4] .bank.pipe.pipe_cbuf_req_valid;
         assign mon_pipe_cbuf_req_info            [4]        = shire_cache.\banks[4] .bank.pipe.pipe_cbuf_req_info;
         assign mon_pipe_cbuf_rsp_valid           [4]        = shire_cache.\banks[4] .bank.pipe.pipe_cbuf_rsp_valid;
         assign mon_pipe_cbuf_rsp_info            [4]        = shire_cache.\banks[4] .bank.pipe.pipe_cbuf_rsp_info;
         assign mon_pipe_idx_cop_l2_busy          [4]        = shire_cache.\banks[4] .bank.pipe.pipe_idx_cop_l2_busy;
         assign mon_pipe_idx_cop_l3_busy          [4]        = shire_cache.\banks[4] .bank.pipe.pipe_idx_cop_l3_busy;
         assign mon_pipe_idx_cop_req_ready        [4]        = shire_cache.\banks[4] .bank.pipe.pipe_idx_cop_req_ready;
         assign mon_pipe_idx_cop_req_valid        [4]        = shire_cache.\banks[4] .bank.pipe.pipe_idx_cop_req_valid;
         assign mon_pipe_idx_cop_cb_inv_start     [4]        = shire_cache.\banks[4] .bank.pipe.idx_cop_sm.idx_cop_sm_load & shire_cache.\banks[4] .bank.pipe.idx_cop_sm.cb_inv_op_xx_next;
         assign mon_pipe_idx_cop_req_info         [4]        = shire_cache.\banks[4] .bank.pipe.pipe_idx_cop_req_info;
         `ifndef ET_ASSERT_OFF
            assign shire_cache.\banks[4] .bank.pipe.dv_reset_esr_ctl_captured = dv_reset_esr_ctl_captured_r;
         `endif // ifndef ET_ASSERT_OFF
                                                                                   
         assign mon_pipe_tc_req_valid             [5]        = shire_cache.\banks[5] .bank.pipe.sub_bank.pipe_valid_tc;
         assign mon_pipe_tc_pipe_req_info         [5]        = shire_cache.\banks[5] .bank.pipe.sub_bank.pipe_req_info_tc;
         assign mon_pipe_tc_pipe_info             [5]        = shire_cache.\banks[5] .bank.pipe.sub_bank.pipe_info_tc;
         assign mon_pipe_tc_rsp_valid             [5]        = shire_cache.\banks[5] .bank.pipe_tag_rsp_valid;
         assign mon_pipe_tc_rsp_info              [5]        = shire_cache.\banks[5] .bank.pipe_tag_rsp_info;
         assign mon_dealloc_ag                    [5]        = shire_cache.\banks[5] .bank.reqq.reqq_entries_dealloc_hot;
         assign mon_reqq_l3_entries               [5]        = shire_cache.\banks[5] .bank.reqq.reqq_l3_entries;
         assign mon_alloc_l2_valid_ag             [5]        = shire_cache.\banks[5] .bank.reqq.alloc_l2_valid_ag;
         assign mon_alloc_l3_valid_ag             [5]        = shire_cache.\banks[5] .bank.reqq.alloc_l3_valid_ag;
         assign mon_alloc_l2_1hot_ag              [5]        = shire_cache.\banks[5] .bank.reqq.alloc_l2_1hot_ag;
         assign mon_alloc_l3_1hot_ag              [5]        = shire_cache.\banks[5] .bank.reqq.alloc_l3_1hot_ag;
         assign mon_alloc_l2_info_ag              [5]        = shire_cache.\banks[5] .bank.reqq.alloc_l2_info_ag;
         assign mon_alloc_l3_info_ag              [5]        = shire_cache.\banks[5] .bank.reqq.alloc_l3_info_ag;
         assign mon_alloc_l2_info_paired_ag       [5]        = shire_cache.\banks[5] .bank.reqq.alloc_l2_info_paired_ag;
         assign mon_alloc_l3_info_paired_ag       [5]        = shire_cache.\banks[5] .bank.reqq.alloc_l3_info_paired_ag;
         assign mon_alloc_l2_alloc_valid_paired_ag[5]        = shire_cache.\banks[5] .bank.reqq.allocs[0].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l3_alloc_valid_paired_ag[5]        = shire_cache.\banks[5] .bank.reqq.allocs[1].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l2_2nd_half_needed_ad   [5]        = shire_cache.\banks[5] .bank.reqq.allocs[0].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l3_2nd_half_needed_ad   [5]        = shire_cache.\banks[5] .bank.reqq.allocs[1].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l2_bank_info_ag         [5]        = shire_cache.\banks[5] .bank.reqq.bank_l2_info;
         assign mon_alloc_l3_bank_info_ag         [5]        = shire_cache.\banks[5] .bank.reqq.bank_l3_info;
         assign mon_pipe_rbuf_rd_req_valid        [5]        = shire_cache.\banks[5] .bank.pipe.pipe_rbuf_rd_req_valid;
         assign mon_pipe_rbuf_rd_req_ready        [5]        = shire_cache.\banks[5] .bank.pipe.pipe_rbuf_rd_req_ready;
         assign mon_pipe_rbuf_rd_req_info         [5]        = shire_cache.\banks[5] .bank.pipe.pipe_rbuf_rd_req_info;
         assign mon_esr_ctl                       [5]        = shire_cache.\banks[5] .bank.esr_ctl;
         assign mon_cbuf_matches_tag_victim       [5]        = shire_cache.\banks[5] .bank.reqq.coalesce_victim_matches_tag_victim;
         assign mon_pipe_cbuf_inv_req_valid       [5]        = shire_cache.\banks[5] .bank.pipe.pipe_cbuf_invalidate_req_valid;
         assign mon_pipe_cbuf_req_valid           [5]        = shire_cache.\banks[5] .bank.pipe.pipe_cbuf_req_valid;
         assign mon_pipe_cbuf_req_info            [5]        = shire_cache.\banks[5] .bank.pipe.pipe_cbuf_req_info;
         assign mon_pipe_cbuf_rsp_valid           [5]        = shire_cache.\banks[5] .bank.pipe.pipe_cbuf_rsp_valid;
         assign mon_pipe_cbuf_rsp_info            [5]        = shire_cache.\banks[5] .bank.pipe.pipe_cbuf_rsp_info;
         assign mon_pipe_idx_cop_l2_busy          [5]        = shire_cache.\banks[5] .bank.pipe.pipe_idx_cop_l2_busy;
         assign mon_pipe_idx_cop_l3_busy          [5]        = shire_cache.\banks[5] .bank.pipe.pipe_idx_cop_l3_busy;
         assign mon_pipe_idx_cop_req_ready        [5]        = shire_cache.\banks[5] .bank.pipe.pipe_idx_cop_req_ready;
         assign mon_pipe_idx_cop_req_valid        [5]        = shire_cache.\banks[5] .bank.pipe.pipe_idx_cop_req_valid;
         assign mon_pipe_idx_cop_cb_inv_start     [5]        = shire_cache.\banks[5] .bank.pipe.idx_cop_sm.idx_cop_sm_load & shire_cache.\banks[5] .bank.pipe.idx_cop_sm.cb_inv_op_xx_next;
         assign mon_pipe_idx_cop_req_info         [5]        = shire_cache.\banks[5] .bank.pipe.pipe_idx_cop_req_info;
         `ifndef ET_ASSERT_OFF
            assign shire_cache.\banks[5] .bank.pipe.dv_reset_esr_ctl_captured = dv_reset_esr_ctl_captured_r;
         `endif // ifndef ET_ASSERT_OFF
                                                                                   
         assign mon_pipe_tc_req_valid             [6]        = shire_cache.\banks[6] .bank.pipe.sub_bank.pipe_valid_tc;
         assign mon_pipe_tc_pipe_req_info         [6]        = shire_cache.\banks[6] .bank.pipe.sub_bank.pipe_req_info_tc;
         assign mon_pipe_tc_pipe_info             [6]        = shire_cache.\banks[6] .bank.pipe.sub_bank.pipe_info_tc;
         assign mon_pipe_tc_rsp_valid             [6]        = shire_cache.\banks[6] .bank.pipe_tag_rsp_valid;
         assign mon_pipe_tc_rsp_info              [6]        = shire_cache.\banks[6] .bank.pipe_tag_rsp_info;
         assign mon_dealloc_ag                    [6]        = shire_cache.\banks[6] .bank.reqq.reqq_entries_dealloc_hot;
         assign mon_reqq_l3_entries               [6]        = shire_cache.\banks[6] .bank.reqq.reqq_l3_entries;
         assign mon_alloc_l2_valid_ag             [6]        = shire_cache.\banks[6] .bank.reqq.alloc_l2_valid_ag;
         assign mon_alloc_l3_valid_ag             [6]        = shire_cache.\banks[6] .bank.reqq.alloc_l3_valid_ag;
         assign mon_alloc_l2_1hot_ag              [6]        = shire_cache.\banks[6] .bank.reqq.alloc_l2_1hot_ag;
         assign mon_alloc_l3_1hot_ag              [6]        = shire_cache.\banks[6] .bank.reqq.alloc_l3_1hot_ag;
         assign mon_alloc_l2_info_ag              [6]        = shire_cache.\banks[6] .bank.reqq.alloc_l2_info_ag;
         assign mon_alloc_l3_info_ag              [6]        = shire_cache.\banks[6] .bank.reqq.alloc_l3_info_ag;
         assign mon_alloc_l2_info_paired_ag       [6]        = shire_cache.\banks[6] .bank.reqq.alloc_l2_info_paired_ag;
         assign mon_alloc_l3_info_paired_ag       [6]        = shire_cache.\banks[6] .bank.reqq.alloc_l3_info_paired_ag;
         assign mon_alloc_l2_alloc_valid_paired_ag[6]        = shire_cache.\banks[6] .bank.reqq.allocs[0].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l3_alloc_valid_paired_ag[6]        = shire_cache.\banks[6] .bank.reqq.allocs[1].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l2_2nd_half_needed_ad   [6]        = shire_cache.\banks[6] .bank.reqq.allocs[0].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l3_2nd_half_needed_ad   [6]        = shire_cache.\banks[6] .bank.reqq.allocs[1].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l2_bank_info_ag         [6]        = shire_cache.\banks[6] .bank.reqq.bank_l2_info;
         assign mon_alloc_l3_bank_info_ag         [6]        = shire_cache.\banks[6] .bank.reqq.bank_l3_info;
         assign mon_pipe_rbuf_rd_req_valid        [6]        = shire_cache.\banks[6] .bank.pipe.pipe_rbuf_rd_req_valid;
         assign mon_pipe_rbuf_rd_req_ready        [6]        = shire_cache.\banks[6] .bank.pipe.pipe_rbuf_rd_req_ready;
         assign mon_pipe_rbuf_rd_req_info         [6]        = shire_cache.\banks[6] .bank.pipe.pipe_rbuf_rd_req_info;
         assign mon_esr_ctl                       [6]        = shire_cache.\banks[6] .bank.esr_ctl;
         assign mon_cbuf_matches_tag_victim       [6]        = shire_cache.\banks[6] .bank.reqq.coalesce_victim_matches_tag_victim;
         assign mon_pipe_cbuf_inv_req_valid       [6]        = shire_cache.\banks[6] .bank.pipe.pipe_cbuf_invalidate_req_valid;
         assign mon_pipe_cbuf_req_valid           [6]        = shire_cache.\banks[6] .bank.pipe.pipe_cbuf_req_valid;
         assign mon_pipe_cbuf_req_info            [6]        = shire_cache.\banks[6] .bank.pipe.pipe_cbuf_req_info;
         assign mon_pipe_cbuf_rsp_valid           [6]        = shire_cache.\banks[6] .bank.pipe.pipe_cbuf_rsp_valid;
         assign mon_pipe_cbuf_rsp_info            [6]        = shire_cache.\banks[6] .bank.pipe.pipe_cbuf_rsp_info;
         assign mon_pipe_idx_cop_l2_busy          [6]        = shire_cache.\banks[6] .bank.pipe.pipe_idx_cop_l2_busy;
         assign mon_pipe_idx_cop_l3_busy          [6]        = shire_cache.\banks[6] .bank.pipe.pipe_idx_cop_l3_busy;
         assign mon_pipe_idx_cop_req_ready        [6]        = shire_cache.\banks[6] .bank.pipe.pipe_idx_cop_req_ready;
         assign mon_pipe_idx_cop_req_valid        [6]        = shire_cache.\banks[6] .bank.pipe.pipe_idx_cop_req_valid;
         assign mon_pipe_idx_cop_cb_inv_start     [6]        = shire_cache.\banks[6] .bank.pipe.idx_cop_sm.idx_cop_sm_load & shire_cache.\banks[6] .bank.pipe.idx_cop_sm.cb_inv_op_xx_next;
         assign mon_pipe_idx_cop_req_info         [6]        = shire_cache.\banks[6] .bank.pipe.pipe_idx_cop_req_info;
         `ifndef ET_ASSERT_OFF
            assign shire_cache.\banks[6] .bank.pipe.dv_reset_esr_ctl_captured = dv_reset_esr_ctl_captured_r;
         `endif // ifndef ET_ASSERT_OFF
                                                                                   
         assign mon_pipe_tc_req_valid             [7]        = shire_cache.\banks[7] .bank.pipe.sub_bank.pipe_valid_tc;
         assign mon_pipe_tc_pipe_req_info         [7]        = shire_cache.\banks[7] .bank.pipe.sub_bank.pipe_req_info_tc;
         assign mon_pipe_tc_pipe_info             [7]        = shire_cache.\banks[7] .bank.pipe.sub_bank.pipe_info_tc;
         assign mon_pipe_tc_rsp_valid             [7]        = shire_cache.\banks[7] .bank.pipe_tag_rsp_valid;
         assign mon_pipe_tc_rsp_info              [7]        = shire_cache.\banks[7] .bank.pipe_tag_rsp_info;
         assign mon_dealloc_ag                    [7]        = shire_cache.\banks[7] .bank.reqq.reqq_entries_dealloc_hot;
         assign mon_reqq_l3_entries               [7]        = shire_cache.\banks[7] .bank.reqq.reqq_l3_entries;
         assign mon_alloc_l2_valid_ag             [7]        = shire_cache.\banks[7] .bank.reqq.alloc_l2_valid_ag;
         assign mon_alloc_l3_valid_ag             [7]        = shire_cache.\banks[7] .bank.reqq.alloc_l3_valid_ag;
         assign mon_alloc_l2_1hot_ag              [7]        = shire_cache.\banks[7] .bank.reqq.alloc_l2_1hot_ag;
         assign mon_alloc_l3_1hot_ag              [7]        = shire_cache.\banks[7] .bank.reqq.alloc_l3_1hot_ag;
         assign mon_alloc_l2_info_ag              [7]        = shire_cache.\banks[7] .bank.reqq.alloc_l2_info_ag;
         assign mon_alloc_l3_info_ag              [7]        = shire_cache.\banks[7] .bank.reqq.alloc_l3_info_ag;
         assign mon_alloc_l2_info_paired_ag       [7]        = shire_cache.\banks[7] .bank.reqq.alloc_l2_info_paired_ag;
         assign mon_alloc_l3_info_paired_ag       [7]        = shire_cache.\banks[7] .bank.reqq.alloc_l3_info_paired_ag;
         assign mon_alloc_l2_alloc_valid_paired_ag[7]        = shire_cache.\banks[7] .bank.reqq.allocs[0].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l3_alloc_valid_paired_ag[7]        = shire_cache.\banks[7] .bank.reqq.allocs[1].alloc.alloc_valid_paired_ag;
         assign mon_alloc_l2_2nd_half_needed_ad   [7]        = shire_cache.\banks[7] .bank.reqq.allocs[0].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l3_2nd_half_needed_ad   [7]        = shire_cache.\banks[7] .bank.reqq.allocs[1].alloc.alloc_2nd_half_needed_ad;
         assign mon_alloc_l2_bank_info_ag         [7]        = shire_cache.\banks[7] .bank.reqq.bank_l2_info;
         assign mon_alloc_l3_bank_info_ag         [7]        = shire_cache.\banks[7] .bank.reqq.bank_l3_info;
         assign mon_pipe_rbuf_rd_req_valid        [7]        = shire_cache.\banks[7] .bank.pipe.pipe_rbuf_rd_req_valid;
         assign mon_pipe_rbuf_rd_req_ready        [7]        = shire_cache.\banks[7] .bank.pipe.pipe_rbuf_rd_req_ready;
         assign mon_pipe_rbuf_rd_req_info         [7]        = shire_cache.\banks[7] .bank.pipe.pipe_rbuf_rd_req_info;
         assign mon_esr_ctl                       [7]        = shire_cache.\banks[7] .bank.esr_ctl;
         assign mon_cbuf_matches_tag_victim       [7]        = shire_cache.\banks[7] .bank.reqq.coalesce_victim_matches_tag_victim;
         assign mon_pipe_cbuf_inv_req_valid       [7]        = shire_cache.\banks[7] .bank.pipe.pipe_cbuf_invalidate_req_valid;
         assign mon_pipe_cbuf_req_valid           [7]        = shire_cache.\banks[7] .bank.pipe.pipe_cbuf_req_valid;
         assign mon_pipe_cbuf_req_info            [7]        = shire_cache.\banks[7] .bank.pipe.pipe_cbuf_req_info;
         assign mon_pipe_cbuf_rsp_valid           [7]        = shire_cache.\banks[7] .bank.pipe.pipe_cbuf_rsp_valid;
         assign mon_pipe_cbuf_rsp_info            [7]        = shire_cache.\banks[7] .bank.pipe.pipe_cbuf_rsp_info;
         assign mon_pipe_idx_cop_l2_busy          [7]        = shire_cache.\banks[7] .bank.pipe.pipe_idx_cop_l2_busy;
         assign mon_pipe_idx_cop_l3_busy          [7]        = shire_cache.\banks[7] .bank.pipe.pipe_idx_cop_l3_busy;
         assign mon_pipe_idx_cop_req_ready        [7]        = shire_cache.\banks[7] .bank.pipe.pipe_idx_cop_req_ready;
         assign mon_pipe_idx_cop_req_valid        [7]        = shire_cache.\banks[7] .bank.pipe.pipe_idx_cop_req_valid;
         assign mon_pipe_idx_cop_cb_inv_start     [7]        = shire_cache.\banks[7] .bank.pipe.idx_cop_sm.idx_cop_sm_load & shire_cache.\banks[7] .bank.pipe.idx_cop_sm.cb_inv_op_xx_next;
         assign mon_pipe_idx_cop_req_info         [7]        = shire_cache.\banks[7] .bank.pipe.pipe_idx_cop_req_info;
         `ifndef ET_ASSERT_OFF
            assign shire_cache.\banks[7] .bank.pipe.dv_reset_esr_ctl_captured = dv_reset_esr_ctl_captured_r;
         `endif // ifndef ET_ASSERT_OFF
         end
      `endif
   endgenerate

   generate
      for (gen_it = 0; gen_it < NUM_BANKS; gen_it++) begin : mon_pipe_tc
         initial begin
            mon_alloc_scp_valid_r[gen_it]        = {`SC_REQQ_DEPTH{1'b0}};
            mon_alloc_l3_idx_cop_valid_r[gen_it] = {`SC_REQQ_DEPTH{1'b0}};
            mon_esr_ctl_r[gen_it]                = sc_bank_esr_ctl_t'(0);
         end

         if (gen_it == 0) begin
            initial begin
               #0 if (m_sc_sb_en != 0) begin
                  forever @(negedge clock) begin
                     m_cycle_count <= m_cycle_count + 1;
                  end
               end
            end
         end

         always @(posedge clock) begin
            if (start_monitor === 1'b1) begin
               if (reset === 1'b1) begin
                  mon_alloc_scp_valid_r[gen_it]        = {`SC_REQQ_DEPTH{1'b0}};
                  mon_alloc_l3_idx_cop_valid_r[gen_it] = {`SC_REQQ_DEPTH{1'b0}};
                  mon_esr_ctl_r[gen_it]                = sc_bank_esr_ctl_t'(0);
                  if (gen_it == 0) begin
                     if (mon_reset_on === 1'b0) begin
                        m_rtl_port.assert_reset();
                     end
                     mon_neigh_id_0_r = {NUM_NEIGH_ID_MAX{1'b0}};
                     mon_neigh_id_1_r = {NUM_NEIGH_ID_MAX{1'b0}};
                     mon_neigh_id_2_r = {NUM_NEIGH_ID_MAX{1'b0}};
                     mon_neigh_id_3_r = {NUM_NEIGH_ID_MAX{1'b0}};
                     mon_reset_on  <= reset;
                     mon_reset_off <= ~reset;
                  end
               end
               else if (reset === 1'b0) begin
                  if (gen_it == 0) begin
                     if (mon_reset_off == 1'b0) begin
                        m_rtl_port.deassert_reset();
                     end
                     mon_reset_on  <= reset;
                     mon_reset_off <= ~reset;
                     if (m_sc_sb_en != 0) begin
                        if ( (mon_neigh_id_0 !== mon_neigh_id_0_r) ||
                             (mon_neigh_id_1 !== mon_neigh_id_1_r) ||
                             (mon_neigh_id_2 !== mon_neigh_id_2_r) ||
                             (mon_neigh_id_3 !== mon_neigh_id_3_r) ) begin
                           evl_sc_packet_u sc_config_packet;

                           sc_config_packet.packet = 0;
                           sc_config_packet.shire_config_info.info.neigh_id_0 = mon_neigh_id_0;
                           sc_config_packet.shire_config_info.info.neigh_id_1 = mon_neigh_id_1;
                           sc_config_packet.shire_config_info.info.neigh_id_2 = mon_neigh_id_2;
                           sc_config_packet.shire_config_info.info.neigh_id_3 = mon_neigh_id_3;
                           sc_config_packet.l3_index_config_info.info.bank_1_bit    = mon_esr_ctl[gen_it].esr_sc_bank_sel_b1;
                           sc_config_packet.l3_index_config_info.info.bank_2_bit    = mon_esr_ctl[gen_it].esr_sc_bank_sel_b2;
                           sc_config_packet.l3_index_config_info.info.subbank_0_bit = mon_esr_ctl[gen_it].esr_sc_sub_bank_sel_b0;
                           sc_config_packet.l3_index_config_info.info.subbank_1_bit = mon_esr_ctl[gen_it].esr_sc_sub_bank_sel_b1;
                           sc_config_packet.l3_index_config_info.info.subbank_2_bit = mon_esr_ctl[gen_it].esr_sc_sub_bank_sel_b2;
                           sc_config_packet.l3_index_config_info.info.shire_0_bit   = mon_esr_ctl[gen_it].esr_sc_shire_sel_b0;
                           m_rtl_port.send_packet(SHIRE_CONFIG, int'(SC_Address_L2),  0, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);
                           m_rtl_port.send_packet(SHIRE_CONFIG, int'(SC_Address_L3),  0, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);
                           m_rtl_port.send_packet(SHIRE_CONFIG, int'(SC_Address_Scp), 0, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);
                           mon_neigh_id_0_r = mon_neigh_id_0;
                           mon_neigh_id_1_r = mon_neigh_id_1;
                           mon_neigh_id_2_r = mon_neigh_id_2;
                           mon_neigh_id_3_r = mon_neigh_id_3;
                        end
                     end
                  end
                  if (m_sc_sb_en != 0) begin
                     if (^mon_rsp_dst_pop[gen_it] === 1'b1) begin
                        evl_sc_packet_u rsp_reqq_packet;

                        rsp_reqq_packet.packet = 0;
                        case (mon_rsp_dst_pop[gen_it])
                           8'b00000001: rsp_reqq_packet.rsp_reqq.info.neigh_id = evl_neighborhood_id_t'(0);
                           8'b00000010: rsp_reqq_packet.rsp_reqq.info.neigh_id = evl_neighborhood_id_t'(1);
                           8'b00000100: rsp_reqq_packet.rsp_reqq.info.neigh_id = evl_neighborhood_id_t'(2);
                           8'b00001000: rsp_reqq_packet.rsp_reqq.info.neigh_id = evl_neighborhood_id_t'(3);
                           8'b00010000: rsp_reqq_packet.rsp_reqq.info.neigh_id = evl_neighborhood_id_t'(4);
                           8'b00100000: rsp_reqq_packet.rsp_reqq.info.neigh_id = evl_neighborhood_id_t'(5);
                           8'b01000000: rsp_reqq_packet.rsp_reqq.info.neigh_id = evl_neighborhood_id_t'(6);
                           8'b10000000: rsp_reqq_packet.rsp_reqq.info.neigh_id = evl_neighborhood_id_t'(7);
                           default:     rsp_reqq_packet.rsp_reqq.info.neigh_id = ~evl_neighborhood_id_t'(0);
                        endcase
                        if (rsp_reqq_packet.rsp_reqq.info.neigh_id != ~evl_neighborhood_id_t'(0)) begin
                           rsp_reqq_packet.rsp_reqq.info.bank    = gen_it;
                           m_rtl_port.send_packet(SHIRE_CACHE_RSP_DSTQ, int'(SC_Address_L2), gen_it, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, rsp_reqq_packet);
                        end
                     end
                     if (mon_rsp_reqq_id_vld[gen_it] === 1) begin
                        evl_sc_packet_u rsp_reqq_packet;

                        rsp_reqq_packet.packet = 0;
                        rsp_reqq_packet.rsp_reqq.info.bank    = gen_it;
                        rsp_reqq_packet.rsp_reqq.info.reqq_id = mon_rsp_reqq_id[gen_it];
                        m_rtl_port.send_packet(SHIRE_CACHE_RSP_REQQ, int'(SC_Address_L2), gen_it, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, rsp_reqq_packet);
                     end
                     if ( (mon_pipe_idx_cop_req_valid[gen_it] === 1) && (mon_pipe_idx_cop_req_ready[gen_it] === 1) ) begin
                        int             addr_space;
                        evl_sc_packet_u idx_cop_req_packet;

                        if (mon_pipe_idx_cop_req_info[gen_it].opcode inside { SC_Idx_L3_Inv, SC_Idx_L3_Flush, SC_Idx_L3_Evict }) begin
                           addr_space = int'(SC_Address_L3);
                        end
                        else begin
                           addr_space = int'(SC_Address_L2);
                        end
                        idx_cop_req_packet.packet = 0;
                        idx_cop_req_packet.idx_cop_req.info.opcode  = evl_sc_reqq_opcode_t'(mon_pipe_idx_cop_req_info[gen_it].opcode);
                        idx_cop_req_packet.idx_cop_req.info.address = evl_paddr_t'(mon_pipe_idx_cop_req_info[gen_it].address);
                        m_rtl_port.send_packet(SHIRE_CACHE_IDX_COP_REQ, addr_space, gen_it, mon_pipe_idx_cop_req_info[gen_it].trans_id, 1'b0, evl_shire_id_t'(0), m_cycle_count, idx_cop_req_packet);

                        if (mon_pipe_idx_cop_req_info[gen_it].opcode inside { SC_Idx_All_Inv }) begin
                           // If it's an All_Inv, just tell all of the scoreboards to clear their caches.
                           m_rtl_port.send_packet(SHIRE_CACHE_ALL_INV, SC_Address_L2,  gen_it, mon_pipe_idx_cop_req_info[gen_it].trans_id, 1'b0, evl_shire_id_t'(0), m_cycle_count, idx_cop_req_packet);
                           m_rtl_port.send_packet(SHIRE_CACHE_ALL_INV, SC_Address_L3,  gen_it, mon_pipe_idx_cop_req_info[gen_it].trans_id, 1'b0, evl_shire_id_t'(0), m_cycle_count, idx_cop_req_packet);
                           m_rtl_port.send_packet(SHIRE_CACHE_ALL_INV, SC_Address_Scp, gen_it, mon_pipe_idx_cop_req_info[gen_it].trans_id, 1'b0, evl_shire_id_t'(0), m_cycle_count, idx_cop_req_packet);
                        end
                     end // if ( (mon_pipe_idx_cop_req_valid[gen_it] === 1) && (mon_pipe_idx_cop_req_ready[gen_it] === 1) )
                     if (mon_pipe_idx_cop_cb_inv_start[gen_it] == 1'b1) begin
                        evl_sc_packet_u idx_cop_packet;
                        idx_cop_packet.packet = 0;
                        m_rtl_port.send_packet(SHIRE_CACHE_CB_INV_START, SC_Address_L2, gen_it, 0, 1'b0, evl_shire_id_t'(0), m_cycle_count, idx_cop_packet);
                     end
                     if ( (mon_pipe_rbuf_rd_req_valid[gen_it] === 1) && (mon_pipe_rbuf_rd_req_ready[gen_it] === 1) ) begin
                        int             addr_space;
                        evl_sc_packet_u rbuf_rd_req_packet;

                        addr_space = (mon_pipe_rbuf_rd_req_info[gen_it].scp == 1'b1) ? int'(SC_Address_Scp) : int'(SC_Address_L2);

                        rbuf_rd_req_packet.packet = 0;
                        rbuf_rd_req_packet.pipe_rbuf_req.info.reqq_id = mon_pipe_rbuf_rd_req_info[gen_it].reqq_id;
                        rbuf_rd_req_packet.pipe_rbuf_req.info.scp     = mon_pipe_rbuf_rd_req_info[gen_it].scp;
                        rbuf_rd_req_packet.pipe_rbuf_req.info.address = mon_pipe_rbuf_rd_req_info[gen_it].address;
                        rbuf_rd_req_packet.pipe_rbuf_req.info.rbuf_id = mon_pipe_rbuf_rd_req_info[gen_it].rbuf_id;
                        m_rtl_port.send_packet(SHIRE_CACHE_RBUF_REQ, addr_space, gen_it, mon_pipe_rbuf_rd_req_info[gen_it].trans_id, 1'b0, evl_shire_id_t'(0), m_cycle_count, rbuf_rd_req_packet);
                     end
                     if (mon_esr_ctl[gen_it] !== mon_esr_ctl_r[gen_it]) begin
                        int             set_size;
                        evl_paddr_t     set_base;
                        evl_sc_packet_u sc_config_packet;

                        sc_config_packet.packet = 0;
                        sc_config_packet.l3_index_config_info.info.bank_0_bit    = mon_esr_ctl[gen_it].esr_sc_bank_sel_b0;
                        sc_config_packet.l3_index_config_info.info.bank_1_bit    = mon_esr_ctl[gen_it].esr_sc_bank_sel_b1;
                        sc_config_packet.l3_index_config_info.info.bank_2_bit    = mon_esr_ctl[gen_it].esr_sc_bank_sel_b2;
                        sc_config_packet.l3_index_config_info.info.subbank_0_bit = mon_esr_ctl[gen_it].esr_sc_sub_bank_sel_b0;
                        sc_config_packet.l3_index_config_info.info.subbank_1_bit = mon_esr_ctl[gen_it].esr_sc_sub_bank_sel_b1;
                        sc_config_packet.l3_index_config_info.info.subbank_2_bit = mon_esr_ctl[gen_it].esr_sc_sub_bank_sel_b2;
                        sc_config_packet.l3_index_config_info.info.shire_0_bit   = mon_esr_ctl[gen_it].esr_sc_shire_sel_b0;
                        m_index_config[int'(SC_RAM_TYPE_L3)].set_bit_info(NUM_BANKS,
                                                                          `SC_SUB_BANKS,
                                                                          mon_esr_ctl[gen_it].esr_sc_bank_sel_b0 + `EVL_LINE_ADDR_LSB,
                                                                          mon_esr_ctl[gen_it].esr_sc_bank_sel_b1 + `EVL_LINE_ADDR_LSB,
                                                                          mon_esr_ctl[gen_it].esr_sc_bank_sel_b2 + `EVL_LINE_ADDR_LSB,
                                                                          mon_esr_ctl[gen_it].esr_sc_sub_bank_sel_b0 + `EVL_LINE_ADDR_LSB,
                                                                          mon_esr_ctl[gen_it].esr_sc_sub_bank_sel_b1 + `EVL_LINE_ADDR_LSB,
                                                                          mon_esr_ctl[gen_it].esr_sc_sub_bank_sel_b2 + `EVL_LINE_ADDR_LSB,
                                                                          `SC_L3_SET_LSB);
                        m_rtl_port.send_packet(SHIRE_CACHE_L3_INDEX_CONFIG, int'(SC_Address_L2), gen_it, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);

                        sc_config_packet.packet = 0;
                        sc_config_packet.pipe_config_info.info.bank              = gen_it;
                        sc_config_packet.pipe_config_info.info.l2_rbuf_enable    = mon_esr_ctl[gen_it].esr_sc_l2_rbuf_enable;
                        sc_config_packet.pipe_config_info.info.scp_rbuf_enable   = mon_esr_ctl[gen_it].esr_sc_scp_rbuf_enable;
                        sc_config_packet.pipe_config_info.info.zero_state_enable = mon_esr_ctl[gen_it].esr_sc_zero_state_enable;
                        sc_config_packet.pipe_config_info.info.cbuf_enable       = mon_esr_ctl[gen_it].esr_sc_cbuf_enable;
                        sc_config_packet.pipe_config_info.info.num_entries       = (mon_esr_ctl[gen_it].esr_sc_cbuf_enable == 1'b0) ? 0 : $countones(mon_esr_ctl[gen_it].esr_sc_cbuf_entry_enable);
                        sc_config_packet.pipe_config_info.info.ram_delay         = mon_esr_ctl[gen_it].esr_sc_ram_delay;
                        m_rtl_port.send_packet(SHIRE_CACHE_PIPE_CONFIG, int'(SC_Address_L2), gen_it, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);
                        m_rtl_port.send_packet(SHIRE_CACHE_PIPE_CONFIG, int'(SC_Address_L3), gen_it, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);
                        m_rtl_port.send_packet(SHIRE_CACHE_PIPE_CONFIG, int'(SC_Address_Scp), gen_it, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);

                        sc_config_packet.packet = 0;
                        sc_config_packet.bank_config_info.info.bank = gen_it;

                        set_size = mon_esr_ctl[gen_it].esr_sc_l2_set_size;
                        set_base = evl_paddr_t'(mon_esr_ctl[gen_it].esr_sc_l2_set_base);
                        sc_config_packet.bank_config_info.info.num_ways                   = `SC_WAYS;
                        sc_config_packet.bank_config_info.info.set_size                   = set_size;
                        sc_config_packet.bank_config_info.info.set_base                   = set_base;
                        sc_config_packet.bank_config_info.info.bypass                     = mon_esr_ctl[gen_it].esr_sc_l2_bypass;
                        sc_config_packet.bank_config_info.info.remote_enable              = 1'b1;
                        sc_config_packet.bank_config_info.info.remote_l3_enable           = mon_esr_ctl[gen_it].esr_sc_remote_l3_enable;
                        sc_config_packet.bank_config_info.info.bank_lsb                   = `SC_BANK_LSB;
                        sc_config_packet.bank_config_info.info.err_rsp_enable             = mon_esr_ctl[gen_it].esr_sc_err_rsp_enable;
                        sc_config_packet.bank_config_info.info.err_interrupt_enable       = mon_esr_ctl[gen_it].esr_sc_err_interrupt_enable;
                        sc_config_packet.bank_config_info.info.two_shire_aliasing_use_lsb = 1'b0;
                        sc_config_packet.bank_config_info.info.all_shire_aliasing         = 1'b0;
                        m_index_config[int'(SC_RAM_TYPE_L2)].process_sc_config(`SC_WAYS, set_size, `SC_BANK_LSB, set_base);
                        m_index_config[int'(SC_RAM_TYPE_L2)].set_bit_info(NUM_BANKS, `SC_SUB_BANKS, `SC_BANK_LSB, `SC_BANK_LSB + 1, `SC_BANK_LSB +1, `SC_SUB_BANK_LSB, `SC_SUB_BANK_LSB + 1, `SC_SUB_BANK_LSB + 2, `SC_SET_LSB);
                        m_rtl_port.send_packet(SHIRE_CACHE_CONFIG, int'(SC_Address_L2), gen_it, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);

                        set_size = mon_esr_ctl[gen_it].esr_sc_l3_set_size;
                        set_base = evl_paddr_t'(mon_esr_ctl[gen_it].esr_sc_l3_set_base);
                        sc_config_packet.bank_config_info.info.num_ways                   = `SC_WAYS;
                        sc_config_packet.bank_config_info.info.set_size                   = set_size;
                        sc_config_packet.bank_config_info.info.set_base                   = set_base;
                        sc_config_packet.bank_config_info.info.bypass                     = mon_esr_ctl[gen_it].esr_sc_l3_bypass;
                        sc_config_packet.bank_config_info.info.remote_enable              = mon_esr_ctl[gen_it].esr_sc_remote_l3_enable;
                        sc_config_packet.bank_config_info.info.bank_lsb                   = `SC_L3_BANK_LSB;
                        sc_config_packet.bank_config_info.info.err_rsp_enable             = mon_esr_ctl[gen_it].esr_sc_err_rsp_enable;
                        sc_config_packet.bank_config_info.info.err_interrupt_enable       = mon_esr_ctl[gen_it].esr_sc_err_interrupt_enable;
                        sc_config_packet.bank_config_info.info.two_shire_aliasing_use_lsb = mon_esr_ctl[gen_it].esr_sc_two_shire_aliasing_use_shire_lsb;
                        sc_config_packet.bank_config_info.info.all_shire_aliasing         = mon_esr_ctl[gen_it].esr_sc_all_shire_aliasing;
                        m_index_config[int'(SC_RAM_TYPE_L3)].process_sc_config(`SC_WAYS, set_size, `SC_L3_BANK_LSB, set_base);
                        m_rtl_port.send_packet(SHIRE_CACHE_CONFIG, int'(SC_Address_L3), gen_it, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);

                        set_size = mon_esr_ctl[gen_it].esr_sc_scp_set_size * `SC_WAYS;
                        set_base = evl_paddr_t'(mon_esr_ctl[gen_it].esr_sc_scp_set_base);
                        sc_config_packet.bank_config_info.info.num_ways                   = 0;
                        sc_config_packet.bank_config_info.info.set_size                   = set_size;
                        sc_config_packet.bank_config_info.info.set_base                   = set_base;
                        sc_config_packet.bank_config_info.info.bypass                     = 1'b0;
                        sc_config_packet.bank_config_info.info.remote_enable              = mon_esr_ctl[gen_it].esr_sc_remote_scp_enable;
                        sc_config_packet.bank_config_info.info.remote_l3_enable           = mon_esr_ctl[gen_it].esr_sc_remote_l3_enable;
                        sc_config_packet.bank_config_info.info.bank_lsb                   = `SC_BANK_LSB;
                        sc_config_packet.bank_config_info.info.err_rsp_enable             = mon_esr_ctl[gen_it].esr_sc_err_rsp_enable;
                        sc_config_packet.bank_config_info.info.err_interrupt_enable       = mon_esr_ctl[gen_it].esr_sc_err_interrupt_enable;
                        sc_config_packet.bank_config_info.info.two_shire_aliasing_use_lsb = 1'b0;
                        sc_config_packet.bank_config_info.info.all_shire_aliasing         = 1'b0;
                        m_index_config[int'(SC_RAM_TYPE_SC)].process_sc_config(0, set_size, `SC_BANK_LSB, set_base);
                        m_index_config[int'(SC_RAM_TYPE_SC)].set_bit_info(NUM_BANKS, `SC_SUB_BANKS, `SC_BANK_LSB, `SC_BANK_LSB + 1, `SC_BANK_LSB +1, `SC_SUB_BANK_LSB, `SC_SUB_BANK_LSB + 1, `SC_SUB_BANK_LSB + 2, `SC_SET_LSB);
                        m_rtl_port.send_packet(SHIRE_CACHE_CONFIG, int'(SC_Address_Scp), gen_it, ~evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, sc_config_packet);

                        mon_esr_ctl_r[gen_it] = mon_esr_ctl[gen_it];
                     end
                     if (mon_pipe_tc_rsp_valid[gen_it:gen_it] === 1) begin
                        int             addr_space;
                        evl_sc_packet_u tc_tag_rsp_packet;

                        if (mon_pipe_tc_rsp_info[gen_it].opcode inside { SC_Idx_L3_Inv, SC_Idx_L3_Flush, SC_Idx_L3_Evict }) begin
                           addr_space = int'(SC_Address_L3);
                        end
                        else begin
                           addr_space = int'(mon_pipe_tc_rsp_info[gen_it].victim_address_space);
                        end
                        
                        tc_tag_rsp_packet.packet = 0;
                        tc_tag_rsp_packet.pipe_tag_rsp_tc.info.reqq_id             = mon_pipe_tc_rsp_info[gen_it].reqq_id;
                        tc_tag_rsp_packet.pipe_tag_rsp_tc.info.hit                 = mon_pipe_tc_rsp_info[gen_it].hit;
                        tc_tag_rsp_packet.pipe_tag_rsp_tc.info.victim              = mon_pipe_tc_rsp_info[gen_it].victim;
                        tc_tag_rsp_packet.pipe_tag_rsp_tc.info.victim_silent       = mon_pipe_tc_rsp_info[gen_it].victim_silent;
                        tc_tag_rsp_packet.pipe_tag_rsp_tc.info.victim_address      = mon_pipe_tc_rsp_info[gen_it].victim_address;
                        tc_tag_rsp_packet.pipe_tag_rsp_tc.info.victim_write_around = mon_pipe_tc_rsp_info[gen_it].victim_write_around;
                        tc_tag_rsp_packet.pipe_tag_rsp_tc.info.victim_qwen         = mon_pipe_tc_rsp_info[gen_it].victim_qwen;
                        tc_tag_rsp_packet.pipe_tag_rsp_tc.info.cbuf_match          = mon_cbuf_matches_tag_victim[gen_it];
                        tc_tag_rsp_packet.pipe_tag_rsp_tc.info.err_rsp             = evl_sc_pipe_err_rsp_t'(mon_pipe_tc_rsp_info[gen_it].err_rsp);
                        m_rtl_port.send_packet(SHIRE_CACHE_TC_TAG_RSP, addr_space, gen_it, mon_pipe_tc_rsp_info[gen_it].trans_id, 1'b0, evl_shire_id_t'(0), m_cycle_count, tc_tag_rsp_packet);
                     end
                     if (mon_pipe_tc_req_valid[gen_it:gen_it] === 1) begin
                        int             addr_space;
                        evl_sc_packet_u tc_req_packet;
                        evl_sc_packet_u tc_info_packet;

                        if ( (mon_pipe_tc_pipe_req_info[gen_it].address_space == SC_Address_Scp) ||
                             (mon_pipe_tc_pipe_req_info[gen_it].opcode inside { `ET_SCP_L2_REQQ_OPCODES }) ) begin
                           addr_space = int'(SC_Address_Scp);
                        end
                        else if (mon_pipe_tc_pipe_req_info[gen_it].opcode inside { SC_Idx_L2_Inv, SC_Idx_L2_Evict, SC_Idx_L2_Flush }) begin
                           addr_space = int'(SC_Address_L2);
                        end
                        else if (mon_pipe_tc_pipe_req_info[gen_it].opcode inside { SC_Idx_L3_Inv, SC_Idx_L3_Evict, SC_Idx_L3_Flush }) begin
                           addr_space = int'(SC_Address_L3);
                        end
                        else begin
                           addr_space = int'(mon_pipe_tc_pipe_req_info[gen_it].address_space);
                        end

                        tc_info_packet.packet = 0;
                        tc_info_packet.pipe_info_tc.info.pipe_set                = mon_pipe_tc_pipe_info[gen_it].pipe_set;
                        tc_info_packet.pipe_info_tc.info.pipe_sub_bank           = mon_pipe_tc_pipe_info[gen_it].pipe_sub_bank;
                        tc_info_packet.pipe_info_tc.info.tag_hit                 = mon_pipe_tc_pipe_info[gen_it].tag_hit;
                        tc_info_packet.pipe_info_tc.info.tag_ram_hit_way         = evl_way_t'(mon_pipe_tc_pipe_info[gen_it].tag_ram_hit_way);
                        tc_info_packet.pipe_info_tc.info.victim                  = mon_pipe_tc_pipe_info[gen_it].victim;
                        tc_info_packet.pipe_info_tc.info.data_rd_req             = mon_pipe_tc_pipe_info[gen_it].data_rd_req;
                        tc_info_packet.pipe_info_tc.info.data_wr_req             = mon_pipe_tc_pipe_info[gen_it].data_wr_req;
                        tc_info_packet.pipe_info_tc.info.data_wr_zero            = mon_pipe_tc_pipe_info[gen_it].data_wr_zero;
                        tc_info_packet.pipe_info_tc.info.data_wr_qwen            = mon_pipe_tc_pipe_info[gen_it].data_wr_qwen;
                        tc_info_packet.pipe_info_tc.info.dataq_sel_qwen          = mon_pipe_tc_pipe_info[gen_it].dataq_sel_qwen;
                        tc_info_packet.pipe_info_tc.info.merge_wr_data_in_rd_rsp = mon_pipe_tc_pipe_info[gen_it].merge_wr_data_in_rd_rsp;
                        tc_info_packet.pipe_info_tc.info.merge_rd_data_in_wr_req = mon_pipe_tc_pipe_info[gen_it].merge_rd_data_in_wr_req;
                        tc_info_packet.pipe_info_tc.info.data_rsp_req            = mon_pipe_tc_pipe_info[gen_it].data_rsp_req;
                        tc_info_packet.pipe_info_tc.info.data_rsp_zero           = mon_pipe_tc_pipe_info[gen_it].data_rsp_zero;
                        tc_info_packet.pipe_info_tc.info.data_rsp_qwen           = mon_pipe_tc_pipe_info[gen_it].data_rsp_qwen;

                        tc_req_packet.packet = 0;
                        tc_req_packet.pipe_req_tc.info.reqq_id   = mon_pipe_tc_pipe_req_info[gen_it].reqq_id;
                        tc_req_packet.pipe_req_tc.info.opcode    = evl_sc_reqq_opcode_t'(mon_pipe_tc_pipe_req_info[gen_it].opcode);
                        tc_req_packet.pipe_req_tc.info.address   = mon_pipe_tc_pipe_req_info[gen_it].address;
                        tc_req_packet.pipe_req_tc.info.zero_data = mon_pipe_tc_pipe_req_info[gen_it].zero_data;
                        tc_req_packet.pipe_req_tc.info.qwen      = mon_pipe_tc_pipe_req_info[gen_it].qwen;
                        tc_req_packet.pipe_req_tc.info.sc_size   = 1 << mon_pipe_tc_pipe_req_info[gen_it].size;

                        m_rtl_port.send_packet(SHIRE_CACHE_TC_INFO, addr_space, gen_it, mon_pipe_tc_pipe_req_info[gen_it].trans_id, 1'b0, evl_shire_id_t'(0), m_cycle_count, tc_info_packet);
                        m_rtl_port.send_packet(SHIRE_CACHE_TC_REQ,  addr_space, gen_it, mon_pipe_tc_pipe_req_info[gen_it].trans_id, 1'b0, evl_shire_id_t'(0), m_cycle_count, tc_req_packet);
                     end
                     if (mon_dealloc_ag[gen_it] !== 0) begin
                        evl_sc_packet_u reqq_dealloc_packet;

                        reqq_dealloc_packet.packet = 0;
                        
                        reqq_dealloc_packet.reqq_alloc.info.reqq_nhot = mon_dealloc_ag[gen_it] & ~mon_reqq_l3_entries[gen_it] & ~mon_alloc_scp_valid_r[gen_it] & ~mon_alloc_l3_idx_cop_valid_r[gen_it];
                        if (reqq_dealloc_packet.reqq_alloc.info.reqq_nhot != {`SC_REQQ_DEPTH{1'b0}}) begin
                           m_rtl_port.send_packet(SHIRE_CACHE_REQQ_DEALLOC, int'(SC_Address_L2), gen_it, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, reqq_dealloc_packet);
                        end

                        reqq_dealloc_packet.reqq_alloc.info.reqq_nhot = mon_dealloc_ag[gen_it] & mon_reqq_l3_entries[gen_it] & ~mon_alloc_scp_valid_r[gen_it] & ~mon_alloc_l3_idx_cop_valid_r[gen_it];
                        if (reqq_dealloc_packet.reqq_alloc.info.reqq_nhot != {`SC_REQQ_DEPTH{1'b0}}) begin
                           m_rtl_port.send_packet(SHIRE_CACHE_REQQ_DEALLOC, int'(SC_Address_L3), gen_it, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, reqq_dealloc_packet);
                        end

                        reqq_dealloc_packet.reqq_alloc.info.reqq_nhot = mon_dealloc_ag[gen_it] & mon_alloc_l3_idx_cop_valid_r[gen_it];
                        if (reqq_dealloc_packet.reqq_alloc.info.reqq_nhot != {`SC_REQQ_DEPTH{1'b0}}) begin
                           m_rtl_port.send_packet(SHIRE_CACHE_REQQ_DEALLOC, int'(SC_Address_L3), gen_it, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, reqq_dealloc_packet);
                           mon_alloc_l3_idx_cop_valid_r[gen_it] &= ~mon_dealloc_ag[gen_it];
                        end

                        reqq_dealloc_packet.reqq_alloc.info.reqq_nhot = mon_dealloc_ag[gen_it] & mon_alloc_scp_valid_r[gen_it];
                        if (reqq_dealloc_packet.reqq_alloc.info.reqq_nhot != {`SC_REQQ_DEPTH{1'b0}}) begin
                           m_rtl_port.send_packet(SHIRE_CACHE_REQQ_DEALLOC, int'(SC_Address_Scp), gen_it, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, reqq_dealloc_packet);
                           mon_alloc_scp_valid_r[gen_it] &= ~mon_dealloc_ag[gen_it];
                        end
                     end
                     if (mon_alloc_l2_valid_ag[gen_it] !== 0) begin
                        bit             use_remote_shire_id;
                        int             addr_space;
                        evl_paddr_t     addr;
                        evl_shire_id_t  remote_shire_id;
                        evl_sc_packet_u reqq_alloc_packet;

                        addr       = mon_alloc_l2_info_ag[gen_it].address;
                        addr_space = int'(mon_alloc_l2_info_ag[gen_it].address_space);
                        
                        if (int'(addr[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) begin
                           use_remote_shire_id = 1'b1;
                           remote_shire_id     = evl_shire_id_t'(addr[`SC_SCP_SHIRE_SEL_ADDR_RANGE]);
                        end
                        else begin
                           use_remote_shire_id = 1'b0;
                           remote_shire_id     = evl_shire_id_t'(0);
                        end
                        

                        if (mon_alloc_l2_info_ag[gen_it].opcode inside { SC_Idx_L2_Inv, SC_Idx_L2_Evict, SC_Idx_L2_Flush }) begin
                           addr_space = int'(SC_Address_L2);
                           // no remote shire id for index cacheops
                           use_remote_shire_id = 1'b0;
                           remote_shire_id     = evl_shire_id_t'(0);
                        end
                        else if (mon_alloc_l2_info_ag[gen_it].opcode inside { SC_Idx_L3_Inv, SC_Idx_L3_Evict, SC_Idx_L3_Flush, SC_Sync }) begin
                           addr_space = int'(SC_Address_L3);
                           mon_alloc_l3_idx_cop_valid_r[gen_it][mon_alloc_l2_info_ag[gen_it].reqq_id] = 1'b1;
                           // no remote shire id for index cacheops
                           use_remote_shire_id = 1'b0;
                           remote_shire_id     = evl_shire_id_t'(0);
                        end
                        
                        if (addr_space == int'(SC_Address_Scp)) begin
                           mon_alloc_scp_valid_r[gen_it] |= mon_alloc_l2_1hot_ag[gen_it];
                        end

                        reqq_alloc_packet.packet = 0;
                        reqq_alloc_packet.reqq_alloc.info.tag_id        = ~evl_bus_req_id_t'(0);
                        reqq_alloc_packet.reqq_alloc.info.reqq_id       = mon_alloc_l2_info_ag[gen_it].reqq_id;
                        reqq_alloc_packet.reqq_alloc.info.port          = mon_alloc_l2_bank_info_ag[gen_it].port_id;
                        reqq_alloc_packet.reqq_alloc.info.orig_opcode   = evl_sc_reqq_orig_opcode_t'(mon_alloc_l2_info_ag[gen_it].orig_opcode);
                        reqq_alloc_packet.reqq_alloc.info.address       = mon_alloc_l2_info_ag[gen_it].address;
                        reqq_alloc_packet.reqq_alloc.info.address_space = evl_shire_cache_types_pkg::evl_sc_address_space_t'(addr_space);

                        if (mon_alloc_l2_alloc_valid_paired_ag[gen_it] == 1'b0) begin
                           if (mon_alloc_l2_2nd_half_needed_ad[gen_it] === 1'b1) begin
                              reqq_alloc_packet.reqq_alloc.info.second_alloc = 1'b1;
                              reqq_alloc_packet.reqq_alloc.info.paired_id    = mon_alloc_l2_info_ag[gen_it].paired_id;
                              m_rtl_port.send_packet(SHIRE_CACHE_REQQ_ALLOC, addr_space, gen_it, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, reqq_alloc_packet);
                           end
                           else begin
                              reqq_alloc_packet.reqq_alloc.info.second_alloc = 1'b0;
                              reqq_alloc_packet.reqq_alloc.info.paired_id    = mon_alloc_l2_info_ag[gen_it].reqq_id;
                              m_rtl_port.send_packet(SHIRE_CACHE_REQQ_ALLOC, addr_space, gen_it, mon_alloc_l2_info_ag[gen_it].trans_id, use_remote_shire_id, remote_shire_id, m_cycle_count, reqq_alloc_packet);
                           end
                        end
                        else begin
                           reqq_alloc_packet.reqq_alloc.info.second_alloc = 1'b0;
                           reqq_alloc_packet.reqq_alloc.info.paired_id    = mon_alloc_l2_info_ag[gen_it].reqq_id;
                           m_rtl_port.send_packet(SHIRE_CACHE_REQQ_ALLOC, addr_space, gen_it, mon_alloc_l2_info_ag[gen_it].trans_id, use_remote_shire_id, remote_shire_id, m_cycle_count, reqq_alloc_packet);

                           reqq_alloc_packet.reqq_alloc.info.reqq_id      = mon_alloc_l2_info_paired_ag[gen_it].reqq_id;
                           reqq_alloc_packet.reqq_alloc.info.paired_id    = mon_alloc_l2_info_paired_ag[gen_it].paired_id;
                           reqq_alloc_packet.reqq_alloc.info.second_alloc = 1'b1;
                           m_rtl_port.send_packet(SHIRE_CACHE_REQQ_ALLOC, addr_space, gen_it, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, reqq_alloc_packet);

                           if (addr_space == int'(SC_Address_Scp)) begin
                              mon_alloc_scp_valid_r[gen_it][reqq_alloc_packet.reqq_alloc.info.reqq_id] = 1'b1;
                           end
                        end
                     end
                     if (mon_alloc_l3_valid_ag[gen_it] !== 0) begin
                        bit             use_remote_shire_id;
                        evl_shire_id_t  remote_shire_id;
                        int             addr_space;
                        evl_paddr_t     addr;
                        evl_sc_packet_u reqq_alloc_packet;

                        addr = mon_alloc_l3_info_ag[gen_it].address;
                        if (int'(addr[`SC_SCP_REGION_SEL_ADDR_RANGE]) == int'(`SC_SCP_REGION_ID)) begin
                           use_remote_shire_id = 1'b1;
                           remote_shire_id     = evl_shire_id_t'(addr[`SC_SCP_SHIRE_SEL_ADDR_RANGE]);
                        end
                        else begin
                           use_remote_shire_id = 1'b0;
                           remote_shire_id     = evl_shire_id_t'(0);
                        end

                        addr_space = int'(mon_alloc_l3_info_ag[gen_it].address_space);
                        if (use_remote_shire_id == 1'b1) begin
                           addr_space = int'(SC_Address_Scp);
                        end
                        if (mon_alloc_l3_info_ag[gen_it].opcode inside { SC_Idx_L3_Inv, SC_Idx_L3_Evict, SC_Idx_L3_Flush, SC_Sync}) begin
                           addr_space = int'(SC_Address_L3);
                           mon_alloc_l3_idx_cop_valid_r[gen_it][mon_alloc_l3_info_ag[gen_it].reqq_id] = 1'b1;
                        end
                        if (addr_space == int'(SC_Address_Scp)) begin
                           mon_alloc_scp_valid_r[gen_it] |= mon_alloc_l3_1hot_ag[gen_it];
                        end

                        reqq_alloc_packet.packet = 0;
                        reqq_alloc_packet.reqq_alloc.info.tag_id        = evl_bus_req_id_t'({ mon_alloc_l3_info_ag[gen_it].source, mon_alloc_l3_info_ag[gen_it].tag_id });
                        reqq_alloc_packet.reqq_alloc.info.reqq_id       = mon_alloc_l3_info_ag[gen_it].reqq_id;
                        reqq_alloc_packet.reqq_alloc.info.port          = `EVL_BASE_PORT_NUM_AXI;
                        reqq_alloc_packet.reqq_alloc.info.orig_opcode   = evl_sc_reqq_orig_opcode_t'(mon_alloc_l3_info_ag[gen_it].orig_opcode);
                        reqq_alloc_packet.reqq_alloc.info.address       = mon_alloc_l3_info_ag[gen_it].address;
                        reqq_alloc_packet.reqq_alloc.info.address_space = evl_shire_cache_types_pkg::evl_sc_address_space_t'(addr_space);

                        if (mon_alloc_l3_alloc_valid_paired_ag[gen_it] == 1'b0) begin
                           if (mon_alloc_l3_2nd_half_needed_ad[gen_it] === 1'b1) begin
                              reqq_alloc_packet.reqq_alloc.info.second_alloc = 1'b1;
                              reqq_alloc_packet.reqq_alloc.info.paired_id    = mon_alloc_l3_info_ag[gen_it].paired_id;
                              m_rtl_port.send_packet(SHIRE_CACHE_REQQ_ALLOC, addr_space, gen_it, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, reqq_alloc_packet);
                           end
                           else begin
                              reqq_alloc_packet.reqq_alloc.info.second_alloc = 1'b0;
                              reqq_alloc_packet.reqq_alloc.info.paired_id    = mon_alloc_l3_info_ag[gen_it].reqq_id;
                              m_rtl_port.send_packet(SHIRE_CACHE_REQQ_ALLOC, addr_space, gen_it, mon_alloc_l3_info_ag[gen_it].trans_id, use_remote_shire_id,remote_shire_id, m_cycle_count, reqq_alloc_packet);
                           end
                        end
                        else begin
                           reqq_alloc_packet.reqq_alloc.info.second_alloc = 1'b0;
                           reqq_alloc_packet.reqq_alloc.info.paired_id    = mon_alloc_l3_info_ag[gen_it].reqq_id;
                           m_rtl_port.send_packet(SHIRE_CACHE_REQQ_ALLOC, addr_space, gen_it, mon_alloc_l3_info_ag[gen_it].trans_id, use_remote_shire_id, remote_shire_id, m_cycle_count, reqq_alloc_packet);

                           reqq_alloc_packet.reqq_alloc.info.reqq_id      = mon_alloc_l3_info_paired_ag[gen_it].reqq_id;
                           reqq_alloc_packet.reqq_alloc.info.paired_id    = mon_alloc_l3_info_paired_ag[gen_it].paired_id;
                           reqq_alloc_packet.reqq_alloc.info.second_alloc = 1'b1;
                           m_rtl_port.send_packet(SHIRE_CACHE_REQQ_ALLOC, addr_space, gen_it, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, reqq_alloc_packet);
                        end
                     end
                     if ((mon_pipe_cbuf_req_valid[gen_it] == 1'b1) && (`evl_log_check_verbosity(UVM_FULL, VMOD_SC_CBUF) != 0)) begin
                        if (mon_pipe_cbuf_req_info[gen_it].opcode inside { SC_CBuf_Clear }) begin
                            `evl_log_addr_trid_id(UVM_FULL, VMOD_SC_CBUF, abstract_name, mon_pipe_cbuf_req_info[gen_it].clear_address, mon_pipe_cbuf_req_info[gen_it].trans_id, mon_pipe_cbuf_req_info[gen_it].reqq_id, $sformatf("CBUF request (bank %0d): %p 0x%x", gen_it, mon_pipe_cbuf_req_info[gen_it].opcode, mon_pipe_cbuf_req_info[gen_it].clear_address))
                        end
                        else if (mon_pipe_cbuf_req_info[gen_it].opcode inside { SC_CBuf_Install, SC_CBuf_MruUpdate }) begin
                            `evl_log_addr_trid_id(UVM_FULL, VMOD_SC_CBUF, abstract_name, mon_pipe_cbuf_req_info[gen_it].install_address, mon_pipe_cbuf_req_info[gen_it].trans_id, mon_pipe_cbuf_req_info[gen_it].reqq_id, $sformatf("CBUF request (bank %0d): %p 0x%x", gen_it, mon_pipe_cbuf_req_info[gen_it].opcode, mon_pipe_cbuf_req_info[gen_it].install_address))
                        end
                        else if (mon_pipe_cbuf_req_info[gen_it].opcode inside { SC_CBuf_Bypass }) begin
                            `evl_log_addr_trid_id(UVM_FULL, VMOD_SC_CBUF, abstract_name, ~evl_paddr_t'(0), mon_pipe_cbuf_req_info[gen_it].trans_id, mon_pipe_cbuf_req_info[gen_it].reqq_id, $sformatf("CBUF request (bank %0d): %p", gen_it, mon_pipe_cbuf_req_info[gen_it].opcode))
                        end
                        else begin
                            `evl_log_addr_trid_id(UVM_FULL, VMOD_SC_CBUF, abstract_name, mon_pipe_cbuf_req_info[gen_it].install_address, mon_pipe_cbuf_req_info[gen_it].trans_id, mon_pipe_cbuf_req_info[gen_it].reqq_id, $sformatf("CBUF request (bank %0d): %p 0x%x (0x%x)", gen_it, mon_pipe_cbuf_req_info[gen_it].opcode, mon_pipe_cbuf_req_info[gen_it].install_address, mon_pipe_cbuf_req_info[gen_it].clear_address))
                        end
                     end
                  end
               end
               else if (reset === 1'b1) begin
                  mon_alloc_scp_valid_r[gen_it]  = {`SC_REQQ_DEPTH{1'b0}};
               end
               mon_pipe_tc_signals[gen_it:gen_it] <= ~mon_pipe_tc_signals[gen_it:gen_it];
            end
         end
      end
   endgenerate


   generate
      `ifndef ET_USE_BIST_RTL
         for (gen_it = 0; gen_it < NUM_BANKS; gen_it++) begin : cbuf_pipe_tc
            initial begin : dv_cbuf_checks
               reg [31:0] ii;
               ii = 32'h00000000;
               @(posedge clock);
               wait ((start_monitor === 1'b1) && (m_send_cbuf_status[gen_it:gen_it] == 1'b1) && (m_sc_sb_en != 0));
               for (ii = 0; ii < `SC_CBUF_SIZE; ii = ii + 'd1) begin
                  if (shire_cache.banks[gen_it].bank.pipe.cbuf.cbuf_mem_valid[ii] == 1'b1) begin
                     evl_sc_packet_u cbuf_status_packet;
            
                     cbuf_status_packet.cbuf_status.info.entry_num = ii[7:0];
                     cbuf_status_packet.cbuf_status.info.valid     = shire_cache.banks[gen_it].bank.pipe.cbuf.cbuf_mem_valid[ii];
                     cbuf_status_packet.cbuf_status.info.address   = shire_cache.banks[gen_it].bank.pipe.cbuf.cbuf_mem[ii];
                     m_rtl_port.send_packet(SHIRE_CACHE_CBUF_STATUS, int'(SC_Address_L2), gen_it, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, cbuf_status_packet);
                  end
               end
            end
         end
      `else

         initial begin : dv_cbuf_checks0
            reg [31:0] ii;
            ii = 32'h00000000;
            @(posedge clock);
            wait ((start_monitor === 1'b1) && (m_send_cbuf_status[0] == 1'b1) && (m_sc_sb_en != 0));
            for (ii = 0; ii < `SC_CBUF_SIZE; ii = ii + 'd1) begin
               if (shire_cache.\banks[0] .bank.pipe.cbuf.cbuf_mem_valid[ii] == 1'b1) begin
                  evl_sc_packet_u cbuf_status_packet;

                  cbuf_status_packet.cbuf_status.info.entry_num = ii[7:0];
                  cbuf_status_packet.cbuf_status.info.valid     = shire_cache.\banks[0] .bank.pipe.cbuf.cbuf_mem_valid[ii];
                  cbuf_status_packet.cbuf_status.info.address   = shire_cache.\banks[0] .bank.pipe.cbuf.cbuf_mem[ii];
                  m_rtl_port.send_packet(SHIRE_CACHE_CBUF_STATUS, int'(SC_Address_L2), 0, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, cbuf_status_packet);
               end
            end
         end

         initial begin : dv_cbuf_checks1
            reg [31:0] ii;
            ii = 32'h00000000;
            @(posedge clock);
            wait ((start_monitor === 1'b1) && (m_send_cbuf_status[1] == 1'b1) && (m_sc_sb_en != 0));
            for (ii = 0; ii < `SC_CBUF_SIZE; ii = ii + 'd1) begin
               if (shire_cache.\banks[1] .bank.pipe.cbuf.cbuf_mem_valid[ii] == 1'b1) begin
                  evl_sc_packet_u cbuf_status_packet;

                  cbuf_status_packet.cbuf_status.info.entry_num = ii[7:0];
                  cbuf_status_packet.cbuf_status.info.valid     = shire_cache.\banks[1] .bank.pipe.cbuf.cbuf_mem_valid[ii];
                  cbuf_status_packet.cbuf_status.info.address   = shire_cache.\banks[1] .bank.pipe.cbuf.cbuf_mem[ii];
                  m_rtl_port.send_packet(SHIRE_CACHE_CBUF_STATUS, int'(SC_Address_L2), 1, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, cbuf_status_packet);
               end
            end
         end

         initial begin : dv_cbuf_checks2
            reg [31:0] ii;
            ii = 32'h00000000;
            @(posedge clock);
            wait ((start_monitor === 1'b1) && (m_send_cbuf_status[2] == 1'b1) && (m_sc_sb_en != 0));
            for (ii = 0; ii < `SC_CBUF_SIZE; ii = ii + 'd1) begin
               if (shire_cache.\banks[2] .bank.pipe.cbuf.cbuf_mem_valid[ii] == 1'b1) begin
                  evl_sc_packet_u cbuf_status_packet;

                  cbuf_status_packet.cbuf_status.info.entry_num = ii[7:0];
                  cbuf_status_packet.cbuf_status.info.valid     = shire_cache.\banks[2] .bank.pipe.cbuf.cbuf_mem_valid[ii];
                  cbuf_status_packet.cbuf_status.info.address   = shire_cache.\banks[2] .bank.pipe.cbuf.cbuf_mem[ii];
                  m_rtl_port.send_packet(SHIRE_CACHE_CBUF_STATUS, int'(SC_Address_L2), 2, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, cbuf_status_packet);
               end
            end
         end

         initial begin : dv_cbuf_checks3
            reg [31:0] ii;
            ii = 32'h00000000;
            @(posedge clock);
            wait ((start_monitor === 1'b1) && (m_send_cbuf_status[3] == 1'b1) && (m_sc_sb_en != 0));
            for (ii = 0; ii < `SC_CBUF_SIZE; ii = ii + 'd1) begin
               if (shire_cache.\banks[3] .bank.pipe.cbuf.cbuf_mem_valid[ii] == 1'b1) begin
                  evl_sc_packet_u cbuf_status_packet;

                  cbuf_status_packet.cbuf_status.info.entry_num = ii[7:0];
                  cbuf_status_packet.cbuf_status.info.valid     = shire_cache.\banks[3] .bank.pipe.cbuf.cbuf_mem_valid[ii];
                  cbuf_status_packet.cbuf_status.info.address   = shire_cache.\banks[3] .bank.pipe.cbuf.cbuf_mem[ii];
                  m_rtl_port.send_packet(SHIRE_CACHE_CBUF_STATUS, int'(SC_Address_L2), 3, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, cbuf_status_packet);
               end
            end
         end

         if (NUM_BANKS == 8) begin : b8_cbuf
         initial begin : dv_cbuf_checks4
            reg [31:0] ii;
            ii = 32'h00000000;
            @(posedge clock);
            wait ((start_monitor === 1'b1) && (m_send_cbuf_status[4] == 1'b1) && (m_sc_sb_en != 0));
            for (ii = 0; ii < `SC_CBUF_SIZE; ii = ii + 'd1) begin
               if (shire_cache.\banks[4] .bank.pipe.cbuf.cbuf_mem_valid[ii] == 1'b1) begin
                  evl_sc_packet_u cbuf_status_packet;

                  cbuf_status_packet.cbuf_status.info.entry_num = ii[7:0];
                  cbuf_status_packet.cbuf_status.info.valid     = shire_cache.\banks[4] .bank.pipe.cbuf.cbuf_mem_valid[ii];
                  cbuf_status_packet.cbuf_status.info.address   = shire_cache.\banks[4] .bank.pipe.cbuf.cbuf_mem[ii];
                  m_rtl_port.send_packet(SHIRE_CACHE_CBUF_STATUS, int'(SC_Address_L2), 4, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, cbuf_status_packet);
               end
            end
         end

         initial begin : dv_cbuf_checks5
            reg [31:0] ii;
            ii = 32'h00000000;
            @(posedge clock);
            wait ((start_monitor === 1'b1) && (m_send_cbuf_status[5] == 1'b1) && (m_sc_sb_en != 0));
            for (ii = 0; ii < `SC_CBUF_SIZE; ii = ii + 'd1) begin
               if (shire_cache.\banks[5] .bank.pipe.cbuf.cbuf_mem_valid[ii] == 1'b1) begin
                  evl_sc_packet_u cbuf_status_packet;

                  cbuf_status_packet.cbuf_status.info.entry_num = ii[7:0];
                  cbuf_status_packet.cbuf_status.info.valid     = shire_cache.\banks[5] .bank.pipe.cbuf.cbuf_mem_valid[ii];
                  cbuf_status_packet.cbuf_status.info.address   = shire_cache.\banks[5] .bank.pipe.cbuf.cbuf_mem[ii];
                  m_rtl_port.send_packet(SHIRE_CACHE_CBUF_STATUS, int'(SC_Address_L2), 5, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, cbuf_status_packet);
               end
            end
         end

         initial begin : dv_cbuf_checks6
            reg [31:0] ii;
            ii = 32'h00000000;
            @(posedge clock);
            wait ((start_monitor === 1'b1) && (m_send_cbuf_status[6] == 1'b1) && (m_sc_sb_en != 0));
            for (ii = 0; ii < `SC_CBUF_SIZE; ii = ii + 'd1) begin
               if (shire_cache.\banks[6] .bank.pipe.cbuf.cbuf_mem_valid[ii] == 1'b1) begin
                  evl_sc_packet_u cbuf_status_packet;

                  cbuf_status_packet.cbuf_status.info.entry_num = ii[7:0];
                  cbuf_status_packet.cbuf_status.info.valid     = shire_cache.\banks[6] .bank.pipe.cbuf.cbuf_mem_valid[ii];
                  cbuf_status_packet.cbuf_status.info.address   = shire_cache.\banks[6] .bank.pipe.cbuf.cbuf_mem[ii];
                  m_rtl_port.send_packet(SHIRE_CACHE_CBUF_STATUS, int'(SC_Address_L2), 6, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, cbuf_status_packet);
               end
            end
         end

         initial begin : dv_cbuf_checks7
            reg [31:0] ii;
            ii = 32'h00000000;
            @(posedge clock);
            wait ((start_monitor === 1'b1) && (m_send_cbuf_status[7] == 1'b1) && (m_sc_sb_en != 0));
            for (ii = 0; ii < `SC_CBUF_SIZE; ii = ii + 'd1) begin
               if (shire_cache.\banks[7] .bank.pipe.cbuf.cbuf_mem_valid[ii] == 1'b1) begin
                  evl_sc_packet_u cbuf_status_packet;

                  cbuf_status_packet.cbuf_status.info.entry_num = ii[7:0];
                  cbuf_status_packet.cbuf_status.info.valid     = shire_cache.\banks[7] .bank.pipe.cbuf.cbuf_mem_valid[ii];
                  cbuf_status_packet.cbuf_status.info.address   = shire_cache.\banks[7] .bank.pipe.cbuf.cbuf_mem[ii];
                  m_rtl_port.send_packet(SHIRE_CACHE_CBUF_STATUS, int'(SC_Address_L2), 7, evl_trans_id_t'(0), 1'b0, evl_shire_id_t'(0), m_cycle_count, cbuf_status_packet);
               end
            end
         end
         end
      `endif
   endgenerate



   //
   // The RTL port is used to send reset and clock information to the verification component.  We
   // want the clock ping information to occur after all monitor information has been sent to the
   // RTL port.
   //
   assign mon_pipe_signal_hi = (mon_pipe_tc_signals == {NUM_BANKS{1'b1}}) ? 1'b1 : 1'b0;
   assign mon_pipe_signal_lo = (mon_pipe_tc_signals == {NUM_BANKS{1'b0}}) ? 1'b1 : 1'b0;

   always @(posedge mon_pipe_signal_hi or posedge mon_pipe_signal_lo) begin
      mon_pipe_signal_done = ~mon_pipe_signal_done;
   end

   always @(mon_pipe_signal_done) begin
      m_rtl_port.ping_clock();
   end


   wire APB_to_bank_ESRs_t   [NUM_BANKS-1:0] mon_esr_req;
   wire APB_from_bank_ESRs_t [NUM_BANKS-1:0] mon_esr_rsp;

   genvar esr_idx;
   generate
      for (esr_idx = 0; esr_idx < NUM_BANKS; esr_idx = esr_idx + 1) begin : m_apb_esr
         assign mon_esr_req[esr_idx] = shire_cache.APB_ESR_req[esr_idx];
         assign mon_esr_rsp[esr_idx] = shire_cache.APB_ESR_rsp[esr_idx];

         evl_apb_interface #(.AGENT_ID    (esr_idx),
                             .ADDR_WIDTH  (`ESR_CACHE_BANK_APB_AD_PP_WIDTH),
                             .DATA_WIDTH  (`ESR_APB_D_WIDTH)) m_apb_esr (
               .isolate      ( 1'b0                             ),
               .pclk         ( clock                            ),
               .presetn      ( ~reset                           ),
               .penable      ( mon_esr_req[esr_idx].apb_penable ),
               .psel         ( mon_esr_req[esr_idx].apb_psel    ),
               .pwrite       ( mon_esr_req[esr_idx].apb_pwrite  ),
               .paddr        ( mon_esr_req[esr_idx].apb_paddr   ),
               .pwdata       ( mon_esr_req[esr_idx].apb_pwdata  ),
               .pready       ( mon_esr_rsp[esr_idx].apb_pready  ),
               .prdata       ( mon_esr_rsp[esr_idx].apb_prdata  ),
               .pslverr      ( mon_esr_rsp[esr_idx].apb_pslverr )
            );
      end
   endgenerate


   initial begin
      int value;

      if (`et_value_plusargs("dv_reset_esr_ctl_captured=%d", value) == 0) begin
         value = 0;
      end
      dv_reset_esr_ctl_captured_r = value[0:0];
      @(posedge clock);
      wait ((start_monitor === 1'b1) && (m_sc_sb_en != 0));
      m_rtl_port.wait_to_send_final_state();
      @(negedge clock);
      while (~m_send_cbuf_status != {NUM_BANKS{1'b0}}) begin
         #0 m_send_cbuf_status = (m_send_cbuf_status << 1) | { {(NUM_BANKS - 1){1'b0}}, 1'b1 };
      end
      @(negedge clock);
//    m_rtl_port.final_state_sent();
   end

   `ifndef ET_USE_ASIC_MEMS
      `ifndef ET_USE_BIST_RTL

         //-----------------------------------------------------------------------------------------
         //
         // Function: mod_extract_state
         //
         function automatic void mod_extract_state(input  bit [`SC_TAG_STATE_RAM_DATA_SIZE-1:0] tag_state_in,
                                                   output bit [6:0]                             tag_way_state[`SC_WAYS]);
            bit [`SC_TAG_STATE_RAM_DATA_SIZE-1:0] tag_state;

            tag_state = tag_state_in >> 5; // Remove LRU (5 bits)
            for (int ii = 0; ii < `SC_WAYS; ii++) begin
               tag_way_state[ii] = tag_state[6:0];
               tag_state = tag_state >> 7;
            end
         endfunction : mod_extract_state


         //-----------------------------------------------------------------------------------------
         //
         // Function: mod_extract_tag
         //
         function automatic void mod_extract_tags(input  bit [`SC_TAG_RAM_DATA_SIZE-1:0]       tag_data_in,
                                                  input  bit [`SC_TAG_STATE_RAM_DATA_SIZE-1:0] tag_state_in,
                                                  output bit [22:0]                            tag_way_data[`SC_WAYS],
                                                  output bit [5:0]                             tag_way_data_ecc[`SC_WAYS],
                                                  output bit [6:0]                             tag_way_state[`SC_WAYS]);
            bit [`SC_TAG_RAM_DATA_SIZE-1:0] tag_data;

            tag_data  = tag_data_in;
            for (int ii = 0; ii < `SC_WAYS; ii++) begin
               tag_way_data[ii]     = tag_data[22:0];
               tag_way_data_ecc[ii] = tag_data[28:23];
               tag_data  = tag_data >> 29;
            end
            mod_extract_state(tag_state_in, tag_way_state);
         endfunction : mod_extract_tags


         //-----------------------------------------------------------------------------------------
         //
         // Function: mod_get_cache_tag
         //
         function automatic evl_paddr_t mod_get_cache_tag(input  evl_paddr_t cindex,
                                                          input  int         cway,
                                                          input  int         ctype);
            int                                   set;
            int                                   bank;
            int                                   subbank;
            evl_paddr_t                           paddr;
            evl_paddr_t                           tag_mask;
            bit [`SC_TAG_RAM_DATA_SIZE-1:0]       tag_data;
            bit [`SC_TAG_STATE_RAM_DATA_SIZE-1:0] tag_state;
            bit [22:0]                            tag_way_data[`SC_WAYS];
            bit [5:0]                             tag_way_data_ecc[`SC_WAYS];
            bit [6:0]                             tag_way_state[`SC_WAYS];

            tag_data  = {`SC_TAG_RAM_DATA_SIZE{1'b1}};
            tag_state = {`SC_TAG_STATE_RAM_DATA_SIZE{1'b0}};
            if (ctype inside { int'(SC_RAM_TYPE_SC), int'(SC_RAM_TYPE_SC_DATA) }) begin
               return m_index_config[int'(SC_RAM_TYPE_SC)].get_cache_set_info(evl_way_t'(cway), cindex, set, bank, subbank);
            end
            else if (ctype inside { int'(SC_RAM_TYPE_L2), int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L2_STATE), int'(SC_RAM_TYPE_L2_DATA) }) begin
               paddr    = m_index_config[int'(SC_RAM_TYPE_L2)].get_cache_set_info(evl_way_t'(cway), cindex, set, bank, subbank);
               tag_mask = m_index_config[int'(SC_RAM_TYPE_L2)].get_tag_mask() & (~evl_paddr_t'(`EVL_LINE_BYTES - 1));
            end
            else if (ctype inside { int'(SC_RAM_TYPE_L3), int'(SC_RAM_TYPE_L3_TAG), int'(SC_RAM_TYPE_L3_STATE), int'(SC_RAM_TYPE_L3_DATA) }) begin
               paddr    = m_index_config[int'(SC_RAM_TYPE_L3)].get_cache_set_info(evl_way_t'(cway), cindex, set, bank, subbank);
               tag_mask = m_index_config[int'(SC_RAM_TYPE_L3)].get_tag_mask() & (~evl_paddr_t'(`EVL_LINE_BYTES - 1));
            end
            else begin
               paddr = ~evl_paddr_t'(0);
            end
            if ((paddr == ~evl_paddr_t'(0)) || (cway < 0) || (cway >= `SC_WAYS)) begin
               return ~evl_paddr_t'(0);
            end

            paddr = paddr & (~evl_paddr_t'(`EVL_LINE_BYTES - 1));
            if (bank == 0) begin
               if (subbank == 0) begin
                  tag_data  = shire_cache.banks[0].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[0].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  tag_data  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  tag_data  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  tag_data  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  tag_data  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  tag_data  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  tag_data  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  tag_data  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 1) && (bank == 1)) begin
               if (subbank == 0) begin
                  tag_data  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  tag_data  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  tag_data  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  tag_data  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  tag_data  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  tag_data  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  tag_data  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  tag_data  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 2) && (bank == 2)) begin
               if (subbank == 0) begin
                  tag_data  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  tag_data  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  tag_data  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  tag_data  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  tag_data  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  tag_data  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  tag_data  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  tag_data  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 3) && (bank == 3)) begin
               if (subbank == 0) begin
                  tag_data  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  tag_data  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  tag_data  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  tag_data  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  tag_data  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  tag_data  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  tag_data  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  tag_data  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 4) && (bank == 4)) begin
               if (subbank == 0) begin
                  tag_data  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  tag_data  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  tag_data  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  tag_data  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  tag_data  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  tag_data  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  tag_data  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  tag_data  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 5) && (bank == 5)) begin
               if (subbank == 0) begin
                  tag_data  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  tag_data  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  tag_data  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  tag_data  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  tag_data  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  tag_data  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  tag_data  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  tag_data  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 6) && (bank == 6)) begin
               if (subbank == 0) begin
                  tag_data  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  tag_data  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  tag_data  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  tag_data  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  tag_data  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  tag_data  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  tag_data  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  tag_data  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 7) && (bank == 7)) begin
               if (subbank == 0) begin
                  tag_data  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  tag_data  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  tag_data  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  tag_data  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  tag_data  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  tag_data  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  tag_data  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  tag_data  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.mem[set];
                  tag_state = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[set];
               end
            end
            mod_extract_tags(tag_data, tag_state, tag_way_data, tag_way_data_ecc, tag_way_state);
            if ((tag_way_state[cway] & 7'h40) == 0) begin
               return ~evl_paddr_t'(0);
            end
            paddr = ((~tag_mask & paddr) | (tag_mask & (evl_paddr_t'(tag_way_data[cway]) << 17)));
            if ((tag_way_state[cway] & 7'h10) != 0) begin
               paddr |= evl_paddr_t'(1);
            end
            return paddr;
         endfunction : mod_get_cache_tag


         //-----------------------------------------------------------------------------------------
         //
         // Function: mod_get_cache_data
         //
         function automatic int mod_get_cache_data(output evl_dword_t cdata[],
                                                   input  evl_paddr_t cindex,
                                                   input  int         cway,
                                                   input  int         ctype);
            int                                   tag_set;
            int                                   set;
            int                                   bank;
            int                                   subbank;
            evl_paddr_t                           paddr;
            bit [143:0]                           qword0;
            bit [143:0]                           qword1;
            bit [143:0]                           qword2;
            bit [143:0]                           qword3;
            bit [`SC_TAG_STATE_RAM_DATA_SIZE-1:0] state;
            bit [6:0]                             tag_way_state[`SC_WAYS];

            qword0 = {144{1'b1}};
            qword1 = {144{1'b1}};
            qword2 = {144{1'b1}};
            qword3 = {144{1'b1}};
            cdata  = new[`EVL_LINE_DWORDS];
            if (ctype inside { int'(SC_RAM_TYPE_SC), int'(SC_RAM_TYPE_SC_DATA) }) begin
               paddr = m_index_config[int'(SC_RAM_TYPE_SC)].get_cache_set_info(evl_way_t'(cway), cindex, tag_set, bank, subbank);
               set   = tag_set;
            end
            else if (ctype inside { int'(SC_RAM_TYPE_L2), int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L2_STATE), int'(SC_RAM_TYPE_L2_DATA) }) begin
               paddr = m_index_config[int'(SC_RAM_TYPE_L2)].get_cache_set_info(evl_way_t'(cway), cindex, tag_set, bank, subbank);
               set   = (tag_set * `SC_WAYS) + cway;
            end
            else if (ctype inside { int'(SC_RAM_TYPE_L3), int'(SC_RAM_TYPE_L3_TAG), int'(SC_RAM_TYPE_L3_STATE), int'(SC_RAM_TYPE_L3_DATA) }) begin
               paddr = m_index_config[int'(SC_RAM_TYPE_L3)].get_cache_set_info(evl_way_t'(cway), cindex, tag_set, bank, subbank);
               set   = (tag_set * `SC_WAYS) + cway;
            end
            else begin
               paddr = -1;
            end
            if ((paddr == ~evl_paddr_t'(0)) || (cway < 0) || (cway >= `SC_WAYS)) begin
               for (int ii = 0; ii < `EVL_LINE_DWORDS; ii++) begin
                  cdata[ii] = ~evl_dword_t'(0);
               end
               return -1;
            end

            if (bank == 0) begin
               if (subbank == 0) begin
                  state  = shire_cache.banks[0].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  state  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  state  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  state  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  state  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  state  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  state  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  state  = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 1) && (bank == 1)) begin
               if (subbank == 0) begin
                  state  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  state  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  state  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  state  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  state  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  state  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  state  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  state  = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 2) && (bank == 2)) begin
               if (subbank == 0) begin
                  state  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  state  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  state  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  state  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  state  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  state  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  state  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  state  = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 3) && (bank == 3)) begin
               if (subbank == 0) begin
                  state  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  state  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  state  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  state  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  state  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  state  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  state  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  state  = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 4) && (bank == 4)) begin
               if (subbank == 0) begin
                  state  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  state  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  state  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  state  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  state  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  state  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  state  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  state  = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 5) && (bank == 5)) begin
               if (subbank == 0) begin
                  state  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  state  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  state  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  state  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  state  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  state  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  state  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  state  = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 6) && (bank == 6)) begin
               if (subbank == 0) begin
                  state  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  state  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  state  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  state  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  state  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  state  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  state  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  state  = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.mem[set];
               end
            end
            else if ((`SC_BANKS > 7) && (bank == 7)) begin
               if (subbank == 0) begin
                  state  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                  state  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                  state  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                  state  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                  state  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                  state  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                  state  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.mem[set];
               end
               else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                  state  = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.mem[tag_set];
                  qword0 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.mem[set];
                  qword1 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.mem[set];
                  qword2 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.mem[set];
                  qword3 = shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.mem[set];
               end
            end
            mod_extract_state(state, tag_way_state);
            if ((tag_way_state[cway] & 'h10) != 0) begin
               qword0 = {144{1'b0}};
               qword1 = {144{1'b0}};
               qword2 = {144{1'b0}};
               qword3 = {144{1'b0}};
            end
            cdata[0] = qword0[63:0];
            cdata[1] = qword0[135:72];
            cdata[2] = qword1[63:0];
            cdata[3] = qword1[135:72];
            cdata[4] = qword2[63:0];
            cdata[5] = qword2[135:72];
            cdata[6] = qword3[63:0];
            cdata[7] = qword3[135:72];
            return 1;
         endfunction : mod_get_cache_data


         //-----------------------------------------------------------------------------------------
         //
         // Function: mod_set_cache_bit_err
         //
         function automatic int mod_set_cache_bit_err(input int cindex,
                                                      input int cbit0,
                                                      input int cbit1 = -1,
                                                      input int cway  = 0,
                                                      input int ctype = 0,
                                                      input int cerr  = int'(EVL_ECC_ERR_EPHEMERAL));
            int         set;
            int         bank;
            int         subbank;
            int         err_bit0;
            int         err_bit1;
            int         err_type;
            evl_paddr_t paddr;

            if (ctype inside { int'(SC_RAM_TYPE_SC), int'(SC_RAM_TYPE_SC_DATA) }) begin
               paddr = m_index_config[int'(SC_RAM_TYPE_SC)].get_cache_set_info(evl_way_t'(cway), cindex, set, bank, subbank);
            end
            else if (ctype inside { int'(SC_RAM_TYPE_L2), int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L2_STATE), int'(SC_RAM_TYPE_L2_DATA) }) begin
               paddr = m_index_config[int'(SC_RAM_TYPE_L2)].get_cache_set_info(evl_way_t'(cway), cindex, set, bank, subbank);
            end
            else if (ctype inside { int'(SC_RAM_TYPE_L3), int'(SC_RAM_TYPE_L3_TAG), int'(SC_RAM_TYPE_L3_STATE), int'(SC_RAM_TYPE_L3_DATA) }) begin
               paddr = m_index_config[int'(SC_RAM_TYPE_L3)].get_cache_set_info(evl_way_t'(cway), cindex, set, bank, subbank);
            end
            else begin
               paddr = ~evl_paddr_t'(0);
            end
            if ((paddr == ~evl_paddr_t'(0)) || (cway < 0) || (cway >= `SC_WAYS)) begin
               return -1;
            end

            err_bit0 = cbit0;
            err_bit1 = cbit1;
            err_type = cerr;
            if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
               if ((cbit0 > 28) || (cbit1 > 28)) begin
                  return -1;
               end
            end
            else if (ctype inside { int'(SC_RAM_TYPE_L2_STATE), int'(SC_RAM_TYPE_L3_STATE) }) begin
               // The state info is {ECC[6:0], valid, locked, zero, qwen[3:0], LRU[4:0]}
               if ((cbit0 > 18) || (cbit1 > 18)) begin
                  return -1;
               end
               if (cbit0 < 5) begin
                  err_bit0 = cbit0;
               end
               else if (cbit0 > 11) begin
                  err_bit0 = (3 * 7) + cbit0;
               end
               else if (cbit0 >= 5) begin
                  err_bit0 = (cway * 7) + cbit0;
               end
               if (cbit1 < 5) begin
                  err_bit1 = cbit1;
               end
               else if (cbit1 > 11) begin
                  err_bit1 = (3 * 7) + cbit1;
               end
               else if (cbit1 >= 5) begin
                  err_bit1 = (cway * 7) + cbit1;
               end
            end
            else begin
               if ((cbit0 > 71) || (cbit1 > 71)) begin
                  return -1;
               end
            end
            if (cbit0 < 0) begin
               err_type = int'(EVL_ECC_ERR_NONE);
            end

            if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG), int'(SC_RAM_TYPE_L2_STATE), int'(SC_RAM_TYPE_L3_STATE) }) begin
               if (bank == 0) begin
                  if (subbank == 0) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
               end
               else if ((`SC_BANKS > 1) && (bank == 1)) begin
                  if (subbank == 0) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
               end
               else if ((`SC_BANKS > 2) && (bank == 2)) begin
                  if (subbank == 0) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
               end
               else if ((`SC_BANKS > 3) && (bank == 3)) begin
                  if (subbank == 0) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
               end
               else if ((`SC_BANKS > 4) && (bank == 4)) begin
                  if (subbank == 0) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
               end
               else if ((`SC_BANKS > 5) && (bank == 5)) begin
                  if (subbank == 0) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
               end
               else if ((`SC_BANKS > 6) && (bank == 6)) begin
                  if (subbank == 0) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
               end
               else if ((`SC_BANKS > 7) && (bank == 7)) begin
                  if (subbank == 0) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     if (ctype inside { int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L3_TAG) }) begin
                        case (cway)
                           0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_2].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                           3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.tag_ram.tag_ram.ge.ge[lp_way_3].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        endcase
                        return -1;
                     end
                     else begin
                        return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.tag_state_ram.tag_state_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     end
                  end
               end
            end
            if (ctype inside { int'(SC_RAM_TYPE_SC), int'(SC_RAM_TYPE_SC_DATA), int'(SC_RAM_TYPE_L2), int'(SC_RAM_TYPE_L2_DATA), int'(SC_RAM_TYPE_L3), int'(SC_RAM_TYPE_L3_DATA) }) begin
               if (ctype inside { int'(SC_RAM_TYPE_L2), int'(SC_RAM_TYPE_L2_DATA), int'(SC_RAM_TYPE_L3), int'(SC_RAM_TYPE_L3_DATA) }) begin
                  set = (set * `SC_WAYS) + cway;
               end
               if (bank == 0) begin
                  if (subbank == 0) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[0].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[0].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
               end
               else if ((`SC_BANKS > 1) && (bank == 1)) begin
                  if (subbank == 0) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_1].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
               end
               else if ((`SC_BANKS > 2) && (bank == 2)) begin
                  if (subbank == 0) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_2].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
               end
               else if ((`SC_BANKS > 3) && (bank == 3)) begin
                  if (subbank == 0) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_3].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
               end
               else if ((`SC_BANKS > 4) && (bank == 4)) begin
                  if (subbank == 0) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_4].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
               end
               else if ((`SC_BANKS > 5) && (bank == 5)) begin
                  if (subbank == 0) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_5].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
               end
               else if ((`SC_BANKS > 6) && (bank == 6)) begin
                  if (subbank == 0) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_6].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
               end
               else if ((`SC_BANKS > 7) && (bank == 7)) begin
                  if (subbank == 0) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[0].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 1) && (subbank == 1)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_1].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 2) && (subbank == 2)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_2].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 3) && (subbank == 3)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_3].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 4) && (subbank == 4)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_4].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 5) && (subbank == 5)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_5].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 6) && (subbank == 6)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_6].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
                  else if ((`SC_SUB_BANKS > 7) && (subbank == 7)) begin
                     case (cindex[5:3])
                        0: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        1: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[0].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        2: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        3: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[1].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        4: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        5: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[2].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        6: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[0].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                        7: return shire_cache.banks[lp_bank_7].bank.pipe.mem.sb[lp_subbank_7].mem.data_ram.qw[3].data_ram.ge.ge[1].err_gen.set_err_bits(err_type, set, err_bit0, err_bit1);
                     endcase
                     return -1;
                  end
               end
            end
            return -1;
         endfunction : mod_set_cache_bit_err

      `endif // ifndef ET_USE_BIST_RTL
   `endif // ifndef ET_USE_ASIC_MEMS


   class my_shire_cache_rtl_port extends evl_shire_cache_rtl_port;

      `ifndef ET_USE_ASIC_MEMS
         `ifndef ET_USE_BIST_RTL

            //--------------------------------------------------------------------------------------
            //
            // Function: get_cache_set
            //
            virtual function int get_cache_set(input evl_paddr_t caddr,
                                               input int cway  = 0,
                                               input int ctype = 0);
               int         set;
               int         bank;
               int         subbank;
               evl_paddr_t paddr;

               if (ctype inside { int'(SC_RAM_TYPE_SC), int'(SC_RAM_TYPE_SC_DATA) }) begin
                  paddr = m_index_config[int'(SC_RAM_TYPE_SC)].get_cache_set_info(evl_way_t'(cway), caddr, set, bank, subbank);
               end
               else if (ctype inside { int'(SC_RAM_TYPE_L2), int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L2_STATE), int'(SC_RAM_TYPE_L2_DATA) }) begin
                  paddr = m_index_config[int'(SC_RAM_TYPE_L2)].get_cache_set_info(evl_way_t'(cway), caddr, set, bank, subbank);
               end
               else if (ctype inside { int'(SC_RAM_TYPE_L3), int'(SC_RAM_TYPE_L3_TAG), int'(SC_RAM_TYPE_L3_STATE), int'(SC_RAM_TYPE_L3_DATA) }) begin
                  paddr = m_index_config[int'(SC_RAM_TYPE_L3)].get_cache_set_info(evl_way_t'(cway), caddr, set, bank, subbank);
               end
               else begin
                  return -1;
               end
               if ((paddr == ~evl_paddr_t'(0)) || (cway < 0) || (cway >= `SC_WAYS)) begin
                  return -1;
               end
               if (ctype inside { int'(SC_RAM_TYPE_L2), int'(SC_RAM_TYPE_L2_DATA), int'(SC_RAM_TYPE_L3), int'(SC_RAM_TYPE_L3_DATA) }) begin
                  return ((`SC_WAYS * cway) + set);
               end
               return set;
            endfunction : get_cache_set


            //--------------------------------------------------------------------------------------
            //
            // Function: get_cache_bank
            //
            virtual function int get_cache_bank(input evl_paddr_t caddr,
                                                input int cway  = 0,
                                                input int ctype = 0);
               int         set;
               int         bank;
               int         subbank;
               evl_paddr_t paddr;

               if (ctype inside { int'(SC_RAM_TYPE_SC), int'(SC_RAM_TYPE_SC_DATA) }) begin
                  paddr = m_index_config[int'(SC_RAM_TYPE_SC)].get_cache_set_info(evl_way_t'(cway), caddr, set, bank, subbank);
               end
               else if (ctype inside { int'(SC_RAM_TYPE_L2), int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L2_STATE), int'(SC_RAM_TYPE_L2_DATA) }) begin
                  paddr = m_index_config[int'(SC_RAM_TYPE_L2)].get_cache_set_info(evl_way_t'(cway), caddr, set, bank, subbank);
               end
               else if (ctype inside { int'(SC_RAM_TYPE_L3), int'(SC_RAM_TYPE_L3_TAG), int'(SC_RAM_TYPE_L3_STATE), int'(SC_RAM_TYPE_L3_DATA) }) begin
                  paddr = m_index_config[int'(SC_RAM_TYPE_L3)].get_cache_set_info(evl_way_t'(cway), caddr, set, bank, subbank);
               end
               else begin
                  return -1;
               end
               if ((paddr == ~evl_paddr_t'(0)) || (cway < 0) || (cway >= `SC_WAYS)) begin
                  return -1;
               end
               return bank;
            endfunction : get_cache_bank


            //--------------------------------------------------------------------------------------
            //
            // Function: get_cache_subbank
            //
            virtual function int get_cache_subbank(input evl_paddr_t caddr,
                                                   input int cway  = 0,
                                                   input int ctype = 0);
               int         set;
               int         bank;
               int         subbank;
               evl_paddr_t paddr;

               if (ctype inside { int'(SC_RAM_TYPE_SC), int'(SC_RAM_TYPE_SC_DATA) }) begin
                  paddr = m_index_config[int'(SC_RAM_TYPE_SC)].get_cache_set_info(evl_way_t'(cway), caddr, set, bank, subbank);
               end
               else if (ctype inside { int'(SC_RAM_TYPE_L2), int'(SC_RAM_TYPE_L2_TAG), int'(SC_RAM_TYPE_L2_STATE), int'(SC_RAM_TYPE_L2_DATA) }) begin
                  paddr = m_index_config[int'(SC_RAM_TYPE_L2)].get_cache_set_info(evl_way_t'(cway), caddr, set, bank, subbank);
               end
               else if (ctype inside { int'(SC_RAM_TYPE_L3), int'(SC_RAM_TYPE_L3_TAG), int'(SC_RAM_TYPE_L3_STATE), int'(SC_RAM_TYPE_L3_DATA) }) begin
                  paddr = m_index_config[int'(SC_RAM_TYPE_L3)].get_cache_set_info(evl_way_t'(cway), caddr, set, bank, subbank);
               end
               else begin
                  return -1;
               end
               if ((paddr == ~evl_paddr_t'(0)) || (cway < 0) || (cway >= `SC_WAYS)) begin
                  return -1;
               end
               return subbank;
            endfunction : get_cache_subbank


            //--------------------------------------------------------------------------------------
            //
            // Function: get_cache_tag
            //
            virtual function evl_paddr_t get_cache_tag(input int cindex,
                                                       input int cway  = 0,
                                                       input int ctype = 0);
               return mod_get_cache_tag(cindex, cway, ctype);
            endfunction : get_cache_tag


            //--------------------------------------------------------------------------------------
            //
            // Function: get_cache_data
            //
            virtual function int get_cache_data(output evl_dword_t cdata[],
                                                input  int         cindex,
                                                input  int         cway  = 0,
                                                input  int         ctype = 0);
               return mod_get_cache_data(cdata, cindex, cway, ctype);
            endfunction : get_cache_data


            //--------------------------------------------------------------------------------------
            //
            // Function: set_cache_bit_err
            //
            virtual function int set_cache_bit_err(input int cindex,
                                                   input int cbit0,
                                                   input int cbit1 = -1,
                                                   input int cway  = 0,
                                                   input int ctype = 0,
                                                   input int cerr  = int'(EVL_ECC_ERR_EPHEMERAL));
               if (mod_set_cache_bit_err(cindex, cbit0, cbit1, cway, ctype, cerr) <= 0) begin
                  return -1;
               end
               return 1;
            endfunction : set_cache_bit_err

         `endif // ifndef ET_USE_BIST_RTL
      `endif // ifndef ET_USE_ASIC_MEMS

      //--------------------------------------------------------------------------------------------
      //
      //
      //
      function new(input string full_name_in = "my_shire_cache_rtl_port", input int skip_levels_in[] = {});
         super.new(full_name_in, skip_levels_in);
      endfunction

   endclass : my_shire_cache_rtl_port


   // Shire_Cache SVA assertions
   `include "dv/shire_common/shire_cache/assert/evl_shire_cache_assert.svh"


endmodule
