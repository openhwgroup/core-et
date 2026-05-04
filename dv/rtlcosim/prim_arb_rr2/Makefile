REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := arb_rr2

NEW_RTL  := $(PRIM_ARB_RR) \
            $(PRIM_ARB_RR2)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr2.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr_base.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_grants2.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/hot2bin.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/onehot_mux.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
