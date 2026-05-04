REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := tima_adder

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/tima/tima_adder.v
ORIG_RENAME := tima_adder

include $(REPO_ROOT)/mk/rtlcosim.mk
