# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_f4

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_adder.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_lxd.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_trz2.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_f4.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_adder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_lxd.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_trz2.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_f4.v
ORIG_RENAME := txfma_adder \
              txfma_trz2 \
              txfma_lxd \
              txfma_f4

include $(REPO_ROOT)/mk/rtlcosim.mk
