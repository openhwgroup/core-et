REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := fifo_sram

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
           $(PRIM_RAM_1P) \
           $(PRIM_RAM_2P) \
           $(PRIM_FIFO_SRAM)
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/rbox/rbox_fifo.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches

include $(REPO_ROOT)/mk/rtlcosim.mk
