REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := minion_dcache_cache_op_unit_l2

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_cache_op_unit_l2.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_cache_op_unit_l2.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v

EXTRA_FLAGS := -Wno-WIDTH -Wno-SYMRSVDWORD -Wno-COMBDLY -Wno-UNUSED

include $(REPO_ROOT)/mk/rtlcosim.mk
