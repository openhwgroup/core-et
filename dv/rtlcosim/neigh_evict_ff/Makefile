REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_evict_ff

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_evict_ff/rtl/neigh_evict_ff_unit.sv \
            $(REPO_ROOT)/hw/ip/neigh_evict_ff/rtl/neigh_evict_ff.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_evict_ff_unit.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_evict_ff.v

ORIG_RENAME := neigh_evict_ff neigh_evict_ff_unit

include $(REPO_ROOT)/mk/rtlcosim.mk
