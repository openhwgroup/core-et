REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := intpipe_alu

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_alu.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_alu.v

ORIG_RENAME := intpipe_alu

include $(REPO_ROOT)/mk/rtlcosim.mk
