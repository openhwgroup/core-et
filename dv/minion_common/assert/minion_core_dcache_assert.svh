// assertion usage example:
// `ASSERT_ALWAYS_MSG(ERROR_nameOfAssertion, clk, reset, (assert always expression), $sformatf("your strings with %stuff", $sampled(var) ) )


/*
Alternative for assertion binding - to be removed

`ASSERT_ALWAYS_MSG(sva_dcache_mh_assigned_to_thread, clock_aon, reset,
( (s2_valid_qual && (s2_mh_req_info.cacheable != 1'b1)) |-> ( ((s2_mh_req_assigned == 2'b01) && (s2_mh_req_info.thread_id == 1'b0)) || ( (s2_mh_req_assigned == 2'b10) && (s2_mh_req_info.thread_id == 1'b1))) ), "Miss Handler not properly assigned to thread_id");

`ASSERT_ALWAYS_MSG(sva_dcache_mh_assigned_to_thread, clock_aon, reset,
    ( (s2_valid_qual && (s2_mh_req_info.cacheable != 1'b1)) |-> ( s2_mh_req_assigned[s2_mh_req_info.thread_id] == 1'b1   ) ), "Miss Handler not properly assigned to thread_id");
*/
 

//
// Appropriate MH assigned to thread id for L1 bypass/UC
//

logic[`DCACHE_MH_FILE_SIZE-1:0] s2_mh_req_assigned;
logic                           s2_mh_req_cacheable;
logic                           s2_mh_req_thread_id;
logic                           s2_valid_qual;

assign s2_mh_req_assigned  = `MIN_IFC_DCACHE.s2_mh_req_assigned;
assign s2_mh_req_cacheable = `MIN_IFC_DCACHE.s2_mh_req_info.cacheable;
assign s2_mh_req_thread_id = `MIN_IFC_DCACHE.s2_mh_req_info.thread_id;
assign s2_valid_qual       = `MIN_IFC_DCACHE.s2_valid_qual;

`ASSERT_ALWAYS_MSG(sva_dcache_mh_assigned_to_thread, clock, reset,
    ( (s2_valid_qual && (s2_mh_req_cacheable != 1'b1)) |-> ( s2_mh_req_assigned[s2_mh_req_thread_id] == 1'b1   ) ), "Miss Handler not properly assigned to thread_id");


//
// Fully masked vector instruction
//
`ASSERT_ALWAYS_MSG(sva_dcache_masked_req_cause_no_xcpt, clock, reset,
    ( (`MIN_IFC_DCACHE.s0_core_req_valid && (`MIN_IFC_DCACHE.s0_core_req.typ >= 4'hb) && (`MIN_IFC_DCACHE.s0_core_req.ps_mask  == 8'b0)) |=> ( `MIN_IFC_DCACHE.s1_has_xcpt == 1'b0 ) ), "Fully masked vector instruction must not rise an exception");


//
// ET_LINK errors
//

et_link_core_miss_id_t          l2_resp_id_t0;
et_link_core_miss_id_t          l2_resp_id_t1;

