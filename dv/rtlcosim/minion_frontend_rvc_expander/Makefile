REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := rvc_expander

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_rvc_expander.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/frontend/frontend_rvc_expander.v

include $(REPO_ROOT)/mk/rtlcosim.mk
