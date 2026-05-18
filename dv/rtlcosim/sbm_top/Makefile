REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := sbm_top

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/apb/rtl/apb_pkg.sv \
           $(REPO_ROOT)/hw/ip/axi/rtl/axi_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_sbm/rtl/shire_sbm_pkg.sv \
           $(REPO_ROOT)/hw/ip/prim_fifo_reg/rtl/prim_fifo_reg.sv \
           $(REPO_ROOT)/hw/ip/prim_arb_rr/rtl/prim_arb_rr.sv \
           $(REPO_ROOT)/hw/ip/shire_sbm/rtl/mshire_axi_to_apb.sv \
           $(REPO_ROOT)/hw/ip/shire_sbm/rtl/shire_bus_master.sv \
           $(REPO_ROOT)/hw/ip/shire_sbm/rtl/sbm_top.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_bus_master/sbm_top.v \
            $(ORIG_ROOT)/rtl/shire/shire_bus_master/shire_bus_master.v \
            $(ORIG_ROOT)/rtl/shire/shire_bus_master/mshire_axi_to_apb.v \
            $(ORIG_ROOT)/rtl/shire/ultrasoc_debug/ms_bpam_decoder.v \
            $(ORIG_ROOT)/rtl/shire/ultrasoc_debug/arb_apb_ust.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/apb_ff.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_fifo_reg.v \
            $(ORIG_ROOT)/rtl/libs/registers/en_ff_gated_struct.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr_data.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr_base.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/onehot_mux.v
ORIG_RENAME := shire_bus_master mshire_axi_to_apb sbm_top
EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
