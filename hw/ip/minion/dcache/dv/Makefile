REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKGS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
        $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
        $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv

COMMON_RTL := $(PKGS)
PRIM_RTL :=
PRIM_RF_PREVIEW_RTL := $(PRIM_RF_1R1W_PREVIEW)
PRIM_RF_HELPER_RTL := $(PRIM_RF_1R1W_REG_PREVIEW) $(PRIM_RF_1R1W_PAR_PREVIEW) $(PRIM_RF_SINGLE_1R1W_PAR_PREVIEW)
PRIM_ARB_RTL := $(PRIM_ARB_LRU)
PRIM_FIFO_RTL := $(PRIM_FIFO_SYNC)
COMMON_RTL := $(PKGS)
RAM_PRIM_RTL := $(PRIM_RAM_2P)
TLB_RTL := $(PRIM_CLK_GATE) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv

TESTS := atomic_alu store_merge pma lru metadata buffer lram64 lram72 data_array tlb_array replay_queue miss_handler miss_handler_unit cache_op_unit cache_op_unit_l2 writeback scratchpad texsend reduce tensor_load top top_debug_apb_off

atomic_alu_TOP := minion_dcache_atomic_alu_unit_tb
atomic_alu_SRCS := $(COMMON_RTL) \
                   $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_atomic_alu_unit.sv \
                   $(CURDIR)/minion_dcache_atomic_alu_unit_tb.sv \
                   $(CURDIR)/minion_dcache_atomic_alu_unit_test.cc

store_merge_TOP := minion_dcache_store_merge_unit_tb
store_merge_SRCS := $(COMMON_RTL) \
                    $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_store_merge_unit.sv \
                    $(CURDIR)/minion_dcache_store_merge_unit_tb.sv \
                    $(CURDIR)/minion_dcache_store_merge_unit_test.cc

pma_TOP := minion_dcache_pma_unit_tb
pma_SRCS := $(COMMON_RTL) \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pma_unit.sv \
            $(CURDIR)/minion_dcache_pma_unit_tb.sv \
            $(CURDIR)/minion_dcache_pma_unit_test.cc

lru_TOP := minion_dcache_lru_array_tb
lru_SRCS := $(COMMON_RTL) \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_lru_array.sv \
            $(CURDIR)/minion_dcache_lru_array_tb.sv \
            $(CURDIR)/minion_dcache_lru_array_test.cc

metadata_TOP := minion_dcache_metadata_array_tb
metadata_SRCS := $(COMMON_RTL) \
                 $(PRIM_RTL) \
                 $(PRIM_RF_PREVIEW_RTL) \
                 $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_metadata_array.sv \
                 $(CURDIR)/minion_dcache_metadata_array_tb.sv \
                 $(CURDIR)/minion_dcache_metadata_array_test.cc

buffer_TOP := minion_dcache_buffer_array_tb
buffer_SRCS := $(COMMON_RTL) \
               $(PRIM_RTL) \
               $(PRIM_RF_PREVIEW_RTL) \
               $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_buffer_array.sv \
               $(CURDIR)/minion_dcache_buffer_array_tb.sv \
               $(CURDIR)/minion_dcache_buffer_array_test.cc

lram64_TOP := minion_dcache_128x64_1r1w_lram_tb
lram64_SRCS := $(COMMON_RTL) \
               $(PRIM_RF_HELPER_RTL) \
               $(RAM_PRIM_RTL) \
               $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_128x64_1r1w_lram.sv \
               $(CURDIR)/minion_dcache_128x64_1r1w_lram_tb.sv \
               $(CURDIR)/minion_dcache_128x64_1r1w_lram_test.cc

lram72_TOP := minion_dcache_128x72_1r1w_lram_tb
lram72_SRCS := $(COMMON_RTL) \
               $(PRIM_RF_HELPER_RTL) \
               $(RAM_PRIM_RTL) \
               $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_128x72_1r1w_lram.sv \
               $(CURDIR)/minion_dcache_128x72_1r1w_lram_tb.sv \
               $(CURDIR)/minion_dcache_128x72_1r1w_lram_test.cc

data_array_TOP := minion_dcache_data_array_tb
data_array_SRCS := $(COMMON_RTL) \
                   $(PRIM_RF_HELPER_RTL) \
                   $(RAM_PRIM_RTL) \
                   $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_128x64_1r1w_lram.sv \
                   $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_data_array.sv \
                   $(CURDIR)/minion_dcache_data_array_tb.sv \
                   $(CURDIR)/minion_dcache_data_array_test.cc

tlb_array_TOP := minion_dcache_tlb_array_tb
tlb_array_SRCS := $(COMMON_RTL) \
                  $(PRIM_RTL) \
                  $(TLB_RTL) \
                  $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_tlb_array.sv \
                  $(CURDIR)/minion_dcache_tlb_array_tb.sv \
                  $(CURDIR)/minion_dcache_tlb_array_test.cc

