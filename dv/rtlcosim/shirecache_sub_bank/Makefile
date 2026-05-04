REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := sub_bank

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(PRIM_ECC_ENC) \
            $(PRIM_ECC_DEC) \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_sbe_insert.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_atomic.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_l3_swizzle_get.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_l3_swizzle_put.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_lru_4w_calc.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_sub_bank.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_sub_bank.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_sbe_insert.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_atomic.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_l3_swizzle_get.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_l3_swizzle_put.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_lru_4w_calc.v \
            $(ORIG_ROOT)/rtl/libs/ecc/ecc_gen.v \
            $(ORIG_ROOT)/rtl/libs/ecc/ecc_corr.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
