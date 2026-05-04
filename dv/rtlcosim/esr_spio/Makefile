REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := esr_spio

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
            $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
            $(REPO_ROOT)/hw/ip/esr_spio/rtl/esr_spio.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/esr/esr_spio.v

ORIG_RENAME := esr_spio
EXTRA_FLAGS := +incdir+$(ORIG_ROOT)/rtl/shire/esr

include $(REPO_ROOT)/mk/rtlcosim.mk
