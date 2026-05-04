TEMPLATE_DEPTH=5000 # hast to be at least NR_TESTS*THREADS

EXTRA_CFLAGS=-std=c++17 -DRAND_TABLES_SEED=0xDEAD -ftemplate-depth=$(TEMPLATE_DEPTH) $(EXTERN_CFLAGS)
include $(MINION_DIAGS)/common/include.mk
