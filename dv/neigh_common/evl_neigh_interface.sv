`include "rtl/inc/soc.vh"
`include "dv/common/apb/apb_vc/evl_apb_pkg.sv"

`ifndef __EVL_NEIGH_PKG
   `include "dv/neigh_common/evl_neigh_vc_pkg.sv"
`endif

module evl_neigh_interface #(parameter int              NEIGH_ID        = 0,
                             parameter [`MIN_PER_N-1:0] DISABLE_MINIONS = {`MIN_PER_N{1'b0}},
                             parameter [`MIN_PER_N-1:0] STUB_MINIONS    = {`MIN_PER_N{1'b0}},
                             parameter bit              GFX_ENABLED     = 1'b0)
                              (input wire                  reset_neigh,
                               input wire                  isolate,
                               input wire                  clock,
                               input wire APB_to_neigh_t   APB_ESR_req,
                               input wire APB_from_neigh_t APB_ESR_rsp
                               );
   import evl_base_pkg::*;
   import evl_neigh_vc_pkg::*;

   `ifdef ENABLE_BEMU_OR_ZSIM
      import "DPI-C" function void esr_write(input longint unsigned paddr, input longint unsigned data);
   `endif // ENABLE_BEMU_OR_ZSIM
   typedef my_neigh_rtl_port;

   reg  start_monitor        = 1'b0;
   reg  mon_power_on         = 1'b0;
   reg  mon_power_off        = 1'b0;
   reg  mon_reset_on         = 1'b0;
   reg  mon_reset_off        = 1'b0;

   // Fast Local Barrier request interface between neigh and L2
   wire                                             flb_mon_req_valid;
   wire [`CSR_FL_BARRIER_SIZE-1:0]                  flb_mon_req_data;

   // Fast Local Barrier response interface between neigh and L2
   wire                                             flb_mon_resp_valid;
   wire [`CSR_FL_BARRIER_SIZE-1:0]                  flb_mon_resp_data;

   // Fast Local Barrier request interface between cores and neigh
   wire [`MIN_PER_N-1:0]                            flb_core_neigh_req_valid;
   wire [`CSR_FL_BARRIER_SIZE-1:0] [`MIN_PER_N-1:0] flb_core_neigh_req_data;

   // Fast Local Barrier response interface between cores and neigh
   wire [`MIN_PER_N-1:0]                            flb_neigh_core_resp_valid;
   wire  [`MIN_PER_N-1:0][11:0]                     flb_neigh_core_resp_data;

   wire                                             mon_apb_penable;
   wire [`MIN_PER_N-1:0]                            mon_apb_psel;
   wire                                             mon_apb_pwrite;
   wire [`MIN_DM_APB_ADDR_WIDTH-1:0]                mon_apb_paddr;
   wire [`ESR_APB_D_WIDTH-1:0]                      mon_apb_pwdata;
   wire [`MIN_PER_N-1:0]                            mon_apb_pready;
   wire [`MIN_PER_N-1:0][`ESR_APB_D_WIDTH - 1:0]    mon_apb_prdata;
   wire [`MIN_PER_N-1:0]                            mon_apb_pslverr;

   tri0 [`EVL_TRANS_ID_MSB:0]                       unused_miss_req_trans_id;
   tri0 [`EVL_TRANS_ID_MSB:0]                       unused_miss_tenb_req_trans_id;
   tri0 [`EVL_TRANS_ID_MSB:0]                       unused_miss_tenscp_req_trans_id;
   tri0 [`EVL_TRANS_ID_MSB:0]                       unused_evict_req_trans_id;

   reg  [`CORE_NR_THREADS-1:0][8:7][4:0]                    mon_neigh_mhpmevent;
   reg  [`CORE_NR_THREADS-1:0][8:7][4:0]                    mon_neigh_mhpmevent_next;
   reg  [`MIN_PER_N-1:0][`CORE_NR_THREADS-1:0][8:7][4:0]    neigh_mhpmevent;

   wire [`MIN_PER_N-1:0][2:0]     l2_dcache_miss_req_valid_ready;
   wire [`MIN_PER_N-1:0][2:0]     l2_dcache_resp_valid_ready;

   reg  [`CORE_NR_THREADS-1:0][8:7]      pmu_neigh_event_seen_st1;
   reg  [`CORE_NR_THREADS-1:0][8:7]      pmu_neigh_event_seen_st2;
   reg  [`CORE_NR_THREADS-1:0][8:7]      pmu_neigh_event_seen_st3;
   reg  [`CORE_NR_THREADS-1:0][8:7]      pmu_neigh_event_seen_st4;

   reg  [`CORE_NR_THREADS-1:0][8:7][4:0] pmu_neigh_event_type_st1;
   reg  [`CORE_NR_THREADS-1:0][8:7][4:0] pmu_neigh_event_type_st2;
   reg  [`CORE_NR_THREADS-1:0][8:7][4:0] pmu_neigh_event_type_st3;
   reg  [`CORE_NR_THREADS-1:0][8:7][4:0] pmu_neigh_event_type_st4;

   // Neigh coverage
   neigh_cvg            neigh_cov;
   neigh_cvg_per_minion neigh_cov_per_minion[8];



`ifndef L1SB_EVL_MINION_ENV

   //
   // Create the Minion interfaces
   //
   genvar gen_it;

   generate for(gen_it = 0; gen_it < `MIN_PER_N; gen_it++)
   begin : gen_minion_itf
      localparam minPerPtwIdx = gen_it % `MIN_PER_PTW;
      localparam ptwIdx       = gen_it / `MIN_PER_PTW;

      //
      // Miss requests interface
      //
      evl_etlink_interface #(
                             .AGENT_ID            ( gen_it                         ),
                             .USE_BANKS           ( 1                              ),
                             .NUM_BANKS           ( 3                              ),
                             .NUM_UC              ( 0                              ),
                             .ETLINK_COV_TYPE     ( int'(EVL_COV_ETLINK_MINION)    ),
                             .REQ_TYPE            ( et_link_minion_miss_req_info_t ),
                             .RSP_TYPE            ( et_link_minion_rsp_info_t      ),
                             .IGNORE_SRC_DST      ( 1                              ),
                             .IGNORE_MESSAGE      ( 1                              ),
                             .IGNORE_DST_RSP      ( 1                              ),
                             .IGNORED_RSP_IDS     ({ET_LINK_Core_Miss_Id_Reduce,
                                                    ET_LINK_Core_Miss_Id_Cache_Ops,
                                                    ET_LINK_Core_Miss_Id_WriteBack,
                                                    ET_LINK_Core_Miss_Id_Tensor_Store_Coop,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_0,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_UC_ST_1,
                                                    ET_LINK_Core_Miss_Id_L2_Prefetch_0,
                                                    ET_LINK_Core_Miss_Id_L2_Prefetch_1,
                                                    ET_LINK_Core_Miss_Id_L2_Prefetch_2,
                                                    ET_LINK_Core_Miss_Id_L2_Prefetch_3
                                                   })
                            )
      m_miss_req (
         .isolate              ( isolate                                    ),
         .reset                ( reset_neigh                                ),
         .clk                  ( clock                                      ),
         .req_valid            ( neigh_top.l2_dcache_miss_req_valid[gen_it] ),
         .req_ready            ( neigh_top.l2_dcache_miss_req_ready[gen_it] ),
         .req_info             ( neigh_top.l2_dcache_miss_req[gen_it]       ),
         .req_trans_id         ( unused_miss_req_trans_id                   ),
         .rsp_valid            ( neigh_top.l2_dcache_resp_valid[gen_it]     ),
         .rsp_ready            ( neigh_top.l2_dcache_resp_ready[gen_it]     ),
         .rsp_info             ( neigh_top.l2_dcache_resp[gen_it]           ),
         .use_rsp_source_is_uc ( 1'b0                                       ),
         .rsp_source_is_uc     ( 1'b0                                       )
         );

      //
      // Evict requests (and anything with data) interface
      //
      evl_etlink_interface #(
                             .AGENT_ID            ( gen_it                          ),
                             .USE_BANKS           ( 1                               ),
                             .NUM_BANKS           ( 2                               ),
                             .NUM_UC              ( 0                               ),
                             .ETLINK_COV_TYPE     ( int'(EVL_COV_ETLINK_MINION)     ),
                             .REQ_TYPE            ( et_link_minion_evict_req_info_t ),
                             .RSP_TYPE            ( et_link_minion_rsp_info_t       ),
                             .IGNORE_SRC_DST      ( 1                               ),
                             .IGNORE_MESSAGE      ( 0                               ),
                             .IGNORE_DST_RSP      ( 1                               ),
                             .IGNORED_RSP_IDS     ({ET_LINK_Core_Miss_Id_Tbox,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_0,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_1,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_0,
                                                    ET_LINK_Core_Miss_Id_Miss_Handler_UC_LD_1,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_0,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_1,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_2,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_3,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_4,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_5,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_6,
                                                    ET_LINK_Core_Miss_Id_Tensor_Load_Ext_7
                                                   })
                            )
      m_evict_req (
         .isolate              ( isolate                                     ),
         .reset                ( reset_neigh                                 ),
         .clk                  ( clock                                       ),
         .req_valid            ( neigh_top.l2_dcache_evict_req_valid[gen_it] ),
         .req_ready            ( neigh_top.l2_dcache_evict_req_ready[gen_it] ),
         .req_info             ( neigh_top.l2_dcache_evict_req[gen_it]       ),
         .req_trans_id         ( unused_evict_req_trans_id                   ),
         .rsp_valid            ( neigh_top.l2_dcache_resp_valid[gen_it]      ),
         .rsp_ready            ( neigh_top.l2_dcache_resp_ready[gen_it]      ),
         .rsp_info             ( neigh_top.l2_dcache_resp[gen_it]            ),
         .use_rsp_source_is_uc ( 1'b0                                        ),
         .rsp_source_is_uc     ( 1'b0                                        )
         );

      evl_ptw_interface #(
                          .AGENT_ID ( gen_it )
                         )
      m_ptw_req (
         .isolate   ( isolate                                           ),
         .reset     ( reset_neigh                                       ),
         .clock     ( clock                                             ),
         .req_valid ( neigh_top.dc_ptw_req_valid[ptwIdx][minPerPtwIdx]  ),
         .req_ready ( neigh_top.dc_ptw_req_ready[ptwIdx][minPerPtwIdx]  ),
         .req_info  ( neigh_top.dc_ptw_req_data[ptwIdx][minPerPtwIdx]   ),
         .rsp_valid ( neigh_top.ptw_dc_resp_valid[ptwIdx][minPerPtwIdx] ),
         .rsp_info  ( neigh_top.ptw_dc_resp_data[ptwIdx]                )
      );

      //
      // Debug APB interface
      //
      assign mon_apb_penable          = neigh_top.minion_apb_penable;
      assign mon_apb_psel    [gen_it] = neigh_top.minion_apb_psel[gen_it];
      assign mon_apb_pwrite           = neigh_top.minion_apb_pwrite;
      assign mon_apb_paddr            = neigh_top.minion_apb_paddr;
      assign mon_apb_pwdata           = neigh_top.minion_apb_pwdata;
      assign mon_apb_pready  [gen_it] = neigh_top.minion_apb_pready[gen_it];
      assign mon_apb_prdata  [gen_it] = neigh_top.minion_apb_prdata[gen_it];
      assign mon_apb_pslverr [gen_it] = neigh_top.minion_apb_pslverr[gen_it];

      evl_apb_interface #(.AGENT_ID    ( gen_it                 ),
                          .ADDR_WIDTH  ( `MIN_DM_APB_ADDR_WIDTH ),
                          .DATA_WIDTH  ( `ESR_APB_D_WIDTH       )
                         )
      m_debug_itf (
         .isolate ( isolate                  ),
         .pclk    ( clock                    ),
         .presetn ( ~reset_neigh             ),
         .penable ( mon_apb_penable          ),
         .psel    ( mon_apb_psel    [gen_it] ),
         .pwrite  ( mon_apb_pwrite           ),
         .paddr   ( mon_apb_paddr            ),
         .pwdata  ( mon_apb_pwdata           ),
         .pready  ( mon_apb_pready  [gen_it] ),
         .prdata  ( mon_apb_prdata  [gen_it] ),
         .pslverr ( mon_apb_pslverr [gen_it] )
      );

      if (DISABLE_MINIONS[gen_it] == 1'b1) begin
         assign neigh_top.l2_dcache_miss_req_valid[gen_it]       = {`DCACHE_L2_MISS_REQ_PORTS{1'b0}};
         assign neigh_top.l2_dcache_resp_ready[gen_it]           = 1'b0;
         assign neigh_top.l2_dcache_evict_req_valid[gen_it]      = 1'b0;
         assign neigh_top.l2_dcache_resp_ready[gen_it]           = 1'b0;
         assign neigh_top.dc_ptw_req_valid[ptwIdx][minPerPtwIdx] = 1'b0;
      end

      if ((STUB_MINIONS[gen_it] == 1'b1) || (DISABLE_MINIONS[gen_it] == 1'b1)) begin
         assign neigh_top.icache_req_valid[gen_it]               = 1'b0;
         assign neigh_top.icache_flush_data[gen_it]              = 1'b0;
         assign neigh_top.tlb_invalidate[gen_it]                 = 1'b0;
         assign neigh_top.flb_neigh_req_valid[gen_it]            = 1'b0;
         assign neigh_top.minion_apb_pready[gen_it]              = 1'b0;
         assign neigh_top.minion_apb_pslverr[gen_it]             = 1'b0;
      end

      //
      // Neigh PMU interface
      //
      assign neigh_mhpmevent[gen_it][1][8]     = neigh_top.pmu_neigh_event_sel[gen_it][19:15];
      assign neigh_mhpmevent[gen_it][1][7]     = neigh_top.pmu_neigh_event_sel[gen_it][9:5];
      assign neigh_mhpmevent[gen_it][0][8]     = neigh_top.pmu_neigh_event_sel[gen_it][14:10];
      assign neigh_mhpmevent[gen_it][0][7]     = neigh_top.pmu_neigh_event_sel[gen_it][4:0];

   end // block: gen_minion_itf

   endgenerate

   //
   // ESR APB interface
   //
   evl_apb_interface #(.AGENT_ID    ( NEIGH_ID             ),
                       .ADDR_WIDTH  ( `ESR_APB_AD_PP_WIDTH ),
                       .DATA_WIDTH  ( `ESR_APB_D_WIDTH     )
                      ) m_apb_esr (
         .isolate ( isolate                 ),
         .pclk    ( clock                   ),
         .presetn ( ~reset_neigh            ),
         .penable ( APB_ESR_req.apb_penable ),
         .psel    ( APB_ESR_req.apb_psel    ),
         .pwrite  ( APB_ESR_req.apb_pwrite  ),
         .paddr   ( APB_ESR_req.apb_paddr   ),
         .pwdata  ( APB_ESR_req.apb_pwdata  ),

         .pready  ( APB_ESR_rsp.apb_pready  ),
         .prdata  ( APB_ESR_rsp.apb_prdata  ),
         .pslverr ( APB_ESR_rsp.apb_pslverr )
      );

`endif // ifndef L1SB_EVL_MINION_ENV



   //
   // Fast Local Barrier interface
   //
   assign flb_mon_req_valid   = neigh_top.flb_neigh_l2_req_valid;
   assign flb_mon_req_data    = neigh_top.flb_neigh_l2_req_data;
   assign flb_mon_resp_valid  = neigh_top.flb_l2_neigh_resp_valid;
   assign flb_mon_resp_data   = neigh_top.flb_l2_neigh_resp_data;

   assign flb_core_neigh_req_valid  = neigh_top.flb_neigh_req_valid;
   assign flb_core_neigh_req_data   = neigh_top.flb_neigh_req_data;
   assign flb_neigh_core_resp_valid = neigh_top.flb_neigh_resp_valid;
   assign flb_neigh_core_resp_data  = neigh_top.flb_neigh_resp_data;


   //
   // Neigh PMU interface
   //
   assign mon_neigh_mhpmevent_next[0][8] = (|neigh_mhpmevent[0][0][8]) ? neigh_mhpmevent[0][0][8] :
                                           (|neigh_mhpmevent[1][0][8]) ? neigh_mhpmevent[1][0][8] :
                                           (|neigh_mhpmevent[2][0][8]) ? neigh_mhpmevent[2][0][8] :
                                           (|neigh_mhpmevent[3][0][8]) ? neigh_mhpmevent[3][0][8] :
                                           (|neigh_mhpmevent[4][0][8]) ? neigh_mhpmevent[4][0][8] :
                                           (|neigh_mhpmevent[5][0][8]) ? neigh_mhpmevent[5][0][8] :
                                           (|neigh_mhpmevent[6][0][8]) ? neigh_mhpmevent[6][0][8] :
                                           (|neigh_mhpmevent[7][0][8]) ? neigh_mhpmevent[7][0][8] :
                                           5'b0;

   assign mon_neigh_mhpmevent_next[1][8] = (|neigh_mhpmevent[0][1][8]) ? neigh_mhpmevent[0][1][8] :
                                           (|neigh_mhpmevent[1][1][8]) ? neigh_mhpmevent[1][1][8] :
                                           (|neigh_mhpmevent[2][1][8]) ? neigh_mhpmevent[2][1][8] :
                                           (|neigh_mhpmevent[3][1][8]) ? neigh_mhpmevent[3][1][8] :
                                           (|neigh_mhpmevent[4][1][8]) ? neigh_mhpmevent[4][1][8] :
                                           (|neigh_mhpmevent[5][1][8]) ? neigh_mhpmevent[5][1][8] :
                                           (|neigh_mhpmevent[6][1][8]) ? neigh_mhpmevent[6][1][8] :
                                           (|neigh_mhpmevent[7][1][8]) ? neigh_mhpmevent[7][1][8] :
                                           5'b0;

   assign mon_neigh_mhpmevent_next[0][7] = (|neigh_mhpmevent[0][0][7]) ? neigh_mhpmevent[0][0][7] :
                                           (|neigh_mhpmevent[1][0][7]) ? neigh_mhpmevent[1][0][7] :
                                           (|neigh_mhpmevent[2][0][7]) ? neigh_mhpmevent[2][0][7] :
                                           (|neigh_mhpmevent[3][0][7]) ? neigh_mhpmevent[3][0][7] :
                                           (|neigh_mhpmevent[4][0][7]) ? neigh_mhpmevent[4][0][7] :
                                           (|neigh_mhpmevent[5][0][7]) ? neigh_mhpmevent[5][0][7] :
                                           (|neigh_mhpmevent[6][0][7]) ? neigh_mhpmevent[6][0][7] :
                                           (|neigh_mhpmevent[7][0][7]) ? neigh_mhpmevent[7][0][7] :
                                           5'b0;

   assign mon_neigh_mhpmevent_next[1][7] = (|neigh_mhpmevent[0][1][7]) ? neigh_mhpmevent[0][1][7] :
                                           (|neigh_mhpmevent[1][1][7]) ? neigh_mhpmevent[1][1][7] :
                                           (|neigh_mhpmevent[2][1][7]) ? neigh_mhpmevent[2][1][7] :
                                           (|neigh_mhpmevent[3][1][7]) ? neigh_mhpmevent[3][1][7] :
                                           (|neigh_mhpmevent[4][1][7]) ? neigh_mhpmevent[4][1][7] :
                                           (|neigh_mhpmevent[5][1][7]) ? neigh_mhpmevent[5][1][7] :
                                           (|neigh_mhpmevent[6][1][7]) ? neigh_mhpmevent[6][1][7] :
                                           (|neigh_mhpmevent[7][1][7]) ? neigh_mhpmevent[7][1][7] :
                                           5'b0;


   assign l2_dcache_miss_req_valid_ready  = neigh_top.l2_dcache_miss_req_valid & neigh_top.l2_dcache_miss_req_ready;
   assign l2_dcache_resp_valid_ready      = neigh_top.l2_dcache_resp_valid & neigh_top.l2_dcache_resp_ready;

   //
   // EVL Proxy Code
   //
   bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), NEIGH_RTL_AGENT, NEIGH_ID, "evl_neigh_env", "", "", "", { 1 });
   my_neigh_rtl_port m_rtl_port = new($sformatf("%m"), NEIGH_ID, { 1 });

   //
   // This small block of code is used to prevent any evaluation at time 0.  Because an x->1
   // transition is considered a positive edge in Verilog, this block forces us to see a ->0
   // transition first (this only applies if the clock is asserted at time 0).
   //
   initial begin
      m_rtl_port.set_int_param_by_name("pk_minion_disable", DISABLE_MINIONS);
      m_rtl_port.set_int_param_by_name("pk_minion_stub", STUB_MINIONS);
      @(negedge clock);
      start_monitor <= 1'b1;
   end

   evl_trans_id_t trans_id[1+`MIN_PER_N-1:0];

   logic [`PA_SIZE-1:0] start_pc;
   initial begin
      if (`et_value_plusargs("START_PC=0x%x", start_pc)) begin
         force neigh_top.channel.esrs.reg_minion_boot = start_pc;
      end
   end

   logic        shutdown_started;
   logic [1:0]  agent_forced;
   logic        force_all_agents;
   logic [1:0]  dest_fifo;
   logic        force_dest_fifo;
   logic        bypass_dcache;
   logic        bypass_icache;
   logic [63:0] reg_data;
   logic [63:0] addr;
   initial begin
      if (!`et_value_plusargs("agent_forced=%d",     agent_forced     )) agent_forced     = 2'h0;
      if (!`et_value_plusargs("force_all_agents=%d", force_all_agents )) force_all_agents = 1'b0;
      if (!`et_value_plusargs("dest_fifo=%d",        dest_fifo        )) dest_fifo        = 2'h0;
      if (!`et_value_plusargs("force_dest_fifo=%d",  force_dest_fifo  )) force_dest_fifo  = 1'b0;
      if (!`et_value_plusargs("bypass_dcache=%d",    bypass_dcache    )) bypass_dcache    = 1'b0;
      if (!`et_value_plusargs("bypass_icache=%d",    bypass_icache    )) bypass_icache    = 1'b0;
      reg_data = {agent_forced, force_all_agents, dest_fifo, force_dest_fifo, bypass_dcache, bypass_icache};
      `ifndef UPF 
      force neigh_top.channel.esrs.reg_neigh_chicken = reg_data;
      `endif
      @(negedge reset_neigh);
      `ifdef ENABLE_BEMU_OR_ZSIM
         addr = (1'b1 << 31) | (neigh_top.shire_id << 24) | (4'hD << 20) | (NEIGH_ID << 16) | (13'hE << 3);
         esr_write(addr, reg_data); // reflect the change in BEMU
      `endif
   end

   always @(posedge clock) begin
      if (m_rtl_port.is_shutdown() === 1'b1) begin
         reg_data = 8'b0;
         `ifndef UPF
         force neigh_top.channel.esrs.reg_neigh_chicken = reg_data;
         `endif
         `ifdef ENABLE_BEMU_OR_ZSIM
            addr = (1'b1 << 31) | (neigh_top.shire_id << 24) | (4'hD << 20) | (NEIGH_ID << 16) | (13'hE << 3);
            esr_write(addr, reg_data); // reflect the change in BEMU
         `endif
      end
   end

   //
   // Monitor the signals here and send appropriate info to the RTL port proxy.
   //
   initial begin
      #1ns;
      forever begin
         #0 wait ((isolate === 1'b1) && (mon_power_off === 1'b0));
         m_rtl_port.power_off();
         mon_power_off = 1'b1;
         mon_power_on  = 1'b0;
         #0 wait (isolate !== 1'b1);
      end
   end

   initial begin
      #1ns;
      forever begin
         #0 wait ((isolate === 1'b0) && (mon_power_on === 1'b0));
         m_rtl_port.power_on();
         mon_power_on  = 1'b1;
         mon_power_off = 1'b0;
         #0 wait (isolate !== 1'b0);
      end
   end

   always @(posedge clock) begin
      if (start_monitor === 1'b1) begin
         if ((reset_neigh === 1'b1) && (mon_reset_on === 1'b0)) begin
            m_rtl_port.assert_reset();
            for (int i=0; i<1+`MIN_PER_N; i++) begin
               trans_id[i] = 0;
            end
            mon_neigh_mhpmevent <= '{{5'b0, 5'b0}, {5'b0, 5'b0}};
         end
         else if ((reset_neigh === 1'b0) && (mon_reset_off === 1'b0)) begin
            m_rtl_port.deassert_reset();
         end
         mon_reset_on  <= reset_neigh;
         mon_reset_off <= ~reset_neigh;
         if ((reset_neigh === 1'b0) && (isolate !== 1'b1)) begin

             // FLB request
            if (flb_mon_req_valid === 1'b1) begin
               evl_neigh_packet_u pkt;
               pkt.flb.info.data =  flb_mon_req_data;
               pkt.flb.info.flb_type =  L2_FLB_REQ;
               m_rtl_port.send_event(L2_FLB_REQ, NEIGH_ID, trans_id[`MIN_PER_N]++, pkt);
            end

            for (int i=0; i<`MIN_PER_N; i++) begin
               // FLB request
               if (flb_core_neigh_req_valid[i] === 1'b1) begin
                  evl_neigh_packet_u pkt;
                  pkt.flb.info.data =  flb_core_neigh_req_data[i];
                  pkt.flb.info.flb_type =  CORE_FLB_REQ;
                  m_rtl_port.send_event(CORE_FLB_REQ, i, trans_id[i]++, pkt);
               end
            end

            // FLB response
            if (flb_mon_resp_valid === 1'b1) begin
               evl_neigh_packet_u pkt;
               pkt.flb.info.data =  flb_mon_resp_data;
               pkt.flb.info.flb_type =  L2_FLB_RSP;
               m_rtl_port.send_event(L2_FLB_RSP, NEIGH_ID, trans_id[`MIN_PER_N]++, pkt);
            end

            for (int i=0; i<`MIN_PER_N; i++) begin
               // FLB request
               if (flb_neigh_core_resp_valid[i] === 1'b1) begin
                  evl_neigh_packet_u pkt;
                  pkt.flb.info.data =  flb_neigh_core_resp_data[i];
                  pkt.flb.info.flb_type =  CORE_FLB_RSP;
                  m_rtl_port.send_event(CORE_FLB_RSP, i, trans_id[i]++, pkt);
               end
            end


            //
            // Monitor for writes to PMU Event CSRs
            //
            mon_neigh_mhpmevent <= mon_neigh_mhpmevent_next;

            for (int ii = 0; ii < `CORE_NR_THREADS; ii = ii + 1) begin
               for (int jj = 7; jj < 9; jj = jj + 1) begin
                  if (mon_neigh_mhpmevent[ii][jj] != mon_neigh_mhpmevent_next[ii][jj]) begin
                     evl_neigh_packet_u pkt;
                     pkt.pmu_event.info.pmu_thread_id  = ii;
                     pkt.pmu_event.info.pmu_event_type = mon_neigh_mhpmevent_next[ii][jj];
                     pkt.pmu_event.info.pmu_cntr_id    = jj;
                     m_rtl_port.send_pmu_event(PMU_TXN_EVENT_WR, pkt);
                  end
               end
            end


            pmu_neigh_event_seen_st4 <= pmu_neigh_event_seen_st3;
            pmu_neigh_event_seen_st3 <= pmu_neigh_event_seen_st2;
            pmu_neigh_event_seen_st2 <= pmu_neigh_event_seen_st1;

            pmu_neigh_event_type_st4 <= pmu_neigh_event_type_st3;
            pmu_neigh_event_type_st3 <= pmu_neigh_event_type_st2;
            pmu_neigh_event_type_st2 <= pmu_neigh_event_type_st1;
            pmu_neigh_event_type_st1 <= mon_neigh_mhpmevent;

            //
            // Monitor PMU Events
            //
            for (int ii = 0; ii < `CORE_NR_THREADS; ii = ii + 1) begin
               for (int jj = 7; jj < 9; jj = jj + 1) begin
                  case (mon_neigh_mhpmevent[ii][jj])
                     PMU_NEIGH_EVENT_MIN_ET_REQ     : begin    //= 1,  // Minion sends ET Link REQ
                                                         if (|(neigh_top.channel.min_l2_req_bid & neigh_top.channel.min_l2_req_ready)) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_MIN_ET_RSP     : begin    //= 2,  // Minion receives ET Link RSP
                                                         if (|(neigh_top.channel.fill_fifo_min_valid & neigh_top.channel.fill_fifo_min_ready)) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_COOP_LD_REQ    : begin    //= 3,  // Coop Load REQ sent
                                                         if (neigh_top.channel.coop_tload_miss_req_valid & neigh_top.channel.coop_tload_miss_req_ready) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_IN_COOP_LD_REQ : begin    //= 4,  // Inter-Neigh Coop Load REQ sent
                                                         if (neigh_top.channel.coop_tload_miss_req_valid & neigh_top.channel.coop_tload_miss_req_ready &
                                                            (neigh_top.channel.coop_tload_miss_req_info.opcode == ET_LINK_REQ_ReadCoop)) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_COOP_LD_RSP    : begin    //= 5,  // Coop Load RSP received
                                                         if (neigh_top.channel.fill_fifo_push & (neigh_top.channel.fill_fifo_in_rsp_info.opcode == ET_LINK_RSP_AckData) &
                                                            (et_link_neigh_source_t'(neigh_top.channel.fill_fifo_in_rsp_info.dest) == ET_LINK_NEIGH_SRC_Coop)) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_COOP_ST_REQ    : begin    //= 6,  // Coop Store REQ sent
                                                         if (|(neigh_top.channel.tstore_evict_valid & neigh_top.channel.tstore_miss_req_ready)) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_COOP_ST_RSP    : begin    //= 7,  // Coop Store RSP received
                                                         if (neigh_top.channel.fill_fifo_push & (neigh_top.channel.fill_fifo_in_rsp_info.opcode == ET_LINK_RSP_Ack) &
                                                            (et_link_core_miss_id_t'(neigh_top.channel.fill_fifo_in_rsp_info.id) == ET_LINK_Core_Miss_Id_Tensor_Store_Coop)) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_MIN_ICACHE_REQ : begin    //= 8,  // Minion sends REQ to ICache
                                                         if (|(neigh_top.channel.icache_req_valid & neigh_top.channel.icache_req_ready)) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_MIN_ICACHE_RSP : begin    //= 9,  // Minion receives RSP from ICache
                                                         if (|neigh_top.channel.icache_resp_valid) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_MIN_PTW_REQ    : begin    //= 10, // Minion sends REQ to PTW
                                                      end
                     PMU_NEIGH_EVENT_MIN_PTW_RSP    : begin    //= 11, // Minion receives RSP from PTW
                                                      end
                     PMU_NEIGH_EVENT_FLN_MSG        : begin    //= 12, // Message sent between Minions through FLN
                                                         if (|(neigh_top.channel.local_message_valid_in & neigh_top.channel.local_message_ready_in)) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_ICACHE_ET_REQ  : begin    //= 13, // ICache sends ET Link REQ
                                                         if (neigh_top.channel.icache_l2_miss_req_valid & neigh_top.channel.icache_l2_miss_req_ready) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_ICACHE_ET_RSP  : begin    //= 14, // ICache receives ET Link RSP
                                                         if (neigh_top.channel.icache_l2_miss_resp_valid & neigh_top.channel.icache_l2_miss_resp_ready) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_ICACHE_L1_REQ  : begin    //= 15, // ICache sends REQ to L1 SRAM
                                                         if (neigh_top.channel.icache_f2_sram_req_valid_lo & neigh_top.channel.icache_f2_sram_req_ready_lo) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_ICACHE_L1_RSP  : begin    //= 16, // ICache receives RSP from L1 SRAM
                                                         if (neigh_top.channel.icache_f0_sram_resp_valid_lo & neigh_top.channel.icache_f0_sram_resp_ready_lo) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_PTW_ET_REQ     : begin    //= 17, // PTW sends ET Link REQ
                                                      end
                     PMU_NEIGH_EVENT_PTW_ET_RSP     : begin    //= 18, // PTW receives ET Link RSP
                                                      end
                     PMU_NEIGH_EVENT_ET_REQ_FIFO    : begin    //= 21, // ET Link REQ pushed into the intermediate FIFO
                                                         if (neigh_top.channel.int_req_push) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_ET_REQ_UC_FIFO : begin    //= 22, // ET Link REQ pushed into any of the BANK/UC FIFOs
                                                         if (|neigh_top.channel.out_req_push) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     PMU_NEIGH_EVENT_ET_REQ_SC_UC   : begin    //= 23  // ET Link RSP received from the SC/UC input
                                                         if (neigh_top.channel.fill_fifo_push) begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b1;
                                                         end
                                                         else begin
                                                            pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                                                         end
                                                      end
                     default                        :  pmu_neigh_event_seen_st1[ii][jj]  <= 1'b0;
                  endcase
                  if (pmu_neigh_event_seen_st4[ii][jj]) begin
                     evl_neigh_packet_u pkt;
                     pkt.pmu_event.info.pmu_thread_id = ii;
                     pkt.pmu_event.info.pmu_cntr_id = jj;
                     pkt.pmu_event.info.pmu_event_type = pmu_neigh_event_type_st4[ii][jj];
                     m_rtl_port.send_pmu_event(PMU_TXN_CNTR_INCR, pkt);
                  end
               end
            end


            // monitor signals and gen events
         end
         m_rtl_port.ping_clock();
      end
   end

   //
   // Functional Coverage Sampling
   //
   `ifdef EVL_COVERAGE_ON
      logic [3:0] signal_bank_req_info_push;
      logic signal_f0_l2_miss_req_disable_next;
      logic [1:0] signal_icache_f2_hit;
      logic [1:0] signal_icache_bypass_en;
      logic icache_f2_sram_req_valid_ready;
      logic icache_f0_sram_resp_valid_ready;
      logic f0_l2_miss_req_valid_ready;
      logic f0_l2_miss_resp_valid_ready;
      logic f4_resp_valid_miss_0;
      logic f4_resp_valid_miss_1;
      int icache_l1_sram_resp_latency;
      int f0_l2_miss_latency;
      int fill_done_latency_0;
      int fill_done_latency_1;
      int l1_sram_done;
      int l2_miss_done;
      int fill_done_0;
      int fill_done_1;

      assign signal_bank_req_info_push = {neigh_top.channel.BANK_FIFOS[3].bank_req_info_push,
                                          neigh_top.channel.BANK_FIFOS[2].bank_req_info_push,
                                          neigh_top.channel.BANK_FIFOS[1].bank_req_info_push,
                                          neigh_top.channel.BANK_FIFOS[0].bank_req_info_push};
      assign signal_icache_f2_hit = {neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.f2_hit,
                                     neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f2_hit};
      assign signal_icache_bypass_en = {neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.bypass_en,
                                        neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.bypass_en};
      assign icache_f2_sram_req_valid_ready = (neigh_top.channel.shared_icache.icache.f2_sram_req_valid && neigh_top.channel.shared_icache.icache.f2_sram_req_ready);
      assign icache_f0_sram_resp_valid_ready = (neigh_top.channel.shared_icache.icache.f0_sram_resp_valid && neigh_top.channel.shared_icache.icache.f0_sram_resp_ready);
      assign f0_l2_miss_req_valid_ready = (neigh_top.channel.shared_icache.icache.f0_l2_miss_req_valid && neigh_top.channel.shared_icache.icache.f0_l2_miss_req_ready);
      assign f0_l2_miss_resp_valid_ready = (neigh_top.channel.shared_icache.icache.f0_l2_miss_resp_valid && neigh_top.channel.shared_icache.icache.f0_l2_miss_resp_ready);
      assign f4_resp_valid_miss_0 = (neigh_top.channel.shared_icache.icache.f4_resp_valid[0] && neigh_top.channel.shared_icache.icache.f4_resp_miss[0]);
      assign f4_resp_valid_miss_1 = (neigh_top.channel.shared_icache.icache.f4_resp_valid[1] && neigh_top.channel.shared_icache.icache.f4_resp_miss[1]);

      wire gather_cov = ((reset_neigh === 1'b0) && (mon_reset_off === 1'b1) && (isolate === 1'b0)) ? 1'b1 : 1'b0;

      initial begin
         neigh_cov = new();
         wait (gather_cov == 1'b1);
         fork
            forever @(posedge clock) begin
               signal_f0_l2_miss_req_disable_next <= neigh_top.channel.shared_icache.icache.f0_l2_miss_req_disable_next;

               // L1 sram latency
               if ((icache_f2_sram_req_valid_ready == 1) && (icache_f0_sram_resp_valid_ready == 0)) begin
                  icache_l1_sram_resp_latency = icache_l1_sram_resp_latency + 1;
                  l1_sram_done <= 1;
               end
               if ((icache_f2_sram_req_valid_ready == 0) && (icache_f0_sram_resp_valid_ready == 0) && (l1_sram_done === 1)) begin
                  icache_l1_sram_resp_latency = icache_l1_sram_resp_latency + 1;
               end
               if ((icache_f2_sram_req_valid_ready == 0) && (icache_f0_sram_resp_valid_ready == 1)) begin
                  if (gather_cov == 1'b1) begin
                     neigh_cov.icache_l1_sram_resp_latency_cg.sample(icache_l1_sram_resp_latency);
                  end
                  icache_l1_sram_resp_latency = 0;
                  l1_sram_done <= 0;
               end

               // L2 miss req latency
               if ((f0_l2_miss_req_valid_ready == 1) && (f0_l2_miss_resp_valid_ready == 0)) begin
                  f0_l2_miss_latency = f0_l2_miss_latency + 1;
                  l2_miss_done <= 1;
               end
               if ((f0_l2_miss_req_valid_ready == 0) && (f0_l2_miss_resp_valid_ready == 0) && (l2_miss_done == 1)) begin
                  f0_l2_miss_latency = f0_l2_miss_latency + 1;
               end
               if ((f0_l2_miss_req_valid_ready == 0) && (f0_l2_miss_resp_valid_ready == 1)) begin
                  if (gather_cov == 1'b1) begin
                     neigh_cov.icache_l2_miss_resp_latency_cg.sample(f0_l2_miss_latency);
                  end
                  f0_l2_miss_latency = 0;
                  l2_miss_done <= 0;
               end

               // Fill done pulse latency
               if (f4_resp_valid_miss_0 == 1) begin
                  fill_done_latency_0 = fill_done_latency_0 + 1;
                  fill_done_0 <= 1;
               end
               if ((f4_resp_valid_miss_0 == 0) && (fill_done_0 === 1)) begin
                  if (gather_cov == 1'b1) begin
                     neigh_cov.icache_resp_fill_done_cg.sample(fill_done_latency_0);
                  end
                  fill_done_latency_0 = 0;
                  fill_done_0 <= 0;
               end

               if (f4_resp_valid_miss_1 == 1) begin
                  fill_done_latency_1 = fill_done_latency_1 + 1;
                  fill_done_1 <= 1;
               end
               if ((f4_resp_valid_miss_1 == 0) && (fill_done_1 === 1)) begin
                  if (gather_cov == 1'b1) begin
                     neigh_cov.icache_resp_fill_done_cg.sample(fill_done_latency_1);
                  end
                  fill_done_latency_1 = 0;
                  fill_done_1 <= 0;
               end
            end

            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.shire_id_cg.sample(neigh_top.shire_id);
                  @(neigh_top.shire_id);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_id_cg.sample(neigh_top.neigh_id);
                  @(neigh_top.neigh_id);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_threads_enable_cg.sample(neigh_top.esr_thread0_enable, neigh_top.esr_thread1_enable);
                  @(neigh_top.esr_thread0_enable or neigh_top.esr_thread1_enable);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_minion_features_cg.sample(neigh_top.esr_minion_features);
                  @(neigh_top.esr_minion_features);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_shire_coop_mode_cg.sample(neigh_top.esr_shire_coop_mode);
                  @(neigh_top.esr_shire_coop_mode);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_int_mtip_cg.sample(neigh_top.int_mtip);
                  @(neigh_top.int_mtip);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_int_meip_cg.sample(neigh_top.int_meip);
                  @(neigh_top.int_meip);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_int_seip_cg.sample(neigh_top.int_seip);
                  @(neigh_top.int_seip);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_ipi_msip_cg.sample(neigh_top.ipi_msip);
                  @(neigh_top.ipi_msip);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_ipi_redirect_trigger_cg.sample(neigh_top.ipi_redirect_trigger);
                  @(neigh_top.ipi_redirect_trigger);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_uc_to_neigh_fcc_cg.sample(neigh_top.uc_to_neigh_fcc);
                  @(neigh_top.uc_to_neigh_fcc);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_uc_to_neigh_fcc_target_cg.sample(neigh_top.uc_to_neigh_fcc_target);
                  @(neigh_top.uc_to_neigh_fcc_target);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_icache_sram_req_cg.sample(neigh_top.icache_f2_sram_req_valid, neigh_top.icache_f2_sram_req_ready, neigh_top.icache_f2_sram_req_write, neigh_top.icache_f2_sram_req_addr);
                  @(neigh_top.icache_f2_sram_req_valid or neigh_top.icache_f2_sram_req_ready or neigh_top.icache_f2_sram_req_write or neigh_top.icache_f2_sram_req_addr);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_min_etl_miss_req_cg.sample(neigh_top.l2_dcache_miss_req_valid, neigh_top.l2_dcache_miss_req_ready);
                  @(neigh_top.l2_dcache_miss_req_valid or neigh_top.l2_dcache_miss_req_ready);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_min_etl_evict_req_cg.sample(neigh_top.l2_dcache_evict_req_valid, neigh_top.l2_dcache_evict_req_ready);
                  @(neigh_top.l2_dcache_evict_req_valid or neigh_top.l2_dcache_evict_req_ready);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_icache_etl_req_cg.sample(neigh_top.channel.icache_l2_miss_req_valid, neigh_top.channel.icache_l2_miss_req_ready);
                  @(neigh_top.channel.icache_l2_miss_req_valid or neigh_top.channel.icache_l2_miss_req_ready);
               end
            end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_ptw_etl_req_cg.sample(neigh_top.channel.ptw_l2_miss_req_valid, neigh_top.channel.ptw_l2_miss_req_ready);
//                @(neigh_top.channel.ptw_l2_miss_req_valid or neigh_top.channel.ptw_l2_miss_req_ready);
//             end
//          end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_tbox_etl_req_cg.sample(neigh_top.channel.tbox_miss_req_valid, neigh_top.channel.tbox_miss_req_ready);
//                @(neigh_top.channel.tbox_miss_req_valid or neigh_top.channel.tbox_miss_req_ready);
//             end
//          end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_coop_tload_etl_req_cg.sample(neigh_top.channel.coop_tload_miss_req_valid, neigh_top.channel.coop_tload_miss_req_ready);
                  @(neigh_top.channel.coop_tload_miss_req_valid or neigh_top.channel.coop_tload_miss_req_ready);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_coop_tstore_etl_req_cg.sample(neigh_top.channel.tstore_evict_valid, neigh_top.channel.tstore_miss_req_ready);
                  @(neigh_top.channel.tstore_evict_valid or neigh_top.channel.tstore_miss_req_ready);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_etl_req_goes_to_int_fifo_cg.sample(neigh_top.channel.miss_req_goes_to_int_fifo, neigh_top.channel.miss_req_valid_ff, neigh_top.channel.miss_req_ready, neigh_top.channel.miss_req_goes_to_tbox);
                  @(neigh_top.channel.miss_req_goes_to_int_fifo or neigh_top.channel.miss_req_valid_ff or neigh_top.channel.miss_req_ready or neigh_top.channel.miss_req_goes_to_tbox);
               end
            end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_etl_req_goes_to_tbox_fifo_cg.sample(neigh_top.channel.min_tbox_req_valid, neigh_top.channel.min_tbox_req_ready);
//                @(neigh_top.channel.min_tbox_req_valid or neigh_top.channel.min_tbox_req_ready);
//             end
//          end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_etl_req_goes_to_bank_fifo_cg.sample(signal_bank_req_info_push);
                  @(signal_bank_req_info_push);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_etl_req_goes_to_uc_fifo_cg.sample(neigh_top.channel.uc_req_info_push);
                  @(neigh_top.channel.uc_req_info_push);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_etl_req_to_sc_cg.sample(neigh_top.channel.neigh_sc_req_valid, neigh_top.channel.neigh_sc_req_ready);
                  @(neigh_top.channel.neigh_sc_req_valid or neigh_top.channel.neigh_sc_req_ready);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_min_etl_rsp_cg.sample(neigh_top.channel.fill_fifo_min_valid, neigh_top.channel.fill_fifo_min_ready);
                  @(neigh_top.channel.fill_fifo_min_valid or neigh_top.channel.fill_fifo_min_ready);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_icache_etl_rsp_cg.sample(neigh_top.channel.icache_l2_miss_resp_valid, neigh_top.channel.icache_l2_miss_resp_ready);
                  @(neigh_top.channel.icache_l2_miss_resp_valid or neigh_top.channel.icache_l2_miss_resp_ready);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_ptw_etl_rsp_cg.sample(neigh_top.channel.ptw_l2_miss_resp_valid, neigh_top.channel.ptw_l2_miss_resp_ready);
                  @(neigh_top.channel.ptw_l2_miss_resp_valid or neigh_top.channel.ptw_l2_miss_resp_ready);
               end
            end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_tbox_etl_rsp_cg.sample(neigh_top.channel.sc_tbox_rsp_valid, neigh_top.channel.sc_tbox_rsp_ready);
//                @(neigh_top.channel.sc_tbox_rsp_valid or neigh_top.channel.sc_tbox_rsp_ready);
//             end
//          end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_coop_tstore_etl_rsp_cg.sample(neigh_top.channel.tstore_min_resp_valid, neigh_top.channel.tstore_min_resp_ready);
                  @(neigh_top.channel.tstore_min_resp_valid or neigh_top.channel.tstore_min_resp_ready);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_etl_rsp_from_sc_cg.sample(neigh_top.channel.neigh_sc_rsp_valid, neigh_top.channel.neigh_sc_rsp_ready);
                  @(neigh_top.channel.neigh_sc_rsp_valid or neigh_top.channel.neigh_sc_rsp_ready);
               end
            end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_etl_msg_rsp_from_tbox_cg.sample(neigh_top.channel.tbox_min_rsp_valid_in, neigh_top.channel.tbox_min_rsp_ready_in);
//                @(neigh_top.channel.tbox_min_rsp_valid_in or neigh_top.channel.tbox_min_rsp_ready_in);
//             end
//          end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_min_flb_req_cg.sample(neigh_top.channel.flb_core_neigh_req_valid);
                  @(neigh_top.channel.flb_core_neigh_req_valid);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_flb_req_cg.sample(neigh_top.flb_neigh_l2_req_valid);
                  @(neigh_top.flb_neigh_l2_req_valid);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_min_flb_resp_cg.sample(neigh_top.channel.flb_neigh_core_resp_valid);
                  @(neigh_top.channel.flb_neigh_core_resp_valid);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_flb_resp_cg.sample(neigh_top.flb_l2_neigh_resp_valid);
                  @(neigh_top.flb_l2_neigh_resp_valid);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_min_coop_miss_req_cg.sample(neigh_top.channel.min_coop_miss_req_valid, neigh_top.channel.min_coop_miss_req_ready);
                  @(neigh_top.channel.min_coop_miss_req_valid or neigh_top.channel.min_coop_miss_req_ready);
               end
            end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_coop_tload_master_neigh_cg.sample(neigh_top.channel.coop_tload.master_neigh);
//                @(neigh_top.channel.coop_tload.master_neigh);
//             end
//          end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_coop_tload_slv_rdy_out_cg.sample(neigh_top.coop_tload_slv_rdy_out_valid);
                  @(neigh_top.coop_tload_slv_rdy_out_valid);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_coop_tload_slv_rdy_in_cg.sample(neigh_top.coop_tload_slv_rdy_in_valid);
                  @(neigh_top.coop_tload_slv_rdy_in_valid);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_coop_tload_mst_done_out_cg.sample(neigh_top.coop_tload_mst_done_out_valid);
                  @(neigh_top.coop_tload_mst_done_out_valid);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_coop_tload_mst_done_in_cg.sample(neigh_top.coop_tload_mst_done_in_valid);
                  @(neigh_top.coop_tload_mst_done_in_valid);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_coop_tload_etl_resp_cg.sample(neigh_top.channel.coop_tload_tag_table_cam_pop);
                  @(neigh_top.channel.coop_tload_tag_table_cam_pop);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_min_coop_evict_req_cg.sample(neigh_top.channel.coop_evict_valid, neigh_top.channel.coop_evict_ready);
                  @(neigh_top.channel.coop_evict_valid or neigh_top.channel.coop_evict_ready);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.neigh_coop_store_resp_ack_cg.sample(neigh_top.channel.l2_dcache_resp_tstore_ack);
               end
            end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_min_evict_req_size_cg.sample(neigh_top.channel.l2_dcache_evict_req, neigh_top.channel.l2_dcache_evict_req_valid, neigh_top.channel.l2_dcache_evict_req_ready);
//                @(neigh_top.channel.l2_dcache_evict_req or neigh_top.channel.l2_dcache_evict_req_valid or neigh_top.channel.l2_dcache_evict_req_ready);
//             end
//          end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_min_miss_req_size_cg.sample(neigh_top.channel.l2_dcache_miss_req, neigh_top.channel.l2_dcache_miss_req_valid, neigh_top.channel.l2_dcache_miss_req_ready);
//                @(neigh_top.channel.l2_dcache_miss_req or neigh_top.channel.l2_dcache_miss_req_valid or neigh_top.channel.l2_dcache_miss_req_ready);
//             end
//          end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_min_resp_size_cg.sample(neigh_top.channel.l2_dcache_resp, neigh_top.channel.l2_dcache_resp_valid, neigh_top.channel.l2_dcache_resp_ready);
//                @(neigh_top.channel.l2_dcache_resp or neigh_top.channel.l2_dcache_resp_valid or neigh_top.channel.l2_dcache_resp_ready);
//             end
//          end

            // ICACHE section
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.icache_flush_cg.sample(neigh_top.channel.shared_icache.icache.f0_flush_data);
                  @(neigh_top.channel.shared_icache.icache.f0_flush_data);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.icache_l1_sram_req_cg.sample(neigh_top.channel.shared_icache.icache.f2_sram_req_valid, neigh_top.channel.shared_icache.icache.f2_sram_req_ready, neigh_top.channel.shared_icache.icache.f2_sram_req_addr);
                  @(neigh_top.channel.shared_icache.icache.f2_sram_req_valid or neigh_top.channel.shared_icache.icache.f2_sram_req_ready or neigh_top.channel.shared_icache.icache.f2_sram_req_addr);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.icache_disable_l2_miss_req_cg.sample(neigh_top.channel.shared_icache.icache.f0_l2_miss_req_valid, signal_f0_l2_miss_req_disable_next);
                  @(neigh_top.channel.shared_icache.icache.f0_l2_miss_req_valid or signal_f0_l2_miss_req_disable_next);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.icache_err_log_ctl_cg.sample(neigh_top.channel.shared_icache.icache.esr_err_log_ctl);
                  @(neigh_top.channel.shared_icache.icache.esr_err_log_ctl);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.icache_prefetch_req_cg.sample(neigh_top.channel.shared_icache.icache.esr_prefetch_conf, neigh_top.channel.shared_icache.icache.esr_prefetch_start, neigh_top.channel.shared_icache.icache.esr_prefetch_done);
                  @(neigh_top.channel.shared_icache.icache.esr_prefetch_conf or neigh_top.channel.shared_icache.icache.esr_prefetch_start or neigh_top.channel.shared_icache.icache.esr_prefetch_done);
               end
            end
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.icache_bypass_cg.sample(neigh_top.channel.shared_icache.icache.esr_bypass_icache, signal_icache_f2_hit, signal_icache_bypass_en);
                  @(neigh_top.channel.shared_icache.icache.esr_bypass_icache or signal_icache_f2_hit or signal_icache_bypass_en);
               end
            end

            //fifo_gen coverage RTLMIN-5554
            forever @(posedge clock) begin
               if (gather_cov == 1'b1) begin
                  neigh_cov.int_req_info_fifo_cg.sample(neigh_top.channel.int_req_info_fifo.full,
                                                        neigh_top.channel.int_req_info_fifo.valid,
                                                        neigh_top.channel.int_req_info_fifo.push,
                                                        neigh_top.channel.int_req_info_fifo.pop);
               end
            end

         join
      end

//    generate
//    if (GFX_ENABLED == 1'b1) begin
//       wait (gather_cov == 1'b1);
//       fork
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_etl_msg_req_to_tbox_cg.sample(neigh_top.channel.tbox_router_gen.tbox_router.min_tbox_rsp_valid, neigh_top.channel.tbox_router_gen.tbox_router.min_tbox_rsp_ready);
//                @(neigh_top.channel.tbox_router_gen.tbox_router.min_tbox_rsp_valid or neigh_top.channel.tbox_router_gen.tbox_router.min_tbox_rsp_ready);
//             end
//          end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_etl_req_from_tbox_cg.sample(neigh_top.channel.tbox_router_gen.tbox_router.tbox_neigh_req_valid, neigh_top.channel.tbox_router_gen.tbox_router.tbox_neigh_req_ready);
//                @(neigh_top.channel.tbox_router_gen.tbox_router.tbox_neigh_req_valid or neigh_top.channel.tbox_router_gen.tbox_router.tbox_neigh_req_ready);
//             end
//          end
//          forever @(posedge clock) begin
//             if (gather_cov == 1'b1) begin
//                neigh_cov.neigh_etl_rsp_to_tbox_cg.sample(neigh_top.channel.tbox_router_gen.tbox_router.neigh_tbox_rsp_valid, neigh_top.channel.tbox_router_gen.tbox_router.neigh_tbox_rsp_ready);
//                @(neigh_top.channel.tbox_router_gen.tbox_router.neigh_tbox_rsp_valid or neigh_top.channel.tbox_router_gen.tbox_router.neigh_tbox_rsp_ready);
//             end
//          end
//       join
//    end
//    endgenerate

      generate
         for (genvar i = 0; i < 8; i++) begin :neigh_cov_per_minion_g
            wire [1:0][`NUM_NEIGH-1:0] master_neigh_d0;
            wire [1:0][`MIN_PER_N-1:0] local_coop_min_mask_d0;
            wire [1:0][`NUM_NEIGH-1:0] local_coop_neigh_mask_d0;
            reg  [1:0][`NUM_NEIGH-1:0] master_neigh_d1;
            reg  [1:0][`MIN_PER_N-1:0] local_coop_min_mask_d1;
            reg  [1:0][`NUM_NEIGH-1:0] local_coop_neigh_mask_d1;
            wire [1:0]                 local_coop_mask_is_x;

            assign master_neigh_d0[0]           = neigh_top.channel.coop_tload.master_neigh[i][0];
            assign local_coop_min_mask_d0[0]    = neigh_top.channel.coop_tload.MINIONS[i].MISS_FF[0].local_coop_min_mask;
            assign local_coop_neigh_mask_d0[0]  = neigh_top.channel.coop_tload.MINIONS[i].MISS_FF[0].local_coop_neigh_mask;
            assign master_neigh_d0[1]           = neigh_top.channel.coop_tload.master_neigh[i][1];
            assign local_coop_min_mask_d0[1]    = neigh_top.channel.coop_tload.MINIONS[i].MISS_FF[1].local_coop_min_mask;
            assign local_coop_neigh_mask_d0[1]  = neigh_top.channel.coop_tload.MINIONS[i].MISS_FF[1].local_coop_neigh_mask;
            assign local_coop_mask_is_x[0]      = (^{local_coop_min_mask_d0[0], local_coop_neigh_mask_d0[0]} === 1'bx) ? 1'b1 : 1'b0;
            assign local_coop_mask_is_x[1]      = (^{local_coop_min_mask_d0[1], local_coop_neigh_mask_d0[1]} === 1'bx) ? 1'b1 : 1'b0;

            initial begin
               neigh_cov_per_minion[i] = new();
               #0 wait (gather_cov == 1'b1);
               @(posedge clock);
               master_neigh_d1[0]          <= master_neigh_d0[0];
               master_neigh_d1[1]          <= master_neigh_d0[1];
               local_coop_min_mask_d1[0]   <= local_coop_min_mask_d0[0];
               local_coop_neigh_mask_d1[0] <= local_coop_neigh_mask_d0[0];
               local_coop_min_mask_d1[1]   <= local_coop_min_mask_d0[1];
               local_coop_neigh_mask_d1[1] <= local_coop_neigh_mask_d0[1];
               forever @(posedge clock) begin
                  if (gather_cov == 1'b1) begin
                     if (master_neigh_d0[0] !== master_neigh_d1[0]) begin
                        neigh_cov_per_minion[i].neigh_coop_tload_master_neigh_cg.sample(0, master_neigh_d0[0]);
                        master_neigh_d1[0] <= master_neigh_d0[0];
                     end
                     else if (master_neigh_d0[1] !== master_neigh_d1[1]) begin
                        neigh_cov_per_minion[i].neigh_coop_tload_master_neigh_cg.sample(1, master_neigh_d0[1]);
                        master_neigh_d1[1] <= master_neigh_d0[1];
                     end
                     if ((local_coop_min_mask_d1[0] !== local_coop_min_mask_d0[0]) || (local_coop_neigh_mask_d1[0] !== local_coop_neigh_mask_d0[0])) begin
                        if (local_coop_mask_is_x[0] === 1'b0) begin
                           neigh_cov_per_minion[i].neigh_coop_tload_mask_cg.sample($countones(local_coop_min_mask_d0[0]), $countones(local_coop_neigh_mask_d0[0]));
                        end
                        local_coop_min_mask_d1[0]   <= local_coop_min_mask_d0[0];
                        local_coop_neigh_mask_d1[0] <= local_coop_neigh_mask_d0[0];
                     end
                     else if ((local_coop_min_mask_d1[1] !== local_coop_min_mask_d0[1]) || (local_coop_neigh_mask_d1[1] !== local_coop_neigh_mask_d0[1])) begin
                        if (local_coop_mask_is_x[1] === 1'b0) begin
                           neigh_cov_per_minion[i].neigh_coop_tload_mask_cg.sample($countones(local_coop_min_mask_d0[1]), $countones(local_coop_neigh_mask_d0[1]));
                        end
                        local_coop_min_mask_d1[1]   <= local_coop_min_mask_d0[1];
                        local_coop_neigh_mask_d1[1] <= local_coop_neigh_mask_d0[1];
                     end

                     if ( ((neigh_top.channel.l2_dcache_evict_req_valid[i][0] & neigh_top.channel.l2_dcache_evict_req_ready[i][0]) === 1'b1) ||
                          ((neigh_top.channel.l2_dcache_evict_req_valid[i][1] & neigh_top.channel.l2_dcache_evict_req_ready[i][1]) === 1'b1) ) begin
                        neigh_cov_per_minion[i].neigh_min_evict_req_size_cg.sample(neigh_top.channel.l2_dcache_evict_req[i]);
                     end

                     if ( ((neigh_top.channel.l2_dcache_miss_req_valid[i][0] & neigh_top.channel.l2_dcache_miss_req_ready[i][0]) === 1'b1) ||
                          ((neigh_top.channel.l2_dcache_miss_req_valid[i][1] & neigh_top.channel.l2_dcache_miss_req_ready[i][1]) === 1'b1) ||
                          ((neigh_top.channel.l2_dcache_miss_req_valid[i][2] & neigh_top.channel.l2_dcache_miss_req_ready[i][2]) === 1'b1) ) begin
                        neigh_cov_per_minion[i].neigh_min_miss_req_size_cg.sample(neigh_top.channel.l2_dcache_miss_req[i]);
                     end

                     if ((neigh_top.channel.l2_dcache_resp_valid[i] & neigh_top.channel.l2_dcache_resp_ready[i]) === 1'b1) begin
                        neigh_cov_per_minion[i].neigh_min_resp_size_cg.sample(neigh_top.channel.l2_dcache_resp[i]);
                     end
                  end
                  else begin
                     master_neigh_d1[0]          <= master_neigh_d0[0];
                     master_neigh_d1[1]          <= master_neigh_d0[1];
                     local_coop_min_mask_d1[0]   <= local_coop_min_mask_d0[0];
                     local_coop_neigh_mask_d1[0] <= local_coop_neigh_mask_d0[0];
                     local_coop_min_mask_d1[1]   <= local_coop_min_mask_d0[1];
                     local_coop_neigh_mask_d1[1] <= local_coop_neigh_mask_d0[1];
                  end
               end
            end
         end
      endgenerate

   `endif

   //
   // Performance monitoring
   //
   logic dreams;

   initial begin
      if (!`et_value_plusargs("DREAMS=%d", dreams)) begin
         dreams = 0;
      end
   end

   // Stores a mapping of dest+id to address
   logic [(1<<`ET_LINK_DEST_SIZE)-1:0][(1<<`ET_LINK_ID_SIZE)-1:0][39:0] id_to_addr;
   logic [(1<<`ET_LINK_DEST_SIZE)-1:0][(1<<`ET_LINK_ID_SIZE)-1:0][39:0] id_to_addr_next;

   `FF(clock, id_to_addr, id_to_addr_next)

   always_comb
   begin
      id_to_addr_next = id_to_addr;
      // New request sent
      if(|neigh_top.neigh_sc_req_valid) begin
         id_to_addr_next[neigh_top.neigh_sc_req_info.source][neigh_top.neigh_sc_req_info.id] = neigh_top.neigh_sc_req_info.address;
      end
   end

   // Actual event dumping
   always_ff @(negedge clock)
   begin
      if (~reset_neigh && dreams) begin
         $timeformat(-9, 3, "", 8);
         // Writing to SC
         for(integer bank = 0; bank < 5; bank++) begin
            if(neigh_top.neigh_sc_req_valid[bank]) begin
               $display("EVENT %9.0f 100 %d %d %d %h %h %h", $floor($realtime + 0.25), (neigh_top.shire_id << 2) + NEIGH_ID, bank, neigh_top.neigh_sc_req_ready[bank], neigh_top.neigh_sc_req_info.id, neigh_top.neigh_sc_req_info.address, neigh_top.neigh_sc_req_info.opcode);
            end
         end

         // Responses from SC
         if(neigh_top.channel.neigh_sc_rsp_valid) begin
            $display("EVENT %9.0f 101 %d %d %h %h %h", $floor($realtime + 0.25), (neigh_top.shire_id << 2) + NEIGH_ID, neigh_top.channel.neigh_sc_rsp_ready, neigh_top.channel.neigh_sc_rsp_info.dest, neigh_top.channel.neigh_sc_rsp_info.id, id_to_addr[neigh_top.channel.neigh_sc_rsp_info.dest][neigh_top.channel.neigh_sc_rsp_info.id]);
         end

         // Fill FIFO selects
         if(|neigh_top.channel.fill_fifo.out_valid_next) begin
            $display("EVENT %9.0f 102 %d %h %h %h", $floor($realtime + 0.25), (neigh_top.shire_id << 2) + NEIGH_ID, neigh_top.channel.fill_fifo.out_data_next.rsp.dest, neigh_top.channel.fill_fifo.out_data_next.rsp.id, id_to_addr[neigh_top.channel.fill_fifo.out_data_next.rsp.dest][neigh_top.channel.fill_fifo.out_data_next.rsp.id]);
         end
      end
   end

`include "dv/neigh_common/cov/neigh_assert.svh"

   `ifndef ET_USE_ASIC_MEMS
      `ifndef ET_USE_BIST_RTL

         localparam int lp_tag_way0 = 0;
         localparam int lp_tag_way1 = (`ICACHE_WAYS < 1) ? 0 : 1;
         localparam int lp_tag_way2 = (`ICACHE_WAYS < 2) ? 0 : 2;
         localparam int lp_tag_way3 = (`ICACHE_WAYS < 3) ? 0 : 3;

         //-----------------------------------------------------------------------------------------
         //
         // Function: mod_get_cache_tag
         //
         function automatic evl_paddr_t mod_get_cache_tag(input evl_paddr_t cindex,
                                                          input int         cway);
            int         set;
            int         way;
            evl_paddr_t index_mask;
            evl_paddr_t paddr;

            index_mask = ((evl_paddr_t'(1) << (`EVL_PADDR_WIDTH - `ICACHE_PA_TAG_SIZE)) - 1) & (~evl_paddr_t'(`EVL_LINE_BYTES - 1));
            set        = (int'(cindex) >> `EVL_LINE_ADDR_LSB) & (`ICACHE_SETS - 1);
            way        = cway % `ICACHE_WAYS;
            paddr      = ~evl_paddr_t'(0);
            if (channel.shared_icache.icache.tag_array.f0_tag_valid[set][way] === 1'b1) begin
               case (way)
                  0: paddr = (cindex & index_mask) | ((evl_paddr_t'(channel.shared_icache.icache.tag_array.TAG_ARRAY_WAYS[lp_tag_way0].icache_tag_data_array.mem[set])) << `ICACHE_DATA_ADDR_SIZE);
                  1: paddr = (cindex & index_mask) | ((evl_paddr_t'(channel.shared_icache.icache.tag_array.TAG_ARRAY_WAYS[lp_tag_way1].icache_tag_data_array.mem[set])) << `ICACHE_DATA_ADDR_SIZE);
                  2: paddr = (cindex & index_mask) | ((evl_paddr_t'(channel.shared_icache.icache.tag_array.TAG_ARRAY_WAYS[lp_tag_way2].icache_tag_data_array.mem[set])) << `ICACHE_DATA_ADDR_SIZE);
                  3: paddr = (cindex & index_mask) | ((evl_paddr_t'(channel.shared_icache.icache.tag_array.TAG_ARRAY_WAYS[lp_tag_way3].icache_tag_data_array.mem[set])) << `ICACHE_DATA_ADDR_SIZE);
               endcase
            end
            return paddr;
         endfunction : mod_get_cache_tag

      `endif // ifndef ET_USE_BIST_RTL
   `endif // ifndef ET_USE_ASIC_MEMS

   class my_neigh_rtl_port extends evl_neigh_rtl_port;

      `ifndef ET_USE_ASIC_MEMS
         `ifndef ET_USE_BIST_RTL

            //--------------------------------------------------------------------------------------
            //
            // Function: get_cache_tag
            //
            virtual function evl_paddr_t get_cache_tag(input int cindex,
                                                       input int cway  = 0,
                                                       input int ctype = 0);
               if (ctype inside { SC_RAM_TYPE_ICACHE_TAG, SC_RAM_TYPE_ICACHE0_TAG, SC_RAM_TYPE_ICACHE1_TAG, SC_RAM_TYPE_ICACHE2_TAG, SC_RAM_TYPE_ICACHE3_TAG }) begin
                  return mod_get_cache_tag(cindex, cway);
               end
               return ~evl_paddr_t'(0);
            endfunction : get_cache_tag

         `endif // ifndef ET_USE_BIST_RTL
      `endif // ifndef ET_USE_ASIC_MEMS


      //-----------------------------------------------------------------------------------------
      //
      // Constructor
      //
      function new(input string         full_name_in = "neigh_rtl_port",
                   input evl_agent_id_t agent_pid_in = ~evl_agent_id_t'(0),
                   input int            skip_levels_in[] = {});
         super.new(full_name_in, agent_pid_in, skip_levels_in);
      endfunction : new
   endclass : my_neigh_rtl_port

endmodule : evl_neigh_interface
