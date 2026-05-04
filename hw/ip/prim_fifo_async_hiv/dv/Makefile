REPO_ROOT := $(shell git rev-parse --show-toplevel)

include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_fifo_async_hiv
RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(PRIM_FIFO_ASYNC_HIV)
CC_SRCS  := $(CURDIR)/prim_fifo_async_test.cc

include $(REPO_ROOT)/mk/verilator.mk
