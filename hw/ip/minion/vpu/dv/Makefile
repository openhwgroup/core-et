REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
include $(REPO_ROOT)/mk/minion_vpu_sources.mk

COMMON_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
              $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv
PRIMS := $(PRIM_CLK_GATE) \
         $(PRIM_RF_1R1W_PREVIEW) \
         $(PRIM_RF_3R2W_PREVIEW)

TXFMA_REAL_RTL := $(MINION_VPU_TXFMA_REAL_RTL)
TXFMA_C_STAGE_COMMON_RTL := $(COMMON_RTL) \
                            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv

TESTS := bypass \
         mask \
         rf_default \
         rf_mmi \
         tensora_rf \
         tensorb_rf \
         tensorc_rf \
         tensortmp_rf \
         txfma_lxd \
         txfma_trz \
         txfma_trz2 \
         txfma_e1 \
         txfma_e2 \
         txfma_e4 \
         txfma_e5 \
         txfma_e6 \
         txfma_ediff_opdorder_logic \
         txfma_exp_special_detect \
         txfma_adder \
         txfma_align_shf \
         txfma_norm_shf \
         txfma_csa \
         txfma_4_2_compressor \
         txfma_4_2_compressor_array \
         txfma_booth_ppg_32r4_norm \
         txfma_booth_ppg_32r4_msb \
         txfma_booth_ppg_32r4 \
         txfma_wallace1 \
         txfma_wallace2 \
         txfma_c0 \
         txfma_c1 \
         txfma_c2 \
         txfma_c3 \
         txfma_c4 \
         txfma_c5 \
         txfma_c6 \
         txfmactl_top \
         txfmafrac_top \
         txfmaexp_top \
         txfma_top \
         txfma_top_fake \
         txfma_trans_top_default \
         txfma_trans_top_fake \
         tima_adder \
         tima_top \
         lane_tima \
         tensorreduce \
         vpu_tensorfma \
         vpu_tensorquant \
         vpu_ml \
         vpu_lane \
         vpu_ctrl \
         vpu_top \
         sh_sw \
         trans_top_default \
         trans_top_extra_trans \
         trans_default \
         trans_extra_trans \
         uinst_decoder_default \
         uinst_decoder_extra_trans

bypass_TOP := vpu_bypass_tb
bypass_SRCS := $(COMMON_RTL) \
               $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_bypass.sv \
               $(CURDIR)/vpu_bypass_tb.sv \
               $(CURDIR)/vpu_bypass_test.cc

mask_TOP := vpu_mask_tb
mask_SRCS := $(COMMON_RTL) \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_mask.sv \
             $(CURDIR)/vpu_mask_tb.sv \
             $(CURDIR)/vpu_mask_test.cc

rf_default_TOP := vpu_rf_tb
rf_default_SRCS := $(COMMON_RTL) \
                   $(PRIMS) \
                   $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_rf.sv \
                   $(CURDIR)/vpu_rf_tb.sv \
                   $(CURDIR)/vpu_rf_test.cc

rf_mmi_TOP := vpu_rf_tb
rf_mmi_SRCS := $(rf_default_SRCS)
rf_mmi_FLAGS := -GUseMmi=1

tensora_rf_TOP := vpu_tensora_rf_tb
tensora_rf_SRCS := $(COMMON_RTL) \
                   $(PRIMS) \
                   $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensora_rf.sv \
                   $(CURDIR)/vpu_tensora_rf_tb.sv \
                   $(CURDIR)/vpu_tensora_rf_test.cc

tensorb_rf_TOP := vpu_tensorb_rf_tb
tensorb_rf_SRCS := $(COMMON_RTL) \
                   $(PRIMS) \
                   $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorb_rf.sv \
                   $(CURDIR)/vpu_tensorb_rf_tb.sv \
                   $(CURDIR)/vpu_tensorb_rf_test.cc

