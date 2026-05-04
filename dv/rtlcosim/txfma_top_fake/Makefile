REPO_ROOT  := $(shell git rev-parse --show-toplevel)

COSIM_NAME := txfma_top_fake

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_top_fake.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_top_fake.v
ORIG_RENAME := txfma_top_fake

include $(REPO_ROOT)/mk/rtlcosim.mk
