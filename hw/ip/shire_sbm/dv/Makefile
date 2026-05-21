REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COMMON_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
              $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
              $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
              $(REPO_ROOT)/hw/ip/apb/rtl/apb_pkg.sv \
              $(REPO_ROOT)/hw/ip/axi/rtl/axi_pkg.sv \
              $(REPO_ROOT)/hw/ip/shire_sbm/rtl/shire_sbm_pkg.sv

RTL_SRCS := $(COMMON_RTL) \
            $(REPO_ROOT)/hw/ip/prim_fifo_reg/rtl/prim_fifo_reg.sv \
            $(REPO_ROOT)/hw/ip/prim_arb_rr/rtl/prim_arb_rr.sv \
            $(REPO_ROOT)/hw/ip/shire_sbm/rtl/mshire_axi_to_apb.sv \
            $(REPO_ROOT)/hw/ip/shire_sbm/rtl/shire_bus_master.sv \
            $(REPO_ROOT)/hw/ip/shire_sbm/rtl/sbm_top.sv \
            $(CURDIR)/shire_sbm_tb.sv

TB_TOP  := shire_sbm_tb
CC_SRCS := $(CURDIR)/shire_sbm_test.cc

LINT_SRCS := $(RTL_SRCS)

include $(REPO_ROOT)/mk/verilator.mk
