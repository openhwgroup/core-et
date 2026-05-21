# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKG_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv

PRIM_SRCS := $(PRIM_ARB_LRU)

COOP_RTL_SRCS := $(REPO_ROOT)/hw/ip/neigh_coop_tload/rtl/neigh_coop_tload_tag_table.sv \
                 $(REPO_ROOT)/hw/ip/neigh_coop_tload/rtl/neigh_coop_tload_ports.sv \
                 $(REPO_ROOT)/hw/ip/neigh_coop_tload/rtl/neigh_coop_tload.sv

TB_TOP := neigh_coop_tload_tb
RTL_SRCS := $(PKG_SRCS) $(PRIM_SRCS) $(COOP_RTL_SRCS) $(CURDIR)/neigh_coop_tload_tb.sv
LINT_SRCS := $(RTL_SRCS)
CC_SRCS := $(CURDIR)/neigh_coop_tload_test.cc

include $(REPO_ROOT)/mk/verilator.mk
