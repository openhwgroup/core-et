MANUAL_DEP=data.raw
EXTRA_CLEAN=diagclean

RAW_DATA_SIZE = 1M

include $(MINION_DIAGS)/common/include.mk

data.raw:
	$(TESTROOT)/scripts/genRandomFile.pl data.raw $(RAW_DATA_SIZE)

diagclean:
	rm -f data.raw
