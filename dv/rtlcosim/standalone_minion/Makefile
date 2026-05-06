REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := standalone_minion

include $(REPO_ROOT)/mk/prim.mk
include $(REPO_ROOT)/mk/minion_vpu_sources.mk

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

TXFMA_RTL := $(MINION_VPU_TXFMA_REAL_RTL)
VPU_RTL := $(MINION_VPU_TRANS_ROM_RTL) \
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

NEW_RTL := $(PKGS) $(PRIMS) $(FRONTEND_RTL) $(INTPIPE_CSR_RTL) $(INTPIPE_RTL) \
           $(DCACHE_RTL) $(VPU_RTL) $(ICACHE_RTL) $(NEIGH_RTL) $(ROOT_RTL) $(WRAPPER_RTL)


ORIG_RTL := $(ORIG_ROOT)/rtl/shire/standalone_minion/standalone_minion.v \
            $(ORIG_ROOT)/rtl/shire/standalone_minion/sp_apb_mux.v \
            $(ORIG_ROOT)/rtl/shire/esr/esr_spio.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_miss_ff.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_evict_ff_unit.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_evict_ff.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_fill_ff.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_fill_fifo.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_shared_ptw.v \
            $(ORIG_ROOT)/rtl/shire/neigh/ptw_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/pseudo_lru.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_top.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_cache.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_tlb_array.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_tag_array.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_data_array.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_pma_unit.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_tag_array.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_lru_array.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_data_array.v \
            $(ORIG_ROOT)/rtl/shire/shire_channel/icache/icache_data_ram_wrap.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/lru_sel.v \
            $(ORIG_ROOT)/rtl/shire/minion/minion_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/core_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/minion_debug_apb_slv.v \
            $(ORIG_ROOT)/rtl/shire/minion/frontend/frontend_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_alu.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_imm.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_inst_bits_stage.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_int_scoreboard.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_fp_scoreboard.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_mask_scoreboard.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_mul_div_dp.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_mul_div_ctl.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_mul_div_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/debug_breakpoint.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_file.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_file_conv.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_file_fl_barrier.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_replay.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_msgs.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_pmu_read_interface.v \
            $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_reduce.v \
            $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_replay_queue.v \
            $(ORIG_ROOT)/rtl/shire/minion/tlb_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_decoder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_ctrl.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_ml.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorfma.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorreduce.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorquant.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_trans.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_uinst_decoder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensora_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensortmp_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_mask.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_lane.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_bypass.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_sh_sw.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_lane_tima.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorb_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorc_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/tima/tima_adder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/tima/tima_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_txfma_trans_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit/trans_top.v \
            $(ORIG_MINION_VPU_TRANS_ROM_RTL) \
            $(ORIG_MINION_VPU_TXFMA_REAL_RTL)

# Keep the original VPU root out of -y search: all needed root VPU
# modules are listed in ORIG_RTL, and the root also contains txfma_top_fake.v
# with the original fake-only port shape that must not satisfy translated
# parameter-elaboration branches in real-TXFMA cosims.
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/shire/standalone_minion \
                $(ORIG_ROOT)/rtl/shire/esr \
                $(ORIG_ROOT)/rtl/shire/neigh \
                $(ORIG_ROOT)/rtl/shire/neigh/icache \
                $(ORIG_ROOT)/rtl/shire/shire_channel/icache \
                $(ORIG_ROOT)/rtl/shire/minion \
                $(ORIG_ROOT)/rtl/shire/minion/frontend \
                $(ORIG_ROOT)/rtl/shire/minion/intpipe \
                $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr \
                $(ORIG_ROOT)/rtl/shire/minion/dcache \
                $(ORIG_ROOT)/rtl/shire/minion/vpu/tima \
                $(ORIG_ROOT)/rtl/shire/minion/vpu/trans_unit \
                $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/counters \
                $(ORIG_ROOT)/rtl/libs/compressors \
                $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/arbiters \
                $(ORIG_ROOT)/rtl/libs/gates \
                $(ORIG_ROOT)/rtl/libs/ecc \
                $(ORIG_ROOT)/rtl/libs

