REPO_ROOT := $(shell git rev-parse --show-toplevel)
ESR_DIR   := $(CURDIR)
CSV       := $(ESR_DIR)/data/esr.csv
GEN       := $(ESR_DIR)/scripts/gen_esr.py

# Add targets here as modules are implemented.
MODULES := rbox

.PHONY: all clean $(MODULES)

all: $(MODULES)

rbox: $(REPO_ROOT)/hw/ip/rbox/rtl/rbox_esr.sv

$(REPO_ROOT)/hw/ip/rbox/rtl/rbox_esr.sv: $(CSV) $(GEN)
	python3 $(GEN) --csv $(CSV) --mod rbox --out $@

clean:
	@echo "Generated ESR files must be removed manually from each IP block."
