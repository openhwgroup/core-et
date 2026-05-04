REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := prim_cmp_42

NEW_RTL  := $(PRIM_CMP_42)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/compressors/r42cmp.v

ORIG_RENAME := r42cmp

include $(REPO_ROOT)/mk/rtlcosim.mk
