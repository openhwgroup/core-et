REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := arb_lru_2bid

NEW_RTL  := $(PRIM_ARB_LRU_2BID)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/arbiters/arb_lru_2_bid.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
