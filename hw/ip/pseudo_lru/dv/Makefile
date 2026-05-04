REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := pseudo_lru_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/pseudo_lru/rtl/pseudo_lru.sv \
            $(CURDIR)/pseudo_lru_tb.sv
CC_SRCS  := $(CURDIR)/pseudo_lru_test.cc

include $(REPO_ROOT)/mk/verilator.mk
