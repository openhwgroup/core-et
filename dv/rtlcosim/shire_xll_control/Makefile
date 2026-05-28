# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := shire_xll_control

NEW_RTL := $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_xll_control.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_channel/shire_xll_control.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches
ORIG_RENAME := shire_xll_control
EXTRA_FLAGS := -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
