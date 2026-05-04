REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := rbox_step_mul

NEW_RTL  := $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_step_mul.sv

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers

include $(REPO_ROOT)/mk/rtlcosim.mk
