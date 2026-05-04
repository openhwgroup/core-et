REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := mesh_slave

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/axi/rtl/axi_pkg.sv \
            $(PRIM_FIFO_REG) \
            $(PRIM_FIFO_ASYNC_HIV) \
            $(PRIM_FIFO_ASYNC_LOV) \
            $(PRIM_ARB_RR) \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_l3_swizzle_get.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_mesh_slave_axi_port.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_mesh_slave.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_mesh_slave.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_mesh_slave_axi_port.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_l3_swizzle_get.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/arbiters

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
