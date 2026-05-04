REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := conv

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_conv.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_file_conv.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs

ORIG_RENAME := intpipe_csr_file_conv

include $(REPO_ROOT)/mk/rtlcosim.mk
