REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := csr_file

include $(REPO_ROOT)/mk/prim.mk

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
	           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
	           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
	           $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pkg.sv \
	           $(PRIM_CLK_GATE) \
	           $(PRIM_CLK_GATE_N) \
	           $(PRIM_WRITE_PREVIEW_EN) $(PRIM_WRITE_COMMIT_EN) $(PRIM_WRITE_COMMIT_RST_EN) \
	           $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_pmu_read_interface.sv \
           $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_replay.sv \
           $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_conv.sv \
           $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_msgs.sv \
           $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file_fl_barrier.sv \
           $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl/intpipe_csr_file.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_file.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_file_conv.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_file_fl_barrier.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_replay.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_msgs.v \
            $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr/intpipe_csr_pmu_read_interface.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/shire/minion/intpipe/csr \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/counters \
                $(ORIG_ROOT)/rtl/libs/mems_and_fifos \
                $(ORIG_ROOT)/rtl/libs

ORIG_RENAME := intpipe_csr_file \
               intpipe_csr_file_conv \
               intpipe_csr_file_fl_barrier \
               intpipe_csr_replay \
               intpipe_csr_msgs \
               intpipe_csr_pmu_read_interface

EXTRA_FLAGS := --converge-limit 10000 --x-initial-edge \
               +incdir+$(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl \
               +incdir+$(ORIG_ROOT)/rtl/shire/minion/intpipe/csr

include $(REPO_ROOT)/mk/rtlcosim.mk
