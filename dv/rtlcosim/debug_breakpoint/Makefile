REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := debug_breakpoint

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/debug_breakpoint.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/debug_breakpoint.v

ORIG_RENAME := debug_breakpoint

include $(REPO_ROOT)/mk/rtlcosim.mk
