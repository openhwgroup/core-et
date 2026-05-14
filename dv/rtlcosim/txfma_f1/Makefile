# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_f1

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_4_2_compressor_array.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_norm.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4_msb.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_booth_ppg_32r4.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_wallace1.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_trz.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f1.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_csa.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_4_2_compressor.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_4_2_compressor_array.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4_norm.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4_msb.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_booth_ppg_32r4.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_wallace1.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_trz.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f1.v
ORIG_RENAME := txfma_4_2_compressor_array \
              txfma_booth_ppg_32r4_norm \
              txfma_booth_ppg_32r4_msb \
              txfma_4_2_compressor \
              txfma_booth_ppg_32r4 \
              txfma_wallace1 \
              txfma_trz \
              txfma_csa \
              txfma_f1

include $(REPO_ROOT)/mk/rtlcosim.mk
