REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := intpipe_int_scoreboard

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/intpipe/rtl/intpipe_int_scoreboard.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_int_scoreboard.v

ORIG_RENAME := intpipe_int_scoreboard

include $(REPO_ROOT)/mk/rtlcosim.mk
