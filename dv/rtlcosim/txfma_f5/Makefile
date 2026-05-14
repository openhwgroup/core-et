# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_f5

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_norm_shf.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_frac_zero_detect.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f5.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_norm_shf.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_frac_zero_detect.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f5.v
ORIG_RENAME := txfma_frac_zero_detect \
              txfma_norm_shf \
              txfma_f5

include $(REPO_ROOT)/mk/rtlcosim.mk
