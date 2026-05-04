# fifo_example — Verilator simulation head

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := fifo_example
RTL_SRCS := \
  $(PRIM_FIFO_SYNC) \
  $(CURDIR)/../rtl/fifo_example.sv

CC_SRCS  := $(CURDIR)/fifo_example_test.cc

include $(REPO_ROOT)/mk/verilator.mk
