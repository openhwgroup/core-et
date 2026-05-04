REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

TB_TOP   := prim_write_commit_en_tb
RTL_SRCS := $(PRIM_WRITE_COMMIT_EN) \
            $(CURDIR)/prim_write_commit_en_tb.sv
CC_SRCS  := $(CURDIR)/prim_write_commit_en_test.cc

include $(REPO_ROOT)/mk/verilator.mk
