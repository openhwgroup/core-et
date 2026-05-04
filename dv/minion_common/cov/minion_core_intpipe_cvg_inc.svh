//==================================================================
//Name: minion_core_intpipe_cvg_inst.svh
//Path: dv/minion_common/cov
//Description: MINION CORE Intpipe coverage instantiation and sampling
//Owner: Nikola Rajovic, Goran Ognjanovic
//==================================================================

`ifdef EVL_COVERAGE_ON

minion_core_intpipe_cvg minion_core_intpipe_cvg_inst = new();

logic vld_reg_data_1_ex_all;
logic vld_reg_data_1_ex_lsb;
logic vld_reg_data_1_vpu;
logic vld_reg_data_1_alu_op;

logic vld_reg_data_2_ex;
logic vld_reg_data_2_alu_op;

intpipe_bypass_t [2] intpipe_bypass;

assign  vld_reg_data_1_ex_all = `MIN_IFC_INTPIPE.id_uses_rs1_all;
assign  vld_reg_data_1_ex_lsb = `MIN_IFC_INTPIPE.id_uses_rs1_lsb;
assign  vld_reg_data_1_vpu    = `MIN_IFC_INTPIPE.id_valid_qual && `MIN_IFC_INTPIPE.id_ctrl.rxs1 && `MIN_IFC_INTPIPE.id_frontend_vpu_ctrl.fromint;
assign  vld_reg_data_1_alu_op = !`MIN_IFC_INTPIPE.ex_gsc_busy && (`MIN_IFC_INTPIPE.id_ctrl_to_ex.sel_alu1 == core_a1_ctrl_RS1) && `MIN_IFC_INTPIPE.id_uses_alu && `MIN_IFC_INTPIPE.id_valid_qual;

assign  vld_reg_data_2_ex     = `MIN_IFC_INTPIPE.id_uses_rs2;
assign  vld_reg_data_2_alu_op = (`MIN_IFC_INTPIPE.id_ctrl_to_ex.sel_alu2 == core_a2_ctrl_RS2) && `MIN_IFC_INTPIPE.id_uses_alu && `MIN_IFC_INTPIPE.id_valid_qual;

