# Shared RTL source list for test_mul_div.
# Included by each head's Makefile.
# Head sets TECH before including this file.

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKGS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
        $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv

PRIMS := $(PRIM_CLK_GATE) \
         $(PRIM_CLK_GATE_N) \
         $(PRIM_PHASE_PAIR_LO_HI) \
         $(PRIM_PHASE_PAIR_HI_LO) \
         $(PRIM_MUL_DIV_SRCS) \
         $(PRIM_CMP_32) \
         $(PRIM_CMP_42)

MUL_DIV_RTL := $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_mul_div_top.sv

UART_MODS := $(REPO_ROOT)/fpga/ulx3s_uart_bringup/rtl/ulx3s_uart_tx.sv

RTL_SRCS := $(PKGS) $(PRIMS) $(MUL_DIV_RTL) $(UART_MODS) \
            $(CURDIR)/../rtl/test_mul_div_tap.sv \
            $(CURDIR)/../rtl/test_mul_div.sv
