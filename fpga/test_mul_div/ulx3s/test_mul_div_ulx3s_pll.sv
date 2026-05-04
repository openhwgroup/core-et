// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// 25 MHz ULX3S clock to 10 MHz core clock PLL.
//
// Generated from a known-good ecppll configuration:
//   refclk divisor = 5
//   feedback divisor = 2
//   clkout divisor = 60
//   VCO = 600 MHz

module test_mul_div_ulx3s_pll (
  input  logic clkin_i,
  output logic clkout_o,
  output logic locked_o
);

  (* FREQUENCY_PIN_CLKI="25" *)
  (* FREQUENCY_PIN_CLKOP="10" *)
  (* ICP_CURRENT="12" *)
  (* LPF_RESISTOR="8" *)
  (* MFG_ENABLE_FILTEROPAMP="1" *)
  (* MFG_GMCREF_SEL="2" *)
  EHXPLLL #(
    .PLLRST_ENA        ("DISABLED"),
    .INTFB_WAKE        ("DISABLED"),
    .STDBY_ENABLE      ("DISABLED"),
    .DPHASE_SOURCE     ("DISABLED"),
    .OUTDIVIDER_MUXA   ("DIVA"),
    .OUTDIVIDER_MUXB   ("DIVB"),
    .OUTDIVIDER_MUXC   ("DIVC"),
    .OUTDIVIDER_MUXD   ("DIVD"),
    .CLKI_DIV          (5),
    .CLKOP_ENABLE      ("ENABLED"),
    .CLKOP_DIV         (60),
    .CLKOP_CPHASE      (30),
    .CLKOP_FPHASE      (0),
    .FEEDBK_PATH       ("CLKOP"),
    .CLKFB_DIV         (2)
  ) u_pll (
    .RST               (1'b0),
    .STDBY             (1'b0),
    .CLKI              (clkin_i),
    .CLKOP             (clkout_o),
    .CLKFB             (clkout_o),
    .CLKINTFB          (),
    .PHASESEL0         (1'b0),
    .PHASESEL1         (1'b0),
    .PHASEDIR          (1'b1),
    .PHASESTEP         (1'b1),
    .PHASELOADREG      (1'b1),
    .PLLWAKESYNC       (1'b0),
    .ENCLKOP           (1'b0),
    .ENCLKOS           (1'b0),
    .ENCLKOS2          (1'b0),
    .ENCLKOS3          (1'b0),
    .LOCK              (locked_o),
    .INTLOCK           (),
    .REFCLK            (),
    .CLKOS             (),
    .CLKOS2            (),
    .CLKOS3            ()
  );

endmodule
