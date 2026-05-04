REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := minion_dcache_tensor_load

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(PRIM_RF_SINGLE_1R1W_PAR_PREVIEW) \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_tensor_load.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_tensor_load.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate_n.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_single_1r_1w_par.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches
EXTRA_FLAGS := -Wno-WIDTH -Wno-SYMRSVDWORD -Wno-COMBDLY -Wno-UNUSEDSIGNAL -Wno-UNUSED -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
