REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := mul_div_ctl

include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(PRIM_PHASE_PAIR_LO_HI) \
           $(PRIM_CLK_GATE) \
           $(PRIM_CLK_GATE_N) \
           $(REPO_ROOT)/hw/ip/tech_generic/prim_mul_div/rtl/intpipe_mul_div_ctl.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_mul_div_ctl.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/shire/minion/intpipe \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/counters \
                $(ORIG_ROOT)/rtl/libs

ORIG_RENAME := intpipe_mul_div_ctl

include $(REPO_ROOT)/mk/rtlcosim.mk
