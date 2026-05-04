// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"


// This is a shire-neigh hi voltage crossing
module neigh_hi_voltage_cross #(
  parameter NUM_ELEMS = 0
  ) (
  // System signals
  input  logic                               reset_c,
  input  logic                               reset_w,
  input  logic                               clock,

  // Static configuration ESRs
  input  logic [`NUM_SHIRE_IDS_R]            shire_id,
  output logic [`NUM_SHIRE_IDS_R]            shire_id_sync,
  input  logic [`NUM_NEIGH_R]                neigh_id,
  output logic [`NUM_NEIGH_R]                neigh_id_sync,
  input  logic [`TBOX_PER_SHIRE_R]           tbox_id,
  output logic [`TBOX_PER_SHIRE_R]           tbox_id_sync,
  input  logic [`ESR_MINION_FEATURE_SZ-1:0]  esr_minion_features,
  output logic [`ESR_MINION_FEATURE_SZ-1:0]  esr_minion_features_sync,
  input  minion_chicken_bits_vc_t            chicken_bits,
  output minion_chicken_bits_vc_t            chicken_bits_sync,
  input  logic                               esr_shire_coop_mode,
  output logic                               esr_shire_coop_mode_sync,
  // Configuration ESRs
  input  logic                               tbox_en,
  output logic                               tbox_en_sync,
  input  logic [`MIN_PER_N-1:0]              neigh_t0_en,
  output logic [`MIN_PER_N-1:0]              neigh_t0_en_sync,
  input  logic [`MIN_PER_N-1:0]              neigh_t1_en,
  output logic [`MIN_PER_N-1:0]              neigh_t1_en_sync,
  // DMCTRL + BPAM run control
  input  esr_ms_dmctrl_t                     dmctrl,
  output esr_ms_dmctrl_t                     dmctrl_sync,
  // Icache prefetch
  input  icache_prefetch_conf_t              esr_icache_prefetch_conf,
  output icache_prefetch_conf_t              esr_icache_prefetch_conf_sync,
  input  logic                               esr_icache_prefetch_start,
  output logic                               esr_icache_prefetch_start_sync,
  // L2 Response bus
  input  logic                               neigh_sc_rsp_valid,
  output logic                               neigh_sc_rsp_valid_sync,
  input  et_link_rsp_info_t                  neigh_sc_rsp_info,
  output et_link_rsp_info_t                  neigh_sc_rsp_info_sync,
  input  logic                               neigh_sc_rsp_ready,
  output logic                               neigh_sc_rsp_ready_sync,

  // Debug signals for the Status Monitor
  input  logic  [`neigh_sm_gpio_width - 1:0] neigh_sm_gpio,
  output logic  [`neigh_sm_gpio_width - 1:0] neigh_sm_gpio_sync,
  input  logic                               hw_dbg_sm_monitor_enabled,
  output logic                               hw_dbg_sm_monitor_enabled_sync,

  // IPI
  input logic [`UC_FCC_TARGET_SIZE-1:0]      uc_to_neigh_fcc_target,
  output logic [`UC_FCC_TARGET_SIZE-1:0]     uc_to_neigh_fcc_target_sync,
  input logic [`MIN_PER_N-1:0]               uc_to_neigh_fcc,
  output logic [`MIN_PER_N-1:0]              uc_to_neigh_fcc_sync,
  // Interrupts
  input  logic [`THREADS_PER_N-1:0]          int_mtip,
  input  logic [`THREADS_PER_N-1:0]          plic_meip,
  input  logic [`THREADS_PER_N-1:0]          plic_seip,
  output logic [`THREADS_PER_N-1:0]          int_mtip_sync,
  output logic [`THREADS_PER_N-1:0]          plic_meip_sync,
  output logic [`THREADS_PER_N-1:0]          plic_seip_sync,
  // IPI
  input  logic [`THREADS_PER_N-1:0]          esr_to_neigh_ipi_msip,
  output logic [`THREADS_PER_N-1:0]          esr_to_neigh_ipi_msip_sync,
  input  logic [`THREADS_PER_N-1:0]          esr_to_neigh_ipi_trigger,
  output logic [`THREADS_PER_N-1:0]          esr_to_neigh_ipi_trigger_sync,

  // Fast Local Barrier response neigh-L2
  input  logic                               flb_l2_neigh_resp_valid,
  output logic                               flb_l2_neigh_resp_valid_sync,
  input  logic                               flb_l2_neigh_resp_data,
  output logic                               flb_l2_neigh_resp_data_sync,
  //icache SRAM blocks
  input  logic [`ICACHE_SRAM_DATA_WIDTH-1:0] icache_f0_sram_resp_dout,
  output logic [`ICACHE_SRAM_DATA_WIDTH-1:0] icache_f0_sram_resp_dout_sync,
  input  logic                               icache_f0_sram_resp_valid,
  output logic                               icache_f0_sram_resp_valid_sync,
  input  logic                               icache_f0_sram_resp_ready,
  output logic                               icache_f0_sram_resp_ready_sync
  );

  localparam DMCTRL_SZ = $bits(dmctrl);

  ///////////////////////////////////////////////////////////////////////////////
  // icache sram resp fifo wr
  ///////////////////////////////////////////////////////////////////////////////

  logic icache_f0_sram_resp_push;
  logic icache_f0_sram_resp_full;
  logic icache_f0_sram_resp_pop;

  assign icache_f0_sram_resp_push = icache_f0_sram_resp_valid & ~icache_f0_sram_resp_full;
  assign icache_f0_sram_resp_ready_sync = ~icache_f0_sram_resp_full;

  logic icache_sram_resp_vcfifo_wr_hiv_full_next_unused_ok;
  logic icache_sram_resp_vcfifo_wr_hiv_valid_next_unused_ok;
  gen_fifo #(
    .ELEM_SIZE ( $bits(icache_f0_sram_resp_dout) ),
    .NUM_ELEMS ( NUM_ELEMS                       )
  ) icache_sram_resp_vcfifo_wr_hiv (
    // System signals
    .clock      ( clock                          ),
    .reset      ( reset_w                        ),
    // Push data
    .push_early ( 1'b0                           ), // Unused
    .push       ( icache_f0_sram_resp_push       ),
    .push_data  ( icache_f0_sram_resp_dout       ),
    .full       ( icache_f0_sram_resp_full       ),
    .full_next  ( icache_sram_resp_vcfifo_wr_hiv_full_next_unused_ok ), // Unused
    // Pop data
    .pop        ( icache_f0_sram_resp_pop        ),
    .pop_data   ( icache_f0_sram_resp_dout_sync  ),
    .valid      ( icache_f0_sram_resp_valid_sync ),
    .valid_next ( icache_sram_resp_vcfifo_wr_hiv_valid_next_unused_ok )  // Unused
  );

  assign icache_f0_sram_resp_pop = icache_f0_sram_resp_ready & icache_f0_sram_resp_valid_sync;

  ///////////////////////////////////////////////////////////////////////////////
  // neigh sc rsp fifo wr
  ///////////////////////////////////////////////////////////////////////////////
  logic neigh_sc_rsp_push;
  logic neigh_sc_rsp_full;
  logic neigh_sc_rsp_pop;

  assign neigh_sc_rsp_push       = neigh_sc_rsp_valid & ~neigh_sc_rsp_full;
  assign neigh_sc_rsp_ready_sync = ~neigh_sc_rsp_full;

  logic neigh_sc_rsp_vcfifo_wr_hiv_full_next_unused_ok;
  logic neigh_sc_rsp_vcfifo_wr_hiv_valid_next_unused_ok;
  gen_fifo #(
    .ELEM_SIZE ( $bits( neigh_sc_rsp_info )),
    .NUM_ELEMS ( NUM_ELEMS )
  ) neigh_sc_rsp_vcfifo_wr_hiv (
    // System signals
    .clock      ( clock                   ),
    .reset      ( reset_w                 ),
    // Push data
    .push_early ( 1'b0                    ), // Unused
    .push       ( neigh_sc_rsp_push       ),
    .push_data  ( neigh_sc_rsp_info       ),
    .full       ( neigh_sc_rsp_full       ),
    .full_next  ( neigh_sc_rsp_vcfifo_wr_hiv_full_next_unused_ok ), // Unused
    // Pop data
    .pop        ( neigh_sc_rsp_pop        ),
    .pop_data   ( neigh_sc_rsp_info_sync  ),
    .valid      ( neigh_sc_rsp_valid_sync ),
    .valid_next ( neigh_sc_rsp_vcfifo_wr_hiv_valid_next_unused_ok ) // Unused
  );

  assign neigh_sc_rsp_pop = neigh_sc_rsp_ready & neigh_sc_rsp_valid_sync;

  ///////////////////////////////////////////////////////////////////////////////
  // flb l2 neigh resp fifo wr
  ///////////////////////////////////////////////////////////////////////////////

  logic flb_l2_neigh_resp_vcfifo_wr_hiv_full_unused_ok;
  logic flb_l2_neigh_resp_vcfifo_wr_hiv_full_next_unused_ok;
  logic flb_l2_neigh_resp_vcfifo_wr_hiv_valid_next_unused_ok;
  gen_fifo #(
   .ELEM_SIZE ( $bits( flb_l2_neigh_resp_data )),
   .NUM_ELEMS ( NUM_ELEMS )
  ) flb_l2_neigh_resp_vcfifo_wr_hiv (
  // System signals
  .clock      ( clock                        ),
  .reset      ( reset_w                      ),
  // Push data
  .push_early ( 1'b0                         ), // Unused
  .push       ( flb_l2_neigh_resp_valid      ),
  .push_data  ( flb_l2_neigh_resp_data       ),
  .full       ( flb_l2_neigh_resp_vcfifo_wr_hiv_full_unused_ok ), // Unused
  .full_next  ( flb_l2_neigh_resp_vcfifo_wr_hiv_full_next_unused_ok ), // Unused
  // Pop data
  .pop        ( flb_l2_neigh_resp_valid_sync ),
  .pop_data   ( flb_l2_neigh_resp_data_sync  ),
  .valid      ( flb_l2_neigh_resp_valid_sync ),
  .valid_next ( flb_l2_neigh_resp_vcfifo_wr_hiv_valid_next_unused_ok ) // Unused
  );

  ///////////////////////////////////////////////////////////////////////////////
  // interrupts sync reg: mtip, meip, seip
  ///////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////////
  //      * IMPORTANT *                                                        //
  ///////////////////////////////////////////////////////////////////////////////
  // Assumption, these are LEVEL-triggered interrupts.                         //
  // IMPORTANT: Assumption, these are LEVEL-triggered interrupts.              //
  ///////////////////////////////////////////////////////////////////////////////
  `RST_FF (clock, reset_w, int_mtip_sync,  int_mtip,  '0)
  `RST_FF (clock, reset_w, plic_meip_sync, plic_meip, '0)
  `RST_FF (clock, reset_w, plic_seip_sync, plic_seip, '0)
                  
  ///////////////////////////////////////////////////////////////////////////////
  // uc neigh ipi  (msip, ipi_trigger)
  ///////////////////////////////////////////////////////////////////////////////

  //msip
  ///////////////////////////////////////////////////////////////////////////////
  //      * IMPORTANT *                                                        //
  ///////////////////////////////////////////////////////////////////////////////
  // Assumption, msip are LEVEL-triggered interrupts.                          //
  // IMPORTANT: Assumption, these are LEVEL-triggered interrupts.              //
  ///////////////////////////////////////////////////////////////////////////////
  `RST_FF (clock, reset_w, esr_to_neigh_ipi_msip_sync, esr_to_neigh_ipi_msip, '0)

  //ipi_trigger
  //This is a pulse-triggered interrupt, requires a fifo
  logic                      esr_to_neigh_ipi_trigger_push;
  logic [`THREADS_PER_N-1:0] esr_to_neigh_ipi_trigger_pop_data;
  logic                      esr_to_neigh_ipi_trigger_vcfifo_valid;

  // Push logic for HV
  // ---------------------------------------------------------------------
  neigh_hv_logic_ipi neigh_hv_logic_ipi_pd_hv (
    .esr_to_neigh_ipi_trigger      ( esr_to_neigh_ipi_trigger ),
    .esr_to_neigh_ipi_trigger_push ( esr_to_neigh_ipi_trigger_push )
  );

  // ---------------------------------------------------------------------
  logic ipi_trigger_vcfifo_wr_hiv_full_unused_ok;
  logic ipi_trigger_vcfifo_wr_hiv_full_next_unused_ok;
  logic ipi_trigger_vcfifo_wr_hiv_valid_next_unused_ok;
  gen_fifo #(
    .ELEM_SIZE ( $bits( esr_to_neigh_ipi_trigger )),
    .NUM_ELEMS ( NUM_ELEMS )
  ) ipi_trigger_vcfifo_wr_hiv (
    // System signals
    .clock      ( clock                                 ),
    .reset      ( reset_w                               ),
    // Push data
    .push_early ( 1'b0                                  ), // Unused
    .push       ( esr_to_neigh_ipi_trigger_push         ),
    .push_data  ( esr_to_neigh_ipi_trigger              ),
    .full       ( ipi_trigger_vcfifo_wr_hiv_full_unused_ok ), // Unused
    .full_next  ( ipi_trigger_vcfifo_wr_hiv_full_next_unused_ok ), // Unused
    // Pop data
    .pop        ( esr_to_neigh_ipi_trigger_vcfifo_valid ),
    .pop_data   ( esr_to_neigh_ipi_trigger_pop_data     ),
    .valid      ( esr_to_neigh_ipi_trigger_vcfifo_valid ),
    .valid_next ( ipi_trigger_vcfifo_wr_hiv_valid_next_unused_ok )  // Unused
  );

  // Pop logic for LV
  // ---------------------------------------------------------------------
  always_comb begin
    esr_to_neigh_ipi_trigger_sync = '0;
    if (esr_to_neigh_ipi_trigger_vcfifo_valid) begin
      esr_to_neigh_ipi_trigger_sync = esr_to_neigh_ipi_trigger_pop_data;
    end
  end

  ///////////////////////////////////////////////////////////////////////////////
  // uc neigh ipi fifo wr
  ///////////////////////////////////////////////////////////////////////////////
  logic                           neigh_uc_fcc_vcfifo_push;
  logic [`UC_FCC_TARGET_SIZE-1:0] uc_to_neigh_fcc_target_pop;
  logic [`MIN_PER_N-1:0]          uc_to_neigh_fcc_pop;
  logic                           neigh_uc_fcc_vcfifo_valid;

  // Push logic for HV
  // ---------------------------------------------------------------------
  neigh_hv_logic_uc_fcc neigh_hv_logic_uc_fcc_pd_hv (
    .uc_to_neigh_fcc          ( uc_to_neigh_fcc          ),
    .neigh_uc_fcc_vcfifo_push ( neigh_uc_fcc_vcfifo_push )
  );

  // ---------------------------------------------------------------------
  logic neigh_us_fcc_vcfifo_wr_hiv_full_unused_ok;
  logic neigh_us_fcc_vcfifo_wr_hiv_full_next_unused_ok;
  logic neigh_us_fcc_vcfifo_wr_hiv_valid_next_unused_ok;
  gen_fifo #(
    .ELEM_SIZE ( $bits( {uc_to_neigh_fcc,
                         uc_to_neigh_fcc_target} ) ),
    .NUM_ELEMS ( NUM_ELEMS                         )
  ) neigh_us_fcc_vcfifo_wr_hiv (
    // System signals
    .clock      ( clock                        ),
    .reset      ( reset_w                      ),
    // Push data
    .push_early ( 1'b0                         ), // Unused
    .push       ( neigh_uc_fcc_vcfifo_push     ),
    .push_data  ( {uc_to_neigh_fcc,
                   uc_to_neigh_fcc_target}     ),
    .full       ( neigh_us_fcc_vcfifo_wr_hiv_full_unused_ok ),
    .full_next  ( neigh_us_fcc_vcfifo_wr_hiv_full_next_unused_ok ), // Unused
    // Pop data
    .pop        ( neigh_uc_fcc_vcfifo_valid    ),
    .pop_data   ( {uc_to_neigh_fcc_pop,
                   uc_to_neigh_fcc_target_pop} ),
    .valid      ( neigh_uc_fcc_vcfifo_valid    ),
    .valid_next ( neigh_us_fcc_vcfifo_wr_hiv_valid_next_unused_ok )  // Unused
  );

  always_comb begin
    uc_to_neigh_fcc_sync         = '0;
    uc_to_neigh_fcc_target_sync  = '0;
    if (neigh_uc_fcc_vcfifo_valid) begin
      uc_to_neigh_fcc_sync         = uc_to_neigh_fcc_pop;
      uc_to_neigh_fcc_target_sync  = uc_to_neigh_fcc_target_pop;
    end
  end

  ///////////////////////////////////////////////////////////////////////////////
  // Static configuration ESRs sync reg
  ///////////////////////////////////////////////////////////////////////////////
  //   CLK    DOUT                      DIN
  `FF (clock, shire_id_sync,            shire_id)
  `FF (clock, neigh_id_sync,            neigh_id)
  `FF (clock, tbox_id_sync,             tbox_id)
  `FF (clock, esr_minion_features_sync, esr_minion_features)
  `FF (clock, chicken_bits_sync,        chicken_bits)
  `FF (clock, esr_shire_coop_mode_sync, esr_shire_coop_mode)

  ///////////////////////////////////////////////////////////////////////////////
  // Configuration ESRs sync reg
  ///////////////////////////////////////////////////////////////////////////////
  //       CLK    RST      DOUT              DIN          DEF
  `RST_FF (clock, reset_w, tbox_en_sync,     tbox_en,     1'b0)
  `RST_FF (clock, reset_w, neigh_t0_en_sync, neigh_t0_en, '0)
  `RST_FF (clock, reset_w, neigh_t1_en_sync, neigh_t1_en, '0)

  ///////////////////////////////////////////////////////////////////////////////
  // Status Monitor debug signals sync reg
  ///////////////////////////////////////////////////////////////////////////////
  //       CLK    RST      DOUT                            DIN                        DEF
  `RST_FF (clock, reset_w, hw_dbg_sm_monitor_enabled_sync, hw_dbg_sm_monitor_enabled, 1'b0)
  `FF     (clock,          neigh_sm_gpio_sync,             neigh_sm_gpio)

  ///////////////////////////////////////////////////////////////////////////////
  // DMCTRL fifo wr
  ///////////////////////////////////////////////////////////////////////////////
  logic [DMCTRL_SZ-1:0]   dmctrl_pop, dmctrl_pop_ff;
  logic dmctrl_vcfifo_valid;
  logic dmctrl_we; // vcfifo_valid +1 cycle

  // ---------------------------------------------------------------------
  logic dmctrl_vcfifo_wr_hiv_full_unused_ok;
  logic dmctrl_vcfifo_wr_hiv_full_next_unused_ok;
  logic dmctrl_vcfifo_wr_hiv_valid_next_unused_ok;
  gen_fifo #(
    .ELEM_SIZE ( DMCTRL_SZ ),
    .NUM_ELEMS ( NUM_ELEMS )
  ) dmctrl_vcfifo_wr_hiv (
    // System signals
    .clock      ( clock               ),
    .reset      ( reset_c             ),
    // Push data
    .push_early ( 1'b0                ), // Unused
    .push       ( dmctrl.write_enable ),
    .push_data  ( dmctrl              ),
    .full       ( dmctrl_vcfifo_wr_hiv_full_unused_ok ), // Unused
    .full_next  ( dmctrl_vcfifo_wr_hiv_full_next_unused_ok ), // Unused
    // Pop data
    .pop        ( dmctrl_vcfifo_valid ),
    .pop_data   ( dmctrl_pop          ),
    .valid      ( dmctrl_vcfifo_valid ),
    .valid_next ( dmctrl_vcfifo_wr_hiv_valid_next_unused_ok )  // Unused
  );

  // Pop logic for LV
  // ---------------------------------------------------------------------
  //         CLK    RST      EN                   DOUT           DIN         DEF
  `RST_EN_FF(clock, reset_c, dmctrl_vcfifo_valid, dmctrl_pop_ff, dmctrl_pop, '0)
  `FF       (clock,                               dmctrl_we,     dmctrl_vcfifo_valid)

  always_comb begin
    dmctrl_sync = dmctrl_pop_ff;
    dmctrl_sync.write_enable = dmctrl_we;
  end

  ///////////////////////////////////////////////////////////////////////////////
  // icache prefetch fifo wr
  ///////////////////////////////////////////////////////////////////////////////
  logic icache_prefetch_vcfifo_wr_hiv_full_unused_ok;
  logic icache_prefetch_vcfifo_wr_hiv_full_next_unused_ok;
  logic icache_prefetch_vcfifo_wr_hiv_valid_next_unused_ok;
  gen_fifo #(
    .ELEM_SIZE ( $bits(esr_icache_prefetch_conf) ),
    .NUM_ELEMS ( NUM_ELEMS                       )
  ) icache_prefetch_vcfifo_wr_hiv (
    // System signals
    .clock      ( clock                          ),
    .reset      ( reset_w                        ),
    // Push data
    .push_early ( 1'b0                           ), // Unused
    .push       ( esr_icache_prefetch_start      ),
    .push_data  ( esr_icache_prefetch_conf       ),
    .full       ( icache_prefetch_vcfifo_wr_hiv_full_unused_ok ), // Unused
    .full_next  ( icache_prefetch_vcfifo_wr_hiv_full_next_unused_ok ), // Unused
    // Pop data
    .pop        ( esr_icache_prefetch_start_sync ),
    .pop_data   ( esr_icache_prefetch_conf_sync  ),
    .valid      ( esr_icache_prefetch_start_sync ),
    .valid_next ( icache_prefetch_vcfifo_wr_hiv_valid_next_unused_ok )  // Unused
  );

endmodule
