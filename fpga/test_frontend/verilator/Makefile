REPO_ROOT := $(shell git rev-parse --show-toplevel)

TECH := generic
include $(CURDIR)/../srcs.mk

TB_TOP  := test_frontend
CC_SRCS := $(CURDIR)/test_frontend_test.cc

include $(REPO_ROOT)/mk/verilator.mk
