REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_rf_1r1w
RTL_SRCS := $(PRIM_RF_1R1W)
CC_SRCS  := $(CURDIR)/prim_rf_1r1w_test.cc

include $(REPO_ROOT)/mk/verilator.mk
