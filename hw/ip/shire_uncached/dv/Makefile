# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)
TECH ?= generic
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := shire_uncached_fcc_tb

RTL_SRCS := \
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
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/dv/shire_uncached_fcc_tb.sv

CC_SRCS := $(REPO_ROOT)/hw/ip/shire_uncached/dv/shire_uncached_fcc_test.cc

include $(REPO_ROOT)/mk/verilator.mk
