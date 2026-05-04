// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// assert_const.vh
//
// Original ideas leveraged from Cliff Cummings 2009 paper:  http://www.sunburst-design.com/papers/CummingsSNUG2009SJ_SVA_Bind.pdf
// (but with many improvements based on actual practice)
//
// Changes for ET:
//  * clean up names
//  * change reset to active high
//  * `ifdef ASSERT_ON changed to `ifndef ET_ASSERT_OFF  - don't like synopsys translate off/on becuase user may not want it on
//  * `ASSERT_ALWAYS changed to `ASSERT_ALWAYS   (look like a keyword/OVL vs. follow `define naming convention)
//
// disable all assertions with +define+ET_ASSERT_OFF
//
// `ASSERT_VALID_PORTS is an assertion that confirms that inputs or outputs
// to a module are valid (not X or Z) when an enable (myValid) is high.
// Use this to speed debugging of simulations that go unknown.
//
// user's instantiation of assertion:
// `ASSERT_VALID_PORTS(nameOfAssertion, myClk, myReset, myValid, myData)
//
// assertion usage example:
// `ASSERT_ALWAYS(ERROR_nameOfAssertion, clk, reset, (assert always expression) )
// `ASSERT_ALWAYS(ERROR_addrGtMax, clk, reset, (addr < 5) )
//
// `ASSERT_ALWAYS_MSG(ERROR_nameOfAssertion, clk, reset, (assert always expression), $sformatf("your strings with %stuff", $sampled(var) ) )
// `ASSERT_ALWAYS_MSG(ERROR_addrGtMax, clk, reset, (addr < 5), $sformatf("Addr(%x) >= 5", $sampled(addr) ) )
//
// `ASSERT_END_OF_SIM(ERROR_nameOfAssertion, (end of sim end_of_sim check expression should be true), -1 )
// `ASSERT_END_OF_SIM(ERROR_NonEmptyFifoAtEndOfSim, fifo_empty, -1 )
//
// `ASSERT_END_OF_SIM_MSG(ERROR_nameOfAssertion, (end of sim end_of_sim check expression should be true), $sformatf("your strings with %stuff", $sampled(var) ), -1 )
// `ASSERT_END_OF_SIM_MSG(ERROR_NonEmptyFifoAtEndOfSim, fifo_empty, $sformatf("Oh know fifo isn't empty!"), -1 )
//
// (Note no final semicolon on all assertions)
//
// Please name all assertions starting with "ERROR_"
//
// For assertions that do NOT require coverage collection, please add "_NOCOV_" to the
// assertion name.
// e.g. `ASSERT_ALWAYS(ERROR_NOCOV_thisNeverHappens, ... )
//
// These macros have the following special features:
// * concurrent assertions allow for assertion coverage collection and may be used by formal tools
// * disable assertion BEFORE and during reset
//
`ifndef __ASSERT_CONST_VH
`define __ASSERT_CONST_VH

`ifdef SYNTHESIS
   // use global SYNTHESIS flag to also turn off assertions
   `ifndef ET_ASSERT_OFF
      `define ET_ASSERT_OFF
   `endif
`endif

`ifndef QUIESCENT_FIFO_MASK
   `define QUIESCENT_FIFO_MASK (32'h00000002)
`endif

