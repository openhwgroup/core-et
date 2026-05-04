REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := icache_micro_cache

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
           $(PRIM_CLK_GATE) \
           $(PRIM_RF_1R1W_PAR_PREVIEW) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(PRIM_RF_1R1W_DIFF_PREVIEW) \
           $(PRIM_RF_1R1W_DEC_PREVIEW) \
           $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv \
           $(REPO_ROOT)/hw/ip/icache_tlb_array/rtl/icache_tlb_array.sv \
           $(REPO_ROOT)/hw/ip/icache_micro_tag_array/rtl/icache_micro_tag_array.sv \
           $(REPO_ROOT)/hw/ip/icache_micro_data_array/rtl/icache_micro_data_array.sv \
           $(REPO_ROOT)/hw/ip/lru_sel/rtl/lru_sel.sv \
           $(REPO_ROOT)/hw/ip/icache_pma_unit/rtl/icache_pma_unit.sv \
           $(REPO_ROOT)/hw/ip/icache_micro_cache/rtl/icache_micro_cache.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_cache.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_tlb_array.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_tag_array.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_data_array.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_pma_unit.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/lru_sel.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/shire/neigh/icache \
                $(ORIG_ROOT)/rtl/shire/minion \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/gates \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/arbiters

ORIG_RENAME := icache_micro_cache \
               icache_tlb_array \
               icache_micro_tag_array \
               icache_micro_data_array \
               icache_pma_unit \
               lru_sel

EXTRA_FLAGS := -Wno-WIDTH -Wno-SYMRSVDWORD -Wno-COMBDLY -Wno-UNUSEDSIGNAL -Wno-UNUSED -Wno-PINCONNECTEMPTY -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
