REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := rf_single_1r1w_par

NEW_RTL  := $(PRIM_RF_SINGLE_1R1W_PAR)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_single_1r_1w_par.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-DECLFILENAME -Wno-COMBDLY -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
