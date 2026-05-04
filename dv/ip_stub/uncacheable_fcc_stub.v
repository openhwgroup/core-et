`include "soc.vh"

module uncacheable_fcc_stub 
# (
     parameter int NUM_UC    = 0,
     parameter int NUM_BANKS = `SC_BANKS
)(
    // General signals
    input  logic                                        clock,
    input  logic                                        reset,

    // Neigh miss request bus
    input  et_link_req_info_t                           neigh_sc_req_info,
    input  logic              [(NUM_BANKS+NUM_UC)-1:0]  neigh_sc_req_valid,

    // To Neigh signals
    output logic              [`MIN_PER_N-1:0]          uc_to_neigh_fcc,
    output logic              [`UC_FCC_TARGET_SIZE-1:0] uc_to_neigh_fcc_target
);

    logic [`MIN_PER_N-1:0] uc_to_neigh_fcc_next;
    logic [`UC_FCC_TARGET_SIZE-1:0] uc_to_neigh_fcc_target_next;

    `RST_FF(clock, reset,        uc_to_neigh_fcc,        uc_to_neigh_fcc_next, '0)
    `RST_FF(clock, reset, uc_to_neigh_fcc_target, uc_to_neigh_fcc_target_next, '0)

    always_comb begin
        if (neigh_sc_req_valid[(NUM_BANKS+NUM_UC)-1] == 1) begin
            if (neigh_sc_req_info.address == `PA_SIZE'h08034_00c0) begin
                uc_to_neigh_fcc_next = neigh_sc_req_info.data[`MIN_PER_N-1:0];
                uc_to_neigh_fcc_target_next = 0;
            end 
            else if (neigh_sc_req_info.address == `PA_SIZE'h08034_00c8) begin
                uc_to_neigh_fcc_next = neigh_sc_req_info.data[(`MIN_PER_N+64)-1:64];
                uc_to_neigh_fcc_target_next = 1;
            end 
            else if (neigh_sc_req_info.address == `PA_SIZE'h08034_00d0) begin
                uc_to_neigh_fcc_next = neigh_sc_req_info.data[(`MIN_PER_N+128)-1:128];
                uc_to_neigh_fcc_target_next = 2;
            end 
            else if (neigh_sc_req_info.address == `PA_SIZE'h08034_00d8) begin
                uc_to_neigh_fcc_next = neigh_sc_req_info.data[(`MIN_PER_N+192)-1:192];
                uc_to_neigh_fcc_target_next = 3;
            end 
            else begin
                uc_to_neigh_fcc_next = '0;
                uc_to_neigh_fcc_target_next = '0;
            end
        end
        else begin
            uc_to_neigh_fcc_next = '0;
            uc_to_neigh_fcc_target_next = '0;
        end
    end

endmodule
