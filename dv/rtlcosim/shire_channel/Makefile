# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)
COSIM_NAME := shire_channel
TECH ?= generic
include $(REPO_ROOT)/mk/prim.mk

IP := $(REPO_ROOT)/hw/ip
SC := $(IP)/shirecache/rtl
RB := $(IP)/rbox/rtl
UC := $(IP)/shire_uncached/rtl
ESR := $(IP)/shire_esr/rtl
LEAVES := $(IP)/shire_channel_leaves/rtl

NEW_RTL := \
  $(IP)/dft/rtl/dft_pkg.sv \
  $(IP)/ram_cfg/rtl/ram_cfg_pkg.sv \
  $(IP)/etlink/rtl/etlink_pkg.sv \
  $(IP)/axi/rtl/axi_pkg.sv \
  $(IP)/apb/rtl/apb_pkg.sv \
  $(IP)/icache_pkg/rtl/icache_geom_pkg.sv \
  $(IP)/minion/rtl/minion_pkg.sv \
  $(IP)/minion/frontend/rtl/minion_frontend_pkg.sv \
  $(IP)/icache_pkg/rtl/icache_pkg.sv \
  $(SC)/shirecache_pkg.sv \
  $(IP)/esr/rtl/esr_pkg.sv \
  $(ESR)/shire_esr_pkg.sv \
  $(IP)/neigh_pkg/rtl/neigh_pkg.sv \
  $(IP)/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
  $(IP)/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
  $(RB)/rbox_pkg.sv \
  $(IP)/shire_sbm/rtl/shire_sbm_pkg.sv \
  $(UC)/shire_uncached_pkg.sv \
  $(LEAVES)/shire_channel_leaves_pkg.sv \
  $(PRIM_ECO_PORTS) \
  $(PRIM_CLK_GATE) \
  $(PRIM_CLK_MUX) \
  $(PRIM_WRITE_PREVIEW_EN) \
  $(PRIM_WRITE_COMMIT_RST_EN) \
  $(PRIM_RAM_1P) \
  $(PRIM_RAM_2P) \
  $(PRIM_RST_SYNC) \
  $(PRIM_FIFO_REG) \
  $(PRIM_FIFO_SRAM) \
  $(PRIM_FIFO_ASYNC_HIV) \
  $(PRIM_FIFO_ASYNC_LOV) \
  $(PRIM_ARB_LRU) \
  $(PRIM_ARB_LRU_2BID) \
  $(PRIM_ARB_RR) \
  $(PRIM_ARB_RR2) \
  $(PRIM_ARB_PRIO) \
  $(PRIM_RF_1R1W_PREVIEW) \
  $(PRIM_RF_3R2W) \
  $(PRIM_ECC_ENC) \
  $(PRIM_ECC_DEC) \
  $(PRIM_HOT2BIN) \
  $(SC)/shirecache_pipe_sbe_insert.sv \
  $(SC)/shirecache_pipe_atomic.sv \
  $(SC)/shirecache_pipe_l3_swizzle_get.sv \
  $(SC)/shirecache_pipe_l3_swizzle_put.sv \
  $(SC)/shirecache_pipe_lru_4w_calc.sv \
  $(SC)/shirecache_pipe_set_calc.sv \
  $(SC)/shirecache_pipe_tag_state_ram_wrap.sv \
  $(SC)/shirecache_pipe_tag_ram_wrap.sv \
  $(SC)/shirecache_pipe_data_ram_wrap.sv \
  $(SC)/shirecache_pipe_sub_bank_mem.sv \
  $(SC)/shirecache_pipe_sub_bank.sv \
  $(SC)/shirecache_pipe_cbuf.sv \
  $(SC)/shirecache_pipe_rbuf.sv \
  $(SC)/shirecache_pipe_idx_cop_sm.sv \
  $(SC)/shirecache_pipe.sv \
  $(SC)/shirecache_bank_dataq.sv \
  $(SC)/shirecache_bank_reqq_entry.sv \
  $(SC)/shirecache_bank_reqq_alloc.sv \
  $(SC)/shirecache_bank_reqq.sv \
  $(SC)/shirecache_bank_rspmux.sv \
  $(SC)/shirecache_bank_mesh.sv \
  $(SC)/shirecache_bank_l3_slave.sv \
  $(SC)/shirecache_bank_perfmon_qual.sv \
  $(SC)/shirecache_perfmon_ctl.sv \
  $(SC)/shirecache_bank_perfmon.sv \
  $(SC)/shirecache_bank_err_logger.sv \
  $(SC)/shirecache_bank_trace.sv \
  $(SC)/shirecache_bist_mbx.sv \
  $(SC)/shirecache_bist_wrapper.sv \
  $(SC)/shirecache_bank_l2hpf.sv \
  $(SC)/shirecache_bank.sv \
  $(SC)/shirecache_xbar.sv \
  $(SC)/shirecache_mesh_master_axi_port.sv \
  $(SC)/shirecache_mesh_master.sv \
  $(SC)/shirecache_mesh_slave_axi_port.sv \
  $(SC)/shirecache_mesh_slave.sv \
  $(SC)/shirecache_top.sv \
  $(RB)/rbox_step_mul.sv \
  $(RB)/rbox_depth_to_fp.sv \
  $(RB)/rbox_depth_to_un.sv \
  $(RB)/rbox_sample_to_fp32.sv \
  $(RB)/rbox_fp_shifter.sv \
  $(RB)/rbox_fp_fast_adder.sv \
  $(RB)/rbox_fp_adder_2stg.sv \
  $(RB)/rbox_sampler.sv \
  $(RB)/rbox_depth_offset.sv \
  $(RB)/rbox_input_fifo.sv \
  $(RB)/rbox_output_buffer.sv \
  $(RB)/rbox_depth_buffer.sv \
  $(RB)/rbox_test.sv \
  $(RB)/rbox_traverse.sv \
  $(RB)/rbox_ctrl.sv \
  $(RB)/rbox_esr.sv \
  $(RB)/rbox_dbg_ram.sv \
  $(RB)/rbox_top.sv \
  $(UC)/uncacheable_fcc.sv \
  $(UC)/uncacheable_flb.sv \
  $(UC)/noc_etlink_to_axi.sv \
  $(UC)/uncached_arb_lru_grant.sv \
  $(UC)/uncacheable_noc_slv_handler.sv \
  $(UC)/uncacheable_noc_tol3_handler.sv \
  $(UC)/uncacheable_noc_tosys_handler.sv \
  $(UC)/uncacheable_noc_rsp.sv \
  $(UC)/uncacheable_noc.sv \
  $(UC)/uncached_hi_voltage_cross_lv_logic.sv \
  $(UC)/uncached_hi_voltage_cross.sv \
  $(UC)/uncached_lo_voltage_cross_lv_logic.sv \
  $(UC)/uncached_lo_voltage_cross.sv \
  $(UC)/uncacheable.sv \
  $(ESR)/apb_esr_ff.sv \
  $(ESR)/esr_cache_bank.sv \
  $(ESR)/esr_shire_other.sv \
  $(LEAVES)/shire_bpam_run_control.sv \
  $(LEAVES)/shire_and_or_tree_daisychain.sv \
  $(LEAVES)/shire_coop_tload_bus.sv \
  $(LEAVES)/shire_ioshire_noc_ints.sv \
  $(LEAVES)/icache_mems.sv \
  $(IP)/icache_data_ram_wrap/rtl/icache_data_ram_wrap.sv \
  $(REPO_ROOT)/hw/ip/shire_channel/rtl/shire_channel.sv \
  $(REPO_ROOT)/hw/ip/shire_channel/dv/shire_channel_tb.sv \
  $(CURDIR)/icache_mems_orig_stub.sv

