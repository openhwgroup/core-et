REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := fifo

NEW_RTL  := $(PRIM_FIFO) \
            $(PRIM_RF_1R1W_PREVIEW)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_fifo.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers

include $(REPO_ROOT)/mk/rtlcosim.mk