assign l2_resp_id_t0 = (et_link_core_miss_id_t'(`MIN_IFC_DCACHE.tensor_load_0.l2_resp.id));
assign l2_resp_id_t1 = (et_link_core_miss_id_t'(`MIN_IFC_DCACHE.tensor_load_1.l2_resp.id));


// Errors returned from L2 response must be handled
// l2_rsp_valid && l2_rsp_is_err && unit_not_idle && other_conditions_specific_to_the_unit

`ASSERT_ALWAYS_MSG(sva_dcache_tl0_et_link_rsp_error_reports_bus_err, clock, reset,
   ( ( `MIN_IFC_DCACHE.tensor_load_0.l2_resp_valid && (`MIN_IFC_DCACHE.tensor_load_0.l2_resp.opcode == 2'b11) && (`MIN_IFC_DCACHE.tensor_load_0.ctrl_state != 3'b0) && 
         ( (l2_resp_id_t0 == `MIN_IFC_DCACHE.tensor_load_0.my_miss_id[0]) || (l2_resp_id_t0 == `MIN_IFC_DCACHE.tensor_load_0.my_miss_id[1]) || (l2_resp_id_t0 == `MIN_IFC_DCACHE.tensor_load_0.my_miss_id[2]) || (l2_resp_id_t0 == `MIN_IFC_DCACHE.tensor_load_0.my_miss_id[3]) ) ) 
                             |-> ##[0:10] (`MIN_IFC_DCACHE.bus_err_tl0 != 2'b0) ), "TensorLoad0 unit did report bus error after receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_tl1_et_link_rsp_error_reports_bus_err, clock, reset,
   ( ( `MIN_IFC_DCACHE.tensor_load_1.l2_resp_valid && (`MIN_IFC_DCACHE.tensor_load_1.l2_resp.opcode == 2'b11) && (`MIN_IFC_DCACHE.tensor_load_1.ctrl_state != 3'b0) &&                
         ( (l2_resp_id_t0 == `MIN_IFC_DCACHE.tensor_load_0.my_miss_id[0]) || (l2_resp_id_t0 == `MIN_IFC_DCACHE.tensor_load_0.my_miss_id[1]) || (l2_resp_id_t0 == `MIN_IFC_DCACHE.tensor_load_0.my_miss_id[2]) || (l2_resp_id_t0 == `MIN_IFC_DCACHE.tensor_load_0.my_miss_id[3]) ) )   
                             |-> ##[0:10] (`MIN_IFC_DCACHE.bus_err_tl1 != 2'b0) ), "TensorLoad1 unit did report bus error after receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_ts_et_link_rsp_error_reports_bus_err, clock, reset,
   ( ( `MIN_IFC_DCACHE.reduce.l2_resp_valid && (`MIN_IFC_DCACHE.reduce.l2_resp.opcode == 2'b11) && (`MIN_IFC_DCACHE.reduce.fsm_state != 3'b0) &&
         `MIN_IFC_DCACHE.reduce.opcode_is_mine )    
                             |-> ##[0:10] (`MIN_IFC_DCACHE.bus_err_re != 2'b0) ), "TensorReduce unit did not report bus error after receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_mh0_et_link_rsp_error_reports_bus_err, clock, reset,
   ( (`MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.l2_resp_valid && (`MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.l2_resp_data.opcode == 2'b11) &&
         (`MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.state != 3'b0) && 
         (`MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.l2_resp_is_my_id || `MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.l2_resp_is_my_id_uc) )
                             |-> ##[0:10] (`MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.bus_err != 2'b0) ), "MissHandler0 did not report bus error after receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_mh1_et_link_rsp_error_reports_bus_err, clock, reset,
   ( (`MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.l2_resp_valid && (`MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.l2_resp_data.opcode == 2'b11) &&     
         (`MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.state != 3'b0) &&
         (`MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.l2_resp_is_my_id || `MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.l2_resp_is_my_id_uc) )
                             |-> ##[0:10] (`MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.bus_err != 2'b0) ), "MissHandler1 did not report bus error after receiving L2 error response");


// Errors returned from L2 response must be handled to complete transaction and unit should return to idle state

`ASSERT_ALWAYS_MSG(sva_dcache_tl0_et_link_rsp_error_followed_by_fsm_idle, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_tl0 != 2'b0)   |-> s_eventually ( `MIN_IFC_DCACHE.tensor_load_0.ctrl_state == 3'b0 ) ), "TensorLoad0 unit did not return to idle state after receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_tl1_et_link_rsp_error_followed_by_fsm_idle, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_tl1 != 2'b0)   |-> s_eventually ( `MIN_IFC_DCACHE.tensor_load_1.ctrl_state == 3'b0 ) ), "TensorLoad1 unit did not return to idle state after receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_ts_et_link_rsp_error_followed_by_fsm_idle, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_re != 2'b0)    |-> s_eventually ( `MIN_IFC_DCACHE.reduce.fsm_state == 4'b0 ) ), "TensorReduce unit did not return to idle state after receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_mh0_et_link_rsp_error_followed_by_fsm_idle, clock, reset,
   ( (`MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.bus_err != 2'b0)   |-> s_eventually ( `MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.state == 3'b0 ) ), "MissHandler0 did not return to idle state after receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_mh1_et_link_rsp_error_followed_by_fsm_idle, clock, reset,
   ( (`MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.bus_err != 2'b0)   |-> s_eventually ( `MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.state == 3'b0 ) ), "MissHandler1 did not return to idle state after receiving L2 error response");


/******************************
// Disabled since by the latest spec bus errors do not update TensorError, VERIF-1157
// Errors returned from L2 response must be reported, TensorErr[7] is set for 

//CacheOp_L2
`ASSERT_ALWAYS_MSG(sva_dcache_co_l2_et_link_rsp_error_sets_tensor_error_bit7_for_th0, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_co_l2[0]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][7] == 1'b1) ), "TensorError[0][7] is not set by CacheOp_L2 unit upon receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_co_l2_et_link_rsp_error_sets_tensor_error_bit7_for_th1, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_co_l2[1]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][7] == 1'b1) ), "TensorError[1][7] is not set by CacheOp_L2 unit upon receiving L2 error response");

//TensorLoad0
`ASSERT_ALWAYS_MSG(sva_dcache_tl0_et_link_rsp_error_sets_tensor_error_bit7_for_th0, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_tl0[0]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][7] == 1'b1) ), "TensorError[0][7] is not set by TensorLoad0 unit upon receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_tl0_et_link_rsp_error_sets_tensor_error_bit7_for_th1, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_tl0[1]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][7] == 1'b1) ), "TensorError[1][7] is not set by TensorLoad0 unit upon receiving L2 error response");

//TensorLoad1
`ASSERT_ALWAYS_MSG(sva_dcache_tl1_et_link_rsp_error_sets_tensor_error_bit7_for_th0, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_tl1[0]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][7] == 1'b1) ), "TensorError[0][7] is not set by TensorLoad1 unit upon receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_tl1_et_link_rsp_error_sets_tensor_error_bit7_for_th1, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_tl1[1]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][7] == 1'b1) ), "TensorError[1][7] is not set by TensorLoad1 unit upon receiving L2 error response");

// TensorStore
`ASSERT_ALWAYS_MSG(sva_dcache_ts_et_link_rsp_error_sets_tensor_error_bit7_for_th0, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_re[0]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][7] == 1'b1) ), "TensorError[0][7] is not set by TensorReduce unit upon receiving L2 error response");

`ASSERT_ALWAYS_MSG(sva_dcache_ts_et_link_rsp_error_sets_tensor_error_bit7_for_th1, clock, reset,
   ( (`MIN_IFC_DCACHE.bus_err_re[1]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][7] == 1'b1) ), "TensorError[1][7] is not set by TensorReduce unit upon receiving L2 error response");
*****************************/


// Memory exceptions and errors for CacheOp_L2, TensorLoad0&1 and TensorStore should set TensorErr[7] 

//CacheOp_L2
`ASSERT_ALWAYS_MSG(sva_dcache_co_l2_mem_error_sets_tensor_error_bit7_for_th0, clock, reset,
   ( (`MIN_IFC_DCACHE.cache_ops_err_flags_l2[0]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][7] == 1'b1) ), "TensorError[0][7] is not set after a memory error happened in CacheOp_L2");
 
`ASSERT_ALWAYS_MSG(sva_dcache_co_l2_mem_error_sets_tensor_error_bit7_for_th1, clock, reset,
   ( (`MIN_IFC_DCACHE.cache_ops_err_flags_l2[1]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][7] == 1'b1) ), "TensorError[1][7] is not set after a memory error happened in CacheOp_L2");
 
//TensorLoad0
`ASSERT_ALWAYS_MSG(sva_dcache_tl0_mem_error_sets_tensor_error_bit7_for_th0, clock, reset,
   ( (`MIN_IFC_DCACHE.tensor_load_err_flags_0[3]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][7] == 1'b1) ), "TensorError[0][7] is not set after a memory error happened in TensorLoad0");
 
`ASSERT_ALWAYS_MSG(sva_dcache_tl0_mem_error_sets_tensor_error_bit7_for_th1, clock, reset,
   ( (`MIN_IFC_DCACHE.tensor_load_err_flags_0[4]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][7] == 1'b1) ), "TensorError[1][7] is not set after a memory error happened in TensorLoad0");

//TensorLoad1
`ASSERT_ALWAYS_MSG(sva_dcache_tl1_mem_error_sets_tensor_error_bit7_for_th0, clock, reset,
   ( (`MIN_IFC_DCACHE.tensor_load_err_flags_1[3]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][7] == 1'b1) ), "TensorError[0][7] is not set after a memory error happened in TensorLoad1");

`ASSERT_ALWAYS_MSG(sva_dcache_tl1_mem_error_sets_tensor_error_bit7_for_th1, clock, reset,
   ( (`MIN_IFC_DCACHE.tensor_load_err_flags_1[4]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][7] == 1'b1) ), "TensorError[1][7] is not set after a memory error happened in TensorLoad1");

// TensorStore
`ASSERT_ALWAYS_MSG(sva_dcache_ts_mem_error_sets_tensor_error_bit7_for_th0, clock, reset,
   ( (`MIN_IFC_DCACHE.tensor_reduce_mem_err_flag[0]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][7] == 1'b1) ), "TensorError[0][7] is not set after a memory error happened in TensorReduce");

`ASSERT_ALWAYS_MSG(sva_dcache_ts_mem_error_sets_tensor_error_bit7_for_th1, clock, reset,
   ( (`MIN_IFC_DCACHE.tensor_reduce_mem_err_flag[1]) |-> ##[0:10] (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][7] == 1'b1) ), "TensorError[1][7] is not set after a memory error happened in TensorReduce");


// TensorLoad_TenB: tenb_credits: 0~6(max); 
// overflow: 6->7:
`ASSERT_ALWAYS_MSG(ERROR_sva_dcache_tenb_credits_overflow, clock, reset,
   ( (`MIN_IFC_DCACHE.tensor_load_1.tenb_credits == 3'h6) |-> (`MIN_IFC_DCACHE.tensor_load_1.tenb_credits_next != 3'h7) ), $sformatf("DCache: tenb_credits overflow (6->7)"))

// underflow: 0->7:
`ASSERT_ALWAYS_MSG(ERROR_sva_dcache_tenb_credits_underflow, clock, reset,
   ( (`MIN_IFC_DCACHE.tensor_load_1.tenb_credits == 3'h0) |-> (`MIN_IFC_DCACHE.tensor_load_1.tenb_credits_next != 3'h7) ), $sformatf("DCache: tenb_credits underflow (0->7)"))


  // Check writes with valid write data
