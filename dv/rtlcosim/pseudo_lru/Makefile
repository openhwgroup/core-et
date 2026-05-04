REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := pseudo_lru

NEW_RTL  := $(REPO_ROOT)/hw/ip/pseudo_lru/rtl/pseudo_lru.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/pseudo_lru.v

ORIG_RENAME := pseudo_lru

include $(REPO_ROOT)/mk/rtlcosim.mk
