`include "rtl/inc/soc.vh"
`include "dv/common/apb/apb_vc/evl_apb_pkg.sv"

`ifndef __EVL_STANDALONE_MINION_PKG
   `include "/dv/soc/ioshire/standalone_minion/evl_standalone_minion_vc_pkg.sv"
`endif

`define  MIN_PER_SP  1        // only one SP_minion (standalone_minion)


module evl_standalone_minion_interface #(parameter int STANDALONE_MINION_ID = 0,
                             parameter [`MIN_PER_SP-1:0] DISABLE_MINIONS = {`MIN_PER_SP{1'b0}},
                             parameter [`MIN_PER_SP-1:0] STUB_MINIONS    = {`MIN_PER_SP{1'b0}},
                             parameter bit              GFX_ENABLED     = 1'b0)
                              (input wire                  reset_standalone_minion,
                               input wire                  clock
                               //input wire APB_to_standalone_minion_t   APB_ESR_req,
                               //input wire APB_from_standalone_minion_t APB_ESR_rsp
                               );
   import evl_base_pkg::*;
   import evl_standalone_minion_vc_pkg::*;

   reg  start_monitor        = 1'b0;
   reg  mon_reset_on         = 1'b0;
   reg  mon_reset_off        = 1'b0;


/***
   // Fast Local Barrier request interface between standalone_minion and L2
   wire                                             flb_mon_req_valid;
   wire [`EVL_FLB_DATA_SIZE-1:0]                    flb_mon_req_data;

   // Fast Local Barrier response interface between standalone_minion and L2
   wire                                             flb_mon_resp_valid;
   wire [`EVL_FLB_DATA_SIZE-1:0]                    flb_mon_resp_data;

   // Fast Local Barrier request interface between cores and standalone_minion
   wire [`MIN_PER_SP-1:0]                            flb_core_standalone_minion_req_valid;
   wire [`EVL_FLB_DATA_SIZE-1:0] [`MIN_PER_SP-1:0]   flb_core_standalone_minion_req_data;

   // Fast Local Barrier response interface between cores and standalone_minion
   wire [`MIN_PER_SP-1:0]                            flb_standalone_minion_core_resp_valid;
   wire  [`MIN_PER_SP-1:0][11:0]                     flb_standalone_minion_core_resp_data;
***/

/***
   wire                                             mon_apb_penable;
   wire [`MIN_PER_SP-1:0]                            mon_apb_psel;
   wire                                             mon_apb_pwrite;
   wire [`MIN_DM_APB_ADDR_WIDTH-1:0]                mon_apb_paddr;
   wire [`ESR_APB_D_WIDTH-1:0]                      mon_apb_pwdata;
   wire [`MIN_PER_SP-1:0]                            mon_apb_pready;
   wire [`MIN_PER_SP-1:0][`ESR_APB_D_WIDTH - 1:0]    mon_apb_prdata;
   wire [`MIN_PER_SP-1:0]                            mon_apb_pslverr;
***/

   tri0 [`EVL_TRANS_ID_MSB:0]                       unused_miss_req_trans_id;
   tri0 [`EVL_TRANS_ID_MSB:0]                       unused_miss_tenb_req_trans_id;
   tri0 [`EVL_TRANS_ID_MSB:0]                       unused_miss_tenscp_req_trans_id;
   tri0 [`EVL_TRANS_ID_MSB:0]                       unused_evict_req_trans_id;


//   // Standalone_Minion coverage
//   standalone_minion_cvg standalone_minion_cov = new();


   // declare monitor wires connecting to RTL signals (logic):
   // DCache request to L2 evict
   wire [`DCACHE_L2_EVICT_REQ_PORTS-1:0]          mon_l2_dcache_evict_req_ready;
   wire [`DCACHE_L2_EVICT_REQ_PORTS-1:0]          mon_l2_dcache_evict_req_valid;
   wire et_link_minion_evict_req_info_t           mon_l2_dcache_evict_req;
   // DCache request to L2 miss
   wire [`DCACHE_L2_MISS_REQ_PORTS-1:0]           mon_l2_dcache_miss_req_ready;
   wire [`DCACHE_L2_MISS_REQ_PORTS-1:0]           mon_l2_dcache_miss_req_valid;
   wire et_link_minion_miss_req_info_t            mon_l2_dcache_miss_req;
   // L2 response to DCache
   wire                                           mon_l2_dcache_resp_ready;
   wire                                           mon_l2_dcache_resp_valid;
   wire                                           mon_l2_dcache_resp_valid_prev;
   wire et_link_minion_rsp_info_t                 mon_l2_dcache_resp;

   // DCache PTW request
   wire minion_ptw_req                            mon_dc_ptw_req_data;
   wire                                           mon_dc_ptw_req_valid;
   wire                                           mon_dc_ptw_req_ready;
   // DCache PTW response
   wire minion_ptw_pte                            mon_ptw_dc_resp_data;
   wire                                           mon_ptw_dc_resp_valid;


   assign mon_l2_dcache_evict_req_ready    = standalone_minion.l2_dcache_evict_req_ready;
   assign mon_l2_dcache_evict_req_valid    = standalone_minion.l2_dcache_evict_req_valid;
   assign mon_l2_dcache_evict_req          = standalone_minion.l2_dcache_evict_req;
   assign mon_l2_dcache_miss_req_ready     = standalone_minion.l2_dcache_miss_req_ready;
   assign mon_l2_dcache_miss_req_valid     = standalone_minion.l2_dcache_miss_req_valid;
   assign mon_l2_dcache_miss_req           = standalone_minion.l2_dcache_miss_req;
   assign mon_l2_dcache_resp_ready         = standalone_minion.l2_dcache_resp_ready;
   assign mon_l2_dcache_resp_valid         = standalone_minion.l2_dcache_resp_valid;
   assign mon_l2_dcache_resp               = standalone_minion.l2_dcache_resp;

   assign mon_dc_ptw_req_ready             = standalone_minion.dc_ptw_req_ready;
   assign mon_dc_ptw_req_valid             = standalone_minion.dc_ptw_req_valid;
   assign mon_dc_ptw_req_data              = standalone_minion.dc_ptw_req_data;
   assign mon_ptw_dc_resp_valid            = standalone_minion.ptw_dc_resp_valid;
   assign mon_ptw_dc_resp_data             = standalone_minion.ptw_dc_resp_data;


   //
   // Create the Minion interfaces
   //
   genvar gen_it;

   generate for(gen_it = 0; gen_it < `MIN_PER_SP; gen_it++)
   begin : gen_minion_itf

      //localparam minPerPtwIdx = gen_it % `MIN_PER_PTW;
      //localparam ptwIdx       = gen_it / `MIN_PER_PTW;

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
         .isolate              ( 1'b0                          ),
         .reset                ( reset_standalone_minion       ),
         .clk                  ( clock                         ),
         .req_valid            ( mon_l2_dcache_miss_req_valid  ),
         .req_ready            ( mon_l2_dcache_miss_req_ready  ),
         .req_info             ( mon_l2_dcache_miss_req        ),
         .req_trans_id         ( unused_miss_req_trans_id      ),
         .rsp_valid            ( mon_l2_dcache_resp_valid      ),
         .rsp_ready            ( mon_l2_dcache_resp_ready      ),
         .rsp_info             ( mon_l2_dcache_resp            ),
         .use_rsp_source_is_uc ( 1'b0                          ),
         .rsp_source_is_uc     ( 1'b0                          )
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
         .isolate              ( 1'b0                           ),
         .reset                ( reset_standalone_minion        ),
         .clk                  ( clock                          ),
         .req_valid            ( mon_l2_dcache_evict_req_valid  ),
         .req_ready            ( mon_l2_dcache_evict_req_ready  ),
         .req_info             ( mon_l2_dcache_evict_req        ),
         .req_trans_id         ( unused_evict_req_trans_id      ),
         .rsp_valid            ( mon_l2_dcache_resp_valid       ),
         .rsp_ready            ( mon_l2_dcache_resp_ready       ),
         .rsp_info             ( mon_l2_dcache_resp             ),
         .use_rsp_source_is_uc ( 1'b0                           ),
         .rsp_source_is_uc     ( 1'b0                           )
         );


      //
      // PTW requests interface
      //
      evl_ptw_interface #(
                          .AGENT_ID ( gen_it )
                         )
      m_ptw_req (
         .isolate   ( 1'b0                    ),
         .reset     ( reset_standalone_minion ),
         .clock     ( clock                   ),
         .req_valid ( mon_dc_ptw_req_valid    ),
         .req_ready ( mon_dc_ptw_req_ready    ),
         .req_info  ( mon_dc_ptw_req_data     ),
         .rsp_valid ( mon_ptw_dc_resp_valid   ),
         .rsp_info  ( mon_ptw_dc_resp_data    )
      );


/***
      //
      // Debug APB interface
      //
      assign mon_apb_penable          = standalone_minion.minion_apb_penable;
      assign mon_apb_psel    [gen_it] = standalone_minion.minion_apb_psel[gen_it];
      assign mon_apb_pwrite           = standalone_minion.minion_apb_pwrite;
      assign mon_apb_paddr            = standalone_minion.minion_apb_paddr;
      assign mon_apb_pwdata           = standalone_minion.minion_apb_pwdata;
      assign mon_apb_pready  [gen_it] = standalone_minion.minion_apb_pready[gen_it];
      assign mon_apb_prdata  [gen_it] = standalone_minion.minion_apb_prdata[gen_it];
      assign mon_apb_pslverr [gen_it] = standalone_minion.minion_apb_pslverr[gen_it];

      evl_apb_interface #(.AGENT_ID    ( gen_it                 ),
                          .ADDR_WIDTH  ( `MIN_DM_APB_ADDR_WIDTH ),
                          .DATA_WIDTH  ( `ESR_APB_D_WIDTH       )
                         )
      m_debug_itf (
         .isolate ( 1'b0                     ),
         .pclk    ( clock                    ),
         .presetn ( ~reset_standalone_minion ),
         .penable ( mon_apb_penable          ),
         .psel    ( mon_apb_psel    [gen_it] ),
         .pwrite  ( mon_apb_pwrite           ),
         .paddr   ( mon_apb_paddr            ),
         .pwdata  ( mon_apb_pwdata           ),
         .pready  ( mon_apb_pready  [gen_it] ),
         .prdata  ( mon_apb_prdata  [gen_it] ),
         .pslverr ( mon_apb_pslverr [gen_it] )
      );
***/


/***
      if (DISABLE_MINIONS[gen_it] == 1'b1) begin
         assign standalone_minion.l2_dcache_miss_req_valid[gen_it]       = {`DCACHE_L2_MISS_REQ_PORTS{1'b0}};
         assign standalone_minion.l2_dcache_resp_ready[gen_it]           = 1'b0;
         assign standalone_minion.l2_dcache_evict_req_valid[gen_it]      = 1'b0;
         assign standalone_minion.l2_dcache_resp_ready[gen_it]           = 1'b0;
         assign standalone_minion.dc_ptw_req_valid[ptwIdx][minPerPtwIdx] = 1'b0;
      end

      if ((STUB_MINIONS[gen_it] == 1'b1) || (DISABLE_MINIONS[gen_it] == 1'b1)) begin
         assign standalone_minion.icache_req_valid[gen_it]               = 1'b0;
         assign standalone_minion.icache_flush_data[gen_it]              = 1'b0;
         assign standalone_minion.tlb_invalidate[gen_it]                 = 1'b0;
         //assign standalone_minion.flb_standalone_minion_req_valid[gen_it]            = 1'b0;
         assign standalone_minion.minion_apb_pready[gen_it]              = 1'b0;
         assign standalone_minion.minion_apb_pslverr[gen_it]             = 1'b0;
      end
***/


   end // block: gen_minion_itf
   endgenerate


/***
   //
   // ESR APB interface
   //
   evl_apb_interface #(.AGENT_ID    ( STANDALONE_MINION_ID             ),
                       .ADDR_WIDTH  ( `ESR_APB_AD_PP_WIDTH ),
                       .DATA_WIDTH  ( `ESR_APB_D_WIDTH     )
                      ) m_apb_esr (
         .isolate ( 1'b0                     ),
         .pclk    ( clock                    ),
         .presetn ( ~reset_standalone_minion ),
         .penable ( APB_ESR_req.apb_penable  ),
         .psel    ( APB_ESR_req.apb_psel     ),
         .pwrite  ( APB_ESR_req.apb_pwrite   ),
         .paddr   ( APB_ESR_req.apb_paddr    ),
         .pwdata  ( APB_ESR_req.apb_pwdata   ),

         .pready  ( APB_ESR_rsp.apb_pready   ),
         .prdata  ( APB_ESR_rsp.apb_prdata   ),
         .pslverr ( APB_ESR_rsp.apb_pslverr  )
      );
***/


/***
   //
   // Fast Local Barrier interface
   //
   assign flb_mon_req_valid   = standalone_minion.flb_standalone_minion_l2_req_valid;
   assign flb_mon_req_data    = standalone_minion.flb_standalone_minion_l2_req_data;
   assign flb_mon_resp_valid  = standalone_minion.flb_l2_standalone_minion_resp_valid;
   assign flb_mon_resp_data   = standalone_minion.flb_l2_standalone_minion_resp_data;

   assign flb_core_standalone_minion_req_valid  = standalone_minion.flb_standalone_minion_req_valid;
   assign flb_core_standalone_minion_req_data   = standalone_minion.flb_standalone_minion_req_data;
   assign flb_standalone_minion_core_resp_valid = standalone_minion.flb_standalone_minion_resp_valid;
   assign flb_standalone_minion_core_resp_data  = standalone_minion.flb_standalone_minion_resp_data;
***/


   //
   // EVL Proxy Code: --- add this interface instance into evl_rtl_instance_map ... ???
   //
   bit m_register_me = evl_rtl_instance_map::add_instance_info($sformatf("%m"), STANDALONE_MINION_RTL_AGENT, STANDALONE_MINION_ID, "evl_standalone_minion_env", "", "", "", { 1 }); 

   evl_standalone_minion_rtl_port m_rtl_port = new($sformatf("%m"), STANDALONE_MINION_ID, { 1 });

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

   evl_trans_id_t trans_id[1+`MIN_PER_SP-1:0];



//   logic [`PA_SIZE-1:0] start_pc;
//   initial begin
//      if (`et_value_plusargs("START_PC=0x%x", start_pc)) begin
//         force standalone_minion.channel.esrs.reg_minion_boot = start_pc;
//      end
//   end



   //
   // Monitor the signals here and send appropriate info to the RTL port proxy.
   //
   always @(posedge clock) begin
      if (start_monitor === 1'b1) begin
         if ((reset_standalone_minion === 1'b1) && (mon_reset_on === 1'b0)) begin
            m_rtl_port.assert_reset();
            for (int i=0; i<1+`MIN_PER_SP; i++)
               trans_id[i] = 0;
         end
         else if ((reset_standalone_minion === 1'b0) && (mon_reset_off === 1'b0)) begin
            m_rtl_port.deassert_reset();
         end
         mon_reset_on  <= ~reset_standalone_minion;
         mon_reset_off <= reset_standalone_minion;

         if (reset_standalone_minion === 1'b0) begin

          /***
             // FLB request
            if (flb_mon_req_valid === 1'b1) begin
               evl_standalone_minion_packet_u pkt;
               pkt.flb.info.data =  flb_mon_req_data;
               pkt.flb.info.flb_type =  L2_FLB_REQ;
               m_rtl_port.send_event(L2_FLB_REQ, STANDALONE_MINION_ID, trans_id[`MIN_PER_SP]++, pkt);
            end

            for (int i=0; i<`MIN_PER_SP; i++) begin
               // FLB request
               if (flb_core_standalone_minion_req_valid[i] === 1'b1) begin
                  evl_standalone_minion_packet_u pkt;
                  pkt.flb.info.data =  flb_core_standalone_minion_req_data[i];
                  pkt.flb.info.flb_type =  CORE_FLB_REQ;
                  m_rtl_port.send_event(CORE_FLB_REQ, i, trans_id[i]++, pkt);
               end
            end

            // FLB response
            if (flb_mon_resp_valid === 1'b1) begin
               evl_standalone_minion_packet_u pkt;
               pkt.flb.info.data =  flb_mon_resp_data;
               pkt.flb.info.flb_type =  L2_FLB_RSP;
               m_rtl_port.send_event(L2_FLB_RSP, STANDALONE_MINION_ID, trans_id[`MIN_PER_SP]++, pkt);
            end

            for (int i=0; i<`MIN_PER_SP; i++) begin
               // FLB request
               if (flb_standalone_minion_core_resp_valid[i] === 1'b1) begin
                  evl_standalone_minion_packet_u pkt;
                  pkt.flb.info.data =  flb_standalone_minion_core_resp_data[i];
                  pkt.flb.info.flb_type =  CORE_FLB_RSP;
                  m_rtl_port.send_event(CORE_FLB_RSP, i, trans_id[i]++, pkt);
               end
            end
          ***/

            // monitor signals and gen events
         end
         m_rtl_port.ping_clock();
      end
   end


/***************** 
   //
   // Functional Coverage Sampling
   //
   `ifdef EVL_COVERAGE_ON
      logic [3:0] signal_bank_req_info_push;
      logic signal_f0_l2_miss_req_disable_next;
      logic [1:0] signal_icache_f2_hit;
      logic [1:0] signal_icache_bypass_en;
      logic [23:0] signal_evict_goes_to_local;
      logic [7:0]  signal_evict_ff_available;
      logic icache_f2_sram_req_valid_ready;
      logic icache_f0_sram_resp_valid_ready;
      logic f0_l2_miss_req_valid_ready;
      logic f0_l2_miss_resp_valid_ready;
      logic f4_resp_valid_miss_0;
      logic f4_resp_valid_miss_1;
      int icache_l1_sram_resp_latency;
      int f0_l2_miss_latency;
      int latency;
      int lat;
      int fill_done_latency_0;
      int fill_done_latency_1;
      int done;
      int d;
      int fill_done_0;
      int fill_done_1;

      assign signal_bank_req_info_push = {standalone_minion.channel.BANK_FIFOS[3].bank_req_info_push,
                       standalone_minion.channel.BANK_FIFOS[2].bank_req_info_push,
                       standalone_minion.channel.BANK_FIFOS[1].bank_req_info_push,
                       standalone_minion.channel.BANK_FIFOS[0].bank_req_info_push};
      assign signal_icache_f2_hit = {standalone_minion.channel.shared_icache.icache.UCACHE[1].micro_cache.f2_hit,
                       standalone_minion.channel.shared_icache.icache.UCACHE[0].micro_cache.f2_hit};
      assign signal_icache_bypass_en = {standalone_minion.channel.shared_icache.icache.UCACHE[1].micro_cache.bypass_en,
                       standalone_minion.channel.shared_icache.icache.UCACHE[0].micro_cache.bypass_en};
      assign signal_evict_goes_to_local = {standalone_minion.channel.MINION_FF[7].evict_ff.standalone_minion_evict_ff_regular.evict_goes_to_local,
                       standalone_minion.channel.MINION_FF[6].evict_ff.standalone_minion_evict_ff_regular.evict_goes_to_local,
                       standalone_minion.channel.MINION_FF[5].evict_ff.standalone_minion_evict_ff_regular.evict_goes_to_local,
                       standalone_minion.channel.MINION_FF[4].evict_ff.standalone_minion_evict_ff_regular.evict_goes_to_local,
                       standalone_minion.channel.MINION_FF[3].evict_ff.standalone_minion_evict_ff_regular.evict_goes_to_local,
                       standalone_minion.channel.MINION_FF[2].evict_ff.standalone_minion_evict_ff_regular.evict_goes_to_local,
                       standalone_minion.channel.MINION_FF[1].evict_ff.standalone_minion_evict_ff_regular.evict_goes_to_local,
		       standalone_minion.channel.MINION_FF[0].evict_ff.standalone_minion_evict_ff_regular.evict_goes_to_local};
      assign signal_evict_ff_available = {standalone_minion.channel.MINION_FF[7].evict_ff.standalone_minion_evict_ff_regular.evict_ff_available,
                       standalone_minion.channel.MINION_FF[6].evict_ff.standalone_minion_evict_ff_regular.evict_ff_available,
                       standalone_minion.channel.MINION_FF[5].evict_ff.standalone_minion_evict_ff_regular.evict_ff_available,
                       standalone_minion.channel.MINION_FF[4].evict_ff.standalone_minion_evict_ff_regular.evict_ff_available,
                       standalone_minion.channel.MINION_FF[3].evict_ff.standalone_minion_evict_ff_regular.evict_ff_available,
                       standalone_minion.channel.MINION_FF[2].evict_ff.standalone_minion_evict_ff_regular.evict_ff_available,
                       standalone_minion.channel.MINION_FF[1].evict_ff.standalone_minion_evict_ff_regular.evict_ff_available,
		       standalone_minion.channel.MINION_FF[0].evict_ff.standalone_minion_evict_ff_regular.evict_ff_available};
      assign icache_f2_sram_req_valid_ready = (standalone_minion.channel.shared_icache.icache.f2_sram_req_valid && standalone_minion.channel.shared_icache.icache.f2_sram_req_ready);
      assign icache_f0_sram_resp_valid_ready = (standalone_minion.channel.shared_icache.icache.f0_sram_resp_valid && standalone_minion.channel.shared_icache.icache.f0_sram_resp_ready);
      assign f0_l2_miss_req_valid_ready = (standalone_minion.channel.shared_icache.icache.f0_l2_miss_req_valid && standalone_minion.channel.shared_icache.icache.f0_l2_miss_req_ready);
      assign f0_l2_miss_resp_valid_ready = (standalone_minion.channel.shared_icache.icache.f0_l2_miss_resp_valid && standalone_minion.channel.shared_icache.icache.f0_l2_miss_resp_ready);
      assign f4_resp_valid_miss_0 = (standalone_minion.channel.shared_icache.icache.f4_resp_valid[0] && standalone_minion.channel.shared_icache.icache.f4_resp_miss[0]);
      assign f4_resp_valid_miss_1 = (standalone_minion.channel.shared_icache.icache.f4_resp_valid[1] && standalone_minion.channel.shared_icache.icache.f4_resp_miss[1]);

      always @(posedge clock) begin
	 signal_f0_l2_miss_req_disable_next <=  standalone_minion.channel.shared_icache.icache.f0_l2_miss_req_disable_next;

	//L1 sram latency
	 if ((icache_f2_sram_req_valid_ready == 1) && (icache_f0_sram_resp_valid_ready == 0)) begin
	 	icache_l1_sram_resp_latency = icache_l1_sram_resp_latency + 1;
 		done = 1;
 	 end
	 if ((icache_f2_sram_req_valid_ready == 0) && (icache_f0_sram_resp_valid_ready == 0) && done == 1) begin
		icache_l1_sram_resp_latency = icache_l1_sram_resp_latency + 1;
	 end
	 if ((icache_f2_sram_req_valid_ready == 0) && (icache_f0_sram_resp_valid_ready == 1)) begin
		latency = icache_l1_sram_resp_latency;
		icache_l1_sram_resp_latency = 0;
//	icache_l1_sram_resp_latency <= 0;
		done = 2;
		done <= 0;
	 end

	//L2 miss req latency
	 if ((f0_l2_miss_req_valid_ready == 1) && (f0_l2_miss_resp_valid_ready == 0)) begin
	 	f0_l2_miss_latency = f0_l2_miss_latency + 1;
 		d = 1;
 	 end
	 if ((f0_l2_miss_req_valid_ready == 0) && (f0_l2_miss_resp_valid_ready == 0) && d == 1) begin
		f0_l2_miss_latency = f0_l2_miss_latency + 1;
	 end
	 if ((f0_l2_miss_req_valid_ready == 0) && (f0_l2_miss_resp_valid_ready == 1)) begin
		lat = f0_l2_miss_latency;
		f0_l2_miss_latency = 0;
//	f0_l2_miss_latency <= 0;
		d = 2;
		d <= 0;
	 end

	//Fill done pulse latency
	 if (f4_resp_valid_miss_0 == 1) begin
	 	fill_done_latency_0 = fill_done_latency_0 + 1;
 		fill_done_0 = 1;
 	 end
	 if ((f4_resp_valid_miss_0 == 0) && fill_done_0 == 1) begin
  	        fill_done_latency_0 <= 0;
		fill_done_0 = 2;
		fill_done_0 <= 0;
	 end

	 if (f4_resp_valid_miss_1 == 1) begin
	 	fill_done_latency_1 = fill_done_latency_1 + 1;
 		fill_done_1 = 1;
 	 end
	 if ((f4_resp_valid_miss_1 == 0) && fill_done_1 == 1) begin
  	        fill_done_latency_1 <= 0;
		fill_done_1 = 2;
		fill_done_1 <= 0;
	 end

         //STANDALONE_MINION section
         standalone_minion_cov.shire_id_cg.sample(standalone_minion.shire_id);
         standalone_minion_cov.standalone_minion_id_cg.sample(standalone_minion.standalone_minion_id);
         standalone_minion_cov.standalone_minion_threads_enable_cg.sample(standalone_minion.esr_thread0_enable, standalone_minion.esr_thread1_enable);
         standalone_minion_cov.standalone_minion_minion_features_cg.sample(standalone_minion.esr_minion_features);
         standalone_minion_cov.standalone_minion_shire_coop_mode_cg.sample(standalone_minion.esr_shire_coop_mode);
         standalone_minion_cov.standalone_minion_int_mtip_cg.sample(standalone_minion.int_mtip);
         standalone_minion_cov.standalone_minion_int_meip_cg.sample(standalone_minion.int_meip);
         standalone_minion_cov.standalone_minion_int_seip_cg.sample(standalone_minion.int_seip);
         standalone_minion_cov.standalone_minion_ipi_msip_cg.sample(standalone_minion.ipi_msip);
         standalone_minion_cov.standalone_minion_ipi_redirect_trigger_cg.sample(standalone_minion.ipi_redirect_trigger);
         standalone_minion_cov.standalone_minion_uc_to_standalone_minion_fcc_cg.sample(standalone_minion.uc_to_standalone_minion_fcc);
         standalone_minion_cov.standalone_minion_uc_to_standalone_minion_fcc_target_cg.sample(standalone_minion.uc_to_standalone_minion_fcc_target);
         standalone_minion_cov.standalone_minion_icache_sram_req_cg.sample(standalone_minion.icache_f2_sram_req_valid, standalone_minion.icache_f2_sram_req_ready, standalone_minion.icache_f2_sram_req_write, standalone_minion.icache_f2_sram_req_addr);
         standalone_minion_cov.standalone_minion_min_etl_miss_req_cg.sample(standalone_minion.l2_dcache_miss_req_valid, standalone_minion.l2_dcache_miss_req_ready);
         standalone_minion_cov.standalone_minion_min_etl_evict_req_cg.sample(standalone_minion.l2_dcache_evict_req_valid, standalone_minion.l2_dcache_evict_req_ready);
         standalone_minion_cov.standalone_minion_icache_etl_req_cg.sample(standalone_minion.channel.icache_l2_miss_req_valid, standalone_minion.channel.icache_l2_miss_req_ready);
         standalone_minion_cov.standalone_minion_ptw_etl_req_cg.sample(standalone_minion.channel.ptw_l2_miss_req_valid, standalone_minion.channel.ptw_l2_miss_req_ready);
         standalone_minion_cov.standalone_minion_tbox_etl_req_cg.sample(standalone_minion.channel.tbox_miss_req_valid, standalone_minion.channel.tbox_miss_req_ready);
         standalone_minion_cov.standalone_minion_coop_tload_etl_req_cg.sample(standalone_minion.channel.coop_tload_miss_req_valid, standalone_minion.channel.coop_tload_miss_req_ready);
         standalone_minion_cov.standalone_minion_coop_tstore_etl_req_cg.sample(standalone_minion.channel.tstore_evict_valid, standalone_minion.channel.tstore_miss_req_ready);
         standalone_minion_cov.standalone_minion_etl_req_goes_to_int_fifo_cg.sample(standalone_minion.channel.miss_req_goes_to_int_fifo, standalone_minion.channel.miss_req_valid_ff, standalone_minion.channel.miss_req_ready, standalone_minion.channel.miss_req_goes_to_tbox);
         standalone_minion_cov.standalone_minion_etl_req_goes_to_tbox_fifo_cg.sample(standalone_minion.channel.min_tbox_req_valid, standalone_minion.channel.min_tbox_req_ready);
         standalone_minion_cov.standalone_minion_etl_req_goes_to_bank_fifo_cg.sample(signal_bank_req_info_push);
         standalone_minion_cov.standalone_minion_etl_req_goes_to_uc_fifo_cg.sample(standalone_minion.channel.uc_req_info_push);
         standalone_minion_cov.standalone_minion_etl_req_to_sc_cg.sample(standalone_minion.channel.standalone_minion_sc_req_valid, standalone_minion.channel.standalone_minion_sc_req_ready);
         standalone_minion_cov.standalone_minion_min_etl_rsp_cg.sample(standalone_minion.channel.fill_fifo_min_valid, standalone_minion.channel.fill_fifo_min_ready);
         standalone_minion_cov.standalone_minion_icache_etl_rsp_cg.sample(standalone_minion.channel.icache_l2_miss_resp_valid, standalone_minion.channel.icache_l2_miss_resp_ready);
         standalone_minion_cov.standalone_minion_ptw_etl_rsp_cg.sample(standalone_minion.channel.ptw_l2_miss_resp_valid, standalone_minion.channel.ptw_l2_miss_resp_ready);
         standalone_minion_cov.standalone_minion_tbox_etl_rsp_cg.sample(standalone_minion.channel.sc_tbox_rsp_valid, standalone_minion.channel.sc_tbox_rsp_ready);
         standalone_minion_cov.standalone_minion_coop_tstore_etl_rsp_cg.sample(standalone_minion.channel.tstore_min_resp_valid, standalone_minion.channel.tstore_min_resp_ready);
         standalone_minion_cov.standalone_minion_etl_rsp_from_sc_cg.sample(standalone_minion.channel.standalone_minion_sc_rsp_valid, standalone_minion.channel.standalone_minion_sc_rsp_ready);
         standalone_minion_cov.standalone_minion_etl_msg_rsp_from_tbox_cg.sample(standalone_minion.channel.tbox_min_rsp_valid_in, standalone_minion.channel.tbox_min_rsp_ready_in);
         standalone_minion_cov.standalone_minion_min_flb_req_cg.sample(standalone_minion.channel.flb_core_standalone_minion_req_valid);
         standalone_minion_cov.standalone_minion_flb_req_cg.sample(standalone_minion.flb_standalone_minion_l2_req_valid);
         standalone_minion_cov.standalone_minion_min_flb_resp_cg.sample(standalone_minion.channel.flb_standalone_minion_core_resp_valid);
         standalone_minion_cov.standalone_minion_flb_resp_cg.sample(standalone_minion.flb_l2_standalone_minion_resp_valid);
         standalone_minion_cov.standalone_minion_min_coop_miss_req_cg.sample(standalone_minion.channel.min_coop_miss_req_valid, standalone_minion.channel.min_coop_miss_req_ready);
//         standalone_minion_cov.standalone_minion_coop_tload_master_standalone_minion_cg.sample(standalone_minion.channel.coop_tload.master_standalone_minion);
         standalone_minion_cov.standalone_minion_coop_tload_slv_rdy_out_cg.sample(standalone_minion.coop_tload_slv_rdy_out_valid);
         standalone_minion_cov.standalone_minion_coop_tload_slv_rdy_in_cg.sample(standalone_minion.coop_tload_slv_rdy_in_valid);
         standalone_minion_cov.standalone_minion_coop_tload_mst_done_out_cg.sample(standalone_minion.coop_tload_mst_done_out_valid);
         standalone_minion_cov.standalone_minion_coop_tload_mst_done_in_cg.sample(standalone_minion.coop_tload_mst_done_in_valid);
         standalone_minion_cov.standalone_minion_coop_tload_etl_resp_cg.sample(standalone_minion.channel.coop_tload_tag_table_cam_pop);
         standalone_minion_cov.standalone_minion_min_coop_evict_req_cg.sample(standalone_minion.channel.coop_evict_valid, standalone_minion.channel.coop_evict_ready);
         standalone_minion_cov.standalone_minion_coop_store_resp_ack_cg.sample(standalone_minion.channel.l2_dcache_resp_tstore_ack);
         standalone_minion_cov.standalone_minion_min_evict_req_size_cg.sample(standalone_minion.channel.l2_dcache_evict_req, standalone_minion.channel.l2_dcache_evict_req_valid, standalone_minion.channel.l2_dcache_evict_req_ready);
//         standalone_minion_cov.standalone_minion_min_miss_req_size_cg.sample(standalone_minion.channel.l2_dcache_miss_req, standalone_minion.channel.l2_dcache_miss_req_valid, standalone_minion.channel.l2_dcache_miss_req_ready);
         standalone_minion_cov.standalone_minion_min_resp_size_cg.sample(standalone_minion.channel.l2_dcache_resp, standalone_minion.channel.l2_dcache_resp_valid, standalone_minion.channel.l2_dcache_resp_ready);
         //ICACHE section
	 if (fill_done_0 == 2) standalone_minion_cov.icache_resp_fill_done_cg.sample(fill_done_latency_0);
	 if (fill_done_1 == 2) standalone_minion_cov.icache_resp_fill_done_cg.sample(fill_done_latency_1);
	 standalone_minion_cov.icache_flush_cg.sample(standalone_minion.channel.shared_icache.icache.f0_flush_data);
         standalone_minion_cov.icache_l1_sram_req_cg.sample(standalone_minion.channel.shared_icache.icache.f2_sram_req_valid, standalone_minion.channel.shared_icache.icache.f2_sram_req_ready, standalone_minion.channel.shared_icache.icache.f2_sram_req_addr);
	 if (done == 2) standalone_minion_cov.icache_l1_sram_resp_latency_cg.sample(latency);
	 if (d == 2) standalone_minion_cov.icache_l2_miss_resp_latency_cg.sample(f0_l2_miss_latency);
	 standalone_minion_cov.icache_disable_l2_miss_req_cg.sample(standalone_minion.channel.shared_icache.icache.f0_l2_miss_req_valid, signal_f0_l2_miss_req_disable_next);
	 standalone_minion_cov.icache_err_log_ctl_cg.sample(standalone_minion.channel.shared_icache.icache.esr_err_log_ctl);
	 standalone_minion_cov.icache_prefetch_req_cg.sample(standalone_minion.channel.shared_icache.icache.esr_prefetch_conf, standalone_minion.channel.shared_icache.icache.esr_prefetch_start, standalone_minion.channel.shared_icache.icache.esr_prefetch_done);
	 standalone_minion_cov.icache_bypass_cg.sample(standalone_minion.channel.shared_icache.icache.esr_bypass_icache, signal_icache_f2_hit, signal_icache_bypass_en);
      end

      generate
      if (GFX_ENABLED == 1'b1) begin
         always @(posedge clock) begin
            standalone_minion_cov.standalone_minion_etl_msg_req_to_tbox_cg.sample(standalone_minion.channel.tbox_router_gen.tbox_router.min_tbox_rsp_valid, standalone_minion.channel.tbox_router_gen.tbox_router.min_tbox_rsp_ready);
            standalone_minion_cov.standalone_minion_etl_req_from_tbox_cg.sample(standalone_minion.channel.tbox_router_gen.tbox_router.tbox_standalone_minion_req_valid, standalone_minion.channel.tbox_router_gen.tbox_router.tbox_standalone_minion_req_ready);
            standalone_minion_cov.standalone_minion_etl_rsp_to_tbox_cg.sample(standalone_minion.channel.tbox_router_gen.tbox_router.standalone_minion_tbox_rsp_valid, standalone_minion.channel.tbox_router_gen.tbox_router.standalone_minion_tbox_rsp_ready);
         end
      end
      endgenerate

      generate
      for (genvar i = 0; i < 8; i++) begin 
            for (genvar j = 0; j < 2; j++) begin 
	          always @(posedge clock) begin
		  standalone_minion_cov.standalone_minion_coop_tload_master_standalone_minion_cg.sample(j, standalone_minion.channel.coop_tload.master_standalone_minion[i][j]);
	          end
	    end
      end
      endgenerate

      generate
      for (genvar i = 0; i < 8; i++) begin 
            for (genvar j = 0; j < 2; j++) begin 
	          always @(posedge clock) begin
		  standalone_minion_cov.standalone_minion_coop_tload_mask_cg.sample(standalone_minion.channel.coop_tload.MINIONS[i].MISS_FF[j].local_coop_min_mask, standalone_minion.channel.coop_tload.MINIONS[i].MISS_FF[j].local_coop_standalone_minion_mask);
	          end
	    end
      end
      endgenerate

      generate
      for (genvar i = 0; i < 8; i++) begin 
            for (genvar j = 0; j < 2; j++) begin 
	          always @(posedge clock) begin
		  if(standalone_minion.channel.l2_dcache_miss_req_valid[i][j] && standalone_minion.channel.l2_dcache_miss_req_ready[i][j])
         	  standalone_minion_cov.standalone_minion_min_miss_req_size_cg.sample(standalone_minion.channel.l2_dcache_miss_req[i].size);
	          end
	    end
      end
      endgenerate

   `endif   // ifdef EVL_COVERAGE_ON
*************/



/***********
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
      if(|standalone_minion.standalone_minion_sc_req_valid)
         id_to_addr_next[standalone_minion.standalone_minion_sc_req_info.source][standalone_minion.standalone_minion_sc_req_info.id] = standalone_minion.standalone_minion_sc_req_info.address;
   end

   // Actual event dumping
   always_ff @ (negedge clock)
   begin
      if (~reset_standalone_minion && dreams) begin
         // Writing to SC
         for(integer bank = 0; bank < 5; bank++)
            if(standalone_minion.standalone_minion_sc_req_valid[bank])
               $display("EVENT %d 100 %d %d %d %h %h %h", $time, (standalone_minion.shire_id << 2) + STANDALONE_MINION_ID, bank, standalone_minion.standalone_minion_sc_req_ready[bank], standalone_minion.standalone_minion_sc_req_info.id, standalone_minion.standalone_minion_sc_req_info.address, standalone_minion.standalone_minion_sc_req_info.opcode);

         // Responses from SC
         if(standalone_minion.channel.standalone_minion_sc_rsp_valid)
            $display("EVENT %d 101 %d %d %h %h %h", $time, (standalone_minion.shire_id << 2) + STANDALONE_MINION_ID, standalone_minion.channel.standalone_minion_sc_rsp_ready, standalone_minion.channel.standalone_minion_sc_rsp_info.dest, standalone_minion.channel.standalone_minion_sc_rsp_info.id, id_to_addr[standalone_minion.channel.standalone_minion_sc_rsp_info.dest][standalone_minion.channel.standalone_minion_sc_rsp_info.id]);

         // Fill FIFO selects
         if(|standalone_minion.channel.fill_fifo.out_valid_next)
            $display("EVENT %d 102 %d %h %h %h", $time, (standalone_minion.shire_id << 2) + STANDALONE_MINION_ID, standalone_minion.channel.fill_fifo.out_data_next.rsp.dest, standalone_minion.channel.fill_fifo.out_data_next.rsp.id, id_to_addr[standalone_minion.channel.fill_fifo.out_data_next.rsp.dest][standalone_minion.channel.fill_fifo.out_data_next.rsp.id]);
      end
   end
*************/



////`include "dv/standalone_minion_common/cov/standalone_minion_assert.svh"  // FUTURE ... ???


endmodule : evl_standalone_minion_interface
