// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_trans (
  // System signals
  input  logic                                               clock_aon,
  input  logic                                               reset,
  // Control port from core to start new Trans
  input  logic                                               id_valid,
  input  logic                                               id_core_thread_id,
  input  [`INST_RANGE]                                       id_instruction,
  input  logic                                               ex_valid,
  input  logic                                               ex_core_thread_id,
  input  [`INST_RANGE]                                       ex_instruction,
  input  [`VPU_LANE_NUM-1:0]                                 ex_mask_rf0,
  // Chicken bit
  input  logic                                               chicken_bit_vputrans,
  // Valid quals
  input  logic                                               f6_killed,
  input  logic                                               f7_killed,
  input  logic                                               f8_killed,
  // Output instruction
  output logic                                               id_insert_en_next,   // Intercepts the slot next cycle
  output logic [`INST_RANGE]                                 id_insert_inst_next, // Inserted instructioun
  output logic                                               id_insert_en,        // Intercepts the slot current cycle
  output logic                                               id_trans_thread_id,
  output logic                                               ex_trans_thread_id,
  output logic                                               f6_trans_ctrl_en,
  output trans_scoreboard_slot [`TRANS_SCOREBOARD_SLOTS-1:0] id_trans_scoreboard,
  output logic [`VPU_LANE_NUM-1:0]                           f6_trans_wmask,
  output logic                                               f6_trans_thread_id,
  output logic                                               f8_trans_ctrl_en,
  output logic                                               f8_trans_fcsr_en,
  output logic                                               id_trans_busy,       // Will insert in the next cycle
  // debug
  output trans_state                                         state
);

  ////////////////////////////////////////////////////////////////////////////////
  // clock gate the whole unit while it is not working
  ////////////////////////////////////////////////////////////////////////////////
  logic clock;
  logic enable_slot;

  et_clk_gate cgate_tl (
    .enclk(clock),
    .en   (enable_slot | chicken_bit_vputrans | reset),
    .clk  (clock_aon),
    .te   (1'b0)                                       // DFT has to connect test signals here
  );

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // Trans Unit Control                                                         //
  // This unit manages the injection of the necessary instructions required for //
  // the different transcendent operations.                                     //
  // Instructions slot_currently implemented:                                   //
  //   -  frcp.ps                                                               //
  //   -  frsqrt.ps                                                             //
  //   -  flog.ps                                                               //
  //   -  fexp.ps                                                               //
  //   -  fsin.ps                                                               //
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////



  trans_state [`TRANS_PIPE_STAGES-1:0] states;
  trans_state [`TRANS_PIPE_STAGES-1:0] push_states;
  trans_state                          id_state;
  trans_state                          ex_state;
  trans_state                          push_ex_state;
  trans_state                          f5_state;
  trans_state                          f6_state;

  assign state = id_state;

  logic [$clog2(`TRANS_PIPE_STAGES)-1:0] slot_current, slot_next, slot_next_next, slot_previous;

  assign enable_slot = push_ex_state.id != TRANS_NULL
                    || states[0].id != TRANS_NULL
                    || states[1].id != TRANS_NULL
                    || states[2].id != TRANS_NULL
                    || states[3].id != TRANS_NULL
                    || states[4].id != TRANS_NULL
                    || states[5].id != TRANS_NULL
                    || states[6].id != TRANS_NULL;

  always_comb begin
    slot_next      = slot_current == (`TRANS_PIPE_STAGES - 1) ? 0 : slot_current + 1;
    slot_next_next = slot_current >= (`TRANS_PIPE_STAGES - 2) ? slot_current - (`TRANS_PIPE_STAGES - 2) : slot_current + 2;
    slot_previous  = slot_current == 0 ? `TRANS_PIPE_STAGES - 1 : slot_current - 1;

    id_state = states[slot_current];
    // ex_state = states[slot_previous];
    f5_state = states[slot_next_next];

    push_states = states;

    // push_states[slot_current] = push_state;
    push_states[slot_previous] = push_ex_state;
  end

  `RST_EN_FF(clock, reset, state_update_en(id_state, ex_state), ex_state, id_state, '0)
  `RST_EN_FF(clock, reset, state_update_en(f5_state, f6_state), f6_state, f5_state, '0)

  function automatic logic state_update_en;
    input trans_state p_state;
    input trans_state c_state;
    begin
      state_update_en = (p_state.id != TRANS_NULL | c_state.id != TRANS_NULL) && (p_state != c_state);
    end
  endfunction

  `RST_EN_FF(clock, reset, enable_slot, slot_current, slot_next, '0)

  `RST_EN_FF(clock, reset, state_update_en(push_states[0], states[0]), states[0], push_states[0], '0)
  `RST_EN_FF(clock, reset, state_update_en(push_states[1], states[1]), states[1], push_states[1], '0)
  `RST_EN_FF(clock, reset, state_update_en(push_states[2], states[2]), states[2], push_states[2], '0)
  `RST_EN_FF(clock, reset, state_update_en(push_states[3], states[3]), states[3], push_states[3], '0)
  `RST_EN_FF(clock, reset, state_update_en(push_states[4], states[4]), states[4], push_states[4], '0)
  `RST_EN_FF(clock, reset, state_update_en(push_states[5], states[5]), states[5], push_states[5], '0)
  `RST_EN_FF(clock, reset, state_update_en(push_states[6], states[6]), states[6], push_states[6], '0)

  logic next_id_trans_thread_id;

  `RST_FF   (clock, reset, id_insert_en,      id_insert_en_next,       '0)
  `RST_EN_FF(clock, reset, id_insert_en_next, id_trans_thread_id,      next_id_trans_thread_id, '0)


  // trans_write_ctrl push_write_ctrl;
  trans_write_ctrl f5_write_ctrl;
  trans_write_ctrl f6_write_ctrl;
  trans_write_ctrl f7_write_ctrl;
  trans_write_ctrl f8_write_ctrl;


  // Detection of next cycle writes

  function automatic logic is_used;
    input trans_state s;
    begin
      case (s.id)
        TRANS_NULL:        is_used = `N;
        TRANS_DIRTY_STATE: is_used = `N;
        default:           is_used = `Y;
      endcase
    end
  endfunction

  // STATE MANAGEMENT
  //
  always_comb begin
    id_trans_busy = 0;

    for (int i = 0; i < `TRANS_PIPE_STAGES; ++i)
      id_trans_busy |= is_used(states[i]);


    // NEW TRANS DETECTION
    if (ex_state == `TRANS_NULL_STATE | ex_state.id == TRANS_DIRTY_STATE || f8_killed) begin
      if (ex_valid) begin
        casex (ex_instruction)
          `FRCP_PS:       push_ex_state = `TRANS_CISTATE(TRANS_RCP_FMA1_STATE, ex_core_thread_id, ex_instruction, ex_mask_rf0);
`ifdef ENABLE_EXTRA_TRANS
          `FRSQ_PS:       push_ex_state = `TRANS_CISTATE(TRANS_RSQRT_FMA1_STATE, ex_core_thread_id, ex_instruction, ex_mask_rf0);
`endif
          `FLOG_PS:       push_ex_state = `TRANS_CISTATE(TRANS_LOG_FMA1_STATE, ex_core_thread_id, ex_instruction, ex_mask_rf0);
`ifdef ENABLE_EXTRA_TRANS
          `FSIN_PS:       push_ex_state = `TRANS_CISTATE(TRANS_SIN_TR_STATE, ex_core_thread_id, ex_instruction, ex_mask_rf0);
`endif
          `FEXP_PS:       push_ex_state = `TRANS_CISTATE(TRANS_EXP_RR_STATE, ex_core_thread_id, ex_instruction, ex_mask_rf0);
          `FRCP_FIX_RAST: push_ex_state = {TRANS_NR_STATE, ex_core_thread_id, ex_instruction[`VPU_INST_REN2_RA_SEL], ex_instruction[`VPU_INST_RD_SEL], `N, ex_mask_rf0};
          default:        push_ex_state = `TRANS_NULL_STATE;
        endcase
        id_trans_busy |= push_ex_state != `TRANS_NULL_STATE;
      end else begin
        push_ex_state = `TRANS_NULL_STATE;
      end
    end else begin
    // STATE TRANSITION
      case (ex_state.id)
        // RCP
        TRANS_RCP_FMA1_STATE:   push_ex_state = `TRANS_CSTATE(TRANS_RCP_FMA2_STATE, ex_state);
        TRANS_RCP_FMA2_STATE:   push_ex_state = `TRANS_CSTATE(TRANS_DIRTY_STATE, ex_state);

`ifdef ENABLE_EXTRA_TRANS
        // RSQRT
        TRANS_RSQRT_FMA1_STATE: push_ex_state = `TRANS_CSTATE(TRANS_RSQRT_FMA2_STATE, ex_state);
        TRANS_RSQRT_FMA2_STATE: push_ex_state = `TRANS_CSTATE(TRANS_DIRTY_STATE, ex_state);
`endif

        // LOG
        TRANS_LOG_FMA1_STATE:   push_ex_state = `TRANS_CSTATE(TRANS_LOG_FMA2_STATE, ex_state);
        TRANS_LOG_FMA2_STATE:   push_ex_state = `TRANS_CSTATE(TRANS_LOG_MUL_STATE, ex_state);
        TRANS_LOG_MUL_STATE:    push_ex_state = `TRANS_CSTATE(TRANS_DIRTY_STATE, ex_state);

`ifdef ENABLE_EXTRA_TRANS
        // SIN
        TRANS_SIN_TR_STATE:     push_ex_state = `TRANS_CSTATE(TRANS_SIN_RR_STATE, ex_state);
        TRANS_SIN_RR_STATE:     push_ex_state = `TRANS_CSTATE(TRANS_SIN_P1_STATE, ex_state);
        TRANS_SIN_P1_STATE:     push_ex_state = `TRANS_CSTATE(TRANS_SIN_P2_STATE, ex_state);
        TRANS_SIN_P2_STATE:     push_ex_state = `TRANS_CSTATE(TRANS_SIN_P3_STATE, ex_state);
        TRANS_SIN_P3_STATE:     push_ex_state = `TRANS_CSTATE(TRANS_DIRTY_STATE, ex_state);
`endif

        // EXP
        TRANS_EXP_RR_STATE:     push_ex_state = `TRANS_CSTATE(TRANS_EXP_FMA1_STATE, ex_state);
        TRANS_EXP_FMA1_STATE:   push_ex_state = `TRANS_CSTATE(TRANS_EXP_FMA2_STATE, ex_state);
        TRANS_EXP_FMA2_STATE:   push_ex_state = `TRANS_CSTATE(TRANS_DIRTY_STATE, ex_state);

        // NR
        TRANS_NR_STATE:         push_ex_state = `TRANS_CSTATE(TRANS_DIRTY_STATE, ex_state);
        default:                push_ex_state = `TRANS_NULL_STATE;
      endcase
    end
  end

  // INSTRUCTION INSERTION MANAGEMENT
  always_comb begin
    if (f6_state.id != TRANS_NULL && f6_state.id != TRANS_DIRTY_STATE && !f6_killed) begin
      id_insert_en_next       = `Y;
      next_id_trans_thread_id = f6_state.thread_id;

      case (f6_state.id) //                                               Instruction         Destination S1          S2

        // RCP
        TRANS_RCP_FMA1_STATE:   id_insert_inst_next = `TRANS_INSTR(`TR_FMA1_RCP,   f6_state.rd, 5'b0,      f6_state.rs);
        TRANS_RCP_FMA2_STATE:   id_insert_inst_next = `TRANS_INSTR(`TR_FMA2_RCP,   f6_state.rd, 5'b0,      f6_state.rs);

`ifdef ENABLE_EXTRA_TRANS
        // RSQRT
        TRANS_RSQRT_FMA1_STATE: id_insert_inst_next = `TRANS_INSTR(`TR_FMA1_RSQRT, f6_state.rd, 5'b0,      f6_state.rs);
        TRANS_RSQRT_FMA2_STATE: id_insert_inst_next = `TRANS_INSTR(`TR_FMA2_RSQRT, f6_state.rd, 5'b0,      f6_state.rs);
`endif

        // LOG
        TRANS_LOG_FMA1_STATE:   id_insert_inst_next = `TRANS_INSTR(`TR_FMA1_LOG,   f6_state.rd, 5'b0,      f6_state.rs);
        TRANS_LOG_FMA2_STATE:   id_insert_inst_next = `TRANS_INSTR(`TR_FMA2_LOG,   f6_state.rd, 5'b0,      f6_state.rs);
        TRANS_LOG_MUL_STATE:    id_insert_inst_next = `TRANS_INSTR(`TR_MUL_LOG,    f6_state.rd, 5'b0,      f6_state.rs);

`ifdef ENABLE_EXTRA_TRANS
        // SIN
        TRANS_SIN_TR_STATE:     id_insert_inst_next = `TRANS_INSTR(`TR_TR_SIN,     f6_state.rd, 5'b0,      5'b0);
        TRANS_SIN_RR_STATE:     id_insert_inst_next = `TRANS_INSTR(`TR_RR_SIN,     f6_state.rd, 5'b0,      f6_state.rd);
        TRANS_SIN_P1_STATE:     id_insert_inst_next = `TRANS_INSTR(`TR_P1_SIN,     f6_state.rd, f6_state.rd, f6_state.rd);
        TRANS_SIN_P2_STATE:     id_insert_inst_next = `TRANS_INSTR(`TR_P2_SIN,     f6_state.rd, 5'b0,      f6_state.rd);
        TRANS_SIN_P3_STATE:     id_insert_inst_next = `TRANS_INSTR(`TR_P3_SIN,     f6_state.rd, 5'b0,      f6_state.rd);
`endif

        // EXP
        TRANS_EXP_RR_STATE:     id_insert_inst_next = `TRANS_INSTR(`TR_RR_EXP,     f6_state.rd, 5'b0,      f6_state.rd);
        TRANS_EXP_FMA1_STATE:   id_insert_inst_next = `TRANS_INSTR(`TR_FMA1_EXP,   f6_state.rd, 5'b0,      f6_state.rd);
        TRANS_EXP_FMA2_STATE:   id_insert_inst_next = `TRANS_INSTR(`TR_FMA2_EXP,   f6_state.rd, 5'b0,      f6_state.rd);

        // NR
        TRANS_NR_STATE:         id_insert_inst_next = `TRANS_INSTR(`TR_NR,         f6_state.rd, 5'b0,      f6_state.rs);
        default:                id_insert_inst_next = `TRANS_NOP;
      endcase
    end else begin
      next_id_trans_thread_id = 1'b0;
      id_insert_en_next       = `N;

      id_insert_inst_next     = `TRANS_NOP;
    end

    ex_trans_thread_id = ex_state.thread_id;
  end

  // WRITE MANAGEMENT
  always_comb begin
    case (f5_state.id) //                              EN  MASK
      TRANS_EXP_RR_STATE: f5_write_ctrl = {`Y, f5_state.mask, f5_state.thread_id, `N};
`ifdef ENABLE_EXTRA_TRANS
      TRANS_SIN_RR_STATE: f5_write_ctrl = {`Y, f5_state.mask, f5_state.thread_id, `N};
`endif
      TRANS_DIRTY_STATE:  f5_write_ctrl = {`Y, f5_state.mask, f5_state.thread_id, |f5_state.mask};
      TRANS_NULL:         f5_write_ctrl = {`N, {(`VPU_LANE_NUM) {1'b0}}, 1'b0};
      default:            f5_write_ctrl = {`Y, `VPU_LANE_NUM'b0, f5_state.thread_id, `N};
    endcase

    case (id_state.id) //                         EN  MASK
      TRANS_EXP_RR_STATE: f7_write_ctrl = {`Y, id_state.mask, id_state.thread_id, `N};
`ifdef ENABLE_EXTRA_TRANS
      TRANS_SIN_RR_STATE: f7_write_ctrl = {`Y, id_state.mask, id_state.thread_id, `N};
`endif
      TRANS_DIRTY_STATE:  f7_write_ctrl = {`Y, id_state.mask, id_state.thread_id, |id_state.mask};
      TRANS_NULL:         f7_write_ctrl = {`N, {(`VPU_LANE_NUM) {1'b0}}, 1'b0};
      default:            f7_write_ctrl = {`Y, `VPU_LANE_NUM'b0, id_state.thread_id, `N};
    endcase
  end

  `RST_EN_FF( clock, reset, f5_write_ctrl.en | f6_write_ctrl.en, f6_write_ctrl, f5_write_ctrl, '0)
  `RST_EN_FF( clock, reset, f7_write_ctrl.en | f8_write_ctrl.en, f8_write_ctrl, f7_write_ctrl, '0)

    // MASK MANAGEMENT

    always_comb begin
        f6_trans_ctrl_en = 1'b0;
        f6_trans_wmask = '0;
        f6_trans_thread_id = '0;
        f8_trans_ctrl_en = 1'b0;
        f8_trans_fcsr_en = 1'b0;

        if( f6_write_ctrl.en && !f6_killed ) begin
            f6_trans_ctrl_en   = 1'b1;
            f6_trans_wmask     = f6_write_ctrl.mask;
            f6_trans_thread_id = f6_write_ctrl.thread_id;
        end

        if( f8_write_ctrl.en && !f8_killed ) begin
            f8_trans_ctrl_en   = 1'b1;
            f8_trans_fcsr_en   = f8_write_ctrl.fcsr_en;
        end
    end

    // SCOREBOARD

    function automatic trans_scoreboard_slot slot;
        input trans_state s;
        begin 
            case(s.id)
                TRANS_RCP_FMA1_STATE:   slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                TRANS_RCP_FMA2_STATE:   slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
`ifdef ENABLE_EXTRA_TRANS
                TRANS_RSQRT_FMA1_STATE: slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                TRANS_RSQRT_FMA2_STATE: slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
`endif
                TRANS_LOG_FMA1_STATE:   slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                TRANS_LOG_FMA2_STATE:   slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                TRANS_LOG_MUL_STATE:    slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                TRANS_NR_STATE:         slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                TRANS_EXP_RR_STATE:     slot = {`N, {$bits(minion_reg_dest){1'h0}},     `Y, 1'b1, s.rd, s.thread_id};
                TRANS_EXP_FMA1_STATE:   slot = {`N, {$bits(minion_reg_dest){1'h0}},     `Y, 1'b1, s.rd, s.thread_id};
                TRANS_EXP_FMA2_STATE:   slot = {`N, {$bits(minion_reg_dest){1'h0}},     `Y, 1'b1, s.rd, s.thread_id};
`ifdef ENABLE_EXTRA_TRANS
                TRANS_SIN_TR_STATE:     slot = {`N, {$bits(minion_reg_dest){1'h0}},     `Y, 1'b1, s.rd, s.thread_id};
                TRANS_SIN_RR_STATE:     slot = {`N, {$bits(minion_reg_dest){1'h0}},     `Y, 1'b1, s.rd, s.thread_id};
                TRANS_SIN_P1_STATE:     slot = {`N, {$bits(minion_reg_dest){1'h0}},     `Y, 1'b1, s.rd, s.thread_id};
                TRANS_SIN_P2_STATE:     slot = {`N, {$bits(minion_reg_dest){1'h0}},     `Y, 1'b1, s.rd, s.thread_id};
                TRANS_SIN_P3_STATE:     slot = {`N, {$bits(minion_reg_dest){1'h0}},     `Y, 1'b1, s.rd, s.thread_id};
`endif
                TRANS_DIRTY_STATE:      slot = {`N, {$bits(minion_reg_dest){1'h0}},     `Y, 1'b1, s.rd, s.thread_id};
                TRANS_NULL:             slot = {`N, {$bits(minion_reg_dest){1'h0}},     `N, {$bits(minion_reg_dest){1'h0}} };
                default: slot = 'b0;
            endcase
        end
    endfunction


    function automatic trans_scoreboard_slot ex_slot;
        input trans_state s;
        input logic                         valid;
        input logic                         thread_id;
        input [`INST_RANGE]                 instruction;
        input logic                         killed;
        begin 
            if(s == `TRANS_NULL_STATE | s.id==TRANS_DIRTY_STATE | killed) begin
                if(valid) begin
                    casex(instruction)
                        `FRCP_PS:  ex_slot = {`Y, 1'b1, instruction[`VPU_INST_REN1_RA_SEL], thread_id,    `Y, 1'b1, instruction[`VPU_INST_RD_SEL], thread_id};
`ifdef ENABLE_EXTRA_TRANS
                        `FRSQ_PS:  ex_slot = {`Y, 1'b1, instruction[`VPU_INST_REN1_RA_SEL], thread_id,    `Y, 1'b1, instruction[`VPU_INST_RD_SEL], thread_id};
`endif
                        `FLOG_PS:  ex_slot = {`Y, 1'b1, instruction[`VPU_INST_REN1_RA_SEL], thread_id,    `Y, 1'b1, instruction[`VPU_INST_RD_SEL], thread_id};
`ifdef ENABLE_EXTRA_TRANS
                        `FSIN_PS:  ex_slot = {`Y, 1'b1, instruction[`VPU_INST_REN1_RA_SEL], thread_id,    `Y, 1'b1, instruction[`VPU_INST_RD_SEL], thread_id};
`endif
                        `FEXP_PS:  ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, instruction[`VPU_INST_RD_SEL], thread_id};
                        `FRCP_FIX_RAST: ex_slot = {`Y, 1'b1, instruction[`VPU_INST_REN1_RA_SEL], thread_id,    `Y, 1'b1, instruction[`VPU_INST_RD_SEL], thread_id};
                        default:   ex_slot = 'b0;
                    endcase
                end
                else
                    ex_slot = 'b0;
            end else begin
                case(s.id)
                    TRANS_RCP_FMA1_STATE:   ex_slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_RCP_FMA2_STATE:   ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
`ifdef ENABLE_EXTRA_TRANS
                    TRANS_RSQRT_FMA1_STATE: ex_slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_RSQRT_FMA2_STATE: ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
`endif
                    TRANS_LOG_FMA1_STATE:   ex_slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_LOG_FMA2_STATE:   ex_slot = {`Y, 1'b1, s.rs, s.thread_id,    `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_LOG_MUL_STATE:    ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_NR_STATE:         ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_EXP_RR_STATE:     ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_EXP_FMA1_STATE:   ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_EXP_FMA2_STATE:   ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
`ifdef ENABLE_EXTRA_TRANS
                    TRANS_SIN_TR_STATE:     ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_SIN_RR_STATE:     ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_SIN_P1_STATE:     ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_SIN_P2_STATE:     ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
                    TRANS_SIN_P3_STATE:     ex_slot = {`N, {$bits(minion_reg_dest){1'b0}},     `Y, 1'b1, s.rd, s.thread_id};
`endif
                    default: ex_slot = 'b0;
                endcase
            end
        end
    endfunction

    trans_scoreboard_slot [`TRANS_SCOREBOARD_SLOTS-1:0] trans_scoreboard_next;

    //Note: this one is using the clock_aon, not clock (clock is gated with enable_slot => this signal needs to be updated with id_valid as well)
    `RST_EN_FF(clock_aon, reset, id_valid || enable_slot, id_trans_scoreboard, trans_scoreboard_next, '0)

    always_comb begin
        trans_scoreboard_next[0] = slot_next == 0 ? ex_slot(push_states[6], id_valid, id_core_thread_id, id_instruction, f7_killed)
            : slot_next == 1 ? ex_slot(push_states[0], id_valid, id_core_thread_id, id_instruction, f7_killed)
            : slot(push_states[0]);
        trans_scoreboard_next[1] = slot_next == 1 ? slot(push_states[6])
            : slot_next == 2 ? ex_slot(push_states[1], id_valid, id_core_thread_id, id_instruction, f7_killed)
            : slot(push_states[1]);
        trans_scoreboard_next[2] = slot_next == 2 ? slot(push_states[6])
            : slot_next == 3 ? ex_slot(push_states[2], id_valid, id_core_thread_id, id_instruction, f7_killed)
            : slot(push_states[2]);
        trans_scoreboard_next[3] = slot_next == 3 ? slot(push_states[6])
            : slot_next == 4 ? ex_slot(push_states[3], id_valid, id_core_thread_id, id_instruction, f7_killed)
            : slot(push_states[3]);
        trans_scoreboard_next[4] = slot_next == 4 ? slot(push_states[6])
            : slot_next == 5 ? ex_slot(push_states[4], id_valid, id_core_thread_id, id_instruction, f7_killed)
            : slot(push_states[4]);
        trans_scoreboard_next[5] = slot_next == 5 ? slot(push_states[6])
            : slot_next == 6 ? ex_slot(push_states[5], id_valid, id_core_thread_id, id_instruction, f7_killed)
            : slot(push_states[5]);
    end

endmodule


