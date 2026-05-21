# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := prim_fifo_semisync_lov

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(PRIM_FIFO_SEMISYNC_LOV)

ORIG_RTL := $(ORIG_ROOT)/rtl/libs/mems_and_fifos/vcfifo_wr_lov_ss.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches

# Original gen_fifo_reg macro zero-extension emits WIDTHEXPAND under Verilator.
EXTRA_FLAGS := -DET_ASSERT_OFF -DUNIT_TEST -Wno-WIDTHEXPAND

include $(REPO_ROOT)/mk/rtlcosim.mk
