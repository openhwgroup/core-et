BUILD_DIR ?= .

all: sim

DESIGN := cpu_clint
TEST_DIR := $(REPOROOT)/dv/standalone/$(DESIGN)
BUILD_LOG := $(BUILD_DIR)/build.log
SIMV_LOG := $(BUILD_DIR)/simv.log
TB_TOP := $(DESIGN)_tb

TRACE := -kdb -lca -debug_access+all
TRACE += +vcs+fsdbon # verdi -ssf novas.fsdb &
TRACE += +vcs+vcdpluson # dve -full64 -vpd vcdplus.vpd
#TRACE += +vcs+dumpvars
VCS_OPTS := -sverilog -full64 $(DEFINES) $(TRACE) -timescale=1ns/1ps +lint=PCWM

FILE_LIST := $(TEST_DIR)/$(TB_TOP).sv +incdir+$(RTLROOT)/inc +libext+.v+.sv \
			 $(RTLROOT)/cpu_subsystem/$(DESIGN)/*.sv

#$(RTLROOT)/libs/rf_latches/gf_clkmux.v

FILE_LIST_VCS := -f $(RTLROOT)/cpu_subsystem/cpu_clint/cpu_clint_rtl.f $(RTLROOT)/cpu_subsystem/$(DESIGN)/$(DESIGN).sv +libext+.v+.sv

# +incdir+$(RTLROOT)/cpu_subsystem/$(DESIGN)

$(BUILD_DIR)/simv: FORCE
	vcs $(VCS_OPTS) $(TEST_DIR)/$(TB_TOP).sv $(FILE_LIST_VCS) -o $(BUILD_DIR)/simv -Mdir=$(BUILD_DIR)/csrc > $(BUILD_LOG)

sim: $(BUILD_DIR)/simv
	$(BUILD_DIR)/simv | tee $(SIMV_LOG)

FORCE:

# slang version 9.1.214+68cad5ea0
# export PATH="/home/alilic/tools/slang/build/bin:$PATH"
slang:
	slang --top $(TB_TOP) $(FILE_LIST) -Wno-unconnected-port -Wno-duplicate-definition --std 1800-2017 --error-limit=1000

.phony: FORCE clean sim slang

clean:
	rm -rf $(BUILD_DIR)/csrc/ $(BUILD_DIR)/simv $(BUILD_DIR)/simv.daidir/ $(BUILD_DIR)/verdi_config_file $(BUILD_DIR)/DVEfiles/ $(BUILD_DIR)/dve_report* $(BUILD_DIR)/novas* $(BUILD_DIR)/ucli.key $(BUILD_DIR)/vcdplus.vpd $(BUILD_DIR)/novas.fsdb $(BUILD_DIR)/verdiLog/ $(SIMV_LOG) $(BUILD_LOG)
