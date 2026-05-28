// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module shire_and_or_tree_daisychain #(
  parameter int unsigned NumNeigh = shire_channel_leaves_pkg::NumNeigh,
  parameter int unsigned ShireIdWidth = shire_channel_leaves_pkg::NumShireIdsBits
) (
  input  logic [ShireIdWidth-1:0]                         shire_id_i,
  input  shire_channel_leaves_pkg::esr_and_or_tree_l2_t   debug_and_or_tree_l2_i,
  output shire_channel_leaves_pkg::esr_and_or_tree_l2_t   debug_and_or_tree_l2_o,
  input  esr_pkg::esr_and_or_tree_l0_t                    esr_and_or_tree_l0_i [NumNeigh-1:0],
  output shire_esr_pkg::esr_and_or_tree_l1_t              esr_and_or_tree_l1_local_o
);

  localparam int unsigned Half = (NumNeigh / 2) + ((NumNeigh % 2) == 0 ? 0 : 1);

  shire_esr_pkg::esr_and_or_tree_l1_t esr_and_or_tree_l1_local;
  logic anyselected_neigh;
  logic anyhalted_local;

  always_comb begin
    esr_and_or_tree_l1_local.anyhalted0 = 1'b0;
    esr_and_or_tree_l1_local.anyhalted1 = 1'b0;
    for (int unsigned i = 0; i < Half; i++) begin
      esr_and_or_tree_l1_local.anyhalted0 |= esr_and_or_tree_l0_i[i].anyhalted;
    end
    for (int unsigned i = Half; i < NumNeigh; i++) begin
      esr_and_or_tree_l1_local.anyhalted1 |= esr_and_or_tree_l0_i[i].anyhalted;
    end
  end

  always_comb begin
    esr_and_or_tree_l1_local.allhalted      = 1'b1;
    esr_and_or_tree_l1_local.allhavereset   = 1'b1;
    esr_and_or_tree_l1_local.allresumeack   = 1'b1;
    esr_and_or_tree_l1_local.allrunning     = 1'b1;
    esr_and_or_tree_l1_local.anyhavereset   = 1'b0;
    esr_and_or_tree_l1_local.anyresumeack   = 1'b0;
    esr_and_or_tree_l1_local.anyrunning     = 1'b0;
    esr_and_or_tree_l1_local.anyselected    = 1'b0;
    esr_and_or_tree_l1_local.anyunavailable = 1'b0;
    anyselected_neigh = 1'b0;

    for (int unsigned i = 0; i < NumNeigh; i++) begin
      anyselected_neigh |= esr_and_or_tree_l0_i[i].anyselected;
    end

    for (int unsigned i = 0; i < NumNeigh; i++) begin
      esr_and_or_tree_l1_local.allhalted &=
          esr_and_or_tree_l0_i[i].anyselected ? esr_and_or_tree_l0_i[i].allhalted :
                                                anyselected_neigh;
      esr_and_or_tree_l1_local.allhavereset &=
          esr_and_or_tree_l0_i[i].anyselected ? esr_and_or_tree_l0_i[i].allhavereset :
                                                anyselected_neigh;
      esr_and_or_tree_l1_local.allresumeack &=
          esr_and_or_tree_l0_i[i].anyselected ? esr_and_or_tree_l0_i[i].allresumeack :
                                                anyselected_neigh;
      esr_and_or_tree_l1_local.allrunning &=
          esr_and_or_tree_l0_i[i].anyselected ? esr_and_or_tree_l0_i[i].allrunning :
                                                anyselected_neigh;
      esr_and_or_tree_l1_local.anyhavereset |= esr_and_or_tree_l0_i[i].anyhavereset;
      esr_and_or_tree_l1_local.anyresumeack |= esr_and_or_tree_l0_i[i].anyresumeack;
      esr_and_or_tree_l1_local.anyrunning |= esr_and_or_tree_l0_i[i].anyrunning;
      esr_and_or_tree_l1_local.anyselected |= esr_and_or_tree_l0_i[i].anyselected;
      esr_and_or_tree_l1_local.anyunavailable |= esr_and_or_tree_l0_i[i].anyunavailable;
    end
  end

  assign esr_and_or_tree_l1_local_o = esr_and_or_tree_l1_local;
  assign anyhalted_local = esr_and_or_tree_l1_local.anyhalted0 |
                           esr_and_or_tree_l1_local.anyhalted1;

  always_comb begin
    debug_and_or_tree_l2_o.anyhalted0 = (shire_id_i < 8'd16) ?
        (anyhalted_local | debug_and_or_tree_l2_i.anyhalted0) :
        debug_and_or_tree_l2_i.anyhalted0;
    debug_and_or_tree_l2_o.anyhalted1 = ((shire_id_i > 8'd16) && (shire_id_i < 8'd32)) ?
        (anyhalted_local | debug_and_or_tree_l2_i.anyhalted1) :
        debug_and_or_tree_l2_i.anyhalted1;
    debug_and_or_tree_l2_o.anyhalted2 = (shire_id_i > 8'd31) ?
        (anyhalted_local | debug_and_or_tree_l2_i.anyhalted2) :
        debug_and_or_tree_l2_i.anyhalted2;
    debug_and_or_tree_l2_o.allhalted = esr_and_or_tree_l1_local.anyselected ?
        (esr_and_or_tree_l1_local.allhalted & debug_and_or_tree_l2_i.allhalted) :
        debug_and_or_tree_l2_i.allhalted;
    debug_and_or_tree_l2_o.anyrunning = esr_and_or_tree_l1_local.anyrunning |
                                        debug_and_or_tree_l2_i.anyrunning;
    debug_and_or_tree_l2_o.allrunning = esr_and_or_tree_l1_local.anyselected ?
        (esr_and_or_tree_l1_local.allrunning & debug_and_or_tree_l2_i.allrunning) :
        debug_and_or_tree_l2_i.allrunning;
    debug_and_or_tree_l2_o.anyresumeack = esr_and_or_tree_l1_local.anyresumeack |
                                          debug_and_or_tree_l2_i.anyresumeack;
    debug_and_or_tree_l2_o.allresumeack = esr_and_or_tree_l1_local.anyselected ?
        (esr_and_or_tree_l1_local.allresumeack & debug_and_or_tree_l2_i.allresumeack) :
        debug_and_or_tree_l2_i.allresumeack;
    debug_and_or_tree_l2_o.anyhavereset = esr_and_or_tree_l1_local.anyhavereset |
                                          debug_and_or_tree_l2_i.anyhavereset;
    debug_and_or_tree_l2_o.allhavereset = esr_and_or_tree_l1_local.anyselected ?
        (esr_and_or_tree_l1_local.allhavereset & debug_and_or_tree_l2_i.allhavereset) :
        debug_and_or_tree_l2_i.allhavereset;
    debug_and_or_tree_l2_o.anyunavailable = esr_and_or_tree_l1_local.anyunavailable |
                                            debug_and_or_tree_l2_i.anyunavailable;
  end

endmodule
