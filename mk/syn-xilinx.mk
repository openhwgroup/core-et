# Common Xilinx synthesis rules.
#
# Thin target-specific wrapper around mk/yosys.mk. Modules or synthesis heads
# that target Xilinx should include this file instead of setting the Yosys pass
# directly.

REPO_ROOT ?= $(shell git rev-parse --show-toplevel)

XILINX_FAMILY ?= xcup
YOSYS_SYNTH_PASS := synth_xilinx -family $(XILINX_FAMILY)
SYNTH_LABEL      := xilinx-$(XILINX_FAMILY)

include $(REPO_ROOT)/mk/yosys.mk
