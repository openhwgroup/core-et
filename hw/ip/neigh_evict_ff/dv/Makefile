REPO_ROOT := $(shell git rev-parse --show-toplevel)

COMMON_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
              $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
              $(REPO_ROOT)/hw/ip/neigh_evict_ff/rtl/neigh_evict_ff_unit.sv

TESTS := unit wrapper

unit_TOP := neigh_evict_ff_unit_tb
unit_SRCS := $(COMMON_RTL) \
             $(CURDIR)/neigh_evict_ff_unit_tb.sv \
             $(CURDIR)/neigh_evict_ff_unit_test.cc

wrapper_TOP := neigh_evict_ff_tb
wrapper_SRCS := $(COMMON_RTL) \
                $(REPO_ROOT)/hw/ip/neigh_evict_ff/rtl/neigh_evict_ff.sv \
                $(CURDIR)/neigh_evict_ff_tb.sv \
                $(CURDIR)/neigh_evict_ff_test.cc

include $(REPO_ROOT)/mk/verilator.mk
