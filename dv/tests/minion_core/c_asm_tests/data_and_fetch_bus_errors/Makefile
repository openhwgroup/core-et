EXTRA_CFLAGS=-I$(RTLROOT)/shire/esr/scripts/
EXTRA_LDFLAGS=-Wl,--section-start=.end_of_dram=0x87fffffe00
include $(MINION_DIAGS)/common/include.mk

all: check


.PHONY: check
check: bus_err
	nm bus_err |c++filt|./check_fn_sizes.pl
