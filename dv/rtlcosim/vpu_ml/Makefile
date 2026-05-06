REPO_ROOT  := $(shell git rev-parse --show-toplevel)

COSIM_NAME := vpu_ml

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorfma.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorreduce.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorquant.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_ml.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorfma.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorreduce.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorquant.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_ml.v
ORIG_RENAME := vpu_tensorfma vpu_tensorreduce vpu_tensorquant vpu_ml

include $(REPO_ROOT)/mk/rtlcosim.mk
