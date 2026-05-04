REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP := icache_micro_cache_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(PRIM_RF_1R1W_PAR_PREVIEW) \
            $(PRIM_RF_1R1W_DEC_PREVIEW) \
            $(PRIM_RF_1R1W_PREVIEW) \
            $(PRIM_RF_1R1W_DIFF_PREVIEW) \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv \
            $(REPO_ROOT)/hw/ip/icache_tlb_array/rtl/icache_tlb_array.sv \
            $(REPO_ROOT)/hw/ip/icache_micro_tag_array/rtl/icache_micro_tag_array.sv \
            $(REPO_ROOT)/hw/ip/icache_micro_data_array/rtl/icache_micro_data_array.sv \
            $(REPO_ROOT)/hw/ip/lru_sel/rtl/lru_sel.sv \
            $(REPO_ROOT)/hw/ip/icache_pma_unit/rtl/icache_pma_unit.sv \
            $(REPO_ROOT)/hw/ip/icache_micro_cache/rtl/icache_micro_cache.sv \
            $(CURDIR)/icache_micro_cache_tb.sv
CC_SRCS := $(CURDIR)/icache_micro_cache_test.cc

include $(REPO_ROOT)/mk/verilator.mk
