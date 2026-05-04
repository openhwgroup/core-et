REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := rf_1r1w_diff

NEW_RTL  := $(PRIM_RF_1R1W_DIFF)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_1w_diff_widths.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs

include $(REPO_ROOT)/mk/rtlcosim.mk
