REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := esr_neigh_tb

RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
            $(PRIM_RST_SYNC) \
            $(PRIM_WRITE_PREVIEW_EN) \
            $(PRIM_WRITE_COMMIT_RST_EN) \
            $(REPO_ROOT)/hw/ip/esr_neigh/rtl/esr_neigh.sv \
            $(CURDIR)/esr_neigh_tb.sv

CC_SRCS := $(CURDIR)/esr_neigh_test.cc

include $(REPO_ROOT)/mk/verilator.mk
