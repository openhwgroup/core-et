`include "soc.vh"


// Shire channel. It contains the L2, uncached blocks, ESRs and arbiters/glue logic
module shire_channel_stub #(
    parameter int       NUM_NEIGH                  = `NUM_NEIGH,
    parameter int       NUM_RBOX                   = `RBOX_PER_SHIRE,
    parameter int       NUM_BANKS                  = `SC_BANKS,
    parameter int       NUM_L3_MASTER_PORTS        = `SC_L3_MASTER_PORTS,
    parameter int       NUM_L3_SLAVE_PORTS         = `SC_L3_SLAVE_PORTS,
    parameter int       NUM_UC                     = 1,
    parameter bit [7:0] MINION_SHIRE_NEIGH_DISABLE = {8{1'b0}},
    parameter bit [7:0] MINION_SHIRE_NEIGH_STUB    = {8{1'b0}}
)
(
    input  logic                                                 shire_clock,
    input  logic                                                 reset_c,
    input  logic                                                 reset_w,
    input  logic                                                 reset_d,
    input  logic [NUM_NEIGH-1:0]                                 reset_w_icache,

    output logic [NUM_NEIGH-1:0]                                 reset_c_shire_scs,
    output logic                                                 reset_sc,
    output logic                                                 reset_rbox,

    // IPI
    output logic [NUM_NEIGH-1:0][`MIN_PER_N-1:0]                 uc_to_neigh_fcc,
    output logic [NUM_NEIGH-1:0][`UC_FCC_TARGET_SIZE-1:0]        uc_to_neigh_fcc_target,
    output logic [`THREADS_PER_S-1:0]                            esr_ipi_trigger,
    output logic [`THREADS_PER_S-1:0]                            esr_ipi_redirect_trigger,

    //Pwr Ctrl
    output logic [NUM_NEIGH-1:0]                                 esr_pwr_ctrl_glb_nsleepin_op,
    output logic [NUM_NEIGH-1:0]                                 esr_pwr_ctrl_glb_iso_op,
    output logic [NUM_NEIGH*`MIN_PER_N-1:0]                      esr_pwr_ctrl_neigh_nsleepin_op,
    output logic [NUM_NEIGH*`MIN_PER_N-1:0]                      esr_pwr_ctrl_neigh_iso_op,

    output logic [`MIN_PER_S-1:0]                                esr_thread0_enable,
    output logic [`MIN_PER_S-1:0]                                esr_thread1_enable,
    output logic [NUM_NEIGH-1:0][`ESR_MINION_FEATURE_SZ-1:0]     esr_minion_features,
    output icache_prefetch_conf_t  [NUM_NEIGH-1:0]               esr_icache_prefetch_conf,
    output logic [`ICACHE_PER_SHIRE-1:0]                         esr_icache_prefetch_start,
    output logic                                                 ioshire_log_err_int,
    output logic [NUM_NEIGH-1:0]                                 esr_shire_coop_mode,
    // APB signals to ESR

    // Mtime
    output logic [`MIN_PER_S-1:0]                                esr_mtime_local_target_op,

    output debug_axi_port_t                                      shire_channel_debug_axi_port,
    output esr_and_or_tree_L2_t                                  debug_and_or_tree_L2_out,

    output bpam_run_control_neigh_t [NUM_NEIGH-1:0]              bpam_run_control_neigh_op,

    // ESRs to minions/neighborhoods
    output logic [NUM_NEIGH-1:0][`NUM_SHIRE_IDS_R]               shire_id,
    output logic [`TBOX_PER_SHIRE-1:0][`TBOX_PER_SHIRE_R]        tbox_id,
    output logic [`TBOX_PER_SHIRE-1:0]                           tbox_en,

    // Neigh miss request bus
    input  logic [NUM_NEIGH-1:0][(NUM_BANKS+NUM_UC)-1:0]         neigh_sc_req_valid,
    input  et_link_req_info_t  [NUM_NEIGH-1:0]                   neigh_sc_req_info,

    // Fill/Ack bus
    input  wire                         [NUM_NEIGH-1:0]          neigh_sc_rsp_ready,
    output `EVL_WIRE et_link_rsp_info_t [NUM_NEIGH-1:0]          neigh_sc_rsp_info,
    output wire                         [NUM_NEIGH-1:0]          neigh_sc_rsp_valid,

    // Fast Local Barrier
    input  logic [NUM_NEIGH-1:0]                                 flb_neigh_l2_req_valid,
    input  logic [NUM_NEIGH-1:0][`CSR_FL_BARRIER_RANGE]          flb_neigh_l2_req_data,
    output logic [NUM_NEIGH-1:0]                                 flb_l2_neigh_resp_valid,
    output logic [NUM_NEIGH-1:0]                                 flb_l2_neigh_resp_data,

    output Neigh_ET_Link_us_monitor_t  [NUM_NEIGH+NUM_RBOX-1:0]  neigh_et_link_monitor,
    output rbox_dbg_us_monitor_t  [NUM_RBOX-1:0]                 rbox_dbg_sig,


    //PLL
    output esr_pll_auto_config_t                                 esr_pll_ctrl_op,
    output logic [`SHIRE_PLL_ESR_CONF_BITS-1:0]                  esp_pll_conf_op,

    //DLL DELAY Estimation
    output esr_clk_dly_ctl_t                                     esr_clk_dly_ctl_op,
    output esr_dll_dly_est_ctl_t                                 esr_dll_dly_est_ctl_op,
    input esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_0_ip,
    input esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_1_ip,
    input esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_2_ip,
    input esr_dll_dly_est_sts_t                                  esr_dll_dly_est_sts_3_ip,


    output esr_clkmux_ctl_t                                      esr_shire_ctrl_clockmux_op,
    output esr_clk_gate_ctrl_t                                   esr_clk_gate_ctrl_op,
    output logic                                                 esr_debug_clk_gate_ctrl_op,

    //DLL
    output logic [`SHIRE_DLL_ESR_CTRL_BITS-1:0]                  esr_dll_ctrl_op,
    output logic [`SHIRE_DLL_ESR_CONF_BITS-1:0]                  esp_dll_conf_op,
    
    //Minion memory write/read_en override 
    output logic [`SHIRE_MINION_MEM_OVERRIDE_BITS-1:0]           esr_minion_mem_override,

    //Shire Bus Master ports
    output logic                                                 sbm_enable_read,
    output logic                                                 sbm_enable_write,
    output logic                                                 sbm_sys_mesh_slave_axi_BREADY,
    output logic                                                 sbm_sys_mesh_slave_axi_RREADY,

    // Neighborhood - Icache L1 SRAM interface
    input  logic [NUM_NEIGH-1:0]                                 icache_f2_sram_req_write,
    input  logic [NUM_NEIGH-1:0][`ICACHE_SRAM_ADD_WIDTH-1:0]     icache_f2_sram_req_addr,
    input  logic [NUM_NEIGH-1:0]                                 icache_f2_sram_req_valid,
    output logic [NUM_NEIGH-1:0]                                 icache_f2_sram_req_ready,
    output logic [NUM_NEIGH-1:0][`ICACHE_SRAM_DATA_WIDTH-1:0]    icache_f0_sram_resp_dout,
    output logic [NUM_NEIGH-1:0]                                 icache_f0_sram_resp_valid,
    input  logic [NUM_NEIGH-1:0]                                 icache_f0_sram_resp_ready,

    output logic                                                 dft__tdr_tdo_sc,
    output logic                                                 use_shire_tdr_clk_cntl,
    output logic                                                 use_shire_tdr_reset_cntl,
    output shire_tdr_clk_cntl_t                                  shire_tdr_clk_cntl,
    output shire_tdr_reset_cntl_t                                shire_tdr_reset_cntl
);

   wire  unused_ok = &{ 1'b0,
                        flb_neigh_l2_req_valid,
                        1'b0 };

   // Tie some signal normally driven by shire channel
   assign esr_ipi_trigger                  = {`THREADS_PER_S{1'b0}};
   assign esr_ipi_redirect_trigger         = {`THREADS_PER_S{1'b0}};
   assign esr_pwr_ctrl_glb_nsleepin_op     = {NUM_NEIGH{1'b0}};
   assign esr_pwr_ctrl_glb_iso_op          = {NUM_NEIGH{1'b0}};
   assign esr_pwr_ctrl_neigh_nsleepin_op   = {(NUM_NEIGH*`MIN_PER_N){1'b0}};
   assign esr_pwr_ctrl_neigh_iso_op        = {(NUM_NEIGH*`MIN_PER_N){1'b0}};
   assign neigh_sc_rsp_valid               = {NUM_NEIGH{1'bz}};

   //reset_c/d/w_shire assignments in case of STUBBING
   always_comb
   begin
      for (int ii = 0; ii < NUM_NEIGH; ii++) begin
         reset_c_shire_scs[ii] = reset_c | (MINION_SHIRE_NEIGH_DISABLE[ii] | MINION_SHIRE_NEIGH_STUB[ii]);
      end
   end


   //
   // Threads enable
   //
   logic [`MIN_PER_S-1:0] tb_esr_t0_en = {(`MIN_PER_S){1'b1}};
   logic [`MIN_PER_S-1:0] tb_esr_t1_en = {(`MIN_PER_S){1'b0}};
   bit   [`MIN_PER_S-1:0] minion_mask;
   bit   [1:0]            thread_mask;

   initial begin
      if (!`et_value_plusargs("MINION_MASK=0x%h", minion_mask)) minion_mask = {{`MIN_PER_S-1{1'b0}}, 1'b1};
      if (!`et_value_plusargs("THREAD_MASK=0x%h", thread_mask)) thread_mask = 2'b01;

      foreach (tb_esr_t0_en[i]) begin
         tb_esr_t0_en[i] = minion_mask[i] & thread_mask[0];
         tb_esr_t1_en[i] = minion_mask[i] & thread_mask[1];
      end

      $display ("NEIGH_TB: Minion mask %32x", minion_mask);
      $display ("NEIGH_TB: Thread mask T0=%x  T1=%x", tb_esr_t0_en, tb_esr_t1_en);

      esr_thread0_enable = tb_esr_t0_en;
      esr_thread1_enable = tb_esr_t1_en;
   end

   //
   // Minion features
   //
   logic disable_lock_unlock;
   logic disable_multithreading;
   logic trap_on_u_scp;
   logic trap_on_u_cacheops;
   logic trap_on_ml;
   logic trap_on_gfx;
   initial begin
      if (!`et_value_plusargs("disable_lock_unlock=%d",    disable_lock_unlock    )) disable_lock_unlock    = 1'b0;
      if (!`et_value_plusargs("disable_multithreading=%d", disable_multithreading )) disable_multithreading = 1'b0;
      if (!`et_value_plusargs("trap_on_u_scp=%d",          trap_on_u_scp          )) trap_on_u_scp          = 1'b0;
      if (!`et_value_plusargs("trap_on_u_cacheops=%d",     trap_on_u_cacheops     )) trap_on_u_cacheops     = 1'b0;
      if (!`et_value_plusargs("trap_on_ml=%d",             trap_on_ml             )) trap_on_ml             = 1'b0;
      if (!`et_value_plusargs("trap_on_gfx=%d",            trap_on_gfx            )) trap_on_gfx            = 1'b1;
      esr_minion_features = {NUM_NEIGH{disable_lock_unlock, disable_multithreading, trap_on_u_scp, trap_on_u_cacheops, trap_on_ml, trap_on_gfx}};
      $display("NEIGH_TB: esr_minion_features = 0x%0x", esr_minion_features);
   end

   //
   // Clock Gating control
   //
   initial begin
      logic [NUM_NEIGH-1:0] neighs_clock_disable;
      logic min_frontend_clock_gate_disable;
      logic rbox_clock_gate_disable;
      logic min_dcache_clock_gate_disable;
      logic min_vputrans_clock_gate_disable;
      logic min_vputima_clock_gate_disable;
      logic min_vpulane_clock_gate_disable;
      logic min_intpipe_clock_gate_disable;
      logic clk_gate_random;

      clk_gate_random = (`et_test_plusargs("clk_gate_random")) ? 1'b1 : 1'b0;

      if (!`et_value_plusargs("neighs_clock_disable=%d",            neighs_clock_disable            )) neighs_clock_disable            = {NUM_NEIGH{1'b0}};
      if (!`et_value_plusargs("min_frontend_clock_gate_disable=%d", min_frontend_clock_gate_disable )) min_frontend_clock_gate_disable = clk_gate_random ? $urandom_range(1) : 1'b0;
      if (!`et_value_plusargs("rbox_clock_gate_disable=%d",         rbox_clock_gate_disable         )) rbox_clock_gate_disable         = clk_gate_random ? $urandom_range(1) : 1'b0;
      if (!`et_value_plusargs("min_dcache_clock_gate_disable=%d",   min_dcache_clock_gate_disable   )) min_dcache_clock_gate_disable   = clk_gate_random ? $urandom_range(1) : 1'b0;
      if (!`et_value_plusargs("min_vputrans_clock_gate_disable=%d", min_vputrans_clock_gate_disable )) min_vputrans_clock_gate_disable = clk_gate_random ? $urandom_range(1) : 1'b0;
      if (!`et_value_plusargs("min_vputima_clock_gate_disable=%d",  min_vputima_clock_gate_disable  )) min_vputima_clock_gate_disable  = clk_gate_random ? $urandom_range(1) : 1'b0;
      if (!`et_value_plusargs("min_vpulane_clock_gate_disable=%d",  min_vpulane_clock_gate_disable  )) min_vpulane_clock_gate_disable  = clk_gate_random ? $urandom_range(1) : 1'b0;
      if (!`et_value_plusargs("min_intpipe_clock_gate_disable=%d",  min_intpipe_clock_gate_disable  )) min_intpipe_clock_gate_disable  = clk_gate_random ? $urandom_range(1) : 1'b0;
      esr_clk_gate_ctrl_op = {neighs_clock_disable, min_frontend_clock_gate_disable, rbox_clock_gate_disable, min_dcache_clock_gate_disable, min_vputrans_clock_gate_disable, min_vputima_clock_gate_disable, min_vpulane_clock_gate_disable, min_intpipe_clock_gate_disable};
      $display("NEIGH_TB: esr_clk_gate_ctrl_op = 0x%0x", esr_clk_gate_ctrl_op);
   end

   assign shire_id                     = '0;
   assign esr_icache_prefetch_conf     = '0;
   assign esr_icache_prefetch_start    = '0;
   assign esr_debug_clk_gate_ctrl_op   = '0;

   esr_shire_cache_ram_cfg_t  esr_shire_cache_ram_cfg;
   assign esr_shire_cache_ram_cfg.dft__scan_dump_cntl = '0;
   assign esr_shire_cache_ram_cfg.dft__ram_rei = 1'b0;
   assign esr_shire_cache_ram_cfg.dft__ram_wei = 1'b0;
   assign esr_shire_cache_ram_cfg.cfg1         = `ESR_SHIRE_CACHE_RAM_CFG1_RESET_VAL;
   assign esr_shire_cache_ram_cfg.cfg2         = `ESR_SHIRE_CACHE_RAM_CFG2_RESET_VAL;
   assign esr_shire_cache_ram_cfg.cfg3         = `ESR_SHIRE_CACHE_RAM_CFG3_RESET_VAL;
   assign esr_shire_cache_ram_cfg.cfg4         = `ESR_SHIRE_CACHE_RAM_CFG4_RESET_VAL;

   assign flb_l2_neigh_resp_valid            = '0;
   assign flb_l2_neigh_resp_data             = '0;
   assign esr_pll_ctrl_op.lock_reset_disable = 1'b0;
   assign esr_shire_ctrl_clockmux_op         = 4'b0011;
   assign esr_minion_mem_override            = '0;
   assign bpam_run_control_neigh_op          = '0;

   assign esp_dll_conf_op                    = '0;
   assign esr_dll_ctrl_op                    = '0;
   assign esp_pll_conf_op                    = '0;

   assign esr_clk_dly_ctl_op                = '0;
   assign esr_dll_dly_est_ctl_op            = '0;
 
   assign debug_and_or_tree_L2_out          = '0;
   assign shire_channel_debug_axi_port      = '0;
   assign neigh_et_link_monitor             = '0;
   assign rbox_dbg_sig                      = '0;

   assign ioshire_log_err_int               = '0;
   assign reset_rbox                        = '0;
   assign reset_sc                          = '0;

   assign sbm_enable_read                   = '0;
   assign sbm_enable_write                  = '0;
   assign sbm_sys_mesh_slave_axi_BREADY     = '0;
   assign sbm_sys_mesh_slave_axi_RREADY     = '0;
   assign dft__tdr_tdo_sc                   = '0;
   assign use_shire_tdr_clk_cntl            = '0;
   assign use_shire_tdr_reset_cntl          = '0;
   assign shire_tdr_clk_cntl                = '0;
   assign shire_tdr_reset_cntl              = '0;

   assign esr_mtime_local_target_op         = '0;
   assign tbox_en                           = '0;
   assign tbox_id                           = '0;

   wire reset_w_icache_unused_ok = &{reset_w_icache};

   initial begin 
      if (`et_test_plusargs ("shire_coop_mode")) 
          assign esr_shire_coop_mode = '1;
      else
          assign esr_shire_coop_mode = '0;
   end

   uncacheable_fcc_stub #(
       .NUM_UC    (NUM_UC),
       .NUM_BANKS (NUM_BANKS)
   ) fcc_stub_i (
       .clock                  ( shire_clock            ),
       .reset                  ( reset_c                ),
       .neigh_sc_req_info      ( neigh_sc_req_info[0]   ),
       .neigh_sc_req_valid     ( neigh_sc_req_valid[0]  ),
       .uc_to_neigh_fcc        ( uc_to_neigh_fcc       [0]),
       .uc_to_neigh_fcc_target ( uc_to_neigh_fcc_target[0])
   );

   logic [`NUM_NEIGH-1:0] reset_icache;
   assign reset_icache = reset_w_icache | { (4) {reset_w} };

   icache_mems
   icache_mems_i
   (
      // System signals
      .clock                   ( shire_clock               ),
      .reset                   ( reset_icache              ),
      .dft__sram_clock         ( '0                        ),
      .dft__clk_override       ( '0                        ),

      // Neighborhood - Icache L1 SRAM interface
      .icache_req_write        ( icache_f2_sram_req_write  ),
      .icache_req_addr         ( icache_f2_sram_req_addr   ),
      .icache_req_valid        ( icache_f2_sram_req_valid  ),
      .icache_req_ready        ( icache_f2_sram_req_ready  ),
      .icache_resp_dout        ( icache_f0_sram_resp_dout  ),
      .icache_resp_valid       ( icache_f0_sram_resp_valid ),
      .icache_resp_ready       ( icache_f0_sram_resp_ready ),
      // L2 - Neighborhood response
      .neigh_sc_rsp_info       ( neigh_sc_rsp_info         ),
      .neigh_sc_rsp_valid      ( neigh_sc_rsp_valid        ),
      .neigh_sc_rsp_ready      ( neigh_sc_rsp_ready        ),

      // ESRs
      .esr_shire_cache_ram_cfg ( esr_shire_cache_ram_cfg   ),

      // BIST
      .bist_req_info           ( '0                        ),
      .bist_rsp_info           (                           ),
      // APB signals from BPAM for debug
      .apb_paddr               ( '0                        ),
      .apb_pwrite              ( 1'b0                      ),
      .apb_psel                ( 1'b0                      ),
      .apb_penable             ( 1'b0                      ),
      .apb_pwdata              ( '0                        ),
      .apb_pready              (                           ),
      .apb_prdata              (                           ),
      .apb_pslverr             (                           )
   );

endmodule // shire_channel_stub
