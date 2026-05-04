REPO_ROOT := $(shell git rev-parse --show-toplevel)

TESTS := rvc \
         intpipe_decode_default \
         intpipe_decode_extra_trans \
         vpu_decoder_default \
         vpu_decoder_extra_trans

rvc_TOP  := minion_frontend_rvc_expander
rvc_SRCS := $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_rvc_expander.sv \
            $(CURDIR)/minion_frontend_rvc_expander_test.cc

intpipe_decode_default_TOP  := intpipe_decode_tb
intpipe_decode_default_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
                               $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
                               $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
                               $(REPO_ROOT)/hw/ip/minion/frontend/rtl/intpipe_decode.sv \
                               $(CURDIR)/intpipe_decode_tb.sv \
                               $(CURDIR)/intpipe_decode_test.cc

intpipe_decode_extra_trans_TOP   := intpipe_decode_tb
intpipe_decode_extra_trans_SRCS  := $(intpipe_decode_default_SRCS)
intpipe_decode_extra_trans_FLAGS := -GEnableExtraTrans=1

vpu_decoder_default_TOP  := vpu_decoder_tb
vpu_decoder_default_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
                            $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
                            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
                            $(REPO_ROOT)/hw/ip/minion/frontend/rtl/vpu_decoder.sv \
                            $(CURDIR)/vpu_decoder_tb.sv \
                            $(CURDIR)/vpu_decoder_test.cc

vpu_decoder_extra_trans_TOP   := vpu_decoder_tb
vpu_decoder_extra_trans_SRCS  := $(vpu_decoder_default_SRCS)
vpu_decoder_extra_trans_FLAGS := -GEnableExtraTrans=1

LINT_SRCS := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
             $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
             $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_pkg.sv \
             $(REPO_ROOT)/hw/ip/minion/frontend/rtl/minion_frontend_rvc_expander.sv \
             $(REPO_ROOT)/hw/ip/minion/frontend/rtl/intpipe_decode.sv \
             $(REPO_ROOT)/hw/ip/minion/frontend/rtl/vpu_decoder.sv
LINT_FLAGS := -Wno-MULTITOP

include $(REPO_ROOT)/mk/verilator.mk
