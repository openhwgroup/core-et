REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_ecc_dec
RTL_SRCS := $(PRIM_ECC_ENC) \
            $(PRIM_ECC_DEC)
CC_SRCS  := $(CURDIR)/prim_ecc_test.cc

include $(REPO_ROOT)/mk/verilator.mk
