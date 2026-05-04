MANUAL_DEP=data.raw
EXTRA_CLEAN=diagclean

include $(MINION_DIAGS)/common/include.mk

RAW_DATA_SIZE = 1M
data.raw:
	dd if=/dev/urandom of=data.raw bs=$(RAW_DATA_SIZE) count=1

diagclean:
	rm -f data.raw
