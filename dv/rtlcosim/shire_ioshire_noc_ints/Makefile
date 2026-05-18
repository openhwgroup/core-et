# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := shire_ioshire_noc_ints

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_channel_leaves_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_ioshire_noc_ints.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_channel/shire_ioshire_noc_ints.v
ORIG_RENAME := shire_ioshire_noc_ints
EXTRA_FLAGS := -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
