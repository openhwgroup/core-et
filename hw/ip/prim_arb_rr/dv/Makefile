REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_arb_rr
RTL_SRCS := $(PRIM_ARB_RR)
CC_SRCS  := $(CURDIR)/prim_arb_rr_test.cc

include $(REPO_ROOT)/mk/verilator.mk
