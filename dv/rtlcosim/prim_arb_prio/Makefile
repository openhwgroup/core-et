REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := arb_prio

NEW_RTL  := $(PRIM_ARB_PRIO)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/arbiters/arb_prio_data_held.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_prio.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
