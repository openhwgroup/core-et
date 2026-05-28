REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

RTL_DIR := $(REPO_ROOT)/hw/ip
SC_RTL  := $(RTL_DIR)/shirecache/rtl

# ── Package and primitive dependencies ───────────────────

PKGS := $(RTL_DIR)/dft/rtl/dft_pkg.sv \
        $(RTL_DIR)/ram_cfg/rtl/ram_cfg_pkg.sv \
        $(RTL_DIR)/etlink/rtl/etlink_pkg.sv \
        $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
        $(SC_RTL)/shirecache_pkg.sv

PRIM := $(PRIM_CLK_GATE) $(PRIM_CLK_MUX) $(PRIM_ECO_PORTS) $(PRIM_RAM_1P) $(PRIM_RAM_2P)

# ── Test definitions ─────────────────────────────────────

TESTS := lru set swz tram tsram dram sbm cbuf ato rbuf icsm dataq reqq_entry reqq_alloc reqq rspmux xbar mesh l3s ms mm l3b bmbx bwrap l2hpf pmq pm errl trc topclk topb

lru_TOP  := shirecache_pipe_lru_4w_calc
lru_SRCS := $(PKGS) $(SC_RTL)/shirecache_pipe_lru_4w_calc.sv \
            $(CURDIR)/shirecache_lru_test.cc

set_TOP  := shirecache_pipe_set_calc
set_SRCS := $(PKGS) $(SC_RTL)/shirecache_pipe_set_calc.sv \
            $(CURDIR)/shirecache_set_calc_test.cc

swz_TOP  := shirecache_l3_swizzle_tb
swz_SRCS := $(PKGS) \
            $(SC_RTL)/shirecache_pipe_l3_swizzle_get.sv \
            $(SC_RTL)/shirecache_pipe_l3_swizzle_put.sv \
            $(CURDIR)/shirecache_l3_swizzle_tb.sv \
            $(CURDIR)/shirecache_l3_swizzle_test.cc

tram_TOP  := shirecache_pipe_tag_ram_wrap
tram_SRCS := $(PKGS) $(PRIM) \
             $(SC_RTL)/shirecache_pipe_tag_ram_wrap.sv \
             $(CURDIR)/shirecache_tag_ram_test.cc

tsram_TOP  := shirecache_pipe_tag_state_ram_wrap
tsram_SRCS := $(PKGS) $(PRIM) \
              $(SC_RTL)/shirecache_pipe_tag_state_ram_wrap.sv \
              $(CURDIR)/shirecache_tag_state_ram_test.cc

dram_TOP  := shirecache_pipe_data_ram_wrap
dram_SRCS := $(PKGS) $(PRIM) \
             $(SC_RTL)/shirecache_pipe_data_ram_wrap.sv \
             $(CURDIR)/shirecache_data_ram_test.cc

sbm_TOP  := shirecache_pipe_sub_bank_mem
sbm_SRCS := $(PKGS) $(PRIM) \
            $(SC_RTL)/shirecache_pipe_tag_ram_wrap.sv \
            $(SC_RTL)/shirecache_pipe_tag_state_ram_wrap.sv \
            $(SC_RTL)/shirecache_pipe_data_ram_wrap.sv \
            $(SC_RTL)/shirecache_pipe_sub_bank_mem.sv \
            $(CURDIR)/shirecache_sub_bank_mem_test.cc

cbuf_TOP  := shirecache_cbuf_tb
cbuf_SRCS := $(PKGS) $(PRIM_ARB_LRU) \
             $(SC_RTL)/shirecache_pipe_cbuf.sv \
             $(CURDIR)/shirecache_cbuf_tb.sv \
             $(CURDIR)/shirecache_cbuf_test.cc

ato_TOP  := shirecache_atomic_tb
ato_SRCS := $(PKGS) \
            $(SC_RTL)/shirecache_pipe_atomic.sv \
            $(CURDIR)/shirecache_atomic_tb.sv \
            $(CURDIR)/shirecache_atomic_test.cc

rbuf_TOP  := shirecache_rbuf_tb
rbuf_SRCS := $(PKGS) $(PRIM_ARB_LRU) $(PRIM_FIFO_REG) \
             $(SC_RTL)/shirecache_pipe_rbuf.sv \
             $(CURDIR)/shirecache_rbuf_tb.sv \
             $(CURDIR)/shirecache_rbuf_test.cc

