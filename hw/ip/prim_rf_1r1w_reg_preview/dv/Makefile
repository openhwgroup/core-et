REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := prim_rf_1r1w_reg_preview_tb

RTL_SRCS := $(PRIM_RF_1R1W_REG_PREVIEW) \
            $(CURDIR)/prim_rf_1r1w_reg_preview_tb.sv

CC_SRCS := $(CURDIR)/prim_rf_1r1w_reg_preview_test.cc

include $(REPO_ROOT)/mk/verilator.mk
