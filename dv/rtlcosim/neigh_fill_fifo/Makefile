REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := neigh_fill_fifo

NEW_RTL := \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
  $(PRIM_RF_1R2W_PAR) \
  $(REPO_ROOT)/hw/ip/prim_queue_arb_lru/rtl/prim_queue_arb_lru.sv \
  $(REPO_ROOT)/hw/ip/neigh_fill_fifo/rtl/neigh_fill_fifo.sv

ORIG_RTL := \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_fill_fifo.v \
  $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_2w_par.v \
  $(ORIG_ROOT)/rtl/libs/arbiters/queue_arb_lru.v

ORIG_LIBDIRS := \
  $(ORIG_ROOT)/rtl/libs/rf_latches \
  $(ORIG_ROOT)/rtl/libs/arbiters \
  $(ORIG_ROOT)/rtl/libs/registers

ORIG_RENAME := neigh_fill_fifo

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-DECLFILENAME -Wno-COMBDLY -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
