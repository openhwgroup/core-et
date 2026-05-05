// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

/* verilator lint_off UNUSEDSIGNAL */  // Imported original lane keeps full scratchpad/helper structs even when some packed bits are unused in this build.
module vpu_lane
#(
    parameter bit EnableRcg2 = EnableRcg2Default,
    parameter bit UseFakeTxfma = UseFakeTxfmaDefault
)
(
    // System signals
    input  logic [$clog2(VPU_LANE_NUM)-1:0]    lane_id,
    input  logic                                clock,
    input  logic                                reset,
    // Chicken bit tima clock gate enable
    input  logic                                chicken_bit_vpulane,
    input  logic                                chicken_bit_vputima,
    // Memory control override
    input  logic [MINION_MEM_CTRL_BITS-1:0]    mem_ctrl_override,
    // ID/F0 data and control inputs
    input  logic [FREG_ADDR_SIZE-1:0]             id_regfile_raddr1,
    input  logic [FREG_ADDR_SIZE-1:0]             id_regfile_raddr2,
    input  logic [FREG_ADDR_SIZE-1:0]             id_regfile_raddr3,
    input  logic [2:0]                          id_regfile_ren,
    input  logic [2:0]                          id_regfile_thread_id,
    input  logic                                id_trans_busy,
    // EX/F1 data and control inputs
    input  logic [FREG_ADDR_SIZE-1:0]             ex_regfile_raddr1,
    input  logic [FREG_ADDR_SIZE-1:0]             ex_regfile_raddr2,
    input  logic [FREG_ADDR_SIZE-1:0]             ex_regfile_raddr3,
    input  logic [2:0]                          ex_regfile_thread_id,
    input  logic                                ex_mask_in1,
    input  logic                                ex_tena_regfile_bypass_en,
    input  logic                                ex_tenb_regfile_bypass_en,
    input  vpu_input_t                          ex_req_lane,
    input  logic [VPU_DATA_S_SZ-1:0]           ex_fromint_data,
    input  vpu_bypass_force_ctrl                ex_bypass_force_ctrl,
    input  logic [VPU_NUM_TIMA-1:0]            ex_tima_valid,
    input  logic                                ex_tima_valid_unqual,
    input  logic                                ex_tima_sa,
    input  logic                                ex_tima_sb,
    input  logic[VPU_TENB_ADDR_W-1:0]                ex_tima_tenb_raddr,
    input  logic [VPU_DATA_S_SZ-1:0]           ex_tena_rf_rdata,
    input  logic [VPU_DATA_S_SZ-1:0]           ex_tena_rf_tima_rdata,
    input  logic                                ex_rom_valid,
    input  logic                                ex_sh_sw_valid,
    input  logic                                ex_txfma_valid,
    input  logic                                ex_txfma_clock_valid,
    input  logic                                ex_rom_clock_valid,
    input  logic                                ex_sh_sw_clock_valid,
    // F2 inputs
    input  logic                                f2_tima_ren3,
    input  logic [VPU_TENC_ADDR_MSB:1]               f2_tima_tenc_raddr,
    input  logic [VPU_NUM_TIMA-1:0]            f2_tima_tenc_wen,
    input  logic [VPU_NUM_TIMA-1:0]            f2_tima_rf_wen,
    input  logic [VPU_DATA_S_SZ-1:0]           f2_fswizz_rdata1,
    input  logic [VPU_DATA_S_SZ-1:0]           f2_fswizz_rdata2,
    input  logic [VPU_DATA_S_SZ-1:0]           f2_fswizz_rdata3,
    input  logic                                f2_tenb_regfile_wen_l,
    input  dcache_vpu_scp_resp                  dcache_scp_resp,
    // F3 inputs
    input  logic                                f3_regfile_wen_l,
    input  logic                                f3_tenb_regfile_wen_l,
    input  logic [VPU_DATA_S_SZ-1:0]           f3_tenb_regfile_wdata_l,
    input  logic [VPU_TENB_ADDR_W-1:0]               f3_tenb_regfile_waddr_l,
    input  logic [VPU_NUM_TIMA-1:0]            f3_tima_tenc_wen,
    input  logic [VPU_TENC_ADDR_MSB:1]               f3_tima_tenc_waddr,
    input  logic [VPU_NUM_TIMA-1:0]            f3_tima_rf_wen,
    input  logic                                f3_thread_id,
    input  logic [$clog2(VPU_REGFILE_NUM)-1:0] f3_regfile_waddr,
    input  logic                                f3_regfile_wmask,
    input  logic                                f3_bypass_clk_en,
    input  logic                                f3_data_vector,
    // F4 inputs
    input  logic                                f4_regfile_wen_l,
    input  logic [$clog2(VPU_REGFILE_NUM)-1:0] f4_regfile_waddr_l,
    input  logic [VPU_DATA_S_SZ-1:0]           f4_regfile_wdata_l,
    input  logic [1:0]                          f4_regfile_wmask_l,
    input  logic                                f4_regfile_thrid_l,
    input  logic                                f4_thread_id,
    input  logic [$clog2(VPU_REGFILE_NUM)-1:0] f4_regfile_waddr,
    input  logic                                f4_regfile_wmask,
    input  logic                                f4_bypass_clk_en,
    // F5 inputs
    input  logic                                f5_thread_id,
    input  logic                                f5_regfile_wmask,
    input  logic [$clog2(VPU_REGFILE_NUM)-1:0] f5_regfile_waddr,
    input  logic                                f5_bypass_clk_en,
    // F6 inputs
    input  logic                                f6_thread_id,
    input  logic                                f6_regfile_wmask,
    input  logic [$clog2(VPU_REGFILE_NUM)-1:0] f6_regfile_waddr,
    input  logic                                f6_bypass_clk_en,
    // F7 inputs
    input  logic                                f7_thread_id,
    input  logic                                f7_regfile_wmask,
    input  logic [$clog2(VPU_REGFILE_NUM)-1:0] f7_regfile_waddr,
    input  logic                                f7_bypass_clk_en,
    input  logic                                f7_regfile_wen,
    // F6 inputs
    input  logic                                f8_thread_id,
    input  logic                                f8_regfile_wmask,
    input  logic [$clog2(VPU_REGFILE_NUM)-1:0] f8_regfile_waddr,
    input  logic                                f8_regfile_wen,
    input  logic                                f8_txfma_op,
    input  logic                                f8_data_vector,
    // EX outputs
    output logic [1:0][VPU_DATA_S_SZ-1:0]      ex_regfile_rdata_bypass,
    // F2 outputs
    output logic [VPU_DATA_S_SZ-1:0]           f2_fswizz_rdata,
    // F3 outputs
    output logic                                f3_maskop_bit,
    // F8 outputs
    output logic                                f8_txfma_comp_res,
    output logic [VPU_DATA_S_SZ-1:0]           f8_wdata,
    output logic [CORE_FCSR_FLAG_BITS_SZ-1:0]  f8_wflags,
    output logic [VPU_DATA_S_SZ-1:0]           f8_sh_sw_wdata,
    //debug
    output logic [1:0]                          txfma_trans_dbg
);


