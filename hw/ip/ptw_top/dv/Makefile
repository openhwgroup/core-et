REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := ptw_top_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
            $(REPO_ROOT)/hw/ip/pseudo_lru/rtl/pseudo_lru.sv \
            $(REPO_ROOT)/hw/ip/ptw_top/rtl/ptw_top.sv \
            $(CURDIR)/ptw_top_tb.sv
CC_SRCS  := $(CURDIR)/ptw_top_test.cc

include $(REPO_ROOT)/mk/verilator.mk
