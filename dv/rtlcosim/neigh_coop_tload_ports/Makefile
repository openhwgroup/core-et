# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_coop_tload_ports

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_coop_tload/rtl/neigh_coop_tload_ports.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_coop_tload_ports.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/vclevel_shft_l2h.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/vclevel_shft_h2l.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_combo_buf.v
ORIG_RENAME := neigh_coop_tload_ports
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SELRANGE -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
