# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_fl_barrier
include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(PRIM_FIFO) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(PRIM_ARB_LRU) \
           $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl/neigh_fl_barrier.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_fl_barrier.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/arbiters \
                $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches
ORIG_RENAME := neigh_fl_barrier
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SELRANGE -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
