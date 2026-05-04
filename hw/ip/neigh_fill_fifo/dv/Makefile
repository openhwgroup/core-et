REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := neigh_fill_fifo_tb
RTL_SRCS := \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
  $(PRIM_RF_1R2W_PAR) \
  $(REPO_ROOT)/hw/ip/prim_queue_arb_lru/rtl/prim_queue_arb_lru.sv \
  $(REPO_ROOT)/hw/ip/neigh_fill_fifo/rtl/neigh_fill_fifo.sv \
  $(CURDIR)/neigh_fill_fifo_tb.sv
CC_SRCS := $(CURDIR)/neigh_fill_fifo_test.cc

include $(REPO_ROOT)/mk/verilator.mk
