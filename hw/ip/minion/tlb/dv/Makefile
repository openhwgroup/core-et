REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := minion_tlb_tb

RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(PRIM_RF_1R1W_PREVIEW) \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv \
            $(CURDIR)/minion_tlb_tb.sv

CC_SRCS := $(CURDIR)/minion_tlb_test.cc

LINT_SRCS := $(RTL_SRCS)

include $(REPO_ROOT)/mk/verilator.mk
