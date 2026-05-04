// assertion usage example:
// `ASSERT_ALWAYS_MSG(ERROR_nameOfAssertion, clk, reset, (assert always expression), $sformatf("your strings with %stuff", $sampled(var) ) )


   // latches with the buffers
`ASSERT_ALWAYS_MSG(ERROR_ckgat_wr_t0, `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.clock_aon, reset_for_assert, !((`MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f6_buffer_wr_1p_next != `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f6_buffer_wr_1p) && `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.clock_enable),
                   "ERROR: buffer_wr_1p latch p2 not clocked")
`ASSERT_ALWAYS_MSG(ERROR_ckgat_wr_t1, `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.clock_aon, reset_for_assert, !((`MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f6_buffer_wr_1p_next != `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f6_buffer_wr_1p) && `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.clock_enable),
                   "ERROR: buffer_wr_1p latch p2 not clocked")


`ASSERT_ALWAYS_MSG(ERROR_ckgat_ffb_not_debug_t0, `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.clock_aon, reset_for_assert, !( (|`MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.debug_ffb_en  || `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.debug_ffb_exec) && !`MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.in_debug_mode ),
                   "ERROR: write or exec to ffb, but not in debug")
`ASSERT_ALWAYS_MSG(ERROR_ckgat_ffb_not_debug_t1, `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.clock_aon, reset_for_assert, !( (|`MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.debug_ffb_en  || `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.debug_ffb_exec) && !`MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.in_debug_mode ),
                   "ERROR: write or exec to ffb, but not in debug")

  // Controls for clock gating
`ASSERT_ALWAYS_MSG(ERROR_ckgat_ready_t0, `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.clock_aon, reset_for_assert, !(`MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f7_inst_ready && `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.f7_inst_valid) || `MIN_IFC_FE.FRONTEND_THREAD[0].thread_buffer.clock_enable,
                   "ERROR: f7_inst_ready with clock gated")
`ASSERT_ALWAYS_MSG(ERROR_ckgat_ready_t1, `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.clock_aon, reset_for_assert, !(`MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f7_inst_ready && `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.f7_inst_valid) || `MIN_IFC_FE.FRONTEND_THREAD[1].thread_buffer.clock_enable,
                   "ERROR: f7_inst_ready with clock gated")
