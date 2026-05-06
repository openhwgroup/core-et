REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := vpu_txfma_trans_top

include $(REPO_ROOT)/mk/minion_vpu_sources.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/tech_generic/prim_clk_gate/rtl/prim_clk_gate.sv \
           $(MINION_VPU_TRANS_ROM_RTL) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
           $(MINION_VPU_TXFMA_REAL_RTL) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_txfma_trans_top.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_txfma_trans_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_top.v \
            $(ORIG_MINION_VPU_TRANS_ROM_RTL) \
            $(ORIG_MINION_VPU_TXFMA_REAL_RTL) \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v \
            $(ORIG_ROOT)/rtl/libs/registers/en_ff.v \
            $(ORIG_ROOT)/rtl/libs/registers/rst_ff.v \
            $(ORIG_ROOT)/rtl/libs/registers/rst_en_ff.v

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
               txfma_wallace1 \
               txfma_wallace2 \
               txfma_rnd_adder \
               txfma_adder \
               trans_top \
               txfma_top \
               txfma_trz2 \
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

include $(REPO_ROOT)/mk/rtlcosim.mk
