REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_mul_div_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(PRIM_PHASE_PAIR_LO_HI) \
            $(PRIM_PHASE_PAIR_HI_LO) \
            $(PRIM_CLK_GATE) \
            $(PRIM_CLK_GATE_N) \
            $(PRIM_CMP_32) \
            $(PRIM_CMP_42) \
            $(PRIM_MUL_DIV_SRCS) \
            $(CURDIR)/prim_mul_div_tb.sv
CC_SRCS  := $(CURDIR)/prim_mul_div_test.cc

include $(REPO_ROOT)/mk/verilator.mk
