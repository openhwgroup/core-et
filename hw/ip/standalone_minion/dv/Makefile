REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKGS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
        $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
        $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
        $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
        $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
        $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
        $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
        $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv

FRONTEND_RTL := $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_rvc_expander.sv \
                $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_thread_buffer.sv \
                $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_thread_sched.sv \
                $(REPO_ROOT)/hw/ip/minion/frontend/rtl/intpipe_decode.sv \
                $(REPO_ROOT)/hw/ip/minion/frontend/rtl/vpu_decoder.sv \
                $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend.sv

INTPIPE_CSR_RTL := $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_fl_barrier.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_conv.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pmu_read_interface.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_replay.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_msgs.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file.sv

INTPIPE_RTL := $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_alu.sv \
               $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_imm.sv \
               $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_inst_bits_stage.sv \
               $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_rf.sv \
               $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_int_scoreboard.sv \
               $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_fp_scoreboard.sv \
               $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_mask_scoreboard.sv \
               $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_mul_div_top.sv \
               $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/debug_breakpoint.sv \
               $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_top.sv

DCACHE_RTL := $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_atomic_alu_unit.sv \
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

TXFMA_RTL := $(wildcard $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma*.sv)
VPU_RTL := $(wildcard $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_*_rom_case_*.sv) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/trans_top.sv \
           $(TXFMA_RTL) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_txfma_trans_top.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_rf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_bypass.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_top.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorb_rf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorc_rf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane_tima.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_sh_sw.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorfma.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorreduce.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorquant.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ml.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_trans.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_uinst_decoder.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensora_rf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensortmp_rf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_mask.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ctrl.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_top.sv

ICACHE_RTL := $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv \
              $(REPO_ROOT)/hw/ip/icache_tlb_array/rtl/icache_tlb_array.sv \
              $(REPO_ROOT)/hw/ip/icache_micro_tag_array/rtl/icache_micro_tag_array.sv \
              $(REPO_ROOT)/hw/ip/icache_micro_data_array/rtl/icache_micro_data_array.sv \
              $(REPO_ROOT)/hw/ip/icache_pma_unit/rtl/icache_pma_unit.sv \
              $(REPO_ROOT)/hw/ip/lru_sel/rtl/lru_sel.sv \
              $(REPO_ROOT)/hw/ip/icache_micro_cache/rtl/icache_micro_cache.sv \
              $(REPO_ROOT)/hw/ip/icache_tag_array/rtl/icache_tag_array.sv \
              $(REPO_ROOT)/hw/ip/icache_lru_array/rtl/icache_lru_array.sv \
              $(REPO_ROOT)/hw/ip/icache_data_array/rtl/icache_data_array.sv \
              $(REPO_ROOT)/hw/ip/icache_top/rtl/icache_top.sv \
              $(REPO_ROOT)/hw/ip/icache_data_ram_wrap/rtl/icache_data_ram_wrap.sv

NEIGH_RTL := $(REPO_ROOT)/hw/ip/neigh_miss_ff/rtl/neigh_miss_ff.sv \
             $(REPO_ROOT)/hw/ip/neigh_evict_ff/rtl/neigh_evict_ff_unit.sv \
             $(REPO_ROOT)/hw/ip/neigh_evict_ff/rtl/neigh_evict_ff.sv \
             $(REPO_ROOT)/hw/ip/neigh_fill_ff/rtl/neigh_fill_ff.sv \
             $(REPO_ROOT)/hw/ip/neigh_fill_fifo/rtl/neigh_fill_fifo.sv \
             $(REPO_ROOT)/hw/ip/pseudo_lru/rtl/pseudo_lru.sv \
             $(REPO_ROOT)/hw/ip/ptw_top/rtl/ptw_top.sv \
             $(REPO_ROOT)/hw/ip/neigh_shared_ptw/rtl/neigh_shared_ptw.sv

ROOT_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_debug_apb_slv.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/null_vpu.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/core_top.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_top.sv

WRAPPER_RTL := $(REPO_ROOT)/hw/ip/sp_apb_mux/rtl/sp_apb_mux.sv \
               $(REPO_ROOT)/hw/ip/esr_spio/rtl/esr_spio.sv \
               $(REPO_ROOT)/hw/ip/standalone_minion/rtl/standalone_minion.sv

PRIMS := $(PRIM_ARB_LRU) \
         $(PRIM_ECO_PORTS) \
         $(PRIM_RF_1R1W_DIFF_PREVIEW) \
         $(PRIM_RF_1R1W_PREVIEW) \
         $(PRIM_RF_1R1W_REG_PREVIEW) \
         $(PRIM_RF_1R1W_PAR_PREVIEW) \
         $(PRIM_RF_1R1W_DEC_PREVIEW) \
         $(PRIM_RF_SINGLE_1R1W_PAR_PREVIEW) \
         $(PRIM_RF_1R2W_PAR) \
         $(PRIM_RF_2R1W_PREVIEW) \
         $(PRIM_RF_3R2W_PREVIEW) \
         $(REPO_ROOT)/hw/ip/prim_queue_arb_lru/rtl/prim_queue_arb_lru.sv \
         $(PRIM_ARB_RR) \
         $(PRIM_PHASE_PAIR_LO_HI) \
         $(PRIM_PHASE_PAIR_HI_LO) \
         $(PRIM_CLK_GATE) \
         $(PRIM_CLK_GATE_N) \
         $(PRIM_CLK_MUX) \
         $(PRIM_CMP_32) \
         $(PRIM_CMP_42) \
         $(PRIM_ECC_ENC) \
         $(PRIM_ECC_DEC) \
         $(PRIM_WRITE_PREVIEW_EN) $(PRIM_WRITE_COMMIT_EN) $(PRIM_WRITE_COMMIT_RST_EN) \
         $(PRIM_MUL_DIV_SRCS) \
         $(PRIM_FIFO) \
         $(PRIM_FIFO_SYNC) \
         $(PRIM_RST_SYNC) \
         $(PRIM_RAM_1P) \
         $(PRIM_RAM_2P)

RTL_SRCS := $(PKGS) $(PRIMS) $(FRONTEND_RTL) $(INTPIPE_CSR_RTL) $(INTPIPE_RTL) \
            $(DCACHE_RTL) $(VPU_RTL) $(ICACHE_RTL) $(NEIGH_RTL) $(ROOT_RTL) $(WRAPPER_RTL)

TESTS := standalone_minion
standalone_minion_TOP := standalone_minion_tb
standalone_minion_SRCS := $(RTL_SRCS) $(CURDIR)/standalone_minion_tb.sv $(CURDIR)/standalone_minion_test.cc
standalone_minion_FLAGS := +incdir+$(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl -Wno-VARHIDDEN -Wno-UNOPTFLAT

LINT_SRCS := $(RTL_SRCS) $(CURDIR)/standalone_minion_tb.sv
LINT_FLAGS := --top-module standalone_minion_tb +incdir+$(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl -Wno-VARHIDDEN -Wno-UNOPTFLAT

include $(REPO_ROOT)/mk/verilator.mk
