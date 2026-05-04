REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := prim_queue_arb_lru
RTL_SRCS := $(REPO_ROOT)/hw/ip/prim_queue_arb_lru/rtl/prim_queue_arb_lru.sv
CC_SRCS  := $(CURDIR)/prim_queue_arb_lru_test.cc

include $(REPO_ROOT)/mk/verilator.mk
