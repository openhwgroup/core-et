# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := txfma_e2

NEW_RTL := $(REPO_ROOT)/hw/ip/dft/rtl/dft_pkg.sv \
           $(REPO_ROOT)/hw/ip/etlink/rtl/etlink_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/dcache/rtl/minion_dcache_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_defs_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_csa.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_adder.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_ediff_opdorder_logic.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/txfma_e2.sv

ORIG_RTL := $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_csa.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_adder.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_ediff_opdorder_logic.v \
            $(ORIG_ROOT)/rtl/shire/minion/vpu/txfma_7s/txfma_e2.v
ORIG_RENAME := txfma_ediff_opdorder_logic txfma_e2 txfma_adder txfma_csa

include $(REPO_ROOT)/mk/rtlcosim.mk
