REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := vpu_mask
BUILD_DIR  := $(CURDIR)/build

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_mask.sv

ORIG_RTL := $(BUILD_DIR)/vpu_mask_orig.v \
            $(ORIG_ROOT)/rtl/libs/rf_latches/et_clk_gate.v

ORIG_LIBDIRS := $(ORIG_ROOT)/rtl/libs/rf_latches

$(BUILD_DIR)/vpu_mask_orig.v: $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_mask.v
	@mkdir -p $(BUILD_DIR)
	sed -E 's/^module[[:space:]]+vpu_mask/module vpu_mask_orig/' < $< > $@
	echo 's|$(BUILD_DIR)/vpu_mask_orig.v|$(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_mask.v|' >> $(BUILD_DIR)/path_fixup.sed

include $(REPO_ROOT)/mk/rtlcosim.mk
