REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_arb_rr2
RTL_SRCS := $(PRIM_ARB_RR) \
            $(PRIM_ARB_RR2)
CC_SRCS  := $(CURDIR)/prim_arb_rr2_test.cc

include $(REPO_ROOT)/mk/verilator.mk
