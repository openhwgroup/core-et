REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := bpam2minions

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/bpam2minions.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/bpam2minions.v

ORIG_RENAME := bpam2minions

# Original soc.vh pulls broad unrelated type headers that trigger width and
# reserved-word warnings even though this leaf only uses debug APB macros.
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
