// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifndef VPU_TRANS_DEFINES

    `define VPU_TRANS_DEFINES

    `include "vpu_defines.vh"
    //INTERNAL TO SEQUENCER

    `define     TRANS_PIPE_STAGES             7

    `define     TRANS_SCOREBOARD_SLOTS        (`VPU_TRANS_SCOREBOARD_SIZE/2)
    `define     TRANS_MASK_DELAY              1
    `define     TRANS_WRITE_CTRL_DELAY        1
    `define     TRANS_CSTATE(st, old)  {st, old.thread_id, old.rs, old.rd, old.mask_valid, old.mask} //Creare trans State from a state id, and its registers.
    `define     TRANS_CISTATE(st, tid, instr, mask)   {st, tid, instr[`VPU_INST_REN1_RA_SEL], instr[`VPU_INST_RD_SEL], 1'b0, mask} //Create a trans State from a state id, and its origin instruction.

    `define     TRANS_NULL_STATE              { TRANS_NULL, {($bits(trans_state) - $bits(trans_state_id)){1'b0}}} //Define the basic state

    `define TRANS_NOP     `OPCODE_WIDTH'b00000000000000000000000000010011

    `define TRANS_INSTR_CLEAN_X_MASK  { 7'h7f, 5'b0, 5'b0, 3'b0, 5'b0, 7'h7f }
    `define TRANS_INSTR_ADD_REGS_MASK(rd, rs1, rs2) { 7'b0, rs2, rs1, RNE, rd, 7'b0 } 
    `define TRANS_INSTR(code, rd, rs1, rs2) ((code & `TRANS_INSTR_CLEAN_X_MASK) | `TRANS_INSTR_ADD_REGS_MASK(rd, rs1, rs2))

    //ROM DEFINES

    `define TRANS_C2_MAX_SIZE 17
    `define TRANS_C1_MAX_SIZE 24
    `define TRANS_C0_MAX_SIZE 32

    `define TRANS_C1_FILLER {(`TRANS_C0_MAX_SIZE - `TRANS_C1_MAX_SIZE){1'b0}}

    `define TRANS_ROM_MAX_INDEX_SIZE 8 //INDEX

    `define TRANS_MASK_SIZE 7

    //RCP ROM DEFINES

    `define TRANS_RCP_ADDR_RANGE 22:16
    `define TRANS_RCP_ADDR_SIZE (22-16+1) //$bits(logic[`TRANS_RCP_ADDR_RANGE])
    `define TRANS_RCP_C2_RANGE 25:16
    `define TRANS_RCP_C1_RANGE 15:0
    `define TRANS_RCP_C0_RANGE 24:0

    //RSQRT ROM DEFINES

    `define TRANS_RSQRT_ADDR_RANGE 23:16
    `define TRANS_RSQRT_ADDR_SIZE (23-16+1) //$bits(logic[`TRANS_RSQRT_ADDR_RANGE])
    `define TRANS_RSQRT_C2_RANGE 23:15
    `define TRANS_RSQRT_C1_RANGE 14:0
    `define TRANS_RSQRT_C0_RANGE 24:0

    //LOG ROM DEFINES

    `define TRANS_LOG_ADDR_RANGE 22:17
    `define TRANS_LOG_ADDR_SIZE (22-17+1) //$bits(logic[`TRANS_LOG_ADDR_RANGE])
    `define TRANS_LOG_C2_RANGE 25:16
    `define TRANS_LOG_C1_RANGE 15:0
    `define TRANS_LOG_C0_RANGE 25:0

    //EXP ROM DEFINES

    `define TRANS_EXP_ADDR_RANGE 23:18
    `define TRANS_EXP_ADDR_SIZE (23-18+1) //$bits(logic[`TRANS_EXP_ADDR_RANGE])
    `define TRANS_EXP_C2_RANGE 28:18
    `define TRANS_EXP_C1_RANGE 17:0
    `define TRANS_EXP_C0_RANGE 25:0

    //SIN ROM DEFINES

    `define TRANS_SIN_ADDR_RANGE 5:0
    `define TRANS_SIN_ADDR_SIZE 6 //$bits(logic[`TRANS_SIN_ADDR_RANGE])
    `define TRANS_SIN_C2_RANGE 34:20
    `define TRANS_SIN_C1_RANGE 19:0
    `define TRANS_SIN_C0_RANGE 26:0


    //ROM INTERNALS

    `define TRANS_ROM_SET_INDEX(range, in) { {(`TRANS_ROM_MAX_INDEX_SIZE - $bits(in[range])){ 1'b0 }}, in[range] }
    `define TRANS_ROM_GET_INDEX(size, in) in[size-1:0]

    `define TRANS_RCTRL(st, en) {st, en}
    `define TRANS_RCTRL_NULL `TRANS_RCTRL(TRANS_NONE_ID, `N)

`endif
