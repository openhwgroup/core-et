# fifo_example — iCE40 HX8K synthesis head

REPO_ROOT := $(shell git rev-parse --show-toplevel)
TECH      := ice40
include $(REPO_ROOT)/mk/prim.mk

SYNTH_TOP := fifo_example

RTL_SRCS := \
  $(PRIM_FIFO_SYNC) \
  $(CURDIR)/../rtl/fifo_example.sv

include $(REPO_ROOT)/mk/syn-ice40.mk
