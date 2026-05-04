# test_intpipe — ULX3S / ECP5 synthesis head

REPO_ROOT := $(shell git rev-parse --show-toplevel)

TECH := ecp5
include $(CURDIR)/../srcs.mk

SYNTH_TOP := test_intpipe_ulx3s
YOSYS_SYNTH_PASS := synth_ecp5
INCDIRS   := $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl
RTL_SRCS  += $(CURDIR)/test_intpipe_ulx3s_pll.sv
RTL_SRCS  += $(CURDIR)/test_intpipe_ulx3s.sv

FREQ_MHZ        ?= 10
NEXTPNR         ?= nextpnr-ecp5 --ignore-loops
ECPPACK         ?= ecppack
OPENFPGALOADER  ?= openFPGALoader
LPF             := $(CURDIR)/ulx3s_v20.lpf
XDG_ROOT        := $(CURDIR)/build/xdg
XDG_CONFIG_HOME := $(XDG_ROOT)/config
XDG_CACHE_HOME  := $(XDG_ROOT)/cache
XDG_DATA_HOME   := $(XDG_ROOT)/data
CFG             = $(SYNTH_BUILD)/$(SYNTH_TOP).config
BIT             = $(SYNTH_BUILD)/$(SYNTH_TOP).bit

include $(REPO_ROOT)/mk/yosys_ecp5.mk

.PHONY: pnr bit prog

pnr: $(CFG)

bit: $(BIT)

prog: $(BIT)
	$(OPENFPGALOADER) -b ulx3s $<

$(CFG): $(JSON) $(LPF)
	mkdir -p $(XDG_CONFIG_HOME) $(XDG_CACHE_HOME) $(XDG_DATA_HOME)
	HOME=$(XDG_ROOT) \
	XDG_CONFIG_HOME=$(XDG_CONFIG_HOME) \
	XDG_CACHE_HOME=$(XDG_CACHE_HOME) \
	XDG_DATA_HOME=$(XDG_DATA_HOME) \
	$(NEXTPNR) \
	  --85k \
	  --package CABGA381 \
	  --freq $(FREQ_MHZ) \
	  --json $(JSON) \
	  --lpf $(LPF) \
	  --textcfg $(CFG)

$(BIT): $(CFG)
	$(ECPPACK) --compress $< $@
