# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_f2

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor_array.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_wallace2.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_adder.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f2.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_csa.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_4_2_compressor.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_4_2_compressor_array.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_wallace2.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_adder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f2.v
ORIG_RENAME := txfma_4_2_compressor_array \
              txfma_4_2_compressor \
              txfma_wallace2 \
              txfma_adder \
              txfma_csa \
              txfma_f2

include $(REPO_ROOT)/mk/rtlcosim.mk
