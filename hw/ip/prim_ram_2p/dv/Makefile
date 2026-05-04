REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_ram_2p
RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(REPO_ROOT)/hw/ip/ram_cfg/rtl/ram_cfg_pkg.sv \
            $(PRIM_RAM_2P)
CC_SRCS  := $(CURDIR)/prim_ram_2p_test.cc

include $(REPO_ROOT)/mk/verilator.mk
