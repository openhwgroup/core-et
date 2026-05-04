# Shared RTL source list for test_intpipe.
# Included by each head's Makefile.
# Head sets TECH before including this file.

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

PKGS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
        $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
        $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pkg.sv \
        $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv

PRIMS := $(PRIM_CLK_GATE) \
         $(PRIM_CLK_GATE_N) \
         $(PRIM_WRITE_PREVIEW_EN) \
         $(PRIM_WRITE_COMMIT_EN) \
         $(PRIM_WRITE_COMMIT_RST_EN) \
         $(PRIM_PHASE_PAIR_LO_HI) \
         $(PRIM_PHASE_PAIR_HI_LO) \
         $(PRIM_MUL_DIV_SRCS) \
         $(PRIM_RF_2R1W) \
         $(PRIM_RF_2R1W_PREVIEW) \
         $(PRIM_CMP_32) \
         $(PRIM_CMP_42) \
         $(REPO_ROOT)/hw/ip/prim_fifo_sync/rtl/prim_fifo_sync.sv

DECODE_RTL := $(REPO_ROOT)/hw/ip/minion/frontend/rtl/intpipe_decode.sv

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

UART_MODS := $(REPO_ROOT)/fpga/ulx3s_uart_bringup/rtl/ulx3s_uart_tx.sv

RTL_SRCS := $(PKGS) $(PRIMS) $(DECODE_RTL) $(INTPIPE_CSR_RTL) $(INTPIPE_RTL) \
            $(UART_MODS) $(CURDIR)/../rtl/test_intpipe.sv
