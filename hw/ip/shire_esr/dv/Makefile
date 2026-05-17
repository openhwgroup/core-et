REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COMMON_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
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
              $(REPO_ROOT)/hw/ip/shire_esr/rtl/shire_esr_pkg.sv

PRIM_RTL := $(PRIM_RST_SYNC) \
            $(PRIM_WRITE_PREVIEW_EN) \
            $(PRIM_WRITE_COMMIT_RST_EN)

TESTS := apb_esr_ff esr_cache_bank esr_shire_other

apb_esr_ff_TOP := apb_esr_ff_tb
apb_esr_ff_SRCS := $(COMMON_RTL) \
                   $(REPO_ROOT)/hw/ip/shire_esr/rtl/apb_esr_ff.sv \
                   $(CURDIR)/apb_esr_ff_tb.sv \
                   $(CURDIR)/apb_esr_ff_test.cc

esr_cache_bank_TOP := esr_cache_bank_tb
esr_cache_bank_SRCS := $(COMMON_RTL) \
                       $(REPO_ROOT)/hw/ip/shire_esr/rtl/esr_cache_bank.sv \
                       $(CURDIR)/esr_cache_bank_tb.sv \
                       $(CURDIR)/esr_cache_bank_test.cc

esr_shire_other_TOP := esr_shire_other_tb
esr_shire_other_SRCS := $(COMMON_RTL) \
                        $(PRIM_RTL) \
                        $(REPO_ROOT)/hw/ip/shire_esr/rtl/esr_shire_other.sv \
                        $(CURDIR)/esr_shire_other_tb.sv \
                        $(CURDIR)/esr_shire_other_test.cc

LINT_SRCS := $(COMMON_RTL) \
             $(PRIM_RTL) \
             $(REPO_ROOT)/hw/ip/shire_esr/rtl/apb_esr_ff.sv \
             $(REPO_ROOT)/hw/ip/shire_esr/rtl/esr_cache_bank.sv \
             $(REPO_ROOT)/hw/ip/shire_esr/rtl/esr_shire_other.sv \
             $(CURDIR)/apb_esr_ff_tb.sv \
             $(CURDIR)/esr_cache_bank_tb.sv \
             $(CURDIR)/esr_shire_other_tb.sv
LINT_FLAGS := -Wno-MULTITOP

include $(REPO_ROOT)/mk/verilator.mk
