REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := minion_dcache_reduce

NEW_RTL  := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(PRIM_CLK_GATE) \
            $(PRIM_FIFO_SYNC) \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_reduce.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_reduce.v \
            $(ORIG_ROOT)/rtl/libs/mems_and_fifos/gen_fifo.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs/rf_latches
EXTRA_FLAGS := -Wno-WIDTH -Wno-SYMRSVDWORD -Wno-COMBDLY -Wno-UNUSEDSIGNAL -Wno-UNUSED

include $(REPO_ROOT)/mk/rtlcosim.mk
