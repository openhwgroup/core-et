# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := uncacheable
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
  $(PRIM_RF_3R2W) \
  $(PRIM_FIFO_ASYNC_HIV) \
  $(PRIM_FIFO_ASYNC_LOV) \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable_fcc.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable_flb.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/noc_etlink_to_axi.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncached_arb_lru_grant.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable_noc_slv_handler.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable_noc_tol3_handler.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable_noc_tosys_handler.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable_noc_rsp.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable_noc.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncached_hi_voltage_cross_lv_logic.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncached_hi_voltage_cross.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncached_lo_voltage_cross_lv_logic.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncached_lo_voltage_cross.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable.sv

ORIG_RTL := \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_fcc.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_flb.v \
  $(ORIG_ROOT)/rtl/shire/uncached/noc_etlink_to_axi.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc_slv_handler.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc_tol3_handler.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc_tosys_handler.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc_rsp.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_hi_voltage_cross_lv_logic.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_hi_voltage_cross.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_lo_voltage_cross_lv_logic.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_lo_voltage_cross.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/arbiters \
                $(ORIG_ROOT)/rtl/libs/registers

ORIG_RENAME := uncacheable_noc_tol3_handler \
               uncacheable_noc_tosys_handler \
               uncacheable_noc_slv_handler \
               uncached_hi_voltage_cross_lv_logic \
               uncached_lo_voltage_cross_lv_logic \
               uncached_hi_voltage_cross \
               uncached_lo_voltage_cross \
               noc_etlink_to_axi \
               uncacheable_noc_rsp \
               uncacheable_noc \
               uncacheable_fcc \
               uncacheable_flb \
               uncacheable

SIM_ARGS := +verilator+noassert

include $(REPO_ROOT)/mk/rtlcosim.mk
