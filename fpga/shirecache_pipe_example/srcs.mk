# Shared RTL source list for shirecache_pipe_example.
# Included by each head's Makefile.
# Head sets TECH before including this file.

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

RTL_DIR := $(REPO_ROOT)/hw/ip
SC_RTL  := $(RTL_DIR)/shirecache/rtl

PKGS := $(RTL_DIR)/dft/rtl/dft_pkg.sv \
        $(RTL_DIR)/ram_cfg/rtl/ram_cfg_pkg.sv \
        $(RTL_DIR)/etlink/rtl/etlink_pkg.sv \
        $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
        $(SC_RTL)/shirecache_pkg.sv

PRIM := $(PRIM_CLK_GATE) \
        $(PRIM_CLK_MUX) \
        $(PRIM_RAM_1P) \
        $(PRIM_RAM_2P) \
        $(PRIM_ECC_ENC) \
        $(PRIM_ECC_DEC) \
        $(PRIM_ARB_LRU) \
        $(PRIM_FIFO_REG) \
        $(PRIM_RST_SYNC)

SC_MODS := $(SC_RTL)/shirecache_pipe_lru_4w_calc.sv \
           $(SC_RTL)/shirecache_pipe_set_calc.sv \
           $(SC_RTL)/shirecache_pipe_l3_swizzle_get.sv \
           $(SC_RTL)/shirecache_pipe_l3_swizzle_put.sv \
           $(SC_RTL)/shirecache_pipe_tag_ram_wrap.sv \
           $(SC_RTL)/shirecache_pipe_tag_state_ram_wrap.sv \
           $(SC_RTL)/shirecache_pipe_data_ram_wrap.sv \
           $(SC_RTL)/shirecache_pipe_sub_bank_mem.sv \
           $(SC_RTL)/shirecache_pipe_sbe_insert.sv \
           $(SC_RTL)/shirecache_pipe_cbuf.sv \
           $(SC_RTL)/shirecache_pipe_atomic.sv \
           $(SC_RTL)/shirecache_pipe_rbuf.sv \
           $(SC_RTL)/shirecache_pipe_idx_cop_sm.sv \
           $(SC_RTL)/shirecache_pipe_sub_bank.sv \
           $(SC_RTL)/shirecache_pipe.sv

RTL_SRCS := $(PKGS) $(PRIM) $(SC_MODS) \
            $(CURDIR)/../rtl/shirecache_pipe_example.sv
