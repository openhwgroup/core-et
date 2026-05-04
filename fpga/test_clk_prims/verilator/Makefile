# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)

TECH ?= generic
include $(CURDIR)/../srcs.mk

TB_TOP    := test_clk_prims
CC_SRCS   := $(CURDIR)/test_clk_prims_test.cc
BUILD_DIR := $(CURDIR)/build-$(TECH)

include $(REPO_ROOT)/mk/verilator.mk
