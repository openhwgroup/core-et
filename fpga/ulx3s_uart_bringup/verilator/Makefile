# ulx3s_uart_bringup — Verilator simulation head

REPO_ROOT := $(shell git rev-parse --show-toplevel)

TB_TOP   := ulx3s_uart_bringup
RTL_SRCS := \
  $(REPO_ROOT)/hw/ip/prim_fifo_sync/rtl/prim_fifo_sync.sv \
  $(CURDIR)/../rtl/ulx3s_uart_rx.sv \
  $(CURDIR)/../rtl/ulx3s_uart_tx.sv \
  $(CURDIR)/../rtl/ulx3s_uart_bringup.sv

CC_SRCS := $(CURDIR)/ulx3s_uart_bringup_test.cc

include $(REPO_ROOT)/mk/verilator.mk
