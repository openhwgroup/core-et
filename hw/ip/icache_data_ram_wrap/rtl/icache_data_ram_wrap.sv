// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

module icache_data_ram_wrap #(
  parameter int unsigned Id = 0,
  parameter bit StandaloneMinion = 1'b0
) (
  input  logic                                           clk_i,
  input  logic                                           rst_ni,
  input  logic                                           dft_sram_clk_i,
  input  dft_pkg::dft_t                                  dft_i,

  input  logic                                           icache_req_write_i,
  input  logic [icache_geom_pkg::IcacheSramAddrWidth-1:0] icache_req_addr_i,
  input  logic                                           icache_req_valid_i,
  output logic                                           icache_req_ready_o,
  output logic [icache_geom_pkg::IcacheSramDataWidth-1:0] icache_resp_dout_o,
  output logic                                           icache_resp_valid_o,
  input  logic                                           icache_resp_ready_i,

  input  etlink_pkg::rsp_t                               neigh_sc_rsp_info_i,
  input  logic                                           neigh_sc_rsp_valid_i,
  input  logic                                           neigh_sc_rsp_ready_i,

  input  ram_cfg_pkg::ram_cfg_t                          ram_cfg_i,

  input  shirecache_pkg::icache_bist_req_t               bist_req_info_i,
  output shirecache_pkg::icache_bist_rsp_t               bist_rsp_info_o,

  input  logic [icache_pkg::IcacheDataApbAddrWidth-1:0]  apb_paddr_i,
  input  logic                                           apb_pwrite_i,
  input  logic                                           apb_psel_i,
  input  logic                                           apb_penable_i,
  input  logic [minion_pkg::BpamShireApbDataWidth-1:0]   apb_pwdata_i,
  output logic                                           apb_pready_o,
  output logic [minion_pkg::BpamShireApbDataWidth-1:0]   apb_prdata_o,
  output logic                                           apb_pslverr_o
);

  import dft_pkg::*;
  import etlink_pkg::*;
  import icache_geom_pkg::*;
  import icache_pkg::*;
  import minion_pkg::BpamShireApbDataWidth;
  import shirecache_pkg::MbiAddrSize;
  import shirecache_pkg::NumNeighLog;
  import shirecache_pkg::NumNeigh;
  import shirecache_pkg::icache_bist_req_t;
  import shirecache_pkg::icache_bist_rsp_t;

  localparam int unsigned RowBits      = (IcacheNrRows == 1) ? 1 : IcacheRowAdWidth;
  localparam int unsigned MbiMemIdxW   = (IcacheNrMems > 1) ? $clog2(IcacheNrMems) : 1;
  localparam int unsigned EccBlockW    = 64 + 8;

  /* verilator lint_off UNUSEDPARAM */  // Preserved for original interface parity; primitive-backed RAM path does not branch on it.
  localparam bit UnusedStandaloneMinion = StandaloneMinion;
  /* verilator lint_on UNUSEDPARAM */

  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0]                      icache_req_din;
  logic                                                                   f1_req_write_q;
  logic                                                                   f1_req_dbg_q;
  logic                                                                   f1_req_valid_q;
  logic                                                                   f1_req_valid_qual;
  logic                                                                   f1_req_ready;
  logic                                                                   f1_mbi_ready;
  logic                                                                   f2_req_valid_q;
  logic                                                                   f2_req_ready;
  logic                                                                   f2_mbi_access_q;
  logic                                                                   f3_req_dbg_q;
  logic                                                                   f3_req_valid_q;
  logic                                                                   f3_req_ready;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0]    f3_dout;
  logic                                                                   f4_req_dbg_q;
  logic                                                                   f4_req_valid_q;
  logic                                                                   f4_req_ready;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0]    f4_dout_q;

  /* verilator lint_off UNUSEDSIGNAL */  // The original response spy captures the full ET-link struct even though this wrapper only consumes fill-specific fields.
  rsp_t                                                                   neigh_sc_rsp_info_reg_q;
  /* verilator lint_on UNUSEDSIGNAL */
  logic                                                                   neigh_sc_rsp_valid_reg_q;
  logic                                                                   neigh_sc_rsp_ready_reg_q;
  logic                                                                   f0_l2_miss_resp_valid;
  logic                                                                   f0_l2_miss_resp_done_q;
  logic                                                                   f0_l2_miss_resp_done_d;
  logic                                                                   f0_waiting_l2_miss_resp;
  logic                                                                   f0_mbi_rd_en_xx;
  logic                                                                   f0_mbi_wr_en_xx;
  logic [IcacheMemAdWidth-1:0]                                            f0_mbi_addr_xx;
  logic [MbiMemIdxW-1:0]                                                  f0_mbi_mem_idx_xx;
  logic [NumNeighLog-1:0]                                                 f0_mbi_neigh_idx_xx;
  logic                                                                   f0_mbi_access_xx;
  logic [IcacheMemWidth-1:0]                                              f0_mbi_data_xx;
  logic                                                                   f0_mbi_ready;
  logic                                                                   f0_mbi_accepted;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        f0_req_ren;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        f0_req_wen;
  logic                                                                   f0_req_write;
  logic                                                                   f0_req_dbg;
  logic [IcacheMemAdWidth-1:0]                                            f0_req_addr;
  logic [RowBits-1:0]                                                     f0_req_row;
  logic [IcacheSramDataWidth-1:0]                                         f0_req_din;
  logic                                                                   f0_req_valid;
  logic                                                                   f0_req_ready;
  logic                                                                   f0_req_accepted;

  logic [etlink_pkg::DataSize-1:0]                                        neigh_sc_rsp_data;
  logic [IcacheEccBlocks*8-1:0]                                           neigh_sc_rsp_ecc;
  logic [IcacheEccBlocks-1:0][EccBlockW-1:0]                              neigh_sc_rsp_data_and_ecc;

  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        icache_req_ren;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        icache_req_wen;
  logic [RowBits-1:0]                                                     icache_req_row;

  icache_data_dbg_state_e                                                 dbg_state_q;
  icache_data_dbg_state_e                                                 dbg_state_d;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        dbg_req_ren_q;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        dbg_req_ren_d;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        dbg_req_wen_q;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        dbg_req_wen_d;
  logic                                                                   dbg_req_write_q;
  logic                                                                   dbg_req_write_d;
  logic [IcacheDbgDataMemChunkAddrWidth-1:0]                              dbg_req_chunk_q;
  logic [IcacheDbgDataMemChunkAddrWidth-1:0]                              dbg_req_chunk_d;
  logic [IcacheColAdWidth-1:0]                                            dbg_req_col_q;
  logic [IcacheColAdWidth-1:0]                                            dbg_req_col_d;
  logic                                                                   dbg_req_ecc_q;
  logic                                                                   dbg_req_ecc_d;
  logic [IcacheMemAdWidth-1:0]                                            dbg_req_addr_q;
  logic [IcacheMemAdWidth-1:0]                                            dbg_req_addr_d;
  logic [RowBits-1:0]                                                     dbg_req_row_q;
  logic [RowBits-1:0]                                                     dbg_req_row_d;
  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0]                      dbg_req_din;
  logic                                                                   dbg_req_valid_q;
  logic                                                                   dbg_req_valid_d;
  logic                                                                   dbg_req_ready;
  logic [BpamShireApbDataWidth-1:0]                                       dbg_dout_ecc;
  logic [BpamShireApbDataWidth-1:0]                                       dbg_dout;
  logic                                                                   apb_pready_d;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        mem_req_ren;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        mem_req_wen;
  logic                                                                   mem_req_write;
  logic [IcacheMemAdWidth-1:0]                                            mem_req_addr;
  logic [RowBits-1:0]                                                     mem_req_row;
  logic                                                                   mem_req_dbg;
  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0]                      mem_req_din;
  logic                                                                   mem_req_valid;
  logic                                                                   mem_req_ready;

  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        f1_req_ren_q;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        f1_req_wen_q;
  logic [IcacheMemAdWidth-1:0]                                            f1_req_addr_q;
  logic [RowBits-1:0]                                                     f1_req_row_q;
  logic [IcacheNrMemsPerRow-1:0][IcacheMemWidth-1:0]                      f1_req_din_q;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        f1_ren;
  logic [IcacheNrRows-1:0][IcacheNrMemsPerRow-1:0]                        f1_wen;
  logic                                                                   f1_mbi_access_q;
  logic [MbiMemIdxW-1:0]                                                  f1_mbi_mem_idx_q;

  logic [RowBits-1:0]                                                     f2_req_row_q;
  logic [IcacheSramDataWidth-1:0]                                         f2_req_din_q;
  logic                                                                   f2_req_write_q;
  logic                                                                   f2_req_dbg_q;
  logic                                                                   f2_req_valid_d;
  logic                                                                   f2_req_accepted;
  logic                                                                   f2_mbi_rd_valid_q;
  logic                                                                   f2_mbi_rd_valid_d;
  logic [MbiMemIdxW-1:0]                                                  f2_mbi_mem_idx_q;
  logic                                                                   f2_mbi_access_d;

  logic [RowBits-1:0]                                                     f3_req_row_q;
  logic [IcacheSramDataWidth-1:0]                                         f3_req_din_q;
  logic                                                                   f3_req_write_q;
  logic                                                                   f3_req_accepted;
  logic                                                                   f3_mbi_rd_valid_q;
  logic [MbiMemIdxW-1:0]                                                  f3_mbi_mem_idx_q;

  logic [RowBits-1:0]                                                     f4_req_row_q;
  logic [IcacheSramDataWidth-1:0]                                         f4_req_din_q;
  logic                                                                   f4_req_write_q;
  logic [IcacheNrMems-1:0][IcacheMemWidth-1:0]                            f4_mbi_dout;
  logic                                                                   f4_mbi_rd_valid_q;
  logic [MbiMemIdxW-1:0]                                                  f4_mbi_mem_idx_q;

  logic                                                                   f5_mbi_rd_valid_q;
  logic [IcacheMemWidth-1:0]                                              f5_mbi_rd_data_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      neigh_sc_rsp_valid_reg_q <= 1'b0;
      neigh_sc_rsp_ready_reg_q <= 1'b0;
      f0_l2_miss_resp_done_q   <= 1'b0;
    end else begin
      if (neigh_sc_rsp_valid_reg_q ^ neigh_sc_rsp_valid_i) begin
        neigh_sc_rsp_valid_reg_q <= neigh_sc_rsp_valid_i;
      end
      if (neigh_sc_rsp_ready_reg_q ^ neigh_sc_rsp_ready_i) begin
        neigh_sc_rsp_ready_reg_q <= neigh_sc_rsp_ready_i;
      end
      if (f0_l2_miss_resp_valid || (f1_req_valid_qual && f1_req_ready)) begin
        f0_l2_miss_resp_done_q <= f0_l2_miss_resp_done_d;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (neigh_sc_rsp_valid_i && neigh_sc_rsp_ready_i) begin
      neigh_sc_rsp_info_reg_q <= neigh_sc_rsp_info_i;
    end
  end

  assign f0_l2_miss_resp_valid =
      neigh_sc_rsp_valid_reg_q && neigh_sc_rsp_ready_reg_q &&
      (neigh_sc_rsp_info_reg_q.opcode != etlink_pkg::RspMsgRcvData) &&
      (etlink_pkg::neigh_source_e'(neigh_sc_rsp_info_reg_q.dest) == etlink_pkg::SrcIcache);

  always_comb begin
    f0_l2_miss_resp_done_d = f0_l2_miss_resp_done_q;
    if (f0_l2_miss_resp_valid) begin
      f0_l2_miss_resp_done_d = 1'b1;
    end else if (f1_req_valid_qual && f1_req_ready) begin
      f0_l2_miss_resp_done_d = 1'b0;
    end
  end

  assign f0_waiting_l2_miss_resp = f1_req_write_q && !f1_req_dbg_q && !f0_l2_miss_resp_done_q;

  for (genvar ecc_idx = 0; ecc_idx < IcacheEccBlocks; ecc_idx++) begin : gen_ecc_blocks
    prim_ecc_enc #(
      .DataWidth(64)
    ) u_icache_ecc_gen (
      .data_i        (neigh_sc_rsp_info_reg_q.data[ecc_idx*64 +: 64]),
      .data_and_ecc_o(neigh_sc_rsp_data_and_ecc[ecc_idx])
    );

    assign neigh_sc_rsp_ecc[ecc_idx*8 +: 8]   = neigh_sc_rsp_data_and_ecc[ecc_idx][71:64];
    assign neigh_sc_rsp_data[ecc_idx*64 +: 64] = neigh_sc_rsp_data_and_ecc[ecc_idx][63:0];
  end

  generate
    if (IcacheNrMemsPerRow > 1) begin : gen_din_wrwidth_gt_mwidth
      always_comb begin
        for (int unsigned mem = 0; mem < IcacheNrMemsPerRow; mem++) begin
          icache_req_din[mem] = {
            neigh_sc_rsp_ecc[IcacheMemEccBits * mem[IcacheColAdWidth-1:0] +: IcacheMemEccBits],
            neigh_sc_rsp_data[IcacheMemUsedBits * mem[IcacheColAdWidth-1:0] +: IcacheMemUsedBits]
          };
        end
      end
    end else begin : gen_din_wrwidth_eq_mwidth
      assign icache_req_din[0] = {
        neigh_sc_rsp_ecc[0 +: IcacheMemEccBits],
        neigh_sc_rsp_data[0 +: IcacheMemUsedBits]
      };
    end
  endgenerate

  generate
    if (IcacheNrRows > 1) begin : gen_gt1row
      assign icache_req_row = icache_req_addr_i[IcacheMemAdWidth +: IcacheRowAdWidth];

      always_comb begin
        icache_req_ren = '0;
        icache_req_wen = '0;
        if (icache_req_write_i) begin
          icache_req_wen[icache_req_row] = '1;
        end else begin
          icache_req_ren[icache_req_row] = '1;
        end
      end
    end else begin : gen_onerow
      assign icache_req_row = 1'b0;
      assign icache_req_ren = icache_req_write_i ? '0 : '1;
      assign icache_req_wen = icache_req_write_i ? '1 : '0;
    end
  endgenerate

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      dbg_state_q     <= IcacheDataDbgStateIdle;
      dbg_req_valid_q <= 1'b0;
      apb_pready_o    <= 1'b0;
    end else begin
      dbg_state_q     <= dbg_state_d;
      if (dbg_req_valid_d) begin
        dbg_req_ren_q   <= dbg_req_ren_d;
        dbg_req_wen_q   <= dbg_req_wen_d;
        dbg_req_write_q <= dbg_req_write_d;
        dbg_req_chunk_q <= dbg_req_chunk_d;
        dbg_req_col_q   <= dbg_req_col_d;
        dbg_req_ecc_q   <= dbg_req_ecc_d;
        dbg_req_addr_q  <= dbg_req_addr_d;
        dbg_req_row_q   <= dbg_req_row_d;
      end
      if (dbg_req_valid_q ^ dbg_req_valid_d) begin
        dbg_req_valid_q <= dbg_req_valid_d;
      end
      if (apb_pready_o ^ apb_pready_d) begin
        apb_pready_o <= apb_pready_d;
      end
    end
  end

  always_ff @(posedge clk_i) begin
    if (apb_pready_d && !apb_pwrite_i) begin
      apb_prdata_o <= dbg_dout;
    end
  end

  always_comb begin
    dbg_state_d     = dbg_state_q;
    dbg_req_write_d = dbg_req_write_q;
    dbg_req_valid_d = dbg_req_valid_q;
    apb_pready_d    = apb_pready_o;

    if (dbg_state_q == IcacheDataDbgStateIdle) begin
      if (apb_psel_i) begin
        dbg_state_d     = IcacheDataDbgStateWaitReadReady;
        dbg_req_write_d = 1'b0;
        dbg_req_valid_d = 1'b1;
      end
    end else if (dbg_state_q == IcacheDataDbgStateWaitReadReady) begin
      if (dbg_req_ready) begin
        dbg_req_valid_d = 1'b0;
        dbg_state_d     = IcacheDataDbgStateRead;
      end
    end else if (dbg_state_q == IcacheDataDbgStateRead) begin
      if (f4_req_valid_q && f4_req_dbg_q) begin
        if (apb_pwrite_i) begin
          dbg_state_d     = IcacheDataDbgStateWaitWriteReady;
          dbg_req_write_d = 1'b1;
          dbg_req_valid_d = 1'b1;
        end else begin
          dbg_state_d  = IcacheDataDbgStateDone;
          apb_pready_d = 1'b1;
        end
      end
    end else if (dbg_state_q == IcacheDataDbgStateWaitWriteReady) begin
      if (dbg_req_ready) begin
        dbg_req_valid_d = 1'b0;
        dbg_state_d     = IcacheDataDbgStateWrite;
      end
    end else if (dbg_state_q == IcacheDataDbgStateWrite) begin
      if (f4_req_valid_q && f4_req_dbg_q) begin
        dbg_state_d  = IcacheDataDbgStateDone;
        apb_pready_d = 1'b1;
      end
    end else if (dbg_state_q == IcacheDataDbgStateDone) begin
      dbg_state_d  = IcacheDataDbgStateIdle;
      apb_pready_d = 1'b0;
    end
  end

  assign dbg_req_chunk_d = apb_paddr_i[0 +: IcacheDbgDataMemChunkAddrWidth];
  assign dbg_req_col_d   = apb_paddr_i[IcacheDbgDataBlockChunkAddrWidth-2 -: IcacheColAdWidth];
  assign dbg_req_ecc_d   = apb_paddr_i[IcacheDbgDataBlockChunkAddrWidth-1];
  assign dbg_req_addr_d  = apb_paddr_i[IcacheDbgDataBlockChunkAddrWidth +: IcacheMemAdWidth];

  generate
    if (IcacheNrRows > 1) begin : gen_dbg_gt1row
      assign dbg_req_row_d =
          apb_paddr_i[IcacheDbgDataBlockChunkAddrWidth + IcacheMemAdWidth +: IcacheRowAdWidth];

      always_comb begin
        for (int unsigned row = 0; row < IcacheNrRows; row++) begin
          for (int unsigned mem = 0; mem < IcacheNrMemsPerRow; mem++) begin
            dbg_req_ren_d[row][mem] =
                (dbg_req_row_d == row[0 +: IcacheRowAdWidth]) &&
                ((dbg_req_col_d == mem[0 +: IcacheColAdWidth]) || dbg_req_ecc_d) &&
                !dbg_req_write_d;
            dbg_req_wen_d[row][mem] =
                (dbg_req_row_d == row[0 +: IcacheRowAdWidth]) &&
                ((dbg_req_col_d == mem[0 +: IcacheColAdWidth]) || dbg_req_ecc_d) &&
                dbg_req_write_d;
          end
        end
      end
    end else begin : gen_dbg_onerow
      assign dbg_req_row_d = 1'b0;

      always_comb begin
        for (int unsigned mem = 0; mem < IcacheNrMemsPerRow; mem++) begin
          dbg_req_ren_d[0][mem] =
              ((dbg_req_col_d == mem[0 +: IcacheColAdWidth]) || dbg_req_ecc_d) && !dbg_req_write_d;
          dbg_req_wen_d[0][mem] =
              ((dbg_req_col_d == mem[0 +: IcacheColAdWidth]) || dbg_req_ecc_d) && dbg_req_write_d;
        end
      end
    end
  endgenerate

  always_comb begin
    dbg_req_din = f4_dout_q;
    if (dbg_req_ecc_q) begin
      for (int i = 0; i < IcacheNrMemsPerRow; i++) begin
        dbg_req_din[i][IcacheMemWidth-1 -: IcacheMemEccBits] =
            apb_pwdata_i[i*IcacheMemEccBits +: IcacheMemEccBits];
      end
    end else begin
      dbg_req_din[dbg_req_col_q][dbg_req_chunk_q*BpamShireApbDataWidth +: BpamShireApbDataWidth] =
          apb_pwdata_i;
    end
  end

  always_comb begin
    for (int i = 0; i < IcacheNrMemsPerRow; i++) begin
      dbg_dout_ecc[i*IcacheMemEccBits +: IcacheMemEccBits] =
          f4_dout_q[dbg_req_row_q][i][IcacheMemWidth-1 -: IcacheMemEccBits];
    end
  end

  assign dbg_dout = dbg_req_ecc_q
                  ? dbg_dout_ecc
                  : f4_dout_q[dbg_req_row_q][dbg_req_col_q]
                        [dbg_req_chunk_q*BpamShireApbDataWidth +: BpamShireApbDataWidth];
  assign apb_pslverr_o = 1'b0;

  assign icache_req_ready_o = mem_req_ready;
  assign dbg_req_ready      = mem_req_ready & ~icache_req_valid_i;

  assign mem_req_ren   = icache_req_valid_i ? icache_req_ren : dbg_req_ren_q;
  assign mem_req_wen   = icache_req_valid_i ? icache_req_wen : dbg_req_wen_q;
  assign mem_req_write = icache_req_valid_i ? icache_req_write_i : dbg_req_write_q;
  assign mem_req_addr  = icache_req_valid_i ? icache_req_addr_i[0 +: IcacheMemAdWidth] : dbg_req_addr_q;
  assign mem_req_din   = f0_l2_miss_resp_valid ? icache_req_din : dbg_req_din;
  assign mem_req_row   = icache_req_row;
  assign mem_req_dbg   = ~icache_req_valid_i;
  assign mem_req_valid = icache_req_valid_i | dbg_req_valid_q;
  assign mem_req_ready = f0_req_ready;

  assign f0_mbi_rd_en_xx     = bist_req_info_i.mbi_sel & bist_req_info_i.mbi_rd_en;
  assign f0_mbi_wr_en_xx     = bist_req_info_i.mbi_sel & bist_req_info_i.mbi_wr_en;
  assign f0_mbi_addr_xx      = bist_req_info_i.mbi_addr[0 +: IcacheMemAdWidth];
  assign f0_mbi_mem_idx_xx   = bist_req_info_i.mbi_addr[IcacheMemAdWidth +: MbiMemIdxW];
  assign f0_mbi_neigh_idx_xx = bist_req_info_i.mbi_addr[IcacheMemAdWidth + MbiMemIdxW +: NumNeighLog];
  assign f0_mbi_access_xx    =
      (f0_mbi_rd_en_xx | f0_mbi_wr_en_xx) && (f0_mbi_neigh_idx_xx == Id[NumNeighLog-1:0]);
  assign f0_mbi_data_xx      = bist_req_info_i.mbi_wdata;
  assign f0_mbi_accepted     = f0_mbi_access_xx & f0_mbi_ready;

  always_comb begin
    f0_req_ren = mem_req_ren;
    f0_req_wen = mem_req_wen;
    if (f0_mbi_access_xx) begin
      f0_req_ren = '0;
      f0_req_wen = '0;
      f0_req_ren[0][f0_mbi_mem_idx_xx] = f0_mbi_rd_en_xx;
      f0_req_wen[0][f0_mbi_mem_idx_xx] = f0_mbi_wr_en_xx;
    end
  end

  assign f0_req_write    = f0_mbi_access_xx ? f0_mbi_wr_en_xx : mem_req_write;
  assign f0_req_dbg      = mem_req_dbg;
  assign f0_req_addr     = f0_mbi_access_xx ? f0_mbi_addr_xx : mem_req_addr;
  assign f0_req_row      = mem_req_row;
  assign f0_req_din      = f0_mbi_access_xx ? {IcacheNrMemsPerRow{f0_mbi_data_xx}} : mem_req_din;
  assign f0_req_valid    = mem_req_valid;
  assign f0_req_accepted = f0_req_valid & f0_req_ready;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f1_req_ren_q     <= '0;
      f1_req_wen_q     <= '0;
      f1_req_addr_q    <= '0;
      f1_req_write_q   <= 1'b0;
      f1_req_dbg_q     <= 1'b0;
      f1_req_valid_q   <= 1'b0;
      f1_mbi_access_q  <= 1'b0;
    end else begin
      if (f0_req_accepted || f0_mbi_accepted) begin
        f1_req_ren_q   <= f0_req_ren;
        f1_req_wen_q   <= f0_req_wen;
        f1_req_addr_q  <= f0_req_addr;
        f1_req_write_q <= f0_req_write;
      end
      if (f0_req_accepted) begin
        f1_req_row_q <= f0_req_row;
        f1_req_dbg_q <= f0_req_dbg;
      end
      if (f0_req_accepted || f0_mbi_accepted || f0_l2_miss_resp_valid) begin
        f1_req_din_q <= f0_req_din;
      end
      if (f0_req_ready && (f1_req_valid_q ^ f0_req_valid)) begin
        f1_req_valid_q <= f0_req_valid;
      end
      if (f0_mbi_ready) begin
        f1_mbi_access_q <= f0_mbi_access_xx;
      end
      if (f0_mbi_accepted) begin
        f1_mbi_mem_idx_q <= f0_mbi_mem_idx_xx;
      end
    end
  end

  assign f0_req_ready      = (f1_req_ready && !f0_waiting_l2_miss_resp) || !f1_req_valid_q;
  assign f0_mbi_ready      = f1_mbi_ready || !f1_mbi_access_q;
  assign f1_req_valid_qual = f1_req_valid_q && !f0_waiting_l2_miss_resp;
  assign f1_ren            = f1_req_ren_q & {IcacheNrRows{ {IcacheNrMemsPerRow{f1_req_valid_qual | f1_mbi_access_q}} }};
  assign f1_wen            = f1_req_wen_q & {IcacheNrRows{ {IcacheNrMemsPerRow{f1_req_valid_qual | f1_mbi_access_q}} }};

  for (genvar r = 0; r < IcacheNrRows; r++) begin : gen_rows
    for (genvar j = 0; j < IcacheNrMemsPerRow; j++) begin : gen_mem_row
      logic icache_f1_mem_en;
      logic icache_f1_mem_we;
      logic clock_en;
      logic sram_clock_pre_q;
      logic sram_clock;
      logic sram_clock_en;
      logic mem_alert_unused;

      assign icache_f1_mem_en = f1_ren[r][j] || f1_wen[r][j];
      assign icache_f1_mem_we = f1_wen[r][j];
      assign clock_en         = icache_f1_mem_en & ~sram_clock_pre_q;
      assign sram_clock_en    = sram_clock_pre_q || clock_en;

      always_ff @(posedge clk_i or negedge rst_ni) begin
        if (!rst_ni) begin
          sram_clock_pre_q <= 1'b0;
        end else if (sram_clock_en) begin
          sram_clock_pre_q <= clock_en;
        end
      end

      prim_clk_mux u_clk_mux (
        .clk0_i(sram_clock_pre_q),
        .clk1_i(dft_sram_clk_i),
        .sel_i (dft_i.sram_clk_override),
        .clk_o (sram_clock)
      );

      prim_ram_1p #(
        .Width(IcacheMemWidth),
        .Depth(IcacheMemWords)
      ) u_mem (
        .clk_i   (sram_clock),
        .rst_ni  (rst_ni),
        .req_i   (icache_f1_mem_en && !dft_i.ram_rei),
        .we_i    (icache_f1_mem_we && !dft_i.ram_wei),
        .addr_i  (f1_req_addr_q),
        .wdata_i (f1_req_din_q[j]),
        .rdata_o (f3_dout[r][j]),
        .cfg_i   (ram_cfg_i),
        .dft_i   (dft_i),
        .alert_o (mem_alert_unused)
      );
    end
  end

  assign f1_req_ready = f2_req_valid_q && f2_req_ready;
  assign f1_mbi_ready = f2_mbi_access_q;

  assign f2_req_row_q     = f1_req_row_q;
  assign f2_req_din_q     = f1_req_din_q;
  assign f2_req_write_q   = f1_req_write_q;
  assign f2_req_dbg_q     = f1_req_dbg_q;
  assign f2_mbi_mem_idx_q = f1_mbi_mem_idx_q;

  always_comb begin
    if (!f2_req_valid_q) begin
      f2_req_valid_d = f1_req_valid_qual;
    end else begin
      f2_req_valid_d = ~f2_req_ready;
    end

    if (!f2_mbi_access_q) begin
      f2_mbi_access_d = f1_mbi_access_q;
    end else begin
      f2_mbi_access_d = 1'b0;
    end
  end

  assign f2_mbi_rd_valid_d = f2_mbi_access_d && (|f1_req_ren_q);
  assign f2_req_accepted   = f2_req_valid_q && f2_req_ready;
  assign f2_req_ready      = f3_req_ready || !f3_req_valid_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f2_req_valid_q    <= 1'b0;
      f2_mbi_access_q   <= 1'b0;
      f2_mbi_rd_valid_q <= 1'b0;
    end else begin
      if (f2_req_valid_q ^ f2_req_valid_d) begin
        f2_req_valid_q <= f2_req_valid_d;
      end
      if (f2_mbi_access_q ^ f2_mbi_access_d) begin
        f2_mbi_access_q <= f2_mbi_access_d;
      end
      if (f2_mbi_rd_valid_q ^ f2_mbi_rd_valid_d) begin
        f2_mbi_rd_valid_q <= f2_mbi_rd_valid_d;
      end
    end
  end

  assign f3_req_accepted = f3_req_valid_q && f3_req_ready;
  assign f3_req_ready    = f4_req_ready || !f4_req_valid_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f3_req_valid_q    <= 1'b0;
      f3_mbi_rd_valid_q <= 1'b0;
    end else begin
      if (f2_req_accepted) begin
        f3_req_row_q   <= f2_req_row_q;
        f3_req_din_q   <= f2_req_din_q;
        f3_req_write_q <= f2_req_write_q;
        f3_req_dbg_q   <= f2_req_dbg_q;
      end
      if (f2_req_ready && (f3_req_valid_q ^ f2_req_valid_q)) begin
        f3_req_valid_q <= f2_req_valid_q;
      end
      if (f3_mbi_rd_valid_q ^ f2_mbi_rd_valid_q) begin
        f3_mbi_rd_valid_q <= f2_mbi_rd_valid_q;
      end
      if (f2_mbi_rd_valid_q) begin
        f3_mbi_mem_idx_q <= f2_mbi_mem_idx_q;
      end
    end
  end

  assign f4_req_ready = icache_resp_ready_i;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f4_req_valid_q    <= 1'b0;
      f4_mbi_rd_valid_q <= 1'b0;
    end else begin
      if (f3_req_accepted) begin
        f4_req_row_q   <= f3_req_row_q;
        f4_req_din_q   <= f3_req_din_q;
        f4_req_write_q <= f3_req_write_q;
        f4_req_dbg_q   <= f3_req_dbg_q;
      end
      if (f3_req_accepted || f3_mbi_rd_valid_q) begin
        f4_dout_q <= f3_dout;
      end
      if (f3_req_ready && (f4_req_valid_q ^ f3_req_valid_q)) begin
        f4_req_valid_q <= f3_req_valid_q;
      end
      if (f4_mbi_rd_valid_q ^ f3_mbi_rd_valid_q) begin
        f4_mbi_rd_valid_q <= f3_mbi_rd_valid_q;
      end
      if (f3_mbi_rd_valid_q) begin
        f4_mbi_mem_idx_q <= f3_mbi_mem_idx_q;
      end
    end
  end

  assign icache_resp_dout_o  = f4_req_write_q ? f4_req_din_q : f4_dout_q[f4_req_row_q];
  assign icache_resp_valid_o = f4_req_valid_q & ~f4_req_dbg_q;

  assign f4_mbi_dout = f4_dout_q;

  always_ff @(posedge clk_i or negedge rst_ni) begin
    if (!rst_ni) begin
      f5_mbi_rd_valid_q <= 1'b0;
    end else if (f5_mbi_rd_valid_q ^ f4_mbi_rd_valid_q) begin
      f5_mbi_rd_valid_q <= f4_mbi_rd_valid_q;
    end
  end

  always_ff @(posedge clk_i) begin
    if (f4_mbi_rd_valid_q) begin
      f5_mbi_rd_data_q <= f4_mbi_dout[f4_mbi_mem_idx_q];
    end
  end

  assign bist_rsp_info_o.mbi_rdata = {IcacheMemWidth{f5_mbi_rd_valid_q}} & f5_mbi_rd_data_q;

  logic unused_ok;
  assign unused_ok = ^{
    UnusedStandaloneMinion,
    apb_penable_i,
    bist_req_info_i.mbist_on
  };

endmodule : icache_data_ram_wrap
