// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

module vpu_top (
  // System signals
  input  logic                                     clock,
  input  logic                                     reset,
  // Control signals coming from core
  input  minion_vpu_id_req                         id_core_req,
  input  vpu_ctrl_sigs_t                           id_vpu_decoder_sigs,
  input  minion_vpu_ex_req                         ex_core_req,
  input  logic                                     f2_core_kill,
  input  logic                                     f3_core_kill,
  input  logic                                     f4_core_kill,
  input  core_vpu_ctrl                             f0_core_ctrl,
  // Chicken_bits
  input  logic                                     chicken_bit_vpulane,
  input  logic                                     chicken_bit_vputima,
  input  logic                                     chicken_bit_vputrans,
  // Memory control override
  input  logic [`MINION_MEM_CTRL_BITS-1:0]         mem_ctrl_override,
  // WB Dcache response
  input  logic                                     wb_dcache_resp_valid,
  input  dcache_vpu_resp                           wb_dcache_resp,
  // DCache SRAM direct port
  output vpu_dcache_ctrl                           dcache_ctrl,
  input  dcache_vpu_scp_resp                       dcache_scp_resp,
  input  logic [`DCACHE_DATA_RANGE]                dcache_scp_data,
  input  logic [`DCACHE_DATA_RANGE]                dcache_tenb_data,
  // Dcache reduce control
  input  dcache_vpu_reduce_ctrl                    dcache_reduce_ctrl,
  // Control signals going to core
  output vpu_minion_id_ctrl                        id_core_ctrl,
  output vpu_minion_ex_ctrl                        ex_core_ctrl,
  output vpu_minion_tag_ctrl                       f2_core_ctrl,
  output vpu_minion_mem_ctrl                       f3_core_ctrl,
  output vpu_minion_wb_ctrl                        wb_core_ctrl,
  // Events for performance counters
  output logic [`CSR_NR_EVENTS_VPU-1:0]            io_events,
  // Signals going to debug monitor
  output logic [`NEIGH_DEBUG_MATCH_WIDTH-1:0]      vpu_dbg_match,
  output logic [`NEIGH_DEBUG_FILTER_WIDTH-1:0]     vpu_dbg_filter,
  output logic [4:0] [`NEIGH_DEBUG_DATA_WIDTH-1:0] vpu_dbg_data
);

////////////////////////////////////////////////////////////////////////////////
// Internal wires
////////////////////////////////////////////////////////////////////////////////

logic [2:0]                                            id_regfile_thread_id;
logic [2:0]                                            id_regfile_ren;
logic [`FREG_ADDR_RANGE]                               id_regfile_raddr1;
logic [`FREG_ADDR_RANGE]                               id_regfile_raddr2;
logic [`FREG_ADDR_RANGE]                               id_regfile_raddr3;
logic [`VPU_LANE_NUM-1:0]                              ex_mask_in1;
logic                                                  ex_tena_regfile_bypass_en;
logic                                                  ex_tenb_regfile_bypass_en;
logic [2:0]                                            ex_regfile_thread_id;
logic                                                  ex_sh_sw_valid;
logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]           ex_tima_valid;
logic                                                  ex_tima_valid_unqual;
logic [`VPU_TENB_ADDR]                                 ex_tima_tenb_raddr;
logic                                                  ex_tima_sa;
logic                                                  ex_tima_sb;
logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          ex_tena_rf_rdata;
logic [`VPU_DATA_S_SZ-1:0]                             ex_tena_rf_tima_rdata;
logic [`VPU_LANE_NUM-1:0]                              ex_txfma_valid;
logic                                                  ex_txfma_clock_valid;
logic                                                  ex_sh_sw_clock_valid;
logic                                                  ex_rom_clock_valid;
logic [`FREG_ADDR_RANGE]                               ex_regfile_raddr1;
logic [`FREG_ADDR_RANGE]                               ex_regfile_raddr2;
logic [`FREG_ADDR_RANGE]                               ex_regfile_raddr3;
logic [`VPU_LANE_NUM-1:0][1:0][`VPU_DATA_S_SZ-1:0]     ex_regfile_rdata_bypass;
vpu_input_t                                            ex_req;
vpu_bypass_force_ctrl                                  ex_bypass_force_ctrl;
logic                                                  ex_rom_valid;
logic                                                  f2_tima_ren3;
logic [`VPU_TENC_ADDR]                                 f2_tima_tenc_raddr;
logic                                                  f2_tenb_regfile_wen_l;
logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          f2_fswizz_rdata1;
logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          f2_fswizz_rdata2;
logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          f2_fswizz_rdata3;
logic [`VPU_LANE_NUM-1:0][`VPU_DATA_S_SZ-1:0]          f2_fswizz_rdata;
logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]           f3_tima_rf_wen;
logic                                                  f3_tenb_regfile_wen_l;
logic [`VPU_TENB_ADDR]                                 f3_tenb_regfile_waddr_l;
logic [`VPU_DATA_SZ-1:0]                               f3_tenb_regfile_wdata_l;
logic                                                  f3_thread_id;
logic [`VPU_LANE_NUM-1:0]                              f3_regfile_wmask;
logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f3_regfile_waddr;
logic [`VPU_LANE_NUM-1:0][`VPU_NUM_TIMA-1:0]           f3_tima_tenc_wen;
logic [`VPU_TENC_ADDR]                                 f3_tima_tenc_waddr;
logic                                                  f3_data_vector;
logic                                                  f3_bypass_clk_en;
logic [`VPU_LANE_NUM-1:0]                              f3_maskop_bit;
logic [`VPU_LANE_NUM-1:0]                              f3_regmask_wdata_lane;
logic                                                  f4_regfile_wen_l;
logic                                                  f4_thread_id;
logic                                                  f4_regfile_thrid_l;
logic [`VPU_LANE_NUM-1:0][1:0]                         f4_regfile_wmask_l;
logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f4_regfile_waddr_l;
logic [`VPU_DATA_SZ-1:0]                               f4_regfile_wdata_l;
logic [`VPU_LANE_NUM-1:0]                              f4_regfile_wmask;
logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f4_regfile_waddr;
logic                                                  f4_bypass_clk_en;
logic                                                  f5_thread_id;
logic [`VPU_LANE_NUM-1:0]                              f5_regfile_wmask;
logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f5_regfile_waddr;
logic                                                  f5_bypass_clk_en;
logic                                                  f6_thread_id;
logic [`VPU_LANE_NUM-1:0]                              f6_regfile_wmask;
logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f6_regfile_waddr;
logic                                                  f6_bypass_clk_en;
logic                                                  f7_thread_id;
logic [`VPU_LANE_NUM-1:0]                              f7_regfile_wmask;
logic [$clog2(`VPU_REGFILE_NUM)-1:0]                   f7_regfile_waddr;
logic                                                  f7_bypass_clk_en;
logic                                                  f8_txfma_op;
logic                                                  f8_data_vector;
logic [`VPU_LANE_NUM-1:0][`CORE_FCSR_FLAG_BITS_SZ-1:0] f8_wflags;
logic [`VPU_LANE_NUM-1:0][1:0]                         txfma_trans_dbg;
logic [`VPU_LANE_NUM-1:0]                              f8_regfile_wen;
logic [`VPU_LANE_NUM-1:0]                              f8_regfile_wmask;
logic                                                  f8_thread_id;
logic [`FREG_ADDR_RANGE]                               f8_regfile_waddr;
logic [`VPU_DATA_RANGE]                                f8_wdata;
logic [`VPU_DATA_RANGE]                                f8_sh_sw_wdata;
logic [`VPU_LANE_NUM-1:0]                              f8_txfma_comp_res;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Multi-stage functional units
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// VPU control
// This unit sends the control signals to the datapath units
////////////////////////////////////////////////////////////////////////////////

for (genvar i = 0; i < `VPU_LANE_NUM; i++) begin: F3_REGMASK_WDATA_LANE
  assign f3_regmask_wdata_lane[i] = f3_maskop_bit[i];
end


vpu_ctrl ctrl (
  // System signals
  .clock_aon                   ( clock                                  ),
  .clock_sec                   ( clock                                  ),
  .reset                       ( reset                                  ),
  // ID/F0 inputs
  .id_core_inst_bits           ( id_core_req.inst_bits                  ),
  .id_core_valid               ( id_core_req.valid                      ),
  .id_core_thread_id           ( id_core_req.thread_id                  ),
  .id_fcsr_rm                  ( id_core_req.fcsr_rm                    ),
  .id_decoder_sigs             ( id_vpu_decoder_sigs                    ),
  .f0_core_ctrl                ( f0_core_ctrl                           ),
  // Chicken bit vputrans
  .chicken_bit_vputrans        ( chicken_bit_vputrans                   ),
  // EX/F1 inputs
  .ex_thread_id                ( ex_core_req.thread_id                  ),
  .ex_core_gscing              ( ex_core_req.gscing                     ),
  .ex_core_kill                ( ex_core_req.kill                       ),
  .ex_core_gsc_src             ( ex_core_req.gsc_src                    ),
  .ex_regfile_rdata_bypass     ( ex_regfile_rdata_bypass                ),
  .ex_bypass_force_ctrl        ( ex_bypass_force_ctrl                   ),
  .ex_core_fromint_data        ( ex_core_req.fromint_data               ),
  // F2 inputs
  .f2_core_kill                ( f2_core_kill                           ),
  .f2_fswizz_rdata             ( f2_fswizz_rdata                        ),
  // F3 inputs
  .f3_core_kill                ( f3_core_kill                           ),
  .f3_regmask_wdata_lane       ( f3_regmask_wdata_lane                  ),
  // F4 inputs
  .f4_core_kill                ( f4_core_kill                           ),
  // F8 inputs
  .f8_wdata_lane0              ( f8_wdata[`VPU_DATA_S_SZ-1:0]           ),
  .f8_wflags                   ( f8_wflags                              ),
  .f8_sh_sw_wdata              ( f8_sh_sw_wdata                         ),
  .f8_txfma_comp_res           ( f8_txfma_comp_res                      ),
  // WB DMEM inputs
  .wb_dmem_resp_val            ( wb_dcache_resp_valid                   ),
  .wb_dmem_resp                ( wb_dcache_resp                         ),
  // ML override inputs
  .dcache_scp_resp             ( dcache_scp_resp                        ),
  .dcache_scp_data             ( dcache_scp_data                        ),
  .dcache_tenb_data            ( dcache_tenb_data                       ),
  // debug inputs
  .txfma_trans_dbg             ( txfma_trans_dbg                        ),
  // ML override outputs
  .dcache_ctrl                 ( dcache_ctrl                            ),
  // Dcache reduce control
  .dcache_reduce_ctrl          ( dcache_reduce_ctrl                     ),
  // ID outputs
  .id_core_ctrl                ( id_core_ctrl                           ),
  .id_regfile_raddr1           ( id_regfile_raddr1                      ),
  .id_regfile_raddr2           ( id_regfile_raddr2                      ),
  .id_regfile_raddr3           ( id_regfile_raddr3                      ),
  .id_regfile_ren              ( id_regfile_ren                         ),
  .id_regfile_thread_id        ( id_regfile_thread_id                   ),
  // EX/F1 outputs
  .ex_regfile_thread_id        ( ex_regfile_thread_id                   ),
  .ex_regfile_raddr1           ( ex_regfile_raddr1                      ),
  .ex_regfile_raddr2           ( ex_regfile_raddr2                      ),
  .ex_regfile_raddr3           ( ex_regfile_raddr3                      ),
  .ex_req                      ( ex_req                                 ),
  .ex_core_ctrl                ( ex_core_ctrl                           ),
  .ex_mask_in1                 ( ex_mask_in1                            ),
  .ex_tena_regfile_bypass_en   ( ex_tena_regfile_bypass_en              ),
  .ex_tenb_regfile_bypass_en   ( ex_tenb_regfile_bypass_en              ),
  .ex_tima_valid               ( ex_tima_valid                          ),
  .ex_tima_valid_unqual        ( ex_tima_valid_unqual                   ),
  .ex_tima_sa                  ( ex_tima_sa                             ),
  .ex_tima_sb                  ( ex_tima_sb                             ),
  .ex_tima_tenb_raddr          ( ex_tima_tenb_raddr                     ),
  .ex_tena_rf_rdata            ( ex_tena_rf_rdata                       ),
  .ex_tena_rf_tima_rdata       ( ex_tena_rf_tima_rdata                  ),
  .ex_rom_valid                ( ex_rom_valid                           ),
  .ex_sh_sw_valid              ( ex_sh_sw_valid                         ),
  .ex_txfma_valid              ( ex_txfma_valid                         ),
  .ex_txfma_clock_valid        ( ex_txfma_clock_valid                   ),
  .ex_rom_clock_valid          ( ex_rom_clock_valid                     ),
  .ex_sh_sw_clock_valid        ( ex_sh_sw_clock_valid                   ),
  // F2 outputs
  .f2_core_ctrl                ( f2_core_ctrl                           ),
  .f2_tenb_regfile_wen_l       ( f2_tenb_regfile_wen_l                  ),
  .f2_tima_ren3                ( f2_tima_ren3                           ),
  .f2_tima_tenc_raddr          ( f2_tima_tenc_raddr                     ),
  .f2_fswizz_rdata1            ( f2_fswizz_rdata1                       ),
  .f2_fswizz_rdata2            ( f2_fswizz_rdata2                       ),
  .f2_fswizz_rdata3            ( f2_fswizz_rdata3                       ),
  // F3 outputs
  .f3_tima_rf_wen              ( f3_tima_rf_wen                         ),
  .f3_tenb_regfile_wdata_l     ( f3_tenb_regfile_wdata_l                ),
  .f3_tenb_regfile_wen_l       ( f3_tenb_regfile_wen_l                  ),
  .f3_tenb_regfile_waddr_l     ( f3_tenb_regfile_waddr_l                ),
  .f3_core_ctrl                ( f3_core_ctrl                           ),
  .f3_tima_tenc_wen            ( f3_tima_tenc_wen                       ),
  .f3_tima_tenc_waddr          ( f3_tima_tenc_waddr                     ),
  .f3_thread_id                ( f3_thread_id                           ),
  .f3_regfile_wmask            ( f3_regfile_wmask                       ),
  .f3_regfile_waddr            ( f3_regfile_waddr                       ),
  .f3_bypass_clk_en            ( f3_bypass_clk_en                       ),
  .f3_data_vector              ( f3_data_vector                         ),
  // F4 outputs
  .f4_thread_id                ( f4_thread_id                           ),
  .f4_regfile_wmask            ( f4_regfile_wmask                       ),
  .f4_regfile_waddr            ( f4_regfile_waddr                       ),
  .f4_bypass_clk_en            ( f4_bypass_clk_en                       ),
  .f4_regfile_wen_l            ( f4_regfile_wen_l                       ),
  .f4_regfile_thrid_l          ( f4_regfile_thrid_l                     ),
  .f4_regfile_wmask_l          ( f4_regfile_wmask_l                     ),
  .f4_regfile_waddr_l          ( f4_regfile_waddr_l                     ),
  .f4_regfile_wdata_l          ( f4_regfile_wdata_l                     ),
  // F5 outputs
  .f5_thread_id                ( f5_thread_id                           ),
  .f5_regfile_wmask            ( f5_regfile_wmask                       ),
  .f5_regfile_waddr            ( f5_regfile_waddr                       ),
  .f5_bypass_clk_en            ( f5_bypass_clk_en                       ),
  // F6 outputs
  .f6_thread_id                ( f6_thread_id                           ),
  .f6_regfile_wmask            ( f6_regfile_wmask                       ),
  .f6_regfile_waddr            ( f6_regfile_waddr                       ),
  .f6_bypass_clk_en            ( f6_bypass_clk_en                       ),
  // F7 outputs
  .f7_thread_id                ( f7_thread_id                           ),
  .f7_regfile_wmask            ( f7_regfile_wmask                       ),
  .f7_regfile_waddr            ( f7_regfile_waddr                       ),
  .f7_bypass_clk_en            ( f7_bypass_clk_en                       ),
  // F8 outputs
  .f8_thread_id                ( f8_thread_id                           ),
  .f8_regfile_waddr            ( f8_regfile_waddr                       ),
  .f8_regfile_wmask            ( f8_regfile_wmask                       ),
  .f8_regfile_wen              ( f8_regfile_wen                         ),
  .f8_txfma_op                 ( f8_txfma_op                            ),
  .f8_data_vector              ( f8_data_vector                         ),
  // WB outputs
  .wb_core_ctrl                ( wb_core_ctrl                           ),
  .io_events                   ( io_events                              ),
  .vpu_dbg_match               ( vpu_dbg_match                          ),
  .vpu_dbg_filter              ( vpu_dbg_filter                         ),
  .vpu_dbg_data                ( vpu_dbg_data                           )
);