genvar dram_block;
generate 
  for (dram_block = 0; dram_block < `DCACHE_LRAM_NUM_BANKS; dram_block++)
  begin : assert_lram_x
    `ASSERT_VALID_PORTS(LRAM_VALID_WRITE_L, clock, reset_for_assert, (`MIN_IFC_DCACHE.s4_da_write_data[dram_block].valid_l & `MIN_IFC_DCACHE.s4_da_write_en_ovr), `MIN_IFC_DCACHE.s4_da_write_data[dram_block].data[0*`DCACHE_LRAM_DATA_SIZE/2+:`DCACHE_LRAM_DATA_SIZE/2])
    `ASSERT_VALID_PORTS(LRAM_VALID_WRITE_H, clock, reset_for_assert, (`MIN_IFC_DCACHE.s4_da_write_data[dram_block].valid_h & `MIN_IFC_DCACHE.s4_da_write_en_ovr), `MIN_IFC_DCACHE.s4_da_write_data[dram_block].data[1*`DCACHE_LRAM_DATA_SIZE/2+:`DCACHE_LRAM_DATA_SIZE/2])
  end
endgenerate

  //Check split exceptions
`ASSERT(dcache_page_split_exception,(`MIN_IFC_DCACHE.s1_valid ==0) || (`MIN_IFC_DCACHE.s1_access_is_page_split == 0) || (`MIN_IFC_DCACHE.vm_enabled == 0),"Page-split access requested",reset_for_assert)


  // Dcache buffer array
// entry_count_available[3:0] valid range is 0 to 8 (max=8):
`ASSERT_ALWAYS_MSG(ERROR_dcache_ba_a1, clock, reset_for_assert, (`MIN_IFC_DCACHE.buffer_array.entry_count_available <= `DCACHE_BUFFER_SIZE), $sformatf("Number of free entries higher than maximum (8) in dcache buffer array!"))

`ASSERT_ALWAYS_MSG(ERROR_dcache_ba_a2, clock, reset_for_assert, (!(`MIN_IFC_DCACHE.buffer_array.s2_write_needed && !`MIN_IFC_DCACHE.buffer_array.entry_mask_available[`MIN_IFC_DCACHE.buffer_array.s2_write_entry])), $sformatf("Can't write (s2_write_needed) to a not-available entry in dcache buffer array!"))

`ASSERT_ALWAYS_MSG(ERROR_dcache_ba_a3, clock, reset_for_assert, (!(`MIN_IFC_DCACHE.buffer_array.s2_dealloc && `MIN_IFC_DCACHE.buffer_array.entry_mask_available[`MIN_IFC_DCACHE.buffer_array.s2_dealloc_entry])), $sformatf("Can't deallocate (s2_dealloc) an entry that is already available in dcache buffer array!"))

`ASSERT_ALWAYS_MSG(ERROR_dcache_ba_a4, clock, reset_for_assert, (!(`MIN_IFC_DCACHE.buffer_array.s1_dealloc_uc && `MIN_IFC_DCACHE.buffer_array.entry_mask_available[`MIN_IFC_DCACHE.buffer_array.s1_dealloc_uc_entry])), $sformatf("Can't deallocate (s1_dealloc_uc) an entry that is already available in dcache buffer array!"))

`ASSERT_ALWAYS_MSG(ERROR_dcache_ba_a5, clock, reset_for_assert, (!(`MIN_IFC_DCACHE.buffer_array.reduce_dealloc_req && `MIN_IFC_DCACHE.buffer_array.entry_mask_available[`MIN_IFC_DCACHE.buffer_array.reduce_dealloc_entry])), $sformatf("Can't deallocate (reduce_dealloc_req) an entry that is already available in dcache buffer array!"))


  // Dcache replay queue
