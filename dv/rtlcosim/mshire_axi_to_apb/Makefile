REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := mshire_axi_to_apb

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/apb/rtl/apb_pkg.sv \
           $(REPO_ROOT)/hw/ip/axi/rtl/axi_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_sbm/rtl/shire_sbm_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_sbm/rtl/mshire_axi_to_apb.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_bus_master/mshire_axi_to_apb.v
ORIG_RENAME := mshire_axi_to_apb
EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
