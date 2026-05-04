REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := neigh_fill_ff_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(PRIM_FIFO) \
            $(REPO_ROOT)/hw/ip/neigh_fill_ff/rtl/neigh_fill_ff.sv \
            $(CURDIR)/neigh_fill_ff_tb.sv
CC_SRCS  := $(CURDIR)/neigh_fill_ff_test.cc

include $(REPO_ROOT)/mk/verilator.mk
