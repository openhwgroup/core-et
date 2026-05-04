MANUAL_DEP=data.raw
EXTRA_CLEAN=diagclean

include $(MINION_DIAGS)/common/include.mk

data.raw:
	$(TESTROOT)/scripts/genRandomFile.pl data.raw 1M

diagclean:
	rm -f data.raw
