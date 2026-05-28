# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)

PKG_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux_pkg.sv

RTL_SRCS := $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/bpam2minions.sv \
            $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux.sv

TESTS := bpam2minions neigh_ch_apb_mux

bpam2minions_TOP := bpam2minions_tb
bpam2minions_SRCS := $(PKG_SRCS) \
                      $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/bpam2minions.sv \
                      $(CURDIR)/bpam2minions_tb.sv \
                      $(CURDIR)/bpam2minions_test.cc

neigh_ch_apb_mux_TOP := neigh_ch_apb_mux_tb
neigh_ch_apb_mux_SRCS := $(PKG_SRCS) $(RTL_SRCS) \
                          $(CURDIR)/neigh_ch_apb_mux_tb.sv \
                          $(CURDIR)/neigh_ch_apb_mux_test.cc

LINT_SRCS := $(PKG_SRCS) $(RTL_SRCS)

include $(REPO_ROOT)/mk/verilator.mk
