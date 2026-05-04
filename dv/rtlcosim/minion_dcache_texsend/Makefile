REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := minion_dcache_texsend

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_texsend.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_texsend.v

EXTRA_FLAGS := -Wno-WIDTH -Wno-SYMRSVDWORD -Wno-COMBDLY

include $(REPO_ROOT)/mk/rtlcosim.mk
