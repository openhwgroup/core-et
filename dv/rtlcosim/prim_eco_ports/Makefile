REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := prim_eco_ports

NEW_RTL  := $(PRIM_ECO_PORTS)
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/et_eco_ports.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches

include $(REPO_ROOT)/mk/rtlcosim.mk
