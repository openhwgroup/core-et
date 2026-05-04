ifndef REPOROOT
    $(error "Please execute 'source ./env.sh' or 'source ./env_csh.sh' in the repository root directory before calling make")
endif

USE_ZEBU=1

all: replay

COSIM = $(REPOROOT)/dv/cosim
cosim_OBJDIR ?= $(COSIM)/replay/build
cosim_LIBDIR ?= $(cosim_OBJDIR)
include $(COSIM)/Makefile

CXXFLAGS += -DCOSIM_REPLAY

replay_TARGET = $(cosim_LIBDIR)/run-replay

testlib_SRCS = testBase.cc testLog.cc
testlib_FLAGS = -Wno-error=unused-parameter -Wno-error=unused-variable

replay_SRCS = main.cc
replay_OBJS = $(patsubst %.cc, $(cosim_OBJDIR)/replay/%.o, $(replay_SRCS))

replay_OBJS += $(patsubst %.cc, $(cosim_OBJDIR)/arch_monitors/libs/%.o, $(testlib_SRCS))
replay_DEPS = $(replay_OBJS:%.o=%.d)
replay_LIBS = -Wl,-rpath=$(cosim_LIBDIR) -lcosim -Wl,-rpath=$(EXTRA_CPP_LIBS)/lz4/lib -llz4 -L$(cosim_LIBDIR)

$(cosim_OBJDIR)/arch_monitors/libs/%.o: $(COSIM_UTILS)/libs/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(testlib_FLAGS) -MMD -MP -MT '$$(cosim_OBJDIR)/$(@:$(cosim_OBJDIR)/%=%) $$(cosim_LIBDIR)/$(@:$(cosim_LIBDIR)/%=%)' -c $< -o $@

$(cosim_OBJDIR)/replay/%.o: $(COSIM)/replay/%.cc
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(testlib_FLAGS) -MMD -MP -MT '$$(cosim_OBJDIR)/$(@:$(cosim_OBJDIR)/%=%) $$(cosim_LIBDIR)/$(@:$(cosim_LIBDIR)/%=%)' -c $< -o $@

$(replay_TARGET): $(replay_OBJS) $(cosim_TARGET)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $(replay_OBJS) -L$(cosim_OBJDIR) -o $@ $(replay_LIBS)

.PHONY: replay
replay: $(replay_TARGET)

-include $(replay_DEPS)
