REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_rf_3r2w_tb
RTL_SRCS := $(PRIM_RF_3R2W) $(CURDIR)/prim_rf_3r2w_tb.sv
CC_SRCS  := $(CURDIR)/prim_rf_3r2w_test.cc

include $(REPO_ROOT)/mk/verilator.mk
