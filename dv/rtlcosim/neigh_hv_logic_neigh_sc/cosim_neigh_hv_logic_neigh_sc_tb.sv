// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_neigh_sc_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [3:0] miss_req_bank_valid_i,
  input  logic [4:0] neigh_sc_req_ready_i,
  input  logic [63:0] bank0_seed_i,
  input  logic [63:0] bank1_seed_i,
  input  logic [63:0] bank2_seed_i,
  input  logic [63:0] bank3_seed_i,
  input  logic [63:0] uc_seed_i,
  input  logic        uc_req_info_pop_valid_i,
  input  logic        uc_wdata_i,

  output logic [3:0] orig_miss_req_bank_ready_o,
  output logic [3:0] new_miss_req_bank_ready_o,
  output logic       orig_uc_req_info_pop_o,
  output logic       new_uc_req_info_pop_o,
  output logic       orig_uc_req_data_pop_o,
  output logic       new_uc_req_data_pop_o,
  output logic [4:0] orig_neigh_sc_req_valid_o,
  output logic [4:0] new_neigh_sc_req_valid_o,
  output logic [7:0] orig_neigh_sc_req_id_o,
  output logic [7:0] new_neigh_sc_req_id_o,
  output logic [3:0] orig_neigh_sc_req_source_o,
  output logic [3:0] new_neigh_sc_req_source_o,
  output logic       orig_neigh_sc_req_wdata_o,
  output logic       new_neigh_sc_req_wdata_o,
  output logic [4:0] orig_neigh_sc_req_opcode_o,
  output logic [4:0] new_neigh_sc_req_opcode_o,
  output logic [6:0] orig_neigh_sc_req_subopcode_o,
  output logic [6:0] new_neigh_sc_req_subopcode_o,
  output logic [39:0] orig_neigh_sc_req_address_o,
  output logic [39:0] new_neigh_sc_req_address_o,
  output logic [2:0] orig_neigh_sc_req_size_o,
  output logic [2:0] new_neigh_sc_req_size_o,
  output logic [3:0] orig_neigh_sc_req_qwen_o,
  output logic [3:0] new_neigh_sc_req_qwen_o,
  output logic [63:0] orig_neigh_sc_req_data0_o,
  output logic [63:0] new_neigh_sc_req_data0_o,
  output logic [63:0] orig_neigh_sc_req_data1_o,
  output logic [63:0] new_neigh_sc_req_data1_o,
  output logic [63:0] orig_neigh_sc_req_data2_o,
  output logic [63:0] new_neigh_sc_req_data2_o,
  output logic [63:0] orig_neigh_sc_req_data3_o,
  output logic [63:0] new_neigh_sc_req_data3_o,
  output logic [63:0] orig_neigh_sc_req_data4_o,
  output logic [63:0] new_neigh_sc_req_data4_o,
  output logic [63:0] orig_neigh_sc_req_data5_o,
  output logic [63:0] new_neigh_sc_req_data5_o,
  output logic [63:0] orig_neigh_sc_req_data6_o,
  output logic [63:0] new_neigh_sc_req_data6_o,
  output logic [63:0] orig_neigh_sc_req_data7_o,
  output logic [63:0] new_neigh_sc_req_data7_o
);

  localparam int unsigned ScBanks = 4;

  logic [ScBanks*584-1:0] orig_miss_req_bank_info;
  logic [71:0] orig_uc_req_info;
  logic [255:0] uc_req_data;
  logic [583:0] orig_neigh_sc_req_info;
  etlink_pkg::req_t [ScBanks-1:0] new_miss_req_bank_info;
  etlink_pkg::req_nodata_t new_uc_req_info;
  etlink_pkg::req_t new_neigh_sc_req_info;

  function automatic logic [63:0] data_word(input logic [63:0] seed, input int unsigned idx);
    data_word = seed ^ (64'h9e37_79b9_7f4a_7c15 * (64'd1 + {32'h0, idx}));
  endfunction

  function automatic logic [511:0] full_data(input logic [63:0] seed);
    full_data = {data_word(seed, 7), data_word(seed, 6), data_word(seed, 5), data_word(seed, 4),
                 data_word(seed, 3), data_word(seed, 2), data_word(seed, 1), data_word(seed, 0)};
  endfunction

  function automatic logic [255:0] half_data(input logic [63:0] seed);
    half_data = {data_word(seed, 3), data_word(seed, 2), data_word(seed, 1), data_word(seed, 0)};
  endfunction

  function automatic logic [583:0] pack_req_bits(input logic [63:0] seed);
    pack_req_bits = {seed[7:0],
                     seed[11:8],
                     seed[12],
                     seed[17:13],
                     seed[57:18],
                     full_data(seed),
                     seed[60:58],
                     seed[63:60],
                     seed[24:18]};
  endfunction

  function automatic logic [71:0] pack_nodata_bits(input logic [63:0] seed, input logic wdata);
    pack_nodata_bits = {seed[7:0],
                        seed[11:8],
                        wdata,
                        seed[17:13],
                        seed[57:18],
                        seed[60:58],
                        seed[63:60],
                        seed[24:18]};
  endfunction

  function automatic etlink_pkg::req_t pack_req(input logic [63:0] seed);
    etlink_pkg::req_t req;
    req.id = seed[7:0];
    req.source = seed[11:8];
    req.wdata = seed[12];
    req.opcode = etlink_pkg::req_opcode_e'(seed[17:13]);
    req.address = seed[57:18];
    req.data = full_data(seed);
    req.size = etlink_pkg::size_e'(seed[60:58]);
    req.qwen = seed[63:60];
    req.subopcode = seed[24:18];
    return req;
  endfunction

  function automatic etlink_pkg::req_nodata_t pack_nodata(input logic [63:0] seed,
                                                          input logic wdata);
    etlink_pkg::req_nodata_t req;
    req.id = seed[7:0];
    req.source = seed[11:8];
    req.wdata = wdata;
    req.opcode = etlink_pkg::req_opcode_e'(seed[17:13]);
    req.address = seed[57:18];
    req.size = etlink_pkg::size_e'(seed[60:58]);
    req.qwen = seed[63:60];
    req.subopcode = seed[24:18];
    return req;
  endfunction

  always_comb begin
    orig_miss_req_bank_info[0*584 +: 584] = pack_req_bits(bank0_seed_i);
    orig_miss_req_bank_info[1*584 +: 584] = pack_req_bits(bank1_seed_i);
    orig_miss_req_bank_info[2*584 +: 584] = pack_req_bits(bank2_seed_i);
    orig_miss_req_bank_info[3*584 +: 584] = pack_req_bits(bank3_seed_i);
    new_miss_req_bank_info[0] = pack_req(bank0_seed_i);
    new_miss_req_bank_info[1] = pack_req(bank1_seed_i);
    new_miss_req_bank_info[2] = pack_req(bank2_seed_i);
    new_miss_req_bank_info[3] = pack_req(bank3_seed_i);
    orig_uc_req_info = pack_nodata_bits(uc_seed_i, uc_wdata_i);
    new_uc_req_info = pack_nodata(uc_seed_i, uc_wdata_i);
    uc_req_data = half_data(uc_seed_i);
  end

  assign orig_neigh_sc_req_id_o = orig_neigh_sc_req_info[583:576];
  assign orig_neigh_sc_req_source_o = orig_neigh_sc_req_info[575:572];
  assign orig_neigh_sc_req_wdata_o = orig_neigh_sc_req_info[571];
  assign orig_neigh_sc_req_opcode_o = orig_neigh_sc_req_info[570:566];
  assign orig_neigh_sc_req_address_o = orig_neigh_sc_req_info[565:526];
  assign {orig_neigh_sc_req_data7_o, orig_neigh_sc_req_data6_o,
          orig_neigh_sc_req_data5_o, orig_neigh_sc_req_data4_o,
          orig_neigh_sc_req_data3_o, orig_neigh_sc_req_data2_o,
          orig_neigh_sc_req_data1_o, orig_neigh_sc_req_data0_o} =
      orig_neigh_sc_req_info[525:14];
  assign orig_neigh_sc_req_size_o = orig_neigh_sc_req_info[13:11];
  assign orig_neigh_sc_req_qwen_o = orig_neigh_sc_req_info[10:7];
  assign orig_neigh_sc_req_subopcode_o = orig_neigh_sc_req_info[6:0];

  assign new_neigh_sc_req_id_o = new_neigh_sc_req_info.id;
  assign new_neigh_sc_req_source_o = new_neigh_sc_req_info.source;
  assign new_neigh_sc_req_wdata_o = new_neigh_sc_req_info.wdata;
  assign new_neigh_sc_req_opcode_o = new_neigh_sc_req_info.opcode;
  assign new_neigh_sc_req_address_o = new_neigh_sc_req_info.address;
  assign {new_neigh_sc_req_data7_o, new_neigh_sc_req_data6_o,
          new_neigh_sc_req_data5_o, new_neigh_sc_req_data4_o,
          new_neigh_sc_req_data3_o, new_neigh_sc_req_data2_o,
          new_neigh_sc_req_data1_o, new_neigh_sc_req_data0_o} =
      new_neigh_sc_req_info.data;
  assign new_neigh_sc_req_size_o = new_neigh_sc_req_info.size;
  assign new_neigh_sc_req_qwen_o = new_neigh_sc_req_info.qwen;
  assign new_neigh_sc_req_subopcode_o = new_neigh_sc_req_info.subopcode;

  neigh_hv_logic_neigh_sc u_new (
    .clk_shire_i              (clk_i),
    .rst_shire_ndm_ni         (rst_ni),
    .miss_req_bank_info_i     (new_miss_req_bank_info),
    .miss_req_bank_valid_i    (miss_req_bank_valid_i),
    .miss_req_bank_ready_o    (new_miss_req_bank_ready_o),
    .uc_req_info_out_i        (new_uc_req_info),
    .uc_req_data_out_i        (uc_req_data),
    .uc_req_info_pop_valid_i  (uc_req_info_pop_valid_i),
    .uc_req_info_pop_o        (new_uc_req_info_pop_o),
    .uc_req_data_pop_o        (new_uc_req_data_pop_o),
    .neigh_sc_req_info_o      (new_neigh_sc_req_info),
    .neigh_sc_req_valid_o     (new_neigh_sc_req_valid_o),
    .neigh_sc_req_ready_i     (neigh_sc_req_ready_i)
  );

  neigh_hv_logic_neigh_sc_orig u_orig (
    .clock_shire              (clk_i),
    .reset_shire_ndm          (!rst_ni),
    .miss_req_bank_info       (orig_miss_req_bank_info),
    .miss_req_bank_valid      (miss_req_bank_valid_i),
    .miss_req_bank_ready      (orig_miss_req_bank_ready_o),
    .uc_req_info_out          (orig_uc_req_info),
    .uc_req_data_out          (uc_req_data),
    .uc_req_info_pop_valid    (uc_req_info_pop_valid_i),
    .uc_req_info_pop          (orig_uc_req_info_pop_o),
    .uc_req_data_pop          (orig_uc_req_data_pop_o),
    .neigh_sc_req_info        (orig_neigh_sc_req_info),
    .neigh_sc_req_valid       (orig_neigh_sc_req_valid_o),
    .neigh_sc_req_ready       (neigh_sc_req_ready_i)
  );

endmodule : cosim_neigh_hv_logic_neigh_sc_tb
