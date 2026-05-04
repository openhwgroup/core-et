REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := minion_dcache_128x64_1r1w_lram

NEW_RTL  := $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(PRIM_RF_1R1W_REG_PREVIEW) \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_128x64_1r1w_lram.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/inc/macros.vh \
            $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_1w_reg.v \
            $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_128x64_1r1w_lram.v

EXTRA_FLAGS := -Wno-COMBDLY -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
