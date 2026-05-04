REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := err_logger

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(PRIM_ARB_PRIO) \
            $(PRIM_HOT2BIN) \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_bank_err_logger.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_bank_err_logger.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_prio_data.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_prio.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/hot2bin.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/onehot_mux.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
