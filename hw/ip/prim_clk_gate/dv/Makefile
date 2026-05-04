REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := prim_clk_gate_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/tech_generic/prim_clk_gate/rtl/prim_clk_gate.sv \
            $(CURDIR)/prim_clk_gate_tb.sv
CC_SRCS  := $(CURDIR)/prim_clk_gate_test.cc

include $(REPO_ROOT)/mk/verilator.mk
