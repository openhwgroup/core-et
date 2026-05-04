REPO_ROOT  := $(shell git rev-parse --show-toplevel)

COSIM_NAME := lru_sel

NEW_RTL  := $(REPO_ROOT)/hw/ip/lru_sel/rtl/lru_sel.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/arbiters/lru_sel.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/arbiters

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-DECLFILENAME
ORIG_RENAME := lru_sel

include $(REPO_ROOT)/mk/rtlcosim.mk
