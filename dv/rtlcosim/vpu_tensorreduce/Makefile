REPO_ROOT  := $(shell git rev-parse --show-toplevel)

COSIM_NAME := vpu_tensorreduce

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorreduce.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorreduce.v
ORIG_RENAME := vpu_tensorreduce

include $(REPO_ROOT)/mk/rtlcosim.mk
