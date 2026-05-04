REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_phase_pair_lo_hi_tb
RTL_SRCS := $(PRIM_PHASE_PAIR_LO_HI) \
            $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(PRIM_CLK_GATE_N) \
            $(CURDIR)/prim_phase_pair_lo_hi_tb.sv
CC_SRCS  := $(CURDIR)/prim_phase_pair_lo_hi_test.cc

include $(REPO_ROOT)/mk/verilator.mk
