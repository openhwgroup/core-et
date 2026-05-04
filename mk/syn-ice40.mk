# Common iCE40 synthesis rules.
#
# Thin target-specific wrapper around mk/yosys.mk. Modules or synthesis heads
# that target iCE40 should include this file instead of setting the Yosys pass
# directly.

REPO_ROOT ?= $(shell git rev-parse --show-toplevel)

YOSYS_SYNTH_PASS := synth_ice40
SYNTH_LABEL      := ice40

include $(REPO_ROOT)/mk/yosys.mk
