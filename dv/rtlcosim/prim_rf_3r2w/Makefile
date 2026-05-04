REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := rf_3r2w
NEW_RTL    := $(PRIM_RF_3R2W)
ORIG_RTL   := $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_3r_2w.v

include $(REPO_ROOT)/mk/rtlcosim.mk
