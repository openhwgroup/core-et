REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := prim_rf_single_1r1w_par_preview_tb

RTL_SRCS := $(PRIM_RF_SINGLE_1R1W_PAR_PREVIEW) \
            $(CURDIR)/prim_rf_single_1r1w_par_preview_tb.sv

CC_SRCS := $(CURDIR)/prim_rf_single_1r1w_par_preview_test.cc

include $(REPO_ROOT)/mk/verilator.mk
