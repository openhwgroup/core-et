REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_ch_apb_mux

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/esr/rtl/esr_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/bpam2minions.sv \
           $(REPO_ROOT)/hw/ip/neigh_ch_apb_mux/rtl/neigh_ch_apb_mux.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/bpam2minions.v \
            $(ORIG_ROOT)/rtl/shire/neigh/neigh_ch_apb_mux.v

ORIG_RENAME := neigh_ch_apb_mux bpam2minions

# Original soc.vh pulls broad unrelated type headers that trigger width and
# reserved-word warnings even though this leaf only uses neighborhood APB types.
EXTRA_FLAGS := -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC -Wno-SYMRSVDWORD

include $(REPO_ROOT)/mk/rtlcosim.mk
