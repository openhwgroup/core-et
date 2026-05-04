REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_fifo_reg
RTL_SRCS := $(PRIM_FIFO_REG)
CC_SRCS  := $(CURDIR)/prim_fifo_reg_test.cc

include $(REPO_ROOT)/mk/verilator.mk
