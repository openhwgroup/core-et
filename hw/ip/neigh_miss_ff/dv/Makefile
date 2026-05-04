REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := neigh_miss_ff_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_miss_ff/rtl/neigh_miss_ff.sv \
            $(CURDIR)/neigh_miss_ff_tb.sv
CC_SRCS  := $(CURDIR)/neigh_miss_ff_test.cc

include $(REPO_ROOT)/mk/verilator.mk
