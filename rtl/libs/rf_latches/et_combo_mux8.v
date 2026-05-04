// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

// et_combo_mux8
//    combinatorial MUX8, built from MUX2 gates, for PD dont touch

module et_combo_mux8 (
   output logic x,
   input  logic [7:0] d,
   input  logic [2:0] s
);
   
`ifdef ET_LIBS_USE_STD_CELLS
   generate
      logic l0_0;
      logic l0_1;
      logic l0_2;
      logic l0_3;

      logic l1_0;
      logic l1_1;

      if (1) begin : ET_DONT_TOUCH_et_combo_mux8
         HDBULT11_MUX2_MM_2 mux2_l0_0 (.X(l0_0), .D0(d[0]), .D1(d[1]), .S(s[0]));
         HDBULT11_MUX2_MM_2 mux2_l0_1 (.X(l0_1), .D0(d[2]), .D1(d[3]), .S(s[0]));
         HDBULT11_MUX2_MM_2 mux2_l0_2 (.X(l0_2), .D0(d[4]), .D1(d[5]), .S(s[0]));
         HDBULT11_MUX2_MM_2 mux2_l0_3 (.X(l0_3), .D0(d[6]), .D1(d[7]), .S(s[0]));

         HDBULT11_MUX2_MM_2 mux2_l1_0 (.X(l1_0), .D0(l0_0), .D1(l0_1), .S(s[1]));
         HDBULT11_MUX2_MM_2 mux2_l1_1 (.X(l1_1), .D0(l0_2), .D1(l0_3), .S(s[1]));

         HDBULT11_MUX2_MM_2 mux2_l2_0 (.X(   x), .D0(l1_0), .D1(l1_1), .S(s[2]));
      end
   endgenerate
`else
   always @(*) begin
       case (s[2:0])
           3'b000 : x = d[0];
           3'b001 : x = d[1];
           3'b010 : x = d[2];
           3'b011 : x = d[3];
           3'b100 : x = d[4];
           3'b101 : x = d[5];
           3'b110 : x = d[6];
           3'b111 : x = d[7];
       endcase
   end
`endif

endmodule


