REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := shirecache_pipe_tag_ram_wrap

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(PRIM_CLK_MUX) \
            $(PRIM_RAM_1P) \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_tag_ram_wrap.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_mux2.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_mem1p.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_tag_ram_wrap.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
