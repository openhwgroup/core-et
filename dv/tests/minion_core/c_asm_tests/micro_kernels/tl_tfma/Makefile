

MANUAL_DEP=in.raw out.raw gen_random_params
EXTRA_CLEAN=diagclean

include $(MINION_DIAGS)/common/include.mk

in.raw:
	$(TESTROOT)/scripts/genRandomFile.pl in.raw 2M

out.raw:
	$(TESTROOT)/scripts/genRandomFile.pl out.raw 1M

gen_random_params:
	python3 $(REPOROOT)/dv/tests/minion_core/c_asm_tests/micro_kernels/gen_random_params.py --shires 4 --scenario "tload_tfma" --target_dir .
# in order to build  and run with a specific seed, see file tensor_stlf_random_seed in the run directory and paste the see in the python command line above

diagclean:
	rm -f *.raw
