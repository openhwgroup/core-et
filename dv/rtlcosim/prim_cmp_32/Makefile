REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := prim_cmp_32

NEW_RTL  := $(PRIM_CMP_32)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/compressors/r32cmp.v

ORIG_RENAME := r32cmp

include $(REPO_ROOT)/mk/rtlcosim.mk
