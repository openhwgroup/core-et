// This is a behavior model of a bidrectional pad cell.

module pad_bidi_beh
#(
  parameter PE = 0,
  parameter PS = 0
)
(
    inout  wire                                          pad,

    input  logic                                         i,
    input  logic                                         oe,

    output logic                                         c
);

assign c = (pad !== 1'bz) ? pad :                             // driven by pad driver state
                            (PE == 0) ? 1'bx :                // not driven and no pull resistor
                                        (PS == 0) ? 1'b0 :    // not driven, pull down
                                                    1'b1;     // not driven, pull up

assign pad = (oe === 1'b1) ? i :
             (oe === 1'b0) ? (PE == 0) ? 1'bz :               // not driven and no pull resisotr
                                         (PS == 0) ? 1'b0 :   // not driven, pull down
                                                     1'b1     // not driven, pull up
                           : 1'bx;                            // (oe === 1'bx || oe === 1'bz)

endmodule
