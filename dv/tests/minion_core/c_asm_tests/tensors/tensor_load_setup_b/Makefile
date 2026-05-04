MANUAL_DEP=data.raw
EXTRA_CLEAN=diagclean

# This test needs at least 24KB of stack space: make it 32KB (default = 16KB)
STACK_SIZE_LOG2=15

include $(MINION_DIAGS)/common/include.mk
tag: $(MINION_BIN)

RAW_DATA_SIZE = 1M
data.raw:
	$(TESTROOT)/scripts/genRandomFile.pl data.raw ${RAW_DATA_SIZE}

diagclean:
	rm -f data.raw