// Signal Declarations
logic [2:0][FREG_ADDR_SIZE-1:0]                 id_rf_raddr;                  // Read addr for the RF
logic [2:0]                                   id_rf_thread_id;              // Read thread id for the RF
logic [2:0][VPU_DATA_S_SZ-1:0]               ex_rf_rdata;                  // Read data from the RF
vpu_input_t                                   ex_req;
logic [VPU_DATA_S_SZ-1:0]                    ex_bypass_tena_rf_rdata;      // Read data from the tena RF
logic [VPU_DATA_S_SZ-1:0]                    ex_bypass_tenb_rf_rdata;      // Read data from the tenb RF
logic [VPU_NUM_TIMA-1:0][VPU_DATA_S_SZ-1:0] ex_tenb_rf_rdata;             // Read data from the tenb RF
logic [2:0][FREG_ADDR_SIZE-1:0]                 ex_rf_bypass_raddr;           // Read addr for the RF
logic [VPU_NUM_TIMA-1:0][VPU_TENB_ADDR_W-1:0]     ex_tenb_rf_raddr;             // Read addr for the tenb RF
logic                                         f2_tenb_rf_wen;
vpu_output_t                                  f3_sh_sw_bits;
logic [VPU_NUM_TIMA-1:0][VPU_DATA_S_SZ-1:0] f3_tima_data;
logic [VPU_TENB_ADDR_W-1:0]                        f3_tenb_rf_waddr;             // Write addr for the tenb RF
logic                                         f3_tenb_pass;
logic                                         f3_tenb_rf_wen;
logic [VPU_DATA_S_SZ-1:0]                    f3_tenb_rf_wdata;             // Write data for the tenb RF
logic [VPU_NUM_TIMA-1:0]                     f2_tima_valid;
logic [VPU_NUM_TIMA-1:0]                     f3_tima_valid;
logic [1:0]                                   f7_rf_wen_early;
vpu_output_t                                  f8_txfma_res;
logic [1:0][VPU_DATA_S_SZ-1:0]               f8_rf_wdata;                  // Write data for the RF
logic [1:0]                                   f8_rf_wmask;                  // Write mask for the RF
logic [1:0][FREG_ADDR_SIZE-1:0]                 f8_rf_waddr;                  // Write addr for the RF
logic [1:0]                                   f8_rf_thrid;                  // Write thread ID for the RF
logic [1:0]                                   f8_rf_wen;                    // Write enables for the RF
logic [2:0]                                   id_regfile_ren_ovr;           // Read enables override for the RF
logic [1:0]                                   id_regfile_wen_ovr;           // Write enables override for the RF


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Clock gating for all vpu sub-modules
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Once clock is enabled, keep it enabled until the functional units are empty

