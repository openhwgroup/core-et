REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := esr_cache_bank

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_esr/rtl/esr_cache_bank.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/esr/esr_cache_bank.v
EXTRA_FLAGS := +incdir+$(ORIG_ROOT)/rtl/shire/esr
ORIG_RENAME := esr_cache_bank

include $(REPO_ROOT)/mk/rtlcosim.mk
