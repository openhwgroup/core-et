REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := intpipe_rf

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(PRIM_RF_2R1W_PREVIEW) \
            $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_rf.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_rf.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs

ORIG_RENAME := intpipe_rf

include $(REPO_ROOT)/mk/rtlcosim.mk
