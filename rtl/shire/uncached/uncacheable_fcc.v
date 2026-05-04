// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

// This is the module that implements atomic operations
module uncacheable_fcc
# ( parameter NUM_NEIGH = `NUM_NEIGH
)
(
    // System signals
    output logic [NUM_NEIGH-1:0][`MIN_PER_N-1:0]        fcc,
    output logic [`UC_FCC_TARGET_SIZE-1:0]               fcc_target,
    //Remote IPI ESRs
    input uc_esr_fcc_enable_sigs                         esr_enables,
    input [`ESR_APB_D_WIDTH-1:0]                         esr_wdata
    );

    //This one implies a priority encoding 
    //always_comb begin
    //    fcc = '0;
    //    fcc_target = 'X;
    //    if(|esr_enables != 0) begin
    //        int i;
    //        fcc = esr_wdata[`MIN_PER_N*NUM_NEIGH-1:0];
    //        for(i = 0; i < `UC_FCC_REGISTERS; ++i) begin
    //            if(esr_enables[i] != 0) begin
    //                fcc_target = i;
    //            end
    //        end
    //    end
    //end

    //Parameterized synthesizable mux optimized for a one-hot input (i.e. no priority encoding) using an OR tree
    always_comb begin
        fcc        = {`MIN_PER_N*NUM_NEIGH{1'b0}};
        fcc_target = {`UC_FCC_TARGET_SIZE {1'b0}}; 
        if(|esr_enables != 0) begin
            fcc = esr_wdata[`MIN_PER_N*NUM_NEIGH-1:0]; 
        end
        for(int unsigned i = 0; i < `UC_FCC_REGISTERS; i++) begin
            fcc_target |= {`UC_FCC_TARGET_SIZE{esr_enables[i]}} & i[`UC_FCC_TARGET_SIZE-1:0];

        end
    end

endmodule

