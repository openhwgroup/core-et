REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := sp_apb_mux_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/sp_apb_mux/rtl/sp_apb_mux.sv \
            $(CURDIR)/sp_apb_mux_tb.sv
CC_SRCS  := $(CURDIR)/sp_apb_mux_test.cc

include $(REPO_ROOT)/mk/verilator.mk

