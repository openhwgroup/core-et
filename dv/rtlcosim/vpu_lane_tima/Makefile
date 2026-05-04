REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := vpu_lane_tima

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(PRIM_CLK_GATE) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_adder.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/tima_top.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorb_rf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorc_rf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_lane_tima.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/tima/tima_adder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/tima/tima_top.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorb_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorc_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_lane_tima.v
ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/clock_gates
ORIG_RENAME := vpu_lane_tima vpu_tensorc_rf vpu_tensorb_rf tima_top tima_adder

include $(REPO_ROOT)/mk/rtlcosim.mk
