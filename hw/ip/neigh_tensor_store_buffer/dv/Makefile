# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKG_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv

PRIM_SRCS := $(PRIM_ARB_RR) \
             $(PRIM_FIFO) \
             $(PRIM_RF_1R1W_PREVIEW)

TSTORE_RTL_SRCS := $(REPO_ROOT)/hw/ip/neigh_tensor_store_buffer/rtl/neigh_tensor_store_buffer_block.sv \
                   $(REPO_ROOT)/hw/ip/neigh_tensor_store_buffer/rtl/neigh_tensor_store_buffer.sv

TB_TOP := neigh_tensor_store_buffer_tb
RTL_SRCS := $(PKG_SRCS) $(PRIM_SRCS) $(TSTORE_RTL_SRCS) $(CURDIR)/neigh_tensor_store_buffer_tb.sv
LINT_SRCS := $(RTL_SRCS)
CC_SRCS := $(CURDIR)/neigh_tensor_store_buffer_test.cc

include $(REPO_ROOT)/mk/verilator.mk
