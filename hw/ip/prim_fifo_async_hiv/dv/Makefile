REPO_ROOT := $(shell git rev-parse --show-toplevel)

include $(REPO_ROOT)/mk/prim.mk

TESTS := hiv lov

hiv_TOP  := prim_fifo_async_hiv
hiv_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(PRIM_FIFO_ASYNC_HIV) \
            $(CURDIR)/prim_fifo_async_test.cc

lov_TOP  := prim_fifo_async_lov
lov_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(PRIM_FIFO_ASYNC_LOV) \
            $(CURDIR)/prim_fifo_async_lov_test.cc

LINT_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
             $(PRIM_FIFO_ASYNC_HIV) \
             $(PRIM_FIFO_ASYNC_LOV)
LINT_FLAGS := -Wno-MULTITOP

include $(REPO_ROOT)/mk/verilator.mk
