REPO_ROOT  := $(shell git rev-parse --show-toplevel)
ALL_DIRS   := $(sort $(dir $(wildcard */Makefile)))
COSIM_DIRS := $(ALL_DIRS)

.PHONY: test coverage-report coverage-html clean

test:
	@pass=0; fail=0; \
	for d in $(COSIM_DIRS); do \
	  echo ""; \
	  $(MAKE) --no-print-directory -C $$d test && pass=$$((pass+1)) || fail=$$((fail+1)); \
	done; \
	echo ""; \
	echo "========================================"; \
	echo "  cosim: $$pass passed, $$fail failed"; \
	echo "========================================"; \
	[ $$fail -eq 0 ]

coverage-report:
	@mkdir -p build/coverage
	@dats=$$(find $(COSIM_DIRS) -path '*/build/obj/coverage.dat' 2>/dev/null); \
	if [ -z "$$dats" ]; then \
	  echo "no coverage data (run 'make test' first)"; \
	  exit 1; \
	fi; \
	for type in line branch toggle; do \
	  verilator_coverage --filter-type $$type --write-info \
	    build/coverage/coverage_$${type}_cosim.info $$dats 2>/dev/null; \
	  for fix in $$(find $(COSIM_DIRS) -name 'path_fixup.sed' 2>/dev/null); do \
	    sed -i.bak -f "$$fix" build/coverage/coverage_$${type}_cosim.info; \
	    rm -f build/coverage/coverage_$${type}_cosim.info.bak; \
	  done; \
	  sed -i.bak \
	    -e "s|^SF:$(REPO_ROOT)/hw/ip/|SF:new/|" \
	    -e "s|^SF:$(REPO_ROOT)/|SF:new/|" \
	    -e '/^SF:new\//!{ /^SF:\//s|^SF:/.*[/]rtl/|SF:orig/rtl/|; }' \
	    -e '/^SF:cosim_/,/^end_of_record/d' \
	    build/coverage/coverage_$${type}_cosim.info; \
	  rm -f build/coverage/coverage_$${type}_cosim.info.bak; \
	done; \
	tmp=$$(mktemp -d); \
	for type in line branch toggle; do \
	  printf "  %-10s" "$$type:"; \
	  verilator_coverage --filter-type $$type --annotate $$tmp $$dats 2>/dev/null \
	    | sed 's/^Total coverage //' | head -1; \
	  rm -rf $$tmp/*; \
	done; \
	rm -rf $$tmp; \
	echo "lcov info files: build/coverage/"

coverage-html: coverage-report
	@mkdir -p build/coverview/dist
	@cp $(REPO_ROOT)/dv/coverview/index.html build/coverview/dist/index.html
	@rm -f build/coverview/dist/index.html.bak
	@cp $(REPO_ROOT)/dv/coverview/config_cosim.json build/coverview/config.json
	@cp build/coverage/coverage_*.info build/coverview/
	@cd build/coverview && python3 $(REPO_ROOT)/dv/coverview/embed.py --inject-data .
	@mv build/coverview/dist/index.html build/coverview/index.html
	@echo "coverage dashboard: build/coverview/index.html"

clean:
	@for d in $(ALL_DIRS); do \
	  $(MAKE) --no-print-directory -C $$d clean; \
	done
	@rm -rf build
