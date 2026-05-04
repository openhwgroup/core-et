REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := perfmon

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_perfmon_ctl.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_bank_perfmon_qual.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_bank_perfmon.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_bank_perfmon.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_perfmon_ctl.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_bank_perfmon_qual.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
