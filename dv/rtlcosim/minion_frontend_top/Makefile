REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := frontend_top

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
           $(PRIM_ARB_LRU) \
           $(PRIM_RF_1R1W_DIFF_PREVIEW) \
           $(PRIM_CLK_GATE) \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_rvc_expander.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_thread_buffer.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_thread_sched.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/intpipe_decode.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/vpu_decoder.sv \
           $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/frontend/frontend_top.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/shire/minion/frontend \
                $(ORIG_ROOT)/rtl/libs/arbiters \
                $(ORIG_ROOT)/rtl/libs/registers \
                $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs

EXTRA_FLAGS := -DET_ASSERT_OFF

include $(REPO_ROOT)/mk/rtlcosim.mk
