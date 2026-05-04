REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := dataq

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(PRIM_RST_SYNC) \
            $(PRIM_FIFO_REG) \
            $(PRIM_ARB_RR) \
            $(PRIM_ARB_PRIO) \
            $(PRIM_RF_1R1W_PREVIEW) \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_bank_dataq.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_cache/shire_cache_bank_dataq.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/rst_repeat.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_fifo_reg.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr_data.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_rr_base.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/onehot_mux.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_prio_data_held.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/arb_prio.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_1w.v \
            $(ORIG_ROOT)/rtl/libs/registers/en_ff_gated_struct.v

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
