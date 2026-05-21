# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := neigh_channel

PKG_SRCS := \
  $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
  $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
  $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
  $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
  $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
  $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
  $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl/neigh_voltage_cross_pkg.sv \
  $(REPO_ROOT)/hw/ip/neigh_ch_dbg/rtl/neigh_ch_dbg_pkg.sv

PRIM_SRCS := $(shell find $(REPO_ROOT)/hw/ip/prim_* -path '*/rtl/*.sv' | sort) \
             $(shell find $(REPO_ROOT)/hw/ip/tech_generic -path '*/rtl/*.sv' | sort)

NEIGH_SRCS := $(shell find \
  $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl \
  $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl \
  $(REPO_ROOT)/hw/ip/neigh_fill_fifo/rtl \
  $(REPO_ROOT)/hw/ip/neigh_shared_icache/rtl \
  $(REPO_ROOT)/hw/ip/neigh_shared_ptw/rtl \
  $(REPO_ROOT)/hw/ip/neigh_miss_ff/rtl \
  $(REPO_ROOT)/hw/ip/neigh_evict_ff/rtl \
  $(REPO_ROOT)/hw/ip/neigh_fill_ff/rtl \
  $(REPO_ROOT)/hw/ip/neigh_channel_routing/rtl \
  $(REPO_ROOT)/hw/ip/neigh_coop_tload/rtl \
  $(REPO_ROOT)/hw/ip/neigh_tensor_store_buffer/rtl \
  $(REPO_ROOT)/hw/ip/neigh_ch_dbg/rtl \
  $(REPO_ROOT)/hw/ip/dll_dly_est/rtl \
  $(REPO_ROOT)/hw/ip/neigh_voltage_cross/rtl \
  $(REPO_ROOT)/hw/ip/esr_neigh/rtl \
  $(REPO_ROOT)/hw/ip/icache_top/rtl \
  $(REPO_ROOT)/hw/ip/icache_data_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_data_ram_wrap/rtl \
  $(REPO_ROOT)/hw/ip/icache_lru_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_micro_cache/rtl \
  $(REPO_ROOT)/hw/ip/icache_micro_data_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_micro_tag_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_pma_unit/rtl \
  $(REPO_ROOT)/hw/ip/icache_tag_array/rtl \
  $(REPO_ROOT)/hw/ip/icache_tlb_array/rtl \
  $(REPO_ROOT)/hw/ip/lru_sel/rtl \
  $(REPO_ROOT)/hw/ip/minion/tlb/rtl \
  $(REPO_ROOT)/hw/ip/ptw_top/rtl \
  $(REPO_ROOT)/hw/ip/pseudo_lru/rtl \
  -name '*.sv' ! -name '*_pkg.sv' 2>/dev/null | sort)

NEW_RTL := \
  $(PKG_SRCS) \
  $(PRIM_SRCS) \
  $(NEIGH_SRCS) \
  $(shell find $(REPO_ROOT)/hw/ip/neigh_channel/rtl -name '*.sv' ! -name 'neigh_channel.sv' | sort) \
  $(REPO_ROOT)/hw/ip/neigh_channel/rtl/neigh_channel.sv

ORIG_RTL := \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_channel.v \
  $(ORIG_ROOT)/rtl/libs/mems_and_fifos/rst_repeat.v \
  $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_fifo.v \
  $(ORIG_ROOT)/rtl/libs/mems_and_fifos/vcfifo_wr_lov_ss.v \
  $(ORIG_ROOT)/rtl/libs/mems_and_fifos/vclevel_shft_h2l.v \
  $(ORIG_ROOT)/rtl/libs/mems_and_fifos/vclevel_shft_l2h.v \
  $(ORIG_ROOT)/rtl/libs/rf_latches/sys_gasket_lov.v \
  $(ORIG_ROOT)/rtl/libs/rf_latches/sys_gasket_lov_tdr_cntl.v \
  $(ORIG_ROOT)/rtl/libs/rf_latches/sys_gasket_reset.v \
  $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr.v \
  $(ORIG_ROOT)/rtl/libs/arbiters/arb_prio_dly_rdy.v \
  $(ORIG_ROOT)/rtl/shire/neigh/bpam2minions.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_ch_apb_mux.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_fill_fifo.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_shared_icache.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_shared_ptw.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_miss_ff.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_evict_ff_unit.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_evict_ff.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_fill_ff.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_local_message_network.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_tbox_router.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_fl_barrier.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_coop_tload_tag_table.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_coop_tload_ports.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_coop_tload.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_tensor_store_buffer_block.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_tensor_store_buffer.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_pmu.v \
  $(ORIG_ROOT)/rtl/shire/esr/esr_neigh.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_ch_dbg.v \
  $(ORIG_ROOT)/rtl/shire/neigh/dll_dly_est_core.v \
  $(ORIG_ROOT)/rtl/shire/neigh/dll_dly_est.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hi_voltage_cross.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_lo_voltage_cross.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_clock_feedback.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_constants.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_apb_req.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_apb_rsp.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_ipi.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_uc_fcc.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_bpam.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_l2_bank.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_neigh_sc.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_icache_err.v \
  $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_bpam_rc_tbox_ack.v \
  $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_top.v \
  $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_cache.v \
  $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_tlb_array.v \
  $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_tag_array.v \
  $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_data_array.v \
  $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_pma_unit.v \
  $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_tag_array.v \
  $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_lru_array.v \
  $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_data_array.v \
  $(ORIG_ROOT)/rtl/libs/arbiters/lru_sel.v \
  $(ORIG_ROOT)/rtl/shire/minion/tlb_top.v \
  $(ORIG_ROOT)/rtl/shire/neigh/ptw_top.v \
  $(ORIG_ROOT)/rtl/shire/minion/pseudo_lru.v

