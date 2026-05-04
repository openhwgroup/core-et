REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := icache_micro_data_array_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(PRIM_RF_1R1W_DEC_PREVIEW) \
            $(REPO_ROOT)/hw/ip/icache_micro_data_array/rtl/icache_micro_data_array.sv \
            $(CURDIR)/icache_micro_data_array_tb.sv
CC_SRCS := $(CURDIR)/icache_micro_data_array_test.cc

include $(REPO_ROOT)/mk/verilator.mk
