# Common Yosys FPGA synthesis rules.
#
# Reads SystemVerilog directly through the Yosys slang frontend, then runs
# Yosys synthesis.
#
# Set these variables before including:
#   SYNTH_TOP  - Top module name for synthesis
#   RTL_SRCS   - RTL source files (SystemVerilog, packages first)
#   YOSYS_SYNTH_PASS - Yosys synthesis pass string
#
# Optional:
#   CHPARAMS   - Yosys chparam arguments (e.g. "-set Width 16 -set Depth 8")
#   SYNTH_LABEL - Short user-facing label for reports
#   YOSYS_PRE_READ_CMDS - Target-specific Yosys commands to run before read_slang
#   YOSYS_SLANG_FLAGS - Additional read_slang flags (e.g. include dirs)
#   YOSYS_SLANG_BASE_FLAGS - Default read_slang flags, override only intentionally

REPO_ROOT ?= $(shell git rev-parse --show-toplevel)

YOSYS              ?= yosys
YOSYS_PLUGIN_FLAGS ?= -m slang
YOSYS_SLANG_BASE_FLAGS ?= --relax-enum-conversions
YOSYS_SLANG_FLAGS ?=
YOSYS_SYNTH_PASS  ?= $(SYNTH_CMD)
YOSYS_PRE_READ_CMDS ?=

SYNTH_BUILD := $(CURDIR)/build/synth
JSON        := $(SYNTH_BUILD)/$(SYNTH_TOP).json
RPT         := $(SYNTH_BUILD)/$(SYNTH_TOP).rpt

# ── Yosys script ────────────────────────────────────────

YOSYS_PARAM := $(if $(CHPARAMS),chparam $(CHPARAMS) $(SYNTH_TOP); ,)
YOSYS_READ_CMD := $(YOSYS_PRE_READ_CMDS) read_slang $(YOSYS_SLANG_BASE_FLAGS) $(YOSYS_SLANG_FLAGS) --top $(SYNTH_TOP) $(RTL_SRCS)
YOSYS_CMD := $(YOSYS_READ_CMD); $(YOSYS_PARAM) $(YOSYS_SYNTH_PASS) -top $(SYNTH_TOP) -json $(JSON)
SYNTH_LABEL ?= $(YOSYS_SYNTH_PASS)

# ── Targets ─────────────────────────────────────────────

.PHONY: synth report synth-clean

synth: $(JSON)

report: $(RPT)
	@echo "──── Resource report: $(SYNTH_TOP) [$(SYNTH_LABEL)] ────"
	@python3 $(REPO_ROOT)/mk/yosys_report.py $(RPT) $(SYNTH_TOP) || echo "(no report — run 'make synth' first)"

$(JSON): $(RTL_SRCS)
	@mkdir -p $(SYNTH_BUILD)
	$(YOSYS) $(YOSYS_PLUGIN_FLAGS) -l $(RPT) -p '$(YOSYS_CMD)'

synth-clean:
	rm -rf $(SYNTH_BUILD)
