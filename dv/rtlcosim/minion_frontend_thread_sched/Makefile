REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := thread_sched

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_thread_sched.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/frontend/frontend_thread_sched.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/registers

include $(REPO_ROOT)/mk/rtlcosim.mk
