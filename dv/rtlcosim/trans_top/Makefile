REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := trans_top

include $(REPO_ROOT)/mk/minion_vpu_sources.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(MINION_VPU_TRANS_ROM_RTL) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_top.v \
            $(ORIG_MINION_VPU_TRANS_ROM_RTL)

ORIG_RENAME := trans_rsqrt_rom_case_c1c2 \
               trans_rsqrt_rom_case_c0 \
               trans_rcp_rom_case_c1c2 \
               trans_rcp_rom_case_c0 \
               trans_log_rom_case_c1c2 \
               trans_log_rom_case_c0 \
               trans_exp_rom_case_c1c2 \
               trans_exp_rom_case_c0 \
               trans_sin_rom_case_c1c2 \
               trans_sin_rom_case_c0 \
               trans_top

include $(REPO_ROOT)/mk/rtlcosim.mk
