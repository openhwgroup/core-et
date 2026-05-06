# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0
#
# Deterministic source lists for translated Minion VPU/TXFMA/transcendental
# builds. Keep real TXFMA and fake TXFMA separated so helper/fake files are
# never pulled into real-VPU targets by a broad txfma*.sv wildcard.

ORIG_ROOT ?= $(REPO_ROOT)/../etcore-soc

MINION_VPU_TRANS_ROM_RTL := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_exp_rom_case_c0.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_exp_rom_case_c1c2.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_log_rom_case_c0.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_log_rom_case_c1c2.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_rcp_rom_case_c0.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_rcp_rom_case_c1c2.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_rsqrt_rom_case_c0.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_rsqrt_rom_case_c1c2.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_sin_rom_case_c0.sv \
                            $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_sin_rom_case_c1c2.sv

MINION_VPU_TXFMA_REAL_RTL := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor_array.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_norm.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_msb.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_wallace1.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_wallace2.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_lxd.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_trz.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_trz2.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_adder.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_align_shf.sv \
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_norm_shf.sv \
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
                             $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_top.sv

MINION_VPU_TXFMA_FAKE_RTL := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_top_fake.sv

ORIG_MINION_VPU_TRANS_ROM_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_exp_rom_case_c0.v \
                                 $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_exp_rom_case_c1c2.v \
                                 $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_log_rom_case_c0.v \
                                 $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_log_rom_case_c1c2.v \
                                 $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_rcp_rom_case_c0.v \
                                 $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_rcp_rom_case_c1c2.v \
                                 $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_rsqrt_rom_case_c0.v \
                                 $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_rsqrt_rom_case_c1c2.v \
                                 $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_sin_rom_case_c0.v \
                                 $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_sin_rom_case_c1c2.v

ORIG_MINION_VPU_TXFMA_REAL_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_csa.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_4_2_compressor.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_4_2_compressor_array.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4_norm.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4_msb.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_wallace1.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_wallace2.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_lxd.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_trz.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_trz2.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_adder.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_align_shf.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_norm_shf.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_frac_zero_detect.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_exp_special_detect.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_ediff_opdorder_logic.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_rnd_adder.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_c0.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_c1.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_c2.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_c3.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_c4.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_c5.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_c6.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_e1.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_e2.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_e4.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_e5.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_e6.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f0.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f1.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f2.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f3.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f4.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f5.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f6.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfmactl_top.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfmaexp_top.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfmafrac_top.v \
                                  $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_top.v

ORIG_MINION_VPU_TXFMA_FAKE_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_top_fake.v
