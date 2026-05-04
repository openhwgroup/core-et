MANUAL_DEP=data.raw
EXTRA_CLEAN=diagclean
EXTRA_SRC=data.S
EXTRA_CFLAGS=-I$(RTLROOT)/shire/esr/scripts/

include $(MINION_DIAGS)/common/include.mk

data.raw:
	dd if=/dev/urandom of=data.raw bs=128K count=1

diagclean:
	rm -f data.raw
