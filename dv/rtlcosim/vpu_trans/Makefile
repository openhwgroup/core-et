REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := vpu_trans

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(PRIM_CLK_GATE) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_trans.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_trans.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches
ORIG_RENAME := vpu_trans

include $(REPO_ROOT)/mk/rtlcosim.mk
