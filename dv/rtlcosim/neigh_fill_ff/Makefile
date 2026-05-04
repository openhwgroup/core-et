REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := neigh_fill_ff

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(PRIM_FIFO) \
            $(PRIM_RF_1R1W_PREVIEW) \
            $(REPO_ROOT)/hw/ip/neigh_fill_ff/rtl/neigh_fill_ff.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_fifo.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_fill_ff.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers

ORIG_RENAME := neigh_fill_ff

include $(REPO_ROOT)/mk/rtlcosim.mk
