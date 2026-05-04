REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk
COSIM_NAME := minion_dcache_replay_queue

NEW_RTL  := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/tlb/rtl/tlb_pkg.sv \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
            $(PRIM_RF_1R1W_PAR_PREVIEW) \
            $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_replay_queue.sv
ORIG_RTL := $(ORIG_ROOT)/rtl/libs/arbiters/queue_arb_lru.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/rf_latch_1r_1w_par.v \
            $(ORIG_ROOT)/rtl/shire/minion/dcache/dcache_replay_queue.v

EXTRA_FLAGS := -Wno-DECLFILENAME -Wno-UNUSED -Wno-WIDTH -Wno-SYMRSVDWORD \
               -Wno-TIMESCALEMOD -Wno-VARHIDDEN -Wno-CASEINCOMPLETE \
               -Wno-COMBDLY -Wno-UNOPTFLAT

include $(REPO_ROOT)/mk/rtlcosim.mk
