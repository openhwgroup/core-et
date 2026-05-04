REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_booth_ppg_32r4

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_norm.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_msb.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4_norm.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4_msb.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4.v
ORIG_RENAME := txfma_booth_ppg_32r4_norm txfma_booth_ppg_32r4_msb txfma_booth_ppg_32r4

include $(REPO_ROOT)/mk/rtlcosim.mk
