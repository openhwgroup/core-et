# Common Verilator build rules.
#
# === Single-test mode (backward compatible) ===
#
#   Set these variables before including:
#     TB_TOP    - Top module name for Verilator
#     RTL_SRCS  - RTL source files
#     CC_SRCS   - C++ test sources
#     INCDIRS   - Extra include directories (optional)
#     SIM_ARGS  - Runtime arguments (optional)
#
# === Multi-test mode ===
#
#   Set TESTS to a list of test names, then for each <t> in TESTS define:
#     <t>_TOP   - Top module name
#     <t>_SRCS  - All source files (RTL + TB wrappers + C++ test)
#   Optionally:
#     <t>_ARGS  - Runtime arguments
#     <t>_FLAGS - Extra Verilator flags (e.g. -Wno-UNOPTFLAT)
#
#   Also set:
#     LINT_SRCS - RTL files to lint (optional, uses RTL_SRCS if unset)
#     LINT_FLAGS - Extra lint flags (optional, e.g. -Wno-MULTITOP)
#
# Optional randomized-startup test mode:
#   XRAND_FLAGS - Extra Verilator compile flags for test-xrand
#   XRAND_ARGS  - Extra runtime args for test-xrand
#   XRAND_SEED  - Seed used by the default XRAND_ARGS

REPO_ROOT ?= $(shell git rev-parse --show-toplevel)
VERILATOR ?= verilator
VERILATOR_JOBS ?= $(shell procs=$$(getconf _NPROCESSORS_ONLN 2>/dev/null || nproc 2>/dev/null || echo 1); jobs=$$((procs * 4 / 5)); if [ $$jobs -lt 1 ]; then jobs=1; fi; echo $$jobs)
XRAND_FLAGS ?= --x-initial unique
XRAND_SEED  ?= 1
XRAND_ARGS  ?= +verilator+rand+reset+2 +verilator+seed+$(XRAND_SEED)

BUILD_DIR  ?= $(CURDIR)/build

COMMON_FLAGS := \
  -sv --cc --exe --build -j $(VERILATOR_JOBS) \
  -Wall \
  --assert \
  --coverage \
  -CFLAGS "-std=c++17 -I$(REPO_ROOT)/dv/common"

# ── Multi-test mode ─────────────────────────────────────

ifdef TESTS

.PHONY: test test-xrand lint coverage-report clean \
        $(addprefix test-,$(TESTS)) \
        $(addprefix test-xrand-,$(TESTS))

test: $(addprefix test-,$(TESTS))
test-xrand: $(addprefix test-xrand-,$(TESTS))

# Generate build+run rules for each test
define TEST_template
test-$(1): $(BUILD_DIR)/obj_$(1)/V$($(1)_TOP)
	@echo "──── run $($(1)_TOP) ────"
	@$$< $($(1)_ARGS) +verilator+coverage+file+$(BUILD_DIR)/obj_$(1)/coverage.dat
	@if [ -f $(BUILD_DIR)/obj_$(1)/coverage.dat ]; then \
	  verilator_coverage --annotate $(BUILD_DIR)/obj_$(1)/coverage \
	    $(BUILD_DIR)/obj_$(1)/coverage.dat; \
	fi

test-xrand-$(1): $(BUILD_DIR)/xrand_obj_$(1)/V$($(1)_TOP)
	@echo "──── run $($(1)_TOP) [xrand seed=$(XRAND_SEED)] ────"
	@$$< $($(1)_ARGS) $(XRAND_ARGS) +verilator+coverage+file+$(BUILD_DIR)/xrand_obj_$(1)/coverage.dat
	@if [ -f $(BUILD_DIR)/xrand_obj_$(1)/coverage.dat ]; then \
	  verilator_coverage --annotate $(BUILD_DIR)/xrand_obj_$(1)/coverage \
	    $(BUILD_DIR)/xrand_obj_$(1)/coverage.dat; \
	fi

