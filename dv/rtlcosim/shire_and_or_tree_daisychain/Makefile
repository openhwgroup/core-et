# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := shire_and_or_tree_daisychain

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_channel_leaves_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_and_or_tree_daisychain.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_channel/shire_and_or_tree_daisychain.v
ORIG_RENAME := shire_and_or_tree_daisychain
EXTRA_FLAGS := -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
