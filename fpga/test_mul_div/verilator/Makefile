REPO_ROOT := $(shell git rev-parse --show-toplevel)

TECH := generic
include $(CURDIR)/../srcs.mk

TB_TOP  := test_mul_div
CC_SRCS := $(CURDIR)/test_mul_div_test.cc
BUILD_DIR := $(CURDIR)/build-$(TECH)

include $(REPO_ROOT)/mk/verilator.mk
