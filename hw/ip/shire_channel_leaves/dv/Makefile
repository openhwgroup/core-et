# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)
TECH ?= generic
include $(REPO_ROOT)/mk/prim.mk

TESTS := leaves

PKG_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
            $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_pkg.sv \
            $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_channel_leaves_pkg.sv

LEAF_SRCS := $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_dmctrl.sv \
             $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_bpam_run_control.sv \
             $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_and_or_tree_daisychain.sv \
             $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_coop_tload_bus.sv \
             $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_ioshire_noc_ints.sv \
             $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_xll_control.sv \
             $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_pll_wrapper.sv \
             $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/icache_mems.sv

RTL_SRCS := $(PKG_SRCS) \
            $(PRIM_CLK_MUX) \
            $(PRIM_CLK_GATE) \
            $(PRIM_RAM_1P) \
            $(REPO_ROOT)/hw/ip/prim_ecc/rtl/prim_ecc_enc.sv \
            $(REPO_ROOT)/hw/ip/icache_data_ram_wrap/rtl/icache_data_ram_wrap.sv \
            $(LEAF_SRCS) \
            $(CURDIR)/shire_channel_leaves_tb.sv

LINT_FLAGS := -Wno-MULTITOP

LINT_SRCS := $(PKG_SRCS) \
             $(PRIM_CLK_MUX) \
             $(PRIM_CLK_GATE) \
             $(PRIM_RAM_1P) \
             $(REPO_ROOT)/hw/ip/prim_ecc/rtl/prim_ecc_enc.sv \
             $(REPO_ROOT)/hw/ip/icache_data_ram_wrap/rtl/icache_data_ram_wrap.sv \
             $(LEAF_SRCS)

leaves_TOP  := shire_channel_leaves_tb
leaves_SRCS := $(RTL_SRCS) $(CURDIR)/shire_channel_leaves_test.cc

include $(REPO_ROOT)/mk/verilator.mk
