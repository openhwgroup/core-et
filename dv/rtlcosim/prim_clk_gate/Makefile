REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := clk_gate

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/tech_generic/prim_clk_gate/rtl/prim_clk_gate.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v

include $(REPO_ROOT)/mk/rtlcosim.mk
