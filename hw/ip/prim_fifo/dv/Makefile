REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_fifo_tb
RTL_SRCS := $(CURDIR)/prim_fifo_tb.sv \
            $(PRIM_FIFO) \
            $(PRIM_RF_1R1W_PREVIEW)
CC_SRCS  := $(CURDIR)/prim_fifo_test.cc

include $(REPO_ROOT)/mk/verilator.mk
