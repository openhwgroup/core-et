# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

# ECP5-specific Yosys frontend setup.
#
# The slang frontend elaborates before synth_ecp5 loads the ECP5 library. Pass
# the ECP5 blackbox definitions to read_slang as a slang library so vendor
# primitive parameters and ports are checked against the same cell definitions
# that synth_ecp5 uses later.

REPO_ROOT ?= $(shell git rev-parse --show-toplevel)
YOSYS_CONFIG ?= yosys-config
YOSYS_DATDIR ?= $(shell $(YOSYS_CONFIG) --datdir)

YOSYS_SYNTH_PASS ?= $(if $(SYNTH_CMD),$(SYNTH_CMD),synth_ecp5)
YOSYS_SLANG_FLAGS += -v $(YOSYS_DATDIR)/ecp5/cells_bb.v

include $(REPO_ROOT)/mk/yosys.mk
