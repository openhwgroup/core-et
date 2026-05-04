REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := phase_pair_hi_lo

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(PRIM_PHASE_PAIR_HI_LO)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate_n.v \
            $(ORIG_ROOT)/rtl/libs/registers/rlatch.v \
            $(ORIG_ROOT)/rtl/libs/registers/rlatchn.v

include $(REPO_ROOT)/mk/rtlcosim.mk
