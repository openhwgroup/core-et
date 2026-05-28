# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := dll_dly_est_core

NEW_RTL := $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/dll_dly_est/rtl/dll_dly_est_core.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/dll_dly_est_core.v

ORIG_RENAME := dll_dly_est_core

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
