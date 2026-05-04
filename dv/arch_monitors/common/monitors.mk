
COSIM_UTILS = $(REPOROOT)/dv/arch_monitors
CSRCS_FOLDER = $(COSIM_UTILS)/csrc
CLIBS_FOLDER = $(COSIM_UTILS)/libs

MONITORS_DEFINES ?=
MONITORS_CFLAGS += -std=c++14 -Os  $(MONITORS_DEFINES)
MONITORS_CXXINC += -I$(VCS_HOME)/include \
          -I$(COSIM_UTILS)/libs \
           $(COSIM_CXXINC) \
          -I$(MONITORS_OBJDIR)
AR = ar
ARFLAGS = rvs

SRCS = $(CSRCS_FOLDER)/minion_monitor.cc \
       $(CSRCS_FOLDER)/dreams_event_monitor.cc \
       $(CSRCS_FOLDER)/testEndMonitor.cc \
       $(CLIBS_FOLDER)/testBase.cc \
       $(CLIBS_FOLDER)/testLog.cc

MONITORS_OBJS = $(patsubst %.cc,$(MONITORS_OBJDIR)/%.o,$(notdir $(SRCS)))

MONITORS_DEPENDENCIES=$(MONITORS_OBJS)

$(MONITORS_OBJDIR)/%.o: $(CSRCS_FOLDER)/%.cc | $(MONITORS_OBJDIR)/vlog_defines.h
	$(CXX) $(MONITORS_CFLAGS) $(MONITORS_CXXINC) -c $< -o $@

$(MONITORS_OBJDIR)/%.o: $(CLIBS_FOLDER)/%.cc | $(MONITORS_OBJDIR)/vlog_defines.h
	$(CXX) $(MONITORS_CFLAGS) $(MONITORS_CXXINC) -c $< -o $@

#$(MONITORS_LIB): $(OBJS)
#	$(AR) $(ARFLAGS) $@ $^

$(MONITORS_OBJDIR)/vlog_defines.h: $(MONITORS_OBJDIR) $(RTLROOT)/inc/shire_defines.vh
	cd $(MONITORS_OBJDIR); $(REPOROOT)/dv/standalone/common/gen_defines.sh
