REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := rbox_top

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_pkg.sv

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/shire/rbox \
                $(ORIG_ROOT)/rtl/shire/esr \
                $(ORIG_ROOT)/rtl/libs/clk_gate

include $(REPO_ROOT)/mk/rtlcosim.mk
