REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_hv_logic_pwr_ctrl_tdr_isolate

NEW_RTL := $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pwr_ctrl_tdr_isolate.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_pwr_ctrl_tdr_isolate.v

ORIG_RENAME := neigh_hv_logic_pwr_ctrl_tdr_isolate

include $(REPO_ROOT)/mk/rtlcosim.mk
