// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

`include "soc.vh"

/* verilator lint_off IMPORTSTAR */
import dft_pkg::*;
import etlink_pkg::*;
import minion_pkg::*;
import minion_dcache_pkg::*;
import vpu_pkg::*;
import vpu_defs_pkg::*;
/* verilator lint_on IMPORTSTAR */

// ORIG_RENAME appends _orig to the vpu_bypass prefix inside the original
// packed typedef name vpu_bypass_force_ctrl. Recreate that renamed typedef so
// the renamed original lane/bypass modules elaborate.
typedef vpu_bypass_force_ctrl vpu_bypass_orig_force_ctrl;

typedef struct packed {
    logic [$clog2(VPU_LANE_NUM)-1:0] lane_id;
    logic chicken_bit_vpulane;
    logic chicken_bit_vputima;
    logic [MINION_MEM_CTRL_BITS-1:0] mem_ctrl_override;
    logic [FREG_ADDR_SIZE-1:0] id_regfile_raddr1;
    logic [FREG_ADDR_SIZE-1:0] id_regfile_raddr2;
    logic [FREG_ADDR_SIZE-1:0] id_regfile_raddr3;
    logic [2:0] id_regfile_ren;
    logic [2:0] id_regfile_thread_id;
    logic id_trans_busy;
    logic [FREG_ADDR_SIZE-1:0] ex_regfile_raddr1;
    logic [FREG_ADDR_SIZE-1:0] ex_regfile_raddr2;
    logic [FREG_ADDR_SIZE-1:0] ex_regfile_raddr3;
    logic [2:0] ex_regfile_thread_id;
    logic ex_mask_in1;
    logic ex_tena_regfile_bypass_en;
    logic ex_tenb_regfile_bypass_en;
    logic [$bits(vpu_pkg::vpu_input_t)-1:0] ex_req_lane;
    logic [VPU_DATA_S_SZ-1:0] ex_fromint_data;
    logic [$bits(vpu_pkg::vpu_bypass_force_ctrl_t)-1:0] ex_bypass_force_ctrl;
    logic [VPU_NUM_TIMA-1:0] ex_tima_valid;
    logic ex_tima_valid_unqual;
    logic ex_tima_sa;
    logic ex_tima_sb;
    logic[VPU_TENB_ADDR_W-1:0] ex_tima_tenb_raddr;
    logic [VPU_DATA_S_SZ-1:0] ex_tena_rf_rdata;
    logic [VPU_DATA_S_SZ-1:0] ex_tena_rf_tima_rdata;
    logic ex_rom_valid;
    logic ex_sh_sw_valid;
    logic ex_txfma_valid;
    logic ex_txfma_clock_valid;
    logic ex_rom_clock_valid;
    logic ex_sh_sw_clock_valid;
    logic f2_tima_ren3;
    logic [VPU_TENC_ADDR_MSB:1] f2_tima_tenc_raddr;
    logic [VPU_NUM_TIMA-1:0] f2_tima_tenc_wen;
    logic [VPU_NUM_TIMA-1:0] f2_tima_rf_wen;
    logic [VPU_DATA_S_SZ-1:0] f2_fswizz_rdata1;
    logic [VPU_DATA_S_SZ-1:0] f2_fswizz_rdata2;
    logic [VPU_DATA_S_SZ-1:0] f2_fswizz_rdata3;
    logic f2_tenb_regfile_wen_l;
    logic [$bits(minion_pkg::dcache_vpu_scp_resp)-1:0] dcache_scp_resp;
    logic f3_regfile_wen_l;
    logic f3_tenb_regfile_wen_l;
    logic [VPU_DATA_S_SZ-1:0] f3_tenb_regfile_wdata_l;
    logic [VPU_TENB_ADDR_W-1:0] f3_tenb_regfile_waddr_l;
    logic [VPU_NUM_TIMA-1:0] f3_tima_tenc_wen;
    logic [VPU_TENC_ADDR_MSB:1] f3_tima_tenc_waddr;
    logic [VPU_NUM_TIMA-1:0] f3_tima_rf_wen;
    logic f3_thread_id;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f3_regfile_waddr;
    logic f3_regfile_wmask;
    logic f3_bypass_clk_en;
    logic f3_data_vector;
    logic f4_regfile_wen_l;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f4_regfile_waddr_l;
    logic [VPU_DATA_S_SZ-1:0] f4_regfile_wdata_l;
    logic [1:0] f4_regfile_wmask_l;
    logic f4_regfile_thrid_l;
    logic f4_thread_id;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f4_regfile_waddr;
    logic f4_regfile_wmask;
    logic f4_bypass_clk_en;
    logic f5_thread_id;
    logic f5_regfile_wmask;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f5_regfile_waddr;
    logic f5_bypass_clk_en;
    logic f6_thread_id;
    logic f6_regfile_wmask;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f6_regfile_waddr;
    logic f6_bypass_clk_en;
    logic f7_thread_id;
    logic f7_regfile_wmask;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f7_regfile_waddr;
    logic f7_bypass_clk_en;
    logic f7_regfile_wen;
    logic f8_thread_id;
    logic f8_regfile_wmask;
    logic [$clog2(VPU_REGFILE_NUM)-1:0] f8_regfile_waddr;
    logic f8_regfile_wen;
    logic f8_txfma_op;
    logic f8_data_vector;
} vpu_lane_inputs_t;

typedef struct packed {
    logic [1:0][VPU_DATA_S_SZ-1:0] ex_regfile_rdata_bypass;
    logic [VPU_DATA_S_SZ-1:0] f2_fswizz_rdata;
    logic f3_maskop_bit;
    logic f8_txfma_comp_res;
    logic [VPU_DATA_S_SZ-1:0] f8_wdata;
    logic [CORE_FCSR_FLAG_BITS_SZ-1:0] f8_wflags;
    logic [VPU_DATA_S_SZ-1:0] f8_sh_sw_wdata;
    logic [1:0] txfma_trans_dbg;
} vpu_lane_outputs_t;

module cosim_vpu_lane_tb (
  input  logic clk_i,
  input  logic rst_ni,
  input  logic [$bits(vpu_lane_inputs_t)-1:0] inputs_i,
  output logic [$bits(vpu_lane_outputs_t)-1:0] orig_outputs_o,
  output logic [$bits(vpu_lane_outputs_t)-1:0] new_outputs_o
);

  vpu_lane_inputs_t in;
  vpu_lane_outputs_t orig_out;
  vpu_lane_outputs_t new_out;

  assign in = vpu_lane_inputs_t'(inputs_i);
  assign orig_outputs_o = orig_out;
  assign new_outputs_o = new_out;
  vpu_input_t orig_ex_req_lane;
  vpu_pkg::vpu_input_t new_ex_req_lane;
  assign orig_ex_req_lane = vpu_input_t'(in.ex_req_lane);
  assign new_ex_req_lane = vpu_pkg::vpu_input_t'(in.ex_req_lane);
  vpu_bypass_force_ctrl orig_ex_bypass_force_ctrl;
  vpu_pkg::vpu_bypass_force_ctrl_t new_ex_bypass_force_ctrl;
  assign orig_ex_bypass_force_ctrl = vpu_bypass_force_ctrl'(in.ex_bypass_force_ctrl);
  assign new_ex_bypass_force_ctrl = vpu_pkg::vpu_bypass_force_ctrl_t'(in.ex_bypass_force_ctrl);
  dcache_vpu_scp_resp orig_dcache_scp_resp;
  minion_pkg::dcache_vpu_scp_resp new_dcache_scp_resp;
  assign orig_dcache_scp_resp = dcache_vpu_scp_resp'(in.dcache_scp_resp);
  assign new_dcache_scp_resp = minion_pkg::dcache_vpu_scp_resp'(in.dcache_scp_resp);

  vpu_lane_orig #(
    .USE_MMI (1'b0)
  ) u_orig (
    .lane_id                      (in.lane_id),
    .clock                        (clk_i),
    .reset                        (~rst_ni),
    .chicken_bit_vpulane          (in.chicken_bit_vpulane),
    .chicken_bit_vputima          (in.chicken_bit_vputima),
    .mem_ctrl_override            (in.mem_ctrl_override),
    .id_regfile_raddr1            (in.id_regfile_raddr1),
    .id_regfile_raddr2            (in.id_regfile_raddr2),
    .id_regfile_raddr3            (in.id_regfile_raddr3),
    .id_regfile_ren               (in.id_regfile_ren),
    .id_regfile_thread_id         (in.id_regfile_thread_id),
    .id_trans_busy                (in.id_trans_busy),
    .ex_regfile_raddr1            (in.ex_regfile_raddr1),
    .ex_regfile_raddr2            (in.ex_regfile_raddr2),
    .ex_regfile_raddr3            (in.ex_regfile_raddr3),
    .ex_regfile_thread_id         (in.ex_regfile_thread_id),
    .ex_mask_in1                  (in.ex_mask_in1),
    .ex_tena_regfile_bypass_en    (in.ex_tena_regfile_bypass_en),
    .ex_tenb_regfile_bypass_en    (in.ex_tenb_regfile_bypass_en),
    .ex_req_lane                  (orig_ex_req_lane),
    .ex_fromint_data              (in.ex_fromint_data),
    .ex_bypass_force_ctrl         (orig_ex_bypass_force_ctrl),
    .ex_tima_valid                (in.ex_tima_valid),
    .ex_tima_valid_unqual         (in.ex_tima_valid_unqual),
    .ex_tima_sa                   (in.ex_tima_sa),
    .ex_tima_sb                   (in.ex_tima_sb),
    .ex_tima_tenb_raddr           (in.ex_tima_tenb_raddr),
    .ex_tena_rf_rdata             (in.ex_tena_rf_rdata),
    .ex_tena_rf_tima_rdata        (in.ex_tena_rf_tima_rdata),
    .ex_rom_valid                 (in.ex_rom_valid),
    .ex_sh_sw_valid               (in.ex_sh_sw_valid),
    .ex_txfma_valid               (in.ex_txfma_valid),
    .ex_txfma_clock_valid         (in.ex_txfma_clock_valid),
    .ex_rom_clock_valid           (in.ex_rom_clock_valid),
    .ex_sh_sw_clock_valid         (in.ex_sh_sw_clock_valid),
    .f2_tima_ren3                 (in.f2_tima_ren3),
    .f2_tima_tenc_raddr           (in.f2_tima_tenc_raddr),
    .f2_tima_tenc_wen             (in.f2_tima_tenc_wen),
    .f2_tima_rf_wen               (in.f2_tima_rf_wen),
    .f2_fswizz_rdata1             (in.f2_fswizz_rdata1),
    .f2_fswizz_rdata2             (in.f2_fswizz_rdata2),
    .f2_fswizz_rdata3             (in.f2_fswizz_rdata3),
    .f2_tenb_regfile_wen_l        (in.f2_tenb_regfile_wen_l),
    .dcache_scp_resp              (orig_dcache_scp_resp),
    .f3_regfile_wen_l             (in.f3_regfile_wen_l),
    .f3_tenb_regfile_wen_l        (in.f3_tenb_regfile_wen_l),
    .f3_tenb_regfile_wdata_l      (in.f3_tenb_regfile_wdata_l),
    .f3_tenb_regfile_waddr_l      (in.f3_tenb_regfile_waddr_l),
    .f3_tima_tenc_wen             (in.f3_tima_tenc_wen),
    .f3_tima_tenc_waddr           (in.f3_tima_tenc_waddr),
    .f3_tima_rf_wen               (in.f3_tima_rf_wen),
    .f3_thread_id                 (in.f3_thread_id),
    .f3_regfile_waddr             (in.f3_regfile_waddr),
    .f3_regfile_wmask             (in.f3_regfile_wmask),
    .f3_bypass_clk_en             (in.f3_bypass_clk_en),
    .f3_data_vector               (in.f3_data_vector),
    .f4_regfile_wen_l             (in.f4_regfile_wen_l),
    .f4_regfile_waddr_l           (in.f4_regfile_waddr_l),
    .f4_regfile_wdata_l           (in.f4_regfile_wdata_l),
    .f4_regfile_wmask_l           (in.f4_regfile_wmask_l),
    .f4_regfile_thrid_l           (in.f4_regfile_thrid_l),
    .f4_thread_id                 (in.f4_thread_id),
    .f4_regfile_waddr             (in.f4_regfile_waddr),
    .f4_regfile_wmask             (in.f4_regfile_wmask),
    .f4_bypass_clk_en             (in.f4_bypass_clk_en),
    .f5_thread_id                 (in.f5_thread_id),
    .f5_regfile_wmask             (in.f5_regfile_wmask),
    .f5_regfile_waddr             (in.f5_regfile_waddr),
    .f5_bypass_clk_en             (in.f5_bypass_clk_en),
    .f6_thread_id                 (in.f6_thread_id),
    .f6_regfile_wmask             (in.f6_regfile_wmask),
    .f6_regfile_waddr             (in.f6_regfile_waddr),
    .f6_bypass_clk_en             (in.f6_bypass_clk_en),
    .f7_thread_id                 (in.f7_thread_id),
    .f7_regfile_wmask             (in.f7_regfile_wmask),
    .f7_regfile_waddr             (in.f7_regfile_waddr),
    .f7_bypass_clk_en             (in.f7_bypass_clk_en),
    .f7_regfile_wen               (in.f7_regfile_wen),
    .f8_thread_id                 (in.f8_thread_id),
    .f8_regfile_wmask             (in.f8_regfile_wmask),
    .f8_regfile_waddr             (in.f8_regfile_waddr),
    .f8_regfile_wen               (in.f8_regfile_wen),
    .f8_txfma_op                  (in.f8_txfma_op),
    .f8_data_vector               (in.f8_data_vector),
    .ex_regfile_rdata_bypass      (orig_out.ex_regfile_rdata_bypass),
    .f2_fswizz_rdata              (orig_out.f2_fswizz_rdata),
    .f3_maskop_bit                (orig_out.f3_maskop_bit),
    .f8_txfma_comp_res            (orig_out.f8_txfma_comp_res),
    .f8_wdata                     (orig_out.f8_wdata),
    .f8_wflags                    (orig_out.f8_wflags),
    .f8_sh_sw_wdata               (orig_out.f8_sh_sw_wdata),
    .txfma_trans_dbg              (orig_out.txfma_trans_dbg)
  );

  vpu_lane u_new (
    .lane_id                      (in.lane_id),
    .clock                        (clk_i),
    .reset                        (~rst_ni),
    .chicken_bit_vpulane          (in.chicken_bit_vpulane),
    .chicken_bit_vputima          (in.chicken_bit_vputima),
    .mem_ctrl_override            (in.mem_ctrl_override),
    .id_regfile_raddr1            (in.id_regfile_raddr1),
    .id_regfile_raddr2            (in.id_regfile_raddr2),
    .id_regfile_raddr3            (in.id_regfile_raddr3),
    .id_regfile_ren               (in.id_regfile_ren),
    .id_regfile_thread_id         (in.id_regfile_thread_id),
    .id_trans_busy                (in.id_trans_busy),
    .ex_regfile_raddr1            (in.ex_regfile_raddr1),
    .ex_regfile_raddr2            (in.ex_regfile_raddr2),
    .ex_regfile_raddr3            (in.ex_regfile_raddr3),
    .ex_regfile_thread_id         (in.ex_regfile_thread_id),
    .ex_mask_in1                  (in.ex_mask_in1),
    .ex_tena_regfile_bypass_en    (in.ex_tena_regfile_bypass_en),
    .ex_tenb_regfile_bypass_en    (in.ex_tenb_regfile_bypass_en),
    .ex_req_lane                  (new_ex_req_lane),
    .ex_fromint_data              (in.ex_fromint_data),
    .ex_bypass_force_ctrl         (new_ex_bypass_force_ctrl),
    .ex_tima_valid                (in.ex_tima_valid),
    .ex_tima_valid_unqual         (in.ex_tima_valid_unqual),
    .ex_tima_sa                   (in.ex_tima_sa),
    .ex_tima_sb                   (in.ex_tima_sb),
    .ex_tima_tenb_raddr           (in.ex_tima_tenb_raddr),
    .ex_tena_rf_rdata             (in.ex_tena_rf_rdata),
    .ex_tena_rf_tima_rdata        (in.ex_tena_rf_tima_rdata),
    .ex_rom_valid                 (in.ex_rom_valid),
    .ex_sh_sw_valid               (in.ex_sh_sw_valid),
    .ex_txfma_valid               (in.ex_txfma_valid),
    .ex_txfma_clock_valid         (in.ex_txfma_clock_valid),
    .ex_rom_clock_valid           (in.ex_rom_clock_valid),
    .ex_sh_sw_clock_valid         (in.ex_sh_sw_clock_valid),
    .f2_tima_ren3                 (in.f2_tima_ren3),
    .f2_tima_tenc_raddr           (in.f2_tima_tenc_raddr),
    .f2_tima_tenc_wen             (in.f2_tima_tenc_wen),
    .f2_tima_rf_wen               (in.f2_tima_rf_wen),
    .f2_fswizz_rdata1             (in.f2_fswizz_rdata1),
    .f2_fswizz_rdata2             (in.f2_fswizz_rdata2),
    .f2_fswizz_rdata3             (in.f2_fswizz_rdata3),
    .f2_tenb_regfile_wen_l        (in.f2_tenb_regfile_wen_l),
    .dcache_scp_resp              (new_dcache_scp_resp),
    .f3_regfile_wen_l             (in.f3_regfile_wen_l),
    .f3_tenb_regfile_wen_l        (in.f3_tenb_regfile_wen_l),
    .f3_tenb_regfile_wdata_l      (in.f3_tenb_regfile_wdata_l),
    .f3_tenb_regfile_waddr_l      (in.f3_tenb_regfile_waddr_l),
    .f3_tima_tenc_wen             (in.f3_tima_tenc_wen),
    .f3_tima_tenc_waddr           (in.f3_tima_tenc_waddr),
    .f3_tima_rf_wen               (in.f3_tima_rf_wen),
    .f3_thread_id                 (in.f3_thread_id),
    .f3_regfile_waddr             (in.f3_regfile_waddr),
    .f3_regfile_wmask             (in.f3_regfile_wmask),
    .f3_bypass_clk_en             (in.f3_bypass_clk_en),
    .f3_data_vector               (in.f3_data_vector),
    .f4_regfile_wen_l             (in.f4_regfile_wen_l),
    .f4_regfile_waddr_l           (in.f4_regfile_waddr_l),
    .f4_regfile_wdata_l           (in.f4_regfile_wdata_l),
    .f4_regfile_wmask_l           (in.f4_regfile_wmask_l),
    .f4_regfile_thrid_l           (in.f4_regfile_thrid_l),
    .f4_thread_id                 (in.f4_thread_id),
    .f4_regfile_waddr             (in.f4_regfile_waddr),
    .f4_regfile_wmask             (in.f4_regfile_wmask),
    .f4_bypass_clk_en             (in.f4_bypass_clk_en),
    .f5_thread_id                 (in.f5_thread_id),
    .f5_regfile_wmask             (in.f5_regfile_wmask),
    .f5_regfile_waddr             (in.f5_regfile_waddr),
    .f5_bypass_clk_en             (in.f5_bypass_clk_en),
    .f6_thread_id                 (in.f6_thread_id),
    .f6_regfile_wmask             (in.f6_regfile_wmask),
    .f6_regfile_waddr             (in.f6_regfile_waddr),
    .f6_bypass_clk_en             (in.f6_bypass_clk_en),
    .f7_thread_id                 (in.f7_thread_id),
    .f7_regfile_wmask             (in.f7_regfile_wmask),
    .f7_regfile_waddr             (in.f7_regfile_waddr),
    .f7_bypass_clk_en             (in.f7_bypass_clk_en),
    .f7_regfile_wen               (in.f7_regfile_wen),
    .f8_thread_id                 (in.f8_thread_id),
    .f8_regfile_wmask             (in.f8_regfile_wmask),
    .f8_regfile_waddr             (in.f8_regfile_waddr),
    .f8_regfile_wen               (in.f8_regfile_wen),
    .f8_txfma_op                  (in.f8_txfma_op),
    .f8_data_vector               (in.f8_data_vector),
    .ex_regfile_rdata_bypass      (new_out.ex_regfile_rdata_bypass),
    .f2_fswizz_rdata              (new_out.f2_fswizz_rdata),
    .f3_maskop_bit                (new_out.f3_maskop_bit),
    .f8_txfma_comp_res            (new_out.f8_txfma_comp_res),
    .f8_wdata                     (new_out.f8_wdata),
    .f8_wflags                    (new_out.f8_wflags),
    .f8_sh_sw_wdata               (new_out.f8_sh_sw_wdata),
    .txfma_trans_dbg              (new_out.txfma_trans_dbg)
  );

endmodule
