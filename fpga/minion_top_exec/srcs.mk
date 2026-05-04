# Shared RTL source list for minion_top_exec.
# Included by each head's Makefile.
# Head sets TECH before including this file.

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKGS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
        $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
        $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv

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

TLB_RTL := $(REPO_ROOT)/hw/ip/minion/tlb/rtl/minion_tlb.sv

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

ROOT_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_debug_apb_slv.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/null_vpu.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/core_top.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_top.sv

PRIMS := $(PRIM_ARB_LRU) \
         $(PRIM_ECO_PORTS) \
         $(PRIM_RF_1R1W_DIFF_PREVIEW) \
         $(PRIM_RF_1R1W_DIFF) \
         $(PRIM_RF_1R1W_PREVIEW) \
         $(PRIM_RF_1R1W_REG) \
         $(PRIM_RF_1R1W_REG_PREVIEW) \
         $(PRIM_RF_1R1W_PAR) \
         $(PRIM_RF_1R1W_PAR_PREVIEW) \
         $(PRIM_RF_SINGLE_1R1W_PAR) \
         $(PRIM_RF_SINGLE_1R1W_PAR_PREVIEW) \
	         $(PRIM_PHASE_PAIR_LO_HI) \
	         $(PRIM_PHASE_PAIR_HI_LO) \
	         $(PRIM_CLK_GATE) \
	         $(PRIM_CLK_GATE_N) \
	         $(PRIM_CMP_32) \
	         $(PRIM_CMP_42) \
	         $(PRIM_WRITE_PREVIEW_EN) $(PRIM_WRITE_COMMIT_EN) $(PRIM_WRITE_COMMIT_RST_EN) \
	         $(PRIM_MUL_DIV_SRCS) \
	         $(PRIM_RF_2R1W) \
         $(PRIM_RF_2R1W_PREVIEW) \
         $(PRIM_RF_1R1W) \
         $(PRIM_FIFO_SYNC) \
         $(PRIM_RST_SYNC)

UART_MODS := $(REPO_ROOT)/fpga/ulx3s_uart_bringup/rtl/ulx3s_uart_tx.sv

RTL_SRCS := $(PKGS) $(PRIMS) $(FRONTEND_RTL) $(INTPIPE_CSR_RTL) $(INTPIPE_RTL) \
            $(TLB_RTL) $(DCACHE_RTL) $(ROOT_RTL) $(UART_MODS) \
            $(CURDIR)/../rtl/minion_top_exec.sv
