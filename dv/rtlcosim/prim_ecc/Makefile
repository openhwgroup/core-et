REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := ecc

NEW_RTL  := $(PRIM_ECC_ENC) \
            $(PRIM_ECC_DEC)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/ecc/ecc_gen.v \
            $(ORIG_ROOT)/rtl/libs/ecc/ecc_corr.v

include $(REPO_ROOT)/mk/rtlcosim.mk
