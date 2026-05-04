ifndef REPOROOT
   $(error "Please execute 'source ./env.sh' or 'source ./env_csh.sh' in the repository root directory before calling make")
endif

ifndef SYSEMU
   $(error "SYSEMU must be defined to point to sysemu source directory")
endif

COSIM = $(REPOROOT)/dv/cosim
BFDROOT = $(RISCV)/x86_64-pc-linux-gnu/riscv64-unknown-elf

# 1st choice: From original BFDROOT ($RISCV/x86_64-pc-linux-gnu/...)
ifneq ($(wildcard $(BFDROOT)/lib/.),)
    BFD_INC  := $(BFDROOT)/include
    BFD_LIBS := -lbfd -lopcodes

# 2nd choice: From custom riscv-gnu-toolchain (aifoundary-org)
else ifneq ($(wildcard $(RISCV_BFD_BUILD)/bfd/.libs/libbfd.a),)
    BFDROOT  := $(RISCV_BFD_BUILD)
    BFD_INC  := $(RISCV_BFD_BUILD)/bfd \
                $(RISCV_GNU_TOOLCHAIN)/binutils/include
    BFD_LIBS := $(RISCV_BFD_BUILD)/opcodes/.libs/libopcodes.a \
                $(RISCV_BFD_BUILD)/bfd/.libs/libbfd.a \
                $(RISCV_BFD_BUILD)/libiberty/libiberty.a \
                $(RISCV_BFD_BUILD)/zlib/libz.a
# 3rd choice: From standard binutils-multiarch and binutils-multiarch-dev packages
else
    BFDROOT  := /usr/lib/x86_64-linux-gnu
    BFD_INC  := /usr/include
    BFD_LIBS := -l:libbfd-multiarch.so -l:libopcodes-multiarch.so
endif

COSIM_UTILS   ?= $(REPOROOT)/dv/arch_monitors

cosim_OBJDIR  ?= $(COSIM)/build
cosim_LIBDIR  ?= $(cosim_OBJDIR)
libfpu_OBJDIR ?= $(cosim_OBJDIR)/fpu
CPP_DEBUG ?= 0
ERBIUM ?= 1

EXTRA_MAKE_OPTS ?=
BFD_EXTRA_LIBS ?=

ifdef GCC_HOME
    CXX = $(GCC_HOME)/bin/g++
    CC  = $(GCC_HOME)/bin/gcc
else
    CXX = g++-13
    CC  = gcc-13
endif

CXXFLAGS = -std=c++17 -g -DFMT_HEADER_ONLY $(EXTRA_MAKE_OPTS)
LDFLAGS  = -Lbuild -L$(BFDROOT)/lib -L$(libfpu_OBJDIR) -L$(EXTRA_SYSTEM_CPP_LIBS)/lz4/1.8.0/lib

AR = ar
ARFLAGS = rvs

# Enable debugging?
ifeq ($(CPP_DEBUG),1)
    CXXFLAGS += -Og -g3 -fsanitize=address -fno-omit-frame-pointer -fno-inline -D_GLIBCXX_DEBUG
    LDFLAGS  += -Og -fsanitize=address
else
    CXXFLAGS += -O2
    LDFLAGS  += -O2
endif

PERF ?= 0
ifeq ($(PERF),1)
    CXXFLAGS += -fno-omit-frame-pointer
endif

# Erbium chip support
ifeq ($(ERBIUM),1)
    CXXFLAGS += -DERBIUM
else
    # Add extra errors
    CXXFLAGS += -Wall -Wextra -Werror -pedantic-errors
endif


EXTRA_CPP_LIBS ?= $(COSIM)/src/3rparty
EXTRA_SYSTEM_CPP_LIBS ?= /tools/aifoundry

# Add extra include flags
CXXFLAGS += -I$(COSIM)/src \
	    -I$(COSIM)/src/checker \
	    -I$(EXTRA_CPP_LIBS)/lz4_stream/\
	    -I$(EXTRA_SYSTEM_CPP_LIBS)/lz4/1.8.0/include \
	    -I$(EXTRA_CPP_LIBS)/spdlog/1.4.1/include \
	    -I$(EXTRA_CPP_LIBS)/cereal/1.3.0/include \
	    -I$(EXTRA_CPP_LIBS)/toml11/3.1.0/include \
	    -I$(SYSEMU) \
	    $(foreach d,$(BFD_INC),-I$(d)) \
	    -I$(COSIM_UTILS)/libs \
	    -I$(VCS_HOME)/include \
	    -I$(cosim_OBJDIR)

# Extra DPI flags
CXXFLAGS += -IDPI_SV_TOP_SCOPE

USE_ZEBU ?= 0
ifeq ($(USE_ZEBU),1)
    CXXFLAGS += -DZEBU_DPI
endif

ifeq ($(MAXION_SA),1)
    CXXFLAGS += -DMAXION_SA
endif

default: all


