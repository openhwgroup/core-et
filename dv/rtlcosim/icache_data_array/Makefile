REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := icache_data_array

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
            $(REPO_ROOT)/hw/ip/prim_ecc/rtl/prim_ecc_enc.sv \
            $(REPO_ROOT)/hw/ip/prim_ecc/rtl/prim_ecc_dec.sv \
            $(REPO_ROOT)/hw/ip/icache_data_array/rtl/icache_data_array.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/ecc/ecc_gen.v \
            $(ORIG_ROOT)/rtl/libs/ecc/ecc_corr.v \
            $(ORIG_ROOT)/rtl/shire/neigh/icache/icache_data_array.v

ORIG_RENAME := icache_data_array

include $(REPO_ROOT)/mk/rtlcosim.mk
