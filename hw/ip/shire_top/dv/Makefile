# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT := $(shell git rev-parse --show-toplevel)
TECH ?= generic
include $(REPO_ROOT)/mk/prim.mk

IP := $(REPO_ROOT)/hw/ip
SC := $(IP)/shirecache/rtl
RB := $(IP)/rbox/rtl
UC := $(IP)/shire_uncached/rtl
ESR := $(IP)/shire_esr/rtl
LEAVES := $(IP)/shire_channel_leaves/rtl
SBM := $(IP)/shire_sbm/rtl
WRAP := $(IP)/shire_channel_wrap/rtl
TOP := $(IP)/shire_top/rtl

PKG_SRCS := \
  $(IP)/dft/rtl/dft_pkg.sv \
  $(IP)/ram_cfg/rtl/ram_cfg_pkg.sv \
  $(IP)/etlink/rtl/etlink_pkg.sv \
  $(IP)/axi/rtl/axi_pkg.sv \
  $(IP)/apb/rtl/apb_pkg.sv \
  $(IP)/icache_pkg/rtl/icache_geom_pkg.sv \
  $(IP)/minion/rtl/minion_pkg.sv \
  $(IP)/minion/dcache/rtl/minion_dcache_pkg.sv \
  $(IP)/minion/frontend/rtl/minion_frontend_pkg.sv \
  $(IP)/minion/tlb/rtl/tlb_pkg.sv \
  $(IP)/icache_pkg/rtl/icache_pkg.sv \
  $(SC)/shirecache_pkg.sv \
  $(IP)/esr/rtl/esr_pkg.sv \
  $(ESR)/shire_esr_pkg.sv \
  $(IP)/neigh_pkg/rtl/neigh_pkg.sv \
  $(IP)/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
  $(IP)/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
  $(IP)/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux_pkg.sv \
  $(IP)/neigh_ch_dbg/rtl/neigh_ch_dbg_pkg.sv \
  $(RB)/rbox_pkg.sv \
  $(SBM)/shire_sbm_pkg.sv \
  $(UC)/shire_uncached_pkg.sv \
  $(LEAVES)/shire_channel_leaves_pkg.sv

PRIM_SRCS := $(shell find $(IP)/prim_* -path '*/rtl/*.sv' | sort) \
             $(shell find $(IP)/tech_generic -path '*/rtl/*.sv' | sort)

SC_SRCS := \
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
  $(SC)/shirecache_top.sv

RB_SRCS := \
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
  $(RB)/rbox_top.sv

UC_SRCS := \
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
  $(UC)/uncacheable.sv

ESR_SRCS := \
  $(ESR)/apb_esr_ff.sv \
  $(ESR)/esr_cache_bank.sv \
  $(ESR)/esr_shire_other.sv

LEAF_SRCS := \
  $(LEAVES)/shire_dmctrl.sv \
  $(LEAVES)/shire_xll_control.sv \
  $(LEAVES)/shire_pll_wrapper.sv \
  $(LEAVES)/shire_bpam_run_control.sv \
  $(LEAVES)/shire_and_or_tree_daisychain.sv \
  $(LEAVES)/shire_coop_tload_bus.sv \
  $(LEAVES)/shire_ioshire_noc_ints.sv \
  $(LEAVES)/icache_mems.sv \
  $(IP)/icache_data_ram_wrap/rtl/icache_data_ram_wrap.sv

SBM_SRCS := \
  $(SBM)/mshire_axi_to_apb.sv \
  $(SBM)/shire_bus_master.sv \
  $(SBM)/sbm_top.sv

NEIGH_SRCS := $(shell find \
  $(IP)/neigh_hv_logic/rtl \
  $(IP)/neigh_ch_apb_mux/rtl \
  $(IP)/neigh_fill_fifo/rtl \
  $(IP)/neigh_shared_icache/rtl \
  $(IP)/neigh_shared_ptw/rtl \
  $(IP)/neigh_miss_ff/rtl \
  $(IP)/neigh_evict_ff/rtl \
  $(IP)/neigh_fill_ff/rtl \
  $(IP)/neigh_channel_routing/rtl \
  $(IP)/neigh_coop_tload/rtl \
  $(IP)/neigh_tensor_store_buffer/rtl \
  $(IP)/neigh_ch_dbg/rtl \
  $(IP)/dll_dly_est/rtl \
  $(IP)/neigh_voltage_cross/rtl \
  $(IP)/esr_neigh/rtl \
  $(IP)/icache_top/rtl \
  $(IP)/icache_data_array/rtl \
  $(IP)/icache_lru_array/rtl \
  $(IP)/icache_micro_cache/rtl \
  $(IP)/icache_micro_data_array/rtl \
  $(IP)/icache_micro_tag_array/rtl \
  $(IP)/icache_pma_unit/rtl \
  $(IP)/icache_tag_array/rtl \
  $(IP)/icache_tlb_array/rtl \
  $(IP)/lru_sel/rtl \
  $(IP)/minion/tlb/rtl \
  $(IP)/ptw_top/rtl \
  $(IP)/pseudo_lru/rtl \
  -name '*.sv' ! -name '*_pkg.sv' 2>/dev/null | sort)

NEIGH_CHANNEL_SRCS := $(shell find $(IP)/neigh_channel/rtl -name '*.sv' ! -name 'neigh_channel.sv' | sort) \
                      $(IP)/neigh_channel/rtl/neigh_channel.sv

CHANNEL_SRCS := $(REPO_ROOT)/hw/ip/shire_channel/rtl/shire_channel.sv
WRAP_SRCS := $(WRAP)/shire_channel_wrap.sv
TOP_SRCS := $(TOP)/shire_top.sv

TB_TOP := shire_top_tb
RTL_SRCS := $(PKG_SRCS) $(PRIM_SRCS) $(SC_SRCS) $(RB_SRCS) $(UC_SRCS) $(ESR_SRCS) \
            $(LEAF_SRCS) $(SBM_SRCS) $(CHANNEL_SRCS) $(WRAP_SRCS) $(NEIGH_SRCS) \
            $(NEIGH_CHANNEL_SRCS) $(IP)/neigh_top/rtl/neigh_top_pwrstub.sv \
            $(IP)/neigh_top/rtl/neigh_top.sv $(TOP_SRCS) $(CURDIR)/shire_top_tb.sv
CC_SRCS := $(CURDIR)/shire_top_test.cc
LINT_SRCS := $(filter-out $(CURDIR)/shire_top_tb.sv,$(filter-out %.cc,$(RTL_SRCS)))
LINT_FLAGS := --top-module shire_top -Wno-MULTITOP -Wno-UNOPTFLAT

# Let Verilator exit before compiling generated C++ so the Verilator process
# memory is not held at the same time as cc1plus on small CI runners.
VERILATOR_BUILD_FLAGS :=
include $(REPO_ROOT)/mk/verilator.mk

# Split generated C++ so GitHub-hosted runners do not compile one very large
# shire integration translation unit in a single cc1plus process.
COMMON_FLAGS += -Wno-SYNCASYNCNET -Wno-UNOPTFLAT -Wno-IMPORTSTAR --output-split 5000 --output-split-cfuncs 5000
