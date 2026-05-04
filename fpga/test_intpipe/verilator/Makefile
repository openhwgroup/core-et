REPO_ROOT := $(shell git rev-parse --show-toplevel)

TECH := generic
include $(CURDIR)/../srcs.mk

INCDIRS := $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl

TB_TOP  := test_intpipe
CC_SRCS := $(CURDIR)/test_intpipe_test.cc

include $(REPO_ROOT)/mk/verilator.mk
