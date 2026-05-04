REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := rbox_traverse

NEW_RTL  := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_pkg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_step_mul.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_sampler.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_sample_to_fp32.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_fp_shifter.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_fp_adder_2stg.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_fp_fast_adder.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_depth_to_fp.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_depth_to_un.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_depth_offset.sv \
            $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_traverse.sv

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/registers

EXTRA_FLAGS := -Wno-CASEINCOMPLETE --x-initial 0
SIM_ARGS    := +verilator+noassert

include $(REPO_ROOT)/mk/rtlcosim.mk
