// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module esr_neigh #(
  parameter int unsigned AdWidth = esr_pkg::EsrNeighApbPpAddrWidth,
  /* verilator lint_off UNUSEDPARAM */  // Preserved original geometry parameters; this ESR block only exposes the register interface.
  parameter int unsigned SetsPerSubBank = shirecache_pkg::SetsPerSubBank,
  parameter int unsigned NumReqqEntries = shirecache_pkg::ReqqDepth
  /* verilator lint_on UNUSEDPARAM */
) (
  input  logic                                      clk_i,
  input  logic                                      rst_c_ni,
  input  logic                                      rst_w_ni,
  input  logic                                      rst_d_ni,
  /* verilator lint_off UNUSEDSIGNAL */  // Original test_en only forced the generated APB write-preview gate; the named write-preview primitive has no separate test-enable port.
  input  logic                                      test_en_i,
  /* verilator lint_on UNUSEDSIGNAL */
  input  dft_pkg::dft_t                             dft_i,

  output logic [minion_pkg::VaSize-1:0]             esr_minion_boot_o,
  output esr_pkg::esr_mprot_t                       esr_mprot_o,
  output minion_pkg::tlb_entry_type                 esr_vmspagesize_o,
  output esr_pkg::esr_ipi_pc_t                      esr_ipi_redirect_pc_o,
  output esr_pkg::esr_hactrl_t                      esr_hactrl_o,
  input  esr_pkg::esr_hastatus0_t                   esr_hastatus0_i,
  input  esr_pkg::esr_and_or_tree_l0_t              esr_and_or_tree_l0_i,
  output esr_pkg::esr_pmu_ctrl_t                    esr_pmu_ctrl_o,
  output esr_pkg::esr_neigh_chicken_t               esr_neigh_chicken_o,
  output icache_pkg::esr_icache_err_log_ctl_t       esr_icache_err_log_ctl_o,
  output esr_pkg::esr_tex_ctl_t                     esr_texture_control_o,
  input  esr_pkg::esr_tex_status_t                  esr_texture_status_i,
  output logic [47:0]                               esr_texture_image_table_ptr_o,

  input  logic                                      esr_icache_err_log_sbe_i,
  input  logic                                      esr_icache_err_log_dbe_i,
  input  icache_pkg::icache_err_log_info_t          esr_icache_err_log_info_i,
  output logic                                      esr_icache_err_detected_o,
  output logic                                      esr_icache_err_logged_o,
  output logic                                      esr_icache_ecc_count_ov_o,

  input  esr_pkg::esr_hastatus1_t                   esr_hastatus1_min_i,
  input  esr_pkg::esr_ms_dmctrl_t                   esr_dmctrl_i,

  input  logic [AdWidth-1:0]                        apb_paddr_i,
  input  logic                                      apb_pwrite_i,
  input  logic                                      apb_psel_i,
  input  logic                                      apb_penable_i,
  input  logic [esr_pkg::EsrApbDataWidth-1:0]       apb_pwdata_i,
  output logic                                      apb_pready_o,
  output logic [esr_pkg::EsrApbDataWidth-1:0]       apb_prdata_o,
  output logic                                      apb_pslverr_o
);
  import esr_pkg::*;

  localparam int unsigned IcacheErrAddressWidth =
      minion_pkg::PaSize - icache_geom_pkg::IcacheBlockAddrSize;
  localparam int unsigned IcacheCountsWidth = EsrIcacheSbeCntSize + EsrIcacheDbeCntSize;

  localparam logic [AdWidth-1:0] AddrMinionBoot = EsrNeighAddrMinionBoot[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrMprot = EsrNeighAddrMprot[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrVmspagesize = EsrNeighAddrVmspagesize[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIpiRedirectPc = EsrNeighAddrIpiRedirectPc[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrHactrl = EsrNeighAddrHactrl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrHastatus0 = EsrNeighAddrHastatus0[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrHastatus1 = EsrNeighAddrHastatus1[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrAndOrTreeL0 = EsrNeighAddrAndOrTreeL0[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrPmuCtrl = EsrNeighAddrPmuCtrl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrNeighChicken = EsrNeighAddrNeighChicken[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIcacheErrLogCtl = EsrNeighAddrIcacheErrLogCtl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIcacheErrLogInfo = EsrNeighAddrIcacheErrLogInfo[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIcacheErrLogAddress = EsrNeighAddrIcacheErrLogAddress[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrIcacheSbeDbeCounts = EsrNeighAddrIcacheSbeDbeCounts[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrTextureControl = EsrNeighAddrTextureControl[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrTextureStatus = EsrNeighAddrTextureStatus[AdWidth-1:0];
  localparam logic [AdWidth-1:0] AddrTextureImageTablePtr = EsrNeighAddrTextureImageTablePtr[AdWidth-1:0];

  logic                                s0_en;
  logic                                s0_addr_valid;
  logic [EsrApbDataWidth-1:0]          s0_rdata;
  logic                                s1_en;
  logic                                s1_addr_valid_q;
  logic [EsrApbDataWidth-1:0]          s1_rdata_q;
  logic [EsrApbDataWidth-1:0]          s1_wdata_q;

  logic                                s0_wen_p2;
  logic [EsrApbDataWidth-1:0]          apb_pwdata_1p;

  logic                                rst_c_sync_n;
  logic                                rst_w_sync_n;
  logic                                reset_c_reg;
  logic                                reset_w_reg;

  logic [minion_pkg::VaSize-1:0]       reg_minion_boot_q;
  logic [minion_pkg::VaSize-1:0]       reg_minion_boot_d;
  logic                                reg_minion_boot_en_q;
  logic                                reg_minion_boot_en_d;
  logic                                reg_minion_boot_latch_en;

  esr_mprot_t                          reg_mprot_q;
  esr_mprot_t                          reg_mprot_d;
  logic                                reg_mprot_en_q;
  logic                                reg_mprot_en_d;
  logic                                reg_mprot_latch_en;

  minion_pkg::tlb_entry_type           reg_vmspagesize_q;
  minion_pkg::tlb_entry_type           reg_vmspagesize_d;
  logic                                reg_vmspagesize_en_q;
  logic                                reg_vmspagesize_en_d;
  logic                                reg_vmspagesize_latch_en;

  esr_ipi_pc_t                         reg_ipi_redirect_pc_q;
  esr_ipi_pc_t                         reg_ipi_redirect_pc_d;
  logic                                reg_ipi_redirect_pc_en_q;
  logic                                reg_ipi_redirect_pc_en_d;

  esr_hactrl_t                         reg_hactrl_q;
  esr_hactrl_t                         reg_hactrl_d;
  logic                                reg_hactrl_en_q;
  logic                                reg_hactrl_en_d;

  esr_hastatus1_t                      reg_hastatus1_q;
  esr_hastatus1_t                      reg_hastatus1_d;
  logic                                reg_hastatus1_en_q;
  logic                                reg_hastatus1_en_d;
  esr_hastatus1_t                      read_hastatus1;

  esr_pmu_ctrl_t                       reg_pmu_ctrl_q;
  esr_pmu_ctrl_t                       reg_pmu_ctrl_d;
  logic                                reg_pmu_ctrl_en_q;
  logic                                reg_pmu_ctrl_en_d;

  esr_neigh_chicken_t                  reg_neigh_chicken_q;
  esr_neigh_chicken_t                  reg_neigh_chicken_d;
  logic                                reg_neigh_chicken_en_q;
  logic                                reg_neigh_chicken_en_d;

  icache_pkg::esr_icache_err_log_ctl_t reg_icache_err_log_ctl_q;
  icache_pkg::esr_icache_err_log_ctl_t reg_icache_err_log_ctl_d;
  logic                                reg_icache_err_log_ctl_en_q;
  logic                                reg_icache_err_log_ctl_en_d;
  logic                                reg_icache_err_log_ctl_latch_en;

  logic                                reg_icache_err_log_info_en_q;
  logic                                reg_icache_err_log_info_en_d;
  logic                                reg_icache_sbe_dbe_counts_en_q;
  logic                                reg_icache_sbe_dbe_counts_en_d;

  esr_tex_ctl_t                        reg_texture_control_q;
  esr_tex_ctl_t                        reg_texture_control_d;
  logic                                reg_texture_control_en_q;
  logic                                reg_texture_control_en_d;

  logic [47:0]                         reg_texture_image_table_ptr_q;
  logic [47:0]                         reg_texture_image_table_ptr_d;
  logic                                reg_texture_image_table_ptr_en_q;
  logic                                reg_texture_image_table_ptr_en_d;

  icache_pkg::esr_icache_err_log_info_t reg_icache_err_log_info_q;
  icache_pkg::esr_icache_err_log_info_t reg_icache_err_log_info_d;
  icache_pkg::esr_icache_err_log_info_t icache_err_log_info_wdata;
  logic [IcacheErrAddressWidth-1:0]     reg_icache_err_log_address_q;
  logic [IcacheErrAddressWidth-1:0]     reg_icache_err_log_address_d;
  logic                                 icache_err_log_info_clr;
  logic                                 icache_err_log_info_new_err;
  icache_pkg::icache_err_code_e         icache_err_log_info_new_err_code;
  logic                                 icache_err_log_info_wr_en;
  logic                                 icache_err_log_ecc_count_ov;

  logic [EsrIcacheSbeCntSize-1:0]       reg_icache_sbe_counter_q;
  logic [EsrIcacheSbeCntSize-1:0]       reg_icache_sbe_counter_d;
  logic                                 icache_sbe_counter_ov;
  logic [EsrIcacheSbeCntSize-1:0]       icache_sbe_counter_wdata;
  logic                                 icache_sbe_counter_clr;
  logic                                 icache_sbe_counter_en;

  logic [EsrIcacheDbeCntSize-1:0]       reg_icache_dbe_counter_q;
  logic [EsrIcacheDbeCntSize-1:0]       reg_icache_dbe_counter_d;
  logic                                 icache_dbe_counter_ov;
  logic [EsrIcacheDbeCntSize-1:0]       icache_dbe_counter_wdata;
  logic                                 icache_dbe_counter_clr;
  logic                                 icache_dbe_counter_en;
  logic [IcacheCountsWidth-1:0]         reg_icache_sbe_dbe_counters;

  logic                                 icache_err_detected_d;
  logic                                 icache_err_logged_d;

  function automatic esr_ipi_pc_t write_ipi_pc(input logic [46:0] address_msb);
    esr_ipi_pc_t ret;
    begin
      ret = '0;
      ret.address_msb = address_msb;
      return ret;
    end
  endfunction

  function automatic esr_hactrl_t write_hactrl(input logic [31:0] d);
    esr_hactrl_t ret;
    begin
      ret = '0;
      ret.hawindow = d[15:0];
      ret.hartmask = d[31:16];
      return ret;
    end
  endfunction

  function automatic esr_hastatus1_t write_hastatus1(input logic [31:0] d);
    esr_hastatus1_t ret;
    begin
      ret = '0;
      ret.exception = d[15:0];
      ret.error = d[31:16];
      return ret;
    end
  endfunction

  prim_rst_sync u_rst_c_sync (
    .clk_i  (clk_i),
    .rst_ni (rst_c_ni),
    .dft_i  (dft_i),
    .rst_no (rst_c_sync_n)
  );

  prim_rst_sync u_rst_w_sync (
    .clk_i  (clk_i),
    .rst_ni (rst_w_ni),
    .dft_i  (dft_i),
    .rst_no (rst_w_sync_n)
  );

  assign reset_c_reg = !rst_c_sync_n;
  assign reset_w_reg = !rst_w_sync_n;

  prim_write_preview_en #(
    .Width (1)
  ) u_s0_wen_p2 (
    .clk_i (clk_i),
    .en_i  (1'b1),
    .d_i   (s0_en && apb_pwrite_i),
    .q_o   (s0_wen_p2)
  );

  prim_write_preview_en #(
    .Width (EsrApbDataWidth)
  ) u_apb_pwdata_1p (
    .clk_i (clk_i),
    .en_i  (s0_wen_p2),
    .d_i   (apb_pwdata_i),
    .q_o   (apb_pwdata_1p)
  );

  always_comb begin
    read_hastatus1 = reg_hastatus1_q;
    read_hastatus1.busy = esr_hastatus1_min_i.busy;
  end

  always_comb begin
    s0_en = apb_psel_i && !apb_penable_i;
    s0_addr_valid = 1'b1;
    s0_rdata = '0;

    reg_minion_boot_en_d = 1'b0;
    reg_mprot_en_d = 1'b0;
    reg_vmspagesize_en_d = 1'b0;
    reg_ipi_redirect_pc_en_d = 1'b0;
    reg_hactrl_en_d = 1'b0;
    reg_hastatus1_en_d = 1'b0;
    reg_pmu_ctrl_en_d = 1'b0;
    reg_neigh_chicken_en_d = 1'b0;
    reg_icache_err_log_ctl_en_d = 1'b0;
    reg_icache_err_log_info_en_d = 1'b0;
    reg_icache_sbe_dbe_counts_en_d = 1'b0;
    reg_texture_control_en_d = 1'b0;
    reg_texture_image_table_ptr_en_d = 1'b0;

    if (s0_en) begin
      unique case (apb_paddr_i)
        AddrMinionBoot: begin
          reg_minion_boot_en_d = apb_pwrite_i;
          s0_rdata[minion_pkg::VaSize-1:0] = reg_minion_boot_q;
        end
        AddrMprot: begin
          reg_mprot_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_mprot_t)-1:0] = reg_mprot_q;
        end
        AddrVmspagesize: begin
          reg_vmspagesize_en_d = apb_pwrite_i;
          s0_rdata[$bits(minion_pkg::tlb_entry_type)-1:0] = reg_vmspagesize_q;
        end
        AddrIpiRedirectPc: begin
          reg_ipi_redirect_pc_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_ipi_pc_t)-1:0] = reg_ipi_redirect_pc_q;
        end
        AddrHactrl: begin
          reg_hactrl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_hactrl_t)-1:0] = reg_hactrl_q;
        end
        AddrHastatus0: begin
          s0_rdata = esr_hastatus0_i;
        end
        AddrHastatus1: begin
          reg_hastatus1_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_hastatus1_t)-1:0] = read_hastatus1;
        end
        AddrAndOrTreeL0: begin
          s0_rdata[$bits(esr_and_or_tree_l0_t)-1:0] = esr_and_or_tree_l0_i;
        end
        AddrPmuCtrl: begin
          reg_pmu_ctrl_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_pmu_ctrl_t)-1:0] = reg_pmu_ctrl_q;
        end
        AddrNeighChicken: begin
          reg_neigh_chicken_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_neigh_chicken_t)-1:0] = reg_neigh_chicken_q;
        end
        AddrIcacheErrLogCtl: begin
          reg_icache_err_log_ctl_en_d = apb_pwrite_i;
          s0_rdata[$bits(icache_pkg::esr_icache_err_log_ctl_t)-1:0] = reg_icache_err_log_ctl_q;
        end
        AddrIcacheErrLogInfo: begin
          reg_icache_err_log_info_en_d = apb_pwrite_i;
          s0_rdata[$bits(icache_pkg::esr_icache_err_log_info_t)-1:0] = reg_icache_err_log_info_q;
        end
        AddrIcacheErrLogAddress: begin
          s0_rdata[IcacheErrAddressWidth-1:0] = reg_icache_err_log_address_q;
        end
        AddrIcacheSbeDbeCounts: begin
          reg_icache_sbe_dbe_counts_en_d = apb_pwrite_i;
          s0_rdata[IcacheCountsWidth-1:0] = reg_icache_sbe_dbe_counters;
        end
        AddrTextureControl: begin
          reg_texture_control_en_d = apb_pwrite_i;
          s0_rdata[$bits(esr_tex_ctl_t)-1:0] = reg_texture_control_q;
        end
        AddrTextureStatus: begin
          s0_rdata[$bits(esr_tex_status_t)-1:0] = esr_texture_status_i;
        end
        AddrTextureImageTablePtr: begin
          reg_texture_image_table_ptr_en_d = apb_pwrite_i;
          s0_rdata[47:0] = reg_texture_image_table_ptr_q;
        end
        default: begin
          s0_addr_valid = 1'b0;
        end
      endcase
    end
  end

  assign s1_en = apb_psel_i && apb_penable_i;

  always_comb begin
    reg_minion_boot_d = apb_pwdata_1p[minion_pkg::VaSize-1:0];
    reg_mprot_d = apb_pwdata_1p[$bits(esr_mprot_t)-1:0];
    reg_vmspagesize_d = minion_pkg::tlb_entry_type_4K;
    reg_vmspagesize_d = minion_pkg::tlb_entry_type'(apb_pwdata_1p[$bits(minion_pkg::tlb_entry_type)-1:0]);
    reg_icache_err_log_ctl_d = apb_pwdata_1p[$bits(icache_pkg::esr_icache_err_log_ctl_t)-1:0];

    reg_minion_boot_latch_en = reg_minion_boot_en_q;
    reg_mprot_latch_en = reg_mprot_en_q;
    reg_vmspagesize_latch_en = reg_vmspagesize_en_q;
    reg_icache_err_log_ctl_latch_en = reg_icache_err_log_ctl_en_q;

    reg_ipi_redirect_pc_d = reg_ipi_redirect_pc_q;
    reg_hactrl_d = reg_hactrl_q;
    reg_hastatus1_d = reg_hastatus1_q;
    reg_pmu_ctrl_d = reg_pmu_ctrl_q;
    reg_neigh_chicken_d = reg_neigh_chicken_q;
    reg_texture_control_d = reg_texture_control_q;
    reg_texture_image_table_ptr_d = reg_texture_image_table_ptr_q;

    if (reg_ipi_redirect_pc_en_q) begin
      reg_ipi_redirect_pc_d = write_ipi_pc(apb_pwdata_i[47:1]);
    end

    if (reg_hactrl_en_q) begin
      reg_hactrl_d = write_hactrl(apb_pwdata_i[31:0]);
    end

    if (reg_hastatus1_en_q) begin
      reg_hastatus1_d = write_hastatus1(apb_pwdata_i[47:16]);
    end

    if (reg_pmu_ctrl_en_q) begin
      reg_pmu_ctrl_d = apb_pwdata_i[$bits(esr_pmu_ctrl_t)-1:0];
    end

    if (reg_neigh_chicken_en_q) begin
      reg_neigh_chicken_d = apb_pwdata_i[$bits(esr_neigh_chicken_t)-1:0];
    end

    if (reg_texture_control_en_q) begin
      reg_texture_control_d = apb_pwdata_i[$bits(esr_tex_ctl_t)-1:0];
    end

    if (reg_texture_image_table_ptr_en_q) begin
      reg_texture_image_table_ptr_d = apb_pwdata_i[47:0];
    end

    reg_hastatus1_d.error |= esr_hastatus1_min_i.error;
    reg_hastatus1_d.exception |= esr_hastatus1_min_i.exception;

    for (int unsigned k = 0; k < EsrNeighThreads; k++) begin
      reg_hactrl_d.resethalt[k] = reg_hactrl_q.resethalt[k];
      if (esr_dmctrl_i.clresethaltreq && esr_dmctrl_i.write_enable &&
          ((esr_dmctrl_i.hasel && reg_hactrl_q.hawindow[k]) || reg_hactrl_q.hartmask[k])) begin
        reg_hactrl_d.resethalt[k] = 1'b0;
      end else if (esr_dmctrl_i.setresethaltreq && esr_dmctrl_i.write_enable &&
                   ((esr_dmctrl_i.hasel && reg_hactrl_q.hawindow[k]) || reg_hactrl_q.hartmask[k])) begin
        reg_hactrl_d.resethalt[k] = 1'b1;
      end
    end
  end

  assign icache_err_log_info_wdata = s1_wdata_q[$bits(icache_pkg::esr_icache_err_log_info_t)-1:0];
  assign icache_err_log_info_clr = reg_icache_err_log_info_en_q &&
      icache_err_log_info_wdata.valid &&
      (reg_icache_err_log_info_q.err_code == icache_err_log_info_wdata.err_code);
  assign icache_err_log_ecc_count_ov = icache_sbe_counter_ov | icache_dbe_counter_ov;
  assign icache_err_log_info_new_err =
      esr_icache_err_log_sbe_i | esr_icache_err_log_dbe_i | icache_err_log_ecc_count_ov;

  always_comb begin
    if (esr_icache_err_log_dbe_i) begin
      icache_err_log_info_new_err_code = icache_pkg::IcacheErrCodeDbe;
    end else if (icache_err_log_ecc_count_ov) begin
      icache_err_log_info_new_err_code = icache_pkg::IcacheErrCodeEccCntOv;
    end else begin
      icache_err_log_info_new_err_code = icache_pkg::IcacheErrCodeSbe;
    end
  end

  assign icache_err_log_info_wr_en = icache_err_log_info_clr | icache_err_log_info_new_err;

  always_comb begin
    reg_icache_err_log_info_d = reg_icache_err_log_info_q;
    reg_icache_err_log_address_d = reg_icache_err_log_address_q;

    if (icache_err_log_info_clr) begin
      reg_icache_err_log_info_d.enabled = 1'b0;
      reg_icache_err_log_info_d.multi = 1'b0;
      reg_icache_err_log_info_d.valid = 1'b0;
    end else if (icache_err_log_info_new_err) begin
      if (!reg_icache_err_log_info_q.valid ||
          (!reg_icache_err_log_info_q.enabled &&
           reg_icache_err_log_ctl_q.err_interrupt_enable[icache_err_log_info_new_err_code[1:0]])) begin
        reg_icache_err_log_address_d = esr_icache_err_log_info_i.address;
        reg_icache_err_log_info_d.dbe_ov = icache_dbe_counter_ov;
        reg_icache_err_log_info_d.err_bits = esr_icache_err_log_dbe_i
            ? esr_icache_err_log_info_i.dbe_per_block
            : esr_icache_err_log_info_i.sbe_per_block;
        reg_icache_err_log_info_d.way = esr_icache_err_log_info_i.way;
        reg_icache_err_log_info_d.set = esr_icache_err_log_info_i.set;
        reg_icache_err_log_info_d.err_code = icache_err_log_info_new_err_code;
        reg_icache_err_log_info_d.imprecise =
            reg_icache_err_log_ctl_q.err_interrupt_enable[icache_err_log_info_new_err_code[1:0]] &
            ~reg_icache_err_log_ctl_q.err_rsp_enable;
      end
      reg_icache_err_log_info_d.enabled |=
          reg_icache_err_log_ctl_q.err_interrupt_enable[icache_err_log_info_new_err_code[1:0]];
      reg_icache_err_log_info_d.multi |=
          reg_icache_err_log_info_q.valid & reg_icache_err_log_info_q.enabled &
          reg_icache_err_log_ctl_q.err_interrupt_enable[icache_err_log_info_new_err_code[1:0]];
      reg_icache_err_log_info_d.valid = 1'b1;
    end
  end

  assign icache_sbe_counter_ov = &reg_icache_sbe_counter_q;
  assign icache_dbe_counter_ov = &reg_icache_dbe_counter_q;
  assign icache_sbe_counter_wdata = s1_wdata_q[0 +: EsrIcacheSbeCntSize];
  assign icache_dbe_counter_wdata = s1_wdata_q[EsrIcacheSbeCntSize +: EsrIcacheDbeCntSize];
  assign icache_sbe_counter_clr = reg_icache_sbe_dbe_counts_en_q &&
      ((icache_sbe_counter_wdata == {EsrIcacheSbeCntSize{1'b1}}) ||
       (icache_sbe_counter_wdata == reg_icache_sbe_counter_q));
  assign icache_dbe_counter_clr = reg_icache_sbe_dbe_counts_en_q &&
      ((icache_dbe_counter_wdata == {EsrIcacheDbeCntSize{1'b1}}) ||
       (icache_dbe_counter_wdata == reg_icache_dbe_counter_q));
  assign icache_sbe_counter_en = icache_sbe_counter_clr ||
      (esr_icache_err_log_sbe_i && !icache_sbe_counter_ov);
  assign icache_dbe_counter_en = icache_dbe_counter_clr ||
      (esr_icache_err_log_dbe_i && !icache_dbe_counter_ov);

  always_comb begin
    reg_icache_sbe_counter_d = reg_icache_sbe_counter_q + {{(EsrIcacheSbeCntSize-1){1'b0}}, 1'b1};
    reg_icache_dbe_counter_d = reg_icache_dbe_counter_q + {{(EsrIcacheDbeCntSize-1){1'b0}}, 1'b1};

    if (icache_sbe_counter_clr) begin
      reg_icache_sbe_counter_d = '0;
    end
    if (icache_dbe_counter_clr) begin
      reg_icache_dbe_counter_d = '0;
    end
  end

  assign reg_icache_sbe_dbe_counters = {reg_icache_dbe_counter_q, reg_icache_sbe_counter_q};
  assign icache_err_detected_d = reg_icache_err_log_info_q.valid &
      reg_icache_err_log_info_q.enabled & reg_icache_err_log_info_q.imprecise;
  assign icache_err_logged_d = reg_icache_err_log_info_q.valid & reg_icache_err_log_info_q.enabled;
  assign esr_icache_ecc_count_ov_o = icache_err_log_ecc_count_ov &
      reg_icache_err_log_ctl_q.err_interrupt_enable[icache_pkg::IcacheErrCodeEccCntOv[1:0]] &
      reg_icache_err_log_ctl_q.err_rsp_enable;

  always_ff @(posedge clk_i) begin
    if (s0_en && apb_pwrite_i) begin
      s1_wdata_q <= apb_pwdata_i;
    end
    if (s0_en && !apb_pwrite_i) begin
      s1_rdata_q <= s0_rdata;
    end
  end

  always_ff @(posedge clk_i or negedge rst_c_ni) begin
    if (!rst_c_ni) begin
      s1_addr_valid_q <= 1'b0;
      reg_minion_boot_en_q <= 1'b0;
      reg_mprot_en_q <= 1'b0;
      reg_vmspagesize_en_q <= 1'b0;
      reg_ipi_redirect_pc_en_q <= 1'b0;
      reg_hactrl_en_q <= 1'b0;
      reg_hastatus1_en_q <= 1'b0;
      reg_pmu_ctrl_en_q <= 1'b0;
      reg_neigh_chicken_en_q <= 1'b0;
      reg_icache_err_log_ctl_en_q <= 1'b0;
      reg_icache_err_log_info_en_q <= 1'b0;
      reg_icache_sbe_dbe_counts_en_q <= 1'b0;
      reg_texture_control_en_q <= 1'b0;
      reg_texture_image_table_ptr_en_q <= 1'b0;
    end else begin
      if (s0_en) begin
        s1_addr_valid_q <= s0_addr_valid;
      end
      if (s0_en || s1_en) begin
        reg_minion_boot_en_q <= reg_minion_boot_en_d;
        reg_mprot_en_q <= reg_mprot_en_d;
        reg_vmspagesize_en_q <= reg_vmspagesize_en_d;
        reg_ipi_redirect_pc_en_q <= reg_ipi_redirect_pc_en_d;
        reg_hactrl_en_q <= reg_hactrl_en_d;
        reg_hastatus1_en_q <= reg_hastatus1_en_d;
        reg_pmu_ctrl_en_q <= reg_pmu_ctrl_en_d;
        reg_neigh_chicken_en_q <= reg_neigh_chicken_en_d;
        reg_icache_err_log_ctl_en_q <= reg_icache_err_log_ctl_en_d;
        reg_icache_err_log_info_en_q <= reg_icache_err_log_info_en_d;
        reg_icache_sbe_dbe_counts_en_q <= reg_icache_sbe_dbe_counts_en_d;
        reg_texture_control_en_q <= reg_texture_control_en_d;
        reg_texture_image_table_ptr_en_q <= reg_texture_image_table_ptr_en_d;
      end
    end
  end

  prim_write_commit_rst_en #(
    .Width      (minion_pkg::VaSize),
    .ResetValue (EsrNeighMinionBootResetVal)
  ) u_reg_minion_boot (
    .clk_i (clk_i),
    .rst_i (reset_c_reg),
    .en_i  (reg_minion_boot_latch_en),
    .d_i   (reg_minion_boot_d),
    .q_o   (reg_minion_boot_q)
  );

  prim_write_commit_rst_en #(
    .Width      ($bits(esr_mprot_t)),
    .ResetValue (EsrMprotResetVal)
  ) u_reg_mprot (
    .clk_i (clk_i),
    .rst_i (reset_c_reg),
    .en_i  (reg_mprot_latch_en),
    .d_i   (reg_mprot_d),
    .q_o   (reg_mprot_q)
  );

  prim_write_commit_rst_en #(
    .Width      ($bits(minion_pkg::tlb_entry_type)),
    .ResetValue (minion_pkg::tlb_entry_type_4K)
  ) u_reg_vmspagesize (
    .clk_i (clk_i),
    .rst_i (reset_w_reg),
    .en_i  (reg_vmspagesize_latch_en),
    .d_i   (reg_vmspagesize_d),
    .q_o   (reg_vmspagesize_q)
  );

  prim_write_commit_rst_en #(
    .Width      ($bits(icache_pkg::esr_icache_err_log_ctl_t)),
    .ResetValue (EsrIcacheErrLogCtlResetVal)
  ) u_reg_icache_err_log_ctl (
    .clk_i (clk_i),
    .rst_i (reset_c_reg),
    .en_i  (reg_icache_err_log_ctl_latch_en),
    .d_i   (reg_icache_err_log_ctl_d),
    .q_o   (reg_icache_err_log_ctl_q)
  );

  always_ff @(posedge clk_i or negedge rst_w_ni) begin
    if (!rst_w_ni) begin
      reg_ipi_redirect_pc_q <= '0;
      reg_pmu_ctrl_q <= '0;
      reg_texture_control_q <= EsrTextureCtlResetVal;
      reg_texture_image_table_ptr_q <= '0;
      reg_icache_err_log_info_q <= '0;
      reg_icache_err_log_address_q <= '0;
      reg_icache_sbe_counter_q <= '0;
      reg_icache_dbe_counter_q <= '0;
      esr_icache_err_detected_o <= 1'b0;
      esr_icache_err_logged_o <= 1'b0;
    end else begin
      reg_ipi_redirect_pc_q <= reg_ipi_redirect_pc_d;
      reg_pmu_ctrl_q <= reg_pmu_ctrl_d;
      reg_texture_control_q <= reg_texture_control_d;
      reg_texture_image_table_ptr_q <= reg_texture_image_table_ptr_d;
      if (icache_err_log_info_wr_en) begin
        reg_icache_err_log_info_q <= reg_icache_err_log_info_d;
        reg_icache_err_log_address_q <= reg_icache_err_log_address_d;
      end
      if (icache_sbe_counter_en) begin
        reg_icache_sbe_counter_q <= reg_icache_sbe_counter_d;
      end
      if (icache_dbe_counter_en) begin
        reg_icache_dbe_counter_q <= reg_icache_dbe_counter_d;
      end
      esr_icache_err_detected_o <= icache_err_detected_d;
      esr_icache_err_logged_o <= icache_err_logged_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_c_ni) begin
    if (!rst_c_ni) begin
      reg_neigh_chicken_q <= '0;
    end else begin
      reg_neigh_chicken_q <= reg_neigh_chicken_d;
    end
  end

  always_ff @(posedge clk_i or negedge rst_d_ni) begin
    if (!rst_d_ni) begin
      reg_hactrl_q <= '0;
      reg_hastatus1_q <= '0;
    end else begin
      reg_hactrl_q <= reg_hactrl_d;
      reg_hastatus1_q <= reg_hastatus1_d;
    end
  end

  assign apb_prdata_o = s1_rdata_q;
  assign apb_pslverr_o = !s1_addr_valid_q;
  assign apb_pready_o = 1'b1;

  assign esr_minion_boot_o = reg_minion_boot_q;
  assign esr_mprot_o = reg_mprot_q;
  assign esr_vmspagesize_o = reg_vmspagesize_q;
  assign esr_ipi_redirect_pc_o = reg_ipi_redirect_pc_q;
  assign esr_hactrl_o = reg_hactrl_q;
  assign esr_pmu_ctrl_o = reg_pmu_ctrl_q;
  assign esr_neigh_chicken_o = reg_neigh_chicken_q;
  assign esr_icache_err_log_ctl_o = reg_icache_err_log_ctl_q;
  assign esr_texture_control_o = reg_texture_control_q;
  assign esr_texture_image_table_ptr_o = reg_texture_image_table_ptr_q;

  logic unused_inputs;
  assign unused_inputs = &{
    1'b0,
    dft_i.sram_clk_override,
    dft_i.ram_rei,
    dft_i.ram_wei,
    s1_wdata_q[63:53],
    apb_pwdata_1p[63:48],
    icache_err_log_info_wdata[52:8],
    icache_err_log_info_wdata[3:1],
    esr_dmctrl_i.dmactive,
    esr_dmctrl_i.ndmreset,
    esr_dmctrl_i.ackhavereset,
    esr_dmctrl_i.hartreset,
    esr_dmctrl_i.resumereq,
    esr_dmctrl_i.haltreq
  };
endmodule