////////////////////////////////////////////////////////////////////////////////
// VPU Lanes
// This unit implements the SIMD instructions, rf and bypass
////////////////////////////////////////////////////////////////////////////////
for (genvar i = 0; i < `VPU_LANE_NUM; i++) begin : VPU_LANES
  vpu_lane lane (
    // System signals
    .lane_id                     ( i[2:0]                                                    ),
    .clock                       ( clock                                                     ),
    .reset                       ( reset                                                     ),
    // Chicken bits clock gate enable
    .chicken_bit_vpulane         ( chicken_bit_vpulane                                       ),
    .chicken_bit_vputima         ( chicken_bit_vputima                                       ),
    // Memory control override
    .mem_ctrl_override           ( mem_ctrl_override                                         ),
    // ID/F0 data and control inputs
    .id_regfile_raddr1           ( id_regfile_raddr1                                         ),
    .id_regfile_raddr2           ( id_regfile_raddr2                                         ),
    .id_regfile_raddr3           ( id_regfile_raddr3                                         ),
    .id_regfile_ren              ( id_regfile_ren                                            ),
    .id_regfile_thread_id        ( id_regfile_thread_id                                      ),
    .id_trans_busy               ( id_core_ctrl.trans_busy                                   ),
    // EX/F1 data and control inputs
    .ex_regfile_raddr1           ( ex_regfile_raddr1                                         ),
    .ex_regfile_raddr2           ( ex_regfile_raddr2                                         ),
    .ex_regfile_raddr3           ( ex_regfile_raddr3                                         ),
    .ex_regfile_thread_id        ( ex_regfile_thread_id                                      ),
    .ex_mask_in1                 ( ex_mask_in1[i]                                            ),
    .ex_tena_regfile_bypass_en   ( ex_tena_regfile_bypass_en                                 ),
    .ex_tenb_regfile_bypass_en   ( ex_tenb_regfile_bypass_en                                 ),
    .ex_req_lane                 ( ex_req                                                    ),
    .ex_fromint_data             ( ex_core_req.fromint_data[`VPU_DATA_S_SZ-1:0]              ),
    .ex_bypass_force_ctrl        ( ex_bypass_force_ctrl                                      ),
    .ex_tima_valid               ( ex_tima_valid[i]                                          ),
    .ex_tima_valid_unqual        ( ex_tima_valid_unqual                                      ),
    .ex_tima_sa                  ( ex_tima_sa                                                ),
    .ex_tima_sb                  ( ex_tima_sb                                                ),
    .ex_tima_tenb_raddr          ( ex_tima_tenb_raddr                                        ),
    .ex_tena_rf_rdata            ( ex_tena_rf_rdata[i]                                       ),
    .ex_tena_rf_tima_rdata       ( ex_tena_rf_tima_rdata                                     ),
    .ex_rom_valid                ( ex_rom_valid                                              ),
    .ex_sh_sw_valid              ( ex_sh_sw_valid                                            ),
    .ex_txfma_valid              ( ex_txfma_valid[i]                                         ),
    .ex_txfma_clock_valid        ( ex_txfma_clock_valid                                      ),
    .ex_rom_clock_valid          ( ex_rom_clock_valid                                        ),
    .ex_sh_sw_clock_valid        ( ex_sh_sw_clock_valid                                      ),
    // F2 inputs
    .f2_tima_ren3                ( f2_tima_ren3                                              ),
    .f2_tima_tenc_raddr          ( f2_tima_tenc_raddr                                        ),
    .f2_tenb_regfile_wen_l       ( f2_tenb_regfile_wen_l                                     ),
    .f2_fswizz_rdata1            ( f2_fswizz_rdata1[i]                                       ),
    .f2_fswizz_rdata2            ( f2_fswizz_rdata2[i]                                       ),
    .f2_fswizz_rdata3            ( f2_fswizz_rdata3[i]                                       ),
    .dcache_scp_resp             ( dcache_scp_resp                                           ),
    // F3 inputs
    .f3_tenb_regfile_wdata_l     ( f3_tenb_regfile_wdata_l[i*`VPU_DATA_S_SZ+:`VPU_DATA_S_SZ] ),
    .f3_tenb_regfile_wen_l       ( f3_tenb_regfile_wen_l                                     ),
    .f3_tenb_regfile_waddr_l     ( f3_tenb_regfile_waddr_l                                   ),
    .f3_tima_tenc_wen            ( f3_tima_tenc_wen[i]                                       ),
    .f3_tima_tenc_waddr          ( f3_tima_tenc_waddr                                        ),
    .f3_tima_rf_wen              ( f3_tima_rf_wen[i]                                         ),
    .f3_thread_id                ( f3_thread_id                                              ),
    .f3_regfile_wmask            ( f3_regfile_wmask[i]                                       ),
    .f3_regfile_waddr            ( f3_regfile_waddr                                          ),
    .f3_bypass_clk_en            ( f3_bypass_clk_en                                          ),
    .f3_data_vector              ( f3_data_vector                                            ),
    // F4 inputs
    .f4_regfile_wen_l            ( f4_regfile_wen_l                                          ),
    .f4_regfile_waddr_l          ( f4_regfile_waddr_l                                        ),
    .f4_regfile_wdata_l          ( f4_regfile_wdata_l[i*`VPU_DATA_S_SZ+:`VPU_DATA_S_SZ]      ),
    .f4_regfile_wmask_l          ( f4_regfile_wmask_l[i]                                     ),
    .f4_regfile_thrid_l          ( f4_regfile_thrid_l                                        ),
    .f4_regfile_wmask            ( f4_regfile_wmask[i]                                       ),
    .f4_regfile_waddr            ( f4_regfile_waddr                                          ),
    .f4_thread_id                ( f4_thread_id                                              ),
    .f4_bypass_clk_en            ( f4_bypass_clk_en                                          ),
    // F5 inputs
    .f5_thread_id                ( f5_thread_id                                              ),
    .f5_regfile_wmask            ( f5_regfile_wmask[i]                                       ),
    .f5_regfile_waddr            ( f5_regfile_waddr                                          ),
    .f5_bypass_clk_en            ( f5_bypass_clk_en                                          ),
    // F6 inputs
    .f6_thread_id                ( f6_thread_id                                              ),
    .f6_regfile_wmask            ( f6_regfile_wmask[i]                                       ),
    .f6_regfile_waddr            ( f6_regfile_waddr                                          ),
    .f6_bypass_clk_en            ( f6_bypass_clk_en                                          ),
    // F7 inputs
    .f7_thread_id                ( f7_thread_id                                              ),
    .f7_regfile_wmask            ( f7_regfile_wmask[i]                                       ),
    .f7_regfile_waddr            ( f7_regfile_waddr                                          ),
    .f7_bypass_clk_en            ( f7_bypass_clk_en                                          ),
    // F8 inputs
    .f8_thread_id                ( f8_thread_id                                              ),
    .f8_regfile_wmask            ( f8_regfile_wmask[i]                                       ),
    .f8_regfile_wen              ( f8_regfile_wen[i]                                         ),
    .f8_regfile_waddr            ( f8_regfile_waddr                                          ),
    .f8_txfma_op                 ( f8_txfma_op                                               ),
    .f8_data_vector              ( f8_data_vector                                            ),
    // EX outputs
    .ex_regfile_rdata_bypass     ( ex_regfile_rdata_bypass[i]                                ),
    // F2 outputs
    .f2_fswizz_rdata             ( f2_fswizz_rdata[i]                                        ),
    // F3 outputs
    .f3_maskop_bit               ( f3_maskop_bit[i]                                          ),
    // F8 outputs
    .f8_wdata                    ( f8_wdata[i*`VPU_DATA_S_SZ+:`VPU_DATA_S_SZ]                ),
    .f8_wflags                   ( f8_wflags[i]                                              ),
    .f8_sh_sw_wdata              ( f8_sh_sw_wdata[i*`VPU_DATA_S_SZ+:`VPU_DATA_S_SZ]          ),
    .f8_txfma_comp_res           ( f8_txfma_comp_res[i]                                      ),
    // debug outputs
    .txfma_trans_dbg             ( txfma_trans_dbg[i]                                        )
  );
end

endmodule
