REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := prim_rf_3r2w_preview_tb

RTL_SRCS := $(PRIM_RF_3R2W_PREVIEW) \
            $(CURDIR)/prim_rf_3r2w_preview_tb.sv

CC_SRCS := $(CURDIR)/prim_rf_3r2w_preview_test.cc

include $(REPO_ROOT)/mk/verilator.mk
