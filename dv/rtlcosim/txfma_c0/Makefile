REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_c0

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_c0.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_c0.v
ORIG_RENAME := txfma_c0

include $(REPO_ROOT)/mk/rtlcosim.mk
