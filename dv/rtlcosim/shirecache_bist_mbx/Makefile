REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := bist_mbx

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_bist_mbx.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_bist_mbx.v

EXTRA_FLAGS := -DET_BIST_INSERTION=1 -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
