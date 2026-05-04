# Common RTL co-simulation build rules.
#
# Builds a Verilator testbench that dual-instantiates original and new
# modules for cycle-by-cycle output comparison.
#
# Set these variables before including:
#   COSIM_NAME      - Short name (e.g. ecc, fifo_sram, sub_bank)
#   NEW_RTL         - New module source files (.sv)
#   ORIG_RTL        - Original module source files (.v)
#   ORIG_LIBDIRS    - Original RTL library search paths (optional)
#   EXTRA_FLAGS     - Extra Verilator flags (optional, e.g. -DET_ASSERT_OFF)
#   SIM_ARGS        - Extra runtime arguments (optional, e.g. +verilator+noassert)
#   ORIG_RENAME     - List of module names to rename in ORIG_RTL (optional).
#                     Each ORIG_RTL file is copied to build/<name>_orig.v
#                     with all listed module names appended with _orig.
#                     Used when old and new modules share the same name.
#
# Files are found by convention from COSIM_NAME:
#   Testbench:  cosim_$(COSIM_NAME)_tb.sv   (in current directory)
#   C++ test:   cosim_$(COSIM_NAME)_test.cc  (in current directory)
#
# Provides targets: test, test-trace, coverage-report, clean

REPO_ROOT ?= $(shell git rev-parse --show-toplevel)
VERILATOR ?= verilator
VERILATOR_JOBS ?= $(shell procs=$$(getconf _NPROCESSORS_ONLN 2>/dev/null || nproc 2>/dev/null || echo 1); jobs=$$((procs * 4 / 5)); if [ $$jobs -lt 1 ]; then jobs=1; fi; echo $$jobs)
ORIG_ROOT ?= $(REPO_ROOT)/../etcore-soc
TECH ?= generic

include $(REPO_ROOT)/mk/prim.mk

BUILD_DIR  := $(CURDIR)/build
OBJ_DIR    := $(BUILD_DIR)/obj
OBJ_DIR_TR := $(BUILD_DIR)/obj_trace

TB_SV  := cosim_$(COSIM_NAME)_tb.sv
CC_SRC := cosim_$(COSIM_NAME)_test.cc
TB_TOP := cosim_$(COSIM_NAME)_tb

ORIG_INCDIRS := $(ORIG_ROOT)/rtl/inc

COMMON_FLAGS := \
  -sv --cc --exe --build -j $(VERILATOR_JOBS) \
  -Wno-fatal \
  --coverage \
  $(EXTRA_FLAGS) \
  -CFLAGS "-std=c++17 -I$(REPO_ROOT)/dv/common -I$(REPO_ROOT)/dv/rtlcosim/common" \
  $(addprefix +incdir+,$(ORIG_INCDIRS)) \
  $(addprefix -y ,$(ORIG_LIBDIRS))

define add_new_rtl_if_missing
$(if $(filter $(1),$(NEW_RTL_AUTO) $(NEW_RTL)),,$(1))
endef

NEW_RTL_AUTO :=

