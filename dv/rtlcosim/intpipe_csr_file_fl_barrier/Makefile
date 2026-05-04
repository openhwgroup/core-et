REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := fl_barrier

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_fl_barrier.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_file_fl_barrier.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs

ORIG_RENAME := intpipe_csr_file_fl_barrier

include $(REPO_ROOT)/mk/rtlcosim.mk
