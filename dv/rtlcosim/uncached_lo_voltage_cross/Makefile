# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := uncached_lo_voltage_cross
TECH ?= generic
include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := \
  $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
  $(REPO_ROOT)/hw/ip/axi/rtl/axi_pkg.sv \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/shire_uncached_pkg.sv \
  $(PRIM_FIFO_ASYNC_LOV) \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncached_lo_voltage_cross_lv_logic.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncached_lo_voltage_cross.sv

ORIG_RTL := \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_lo_voltage_cross_lv_logic.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_lo_voltage_cross.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches

ORIG_RENAME := uncached_lo_voltage_cross \
               uncached_lo_voltage_cross_lv_logic
SIM_ARGS := +verilator+noassert

include $(REPO_ROOT)/mk/rtlcosim.mk
