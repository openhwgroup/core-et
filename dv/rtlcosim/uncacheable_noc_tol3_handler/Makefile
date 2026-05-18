# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := uncacheable_noc_tol3_handler

NEW_RTL := \
  $(REPO_ROOT)/hw/ip/axi/rtl/axi_pkg.sv \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
  $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/shire_uncached_pkg.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncacheable_noc_tol3_handler.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc_tol3_handler.v
ORIG_RENAME := uncacheable_noc_tol3_handler

include $(REPO_ROOT)/mk/rtlcosim.mk
