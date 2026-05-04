# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

# Shared RTL source list for test_clk_prims.
# Included by each head's Makefile.
# Head sets TECH before including this file.

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKGS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv

PRIMS := $(PRIM_CLK_GATE) \
         $(PRIM_CLK_GATE_N) \
         $(PRIM_CLK_MUX)

UART_MODS := $(REPO_ROOT)/fpga/ulx3s_uart_bringup/rtl/ulx3s_uart_tx.sv

RTL_SRCS := $(PKGS) $(PRIMS) $(UART_MODS) $(CURDIR)/../rtl/test_clk_prims.sv
