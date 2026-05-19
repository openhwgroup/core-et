# Top-level Makefile
# Run `make test` to build and run all tests with Verilator.

IP_MAKEFILES := $(wildcard hw/ip/*/dv/Makefile) $(wildcard hw/ip/*/*/dv/Makefile)
# Keep top-level test order identical to the CI matrix helper's shell-sorted output.
IP_DIRS      := $(shell printf '%s\n' $(patsubst %/,%,$(dir $(IP_MAKEFILES))) | sort)
COSIM_DIRS   := $(sort $(dir $(wildcard dv/rtlcosim/*/Makefile)))

.PHONY: ci-unit-test-dirs ci-unit-test-matrix test test-xrand cosim-test coverage-report coverage-html update-cosim-coverage lint clean help

ci-unit-test-dirs:
	@for d in $(IP_DIRS); do printf '%s\n' "$${d%/}"; done

ci-unit-test-matrix:
	@python3 -c 'import json,re,sys; print(json.dumps({"include":[{"dv_dir":d,"artifact":"unit-coverage-"+re.sub(r"[^A-Za-z0-9_.-]+","-",d).strip("-")} for d in sys.argv[1:]]},separators=(",",":")))' $(patsubst %/,%,$(IP_DIRS))

test:
	@pass=0; fail=0; \
	for d in $(IP_DIRS); do \
	  echo ""; \
	  $(MAKE) --no-print-directory -C $$d test && pass=$$((pass+1)) || fail=$$((fail+1)); \
	done; \
	echo ""; \
	echo "========================================"; \
	echo "  $$pass passed, $$fail failed"; \
	echo "========================================"; \
	[ $$fail -eq 0 ]

test-xrand:
	@pass=0; fail=0; \
	for d in $(IP_DIRS); do \
	  echo ""; \
	  $(MAKE) --no-print-directory -C $$d test-xrand && pass=$$((pass+1)) || fail=$$((fail+1)); \
	done; \
	echo ""; \
	echo "========================================"; \
	echo "  $$pass passed, $$fail failed [xrand]"; \
	echo "========================================"; \
	[ $$fail -eq 0 ]

cosim-test:
	@pass=0; fail=0; skip=0; \
	for d in $(COSIM_DIRS); do \
	  echo ""; \
	  $(MAKE) --no-print-directory -C $$d test && pass=$$((pass+1)) || { \
	    rc=$$?; \
	    if [ $$rc -eq 2 ]; then skip=$$((skip+1)); else fail=$$((fail+1)); fi; \
	  }; \
	done; \
	echo ""; \
	echo "========================================"; \
	echo "  $$pass passed, $$fail failed, $$skip skipped (cosim)"; \
	echo "========================================"; \
	[ $$fail -eq 0 ]

coverage-report:
	@mkdir -p build/coverage
	@rm -f build/coverage/coverage_*.info
	@unit_dats=$$(find hw/ip -path '*/build/obj*/coverage.dat' ); \
	cosim_dats=$$(find dv/rtlcosim -path '*/build/obj/coverage.dat' ); \
	if [ -z "$$unit_dats" ] && [ -z "$$cosim_dats" ] && ! ls dv/rtlcosim/coverage/coverage_*_cosim.info >/dev/null 2>&1; then \
	  echo "no coverage data (run 'make test' and/or 'make -C dv/rtlcosim test' first)"; \
	  exit 1; \
	fi; \
	repo_root=$$(git rev-parse --show-toplevel); \
	for type in line branch toggle; do \
	  if [ -n "$$unit_dats" ]; then \
	    verilator_coverage --filter-type $$type --write-info \
	      build/coverage/coverage_$${type}_unit.info $$unit_dats  || true; \
	    if [ -f build/coverage/coverage_$${type}_unit.info ]; then \
	      sed -i.bak \
	        -e "s|^SF:$$repo_root/hw/ip/|SF:|" \
	        -e "s|^SF:$$repo_root/|SF:|" \
	        build/coverage/coverage_$${type}_unit.info; \
	      rm -f build/coverage/coverage_$${type}_unit.info.bak; \
	    fi; \
	  fi; \
	  if [ -n "$$cosim_dats" ]; then \
	    verilator_coverage --filter-type $$type --write-info \
	      build/coverage/coverage_$${type}_cosim.info $$cosim_dats  || true; \
	    if [ -f build/coverage/coverage_$${type}_cosim.info ]; then \
	      fixups=$$(find dv/rtlcosim -name 'path_fixup.sed' ); \
	      for fix in $$fixups; do \
	        sed -i.bak -f "$$fix" build/coverage/coverage_$${type}_cosim.info; \
	        rm -f build/coverage/coverage_$${type}_cosim.info.bak; \
	      done; \
	      sed -i.bak \
	        -e "s|^SF:$$repo_root/hw/ip/|SF:new/|" \
	        -e "s|^SF:$$repo_root/|SF:new/|" \
	        -e '/^SF:new\//!{ /^SF:\//s|^SF:/.*[/]rtl/|SF:orig/rtl/|; }' \
	        -e '/^SF:cosim_/,/^end_of_record/d' \
	        build/coverage/coverage_$${type}_cosim.info; \
	      rm -f build/coverage/coverage_$${type}_cosim.info.bak; \
	    fi; \
	  elif [ -f dv/rtlcosim/coverage/coverage_$${type}_cosim.info ]; then \
	    cp dv/rtlcosim/coverage/coverage_$${type}_cosim.info build/coverage/; \
	  fi; \
	done; \
	tmp=$$(mktemp -d); \
	echo "──── unit test coverage ────"; \
	if [ -n "$$unit_dats" ]; then \
	  for type in line branch toggle; do \
	    printf "  %-10s" "$$type:"; \
	    verilator_coverage --filter-type $$type --annotate $$tmp $$unit_dats  \
	      | sed 's/^Total coverage //' | head -1; \
	    rm -rf $$tmp/*; \
	  done; \
	fi; \
	echo "──── cosim coverage ────"; \
	if [ -n "$$cosim_dats" ]; then \
	  for type in line branch toggle; do \
	    printf "  %-10s" "$$type:"; \
	    verilator_coverage --filter-type $$type --annotate $$tmp $$cosim_dats  \
	      | sed 's/^Total coverage //' | head -1; \
	    rm -rf $$tmp/*; \
	  done; \
	elif ls dv/rtlcosim/coverage/coverage_*_cosim.info >/dev/null 2>&1; then \
	  echo "  using checked-in cosim coverage from dv/rtlcosim/coverage/"; \
	  echo "  line:       branch:     toggle:"; \
	fi; \
	rm -rf $$tmp; \
	echo ""; \
	echo "lcov info files: build/coverage/"

coverage-html: coverage-report
	@mkdir -p build/coverview/dist
	@cp dv/coverview/index.html build/coverview/dist/index.html
	@rm -f build/coverview/dist/index.html.bak
	@cp dv/coverview/config.json build/coverview/
	@cp build/coverage/coverage_*_unit.info build/coverview/ ; true
	@if ls build/coverage/coverage_*_cosim.info >/dev/null 2>&1; then \
	  cp build/coverage/coverage_*_cosim.info build/coverview/; \
	elif ls dv/rtlcosim/coverage/coverage_*_cosim.info >/dev/null 2>&1; then \
	  echo "using checked-in cosim coverage from dv/rtlcosim/coverage/"; \
	  cp dv/rtlcosim/coverage/coverage_*_cosim.info build/coverview/; \
	fi
	@cd build/coverview && python3 ../../dv/coverview/embed.py --inject-data .
	@mv build/coverview/dist/index.html build/coverview/index.html
	@echo "coverage dashboard: build/coverview/index.html"

update-cosim-coverage:
	@$(MAKE) --no-print-directory -C dv/rtlcosim coverage-report
	@mkdir -p dv/rtlcosim/coverage
	@cp dv/rtlcosim/build/coverage/coverage_*.info dv/rtlcosim/coverage/
	@echo "updated dv/rtlcosim/coverage/ — commit these files"

lint:
	@pass=0; fail=0; \
	for d in $(IP_DIRS); do \
	  $(MAKE) --no-print-directory -C $$d lint && pass=$$((pass+1)) || fail=$$((fail+1)); \
	done; \
	echo ""; \
	echo "========================================"; \
	echo "  $$pass passed, $$fail failed [lint]"; \
	echo "========================================"; \
	[ $$fail -eq 0 ]

clean:
	@for d in $(IP_DIRS); do \
	  $(MAKE) --no-print-directory -C $$d clean; \
	done
	@for d in $(COSIM_DIRS); do \
	  $(MAKE) --no-print-directory -C $$d clean; \
	done
	@rm -rf build

help:
	@echo "Targets:"
	@echo "  ci-unit-test-dirs    - List auto-discovered IP unit-test DV directories"
	@echo "  ci-unit-test-matrix  - Print GitHub Actions matrix JSON for unit tests"
	@echo "  test                 - Build and run all IP unit tests"
	@echo "  test-xrand           - Build and run all IP unit tests with randomized startup"
	@echo "  cosim-test           - Build and run all RTL co-simulation tests"
	@echo "  coverage-report        - Generate LCOV .info files (line/branch/toggle)"
	@echo "  coverage-html          - Generate interactive coverage dashboard"
	@echo "  update-cosim-coverage  - Regenerate checked-in cosim .info files"
	@echo "  lint            - Lint all RTL (Verilator --lint-only)"
	@echo "  clean           - Remove all build artifacts"
	@echo ""
	@echo "Per-IP:    make -C hw/ip/<block>/dv test"
	@echo "Per-cosim: make -C dv/rtlcosim/<block> test"
