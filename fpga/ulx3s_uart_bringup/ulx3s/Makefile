# ulx3s_uart_bringup — ULX3S / ECP5 synthesis head

REPO_ROOT := $(shell git rev-parse --show-toplevel)

SYNTH_TOP := ulx3s_uart_bringup_ulx3s
SYNTH_CMD := synth_ecp5 -abc9
RTL_SRCS  := \
  $(REPO_ROOT)/hw/ip/prim_fifo_sync/rtl/prim_fifo_sync.sv \
  $(CURDIR)/../rtl/ulx3s_uart_rx.sv \
  $(CURDIR)/../rtl/ulx3s_uart_tx.sv \
  $(CURDIR)/../rtl/ulx3s_uart_bringup.sv \
  $(CURDIR)/ulx3s_uart_bringup_ulx3s.sv

NEXTPNR         ?= nextpnr-ecp5
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
	  --freq 25 \
	  --json $(JSON) \
	  --lpf $(LPF) \
	  --textcfg $(CFG)

$(BIT): $(CFG)
	$(ECPPACK) --compress $< $@