ORIG_RENAME := standalone_minion \
               icache_data_ram_wrap \
               icache_micro_data_array \
               icache_micro_tag_array \
               icache_micro_cache \
               icache_tlb_array \
               icache_pma_unit \
               icache_data_array \
               icache_lru_array \
               icache_tag_array \
               neigh_evict_ff_unit \
               neigh_shared_ptw \
               neigh_fill_fifo \
               neigh_miss_ff \
               neigh_evict_ff \
               neigh_fill_ff \
               sp_apb_mux \
               esr_spio \
               ptw_top \
               pseudo_lru \
               icache_top \
               lru_sel \
               minion_top \
               core_top \
               minion_debug_apb_slv \
               vpu_decoder \
               intpipe_top \
               intpipe_alu \
               intpipe_imm \
               intpipe_inst_bits_stage \
               intpipe_rf \
               intpipe_int_scoreboard \
               intpipe_fp_scoreboard \
               intpipe_mask_scoreboard \
               intpipe_mul_div_dp \
               intpipe_mul_div_ctl \
               intpipe_mul_div_top \
               debug_breakpoint \
               intpipe_csr_file_fl_barrier \
               intpipe_csr_pmu_read_interface \
               intpipe_csr_file_conv \
               intpipe_csr_replay \
               intpipe_csr_msgs \
               intpipe_csr_file \
               txfma_4_2_compressor_array \
               txfma_booth_ppg_32r4_norm \
               txfma_ediff_opdorder_logic \
               txfma_exp_special_detect \
               txfma_frac_zero_detect \
               trans_rsqrt_rom_case_c1c2 \
               trans_rcp_rom_case_c1c2 \
               trans_log_rom_case_c1c2 \
               trans_exp_rom_case_c1c2 \
               trans_sin_rom_case_c1c2 \
               vpu_txfma_trans_top \
               txfma_4_2_compressor \
               txfma_booth_ppg_32r4_msb \
               trans_rsqrt_rom_case_c0 \
               trans_rcp_rom_case_c0 \
               trans_log_rom_case_c0 \
               trans_exp_rom_case_c0 \
               trans_sin_rom_case_c0 \
               txfma_booth_ppg_32r4 \
               txfma_align_shf \
               txfma_norm_shf \
               txfmactl_top \
               txfmaexp_top \
               txfmafrac_top \
               vpu_uinst_decoder \
               vpu_tensorreduce \
               vpu_tensorquant \
               vpu_tensorfma \
               vpu_tensortmp_rf \
               vpu_tensora_rf \
               vpu_lane_tima \
               vpu_tensorc_rf \
               vpu_tensorb_rf \
               txfma_wallace1 \
               txfma_wallace2 \
               txfma_rnd_adder \
               txfma_adder \
               vpu_bypass \
               vpu_sh_sw \
               vpu_trans \
               vpu_mask \
               trans_top \
               txfma_top \
               vpu_ctrl \
               vpu_lane \
               vpu_top \
               vpu_rf \
               vpu_ml \
               tima_top \
               txfma_trz2 \
               tima_adder \
               txfma_lxd \
               txfma_trz \
               txfma_csa \
               txfma_c0 \
               txfma_c1 \
               txfma_c2 \
               txfma_c3 \
               txfma_c4 \
               txfma_c5 \
               txfma_c6 \
               txfma_e1 \
               txfma_e2 \
               txfma_e4 \
               txfma_e5 \
               txfma_e6 \
               txfma_f0 \
               txfma_f1 \
               txfma_f2 \
               txfma_f3 \
               txfma_f4 \
               txfma_f5 \
               txfma_f6

EXTRA_FLAGS := --converge-limit 10000 --x-initial-edge -DET_ASSERT_OFF -DVPU_EN_RCG_2 \
               -Wno-TIMESCALEMOD -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC \
               -Wno-VARHIDDEN -Wno-UNOPTFLAT -Wno-PINCONNECTEMPTY \
               +incdir+$(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl \
               +incdir+$(ORIG_ROOT)/rtl/shire/minion/intpipe/csr \
               +incdir+$(ORIG_ROOT)/rtl/shire/esr

include $(REPO_ROOT)/mk/rtlcosim.mk
