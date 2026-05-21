# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_pmu
include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(PRIM_CLK_GATE) \
           $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl/neigh_pmu.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_pmu.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches
ORIG_RENAME := neigh_pmu
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SELRANGE -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
