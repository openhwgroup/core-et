# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := uncached_hi_voltage_cross_lv_logic

NEW_RTL := \
  $(REPO_ROOT)/hw/ip/axi/rtl/axi_pkg.sv \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncached_hi_voltage_cross_lv_logic.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/uncached/uncached_hi_voltage_cross_lv_logic.v
ORIG_RENAME := uncached_hi_voltage_cross_lv_logic

include $(REPO_ROOT)/mk/rtlcosim.mk
