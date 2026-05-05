REPO_ROOT  := $(shell git rev-parse --show-toplevel)

COSIM_NAME := vpu_tensorfma

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_tensorfma.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_tensorfma.v
ORIG_RENAME := vpu_tensorfma

include $(REPO_ROOT)/mk/rtlcosim.mk
