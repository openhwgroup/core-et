REPO_ROOT  := $(shell git rev-parse --show-toplevel)

COSIM_NAME := queue_arb_lru

NEW_RTL  := $(REPO_ROOT)/hw/ip/prim_queue_arb_lru/rtl/prim_queue_arb_lru.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/arbiters/queue_arb_lru.v

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-DECLFILENAME -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
