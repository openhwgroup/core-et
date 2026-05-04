# Shared RTL source list for test_frontend.
# Included by each head's Makefile.
# Head sets TECH before including this file.

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

RTL_DIR := $(REPO_ROOT)/hw/ip
FE_RTL  := $(RTL_DIR)/minion/frontend/rtl

PKGS := $(RTL_DIR)/dft/rtl/dft_pkg.sv \
        $(RTL_DIR)/etlink/rtl/etlink_pkg.sv \
        $(RTL_DIR)/minion/rtl/minion_pkg.sv \
        $(FE_RTL)/minion_frontend_pkg.sv

PRIM := $(PRIM_CLK_GATE) \
        $(PRIM_WRITE_PREVIEW_EN) \
        $(PRIM_ARB_LRU) \
        $(PRIM_RF_1R1W_DIFF_PREVIEW) \
        $(PRIM_RF_1R1W_DIFF) \
        $(RTL_DIR)/prim_fifo_sync/rtl/prim_fifo_sync.sv

FE_MODS := $(FE_RTL)/minion_frontend_rvc_expander.sv \
           $(FE_RTL)/minion_frontend_thread_buffer.sv \
           $(FE_RTL)/minion_frontend_thread_sched.sv \
           $(FE_RTL)/intpipe_decode.sv \
           $(FE_RTL)/vpu_decoder.sv \
           $(FE_RTL)/minion_frontend.sv

UART_MODS := $(REPO_ROOT)/fpga/ulx3s_uart_bringup/rtl/ulx3s_uart_tx.sv

RTL_SRCS := $(PKGS) $(PRIM) $(FE_MODS) $(UART_MODS) \
            $(CURDIR)/../rtl/test_frontend.sv
