REPO_ROOT  := $(shell git rev-parse --show-toplevel)

COSIM_NAME := prim_hot2bin

NEW_RTL  := $(REPO_ROOT)/hw/ip/prim_hot2bin/rtl/prim_hot2bin.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/arbiters/hot2bin.v \
            $(ORIG_ROOT)/rtl/libs/arbiters/onehot_mux.v

include $(REPO_ROOT)/mk/rtlcosim.mk
