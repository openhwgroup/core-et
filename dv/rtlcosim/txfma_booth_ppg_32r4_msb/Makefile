REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_booth_ppg_32r4_msb

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_msb.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4_msb.v
ORIG_RENAME := txfma_booth_ppg_32r4_msb

include $(REPO_ROOT)/mk/rtlcosim.mk
