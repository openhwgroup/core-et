MANUAL_DEP=data.raw
EXTRA_CLEAN=diagclean
EXTRA_CFLAGS=-Wl,-section-start=.address_array=0x0000008000200000
EXTRA_CFLAGS=-Wl,-section-start=.rand_init_array=0x0000008000400000

include $(MINION_DIAGS)/common/include.mk

data.raw:
	$(TESTROOT)/scripts/genRandomFile.pl data.raw 1M

data2.raw:
	$(TESTROOT)/scripts/genRandomFile.pl data2.raw 1M

diagclean:
	rm -f data.raw