icsm_TOP  := shirecache_idx_cop_sm_tb
icsm_SRCS := $(PKGS) $(PRIM_RST_SYNC) \
             $(SC_RTL)/shirecache_pipe_idx_cop_sm.sv \
             $(CURDIR)/shirecache_idx_cop_sm_tb.sv \
             $(CURDIR)/shirecache_idx_cop_sm_test.cc

dataq_TOP  := shirecache_dataq_tb
dataq_SRCS := $(PKGS) $(PRIM_RST_SYNC) $(PRIM_FIFO_REG) $(PRIM_ARB_RR) $(PRIM_ARB_PRIO) $(PRIM_RF_1R1W_PREVIEW) \
              $(SC_RTL)/shirecache_bank_dataq.sv \
              $(CURDIR)/shirecache_dataq_tb.sv \
              $(CURDIR)/shirecache_dataq_test.cc

reqq_entry_TOP  := shirecache_reqq_entry_tb
reqq_entry_SRCS := $(PKGS) $(PRIM_RST_SYNC) $(PRIM_CLK_GATE) \
                   $(SC_RTL)/shirecache_pipe_l3_swizzle_get.sv \
                   $(SC_RTL)/shirecache_bank_reqq_entry.sv \
                   $(CURDIR)/shirecache_reqq_entry_tb.sv \
                   $(CURDIR)/shirecache_reqq_entry_test.cc

reqq_alloc_TOP  := shirecache_reqq_alloc_tb
reqq_alloc_SRCS := $(PKGS) $(PRIM_CLK_GATE) \
                   $(PRIM_ARB_RR) $(PRIM_ARB_RR2) \
                   $(SC_RTL)/shirecache_bank_reqq_alloc.sv \
                   $(CURDIR)/shirecache_reqq_alloc_tb.sv \
                   $(CURDIR)/shirecache_reqq_alloc_test.cc

reqq_TOP  := shirecache_reqq_tb
reqq_SRCS := $(PKGS) $(PRIM_RST_SYNC) $(PRIM_CLK_GATE) \
             $(PRIM_ARB_RR) $(PRIM_ARB_RR2) $(PRIM_ARB_PRIO) $(PRIM_FIFO_REG) \
             $(SC_RTL)/shirecache_pipe_l3_swizzle_get.sv \
             $(SC_RTL)/shirecache_bank_reqq_entry.sv \
             $(SC_RTL)/shirecache_bank_reqq_alloc.sv \
             $(SC_RTL)/shirecache_bank_reqq.sv \
             $(CURDIR)/shirecache_reqq_tb.sv \
             $(CURDIR)/shirecache_reqq_test.cc

rspmux_TOP  := shirecache_rspmux_tb
rspmux_SRCS := $(PKGS) $(PRIM_ARB_RR) $(PRIM_ARB_LRU_2BID) $(PRIM_FIFO_REG) \
               $(SC_RTL)/shirecache_bank_rspmux.sv \
               $(CURDIR)/shirecache_rspmux_tb.sv \
               $(CURDIR)/shirecache_rspmux_test.cc

xbar_TOP  := shirecache_xbar_tb
xbar_SRCS := $(PKGS) $(PRIM_ARB_RR) $(PRIM_FIFO_REG) \
             $(SC_RTL)/shirecache_xbar.sv \
             $(CURDIR)/shirecache_xbar_tb.sv \
             $(CURDIR)/shirecache_xbar_test.cc

mesh_TOP  := shirecache_bank_mesh_tb
mesh_SRCS := $(PKGS) $(PRIM_CLK_GATE) $(PRIM_FIFO_REG) $(PRIM_ARB_PRIO) \
             $(SC_RTL)/shirecache_bank_mesh.sv \
             $(CURDIR)/shirecache_bank_mesh_tb.sv \
             $(CURDIR)/shirecache_bank_mesh_test.cc

l3s_TOP  := shirecache_l3_slave_tb
l3s_SRCS := $(PKGS) $(PRIM_FIFO_REG) $(PRIM_ARB_RR) \
            $(SC_RTL)/shirecache_bank_l3_slave.sv \
            $(CURDIR)/shirecache_l3_slave_tb.sv \
            $(CURDIR)/shirecache_l3_slave_test.cc

AXI_PKG         := $(RTL_DIR)/axi/rtl/axi_pkg.sv

topclk_TOP  := shirecache_top_freeclk_tb
topclk_SRCS := $(PKGS) $(AXI_PKG) $(PRIM_RST_SYNC) $(PRIM_FIFO_REG) \
               $(SC_RTL)/shirecache_top.sv \
               $(CURDIR)/shirecache_top_freeclk_tb.sv \
               $(CURDIR)/shirecache_top_freeclk_test.cc