$(BUILD_DIR)/obj_$(1)/V$($(1)_TOP): $($(1)_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(COMMON_FLAGS) $($(1)_FLAGS) \
	  --Mdir $(BUILD_DIR)/obj_$(1) \
	  --top-module $($(1)_TOP) \
	  -o V$($(1)_TOP) \
	  $$^

$(BUILD_DIR)/xrand_obj_$(1)/V$($(1)_TOP): $($(1)_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(COMMON_FLAGS) $(XRAND_FLAGS) $($(1)_FLAGS) \
	  --Mdir $(BUILD_DIR)/xrand_obj_$(1) \
	  --top-module $($(1)_TOP) \
	  -o V$($(1)_TOP) \
	  $$^
endef

$(foreach t,$(TESTS),$(eval $(call TEST_template,$(t))))

coverage-report:
	@for f in $(wildcard $(BUILD_DIR)/obj_*/coverage.dat); do \
	  name=$$(basename $$(dirname $$f) | sed 's/^obj_//'); \
	  printf "  %-40s " "$$name"; \
	  verilator_coverage --annotate $(BUILD_DIR)/coverage_$$name $$f \
	    | sed 's/^Total coverage //' | head -1; \
	done

lint:
ifdef LINT_SRCS
	@echo "──── lint ────"
	@$(VERILATOR) --lint-only -sv -Wall $(LINT_FLAGS) $(LINT_SRCS)
else ifdef RTL_SRCS
	@echo "──── lint ────"
	@$(VERILATOR) --lint-only -sv -Wall $(LINT_FLAGS) $(RTL_SRCS)
endif

clean:
	rm -rf $(BUILD_DIR)

else

# ── Single-test mode (backward compatible) ──────────────

OBJ_DIR    := $(BUILD_DIR)/obj
OBJ_DIR_TR := $(BUILD_DIR)/obj_trace
OBJ_DIR_XR := $(BUILD_DIR)/xrand_obj

.PHONY: test test-xrand test-trace lint coverage-report clean

test: $(OBJ_DIR)/V$(TB_TOP)
	@echo "──── run $(TB_TOP) ────"
	@$< $(SIM_ARGS) +verilator+coverage+file+$(OBJ_DIR)/coverage.dat
	@if [ -f $(OBJ_DIR)/coverage.dat ]; then \
	  verilator_coverage --annotate $(BUILD_DIR)/coverage \
	    $(OBJ_DIR)/coverage.dat; \
	fi

test-xrand: $(OBJ_DIR_XR)/V$(TB_TOP)
	@echo "──── run $(TB_TOP) [xrand seed=$(XRAND_SEED)] ────"
	@$< $(SIM_ARGS) $(XRAND_ARGS) +verilator+coverage+file+$(OBJ_DIR_XR)/coverage.dat
	@if [ -f $(OBJ_DIR_XR)/coverage.dat ]; then \
	  verilator_coverage --annotate $(BUILD_DIR)/xrand_coverage \
	    $(OBJ_DIR_XR)/coverage.dat; \
	fi

test-trace: $(OBJ_DIR_TR)/V$(TB_TOP)
	@echo "──── run $(TB_TOP) [trace] ────"
	@$< $(SIM_ARGS)
	@echo "waveform: sim.vcd"

coverage-report:
	@if [ -f $(OBJ_DIR)/coverage.dat ]; then \
	  printf "  %-40s " "$(TB_TOP)"; \
	  verilator_coverage --annotate $(BUILD_DIR)/coverage \
	    $(OBJ_DIR)/coverage.dat \
	    | sed 's/^Total coverage //' | head -1; \
	fi

lint:
	@echo "──── lint $(TB_TOP) ────"
	@$(VERILATOR) --lint-only -sv -Wall $(LINT_FLAGS) \
	  $(addprefix -I,$(INCDIRS)) \
	  --top-module $(TB_TOP) \
	  $(RTL_SRCS)

$(OBJ_DIR)/V$(TB_TOP): $(RTL_SRCS) $(CC_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(COMMON_FLAGS) \
	  --Mdir $(OBJ_DIR) \
	  $(addprefix -I,$(INCDIRS)) \
	  --top-module $(TB_TOP) \
	  -o V$(TB_TOP) \
	  $^

$(OBJ_DIR_XR)/V$(TB_TOP): $(RTL_SRCS) $(CC_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(COMMON_FLAGS) $(XRAND_FLAGS) \
	  --Mdir $(OBJ_DIR_XR) \
	  $(addprefix -I,$(INCDIRS)) \
	  --top-module $(TB_TOP) \
	  -o V$(TB_TOP) \
	  $^

$(OBJ_DIR_TR)/V$(TB_TOP): $(RTL_SRCS) $(CC_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(COMMON_FLAGS) \
	  --trace \
	  -CFLAGS "-DTRACE" \
	  --Mdir $(OBJ_DIR_TR) \
	  $(addprefix -I,$(INCDIRS)) \
	  --top-module $(TB_TOP) \
	  -o V$(TB_TOP) \
	  $^

clean:
	rm -rf $(BUILD_DIR)

endif