always_comb
begin
    // For all the sources
    for(integer i = 0; i < 2; i++)
    begin
        // Data coming from EX stage
        if((`MIN_IFC_INTPIPE.id_raddr[i] != 5'b0) && (`MIN_IFC_INTPIPE.ex_thread_id == `MIN_IFC_INTPIPE.id_thread_id) && `MIN_IFC_INTPIPE.ex_reg_valid && `MIN_IFC_INTPIPE.ex_ctrl.wxd && (`MIN_IFC_INTPIPE.ex_waddr == `MIN_IFC_INTPIPE.id_raddr[i]))
            intpipe_bypass[i]=  EX_2_ID;

        // Data coming from TAG stage alu
        else if((`MIN_IFC_INTPIPE.id_raddr[i] != 5'b0) && (`MIN_IFC_INTPIPE.tag_thread_id == `MIN_IFC_INTPIPE.id_thread_id) && `MIN_IFC_INTPIPE.tag_reg_valid && `MIN_IFC_INTPIPE.tag_ctrl.wxd && (`MIN_IFC_INTPIPE.tag_waddr == `MIN_IFC_INTPIPE.id_raddr[i]))
            intpipe_bypass[i]= TAG_2_ID;

        // Data coming from MEM stage alu
        else if((`MIN_IFC_INTPIPE.id_raddr[i] != 5'b0) && (`MIN_IFC_INTPIPE.mem_thread_id == `MIN_IFC_INTPIPE.id_thread_id) && `MIN_IFC_INTPIPE.mem_reg_valid && `MIN_IFC_INTPIPE.mem_ctrl.wxd && (`MIN_IFC_INTPIPE.mem_waddr == `MIN_IFC_INTPIPE.id_raddr[i]))
            intpipe_bypass[i]= MEM_2_ID;

        // data coming from dcache
        else if((`MIN_IFC_INTPIPE.id_raddr[i] != 5'b0) && (`MIN_IFC_INTPIPE.wb_dcache_resp.dest.thread_id == `MIN_IFC_INTPIPE.id_thread_id) && (`MIN_IFC_INTPIPE.wb_dcache_wen) && (`MIN_IFC_INTPIPE.wb_dcache_resp.dest.addr == `MIN_IFC_INTPIPE.id_raddr[i]))
            intpipe_bypass[i]= DCACHE_2_ID;

        // Data coming from WB stage alu
        else if((`MIN_IFC_INTPIPE.id_raddr[i] != 5'b0) && (`MIN_IFC_INTPIPE.wb_thread_id == `MIN_IFC_INTPIPE.id_thread_id) && `MIN_IFC_INTPIPE.wb_wen && (`MIN_IFC_INTPIPE.wb_waddr == `MIN_IFC_INTPIPE.id_raddr[i]))
            intpipe_bypass[i]= WB_2_ID;

        // Data coming from RF
        else
            intpipe_bypass[i]= OFF;
    end
end


minion_control id_ctrl;
minion_control ex_ctrl;
minion_control tag_ctrl;
minion_control mem_ctrl;
minion_control wb_ctrl;

logic[44:0] id_ctrl_p;
logic[44:0] ex_ctrl_p;
logic[44:0] tag_ctrl_p;
logic[44:0] mem_ctrl_p;
logic[44:0] wb_ctrl_p;


logic [31:0] ex_inst_bits;
logic [31:0] tag_inst_bits;
logic [31:0] mem_inst_bits;
logic [31:0] wb_inst_bits;


assign  id_ctrl   = `MIN_IFC_INTPIPE.id_ctrl;
assign  ex_ctrl   = `MIN_IFC_INTPIPE.ex_ctrl;
assign  tag_ctrl  = `MIN_IFC_INTPIPE.tag_ctrl;
assign  mem_ctrl  = `MIN_IFC_INTPIPE.mem_ctrl;
assign  wb_ctrl   = `MIN_IFC_INTPIPE.wb_ctrl;

assign  id_ctrl_p = {id_ctrl.legal,id_ctrl.mcode,id_ctrl.fp,id_ctrl.br,id_ctrl.jal,id_ctrl.jalr,id_ctrl.rxs2,id_ctrl.rxs1,
                     id_ctrl.sel_alu2,id_ctrl.sel_alu1,id_ctrl.sel_imm,id_ctrl.alu_dw,id_ctrl.alu_fn,id_ctrl.mem,
                     id_ctrl.mem_cmd,id_ctrl.mem_type,id_ctrl.rma,id_ctrl.wmd,id_ctrl.wmad,id_ctrl.wfd,id_ctrl.div,
                     id_ctrl.wxd,id_ctrl.csr,id_ctrl.fence,id_ctrl.mem_g,id_ctrl.gsc,id_ctrl.gfx,id_ctrl.x31};

assign  ex_ctrl_p = {ex_ctrl.legal,ex_ctrl.mcode,ex_ctrl.fp,ex_ctrl.br,ex_ctrl.jal,ex_ctrl.jalr,ex_ctrl.rxs2,ex_ctrl.rxs1,
                     ex_ctrl.sel_alu2,ex_ctrl.sel_alu1,ex_ctrl.sel_imm,ex_ctrl.alu_dw,ex_ctrl.alu_fn,ex_ctrl.mem,
                     ex_ctrl.mem_cmd,ex_ctrl.mem_type,ex_ctrl.rma,ex_ctrl.wmd,ex_ctrl.wmad,ex_ctrl.wfd,ex_ctrl.div,
                     ex_ctrl.wxd,ex_ctrl.csr,ex_ctrl.fence,ex_ctrl.mem_g,ex_ctrl.gsc,ex_ctrl.gfx,ex_ctrl.x31};

assign  tag_ctrl_p = {tag_ctrl.legal,tag_ctrl.mcode,tag_ctrl.fp,tag_ctrl.br,tag_ctrl.jal,tag_ctrl.jalr,tag_ctrl.rxs2,tag_ctrl.rxs1,
                      tag_ctrl.sel_alu2,tag_ctrl.sel_alu1,tag_ctrl.sel_imm,tag_ctrl.alu_dw,tag_ctrl.alu_fn,tag_ctrl.mem,
                      tag_ctrl.mem_cmd,tag_ctrl.mem_type,tag_ctrl.rma,tag_ctrl.wmd,tag_ctrl.wmad,tag_ctrl.wfd,tag_ctrl.div,
                      tag_ctrl.wxd,tag_ctrl.csr,tag_ctrl.fence,tag_ctrl.mem_g,tag_ctrl.gsc,tag_ctrl.gfx,tag_ctrl.x31};

assign  mem_ctrl_p = {mem_ctrl.legal,mem_ctrl.mcode,mem_ctrl.fp,mem_ctrl.br,mem_ctrl.jal,mem_ctrl.jalr,mem_ctrl.rxs2,mem_ctrl.rxs1,
                      mem_ctrl.sel_alu2,mem_ctrl.sel_alu1,mem_ctrl.sel_imm,mem_ctrl.alu_dw,mem_ctrl.alu_fn,mem_ctrl.mem,
                      mem_ctrl.mem_cmd,mem_ctrl.mem_type,mem_ctrl.rma,mem_ctrl.wmd,mem_ctrl.wmad,mem_ctrl.wfd,mem_ctrl.div,
                      mem_ctrl.wxd,mem_ctrl.csr,mem_ctrl.fence,mem_ctrl.mem_g,mem_ctrl.gsc,mem_ctrl.gfx,mem_ctrl.x31};

assign  wb_ctrl_p = {wb_ctrl.legal,wb_ctrl.mcode,wb_ctrl.fp,wb_ctrl.br,wb_ctrl.jal,wb_ctrl.jalr,wb_ctrl.rxs2,wb_ctrl.rxs1,
                     wb_ctrl.sel_alu2,wb_ctrl.sel_alu1,wb_ctrl.sel_imm,wb_ctrl.alu_dw,wb_ctrl.alu_fn,wb_ctrl.mem,
                     wb_ctrl.mem_cmd,wb_ctrl.mem_type,wb_ctrl.rma,wb_ctrl.wmd,wb_ctrl.wmad,wb_ctrl.wfd,wb_ctrl.div,
                     wb_ctrl.wxd,wb_ctrl.csr,wb_ctrl.fence,wb_ctrl.mem_g,wb_ctrl.gsc,wb_ctrl.gfx,wb_ctrl.x31};

`define MIN_IFC_MONITOR arch_state_mon

// The monitor saves the instruction bits as they pass through the pipeline
`ifdef ENABLE_BEMU
assign ex_inst_bits   = `MIN_IFC_MONITOR.ex_reg_inst;  // *_reg_inst should be the same as *_reg_inst_xcpt
assign tag_inst_bits  = `MIN_IFC_MONITOR.tag_reg_inst; // use *_reg_inst just in case..
assign mem_inst_bits  = `MIN_IFC_MONITOR.mem_reg_inst;
assign wb_inst_bits   = `MIN_IFC_MONITOR.wb_reg_inst_xcpt;
`else // ifdef ENABLE_BEMU
assign ex_inst_bits   = '0;
assign tag_inst_bits  = '0;
assign mem_inst_bits  = '0;
assign wb_inst_bits   = '0;
`endif // !ifdef ENABLE_BEMU



////////////////////////////////////////////////////////////////////

// FCC stall/blocking & unblocking related signals and logic:
logic [1:0]       wb_csr_fcc_replay;
logic [1:0][1:0]  csr_fcc_replay_stall;
logic [1:0][1:0]  clear_fcc_stall;

assign  wb_csr_fcc_replay       = `MIN_IFC_INTPIPE.csr_file.csr_replay.wb_csr_replay[11:10];    // [10]=FCC0 [11]=FCC1 
assign  csr_fcc_replay_stall[0] = `MIN_IFC_INTPIPE.csr_file.csr_replay.csr_replay_stall[0][11:10];
assign  csr_fcc_replay_stall[1] = `MIN_IFC_INTPIPE.csr_file.csr_replay.csr_replay_stall[1][11:10];
assign  clear_fcc_stall[0]      = `MIN_IFC_INTPIPE.csr_file.csr_replay.clear_stall[0][11:10];
assign  clear_fcc_stall[1]      = `MIN_IFC_INTPIPE.csr_file.csr_replay.clear_stall[1][11:10];





//=============================
//Covergroup sampling
//=============================
always @(posedge m_clock) begin


    if ((vld_reg_data_1_ex_all | vld_reg_data_1_ex_lsb | vld_reg_data_1_vpu | vld_reg_data_1_alu_op | vld_reg_data_2_ex | vld_reg_data_2_alu_op) === 1'b1) begin
        minion_core_intpipe_cvg_inst.cg_intpipe_general.sample(vld_reg_data_1_ex_all,
                                                               vld_reg_data_1_ex_lsb,
                                                               vld_reg_data_1_vpu,
                                                               vld_reg_data_1_alu_op,
                                                               vld_reg_data_2_ex,
                                                               vld_reg_data_2_alu_op,
                                                               intpipe_bypass[0],
                                                               intpipe_bypass[1],
                                                               `MIN_IFC_INTPIPE.id_thread_id,
                                                               `MIN_IFC_INTPIPE.chicken_bit_intpipe);
    end


    if (`MIN_IFC_INTPIPE.wb_xcpt === 1'b1) begin
        minion_core_intpipe_cvg_inst.cg_intpipe_xcpt.sample(`MIN_IFC_INTPIPE.id_inst_bits,
                                                            ex_inst_bits,
                                                            tag_inst_bits,
                                                            mem_inst_bits,
                                                            wb_inst_bits,
                                                            //`MIN_IFC_INTPIPE.wb_reg_cause,
                                                            `MIN_IFC_INTPIPE.csr_file.cause.cause,
                                                            `MIN_IFC_INTPIPE.id_inst_en,
                                                            `MIN_IFC_INTPIPE.ex_pc_valid,
                                                            `MIN_IFC_INTPIPE.tag_pc_valid,
                                                            `MIN_IFC_INTPIPE.mem_pc_valid,
                                                            `MIN_IFC_INTPIPE.wb_valid,
                                                            `MIN_IFC_INTPIPE.wb_xcpt,
                                                            `MIN_IFC_INTPIPE.id_thread_id,
                                                            `MIN_IFC_INTPIPE.ex_thread_id,
                                                            `MIN_IFC_INTPIPE.tag_thread_id,
                                                            `MIN_IFC_INTPIPE.mem_thread_id,
                                                            `MIN_IFC_INTPIPE.wb_thread_id,
                                                            `MIN_IFC_INTPIPE.chicken_bit_intpipe);
    end


    if (`MIN_IFC_INTPIPE.tag_misprediction === 1'b1) begin
        minion_core_intpipe_cvg_inst.cg_intpipe_branches.sample(`MIN_IFC_INTPIPE.tag_misprediction,
                                                                `MIN_IFC_INTPIPE.id_inst_bits,
                                                                ex_inst_bits,
                                                                `MIN_IFC_INTPIPE.id_valid,
                                                                `MIN_IFC_INTPIPE.ex_reg_valid,
                                                                `MIN_IFC_INTPIPE.tag_valid_qual,
                                                                `MIN_IFC_INTPIPE.id_thread_id,
                                                                `MIN_IFC_INTPIPE.ex_thread_id,
                                                                `MIN_IFC_INTPIPE.tag_thread_id);
    end


    if (`MIN_IFC_INTPIPE.id_valid_qual === 1'b1) begin
        minion_core_intpipe_cvg_inst.cg_intpipe_minstmatch_mask.sample(`MIN_IFC_INTPIPE.id_inst_bits,
                                                                       `MIN_IFC_INTPIPE.minstmatch_reg[0],
                                                                       `MIN_IFC_INTPIPE.minstmatch_reg[1],
                                                                       `MIN_IFC_INTPIPE.minstmask_reg[0].enable,
                                                                       `MIN_IFC_INTPIPE.minstmask_reg[1].enable,
                                                                       `MIN_IFC_INTPIPE.minstmask_reg[0].mask,
                                                                       `MIN_IFC_INTPIPE.minstmask_reg[1].mask,
                                                                       `MIN_IFC_INTPIPE.id_thread_id,
                                                                       `MIN_IFC_INTPIPE.id_valid_qual);
    end


    if (`MIN_IFC_INTPIPE.id_valid_qual === 1'b1) begin
        minion_core_intpipe_cvg_inst.cg_intpipe_fence_in_id.sample(`MIN_IFC_INTPIPE.id_inst_bits,
                                                                   `MIN_IFC_INTPIPE.id_valid_qual,
                                                                   `MIN_IFC_INTPIPE.ex_valid_qual,
                                                                   `MIN_IFC_INTPIPE.tag_valid_qual,
                                                                   `MIN_IFC_INTPIPE.mem_valid_qual,
                                                                   `MIN_IFC_INTPIPE.wb_valid,
                                                                   `MIN_IFC_INTPIPE.wb_xcpt,
                                                                   ex_ctrl.mem,
                                                                   tag_ctrl.mem,
                                                                   mem_ctrl.mem,
                                                                   wb_ctrl.mem,
                                                                   `MIN_IFC_INTPIPE.id_thread_id,
                                                                   `MIN_IFC_INTPIPE.ex_thread_id,
                                                                   `MIN_IFC_INTPIPE.tag_thread_id,
                                                                   `MIN_IFC_INTPIPE.mem_thread_id,
                                                                   `MIN_IFC_INTPIPE.wb_thread_id);
    end


    if (`MIN_IFC_INTPIPE.csr_excl_mode_transition === 1'b1) begin
        minion_core_intpipe_cvg_inst.cg_intpipe_exclusive_mode.sample(`MIN_IFC_INTPIPE.csr_excl_mode,
                                                                      `MIN_IFC_INTPIPE.csr_excl_mode_transition,
                                                                      `MIN_IFC_INTPIPE.csr_excl_mode_sel,
                                                                      `MIN_IFC_INTPIPE.id_valid,
                                                                      `MIN_IFC_INTPIPE.ex_reg_valid,
                                                                      `MIN_IFC_INTPIPE.tag_reg_valid,
                                                                      `MIN_IFC_INTPIPE.mem_reg_valid,
                                                                      `MIN_IFC_INTPIPE.wb_reg_valid,
                                                                      `MIN_IFC_INTPIPE.id_thread_id,
                                                                      `MIN_IFC_INTPIPE.ex_thread_id,
                                                                      `MIN_IFC_INTPIPE.tag_thread_id,
                                                                      `MIN_IFC_INTPIPE.mem_thread_id,
                                                                      `MIN_IFC_INTPIPE.wb_thread_id);
    end


    if (`MIN_IFC_INTPIPE.csr_file.io_rw_mem_valid_qual === 1'b1) begin
        minion_core_intpipe_cvg_inst.cg_csr.sample(`MIN_IFC_INTPIPE.csr_file.io_rw_mem_addr,
                                                   `MIN_IFC_INTPIPE.csr_file.io_rw_mem_valid_qual,
                                                   `MIN_IFC_INTPIPE.csr_file.io_rw_mem_cmd,
                                                   `MIN_IFC_INTPIPE.csr_file.prv[0],
                                                   `MIN_IFC_INTPIPE.csr_file.prv[1],
                                                   `MIN_IFC_INTPIPE.mem_thread_id);

        minion_core_intpipe_cvg_inst.cg_intpipe_csr_fields.sample(`MIN_IFC_INTPIPE.csr_file.io_rw_mem_addr,
                                                                  `MIN_IFC_INTPIPE.csr_file.io_rw_mem_valid_qual,
                                                                  `MIN_IFC_INTPIPE.csr_file.io_rw_mem_wdata,
                                                                  `MIN_IFC_INTPIPE.csr_file.reg_matp.lock,
                                                                  `MIN_IFC_INTPIPE.csr_file.int_meip,
                                                                  `MIN_IFC_INTPIPE.csr_file.int_mieco,
                                                                  `MIN_IFC_INTPIPE.csr_file.int_msip,
                                                                  `MIN_IFC_INTPIPE.csr_file.int_mtip,
                                                                  `MIN_IFC_INTPIPE.csr_file.int_seip,
                                                                  `MIN_IFC_INTPIPE.mem_thread_id);
    end


    // FCC stall/blocking & unblocking related:
    if ( (|`MIN_IFC_INTPIPE.csr_file.csr_replay.reg_fcc_en[1:0] === 1'b1) ||
         (|`MIN_IFC_INTPIPE.csr_file.csr_replay.clear_stall[0][11:10] === 1'b1) ||
         (|`MIN_IFC_INTPIPE.csr_file.csr_replay.clear_stall[1][11:10] === 1'b1) )
    begin 
       minion_core_intpipe_cvg_inst.cg_csr_fcc_blocking_unblocking.sample (
                     `MIN_IFC_INTPIPE.csr_file.csr_replay.reg_fcc_en,
                     `MIN_IFC_INTPIPE.csr_file.csr_replay.fcc_ready,
                     `MIN_IFC_INTPIPE.csr_file.csr_replay.wb_fcc_addr,
                     wb_csr_fcc_replay,
                     csr_fcc_replay_stall,
                     clear_fcc_stall,
                     `MIN_IFC_INTPIPE.csr_file.csr_replay.io_interrupt,
                     `MIN_IFC_INTPIPE.csr_file.csr_replay.io_redirect
                   );
    end


end

`endif