ms_TOP  := shirecache_mesh_slave_tb
ms_SRCS := $(PKGS) $(AXI_PKG) $(PRIM_FIFO_REG) $(PRIM_FIFO_ASYNC_HIV) $(PRIM_FIFO_ASYNC_LOV) $(PRIM_ARB_RR) \
           $(SC_RTL)/shirecache_pipe_l3_swizzle_get.sv \
           $(SC_RTL)/shirecache_mesh_slave_axi_port.sv \
           $(SC_RTL)/shirecache_mesh_slave.sv \
           $(CURDIR)/shirecache_mesh_slave_tb.sv \
           $(CURDIR)/shirecache_mesh_slave_test.cc

mm_TOP  := shirecache_mesh_master_tb
mm_SRCS := $(PKGS) $(AXI_PKG) $(PRIM_CLK_GATE) $(PRIM_FIFO_REG) $(PRIM_FIFO_ASYNC_HIV) $(PRIM_FIFO_ASYNC_LOV) $(PRIM_ARB_RR) \
           $(SC_RTL)/shirecache_pipe_l3_swizzle_get.sv \
           $(SC_RTL)/shirecache_mesh_master_axi_port.sv \
           $(SC_RTL)/shirecache_mesh_master.sv \
           $(CURDIR)/shirecache_mesh_master_tb.sv \
           $(CURDIR)/shirecache_mesh_master_test.cc

l3b_TOP  := shirecache_l3_to_sys_bridge_tb
l3b_SRCS := $(PKGS) $(AXI_PKG) $(PRIM_FIFO_ASYNC_HIV) $(PRIM_FIFO_ASYNC_LOV) $(PRIM_ARB_RR) \
            $(RTL_DIR)/prim_hot2bin/rtl/prim_hot2bin.sv \
            $(SC_RTL)/shirecache_l3_to_sys_bridge.sv \
            $(CURDIR)/shirecache_l3_to_sys_bridge_tb.sv \
            $(CURDIR)/shirecache_l3_to_sys_bridge_test.cc

bmbx_TOP  := shirecache_bist_mbx_tb
bmbx_SRCS := $(PKGS) \
             $(SC_RTL)/shirecache_bist_mbx.sv \
             $(CURDIR)/shirecache_bist_mbx_tb.sv \
             $(CURDIR)/shirecache_bist_mbx_test.cc

bwrap_TOP  := shirecache_bist_wrapper_tb
bwrap_SRCS := $(PKGS) \
              $(SC_RTL)/shirecache_bist_mbx.sv \
              $(SC_RTL)/shirecache_bist_wrapper.sv \
              $(CURDIR)/shirecache_bist_wrapper_tb.sv \
              $(CURDIR)/shirecache_bist_wrapper_test.cc

l2hpf_TOP  := shirecache_l2hpf_tb
l2hpf_SRCS := $(PKGS) \
              $(SC_RTL)/shirecache_bank_l2hpf.sv \
              $(CURDIR)/shirecache_l2hpf_tb.sv \
              $(CURDIR)/shirecache_l2hpf_test.cc

pmq_TOP  := shirecache_perfmon_qual_tb
pmq_SRCS := $(PKGS) \
            $(SC_RTL)/shirecache_bank_perfmon_qual.sv \
            $(CURDIR)/shirecache_perfmon_qual_tb.sv \
            $(CURDIR)/shirecache_perfmon_qual_test.cc

pm_TOP  := shirecache_perfmon_tb
pm_SRCS := $(PKGS) $(PRIM_CLK_GATE) \
           $(SC_RTL)/shirecache_perfmon_ctl.sv \
           $(SC_RTL)/shirecache_bank_perfmon_qual.sv \
           $(SC_RTL)/shirecache_bank_perfmon.sv \
           $(CURDIR)/shirecache_perfmon_tb.sv \
           $(CURDIR)/shirecache_perfmon_test.cc

errl_TOP  := shirecache_err_logger_tb
errl_SRCS := $(PKGS) $(PRIM_ARB_PRIO) \
             $(RTL_DIR)/prim_hot2bin/rtl/prim_hot2bin.sv \
             $(SC_RTL)/shirecache_bank_err_logger.sv \
             $(CURDIR)/shirecache_err_logger_tb.sv \
             $(CURDIR)/shirecache_err_logger_test.cc

trc_TOP  := shirecache_trace_tb
trc_SRCS := $(PKGS) $(PRIM_CLK_GATE) \
            $(SC_RTL)/shirecache_bank_trace.sv \
            $(CURDIR)/shirecache_trace_tb.sv \
            $(CURDIR)/shirecache_trace_test.cc

