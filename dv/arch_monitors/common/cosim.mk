# Include this from a per-TB Makefile to pull in CoSim build targets.
# Public API: COSIM_DEPENDENCIES, COSIM_LDFLAGS, COSIM_CXXINC, COSIM_EXTRA_CXXFILES.

COSIM             ?= $(REPOROOT)/dv/cosim
MONITORS_CPATH    ?= $(REPOROOT)/dv/arch_monitors/csrc
MAX_PARALLEL_JOBS ?= 1
EXTRA_CPP_LIBS ?= $(COSIM)/src/3rparty
EXTRA_SYSTEM_CPP_LIBS ?= /tools/aifoundry
EVL_BUILD         ?= 0

# These are external files to CoSim:
# Only arch state monitor cc needed
COSIM_EXTRA_CXXFILES := \
	$(MONITORS_CPATH)/minion_monitor.cc 

cosim_OBJDIR ?= $(COSIM)/build

BFDROOT ?= $(RISCV)/x86_64-pc-linux-gnu/riscv64-unknown-elf

CPP_DEBUG ?=0 

 # Add extra include flags

COSIM_DEPENDENCIES += $(cosim_OBJDIR)/libcosim.so
COSIM_LDFLAGS +=-L$(cosim_OBJDIR) -Wl,-rpath=$(cosim_OBJDIR) -lcosim
COSIM_CXXINC += -I$(COSIM)/src \
            -I$(COSIM)/src/checker \
            -I$(COSIM)/src/events \
            -I$(EXTRA_CPP_LIBS)/lz4_stream/\
            -I$(EXTRA_SYSTEM_CPP_LIBS)/lz4/1.8.0/include \
            -I$(EXTRA_CPP_LIBS)/spdlog/1.4.1/include \
            -I$(EXTRA_CPP_LIBS)/cereal/1.3.0/include \
            -I$(EXTRA_CPP_LIBS)/toml11/3.1.0/include \
	    -I$(BFDROOT)/include \
	    -I$(MONITORS_CPATH) \
	    -I$(SYSEMU)

$(cosim_OBJDIR)/libcosim.so: force_cosim
	flock $(REPOROOT)/dv/cosim $(MAKE) -j $(MAX_PARALLEL_JOBS) -C $(COSIM) \
		cosim_OBJDIR=$(cosim_OBJDIR) \
		libfpu_OBJDIR=$(libfpu_OBJDIR) \
		CPP_DEBUG=$(CPP_DEBUG) EXTRA_MAKE_OPTS="$(EXTRA_MAKE_OPTS)" all 2>&1 | tee cosim_compile.log

.PHONY: clean_cosim
clean_cosim: clean_libfpu
	flock $(REPOROOT)/dv/cosim $(MAKE) -C $(COSIM) clean
	@-rm -f cosim_compile.log

.PHONY: force_cosim
force_cosim:
