REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := sp_apb_mux

NEW_RTL  := $(REPO_ROOT)/hw/ip/sp_apb_mux/rtl/sp_apb_mux.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/standalone_minion/sp_apb_mux.v

ORIG_RENAME := sp_apb_mux

include $(REPO_ROOT)/mk/rtlcosim.mk