logic                             clock_txfma;     // Clock for TXFMA
logic                             clock_trans_rom; // Clock for trans ROM
logic                             clock_sh_sw;     // Clock for sh-sw
logic                             clock_bypass;    // Clock for bypass
logic                             clock_en_tima;

logic                                         txfma_clock_valid_comb;
logic [TXFMA_CLOCK_EN_DELAY-1:0]             txfma_clock_valid_delay;
logic                                         txfma_clock_valid_delay_en;
logic [TXFMA_CLOCK_EN_DELAY-1:0]             txfma_clock_valid_delay_next;
logic                                         clock_en_txfma;
logic                                         trans_rom_clock_valid_comb;
logic [TRANS_ROM_CLOCK_EN_DELAY-1:0]         trans_rom_clock_valid_delay;
logic                                         trans_rom_clock_valid_delay_en;
logic [TRANS_ROM_CLOCK_EN_DELAY-1:0]         trans_rom_clock_valid_delay_next;
logic                                         clock_en_trans_rom;
logic                                         sh_sw_clock_valid_comb;
logic [SH_SW_CLOCK_EN_DELAY-1:0]             sh_sw_clock_valid_delay;
logic                                         sh_sw_clock_valid_delay_en;
logic [SH_SW_CLOCK_EN_DELAY-1:0]             sh_sw_clock_valid_delay_next;
logic                                         clock_en_sh_sw;
logic                                         clock_en_bypass;
logic                                         tima_clock_valid_comb;
logic                                         tima_clock_valid_delay_en;
logic [TIMA_CLOCK_EN_DELAY-1:0]              tima_clock_valid_delay;
logic [TIMA_CLOCK_EN_DELAY-1:0]              tima_clock_valid_delay_next;

