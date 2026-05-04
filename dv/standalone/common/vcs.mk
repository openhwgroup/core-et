ifndef TESTROOT
   $(error "Please execute 'source ./env.sh' or 'source ./env_csh.sh' in the repository root directory before calling make")
endif
ifndef REPOROOT
   $(error "Please execute 'source ./env.sh' or 'source ./env_csh.sh' in the repository root directory before calling make")
endif

#
# Target Directories for the object files
#
BUILD_DIR?=$(CURDIR)/vbuild
BUILD_DIR_CSRC_LIB?=$(BUILD_DIR)/csrc_lib

VCS_BIN=vcs

## Enable Xprop (disable by default)
ENABLE_XPROP?=0
XPROP_CFG?=
XPROP_OPT=$(if $(XPROP_CFG),=$(XPROP_CFG),)
ifeq ($(ENABLE_XPROP), 1)
   XPROP=-xprop$(XPROP_OPT)
endif

# This makefile is not safe to use in context of 'make -j n'
.NOTPARALLEL:

### BEMU
ENABLE_BEMU?=1

# Extra arguments for VCS build phase
VCS_CMPL_OPTS?=
VCS_EXTRA_CMPL_OPTS?=

# Extra defines for VCS in case they need to be provided through make
VCS_EXTRA_DEFINES?=+define+NO_COLOR

EXTRA_CFLAGS += -DDPI_SV_TOP_SCOPE

## Common C++ files
include $(REPOROOT)/dv/standalone/common/common_cc.mk

#
# VCS Compilation
#
ifndef VCS_FILELIST
   $(error "VCS_FILELIST must point at the per-TB run.vcs.f")
endif

# Enables debug (defaults to off; raises compile time)
VCS_DEBUG?=0

ifeq ($(VCS_DEBUG), 1)
   VCS_DBG_OPTS+= -debug_access -debug_region+lib+cell
endif
ifeq ($(VCS_DEBUG), 2)
   VCS_DBG_OPTS+= -debug_access+all -debug_region+lib+cell
endif
ifeq ($(VCS_DEBUG), 3)
   VCS_DBG_OPTS+= -debug_access+r+drivers -debug_region+lib
endif
ifeq ($(VCS_DEBUG), 4)
   VCS_DBG_OPTS+= -debug_access+dmptf+all -debug_region+lib+cell
endif

# DUT top
VCS_TOP?=top

## Option to build Verdi Database for Waveform viewing, add -kdb to enable
BUILD_VERDI_IMAGE?=

# C/C++ Sources and Libraries to statically link with the VCS executable
VCS_CPP_LIBS = -Mlib=$(BUILD_DIR_CSRC_LIB)                                                       \
               -LDFLAGS "$(LDFLAGS)"                                                             \
               -CFLAGS  "$(COSIM_CXXINC) $(MONITORS_CXXINC) $(MONITORS_DEFINES) $(EXTRA_CFLAGS)" \
               $(MONITORS_OBJS)                                                                  \
               $(COSIM_DEPENDENCIES)                                                             \
               $(EXTRA_LINK_OBJ)

VCS_LIB_DEP = $(EXTRA_LINK_OBJ) $(MONITORS_DEPENDENCIES) $(COSIM_DEPENDENCIES)

ifeq ($(ENABLE_BEMU),1)
   VCS_EXTRA_DEFINES+= +define+ENABLE_BEMU +define+ENABLE_BEMU_OR_ZSIM
endif

ifndef VCS_MAIN_CPP
  $(error "VCS_MAIN_CPP must point at the per-TB vcs_main.cpp")
endif
VCS_MAIN = -e vcs_main $(VCS_MAIN_CPP)

VCS_BUILD_CMD=vcs +vcs+lic+wait                                        \
              -full64 -lca $(XPROP)                                    \
              -f $(VCS_FILELIST)                                       \
              -Mdir=$(BUILD_DIR)/csrc                                  \
              $(VCS_DBG_OPTS)                                          \
              $(VCS_EXTRA_DEFINES)                                     \
              $(VCS_CMPL_OPTS)                                         \
              $(VCS_EXTRA_CMPL_OPTS)                                   \
              $(VCS_CPP_LIBS)                                          \
              $(BUILD_VERDI_IMAGE)                                     \
              $(VCS_MAIN)                                              \
              -l $(BUILD_DIR)/compile.log                              \
              -o $(BUILD_DIR)/simv                                     \
              -top $(VCS_TOP)                                          \
              +v2k -ntb_opts uvm-1.2 -sverilog -timescale=1ns/1ps      \
              -assert svaext -reportstats -vpi

SIMV_DEP = $(VCS_LIB_DEP)

#
# Rules
#
.PHONY: VCS clean all run
VCS all: $(BUILD_DIR)/simv

clean:
	cd $(BEMU)/sys_emu; flock . $(MAKE) -C $(BEMU)/sys_emu clean

ifeq ($(ENABLE_BEMU), 1)
clean: clean_cosim
endif

$(BUILD_DIR)/simv: $(SIMV_DEP)
	mkdir -p $(BUILD_DIR)
	$(VCS_BUILD_CMD)

run: $(BUILD_DIR)/simv
	$(PRE_SIMV_CMD) $(BUILD_DIR)/simv $(SIMV_ARGS)
