REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_hot2bin_tb
RTL_SRCS := $(PRIM_HOT2BIN) \
            $(CURDIR)/prim_hot2bin_tb.sv
CC_SRCS  := $(CURDIR)/prim_hot2bin_test.cc

include $(REPO_ROOT)/mk/verilator.mk
