// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"
// This is a wrapper for the level shifters from low to high voltage for 
// neighborhood cooperative interface
module neigh_coop_tload_ports(
  // System signals
  input  logic                                             clock,
  input  logic                                             reset,
  // Cooperative TLoad ports on HV part
  output coop_tload_slv_rdy_req_t                          coop_tload_slv_rdy_out_data,
  output logic                                             coop_tload_slv_rdy_out_valid,
  input  coop_tload_slv_rdy_req_t [(`NUM_NEIGH-1)-1:0]     coop_tload_slv_rdy_in_data,
  input  logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_slv_rdy_in_valid,
  output logic [`SHIRE_COOP_ID_RANGE]                      coop_tload_mst_done_out_coop_id,
  output logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_mst_done_out_valid,
  input  logic [(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_ID_RANGE]  coop_tload_mst_done_in_coop_id,
  input  logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_mst_done_in_valid,
  // Cooperative TLoad ports on LV part
  input  coop_tload_slv_rdy_req_t                          coop_tload_slv_rdy_out_data_lv,
  input  logic                                             coop_tload_slv_rdy_out_valid_lv,
  output coop_tload_slv_rdy_req_t [(`NUM_NEIGH-1)-1:0]     coop_tload_slv_rdy_in_data_lv,
  output logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_slv_rdy_in_valid_lv,
  input  logic [`SHIRE_COOP_ID_RANGE]                      coop_tload_mst_done_out_coop_id_lv,
  input  logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_mst_done_out_valid_lv,
  output logic [(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_ID_RANGE]  coop_tload_mst_done_in_coop_id_lv,
  output logic [(`NUM_NEIGH-1)-1:0]                        coop_tload_mst_done_in_valid_lv
);

  coop_tload_slv_rdy_req_t                         coop_tload_slv_rdy_out_data_hv;
  logic                                            coop_tload_slv_rdy_out_valid_hv;
  coop_tload_slv_rdy_req_t [(`NUM_NEIGH-1)-1:0]    coop_tload_slv_rdy_in_data_hv;
  logic [(`NUM_NEIGH-1)-1:0]                       coop_tload_slv_rdy_in_valid_hv;
  logic [`SHIRE_COOP_ID_RANGE]                     coop_tload_mst_done_out_coop_id_hv;
  logic [(`NUM_NEIGH-1)-1:0]                       coop_tload_mst_done_out_valid_hv;
  logic [(`NUM_NEIGH-1)-1:0][`SHIRE_COOP_ID_RANGE] coop_tload_mst_done_in_coop_id_hv;
  logic [(`NUM_NEIGH-1)-1:0]                       coop_tload_mst_done_in_valid_hv;

// Low to high output ports to shire_channel
// -----------------------------------------
// Level shifters
  vclevel_shft_l2h #(
    .WIDTH ($bits(coop_tload_slv_rdy_req_t) + 1)
  ) ls_l2h_coop_tload_rdy_out_pd_hv (
    .inp_lv ({coop_tload_slv_rdy_out_data_lv, coop_tload_slv_rdy_out_valid_lv}),
    .out_hv ({coop_tload_slv_rdy_out_data_hv, coop_tload_slv_rdy_out_valid_hv})
  );
  // level shifters should be automatically added because UPF constraints define
  // set_isolation to these *_lv signals
  //assign {coop_tload_slv_rdy_out_data_hv, coop_tload_slv_rdy_out_valid_hv} = {coop_tload_slv_rdy_out_data_lv, coop_tload_slv_rdy_out_valid_lv};
       
  vclevel_shft_l2h #(
    .WIDTH (`SHIRE_COOP_ID_SIZE + `NUM_NEIGH - 1)
  )
  ls_l2h_coop_tload_done_out_pd_hv (
    .inp_lv ({coop_tload_mst_done_out_coop_id_lv, coop_tload_mst_done_out_valid_lv}),
    .out_hv ({coop_tload_mst_done_out_coop_id_hv, coop_tload_mst_done_out_valid_hv})
  );
  // level shifters should be automatically added because UPF constraints define
  // set_isolation to these *_lv signals
  //assign {coop_tload_mst_done_out_coop_id_hv, coop_tload_mst_done_out_valid_hv} = {coop_tload_mst_done_out_coop_id_lv, coop_tload_mst_done_out_valid_lv};

// FF stage
  //      CLK    RST    EN                                 DOUT                              DIN                               DEF
  `RST_FF(clock, reset,                                    coop_tload_slv_rdy_out_valid,     coop_tload_slv_rdy_out_valid_hv,  1'b0)
  `EN_FF (clock,        coop_tload_slv_rdy_out_valid_hv,   coop_tload_slv_rdy_out_data,      coop_tload_slv_rdy_out_data_hv)
  `RST_FF(clock, reset,                                    coop_tload_mst_done_out_valid,    coop_tload_mst_done_out_valid_hv, 1'b0)
  `EN_FF (clock,        |coop_tload_mst_done_out_valid_hv, coop_tload_mst_done_out_coop_id,  coop_tload_mst_done_out_coop_id_hv)


// High to low input ports from shire_channel
// ------------------------------------------
// FF stage
  for (genvar i = 0; i < `NUM_NEIGH-1; i++)
  begin
    //      CLK    RST    EN                               DOUT                                  DIN                              DEF
    `RST_FF(clock, reset,                                  coop_tload_slv_rdy_in_valid_hv[i],    coop_tload_slv_rdy_in_valid[i],  1'b0)
    `EN_FF (clock,        coop_tload_slv_rdy_in_valid[i],  coop_tload_slv_rdy_in_data_hv[i],     coop_tload_slv_rdy_in_data[i])
    `RST_FF(clock, reset,                                  coop_tload_mst_done_in_valid_hv[i],   coop_tload_mst_done_in_valid[i], 1'b0)
    `EN_FF (clock,        coop_tload_mst_done_in_valid[i], coop_tload_mst_done_in_coop_id_hv[i], coop_tload_mst_done_in_coop_id[i])
  end

// Level shifters
  vclevel_shft_h2l #(
    .WIDTH (($bits(coop_tload_slv_rdy_req_t) + 1) * (`NUM_NEIGH - 1))
  )
  ls_h2l_coop_tload_rdy_in_pd_hv (
    .inp_hv ({coop_tload_slv_rdy_in_data_hv, coop_tload_slv_rdy_in_valid_hv}),
    .out_lv ({coop_tload_slv_rdy_in_data_lv, coop_tload_slv_rdy_in_valid_lv})
  );
  // level shifters should be automatically added
  //assign {coop_tload_slv_rdy_in_data_lv, coop_tload_slv_rdy_in_valid_lv} = {coop_tload_slv_rdy_in_data_hv, coop_tload_slv_rdy_in_valid_hv};
       

//
  vclevel_shft_h2l #(
    .WIDTH ((`SHIRE_COOP_ID_SIZE + 1) * (`NUM_NEIGH - 1))
  ) ls_h2l_coop_tload_done_in_pd_hv (
    .inp_hv ({coop_tload_mst_done_in_coop_id_hv, coop_tload_mst_done_in_valid_hv}),
    .out_lv ({coop_tload_mst_done_in_coop_id_lv, coop_tload_mst_done_in_valid_lv})
  );
  // level shifters should be automatically added
  //assign {coop_tload_mst_done_in_coop_id_lv, coop_tload_mst_done_in_valid_lv} = {coop_tload_mst_done_in_coop_id_hv, coop_tload_mst_done_in_valid_hv};
        
endmodule
