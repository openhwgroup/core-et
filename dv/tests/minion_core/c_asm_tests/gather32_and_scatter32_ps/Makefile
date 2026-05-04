MANUAL_DEP=data.raw
EXTRA_CLEAN=diagclean

include $(MINION_DIAGS)/common/include.mk

data.raw:
	dd if=/dev/urandom of=data.raw bs=1M count=1

diagclean:
	rm -f data.raw
