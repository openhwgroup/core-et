REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := prim_write_commit_rst_en

NEW_RTL := $(PRIM_WRITE_COMMIT_RST_EN)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches

EXTRA_FLAGS := -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
