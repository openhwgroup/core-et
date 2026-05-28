# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_local_message_network

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl/neigh_local_message_network.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_local_message_network.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/arbiters
ORIG_RENAME := neigh_local_message_network
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SELRANGE -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
