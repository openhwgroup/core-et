REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := icache_pma_unit

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pma_unit/rtl/icache_pma_unit.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_pma_unit.v
ORIG_RENAME := icache_pma_unit

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-DECLFILENAME -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
