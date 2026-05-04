REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := swizzle

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_l3_swizzle_get.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_l3_swizzle_put.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_l3_swizzle_get.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_l3_swizzle_put.v

include $(REPO_ROOT)/mk/rtlcosim.mk
