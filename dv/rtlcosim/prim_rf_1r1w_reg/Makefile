REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := rf_1r1w_reg

NEW_RTL  := $(PRIM_RF_1R1W_REG)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_1w_reg.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-DECLFILENAME -Wno-COMBDLY -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
