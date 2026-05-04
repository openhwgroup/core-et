REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_rf_1r1w_dec
RTL_SRCS := $(PRIM_RF_1R1W_DEC)
CC_SRCS  := $(CURDIR)/prim_rf_1r1w_dec_test.cc

include $(REPO_ROOT)/mk/verilator.mk
