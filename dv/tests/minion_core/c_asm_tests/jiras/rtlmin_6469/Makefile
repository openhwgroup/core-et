EXTRA_CFLAGS=-std=c++17 --fixed-x31
MANUAL_DEP=data.raw
EXTRA_CLEAN=diagclean

include $(MINION_DIAGS)/common/include.mk

RAW_DATA_SIZE = 1M
data.raw:
	$(TESTROOT)/scripts/genRandomFile.pl data.raw ${RAW_DATA_SIZE}

data2.raw:
	$(TESTROOT)/scripts/genRandomFile.pl data2.raw ${RAW_DATA_SIZE}

diagclean:
	rm -f data.raw
