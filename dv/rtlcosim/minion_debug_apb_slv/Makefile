REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := minion_debug_apb_slv

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_debug_apb_slv.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/minion_debug_apb_slv.v

ORIG_RENAME := minion_debug_apb_slv
EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
