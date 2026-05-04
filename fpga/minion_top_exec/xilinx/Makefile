# minion_top_exec — Xilinx Alveo U200 / UltraScale+ synthesis head

REPO_ROOT      := $(shell git rev-parse --show-toplevel)
TECH           := xilinx
XILINX_FAMILY ?= xcup

include $(CURDIR)/../srcs.mk

SYNTH_TOP := minion_top_exec

include $(REPO_ROOT)/mk/syn-xilinx.mk
