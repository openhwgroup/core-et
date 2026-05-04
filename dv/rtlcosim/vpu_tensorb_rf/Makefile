REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := vpu_tensorb_rf

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorb_rf.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorb_rf.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches
ORIG_RENAME := vpu_tensorb_rf

include $(REPO_ROOT)/mk/rtlcosim.mk
