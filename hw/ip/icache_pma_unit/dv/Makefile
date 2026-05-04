REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := icache_pma_unit_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pma_unit/rtl/icache_pma_unit.sv \
            $(CURDIR)/icache_pma_unit_tb.sv
CC_SRCS := $(CURDIR)/icache_pma_unit_test.cc

include $(REPO_ROOT)/mk/verilator.mk
