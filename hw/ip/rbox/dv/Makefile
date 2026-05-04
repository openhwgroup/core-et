REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

RTL_DIR := $(REPO_ROOT)/hw/ip
RB_RTL  := $(RTL_DIR)/rbox/rtl

PKG_RTL := $(RTL_DIR)/dft/rtl/dft_pkg.sv \
           $(RTL_DIR)/ram_cfg/rtl/ram_cfg_pkg.sv \
           $(RTL_DIR)/etlink/rtl/etlink_pkg.sv \
           $(RB_RTL)/rbox_pkg.sv

ARITH_RTL := $(RB_RTL)/rbox_step_mul.sv \
             $(RB_RTL)/rbox_depth_to_fp.sv \
             $(RB_RTL)/rbox_depth_to_un.sv \
             $(RB_RTL)/rbox_sample_to_fp32.sv \
             $(RB_RTL)/rbox_fp_shifter.sv \
             $(RB_RTL)/rbox_fp_fast_adder.sv \
             $(RB_RTL)/rbox_fp_adder_2stg.sv \
             $(RB_RTL)/rbox_sampler.sv \
             $(RB_RTL)/rbox_depth_offset.sv

BUF_RTL := $(PRIM_RAM_1P) \
           $(PRIM_RAM_2P) \
           $(PRIM_FIFO_SRAM) \
           $(RB_RTL)/rbox_input_fifo.sv \
           $(RB_RTL)/rbox_output_buffer.sv

ALL_RTL := $(PKG_RTL) $(ARITH_RTL) $(BUF_RTL)

# Unit tests use a TB wrapper to instantiate the module with specific params
TESTS := step_mul depth_to_fp depth_to_un sample_to_fp32 \
         fp_adder_2stg fp_fast_adder sampler depth_offset \
         input_fifo output_buffer depth_buffer \
         rbox_test traverse ctrl dbg_ram rbox_top

step_mul_TOP  := rbox_step_mul_tb
step_mul_SRCS := $(RB_RTL)/rbox_step_mul.sv \
                 $(CURDIR)/rbox_step_mul_tb.sv \
                 $(CURDIR)/rbox_step_mul_test.cc

depth_to_fp_TOP  := rbox_depth_to_fp_tb
depth_to_fp_SRCS := $(PKG_RTL) \
                    $(RB_RTL)/rbox_depth_to_fp.sv \
                    $(CURDIR)/rbox_depth_to_fp_tb.sv \
                    $(CURDIR)/rbox_depth_to_fp_test.cc

depth_to_un_TOP  := rbox_depth_to_un_tb
depth_to_un_SRCS := $(RB_RTL)/rbox_depth_to_un.sv \
                    $(CURDIR)/rbox_depth_to_un_tb.sv \
                    $(CURDIR)/rbox_depth_to_un_test.cc

sample_to_fp32_TOP  := rbox_sample_to_fp32_tb
sample_to_fp32_SRCS := $(PKG_RTL) \
                       $(RB_RTL)/rbox_sample_to_fp32.sv \
                       $(CURDIR)/rbox_sample_to_fp32_tb.sv \
                       $(CURDIR)/rbox_sample_to_fp32_test.cc

fp_adder_2stg_TOP  := rbox_fp_adder_2stg_tb
fp_adder_2stg_SRCS := $(PKG_RTL) \
                      $(RB_RTL)/rbox_fp_shifter.sv \
                      $(RB_RTL)/rbox_fp_adder_2stg.sv \
                      $(CURDIR)/rbox_fp_adder_2stg_tb.sv \
                      $(CURDIR)/rbox_fp_adder_2stg_test.cc

fp_fast_adder_TOP  := rbox_fp_fast_adder_tb
fp_fast_adder_SRCS := $(PKG_RTL) \
                      $(RB_RTL)/rbox_fp_shifter.sv \
                      $(RB_RTL)/rbox_fp_fast_adder.sv \
                      $(CURDIR)/rbox_fp_fast_adder_tb.sv \
                      $(CURDIR)/rbox_fp_fast_adder_test.cc

sampler_TOP  := rbox_sampler_tb
sampler_SRCS := $(PKG_RTL) \
               $(RB_RTL)/rbox_step_mul.sv \
               $(RB_RTL)/rbox_sampler.sv \
               $(CURDIR)/rbox_sampler_tb.sv \
               $(CURDIR)/rbox_sampler_test.cc

