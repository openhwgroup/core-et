`ifdef MINSHIRE_GATESIM_ENABLE
`ifdef UPF   

bit safe_off=0;

initial begin
   if (!`et_value_plusargs("SAFE_OFF=%d", safe_off)) begin
      safe_off=0;
   end
end

wire reset_neigh_gates = top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.reset_w_minion[0];
wire isolate = top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.pwr_ctrl_glb_isolate_int;

wire reset_for_assert = ((reset_neigh_gates | isolate) !== 1'b0) ? 1'b1 : 1'b0;
wire reset_for_assert_isolate_only = ((reset_neigh_gates | !isolate) !== 1'b0) ? 1'b1 : 1'b0;
wire reset_for_assert_safe_isolate_only = ((reset_neigh_gates | !isolate | !safe_off) !== 1'b0) ? 1'b1 : 1'b0;

//ISOLATION CHECK ASSERTIONS
`ASSERT_ALWAYS_MSG(sva_neigh_iso_reset_w_icache_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.reset_w_icache === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.reset_w_icache is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_iso_eco_o_gates, clock, reset_for_assert_isolate_only, (|top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.eco_o === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.eco_o is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_iso_pwr_ctrl_glb_nsleepout_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.pwr_ctrl_glb_nsleepout === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.pwr_ctrl_glb_nsleepout is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_iso_pwr_ctrl_min_nsleepout_gates, clock, reset_for_assert_isolate_only, (|top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.pwr_ctrl_min_nsleepout === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.pwr_ctrl_min_nsleepout is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_bpam_rc_tbox_ack_hi_gates, clock, reset_for_assert_isolate_only, (&top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.bpam_rc_tbox_ack_hi === 1), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.bpam_rc_tbox_ack_hi is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_esr_icache_prefetch_done_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.esr_icache_prefetch_done === 1), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.esr_icache_prefetch_done is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_esr_icache_err_detected_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.esr_icache_err_detected === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.esr_icache_err_detected is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_esr_icache_err_logged_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.esr_icache_err_logged === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.esr_icache_err_detected is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_esr_and_or_tree_L0, clock, reset_for_assert_isolate_only, (|top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.esr_and_or_tree_L0 === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.esr_and_or_tree_L0 is not '0' during isolation");
//`ASSERT_ALWAYS_MSG(sva_neigh_dll_feedback_shire_gates, clock, reset_for_assert_isolate_only, (neigh_top.dll_feedback_shire === 0), "neigh_top.dll_feedback_shire is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_dll_feedback_neigh_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.dll_feedback_neigh === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.dll_feedback_neigh is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_neigh_sc_req_valid_gates, clock, reset_for_assert_isolate_only, (|top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.neigh_sc_req_valid === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.neigh_sc_req_valid is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_neigh_sc_rsp_ready_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.neigh_sc_rsp_ready === 1), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.neigh_sc_rsp_ready is not '1' during isolation");
//`ASSERT_ALWAYS_MSG(sva_neigh_APB_ESR_rsp_gates, clock, reset_for_assert_isolate_only, (&neigh_top.APB_ESR_rsp === 1), "neigh_top.APB_ESR_rsp is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_APB_ESR_rsp_pready_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.APB_ESR_rsp[0] === 1), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.APB_ESR_rsp[0] (pready) is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_APB_ESR_rsp_pslverr_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.APB_ESR_rsp[1] === 1), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.APB_ESR_rsp[1] (pslverr) is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_flb_neigh_l2_req_valid_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.flb_neigh_l2_req_valid === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.flb_neigh_l2_req_valid is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_icache_f2_sram_req_write_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.icache_f2_sram_req_write === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.icache_f2_sram_req_write is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_icache_f2_sram_req_valid_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.icache_f2_sram_req_valid === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.icache_f2_sram_req_valid is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_icache_f0_sram_resp_ready_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.icache_f0_sram_resp_ready === 1), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.icache_f0_sram_resp_ready is not '1' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_coop_tload_slv_rdy_out_valid_gates, clock, reset_for_assert_isolate_only, (top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.coop_tload_slv_rdy_out_valid === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.coop_tload_slv_rdy_out_valid is not '0' during isolation");
`ASSERT_ALWAYS_MSG(sva_neigh_coop_tload_mst_done_out_valid_gates, clock, reset_for_assert_isolate_only, (|top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.coop_tload_mst_done_out_valid === 0), "top.soc_top.SHIRE[0].shire_top.shire_top.shire_top.shire_top.gen_neigh_0__neigh_neigh_inst.coop_tload_mst_done_out_valid is not '0' during isolation");

`endif
`endif