// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
module neigh_ch_dbg (
  // System signals
  input logic                                                  clock,

  input logic                                                  reset_w,
  input logic                                                  reset_c,
  input logic                                                  reset_d,
  output logic [`MIN_PER_N-1:0]                                reset_w_per_minion,
  output logic                                                 reset_w_neigh, 

  // Run control for minions and tbox
  output minion_debug_in_t [`MIN_PER_N-1:0]                    debug_minion_req,
  input minion_debug_out_t [`MIN_PER_N-1:0]                    debug_minion_status, 

  // Debug ESRs
  input esr_ms_dmctrl_t                                        dmctrl, 
  output esr_hastatus0_t                                       esr_hastatus0,
  output esr_hastatus1_t                                       esr_hastatus1_min,
  input esr_hactrl_t                                           esr_hactrl,
  output esr_and_or_tree_L0_t                                  esr_and_or_tree_L0, 
    

  // Status Monitor
  input logic                                                  status_monitor_enable, 
  input logic [`neigh_sm_gpio_width - 1:0]                     neigh_sm_gpio,
  output logic [`MIN_PER_N-1:0]                                minion_dbg_signals_en,
  output logic [`MIN_PER_N-1:0][`NEIGH_DEBUG_SM_MUX_WIDTH-1:0] minion_dbg_signals_mux,
  input neigh_sm_dbg_monitor_t [`MIN_PER_N-1:0]                minion_dbg_signals,
  output logic                                                 tbox_dbg_signals_en, 
  output logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]                 tbox_dbg_signals_mux,
  input neigh_sm_dbg_monitor_t                                 tbox_dbg_signals,
  output logic                                                 neigh_ch_dbg_signals_en,
  output logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]                 neigh_ch_dbg_signals_mux,
  input neigh_sm_dbg_monitor_t                                 neigh_ch_dbg_signals,
  output neigh_sm_dbg_monitor_t                                neigh_sm_signals
);

// AndOrTreeL0
logic [`THREADS_PER_N-1:0] sel_harts;
assign sel_harts = ({`THREADS_PER_N{dmctrl.hasel}} & esr_hactrl.hawindow) | esr_hactrl.hartmask ;
esr_and_or_tree_L0_t esr_and_or_tree_L0_next; 
//         CLK    RST           DOUT                 DIN                    DEF
`RST_FF(clock, reset_d,  esr_and_or_tree_L0, esr_and_or_tree_L0_next, '0)


always_comb begin
  esr_and_or_tree_L0_next.anyhalted      = |(esr_hastatus0.halted & sel_harts); //any halted
  esr_and_or_tree_L0_next.allhalted      = ((esr_hastatus0.halted & sel_harts) == sel_harts) && |sel_harts; //all halted
  esr_and_or_tree_L0_next.anyrunning     = |(esr_hastatus0.running & sel_harts); //any running
  esr_and_or_tree_L0_next.allrunning     = ((esr_hastatus0.running & sel_harts) == sel_harts) && |sel_harts; //all running
  esr_and_or_tree_L0_next.anyresumeack   = |(esr_hastatus0.resumeack & sel_harts); //any resumeack
  esr_and_or_tree_L0_next.allresumeack   = ((esr_hastatus0.resumeack & sel_harts) == sel_harts) && |sel_harts; //all resumeack
  esr_and_or_tree_L0_next.anyhavereset   = |(esr_hastatus0.havereset & sel_harts); //any have reset
  esr_and_or_tree_L0_next.allhavereset   = ((esr_hastatus0.havereset & sel_harts) == sel_harts) && |sel_harts; //all have reset
  esr_and_or_tree_L0_next.anyunavailable = |((~(esr_hastatus0.halted | esr_hastatus0.running)) & sel_harts); //any unavailable
  esr_and_or_tree_L0_next.anyselected    = |sel_harts; //any selected
end

/***************************************************
************ RUN-CONTROL FOR MINIONS ***************
****************************************************/

minion_debug_in_t [`MIN_PER_N-1:0] debug_minion_req_next;