tensorc_rf_TOP := vpu_tensorc_rf_tb
tensorc_rf_SRCS := $(COMMON_RTL) \
                   $(PRIMS) \
                   $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorc_rf.sv \
                   $(CURDIR)/vpu_tensorc_rf_tb.sv \
                   $(CURDIR)/vpu_tensorc_rf_test.cc

tensortmp_rf_TOP := vpu_tensortmp_rf_tb
tensortmp_rf_SRCS := $(COMMON_RTL) \
                     $(PRIMS) \
                     $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensortmp_rf.sv \
                     $(CURDIR)/vpu_tensortmp_rf_tb.sv \
                     $(CURDIR)/vpu_tensortmp_rf_test.cc

txfma_lxd_TOP := txfma_lxd_tb
txfma_lxd_SRCS := $(COMMON_RTL) \
                  $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_lxd.sv \
                  $(CURDIR)/txfma_lxd_tb.sv \
                  $(CURDIR)/txfma_lxd_test.cc

txfma_trz_TOP := txfma_trz_tb
txfma_trz_SRCS := $(COMMON_RTL) \
                  $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_trz.sv \
                  $(CURDIR)/txfma_trz_tb.sv \
                  $(CURDIR)/txfma_trz_test.cc

txfma_trz2_TOP := txfma_trz2_tb
txfma_trz2_SRCS := $(COMMON_RTL) \
                   $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_trz2.sv \
                   $(CURDIR)/txfma_trz2_tb.sv \
                   $(CURDIR)/txfma_trz2_test.cc

TXFMA_E_STAGE_COMMON_RTL := $(COMMON_RTL) \
                            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv

TXFMA_E_STAGE_MATH_RTL := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
                          $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_adder.sv

