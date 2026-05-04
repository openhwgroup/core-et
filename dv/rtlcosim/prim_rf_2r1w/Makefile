REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := rf_2r1w

NEW_RTL  := $(PRIM_RF_2R1W)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_2r_1w.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs

include $(REPO_ROOT)/mk/rtlcosim.mk
