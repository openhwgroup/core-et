REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_csa

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_csa.v
ORIG_RENAME := txfma_csa

include $(REPO_ROOT)/mk/rtlcosim.mk
