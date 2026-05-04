REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := rbox_depth_buffer

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_pkg.sv \
            $(PRIM_RAM_1P) \
            $(PRIM_RAM_2P) \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_depth_buffer.sv

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/shire/rbox

include $(REPO_ROOT)/mk/rtlcosim.mk
