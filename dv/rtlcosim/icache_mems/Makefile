# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := icache_mems

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
            $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_pkg.sv \
            $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_channel_leaves_pkg.sv \
            $(REPO_ROOT)/hw/ip/prim_ecc/rtl/prim_ecc_enc.sv \
            $(REPO_ROOT)/hw/ip/tech_generic/prim_clk_mux/rtl/prim_clk_mux.sv \
            $(REPO_ROOT)/hw/ip/tech_generic/prim_ram_1p/rtl/prim_ram_1p.sv \
            $(REPO_ROOT)/hw/ip/icache_data_ram_wrap/rtl/icache_data_ram_wrap.sv \
            $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/icache_mems.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/ecc/ecc_gen.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_mem1p.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_mux2.v \
            $(ORIG_ROOT)/rtl/shire/shire_channel/icache/icache_data_ram_wrap.v \
            $(ORIG_ROOT)/rtl/shire/shire_channel/icache/icache_mems.v

ORIG_RENAME := icache_data_ram_wrap icache_mems
EXTRA_FLAGS := -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
