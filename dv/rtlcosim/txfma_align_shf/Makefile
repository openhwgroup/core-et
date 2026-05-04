REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_align_shf

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_align_shf.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_align_shf.v
ORIG_RENAME := txfma_align_shf

include $(REPO_ROOT)/mk/rtlcosim.mk
