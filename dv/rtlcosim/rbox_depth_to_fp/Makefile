REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := rbox_depth_to_fp

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_depth_to_fp.sv

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers

include $(REPO_ROOT)/mk/rtlcosim.mk
