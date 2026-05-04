ifndef SYSEMU
   $(error "Please execute 'source ./env.sh' or 'source ./env_csh.sh' in the repository root directory before calling make")
endif

cosim_OBJDIR    = $(BUILD_DIR_CSRC_LIB)
libfpu_OBJDIR   = $(BUILD_DIR_CSRC_LIB)
MONITORS_OBJDIR = $(BUILD_DIR_CSRC_LIB)

$(BUILD_DIR_CSRC_LIB):
	mkdir -p $@

ifeq ($(VERILATOR),1)
   VERILATOR_SHARE  = $(VERILATOR_HOME)/share/verilator/
   VERILATED_FLAGS  = -I$(VERILATOR_SHARE)/include -I$(VERILATOR_SHARE)/include/vltstd -DVERILATOR
   EXTRA_CFLAGS    += $(VERILATED_FLAGS)
   EXTRA_MAKE_OPTS += $(VERILATED_FLAGS)
endif

COSIM_UTILS = $(REPOROOT)/dv/arch_monitors

ifeq ($(ENABLE_BEMU),1)
   include $(COSIM_UTILS)/common/cosim.mk
   include $(COSIM_UTILS)/common/monitors.mk
endif

# Tell testBase.cc to use TB_DPI_SCOPE="top" for DPI scope
MONITORS_DEFINES += -DDPI_SV_TOP_SCOPE

# Under Verilator, monitors must compile with -DVERILATOR (so they pick the
# Verilator branches in plusArgs.h / testBase.cc / testLog.cc) plus the
# verilated include paths so <verilated.h> resolves.
ifeq ($(VERILATOR),1)
   MONITORS_DEFINES += $(VERILATED_FLAGS)
endif

EXTRA_LDFLAGS ?=
LDFLAGS = -ldl -lz -rdynamic

ifeq ($(CPP_DEBUG),1)
   EXTRA_CFLAGS  += -O0 -g3 -fsanitize=address -fno-omit-frame-pointer -fno-inline -D_GLIBCXX_DEBUG
   EXTRA_LDFLAGS += -O0 -fsanitize=address
endif

ifeq ($(ENABLE_BEMU),1)
   LDFLAGS += $(COSIM_LDFLAGS) $(EXTRA_LDFLAGS)
endif
