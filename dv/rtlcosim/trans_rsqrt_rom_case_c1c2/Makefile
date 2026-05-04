REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := trans_rsqrt_rom_case_c1c2

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_rsqrt_rom_case_c1c2.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_rsqrt_rom_case_c1c2.v
ORIG_RENAME := trans_rsqrt_rom_case_c1c2

include $(REPO_ROOT)/mk/rtlcosim.mk
