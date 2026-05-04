# The main source file is generated using the random_sample script
EXTRA_SRC=texture.cc tbox_test.cc 
EXTRA_CFLAGS= -std=gnu++2a 
USE_STDLIB=0

ifndef TBOX_TEST_HARTS
	TBOX_TEST_HARTS=2
endif

EXTRA_CFLAGS+= -DTBOX_HARTS=$(TBOX_TEST_HARTS)	

include $(MINION_DIAGS)/common/include.mk

