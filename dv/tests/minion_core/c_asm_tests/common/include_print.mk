USE_STDLIB=1

EXTRA_SRC +=$(MINION_DIAGS)/common/print_dbg.c


EXTRA_INCLUDE += -I$(REPOROOT)/dv/common/sw/ip/inc
EXTRA_INCLUDE += -I$(REPOROOT)/dv/tests/ioshire/sw/inc

EXTRA_PRINT_DBG_CFLAGS =  -Wno-error=write-strings
EXTRA_PRINT_DBG_CFLAGS += -Wno-error=strict-aliasing 
EXTRA_PRINT_DBG_CFLAGS += -Wno-error=maybe-uninitialized
EXTRA_PRINT_DBG_CFLAGS += -Wno-error=unused-local-typedefs
EXTRA_PRINT_DBG_CFLAGS += -l c_nano -l nosys


