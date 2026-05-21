# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_ch_dbg

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_ch_dbg/rtl/neigh_ch_dbg_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_ch_dbg/rtl/neigh_ch_dbg.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_ch_dbg.v
ORIG_RENAME := neigh_ch_dbg

# Original soc.vh pulls broad unrelated headers; this leaf also preserves a
# vector condition and one-shot status monitor shift that trigger width noise.
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SYMRSVDWORD -Wno-UNOPTFLAT

include $(REPO_ROOT)/mk/rtlcosim.mk