// entry_count_available[3:0] valid range is 0 to 8 (max=8):
`ASSERT_ALWAYS_MSG(ERROR_cache_rpq_a1, clock, reset_for_assert, (`MIN_IFC_DCACHE.replay_queue.entry_count_available <= `DCACHE_REPLAYQ_SIZE), $sformatf("Number of free entries higher than maximum (8) in dcache replay queue!"))

`ASSERT_ALWAYS_MSG(ERROR_dcache_rpq_a2, clock, reset_for_assert, (!(`MIN_IFC_DCACHE.replay_queue.push_req && !`MIN_IFC_DCACHE.replay_queue.entry_mask_available[`MIN_IFC_DCACHE.replay_queue.push_entry])), $sformatf("Can't write (push_req) to a not-available entry in dcache replay queue!"))

`ASSERT_ALWAYS_MSG(ERROR_dcache_rpq_a3, clock, reset_for_assert, (!(`MIN_IFC_DCACHE.replay_queue.dealloc && `MIN_IFC_DCACHE.replay_queue.entry_mask_available[`MIN_IFC_DCACHE.replay_queue.dealloc_entry])), $sformatf("Can't deallocate an entry that is already available in dcache replay queue!"))



bit  sva_dcache_replayq_entry_count_vs_mask_chk;
bit  sva_dcache_buffarr_entry_count_vs_mask_chk;
initial begin
   if (!`et_value_plusargs("sva_dcache_replayq_entry_count_vs_mask_chk=%d", sva_dcache_replayq_entry_count_vs_mask_chk)) begin
      sva_dcache_replayq_entry_count_vs_mask_chk = 1;   // enabled by default
   end
   if (!`et_value_plusargs("sva_dcache_buffarr_entry_count_vs_mask_chk=%d", sva_dcache_buffarr_entry_count_vs_mask_chk)) begin
      sva_dcache_buffarr_entry_count_vs_mask_chk = 1;   // enabled by default
   end
end

// Dcache ReplayQueue: whenever the alloc_clk_en is 0, both entry_count_available (pre-allocated) and entry_mask_available (real allocated) must match:
`ASSERT_ALWAYS_MSG(ERROR_dcache_replayq_entry_count_vs_mask, clock, (reset_for_assert | ~sva_dcache_replayq_entry_count_vs_mask_chk), 
   ( (`MIN_IFC_DCACHE.replay_queue.alloc_clk_en == 1'b0) |-> 
     (`MIN_IFC_DCACHE.replay_queue.entry_count_available == $countones(`MIN_IFC_DCACHE.replay_queue.entry_mask_available[7:0])) ),
   $sformatf("Dcache: ReplayQ: when alloc_clk_en=0: mismatch: entry_count_available=%0d entry_mask_available[7:0]=0x%h", $sampled(`MIN_IFC_DCACHE.replay_queue.entry_count_available), $sampled(`MIN_IFC_DCACHE.replay_queue.entry_mask_available)) )

// Dcache BufferArray: whenever the alloc_clk_en is 0, both entry_count_available (pre-allocated) and entry_mask_available (real allocated) must match:
// Note: exclude TensorReduce case;
`ASSERT_ALWAYS_MSG(ERROR_dcache_buffarr_entry_count_vs_mask, clock, (reset_for_assert | ~sva_dcache_buffarr_entry_count_vs_mask_chk), 
   ( (`MIN_IFC_DCACHE.buffer_array.alloc_clk_en == 1'b0) &&
     /*(`MIN_IFC_DCACHE.buffer_array.reduce_count_available == 4'h4)*/  // change to code below
     (`MIN_IFC_DCACHE.reduce.fsm_state[3:0] == 4'h0)          // state=Reduce_Invalid 
     |-> 
     (`MIN_IFC_DCACHE.buffer_array.entry_count_available == $countones(`MIN_IFC_DCACHE.buffer_array.entry_mask_available[7:0])) ),
   $sformatf("Dcache: BuffArray: when alloc_clk_en=0 (exclude TensorReduce): mismatch: entry_count_available=%0d entry_mask_available[7:0]=0x%h", $sampled(`MIN_IFC_DCACHE.buffer_array.entry_count_available), $sampled(`MIN_IFC_DCACHE.buffer_array.entry_mask_available)) )



  // Dcache miss handler unit
integer writeback_count;

always_comb
begin
    writeback_count = 'b0;
    for(integer i = 0; i < `DCACHE_MH_FILE_SIZE; i++)
        if(`MIN_IFC_DCACHE.miss_handler_unit.mh_wb_valid[i])
            writeback_count += 'b1;
end
`ASSERT_ALWAYS_MSG(ERROR_writeback_count_le_1, clock, reset_for_assert, (writeback_count <= 1), $sformatf("Only 1 writeback request per cycle"))


  // Dcache reduce
`ASSERT_ALWAYS_MSG(ERROR_writearound_cnt_zero_resp, clock, reset_for_assert, !(`MIN_IFC_DCACHE.reduce.write_around_cnt_dec && !`MIN_IFC_DCACHE.reduce.last_packet_row && (`MIN_IFC_DCACHE.reduce.write_around_cnt == '0)), $sformatf("Received one unexpected WriteAround ACK/ERR (opcode = 0x%0x) in Minion 0x%0x",`MIN_IFC_DCACHE.reduce.l2_resp.opcode,`MIN_IFC_DCACHE.reduce.global_min_id))



// VERIF-3295: 
// when "scratchpad_ctrl" block FSM is processing a dcache_cfg_mode change (state != SCP_CTRL_All_Set):
// input signals "core_ctrl.cache_ctrl": d1split and scpenable should NOT change;
`ASSERT_ALWAYS_MSG(ERROR_dcache_cfg_update_input_stable, clock, reset_for_assert, 
   ( (`MIN_IFC_DCACHE.scratchpad_ctrl.state[1:0] != 2'b11)   // not SCP_CTRL_All_Set
     |->
     ($stable(`MIN_IFC_DCACHE.core_ctrl.cache_ctrl.d1split) && $stable(`MIN_IFC_DCACHE.core_ctrl.cache_ctrl.scpenable))
   ),
   $sformatf("Dcache: during cfg_mode update: input signals {d1split, scpenable} values should not change") ) 



// Vhecker for RTLMIN-5813 and RTLMIN-6000 (T0 trying to do TensorStore or TensorStore_SCP is being starved by T1 memory_access):
//
// My Idea: 
// When TensorStore (or TensorReduce) FSM is in a state requesting to allocate Buffer Array but it cannot get any BA entry, 
// but in the meantime if other memory access (from Thread 1) is able to allocate Buffer Array multiple times ....  
// If such a condition is detected, we flag it as an error ...
//
// Xavier's response:
// I think you are right. 
// The signals that you could use for this have been involved in the bugs.
// In dcache_reduce.v there is the signal "ba_entries_required" that indicates that tensor_store/reduce is 
// willing to allocate entries. This signal was wrong, so if we used it we would end up not detecting the issue. 
// The checker should use the FSM states to identify that it is in a state willing to do allocations.
// Together with this, the signal "ba_alloc" confirms an allocation.
// On the other hand, to know that thread 1 is allocating entries you can just check (at dcache_top.v) that 
// signal "s0_core_alloc_rq_val" is set together with "s0_core_req.dest.thread_id". 
// I think that any instruction cancellation in S0 is already reflected in "s0_core_alloc_rq_val", 
// so no need to further validate the signal.

logic  treduce_fsm_need_alloc_buffarr;      // for both TensorReduce (including both Recv and Send) and TensorStore (including TS_from_SCP)
int    num_of_buffarr_alloc_by_others_v1;   // usually by Thread 1 memory_access (during time of treduce_fsm_need_alloc_buffarr)
int    num_of_buffarr_alloc_by_others_v2;   // usually by Thread 1 memory_access (during time of treduce_fsm_need_alloc_buffarr)
int    max_num_of_buffarr_alloc_by_others_v1;
int    max_num_of_buffarr_alloc_by_others_v2;
bit    en_rtlmin_5813_6000_check1;

initial begin
   if(!`et_value_plusargs("max_num_of_buffarr_alloc_by_others_v1=%0d", max_num_of_buffarr_alloc_by_others_v1)) begin
      max_num_of_buffarr_alloc_by_others_v1 = 2;   // default value 
   end
   if(!`et_value_plusargs("max_num_of_buffarr_alloc_by_others_v2=%0d", max_num_of_buffarr_alloc_by_others_v2)) begin
      max_num_of_buffarr_alloc_by_others_v2 = 20;   // default value 
   end

   if(!`et_value_plusargs("en_rtlmin_5813_6000_check1=%0d", en_rtlmin_5813_6000_check1)) begin
      en_rtlmin_5813_6000_check1 = 1'b1;   // enabled by default
   end
end

// when Reduce FSM in State which needs to allocate BA entry:
assign  treduce_fsm_need_alloc_buffarr = 
           ((`MIN_IFC_DCACHE.reduce.fsm_state == 4'b0110 /*Reduce_Wait_Data*/) || 
            (`MIN_IFC_DCACHE.reduce.fsm_state == 4'b0011 /*Reduce_Send_Ready*/) ||
            (`MIN_IFC_DCACHE.reduce.fsm_state == 4'b0101 /*Reduce_Send_Data*/) ||
            (`MIN_IFC_DCACHE.reduce.fsm_state == 4'b1000 /*Reduce_Store_Data*/) ); 

always @(posedge `MIN_IFC_DCACHE.clock) begin: rtlmin_5813_6000_check1
   if(`MIN_IFC_DCACHE.reset === 1'b1) begin
      num_of_buffarr_alloc_by_others_v1 = 0;
      num_of_buffarr_alloc_by_others_v2 = 0;
   end
   else if(`MIN_IFC_DCACHE.reset === 1'b0) begin
      // when (treduce_fsm_need_alloc_buffarr==1): 
      if(treduce_fsm_need_alloc_buffarr == 1'b1) begin
         // if others (not Reduce FSM) still (conitnuously) allocates BA entry (when Reduce should have allocated but cannot):

         // this code belew is my idea:
         //if((`MIN_IFC_DCACHE.buffer_array.alloc_req_pre_applied == 1'b1) && 
         //   (`MIN_IFC_DCACHE.buffer_array.alloc_req_val == 1'b1) 
         //   &&

         // this code belew is Xavier's suggestion: 
         if((`MIN_IFC_DCACHE.s0_core_alloc_rq_val == 1'b1) &&
            (`MIN_IFC_DCACHE.s0_core_req.dest.thread_id == 1'b1) 
            &&   
            // need to qualify with conditions below:
            (`MIN_IFC_DCACHE.buffer_array.reduce_count_available[3:0] > 4'h2) &&   // should be >2 or >0 ...???
            (`MIN_IFC_DCACHE.reduce.reg_count != 'b0)                              // (reg_count=='b0) means it has already processed ALL VPU regs 
         ) begin
            // without consider "address_available" or "s2_valid" conditions:
            // they might cause temporary starvation, but not real deadlock (???)
            num_of_buffarr_alloc_by_others_v2++;

            if(num_of_buffarr_alloc_by_others_v2 > max_num_of_buffarr_alloc_by_others_v2) begin
               if(en_rtlmin_5813_6000_check1 == 1'b1) begin
                  $error("%0t ERROR: minion_core_dcache_assert: while Reduce FSM waiting to allocate BA, but Others has allocated BA multiple times: num_of_buffarr_alloc_by_others_v2=%0d", $time, num_of_buffarr_alloc_by_others_v2);
                  // add some delay before stop simulation:
                  repeat(50) @(posedge `MIN_IFC_DCACHE.clock); 
                  $finish(1); 
               end
               else begin
                  $display("%0t WARNING: minion_core_dcache_assert: while Reduce FSM waiting to allocate BA, but Others has allocated BA multiple times: num_of_buffarr_alloc_by_others_v2=%0d", $time, num_of_buffarr_alloc_by_others_v2);
               end
            end

            // further: if consider ""address_available" or "s2_valid" conditions:
            // also need to qualify with conditions below: might cause temporary starvation, but not real deadlock (???)
            if(!((`MIN_IFC_DCACHE.reduce.fsm_state == 4'b1000 /*Reduce_Store_Data*/) && 
                 (`MIN_IFC_DCACHE.reduce.address_available == 1'b0)) &&               // TensorStore is blocked by PA unavailable (due to other s0_reqs)
               !(((`MIN_IFC_DCACHE.reduce.fsm_state == 4'b0110 /*Reduce_Wait_Data*/) || 
                  (`MIN_IFC_DCACHE.reduce.fsm_state == 4'b0011 /*Reduce_Send_Ready*/)) &&
                 (`MIN_IFC_DCACHE.reduce.s2_valid == 1'b1) )                          // Reduce_receiver is blocked by (other) s2_valid (from Thread 1) 
            ) begin
               num_of_buffarr_alloc_by_others_v1++;
               //$display("%0t WARNING: minion_core_dcache_assert: while Reduce FSM waiting to allocate BA, but Others still allocated BA %0d times", $time, num_of_buffarr_alloc_by_others_v1);

               if(num_of_buffarr_alloc_by_others_v1 > max_num_of_buffarr_alloc_by_others_v1) begin
                  if(en_rtlmin_5813_6000_check1 == 1'b1) begin
                     $error("%0t ERROR: minion_core_dcache_assert: while Reduce FSM waiting to allocate BA, but Others has allocated BA multiple times: num_of_buffarr_alloc_by_others_v1=%0d", $time, num_of_buffarr_alloc_by_others_v1);
                     // add some delay before stop simulation:
                     repeat(50) @(posedge `MIN_IFC_DCACHE.clock); 
                     $finish(1); 
                  end
                  else begin
                     $display("%0t WARNING: minion_core_dcache_assert: while Reduce FSM waiting to allocate BA, but Others has allocated BA multiple times: num_of_buffarr_alloc_by_others_v1=%0d", $time, num_of_buffarr_alloc_by_others_v1);
                  end
               end
            end
         end

         // if Reduce FSM allocates a BA entry: reset the above counter ("num"):
         if(`MIN_IFC_DCACHE.buffer_array.reduce_alloc_req == 1'b1) begin 
            num_of_buffarr_alloc_by_others_v1 = 0; 
            num_of_buffarr_alloc_by_others_v2 = 0; 
         end
      end

      if(treduce_fsm_need_alloc_buffarr == 1'b0) begin
         if(num_of_buffarr_alloc_by_others_v1 > 0) begin
            num_of_buffarr_alloc_by_others_v1 = 0; 
         end
         if(num_of_buffarr_alloc_by_others_v2 > 0) begin
            num_of_buffarr_alloc_by_others_v2 = 0; 
         end
      end
   end
end  // rtlmin_5813_6000_check1



// Checker/assertion 2 for RTLMIN-5813 and RTLMIN-6000:
// If signal "buffer_array.reduce_alloc_req" is asserted (meaning Reduce unit has just allocated a BA entry), 
// we should see that signal "reduce.ba_entries_required" (or the older version signal "reduce.wait_data_from_sender" 
// in the older RTL before any fix) must already being 1 (level signal) .... Otherwise flag an error.

bit  en_rtlmin_5813_6000_check2;
initial begin
   if(!`et_value_plusargs("en_rtlmin_5813_6000_check2=%0d", en_rtlmin_5813_6000_check2)) begin
      en_rtlmin_5813_6000_check2 = 1'b1;   // enabled by default
   end
end

`ASSERT_ALWAYS_MSG(ERROR_dcache_rtlmin_5813_6000_check2, clock, (reset_for_assert | ~en_rtlmin_5813_6000_check2), 
   ( (`MIN_IFC_DCACHE.buffer_array.reduce_alloc_req == 1'b1)   // Reduce unit actually allocates a BA entry 
     |->
     (`MIN_IFC_DCACHE.reduce.ba_entries_required == 1'b1)
   ),
   $sformatf("Dcache: when signal <buffer_array.reduce_alloc_req> asserted, signal <reduce.ba_entries_required> is expected to be 1 (but actually not) ...") ) 





// VERIF-3428: 
// A "restart_tenb" indication should never happen if the TFMA that pairs with the TenB is already active.

bit  en_sva_restart_tenb_after_tfma_enabled;
initial begin
   if(!`et_value_plusargs("en_sva_restart_tenb_after_tfma_enabled=%0d", en_sva_restart_tenb_after_tfma_enabled)) begin
      en_sva_restart_tenb_after_tfma_enabled = 1'b1;   // enabled by default
   end
end

`ASSERT_ALWAYS_MSG(ERROR_restart_tenb_after_tfma_enabled, clock, (reset_for_assert | ~en_sva_restart_tenb_after_tfma_enabled), 
   ( (`MIN_IFC_DCACHE.tensor_load_1.tensor_ctrl.restart_b == 1'b1) ||
     (`MIN_IFC_DCACHE.tensor_load_1.tenb_flush == 1'b1)
     |->
     (`MIN_IFC_DCACHE.tensor_load_1.tfma_enabled == 1'b0)
   ),
   $sformatf("Dcache: A restart_tenb indication came when the TFMA that pairs with the TenB is already active") ) 



// to detect RTLMIN-6060 condition: in block "dcache.tensor_load_0":
// when FSM ctrl_state is in ML_CTRL_Wait_Start and (tensor_ctrl_use_tmask==1) and (save_tmask_pending==1):
// tensor_ctrl.tmask_bits[15:0] should not change (in the middle) when tensor_ctrl.tmask_ready not asserted ;

`ASSERT_ALWAYS_MSG(ERROR_dcache_tensorload_tmask_change_when_waiting, clock, reset_for_assert,
   (((`MIN_IFC_DCACHE.tensor_load_0.ctrl_state_next[2:0] == 3'b001) &&   // ML_CTRL_Wait_Start
     (`MIN_IFC_DCACHE.tensor_load_0.tensor_ctrl_use_tmask == 1'b1) &&
     (`MIN_IFC_DCACHE.tensor_load_0.save_tmask_pending == 1'b1) &&
     (`MIN_IFC_DCACHE.tensor_load_0.tensor_ctrl.tmask_ready == 1'b0) &&  // exclude tmask_ready==1
     ($past(`MIN_IFC_INTPIPE.csr_file.io_replay,1) == 1'b0)              // exclude io_replay==1 (VERIF-3574)
    ) |->
    ($stable(`MIN_IFC_DCACHE.tensor_load_0.tensor_ctrl.tmask_bits[15:0]))
   ),
   $sformatf("Dcache: when Tensor_Load_0 FSM is waiting for TensorMask: tmask_bits value changed unexpectedly (while tmask_ready==0 and io_replay==0)") ) 



// VERIF-3508: checker to directly detect the condition of RTLMIN-6078:
// For case: TL_TenB(pair_1) -> TFMA_TenB(pair_1) -> TL_TenB(pair_2) -> TFMA_TenB(pair_2) ....
// if TL_TenB(pair_2) comes but TFMA_TenB(pair_1) has not yet started processing (tfma_enabled not yet asserted):
// TL_TenB(pair_2) must be replayed (restart_tenb and tenb_flush);

// Note: in module "vpu.ctrl.ml.tensorfma": 
// We should only check at the beginning when signal "enabled" just asserts but "tfma_enabled" not yet asserts.
// We should exclude at the end after "tfma_enabled" already deasserts and "enabled" still remains asserting. 
// Note: 
// Don't need to do the above; instead, we can use signal "vpu.ctrl.ml.tensorfma.enabled_int" (instead of "enabled").

`ASSERT_ALWAYS_MSG(ERROR_tl_tenb_enter_ML_CTRL_NoT_Req_when_prev_tfma_not_start_process, clock, reset_for_assert,
   (((`MIN_VPU.ctrl.ml.tensorfma.enabled_int == 1'b1) &&                         // TFMA_Req starts
     (`MIN_VPU.ctrl.ml.tensorfma.f0_core_ctrl.tensorfma_ctrl.ten_b == 1'b1) &&   // and for TenB
     (`MIN_VPU.ctrl.ml.tensorfma.dcache_ctrl.tfma_enabled == 1'b0)               // but NOT start processing yet
     /* && (mon_tensorfma_tfma_enabled_sticky == 1'b0)   // exclude special behavior at the end after "tfma_enabled" deasserted */ 
    ) |-> 
    !((`MIN_IFC_DCACHE.tensor_load_1.ctrl_state_next == 3'b010) &&   // a new TL_TenB enters 3'b010=ML_CTRL_NoT_Req
      (`MIN_IFC_DCACHE.tensor_load_1.ctrl_state != 3'b010))
   ),
   $sformatf("(RTLMIN-6078 VERIF-3058 sva1) Minion: second TL_TenB enters FSM state ML_CTRL_NoT_Req when a previous TFMA has not yet started processing (tfma_enabled not yet asserted)") ) 

// Problem: TL_TenB(pair_2) could come and enter ML_CTRL_Wait_Start (due to "ctrl_start_previous_wait"), 
//          and then it does not necessarily have to be "restarted/replayed" ... ??? 
`ASSERT_ALWAYS_MSG(ERROR_tl_tenb_should_replay_if_prev_tfma_not_start_process, clock, reset_for_assert,
   (((`MIN_IFC_DCACHE.tensor_load_1.tensor_ctrl.start == 1'b1) &&
     (`MIN_IFC_DCACHE.tensor_load_1.tensor_ctrl.ctrl.tl_sel == 1'b1)        // TL_TenB Req start
     &&
     (`MIN_VPU.ctrl.ml.tensorfma.enabled_int == 1'b1) &&
     (`MIN_VPU.ctrl.ml.tensorfma.f0_core_ctrl.tensorfma_ctrl.ten_b == 1'b1) &&
     (`MIN_VPU.ctrl.ml.tensorfma.dcache_ctrl.tfma_enabled == 1'b0)
    ) |-> ##[1:5]
    (`MIN_IFC_DCACHE.tensor_load_1.tensor_ctrl.restart_b == 1'b1)
   ),
   $sformatf("(RTLMIN-6078 VERIF-3058 sva2) Minion: second TL_TenB did not get restart_b when a previous TFMA has not yet started processing (tfma_enabled not yet asserted)") ) 




// End-of-Sim or End-of-Test checks:

// End-of-Sim: ReplayQueue pre-allocation and actual/real/valid allocation must return back to 0:
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_replayq_must_empty, 
   ((`MIN_IFC_DCACHE.replay_queue.entry_count_available[3:0] == 4'h8) && 
    (`MIN_IFC_DCACHE.replay_queue.entry_mask_available[7:0] == 8'hff) &&
    (`MIN_IFC_DCACHE.replay_queue.age_id_mask_available[15:0] == 16'hffff) ), 
   $sformatf("DCache: At the end of test: Dcache ReplayQ not empty: entry_count_available=%0d entry_mask_available=0x%h age_id_mask_available=0x%h", 
             $sampled(`MIN_IFC_DCACHE.replay_queue.entry_count_available), $sampled(`MIN_IFC_DCACHE.replay_queue.entry_mask_available), $sampled(`MIN_IFC_DCACHE.replay_queue.age_id_mask_available)), 
   -1) 

// End-of-Sim: BufferArray pre-allocation and actual/real/valid allocation must return back to 0:
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_buffarr_must_empty, 
   ((`MIN_IFC_DCACHE.buffer_array.entry_count_available == 4'h8) && 
    (`MIN_IFC_DCACHE.buffer_array.entry_mask_available == 8'hff) &&
    (`MIN_IFC_DCACHE.buffer_array.reduce_count_available == 4'h4) ), 
   $sformatf("DCache: At the end of test: Dcache BuffArray not empty: entry_count_available=%0d entry_mask_available=0x%h reduce_count_available=%0d", 
             $sampled(`MIN_IFC_DCACHE.buffer_array.entry_count_available), $sampled(`MIN_IFC_DCACHE.buffer_array.entry_mask_available), $sampled(`MIN_IFC_DCACHE.buffer_array.reduce_count_available)), 
   -1) 


// End-of-Sim: Tensor Load 0/1, Tensor Store/Reduce, Cache Ops, Cache Ops L2, Miss Handler 0/1 FSMs have all returned to state '0':

// Tensor_Load_0:
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_TL0_fsm_state_Idle, 
   ( (`MIN_IFC_DCACHE.tensor_load_0.ctrl_state[2:0] == 3'b0) &&   // ML_CTRL_Idle
     (`MIN_IFC_DCACHE.tensor_load_0.enabled == 1'b0) &&
     (`MIN_IFC_DCACHE.tensor_load_0.l2i_state[0][1:0] == 2'b0) &&   // ML_L2I_Wait_Req
     (`MIN_IFC_DCACHE.tensor_load_0.l2i_state[1][1:0] == 2'b0) &&   // ML_L2I_Wait_Req
     (`MIN_IFC_DCACHE.tensor_load_0.l2i_state[2][1:0] == 2'b0) &&   // ML_L2I_Wait_Req
     (`MIN_IFC_DCACHE.tensor_load_0.l2i_state[3][1:0] == 2'b0)      // ML_L2I_Wait_Req
   ),
   $sformatf("DCache: At the end of test: Dcache TL_0 FSM states did not return to Idle"),
   -1)

// Tensor_Load_1:
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_TL1_fsm_state_Idle, 
   ( (`MIN_IFC_DCACHE.tensor_load_1.ctrl_state[2:0] == 3'b0) &&   // ML_CTRL_Idle
     (`MIN_IFC_DCACHE.tensor_load_1.enabled == 1'b0) &&
     (`MIN_IFC_DCACHE.tensor_load_1.l2i_state[0][1:0] == 2'b0) &&   // ML_L2I_Wait_Req
     (`MIN_IFC_DCACHE.tensor_load_1.l2i_state[1][1:0] == 2'b0) &&   // ML_L2I_Wait_Req
     (`MIN_IFC_DCACHE.tensor_load_1.l2i_state[2][1:0] == 2'b0) &&   // ML_L2I_Wait_Req
     (`MIN_IFC_DCACHE.tensor_load_1.l2i_state[3][1:0] == 2'b0)      // ML_L2I_Wait_Req
   ),
   $sformatf("DCache: At the end of test: Dcache TL_1 FSM states did not return to Idle"),
   -1)

// End-of-Sim: All tenb_credits returned to D$ (VPU~DCache interaction
// `DCACHE_TL_TENB_CREDITS = 6:
`ASSERT_END_OF_SIM_MSG(ERROR_tenb_credits_returned_to_dcache, 
   (`MIN_IFC_DCACHE.tensor_load_1.tenb_credits == `DCACHE_TL_TENB_CREDITS), 
   $sformatf("DCache: At the end of test: Dcache: only %d tenb_credits (out of %0d) were returned to dcache", 
             $sampled(`MIN_IFC_DCACHE.tensor_load_1.tenb_credits), `DCACHE_TL_TENB_CREDITS), 
   -1)

// Tensor_Load_1: more to check: tensor_ctrl_restart, tenb_flush: =0; tenb_avail_entry[3:0]=4'b1111;
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_TL1_EoT_a2,
   ( (`MIN_IFC_DCACHE.tensor_load_1.tenb_avail_entry[3:0] == 4'hf) &&
     (`MIN_IFC_DCACHE.tensor_load_1.tensor_ctrl_restart == 1'b0) &&
     (`MIN_IFC_DCACHE.tensor_load_1.tenb_flush == 1'b0) 
   ),
   $sformatf("DCache: At the end of test: Dcache TL_1 tenb_avail_entry[3:0] did not return to initial value 4'b1111"),
   -1)
   

// Tensor Store/Reduce: check the next variables/states to be '0: fsm_state, atrans_state, 
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_TReduce_EoT_a1,
   ( (`MIN_IFC_DCACHE.reduce.fsm_state[3:0] == 4'b0) &&     // Reduce_Invalid
     (`MIN_IFC_DCACHE.reduce.atrans_state[2:0] == 3'b0)     // Atrans_Idle
   ),
   $sformatf("DCache: At the end of test: Dcache Tensor_Reduce/Store: fsm_state, atrans_state did not return to 0 ..."), 
   -1)
  
// Tensor Store/Reduce: check the next variables/states to be '0: ba_alloc_fifo_val, ba_op_fifo_valid, s4_send_pending_pre, s4_send_pending, address_for_l2_ready, write_around_cnt
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_TReduce_EoT_a2,
   ( (`MIN_IFC_DCACHE.reduce.write_around_cnt[3:0] == 4'b0) &&     
     (`MIN_IFC_DCACHE.reduce.ba_alloc_fifo_val == 1'b0) &&     
     (`MIN_IFC_DCACHE.reduce.ba_op_fifo_valid == 1'b0) &&  
     (`MIN_IFC_DCACHE.reduce.s4_send_pending_pre == 1'b0) &&  
     (`MIN_IFC_DCACHE.reduce.s4_send_pending == 1'b0) &&  
     (`MIN_IFC_DCACHE.reduce.address_for_l2_ready == 1'b0) 
   ),
   $sformatf("DCache: At the end of test: Dcache Tensor_Reduce/Store: signals write_around_cnt, ba_alloc_fifo_val, ba_op_fifo_valid, s4_send_pending, s4_send_pending_pre, etc did not return to 0 ..."), 
   -1)

// Tensor Reduce/Store: more signals to check: ba_alloc_fifo.counter, ba_data_send_fifo.counter, ba_op_fifo.counter, phys_addr_send_fifo.counter: all be 0:
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_TReduce_EoT_a3,
   ( (`MIN_IFC_DCACHE.reduce.ba_alloc_fifo.counter == 0) && 
     (`MIN_IFC_DCACHE.reduce.ba_data_send_fifo.counter == 0) && 
     (`MIN_IFC_DCACHE.reduce.ba_op_fifo.counter == 0) && 
     (`MIN_IFC_DCACHE.reduce.phys_addr_send_fifo.counter == 0) 
   ),
   $sformatf("DCache: At the end of test: Dcache Tensor_Reduce/Store: ba_alloc_fifo, ba_data_send_fifo, ba_op_fifo, phys_addr_send_fifo: counter did not return to 0 ..."), 
   -1)


// It still fires falsely in some cases: see Jira VERIF-3326 ... disable for now.
/**
// note: in some cases, reg_count does not go back to 0: for example: see test: tensor_reduce_auto_8c:

// Tensor Store/Reduce: check the next variables/states to be '0: reg_count; 
// note: reg_count sometimes could be "x" (never initialized or used);
// note: when signal "action" = 2'b10 (DCACHE_REDUCE_DO_NOTHING): this property does not hold;    
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_TReduce_EoT_a4,
   ((`MIN_IFC_DCACHE.reduce.action == `DCACHE_REDUCE_DO_NOTHING) ||
    (((`MIN_IFC_DCACHE.reduce.reg_count.cnt[7:0] == 8'h0) || (`MIN_IFC_DCACHE.reduce.reg_count.cnt[7:0] === 8'bxxxx_xxxx)) &&     
     ((`MIN_IFC_DCACHE.reduce.reg_count.rc.ov == 1'b0) || (`MIN_IFC_DCACHE.reduce.reg_count.rc.ov === 1'bx)) &&     
     ((`MIN_IFC_DCACHE.reduce.reg_count.rc.rows[3:0] == 4'b0) || (`MIN_IFC_DCACHE.reduce.reg_count.rc.rows[3:0] === 4'bxxxx)) &&     
     ((`MIN_IFC_DCACHE.reduce.reg_count.rc.cols[2:0] == 3'b0) || (`MIN_IFC_DCACHE.reduce.reg_count.rc.cols[2:0] === 3'bxxx)) )
   ),
   $sformatf("DCache: At the end of test: Dcache Tensor_Reduce/Store: signals reg_count did not return to 0 ..."), 
   -1)
**/


// After a TensorReduce or TensorStore completes (w/o error condition or DO_THING case):
// its "reg_count" should go back to 0;

// we need to track FSM in Tensor_Reduce/Store:
logic   my_reduce_in_process;  // for both TensorReduce and TensorStore
logic   my_reduce_in_process_d1;       // 1-cycle delayed version
logic   my_reduce_err_or_donothing;    // for both TensorReduce and TensorStore
logic   my_reduce_err_or_donothing_sticky;    // keep asserting 
logic   my_reduce_complete;            // 1-cycle pulse

// in_process: when fsm_state is Not in Reduce_Invalid state
assign  my_reduce_in_process = (`MIN_IFC_DCACHE.reduce.fsm_state != 4'h0);  
assign  my_reduce_complete = (my_reduce_in_process_d1 & ~my_reduce_in_process);

// this signal is only 1-cycle:
assign  my_reduce_err_or_donothing = 
           ((my_reduce_in_process == 1'b1) && 
            (((`MIN_IFC_DCACHE.reduce.fsm_state == 4'h2 /*Reduce_New_Req*/) && `MIN_IFC_DCACHE.reduce.err_partner_id) ||
             ((`MIN_IFC_DCACHE.reduce.fsm_state == 4'h2 /*Reduce_New_Req*/) && `MIN_IFC_DCACHE.reduce.err_operation) ||
             ((`MIN_IFC_DCACHE.reduce.fsm_state == 4'h2 /*Reduce_New_Req*/) && (`MIN_IFC_DCACHE.reduce.reg_count == 0)) ||
             ((`MIN_IFC_DCACHE.reduce.fsm_state == 4'h2 /*Reduce_New_Req*/) && (`MIN_IFC_DCACHE.reduce.action == `DCACHE_REDUCE_DO_NOTHING)) ||
             ((`MIN_IFC_DCACHE.reduce.fsm_state == 4'h8 /*Reduce_Store_Data*/) && (`MIN_IFC_DCACHE.reduce.atrans_state == 3'h6 /*Atrans_Fail*/))) 
           );

always @(posedge `MIN_IFC_DCACHE.clock) begin
   my_reduce_in_process_d1 <= my_reduce_in_process;

   if(`MIN_IFC_DCACHE.reset === 1'b1) begin
      my_reduce_err_or_donothing_sticky <= 1'b0;
   end
   else if(`MIN_IFC_DCACHE.reset === 1'b0) begin
      // create a sticky version:
      if(my_reduce_err_or_donothing == 1'b1) begin
         my_reduce_err_or_donothing_sticky <= 1'b1;
      end 
      if((my_reduce_complete == 1'b1) && (my_reduce_err_or_donothing_sticky == 1'b1)) begin
         my_reduce_err_or_donothing_sticky <= 1'b0;
      end
   end
end

`ASSERT_ALWAYS_MSG (ERROR_dcache_reduce_reg_count_cleared_a1, clock, reset_for_assert, 
   (((my_reduce_complete == 1'b1) && (my_reduce_err_or_donothing_sticky == 1'b0))
    |-> 
    ((`MIN_IFC_DCACHE.reduce.reg_count.cnt[7:0] == 8'h0) &&
     (`MIN_IFC_DCACHE.reduce.reg_count.rc.ov == 1'b0) &&
     (`MIN_IFC_DCACHE.reduce.reg_count.rc.rows[3:0] == 4'b0) &&
     (`MIN_IFC_DCACHE.reduce.reg_count.rc.cols[2:0] == 3'b0) )
   ),
   $sformatf("Dcache: when TReduce or TStore completed (w/o Error or DO_NOTHING): signal reg_count did not return to 0") 
)




// Cache Ops: check the next variables/states to be '0: state, err_flags:
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_cache_op_EoT_a1,
   ( (`MIN_IFC_DCACHE.cache_op_unit.state[3:0] == 4'b0) &&     // Cache_Op_Invalid
     (`MIN_IFC_DCACHE.cache_op_unit.err_flags[0][1:0] == 2'b0) && 
     (`MIN_IFC_DCACHE.cache_op_unit.err_flags[1][1:0] == 2'b0) 
   ),
   $sformatf("DCache: At the end of test: Dcache CacheOp: FSM State and err_flags did not return to 0 ..."), 
   -1)

// Miss Handler 0/1: check the next variables/states to be '0:
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_miss_handler_EoT_a1,
   ( (`MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.state[3:0] == 4'b0) &&     // MH_Invalid
     (`MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.state[3:0] == 4'b0)        // MH_Invalid
   ),
   $sformatf("DCache: At the end of test: Dcache MissHandler 0 or 1: FSM State did not return to 0 ..."), 
   -1)


// note: dcache_idle might not be 1 ... see failure: tbox_tbox_test
 
// DCache Scratchapd Control:
// 1) Input port dcache_idle must be '1 (this implies that RQ has not any used entries)
// 2) Internal FSM must be in state = SCP_CTRL_All_Set;
// Note:
//  1) exclude when the Dcache is still in reset at EoT (possible);
//  2) "dcache_idle==1" condition failed in "tbox_test": disable for this specific test;

bit  dis_sva_dcache_scp_ctrl_EoT_a1 = 1'b0;
initial begin
   if(!(`et_value_plusargs("dis_sva_dcache_scp_ctrl_EoT_a1=%0d", dis_sva_dcache_scp_ctrl_EoT_a1))) begin
      dis_sva_dcache_scp_ctrl_EoT_a1 = 1'b0;
   end
end

`ASSERT_END_OF_SIM_MSG(ERROR_dcache_scp_ctrl_EoT_a1,
   ((dis_sva_dcache_scp_ctrl_EoT_a1 == 1'b1) || (`MIN_IFC_DCACHE.reset == 1'b1) || 
    ((`MIN_IFC_DCACHE.reset == 1'b0) &&
     (`MIN_IFC_DCACHE.scratchpad_ctrl.dcache_idle == 1'b1) && 
     (`MIN_IFC_DCACHE.scratchpad_ctrl.state[1:0] == 2'b11))       // state = SCP_CTRL_All_Set
   ),
   $sformatf("DCache: At the end of test: ScratchPad_Ctrl: FSM State and dcache_idle did not return to expected value ..."), 
   -1)



// At Dcache top, the L2 interfaces must be flushed and not having any pending transaction:
// 1) l2_resp_valid_int = 0
// 2) l2_miss_req_valid = 0
// 3) l2_evict_req_valid = 0
`ASSERT_END_OF_SIM_MSG(ERROR_dcache_l2_intf_EoT_a1,
   ( (`MIN_IFC_DCACHE.l2_resp_valid_int == 1'b0) &&
     (`MIN_IFC_DCACHE.l2_miss_req_valid[2:0] == 3'b0) &&
     (`MIN_IFC_DCACHE.l2_evict_req_valid[1:0] == 2'b0) 
   ),
   $sformatf("DCache: At the end of test: Dcache L2 Intf: l2_resp_valid_int, l2_miss_req_valid, l2_evict_req_valid are not 0 ..."), 
   -1)

































 







