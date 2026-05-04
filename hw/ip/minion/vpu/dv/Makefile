REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COMMON_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
              $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv
PRIMS := $(PRIM_CLK_GATE) \
         $(PRIM_RF_1R1W_PREVIEW) \
         $(PRIM_RF_3R2W_PREVIEW)

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
         tima_adder \
         tima_top \
         lane_tima \
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

sh_sw_TOP := vpu_sh_sw_tb
sh_sw_SRCS := $(COMMON_RTL) \
              $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_sh_sw.sv \
              $(CURDIR)/vpu_sh_sw_tb.sv \
              $(CURDIR)/vpu_sh_sw_test.cc

trans_top_default_TOP := trans_top_tb
trans_top_default_SRCS := $(COMMON_RTL) \
                          $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
                          $(wildcard $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_*_rom_case_*.sv) \
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
             $(PRIMS) \
             $(wildcard $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_*_rom_case_*.sv) \
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
             $(CURDIR)/vpu_lint_top.sv
LINT_FLAGS := --top-module vpu_lint_top

include $(REPO_ROOT)/mk/verilator.mk
