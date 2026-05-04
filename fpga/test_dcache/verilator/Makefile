REPO_ROOT := $(shell git rev-parse --show-toplevel)

TECH := generic
include $(CURDIR)/../srcs.mk

CC_SRCS := $(CURDIR)/test_dcache_test.cc

TESTS := test_dcache

test_dcache_TOP   := test_dcache
test_dcache_SRCS  := $(RTL_SRCS) $(CC_SRCS)
test_dcache_FLAGS := -GClkHz=10000000 \
                     -GBaudRate=115200 \
                     -GDebugMonEn=1 \
                     -GVpuEn=0

include $(REPO_ROOT)/mk/verilator.mk
