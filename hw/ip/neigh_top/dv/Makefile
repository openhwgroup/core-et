# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKG_SRCS := \
  $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
  $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
  $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_ch_dbg/rtl/neigh_ch_dbg_pkg.sv

PRIM_SRCS := $(shell find $(REPO_ROOT)/hw/ip/prim_* -path '*/rtl/*.sv' | sort) \
             $(shell find $(REPO_ROOT)/hw/ip/tech_generic -path '*/rtl/*.sv' | sort)

NEIGH_SRCS := $(shell find \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl \
  $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl \
  $(REPO_ROOT)/hw/ip/neigh_fill_fifo/rtl \
  $(REPO_ROOT)/hw/ip/neigh_shared_icache/rtl \
  $(REPO_ROOT)/hw/ip/neigh_shared_ptw/rtl \
  $(REPO_ROOT)/hw/ip/neigh_miss_ff/rtl \
  $(REPO_ROOT)/hw/ip/neigh_evict_ff/rtl \
  $(REPO_ROOT)/hw/ip/neigh_fill_ff/rtl \
  $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl \
  $(REPO_ROOT)/hw/ip/neigh_coop_tload/rtl \
  $(REPO_ROOT)/hw/ip/neigh_tensor_store_buffer/rtl \
  $(REPO_ROOT)/hw/ip/neigh_ch_dbg/rtl \
  $(REPO_ROOT)/hw/ip/dll_dly_est/rtl \
  $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl \
  $(REPO_ROOT)/hw/ip/esr_neigh/rtl \
  $(REPO_ROOT)/hw/ip/icache_top/rtl \
  $(REPO_ROOT)/hw/ip/icache_data_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_data_ram_wrap/rtl \
  $(REPO_ROOT)/hw/ip/icache_lru_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_micro_cache/rtl \
  $(REPO_ROOT)/hw/ip/icache_micro_data_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_micro_tag_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_pma_unit/rtl \
  $(REPO_ROOT)/hw/ip/icache_tag_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_tlb_array/rtl \
  $(REPO_ROOT)/hw/ip/lru_sel/rtl \
  $(REPO_ROOT)/hw/ip/minion/tlb/rtl \
  $(REPO_ROOT)/hw/ip/ptw_top/rtl \
  $(REPO_ROOT)/hw/ip/pseudo_lru/rtl \
  -name '*.sv' ! -name '*_pkg.sv' 2>/dev/null | sort)

CHANNEL_SRCS := $(shell find $(REPO_ROOT)/hw/ip/neigh_channel/rtl -name '*.sv' ! -name 'neigh_channel.sv' | sort) \
                $(REPO_ROOT)/hw/ip/neigh_channel/rtl/neigh_channel.sv

TOP_SRCS := $(PKG_SRCS) $(PRIM_SRCS) $(NEIGH_SRCS) $(CHANNEL_SRCS) \
            $(REPO_ROOT)/hw/ip/neigh_top/rtl/neigh_top_pwrstub.sv \
            $(REPO_ROOT)/hw/ip/neigh_top/rtl/neigh_top.sv

TESTS := pwrstub top

pwrstub_TOP := neigh_top_pwrstub_tb
pwrstub_SRCS := $(PKG_SRCS) \
                $(REPO_ROOT)/hw/ip/neigh_top/rtl/neigh_top_pwrstub.sv \
                $(CURDIR)/neigh_top_pwrstub_tb.sv \
                $(CURDIR)/neigh_top_pwrstub_test.cc

top_TOP := neigh_top_tb
top_SRCS := $(TOP_SRCS) \
            $(CURDIR)/neigh_top_tb.sv \
            $(CURDIR)/neigh_top_test.cc

LINT_SRCS := $(filter-out %.cc,$(top_SRCS))
LINT_FLAGS := --top-module $(top_TOP)

include $(REPO_ROOT)/mk/verilator.mk
