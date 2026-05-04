REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := icache_micro_data_array

NEW_RTL := $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(PRIM_RF_1R1W_DEC_PREVIEW) \
           $(REPO_ROOT)/hw/ip/icache_micro_data_array/rtl/icache_micro_data_array.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_micro_data_array.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_1w_dec.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches

EXTRA_FLAGS := -DET_ASSERT_OFF -Wno-DECLFILENAME -Wno-COMBDLY -Wno-TIMESCALEMOD
ORIG_RENAME := icache_micro_data_array

include $(REPO_ROOT)/mk/rtlcosim.mk
