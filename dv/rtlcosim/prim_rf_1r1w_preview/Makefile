REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := rf_1r1w_preview

NEW_RTL := $(PRIM_RF_1R1W_PREVIEW)

ORIG_RTL := $(ORIG_ROOT)/rtl/libs/registers/rlatchn.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_1w.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches

EXTRA_FLAGS := -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
