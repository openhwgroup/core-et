REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := replay

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_replay.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_replay.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs

ORIG_RENAME := intpipe_csr_replay

include $(REPO_ROOT)/mk/rtlcosim.mk
