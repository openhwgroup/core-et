REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := rbox_depth_offset

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_fp_shifter.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_fp_adder_2stg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_depth_offset.sv

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/shire/rbox

include $(REPO_ROOT)/mk/rtlcosim.mk
