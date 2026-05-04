REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_shared_ptw

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
            $(REPO_ROOT)/hw/ip/prim_arb_lru/rtl/prim_arb_lru.sv \
            $(REPO_ROOT)/hw/ip/pseudo_lru/rtl/pseudo_lru.sv \
            $(REPO_ROOT)/hw/ip/ptw_top/rtl/ptw_top.sv \
            $(REPO_ROOT)/hw/ip/neigh_shared_ptw/rtl/neigh_shared_ptw.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_shared_ptw.v \
            $(ORIG_ROOT)/rtl/shire/neigh/ptw_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/pseudo_lru.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_lru_data.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_lru.v

ORIG_RENAME := neigh_shared_ptw ptw_top pseudo_lru

include $(REPO_ROOT)/mk/rtlcosim.mk
