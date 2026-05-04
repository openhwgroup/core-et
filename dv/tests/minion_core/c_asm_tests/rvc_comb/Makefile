MANUAL_DEP=data.s code.s
EXTRA_CLEAN=autogen_clean

include $(MINION_DIAGS)/common/include.mk

data.s: rvc_comb.py
	./rvc_comb.py -d --seed 0

code.s: rvc_comb.py
	./rvc_comb.py -c --seed 0

autogen_clean:
	rm -rf data.s code.s
