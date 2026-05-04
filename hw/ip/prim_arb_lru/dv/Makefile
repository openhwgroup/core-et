REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_arb_lru_2bid
RTL_SRCS := $(PRIM_ARB_LRU_2BID)
CC_SRCS  := $(CURDIR)/prim_arb_lru_2bid_test.cc

include $(REPO_ROOT)/mk/verilator.mk
