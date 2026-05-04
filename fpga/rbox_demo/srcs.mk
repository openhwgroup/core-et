# Shared RTL source list for rbox_demo.
# Reuses the rbox project sources.

REPO_ROOT := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

RTL_DIR := $(REPO_ROOT)/hw/ip
RB_RTL  := $(RTL_DIR)/rbox/rtl

PKGS := $(RTL_DIR)/dft/rtl/dft_pkg.sv \
        $(RTL_DIR)/ram_cfg/rtl/ram_cfg_pkg.sv \
        $(RTL_DIR)/etlink/rtl/etlink_pkg.sv \
        $(RTL_DIR)/apb/rtl/apb_pkg.sv \
        $(RB_RTL)/rbox_pkg.sv

PRIM := $(PRIM_CLK_GATE) \
        $(PRIM_RAM_1P) \
        $(PRIM_RAM_2P) \
        $(PRIM_FIFO_SRAM)

RB_MODS := $(RB_RTL)/rbox_step_mul.sv \
           $(RB_RTL)/rbox_depth_to_fp.sv \
           $(RB_RTL)/rbox_depth_to_un.sv \
           $(RB_RTL)/rbox_sample_to_fp32.sv \
           $(RB_RTL)/rbox_fp_shifter.sv \
           $(RB_RTL)/rbox_fp_fast_adder.sv \
           $(RB_RTL)/rbox_fp_adder_2stg.sv \
           $(RB_RTL)/rbox_sampler.sv \
           $(RB_RTL)/rbox_depth_offset.sv \
           $(RB_RTL)/rbox_input_fifo.sv \
           $(RB_RTL)/rbox_output_buffer.sv \
           $(RB_RTL)/rbox_depth_buffer.sv \
           $(RB_RTL)/rbox_test.sv \
           $(RB_RTL)/rbox_traverse.sv \
           $(RB_RTL)/rbox_ctrl.sv \
           $(RB_RTL)/rbox_esr.sv \
           $(RB_RTL)/rbox_dbg_ram.sv \
           $(RB_RTL)/rbox_top.sv

RTL_SRCS := $(PKGS) $(PRIM) $(RB_MODS)
