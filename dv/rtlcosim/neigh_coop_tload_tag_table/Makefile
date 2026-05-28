# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_coop_tload_tag_table

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(REPO_ROOT)/hw/ip/prim_arb_lru/rtl/prim_arb_lru.sv \
           $(REPO_ROOT)/hw/ip/neigh_coop_tload/rtl/neigh_coop_tload_tag_table.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_coop_tload_tag_table.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/arbiters
ORIG_RENAME := neigh_coop_tload_tag_table
# Original tag-table ready-generation logic forms a harmless combinational loop
# through wr_ready/wr_en that the new RTL preserves; new RTL lint is covered by
# the IP lint target.
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SELRANGE -Wno-SYMRSVDWORD \
               -Wno-UNOPTFLAT

include $(REPO_ROOT)/mk/rtlcosim.mk
