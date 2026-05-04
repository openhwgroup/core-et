REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_4_2_compressor

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_csa.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_4_2_compressor.v
ORIG_RENAME := txfma_4_2_compressor txfma_csa

include $(REPO_ROOT)/mk/rtlcosim.mk
