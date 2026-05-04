REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := reqq_alloc

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(PRIM_ARB_RR) \
            $(PRIM_ARB_RR2) \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_bank_reqq_alloc.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_bank_reqq_alloc.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr2.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr_base.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_grants2.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/hot2bin.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/onehot_mux.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
