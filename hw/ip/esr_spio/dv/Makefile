REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := esr_spio_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
            $(REPO_ROOT)/hw/ip/esr_spio/rtl/esr_spio.sv \
            $(CURDIR)/esr_spio_tb.sv
CC_SRCS  := $(CURDIR)/esr_spio_test.cc

include $(REPO_ROOT)/mk/verilator.mk
