# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := shire_dmctrl

NEW_RTL := $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_channel_leaves/rtl/shire_dmctrl.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_channel/shire_dmctrl.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos
ORIG_RENAME := shire_dmctrl
EXTRA_FLAGS := -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
