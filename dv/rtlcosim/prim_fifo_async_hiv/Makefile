# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := prim_fifo_async_hiv

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(PRIM_FIFO_ASYNC_HIV)

ORIG_RTL := $(ORIG_ROOT)/rtl/libs/mems_and_fifos/vcfifo_wr_hiv_gcd.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