record_SRCS =	events/cosim_evt.cc \
		events/dispatcher.cc \
		events/recorder.cc \
		events/manual_dpi_impl.cc
cosim_SRCS =	$(record_SRCS) \
		checker/checker.cc \
		checker/log.cc \
		checker/riscv_disasm.cc \
		support/file_utils.cc \
		support/hart_id.cc \
		support/insn_desc.cc \
		support/stats.cc \
		coverage/group.cc \
		coverage/inflight_monitor.cc \
		cosim_error.cc \
		cosim_config.cc \
		cosim.cc

record_OBJS = $(patsubst %.cc, $(cosim_OBJDIR)/%.pic.o, $(record_SRCS))
cosim_OBJS = $(patsubst %.cc, $(cosim_OBJDIR)/%.pic.o, $(cosim_SRCS))

ifeq ($(ERBIUM),1)
    include $(SYSEMU)/emu-er.mk
else
    include $(SYSEMU)/emu-et.mk
endif
include $(SYSEMU)/fpu/fpu.mk
include $(SYSEMU)/softfloat/softfloat.mk

cosim_OBJS += $(patsubst %.cpp, $(cosim_OBJDIR)/sysemu/%.pic.o, $(emu_cpp_srcs))

cosim_DEPS = $(cosim_OBJS:%.o=%.d)

# -lzstd is required for libcosim.so
cosim_LIBS = -Wl,-rpath=$(BFDROOT)/lib $(BFD_LIBS) $(BFD_EXTRA_LIBS) -lz -lzstd -Wl,-rpath=$(EXTRA_SYSTEM_CPP_LIBS)/lz4/1.8.0/lib -llz4

cosim_TARGET = $(cosim_LIBDIR)/libcosim.so
$(cosim_TARGET): $(cosim_OBJS) $(libfpu_OBJDIR)/libfpu.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared $(LDFLAGS) $(cosim_OBJS) -Wl,--whole-archive $(libfpu_OBJDIR)/libfpu.a -Wl,--no-whole-archive -o $@ $(cosim_LIBS)

cosim_AR = $(cosim_LIBDIR)/libcosim.a
$(cosim_AR): $(cosim_OBJS) $(libfpu_OBJDIR)/libfpu.a
	@-/bin/rm -f $@
	@/bin/cp $(libfpu_OBJDIR)/libfpu.a $@
	$(AR) $(ARFLAGS) $@ $(cosim_OBJS)

record_AR = $(cosim_LIBDIR)/librecord.a
$(record_AR): $(record_OBJS)
	@-/bin/rm -f $@
	$(AR) $(ARFLAGS) $@ $(record_OBJS)

# Position independent code (shared libraries)
$(cosim_OBJDIR)/%.pic.o: $(COSIM)/src/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -MT '$$(cosim_OBJDIR)/$(@:$(cosim_OBJDIR)/%=%)' -fPIC -c $< -o $@

# Special rule for sysemu sources
$(cosim_OBJDIR)/sysemu/%.pic.o: $(SYSEMU)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -MT '$$(cosim_OBJDIR)/$(@:$(cosim_OBJDIR)/%=%)' -fPIC -c $< -o $@

$(libfpu_OBJDIR)/libfpu.a: force
	$(MAKE) -C $(SYSEMU)/fpu libfpu_OBJDIR=$(abspath $(libfpu_OBJDIR)) DEBUG=$(CPP_DEBUG) CXX=$(CXX) CC=$(CC) 'EXTRA_MAKE_OPTS=$(EXTRA_MAKE_OPTS)' all

.PHONY: force
force:

.PHONY: all
all: verify-events cosim

.PHONY: cosim
cosim: $(cosim_TARGET) $(cosim_AR) $(record_AR)

-include $(cosim_DEPS)

.PHONY: clean
clean:
	$(RM) -r $(cosim_OBJDIR)

.PHONY: format
format:
	clang-format -i $$(find $(COSIM)/src -name '*.cc' -or -name '*.h') $$(find $(COSIM)/replay -name '*.cc' -or -name '*.h')

.PHONY: events
events:
	$(COSIM)/tools/events.py $(COSIM)/tools/cosim.evt $(COSIM)/tools/templates/template.cc > $(COSIM)/src/events/cosim_evt.cc
	$(COSIM)/tools/events.py $(COSIM)/tools/cosim.evt $(COSIM)/tools/templates/template.h  > $(COSIM)/src/events/cosim_evt.h
	$(COSIM)/tools/events.py $(COSIM)/tools/cosim.evt $(COSIM)/tools/templates/template.vh > $(COSIM)/src/events/cosim_evt.vh
	clang-format -i $(COSIM)/src/events/cosim_evt.cc $(COSIM)/src/events/cosim_evt.h
	$(COSIM)/tools/add-chksum $(COSIM)/src/events/cosim_evt.*

.PHONY: verify-events
verify-events:
	@$(COSIM)/tools/verify-chksum $(COSIM)/src/events/cosim_evt.*

