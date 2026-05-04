//==================================================================
//Name: minion_core_intpipe_cvg_inst.svh
//Path: dv/minion_common/cov
//Description: MINION CORE Intpipe assertions
//Owner: Nikola Rajovic, Goran Ognjanovic
//==================================================================

// assertion usage example:
// `ASSERT_ALWAYS_MSG(ERROR_nameOfAssertion, clk, reset, (assert always expression), $sformatf("your strings with %stuff", $sampled(var) ) )


//==================================================================
// Intpipe - exclusive mode assertions
//==================================================================
logic csr_excl_mode;
logic csr_excl_mode_transition;
logic csr_excl_mode_sel;
logic id_thread_id;
logic id_valid_qual;
logic id_xcpt;
logic dcache_traffic_done;
logic vpu_instructions_done;

assign csr_excl_mode            = `MIN_IFC_INTPIPE.csr_excl_mode;
assign csr_excl_mode_transition = `MIN_IFC_INTPIPE.csr_excl_mode_transition;
assign csr_excl_mode_sel        = `MIN_IFC_INTPIPE.csr_excl_mode_sel;
assign id_thread_id             = `MIN_IFC_INTPIPE.id_thread_id;
assign id_valid_qual            = `MIN_IFC_INTPIPE.id_valid_qual;
assign id_xcpt                  = `MIN_IFC_INTPIPE.id_xcpt;
assign dcache_traffic_done      = `MIN_IFC_INTPIPE.id_dcache_idle;
assign vpu_instructions_done    = !(`MIN_IFC_INTPIPE.id_vpu_ctrl.tfma_enabled ||
                                    `MIN_IFC_INTPIPE.id_vpu_ctrl.tfma_wrrf_enabled ||
                                    `MIN_IFC_INTPIPE.id_vpu_ctrl.tquant_enabled ||
                                    `MIN_IFC_INTPIPE.id_vpu_ctrl.reduce_enabled);

`ASSERT_ALWAYS_MSG(sva_during_excl_mode_no_other_thread_inst, clock, reset,
    ( ((csr_excl_mode == 0)  && (csr_excl_mode_transition == 1)) |-> !((csr_excl_mode_sel != id_thread_id) && id_valid_qual)[*1:$] ##1 ((csr_excl_mode == 1)  && (csr_excl_mode_transition == 1)) ),
    "While in exclusive mode for one thread, we shouldn't see an instruction dispatched for the other thread");

`ASSERT_ALWAYS_MSG(sva_enter_excl_mode_transition, clock, reset,
    ( ((csr_excl_mode == 0)  && (csr_excl_mode_transition == 1)) |=> ((csr_excl_mode == 1)  && (csr_excl_mode_transition == 0)) ),
    "When entering exclusive mode transition should last one cycle");

`ASSERT_ALWAYS_MSG(sva_exit_excl_mode_transition, clock, reset,
    ( ((csr_excl_mode == 1)  && (csr_excl_mode_transition == 1)) |=> ((csr_excl_mode == 0)  && (csr_excl_mode_transition == 0)) ),
    "When exiting exclusive mode transition should last one cycle");

`ASSERT_ALWAYS_MSG(sva_enter_excl_mode_wait_dcache_and_vpu_inst_done, clock, reset,
    ( ((csr_excl_mode == 0)  && (csr_excl_mode_transition == 1)) |-> !id_valid_qual[*0:$] ##1 dcache_traffic_done ),//(dcache_traffic_done && vpu_instructions_done) ), //VERIF-2782
    "When entering exclusive mode, no new instruction can be issued until all dcache traffic and VPU instructions are done");

`ASSERT_ALWAYS_MSG(sva_exit_excl_mode_wait_dcache_and_vpu_inst_done, clock, reset,
    ( ((csr_excl_mode == 1)  && (csr_excl_mode_transition == 1)) |-> !id_valid_qual[*0:$] ##1 dcache_traffic_done ),//(dcache_traffic_done && vpu_instructions_done) ), //VERIF-2782
    "When exiting exclusive mode, no new instruction can be issued until all dcache traffic and VPU instructions are done");


csr_ad_t ex_csr_addr;
csr_ad_t tag_csr_addr;
csr_ad_t mem_csr_addr;
csr_ad_t wb_csr_addr;

assign ex_csr_addr  = csr_ad_t'(`MIN_IFC_INTPIPE.ex_reg_inst[31:20]);
assign tag_csr_addr = csr_ad_t'(`MIN_IFC_INTPIPE.tag_reg_inst[31:20]);
assign mem_csr_addr = csr_ad_t'(`MIN_IFC_INTPIPE.mem_reg_inst[31:20]);
assign wb_csr_addr  = csr_ad_t'(`MIN_IFC_INTPIPE.wb_reg_inst[31:20]);

`ASSERT_ALWAYS_MSG(sva_no_gsc_during_excl_mode_csr_write, clock, reset,
    ( (`MIN_IFC_INTPIPE.id_valid_qual == 1 && `MIN_IFC_INTPIPE.id_ctrl.gsc == 1) 
      |-> 
      (!((`MIN_IFC_INTPIPE.id_thread_id != `MIN_IFC_INTPIPE.ex_thread_id) && 
         (`MIN_IFC_INTPIPE.ex_valid_qual == 1) && 
         (`MIN_IFC_INTPIPE.ex_ctrl.csr != core_csr_ctrl_R) && 
         (`MIN_IFC_INTPIPE.ex_ctrl.csr != core_csr_ctrl_N) && 
         (`MIN_IFC_INTPIPE.ex_ctrl.csr != core_csr_ctrl_I) && 
         (ex_csr_addr == csr_ad_EXCL_MODE)) 
        &&
       !((`MIN_IFC_INTPIPE.id_thread_id != `MIN_IFC_INTPIPE.tag_thread_id) && 
         (`MIN_IFC_INTPIPE.tag_valid_qual == 1) &&
         (`MIN_IFC_INTPIPE.tag_ctrl.csr != core_csr_ctrl_R) &&
         (`MIN_IFC_INTPIPE.tag_ctrl.csr != core_csr_ctrl_N) && 
         (`MIN_IFC_INTPIPE.tag_ctrl.csr != core_csr_ctrl_I) && 
         (tag_csr_addr == csr_ad_EXCL_MODE))
        &&
       !((`MIN_IFC_INTPIPE.id_thread_id != `MIN_IFC_INTPIPE.mem_thread_id) && 
         (`MIN_IFC_INTPIPE.mem_valid_qual == 1) && 
         (`MIN_IFC_INTPIPE.mem_ctrl.csr != core_csr_ctrl_R) &&
         (`MIN_IFC_INTPIPE.mem_ctrl.csr != core_csr_ctrl_N) && 
         (`MIN_IFC_INTPIPE.mem_ctrl.csr != core_csr_ctrl_I) && 
         (mem_csr_addr == csr_ad_EXCL_MODE))   
        &&    
       !((`MIN_IFC_INTPIPE.id_thread_id != `MIN_IFC_INTPIPE.wb_thread_id) && 
         (`MIN_IFC_INTPIPE.wb_valid == 1) && 
         (`MIN_IFC_INTPIPE.wb_ctrl.csr != core_csr_ctrl_R) &&
         (`MIN_IFC_INTPIPE.wb_ctrl.csr != core_csr_ctrl_N) && 
         (`MIN_IFC_INTPIPE.wb_ctrl.csr != core_csr_ctrl_I) && 
         (wb_csr_addr == csr_ad_EXCL_MODE) &&
         (`MIN_IFC_INTPIPE.csr_excl_mode_transition == 1)) ) ),
    "Do not start a gather/scatter instruction if the other thread will write the excl mode CSR");


