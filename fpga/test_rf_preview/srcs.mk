# Shared RTL source list for test_rf_preview.
# Included by each head's Makefile.
# Head sets TECH before including this file.

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PRIMS := $(PRIM_CLK_GATE) \
         $(PRIM_WRITE_PREVIEW_EN) \
         $(PRIM_RF_1R1W) \
         $(PRIM_RF_1R1W_DEC) \
         $(PRIM_RF_1R1W_DEC_PREVIEW) \
         $(PRIM_RF_1R1W_PAR) \
         $(PRIM_RF_1R1W_PAR_PREVIEW) \
         $(PRIM_RF_1R1W_PREVIEW) \
         $(PRIM_RF_1R1W_DIFF) \
         $(PRIM_RF_1R1W_DIFF_PREVIEW) \
         $(PRIM_RF_1R1W_REG) \
         $(PRIM_RF_1R1W_REG_PREVIEW) \
         $(PRIM_RF_SINGLE_1R1W_PAR) \
         $(PRIM_RF_SINGLE_1R1W_PAR_PREVIEW) \
         $(PRIM_RF_2R1W) \
         $(PRIM_RF_2R1W_PREVIEW) \
         $(PRIM_RF_3R2W) \
         $(PRIM_RF_3R2W_PREVIEW)

UART_MODS := $(REPO_ROOT)/fpga/ulx3s_uart_bringup/rtl/ulx3s_uart_tx.sv

DCACHE_ARRAY_MODS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
                     $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
                     $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
                     $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
                     $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
                     $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_128x64_1r1w_lram.sv \
                     $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_metadata_array.sv \
                     $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_data_array.sv

RTL_SRCS := $(DCACHE_ARRAY_MODS) $(PRIMS) $(UART_MODS) $(CURDIR)/../rtl/test_rf_preview.sv
