REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := neigh_shared_ptw_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
            $(REPO_ROOT)/hw/ip/prim_arb_lru/rtl/prim_arb_lru.sv \
            $(REPO_ROOT)/hw/ip/pseudo_lru/rtl/pseudo_lru.sv \
            $(REPO_ROOT)/hw/ip/ptw_top/rtl/ptw_top.sv \
            $(REPO_ROOT)/hw/ip/neigh_shared_ptw/rtl/neigh_shared_ptw.sv \
            $(CURDIR)/neigh_shared_ptw_tb.sv
CC_SRCS  := $(CURDIR)/neigh_shared_ptw_test.cc

include $(REPO_ROOT)/mk/verilator.mk
