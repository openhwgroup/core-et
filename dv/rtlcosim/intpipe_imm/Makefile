REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := intpipe_imm

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_imm.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_imm.v

ORIG_RENAME := intpipe_imm

include $(REPO_ROOT)/mk/rtlcosim.mk
