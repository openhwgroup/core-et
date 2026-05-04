REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := lru_sel
RTL_SRCS := $(REPO_ROOT)/hw/ip/lru_sel/rtl/lru_sel.sv
CC_SRCS  := $(CURDIR)/lru_sel_test.cc

include $(REPO_ROOT)/mk/verilator.mk
