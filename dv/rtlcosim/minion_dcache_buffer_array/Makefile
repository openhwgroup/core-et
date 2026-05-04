REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := minion_dcache_buffer_array

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(PRIM_RF_1R1W_PREVIEW) \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_buffer_array.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_1w.v \
            $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_buffer_array.v

EXTRA_FLAGS := -Wno-WIDTH -Wno-SYMRSVDWORD -Wno-COMBDLY

include $(REPO_ROOT)/mk/rtlcosim.mk
