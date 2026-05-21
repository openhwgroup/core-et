REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := esr_shire_other

include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
           $(REPO_ROOT)/hw/ip/apb/rtl/apb_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
           $(PRIM_RST_SYNC) \
           $(REPO_ROOT)/hw/ip/shire_esr/rtl/esr_shire_other.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate_n.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/rst_repeat.v \
            $(ORIG_ROOT)/rtl/shire/esr/esr_clk_gate.v \
            $(ORIG_ROOT)/rtl/shire/esr/esr_shire_other.v

EXTRA_FLAGS := +incdir+$(ORIG_ROOT)/rtl/shire/esr
ORIG_RENAME := esr_shire_other

include $(REPO_ROOT)/mk/rtlcosim.mk
