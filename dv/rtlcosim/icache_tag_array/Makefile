REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := icache_tag_array

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
            $(PRIM_RF_2R1W_PREVIEW) \
            $(REPO_ROOT)/hw/ip/icache_tag_array/rtl/icache_tag_array.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_tag_array.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/gates

ORIG_RENAME := icache_tag_array

include $(REPO_ROOT)/mk/rtlcosim.mk
