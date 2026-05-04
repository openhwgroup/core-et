REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_eco_ports
RTL_SRCS := $(PRIM_ECO_PORTS)
CC_SRCS  := $(CURDIR)/prim_eco_ports_test.cc

include $(REPO_ROOT)/mk/verilator.mk