`ifndef ET_ASSERT_OFF
   `ifndef VERILATOR
      // intra-assignment delays
      `ifndef TD
         `define TD
      `endif

      // Can print red color bar on xterms:
      `ifdef NO_COLOR
         `define VALIDPORT_RED_5_SPACES ""
      `else
         `define VALIDPORT_RED_5_SPACES "\033[7m\033[5m\033[31m     \033[0m"
      `endif

      `ifndef assert_sva_error
         `ifdef EVL_SIMULATION
            `define assert_sva_error(NAME, MSG = "") evl_base_pkg::evl_dut_error("Assertion", $sformatf("%s %s %s (%s, line %0d)", `VALIDPORT_RED_5_SPACES, NAME, MSG, `__FILE__, `__LINE__));
         `else
             //synopsys translate_off
            `define assert_sva_error(NAME, MSG = "") $display("%t (ERROR%s) ASSERT Error: %s %s", $time, `VALIDPORT_RED_5_SPACES, NAME, MSG);
             //synopsys translate_on
         `endif
      `endif
      //synopsys translate_off
      `define assert_error(NAME, MSG = "") `assert_sva_error(NAME, MSG)
      //synopsys translate_on
   `else // ifndef VERILATOR
      `define assert_error(NAME, MSG)
   `endif // !ifndef VERILATOR
`else
   `define assert_error(NAME, MSG)
`endif

`ifdef ET_ASSERT_OFF
   // define empty calls for synthesis
   `define ASSERT_VALID_PORTS(NAME, CLK, RESET, VALID, DATA)
`else
   `ifdef VERILATOR
      `define ASSERT_VALID_PORTS(NAME, CLK, RESET, VALID, DATA)
   `else
      //synopsys translate_off
      property p_validPorts(clk, disable_me, valid, data); // uniquify_all_pragma p_validPorts
         @(posedge clk)
             disable iff (disable_me)
                 (valid === 1'b1) |-> not $isunknown(data);
      endproperty

      property p_validValid(clk, disable_me, valid);      // uniquify_all_pragma p_validValid
         @(posedge clk)
             disable iff (disable_me)
                 not $isunknown(valid);
      endproperty
      //synopsys translate_on

      // Usage:  `ASSERT_VALID_PORTS(name, clk, reset, valid, data)
      // (Note no final semicolon)
      // `assert_sva_error already has a semi-colon

      // initial #1 forever #0 wait... loop is used to avoid start of simulation non-functional changes on RESET that may enable this check before reset

      `define ASSERT_VALID_PORTS(NAME, CLK, RESET, VALID, DATA) \
         static logic EXCLUDEME_``NAME``_disable; \
         initial begin // run once at initial time \
            #1 forever begin\
               #0 wait (RESET === 1'b1); \
               EXCLUDEME_``NAME``_disable <= `TD 1'b0; \
               #0 wait (RESET !== 1'b1); \
            end \
         end \
         VALID_PORT_``NAME``_x_check: assert property (p_validPorts(CLK, ((EXCLUDEME_``NAME``_disable !== 1'b0) || (RESET !== 1'b0)), VALID, DATA)) \
             else `assert_error("ASSERT_VALID_PORTS", $sformatf("ERROR:%s%m:Claimed valid signal DATA is not valid ('h%0X) when control VALID is high", `VALIDPORT_RED_5_SPACES, $sampled(DATA))) \
         VALID_PORT_``NAME``_valid_x_check: assert property (p_validValid(CLK, ((EXCLUDEME_``NAME``_disable !== 1'b0) || (RESET !== 1'b0)), VALID)) \
             else `assert_error("ASSERT_VALID_PORTS", $sformatf("ERROR:%s%m:Claimed valid signal control VALID is not valid, it's 'h%0x", `VALIDPORT_RED_5_SPACES, $sampled(VALID)))
   `endif // !ifdef VERILATOR
`endif // !ifdef ET_ASSERT_OFF


// -----------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------
// in-line assertion accelerators.
//  Use these to write assertions quickly...

// assertion usage example:
// `ASSERT_ALWAYS(ERROR_nameOfAssertion, clk, reset, (assert always expression) )
// `ASSERT_ALWAYS(ERROR_addrGtMax, clk, reset, (addr < 5) )
// (Note no final semicolon)
`ifdef ET_ASSERT_OFF
   `define ASSERT_ALWAYS(NAME, CLK, RESET, ARG)
