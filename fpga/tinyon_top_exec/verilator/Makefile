# tinyon_top_exec — Verilator simulation head

REPO_ROOT := $(shell git rev-parse --show-toplevel)

TECH := generic
include $(CURDIR)/../srcs.mk

TB_TOP  := tinyon_top_exec
CC_SRCS := $(CURDIR)/tinyon_top_exec_test.cc
INCDIRS := $(REPO_ROOT)/hw/ip/minion/intpipe/csr/rtl
SIM_ARGS := +verilator+noassert

include $(REPO_ROOT)/mk/verilator.mk
