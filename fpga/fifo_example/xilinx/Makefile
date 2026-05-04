# fifo_example — Xilinx Alveo U200 (Ultrascale+) synthesis head

REPO_ROOT := $(shell git rev-parse --show-toplevel)
TECH      := xilinx
include $(REPO_ROOT)/mk/prim.mk

SYNTH_TOP := fifo_example

RTL_SRCS := \
  $(PRIM_FIFO_SYNC) \
  $(CURDIR)/../rtl/fifo_example.sv

include $(REPO_ROOT)/mk/syn-xilinx.mk
