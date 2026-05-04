REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := pmu_read_interface

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pmu_read_interface.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_pmu_read_interface.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs

ORIG_RENAME := intpipe_csr_pmu_read_interface

include $(REPO_ROOT)/mk/rtlcosim.mk
