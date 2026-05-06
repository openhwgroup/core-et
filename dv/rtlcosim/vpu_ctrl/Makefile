REPO_ROOT  := $(shell git rev-parse --show-toplevel)
include $(REPO_ROOT)/mk/prim.mk

COSIM_NAME := vpu_ctrl

COMMON_NEW := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
              $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
              $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv

NEW_RTL := $(COMMON_NEW) \
           $(PRIM_CLK_GATE) \
           $(PRIM_RF_1R1W_PREVIEW) \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorfma.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorreduce.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorquant.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ml.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_trans.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_uinst_decoder.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensora_rf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensortmp_rf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_mask.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ctrl.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_ctrl.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_ml.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorfma.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorreduce.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorquant.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_trans.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_uinst_decoder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensora_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensortmp_rf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_mask.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches \
                $(ORIG_ROOT)/rtl/libs/clock_gates \
                $(ORIG_ROOT)/rtl/libs/registers

ORIG_RENAME := vpu_uinst_decoder \
               vpu_tensorreduce \
               vpu_tensorquant \
               vpu_tensorfma \
               vpu_tensortmp_rf \
               vpu_tensora_rf \
               vpu_trans \
               vpu_mask \
               vpu_ctrl \
               vpu_ml

include $(REPO_ROOT)/mk/rtlcosim.mk
