# Shared RTL source list for test_dcache.
# Included by each head's Makefile.
# Head sets TECH before including this file.

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKGS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
        $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
        $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv

COMMON_RTL := $(PKGS) $(PRIM_WRITE_PREVIEW_EN)
PRIM_RTL := $(PRIM_RF_1R1W)
PRIM_RF_HELPER_RTL := $(PRIM_RF_1R1W_REG) $(PRIM_RF_1R1W_REG_PREVIEW) $(PRIM_RF_1R1W_PAR) $(PRIM_RF_1R1W_PAR_PREVIEW) $(PRIM_RF_SINGLE_1R1W_PAR) $(PRIM_RF_SINGLE_1R1W_PAR_PREVIEW)
PRIM_ARB_RTL := $(PRIM_ARB_LRU)
PRIM_FIFO_RTL := $(PRIM_FIFO_SYNC)
RAM_PRIM_RTL := $(PRIM_RAM_2P)
TLB_RTL := $(PRIM_CLK_GATE) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv

DCACHE_RTL := $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_atomic_alu_unit.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_store_merge_unit.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pma_unit.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_lru_array.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_metadata_array.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_buffer_array.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_128x64_1r1w_lram.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_128x72_1r1w_lram.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_data_array.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_tlb_array.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_replay_queue.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_miss_handler.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_miss_handler_unit.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_cache_op_unit.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_cache_op_unit_l2.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_writeback_unit.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_scratchpad_ctrl.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_texsend.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_reduce.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_tensor_load.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_top.sv

WRAPPER_RTL := $(REPO_ROOT)/hw/ip/minion/dcache/dv/minion_dcache_top_tb.sv
UART_MODS := $(REPO_ROOT)/fpga/ulx3s_uart_bringup/rtl/ulx3s_uart_tx.sv

RTL_SRCS := $(COMMON_RTL) \
            $(PRIM_RTL) \
            $(PRIM_RF_HELPER_RTL) \
            $(PRIM_ARB_RTL) \
            $(PRIM_FIFO_RTL) \
            $(TLB_RTL) \
            $(RAM_PRIM_RTL) \
            $(DCACHE_RTL) \
            $(WRAPPER_RTL) \
            $(UART_MODS) \
            $(CURDIR)/../rtl/test_dcache.sv
