REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := vpu_top

COMMON_NEW := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
              $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv

TXFMA_REAL_NEW := $(wildcard $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma*.sv)

NEW_RTL := $(COMMON_NEW) \
           $(PRIM_CLK_GATE) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(PRIM_RF_3R2W_PREVIEW) \
           $(wildcard $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_*_rom_case_*.sv) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
           $(TXFMA_REAL_NEW) \
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
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_top.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_ctrl.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_ml.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorfma.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorreduce.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorquant.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_trans.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_uinst_decoder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensora_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensortmp_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_mask.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_lane.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_bypass.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_sh_sw.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_lane_tima.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorb_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorc_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/tima/tima_adder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/tima/tima_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_txfma_trans_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_top.v \
            $(wildcard $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_*_rom_case_*.v) \
            $(wildcard $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/*.v)

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/clock_gates \
                $(ORIG_ROOT)/rtl/libs/registers

ORIG_RENAME := txfma_4_2_compressor_array \
               txfma_booth_ppg_32r4_norm \
               txfma_ediff_opdorder_logic \
               txfma_exp_special_detect \
               txfma_frac_zero_detect \
               trans_rsqrt_rom_case_c1c2 \
               trans_rcp_rom_case_c1c2 \
               trans_log_rom_case_c1c2 \
               trans_exp_rom_case_c1c2 \
               trans_sin_rom_case_c1c2 \
               vpu_txfma_trans_top \
               txfma_4_2_compressor \
               txfma_booth_ppg_32r4_msb \
               trans_rsqrt_rom_case_c0 \
               trans_rcp_rom_case_c0 \
               trans_log_rom_case_c0 \
               trans_exp_rom_case_c0 \
               trans_sin_rom_case_c0 \
               txfma_booth_ppg_32r4 \
               txfma_align_shf \
               txfma_norm_shf \
               txfmactl_top \
               txfmaexp_top \
               txfmafrac_top \
               vpu_uinst_decoder \
               vpu_tensorreduce \
               vpu_tensorquant \
               vpu_tensorfma \
               vpu_tensortmp_rf \
               vpu_tensora_rf \
               vpu_lane_tima \
               vpu_tensorc_rf \
               vpu_tensorb_rf \
               txfma_wallace1 \
               txfma_wallace2 \
               txfma_rnd_adder \
               txfma_adder \
               vpu_bypass \
               vpu_sh_sw \
               vpu_trans \
               vpu_mask \
               trans_top \
               txfma_top \
               vpu_ctrl \
               vpu_lane \
               vpu_top \
               vpu_rf \
               vpu_ml \
               tima_top \
               txfma_trz2 \
               tima_adder \
               txfma_lxd \
               txfma_trz \
               txfma_csa \
               txfma_c0 \
               txfma_c1 \
               txfma_c2 \
               txfma_c3 \
               txfma_c4 \
               txfma_c5 \
               txfma_c6 \
               txfma_e1 \
               txfma_e2 \
               txfma_e4 \
               txfma_e5 \
               txfma_e6 \
               txfma_f0 \
               txfma_f1 \
               txfma_f2 \
               txfma_f3 \
               txfma_f4 \
               txfma_f5 \
               txfma_f6

EXTRA_FLAGS := -DVPU_EN_RCG_2

include $(REPO_ROOT)/mk/rtlcosim.mk
