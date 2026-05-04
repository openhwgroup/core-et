REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := core_top

include $(REPO_ROOT)/mk/prim.mk

NEW_PKGS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv

NEW_FRONTEND_RTL := $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_rvc_expander.sv \
                    $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_thread_buffer.sv \
                    $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_thread_sched.sv \
                    $(REPO_ROOT)/hw/ip/minion/frontend/rtl/intpipe_decode.sv \
                    $(REPO_ROOT)/hw/ip/minion/frontend/rtl/vpu_decoder.sv \
                    $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend.sv

NEW_INTPIPE_CSR_RTL := $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_fl_barrier.sv \
                       $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_conv.sv \
                       $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pmu_read_interface.sv \
                       $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_replay.sv \
                       $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_msgs.sv \
                       $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file.sv

NEW_INTPIPE_RTL := $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_alu.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_imm.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_inst_bits_stage.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_rf.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_int_scoreboard.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_fp_scoreboard.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_mask_scoreboard.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_mul_div_top.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/debug_breakpoint.sv \
                   $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_top.sv

NEW_TLB_RTL := $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv

NEW_DCACHE_RTL := $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_atomic_alu_unit.sv \
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

NEW_ROOT_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_debug_apb_slv.sv \
                $(REPO_ROOT)/hw/ip/minion/rtl/core_top.sv

NEW_PRIMS := $(PRIM_ARB_LRU) \
             $(PRIM_RF_1R1W_DIFF_PREVIEW) \
             $(PRIM_RF_1R1W_PREVIEW) \
             $(PRIM_RF_1R1W_REG_PREVIEW) \
             $(PRIM_RF_1R1W_PAR_PREVIEW) \
             $(PRIM_RF_SINGLE_1R1W_PAR_PREVIEW) \
             $(PRIM_RF_2R1W_PREVIEW) \
	             $(PRIM_PHASE_PAIR_LO_HI) \
	             $(PRIM_PHASE_PAIR_HI_LO) \
	             $(PRIM_CLK_GATE) \
	             $(PRIM_CLK_GATE_N) \
	             $(PRIM_CMP_32) \
	             $(PRIM_CMP_42) \
	             $(PRIM_WRITE_PREVIEW_EN) $(PRIM_WRITE_COMMIT_EN) $(PRIM_WRITE_COMMIT_RST_EN) \
	             $(PRIM_MUL_DIV_SRCS) \
             $(PRIM_FIFO_SYNC) \
             $(PRIM_CLK_MUX) \
             $(PRIM_RAM_2P)

NEW_RTL := $(NEW_PKGS) \
           $(NEW_PRIMS) \
           $(NEW_FRONTEND_RTL) \
           $(NEW_INTPIPE_CSR_RTL) \
           $(NEW_INTPIPE_RTL) \
           $(NEW_TLB_RTL) \
           $(NEW_DCACHE_RTL) \
           $(NEW_ROOT_RTL)

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/core_top.v \
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
            $(ORIG_ROOT)/rtl/shire/minion/tlb_top.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/shire/minion \
                $(ORIG_ROOT)/rtl/shire/minion/frontend \
                $(ORIG_ROOT)/rtl/shire/minion/intpipe \
                $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr \
                $(ORIG_ROOT)/rtl/shire/minion/dcache \
                $(ORIG_ROOT)/rtl/shire/minion/vpu \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/counters \
                $(ORIG_ROOT)/rtl/libs/compressors \
                $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/arbiters \
                $(ORIG_ROOT)/rtl/libs

ORIG_RENAME := core_top \
               minion_debug_apb_slv \
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
               intpipe_csr_file \
               intpipe_csr_file_conv \
               intpipe_csr_file_fl_barrier \
               intpipe_csr_replay \
               intpipe_csr_msgs \
               intpipe_csr_pmu_read_interface

EXTRA_FLAGS := --converge-limit 10000 --x-initial-edge -DET_ASSERT_OFF \
               +incdir+$(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl \
               +incdir+$(ORIG_ROOT)/rtl/shire/minion/intpipe/csr

include $(REPO_ROOT)/mk/rtlcosim.mk
