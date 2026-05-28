# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_tensor_store_buffer
include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(PRIM_ARB_RR) \
           $(PRIM_FIFO) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(REPO_ROOT)/hw/ip/neigh_tensor_store_buffer/rtl/neigh_tensor_store_buffer_block.sv \
           $(REPO_ROOT)/hw/ip/neigh_tensor_store_buffer/rtl/neigh_tensor_store_buffer.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_tensor_store_buffer.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_tensor_store_buffer_block.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/arbiters \
                $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches
ORIG_RENAME := neigh_tensor_store_buffer_block neigh_tensor_store_buffer
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SELRANGE -Wno-SYMRSVDWORD \
               -Wno-LATCH -Wno-UNOPTFLAT

include $(REPO_ROOT)/mk/rtlcosim.mk
