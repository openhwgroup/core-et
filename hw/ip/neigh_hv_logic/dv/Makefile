REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := neigh_hv_logic_tb
RTL_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_pkg/rtl/neigh_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
            $(REPO_ROOT)/hw/ip/prim_arb_rr/rtl/prim_arb_rr.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_apb_req.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_apb_rsp.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_bpam.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_bpam_rc_tbox_ack.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_clock_feedback.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_constants.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_icache_err.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_ipi.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_l2_bank.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_neigh_sc.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pwr_ctrl_tdr_isolate.sv \
            $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_uc_fcc.sv \
            $(CURDIR)/neigh_hv_logic_tb.sv
CC_SRCS  := $(CURDIR)/neigh_hv_logic_test.cc

include $(REPO_ROOT)/mk/verilator.mk
