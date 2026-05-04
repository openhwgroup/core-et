REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := minion_dcache_tlb_array

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(PRIM_RF_1R1W_PREVIEW) \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_tlb_array.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/inc/macros.vh \
            $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_tlb_array.v \
            $(ORIG_ROOT)/rtl/shire/minion/tlb_top.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate_n.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_1w.v

EXTRA_FLAGS := -Wno-DECLFILENAME -Wno-UNUSED -Wno-WIDTH -Wno-SYMRSVDWORD \
               -Wno-TIMESCALEMOD -Wno-VARHIDDEN -Wno-CASEINCOMPLETE \
               -Wno-COMBDLY -Wno-UNOPTFLAT

include $(REPO_ROOT)/mk/rtlcosim.mk
