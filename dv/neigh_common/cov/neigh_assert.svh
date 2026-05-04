localparam OUT_FIFOS   = `SC_BANKS+1;
localparam OUT_FIFOS_L = $clog2(OUT_FIFOS);
localparam DEST_FIFOS  = OUT_FIFOS+1;

bit safe_off=0;

initial begin
   if (!`et_value_plusargs("SAFE_OFF=%d", safe_off)) begin
      safe_off=0;
   end
end

//
// Include the isolation signal when resetting assertions
//
wire reset_for_assert = ((reset_neigh | isolate) !== 1'b0) ? 1'b1 : 1'b0;
wire reset_for_assert_isolate_only = ((reset_neigh | !isolate) !== 1'b0) ? 1'b1 : 1'b0;
wire reset_for_assert_safe_isolate_only = ((reset_neigh | !isolate | !safe_off) !== 1'b0) ? 1'b1 : 1'b0;

/*
// Tbox ID
`ASSERT_ALWAYS_MSG(sva_neigh_tbox_id, clock, reset_for_assert, (neigh_top.tbox_id |-> ##[0:8] neigh_top.tbox_id_lo), "Tbox_id input port is not properly connected to tbox_id_lo output port");

// Tbox control ESR
`ASSERT_ALWAYS_MSG(sva_neigh_tbox_control_esr, clock, reset_for_assert, (neigh_top.channel.tbox_control_esr |-> ##[0:1] neigh_top.channel.esr_texture_control_op), "ESR esr_texture_control_op is not properly connected to tbox_control_esr output port");

// Tbox image table ptr ESR
`ASSERT_ALWAYS_MSG(sva_neigh_tbox_img_table_ptr_esr, clock, reset_for_assert, (neigh_top.channel.esr_texture_image_table_ptr_op |-> ##[0:1] neigh_top.channel.tbox_image_table_ptr_esr), "ESR esr_texture_image_table_ptr_op is not properly connected to tbox_image_table_ptr_esr output port");
*/
// Thread0
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_thread0_enable
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_thread0_enable, clock, reset_for_assert, ((neigh_top.esr_thread0_enable[i] === 1'b1) |-> ##[0:8] neigh_top.gen_minions[i].min_inst.minion.enabled[0]), "Esr thread0 is not properly connected to minion1's port enabled[0]");
      end
   end
endgenerate

// Thread 1
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_thread1_enable
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_thread1_enable, clock, reset_for_assert, ((neigh_top.esr_thread1_enable[i] === 1'b1) |-> ##[0:8] neigh_top.gen_minions[i].min_inst.minion.enabled[1]), "Esr thread1 is not properly connected to minion0's port enabled[1]");
      end
   end
endgenerate

// Minion features
property neigh_minion_features(signal1, signal2);
   logic [5:0] target;
   @(posedge clock) disable iff (reset_for_assert)
   (signal1 !== $past(signal1, 1), target = signal1) |-> ##[0:8] signal2 === target;
endproperty

generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
      sva_neigh_minion_features: assert property (neigh_minion_features(neigh_top.esr_minion_features, neigh_top.gen_minions[i].min_inst.minion.esr_features)) else $display("Esr minion features not properly connected to minion's port esr features");
   end
endgenerate

// ESR shire coop mode
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_shire_coop_mode
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_shire_coop_mode, clock, reset_for_assert, (neigh_top.esr_shire_coop_mode |-> ##[0:8] neigh_top.gen_minions[i].min_inst.minion.esr_shire_coop_mode), "Esr shire coop mode not properly connected to minion's port esr shire coop mode");
      end
   end
endgenerate

// Int mtip
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_int_mtip
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_int_mtip, clock, reset_for_assert, ({`CORE_NR_THREADS{neigh_top.int_mtip[i]}} |-> ##[0:9] neigh_top.gen_minions[i].min_inst.minion.interrupts.mtip[1:0]), "Int mtip not properly connected to minion's port interrupts.mtip");
      end
   end
endgenerate

// Int meip
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_int_meip
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_int_meip, clock, reset_for_assert, ({`CORE_NR_THREADS{neigh_top.int_meip}} |-> ##[0:9] neigh_top.gen_minions[i].min_inst.minion.interrupts.meip[1:0]), "Int meip not properly connected to minion's port interrupts.meip");
      end
   end
endgenerate

// Int seip
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_int_seip
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_int_seip, clock, reset_for_assert, ({`CORE_NR_THREADS{neigh_top.int_seip}} |-> ##[0:9] neigh_top.gen_minions[i].min_inst.minion.interrupts.seip[1:0]), "Int seip not properly connected to minion's port interrupts.seip");
      end
   end
endgenerate

// Ipi msip
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_ipi_msip
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_ipi_msip, clock, reset_for_assert, (neigh_top.ipi_msip[(i*2+1)-:2] |-> ##[0:9] neigh_top.gen_minions[i].min_inst.minion.interrupts.msip), "Ipi msip not properly connected to minion's port interrupts.msip");
      end
   end
endgenerate

// Ipi redirect trigger
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_ipi_redirect_trigger
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_ipi_redirect_trigger, clock, reset_for_assert, (neigh_top.ipi_redirect_trigger[(i*2+1)-:2] |-> ##[0:9] neigh_top.gen_minions[i].min_inst.minion.interrupts.ipi_with_pc), "Int ipi_redirect_trigger not properly connected to minion's port interrupts.ipi_with_pc");
      end
   end
endgenerate

// Uc to neigh fcc
property neigh_uc_to_neigh_fcc(signal1, signal2);
   logic [2:0] target;
   @(posedge clock) disable iff (reset_for_assert)
   (( signal1 && (neigh_top.uc_to_neigh_fcc_target < 4)), target = neigh_top.uc_to_neigh_fcc_target) |-> ##[0:9] signal2[target];
endproperty

generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
      sva_neigh_uc_to_neigh_fcc: assert property (neigh_uc_to_neigh_fcc(neigh_top.uc_to_neigh_fcc[i], neigh_top.gen_minions[i].min_inst.minion.interrupts.fcc)) else $display("FCC not properly connected to minion's port interrupts.fcc");
   end
endgenerate

// ESR minion boot op
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_esr_minion_boot
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_esr_minion_boot_1, clock, reset_for_assert, (neigh_top.channel.esrs.esr_minion_boot_op |-> neigh_top.gen_minions[i].min_inst.minion.reset_vector), "ESR esr minion_boot_op not properly connected to minion's port reset_vector");
      end
   end
endgenerate

// ESR mport op
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_esr_min_mprot
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_esr_min_mprot, clock, reset_for_assert, (neigh_top.channel.esrs.esr_mprot_op |-> neigh_top.gen_minions[i].min_inst.minion.mprot), "ESR esr_mprot_op not properly connected to minion's port mprot");
      end
   end
endgenerate


// ESR ipi redirect pc op
property neigh_esr_min_ipi_redirect_pc(signal1, signal2);
   logic [46:0] target;
   @(posedge clock) disable iff (reset_for_assert)
   (signal1 !== $past(signal1, 1), target = signal1) |-> ##[0:1] signal2 === target;
endproperty

generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
      sva_neigh_esr_min_ipi_redirect_pc: assert property (neigh_esr_min_ipi_redirect_pc(neigh_top.channel.esrs.esr_ipi_redirect_pc_op[`PC_SIZE-1:1], neigh_top.gen_minions[i].min_inst.minion.interrupts.ipi_pc)) else $display("ESR esr_ipi_redirect_pc_op not properly connected to minion's port interrupts.ipi_pc");
   end
endgenerate

// ESR min bypass dcache
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_esr_min_bypass_dcache
      if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin
         `ASSERT_ALWAYS_MSG(sva_neigh_esr_min_bypass_dcache, clock, reset_for_assert, (neigh_top.channel.esrs.esr_neigh_chicken_op.bypass_dcache |-> neigh_top.gen_minions[i].min_inst.minion.esr_bypass_dcache), "ESR esr_neigh_chicken_op.bypass_dcache not properly connected to minion's port esr_bypass_dcache");
      end
   end
endgenerate

// Icache prefetch conf
property neigh_icache_prefetch_conf;
   `toVector(icache_prefetch_conf_t) prefetch_conf;
   @(posedge clock) disable iff (reset_for_assert)
   (neigh_top.esr_icache_prefetch_start === 1, prefetch_conf = neigh_top.esr_icache_prefetch_conf) |-> ##[0:8] neigh_top.channel.shared_icache.icache.esr_prefetch_conf === prefetch_conf;
endproperty

sva_neigh_icache_prefetch_conf: assert property (neigh_icache_prefetch_conf) else $display("Esr_icache_prefetch_conf port is not properly connected to icache port esr_prefetch_conf");

// Icache prefetch start
`ASSERT_ALWAYS_MSG(sva_neigh_icache_prefetch_start, clock, reset_for_assert, (neigh_top.esr_icache_prefetch_start |-> ##[0:8] neigh_top.channel.shared_icache.icache.esr_prefetch_start), "Esr_icache_prefetch_start port is not properly connected to icache port esr_prefetch_start");

// Icache prefetch done
`ASSERT_ALWAYS_MSG(sva_neigh_icache_prefetch_done, clock, reset_for_assert, (neigh_top.channel.shared_icache.icache.esr_prefetch_done |-> ##[0:8] neigh_top.esr_icache_prefetch_done), "Esr_icache_prefetch_done port is not properly connected to icache port esr_prefetch_done");

// Icache err detected
`ASSERT_ALWAYS_MSG(sva_neigh_icache_err_detected, clock, reset_for_assert, (neigh_top.channel.esrs.esr_icache_err_detected_op |-> ##[0:8] neigh_top.esr_icache_err_detected), "ESR esr_icache_err_detected_op is not properly connected to output port esr_icache_err_detected");

// Icache err logged
`ASSERT_ALWAYS_MSG(sva_neigh_icache_err_logged, clock, reset_for_assert, (neigh_top.channel.esrs.esr_icache_err_logged_op |-> ##[0:8] neigh_top.esr_icache_err_logged), "ESR esr_icache_err_logged_op is not properly connected to output port esr_icache_err_logged");

// Icache f2 sram req valid ready
`ASSERT_ALWAYS_MSG(sva_neigh_icache_sram_req_valid_ready, clock, reset_for_assert, (((neigh_top.channel.shared_icache.icache.f2_sram_req_ready & neigh_top.channel.shared_icache.icache.f2_sram_req_valid) === 1'b1) |-> ##[0:8] (neigh_top.channel.icache_f2_sram_req_ready & neigh_top.channel.icache_f2_sram_req_valid)), "Icache_f2_sram_req_ready not properly connected to the icache's port f2_sram_req_ready");

// Icache f2 sram req write
`ASSERT_ALWAYS_MSG(sva_neigh_icache_sram_req_write, clock, reset_for_assert, (neigh_top.channel.shared_icache.icache.f2_sram_req_write |-> ##[0:8] neigh_top.channel.icache_f2_sram_req_write), "Icache_f2_sram_req_write not properly connected to the icache's port f2_sram_req_write");

// Icache f2 sram req addr
property neigh_icache_sram_req_addr(signal1, signal2);
   logic[8:0] target;
   @(posedge clock) disable iff (reset_for_assert)
   (signal1 !== $past(signal1, 1) && (!isolate), target = signal1) |-> ##[0:8] signal2 === target;
endproperty

sva_neigh_icache_sram_req_addr: assert property (neigh_icache_sram_req_addr(neigh_top.channel.shared_icache.icache.f2_sram_req_addr, neigh_top.channel.icache_f2_sram_req_addr)) else $display("Icache_f2_sram_req_addr not properly connected to the icache's port f2_sram_req_write");

// Icache f0 sram resp valid
`ASSERT_ALWAYS_MSG(sva_neigh_icache_sram_resp_valid, clock, reset_for_assert, (neigh_top.channel.icache_f0_sram_resp_valid |-> ##[0:8] neigh_top.channel.shared_icache.icache.f0_sram_resp_valid), "icache_f0_sram_resp_valid not properly connected to the icache's port f0_sram_resp_valid");

// Icache f0 sram resp ready
`ASSERT_ALWAYS_MSG(sva_neigh_icache_sram_resp_ready, clock, reset_for_assert, (neigh_top.channel.shared_icache.icache.f0_sram_resp_ready |-> ##[0:8] neigh_top.channel.icache_f0_sram_resp_ready), "Icache_f0_sram_resp_ready not properly connected to the icache's port f0_sram_resp_ready");

// Icache f0 sram resp dout
property neigh_icache_sram_resp_dout(signal1, signal2);
   logic[575:0] target;
   @(posedge clock) disable iff (reset_for_assert)
   (signal1 !== $past(signal1, 1) && (!isolate), target = signal1) |-> ##[0:8] signal2 === target;
endproperty

// sva_neigh_icache_sram_resp_dout: assert property (neigh_icache_sram_resp_dout(neigh_top.channel.shared_icache.icache.f0_sram_resp_dout, neigh_top.channel.icache_f0_sram_resp_dout)) else $display("Icache_f0_sram_resp_dout not properly connected to the icache's port f0_sram_resp_dout");

// ESR icache mprot
`ASSERT_ALWAYS_MSG(sva_neigh_esr_icache_mprot, clock, reset_for_assert, (neigh_top.channel.esrs.esr_mprot_op |-> neigh_top.channel.shared_icache.icache.esr_mprot), "ESR esr_mprot_op not properly connected to the icache's port esr_mprot");



// ESR icache bypass
`ASSERT_ALWAYS_MSG(sva_neigh_esr_icache_bypass, clock, reset_for_assert, (neigh_top.channel.esrs.esr_neigh_chicken_op.bypass_icache |-> neigh_top.channel.shared_icache.icache.esr_bypass_icache), "ESR esr_neigh_chicken_op.bypass_icache not properly connected to icache's port esr_bypass_icache");
/*
// Debug assertions for minions
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
begin

    // Assertion for minion signals
    `ASSERT_ALWAYS_MSG(ERROR_dbg_neigh_minion_sm_signals, clock, neigh_top.channel.reset_d_neigh_ff,
                       ( neigh_top.gen_minions[i].min_inst.minion.minion_dbg_sig_enable & (neigh_top.channel.neigh_ch_dbg.neigh_sm_gpio_ff[10:7] === i ) ) |-> ##`DBG_SIGNALS_NEIGH_MIN_DELAY
                       ( neigh_top.channel.neigh_ch_dbg.neigh_sm_signals === $past(neigh_top.gen_minions[i].min_inst.minion.minion_dbg_signals, `DBG_SIGNALS_NEIGH_MIN_DELAY, , @(posedge clock)) ),
                       $sformatf("[Minion Shire][Neigh Channel]: Status Monitor: The signals at the source and the endpoint does not have the same value. Check the connection between the neighborhood ( %x ) and the selected minion ( %x )",
                                 neigh_top.channel.neigh_ch_dbg.neigh_sm_signals, $past(neigh_top.gen_minions[i].min_inst.minion.minion_dbg_signals, `DBG_SIGNALS_NEIGH_MIN_DELAY, , @(posedge clock))) )

    `ASSERT_ALWAYS_MSG(ERROR_dbg_neigh_minion_sm_enable, clock, neigh_top.channel.reset_d_neigh_ff,
                       ( neigh_top.channel.neigh_ch_dbg.status_monitor_enable & (neigh_top.channel.neigh_ch_dbg.neigh_sm_gpio_ff[10:7] === i ) ) |-> ##`DBG_SIGNALS_NEIGH_MIN_DELAY
                       ( neigh_top.gen_minions[i].min_inst.minion.minion_dbg_sig_enable === 1'b1 ),
                       $sformatf("[Minion Shire][Neigh Channel]: Status Monitor: The enable signal is not asserted properly. Check the connection between the neighborhood and the selected minion"))

    `ASSERT_ALWAYS_MSG(ERROR_dbg_neigh_minion_sm_mux, clock, neigh_top.channel.reset_d_neigh_ff,
                       ( neigh_top.channel.neigh_ch_dbg.status_monitor_enable & (neigh_top.channel.neigh_ch_dbg.neigh_sm_gpio_ff[10:7] === i ) ) |->
                       ( neigh_top.channel.neigh_ch_dbg.neigh_sm_gpio_ff[6:0] === neigh_top.gen_minions[i].min_inst.minion.minion_dbg_signals_mux),
                       $sformatf("[Minion Shire][Neigh Channel]: Status Monitor: The mux signal is not set to the correspondent bits. Check the connection between the neighborhood ( %x ) and the selected minion ( %x )",
                                  neigh_top.channel.neigh_ch_dbg.neigh_sm_gpio_ff[6:0], neigh_top.gen_minions[i].min_inst.minion.minion_dbg_signals_mux ))
end
endgenerate


// Debug assertions for TBOX signals
`ASSERT_ALWAYS_MSG(ERROR_dbg_neigh_tbox_sm_signals, clock, neigh_top.channel.reset_d_neigh_ff,
                  ( neigh_top.channel.neigh_ch_dbg.status_monitor_enable & (neigh_top.channel.neigh_ch_dbg.dbg_target_sel === 8 )) |-> ##`DBG_SIGNALS_NEIGH_TBOX_DELAY
                  ( neigh_top.channel.neigh_ch_dbg.neigh_sm_signals === $past(neigh_top.tbox_dbg_signals, `DBG_SIGNALS_NEIGH_TBOX_DELAY, , @(posedge clock)) ),
                  $sformatf("[Minion Shire][Neigh Channel]: Status Monitor: The signals at the source and the endpoint have not the same value."))


`ASSERT_ALWAYS_MSG(ERROR_dbg_neigh_tbox_sm_enable, clock, neigh_top.channel.reset_d_neigh_ff,
                  ( neigh_top.channel.neigh_ch_dbg.status_monitor_enable & (neigh_top.channel.neigh_ch_dbg.dbg_target_sel === 8 )) |-> ##`DBG_SIGNALS_NEIGH_TBOX_DELAY ( neigh_top.tbox_dbg_signals_en === 1'b1 ),
                  $sformatf("[Minion Shire][Neigh Channel]: Status Monitor: The enable signal is not asserted properly."))


// Debug assertions for neigh channel signals
`ASSERT_ALWAYS_MSG(ERROR_dbg_neigh_channel_sm_signals, clock, neigh_top.channel.reset_d_neigh_ff,
                  ( neigh_top.channel.neigh_ch_dbg.status_monitor_enable & (neigh_top.channel.neigh_ch_dbg.dbg_target_sel >= 9 )) |-> ##`DBG_SIGNALS_NEIGH_CH_DELAY
                  ( neigh_top.channel.neigh_ch_dbg.neigh_sm_signals === $past(neigh_top.channel.neigh_ch_dbg_signals_next, `DBG_SIGNALS_NEIGH_CH_DELAY, , @(posedge clock)) ),
                  $sformatf("[Minion Shire][Neigh Channel]: Status Monitor: The signals at the source and the endpoint have not the same value."))


`ASSERT_ALWAYS_MSG(ERROR_dbg_neigh_channel_sm_enable, clock, neigh_top.channel.reset_d_neigh_ff,
                  ( neigh_top.channel.neigh_ch_dbg.status_monitor_enable & (neigh_top.channel.neigh_ch_dbg.dbg_target_sel >= 9 )) |-> ##`DBG_SIGNALS_NEIGH_CH_DELAY ( neigh_top.channel.neigh_ch_dbg_signals_en === 1'b1 ),
                  $sformatf("[Minion Shire][Neigh Channel]: Status Monitor: The enable signal is not asserted properly."))

// Tbox stay idle FUTURE
//`ASSERT_ALWAYS_MSG(sva_neigh_tbox_stay_idle, clock, reset_for_assert, ((neigh_top.channel.reset_w_tbox === 0) ||
// (neigh_top.channel.min_tbox_req_ready === 1 &&
// neigh_top.channel.tbox_min_rsp_valid === 0 &&
// neigh_top.channel.tbox_miss_req_valid === 0 &&
// neigh_top.channel.sc_tbox_rsp_ready === 1 &&
// neigh_top.channel.neigh_tbox_rsp_valid === 0)),
// "Messages sent to the TBOX while disabled hang the system or cause side effect");
*/
// Miss request arbiter output goes into the intermediate FIFO
`ASSERT_ALWAYS_MSG(sva_neigh_miss_req_to_int_fifo, clock, reset_for_assert, (neigh_top.channel.miss_req_goes_to_int_fifo === 1 |-> (!neigh_top.channel.miss_req_goes_to_tbox &&
   (neigh_top.channel.out_req_fifo_full[neigh_top.channel.miss_req_info_dest_fifo[OUT_FIFOS_L-1:0]] ||
   (neigh_top.channel.int_req_fifo_counter[neigh_top.channel.miss_req_info_dest_fifo[OUT_FIFOS_L-1:0]] !== '0) ||
   neigh_top.channel.int_req_info_pop)) === 1), "Request sent to the intermediate FIFO doesn't meet the rules");
`ifndef IOSHIRE_GATESIM
// Minion coop mask not zero
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin
   for (genvar j = 0; j < 2; j++)
      begin
         `ASSERT_ALWAYS_MSG(sva_min_coop_mask_not_zero, clock, reset_for_assert, (neigh_top.channel.coop_tload.MINIONS[i].MISS_FF[j].local_coop_min_mask !== 0), "Minion coop mask shouldn't be zero");
      end
   end
endgenerate

// Neigh coop mask not zero
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin
   for (genvar j = 0; j < 2; j++)
      begin : sva_neigh_coop_mask_not_zero
         `ASSERT_ALWAYS_MSG(sva_neigh_coop_mask_not_zero, clock, reset_for_assert, (neigh_top.channel.coop_tload.MINIONS[i].MISS_FF[j].local_coop_neigh_mask !== 0), "Neigh coop mask shouldn't be zero");
      end
   end
endgenerate
`endif //  `ifdef IOSHIRE_GATESIM
   
// Cooperative stores invalid combinations are not sent
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_coop_store_conf
      `ASSERT_ALWAYS_MSG(sva_neigh_coop_store_conf, clock, reset_for_assert, (((neigh_top.channel.coop_evict_valid[i] & neigh_top.channel.coop_evict_ready[i]) === 1'b1) |->
         ( ( (neigh_top.channel.coop_evict_req[i].subopcode === 7'd1) &&
             ( ((neigh_top.channel.coop_evict_req[i].size >= ET_LINK_QWord) && (neigh_top.channel.coop_evict_req[i].size <= ET_LINK_HLine)) ||
               ((neigh_top.channel.coop_evict_req[i].size >= ET_LINK_HLine) && (neigh_top.channel.coop_evict_req[i].size <= ET_LINK_Line)) ) ) ||
           ( (neigh_top.channel.coop_evict_req[i].subopcode === 7'd2) &&
             ((neigh_top.channel.coop_evict_req[i].size >= ET_LINK_QWord) && (neigh_top.channel.coop_evict_req[i].size <= ET_LINK_Line)) ) )),
         "Invalid cooperative store combination is sent");
   end
endgenerate

// Master neighborhood check
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin
   for (genvar j = 0; j < 2; j++)
      begin : sva_neigh_coop_tload_master_neigh
         `ASSERT_ALWAYS_MSG(sva_neigh_coop_tload_master_neigh, clock, reset_for_assert, ( ((neigh_top.channel.coop_tload.coop_tload_local_rdy[i][j] === 1'b1) &&
            (neigh_top.channel.coop_tload.master_neigh[i][j] === neigh_top.channel.coop_tload.neigh_id) && (neigh_top.channel.coop_tload.MINIONS[i].MISS_FF[j].mst_all_ext_are_rdy) === 1'b1) |->
            ((neigh_top.channel.coop_tload.master_neigh[i][j] === neigh_top.channel.coop_tload.masterNeigh(neigh_top.channel.coop_tload.local_ff_req[i][j].address, neigh_top.channel.coop_tload.MINIONS[i].MISS_FF[j].local_coop_neigh_mask)) &&
             (neigh_top.channel.coop_tload.MINIONS[i].MISS_FF[j].local_coop_neigh_mask === neigh_top.channel.coop_tload.local_ff_req[i][j].data[`SHIRE_COOP_NEIGH_MASK_START +: `SHIRE_COOP_NEIGH_MASK_SIZE])) ), "Master neigh is not correct");
      end
   end
endgenerate

// Messages between minions go through the fast local network if they meet the requirements
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin
   for (genvar j = 0; j < `NEIGH_MAX_LOCAL_CONNECT; j++)
      begin
         `ASSERT_ALWAYS_MSG(sva_neigh_msg_fln, clock, reset_for_assert, ( (neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.evict_goes_to_local[j] === 1) &&
            (neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.evict_ff_available === 0) |->
            ( (neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.evict_ff_req.opcode === ET_LINK_REQ_MsgSendData) &&
              (neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.evict_ff_req.address[`AD_ESR_REGION_RANGE] === `AD_ESR_REGION) &&
              ( (neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.evict_ff_req.address[`AD_ESR_SHIRE_ID_RANGE] === neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.shire_id) ||
                (neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.evict_ff_req.address[`AD_ESR_SHIRE_ID_RANGE] === `LOCAL_SHIRE_ID) ) &&
              (neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.evict_ff_req.address[`AD_ESR_AGENT_TYPE_RANGE] === `AD_ESR_AGENT_TYPE_MINION) &&
              (neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.evict_ff_req.address[`AD_ESR_SHIRE_MIN_ID_RANGE] ===
                 (`AD_ESR_SHIRE_MIN_ID_SIZE)'({neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.neigh_id, neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.local_dest_id[j]})) &&
              (neigh_top.channel.MINION_FF[i].evict_ff.neigh_evict_ff_regular.evict_ff_req.size <= ET_LINK_HLine) )),
            "Message between minions doesn't meet the requirements");
      end
   end
endgenerate

// Miss request multiplexer enables only one valid
`ASSERT_ALWAYS_MSG(sva_miss_req_mux_more_than_one_valid, clock, reset_for_assert, $onehot0(neigh_top.channel.miss_req_valid_xx), "More than one valid in the miss request multiplexer!")

// Intermediate FIFO counters do not overflow
generate
for (genvar i = 0; i < `SC_BANKS+1; i++)
   begin : sva_int_fifo_cnt_ov
      `ASSERT_ALWAYS_MSG(sva_neigh_int_fifo_cnt_ovf, clock, reset_for_assert, (neigh_top.channel.int_req_fifo_counter[i] <= `NEIGH_INT_FIFO_SIZE), "Intermediate FIFO counter overflow")
   end
endgenerate

// Cooperative TLoad tag table hits one entry
`ASSERT_ALWAYS_MSG(sva_neigh_coop_tload_tag_table_one_hit, clock, reset_for_assert, $onehot(neigh_top.channel.coop_tload.coop_tload_tag_table.cam_hit_array) |
                                                                                    ~neigh_top.channel.coop_tload.coop_tload_tag_table.cam_pop, "More than one tag (or none) had a hit in the coop TLoad tag table!")

// Neigh-SC output request arbiter sets 1-hot ready
`ASSERT_ALWAYS_MSG(sva_out_req_ready_one_hot, clock, reset_for_assert, $onehot0(neigh_top.channel.neigh_hv_logic_neigh_sc_pd_hv.out_req_ready), $sformatf("multi-bit valid/ready (request/grant) handshake should only send 1-hot ready(grant). Arbiter is returning out_req_ready %b", $sampled(neigh_top.channel.neigh_hv_logic_neigh_sc_pd_hv.out_req_ready)))

// Tensor store buffer pending ACK counter do not overflow
generate
for(genvar i = 0; i < `MIN_PER_N/2; i++)
   begin : sva_tensor_store_buffer_pending_ack
      `ASSERT_ALWAYS_MSG(sva_neigh_coop_store_north_pending_ack, clock, reset_for_assert, (neigh_top.channel.tensor_store_buffer.neigh_tensor_store_buffer_north.min_pending_ack[i] < 5'd31), "More than 30 pending ACKs in north tensor_store_buffer")
      `ASSERT_ALWAYS_MSG(sva_neigh_coop_store_south_pending_ack, clock, reset_for_assert, (neigh_top.channel.tensor_store_buffer.neigh_tensor_store_buffer_south.min_pending_ack[i] < 5'd31), "More than 30 pending ACKs in south tensor_store_buffer")
   end
endgenerate

generate
for(genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_gen_minions
      if((STUB_MINIONS[i] === 1'b0) && (DISABLE_MINIONS[i] === 1'b0))
      begin : sva_min_inst
          // Writeback can't be Xs
          `ASSERT_ALWAYS_MSG(sva_writeback_xs, clock, reset_for_assert, neigh_top.gen_minions[i].min_inst.minion.core.intpipe.wb_valid !== 1'bX, "Writeback can't be Xs")
      end
   end
endgenerate

// X propagation on neigh - SC interface
`ASSERT_VALID_PORTS(sva_neigh_sc_req_xs, clock, reset_for_assert, neigh_top.channel.neigh_sc_req_valid, {neigh_top.channel.neigh_sc_req_info.id,
                                                                                                         neigh_top.channel.neigh_sc_req_info.source,
                                                                                                         neigh_top.channel.neigh_sc_req_info.wdata,
                                                                                                         neigh_top.channel.neigh_sc_req_info.opcode,
                                                                                                         neigh_top.channel.neigh_sc_req_info.address,
                                                                                                         neigh_top.channel.neigh_sc_req_info.size,
                                                                                                         neigh_top.channel.neigh_sc_req_info.qwen,
                                                                                                         neigh_top.channel.neigh_sc_req_info.subopcode})
`ASSERT_VALID_PORTS(sva_neigh_sc_rsp_xs, clock, reset_for_assert, neigh_top.channel.neigh_sc_rsp_valid_lo, {neigh_top.channel.neigh_sc_rsp_info_lo.id,
                                                                                                            neigh_top.channel.neigh_sc_rsp_info_lo.dest,
                                                                                                            neigh_top.channel.neigh_sc_rsp_info_lo.wdata,
                                                                                                            neigh_top.channel.neigh_sc_rsp_info_lo.opcode,
                                                                                                            neigh_top.channel.neigh_sc_rsp_info_lo.size,
                                                                                                            neigh_top.channel.neigh_sc_rsp_info_lo.qwen})
// X propagation on neigh - TBOX interface
`ASSERT_VALID_PORTS(sva_tbox_neigh_req_xs, clock, reset_for_assert, neigh_top.channel.tbox_neigh_req_valid, {neigh_top.channel.tbox_neigh_req_info.id,
                                                                                                             neigh_top.channel.tbox_neigh_req_info.source,
                                                                                                             neigh_top.channel.tbox_neigh_req_info.wdata,
                                                                                                             neigh_top.channel.tbox_neigh_req_info.opcode,
                                                                                                             neigh_top.channel.tbox_neigh_req_info.address,
                                                                                                             neigh_top.channel.tbox_neigh_req_info.size,
                                                                                                             neigh_top.channel.tbox_neigh_req_info.qwen,
                                                                                                             neigh_top.channel.tbox_neigh_req_info.subopcode})
`ASSERT_VALID_PORTS(sva_tbox_neigh_rsp_xs, clock, reset_for_assert, neigh_top.channel.tbox_neigh_rsp_valid, {neigh_top.channel.tbox_neigh_rsp_info.id,
                                                                                                             neigh_top.channel.tbox_neigh_rsp_info.dest,
                                                                                                             neigh_top.channel.tbox_neigh_rsp_info.wdata,
                                                                                                             neigh_top.channel.tbox_neigh_rsp_info.opcode,
                                                                                                             neigh_top.channel.tbox_neigh_rsp_info.size,
                                                                                                             neigh_top.channel.tbox_neigh_rsp_info.qwen})
generate
for (genvar i = 0; i < `MIN_PER_N; i++)
   begin : sva_neigh_per_minion
      // X propagation on minion - neigh interface
      `ASSERT_VALID_PORTS(sva_l2_dcache_evict_req_xs, clock, reset_for_assert, neigh_top.channel.l2_dcache_evict_req_valid[i], {neigh_top.channel.l2_dcache_evict_req[i].id,
                                                                                                                                neigh_top.channel.l2_dcache_evict_req[i].source,
                                                                                                                                neigh_top.channel.l2_dcache_evict_req[i].wdata,
                                                                                                                                neigh_top.channel.l2_dcache_evict_req[i].opcode,
                                                                                                                                neigh_top.channel.l2_dcache_evict_req[i].address,
                                                                                                                                neigh_top.channel.l2_dcache_evict_req[i].size,
                                                                                                                                neigh_top.channel.l2_dcache_evict_req[i].qwen,
                                                                                                                                neigh_top.channel.l2_dcache_evict_req[i].subopcode})
      `ASSERT_VALID_PORTS(sva_l2_dcache_miss_req_xs, clock, reset_for_assert, neigh_top.channel.l2_dcache_miss_req_valid[i], {neigh_top.channel.l2_dcache_miss_req[i].id,
                                                                                                                              neigh_top.channel.l2_dcache_miss_req[i].source,
                                                                                                                              neigh_top.channel.l2_dcache_miss_req[i].wdata,
                                                                                                                              neigh_top.channel.l2_dcache_miss_req[i].opcode,
                                                                                                                              neigh_top.channel.l2_dcache_miss_req[i].address,
                                                                                                                              neigh_top.channel.l2_dcache_miss_req[i].size,
                                                                                                                              neigh_top.channel.l2_dcache_miss_req[i].qwen,
                                                                                                                              neigh_top.channel.l2_dcache_miss_req[i].subopcode})
      `ASSERT_VALID_PORTS(sva_l2_dcache_resp_xs, clock, reset_for_assert, neigh_top.channel.l2_dcache_resp_valid[i], {neigh_top.channel.l2_dcache_resp[i].id,
                                                                                                                      neigh_top.channel.l2_dcache_resp[i].dest,
                                                                                                                      neigh_top.channel.l2_dcache_resp[i].wdata,
                                                                                                                      neigh_top.channel.l2_dcache_resp[i].opcode,
                                                                                                                      neigh_top.channel.l2_dcache_resp[i].size,
                                                                                                                      neigh_top.channel.l2_dcache_resp[i].qwen})
      // X propagation on minion - icache request interface
      `ASSERT_VALID_PORTS(sva_icache_req_xs, clock, reset_for_assert, neigh_top.channel.icache_req_valid[i], neigh_top.channel.icache_req[i])
   end
endgenerate

// X propagation on minion - icache response interface
generate
for (genvar i = 0; i < `ICACHE_NR_REQS; i++)
   begin : sva_icache_resp
      `ASSERT_VALID_PORTS(sva_icache_resp_miss_xs, clock, reset_for_assert, neigh_top.channel.icache_resp_valid[i], neigh_top.channel.icache_resp_miss[i])
      `ASSERT_VALID_PORTS(sva_icache_resp_xs, clock, reset_for_assert, neigh_top.channel.icache_resp_valid[i] & ~neigh_top.channel.icache_resp_miss[i], neigh_top.channel.icache_resp[i])
   end
endgenerate
// X propagation on coop TLoad buses
`ASSERT_VALID_PORTS(sva_tload_slv_rdy_xs, clock, reset_for_assert, neigh_top.channel.coop_tload_slv_rdy_out_valid_lv, neigh_top.channel.coop_tload_slv_rdy_out_data_lv)
`ASSERT_VALID_PORTS(sva_tload_mst_done_xs, clock, reset_for_assert, neigh_top.channel.coop_tload_mst_done_out_valid_lv, neigh_top.channel.coop_tload_mst_done_out_coop_id_lv)
// X propagation on icache L1 SRAM interface
`ASSERT_VALID_PORTS(sva_icache_sram_req_xs, clock, reset_for_assert, neigh_top.channel.icache_f2_sram_req_valid, {neigh_top.channel.icache_f2_sram_req_write,
                                                                                                           neigh_top.channel.icache_f2_sram_req_addr})
`ASSERT_VALID_PORTS(sva_icache_sram_resp_xs, clock, reset_for_assert, neigh_top.channel.icache_f0_sram_resp_valid, neigh_top.channel.icache_f0_sram_resp_dout)
// X propagation on miss request arbiter
`ASSERT_VALID_PORTS(sva_miss_req_info_ff_proc_xs, clock, reset_for_assert, neigh_top.channel.miss_req_valid_ff_proc, neigh_top.channel.miss_req_info_ff_proc)
// X propagation on fill FIFO
generate
for (genvar i = 0; i < `NEIGH_NUM_RESP_AGENTS; i++)
   begin : sva_fill_fifo
      `ASSERT_VALID_PORTS(sva_fill_fifo_xs, clock, reset_for_assert, neigh_top.channel.fill_fifo_out_rsp_valid[i], {neigh_top.channel.fill_fifo_out_rsp_info[i].id,
                                                                                                                    neigh_top.channel.fill_fifo_out_rsp_info[i].dest,
                                                                                                                    neigh_top.channel.fill_fifo_out_rsp_info[i].wdata,
                                                                                                                    neigh_top.channel.fill_fifo_out_rsp_info[i].opcode,
                                                                                                                    neigh_top.channel.fill_fifo_out_rsp_info[i].size,
                                                                                                                    neigh_top.channel.fill_fifo_out_rsp_info[i].qwen})
   end
endgenerate

// ICACHE section

// Icache reset
generate
for (genvar i = 0; i < 2; i++)
   begin : sva_icache_reset
      `ASSERT_ALWAYS_MSG(sva_icache_reset, clock, reset_for_assert, ($fell(neigh_top.channel.shared_icache.icache.reset) |->
         ((neigh_top.channel.shared_icache.icache.f0_miss_state === icache_miss_state_Ready) &&
          (neigh_top.channel.shared_icache.icache.f0_l2_miss_resp_done === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.f0_l1_resp_done === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.esr_err_log_sbe === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.esr_err_log_dbe === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.ucache_l1_miss_req_ff_available === '1) &&
          (neigh_top.channel.shared_icache.icache.f0_l1_miss_req_valid === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.f0_l1_miss_req_ready === 1'b1) &&
          (neigh_top.channel.shared_icache.icache.f0_l2_miss_req_valid === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.tag_array.f0_tag_valid === '0) &&
          (neigh_top.channel.shared_icache.icache.tag_array.apb_pready === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.data_array.f2_mem_req_valid === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f0_prefetch_state === icache_prefetch_state_Idle) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f0_prefetch_req_valid === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.esr_prefetch_done === 1'b1) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f0_miss_state === icache_miss_state_Ready) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f2_tlb_fill_was_pending === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f2_tlb_resp_data === '0) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f3_hit === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f3_tlb_miss === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f5_resp_fill_done === 1'b0) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.micro_tag_array.f2_valid_entries === '0) &&
          (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.micro_tag_array.apb_pready === 1'b0))),
      "Icache cannot return to a known state after a reset");
   end
endgenerate

// Shared icache request arbiters
generate
for (genvar i = 0; i < `ICACHE_NR_REQS; i++)
   begin: sva_shared_icache_req_arb
      logic                                                          th_miss_any_cnt_sat;
      logic [`MIN_PER_ICACHE*`CORE_NR_THREADS/`ICACHE_NR_REQS/2-1:0] min_miss_mask;

      // Check whether any counter saturated
      `FF(clock, th_miss_any_cnt_sat, |neigh_top.channel.shared_icache.REQ_ARB[i].th_miss_cnt_sat)

      // Combine mask from threads of a minion into minion mask
      always_comb
      begin
          for (integer t_idx = 0; t_idx < `MIN_PER_ICACHE*`CORE_NR_THREADS/`ICACHE_NR_REQS; t_idx=t_idx+2)
              min_miss_mask[t_idx/2] = |neigh_top.channel.shared_icache.REQ_ARB[i].th_miss_mask[t_idx +: 2];
      end

      `ASSERT_ALWAYS_MSG(sva_icache_one_min_allowed_on_miss_sat, clock, reset_for_assert, $onehot(min_miss_mask) | ~th_miss_any_cnt_sat, "More than one minion allowed after an icache miss counter saturated")
      `ASSERT_ALWAYS_MSG(sva_icache_masked_min_req, clock, reset_for_assert, ~|(neigh_top.channel.shared_icache.f1_req_ready[i*`MIN_PER_ICACHE/`ICACHE_NR_REQS +: `MIN_PER_ICACHE/`ICACHE_NR_REQS] &
                                                                                ~neigh_top.channel.shared_icache.REQ_ARB[i].th_miss_mask), "Masked thread request went through after an icache miss counter saturated")
   end
endgenerate

// Icache resp latency
generate
for (genvar i = 0; i < 2; i++)
   begin : sva_icache_resp_latency
      `ASSERT_ALWAYS_MSG(sva_icache_resp_latency, clock, reset_for_assert, ((neigh_top.channel.shared_icache.icache.f0_req_valid[i] && neigh_top.channel.shared_icache.icache.f0_req_ready[i]) === 1 |-> ##4
         (neigh_top.channel.shared_icache.icache.f4_resp_valid[i] === 1)), "Valid response to a requestor is not generated 4 cycles after the corresponding request");
   end
endgenerate

// APB debug interface
`ASSERT_ALWAYS_MSG(sva_icache_apb_exclusive_pready, clock, reset_for_assert, $onehot0({neigh_top.channel.shared_icache.icache.ucache_apb_pready, neigh_top.channel.shared_icache.icache.tag_apb_pready}), "I-cache APB request had response from more than one ucache or ucache and i-cache simultaneously")
`ASSERT_ALWAYS_MSG(sva_icache_apb_exclusive_penable, clock, reset_for_assert, $onehot0({neigh_top.channel.shared_icache.icache.ucache_apb_penable, neigh_top.channel.shared_icache.icache.tag_apb_penable}), "I-cache ABP request triggered access to  more than one ucache or ucache and i-cache simultaneously")

// Icache L0 microcache hit one way
generate
for (genvar i = 0; i < 2; i++)
   begin : sva_icache_ucache    
      `ASSERT_ALWAYS_MSG(sva_icache_ucache_hit_one_way, clock, reset_for_assert, $onehot0(neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f2_hit_array) |
                                                                                 ~neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f2_valid |
                                                                                 neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f2_tlb_resp_data.miss, "More than one way had a hit in the icache!")
   end
endgenerate

// Icache hit one way
`ASSERT_ALWAYS_MSG(sva_icache_l1_hit_one_way, clock, reset_for_assert, $onehot0(neigh_top.channel.shared_icache.icache.f1_tag_hit) | ~neigh_top.channel.shared_icache.icache.f1_valid, "More than one way had a hit in the icache!")

// Icache disable l2 miss req
`ASSERT_ALWAYS_MSG(sva_icache_disable_l2_miss_req, clock, reset_for_assert, ((neigh_top.channel.shared_icache.icache.f0_l2_miss_req_disable_next === 1) |-> ##1 (neigh_top.channel.shared_icache.icache.f0_l2_miss_req_valid === 0)), "ET Link miss requests to L2 are sent despite disable_next signal is enabled in the previous clock cycle");

// Icache mem conf
generate
for (genvar i = 0; i < 2; i++)
   begin : sva_icache_mem_conf
      `ASSERT_ALWAYS_MSG(sva_icache_mem_conf_vmspagesize, clock, reset_for_assert, (neigh_top.channel.shared_icache.icache.esr_vmspagesize |-> ##[0:3]
         neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.tlb_array.tlb_top.vmspagesize), "esr_vmspagesize not properly connected to icache.UCACHE[i].micro_cache.tlb_array.tlb_top.vmspagesize");

      `ASSERT_ALWAYS_MSG(sva_icache_mem_conf_coop_mode, clock, reset_for_assert, (neigh_top.channel.shared_icache.icache.esr_shire_coop_mode |-> ##[0:3]
         neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.tlb_array.tlb_top.coop_mode), "esr_shire_coop_mode not properly connected to icache.UCACHE[i].micro_cache.tlb_array.tlb_top.coop_mode");

      `ASSERT_ALWAYS_MSG(sva_icache_mem_conf_mprot, clock, reset_for_assert, (neigh_top.channel.shared_icache.icache.esr_mprot |-> ##[0:3]
         neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.pma_unit.mprot), "esr_mprot not properly connected to icache.UCACHE[i].micro_cache.pma_unit.mprot");
   end
endgenerate

// Prefetch request
`ASSERT_ALWAYS_MSG(sva_icache_prefetch_not_done, clock, reset_for_assert, ((neigh_top.channel.shared_icache.icache.esr_prefetch_start === 1) |-> ##[0:5] $fell(neigh_top.channel.shared_icache.icache.esr_prefetch_done)), "Prefetch request is not received with a negative edge of the done signal");

// Prefetch dis ucache
`ASSERT_ALWAYS_MSG(sva_icache_prefetch_dis_ucache, clock, reset_for_assert, (((neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f2_miss === 1) &&
   (neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f2_is_prefetch === 1)) |=> ((neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f0_tag_array_wr_valid === 0) &&
   (neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f0_data_array_wr_en === 0))), "Tags and data of the microcache are written on a prefetch request");

// Icache prefetch req ucache dis hit
`ASSERT_ALWAYS_MSG(sva_icache_prefetch_ucache_dis_hit, clock, reset_for_assert, ((neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f2_is_prefetch === 1) |->
   (neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f2_hit === 0)), "Microcache hits on stored line on a prefetch request");

// Icache prefetch dis resp
`ASSERT_ALWAYS_MSG(sva_icache_prefetch_dis_resp, clock, reset_for_assert, ((neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f0_prefetch_req_access === 1) |-> ##4
   (neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f4_resp_valid === 0)), "Valid response to requestors is generated after 4 cycles after a prefetch request entered the pipeline");

//L1 hit ucache fill FUTURE
//generate
//for (genvar i = 0; i < 2; i++)
//    begin : sva_icache_l1_hit_ucache_fill
   `ASSERT_ALWAYS_MSG(sva_icache_l1_hit_ucache_fill, clock, reset_for_assert, ((neigh_top.channel.shared_icache.icache.f1_hit === 1) |-> ##[8:15]
      ( ((neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.micro_data_array.wr_en === 1) &&
         (neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.micro_tag_array.f0_tag_wr_en === 1)) ||
        ((neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.micro_data_array.wr_en === 1) &&
         (neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.micro_tag_array.f0_tag_wr_en === 1)) ||
        (neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f0_miss_disable_l0_access === 1) ||
        (neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.f0_miss_disable_l0_access === 1) ) ),
      "On an L1 hit, the line is not stored into the microcache");
//    end
//endgenerate

// Icache L2 resp L1 ucache fill FUTURE
//generate
//for (genvar i = 0; i < 2; i++)
//    begin : sva_icache_l2_resp_l1_ucache_fill
   `ASSERT_ALWAYS_MSG(sva_icache_l2_resp_l1_ucache_fill, clock, reset_for_assert, (((neigh_top.channel.shared_icache.icache.f0_l2_miss_resp_ready === 1) &&
      (neigh_top.channel.shared_icache.icache.f0_l2_miss_resp_valid === 1) &&
      (neigh_top.channel.shared_icache.icache.f0_l2_miss_resp.opcode !== ET_LINK_RSP_Err) &&
      (neigh_top.channel.shared_icache.icache.esr_bypass_icache === 0)) |-> ##1
      ((neigh_top.channel.shared_icache.icache.tag_array.TAG_ARRAY_WAYS[0].f0_tag_wr_en |
        neigh_top.channel.shared_icache.icache.tag_array.TAG_ARRAY_WAYS[1].f0_tag_wr_en |
        neigh_top.channel.shared_icache.icache.tag_array.TAG_ARRAY_WAYS[2].f0_tag_wr_en |
        neigh_top.channel.shared_icache.icache.tag_array.TAG_ARRAY_WAYS[3].f0_tag_wr_en) === 1) ##[1:9]
      ( ((neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.micro_tag_array.f0_tag_wr_en === 1) &&
         (neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.micro_data_array.wr_en === 1)) ||
        ((neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.micro_tag_array.f0_tag_wr_en === 1) &&
         (neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.micro_data_array.wr_en === 1)) ) ||
      (neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f0_miss_disable_l0_access === 1) ||
      (neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.f0_miss_disable_l0_access === 1) ),
      "After a L2 miss response is received, the line is not stored into the L1 SRAM and the microcache");
//    end
//endgenerate

// Icache bypass L1 tags
`ASSERT_ALWAYS_MSG(sva_icache_bypass_l1_tags, clock, reset_for_assert, ((neigh_top.channel.shared_icache.icache.esr_bypass_icache === 1) |-> (neigh_top.channel.shared_icache.icache.f0_tag_write_valid === 0)), "L1 tags are written when bypass is enabled");

// Icache bypass L1 hit
`ASSERT_ALWAYS_MSG(sva_icache_bypass_l1_hit, clock, reset_for_assert, ((neigh_top.channel.shared_icache.icache.esr_bypass_icache === 1) |-> (neigh_top.channel.shared_icache.icache.f1_hit === 0)), "L1 hits are notified when bypass is enabled");

// Icache bypass ucache FUTURE
//generate
//for (genvar i = 0; i < 2; i++)
//    begin : sva_icache_bypass_ucache
   `ASSERT_ALWAYS_MSG(sva_icache_bypass_ucache, clock, reset_for_assert, ((neigh_top.channel.shared_icache.icache.esr_bypass_icache === 1) |->
      ((neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f0_tag_array_wr_valid === 0) && (neigh_top.channel.shared_icache.icache.UCACHE[0].micro_cache.f0_data_array_wr_en === 0)) ||
      ((neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.f0_tag_array_wr_valid === 0) && (neigh_top.channel.shared_icache.icache.UCACHE[1].micro_cache.f0_data_array_wr_en === 0))),
      "Tags and data of the microcache are written when bypass is enabled");
//    end
//endgenerate

// Icache L2 resp bypass fill FUTURE
generate
for (genvar i = 0; i < 2; i++)
    begin : sva_icache_l2_resp_bypass_fill
   `ASSERT_ALWAYS_MSG(sva_icache_l2_resp_bypass_fill, clock, reset_for_assert, ( ((neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f0_miss_bypass_icache === 1) &&
      (neigh_top.channel.shared_icache.icache.f0_l1_miss_req_idx === i[neigh_top.channel.shared_icache.icache.NR_REQUESTORS_L-1:0]) &&
      (neigh_top.channel.shared_icache.icache.f0_l2_miss_resp_ready === 1) &&
      (neigh_top.channel.shared_icache.icache.f0_l2_miss_resp_valid === 1) &&
      (neigh_top.channel.shared_icache.icache.f0_l2_miss_resp.opcode !== ET_LINK_RSP_Err) &&
      (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.f0_miss_is_prefetch === 0)) |-> ##[2:10]
      (neigh_top.channel.shared_icache.icache.UCACHE[i].micro_cache.bypass_en === 1) ), "After a L2 miss response is received, the line is not stored into the bypass FF");
    end
endgenerate

//ISOLATION CHECK ASSERTIONS
`ASSERT_ALWAYS_MSG(sva_neigh_iso_reset_w_icache, clock, reset_for_assert_isolate_only, (neigh_top.reset_w_icache === 0), "neigh_top.reset_w_icache is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_iso_eco_o, clock, reset_for_assert_isolate_only, (|neigh_top.eco_o === 0), "neigh_top.eco_o is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_iso_pwr_ctrl_glb_nsleepout, clock, reset_for_assert_isolate_only, (neigh_top.pwr_ctrl_glb_nsleepout === 0), "neigh_top.pwr_ctrl_glb_nsleepout is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_iso_pwr_ctrl_min_nsleepout, clock, reset_for_assert_isolate_only, (|neigh_top.pwr_ctrl_min_nsleepout === 0), "neigh_top.pwr_ctrl_min_nsleepout is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_bpam_rc_tbox_ack_hi, clock, reset_for_assert_isolate_only, (&neigh_top.bpam_rc_tbox_ack_hi === 1), "neigh_top.bpam_rc_tbox_ack_hi is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_esr_icache_prefetch_done, clock, reset_for_assert_isolate_only, (neigh_top.esr_icache_prefetch_done === 1), "neigh_top.esr_icache_prefetch_done is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_esr_icache_err_detected, clock, reset_for_assert_isolate_only, (neigh_top.esr_icache_err_detected === 0), "neigh_top.esr_icache_err_detected is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_esr_icache_err_logged, clock, reset_for_assert_isolate_only, (neigh_top.esr_icache_err_logged === 0), "neigh_top.esr_icache_err_detected is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_esr_and_or_tree_L0, clock, reset_for_assert_isolate_only, (|neigh_top.esr_and_or_tree_L0 === 0), "neigh_top.esr_and_or_tree_L0 is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_neigh_sc_req_valid, clock, reset_for_assert_isolate_only, (|neigh_top.neigh_sc_req_valid === 0), "neigh_top.neigh_sc_req_valid is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_neigh_sc_rsp_ready, clock, reset_for_assert_isolate_only, (neigh_top.neigh_sc_rsp_ready === 1), "neigh_top.neigh_sc_rsp_ready is not '1' during isolation");
//`ASSERT_ALWAYS_MSG(sva_neigh_APB_ESR_rsp, clock, reset_for_assert_isolate_only, (&neigh_top.APB_ESR_rsp === 1), "neigh_top.APB_ESR_rsp is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_APB_ESR_rsp_pready, clock, reset_for_assert_isolate_only, (&neigh_top.APB_ESR_rsp.apb_pready === 1), "neigh_top.APB_ESR_rsp is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_APB_ESR_rsp_pslverr, clock, reset_for_assert_isolate_only, (&neigh_top.APB_ESR_rsp.apb_pslverr === 1), "neigh_top.APB_ESR_rsp is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_flb_neigh_l2_req_valid, clock, reset_for_assert_isolate_only, (neigh_top.flb_neigh_l2_req_valid === 0), "neigh_top.flb_neigh_l2_req_valid is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_icache_f2_sram_req_write, clock, reset_for_assert_isolate_only, (neigh_top.icache_f2_sram_req_write === 0), "neigh_top.icache_f2_sram_req_write is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_icache_f2_sram_req_valid, clock, reset_for_assert_isolate_only, (neigh_top.icache_f2_sram_req_valid === 0), "neigh_top.icache_f2_sram_req_valid is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_icache_f0_sram_resp_ready, clock, reset_for_assert_isolate_only, (neigh_top.icache_f0_sram_resp_ready === 1), "neigh_top.icache_f0_sram_resp_ready is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_coop_tload_slv_rdy_out_valid, clock, reset_for_assert_isolate_only, (neigh_top.coop_tload_slv_rdy_out_valid === 0), "neigh_top.coop_tload_slv_rdy_out_valid is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_coop_tload_mst_done_out_valid, clock, reset_for_assert_isolate_only, (|neigh_top.coop_tload_mst_done_out_valid === 0), "neigh_top.coop_tload_mst_done_out_valid is not '0' during isolation");


//MINION IDLENESS CHECK WHEN ISOLATION STARTS
generate
for (genvar i=0; i<`MIN_PER_N; i++) begin : sva_minion_idle

  if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1))) begin  //(Charles CYE 11/15/2019) fix

    //dcache_reduce
    `ASSERT_ALWAYS_MSG(sva_neigh_dcache_reduce_ba_alloc_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.gen_minions[i].min_inst.minion.core.dcache.reduce.ba_alloc_fifo.valid === 0)), $sformatf("neigh_top.gen_minions[%d].min_inst.minion.core.dcache.reduce.ba_alloc_fifo.valid is not 0 when isolation starts", i));
    `ASSERT_ALWAYS_MSG(sva_neigh_dcache_reduce_ba_op_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.gen_minions[i].min_inst.minion.core.dcache.reduce.ba_op_fifo.valid === 0)), $sformatf("neigh_top.gen_minions[%d].min_inst.minion.core.dcache.reduce.ba_op_fifo.valid is not 0 when isolation startes", i));    
    `ASSERT_ALWAYS_MSG(sva_neigh_dcache_reduce_ba_data_send_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.gen_minions[i].min_inst.minion.core.dcache.reduce.ba_data_send_fifo.valid === 0)), $sformatf("neigh_top.gen_minions[%d].min_inst.minion.core.dcache.reduce.ba_data_send_fifo.valid is not 0 when isolation starts", i));
    `ASSERT_ALWAYS_MSG(sva_neigh_dcache_reduce_phys_addr_send_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.gen_minions[i].min_inst.minion.core.dcache.reduce.phys_addr_send_fifo.valid === 0)), $sformatf("neigh_top.gen_minions[%d].min_inst.minion.core.dcache.reduce.phys_addr_send_fifo.valid is not 0 when isolation starts", i));    
   `ifdef GFX_ENABLED
      //dcache_texsend
    `ASSERT_ALWAYS_MSG(sva_neigh_dcache_texsend_req_tbpull_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.gen_minions[i].min_inst.minion.core.dcache.texsend.req_tbpull_fifo.valid === 0)), $sformatf("neigh_top.gen_minions[%d].min_inst.minion.core.dcache.texsend.req_tbpull_fifo.valid is not 0 when isolation starts", i));
   `endif

  end  // if (!((STUB_MINIONS[i] === 1'b1) || (DISABLE_MINIONS[i] === 1'b1)))

end
endgenerate

//NEIGHBORHOOD CHECK WHEN ISOLATION STARTS
`ASSERT_ALWAYS_MSG(sva_neigh_int_req_info_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.int_req_info_fifo.valid === 0)), $sformatf("neigh_top.channel.int_req_info_fifo.valid is not 0 when isolation starts"));
`ASSERT_ALWAYS_MSG(sva_neigh_int_req_data_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.int_req_data_fifo.valid === 0)), $sformatf("neigh_top.channel.int_req_data_fifo.valid is not 0 when isolation starts"));
generate
  for (genvar i=0; i<`SC_BANKS; i++) begin
    `ASSERT_ALWAYS_MSG(sva_neigh_bank_req_info_vcfifo_wr_lov, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.BANK_FIFOS[i].bank_req_info_vcfifo_wr_lov.valid === 0)), $sformatf("neigh_top.channel.BANK_FIFOS[%d].bank_req_info_vcfifo_wr_lov.valid is not 0 when isolation starts", i));
    `ASSERT_ALWAYS_MSG(sva_neigh_bank_req_data_bank_vcfifo_wr_lov, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.BANK_FIFOS[i].bank_req_data_bank_vcfifo_wr_lov.valid === 0)), $sformatf("neigh_top.channel.BANK_FIFOS[%d].bank_req_data_bank_vcfifo_wr_lov.valid is not 0 when isolation starts", i));
  end
endgenerate
`ASSERT_ALWAYS_MSG(sva_neigh_uc_req_info_vcfifo_wr_lov, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.uc_req_info_vcfifo_wr_lov.valid === 0)), $sformatf("neigh_top.channel.uc_req_info_vcfifo_wr_lov.valid is not 0 when isolation starts"));
`ASSERT_ALWAYS_MSG(sva_neigh_uc_req_data_vcfifo_wr_lov, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.uc_req_data_vcfifo_wr_lov.valid === 0)), $sformatf("neigh_top.channel.uc_req_data_vcfifo_wr_lov.valid is not 0 when isolation starts"));
`ASSERT_ALWAYS_MSG(sva_neigh_fill_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.fill_fifo.valid === 0)), $sformatf("neigh_top.channel.fill_fifo.valid is not 0 when isolation starts"));
generate
  for (genvar i=0; i<`MIN_PER_N; i++) begin
    `ASSERT_ALWAYS_MSG(sva_neigh_fill_ff_info_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.MINION_FF[i].fill_ff.fill_ff_info_fifo.valid === 0)), $sformatf("neigh_top.channel.MINION_FF[%d]/fill_ff.fill_ff_info_fifo.valid is not 0 when isolation starts", i));
    `ASSERT_ALWAYS_MSG(sva_neigh_fill_ff_data_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.MINION_FF[i].fill_ff.fill_ff_data_fifo.valid === 0)), $sformatf("neigh_top.channel.MINION_FF[%d]/fill_ff.fill_ff_data_fifo.valid is not 0 when isolation starts", i));    
  end
endgenerate
`ASSERT_ALWAYS_MSG(sva_neigh_req_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.fl_barrier.req_fifo.valid === 0)), $sformatf("neigh_top.channel.fl_barrier.req_fifo.valid is not 0 when isolation starts"));
/*
//neigh_tbox_router
`ASSERT_ALWAYS_MSG(sva_neigh_min_tbox_req_fifo, clock, reset_for_assert_safe_isolate_only, ((posedge isolate) |-> (neigh_top.channel.tbox_router.min_tbox_req_fifo.valid === 0)), $sformatf("neigh_top.channel.tbox_router.min_tbox_req_fifo.valid is not 0 when isolation starts", i));
`ASSERT_ALWAYS_MSG(sva_neigh_tbox_neigh_req_fifo, clock, reset_for_assert_safe_isolate_only, ((posedge isolate) |-> (neigh_top.channel.tbox_router.tbox_neigh_req_fifo.valid === 0)), $sformatf("neigh_top.channel.tbox_router.tbox_neigh_req_fifo.valid is not 0 when isolation starts", i));
`ASSERT_ALWAYS_MSG(sva_neigh_sc_tbox_rsp_fifo, clock, reset_for_assert_safe_isolate_only, ((posedge isolate) |-> (neigh_top.channel.tbox_router.sc_tbox_rsp_fifo.valid === 0)), $sformatf("neigh_top.channel.tbox_router.sc_tbox_rsp_fifo.valid is not 0 when isolation starts", i));
`ASSERT_ALWAYS_MSG(sva_neigh_tbox_neigh_rsp_fifo, clock, reset_for_assert_safe_isolate_only, ((posedge isolate) |-> (neigh_top.channel.tbox_router.tbox_neigh_rsp_fifo.valid === 0)), $sformatf("neigh_top.channel.tbox_router.tbox_neigh_rsp_fifo.valid is not 0 when isolation starts", i));
*/
`ASSERT_ALWAYS_MSG(sva_neigh_tensor_store_req_fifo, clock, reset_for_assert_safe_isolate_only, ($rose(isolate) |-> (neigh_top.channel.tensor_store_buffer.tensor_store_req_fifo.valid === 0)), $sformatf("neigh_top.channel.tensor_store_buffer.tensor_store_req_fifo.valid is not 0 when isolation starts"));




// End-of-Sim or End-of-Test checks:

// neigh/channel/int_req_fifo_counter[OUT_FIFO] returns to 0;   // [4:0][1:0]
// neigh/channel/fill_fifo/used_entries == '0;                  // [3:0]
// neigh/channel/coop_tload/rdy_avail_creds[SHIRE_COOP_ID] returns to initial values 3;  // [31:0][1:0]
// neigh/channel/coop_tload/coop_tload_ext_rdy[SHIRE_COOP_ID][NEIGH] returns to 0;       // [31:0][2:0][1:0]
// neigh/channel/coop_tload/coop_tload_tag_table/used_entries[31:0] returns to 0;        // [31:0]

`ASSERT_END_OF_SIM_MSG(ERROR_neigh_all_buffers_queues_empty, 
     (
      (neigh_top.channel.int_req_fifo_counter[4:0] ==                       // actually [4:0][1:0]
                                 {2'b0, 2'b0, 2'b0, 2'b0, 2'b0}) &&    
      (neigh_top.channel.fill_fifo.used_entries[3:0] == 'h0) &&
      (neigh_top.channel.coop_tload.rdy_avail_creds[31:0] ==                            // actually [31:0][1:0] 
                                {2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3,
                                 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 
                                 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 
                                 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3, 2'h3} ) && 
      (neigh_top.channel.coop_tload.coop_tload_ext_rdy[31:0] ==                         // actually [31:0][2:0][1:0]
                                {{2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0},
                                 {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0},
                                 {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0},
                                 {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0},
                                 {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0},
                                 {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0},
                                 {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0},
                                 {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}, {2'h0,2'h0,2'h0}} ) && 
      (neigh_top.channel.coop_tload.coop_tload_tag_table.used_entries[31:0] == 'h0)
     ),
     $sformatf("Neigh: At the end of test: neigh_top.channel: not all buffers and queues are empty"), -1)



//RESET WARM Assertions
//Next target resets must be asserted after these 2 events:
// * neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset          
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__00_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.neigh_ch_dbg.reset_w    ), "Debug Reset Control _00_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__01_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.miss_req_arbiter.reset  ), "Debug Reset Control _01_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__02_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.int_req_data_fifo.reset ), "Debug Reset Control _02_____");
genvar l2_bank;
generate
  for (l2_bank = 0; l2_bank < `SC_BANKS; l2_bank++) begin: gen_BANK_FIFOS_dbg_reset_asrts
    `ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__03_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset        ) |-> ##[0:8] neigh_top.channel.BANK_FIFOS[l2_bank].bank_req_info_vcfifo_wr_lov.reset        ), "Debug Reset Control _03_____");
    `ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__04_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset        ) |-> ##[0:8] neigh_top.channel.BANK_FIFOS[l2_bank].bank_req_data_bank_vcfifo_wr_lov.reset   ), "Debug Reset Control _04_____");
  end
endgenerate
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__05_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.uc_req_info_vcfifo_wr_lov.reset      ), "Debug Reset Control _05_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__06_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.uc_req_data_vcfifo_wr_lov.reset      ), "Debug Reset Control _06_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__07_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.fill_fifo.reset                      ), "Debug Reset Control _07_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__08_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.shared_icache.reset                  ), "Debug Reset Control _08_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__09_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.PTW[0].shared_ptw.reset              ), "Debug Reset Control _09_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__10_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.PTW[1].shared_ptw.reset              ), "Debug Reset Control _10_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__11_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.local_message_network.reset          ), "Debug Reset Control _11_____");
genvar min_it;
generate
for (min_it= 0; min_it<`MIN_PER_N; min_it++) begin :gen_min_dbg_reset_asrts
  `ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__12_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset          ) |-> ##[0:8] neigh_top.channel.MINION_FF[min_it].miss_ff.reset         ), "Debug Reset Control _12_____");
  `ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__13_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset          ) |-> ##[0:8] neigh_top.channel.MINION_FF[min_it].evict_ff.reset        ), "Debug Reset Control _13_____");
  `ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__14_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset          ) |-> ##[0:8] neigh_top.channel.MINION_FF[min_it].min_req_arbiter.reset ), "Debug Reset Control _14_____");
  `ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__15_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset          ) |-> ##[0:8] neigh_top.channel.MINION_FF[min_it].fill_ff.reset         ), "Debug Reset Control _15_____");
end
endgenerate
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__16_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.fl_barrier.reset             ), "Debug Reset Control _16_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__17_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.coop_tload.reset             ), "Debug Reset Control _17_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__18_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.tensor_store_buffer.reset    ), "Debug Reset Control _18_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__19_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.esrs.reset_w                 ), "Debug Reset Control _19_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__21_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.hi_voltage_cross.reset_w     ), "Debug Reset Control _21_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__22_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.lo_voltage_cross.reset_w     ), "Debug Reset Control _22_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__23_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.pmu.reset                    ), "Debug Reset Control _23_____");
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__24_00, clock, reset_for_assert, ( (neigh_top.channel.dmctrl.dmactive && neigh_top.channel.dmctrl.ndmreset            ) |-> ##[0:8] neigh_top.channel.coop_tload_ports_pd_hv.reset ), "Debug Reset Control _24_____");

//RESET_DEBUG Assertions
//Next target resets must be asserted after these 2 events:
// * after fall down neigh_top.channel.dmctrl.dmactive
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__25_00, clock, reset_for_assert, ( $fell(neigh_top.channel.dmctrl.dmactive) |-> ##[0:8] neigh_top.channel.neigh_ch_dbg.reset_d     ) , " Debug reset control _25_00") 
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__26_00, clock, reset_for_assert, ( $fell(neigh_top.channel.dmctrl.dmactive) |-> ##[0:8] neigh_top.channel.esrs.reset_d             ) , " Debug reset control _26_00") 
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__27_00, clock, reset_for_assert, ( $fell(neigh_top.channel.dmctrl.dmactive) |-> ##[0:8] neigh_top.channel.lo_voltage_cross.reset_d ) , " Debug reset control _27_00") 
generate                                                                                                                                                                                    
for (min_it= 0; min_it<`MIN_PER_N; min_it++) begin :gen_min_dbg_reset_2_asrts                                                                                                                 
  if((STUB_MINIONS[min_it] == 1'b0) && (DISABLE_MINIONS[min_it] == 1'b0)) begin : min_inst
`ASSERT_ALWAYS_MSG(sva_neigh__dbg_reset_control__28_00, clock, reset_for_assert, ( $fell(neigh_top.channel.dmctrl.dmactive) |-> ##[0:8] neigh_top.gen_minions[min_it].min_inst.minion.reset_debug  ) , " Debug reset control _28_00") 
  end
end
endgenerate

// End-of-Sim:
