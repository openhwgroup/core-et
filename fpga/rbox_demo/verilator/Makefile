# rbox_demo — Verilator RGB triangle demo

include $(CURDIR)/../srcs.mk

TB_TOP   := rbox_demo_top
CC_SRCS  := $(CURDIR)/rbox_demo.cc
RTL_SRCS += $(CURDIR)/rbox_demo_top.sv
SIM_ARGS := +verilator+error+limit+10000

include $(REPO_ROOT)/mk/verilator.mk