generate
if (EnableRcg2) begin : gen_rcg2

  always_ff @(posedge clock) begin
    if (reset) begin
      txfma_clock_valid_delay <= {TXFMA_CLOCK_EN_DELAY{1'b0}};
      trans_rom_clock_valid_delay <= {TRANS_ROM_CLOCK_EN_DELAY{1'b0}};
      sh_sw_clock_valid_delay <= {SH_SW_CLOCK_EN_DELAY{1'b0}};
      tima_clock_valid_delay <= {TIMA_CLOCK_EN_DELAY{1'b0}};
    end else begin
      if (txfma_clock_valid_delay_en) begin
        txfma_clock_valid_delay <= txfma_clock_valid_delay_next;
      end
      if (trans_rom_clock_valid_delay_en) begin
        trans_rom_clock_valid_delay <= trans_rom_clock_valid_delay_next;
      end
      if (sh_sw_clock_valid_delay_en) begin
        sh_sw_clock_valid_delay <= sh_sw_clock_valid_delay_next;
      end
      if (tima_clock_valid_delay_en) begin
        tima_clock_valid_delay <= tima_clock_valid_delay_next;
      end
    end
  end

    //txfma
  assign txfma_clock_valid_comb = ex_txfma_clock_valid | id_trans_busy;
  assign txfma_clock_valid_delay_next = txfma_clock_valid_comb ? {TXFMA_CLOCK_EN_DELAY{1'b1}}
                                      : {txfma_clock_valid_delay[TXFMA_CLOCK_EN_DELAY-2:0],txfma_clock_valid_comb};
  assign clock_en_txfma = ex_txfma_clock_valid | txfma_clock_valid_delay[TXFMA_CLOCK_EN_DELAY-1];
  assign txfma_clock_valid_delay_en = txfma_clock_valid_comb | txfma_clock_valid_delay[TXFMA_CLOCK_EN_DELAY-1];

    //trans
  assign trans_rom_clock_valid_comb = ex_rom_clock_valid | id_trans_busy;
  assign trans_rom_clock_valid_delay_next = trans_rom_clock_valid_comb ? {TRANS_ROM_CLOCK_EN_DELAY{1'b1}}
                                      : {trans_rom_clock_valid_delay[TRANS_ROM_CLOCK_EN_DELAY-2:0],trans_rom_clock_valid_comb};
  assign clock_en_trans_rom = ex_rom_clock_valid | trans_rom_clock_valid_delay[TRANS_ROM_CLOCK_EN_DELAY-1];
  assign trans_rom_clock_valid_delay_en = trans_rom_clock_valid_comb | trans_rom_clock_valid_delay[TRANS_ROM_CLOCK_EN_DELAY-1];

    //sh-sw
  assign sh_sw_clock_valid_comb = ex_sh_sw_clock_valid;
  assign sh_sw_clock_valid_delay_next = sh_sw_clock_valid_comb ? {SH_SW_CLOCK_EN_DELAY{1'b1}}
                                      : {sh_sw_clock_valid_delay[SH_SW_CLOCK_EN_DELAY-2:0],sh_sw_clock_valid_comb};
  assign clock_en_sh_sw = ex_sh_sw_clock_valid | sh_sw_clock_valid_delay[SH_SW_CLOCK_EN_DELAY-1];
  assign sh_sw_clock_valid_delay_en = sh_sw_clock_valid_comb | sh_sw_clock_valid_delay[SH_SW_CLOCK_EN_DELAY-1];

    //tima
  assign tima_clock_valid_comb = ex_tima_valid_unqual;
  assign tima_clock_valid_delay_next =  tima_clock_valid_comb ? {TIMA_CLOCK_EN_DELAY{1'b1}} : {tima_clock_valid_delay[TIMA_CLOCK_EN_DELAY-2:0],tima_clock_valid_comb};
  assign clock_en_tima = ex_tima_valid_unqual | tima_clock_valid_delay[TIMA_CLOCK_EN_DELAY-1];
  assign tima_clock_valid_delay_en = tima_clock_valid_comb | tima_clock_valid_delay[TIMA_CLOCK_EN_DELAY-1];

    //bypass
  assign clock_en_bypass = f3_bypass_clk_en | f4_bypass_clk_en | f5_bypass_clk_en | f6_bypass_clk_en | f7_bypass_clk_en;

  // Clock for TXFMA
  prim_clk_gate cgate_txfma (
    .clk_i ( clock                                    ),
    .en_i  ( clock_en_txfma | chicken_bit_vpulane     ),
    .dft_i ( '0                                       ),
    .clk_o ( clock_txfma                              )
  );

  // Clock for TRANS ROM
  prim_clk_gate cgate_trans_rom (
    .clk_i ( clock                                    ),
    .en_i  ( clock_en_trans_rom | chicken_bit_vpulane ),
    .dft_i ( '0                                       ),
    .clk_o ( clock_trans_rom                          )
  );

  // Clock for SH SW
  prim_clk_gate cgate_sh_sw (
    .clk_i ( clock                                    ),
    .en_i  ( clock_en_sh_sw | chicken_bit_vpulane     ),
    .dft_i ( '0                                       ),
    .clk_o ( clock_sh_sw                              )
  );

  // Clock for bypass
  prim_clk_gate cgate_bypass (
    .clk_i ( clock                                    ),
    .en_i  ( clock_en_bypass | chicken_bit_vpulane    ),
    .dft_i ( '0                                       ),
    .clk_o ( clock_bypass                             )
  );
end else begin : gen_no_rcg2
  assign clock_txfma     = clock;
  assign clock_trans_rom = clock;
  assign clock_sh_sw     = clock;
  assign clock_bypass    = clock;
  assign clock_en_tima   = 1'b1;
end
endgenerate



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// ID Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Register file
////////////////////////////////////////////////////////////////////////////////

assign id_rf_raddr     = { id_regfile_raddr3,
                           id_regfile_raddr2,
                           id_regfile_raddr1 };
assign id_rf_thread_id =   id_regfile_thread_id;


// Override read and write signals for testing
assign id_regfile_ren_ovr = id_regfile_ren & {3{!mem_ctrl_override[MINION_MEM_CTRL_RE]}};
assign id_regfile_wen_ovr = f8_rf_wen & {2{!mem_ctrl_override[MINION_MEM_CTRL_WE]}};


vpu_rf
rf
(
  // System signal
  .clk_i       ( clock                ),
  // Read ports
  .rd_en_i     ( id_regfile_ren_ovr   ),
  .rd_thread_id_i( id_rf_thread_id      ),
  .rd_addr_i   ( id_rf_raddr          ),
  .rd_data_o   ( ex_rf_rdata          ),
  // Write ports
  .wr_en_i     ( id_regfile_wen_ovr   ),
  .wr_en_early_i( f7_rf_wen_early[1:0] ),
  .wr_thread_id_i( f8_rf_thrid[1:0]     ),
  .wr_mask_i   ( f8_rf_wmask[1:0]     ),
  .wr_addr_i   ( f8_rf_waddr[1:0]     ),
  .wr_data_i   ( f8_rf_wdata[1:0]     )
);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// EX/F1 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Register File Bypass
////////////////////////////////////////////////////////////////////////////////

assign ex_rf_bypass_raddr = { ex_regfile_raddr3,
                              ex_regfile_raddr2,
                              ex_regfile_raddr1 };
assign ex_bypass_tena_rf_rdata = ex_tena_rf_rdata;
assign ex_bypass_tenb_rf_rdata = (ex_tenb_rf_raddr[0][0] ? ex_tenb_rf_rdata[1] : ex_tenb_rf_rdata[0]) & {VPU_DATA_S_SZ{!ex_tima_valid_unqual}}; // Selects data from correct TENB

vpu_bypass
bypass
(
  // System signals
  .lane_id_i                   ( lane_id                     ),
  .clk_i                       ( clock_bypass                ),
  // EX/F1 inputs
  .ex_rf_raddr_i               ( ex_rf_bypass_raddr          ),
  .ex_regfile_thread_id_i      ( ex_regfile_thread_id        ),
  .ex_rf_rdata_i               ( ex_rf_rdata                 ),
  .ex_req_lane_i               ( ex_req_lane                 ),
  .ex_fromint_data_i           ( ex_fromint_data             ),
  .ex_tena_rf_rdata_i          ( ex_bypass_tena_rf_rdata     ),
  .ex_tenb_rf_rdata_i          ( ex_bypass_tenb_rf_rdata     ),
  .ex_tena_regfile_bypass_en_i ( ex_tena_regfile_bypass_en   ),
  .ex_tenb_regfile_bypass_en_i ( ex_tenb_regfile_bypass_en   ),
  .ex_bypass_force_ctrl_i      ( ex_bypass_force_ctrl        ),
  // F3 inputs
  .f3_sh_sw_bits_i             ( f3_sh_sw_bits               ),
  .f3_regfile_waddr_i          ( f3_regfile_waddr            ),
  .f3_thread_id_i              ( f3_thread_id                ),
  .f3_regfile_wmask_i          ( f3_regfile_wmask            ),
  .f3_bypass_clk_en_i          ( f3_bypass_clk_en            ),
  .f3_data_vector_i            ( f3_data_vector              ),
  // F4 inputs
  .f4_regfile_waddr_l_i        ( f4_regfile_waddr_l          ),
  .f4_regfile_waddr_i          ( f4_regfile_waddr            ),
  .f4_regfile_wmask_l_i        ( f4_regfile_wmask_l          ),
  .f4_regfile_wmask_i          ( f4_regfile_wmask            ),
  .f4_regfile_thread_id_l_i    ( f4_regfile_thrid_l          ),
  .f4_thread_id_i              ( f4_thread_id                ),
  .f4_regfile_wdata_l_i        ( f4_regfile_wdata_l          ),
  .f4_bypass_clk_en_i          ( f4_bypass_clk_en            ),
  // F5 inputs
  .f5_regfile_waddr_i          ( f5_regfile_waddr            ),
  .f5_regfile_wmask_i          ( f5_regfile_wmask            ),
  .f5_thread_id_i              ( f5_thread_id                ),
  .f5_bypass_clk_en_i          ( f5_bypass_clk_en            ),
  // F6 inputs
  .f6_regfile_waddr_i          ( f6_regfile_waddr            ),
  .f6_regfile_wmask_i          ( f6_regfile_wmask            ),
  .f6_thread_id_i              ( f6_thread_id                ),
  .f6_bypass_clk_en_i          ( f6_bypass_clk_en            ),
  // F7 inputs
  .f7_regfile_waddr_i          ( f7_regfile_waddr            ),
  .f7_regfile_wmask_i          ( f7_regfile_wmask            ),
  .f7_thread_id_i              ( f7_thread_id                ),
  .f7_bypass_clk_en_i          ( f7_bypass_clk_en            ),
  // F8 inputs
  .f8_regfile_waddr_i          ( f8_regfile_waddr            ),
  .f8_regfile_wmask_i          ( f8_regfile_wmask            ),
  .f8_thread_id_i              ( f8_thread_id                ),
  .f8_txfma_res_i              ( f8_txfma_res                ),
  .f8_data_vector_i            ( f8_data_vector              ),
  .f8_txfma_op_i               ( f8_txfma_op                 ),
  // EX/F1 outputs
  .ex_req_o                    ( ex_req                      ),
  // F3 outputs
  .f3_maskop_bit_o             ( f3_maskop_bit               ),
  // F8 outputs
  .f8_wdata_o                  ( f8_wdata                    ),
  .f8_wflags_o                 ( f8_wflags                   ),
  .f8_sh_sw_wdata_o            ( f8_sh_sw_wdata              )
);

assign ex_regfile_rdata_bypass[0] = ex_req.in1;
assign ex_regfile_rdata_bypass[1] = ex_req.in2;

///////////////////////////////////////////////////////////////////////////////
// TXFMA-TRANS
///////////////////////////////////////////////////////////////////////////////

vpu_txfma_trans_top
#(
    .UseFakeTxfma      ( UseFakeTxfma        )
)
txfma_trans_top
(
  .clock_txfma         ( clock_txfma         ),
  .clock_trans_rom     ( clock_trans_rom     ),
  .reset               ( reset               ),
  .ex_rom_valid        ( ex_rom_valid        ),
  .ex_in_bits          ( ex_req              ),
  .ex_txfma_valid      ( ex_txfma_valid      ),
  .f8_txfma_res        ( f8_txfma_res        ),
  .f8_txfma_comp_res   ( f8_txfma_comp_res   ),
  .txfma_trans_dbg     ( txfma_trans_dbg     )
);

///////////////////////////////////////////////////////////////////////////////
// VPU lane TIMA
///////////////////////////////////////////////////////////////////////////////

assign ex_tenb_rf_raddr[0] = ex_tima_valid_unqual ? ex_tima_tenb_raddr                                       : ex_regfile_raddr2[VPU_TENB_ADDR_W-1:0];
assign ex_tenb_rf_raddr[1] = ex_tima_valid_unqual ? { ex_tima_tenb_raddr[$clog2(VPU_TENB_SIZE)-1:1], 1'b1 } : ex_regfile_raddr2[VPU_TENB_ADDR_W-1:0]; // Second port always read odd entries for TIMA operations

genvar gen_it;

generate for(gen_it = 0; gen_it < VPU_NUM_TIMA; gen_it++)
begin : LANE_TIMA

  logic       f3_tenb_wr_en;

  always_comb
  begin
    //tenb write enable
    f3_tenb_wr_en = f3_tenb_rf_wen;

    if (gen_it==0)
      f3_tenb_wr_en &= ~f3_tenb_rf_waddr[0];
    else
      f3_tenb_wr_en &= f3_tenb_rf_waddr[0];
  end

  /* verilator lint_off UNUSEDSIGNAL */  // Imported original lane keeps the full scratchpad response struct even when this lane only consumes a subset of the bits.
  vpu_lane_tima
  lane_tima
  (
    // System signals
    .te_i              ( 1'b0                                                 ),
    .clk_i             ( clock                                                ),
    .tima_clk_en_i     ( clock_en_tima | chicken_bit_vputima                  ),
    // F1 inputs
    .f1_tima_mul_en_i  ( ex_tima_valid[gen_it]                                ),
    .tima_in1_signed_i ( ex_tima_sa                                           ),
    .tima_in2_signed_i ( ex_tima_sb                                           ),
    // F2 inputs
    .f2_tima_en_i      ( f2_tima_valid[gen_it]                                ),
    .f2_tima_in3_en_i  ( f2_tima_ren3                                         ),
    .f2_tima_zap_opdc_i(!f2_tima_ren3                                         ),
    // TEN A inputs
    .tena_rd_data_i    ( ex_tena_rf_tima_rdata                                ),
    .tenb_wr_en_early_i(f2_tenb_rf_wen                                        ),
    .tenb_wr_en_i      ( f3_tenb_wr_en                                        ),
    .tenb_rd_addr_i    ( ex_tenb_rf_raddr[gen_it][$clog2(VPU_TENB_SIZE)-1:1] ),
    .tenb_wr_addr_i    ( f3_tenb_rf_waddr[$clog2(VPU_TENB_SIZE)-1:1]         ),
    .tenb_wr_data_i    ( f3_tenb_rf_wdata                                     ),
    // TEN C inputs
    .tenc_wr_en_early_i(f2_tima_tenc_wen[gen_it] & f2_tima_valid[gen_it]     ),
    .tenc_rd_addr_i    ( f2_tima_tenc_raddr                                   ),
    .tenc_wr_addr_i    ( f3_tima_tenc_waddr                                   ),
    .tenc_wr_en_i      ( f3_tima_tenc_wen[gen_it] & f3_tima_valid[gen_it]     ),
    // F3 outputs
    .tenb_out_o        ( ex_tenb_rf_rdata[gen_it]                             ),
    .tima_out_o        ( f3_tima_data[gen_it]                                 )
  );
  /* verilator lint_on UNUSEDSIGNAL */

end
endgenerate


///////////////////////////////////////////////////////////////////////////////
// Short-swizzle unit
///////////////////////////////////////////////////////////////////////////////
vpu_sh_sw
sh_sw
(
  .lane_id_i         ( lane_id           ),
  .clk_i             ( clock_sh_sw       ),
  .rst_ni            ( ~reset            ),
  .ex_in_valid_i     ( ex_sh_sw_valid    ),
  .ex_cmd_i          ( ex_req.sigs.cmd   ),
  .ex_in1_i          ( ex_req.in1        ),
  .ex_in2_i          ( ex_req.in2        ),
  .ex_in3_i          ( ex_req.in3        ),
  .ex_imm_i          ( ex_req.imm        ),
  .ex_mask_in1_i     ( ex_mask_in1       ),
  .ex_dtype_i        ( ex_req.sigs.dtype ),
  .f2_swizz_in1_i    ( f2_fswizz_rdata1  ),
  .f2_swizz_in2_i    ( f2_fswizz_rdata2  ),
  .f2_swizz_in3_i    ( f2_fswizz_rdata3  ),
  .f2_fswizz_rdata_o ( f2_fswizz_rdata   ),
  .f3_out_bits_o     ( f3_sh_sw_bits     )
);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F2 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
always_comb
begin
    f2_tenb_rf_wen = f2_tenb_regfile_wen_l | dcache_scp_resp.fill_is_tenb_early;
end



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F3 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

always_ff @(posedge clock) begin
    if (reset) begin
        f3_tenb_pass <= 1'b0;
        f2_tima_valid <= '0;
        f3_tima_valid <= '0;
    end else begin
        if (dcache_scp_resp.fill_is_tenb) begin
            f3_tenb_pass <= ~f3_tenb_pass;
        end
        f2_tima_valid <= ex_tima_valid;
        f3_tima_valid <= f2_tima_valid;
    end
end

////////////////////////////////////////////////////////////////////////////////
// Tensor B/C register file
////////////////////////////////////////////////////////////////////////////////

always_comb
begin
    f3_tenb_rf_wen    = f3_tenb_regfile_wen_l | dcache_scp_resp.fill_is_tenb;
    f3_tenb_rf_waddr  = dcache_scp_resp.fill_is_tenb ? { dcache_scp_resp.tenb_line[1:0], f3_tenb_pass } : f3_tenb_regfile_waddr_l;
    f3_tenb_rf_wdata  = f3_tenb_regfile_wdata_l;
end
/* verilator lint_on UNUSEDSIGNAL */

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F7 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Early write enable to activate data input latch in RF
assign f7_rf_wen_early[0] = f7_regfile_wen   | f2_tima_rf_wen[0];
assign f7_rf_wen_early[1] = f3_regfile_wen_l | f2_tima_rf_wen[1];

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// F8 Stage
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Functional units port
assign f8_rf_wmask[0] = f3_tima_rf_wen[0]                                | (f8_regfile_wen & f8_regfile_wmask);
assign f8_rf_wdata[0] = f3_tima_rf_wen[0] ? f3_tima_data[0]              : f8_wdata;
assign f8_rf_waddr[0] = f3_tima_rf_wen[0] ? { f3_tima_tenc_waddr, 1'b0 } : f8_regfile_waddr;
assign f8_rf_thrid[0] = f3_tima_rf_wen[0] ? 1'b0                         : f8_thread_id;
assign f8_rf_wen[0]   = f3_tima_rf_wen[0]                                | f8_regfile_wen;

// Load port (it might write at any stage)
assign f8_rf_wmask[1] = f3_tima_rf_wen[1]                                | f4_regfile_wmask_l[0];
assign f8_rf_wdata[1] = f3_tima_rf_wen[1] ? f3_tima_data[1]              : f4_regfile_wdata_l;
assign f8_rf_waddr[1] = f3_tima_rf_wen[1] ? { f3_tima_tenc_waddr, 1'b1 } : f4_regfile_waddr_l;
assign f8_rf_thrid[1] = f3_tima_rf_wen[1] ? 1'b0                         : f4_regfile_thrid_l;
assign f8_rf_wen[1]   = f3_tima_rf_wen[1]                                | f4_regfile_wen_l;


endmodule
/* verilator lint_on UNUSEDSIGNAL */
