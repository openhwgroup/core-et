REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := esr_neigh

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(PRIM_RST_SYNC) \
           $(REPO_ROOT)/hw/ip/esr_neigh/rtl/esr_neigh.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/esr/esr_neigh.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/rst_repeat.v \
            $(ORIG_ROOT)/rtl/shire/esr/esr_clk_gate.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate_n.v

EXTRA_FLAGS := +incdir+$(ORIG_ROOT)/rtl/shire/esr

ORIG_RENAME := esr_neigh

include $(REPO_ROOT)/mk/rtlcosim.mk
