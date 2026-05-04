REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := ptw_top

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
            $(REPO_ROOT)/hw/ip/pseudo_lru/rtl/pseudo_lru.sv \
            $(REPO_ROOT)/hw/ip/ptw_top/rtl/ptw_top.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/pseudo_lru.v \
            $(ORIG_ROOT)/rtl/shire/neigh/ptw_top.v

ORIG_RENAME := ptw_top pseudo_lru

include $(REPO_ROOT)/mk/rtlcosim.mk
