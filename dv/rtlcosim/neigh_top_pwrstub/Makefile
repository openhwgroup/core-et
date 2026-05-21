# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_top_pwrstub

NEW_RTL := \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
  $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_top/rtl/neigh_top_pwrstub.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_top_pwrstub.v
ORIG_RENAME := neigh_top_pwrstub

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-WIDTH -Wno-TIMESCALEMOD -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND
SIM_ARGS := +verilator+noassert

include $(REPO_ROOT)/mk/rtlcosim.mk