replay_queue_TOP := minion_dcache_replay_queue_tb
replay_queue_SRCS := $(COMMON_RTL) \
                     $(PRIM_RF_HELPER_RTL) \
                     $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_replay_queue.sv \
                     $(CURDIR)/minion_dcache_replay_queue_tb.sv \
                     $(CURDIR)/minion_dcache_replay_queue_test.cc

miss_handler_TOP := minion_dcache_miss_handler_tb
miss_handler_SRCS := $(COMMON_RTL) \
                     $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_miss_handler.sv \
                     $(CURDIR)/minion_dcache_miss_handler_tb.sv \
                     $(CURDIR)/minion_dcache_miss_handler_test.cc

miss_handler_unit_TOP := minion_dcache_miss_handler_unit_tb
miss_handler_unit_SRCS := $(COMMON_RTL) \
                          $(PRIM_ARB_RTL) \
                          $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_miss_handler.sv \
                          $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_miss_handler_unit.sv \
                          $(CURDIR)/minion_dcache_miss_handler_unit_tb.sv \
                          $(CURDIR)/minion_dcache_miss_handler_unit_test.cc

cache_op_unit_TOP := minion_dcache_cache_op_unit_tb
cache_op_unit_SRCS := $(COMMON_RTL) \
                      $(PRIM_CLK_GATE) \
                      $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_cache_op_unit.sv \
                      $(CURDIR)/minion_dcache_cache_op_unit_tb.sv \
                      $(CURDIR)/minion_dcache_cache_op_unit_test.cc

cache_op_unit_l2_TOP := minion_dcache_cache_op_unit_l2_tb
cache_op_unit_l2_SRCS := $(COMMON_RTL) \
                         $(PRIM_CLK_GATE) \
                         $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_cache_op_unit_l2.sv \
                         $(CURDIR)/minion_dcache_cache_op_unit_l2_tb.sv \
                         $(CURDIR)/minion_dcache_cache_op_unit_l2_test.cc

writeback_TOP := minion_dcache_writeback_unit_tb
writeback_SRCS := $(COMMON_RTL) \
                  $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_writeback_unit.sv \
                  $(CURDIR)/minion_dcache_writeback_unit_tb.sv \
                  $(CURDIR)/minion_dcache_writeback_unit_test.cc

scratchpad_TOP := minion_dcache_scratchpad_ctrl_tb
scratchpad_SRCS := $(COMMON_RTL) \
                   $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_scratchpad_ctrl.sv \
                   $(CURDIR)/minion_dcache_scratchpad_ctrl_tb.sv \
                   $(CURDIR)/minion_dcache_scratchpad_ctrl_test.cc

texsend_TOP := minion_dcache_texsend_tb
texsend_SRCS := $(COMMON_RTL) \
                $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_texsend.sv \
                $(CURDIR)/minion_dcache_texsend_tb.sv \
                $(CURDIR)/minion_dcache_texsend_test.cc

reduce_TOP := minion_dcache_reduce_tb
reduce_SRCS := $(COMMON_RTL) \
               $(PRIM_CLK_GATE) \
               $(PRIM_FIFO_RTL) \
               $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_reduce.sv \
               $(CURDIR)/minion_dcache_reduce_tb.sv \
               $(CURDIR)/minion_dcache_reduce_test.cc

tensor_load_TOP := minion_dcache_tensor_load_tb
tensor_load_SRCS := $(COMMON_RTL) \
                    $(PRIM_CLK_GATE) \
                    $(PRIM_RF_HELPER_RTL) \
                    $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_tensor_load.sv \
                    $(CURDIR)/minion_dcache_tensor_load_tb.sv \
                    $(CURDIR)/minion_dcache_tensor_load_test.cc

LINT_SRCS := $(COMMON_RTL) \
             $(PRIM_ARB_RTL) \
             $(PRIM_RTL) \
             $(PRIM_RF_HELPER_RTL) \
             $(PRIM_FIFO_RTL) \
             $(TLB_RTL) \
             $(RAM_PRIM_RTL) \
             $(CURDIR)/minion_dcache_lint_top.sv \
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

top_TOP := minion_dcache_top_tb
top_SRCS = $(filter-out $(CURDIR)/minion_dcache_lint_top.sv,$(LINT_SRCS)) \
           $(CURDIR)/minion_dcache_top_tb.sv \
           $(CURDIR)/minion_dcache_top_test.cc

top_debug_apb_off_TOP := minion_dcache_top_tb
top_debug_apb_off_SRCS = $(filter-out $(CURDIR)/minion_dcache_lint_top.sv,$(LINT_SRCS)) \
                         $(CURDIR)/minion_dcache_top_tb.sv \
                         $(CURDIR)/minion_dcache_top_debug_apb_off_test.cc
top_debug_apb_off_FLAGS := -GDebugApbEn=0 -GDebugMonEn=1

include $(REPO_ROOT)/mk/verilator.mk
