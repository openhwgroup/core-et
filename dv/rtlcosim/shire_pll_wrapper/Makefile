# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := shire_pll_wrapper

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_channel_leaves_pkg.sv \
           $(REPO_ROOT)/hw/ip/tech_generic/prim_clk_mux/rtl/prim_clk_mux.sv \
           $(REPO_ROOT)/hw/ip/tech_generic/prim_clk_gate/rtl/prim_clk_gate.sv \
           $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_xll_control.sv \
           $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_pll_wrapper.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/libs/mems_and_fifos/et_cdc_sync.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/sync_rst.v \
            $(ORIG_ROOT)/rtl/shire/shire_channel/shire_xll_control.v \
            $(CURDIR)/pll_debug_stub.sv \
            $(ORIG_ROOT)/rtl/shire/shire_channel/shire_pll_wrapper.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches
ORIG_RENAME := shire_pll_wrapper shire_xll_control
EXTRA_FLAGS := --no-timing -Wno-COMBDLY -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-SYMRSVDWORD -Wno-REALCVT -Wno-STMTDLY -DET_ASSERT_OFF
SIM_ARGS := +verilator+noassert

include $(REPO_ROOT)/mk/rtlcosim.mk
