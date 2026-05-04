REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := intpipe_inst_bits_stage

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_inst_bits_stage.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_inst_bits_stage.v

ORIG_RENAME := intpipe_inst_bits_stage

include $(REPO_ROOT)/mk/rtlcosim.mk
