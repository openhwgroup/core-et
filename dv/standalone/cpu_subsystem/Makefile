ifndef REPOROOT
    $(error "Please execute 'source ./env.sh' or 'source ./env_csh.sh' in the repository root directory before calling make")
endif

# BEMU
ENABLE_BEMU?=1

# Simulator
VERILATOR?=0

# Enables debug
VCS_DEBUG?=1

XPROP_CFG:=$(REPOROOT)/dv/standalone/cpu_subsystem/xprop.cfg

ifeq ($(VERILATOR),1)
	VERILATOR_FILELIST?=$(REPOROOT)/dv/standalone/cpu_subsystem/run.verilator.f
	VERILATOR_CONTROL_FILE=$(REPOROOT)/dv/standalone/cpu_subsystem/verilator_ctrl.vlt
	VERILATOR_MAIN_CPP=$(REPOROOT)/dv/standalone/cpu_subsystem/csrc/Vtop_main.cpp
	include $(REPOROOT)/dv/standalone/common/verilator.mk
else
	VCS_FILELIST?=$(REPOROOT)/dv/standalone/cpu_subsystem/run.vcs.f
	VCS_MAIN_CPP=$(REPOROOT)/dv/standalone/cpu_subsystem/csrc/vcs_main.cpp
	include $(REPOROOT)/dv/standalone/common/vcs.mk
endif

.PHONY : all