# ── Lint ─────────────────────────────────────────────────

LINT_SRCS := $(PKGS) $(PRIM) $(PRIM_ARB_LRU) $(PRIM_ARB_LRU_2BID) $(PRIM_FIFO_REG) $(PRIM_RST_SYNC) $(PRIM_ARB_RR) $(PRIM_ARB_PRIO) $(PRIM_RF_1R1W_PREVIEW) $(PRIM_ARB_RR2) $(PRIM_FIFO_ASYNC_HIV) $(PRIM_FIFO_ASYNC_LOV) $(AXI_PKG) \
  $(RTL_DIR)/prim_ecc/rtl/prim_ecc_enc.sv \
  $(RTL_DIR)/prim_ecc/rtl/prim_ecc_dec.sv \
  $(SC_RTL)/shirecache_pipe_lru_4w_calc.sv \
  $(SC_RTL)/shirecache_pipe_set_calc.sv \
  $(SC_RTL)/shirecache_pipe_l3_swizzle_get.sv \
  $(SC_RTL)/shirecache_pipe_l3_swizzle_put.sv \
  $(SC_RTL)/shirecache_pipe_tag_ram_wrap.sv \
  $(SC_RTL)/shirecache_pipe_tag_state_ram_wrap.sv \
  $(SC_RTL)/shirecache_pipe_data_ram_wrap.sv \
  $(SC_RTL)/shirecache_pipe_sub_bank_mem.sv \
  $(SC_RTL)/shirecache_pipe_sbe_insert.sv \
  $(SC_RTL)/shirecache_pipe_cbuf.sv \
  $(SC_RTL)/shirecache_pipe_atomic.sv \
  $(SC_RTL)/shirecache_pipe_rbuf.sv \
  $(SC_RTL)/shirecache_pipe_idx_cop_sm.sv \
  $(SC_RTL)/shirecache_pipe_sub_bank.sv \
  $(SC_RTL)/shirecache_pipe.sv \
  $(SC_RTL)/shirecache_bank_dataq.sv \
  $(SC_RTL)/shirecache_bank_reqq_entry.sv \
  $(SC_RTL)/shirecache_bank_reqq_alloc.sv \
  $(SC_RTL)/shirecache_bank_reqq.sv \
  $(SC_RTL)/shirecache_bank_rspmux.sv \
  $(SC_RTL)/shirecache_xbar.sv \
  $(SC_RTL)/shirecache_bank_mesh.sv \
  $(SC_RTL)/shirecache_bank_l3_slave.sv \
  $(SC_RTL)/shirecache_mesh_master_axi_port.sv \
  $(SC_RTL)/shirecache_mesh_master.sv \
  $(SC_RTL)/shirecache_mesh_slave_axi_port.sv \
  $(SC_RTL)/shirecache_mesh_slave.sv \
  $(RTL_DIR)/prim_hot2bin/rtl/prim_hot2bin.sv \
  $(SC_RTL)/shirecache_l3_to_sys_bridge.sv \
  $(SC_RTL)/shirecache_bist_mbx.sv \
  $(SC_RTL)/shirecache_bist_wrapper.sv \
  $(SC_RTL)/shirecache_bank_l2hpf.sv \
  $(SC_RTL)/shirecache_bank_perfmon_qual.sv \
  $(SC_RTL)/shirecache_perfmon_ctl.sv \
  $(SC_RTL)/shirecache_bank_perfmon.sv \
  $(SC_RTL)/shirecache_bank_err_logger.sv \
  $(SC_RTL)/shirecache_bank_trace.sv \
  $(SC_RTL)/shirecache_bank.sv \
  $(SC_RTL)/shirecache_top.sv
LINT_FLAGS := -Wno-MULTITOP

topb_TOP  := shirecache_top_bist_tb
topb_SRCS := $(LINT_SRCS) \
             $(CURDIR)/shirecache_top_bist_tb.sv \
             $(CURDIR)/shirecache_top_bist_test.cc

# Let Verilator exit before compiling generated C++ so the Verilator process
# memory is not held at the same time as cc1plus on small CI runners.
VERILATOR_BUILD_FLAGS :=
# Split generated C++ for the largest cache tests so GitHub-hosted runners do
# not compile oversized generated translation units in one cc1plus process.
SHIRECACHE_VERILATOR_FLAGS := --output-split 5000 --output-split-cfuncs 5000
$(foreach t,$(TESTS),$(eval $(t)_FLAGS += $(SHIRECACHE_VERILATOR_FLAGS)))

include $(REPO_ROOT)/mk/verilator.mk
