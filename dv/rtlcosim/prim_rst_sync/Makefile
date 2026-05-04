REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := rst_sync

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(PRIM_RST_SYNC)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/mems_and_fifos/rst_repeat.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches

include $(REPO_ROOT)/mk/rtlcosim.mk
