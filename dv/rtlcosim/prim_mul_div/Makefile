REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := prim_mul_div

include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(PRIM_PHASE_PAIR_LO_HI) \
           $(PRIM_PHASE_PAIR_HI_LO) \
           $(PRIM_CLK_GATE) \
           $(PRIM_CLK_GATE_N) \
           $(PRIM_CMP_32) \
           $(PRIM_CMP_42) \
           $(PRIM_MUL_DIV_SRCS)

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_mul_div_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_mul_div_ctl.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/intpipe_mul_div_dp.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/shire/minion/intpipe \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/counters \
                $(ORIG_ROOT)/rtl/libs/compressors \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs

EXTRA_FLAGS := --converge-limit 10000 --x-initial-edge

ORIG_RENAME := intpipe_mul_div_top \
               intpipe_mul_div_ctl \
               intpipe_mul_div_dp

include $(REPO_ROOT)/mk/rtlcosim.mk
