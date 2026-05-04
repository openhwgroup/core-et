
NUM_VECTORS=100
NUM_INSTRUCTIONS=10000
INST_FILE= instr.S
DATA_FILE= data.c

EXTRA_SRC= $(INST_FILE) $(DATA_FILE)
MANUAL_DEP = gen_tests
EXTRA_CFLAGS = -DINST_FILE="$(INST_FILE)"
EXTRA_CLEAN=extra_clean
include $(MINION_DIAGS)/common/include.mk

gen_tests: $(INST_FILE) $(DATA_FILE)

$(INST_FILE):
	./generate_instruction_test.py -o $@ -n $(NUM_INSTRUCTIONS)
$(DATA_FILE):
	./generate_test_data_graphics.py -o $@ -n $(NUM_VECTORS)
extra_clean:
	rm -f $(INST_FILE) $(DATA_FILE)

.PHONY: gen_tests clean

