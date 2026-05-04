REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_lxd

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_lxd.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_lxd.v
ORIG_RENAME := txfma_lxd

include $(REPO_ROOT)/mk/rtlcosim.mk
