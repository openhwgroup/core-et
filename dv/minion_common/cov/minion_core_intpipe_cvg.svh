//==================================================================
//Name: minion_core_intpipe_cvg.svh
//Path: dv/minion_common/cov
//Description: Collecting coverage related to MINION CORE Intpipe
//Owner: Nikola Rajovic, Goran Ognjanovic
//==================================================================

`ifdef EVL_COVERAGE_ON

`ifndef __MINION_CORE_INTPIPE_CVG
`define __MINION_CORE_INTPIPE_CVG


`include "dv/minion_common/cov/minion_core_intpipe_cvg_macros.svh"
`include "dv/minion_common/cov/instructions_table.svh"

typedef enum bit[2:0] {EX_2_ID, TAG_2_ID, MEM_2_ID, DCACHE_2_ID, WB_2_ID, OFF} intpipe_bypass_t;

class minion_core_intpipe_cvg;
    // ============================================================
    // General MINION CORE Intpipe cover group
    // ============================================================
    covergroup cg_intpipe_general with function sample (logic vld_reg_data_1_ex_all,
                                                        logic vld_reg_data_1_ex_lsb,
                                                        logic vld_reg_data_1_vpu,
                                                        logic vld_reg_data_1_alu_op,
                                                        logic vld_reg_data_2_ex,
                                                        logic vld_reg_data_2_alu_op,
                                                        intpipe_bypass_t intpipe_bypass_1,
                                                        intpipe_bypass_t intpipe_bypass_2,
                                                        logic id_thread_id,
                                                        logic chicken_bit_intpipe);

        option.name = "cg_intpipe_general";
        option.per_instance = 0;
        type_option.merge_instances = 1;

        // covering all bypasses when ID data is used
        cp_intpipe_bypass_1 : coverpoint intpipe_bypass_1 iff (vld_reg_data_1_ex_all || vld_reg_data_1_ex_lsb || vld_reg_data_1_vpu || vld_reg_data_1_alu_op) {
            bins EX_2_ID_bypass     = {EX_2_ID};
            bins AG_2_ID_bypass     = {TAG_2_ID};
            bins MEM_2_ID_bypass    = {MEM_2_ID};
            bins DCACHE_2_ID_bypass = {DCACHE_2_ID};
            bins WB_2_ID_bypass     = {WB_2_ID};
            bins no_bypass          = {OFF};
        }

        cp_intpipe_bypass_2 : coverpoint intpipe_bypass_2 iff (vld_reg_data_2_ex || vld_reg_data_2_alu_op) {
            bins EX_2_ID_bypass     = {EX_2_ID};
            bins AG_2_ID_bypass     = {TAG_2_ID};
            bins MEM_2_ID_bypass    = {MEM_2_ID};
            bins DCACHE_2_ID_bypass = {DCACHE_2_ID};
            bins WB_2_ID_bypass     = {WB_2_ID};
            bins no_bypass          = {OFF};
        }

        cp_thread_id : coverpoint id_thread_id {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_valid_bypass_1 : coverpoint {vld_reg_data_1_ex_all, vld_reg_data_1_ex_lsb, vld_reg_data_1_vpu, vld_reg_data_1_alu_op} {
            wildcard bins ALU_op_valid = {4'b???1};
            wildcard bins VPU_valid    = {4'b??1?};
            wildcard bins EX_LSB_valid = {4'b?1??};
            wildcard bins EX_ALL_valid = {4'b1???};
                     bins NOT_valid    = default;
        }

        cp_valid_bypass_2 : coverpoint {vld_reg_data_2_ex, vld_reg_data_2_alu_op} {
            wildcard bins ALU_op_valid = {2'b?1};
            wildcard bins EX_valid     = {2'b1?};
                     bins NOT_valid    = default;
        }

        cp_chicken_bit_intpipe: coverpoint {chicken_bit_intpipe} {
            bins enabled  = {1'b0};
            bins disabled = {1'b1};
        }

        cross_intpipe_bypass: cross cp_intpipe_bypass_1, cp_intpipe_bypass_2 {
            ignore_bins bypass_cross_NOT_valid_1 = binsof(cp_intpipe_bypass_1.DCACHE_2_ID_bypass) && binsof(cp_intpipe_bypass_2.WB_2_ID_bypass);
            ignore_bins bypass_cross_NOT_valid_2 = binsof(cp_intpipe_bypass_2.DCACHE_2_ID_bypass) && binsof(cp_intpipe_bypass_1.WB_2_ID_bypass);
        }


        cross_intpipe_bypass_1_thread_id: cross cp_intpipe_bypass_1, cp_thread_id;
        cross_intpipe_bypass_2_thread_id: cross cp_intpipe_bypass_2, cp_thread_id;

        cross_intpipe_bypass_1_valid: cross cp_intpipe_bypass_1, cp_valid_bypass_1 {
            //ignore_bins bypass_NOT_valid = binsof(cp_valid_bypass_1.NOT_valid);
            ignore_bins bypass_NOT_valid_0 = binsof(cp_intpipe_bypass_1.EX_2_ID_bypass) && binsof(cp_valid_bypass_1.NOT_valid);
            ignore_bins bypass_NOT_valid_1 = binsof(cp_intpipe_bypass_1.AG_2_ID_bypass) && binsof(cp_valid_bypass_1.NOT_valid);
            ignore_bins bypass_NOT_valid_2 = binsof(cp_intpipe_bypass_1.MEM_2_ID_bypass) && binsof(cp_valid_bypass_1.NOT_valid);
            ignore_bins bypass_NOT_valid_3 = binsof(cp_intpipe_bypass_1.DCACHE_2_ID_bypass) && binsof(cp_valid_bypass_1.NOT_valid);
            ignore_bins bypass_NOT_valid_4 = binsof(cp_intpipe_bypass_1.WB_2_ID_bypass) && binsof(cp_valid_bypass_1.NOT_valid);
            ignore_bins bypass_NOT_valid_5 = binsof(cp_intpipe_bypass_1.no_bypass) && binsof(cp_valid_bypass_1.NOT_valid);
        }

        cross_intpipe_bypass_2_valid: cross cp_intpipe_bypass_2, cp_valid_bypass_2{
            //ignore_bins bypass_NOT_valid = binsof(cp_valid_bypass_2.NOT_valid);
            ignore_bins bypass_NOT_valid_0 = binsof(cp_intpipe_bypass_2.EX_2_ID_bypass) && binsof(cp_valid_bypass_2.NOT_valid);
            ignore_bins bypass_NOT_valid_1 = binsof(cp_intpipe_bypass_2.AG_2_ID_bypass) && binsof(cp_valid_bypass_2.NOT_valid);
            ignore_bins bypass_NOT_valid_2 = binsof(cp_intpipe_bypass_2.MEM_2_ID_bypass) && binsof(cp_valid_bypass_2.NOT_valid);
            ignore_bins bypass_NOT_valid_3 = binsof(cp_intpipe_bypass_2.DCACHE_2_ID_bypass) && binsof(cp_valid_bypass_2.NOT_valid);
            ignore_bins bypass_NOT_valid_4 = binsof(cp_intpipe_bypass_2.WB_2_ID_bypass) && binsof(cp_valid_bypass_2.NOT_valid);
            ignore_bins bypass_NOT_valid_5 = binsof(cp_intpipe_bypass_2.no_bypass) && binsof(cp_valid_bypass_2.NOT_valid);
        }

        cross_intpipe_bypass_1_chicken_bit_intpipe: cross cp_intpipe_bypass_1, cp_chicken_bit_intpipe;
        corss_intpipe_bypass_2_chicken_bit_intpipe: cross cp_intpipe_bypass_2, cp_chicken_bit_intpipe;

    endgroup : cg_intpipe_general



    // ============================================================
    // Exception MINION CORE Intpipe cover group
    // ============================================================
    covergroup cg_intpipe_xcpt with function sample (logic [31:0] id_inst_bits,
                                                     logic [31:0] ex_inst_bits,
                                                     logic [31:0] tag_inst_bits,
                                                     logic [31:0] mem_inst_bits,
                                                     logic [31:0] wb_inst_bits,
                                                     core_xcpt_cause xcpt_type,
                                                     logic id_valid,
                                                     logic ex_valid,
                                                     logic tag_valid,
                                                     logic mem_valid,
                                                     logic wb_valid,
                                                     logic wb_xcpt,
                                                     logic id_thread_id,
                                                     logic ex_thread_id,
                                                     logic tag_thread_id,
                                                     logic mem_thread_id,
                                                     logic wb_thread_id,
                                                     logic chicken_bit_intpipe);

        option.name = "cg_intpipe_xcpt";
        option.per_instance = 0;
        type_option.merge_instances = 1;

        // covering all exceptions in WB stage
        cp_intpipe_expt_type : coverpoint xcpt_type iff (wb_xcpt) {
            bins FETCH_ACCESS         = { core_xcpt_cause_FETCH_ACCESS };
            bins ILLEGAL_INSTRUCTION  = { core_xcpt_cause_ILLEGAL_INSTRUCTION };
            bins BREAKPOINT           = { core_xcpt_cause_BREAKPOINT };
            bins LOAD_ACCESS          = { core_xcpt_cause_LOAD_ACCESS };
            bins STORE_ACCESS         = { core_xcpt_cause_STORE_ACCESS };
            bins USER_ECALL           = { core_xcpt_cause_USER_ECALL };
            bins SUPERVISOR_ECALL     = { core_xcpt_cause_SUPERVISOR_ECALL };
            bins MACHINE_ECALL        = { core_xcpt_cause_MACHINE_ECALL };
            bins DEBUG                = { core_xcpt_cause_DEBUG };
            bins MCODE_INSTRUCTION    = { core_xcpt_cause_MCODE_INSTRUCTION };
            ignore_bins ignore_values = { core_xcpt_cause_FETCH_PAGE_FAULT,
                                          core_xcpt_cause_LOAD_PAGE_FAULT,
                                          core_xcpt_cause_STORE_PAGE_FAULT,
                                          core_xcpt_cause_FETCH_BUS_ERROR,
                                          core_xcpt_cause_FETCH_ECC_ERROR,
                                          core_xcpt_cause_LOAD_PAGE_SPLIT_FAULT,
                                          core_xcpt_cause_STORE_PAGE_SPLIT_FAULT,
                                          core_xcpt_cause_MISALIGNED_FETCH,
                                          core_xcpt_cause_MISALIGNED_LOAD,
                                          core_xcpt_cause_MISALIGNED_STORE
                                        };
        }
        // core_xcpt_cause_MEIP
        // core_xcpt_cause_SEIP
        // core_xcpt_cause_UEIP
        // core_xcpt_cause_MSIP
        // core_xcpt_cause_SSIP
        // core_xcpt_cause_USIP
        // core_xcpt_cause_MTIP
        // core_xcpt_cause_STIP
        // core_xcpt_cause_UTIP
        // core_xcpt_cause_MBAD_RED
        // core_xcpt_cause_MIECO
        // core_xcpt_cause_BUS_ERROR

        cp_chicken_bit_intpipe: coverpoint {chicken_bit_intpipe} {
            bins enabled  = {1'b0};
            bins disabled = {1'b1};
        }

        // covering all instructions in WB stage => same as cp_ins_type_WB_expt_WB
        // cp_instructions: coverpoint wb_inst_bits iff (wb_xcpt) { `instruction_groups_macro }

        cp_id_thread_id : coverpoint id_thread_id iff (wb_xcpt) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_ex_thread_id : coverpoint ex_thread_id iff (wb_xcpt) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_tag_thread_id : coverpoint tag_thread_id iff (wb_xcpt) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_mem_thread_id : coverpoint mem_thread_id iff (wb_xcpt) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_wb_thread_id : coverpoint wb_thread_id iff (wb_xcpt) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        // covering all instruction types in ID stage when exception is taken in WB stage
        cp_ins_type_ID_expt_WB : coverpoint id_inst_bits iff (id_valid && wb_xcpt) { `instruction_groups_macro }

        // covering all instruction types in EX stage when exception is taken in WB stage
        cp_ins_type_EX_expt_WB : coverpoint ex_inst_bits iff (ex_valid && wb_xcpt) { `instruction_groups_macro }

        // covering all instruction types in TAG stage when exception is taken in WB stage
        cp_ins_type_TAG_expt_WB : coverpoint tag_inst_bits iff (tag_valid && wb_xcpt) { `instruction_groups_macro }

        // covering all instruction types in MEM stage when exception is taken in WB stage
        cp_ins_type_MEM_expt_WB : coverpoint mem_inst_bits iff (mem_valid && wb_xcpt) { `instruction_groups_macro }

        // covering all instruction types in WB stage when exception is taken in WB stage
        cp_ins_type_WB_expt_WB : coverpoint wb_inst_bits iff (wb_xcpt) { `instruction_groups_macro }


        cross_intpipe_expt_type_instruction_thread_id: cross cp_intpipe_expt_type, cp_ins_type_WB_expt_WB{
            ignore_bins expt_nstruction_NOT_valid_00 = binsof(cp_intpipe_expt_type.FETCH_ACCESS) && !binsof(cp_ins_type_WB_expt_WB.others);
            ignore_bins expt_nstruction_NOT_valid_01 = binsof(cp_intpipe_expt_type.ILLEGAL_INSTRUCTION) && !binsof(cp_ins_type_WB_expt_WB.others);

            ignore_bins expt_nstruction_NOT_valid_02 = binsof(cp_intpipe_expt_type.LOAD_ACCESS) && !binsof(cp_ins_type_WB_expt_WB.load_ins) &&
                                                                                                   !binsof(cp_ins_type_WB_expt_WB.f_single_load_ins) &&
                                                                                                   !binsof(cp_ins_type_WB_expt_WB.et_ps_load_ins) &&
                                                                                                   !binsof(cp_ins_type_WB_expt_WB.atom_fatom_gsc_ins) &&
                                                                                                   !binsof(cp_ins_type_WB_expt_WB.et_ps_cuda_gsc_rd_ins);

            // ignore_bins expt_nstruction_NOT_valid_03 = binsof(cp_intpipe_expt_type.MISALIGNED_LOAD) && !binsof(cp_ins_type_WB_expt_WB.load_ins) &&
            //                                                                                            !binsof(cp_ins_type_WB_expt_WB.f_single_load_ins) &&
            //                                                                                            !binsof(cp_ins_type_WB_expt_WB.et_ps_load_ins) &&
            //                                                                                            !binsof(cp_ins_type_WB_expt_WB.atom_fatom_gsc_ins) &&
            //                                                                                            !binsof(cp_ins_type_WB_expt_WB.et_ps_cuda_gsc_rd_ins);

            ignore_bins expt_nstruction_NOT_valid_04 = binsof(cp_intpipe_expt_type.STORE_ACCESS) && !binsof(cp_ins_type_WB_expt_WB.store_ins) &&
                                                                                                    !binsof(cp_ins_type_WB_expt_WB.f_single_store_ins) &&
                                                                                                    !binsof(cp_ins_type_WB_expt_WB.et_ps_store_ins) &&
                                                                                                    !binsof(cp_ins_type_WB_expt_WB.atom_fatom_gsc_ins) &&
                                                                                                    !binsof(cp_ins_type_WB_expt_WB.et_ps_cuda_gsc_wr_ins);

            // ignore_bins expt_nstruction_NOT_valid_05 = binsof(cp_intpipe_expt_type.MISALIGNED_STORE) && !binsof(cp_ins_type_WB_expt_WB.store_ins) &&
            //                                                                                             !binsof(cp_ins_type_WB_expt_WB.f_single_store_ins) &&
            //                                                                                             !binsof(cp_ins_type_WB_expt_WB.et_ps_store_ins) &&
            //                                                                                             !binsof(cp_ins_type_WB_expt_WB.atom_fatom_gsc_ins) &&
            //                                                                                             !binsof(cp_ins_type_WB_expt_WB.et_ps_cuda_gsc_wr_ins);

            ignore_bins expt_nstruction_NOT_valid_06 = binsof(cp_intpipe_expt_type.MCODE_INSTRUCTION) && !binsof(cp_ins_type_WB_expt_WB.mcode_ins);
            ignore_bins expt_nstruction_NOT_valid_07 = binsof(cp_intpipe_expt_type.USER_ECALL) && !binsof(cp_ins_type_WB_expt_WB.csr_i_ins);
            ignore_bins expt_nstruction_NOT_valid_08 = binsof(cp_intpipe_expt_type.SUPERVISOR_ECALL) && !binsof(cp_ins_type_WB_expt_WB.csr_i_ins);
            ignore_bins expt_nstruction_NOT_valid_09 = binsof(cp_intpipe_expt_type.MACHINE_ECALL) && !binsof(cp_ins_type_WB_expt_WB.csr_i_ins);
        }

        cross_cp_ins_type_ID_expt_WB_thread_id:  cross cp_ins_type_ID_expt_WB,  cp_wb_thread_id, cp_id_thread_id;
        cross_cp_ins_type_EX_expt_WB_thread_id:  cross cp_ins_type_EX_expt_WB,  cp_wb_thread_id, cp_ex_thread_id;
        cross_cp_ins_type_TAG_expt_WB_thread_id: cross cp_ins_type_TAG_expt_WB, cp_wb_thread_id, cp_tag_thread_id;
        cross_cp_ins_type_MEM_expt_WB_thread_id: cross cp_ins_type_MEM_expt_WB, cp_wb_thread_id, cp_mem_thread_id;
        cross_cp_ins_type_WB_expt_WB_thread_id:  cross cp_ins_type_WB_expt_WB,  cp_wb_thread_id;

        cross_intpipe_expt_type_chicken_bit_intpipe: cross cp_intpipe_expt_type, cp_chicken_bit_intpipe;

    endgroup : cg_intpipe_xcpt



    // ============================================================
    // Branch MINION CORE Intpipe cover group
    // ============================================================
    covergroup cg_intpipe_branches with function sample (logic tag_misprediction,
                                                         logic [31:0] id_inst_bits,
                                                         logic [31:0] ex_inst_bits,
                                                         logic id_valid,
                                                         logic ex_reg_valid,
                                                         logic tag_valid_qual,
                                                         logic id_thread_id,
                                                         logic ex_thread_id,
                                                         logic tag_thread_id);

        option.name = "cg_intpipe_branches";
        option.per_instance = 0;
        type_option.merge_instances = 1;

        cp_id_instructions: coverpoint id_inst_bits iff (tag_misprediction && tag_valid_qual && id_valid) { `instructions_table }

        cp_ex_instructions: coverpoint ex_inst_bits iff (tag_misprediction && tag_valid_qual && ex_reg_valid) { `instructions_table }

        cp_id_thread_id : coverpoint id_thread_id iff (tag_misprediction && tag_valid_qual && id_valid) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_ex_thread_id : coverpoint ex_thread_id iff (tag_misprediction && tag_valid_qual && ex_reg_valid) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_tag_thread_id : coverpoint tag_thread_id iff (tag_misprediction && tag_valid_qual) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cross_intpipe_branch_id: cross cp_id_instructions, cp_id_thread_id, cp_tag_thread_id;
        cross_intpipe_branch_ex: cross cp_ex_instructions, cp_ex_thread_id, cp_tag_thread_id;

    endgroup: cg_intpipe_branches



    // ============================================================
    // Inst Match MINION CORE Intpipe cover group
    // ============================================================
    covergroup cg_intpipe_minstmatch_mask with function sample ( logic [31:0] id_inst_bits,
                                                                 logic [31:0] minstmatch_reg_0,
                                                                 logic [31:0] minstmatch_reg_1,
                                                                 logic minstmask_reg_enable_0,
                                                                 logic minstmask_reg_enable_1,
                                                                 logic [31:0] minstmask_reg_mask_0,
                                                                 logic [31:0] minstmask_reg_mask_1,
                                                                 logic id_thread_id,
                                                                 logic id_valid_qual);

        option.name = "cg_intpipe_minstmatch_mask";
        option.per_instance = 0;
        type_option.merge_instances = 1;

        cp_minstmask_enable_0: coverpoint minstmask_reg_enable_0 iff (id_valid_qual && (id_thread_id == 0)) {
            bins enabled  = {1'b1};
            bins disabled = {1'b0};
        }

        cp_minstmask_enable_1: coverpoint minstmask_reg_enable_1 iff (id_valid_qual && (id_thread_id == 1)) {
            bins enabled  = {1'b1};
            bins disabled = {1'b0};
        }

        cp_minst_condition_0: coverpoint ((id_inst_bits & minstmask_reg_mask_0) == (minstmatch_reg_0 & minstmask_reg_mask_0)) iff (id_valid_qual && (id_thread_id == 0)) {
            bins true  = {1'b1};
            bins false = {1'b0};
        }

        cp_minst_condition_1: coverpoint ((id_inst_bits & minstmask_reg_mask_1) == (minstmatch_reg_1 & minstmask_reg_mask_1)) iff (id_valid_qual && (id_thread_id == 1)) {
            bins true  = {1'b1};
            bins false = {1'b0};
        }

        cross_minstmask_enable_0_minst_cond_0: cross cp_minstmask_enable_0, cp_minst_condition_0;
        cross_minstmask_enable_1_minst_cond_1: cross cp_minstmask_enable_1, cp_minst_condition_1;

    endgroup: cg_intpipe_minstmatch_mask



    // ============================================================
    // Fence MINION CORE Intpipe cover group
    // ============================================================
    covergroup cg_intpipe_fence_in_id with function sample (logic [31:0] id_inst_bits,
                                                            logic id_valid_qual,
                                                            logic ex_valid_qual,
                                                            logic tag_valid_qual,
                                                            logic mem_valid_qual,
                                                            logic wb_valid,
                                                            logic wb_xcpt,
                                                            logic ex_ctrl_mem,
                                                            logic tag_ctrl_mem,
                                                            logic mem_ctrl_mem,
                                                            logic wb_ctrl_mem,
                                                            logic id_thread_id,
                                                            logic ex_thread_id,
                                                            logic tag_thread_id,
                                                            logic mem_thread_id,
                                                            logic wb_thread_id);

        option.name = "cg_intpipe_fence_in_id";
        option.per_instance = 0;
        type_option.merge_instances = 1;

        cp_fence_id_ex_mem: coverpoint ((id_inst_bits ==? 32'b0000????????00000000000000001111) || (id_inst_bits == 32'b00000000000000000001000000001111)) iff (id_valid_qual && ex_ctrl_mem && ex_valid_qual) {
            bins true  = {1'b1};
            bins false = {1'b0};
        }

        cp_fence_id_tag_mem: coverpoint ((id_inst_bits ==? 32'b0000????????00000000000000001111) || (id_inst_bits == 32'b00000000000000000001000000001111)) iff (id_valid_qual && tag_ctrl_mem && tag_valid_qual) {
            bins true  = {1'b1};
            bins false = {1'b0};
        }

        cp_fence_id_mem_mem: coverpoint ((id_inst_bits ==? 32'b0000????????00000000000000001111) || (id_inst_bits == 32'b00000000000000000001000000001111)) iff (id_valid_qual && mem_ctrl_mem && mem_valid_qual) {
            bins true  = {1'b1};
            bins false = {1'b0};
        }

        cp_fence_id_wb_mem: coverpoint ((id_inst_bits ==? 32'b0000????????00000000000000001111) || (id_inst_bits == 32'b00000000000000000001000000001111)) iff (id_valid_qual && wb_ctrl_mem && (wb_valid || wb_xcpt)) {
            bins true  = {1'b1};
            bins false = {1'b0};
        }

        cp_id_thread_id: coverpoint id_thread_id iff (id_valid_qual) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_ex_thread_id: coverpoint ex_thread_id iff (ex_valid_qual) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_tag_thread_id: coverpoint tag_thread_id iff (tag_valid_qual) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_mem_thread_id: coverpoint mem_thread_id iff (mem_valid_qual) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cp_wb_thread_id: coverpoint wb_thread_id iff (wb_valid || wb_xcpt) {
            bins thread_ID_0 = {1'b0};
            bins thread_ID_1 = {1'b1};
        }

        cross_fence_id_ex_mem_thread_id: cross cp_fence_id_ex_mem, cp_id_thread_id, cp_ex_thread_id;
        cross_fence_id_tag_mem_thread_id: cross cp_fence_id_tag_mem, cp_id_thread_id, cp_tag_thread_id;
        cross_fence_id_mem_mem_thread_id: cross cp_fence_id_mem_mem, cp_id_thread_id, cp_mem_thread_id;
        cross_fence_id_wb_mem_thread_id: cross cp_fence_id_wb_mem, cp_id_thread_id, cp_wb_thread_id;

    endgroup: cg_intpipe_fence_in_id



    // ============================================================
    // Exclusive Mode MINION CORE Intpipe cover group
    // ============================================================
    covergroup cg_intpipe_exclusive_mode with function sample ( logic csr_excl_mode,
                                                                logic csr_excl_mode_transition,
                                                                logic csr_excl_mode_sel,
                                                                logic id_valid,
                                                                logic ex_reg_valid,
                                                                logic tag_reg_valid,
                                                                logic mem_reg_valid,
                                                                logic wb_reg_valid,
                                                                logic id_thread_id,
                                                                logic ex_thread_id,
                                                                logic tag_thread_id,
                                                                logic mem_thread_id,
                                                                logic wb_thread_id);

        option.name = "cg_intpipe_exclusive_mode";
        option.per_instance = 0;
        type_option.merge_instances = 1;

        cp_exclusive: coverpoint (csr_excl_mode) iff (csr_excl_mode_transition) {
            bins entering = {1'b0};
            bins leaving  = {1'b1};
        }

        cp_exclusive_selection: coverpoint (csr_excl_mode_sel) iff (csr_excl_mode_transition) {
            bins thread_0 = {1'b0};
            bins thread_1 = {1'b1};
        }

        cp_exclusive_id_valid: coverpoint (id_thread_id != csr_excl_mode_sel) iff (!csr_excl_mode && csr_excl_mode_transition && id_valid) {
            bins other_thread = {1'b1};
            bins same_thread  = {1'b0};
        }

        cp_exclusive_ex_valid: coverpoint (ex_thread_id != csr_excl_mode_sel) iff (!csr_excl_mode && csr_excl_mode_transition && ex_reg_valid) {
            bins other_thread = {1'b1};
            bins same_thread  = {1'b0};
        }

        cp_exclusive_tag_valid: coverpoint (tag_thread_id != csr_excl_mode_sel) iff (!csr_excl_mode && csr_excl_mode_transition && tag_reg_valid) {
            bins other_thread = {1'b1};
            bins same_thread  = {1'b0};
        }

        cp_exclusive_mem_valid: coverpoint (mem_thread_id != csr_excl_mode_sel) iff (!csr_excl_mode && csr_excl_mode_transition && mem_reg_valid) {
            bins other_thread = {1'b1};
            bins same_thread  = {1'b0};
        }

        cp_exclusive_wb_valid: coverpoint (wb_thread_id != csr_excl_mode_sel) iff (!csr_excl_mode && csr_excl_mode_transition && wb_reg_valid) {
            bins other_thread = {1'b1};
            bins same_thread  = {1'b0};
        }

        cross_exclusive_entering_selection:  cross cp_exclusive, cp_exclusive_selection;
        cross_exclusive_selection_id_valid:  cross cp_exclusive_selection, cp_exclusive_id_valid;
        cross_exclusive_selection_ex_valid:  cross cp_exclusive_selection, cp_exclusive_ex_valid;
        cross_exclusive_selection_tag_valid: cross cp_exclusive_selection, cp_exclusive_tag_valid;
        cross_exclusive_selection_mem_valid: cross cp_exclusive_selection, cp_exclusive_mem_valid;
        cross_exclusive_selection_wb_valid:  cross cp_exclusive_selection, cp_exclusive_wb_valid;

    endgroup: cg_intpipe_exclusive_mode



    // ============================================================
    // CSR Fields MINION CORE Intpipe cover group
    // ============================================================
    covergroup cg_intpipe_csr_fields with function sample (logic [11:0] io_rw_mem_addr,
                                                           logic io_rw_mem_valid_qual,
                                                           logic [63:0] io_rw_mem_wdata,
                                                           logic matp_lock,
                                                           logic[1:0] int_meip,
                                                           logic[1:0] int_mieco,
                                                           logic[1:0] int_msip,
                                                           logic[1:0] int_mtip,
                                                           logic[1:0] int_seip,
                                                           logic mem_thread_id );

        option.name = "cg_intpipe_csr_fields";
        option.per_instance = 0;
        type_option.merge_instances = 1;

        cp_sgn_ext: coverpoint (((|(io_rw_mem_wdata[63:49]) == 1'b0) && (io_rw_mem_wdata[48] == 1'b0)) ||
                                ((&(io_rw_mem_wdata[63:49]) == 1'b1) && (io_rw_mem_wdata[48] == 1'b1))) iff (io_rw_mem_valid_qual) {
            bins mepc_valid    = {1'b1} iff (io_rw_mem_addr == 12'h341);
            bins mepc_invalid  = {1'b0} iff (io_rw_mem_addr == 12'h341);
            bins sepc_valid    = {1'b1} iff (io_rw_mem_addr == 12'h141);
            bins sepc_invalid  = {1'b0} iff (io_rw_mem_addr == 12'h141);
            bins stvec_valid   = {1'b1} iff (io_rw_mem_addr == 12'h105);
            bins stvec_invalid = {1'b0} iff (io_rw_mem_addr == 12'h105);
            bins stval_valid   = {1'b1} iff (io_rw_mem_addr == 12'h143);
            bins stval_invalid = {1'b0} iff (io_rw_mem_addr == 12'h143);
            bins mtvec_valid   = {1'b1} iff (io_rw_mem_addr == 12'h305);
            bins mtvec_invalid = {1'b0} iff (io_rw_mem_addr == 12'h305);
            bins mtval_valid   = {1'b1} iff (io_rw_mem_addr == 12'h343);
            bins mtval_invalid = {1'b0} iff (io_rw_mem_addr == 12'h343);
            bins dpc_valid     = {1'b1} iff (io_rw_mem_addr == 12'h7b1);
            bins dpc_invalid   = {1'b0} iff (io_rw_mem_addr == 12'h7b1);
        }

        cp_warl_mstatus_privilege: coverpoint (io_rw_mem_wdata[12:11] != 2'b10) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h300)) {
            bins valid   = {1'b1};
            bins invalid = {1'b0};
        }

        cp_sd_mstatus_read_only: coverpoint (io_rw_mem_wdata[63:63] == |io_rw_mem_wdata[16:13]) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h300)) {
            bins valid   = {1'b1};
            bins invalid = {1'b0};
        }

        cp_sxl_mstatus_read_only: coverpoint (io_rw_mem_wdata[35:34] == 2'b10) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h300)) {
            bins valid   = {1'b1};
            bins invalid = {1'b0};
        }

        cp_uxl_mstatus_read_only: coverpoint (io_rw_mem_wdata[33:32] == 2'b10) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h300)) {
            bins valid   = {1'b1};
            bins invalid = {1'b0};
        }

        cp_warl_dcsr_privilege: coverpoint (io_rw_mem_wdata[1:0] != 2'b10) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h7b0)) {
            bins valid   = {1'b1};
            bins invalid = {1'b0};
        }

        cp_dcsr_read_only: coverpoint ((io_rw_mem_wdata[11:9]  == 0) &&
                                       (io_rw_mem_wdata[31:28] == 4)) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h7b0)) {
            bins valid   = {1'b1};
            bins invalid = {1'b0};
        }

        cp_warl_satp: coverpoint (io_rw_mem_wdata) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h180)) {
            wildcard bins valid_mode_0 = {64'h0???????????????};
            wildcard bins valid_mode_8 = {64'h8???????????????};
            wildcard bins valid_mode_9 = {64'h9???????????????};
                     bins invalid_mode = default;
        }
        //---currently not supported by BEMU
        cp_warl_matp: coverpoint (io_rw_mem_wdata) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h7c0)) {
            wildcard bins valid_mode_0_lock_disable = {64'h0???????????????} iff (matp_lock ==0);
            wildcard bins valid_mode_8_lock_disable = {64'h8???????????????} iff (matp_lock ==0);
            wildcard bins valid_mode_9_lock_disable = {64'h9???????????????} iff (matp_lock ==0);
            wildcard bins valid_mode_0_lock_enable  = {64'h0???????????????} iff (matp_lock ==1);
            wildcard bins valid_mode_8_lock_enable  = {64'h8???????????????} iff (matp_lock ==1);
            wildcard bins valid_mode_9_lock_enable  = {64'h9???????????????} iff (matp_lock ==1);
                     bins invalid_mode              = default;
        }

        cp_sie_read_zero: coverpoint ((|io_rw_mem_wdata[63:10] == 1'b0) &&
                                      (io_rw_mem_wdata[8]      == 1'b0) &&
                                      (|io_rw_mem_wdata[5:4]   == 1'b0) &&
                                      (io_rw_mem_wdata[0]      == 1'b0)) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h104)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_sip_read_zero: coverpoint ((|io_rw_mem_wdata[63:10] == 1'b0) &&
                                      (|io_rw_mem_wdata[8:6]   == 1'b0) &&
                                      (|io_rw_mem_wdata[4:2]   == 1'b0) &&
                                      (io_rw_mem_wdata[0]      == 1'b0)) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h144)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_mip_read_zero: coverpoint ((|io_rw_mem_wdata[63:20] == 0) &&
                                      (|io_rw_mem_wdata[18:16] == 0) &&
                                      (|io_rw_mem_wdata[14:12] == 0) &&
                                      (io_rw_mem_wdata[10]     == 0) &&
                                      (io_rw_mem_wdata[8]      == 0) &&
                                      (io_rw_mem_wdata[6]      == 0) &&
                                      (io_rw_mem_wdata[4]      == 0) &&
                                      (io_rw_mem_wdata[2]      == 0) &&
                                      (io_rw_mem_wdata[0]      == 0)) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h344)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_mip_vs_interrupts_thread0: coverpoint ((io_rw_mem_wdata[19] == int_mieco[0]) &&
                                                  (io_rw_mem_wdata[11] == int_meip[0])  &&
                                                  (io_rw_mem_wdata[9]  == int_seip[0])  &&
                                                  (io_rw_mem_wdata[7]  == int_mtip[0])  &&
                                                  (io_rw_mem_wdata[3]  == int_msip[0])) iff ((mem_thread_id == 1'b0) && io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h344)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_mip_vs_interrupts_thread1: coverpoint ((io_rw_mem_wdata[19] == int_mieco[1]) &&
                                                  (io_rw_mem_wdata[11] == int_meip[1])  &&
                                                  (io_rw_mem_wdata[9]  == int_seip[1])  &&
                                                  (io_rw_mem_wdata[7]  == int_mtip[1])  &&
                                                  (io_rw_mem_wdata[3]  == int_msip[1])) iff ((mem_thread_id == 1'b1) && io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h344)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_mideleg_read_zero: coverpoint ((|io_rw_mem_wdata[63:10] == 1'b0) &&
                                          (|io_rw_mem_wdata[8:6]   == 1'b0) &&
                                          (|io_rw_mem_wdata[4:2]   == 1'b0) &&
                                          (io_rw_mem_wdata[0]      == 1'b0)) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h303)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_medeleg_read_zero: coverpoint ((|io_rw_mem_wdata[31:16] == 0) &&
                                          (|io_rw_mem_wdata[14:14] == 0) &&
                                          (|io_rw_mem_wdata[11:9]  == 0) &&
                                          (io_rw_mem_wdata[7:4]    == 0) &&
                                          (io_rw_mem_wdata[2:1]    == 0)) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h302)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }


        cp_tdata1_read_valid: coverpoint ((io_rw_mem_wdata[63:60] == 4'h2) &&
                                          (io_rw_mem_wdata[58:53] == 6'h6) &&
                                          (io_rw_mem_wdata[52:19] == 0) &&
                                          (io_rw_mem_wdata[17:13] == 0) &&
                                          (io_rw_mem_wdata[11:8] == 0) &&
                                          (io_rw_mem_wdata[5:5] == 0) )
                                         iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h7A1)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_sstatus_read_zero: coverpoint ((io_rw_mem_wdata[62:34] == 0) &&
                                          (io_rw_mem_wdata[31:20] == 0) &&
                                          (io_rw_mem_wdata[17]    == 0) &&
                                          (io_rw_mem_wdata[12:9]  == 0) &&
                                          (io_rw_mem_wdata[7:6]   == 0) &&
                                          (io_rw_mem_wdata[4:2]   == 0) &&
                                          (io_rw_mem_wdata[0]     == 0)) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h100)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_stvec_read_zero: coverpoint (io_rw_mem_wdata[11:1] == 0) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h105)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_sepc_read_zero: coverpoint (io_rw_mem_wdata[0] == 0) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h141)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_scause_read_zero: coverpoint (io_rw_mem_wdata[62:5] == 0) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h142)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_satp_read_zero: coverpoint (io_rw_mem_wdata[59:28] == 0) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h180)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_mepc_read_zero: coverpoint (io_rw_mem_wdata[0] == 0) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h341)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_mcause_read_zero: coverpoint (io_rw_mem_wdata[62:5] == 0) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h342)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_tdata1_read_zero: coverpoint ((io_rw_mem_wdata[52:19] == 0) &&
                                         (io_rw_mem_wdata[17:13] == 0) &&
                                         (io_rw_mem_wdata[11:8]  == 0) &&
                                         (io_rw_mem_wdata[5]     == 0)) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h7a1)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_dcsr_read_zero: coverpoint ((io_rw_mem_wdata[27:16] == 0) &&
                                       (io_rw_mem_wdata[14]    == 0) &&
                                       (io_rw_mem_wdata[11:9]  == 0) &&
                                       (io_rw_mem_wdata[5]     == 0)) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h7b0)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_dpc_read_zero: coverpoint (io_rw_mem_wdata[0] == 0) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h7b1)) {
            bins valid_tried_to_write   = {1'b1};
            bins invalid_tried_to_write = {1'b0};
        }

        cp_matp_read_zero: coverpoint (io_rw_mem_wdata[58:28] == 0) iff (io_rw_mem_valid_qual && (io_rw_mem_addr == 12'h7c0)) {
            bins valid_tried_to_write =   {1'b1};
            bins invalid_tried_to_write=  {1'b0};
        }

    endgroup: cg_intpipe_csr_fields



    // ============================================================
    // FCC stall/blocking & unblocking cover group:
    // ## unblocking due to: (1) credit available; (2) interrupt;
    // ============================================================
    covergroup cg_csr_fcc_blocking_unblocking with function sample (
                             logic [1:0]       reg_fcc_en,
                             logic [3:0]       fcc_ready,              // fcc_ready[wb_fcc_addr]
                             logic [1:0]       wb_fcc_addr,            // = {wb_thread_id, wb_wdata[0]}
                             logic [1:0]       wb_csr_fcc_replay,      // [0]=FCC0 [1]=FCC1
                             logic [1:0][1:0]  csr_fcc_replay_stall,   // first [] is for Threads; second [0]=FCC0 [1]=FCC1
                             logic [1:0][1:0]  clear_fcc_stall,        // first [] is for Threads; second [0]=FCC0 [1]=FCC1
                             logic [1:0]       io_interrupt,
                             logic [1:0]       io_redirect
                            );

        option.name = "cg_csr_fcc_blocking_unblocking";
        option.per_instance = 0;
        type_option.merge_instances = 1;

        cp_fcc_blocking: coverpoint (wb_fcc_addr)
                //iff ((|reg_fcc_en[1:0] == 1'b1) &&        // [0]=Th0, [1]=Th1 
                //     (|wb_csr_fcc_replay[1:0] == 1'b1))   // [0]=CSR_REPLAY_FCC0, [1]=CSR_REPLAY_FCC1
                iff (   (reg_fcc_en[0] && !fcc_ready[{1'b0,1'b0}] && wb_csr_fcc_replay[0])   // Thread0 FCC0
                     || (reg_fcc_en[0] && !fcc_ready[{1'b0,1'b1}] && wb_csr_fcc_replay[1])   // Thread0 FCC1
                     || (reg_fcc_en[1] && !fcc_ready[{1'b1,1'b0}] && wb_csr_fcc_replay[0])   // Thread1 FCC0
                     || (reg_fcc_en[1] && !fcc_ready[{1'b1,1'b1}] && wb_csr_fcc_replay[1])   // Thread1 FCC1
                    )
        {
            bins wb_fcc_addr__th0_fcc0 = {2'b00};
            bins wb_fcc_addr__th0_fcc1 = {2'b01};
            bins wb_fcc_addr__th1_fcc0 = {2'b10};
            bins wb_fcc_addr__th1_fcc1 = {2'b11};
        }

        cp_fcc_unblock_by_credit: coverpoint (clear_fcc_stall)
                iff (   (csr_fcc_replay_stall[0][0] && clear_fcc_stall[0][0] && fcc_ready[{1'b0,1'b0}])  // Thread0 FCC0
                     || (csr_fcc_replay_stall[0][1] && clear_fcc_stall[0][1] && fcc_ready[{1'b0,1'b1}])  // Thread0 FCC1
                     || (csr_fcc_replay_stall[1][0] && clear_fcc_stall[1][0] && fcc_ready[{1'b1,1'b0}])  // Thread1 FCC0
                     || (csr_fcc_replay_stall[1][1] && clear_fcc_stall[1][1] && fcc_ready[{1'b1,1'b1}])  // Thread1 FCC1
                    ) 
        {
           bins clear_fcc_stall__th0_fcc0 = {{2'b00, 2'b01}};
           bins clear_fcc_stall__th0_fcc1 = {{2'b00, 2'b10}};
           bins clear_fcc_stall__th1_fcc0 = {{2'b01, 2'b00}};
           bins clear_fcc_stall__th1_fcc1 = {{2'b10, 2'b00}};
        }

        // Note: When FCC unblock by interrupt: it unblocks both FCC0 and FCC1 in the same Thread; so we will see
        //       either clear_fcc_stall[0]=3'b11 (for Th0) or clear_fcc_stall[1]=3'b11 (for Th1).
        cp_fcc_unblock_by_interrupt_v1: coverpoint (clear_fcc_stall)
                iff (   (csr_fcc_replay_stall[0][0] && clear_fcc_stall[0][0] && (io_interrupt[0]|io_redirect[0]))  // Thread0 FCC0
                     || (csr_fcc_replay_stall[0][1] && clear_fcc_stall[0][1] && (io_interrupt[0]|io_redirect[0]))  // Thread0 FCC1
                     || (csr_fcc_replay_stall[1][0] && clear_fcc_stall[1][0] && (io_interrupt[1]|io_redirect[1]))  // Thread1 FCC0
                     || (csr_fcc_replay_stall[1][1] && clear_fcc_stall[1][1] && (io_interrupt[1]|io_redirect[1]))  // Thread1 FCC1
                    ) 
        {
           bins clear_fcc_stall__th0_fcc0_fcc1 = {{2'b00, 2'b11}};
           bins clear_fcc_stall__th1_fcc0_fcc1 = {{2'b11, 2'b00}};
        }

        // Or: For FCC unblock by interrupt:
        //     we can change to coverpoint on csr_fcc_replay_stall (instead of clear_fcc_stall);
        cp_fcc_unblock_by_interrupt_v2: coverpoint (csr_fcc_replay_stall)
                iff (   (csr_fcc_replay_stall[0][0] && clear_fcc_stall[0][0] && (io_interrupt[0]|io_redirect[0]))  // Thread0 FCC0
                     || (csr_fcc_replay_stall[0][1] && clear_fcc_stall[0][1] && (io_interrupt[0]|io_redirect[0]))  // Thread0 FCC1
                     || (csr_fcc_replay_stall[1][0] && clear_fcc_stall[1][0] && (io_interrupt[1]|io_redirect[1]))  // Thread1 FCC0
                     || (csr_fcc_replay_stall[1][1] && clear_fcc_stall[1][1] && (io_interrupt[1]|io_redirect[1]))  // Thread1 FCC1
                    ) 
        {
           bins csr_fcc_replay_stall__th0_fcc0 = {{2'b00, 2'b01}};
           bins csr_fcc_replay_stall__th0_fcc1 = {{2'b00, 2'b10}};
           bins csr_fcc_replay_stall__th1_fcc0 = {{2'b01, 2'b00}};
           bins csr_fcc_replay_stall__th1_fcc1 = {{2'b10, 2'b00}};
        }


    endgroup: cg_csr_fcc_blocking_unblocking



    `include "dv/minion_common/cov/minion_core_intpipe_csr_auto_cvg.svh"


    //================================
    //Covergroup instances
    //================================
    function new();
        cg_intpipe_general = new();
        cg_intpipe_xcpt = new();
        cg_intpipe_branches = new();
        cg_intpipe_minstmatch_mask = new();
        cg_intpipe_fence_in_id = new();
        cg_intpipe_exclusive_mode = new();
        cg_csr = new();
        cg_intpipe_csr_fields = new();
        cg_csr_fcc_blocking_unblocking = new();
    endfunction : new

endclass : minion_core_intpipe_cvg


`endif
`endif
