REPO_ROOT  := $(shell git rev-parse --show-toplevel)
COSIM_NAME := vpu_bypass
BUILD_DIR  := $(CURDIR)/build

NEW_RTL := $(REPO_ROOT)/hw/ip/minion/rtl/minion_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_pkg.sv \
           $(REPO_ROOT)/hw/ip/minion/vpu/rtl/vpu_bypass.sv

ORIG_RTL := $(BUILD_DIR)/vpu_bypass_orig.v

$(BUILD_DIR)/vpu_bypass_orig.v: $(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_bypass.v
	@mkdir -p $(BUILD_DIR)
	sed -E 's/^module[[:space:]]+vpu_bypass/module vpu_bypass_orig/' < $< > $@
	echo 's|$(BUILD_DIR)/vpu_bypass_orig.v|$(ORIG_ROOT)/rtl/shire/minion/vpu/vpu_bypass.v|' >> $(BUILD_DIR)/path_fixup.sed

include $(REPO_ROOT)/mk/rtlcosim.mk