//      CLK    RST      DOUT              DIN             DEF
`RST_FF(clock, reset_d, debug_minion_req, debug_minion_req_next, '0)


// Resume FSM implementation
for(genvar min_it = 0; min_it < `MIN_PER_N; min_it++) begin : MINION_DBG_REG
  minion_debug_out_t                        debug_out_struct;
  logic [`CORE_NR_THREADS-1:0]              resume_req_level;
  logic [`CORE_NR_THREADS-1:0]              resume_performed;
  resume_state_type [`CORE_NR_THREADS-1:0]  resume_state, resume_state_next;   
  logic  [`CORE_NR_THREADS-1:0]             resume_state_en;
  logic [`CORE_NR_THREADS-1:0]              resumeack, resumeack_reg, resumeack_next, resumeack_clear;
  logic [`CORE_NR_THREADS-1:0]              resumeack_en;
  logic [`CORE_NR_THREADS-1:0]              resume_to_core;
    
  //         CLK    RST      EN                                                         DOUT                       DIN                          DEF
  `RST_EN_FF(clock, reset_d, resume_state_en[0],                                        resume_state[0],           resume_state_next[0],        resume_IDLE)
  `RST_EN_FF(clock, reset_d, resume_state_en[1],                                        resume_state[1],           resume_state_next[1],        resume_IDLE)
  `RST_EN_FF(clock, reset_d, resumeack_en[0],                                           resumeack_reg[0],              resumeack_next[0],        '1)
  `RST_EN_FF(clock, reset_d, resumeack_en[1],                                           resumeack_reg[1],              resumeack_next[1],        '1)

  assign resumeack = resumeack_reg & ~resumeack_clear;
  
    // State machine for resume request to the cores
    always_comb begin
      for (int i = 0; i < `CORE_NR_THREADS; ++i) begin
        // Resets
        resume_state_en[i] = 1'b0;
        resume_state_next[i] = resume_state[i];
        resumeack_en[i] = 1'b0;
        resumeack_next[i] = resumeack[i];
        resumeack_clear[i] = 1'b0;
        // Logic
        case(resume_state[i]) 
          resume_IDLE: begin
            if (resume_req_level[i] ) begin
              // Start resume
              resume_state_next[i] = resume_RESUMING;
              resume_state_en[i] = 1'b1;
              // Clear resumeack
              resumeack_en[i] = 1'b1;
              resumeack_next[i] = 1'b0;
              resumeack_clear[i] = 1'b1;
            end
          end
          resume_RESUMING: begin
            if ( esr_hastatus0.running[`CORE_NR_THREADS*min_it + i] ) begin
                resume_state_next[i] = resume_WAIT;
                resume_state_en[i] = 1'b1;
            end
          end
          resume_WAIT: begin
            if (!resume_req_level[i]) begin
              resume_state_next[i] = resume_IDLE;
              resume_state_en[i] = 1'b1;
            end
             // Capture resumeack
             resumeack_next[i] = resume_performed[i];
             resumeack_en[i] = 1'b1;             
           end
           default: begin
             resume_state_en[i] = 1'b0;
             resume_state_next[i] = resume_state[i];
             resumeack_en[i] = 1'b0;
             resumeack_next[i] = resumeack[i];
             resumeack_clear[i] = 1'b0;
           end
        endcase // case (resume_state[i])
      end
    end // always_comb

    always_comb begin
      for(int i = 0; i < `CORE_NR_THREADS; ++i) begin
         resume_performed[i] = (resume_state[i] == resume_WAIT);
      end
    end

   always_comb begin
      debug_out_struct                    = debug_minion_status[min_it];
      resume_req_level                    = ({`CORE_NR_THREADS{dmctrl.dmactive}}  & 
                                             {`CORE_NR_THREADS{dmctrl.resumereq && dmctrl.write_enable}} & 
                                             sel_harts[`CORE_NR_THREADS*min_it+:`CORE_NR_THREADS] ); // dmctrl resume req
      resume_to_core                      = {(resume_state[1] == resume_RESUMING) | resume_req_level[1],
                                             (resume_state[0] == resume_RESUMING) | resume_req_level[0]
                                             };
      
      // DEBUG RUN CONTROL REQUEST FOR MINIONS
      debug_minion_req_next[min_it].halt          = ~debug_out_struct.halted &
                                                    (({`CORE_NR_THREADS{dmctrl.dmactive}} & 
                                                      {`CORE_NR_THREADS{dmctrl.haltreq}}  & 
                                                      sel_harts[`CORE_NR_THREADS*min_it+:`CORE_NR_THREADS] )   //dmctrl halt req
                                                     );

      debug_minion_req_next[min_it].resume       =  resume_to_core & ~resumeack;

      debug_minion_req_next[min_it].resethalt    = {`CORE_NR_THREADS{dmctrl.dmactive}} & 
                                                   esr_hactrl.resethalt[`CORE_NR_THREADS*min_it+:`CORE_NR_THREADS];

      
      debug_minion_req_next[min_it].ackhavereset = debug_out_struct.have_reset & {`CORE_NR_THREADS{dmctrl.dmactive}} &
                                                   {`CORE_NR_THREADS{dmctrl.ackhavereset && dmctrl.write_enable}} &  
                                                   sel_harts[`CORE_NR_THREADS*min_it+:`CORE_NR_THREADS];

      reset_w_per_minion[min_it]          = reset_w | 
                                            (dmctrl.dmactive && dmctrl.hartreset && 
                                             (|sel_harts[`CORE_NR_THREADS*min_it+:`CORE_NR_THREADS]));
      
      // ESR output   

      // HASTATUS0
      esr_hastatus0.halted[`CORE_NR_THREADS*min_it +: `CORE_NR_THREADS]    = debug_out_struct.halted;
      esr_hastatus0.running[`CORE_NR_THREADS*min_it +: `CORE_NR_THREADS]   = debug_out_struct.running;
      esr_hastatus0.resumeack[`CORE_NR_THREADS*min_it +: `CORE_NR_THREADS] = resumeack;
      esr_hastatus0.havereset[`CORE_NR_THREADS*min_it +: `CORE_NR_THREADS] = debug_out_struct.have_reset;

      // HASTATUS1
      esr_hastatus1_min.error[`CORE_NR_THREADS*min_it +: `CORE_NR_THREADS] = debug_out_struct.error;
      esr_hastatus1_min.exception[`CORE_NR_THREADS*min_it +: `CORE_NR_THREADS] = debug_out_struct.exception;
      esr_hastatus1_min.busy[`CORE_NR_THREADS*min_it +: `CORE_NR_THREADS] = debug_out_struct.busy;
   end
end // block: MINION_DBG_REG


assign reset_w_neigh = reset_w;
   


/******************************************
************ STATUS MONITOR ***************
*******************************************/
logic [`neigh_sm_gpio_width-1:0]        neigh_sm_gpio_ff;
logic [`NEIGH_SM_TARGET_SEL-1:0]        dbg_target_sel;
logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]   minion_north_dbg_signals_mux;
logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]   minion_north_dbg_signals_mux_next;
logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]   minion_south_dbg_signals_mux;
logic [`NEIGH_DEBUG_SM_MUX_WIDTH-1:0]   minion_south_dbg_signals_mux_next;
logic [`MIN_PER_N-1:0]                  minion_dbg_signals_en_next;
logic                                   tbox_dbg_signals_en_next;
logic                                   neigh_ch_dbg_signals_en_next;
neigh_sm_dbg_monitor_t [`MIN_PER_N-1:0] minion_dbg_signals_ff;
neigh_sm_dbg_monitor_t                  minion_north_dbg_signals;
neigh_sm_dbg_monitor_t                  minion_north_dbg_signals_ff;
neigh_sm_dbg_monitor_t                  minion_south_dbg_signals;
neigh_sm_dbg_monitor_t                  minion_south_dbg_signals_ff;
neigh_sm_dbg_monitor_t                  tbox_dbg_signals_ff;
neigh_sm_dbg_monitor_t                  neigh_ch_dbg_signals_ff;

//         CLK    RST      EN                     DOUT                          DIN                                DEF
`RST_EN_FF(clock, reset_c, status_monitor_enable, neigh_sm_gpio_ff,             neigh_sm_gpio,                     '0)
`RST_EN_FF(clock, reset_c, status_monitor_enable, minion_north_dbg_signals_mux, minion_north_dbg_signals_mux_next, '0)
`RST_EN_FF(clock, reset_c, status_monitor_enable, minion_south_dbg_signals_mux, minion_south_dbg_signals_mux_next, '0)

assign dbg_target_sel = neigh_sm_gpio_ff[`neigh_sm_gpio_width-1 -: `NEIGH_SM_TARGET_SEL];

// Output signals
assign minion_north_dbg_signals_mux_next = neigh_sm_gpio_ff[(`neigh_sm_gpio_width-`NEIGH_SM_TARGET_SEL)-1:0];
assign minion_south_dbg_signals_mux_next = neigh_sm_gpio_ff[(`neigh_sm_gpio_width-`NEIGH_SM_TARGET_SEL)-1:0];
// Signals for minions are re-flopped for north and south minions
always_comb begin
  for (integer i = 0; i < `MIN_PER_N/2; i++)
      minion_dbg_signals_mux[i] = minion_north_dbg_signals_mux;
  for (integer i = `MIN_PER_N/2; i < `MIN_PER_N; i++)
      minion_dbg_signals_mux[i] = minion_south_dbg_signals_mux;
end
assign tbox_dbg_signals_mux              = neigh_sm_gpio_ff[(`neigh_sm_gpio_width-`NEIGH_SM_TARGET_SEL)-1:0];
assign neigh_ch_dbg_signals_mux          = neigh_sm_gpio_ff[(`neigh_sm_gpio_width-`NEIGH_SM_TARGET_SEL)-1:0];

always_comb begin
  minion_dbg_signals_en_next   = '0;
  tbox_dbg_signals_en_next     = 1'b0;
  neigh_ch_dbg_signals_en_next = 1'b0;

  // Minions
  if (dbg_target_sel < `NEIGH_SM_TARGET_TBOX)
    minion_dbg_signals_en_next   = status_monitor_enable << dbg_target_sel;
  // TBOX
  else if (dbg_target_sel == `NEIGH_SM_TARGET_TBOX)
    tbox_dbg_signals_en_next     = status_monitor_enable;
  // Neighborhood channel
  else
    neigh_ch_dbg_signals_en_next = status_monitor_enable;
end

//         CLK    RST      EN                     DOUT                     DIN                           DEF
`RST_EN_FF(clock, reset_d, status_monitor_enable, minion_dbg_signals_en,   minion_dbg_signals_en_next,   '0)
`RST_EN_FF(clock, reset_d, status_monitor_enable, tbox_dbg_signals_en,     tbox_dbg_signals_en_next,     '0)
`RST_EN_FF(clock, reset_d, status_monitor_enable, neigh_ch_dbg_signals_en, neigh_ch_dbg_signals_en_next, '0)

// Input signals

//         CLK    RST      EN                     DOUT                         DIN                       DEF
`RST_EN_FF(clock, reset_d, status_monitor_enable, minion_dbg_signals_ff,       minion_dbg_signals,       '0)
`RST_EN_FF(clock, reset_d, status_monitor_enable, minion_north_dbg_signals_ff, minion_north_dbg_signals, '0)
`RST_EN_FF(clock, reset_d, status_monitor_enable, minion_south_dbg_signals_ff, minion_south_dbg_signals, '0)
`RST_EN_FF(clock, reset_d, status_monitor_enable, tbox_dbg_signals_ff,         tbox_dbg_signals,         '0)
`RST_EN_FF(clock, reset_d, status_monitor_enable, neigh_ch_dbg_signals_ff,     neigh_ch_dbg_signals,     '0)

// First multiplex and flop north and south minion signals independently
assign minion_north_dbg_signals = minion_dbg_signals_ff[{1'b0,dbg_target_sel[0+:`MIN_PER_N_L-1]}];
assign minion_south_dbg_signals = minion_dbg_signals_ff[{1'b1,dbg_target_sel[0+:`MIN_PER_N_L-1]}];

always_comb begin
  // North minions
  if (dbg_target_sel <= `NEIGH_SM_TARGET_MIN3)
      neigh_sm_signals = minion_north_dbg_signals_ff;
  // South minions
  else if (dbg_target_sel <= `NEIGH_SM_TARGET_MIN7)
      neigh_sm_signals = minion_south_dbg_signals_ff;
  // TBOX
  else if (dbg_target_sel == `NEIGH_SM_TARGET_TBOX)
      neigh_sm_signals = tbox_dbg_signals_ff;
  // Neighborhood channel
  else
      neigh_sm_signals = neigh_ch_dbg_signals_ff;
end


endmodule