ORIG_RTL := \
  $(ORIG_ROOT)/rtl/shire/shire_channel/orig/shire_channel.v \
  $(ORIG_ROOT)/rtl/shire/shire_channel/apb_esr_ff.v \
  $(ORIG_ROOT)/rtl/shire/shire_channel/shire_bpam_run_control.v \
  $(ORIG_ROOT)/rtl/shire/shire_channel/shire_and_or_tree_daisychain.v \
  $(ORIG_ROOT)/rtl/shire/esr/esr_shire_other.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_fcc.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_flb.v \
  $(ORIG_ROOT)/rtl/shire/uncached/noc_etlink_to_axi.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc_slv_handler.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc_tol3_handler.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc_tosys_handler.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc_rsp.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable_noc.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_hi_voltage_cross_lv_logic.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_hi_voltage_cross.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_lo_voltage_cross_lv_logic.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncached_lo_voltage_cross.v \
  $(ORIG_ROOT)/rtl/shire/uncached/uncacheable.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/arbiters \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/shire/esr

ORIG_RENAME := uncacheable_noc_tol3_handler \
               uncacheable_noc_tosys_handler \
               uncacheable_noc_slv_handler \
               uncached_hi_voltage_cross_lv_logic \
               uncached_lo_voltage_cross_lv_logic \
               uncached_hi_voltage_cross \
               uncached_lo_voltage_cross \
               shire_and_or_tree_daisychain \
               shire_bpam_run_control \
               noc_etlink_to_axi \
               icache_mems \
               uncacheable_noc_rsp \
               uncacheable_noc \
               esr_shire_other \
               uncacheable_fcc \
               uncacheable_flb \
               apb_esr_ff \
               uncacheable \
               shire_channel

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-SYMRSVDWORD \
               +incdir+$(ORIG_ROOT)/rtl/shire/esr
SIM_ARGS := +verilator+noassert

include $(REPO_ROOT)/mk/rtlcosim.mk