depth_offset_TOP  := rbox_depth_offset_tb
depth_offset_SRCS := $(PKG_RTL) \
                    $(RB_RTL)/rbox_fp_shifter.sv \
                    $(RB_RTL)/rbox_fp_adder_2stg.sv \
                    $(RB_RTL)/rbox_depth_offset.sv \
                    $(CURDIR)/rbox_depth_offset_tb.sv \
                    $(CURDIR)/rbox_depth_offset_test.cc

input_fifo_TOP  := rbox_input_fifo_tb
input_fifo_SRCS := $(PKG_RTL) \
                   $(PRIM_RAM_1P) \
                   $(RB_RTL)/rbox_input_fifo.sv \
                   $(CURDIR)/rbox_input_fifo_tb.sv \
                   $(CURDIR)/rbox_input_fifo_test.cc

output_buffer_TOP   := rbox_output_buffer_tb
output_buffer_SRCS  := $(PKG_RTL) \
                       $(PRIM_RAM_1P) $(PRIM_RAM_2P) $(PRIM_FIFO_SRAM) \
                       $(RB_RTL)/rbox_output_buffer.sv \
                       $(CURDIR)/rbox_output_buffer_tb.sv \
                       $(CURDIR)/rbox_output_buffer_test.cc
output_buffer_FLAGS := -Wno-UNOPTFLAT  # prim_fifo_sram struct fields cause false circular dependency with coverage

depth_buffer_TOP  := rbox_depth_buffer_tb
depth_buffer_SRCS := $(PKG_RTL) \
                     $(PRIM_RAM_2P) \
                     $(RB_RTL)/rbox_depth_buffer.sv \
                     $(CURDIR)/rbox_depth_buffer_tb.sv \
                     $(CURDIR)/rbox_depth_buffer_test.cc

rbox_test_TOP  := rbox_test_tb
rbox_test_SRCS := $(PKG_RTL) \
                  $(PRIM_RAM_2P) $(PRIM_FIFO_SRAM) $(PRIM_RAM_1P) \
                  $(RB_RTL)/rbox_depth_buffer.sv \
                  $(RB_RTL)/rbox_test.sv \
                  $(CURDIR)/rbox_test_tb.sv \
                  $(CURDIR)/rbox_test_test.cc

traverse_TOP  := rbox_traverse_tb
traverse_SRCS := $(PKG_RTL) \
                $(ARITH_RTL) \
                $(RB_RTL)/rbox_traverse.sv \
                $(CURDIR)/rbox_traverse_tb.sv \
                $(CURDIR)/rbox_traverse_test.cc

ctrl_TOP  := rbox_ctrl_tb
ctrl_SRCS := $(ALL_RTL) \
             $(RB_RTL)/rbox_ctrl.sv \
             $(CURDIR)/rbox_ctrl_tb.sv \
             $(CURDIR)/rbox_ctrl_test.cc

dbg_ram_TOP  := rbox_dbg_ram_tb
dbg_ram_SRCS := $(PKG_RTL) \
                $(RTL_DIR)/apb/rtl/apb_pkg.sv \
                $(RB_RTL)/rbox_dbg_ram.sv \
                $(CURDIR)/rbox_dbg_ram_tb.sv \
                $(CURDIR)/rbox_dbg_ram_test.cc

rbox_top_TOP   := rbox_top_tb
rbox_top_FLAGS := -Wno-UNOPTFLAT  # prim_fifo_sram in rbox_output_buffer
rbox_top_SRCS  := $(PKG_RTL) \
                 $(RTL_DIR)/apb/rtl/apb_pkg.sv \
                 $(ARITH_RTL) $(BUF_RTL) \
                 $(PRIM_CLK_GATE) \
                 $(RB_RTL)/rbox_depth_buffer.sv \
                 $(RB_RTL)/rbox_test.sv \
                 $(RB_RTL)/rbox_traverse.sv \
                 $(RB_RTL)/rbox_ctrl.sv \
                 $(RB_RTL)/rbox_esr.sv \
                 $(RB_RTL)/rbox_dbg_ram.sv \
                 $(RB_RTL)/rbox_top.sv \
                 $(CURDIR)/rbox_top_tb.sv \
                 $(CURDIR)/rbox_top_test.cc

LINT_SRCS := $(ALL_RTL)
LINT_FLAGS := -Wno-MULTITOP

include $(REPO_ROOT)/mk/verilator.mk
