REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := rf_3r2w_preview

NEW_RTL := $(PRIM_RF_3R2W_PREVIEW)

ORIG_RTL := $(ORIG_ROOT)/rtl/libs/registers/rlatchn.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_3r_2w.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches

EXTRA_FLAGS := -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
