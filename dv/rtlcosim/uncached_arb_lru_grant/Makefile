# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := uncached_arb_lru_grant

NEW_RTL := \
  $(REPO_ROOT)/hw/ip/shire_uncached/rtl/uncached_arb_lru_grant.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/libs/arbiters/arb_lru_grant.v
ORIG_RENAME := arb_lru_grant
EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
