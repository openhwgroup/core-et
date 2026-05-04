REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := reqq_entry

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(PRIM_RST_SYNC) \
            $(PRIM_CLK_GATE) \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_l3_swizzle_get.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_bank_reqq_entry.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_bank_reqq_entry.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_l3_swizzle_get.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/rst_repeat.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
