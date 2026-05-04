REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_arb_prio
RTL_SRCS := $(PRIM_ARB_PRIO)
CC_SRCS  := $(CURDIR)/prim_arb_prio_test.cc

include $(REPO_ROOT)/mk/verilator.mk
