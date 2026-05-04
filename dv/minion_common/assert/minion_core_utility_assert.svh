//==================================================================
//Name: minion_core_utility_assert.svh
//Path: dv/minion_common/assert
//Description: MINION CORE Utility assertions
//Owner: Nikola Rajovic, Goran Ognjanovic
//==================================================================

// assertion usage example:
// `ASSERT_ALWAYS_MSG(ERROR_nameOfAssertion, clk, reset, (assert always expression), $sformatf("your strings with %stuff", $sampled(var) ) )

  `ASSERT_ALWAYS_MSG(sva_trap_handler_not_set, clock, reset,
    (  (`MIN_IFC_INTPIPE.wb_xcpt && !`MIN_IFC_INTPIPE.csr_file.xcpt_traps_to_debug 
    	&& (`MIN_IFC_INTPIPE.csr_file.reg_debug[wb_thread_id] ==0)   )  |-> not $isunknown(`MIN_IFC_INTPIPE.csr_file.tvec) ),
    "Trap handler has never been set in software");

  //minion top
`ASSERT_VALID_PORTS(debug_out_valid, clock, (reset_for_assert | `MIN_IFC_TOP.reset_debug | `MIN_IFC_TOP.reset_non_debug), 1'b1, `MIN_IFC_TOP.debug_out)
`ASSERT_VALID_PORTS(debug_in_valid,  clock, (reset_for_assert | `MIN_IFC_TOP.reset_debug | `MIN_IFC_TOP.reset_non_debug), 1'b1, `MIN_IFC_TOP.debug_in)


//minion debug
  // there shouldn't be writes from csr and apb at the same time to ddata0
  `ASSERT_ALWAYS( ERROR_ddata0_concurrent_writes, clock, reset_for_assert,
                 !((`MIN_IFC_CORE.debug_apb_slv.s1_reg_en[minion_debug_axdata0] || `MIN_IFC_CORE.debug_apb_slv.s1_reg_en[minion_debug_axdata1] || `MIN_IFC_CORE.debug_apb_slv.s1_reg_en[minion_debug_nxdata0] ||`MIN_IFC_CORE.debug_apb_slv.s1_reg_en[minion_debug_nxdata1]) &&
			     `MIN_IFC_CORE.debug_apb_slv.update_ddata0_ip[`MIN_IFC_CORE.debug_apb_slv.thread_sel]))