ORIG_LIBDIRS := \
  $(ORIG_ROOT)/rtl/shire/neigh \
  $(ORIG_ROOT)/rtl/shire/neigh/icache \
  $(ORIG_ROOT)/rtl/shire/esr \
  $(ORIG_ROOT)/rtl/shire/minion \
  $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
  $(ORIG_ROOT)/rtl/libs/rf_latches \
  $(ORIG_ROOT)/rtl/libs/gates \
  $(ORIG_ROOT)/rtl/libs/registers \
  $(ORIG_ROOT)/rtl/libs/arbiters \
  $(ORIG_ROOT)/rtl/libs/ecc

ORIG_RENAME := neigh_hv_logic_bpam_rc_tbox_ack \
               neigh_tensor_store_buffer_block \
               neigh_tensor_store_buffer \
               neigh_hv_logic_clock_feedback \
               neigh_local_message_network \
               neigh_hv_logic_constants \
               neigh_hv_logic_icache_err \
               neigh_hv_logic_apb_req \
               neigh_hv_logic_apb_rsp \
               neigh_hv_logic_l2_bank \
               neigh_hv_logic_neigh_sc \
               neigh_hv_logic_uc_fcc \
               neigh_hv_logic_bpam \
               neigh_hv_logic_ipi \
               neigh_coop_tload_tag_table \
               neigh_coop_tload_ports \
               neigh_coop_tload \
               neigh_hi_voltage_cross \
               neigh_lo_voltage_cross \
               neigh_shared_icache \
               neigh_shared_ptw \
               neigh_tbox_router \
               neigh_ch_apb_mux \
               neigh_pmu \
               neigh_fl_barrier \
               neigh_fill_fifo \
               neigh_evict_ff_unit \
               neigh_evict_ff \
               neigh_miss_ff \
               neigh_fill_ff \
               neigh_channel \
               bpam2minions \
               dll_dly_est_core \
               dll_dly_est \
               esr_neigh \
               neigh_ch_dbg \
               icache_micro_data_array \
               icache_micro_tag_array \
               icache_micro_cache \
               icache_tlb_array \
               icache_data_array \
               icache_lru_array \
               icache_tag_array \
               icache_pma_unit \
               icache_top \
               sys_gasket_lov_tdr_cntl \
               sys_gasket_reset \
               sys_gasket_lov \
               arb_prio_dly_rdy \
               vcfifo_wr_lov_ss \
               vclevel_shft_h2l \
               vclevel_shft_l2h \
               rst_repeat \
               gen_fifo \
               arb_rr \
               lru_sel \
               ptw_top \
               pseudo_lru

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-WIDTH -Wno-SYMRSVDWORD -Wno-COMBDLY -Wno-UNUSEDSIGNAL \
               -Wno-UNUSED -Wno-PINCONNECTEMPTY -Wno-TIMESCALEMOD -Wno-WIDTHEXPAND \
               -Wno-WIDTHTRUNC -Wno-CASEINCOMPLETE -Wno-ENUMVALUE -Wno-UNOPTFLAT \
               -Wno-SYNCASYNCNET -Wno-IMPLICITSTATIC -Wno-IMPORTSTAR
SIM_ARGS := +verilator+noassert

include $(REPO_ROOT)/mk/rtlcosim.mk
