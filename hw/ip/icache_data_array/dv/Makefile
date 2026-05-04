REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := icache_data_array_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
            $(REPO_ROOT)/hw/ip/prim_ecc/rtl/prim_ecc_enc.sv \
            $(REPO_ROOT)/hw/ip/prim_ecc/rtl/prim_ecc_dec.sv \
            $(REPO_ROOT)/hw/ip/icache_data_array/rtl/icache_data_array.sv \
            $(CURDIR)/icache_data_array_tb.sv
CC_SRCS  := $(CURDIR)/icache_data_array_test.cc

include $(REPO_ROOT)/mk/verilator.mk
