REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := vpu_decoder

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/vpu_decoder.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_decoder.v

ORIG_RENAME := vpu_decoder
EXTRA_FLAGS := -GEnableExtraTrans=1 +define+ENABLE_EXTRA_TRANS

include $(REPO_ROOT)/mk/rtlcosim.mk
