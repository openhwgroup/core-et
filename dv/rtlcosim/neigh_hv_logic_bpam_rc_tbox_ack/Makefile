REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := neigh_hv_logic_bpam_rc_tbox_ack

NEW_RTL := $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/icache_pkg/rtl/icache_geom_pkg.sv \
           $(REPO_ROOT)/hw/ip/shirecache/rtl/shirecache_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_pkg.sv \
           $(REPO_ROOT)/hw/ip/neigh_hv_logic/rtl/neigh_hv_logic_bpam_rc_tbox_ack.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/neigh/neigh_hv_logic_bpam_rc_tbox_ack.v

ORIG_RENAME := neigh_hv_logic_bpam_rc_tbox_ack

include $(REPO_ROOT)/mk/rtlcosim.mk