ifneq ($(filter $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv,$(NEW_RTL)),)
NEW_RTL_AUTO += $(call add_new_rtl_if_missing,$(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv)
endif

NEW_RTL_AUTO += $(call add_new_rtl_if_missing,$(PRIM_WRITE_PREVIEW_EN))
NEW_RTL_AUTO += $(call add_new_rtl_if_missing,$(PRIM_WRITE_COMMIT_EN))
NEW_RTL_AUTO += $(call add_new_rtl_if_missing,$(PRIM_WRITE_COMMIT_RST_EN))

NEW_RTL_FINAL := $(NEW_RTL_AUTO) $(NEW_RTL)

# ── Module renaming ─────────────────────────────────────
#
# When ORIG_RENAME is set, each file in ORIG_RTL is copied to
# build/<basename>_orig.v with all names in ORIG_RENAME appended
# with _orig (longest names first to avoid partial matches).
# The sed expression is built once and applied to every file.

ifdef ORIG_RENAME

# Sort names longest-first so 'intpipe_csr_file_conv' is renamed
# before 'intpipe_csr_file' (avoids partial match).
ORIG_RENAME_SORTED := $(shell echo '$(ORIG_RENAME)' | tr ' ' '\n' | awk '{ print length, $$0 }' | sort -rn | awk '{ print $$2 }')

# Build sed expression: -e '/include/!s/<name>/<name>_orig/g' for each name.
ORIG_RENAME_SED := $(foreach n,$(ORIG_RENAME_SORTED),-e '/include/!s/$(n)/$(n)_orig/g')

# For each ORIG_RTL file, create a renamed copy in BUILD_DIR.
# Also write a path-fixup sed script so .info files can map
# renamed paths back to original source locations.
ORIG_RTL_RENAMED :=
define RENAME_template
ORIG_RTL_RENAMED += $(BUILD_DIR)/$(notdir $(1))

$(BUILD_DIR)/$(notdir $(1)): $(1)
	@mkdir -p $(BUILD_DIR)
	sed $(ORIG_RENAME_SED) < $$< > $$@
	echo 's|$(BUILD_DIR)/$(notdir $(1))|$(1)|' >> $(BUILD_DIR)/path_fixup.sed
endef
$(foreach f,$(ORIG_RTL),$(eval $(call RENAME_template,$(f))))

ORIG_RTL_FINAL := $(ORIG_RTL_RENAMED)
else
ORIG_RTL_FINAL := $(ORIG_RTL)
endif

ALL_SRCS := $(NEW_RTL_FINAL) $(TB_SV) $(ORIG_RTL_FINAL) $(CC_SRC)

.PHONY: test test-trace coverage-report clean check-orig

check-orig:
	@if [ ! -d "$(ORIG_ROOT)/rtl" ]; then \
	  echo "ERROR: ORIG_ROOT not found at $(ORIG_ROOT)"; \
	  echo "Set ORIG_ROOT to point to the etcore-soc repository."; \
	  exit 1; \
	fi

test: check-orig $(OBJ_DIR)/V$(TB_TOP)
	@echo "──── cosim $(COSIM_NAME) ────"
	@$(OBJ_DIR)/V$(TB_TOP) $(SIM_ARGS) +verilator+coverage+file+$(OBJ_DIR)/coverage.dat
	@if [ -f $(OBJ_DIR)/coverage.dat ]; then \
	  verilator_coverage --annotate $(BUILD_DIR)/coverage \
	    $(OBJ_DIR)/coverage.dat 2>/dev/null; \
	fi

test-trace: check-orig $(OBJ_DIR_TR)/V$(TB_TOP)
	@echo "──── cosim $(COSIM_NAME) [trace] ────"
	@$(OBJ_DIR_TR)/V$(TB_TOP)
	@echo "waveform: sim.vcd"

coverage-report:
	@if [ -f $(OBJ_DIR)/coverage.dat ]; then \
	  mkdir -p $(BUILD_DIR)/coverage; \
	  verilator_coverage --annotate $(BUILD_DIR)/coverage \
	    $(OBJ_DIR)/coverage.dat >/dev/null 2>&1; \
	  echo "  $(COSIM_NAME):"; \
	  for f in $(BUILD_DIR)/coverage/*.sv; do \
	    [ -f "$$f" ] || continue; \
	    base=$$(basename "$$f"); \
	    case "$$base" in cosim_*) continue;; esac; \
	    hit=$$(grep -c '^[[:space:]]*[0-9]' "$$f" 2>/dev/null) || hit=0; \
	    miss=$$(grep -c '^%' "$$f" 2>/dev/null) || miss=0; \
	    total=$$((hit + miss)); \
	    if [ $$total -gt 0 ]; then \
	      pct=$$((100 * hit / total)); \
	      printf "    new:  %-42s %3d%%\n" "$$base" "$$pct"; \
	    fi; \
	  done; \
	  for f in $(BUILD_DIR)/coverage/*.v; do \
	    [ -f "$$f" ] || continue; \
	    base=$$(basename "$$f"); \
	    hit=$$(grep -c '^[[:space:]]*[0-9]' "$$f" 2>/dev/null) || hit=0; \
	    miss=$$(grep -c '^%' "$$f" 2>/dev/null) || miss=0; \
	    total=$$((hit + miss)); \
	    if [ $$total -gt 0 ]; then \
	      pct=$$((100 * hit / total)); \
	      printf "    orig: %-42s %3d%%\n" "$$base" "$$pct"; \
	    fi; \
	  done; \
	fi

$(OBJ_DIR)/V$(TB_TOP): $(ALL_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(COMMON_FLAGS) \
	  --Mdir $(OBJ_DIR) \
	  --top-module $(TB_TOP) \
	  -o V$(TB_TOP) \
	  $^

$(OBJ_DIR_TR)/V$(TB_TOP): $(ALL_SRCS)
	@mkdir -p $(BUILD_DIR)
	$(VERILATOR) $(COMMON_FLAGS) --trace -CFLAGS "-DTRACE" \
	  --Mdir $(OBJ_DIR_TR) \
	  --top-module $(TB_TOP) \
	  -o V$(TB_TOP) \
	  $^

clean:
	rm -rf $(BUILD_DIR)
