REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := minion_dcache_top

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(PRIM_ARB_LRU) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(PRIM_RF_1R1W_REG_PREVIEW) \
           $(PRIM_RF_1R1W_PAR_PREVIEW) \
           $(PRIM_RF_SINGLE_1R1W_PAR_PREVIEW) \
           $(PRIM_FIFO_SYNC) \
           $(PRIM_CLK_GATE) \
           $(PRIM_CLK_MUX) \
           $(PRIM_RAM_2P) \
           $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_atomic_alu_unit.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_store_merge_unit.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pma_unit.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_lru_array.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_metadata_array.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_buffer_array.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_128x64_1r1w_lram.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_128x72_1r1w_lram.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_data_array.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_tlb_array.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_replay_queue.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_miss_handler.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_miss_handler_unit.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_cache_op_unit.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_cache_op_unit_l2.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_writeback_unit.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_scratchpad_ctrl.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_texsend.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_reduce.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_tensor_load.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_top.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/tlb_top.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/shire/minion/dcache \
                $(ORIG_ROOT)/rtl/shire/minion \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/arbiters \
                $(ORIG_ROOT)/rtl/libs/registers

EXTRA_FLAGS := -Wno-WIDTH -Wno-SYMRSVDWORD -Wno-COMBDLY -Wno-UNUSEDSIGNAL -Wno-UNUSED -Wno-PINCONNECTEMPTY -Wno-TIMESCALEMOD

include $(REPO_ROOT)/mk/rtlcosim.mk
