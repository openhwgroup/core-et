REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_adder

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_adder.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_adder.v
ORIG_RENAME := txfma_adder

include $(REPO_ROOT)/mk/rtlcosim.mk
