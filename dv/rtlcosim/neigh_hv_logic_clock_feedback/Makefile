REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_hv_logic_clock_feedback

NEW_RTL := $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_clock_feedback.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_clock_feedback.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_combo_inv.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_combo_buf.v

ORIG_RENAME := neigh_hv_logic_clock_feedback

include $(REPO_ROOT)/mk/rtlcosim.mk
