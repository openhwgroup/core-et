# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP := neigh_ch_dbg_tb

NEIGH_CH_DBG_SRCS := \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
  $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_ch_dbg/rtl/neigh_ch_dbg_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_ch_dbg/rtl/neigh_ch_dbg.sv

RTL_SRCS := $(NEIGH_CH_DBG_SRCS) neigh_ch_dbg_tb.sv
LINT_SRCS := $(NEIGH_CH_DBG_SRCS)
CC_SRCS := neigh_ch_dbg_test.cc

include $(REPO_ROOT)/mk/verilator.mk
