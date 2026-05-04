REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := icache_data_ram_wrap_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/prim_ecc/rtl/prim_ecc_enc.sv \
            $(REPO_ROOT)/hw/ip/tech_generic/prim_clk_mux/rtl/prim_clk_mux.sv \
            $(REPO_ROOT)/hw/ip/tech_generic/prim_ram_1p/rtl/prim_ram_1p.sv \
            $(REPO_ROOT)/hw/ip/icache_data_ram_wrap/rtl/icache_data_ram_wrap.sv \
            $(CURDIR)/icache_data_ram_wrap_tb.sv
CC_SRCS  := $(CURDIR)/icache_data_ram_wrap_test.cc

include $(REPO_ROOT)/mk/verilator.mk
