# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := neigh_voltage_cross_tb

NEIGH_VOLTAGE_CROSS_SRCS := \
  $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
  $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
  $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_constants.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_apb_req.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_apb_rsp.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_bpam.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_ipi.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_uc_fcc.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_icache_err.sv \
  $(PRIM_RST_SYNC) \
  $(PRIM_FIFO_SEMISYNC_HIV) \
  $(PRIM_FIFO_SEMISYNC_LOV) \
  $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_hi_voltage_cross.sv \
  $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_lo_voltage_cross.sv

RTL_SRCS := $(NEIGH_VOLTAGE_CROSS_SRCS) neigh_voltage_cross_tb.sv
LINT_SRCS := $(NEIGH_VOLTAGE_CROSS_SRCS)
CC_SRCS := neigh_voltage_cross_test.cc

include $(REPO_ROOT)/mk/verilator.mk
