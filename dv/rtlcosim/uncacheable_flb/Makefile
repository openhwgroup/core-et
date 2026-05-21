# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := uncacheable_flb
TECH ?= generic
include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := \
  $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
  $(REPO_ROOT)/hw/ip/axi/rtl/axi_pkg.sv \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
  $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/shire_uncached_pkg.sv \
  $(PRIM_CLK_GATE) \
  $(PRIM_FIFO_REG) \
  $(PRIM_ARB_LRU) \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable_flb.sv

ORIG_RTL := \
  $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v \
  $(ORIG_ROOT)/rtl/libs/registers/en_ff_gated_struct.v \
  $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_fifo_reg.v \
  $(ORIG_ROOT)/rtl/libs/arbiters/arb_lru.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_flb.v

ORIG_RENAME := uncacheable_flb
SIM_ARGS := +verilator+noassert

include $(REPO_ROOT)/mk/rtlcosim.mk