`else
   `ifdef VERILATOR
      `define ASSERT_ALWAYS(NAME, CLK, RESET, ARG)
   `else
      `define ASSERT_ALWAYS(NAME, CLK, RESET, ARG) \
         ast_``NAME : assert property (@(posedge CLK) disable iff ((EXCLUDEME_``NAME``_disable !== 1'b0) || (RESET !== 1'b0)) ARG) \
         else `assert_error("NAME", $sformatf("ERROR:%s%m: ASSERT_ALWAYS failed ", `VALIDPORT_RED_5_SPACES)) \
         static logic EXCLUDEME_``NAME``_disable; \
         always_comb begin // run once at initial time \
            if (RESET === 1'b1) begin \
               EXCLUDEME_``NAME``_disable <= `TD 1'b0; \
            end \
         end
   `endif // !ifdef VERILATOR
`endif // !ifdef ET_ASSERT_OFF


// `ASSERT_ALWAYS_MSG(ERROR_nameOfAssertion, clk, reset, (assert always expression), $sformatf("your strings with %stuff", $sampled(var) ) )
// `ASSERT_ALWAYS_MSG(ERROR_addrGtMax, clk, reset, (addr < 5), $sformatf("Addr(%x) >= 5", $sampled(addr) ) )
// (Note no final semicolon)

`ifdef ET_ASSERT_OFF
   `define ASSERT_ALWAYS_MSG(NAME, CLK, RESET, ARG, MSG)
`else
   `ifdef VERILATOR
      `define ASSERT_ALWAYS_MSG(NAME, CLK, RESET, ARG, MSG)
   `else
      `define ASSERT_ALWAYS_MSG(NAME, CLK, RESET, ARG, MSG) \
         ast_``NAME : assert property (@(posedge CLK) disable iff ((EXCLUDEME_``NAME``_disable !== 1'b0) || (RESET !== 1'b0)) ARG) \
         else `assert_error("ASSERT_ALWAYS_MSG", $sformatf("ERROR:%s%m: Assertion failed: %s", `VALIDPORT_RED_5_SPACES, MSG)) \
         static logic EXCLUDEME_``NAME``_disable; \
         always_comb begin // run once at initial time \
            if (RESET === 1'b1) begin \
               EXCLUDEME_``NAME``_disable <= `TD 1'b0; // ri lint_check_waive MACRO_OPER_PARENS \
            end \
         end
   `endif // !ifdef VERILATOR
`endif // !ifdef ET_ASSERT_OFF


// -----------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------
// in-line end-of-simulation end_of_sim (idle) checks.
//
// Use these to check that designs are in the desired state at simulation completion.

// `ASSERT_END_OF_SIM(ERROR_nameOfAssertion, (end of sim end_of_sim check expression should be true), -1)
// `ASSERT_END_OF_SIM(ERROR_NonEmptyFifoAtEndOfSim, fifo_empty, -1)
// -1 can be replaced with some mask method to prevent the end_of_sim check from being performed if not desired
// (Note no final semicolon)
`ifdef ET_ASSERT_OFF
   `define ASSERT_END_OF_SIM(NAME, ARG, MASK)
`else
   `ifdef VERILATOR
      `define ASSERT_END_OF_SIM(NAME, ARG, MASK)
   `else
      `ifndef QUIESCENT_SIGNAL
         `define ASSERT_END_OF_SIM(NAME, ARG, MASK)
      `else
         `define ASSERT_END_OF_SIM(NAME, ARG, MASK) \
            NAME : assert property (@(posedge ((`QUIESCENT_SIGNAL & MASK) != 0)) ARG) \
            else `assert_error("NAME", $sformatf("ERROR:%s%m: End of sim ASSERT_END_OF_SIM failed", `VALIDPORT_RED_5_SPACES))
      `endif
   `endif // !ifdef VERILATOR
`endif // !ifdef ET_ASSERT_OFF

// `ASSERT_END_OF_SIM_MSG(ERROR_nameOfAssertion, (end of sim end_of_sim check expression should be true), $sformatf("your strings with %stuff", $sampled(var) ), -1 )
// `ASSERT_END_OF_SIM_MSG(ERROR_NonEmptyFifoAtEndOfSim, fifo_empty, $sformatf("Oh know fifo isn't empty!"), -1 )
// (Note no final semicolon)
`ifdef ET_ASSERT_OFF
   `define ASSERT_END_OF_SIM_MSG(NAME, ARG, MSG, MASK)
`else
   `ifdef VERILATOR
      `define ASSERT_END_OF_SIM_MSG(NAME, ARG, MSG, MASK)
   `else
      `ifndef QUIESCENT_SIGNAL
         `define ASSERT_END_OF_SIM_MSG(NAME, ARG, MSG, MASK)
      `else
         `define ASSERT_END_OF_SIM_MSG(NAME, ARG, MSG, MASK) \
            ast_``NAME : assert property (@(posedge ((`QUIESCENT_SIGNAL & MASK) != 0)) ARG) \
            else `assert_error("NAME", $sformatf("ERROR:%s%m: End of sim assertion failed: %s", `VALIDPORT_RED_5_SPACES, MSG))
      `endif
   `endif // !ifdef VERILATOR
`endif // !ifdef ET_ASSERT_OFF

`ifdef ET_ASSERT_OFF
    `define ASSERT_ALWAYS_MESH_LAYER(MODULE, NAME, CLK, RESET, SOURCE_DIR, SOURCE_ID, TARGET_DIR, TARGET_ID, LAYER, VC_MASK)
`else
    `ifdef VERILATOR
        `define ASSERT_ALWAYS_MESH_LAYER(MODULE, NAME, CLK, RESET, SOURCE_DIR, SOURCE_ID, TARGET_DIR, TARGET_ID, LAYER, VC_MASK)
    `else
        `define ASSERT_ALWAYS_MESH_LAYER(MODULE, NAME, CLK, RESET, SOURCE_DIR, SOURCE_ID, TARGET_DIR, TARGET_ID, LAYER, VC_MASK)\
        generate begin :ASSERT_``NAME\
            localparam logic [3:0] VC_MASK_P = VC_MASK;\
            logic valid_request;\
            logic [3:0] vc_valids;\
            if (VC_MASK_P[0]) begin\
                assign vc_valids[0] = MODULE.ns_o_``SOURCE_DIR``_flit_valid_``LAYER``_``SOURCE_ID``_0;\
                `ASSERT_ALWAYS_MSG(valid_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_0,     CLK,    RESET,  (~$isunknown(vc_valids[0])), $sformatf("Valid for layer %d vc %d is unknown.", LAYER, 0));\
                `ASSERT_ALWAYS_MSG(credit_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_0,    CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_credit_inc_``LAYER``_``TARGET_ID``_0)), $sformatf("Credit for layer %d vc %d is unknown.", LAYER, 0));\
            end\
            else begin\
                assign vc_valids[0] = 1'b0;\
            end\
            if (VC_MASK_P[1]) begin\
                assign vc_valids[1] = MODULE.ns_o_``SOURCE_DIR``_flit_valid_``LAYER``_``SOURCE_ID``_1;\
                `ASSERT_ALWAYS_MSG(valid_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_1,     CLK,    RESET,  (~$isunknown(vc_valids[1])), $sformatf("Valid for layer %d vc %d is unknown.", LAYER, 1));\
                `ASSERT_ALWAYS_MSG(credit_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_1,    CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_credit_inc_``LAYER``_``TARGET_ID``_1)), $sformatf("Credit for layer %d vc %d is unknown.", LAYER, 1));\
            end\
            else begin\
                assign vc_valids[1] = 1'b0;\
            end\
            if (VC_MASK_P[2]) begin\
                assign vc_valids[2] = MODULE.ns_o_``SOURCE_DIR``_flit_valid_``LAYER``_``SOURCE_ID``_2;\
                `ASSERT_ALWAYS_MSG(valid_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_2,     CLK,    RESET,  (~$isunknown(vc_valids[2])), $sformatf("Valid for layer %d vc %d is unknown.", LAYER, 2));\
                `ASSERT_ALWAYS_MSG(credit_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_2,    CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_credit_inc_``LAYER``_``TARGET_ID``_2)), $sformatf("Credit for layer %d vc %d is unknown.", LAYER, 2));\
            end\
            else begin\
                assign vc_valids[2] = 1'b0;\
            end\
            if (VC_MASK_P[3]) begin\
                assign vc_valids[3] = MODULE.ns_o_``SOURCE_DIR``_flit_valid_``LAYER``_``SOURCE_ID``_3;\
                `ASSERT_ALWAYS_MSG(valid_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_3,     CLK,    RESET,  (~$isunknown(vc_valids[3])), $sformatf("Valid for layer %d vc %d is unknown.", LAYER, 3));\
                `ASSERT_ALWAYS_MSG(credit_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_3,    CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_credit_inc_``LAYER``_``TARGET_ID``_3)), $sformatf("Credit for layer %d vc %d is unknown.", LAYER, 3));\
            end\
            else begin\
                assign vc_valids[3] = 1'b0;\
            end\
            assign valid_request = |vc_valids;\
            \
            `ASSERT_ALWAYS_MSG(cg_busy_``NAME``_``SOURCE_ID``_``SOURCE_DIR,     CLK,    RESET,  (~$isunknown(MODULE.ns_o_``SOURCE_DIR``_cg_busy_``LAYER``_``SOURCE_ID)), $sformatf("ClockGate busy from layer %d direction %s is unknown", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(link_avail_``NAME``_``SOURCE_ID``_``SOURCE_DIR,  CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_link_available_``LAYER``_``TARGET_ID)), $sformatf("Link availablefrom layer %d direction %s is unknown", LAYER, TARGET_DIR));\
            \
            `ASSERT_ALWAYS_MSG(flit_sb_0_``NAME``_``SOURCE_ID``_``SOURCE_DIR,   CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_sb_``LAYER``_``SOURCE_ID``_0))), $sformatf("Flit sb 0 from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_sb_1_``NAME``_``SOURCE_ID``_``SOURCE_DIR,   CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_sb_``LAYER``_``SOURCE_ID``_1))), $sformatf("Flit sb 1 from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_sop_``NAME``_``SOURCE_ID``_``SOURCE_DIR,    CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_sop_``LAYER``_``SOURCE_ID))),    $sformatf("Flit sop from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_eop_``NAME``_``SOURCE_ID``_``SOURCE_DIR,    CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_eop_``LAYER``_``SOURCE_ID))),    $sformatf("Flit eop from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_bv_``NAME``_``SOURCE_ID``_``SOURCE_DIR,     CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_bv_``LAYER``_``SOURCE_ID))),     $sformatf("Flit bv from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_type_``NAME``_``SOURCE_ID``_``SOURCE_DIR,   CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_type_``LAYER``_``SOURCE_ID))),   $sformatf("Flit type from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_outp_``NAME``_``SOURCE_ID``_``SOURCE_DIR,   CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_outp_``LAYER``_``SOURCE_ID))),   $sformatf("Flit outp from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
        end\
        endgenerate
    `endif // ifdef VERILATOR
`endif // ifdef ET_ASSERT_OFF


`ifdef ET_ASSERT_OFF
    `define ASSERT_ALWAYS_MESH_PPLN_LAYER(MODULE, NAME, CLK, RESET, SOURCE_DIR, SOURCE_ID, SOURCE_INITIAL, TARGET_DIR, TARGET_ID, TARGET_INITIAL, LAYER, VC_MASK)
`else
    `ifdef VERILATOR
        `define ASSERT_ALWAYS_MESH_PPLN_LAYER(MODULE, NAME, CLK, RESET, SOURCE_DIR, SOURCE_ID, SOURCE_INITIAL, TARGET_DIR, TARGET_ID, TARGET_INITIAL, LAYER, VC_MASK)
    `else
        `define ASSERT_ALWAYS_MESH_PPLN_LAYER(MODULE, NAME, CLK, RESET, SOURCE_DIR, SOURCE_ID, SOURCE_INITIAL, TARGET_DIR, TARGET_ID, TARGET_INITIAL, LAYER, VC_MASK)\
        generate begin :ASSERT_``NAME\
            localparam logic [3:0] VC_MASK_P = VC_MASK;\
            logic valid_request;\
            logic [3:0] vc_valids;\
            if (VC_MASK_P[0]) begin\
                assign vc_valids[0] = MODULE.ns_o_``SOURCE_DIR``_flit_valid_``LAYER``_``SOURCE_ID``_0_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0;\
                `ASSERT_ALWAYS_MSG(valid_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_0,     CLK,    RESET,  (~$isunknown(vc_valids[0])), $sformatf("Valid for layer %d vc %d is unknown.", LAYER, 0));\
                `ASSERT_ALWAYS_MSG(credit_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_0,    CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_credit_inc_``LAYER``_``TARGET_ID``_0_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_1)), $sformatf("Credit for layer %d vc %d is unknown.", LAYER, 0));\
            end\
            else begin\
                assign vc_valids[0] = 1'b0;\
            end\
            if (VC_MASK_P[1]) begin\
                assign vc_valids[1] = MODULE.ns_o_``SOURCE_DIR``_flit_valid_``LAYER``_``SOURCE_ID``_1_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0;\
                `ASSERT_ALWAYS_MSG(valid_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_1,     CLK,    RESET,  (~$isunknown(vc_valids[1])), $sformatf("Valid for layer %d vc %d is unknown.", LAYER, 1));\
                `ASSERT_ALWAYS_MSG(credit_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_1,    CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_credit_inc_``LAYER``_``TARGET_ID``_1_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_1)), $sformatf("Credit for layer %d vc %d is unknown.", LAYER, 1));\
            end\
            else begin\
                assign vc_valids[1] = 1'b0;\
            end\
            if (VC_MASK_P[2]) begin\
                assign vc_valids[2] = MODULE.ns_o_``SOURCE_DIR``_flit_valid_``LAYER``_``SOURCE_ID``_2_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0;\
                `ASSERT_ALWAYS_MSG(valid_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_2,     CLK,    RESET,  (~$isunknown(vc_valids[2])), $sformatf("Valid for layer %d vc %d is unknown.", LAYER, 2));\
                `ASSERT_ALWAYS_MSG(credit_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_2,    CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_credit_inc_``LAYER``_``TARGET_ID``_2_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_1)), $sformatf("Credit for layer %d vc %d is unknown.", LAYER, 2));\
            end\
            else begin\
                assign vc_valids[2] = 1'b0;\
            end\
            if (VC_MASK_P[3]) begin\
                assign vc_valids[3] = MODULE.ns_o_``SOURCE_DIR``_flit_valid_``LAYER``_``SOURCE_ID``_3_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0;\
                `ASSERT_ALWAYS_MSG(valid_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_3,     CLK,    RESET,  (~$isunknown(vc_valids[3])), $sformatf("Valid for layer %d vc %d is unknown.", LAYER, 3));\
                `ASSERT_ALWAYS_MSG(credit_``NAME``_``SOURCE_ID``_``SOURCE_DIR``_vc_3,    CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_credit_inc_``LAYER``_``TARGET_ID``_3_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_1)), $sformatf("Credit for layer %d vc %d is unknown.", LAYER, 3));\
            end\
            else begin\
                assign vc_valids[3] = 1'b0;\
            end\
            assign valid_request = |vc_valids;\
            \
            `ASSERT_ALWAYS_MSG(cg_busy_``NAME``_``SOURCE_ID``_``SOURCE_DIR,     CLK,    RESET,  (~$isunknown(MODULE.ns_o_``SOURCE_DIR``_cg_busy_``LAYER``_``SOURCE_ID``_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0)), $sformatf("ClockGate busy from layer %d direction %s is unknown", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(link_avail_``NAME``_``SOURCE_ID``_``SOURCE_DIR,  CLK,    RESET,  (~$isunknown(MODULE.ns_o_``TARGET_DIR``_link_available_``LAYER``_``TARGET_ID``_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_1)), $sformatf("Link availablefrom layer %d direction %s is unknown", LAYER, TARGET_DIR));\
            \
            `ASSERT_ALWAYS_MSG(flit_sb_0_``NAME``_``SOURCE_ID``_``SOURCE_DIR,   CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_sb_``LAYER``_``SOURCE_ID``_0_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0))), $sformatf("Flit sb 0 from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_sb_1_``NAME``_``SOURCE_ID``_``SOURCE_DIR,   CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_sb_``LAYER``_``SOURCE_ID``_1_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0))), $sformatf("Flit sb 1 from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_sop_``NAME``_``SOURCE_ID``_``SOURCE_DIR,    CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_sop_``LAYER``_``SOURCE_ID``_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0))),    $sformatf("Flit sop from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_eop_``NAME``_``SOURCE_ID``_``SOURCE_DIR,    CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_eop_``LAYER``_``SOURCE_ID``_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0))),    $sformatf("Flit eop from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_bv_``NAME``_``SOURCE_ID``_``SOURCE_DIR,     CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_bv_``LAYER``_``SOURCE_ID``_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0))),     $sformatf("Flit bv from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_type_``NAME``_``SOURCE_ID``_``SOURCE_DIR,   CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_type_``LAYER``_``SOURCE_ID``_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0))),   $sformatf("Flit type from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
            `ASSERT_ALWAYS_MSG(flit_outp_``NAME``_``SOURCE_ID``_``SOURCE_DIR,   CLK,    RESET,  (~(valid_request && $isunknown(MODULE.ns_o_``SOURCE_DIR``_flit_outp_``LAYER``_``SOURCE_ID``_PPLN_R``LAYER``_``TARGET_ID````TARGET_INITIAL``_0))),   $sformatf("Flit outp from layer %d direction %s in unknown during request", LAYER, SOURCE_DIR));\
        end\
        endgenerate
    `endif // ifdef VERILATOR
`endif // ifdef ET_ASSERT_OFF

`endif // ifndef __ASSERT_CONST_VH
