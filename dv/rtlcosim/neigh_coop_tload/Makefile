# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_coop_tload

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(REPO_ROOT)/hw/ip/prim_arb_lru/rtl/prim_arb_lru.sv \
           $(REPO_ROOT)/hw/ip/neigh_coop_tload/rtl/neigh_coop_tload_tag_table.sv \
           $(REPO_ROOT)/hw/ip/neigh_coop_tload/rtl/neigh_coop_tload.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_coop_tload.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_coop_tload_tag_table.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/arbiters
ORIG_RENAME := neigh_coop_tload_tag_table neigh_coop_tload
# Original neigh_coop_tload writes disjoint fields of tag_table_wr_req from
# continuous assigns plus always_comb blocks, and the original tag-table ready
# path forms a harmless combinational loop. Keep these suppressions scoped to
# this cosim; new RTL lint is covered by the IP lint target.
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SELRANGE -Wno-SYMRSVDWORD \
               -Wno-MULTIDRIVEN -Wno-UNOPTFLAT

include $(REPO_ROOT)/mk/rtlcosim.mk
