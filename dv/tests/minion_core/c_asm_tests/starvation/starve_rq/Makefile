override EXTRA_CFLAGS += -std=c++17 -I $(MINION_DIAGS)/starvation/common/ -ftemplate-depth=25000

MANUAL_DEP=seed.h
EXTRA_CLEAN=clean_seed


include $(MINION_DIAGS)/common/include.mk


.PHONY: clean_seed FORCE

clean_seed:
	rm -f seed.h

seed.h: FORCE
	./get_seed.sh > seed.h


