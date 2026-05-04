REPO_ROOT := $(shell git rev-parse --show-toplevel)

RTL_DIR := $(REPO_ROOT)/hw/ip
ALL_RTL := $(RTL_DIR)/dft/rtl/dft_pkg.sv \
           $(RTL_DIR)/ram_cfg/rtl/ram_cfg_pkg.sv \
           $(RTL_DIR)/tech_generic/prim_ram_1p/rtl/prim_ram_1p.sv \
           $(RTL_DIR)/tech_generic/prim_ram_2p/rtl/prim_ram_2p.sv \
           $(RTL_DIR)/prim_fifo_sram/rtl/prim_fifo_sram.sv

TESTS := 2p 1p

2p_TOP  := prim_fifo_sram
2p_SRCS := $(ALL_RTL) $(CURDIR)/prim_fifo_sram_test.cc

1p_TOP  := prim_fifo_sram_1p_tb
1p_SRCS := $(ALL_RTL) $(CURDIR)/prim_fifo_sram_1p_tb.sv $(CURDIR)/prim_fifo_sram_1p_test.cc

LINT_SRCS := $(ALL_RTL)

include $(REPO_ROOT)/mk/verilator.mk
