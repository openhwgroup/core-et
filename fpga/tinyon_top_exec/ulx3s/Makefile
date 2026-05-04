# tinyon_top_exec — ULX3S / ECP5 synthesis head

REPO_ROOT := $(shell git rev-parse --show-toplevel)

TECH := ecp5
include $(CURDIR)/../srcs.mk

SYNTH_TOP := tinyon_top_exec_ulx3s
ECP5_SYNTH_FLAGS ?= -noccu2
YOSYS_SYNTH_PASS := synth_ecp5 $(ECP5_SYNTH_FLAGS)
YOSYS_SLANG_FLAGS += -I$(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl
RTL_SRCS  += $(CURDIR)/tinyon_top_exec_ulx3s_pll.sv
RTL_SRCS  += $(CURDIR)/tinyon_top_exec_ulx3s.sv

NPROC           ?= $(shell getconf _NPROCESSORS_ONLN 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 1)
NTHREADS        := $(shell n=$$(( $(NPROC) / 2 )); if [ $$n -gt 1 ]; then echo $$n; else echo 1; fi)
NEXTPNR         ?= nextpnr-ecp5 --ignore-loops --threads $(NTHREADS) --parallel-refine --router router2 --seed 42
ECPPACK         ?= ecppack
OPENFPGALOADER  ?= openFPGALoader
LPF             := $(CURDIR)/ulx3s_v20.lpf
FREQ_MHZ        ?= 10
XDG_ROOT        := $(CURDIR)/build/xdg
XDG_CONFIG_HOME := $(XDG_ROOT)/config
XDG_CACHE_HOME  := $(XDG_ROOT)/cache
XDG_DATA_HOME   := $(XDG_ROOT)/data
CFG             = $(SYNTH_BUILD)/$(SYNTH_TOP).config
BIT             = $(SYNTH_BUILD)/$(SYNTH_TOP).bit
MAP_CFG         = $(SYNTH_BUILD)/$(SYNTH_TOP).map.config
PLACED_SVG      = $(SYNTH_BUILD)/$(SYNTH_TOP).placed.svg
ROUTED_SVG      = $(SYNTH_BUILD)/$(SYNTH_TOP).routed.svg
MAP_LOG         = $(SYNTH_BUILD)/$(SYNTH_TOP).map.log

include $(REPO_ROOT)/mk/yosys_ecp5.mk

.PHONY: pnr bit prog map

pnr: $(CFG)

bit: $(BIT)

time: $(CFG)
		-HOME=$(XDG_ROOT) \
	  XDG_CONFIG_HOME=$(XDG_CONFIG_HOME) \
	  XDG_CACHE_HOME=$(XDG_CACHE_HOME) \
	  XDG_DATA_HOME=$(XDG_DATA_HOME) \
	  $(NEXTPNR) \
	    --85k \
	    --package CABGA381 \
	    --freq $(FREQ_MHZ) \
	    --json $(JSON) \
	    --lpf $(LPF) \
	    --textcfg $(MAP_CFG) \
	    > $(MAP_LOG) 2>&1

map:
	@test -f $(JSON) || { echo "missing synth JSON: $(JSON)" >&2; exit 1; }
	mkdir -p $(XDG_CONFIG_HOME) $(XDG_CACHE_HOME) $(XDG_DATA_HOME)
	rm -f $(PLACED_SVG) $(ROUTED_SVG) $(MAP_CFG) $(MAP_LOG)
	-HOME=$(XDG_ROOT) \
	  XDG_CONFIG_HOME=$(XDG_CONFIG_HOME) \
	  XDG_CACHE_HOME=$(XDG_CACHE_HOME) \
	  XDG_DATA_HOME=$(XDG_DATA_HOME) \
	  $(NEXTPNR) \
	    --85k \
	    --package CABGA381 \
	    --freq $(FREQ_MHZ) \
	    --json $(JSON) \
	    --lpf $(LPF) \
	    --textcfg $(MAP_CFG) \
	    > $(MAP_LOG) 2>&1
	@test -f $(PLACED_SVG) || test -f $(ROUTED_SVG)
	@echo "map log: $(MAP_LOG)"
	@if test -f $(PLACED_SVG); then echo "placed svg: $(PLACED_SVG)"; fi
	@if test -f $(ROUTED_SVG); then echo "routed svg: $(ROUTED_SVG)"; fi

prog:  $(BIT)
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
          --placed-svg $(PLACED_SVG) \
          --routed-svg $(ROUTED_SVG) \
	  --textcfg $(CFG)

$(BIT): $(CFG)
	$(ECPPACK) --compress $< $@
