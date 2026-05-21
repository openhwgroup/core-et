# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_tbox_router
include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(PRIM_FIFO) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(PRIM_ARB_RR) \
           $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl/neigh_tbox_router.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_tbox_router.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/arbiters \
                $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches
ORIG_RENAME := neigh_tbox_router
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SELRANGE -Wno-SYMRSVDWORD -Wno-COMBDLY

include $(REPO_ROOT)/mk/rtlcosim.mk
