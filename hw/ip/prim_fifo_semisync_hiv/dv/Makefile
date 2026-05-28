# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)

include $(REPO_ROOT)/mk/prim.mk

TESTS := hiv lov

hiv_TOP  := prim_fifo_semisync_hiv
hiv_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(PRIM_FIFO_SEMISYNC_HIV) \
            $(CURDIR)/prim_fifo_semisync_hiv_test.cc

lov_TOP  := prim_fifo_semisync_lov
lov_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(PRIM_FIFO_SEMISYNC_LOV) \
            $(CURDIR)/prim_fifo_semisync_lov_test.cc

LINT_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
             $(PRIM_FIFO_SEMISYNC_HIV) \
             $(PRIM_FIFO_SEMISYNC_LOV)
LINT_FLAGS := -Wno-MULTITOP

include $(REPO_ROOT)/mk/verilator.mk
