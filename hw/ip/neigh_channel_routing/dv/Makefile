# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKG_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
            $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv

PRIM_SRCS := $(PRIM_CLK_GATE) \
             $(PRIM_FIFO) \
             $(PRIM_RF_1R1W_PREVIEW) \
             $(PRIM_ARB_LRU) \
             $(PRIM_ARB_RR)

RTL_SRCS := $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl/neigh_fl_barrier.sv \
            $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl/neigh_local_message_network.sv \
            $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl/neigh_pmu.sv \
            $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl/neigh_tbox_router.sv

TB_TOP := neigh_channel_routing_tb
RTL_SRCS := $(PKG_SRCS) $(PRIM_SRCS) $(RTL_SRCS) $(CURDIR)/neigh_channel_routing_tb.sv
LINT_SRCS := $(RTL_SRCS)
CC_SRCS := $(CURDIR)/neigh_channel_routing_test.cc

include $(REPO_ROOT)/mk/verilator.mk
