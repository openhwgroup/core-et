REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := apb_esr_ff

NEW_RTL := $(REPO_ROOT)/hw/ip/apb/rtl/apb_pkg.sv \
           $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/shire_esr/rtl/apb_esr_ff.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/shire_channel/apb_esr_ff.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/apb_ff.v

ORIG_RENAME := apb_esr_ff

include $(REPO_ROOT)/mk/rtlcosim.mk
