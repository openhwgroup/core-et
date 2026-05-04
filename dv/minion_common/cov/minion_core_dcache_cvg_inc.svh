`ifdef EVL_COVERAGE_ON

    minion_core_dcache_cvg minion_core_dcache_cvg_inst = new();
//  minion_core_tensor_cvg minion_core_tensor_cvg_inst = new();

    // ---------------------------------------------
    //     Auxiliar calculations for bypass
    // ----------------------------------------------

    logic[2:0] bypass;
    logic[2:0] bypass_tmp4;
    logic[2:0] bypass_tmp5;
    logic[2:0] s3_bypass;
    logic[2:0] s4_bypass;
    logic[2:0] s5_bypass;
    logic      pot_byp_from_s5;
    logic      pot_byp_from_s4;
    logic      pot_byp_from_s3;
    logic[3:0] s2_block_addr;
    logic[3:0] s3_block_addr;
    logic[3:0] s4_block_addr;
    logic[3:0] s5_block_addr;
    logic[3:0] s3_req_chunk_read;
    logic[3:0] s4_req_chunk_read;
    logic[3:0] s5_req_chunk_read;


    assign  pot_byp_from_s5   = `MIN_IFC_DCACHE.s5_valid && `MIN_IFC_DCACHE.s5_cl_addr_match_s5_s2;
    assign  pot_byp_from_s4   = `MIN_IFC_DCACHE.s4_valid && `MIN_IFC_DCACHE.s5_cl_addr_match_s4_s2;
    assign  pot_byp_from_s3   = `MIN_IFC_DCACHE.s3_valid && `MIN_IFC_DCACHE.s5_cl_addr_match_s3_s2;
    assign  s2_block_addr     = `MIN_IFC_DCACHE.s2_block_addr;
    assign  s3_block_addr     = `MIN_IFC_DCACHE.s3_block_addr;
    assign  s4_block_addr     = `MIN_IFC_DCACHE.s4_block_addr;
    assign  s5_block_addr     = `MIN_IFC_DCACHE.s5_block_addr;
    assign  s3_req_chunk_read = `MIN_IFC_DCACHE.s3_req.chunk_read;
    assign  s4_req_chunk_read = `MIN_IFC_DCACHE.s4_req.chunk_read;
    assign  s5_req_chunk_read = `MIN_IFC_DCACHE.s5_req.chunk_read;


    //      bypass logic
    assign s5_bypass = pot_byp_from_s5 ? (s2_block_addr && s5_block_addr & s5_req_chunk_read) : 1'b0;
    assign bypass_tmp5 = (pot_byp_from_s5 && s5_bypass) ? 3'h4 : 3'b0;   //bypass happens in s5 - 3'b100

    assign s4_bypass = pot_byp_from_s4 ? (s2_block_addr && s4_block_addr & s4_req_chunk_read) : 1'b0;
    assign bypass_tmp4 = (pot_byp_from_s4 && s4_bypass) ? (s4_bypass ^ s5_bypass ? 3'h6 : 3'h2) : bypass_tmp5;

    assign s3_bypass = pot_byp_from_s3 ? (s2_block_addr && s3_block_addr & s3_req_chunk_read) : 1'b0;
    assign bypass = (pot_byp_from_s3 && s3_bypass) ?
                      ( ((s3_bypass ^ s4_bypass ^ s5_bypass) && (s4_bypass ^ s5_bypass)) ? 3'h7 : ((s3_bypass ^ s4_bypass) ? 3'h3 : ((s3_bypass ^ s5_bypass) ? 3'h5 : 3'h1)) )
                      : bypass_tmp4;


    // ---------------------------------------------
    //     Auxiliar signals for xcpts
    // ----------------------------------------------
    logic[1:0] dcache_access_fault_xcpt;
    logic[1:0] dcache_page_split_xcpt;
    logic[1:0] dcache_page_fault_xcpt;

    assign dcache_access_fault_xcpt = { `MIN_IFC_DCACHE.s1_core_xcpt.af_st, `MIN_IFC_DCACHE.s1_core_xcpt.af_ld };
    assign dcache_page_split_xcpt   = { `MIN_IFC_DCACHE.s1_core_xcpt.ps_st, `MIN_IFC_DCACHE.s1_core_xcpt.ps_ld };
    assign dcache_page_fault_xcpt   = { `MIN_IFC_DCACHE.s1_core_xcpt.pf_st, `MIN_IFC_DCACHE.s1_core_xcpt.pf_ld };


       // ---------------------------------------------
       //     Auxiliar calculations for PMA accesses
       // ---------------------------------------------

    logic[9:0]  dcache_mem_region_permitted;
    logic[4:0]  dcache_mprot;
    logic[1:0]  dcache_dram_size;
    logic       paddr_io_region;
    logic       paddr_scp_region;
    logic       paddr_esr_region;
    logic       paddr_pcie_region;
    logic       paddr_mbox_region;
    logic       paddr_os_region;
    logic       paddr_dram_region;


    assign paddr_io_region   = paddr_is_io_space(`MIN_IFC_DCACHE.s1_pma_req_data.paddr);
    assign paddr_scp_region  = paddr_is_scratchpad(`MIN_IFC_DCACHE.s1_pma_req_data.paddr);
    assign paddr_esr_region  = paddr_is_esr_space(`MIN_IFC_DCACHE.s1_pma_req_data.paddr);
    assign paddr_pcie_region = paddr_is_pcie_space(`MIN_IFC_DCACHE.s1_pma_req_data.paddr);
    assign paddr_mbox_region = paddr_is_dram_mbox(`MIN_IFC_DCACHE.s1_pma_req_data.paddr);
    assign paddr_os_region   = paddr_is_dram_osbox(`MIN_IFC_DCACHE.s1_pma_req_data.paddr);
    assign paddr_dram_region = paddr_is_dram_other(`MIN_IFC_DCACHE.s1_pma_req_data.paddr,`MIN_IFC_DCACHE.pma_unit.mprot.dram_size);

    assign dcache_mem_region_permitted = { {1'b1}, `MIN_IFC_DCACHE.pma_unit.dcache_req_osbox_permitted, `MIN_IFC_DCACHE.pma_unit.dcache_req_mcode_permitted, `MIN_IFC_DCACHE.pma_unit.dcache_req_mdata_permitted, `MIN_IFC_DCACHE.pma_unit.dcache_req_scode_permitted, `MIN_IFC_DCACHE.pma_unit.dcache_req_sdata_permitted, `MIN_IFC_DCACHE.pma_unit.dcache_req_pcie_permitted, `MIN_IFC_DCACHE.pma_unit.dcache_req_esr_permitted, `MIN_IFC_DCACHE.pma_unit.dcache_req_scp_permitted, `MIN_IFC_DCACHE.pma_unit.dcache_req_io_permitted };

    assign dcache_mprot                = { `MIN_IFC_DCACHE.pma_unit.mprot.enable_secure_memory, `MIN_IFC_DCACHE.pma_unit.mprot.disable_osbox_access, `MIN_IFC_DCACHE.pma_unit.mprot.disable_pcie_access, `MIN_IFC_DCACHE.pma_unit.mprot.io_access_mode };
    assign dcache_dram_size            = `MIN_IFC_DCACHE.pma_unit.mprot.dram_size;



    // ---------------------------------------------
    //     Auxiliar calculations for Pipeline state
    // ---------------------------------------------
    logic[5:0]  dcache_pipeline_state;

    assign dcache_pipeline_state = {`MIN_IFC_DCACHE.s5_valid, `MIN_IFC_DCACHE.s4_valid, `MIN_IFC_DCACHE.s3_valid, `MIN_IFC_DCACHE.s2_valid, `MIN_IFC_DCACHE.s1_valid, `MIN_IFC_DCACHE.s0_valid_to_s1};



    // --------------------------------------------------------
    //     Auxiliary calculations for TensorReduce handshaking
    // --------------------------------------------------------
    logic       receiver_ready;
    logic       sender_ready;
    logic       handshake_is_done;
    logic       receiver_is_first;
    logic       sender_is_first;
    logic [1:0] cacheop_busy_prev;
    wire  [1:0] cacheop_busy_change;

    assign cacheop_busy_change = cacheop_busy_prev ^ { `MIN_IFC_DCACHE.cacheop_l2_busy, `MIN_IFC_DCACHE.cacheop_busy };

    initial begin
        receiver_is_first = 0;
        sender_is_first   = 0;
        cacheop_busy_prev = 0;
    end


    assign receiver_ready = ( (`MIN_IFC_DCACHE.reduce.fsm_state == 4'h2) && (`MIN_IFC_DCACHE.reduce.action == 2'b0) );
    assign sender_ready   = `MIN_IFC_DCACHE.reduce.new_partner_ready;

    always @(posedge m_clock) begin
        // starting condition
        if ((receiver_ready == 1) && (sender_ready != 1) && ((receiver_is_first == 0) && (sender_is_first == 0))) begin
            receiver_is_first <= 1;
        end
        else if ((receiver_ready != 1) && (sender_ready == 1) && ((receiver_is_first == 0) && (sender_is_first == 0))) begin
            sender_is_first <= 1;
        end
        // handshaking success
        if ((receiver_is_first == 1) && (sender_ready == 1)) begin
            handshake_is_done <= 1;
            // FUTURE: Charles Ye should we be resetting receiver_is_first and sender_is_first here?
        end
        else if ((sender_is_first == 1) && (receiver_ready == 1)) begin
            handshake_is_done <= 1;
            // FUTURE: Charles Ye should we be resetting receiver_is_first and sender_is_first here?
        end
        // after successful handshake, reset
        if (handshake_is_done == 1) begin
            receiver_is_first <= 0;
            sender_is_first   <= 0;
            handshake_is_done <= 0;
        end
        cacheop_busy_prev <= { `MIN_IFC_DCACHE.cacheop_l2_busy, `MIN_IFC_DCACHE.cacheop_busy };
    end

    logic[4:0] tensor_load_l2_scp_tmask_sum, tensor_load_tmask_sum;
    assign tensor_load_l2_scp_tmask_sum = $countones(~(`MIN_IFC_DCACHE.core_ctrl.cache_op_req.tmask_bits));
    assign tensor_load_tmask_sum        = $countones(~(mon_core_ctrl.tensorload_tmask_bits));


    // --------------------------------------------------------
    //     Auxiliar signal for DCache-VPU interaction
    // --------------------------------------------------------

    logic[1:0] tfma_status;
    assign tfma_status = {`MIN_IFC_DCACHE.tensor_load_1.tfma_started,`MIN_IFC_DCACHE.tensor_load_1.tfma_active_start};


    // Signals for tensor sequences
        assign    tensor_load_en                 = minion_top.core.intpipe.csr_file.reg_tensor_load_en & ~minion_top.core.intpipe.csr_file.wb_wdata[52];
        assign    tensor_load_setupB_en          = minion_top.core.intpipe.csr_file.reg_tensor_load_en & minion_top.core.intpipe.csr_file.wb_wdata[52];  
        assign    tensor_load_l2scp_en           = minion_top.core.intpipe.csr_file.reg_tensor_load_l2scp_en;
        assign    tensor_fma_en                  = minion_top.core.intpipe.csr_file.reg_tensor_fma_en; 
        assign    tensor_quant_en                = minion_top.core.intpipe.csr_file.reg_tensor_quant_en; 
        assign    tensor_reduce_en               = minion_top.core.intpipe.csr_file.reg_tensor_reduce_en; 
        assign    tensor_store_en                = minion_top.core.intpipe.csr_file.reg_tensor_store_en; 
        assign    tensor_wait_en_load            = minion_top.core.intpipe.csr_file.reg_tensor_wait_en[0] & (minion_top.core.intpipe.csr_file.wb_wdata[3:0] inside {0,1});
        assign    tensor_wait_en_load_l2scp      = minion_top.core.intpipe.csr_file.reg_tensor_wait_en[0] & (minion_top.core.intpipe.csr_file.wb_wdata[3:0] inside {2,3});
        assign    tensor_wait_en_fma             = minion_top.core.intpipe.csr_file.reg_tensor_wait_en[0] & (minion_top.core.intpipe.csr_file.wb_wdata[3:0] == 4'd7);
        assign    tensor_wait_en_store           = minion_top.core.intpipe.csr_file.reg_tensor_wait_en[0] & (minion_top.core.intpipe.csr_file.wb_wdata[3:0] == 4'd8);
        assign    tensor_wait_en_reduce          = minion_top.core.intpipe.csr_file.reg_tensor_wait_en[0] & (minion_top.core.intpipe.csr_file.wb_wdata[3:0] == 4'd9);
        assign    tensor_wait_en_quant           = minion_top.core.intpipe.csr_file.reg_tensor_wait_en[0] & (minion_top.core.intpipe.csr_file.wb_wdata[3:0] == 4'd10);

        enum bit [5:0] {init = 0, ten_load, ten_load_setupB, ten_load_l2scp, ten_fma, ten_quant, ten_red, ten_store, ten_load_fma, ten_quant_fma, ten_red_fma, ten_store_fma, ten_load_l2scp_fma, ten_load_quant, ten_fma_quant, ten_red_quant, ten_store_quant, ten_load_l2scp_quant, ten_load_red, ten_fma_red, ten_quant_red, ten_store_red, ten_load_l2scp_red, ten_load_store, ten_fma_store, ten_quant_store, ten_red_store, ten_load_l2scp_store, ten_load_wait, ten_load_setupB_wait, ten_load_l2scp_wait, ten_fma_wait, ten_store_wait, ten_red_wait, ten_quant_wait, ten_load_setupB_fma, ten_load_setupB_quant, ten_load_setupB_red, ten_load_setupB_store, ten_load_wait_fma, ten_load_l2scp_wait_fma, ten_load_setupB_wait_fma, ten_load_wait_quant, ten_load_l2scp_wait_quant, ten_load_setupB_wait_quant, ten_load_wait_red, ten_load_l2scp_wait_red, ten_load_setupB_wait_red, ten_load_wait_store, ten_load_l2scp_wait_store, ten_load_setupB_wait_store, ten_fma_wait_store, ten_store_wait_fma, ten_fma_wait_quant, ten_quant_wait_fma, ten_fma_wait_red, ten_red_wait_fma, ten_quant_wait_store, ten_store_wait_quant, ten_store_wait_red, ten_red_wait_store, ten_quant_wait_red, ten_red_wait_quant } seq_state;

        bit [5:0] new_tensor_seq;
        bit [5:0] tensor_seq;

        always @(posedge m_clock) begin
  
            tensor_seq <= new_tensor_seq;

            case({tensor_load_en, tensor_load_setupB_en, tensor_load_l2scp_en, tensor_fma_en, tensor_quant_en, tensor_reduce_en, tensor_store_en, tensor_wait_en_load, tensor_wait_en_load_l2scp, tensor_wait_en_fma, tensor_wait_en_store, tensor_wait_en_reduce, tensor_wait_en_quant})
             13'b1000000000000: new_tensor_seq = ten_load;
             13'b0100000000000: new_tensor_seq = ten_load_setupB;
             13'b0010000000000: new_tensor_seq = ten_load_l2scp;
             13'b0001000000000: begin  
                                 if(tensor_seq == 0 || tensor_seq == ten_fma || tensor_seq == ten_load_fma) new_tensor_seq <= ten_fma;
                                 if(tensor_seq == ten_load) new_tensor_seq <= ten_load_fma;
                                 if(tensor_seq == ten_quant || tensor_seq == ten_load_quant) new_tensor_seq <= ten_quant_fma;
                                 if(tensor_seq == ten_red || tensor_seq == ten_load_red) new_tensor_seq <= ten_red_fma;
                                 if(tensor_seq == ten_store || tensor_seq == ten_load_store) new_tensor_seq <= ten_store_fma;
                                 if(tensor_seq == ten_load_l2scp) new_tensor_seq <= ten_load_l2scp_fma;
                                 if(tensor_seq == ten_load_setupB) new_tensor_seq <= ten_load_setupB_fma;
                                 if(tensor_seq == ten_load_wait) new_tensor_seq <= ten_load_wait_fma;
                                 if(tensor_seq == ten_load_l2scp_wait) new_tensor_seq <= ten_load_l2scp_wait_fma;
                                 if(tensor_seq == ten_load_setupB_wait) new_tensor_seq <= ten_load_setupB_wait_fma;
                                 if(tensor_seq == ten_quant_wait) new_tensor_seq <= ten_quant_wait_fma;
                                 if(tensor_seq == ten_red_wait) new_tensor_seq <= ten_red_wait_fma;
                                 if(tensor_seq == ten_store_wait) new_tensor_seq <= ten_store_wait_fma;
                                end   
             13'b0000100000000: begin 
                                 if(tensor_seq == 0 || tensor_seq == ten_quant || tensor_seq == ten_load_quant) new_tensor_seq <= ten_quant;
                                 if(tensor_seq == ten_load) new_tensor_seq <= ten_load_quant;
                                 if(tensor_seq == ten_fma || tensor_seq == ten_load_fma || tensor_seq == ten_load_fma) new_tensor_seq <= ten_fma_quant;
                                 if(tensor_seq == ten_red || tensor_seq == ten_load_red) new_tensor_seq <= ten_red_quant;
                                 if(tensor_seq == ten_store || tensor_seq == ten_load_store) new_tensor_seq <= ten_store_quant;
                                 if(tensor_seq == ten_load_l2scp) new_tensor_seq <= ten_load_l2scp_quant;
                                 if(tensor_seq == ten_load_setupB) new_tensor_seq <= ten_load_setupB_quant;
                                 if(tensor_seq == ten_load_wait) new_tensor_seq <= ten_load_wait_quant;
                                 if(tensor_seq == ten_load_l2scp_wait) new_tensor_seq <= ten_load_l2scp_wait_quant;
                                 if(tensor_seq == ten_load_setupB_wait) new_tensor_seq <= ten_load_setupB_wait_quant;
                                 if(tensor_seq == ten_fma_wait) new_tensor_seq <= ten_fma_wait_quant;
                                 if(tensor_seq == ten_red_wait) new_tensor_seq <= ten_red_wait_quant;
                                 if(tensor_seq == ten_store_wait) new_tensor_seq <= ten_store_wait_quant;
                                end 
             13'b0000010000000: begin
                                 if(tensor_seq == 0 || tensor_seq == ten_red || tensor_seq == ten_load_red) new_tensor_seq <= ten_red;
                                 if(tensor_seq == ten_load) new_tensor_seq <= ten_load_red;
                                 if(tensor_seq == ten_fma || tensor_seq == ten_load_fma) new_tensor_seq <= ten_fma_red;
                                 if(tensor_seq == ten_quant || tensor_seq == ten_load_quant) new_tensor_seq <= ten_quant_red;
                                 if(tensor_seq == ten_store || tensor_seq == ten_load_store) new_tensor_seq <= ten_store_red;
                                 if(tensor_seq == ten_load_l2scp) new_tensor_seq <= ten_load_l2scp_red;
                                 if(tensor_seq == ten_load_setupB) new_tensor_seq <= ten_load_setupB_red;
                                 if(tensor_seq == ten_load_wait) new_tensor_seq <= ten_load_wait_red;
                                 if(tensor_seq == ten_load_l2scp_wait) new_tensor_seq <= ten_load_l2scp_wait_red;
                                 if(tensor_seq == ten_load_setupB_wait) new_tensor_seq <= ten_load_setupB_wait_red;
                                 if(tensor_seq == ten_fma_wait) new_tensor_seq <= ten_fma_wait_red;
                                 if(tensor_seq == ten_quant_wait) new_tensor_seq <= ten_quant_wait_red;
                                 if(tensor_seq == ten_store_wait) new_tensor_seq <= ten_store_wait_red;
                                end
             13'b0000001000000: begin
                                 if(tensor_seq == 0 || tensor_seq == ten_store || tensor_seq == ten_load_store) new_tensor_seq <= ten_store;
                                 if(tensor_seq == ten_load) new_tensor_seq <= ten_load_store;
                                 if(tensor_seq == ten_fma || tensor_seq == ten_load_fma) new_tensor_seq <= ten_fma_store;
                                 if(tensor_seq == ten_quant || tensor_seq == ten_load_quant) new_tensor_seq <= ten_quant_store;
                                 if(tensor_seq == ten_red || tensor_seq == ten_load_red) new_tensor_seq <= ten_red_store;
                                 if(tensor_seq == ten_load_l2scp) new_tensor_seq <= ten_load_l2scp_store;
                                 if(tensor_seq == ten_load_setupB) new_tensor_seq <= ten_load_setupB_store;
                                 if(tensor_seq == ten_load_wait) new_tensor_seq <= ten_load_wait_store;
                                 if(tensor_seq == ten_load_l2scp_wait) new_tensor_seq <= ten_load_l2scp_wait_store;
                                 if(tensor_seq == ten_load_setupB_wait) new_tensor_seq <= ten_load_setupB_wait_store;
                                 if(tensor_seq == ten_fma_wait) new_tensor_seq <= ten_fma_wait_store;
                                 if(tensor_seq == ten_quant_wait) new_tensor_seq <= ten_quant_wait_store;
                                 if(tensor_seq == ten_red_wait) new_tensor_seq <= ten_red_wait_store;
                                end
             13'b0000000100000: begin
                                 if(tensor_seq == ten_load) new_tensor_seq <= ten_load_wait;
                                 if(tensor_seq == ten_load_setupB) new_tensor_seq <= ten_load_setupB_wait;
                                end
             13'b0000000010000: if(tensor_seq == ten_load_l2scp) new_tensor_seq <= ten_load_l2scp_wait;
             13'b0000000001000: if(tensor_seq == ten_fma || tensor_seq == ten_load_fma) new_tensor_seq <= ten_fma_wait;
             13'b0000000000100: if(tensor_seq == ten_store || tensor_seq == ten_load_store) new_tensor_seq <= ten_store_wait;
             13'b0000000000010: if(tensor_seq == ten_red || tensor_seq == ten_load_red) new_tensor_seq <= ten_red_wait;
             13'b0000000000001: if(tensor_seq == ten_quant || tensor_seq == ten_load_quant) new_tensor_seq <= ten_quant_wait;
             default:           new_tensor_seq <= tensor_seq;
             endcase 


        end


    // -----------------------------------
    //     Covergroup sampling
    // -----------------------------------

    always @(posedge m_clock) begin

//`ifndef USE_PSHIRE_ENV

        minion_core_dcache_cvg_inst.cg_dcache_general.sample(evl_dcache_tag_xcpt'(`MIN_IFC_DCACHE.s1_core_xcpt),
                                                             bypass,
                                                             `MIN_IFC_DCACHE.s0_core_req.cmd,
                                                             `MIN_IFC_DCACHE.s0_core_req.typ,
                                                             `MIN_IFC_DCACHE.s0_core_req_valid,
                                                             `MIN_IFC_DCACHE.s2_valid_qual,
                                                             `MIN_IFC_DCACHE.miss_handler_unit.req_ready,
                                                             `MIN_IFC_DCACHE.miss_handler_unit.mh_tag_match,
                                                             `MIN_IFC_DCACHE.s2_nack_miss,
                                                             `MIN_IFC_DCACHE.s2_mh_req_assigned,
                                                             `MIN_IFC_DCACHE.s2_mh_req_info.cacheable,
                                                             `MIN_IFC_DCACHE.s2_mh_req_info.thread_id,
                                                             `MIN_IFC_DCACHE.s2_hit,
                                                             `MIN_IFC_DCACHE.s2_to_s3_en_store_byp,
                                                             `MIN_IFC_DCACHE.s3_store_bypass,
                                                             `MIN_IFC_DCACHE.id_rq_full,
                                                             `MIN_IFC_DCACHE.id_ba_full,
                                                             `MIN_IFC_DCACHE.s2_req.cmd,
                                                             `MIN_IFC_DCACHE.s2_req.typ,
                                                             `MIN_IFC_DCACHE.s2_rq_renack,
                                                             `MIN_IFC_DCACHE.s1_has_xcpt,
                                                             `MIN_IFC_DCACHE.s1_req.cmd,
                                                             `MIN_IFC_DCACHE.s1_req.typ,
                                                             `MIN_IFC_DCACHE.s1_valid,
                                                             `MIN_IFC_DCACHE.core_ctrl_resp.gsc_progress,
                                                             `MIN_IFC_DCACHE.core_ctrl_resp.gsc_progress_save,
                                                             `MIN_IFC_DCACHE.s2_req.misaligned_2nd,
                                                             `MIN_IFC_DCACHE.s2_valid_masked,
                                                             `MIN_IFC_DCACHE.s0_core_req.ps_mask,
                                                             minion_top.frontend_clock_gate_disable,
                                                             minion_top.dcache_clock_gate_disable,
                                                             minion_top.vputrans_clock_gate_disable,
                                                             minion_top.vputima_clock_gate_disable,
                                                             minion_top.vpulane_clock_gate_disable,
                                                             minion_top.intpipe_clock_gate_disable);


        if ((`MIN_IFC_DCACHE.id_co_mr_grant | `MIN_IFC_DCACHE.id_dbg_grant | `MIN_IFC_DCACHE.id_wb_da_grant | `MIN_IFC_DCACHE.id_rq_grant | `MIN_IFC_DCACHE.id_co_pf_grant) === 1'b1) begin
            minion_core_dcache_cvg_inst.cg_dcache_id_arbiter.sample(`MIN_IFC_DCACHE.id_co_mr_grant,
                                                                    `MIN_IFC_DCACHE.id_dbg_grant,
                                                                    `MIN_IFC_DCACHE.id_wb_da_grant,
                                                                    `MIN_IFC_DCACHE.id_rq_grant,
                                                                    `MIN_IFC_DCACHE.id_co_pf_grant);
        end


//      minion_core_dcache_cvg_inst.cg_dcache_va_pa_translate.sample(evl_dcache_tag_xcpt'(`MIN_IFC_DCACHE.s1_core_xcpt),
//                                                                   `MIN_IFC_DCACHE.s1_req.vm_sel[2:0],
//                                                                   `MIN_IFC_DCACHE.s1_tlb_req_valid,
//                                                                   `MIN_IFC_DCACHE.s1_tlb_resp_data.miss,
//                                                                   `MIN_IFC_DCACHE.s1_has_xcpt,
//                                                                   `MIN_IFC_DCACHE.s1_req.cmd);


        if (|{ `MIN_IFC_DCACHE.core_ctrl.cache_op_valid, cacheop_busy_change } === 1'b1) begin
            minion_core_dcache_cvg_inst.cg_dcache_cache_ops.sample(`MIN_IFC_DCACHE.core_ctrl.cache_op_req.cmd,
                                                                   `MIN_IFC_DCACHE.core_ctrl.cache_op_valid,
                                                                   `MIN_IFC_DCACHE.dcache_cfg_mode,
                                                                   `MIN_IFC_DCACHE.cache_op_unit.req.cmd,
                                                                   `MIN_IFC_DCACHE.cache_op_unit.req_way,
                                                                   `MIN_IFC_DCACHE.cache_op_unit.req_set,
                                                                   `MIN_IFC_DCACHE.cache_op_unit.lock_en,
                                                                   `MIN_IFC_DCACHE.core_ctrl.cache_op_req.l2_scp_fill,
                                                                   `MIN_IFC_DCACHE.core_ctrl.cache_op_req.count,
                                                                   `MIN_IFC_DCACHE.core_ctrl.cache_op_req.use_tmask,
                                                                   `MIN_IFC_DCACHE.core_ctrl.cache_op_req.tmask_bits,
                                                                   tensor_load_l2_scp_tmask_sum,
                                                                   `MIN_IFC_DCACHE.cacheop_busy,
                                                                   `MIN_IFC_DCACHE.cacheop_l2_busy,
                                                                   `MIN_IFC_DCACHE.cache_op_unit_l2.cache_op_req_is_l2,
                                                                   `MIN_IFC_DCACHE.cache_op_unit_l2.cache_op_valid_byp);
        end


        if ((`MIN_IFC_DCACHE.s1_req_to_s2.cacheable === 1'b0) && (|{ `MIN_IFC_DCACHE.s1_mf_xcpt, `MIN_IFC_DCACHE.s1_pma_resp_data.access_fault, `MIN_IFC_DCACHE.s1_access_is_page_split, dcache_page_fault_xcpt } === 1'b1)) begin
            minion_core_dcache_cvg_inst.cg_dcache_uncacheable.sample(`MIN_IFC_DCACHE.s1_req_to_s2.cacheable,
                                                                    `MIN_IFC_DCACHE.s1_is_amo,
                                                                    `MIN_IFC_DCACHE.s1_is_bypass,
                                                                    dcache_access_fault_xcpt,
                                                                    `MIN_IFC_DCACHE.s1_is_read,
                                                                    `MIN_IFC_DCACHE.s1_is_write,
                                                                    dcache_page_split_xcpt,
                                                                    `MIN_IFC_DCACHE.s1_access_is_page_split,
                                                                    `MIN_IFC_DCACHE.s1_pma_resp_data.access_fault,
                                                                    `MIN_IFC_DCACHE.s1_mf_xcpt,
                                                                    dcache_page_fault_xcpt,
                                                                    `MIN_IFC_DCACHE.s1_is_read_no_amo,
                                                                    `MIN_IFC_DCACHE.s1_is_store);
        end


        if ((`MIN_IFC_DCACHE.s1_valid === 1'b1) || (`MIN_IFC_DCACHE.s1_access_is_page_split === 1'b1) || (dcache_access_fault_xcpt > 0)) begin
            minion_core_dcache_cvg_inst.cg_dcache_misaligned.sample(`MIN_IFC_DCACHE.s1_req_to_s2.cacheable,
                                                                    `MIN_IFC_DCACHE.s1_is_amo,
                                                                    `MIN_IFC_DCACHE.s1_is_bypass,
                                                                    dcache_access_fault_xcpt,
                                                                    `MIN_IFC_DCACHE.s1_is_read,
                                                                    `MIN_IFC_DCACHE.s1_is_write,
                                                                    dcache_page_split_xcpt,
                                                                    `MIN_IFC_DCACHE.s1_access_is_page_split,
                                                                    `MIN_IFC_DCACHE.s1_req.cmd,
                                                                    `MIN_IFC_DCACHE.s1_req.typ,
                                                                    `MIN_IFC_DCACHE.vm_enabled,
                                                                    `MIN_IFC_DCACHE.s1_access_size,
                                                                    `MIN_IFC_DCACHE.s1_req.addr[4:0],
                                                                    `MIN_IFC_DCACHE.s1_valid);
        end


        minion_core_dcache_cvg_inst.cg_dcache_fsm_states.sample(`MIN_IFC_DCACHE.cache_op_unit.state,
                                                                `MIN_IFC_DCACHE.cacheop_busy,
                                                                `MIN_IFC_DCACHE.cache_op_unit_l2.state,
                                                                `MIN_IFC_DCACHE.cacheop_l2_busy,
                                                                `MIN_IFC_DCACHE.miss_handler_unit.handlers[0].miss_handler.state,
                                                                `MIN_IFC_DCACHE.miss_handler_unit.handlers[1].miss_handler.state,
                                                                `MIN_IFC_DCACHE.tensor_load_0.ctrl_state,
                                                                `MIN_IFC_DCACHE.tensor_load_1.ctrl_state,
                                                                `MIN_IFC_DCACHE.reduce.fsm_state,
                                                                `MIN_IFC_DCACHE.reduce.atrans_state,
                                                                `MIN_IFC_DCACHE.writeback_unit.active,
                                                                 dcache_pipeline_state,
                                                                `MIN_IFC_DCACHE.texsend.req_state,
                                                                `MIN_IFC_DCACHE.texsend.pull_state);


        if (|{ mon_core_ctrl.tensorload_start, `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][9:0], `MIN_IFC_DCACHE.tensor_load_mem_err_flags } === 1'b1) begin
            minion_core_dcache_cvg_inst.cg_dcache_tensor_load.sample(`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][9:0],
                                                                     `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_en[0],
                                                                     $past(`MIN_IFC_INTPIPE.csr_file.reg_tensor_load_en_qual,1),
                                                                     mon_core_ctrl.tensorload_start,
                                                                     mon_core_ctrl.tensorload_ctrl.num_lines,
                                                                     mon_core_ctrl.tensorload_ctrl.use_coop,
                                                                     mon_core_ctrl.tensorload_ctrl.use_tmask,
                                                                     mon_core_ctrl.tensorload_tmask_bits,
                                                                     tensor_load_tmask_sum,
                                                                     `MIN_IFC_DCACHE.core_ctrl.tensorload_ctrl.tl_sel,
                                                                     `MIN_IFC_DCACHE.tensor_load_mem_err_flags);
        end


        minion_core_dcache_cvg_inst.cg_dcache_tensor_store.sample(`MIN_IFC_DCACHE.reduce.vpu_send_reg_tensor,
                                                                  `MIN_IFC_DCACHE.reduce.scp_read_reg,
                                                                  $past(`MIN_IFC_INTPIPE.csr_file.reg_tensor_store_en_qual,1),
                                                                  `MIN_IFC_INTPIPE.csr_file.wb_wdata,
                                                                  `MIN_IFC_DCACHE.reduce.fsm_state,
                                                                  `MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][9:0],
                                                                  `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_en[0],
                                                                  `MIN_IFC_INTPIPE.csr_file.reg_tensor_fma_en,
                                                                  `MIN_IFC_DCACHE.tensor_reduce_mem_err_flag);


        minion_core_dcache_cvg_inst.cg_dcache_tensor_reduce.sample(`MIN_IFC_INTPIPE.csr_file.reg_tensor_error[0][9:0],
                                                                   `MIN_IFC_INTPIPE.csr_file.reg_tensor_error_en[0],
                                                                   mon_core_ctrl.reduce_start,
                                                                   mon_core_ctrl.reduce_tensorstore_ctrl.reduce.start_reg,
                                                                   mon_core_ctrl.reduce_tensorstore_ctrl.reduce.op,
                                                                   mon_core_ctrl.reduce_tensorstore_ctrl.reduce.num_regs,
                                                                   mon_core_ctrl.reduce_tensorstore_ctrl.reduce.action,
                                                                   mon_core_ctrl.reduce_tensorstore_ctrl.reduce.partner,
                                                                   handshake_is_done,
                                                                   receiver_is_first);


        minion_core_dcache_cvg_inst.cg_dcache_pma.sample(`MIN_IFC_DCACHE.pma_unit.valid_addr,
                                                         `MIN_IFC_DCACHE.s1_pma_req_data.paddr,
                                                         dcache_mem_region_permitted,
                                                         paddr_io_region, paddr_scp_region,
                                                         paddr_esr_region, paddr_pcie_region,
                                                         paddr_mbox_region, paddr_os_region,
                                                         paddr_dram_region,
                                                         dcache_dram_size,
                                                         dcache_mprot,
                                                         `MIN_IFC_DCACHE.pma_unit.privilege_mode);

        minion_core_dcache_cvg_inst.cg_tensor_seq.sample(tensor_seq);

        minion_core_dcache_cvg_inst.cg_dcache_vpu_interaction.sample(`MIN_IFC_DCACHE.s1_vpu_ctrl.scp_req.read_en,
                                                                     `MIN_IFC_DCACHE.s1_valid_masked,
                                                                     `MIN_IFC_DCACHE.s1_wb_da_valid,
                                                                     `MIN_IFC_DCACHE.s1_da_read_en,
                                                                     `MIN_IFC_DCACHE.dbg_da_read_en,
                                                                     `MIN_IFC_DCACHE.s1_translate_only_addr_load,
                                                                     `MIN_IFC_DCACHE.s1_valid_to_s2,
                                                                     `MIN_IFC_DCACHE.s1_is_msg,
                                                                     `MIN_IFC_DCACHE.s1_is_amo,
                                                                     `MIN_IFC_DCACHE.s1_is_bypass,
                                                                     `MIN_IFC_DCACHE.s1_nack_co_collide,
                                                                     `MIN_IFC_DCACHE.tensor_load_1.tfma_enabled,
                                                                     `MIN_IFC_DCACHE.tensor_load_1.tenb_credits,
                                                                     `MIN_IFC_DCACHE.tensor_load_1.busy,
                                                                     tfma_status);


//      minion_core_dcache_cvg_inst.cg_dcache_chicken_bits.sample(minion_top.frontend_clock_gate_disable,
//                                                                minion_top.dcache_clock_gate_disable,
//                                                                minion_top.vputrans_clock_gate_disable,
//                                                                minion_top.vputima_clock_gate_disable,
//                                                                minion_top.vpulane_clock_gate_disable,
//                                                                minion_top.intpipe_clock_gate_disable);

    end


    // -----------------------------------
    //     Cover Properties
    // -----------------------------------


    // RTLMIN-5992
    // Cover that TenB is restarted after few cycles and that all l2i_state FSMs are in their init state
    logic[1:0] l2i_state;
    assign l2i_state = |{`MIN_IFC_DCACHE.tensor_load_1.l2i_state};

    cov_tenb_restart : cover property (@(posedge clock) (`MIN_IFC_DCACHE.tensor_load_1.tensor_ctrl_start |-> ##[0:10] (`MIN_IFC_DCACHE.tensor_load_1.tensor_ctrl_restart && (l2i_state==2'b0))));


`endif
