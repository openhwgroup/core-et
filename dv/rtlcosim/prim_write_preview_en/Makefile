REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := prim_write_preview_en

NEW_RTL := $(PRIM_WRITE_PREVIEW_EN)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate_n.v \
            $(ORIG_ROOT)/rtl/libs/registers/rlatchn.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers

EXTRA_FLAGS := -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
