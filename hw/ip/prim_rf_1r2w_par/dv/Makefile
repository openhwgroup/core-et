REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_rf_1r2w_par
RTL_SRCS := $(PRIM_RF_1R2W_PAR)
CC_SRCS  := $(CURDIR)/prim_rf_1r2w_par_test.cc

include $(REPO_ROOT)/mk/verilator.mk
