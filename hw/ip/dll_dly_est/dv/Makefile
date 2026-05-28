# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP := dll_dly_est_tb

DLL_DLY_EST_SRCS := \
  $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
  $(REPO_ROOT)/hw/ip/dll_dly_est/rtl/dll_dly_est_core.sv \
  $(REPO_ROOT)/hw/ip/dll_dly_est/rtl/dll_dly_est.sv

RTL_SRCS := $(DLL_DLY_EST_SRCS) dll_dly_est_tb.sv
LINT_SRCS := $(DLL_DLY_EST_SRCS)
CC_SRCS := dll_dly_est_test.cc

include $(REPO_ROOT)/mk/verilator.mk
