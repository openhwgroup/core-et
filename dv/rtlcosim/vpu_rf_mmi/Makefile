REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := vpu_rf_mmi

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(PRIM_RF_3R2W_PREVIEW) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_rf.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_rf.v \
            $(ORIG_ROOT)/rtl/libs/macros/vpu/etcust_64x32_3r2w_vpurf_model/etcust_64x32_3r2w_vpurf_model.v
ORIG_RENAME := vpu_rf
EXTRA_FLAGS := -DMINION_USE_MMI_VPU_RF

include $(REPO_ROOT)/mk/rtlcosim.mk
