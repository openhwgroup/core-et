REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_rst_sync
RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
            $(PRIM_RST_SYNC)
CC_SRCS  := $(CURDIR)/prim_rst_sync_test.cc

SYNTH_ICE40_TOP      := prim_rst_sync
SYNTH_ICE40_RTL_SRCS := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
                        $(REPO_ROOT)/hw/ip/tech_ice40/prim_rst_sync/rtl/prim_rst_sync.sv
SYNTH_ICE40_BUILD    := $(CURDIR)/build/synth_ice40

.PHONY: synth-ice40 test-synth-ice40 synth-ice40-clean

synth-ice40:
	$(MAKE) --no-print-directory -f $(REPO_ROOT)/mk/syn-ice40.mk \
	  SYNTH_TOP=$(SYNTH_ICE40_TOP) \
	  RTL_SRCS="$(SYNTH_ICE40_RTL_SRCS)" \
	  SYNTH_BUILD=$(SYNTH_ICE40_BUILD) \
	  synth

test-synth-ice40: synth-ice40

include $(REPO_ROOT)/mk/verilator.mk

clean: synth-ice40-clean

synth-ice40-clean:
	$(MAKE) --no-print-directory -f $(REPO_ROOT)/mk/syn-ice40.mk \
	  SYNTH_TOP=$(SYNTH_ICE40_TOP) \
	  RTL_SRCS="$(SYNTH_ICE40_RTL_SRCS)" \
	  SYNTH_BUILD=$(SYNTH_ICE40_BUILD) \
	  synth-clean
