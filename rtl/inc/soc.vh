// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`ifdef ET_SIMULATION

   `include "timescale.vh"

   `ifdef ET_USE_DELAY_MODE_ZERO

      `ifndef MVLS_DELAY_MODE_ZERO
         `define MVLS_DELAY_MODE_ZERO
      `endif // ifndef MVLS_DELAY_MODE_ZERO

      `delay_mode_zero

   `endif // ifdef ET_USE_DELAY_MODE_ZERO

   `ifndef __ET_TB_DEFINES_VH__
      `include "dv/inc/et_tb_defines.vh"
   `endif // __ET_TB_DEFINES_VH__

`else // ET_SIMULATION

   `ifdef USE_ET_PARAMS_IN_BUILD

      `include "timescale.vh"

      `ifndef __ET_TB_DEFINES_VH__
         `include "dv/inc/et_tb_defines.vh"
      `endif // __ET_TB_DEFINES_VH__

   `endif // ifdef USE_ET_PARAMS_IN_BUILD

`endif // !ifdef ET_SIMULATION

`ifndef __SOC__
   `define __SOC__

   `ifndef ET_TB_HOLD
      `define ET_TB_HOLD
   `endif // ifndef ET_TB_HOLD

   `ifndef evl_wait_for_sim_complete
      `define evl_wait_for_sim_complete 
   `endif // ifndef evl_wait_for_sim_complete

   `ifndef et_test_plusargs
      `ifdef ZEBU
         `define et_test_plusargs(PLUS_ARG) 0
         `define et_test_plusargs_def "0"
      `else // ZEBU
         `ifdef ET_SIMULATION
            `define et_test_plusargs(PLUS_ARG) $test$plusargs(PLUS_ARG)
            `define et_test_plusargs_def "$test$plusargs(PLUS_ARG)"
         `else // ifdef ET_SIMULATION
            `ifdef SYNOPSYS_SMS
               `define et_test_plusargs(PLUS_ARG) $test$plusargs(PLUS_ARG)
               `define et_test_plusargs_def "$test$plusargs(PLUS_ARG)"
            `else // ifdef SYNOPSYS_SMS
               `ifdef UNIT_TEST
                  `define et_test_plusargs(PLUS_ARG) $test$plusargs(PLUS_ARG)
                  `define et_test_plusargs_def "$test$plusargs(PLUS_ARG)"
               `else // UNIT_TEST
                  `define et_test_plusargs(PLUS_ARG) 0
                  `define et_test_plusargs_def "0"
               `endif
            `endif  // !SYNOPSYS_SMS
         `endif // !ifdef ET_SIMULATION
      `endif // !ZEBU
   `endif // ifndef et_test_plusargs

   `ifndef et_value_plusargs
      `ifdef ZEBU
         `define et_value_plusargs(PLUS_ARG, VALUE) 0
         `define et_value_plusargs_def "0"
      `else // ZEBU
         `ifdef ET_SIMULATION
            `define et_value_plusargs(PLUS_ARG, VALUE) $value$plusargs(PLUS_ARG, VALUE)
            `define et_value_plusargs_def "$value$plusargs(PLUS_ARG, VALUE)"
         `else // ifdef ET_SIMULATION
            `ifdef SYNOPSYS_SMS
               `define et_value_plusargs(PLUS_ARG, VALUE) $value$plusargs(PLUS_ARG, VALUE)
               `define et_value_plusargs_def "$value$plusargs(PLUS_ARG, VALUE)"
            `else // ifdef SYNOPSYS_SMS
               `ifdef UNIT_TEST
                  `define et_value_plusargs(PLUS_ARG, VALUE) $value$plusargs(PLUS_ARG, VALUE)
                  `define et_value_plusargs_def "$value$plusargs(PLUS_ARG, VALUE)"
               `else // UNIT_TEST
                  `define et_value_plusargs(PLUS_ARG, VALUE) 0
                  `define et_value_plusargs_def "0"
               `endif
            `endif  // !SYNOPSYS_SMS
         `endif // !ifdef ET_SIMULATION
      `endif // !ZEBU
   `endif // ifndef et_value_plusargs

   `include "soc_defines.vh"
   `include "soc_types.vh"

`endif // __SOC__
