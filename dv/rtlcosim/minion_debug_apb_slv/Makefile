REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := minion_debug_apb_slv

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_debug_apb_slv.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/minion_debug_apb_slv.v

ORIG_RENAME := minion_debug_apb_slv

# The original soc.vh include stack defines many unrelated helper functions with
# legacy width and reserved-word warnings. The standalone DUT cosim only needs
# the debug APB types from that stack, so keep those original-header diagnostics
# out of the focused block-level run.
EXTRA_FLAGS := -Wno-WIDTH -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