//==================================================================
// Intpipe - fence assertions
//==================================================================
logic id_ctrl_fence_inst;
logic ex_dcache_req_valid;
logic ex_thread_id;
logic[1:0] id_dcache_ordered;

assign id_ctrl_fence_inst  = `MIN_IFC_INTPIPE.id_ctrl.fence;
assign ex_dcache_req_valid = `MIN_IFC_INTPIPE.ex_dcache_req_valid;
assign ex_thread_id        = `MIN_IFC_INTPIPE.ex_thread_id;
assign id_dcache_ordered   = `MIN_IFC_INTPIPE.id_dcache_ordered;

`ASSERT_ALWAYS_MSG(sva_fence_no_new_dcache_req_thread_id_0, clock, reset,
    ( (id_ctrl_fence_inst && id_valid_qual && id_thread_id == 0 && !id_xcpt) |=> ((ex_dcache_req_valid == 0 && ex_thread_id == 0 && id_dcache_ordered[0] == 0) || ex_thread_id == 1)[*0:$] ##1 (id_dcache_ordered[0] == 1) ),
    "While a FENCE is outstanding, there shouldn't be a new request to the dcache, thread id [0]");

`ASSERT_ALWAYS_MSG(sva_fence_no_new_dcache_req_thread_id_1, clock, reset,
    ( (id_ctrl_fence_inst && id_valid_qual && id_thread_id == 1 && !id_xcpt) |=> ((ex_dcache_req_valid == 0 && ex_thread_id == 1 && id_dcache_ordered[1] == 0) || ex_thread_id == 0)[*0:$] ##1 (id_dcache_ordered[1] == 1) ),
    "While a FENCE is outstanding, there shouldn't be a new request to the dcache, thread id [1]");  

//==================================================================
// Intpipe - mcode exception assertions
//==================================================================
logic [31:0] id_inst_bits;
logic [31:0] minstmatch_reg_0;
logic [31:0] minstmatch_reg_1;
logic minstmask_reg_enable_0;
logic minstmask_reg_enable_1;
logic [31:0] minstmask_reg_mask_0;
logic [31:0] minstmask_reg_mask_1;
logic wb_xcpt;
logic [`CORE_USED_XCPT_CAUSE_BITS-1:0] wb_xcpt_cause;
logic ex_valid_qual;
logic tag_valid_qual;
logic mem_valid_qual;
logic wb_valid;
logic wb_replay;
logic wb_thread_id;
logic wb_reg_gscing;
logic wb_csr_xcpt;
logic wb_reg_xcpt;

assign id_inst_bits           = `MIN_IFC_INTPIPE.id_inst_bits;
assign minstmatch_reg_0       = `MIN_IFC_INTPIPE.minstmatch_reg[0];
assign minstmatch_reg_1       = `MIN_IFC_INTPIPE.minstmatch_reg[1];
assign minstmask_reg_enable_0 = `MIN_IFC_INTPIPE.minstmask_reg[0].enable;   
assign minstmask_reg_enable_1 = `MIN_IFC_INTPIPE.minstmask_reg[1].enable;   
assign minstmask_reg_mask_0   = `MIN_IFC_INTPIPE.minstmask_reg[0].mask;
assign minstmask_reg_mask_1   = `MIN_IFC_INTPIPE.minstmask_reg[1].mask;
assign wb_xcpt                = `MIN_IFC_INTPIPE.wb_xcpt;
assign wb_thread_id           = `MIN_IFC_INTPIPE.wb_thread_id;
assign wb_xcpt_cause          = `MIN_IFC_INTPIPE.csr_file.cause.cause;
assign ex_valid_qual          = `MIN_IFC_INTPIPE.ex_valid_qual;
assign tag_valid_qual         = `MIN_IFC_INTPIPE.tag_valid_qual;
assign mem_valid_qual         = `MIN_IFC_INTPIPE.mem_valid_qual;
assign wb_valid               = `MIN_IFC_INTPIPE.wb_valid;
assign wb_replay              = `MIN_IFC_INTPIPE.wb_reg_replay || `MIN_IFC_INTPIPE.wb_csr_replay;
assign wb_reg_gscing          = `MIN_IFC_INTPIPE.wb_reg_gscing;
assign wb_csr_xcpt            = `MIN_IFC_INTPIPE.wb_csr_xcpt;
assign wb_reg_xcpt            = `MIN_IFC_INTPIPE.wb_reg_xcpt;

logic minstmatch_en_th0;
logic minstmatch_en_th1;
logic minstmatch_dis_th0;
logic minstmatch_dis_th1;
logic minstmatch_en_th0_id;
logic minstmatch_en_th1_id;
logic minstmatch_dis_th0_id;
logic minstmatch_dis_th1_id;
logic minstmatch_en_th0_ex;
logic minstmatch_en_th1_ex;
logic minstmatch_dis_th0_ex;
logic minstmatch_dis_th1_ex;
logic minstmatch_en_th0_tag;
logic minstmatch_en_th1_tag;
logic minstmatch_dis_th0_tag;
logic minstmatch_dis_th1_tag;

always @(posedge clock) begin
    minstmatch_en_th0 = 0;
    minstmatch_en_th1 = 0;
    minstmatch_dis_th0 = 0;
    minstmatch_dis_th1 = 0;    
    if (mem_valid_qual === 1 && minstmatch_en_th0_tag === 1) minstmatch_en_th0 = 1;
    if (mem_valid_qual === 1 && minstmatch_en_th1_tag === 1) minstmatch_en_th1 = 1;
    if (mem_valid_qual === 1 && minstmatch_dis_th0_tag === 1) minstmatch_dis_th0 = 1;
    if (mem_valid_qual === 1 && minstmatch_dis_th1_tag === 1) minstmatch_dis_th1 = 1;

    minstmatch_en_th0_tag = 0;
    minstmatch_en_th1_tag = 0;
    minstmatch_dis_th0_tag = 0;    
    minstmatch_dis_th1_tag = 0; 
    if (tag_valid_qual  === 1 && minstmatch_en_th0_ex === 1) minstmatch_en_th0_tag = 1; 
    if (tag_valid_qual  === 1 && minstmatch_en_th1_ex === 1) minstmatch_en_th1_tag = 1;
    if (tag_valid_qual  === 1 && minstmatch_dis_th0_ex === 1) minstmatch_dis_th0_tag = 1;
    if (tag_valid_qual  === 1 && minstmatch_dis_th1_ex === 1) minstmatch_dis_th1_tag = 1;

    minstmatch_en_th0_ex = 0; 
    minstmatch_en_th1_ex = 0;
    minstmatch_dis_th0_ex = 0;
    minstmatch_dis_th1_ex = 0;
    if (ex_valid_qual  === 1 && minstmatch_en_th0_id === 1) minstmatch_en_th0_ex = 1;
    if (ex_valid_qual  === 1 && minstmatch_en_th1_id === 1) minstmatch_en_th1_ex = 1;
    if (ex_valid_qual  === 1 && minstmatch_dis_th0_id === 1) minstmatch_dis_th0_ex = 1;
    if (ex_valid_qual  === 1 && minstmatch_dis_th1_id === 1) minstmatch_dis_th1_ex = 1; 

    minstmatch_en_th0_id = 0;
    minstmatch_en_th1_id = 0;
    minstmatch_dis_th0_id = 0;
    minstmatch_dis_th1_id = 0;
    if (id_valid_qual === 1 && id_thread_id === 0 && minstmask_reg_enable_0 === 1 && ((id_inst_bits & minstmask_reg_mask_0) === (minstmatch_reg_0 & minstmask_reg_mask_0))) minstmatch_en_th0_id = 1;
    if (id_valid_qual === 1 && id_thread_id === 1 && minstmask_reg_enable_1 === 1 && ((id_inst_bits & minstmask_reg_mask_1) === (minstmatch_reg_1 & minstmask_reg_mask_1))) minstmatch_en_th1_id = 1;
    if (id_valid_qual === 1 && id_thread_id === 0 && minstmask_reg_enable_0 === 0) minstmatch_dis_th0_id = 1;
    if (id_valid_qual === 1 && id_thread_id === 1 && minstmask_reg_enable_1 === 0) minstmatch_dis_th1_id = 1;     
end   


// minstmatch_en_th0 |=> (wb_xcpt && (wb_xcpt_cause == core_xcpt_cause_MCODE_INSTRUCTION....
// minstmatch_en_th0 |-> (wb_xcpt && (wb_xcpt_cause == core_xcpt_cause_MCODE_INSTRUCTION....


`ASSERT_ALWAYS_MSG(sva_minstmatch_mask_enabled_thread_id_0, clock, reset,
    (  //((id_valid_qual && id_thread_id ==0) && (minstmask_reg_enable_0 ==1)  && ((id_inst_bits & minstmask_reg_mask_0) == (minstmatch_reg_0 & minstmask_reg_mask_0))) 
       // ##1 (ex_valid_qual) 
       // ##1 (tag_valid_qual) 
       // ##1 (mem_valid_qual) 
       // |=> 
        minstmatch_en_th0
        |->
        (wb_xcpt && (wb_xcpt_cause == core_xcpt_cause_MCODE_INSTRUCTION || wb_xcpt_cause inside {core_xcpt_cause_BUS_ERROR, core_xcpt_cause_FETCH_BUS_ERROR, core_xcpt_cause_FETCH_ECC_ERROR,
                                                                                                 core_xcpt_cause_MEIP, core_xcpt_cause_SEIP, core_xcpt_cause_MSIP, 
                                                                                                 core_xcpt_cause_SSIP, core_xcpt_cause_MTIP, core_xcpt_cause_STIP,
                                                                                                 `ifdef CSR_U_INTERRUPTS 
                                                                                                 core_xcpt_cause_UEIP, core_xcpt_cause_USIP, core_xcpt_cause_UTIP,
                                                                                                 `endif 
                                                                                                 core_xcpt_cause_MBAD_RED, core_xcpt_cause_MIECO, core_xcpt_cause_FETCH_ACCESS, 
                                                                                                 core_xcpt_cause_ILLEGAL_INSTRUCTION, core_xcpt_cause_DEBUG, core_xcpt_cause_BREAKPOINT, 
                                                                                                 core_xcpt_cause_FETCH_PAGE_FAULT})) ),
    "If ID instruction matches MINSTMATCH/MASK CSRs and it is enabled, an mcode exception should be taken - thread_0");
    //Note: minstmatch_mask ASSERTs works ONLY in situations when we have clean instruction propagation through pipe stages, cycle by cycle, without stalls/exceptions/delays... (condition before implication "|=>")
    //Note: it is possible that exception of higher priority than M-Code occur!

`ASSERT_ALWAYS_MSG(sva_minstmatch_mask_enabled_thread_id_1, clock, reset,
    (  //((id_valid_qual && id_thread_id ==1) && (minstmask_reg_enable_1 ==1)  && ((id_inst_bits & minstmask_reg_mask_1) == (minstmatch_reg_1 & minstmask_reg_mask_1))) 
       // ##1 (ex_valid_qual) 
       // ##1 (tag_valid_qual) 
       // ##1 (mem_valid_qual) 
       // |=> 
        minstmatch_en_th1
        |->
        (wb_xcpt && (wb_xcpt_cause == core_xcpt_cause_MCODE_INSTRUCTION || wb_xcpt_cause inside {core_xcpt_cause_BUS_ERROR, core_xcpt_cause_FETCH_BUS_ERROR, core_xcpt_cause_FETCH_ECC_ERROR,
                                                                                                 core_xcpt_cause_MEIP, core_xcpt_cause_SEIP, core_xcpt_cause_MSIP, 
                                                                                                 core_xcpt_cause_SSIP, core_xcpt_cause_MTIP, core_xcpt_cause_STIP,
                                                                                                 `ifdef CSR_U_INTERRUPTS 
                                                                                                 core_xcpt_cause_UEIP, core_xcpt_cause_USIP, core_xcpt_cause_UTIP,
                                                                                                 `endif 
                                                                                                 core_xcpt_cause_MBAD_RED, core_xcpt_cause_MIECO, core_xcpt_cause_FETCH_ACCESS, 
                                                                                                 core_xcpt_cause_ILLEGAL_INSTRUCTION, core_xcpt_cause_DEBUG, core_xcpt_cause_BREAKPOINT, 
                                                                                                 core_xcpt_cause_FETCH_PAGE_FAULT})) ),
    "If ID instruction matches MINSTMATCH/MASK CSRs and it is enabled, an mcode exception should be taken - thread_1");
    //Note: minstmatch_mask ASSERTs works ONLY in situations when we have clean instruction propagation through pipe stages, cycle by cycle, without stalls/exceptions/delays... (condition before implication "|=>")
    //Note: it is possible that exception of higher priority than M-Code occur!
    
`ASSERT_ALWAYS_MSG(sva_minstmatch_mask_disabled_thread_id_0, clock, reset,
    (  //(id_valid_qual && (id_thread_id ==0) && (minstmask_reg_enable_0 ==0))  //&& ((id_inst_bits & minstmask_reg_mask_0) == (minstmatch_reg_0 & minstmask_reg_mask_0))) 
       // ##1 (ex_valid_qual)                                                   // minimizing the expression covers more possible errorenous cases  
       // ##1 (tag_valid_qual) 
       // ##1 (mem_valid_qual) 
       // |=> 
        minstmatch_dis_th0
        |->
        (wb_valid) || (wb_replay) || (wb_reg_gscing)|| (wb_xcpt && wb_xcpt_cause !== core_xcpt_cause_MCODE_INSTRUCTION) ),
    "If ID instruction matches MINSTMATCH/MASK CSRs and it is disabled, an mcode exception should not be taken - thread_0");
    //Note: minstmatch_mask ASSERTs works ONLY in situations when we have clean instruction propagation through pipe stages, cycle by cycle, without stalls/exceptions/delays... (condition before implication "|=>")

`ASSERT_ALWAYS_MSG(sva_minstmatch_mask_disabled_thread_id_1, clock, reset,
    (  //(id_valid_qual && (id_thread_id ==1) && (minstmask_reg_enable_1 ==0))  //&& ((id_inst_bits & minstmask_reg_mask_0) == (minstmatch_reg_0 & minstmask_reg_mask_0))) 
       // ##1 (ex_valid_qual)                                                   // minimizing the expression covers more possible errorenous cases  
       // ##1 (tag_valid_qual) 
       // ##1 (mem_valid_qual) 
       // |=> 
        minstmatch_dis_th1
        |->
        (wb_valid) || (wb_replay) || (wb_reg_gscing) || (wb_xcpt && wb_xcpt_cause !== core_xcpt_cause_MCODE_INSTRUCTION) ),
    "If ID instruction matches MINSTMATCH/MASK CSRs and it is disabled, an mcode exception should not be taken - thread_1");
    //Note: minstmatch_mask ASSERTs works ONLY in situations when we have clean instruction propagation through pipe stages, cycle by cycle, without stalls/exceptions/delays... (condition before implication "|=>")



//
// CSR TensorError[8]: TensorStore Config error:
//

logic  valid_tstore_config;    // only for TensorStore (exclude TensorStore_FromSCP)
assign valid_tstore_config = ((`MIN_IFC_INTPIPE.csr_file.tensor_store_wb_data.cols == 2'd0) && 
                              (`MIN_IFC_INTPIPE.csr_file.tensor_store_wb_data.coop != 2'd2)) ||
                             ((`MIN_IFC_INTPIPE.csr_file.tensor_store_wb_data.cols == 2'd1) && 
                              (`MIN_IFC_INTPIPE.csr_file.tensor_store_wb_data.coop[1] == 1'b0)) ||
                             ((`MIN_IFC_INTPIPE.csr_file.tensor_store_wb_data.cols == 2'd3) && 
                              (`MIN_IFC_INTPIPE.csr_file.tensor_store_wb_data.coop == 2'd0)) ; 
 
// TensorStore config error (exclude TensorStoreFromSCP): -> set Tensor_Error[0][8]=1:

`ASSERT_ALWAYS_MSG(sva_csr_file_tensor_store_config_err_sets_tensor_error_bit8, clock, reset,
   ( ( (`MIN_IFC_INTPIPE.csr_file.reg_tensor_store_en_qual == 1'b1) && 
       (`MIN_IFC_INTPIPE.csr_file.tensor_store_wb_data.scp == 1'b0) && 
       (valid_tstore_config == 1'b0) &&
       (`MIN_IFC_INTPIPE.csr_file.io_replay == 1'b0)    // Brendan: updated due to RTL change (exclude "replay" case) 
     ) |-> ##[1:5] 
     (`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][8] == 1'b1) 
   ), "TensorError[0][8] is not set after TensorStore config error");



//
// RTL: intpipe_csr_file: fcc_overflow[1:0] -> tensor_error[0][3]; fcc_overflow[3:2] -> tensor_error[1][3];
//

`ASSERT_ALWAYS_MSG(sva_csr_file_fcc_overflow_sets_tensor_error_t0_bit3, clock, reset,
   (  ( |`MIN_IFC_INTPIPE.csr_file.fcc_overflow[1:0] == 1'b1 )
      |-> ##[1:5]
      ( `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][3] == 1'b1 )
   ), "CSF_file: TensorError[0][3] is not set after FCC_overflow[1:0] asserted ...");

`ASSERT_ALWAYS_MSG(sva_csr_file_fcc_overflow_sets_tensor_error_t1_bit3, clock, reset,
   (  ( |`MIN_IFC_INTPIPE.csr_file.fcc_overflow[3:2] == 1'b1 )
      |-> ##[1:5]
      ( `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][3] == 1'b1 )
   ), "CSF_file: TensorError[1][3] is not set after FCC_overflow[3:2] asserted ...");


 
//
// Tensor_Quant but L1SCP disabled -> set Tensor_Error[0][4] 
//

// Thread 0:
`ASSERT_ALWAYS_MSG(sva_csr_file_tensor_quant_l1scp_dis_sets_tensor_error_t0_bit4, clock, reset,
   (  ( `MIN_IFC_INTPIPE.csr_file.tensor_quant_wen && 
        `MIN_IFC_INTPIPE.csr_file.tquant_scp(`MIN_IFC_INTPIPE.csr_file.reg_tensor_quant.trans) && 
        (`MIN_IFC_INTPIPE.csr_file.reg_ucache_control[1:0] != DcacheMode_Scratchpad) && 
        (`MIN_IFC_INTPIPE.csr_file.wb_thread_id == 1'b0)  )
      |-> ##[1:5]
      ( `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][4] == 1'b1 )
   ), "CSF_file: TensorError[0][4] is not set after Tensor_Quant (T0) but L1SCP disabled ...");

// Thread 1:
`ASSERT_ALWAYS_MSG(sva_csr_file_tensor_quant_l1scp_dis_sets_tensor_error_t1_bit4, clock, reset,
   (  ( `MIN_IFC_INTPIPE.csr_file.tensor_quant_wen && 
        `MIN_IFC_INTPIPE.csr_file.tquant_scp(`MIN_IFC_INTPIPE.csr_file.reg_tensor_quant.trans) && 
        (`MIN_IFC_INTPIPE.csr_file.reg_ucache_control[1:0] != DcacheMode_Scratchpad) && 
        (`MIN_IFC_INTPIPE.csr_file.wb_thread_id == 1'b1)  )
      |-> ##[1:5]
      ( `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][4] == 1'b1 )
   ), "CSF_file: TensorError[1][4] is not set after Tensor_Quant (T1) but L1SCP disabled ...");


//
// Tensor_FMA but L1SCP disabled -> set Tensor_Error[0][4] 
//

// Thread 0:
`ASSERT_ALWAYS_MSG(sva_csr_file_tensor_fma_l1scp_dis_sets_tensor_error_t0_bit4, clock, reset,
   (  ( `MIN_IFC_INTPIPE.csr_file.reg_tensor_fma_en_qual && 
        (`MIN_IFC_INTPIPE.csr_file.reg_ucache_control[1:0] != DcacheMode_Scratchpad) && 
        (`MIN_IFC_INTPIPE.csr_file.wb_thread_id == 1'b0)  )
      |-> ##[1:5]
      ( `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][4] == 1'b1 )
   ), "CSF_file: TensorError[0][4] is not set after Tensor_FMA but L1SCP disabled ...");

// Thread 1:
`ASSERT_ALWAYS_MSG(sva_csr_file_tensor_fma_l1scp_dis_sets_tensor_error_t1_bit4, clock, reset,
   (  ( `MIN_IFC_INTPIPE.csr_file.reg_tensor_fma_en_qual && 
        (`MIN_IFC_INTPIPE.csr_file.reg_ucache_control[1:0] != DcacheMode_Scratchpad) && 
        (`MIN_IFC_INTPIPE.csr_file.wb_thread_id == 1'b1)  )
      |-> ##[1:5]
      ( `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[1][4] == 1'b1 )
   ), "CSF_file: TensorError[1][4] is not set after Tensor_FMA but L1SCP disabled ...");


//
// Muxes the different results to the int RF write port
//

`ASSERT_ALWAYS_MSG(ERROR_uniqueRFWen, clock, reset_for_assert, (`MIN_IFC_INTPIPE.wb_dcache_wen + `MIN_IFC_INTPIPE.wb_div_wen + (`MIN_IFC_INTPIPE.wb_wen && !`MIN_IFC_INTPIPE.wb_ctrl.mem) + `MIN_IFC_INTPIPE.wb_fp_toint_wen <=1), "ERROR: multiple writes to int RF in the same cycle")
`ASSERT_ALWAYS_MSG(ERROR_uniqueRFWEN_X, clock, reset_for_assert, (`MIN_IFC_INTPIPE.wb_dcache_wen !== 1'bx && `MIN_IFC_INTPIPE.wb_div_wen !== 1'bx && (`MIN_IFC_INTPIPE.wb_wen && !`MIN_IFC_INTPIPE.wb_ctrl.mem) !== 1'bx + `MIN_IFC_INTPIPE.wb_fp_toint_wen !== 1'bx), "ERROR: wen to int RF is 'X'")


//
// Check valid ports
//

`ASSERT_VALID_PORTS(ERROR_intpipe_ipi_xs,        clock, reset_for_assert, |`MIN_IFC_INTPIPE.interrupts.ipi_with_pc,                      `MIN_IFC_INTPIPE.interrupts.ipi_pc)
`ASSERT_VALID_PORTS(ERROR_intpipe_id_valid_xs,   clock, reset_for_assert, `MIN_IFC_INTPIPE.id_valid_qual,                                {`MIN_IFC_INTPIPE.id_ctrl_to_ex_masked, `MIN_IFC_INTPIPE.id_rvc, `MIN_IFC_INTPIPE.id_flush_pipe})
`ASSERT_VALID_PORTS(ERROR_intpipe_id_xcpt_xs,    clock, reset_for_assert, `MIN_IFC_INTPIPE.id_xcpt,                                      `MIN_IFC_INTPIPE.id_xcpt_cause)
`ASSERT_VALID_PORTS(ERROR_intpipe_id_inst_xs,    clock, reset_for_assert, `MIN_IFC_INTPIPE.id_inst_en,                                   {`MIN_IFC_INTPIPE.id_pc, `MIN_IFC_INTPIPE.id_uses_alu, `MIN_IFC_INTPIPE.id_thread_id, `MIN_IFC_INTPIPE.id_csr_flush})
`ASSERT_VALID_PORTS(ERROR_intpipe_ex_ps_mask_xs, clock, reset_for_assert, `MIN_IFC_INTPIPE.ex_valid_qual_to_gsc,                         `MIN_IFC_INTPIPE.ex_vpu_ctrl.ps_mask)
`ASSERT_VALID_PORTS(ERROR_intpipe_ex_valid_xs,   clock, reset_for_assert, `MIN_IFC_INTPIPE.ex_pc_valid || `MIN_IFC_INTPIPE.gsc_pc_valid, {`MIN_IFC_INTPIPE.ex_ctrl_masked, `MIN_IFC_INTPIPE.ex_reg_rvc_masked, `MIN_IFC_INTPIPE.ex_reg_pc, `MIN_IFC_INTPIPE.ex_thread_id, `MIN_IFC_INTPIPE.ex_csr_flush})
`ASSERT_VALID_PORTS(ERROR_intpipe_ex_xcpt_xs,    clock, reset_for_assert, `MIN_IFC_INTPIPE.ex_xcpt,                                      `MIN_IFC_INTPIPE.ex_xcpt_cause)
`ASSERT_VALID_PORTS(ERROR_intpipe_tag_valid_xs,  clock, reset_for_assert, `MIN_IFC_INTPIPE.tag_pc_valid,                                 {`MIN_IFC_INTPIPE.tag_ctrl_masked, `MIN_IFC_INTPIPE.tag_reg_rvc_masked, `MIN_IFC_INTPIPE.tag_reg_pc, `MIN_IFC_INTPIPE.tag_uses_wdata, `MIN_IFC_INTPIPE.tag_csr_flush, `MIN_IFC_INTPIPE.tag_thread_id})
`ASSERT_VALID_PORTS(ERROR_intpipe_tag_xcpt_xs,   clock, reset_for_assert, `MIN_IFC_INTPIPE.tag_xcpt,                                     {`MIN_IFC_INTPIPE.tag_xcpt_cause, `MIN_IFC_INTPIPE.dcache_bp_xcpt, `MIN_IFC_INTPIPE.tag_reg_xcpt_interrupt})
`ASSERT_VALID_PORTS(ERROR_intpipe_mem_valid_xs,  clock, reset_for_assert, `MIN_IFC_INTPIPE.mem_pc_valid,                                 {`MIN_IFC_INTPIPE.mem_ctrl_masked, `MIN_IFC_INTPIPE.mem_reg_pc, `MIN_IFC_INTPIPE.mem_csr_flush, `MIN_IFC_INTPIPE.mem_reg_rvc_masked, `MIN_IFC_INTPIPE.mem_thread_id})
`ASSERT_VALID_PORTS(ERROR_intpipe_mem_xcpt_xs,   clock, reset_for_assert, `MIN_IFC_INTPIPE.mem_xcpt,                                     {`MIN_IFC_INTPIPE.mem_xcpt_cause, `MIN_IFC_INTPIPE.mem_reg_xcpt_interrupt})


//
// Intpipe CSR file
//

`ASSERT_ALWAYS_MSG(ERROR_fflags, clock, reset_for_assert, !(`MIN_IFC_INTPIPE.csr_file.io_fcsr_flags_valid && `MIN_IFC_INTPIPE.csr_file.reg_fcsr_en[`MIN_IFC_INTPIPE.csr_file.io_fcsr_thread_id]),
                   "ERROR: updating and writing fp flags at the same time")

`ASSERT_ALWAYS_MSG(ERROR_one_resume, clock, reset_for_assert, `MIN_IFC_INTPIPE.csr_file.resume_thread != 2'b11, "ERROR: only 1 thread can be resumed at a time")

`ASSERT_ALWAYS_MSG(ERROR_interrupt_handled, clock, reset_for_assert, 
                   !(`MIN_IFC_INTPIPE.csr_file.cause.interrupt_x && `MIN_IFC_INTPIPE.csr_file.exception && 
                   ((`MIN_IFC_INTPIPE.csr_file.pending_interrupts[`MIN_IFC_INTPIPE.csr_file.wb_thread_id] >> `MIN_IFC_INTPIPE.csr_file.cause.cause) & 1 ) != 0 && 
                   ((`MIN_IFC_INTPIPE.csr_file.all_interrupts[`MIN_IFC_INTPIPE.csr_file.wb_thread_id] >> `MIN_IFC_INTPIPE.csr_file.cause.cause) & 1 ) == 0 ),
                   "ERROR: received an interrupt that shouldn't have been handled: pending, but should not be enabled")

`ASSERT_ALWAYS_MSG(ERROR_excl_xcpt, clock, reset_for_assert, (`MIN_IFC_INTPIPE.csr_file.insn_ret_qual + `MIN_IFC_INTPIPE.csr_file.io_exception + `MIN_IFC_INTPIPE.csr_file.io_csr_xcpt <= 1), "ERROR: mutually exclusive conditions")
`ASSERT_ALWAYS_MSG(ERROR_dpc_not_updated, clock, reset_for_assert, `MIN_IFC_INTPIPE.csr_file.trapToDebug |-> `MIN_IFC_INTPIPE.csr_file.reg_dpc_next[`MIN_IFC_INTPIPE.csr_file.wb_thread_id] == `MIN_IFC_INTPIPE.csr_file.io_pc, "ERROR: pc and dpc mismatch")
`ASSERT_ALWAYS_MSG(ERROR_resume_not_onehot, clock, reset_for_assert, $onehot0(`MIN_IFC_INTPIPE.csr_file.resume_thread), "ERROR: resuming more than one thread at the same time")


//
// Intpipe CSR reply
//

`ASSERT_ALWAYS(ERROR_replayUnique, clock, reset_for_assert, $onehot(`MIN_IFC_INTPIPE.csr_file.csr_replay.wb_csr_replay) || `MIN_IFC_INTPIPE.csr_file.csr_replay.wb_csr_replay==0);
`ASSERT_ALWAYS(ERROR_stallUnique_t0, clock, reset_for_assert, $onehot(`MIN_IFC_INTPIPE.csr_file.csr_replay.csr_replay_stall[0]) || `MIN_IFC_INTPIPE.csr_file.csr_replay.csr_replay_stall[0]==0);
`ASSERT_ALWAYS(ERROR_stallUnique_t1, clock, reset_for_assert, $onehot(`MIN_IFC_INTPIPE.csr_file.csr_replay.csr_replay_stall[1]) || `MIN_IFC_INTPIPE.csr_file.csr_replay.csr_replay_stall[1]==0);



//
// checker to detect RTLMIN-6060: in "intpipe.csr_file":
// when DCache/tensor_load_0 FSM ctrl_state is in ML_CTRL_Wait_Start and (tensor_ctrl_use_tmask==1) and (save_tmask_pending==1):
// conv_bits_ready[1:0] asserting (being 1) must pass onto reg_tensor_mask_ready[1:0] (with 1-cycle delay), for at least 1 cycle;
//

`ASSERT_ALWAYS_MSG(ERROR_t0_from_conv_bits_ready_to_tensor_mask_ready, clock, reset_for_assert, 
   (((`MIN_IFC_DCACHE.tensor_load_0.ctrl_state_next[2:0] == 3'b001) &&    // ML_CTRL_Wait_Start
     (`MIN_IFC_DCACHE.tensor_load_0.tensor_ctrl_use_tmask == 1'b1) &&
     (`MIN_IFC_DCACHE.tensor_load_0.save_tmask_pending == 1'b1) &&
     (`MIN_IFC_INTPIPE.csr_file.conv_bits_ready[0] == 1'b1) && ($past(`MIN_IFC_INTPIPE.csr_file.conv_bits_ready[0]) == 1'b0)
    ) |-> ##1
    (`MIN_IFC_INTPIPE.csr_file.reg_tensor_mask_ready[0] == 1'b1) 
   ),
   $sformatf("IntPipe: csr_file: T0: something wrong when conv_bits_ready[0] try to pass onto reg_tensor_mask_ready[0]") )

`ASSERT_ALWAYS_MSG(ERROR_t1_from_conv_bits_ready_to_tensor_mask_ready, clock, reset_for_assert, 
   (((`MIN_IFC_DCACHE.tensor_load_0.ctrl_state_next[2:0] == 3'b001) &&    // ML_CTRL_Wait_Start
     (`MIN_IFC_DCACHE.tensor_load_0.tensor_ctrl_use_tmask == 1'b1) &&
     (`MIN_IFC_DCACHE.tensor_load_0.save_tmask_pending == 1'b1) &&
     (`MIN_IFC_INTPIPE.csr_file.conv_bits_ready[1] == 1'b1) && ($past(`MIN_IFC_INTPIPE.csr_file.conv_bits_ready[1]) == 1'b0)
    ) |-> ##1
    (`MIN_IFC_INTPIPE.csr_file.reg_tensor_mask_ready[1] == 1'b1) 
   ),
   $sformatf("IntPipe: csr_file: T1: something wrong when conv_bits_ready[1] try to pass onto reg_tensor_mask_ready[1]") )




//
// more checker to detect RTLMIN-6060: in "intpipe.csr_file":
// when DCache/tensor_load_0 FSM ctrl_state is in ML_CTRL_Wait_Start and (tensor_ctrl_use_tmask==1) and (save_tmask_pending==1):
// conv_bits_ready[0] or [1] should never asserted more than once ...  
//

logic [1:0]  mon_conv_bits_ready;
logic [1:0]  mon_conv_bits_ready_d1;    // 1-cyc delayed version
int  num_of_rise_conv_bits_ready [2];

assign  mon_conv_bits_ready = `MIN_IFC_INTPIPE.csr_file.conv_bits_ready;
always @(posedge clock) begin
   mon_conv_bits_ready_d1 <= mon_conv_bits_ready;
end

always @(posedge clock) begin: tensorload_wait_tmask_but_conv_bits_ready_rise_twice
   if(reset_for_assert == 1'b1) begin
      num_of_rise_conv_bits_ready = {0, 0};
   end 
   else begin
      if((`MIN_IFC_DCACHE.tensor_load_0.ctrl_state_next[2:0] == 3'b001) &&  // ML_CTRL_Wait_Start
         (`MIN_IFC_DCACHE.tensor_load_0.tensor_ctrl_use_tmask == 1'b1) &&
         (`MIN_IFC_DCACHE.tensor_load_0.save_tmask_pending == 1'b1) 
      ) begin
         for(int ii=0; ii<2; ii++) begin
            if(mon_conv_bits_ready[ii] & ~mon_conv_bits_ready_d1[ii]) begin
               num_of_rise_conv_bits_ready[ii]++;
               if(num_of_rise_conv_bits_ready[ii] >= 2) begin
                  $error("%0t ERROR: minion_core_intpipe_assert: while Dcache Tensor_Load_0 FSM waiting for tensor_mask, but conv_bits_ready[%0d] rose twice", $time, ii);
                  // add some delay before stop simulation:
                  repeat(50) @(posedge `MIN_IFC_DCACHE.clock); 
                  $finish(1); 
               end
            end
         end
      end
      else begin
         num_of_rise_conv_bits_ready = {0, 0};
      end
   end
end  // tensorload_wait_tmask_but_conv_bits_ready_rise_twice



//
// Regarding the reg_tensor_mask:
// 1) we could do direct csr write to reg_tensor_mask to set/update its value;
// 2) whenever we do csr write to conv_size or conv_ctrl register,
//     it will (re-)compute the conv_bits,
//     using both conv_size and conv_ctrl registers,
//     and then set tensor_mask = conv_bits;
//     if either conv register has "x" value, the result is "problematic".
// Therefore:
// If we do Step #1 (above), then tensor_mask value will be "good", regardless what has been done previously.
// If we do Step #2 (above), then we need to initialize both conv_size and conv_ctrl registers, 
// otherwise conv_bits / tensor_mask result will be "problematic".
// 

reg [1:0]  reg_tensor_mask_is_good;

always @(posedge clock) begin
   if (reset_for_assert == 1'b1) begin
      reg_tensor_mask_is_good <= 2'b0;
   end
   else begin
      // after a Core direct CSR write to tensor_mask[] register:
      for (int thread_id = 0; thread_id < 2; thread_id++) begin
         if (`MIN_IFC_INTPIPE.csr_file.reg_tensor_mask_en[thread_id] == 1'b1) begin 
            if (|`MIN_IFC_INTPIPE.csr_file.wb_wdata[15:0] !== 1'bx) begin 
               reg_tensor_mask_is_good[thread_id] <= 1'b1;
            end else begin
               reg_tensor_mask_is_good[thread_id] <= 1'b0;
            end 
         end
      end

      // after a CSR write to tensor_conv_ctrl[] or tensor_conv_size[]: re-compute conv_bits and then save to tensor_mask:
      for (int thread_id = 0; thread_id < 2; thread_id++) begin
         if (`MIN_IFC_INTPIPE.csr_file.conv_save_mask[thread_id] == 1'b1) begin
            if ((|`MIN_IFC_INTPIPE.csr_file.reg_tensor_conv_ctrl[thread_id].row_pos[15:0] !== 1'bx) &&
                (|`MIN_IFC_INTPIPE.csr_file.reg_tensor_conv_ctrl[thread_id].col_pos[15:0] !== 1'bx) &&
                (|`MIN_IFC_INTPIPE.csr_file.reg_tensor_conv_size[thread_id].row_size[15:0] !== 1'bx) &&
                (|`MIN_IFC_INTPIPE.csr_file.reg_tensor_conv_size[thread_id].row_step_offset[7:0] !== 1'bx) &&
                (|`MIN_IFC_INTPIPE.csr_file.reg_tensor_conv_size[thread_id].col_size[15:0] !== 1'bx) &&
                (|`MIN_IFC_INTPIPE.csr_file.reg_tensor_conv_size[thread_id].col_step_offset[7:0] !== 1'bx) )
            begin
               reg_tensor_mask_is_good[thread_id] <= 1'b1;
            end else begin
               reg_tensor_mask_is_good[thread_id] <= 1'b0;
            end 
         end
      end
   end
end



//
// checker: we must initialize reg_tensor_conv_size[] and reg_tensor_conv_ctrl[] before doing 
//          any Tensor_Load using tensor_mask or doing Tensor_FMA using tensor_mask;
//

bit  tensor_load_but_conv_ctrl_size_uninitialized_check;
initial begin
   if(!`et_value_plusargs("tensor_load_but_conv_ctrl_size_uninitialized_check=%0d", tensor_load_but_conv_ctrl_size_uninitialized_check)) begin
      tensor_load_but_conv_ctrl_size_uninitialized_check = 1'b0; // default: disabled 
   end
end 

wire  mon_tensor_load_thread_id = `MIN_IFC_INTPIPE.csr_file.tensor_load_thread_id;

`ASSERT_ALWAYS_MSG (ERROR_tensor_load_but_conv_ctrl_size_uninitialized, clock, (reset_for_assert | ~tensor_load_but_conv_ctrl_size_uninitialized_check), 
   (((`MIN_IFC_INTPIPE.csr_file.dcache_ctrl.tensorload_start) && 
     (`MIN_IFC_INTPIPE.csr_file.dcache_ctrl.tensorload_ctrl.use_tmask) &&
     (`MIN_IFC_INTPIPE.csr_file.dcache_ctrl.tensorload_tmask_ready) 
    ) |-> 
    (reg_tensor_mask_is_good[mon_tensor_load_thread_id] == 1'b1) 
   ),
   $sformatf("IntPipe: csr_file: reg_tensor_mask[] or reg_tensor_conv_ctrl[] or reg_tensor_conv_size is uninitialized when starting Tensor_Load using tensor_mask") 
)


//
// checker: we must initialize reg_tensor_conv_size[] and reg_tensor_conv_ctrl[] before doing 
//          any Tensor_FMA using tensor_mask (conv_bits);
//

bit  tensor_fma_but_conv_ctrl_size_uninitialized_check;
initial begin
   if(!`et_value_plusargs("tensor_fma_but_conv_ctrl_size_uninitialized_check=%0d", tensor_fma_but_conv_ctrl_size_uninitialized_check)) begin
      tensor_fma_but_conv_ctrl_size_uninitialized_check = 1'b0; // default: disabled 
   end
end 

wire  mon_tensor_fma_thread_id = `MIN_IFC_INTPIPE.csr_file.tensor_fma_thread_id;

`ASSERT_ALWAYS_MSG (ERROR_tensor_fma_but_conv_ctrl_size_uninitialized, clock, (reset_for_assert | ~tensor_fma_but_conv_ctrl_size_uninitialized_check), 
   (((`MIN_IFC_INTPIPE.csr_file.vpu_ctrl.tensorfma_start) && 
     (`MIN_IFC_INTPIPE.csr_file.vpu_ctrl.tensorfma_ctrl.is_conv) &&         // meaning use_tmask
     (`MIN_IFC_INTPIPE.csr_file.vpu_ctrl.tensorfma_conv_bits_ready) 
    ) |-> 
    (reg_tensor_mask_is_good[mon_tensor_fma_thread_id] == 1'b1) 
   ),
   $sformatf("IntPipe: csr_file: reg_tensor_mask[] or reg_tensor_conv_ctrl[] or reg_tensor_conv_size is uninitialized when starting Tensor_FMA using tensor_mask (conv_bits)") 
)









