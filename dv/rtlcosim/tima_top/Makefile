REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := tima_top

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_top.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/tima/tima_adder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/tima/tima_top.v
ORIG_RENAME := tima_top tima_adder

include $(REPO_ROOT)/mk/rtlcosim.mk
