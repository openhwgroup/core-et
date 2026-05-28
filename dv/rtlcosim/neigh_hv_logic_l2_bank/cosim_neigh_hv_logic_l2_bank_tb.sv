// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module cosim_neigh_hv_logic_l2_bank_tb (
  input  logic        clk_i,
  input  logic        rst_ni,
  input  logic        bank_req_out_is_first_half_i,
  input  logic [7:0]  bank_req_id_i,
  input  logic [3:0]  bank_req_source_i,
  input  logic        bank_req_wdata_i,
  input  logic [4:0]  bank_req_opcode_i,
  input  logic [6:0]  bank_req_subopcode_i,
  input  logic [39:0] bank_req_address_i,
  input  logic [2:0]  bank_req_size_i,
  input  logic [3:0]  bank_req_qwen_i,
  input  logic [63:0] bank_req_data0_i,
  input  logic [63:0] bank_req_data1_i,
  input  logic [63:0] bank_req_data2_i,
  input  logic [63:0] bank_req_data3_i,
  input  logic        bank_req_info_valid_i,
  input  logic        miss_req_bank_ready_i,

  output logic        orig_bank_req_info_pop_o,
  output logic        new_bank_req_info_pop_o,
  output logic        orig_bank_req_data_pop_o,
  output logic        new_bank_req_data_pop_o,
  output logic [7:0]  orig_miss_req_bank_id_o,
  output logic [7:0]  new_miss_req_bank_id_o,
  output logic [3:0]  orig_miss_req_bank_source_o,
  output logic [3:0]  new_miss_req_bank_source_o,
  output logic        orig_miss_req_bank_wdata_o,
  output logic        new_miss_req_bank_wdata_o,
  output logic [4:0]  orig_miss_req_bank_opcode_o,
  output logic [4:0]  new_miss_req_bank_opcode_o,
  output logic [6:0]  orig_miss_req_bank_subopcode_o,
  output logic [6:0]  new_miss_req_bank_subopcode_o,
  output logic [39:0] orig_miss_req_bank_address_o,
  output logic [39:0] new_miss_req_bank_address_o,
  output logic [2:0]  orig_miss_req_bank_size_o,
  output logic [2:0]  new_miss_req_bank_size_o,
  output logic [3:0]  orig_miss_req_bank_qwen_o,
  output logic [3:0]  new_miss_req_bank_qwen_o,
  output logic [63:0] orig_miss_req_bank_data0_o,
  output logic [63:0] new_miss_req_bank_data0_o,
  output logic [63:0] orig_miss_req_bank_data1_o,
  output logic [63:0] new_miss_req_bank_data1_o,
  output logic [63:0] orig_miss_req_bank_data2_o,
  output logic [63:0] new_miss_req_bank_data2_o,
  output logic [63:0] orig_miss_req_bank_data3_o,
  output logic [63:0] new_miss_req_bank_data3_o,
  output logic [63:0] orig_miss_req_bank_data4_o,
  output logic [63:0] new_miss_req_bank_data4_o,
  output logic [63:0] orig_miss_req_bank_data5_o,
  output logic [63:0] new_miss_req_bank_data5_o,
  output logic [63:0] orig_miss_req_bank_data6_o,
  output logic [63:0] new_miss_req_bank_data6_o,
  output logic [63:0] orig_miss_req_bank_data7_o,
  output logic [63:0] new_miss_req_bank_data7_o,
  output logic        orig_miss_req_bank_valid_o,
  output logic        new_miss_req_bank_valid_o
);

  logic [71:0] orig_bank_req_info;
  logic [255:0] bank_req_data;
  logic [583:0] orig_miss_req_bank_info;
  etlink_pkg::req_nodata_t new_bank_req_info;
  etlink_pkg::req_t new_miss_req_bank_info;

  assign orig_bank_req_info = {bank_req_id_i,
                               bank_req_source_i,
                               bank_req_wdata_i,
                               bank_req_opcode_i,
                               bank_req_address_i,
                               bank_req_size_i,
                               bank_req_qwen_i,
                               bank_req_subopcode_i};
  assign bank_req_data = {bank_req_data3_i, bank_req_data2_i,
                          bank_req_data1_i, bank_req_data0_i};

  always_comb begin
    new_bank_req_info.id = bank_req_id_i;
    new_bank_req_info.source = bank_req_source_i;
    new_bank_req_info.wdata = bank_req_wdata_i;
    new_bank_req_info.opcode = etlink_pkg::req_opcode_e'(bank_req_opcode_i);
    new_bank_req_info.address = bank_req_address_i;
    new_bank_req_info.size = etlink_pkg::size_e'(bank_req_size_i);
    new_bank_req_info.qwen = bank_req_qwen_i;
    new_bank_req_info.subopcode = bank_req_subopcode_i;
  end

  assign orig_miss_req_bank_id_o = orig_miss_req_bank_info[583:576];
  assign orig_miss_req_bank_source_o = orig_miss_req_bank_info[575:572];
  assign orig_miss_req_bank_wdata_o = orig_miss_req_bank_info[571];
  assign orig_miss_req_bank_opcode_o = orig_miss_req_bank_info[570:566];
  assign orig_miss_req_bank_address_o = orig_miss_req_bank_info[565:526];
  assign {orig_miss_req_bank_data7_o, orig_miss_req_bank_data6_o,
          orig_miss_req_bank_data5_o, orig_miss_req_bank_data4_o,
          orig_miss_req_bank_data3_o, orig_miss_req_bank_data2_o,
          orig_miss_req_bank_data1_o, orig_miss_req_bank_data0_o} =
      orig_miss_req_bank_info[525:14];
  assign orig_miss_req_bank_size_o = orig_miss_req_bank_info[13:11];
  assign orig_miss_req_bank_qwen_o = orig_miss_req_bank_info[10:7];
  assign orig_miss_req_bank_subopcode_o = orig_miss_req_bank_info[6:0];

  assign new_miss_req_bank_id_o = new_miss_req_bank_info.id;
  assign new_miss_req_bank_source_o = new_miss_req_bank_info.source;
  assign new_miss_req_bank_wdata_o = new_miss_req_bank_info.wdata;
  assign new_miss_req_bank_opcode_o = new_miss_req_bank_info.opcode;
  assign new_miss_req_bank_address_o = new_miss_req_bank_info.address;
  assign {new_miss_req_bank_data7_o, new_miss_req_bank_data6_o,
          new_miss_req_bank_data5_o, new_miss_req_bank_data4_o,
          new_miss_req_bank_data3_o, new_miss_req_bank_data2_o,
          new_miss_req_bank_data1_o, new_miss_req_bank_data0_o} =
      new_miss_req_bank_info.data;
  assign new_miss_req_bank_size_o = new_miss_req_bank_info.size;
  assign new_miss_req_bank_qwen_o = new_miss_req_bank_info.qwen;
  assign new_miss_req_bank_subopcode_o = new_miss_req_bank_info.subopcode;

  neigh_hv_logic_l2_bank u_new (
    .clk_shire_i                    (clk_i),
    .rst_shire_ndm_ni               (rst_ni),
    .bank_req_out_is_first_half_i   (bank_req_out_is_first_half_i),
    .bank_req_info_out_i            (new_bank_req_info),
    .bank_req_data_out_i            (bank_req_data),
    .bank_req_info_valid_i          (bank_req_info_valid_i),
    .bank_req_info_pop_o            (new_bank_req_info_pop_o),
    .bank_req_data_pop_o            (new_bank_req_data_pop_o),
    .miss_req_bank_info_o           (new_miss_req_bank_info),
    .miss_req_bank_valid_o          (new_miss_req_bank_valid_o),
    .miss_req_bank_ready_i          (miss_req_bank_ready_i)
  );

  neigh_hv_logic_l2_bank_orig u_orig (
    .clock_shire                  (clk_i),
    .reset_shire_ndm              (!rst_ni),
    .bank_req_out_is_first_half   (bank_req_out_is_first_half_i),
    .bank_req_info_out            (orig_bank_req_info),
    .bank_req_data_out            (bank_req_data),
    .bank_req_info_valid          (bank_req_info_valid_i),
    .bank_req_info_pop            (orig_bank_req_info_pop_o),
    .bank_req_data_pop            (orig_bank_req_data_pop_o),
    .miss_req_bank_info           (orig_miss_req_bank_info),
    .miss_req_bank_valid          (orig_miss_req_bank_valid_o),
    .miss_req_bank_ready          (miss_req_bank_ready_i)
  );

endmodule : cosim_neigh_hv_logic_l2_bank_tb
