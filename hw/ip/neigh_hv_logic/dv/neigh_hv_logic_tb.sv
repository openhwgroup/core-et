// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module neigh_hv_logic_tb (
  input  logic clk_i,
  input  logic rst_ni,

  input  logic        apb_pready_neigh_i,
  input  logic        apb_req_psel_i,
  input  logic        apb_req_penable_i,
  output logic        apb_req_push_o,

  input  logic        apb_rsp_valid_i,
  input  logic [63:0] apb_rsp_pop_prdata_i,
  input  logic        apb_rsp_pop_pready_i,
  input  logic        apb_rsp_pop_pslverr_i,
  output logic [63:0] apb_rsp_sync_prdata_o,
  output logic        apb_rsp_sync_pready_o,
  output logic        apb_rsp_sync_pslverr_o,

  input  logic [3:0]  bpam_run_control_i,
  input  logic        dmctrl_write_enable_i,
  output logic        bpam_push_o,

  input  logic        bpam_ack_halt_i,
  input  logic        bpam_ack_resume_i,
  output logic        bpam_ack_halt_o,
  output logic        bpam_ack_resume_o,

  input  logic        clk_shire_fb_i,
  input  logic        clk_neigh_fb_i,
  output logic        dll_feedback_shire_o,
  output logic        dll_feedback_neigh_o,

  output logic        ct_one_o,

  input  logic        icache_err_datain_i,
  output logic        icache_err_dataout_o,

  input  logic [neigh_hv_logic_pkg::ThreadsPerN-1:0] ipi_trigger_i,
  output logic        ipi_trigger_push_o,

  input  logic        dft_tdr_ctrl_i,
  input  logic        pwr_ctrl_glb_isolate_i,
  output logic        pwr_ctrl_glb_isolate_o,

  input  logic [neigh_hv_logic_pkg::MinPerN-1:0] uc_to_neigh_fcc_i,
  output logic        neigh_uc_fcc_push_o,

  input  logic        l2_bank_first_half_i,
  input  logic [7:0]  l2_bank_id_i,
  input  logic [3:0]  l2_bank_source_i,
  input  logic        l2_bank_wdata_i,
  input  logic [4:0]  l2_bank_opcode_i,
  input  logic [6:0]  l2_bank_subopcode_i,
  input  logic [39:0] l2_bank_address_i,
  input  logic [2:0]  l2_bank_size_i,
  input  logic [3:0]  l2_bank_qwen_i,
  input  logic [63:0] l2_bank_data0_i,
  input  logic [63:0] l2_bank_data1_i,
  input  logic [63:0] l2_bank_data2_i,
  input  logic [63:0] l2_bank_data3_i,
  input  logic        l2_bank_valid_i,
  input  logic        l2_bank_ready_i,
  output logic        l2_bank_info_pop_o,
  output logic        l2_bank_data_pop_o,
  output logic [7:0]  l2_miss_id_o,
  output logic [3:0]  l2_miss_source_o,
  output logic        l2_miss_wdata_o,
  output logic [4:0]  l2_miss_opcode_o,
  output logic [6:0]  l2_miss_subopcode_o,
  output logic [39:0] l2_miss_address_o,
  output logic [2:0]  l2_miss_size_o,
  output logic [3:0]  l2_miss_qwen_o,
  output logic [63:0] l2_miss_data0_o,
  output logic [63:0] l2_miss_data1_o,
  output logic [63:0] l2_miss_data2_o,
  output logic [63:0] l2_miss_data3_o,
  output logic [63:0] l2_miss_data4_o,
  output logic [63:0] l2_miss_data5_o,
  output logic [63:0] l2_miss_data6_o,
  output logic [63:0] l2_miss_data7_o,
  output logic        l2_miss_valid_o,

  input  logic [neigh_hv_logic_pkg::ScBanks-1:0] sc_bank_valid_i,
  input  logic [neigh_hv_logic_pkg::ScBanks:0]   sc_req_ready_i,
  input  logic [7:0]  sc_bank0_id_i,
  input  logic [7:0]  sc_bank1_id_i,
  input  logic [7:0]  sc_bank2_id_i,
  input  logic [7:0]  sc_bank3_id_i,
  input  logic [63:0] sc_bank0_data0_i,
  input  logic [63:0] sc_bank1_data0_i,
  input  logic [63:0] sc_bank2_data0_i,
  input  logic [63:0] sc_bank3_data0_i,
  input  logic        sc_uc_valid_i,
  input  logic [7:0]  sc_uc_id_i,
  input  logic        sc_uc_wdata_i,
  input  logic [63:0] sc_uc_data0_i,
  output logic [neigh_hv_logic_pkg::ScBanks-1:0] sc_bank_ready_o,
  output logic        sc_uc_info_pop_o,
  output logic        sc_uc_data_pop_o,
  output logic [neigh_hv_logic_pkg::ScBanks:0] sc_req_valid_o,
  output logic [7:0]  sc_req_id_o,
  output logic [63:0] sc_req_data0_o,
  output logic [63:0] sc_req_data4_o
);

  neigh_hv_logic_pkg::apb_from_neigh_t apb_rsp_pop;
  neigh_hv_logic_pkg::apb_from_neigh_t apb_rsp_sync;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_ack_in;
  neigh_hv_logic_pkg::bpam_rc_tbox_ack_t bpam_ack_out;
  etlink_pkg::req_nodata_t l2_bank_info;
  etlink_pkg::req_t l2_miss_info;
  etlink_pkg::req_t [neigh_hv_logic_pkg::ScBanks-1:0] sc_bank_info;
  etlink_pkg::req_nodata_t sc_uc_info;
  /* verilator lint_off UNUSEDSIGNAL */  // Unit TB exposes only ID and data words needed by directed checks; the DUT still drives the full request struct.
  etlink_pkg::req_t sc_req_info;
  /* verilator lint_on UNUSEDSIGNAL */

  assign apb_rsp_pop.apb_prdata = apb_rsp_pop_prdata_i;
  assign apb_rsp_pop.apb_pready = apb_rsp_pop_pready_i;
  assign apb_rsp_pop.apb_pslverr = apb_rsp_pop_pslverr_i;
  assign apb_rsp_sync_prdata_o = apb_rsp_sync.apb_prdata;
  assign apb_rsp_sync_pready_o = apb_rsp_sync.apb_pready;
  assign apb_rsp_sync_pslverr_o = apb_rsp_sync.apb_pslverr;

  assign bpam_ack_in.halt_ack = bpam_ack_halt_i;
  assign bpam_ack_in.resume_ack = bpam_ack_resume_i;
  assign bpam_ack_halt_o = bpam_ack_out.halt_ack;
  assign bpam_ack_resume_o = bpam_ack_out.resume_ack;

  always_comb begin
    l2_bank_info.id = l2_bank_id_i;
    l2_bank_info.source = l2_bank_source_i;
    l2_bank_info.wdata = l2_bank_wdata_i;
    l2_bank_info.opcode = etlink_pkg::req_opcode_e'(l2_bank_opcode_i);
    l2_bank_info.subopcode = l2_bank_subopcode_i;
    l2_bank_info.address = l2_bank_address_i;
    l2_bank_info.size = etlink_pkg::size_e'(l2_bank_size_i);
    l2_bank_info.qwen = l2_bank_qwen_i;
  end

  assign l2_miss_id_o = l2_miss_info.id;
  assign l2_miss_source_o = l2_miss_info.source;
  assign l2_miss_wdata_o = l2_miss_info.wdata;
  assign l2_miss_opcode_o = l2_miss_info.opcode;
  assign l2_miss_subopcode_o = l2_miss_info.subopcode;
  assign l2_miss_address_o = l2_miss_info.address;
  assign l2_miss_size_o = l2_miss_info.size;
  assign l2_miss_qwen_o = l2_miss_info.qwen;
  assign {l2_miss_data7_o, l2_miss_data6_o, l2_miss_data5_o, l2_miss_data4_o,
          l2_miss_data3_o, l2_miss_data2_o, l2_miss_data1_o, l2_miss_data0_o} = l2_miss_info.data;

  always_comb begin
    sc_bank_info = '0;
    sc_bank_info[0].id = sc_bank0_id_i;
    sc_bank_info[0].data[63:0] = sc_bank0_data0_i;
    sc_bank_info[1].id = sc_bank1_id_i;
    sc_bank_info[1].data[63:0] = sc_bank1_data0_i;
    sc_bank_info[2].id = sc_bank2_id_i;
    sc_bank_info[2].data[63:0] = sc_bank2_data0_i;
    sc_bank_info[3].id = sc_bank3_id_i;
    sc_bank_info[3].data[63:0] = sc_bank3_data0_i;
    for (int unsigned i = 0; i < neigh_hv_logic_pkg::ScBanks; i++) begin
      sc_bank_info[i].source = i[etlink_pkg::SourceSize-1:0];
      sc_bank_info[i].wdata = 1'b1;
      sc_bank_info[i].opcode = etlink_pkg::ReqWrite;
      sc_bank_info[i].address = {32'h0, sc_bank_info[i].id};
      sc_bank_info[i].size = etlink_pkg::SizeLine;
      sc_bank_info[i].qwen = 4'hf;
      sc_bank_info[i].subopcode = 7'h55;
    end

    sc_uc_info.id = sc_uc_id_i;
    sc_uc_info.source = 4'hd;
    sc_uc_info.wdata = sc_uc_wdata_i;
    sc_uc_info.opcode = etlink_pkg::ReqRead;
    sc_uc_info.address = {32'h0, sc_uc_id_i};
    sc_uc_info.size = etlink_pkg::SizeHLine;
    sc_uc_info.qwen = 4'h3;
    sc_uc_info.subopcode = 7'h2a;
  end

  assign sc_req_id_o = sc_req_info.id;
  assign sc_req_data0_o = sc_req_info.data[63:0];
  assign sc_req_data4_o = sc_req_info.data[319:256];

  neigh_hv_logic_apb_req u_apb_req (
    .rst_push_ni                 (rst_ni),
    .clk_push_i                  (clk_i),
    .apb_pready_neigh_i          (apb_pready_neigh_i),
    .apb_esr_req_apb_psel_i      (apb_req_psel_i),
    .apb_esr_req_apb_penable_i   (apb_req_penable_i),
    .apb_esr_req_vcfifo_push_o   (apb_req_push_o)
  );

  neigh_hv_logic_apb_rsp u_apb_rsp (
    .apb_esr_rsp_vcfifo_valid_i (apb_rsp_valid_i),
    .apb_esr_rsp_pop_i          (apb_rsp_pop),
    .apb_esr_rsp_sync_o         (apb_rsp_sync)
  );

  neigh_hv_logic_bpam u_bpam (
    .rst_push_ni                      (rst_ni),
    .clk_push_i                       (clk_i),
    .bpam_run_control_i               (bpam_run_control_i),
    .dmctrl_write_enable_i            (dmctrl_write_enable_i),
    .bpam_run_control_vcfifo_push_o   (bpam_push_o)
  );

  neigh_hv_logic_bpam_rc_tbox_ack u_bpam_ack (
    .clk_hv_i                      (clk_i),
    .rst_hv_ni                     (rst_ni),
    .bpam_rc_tbox_ack_hi_reg_i     (bpam_ack_in),
    .bpam_rc_tbox_ack_hi_reg_o     (bpam_ack_out)
  );

  neigh_hv_logic_clock_feedback u_clock_feedback (
    .clk_shire_i           (clk_shire_fb_i),
    .clk_neigh_i           (clk_neigh_fb_i),
    .dll_feedback_shire_o  (dll_feedback_shire_o),
    .dll_feedback_neigh_o  (dll_feedback_neigh_o)
  );

  neigh_hv_logic_constants u_constants (
    .ct_one_o (ct_one_o)
  );

  neigh_hv_logic_icache_err u_icache_err (
    .rst_pop_ni    (rst_ni),
    .clk_pop_i     (clk_i),
    .ff_datain_i   (icache_err_datain_i),
    .ff_dataout_o  (icache_err_dataout_o)
  );

  neigh_hv_logic_ipi u_ipi (
    .esr_to_neigh_ipi_trigger_i       (ipi_trigger_i),
    .esr_to_neigh_ipi_trigger_push_o  (ipi_trigger_push_o)
  );

  neigh_hv_logic_pwr_ctrl_tdr_isolate u_pwr_ctrl (
    .dft_tdr_ctrl_i           (dft_tdr_ctrl_i),
    .pwr_ctrl_glb_isolate_i   (pwr_ctrl_glb_isolate_i),
    .pwr_ctrl_glb_isolate_o   (pwr_ctrl_glb_isolate_o)
  );

  neigh_hv_logic_uc_fcc u_uc_fcc (
    .uc_to_neigh_fcc_i             (uc_to_neigh_fcc_i),
    .neigh_uc_fcc_vcfifo_push_o    (neigh_uc_fcc_push_o)
  );

  neigh_hv_logic_l2_bank u_l2_bank (
    .clk_shire_i                    (clk_i),
    .rst_shire_ndm_ni               (rst_ni),
    .bank_req_out_is_first_half_i   (l2_bank_first_half_i),
    .bank_req_info_out_i            (l2_bank_info),
    .bank_req_data_out_i            ({l2_bank_data3_i, l2_bank_data2_i,
                                      l2_bank_data1_i, l2_bank_data0_i}),
    .bank_req_info_valid_i          (l2_bank_valid_i),
    .bank_req_info_pop_o            (l2_bank_info_pop_o),
    .bank_req_data_pop_o            (l2_bank_data_pop_o),
    .miss_req_bank_info_o           (l2_miss_info),
    .miss_req_bank_valid_o          (l2_miss_valid_o),
    .miss_req_bank_ready_i          (l2_bank_ready_i)
  );

  neigh_hv_logic_neigh_sc u_neigh_sc (
    .clk_shire_i                (clk_i),
    .rst_shire_ndm_ni           (rst_ni),
    .miss_req_bank_info_i       (sc_bank_info),
    .miss_req_bank_valid_i      (sc_bank_valid_i),
    .miss_req_bank_ready_o      (sc_bank_ready_o),
    .uc_req_info_out_i          (sc_uc_info),
    .uc_req_data_out_i          ({sc_uc_data0_i, sc_uc_data0_i,
                                  sc_uc_data0_i, sc_uc_data0_i}),
    .uc_req_info_pop_valid_i    (sc_uc_valid_i),
    .uc_req_info_pop_o          (sc_uc_info_pop_o),
    .uc_req_data_pop_o          (sc_uc_data_pop_o),
    .neigh_sc_req_info_o        (sc_req_info),
    .neigh_sc_req_valid_o       (sc_req_valid_o),
    .neigh_sc_req_ready_i       (sc_req_ready_i)
  );

endmodule : neigh_hv_logic_tb