txfma_e1_TOP := txfma_e1_tb
txfma_e1_SRCS := $(TXFMA_E_STAGE_COMMON_RTL) \
                 $(TXFMA_E_STAGE_MATH_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e1.sv \
                 $(CURDIR)/txfma_e1_tb.sv \
                 $(CURDIR)/txfma_e1_test.cc

txfma_e2_TOP := txfma_e2_tb
txfma_e2_SRCS := $(TXFMA_E_STAGE_COMMON_RTL) \
                 $(TXFMA_E_STAGE_MATH_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_ediff_opdorder_logic.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e2.sv \
                 $(CURDIR)/txfma_e2_tb.sv \
                 $(CURDIR)/txfma_e2_test.cc

txfma_e4_TOP := txfma_e4_tb
txfma_e4_SRCS := $(TXFMA_E_STAGE_COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e4.sv \
                 $(CURDIR)/txfma_e4_tb.sv \
                 $(CURDIR)/txfma_e4_test.cc

txfma_e5_TOP := txfma_e5_tb
txfma_e5_SRCS := $(TXFMA_E_STAGE_COMMON_RTL) \
                 $(TXFMA_E_STAGE_MATH_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e5.sv \
                 $(CURDIR)/txfma_e5_tb.sv \
                 $(CURDIR)/txfma_e5_test.cc

txfma_e6_TOP := txfma_e6_tb
txfma_e6_SRCS := $(TXFMA_E_STAGE_COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_adder.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e6.sv \
                 $(CURDIR)/txfma_e6_tb.sv \
                 $(CURDIR)/txfma_e6_test.cc

txfma_ediff_opdorder_logic_TOP := txfma_ediff_opdorder_logic_tb
txfma_ediff_opdorder_logic_SRCS := $(TXFMA_E_STAGE_COMMON_RTL) \
                                   $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_ediff_opdorder_logic.sv \
                                   $(CURDIR)/txfma_ediff_opdorder_logic_tb.sv \
                                   $(CURDIR)/txfma_ediff_opdorder_logic_test.cc

txfma_exp_special_detect_TOP := txfma_exp_special_detect_tb
txfma_exp_special_detect_SRCS := $(TXFMA_E_STAGE_COMMON_RTL) \
                                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_exp_special_detect.sv \
                                 $(CURDIR)/txfma_exp_special_detect_tb.sv \
                                 $(CURDIR)/txfma_exp_special_detect_test.cc
txfma_exp_special_detect_FLAGS := -Wno-UNUSEDSIGNAL -Wno-CASEINCOMPLETE

txfma_adder_TOP := txfma_adder_tb
txfma_adder_SRCS := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_adder.sv \
                    $(CURDIR)/txfma_adder_tb.sv \
                    $(CURDIR)/txfma_adder_test.cc

txfma_align_shf_TOP := txfma_align_shf_tb
txfma_align_shf_SRCS := $(COMMON_RTL) \
                        $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_align_shf.sv \
                        $(CURDIR)/txfma_align_shf_tb.sv \
                        $(CURDIR)/txfma_align_shf_test.cc

txfma_norm_shf_TOP := txfma_norm_shf_tb
txfma_norm_shf_SRCS := $(COMMON_RTL) \
                       $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_norm_shf.sv \
                       $(CURDIR)/txfma_norm_shf_tb.sv \
                       $(CURDIR)/txfma_norm_shf_test.cc

txfma_csa_TOP := txfma_csa_tb
txfma_csa_SRCS := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
                  $(CURDIR)/txfma_csa_tb.sv \
                  $(CURDIR)/txfma_csa_test.cc

txfma_4_2_compressor_TOP := txfma_4_2_compressor_tb
txfma_4_2_compressor_SRCS := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor.sv \
                             $(CURDIR)/txfma_4_2_compressor_tb.sv \
                             $(CURDIR)/txfma_4_2_compressor_test.cc

txfma_4_2_compressor_array_TOP := txfma_4_2_compressor_array_tb
txfma_4_2_compressor_array_SRCS := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
                                   $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor.sv \
                                   $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor_array.sv \
                                   $(CURDIR)/txfma_4_2_compressor_array_tb.sv \
                                   $(CURDIR)/txfma_4_2_compressor_array_test.cc

txfma_booth_ppg_32r4_norm_TOP := txfma_booth_ppg_32r4_norm_tb
txfma_booth_ppg_32r4_norm_SRCS := $(COMMON_RTL) \
                                  $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_norm.sv \
                                  $(CURDIR)/txfma_booth_ppg_32r4_norm_tb.sv \
                                  $(CURDIR)/txfma_booth_ppg_32r4_norm_test.cc

txfma_booth_ppg_32r4_msb_TOP := txfma_booth_ppg_32r4_msb_tb
txfma_booth_ppg_32r4_msb_SRCS := $(COMMON_RTL) \
                                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_msb.sv \
                                 $(CURDIR)/txfma_booth_ppg_32r4_msb_tb.sv \
                                 $(CURDIR)/txfma_booth_ppg_32r4_msb_test.cc

txfma_booth_ppg_32r4_TOP := txfma_booth_ppg_32r4_tb
txfma_booth_ppg_32r4_SRCS := $(COMMON_RTL) \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_norm.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_msb.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4.sv \
                             $(CURDIR)/txfma_booth_ppg_32r4_tb.sv \
                             $(CURDIR)/txfma_booth_ppg_32r4_test.cc

txfma_wallace1_TOP := txfma_wallace1_tb
txfma_wallace1_SRCS := $(COMMON_RTL) \
                       $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
                       $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor.sv \
                       $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor_array.sv \
                       $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_wallace1.sv \
                       $(CURDIR)/txfma_wallace1_tb.sv \
                       $(CURDIR)/txfma_wallace1_test.cc

txfma_wallace2_TOP := txfma_wallace2_tb
txfma_wallace2_SRCS := $(COMMON_RTL) \
                       $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
                       $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor.sv \
                       $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor_array.sv \
                       $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_wallace2.sv \
                       $(CURDIR)/txfma_wallace2_tb.sv \
                       $(CURDIR)/txfma_wallace2_test.cc

txfma_c0_TOP := txfma_c0_tb
txfma_c0_SRCS := $(TXFMA_C_STAGE_COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c0.sv \
                 $(CURDIR)/txfma_c0_tb.sv \
                 $(CURDIR)/txfma_c0_test.cc

txfma_c1_TOP := txfma_c1_tb
txfma_c1_SRCS := $(TXFMA_C_STAGE_COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c1.sv \
                 $(CURDIR)/txfma_c1_tb.sv \
                 $(CURDIR)/txfma_c1_test.cc

txfma_c2_TOP := txfma_c2_tb
txfma_c2_SRCS := $(TXFMA_C_STAGE_COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c2.sv \
                 $(CURDIR)/txfma_c2_tb.sv \
                 $(CURDIR)/txfma_c2_test.cc

txfma_c3_TOP := txfma_c3_tb
txfma_c3_SRCS := $(TXFMA_C_STAGE_COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c3.sv \
                 $(CURDIR)/txfma_c3_tb.sv \
                 $(CURDIR)/txfma_c3_test.cc

txfma_c4_TOP := txfma_c4_tb
txfma_c4_SRCS := $(TXFMA_C_STAGE_COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c4.sv \
                 $(CURDIR)/txfma_c4_tb.sv \
                 $(CURDIR)/txfma_c4_test.cc

txfma_c5_TOP := txfma_c5_tb
txfma_c5_SRCS := $(TXFMA_C_STAGE_COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c5.sv \
                 $(CURDIR)/txfma_c5_tb.sv \
                 $(CURDIR)/txfma_c5_test.cc

txfma_c6_TOP := txfma_c6_tb
txfma_c6_SRCS := $(TXFMA_C_STAGE_COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c6.sv \
                 $(CURDIR)/txfma_c6_tb.sv \
                 $(CURDIR)/txfma_c6_test.cc

txfmactl_top_TOP := txfmactl_top_tb
txfmactl_top_SRCS := $(COMMON_RTL) \
                      $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                      $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                      $(PRIM_CLK_GATE) \
                      $(TXFMA_REAL_RTL) \
                      $(CURDIR)/txfmactl_top_tb.sv \
                      $(CURDIR)/txfmactl_top_test.cc

txfmafrac_top_TOP := txfmafrac_top_tb
txfmafrac_top_SRCS := $(COMMON_RTL) \
                      $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                      $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                      $(PRIM_CLK_GATE) \
                      $(TXFMA_REAL_RTL) \
                      $(CURDIR)/txfmafrac_top_tb.sv \
                      $(CURDIR)/txfmafrac_top_test.cc

txfmaexp_top_TOP := txfmaexp_top_tb
txfmaexp_top_SRCS := $(COMMON_RTL) \
                     $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                     $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                     $(PRIM_CLK_GATE) \
                     $(TXFMA_REAL_RTL) \
                     $(CURDIR)/txfmaexp_top_tb.sv \
                     $(CURDIR)/txfmaexp_top_test.cc

txfma_top_TOP := txfma_top_tb
txfma_top_SRCS := $(COMMON_RTL) \
                  $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                  $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                  $(PRIM_CLK_GATE) \
                  $(TXFMA_REAL_RTL) \
                  $(CURDIR)/txfma_top_tb.sv \
                  $(CURDIR)/txfma_top_test.cc

txfma_top_fake_TOP := txfma_top_fake_tb
txfma_top_fake_SRCS := $(COMMON_RTL) \
                       $(MINION_VPU_TXFMA_FAKE_RTL) \
                       $(CURDIR)/txfma_top_fake_tb.sv \
                       $(CURDIR)/txfma_top_fake_test.cc
# This standalone slice imports the VPU package without the larger TXFMA package set;
# package-width name shadows are covered by targeted source pragmas.
txfma_top_fake_FLAGS :=

txfma_trans_top_fake_TOP := vpu_txfma_trans_top_tb
txfma_trans_top_fake_SRCS := $(COMMON_RTL) \
                             $(MINION_VPU_TRANS_ROM_RTL) \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
                             $(MINION_VPU_TXFMA_FAKE_RTL) \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_txfma_trans_top.sv \
                             $(CURDIR)/vpu_txfma_trans_top_tb.sv \
                             $(CURDIR)/vpu_txfma_trans_top_test.cc
txfma_trans_top_fake_FLAGS := -GUseFakeTxfma=1

txfma_trans_top_default_TOP := vpu_txfma_trans_top_tb
txfma_trans_top_default_SRCS := $(COMMON_RTL) \
                                $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                                $(PRIM_CLK_GATE) \
                                $(MINION_VPU_TRANS_ROM_RTL) \
                                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
                                $(TXFMA_REAL_RTL) \
                                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_txfma_trans_top.sv \
                                $(CURDIR)/vpu_txfma_trans_top_tb.sv \
                                $(CURDIR)/vpu_txfma_trans_top_default_test.cc
txfma_trans_top_default_FLAGS := -GUseFakeTxfma=0

tima_adder_TOP := tima_adder_tb
tima_adder_SRCS := $(COMMON_RTL) \
                   $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv \
                   $(CURDIR)/tima_adder_tb.sv \
                   $(CURDIR)/tima_adder_test.cc

tima_top_TOP := tima_top_tb
tima_top_SRCS := $(COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_top.sv \
                 $(CURDIR)/tima_top_tb.sv \
                 $(CURDIR)/tima_top_test.cc

lane_tima_TOP := vpu_lane_tima_tb
lane_tima_SRCS := $(COMMON_RTL) \
                  $(PRIMS) \
                  $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv \
                  $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_top.sv \
                  $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorb_rf.sv \
                  $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorc_rf.sv \
                  $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane_tima.sv \
                  $(CURDIR)/vpu_lane_tima_tb.sv \
                  $(CURDIR)/vpu_lane_tima_test.cc

tensorreduce_TOP := vpu_tensorreduce_tb
tensorreduce_SRCS := $(COMMON_RTL) \
                     $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                     $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                     $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorreduce.sv \
                     $(CURDIR)/vpu_tensorreduce_tb.sv \
                     $(CURDIR)/vpu_tensorreduce_test.cc

vpu_tensorfma_TOP := vpu_tensorfma_tb
vpu_tensorfma_SRCS := $(COMMON_RTL) \
                      $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                      $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                      $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorfma.sv \
                      $(CURDIR)/vpu_tensorfma_tb.sv \
                      $(CURDIR)/vpu_tensorfma_test.cc

vpu_tensorquant_TOP := vpu_tensorquant_tb
vpu_tensorquant_SRCS := $(COMMON_RTL) \
                        $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                        $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                        $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorquant.sv \
                        $(CURDIR)/vpu_tensorquant_tb.sv \
                        $(CURDIR)/vpu_tensorquant_test.cc

vpu_ml_TOP := vpu_ml_tb
vpu_ml_SRCS := $(COMMON_RTL) \
               $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
               $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
               $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorfma.sv \
               $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorreduce.sv \
               $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorquant.sv \
               $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ml.sv \
               $(CURDIR)/vpu_ml_tb.sv \
               $(CURDIR)/vpu_ml_test.cc

vpu_lane_TOP := vpu_lane_tb
vpu_lane_SRCS := $(COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                 $(PRIMS) \
                 $(MINION_VPU_TRANS_ROM_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
                 $(TXFMA_REAL_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_txfma_trans_top.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_rf.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_bypass.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_top.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorb_rf.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorc_rf.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane_tima.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_sh_sw.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane.sv \
                 $(CURDIR)/vpu_lane_tb.sv \
                 $(CURDIR)/vpu_lane_test.cc

vpu_ctrl_TOP := vpu_ctrl_tb
vpu_ctrl_SRCS := $(COMMON_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                 $(PRIMS) \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorfma.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorreduce.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorquant.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ml.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_trans.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_uinst_decoder.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensora_rf.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensortmp_rf.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_mask.sv \
                 $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ctrl.sv \
                 $(CURDIR)/vpu_ctrl_tb.sv \
                 $(CURDIR)/vpu_ctrl_test.cc

vpu_top_TOP := vpu_top_tb
vpu_top_SRCS := $(COMMON_RTL) \
                $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
                $(PRIMS) \
                $(MINION_VPU_TRANS_ROM_RTL) \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
                $(TXFMA_REAL_RTL) \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_txfma_trans_top.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_rf.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_bypass.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_top.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorb_rf.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorc_rf.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane_tima.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_sh_sw.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorfma.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorreduce.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorquant.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ml.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_trans.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_uinst_decoder.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensora_rf.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensortmp_rf.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_mask.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ctrl.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane.sv \
                $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_top.sv \
                $(CURDIR)/vpu_top_tb.sv \
                $(CURDIR)/vpu_top_test.cc

sh_sw_TOP := vpu_sh_sw_tb
sh_sw_SRCS := $(COMMON_RTL) \
              $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_sh_sw.sv \
              $(CURDIR)/vpu_sh_sw_tb.sv \
              $(CURDIR)/vpu_sh_sw_test.cc

trans_top_default_TOP := trans_top_tb
trans_top_default_SRCS := $(COMMON_RTL) \
                          $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
                          $(MINION_VPU_TRANS_ROM_RTL) \
                          $(CURDIR)/trans_top_tb.sv \
                          $(CURDIR)/trans_top_test.cc

trans_top_extra_trans_TOP := trans_top_tb
trans_top_extra_trans_SRCS := $(trans_top_default_SRCS)
trans_top_extra_trans_FLAGS := -GEnableExtraTrans=1

trans_default_TOP := vpu_trans_tb
trans_default_SRCS := $(COMMON_RTL) \
                      $(PRIM_CLK_GATE) \
                      $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_trans.sv \
                      $(CURDIR)/vpu_trans_tb.sv \
                      $(CURDIR)/vpu_trans_test.cc

trans_extra_trans_TOP := vpu_trans_tb
trans_extra_trans_SRCS := $(trans_default_SRCS)
trans_extra_trans_FLAGS := -GEnableExtraTrans=1

uinst_decoder_default_TOP  := vpu_uinst_decoder_tb
uinst_decoder_default_SRCS := $(COMMON_RTL) \
                              $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_uinst_decoder.sv \
                              $(CURDIR)/vpu_uinst_decoder_tb.sv \
                              $(CURDIR)/vpu_uinst_decoder_test.cc

uinst_decoder_extra_trans_TOP   := vpu_uinst_decoder_tb
uinst_decoder_extra_trans_SRCS  := $(uinst_decoder_default_SRCS)
uinst_decoder_extra_trans_FLAGS := -GEnableExtraTrans=1

LINT_SRCS := $(COMMON_RTL) \
             $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
             $(PRIMS) \
             $(MINION_VPU_TRANS_ROM_RTL) \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_lxd.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_trz.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_trz2.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_adder.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_align_shf.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_norm_shf.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor_array.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_norm.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_msb.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_wallace1.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_wallace2.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_frac_zero_detect.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_exp_special_detect.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_ediff_opdorder_logic.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_rnd_adder.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c0.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c1.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c2.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c3.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c4.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c5.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c6.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e1.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e2.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e4.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e5.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e6.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f0.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f1.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f2.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f3.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f4.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f5.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f6.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfmactl_top.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfmaexp_top.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfmafrac_top.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_top.sv \
             $(MINION_VPU_TXFMA_FAKE_RTL) \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_bypass.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_mask.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_rf.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensora_rf.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorb_rf.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorc_rf.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensortmp_rf.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_top.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane_tima.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_sh_sw.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_trans.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_uinst_decoder.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorreduce.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_txfma_trans_top.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorfma.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorquant.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ml.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ctrl.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane.sv \
             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_top.sv \
             $(CURDIR)/vpu_lint_top.sv
LINT_FLAGS := --top-module vpu_lint_top

include $(REPO_ROOT)/mk/verilator.mk
