REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_miss_ff

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_miss_ff/rtl/neigh_miss_ff.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_miss_ff.v

ORIG_RENAME := neigh_miss_ff

include $(REPO_ROOT)/mk/rtlcosim.mk
