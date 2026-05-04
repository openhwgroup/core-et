REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := pipe

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(PRIM_CLK_MUX) \
            $(PRIM_RAM_1P) \
            $(PRIM_RAM_2P) \
            $(PRIM_RST_SYNC) \
            $(PRIM_FIFO_REG) \
            $(PRIM_ARB_LRU) \
            $(PRIM_ECC_ENC) \
            $(PRIM_ECC_DEC) \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_sbe_insert.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_atomic.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_l3_swizzle_get.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_l3_swizzle_put.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_lru_4w_calc.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_set_calc.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_tag_state_ram_wrap.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_tag_ram_wrap.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_data_ram_wrap.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_sub_bank_mem.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_sub_bank.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_cbuf.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_rbuf.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe_idx_cop_sm.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pipe.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_sbe_insert.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_atomic.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_l3_swizzle_get.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_l3_swizzle_put.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_lru_4w_calc.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_set_calc.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_tag_state_ram_wrap.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_tag_ram_wrap.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_data_ram_wrap.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_sub_bank_mem.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_sub_bank.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_cbuf.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_rbuf.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_idx_cop_sm.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe_mem.v \
            $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_pipe.v \
            $(ORIG_ROOT)/rtl/libs/ecc/ecc_gen.v \
            $(ORIG_ROOT)/rtl/libs/ecc/ecc_corr.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/rst_repeat.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_fifo_reg.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_mem1p.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_mem2p.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_lru.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_mux2.v \
            $(ORIG_ROOT)/rtl/libs/registers/en_ff_gated_struct.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
