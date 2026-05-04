REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := minion_dcache_miss_handler_unit

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(PRIM_ARB_LRU) \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_miss_handler.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_miss_handler_unit.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_miss_handler.v \
            $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_miss_handler_unit.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/arbiters

EXTRA_FLAGS := -Wno-WIDTH -Wno-SYMRSVDWORD -Wno-COMBDLY -Wno-UNUSED -Wno-UNOPTFLAT

include $(REPO_ROOT)/mk/rtlcosim.mk
