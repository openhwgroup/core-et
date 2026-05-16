# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := neigh_lo_voltage_cross

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
           $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_apb_rsp.sv \
           $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_icache_err.sv \
           $(PRIM_RST_SYNC) \
           $(PRIM_FIFO_SEMISYNC_LOV) \
           $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_lo_voltage_cross.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_lo_voltage_cross.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_apb_rsp.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_icache_err.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches

ORIG_RENAME := neigh_lo_voltage_cross \
               neigh_hv_logic_apb_rsp \
               neigh_hv_logic_icache_err

EXTRA_FLAGS := -DET_ASSERT_OFF -DUNIT_TEST -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
