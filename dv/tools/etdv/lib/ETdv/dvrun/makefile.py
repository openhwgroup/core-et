# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Makefile generator.
"""

import glob
import os
import re
import sys
from functools import reduce

from ETdv.dvrun.messaging import (debug, info, warn, early_termination_error)
from ETdv.dvrun.random_gen import etdv_randint
from ETdv.dvrun.status import write_istatus
from ETdv.ostream import OStream
from ETdv.util import (exec, ddmmmyyyy_hhmmss, getenv, flatten,
                       unique, setenv, multi_line_strip, as_list, basename_parts,
                       split_uniq, template_ize, write_file, cross_str, filter_using_regexp,
                       delete_file, read_file)

TIME_IT = '/usr/bin/time -f "metrix: e=%e s=%S u=%U m=%MKB" '


class Makefile:
    def __init__(self, main):
        assert not Makefile.__the_one
        self.__main = main
        self.__configs = None
        self.__outdir = None
        self.__all_builds = []
        self.__all_tests = []
        self.__test_order = []
        self.__tags = []
        self.__tags_exclude = []
        self.__filter_regex = {}  # keys --test_name_enable, ...
        self.__reset_filter_counts()
        self.__after_test_filter = None
        Makefile.__the_one = self

    RUN_ROOTDIR_ENV = 'ETDV_RUNROOT'
    STATUS_NOW_CMD = 'ETDV_STATUS_NOW'

    def __reset_filter_counts(self):
        self.__filter_counts = {
            'test': {'considered': 0, 'valid': 0},
            'build': {'considered': 0, 'valid': 0}
        }
        return self

    def set_after_test_filter(self, callme):
        self.__after_test_filter = callme

    def process(self):
        self.__configs = self.__main.parse_configurations()
        self.__option_tags().__option_filters()
        if self.__outdir is None:
            self.__outdir = self.__main.get_makefile_dir()
        setenv(self.RUN_ROOTDIR_ENV, os.path.realpath(self.__outdir))
        # flatten builds and tests
        self \
            .__flatten() \
            .__get_test_order() \
            .__to_makefile() \
            .__add_killall() \
            .__check_rof_config()

    def __check_rof_config(self):
        self.__main.check_rof_config(self.__all_builds, self.__all_tests)
        return self

    @property
    def options(self):
        return self.__main.options

    @property
    def outdir(self):
        return self.__outdir

    @property
    def slurm(self):
        return self.options['--noslurm'] is None

    @property
    def slurm_reservation(self):
        return self.options['--reservation']

    @property
    def slurm_opts(self):
        return self.options['--slurmopts']

    @property
    def all_tests(self):
        return self.__all_tests

    @staticmethod
    def create(main):
        return Makefile(main)

    @staticmethod
    def the_one():
        return Makefile.__the_one

    # Private
    __the_one = None

    class _Build:
        @property
        def full_name(self):
            return Makefile.get_target_name(self)

        @property
        def name(self):
            return self['name']

        @property
        def config_name(self):
            return self.config['name']

        def __init__(self, parent, build):
            self.config = parent
            self.build = build
            self.rule = None
            self.log = None
            self.rundir = None
            self.is_valid = True

        def __getitem__(self, name):
            """
            Convenience for accessing build items (attributes).
            :param name: attribute name.
            :return: attribute value or None (if attribute does not exist).
            """
            return self.build[name]

    class _Test:
        @property
        def full_name(self):
            return Makefile.get_target_name(self)

        @property
        def name(self):
            return self['name']

        def __init__(self, parent, test):
            self.build = parent
            if test['tags'] is not None: test['tags'] = split_uniq(test['tags'])
            self.test = test
            self.rule = None
            self.log = None

        def __getitem__(self, name):
            """
            Convenience for accessing test items (attributes).
            :param name: attribute name.
            :return: attribute value or None (if attribute does not exist).
            """
            return self.test[name]

    GET_RUN_ID_TEMPLATE = """#!/usr/bin/env python3.6

import sys

sys.path.insert(0, '@libpath@')
from ETdv.dvrun.metrix import create_run
from ETdv.dvrun.util import get_branch_and_bist
from ETdv.util import write_file

project = "@project@"
group = "g=@group@"
detail = '@detail@'
branch, is_bist = get_branch_and_bist()
if is_bist: detail = f'{detail}:is_bist=1'
detail = f'{detail}:branchx={branch}'
rndseed = int('@rndseed@')
ntests = int(sys.argv[1])

run_id = create_run(project=project, brief=group, detail=detail, ntests=ntests, rndseed=rndseed)
if run_id is not None:
    write_file('@ofn@', f'{run_id}')
"""

    def __create_get_run_id(self, fname='.get_run_id'):
        from ETdv.dvrun.random_gen import RandomGen
        if not self.__main.collect_metrix: return None
        opts = self.__main.metrix_opts
        ofn = os.path.join(self.outdir, fname)
        detail = opts['--detail'] + 'runroot=' + os.path.dirname(self.outdir)
        contents = template_ize(self.GET_RUN_ID_TEMPLATE,
                                libpath=os.path.realpath(os.path.join(sys.argv[0], '../../lib')),
                                project=opts['--project'],
                                group=','.join(opts['--group']),
                                detail=detail,
                                rndseed=str(RandomGen.the_one().get_seed()),
                                ofn=os.path.join(self.outdir, '.run_id')
                                )
        write_file(ofn, contents)
        os.chmod(ofn, 0o755)
        return ofn

    def __option_tags(self):
        if self.options['--tags'] is not None:
            self.__tags = split_uniq(as_list(self.options['--tags']))
        if self.options['--tags_exclude'] is not None:
            self.__tags_exclude = split_uniq(as_list(self.options['--tags_exclude']))
        return self

    def __option_filters(self):
        """
        Process name/cmd filters.
        :return: self
        """
        for opt in cross_str(['test', 'build'], cross_str(['name', 'cmd'], ['enable', 'disable'])):
            xopt = f'--{opt}'
            rex = self.options[xopt]
            if rex:
                rex = as_list(rex)
                rex = [re.sub("'", '', rexi) for rexi in rex]  # remove quotes
                rex = split_uniq(rex)
                compiled = []
                for rexi in rex:
                    comp = None
                    try:
                        comp = re.compile(rexi)
                    finally:
                        if comp is None:
                            early_termination_error('Filter-1', opt, rexi)
                        else:
                            compiled.append(comp)
                rex = compiled
            self.__filter_regex[opt] = rex
        return self

    def __test_is_valid(self, test):
        """
        Check is test is valid: i.e., has tag matching valid tags.
        :param test: test to check for tags.
        :return: True if test has no tags or tag in tags set
        """
        if 0 == len(self.__tags): return True
        has_exclude = 0 < len(self.__tags_exclude)
        tags = test['tags']
        if tags is None: return False or has_exclude
        tests_tags = set(as_list(tags))
        tags = set(self.__tags) & tests_tags
        # tags is set of tags in test which match --tags
        if 0 == len(tags): return False
        if has_exclude:
            # iff. matching tags NOT in exclude
            return 0 == len(tests_tags & set(self.__tags_exclude))
        else:
            return 0 < len(tags)

    def __filter_build_or_test_using_and_logic(self, keeps, item):
        """
        Apply build/test filter(s) and update keeps.
        Here: we use AND logic: i.e., all filters must match.
        :param keeps: list of build/tests to keep.
        :param item: _Build/_Test to consider.
        :param type: build|test.
        :return: self
        """
        if type(item) is self._Test:
            xtype = 'test'
        else:
            xtype = 'build'

        def keepit():
            self.__filter_counts[xtype]['valid'] += 1
            keeps.append(item)

        def fqn():
            return self.__get_target_name(item)

        score = {
            'enable': {'tried': 0, 'match': 0, 'reasons': []}
        }
        self.__filter_counts[xtype]['considered'] += 1
        # Check case where parent build no longer valid
        if xtype == 'test' and not item.build.is_valid:
            debug('Debug-1', f"Reject {xtype} '{fqn()}' since (parent) build not valid")
            return self
        tried, check_rex = 0, False
        name = item['name']
        # Perhaps short-circuit if test not match tag
        if xtype == 'test' and not self.__test_is_valid(item):
            debug('Debug-1', f"Reject {xtype} '{fqn()}' since does not match tag(s)")
            return self
        # First we try enables: must match ALL/AND of them
        opt = f'{xtype}_name_enable'
        rex = self.__filter_regex[opt]
        if rex:
            score['enable']['tried'] += 1
            check_rex = filter_using_regexp(name, rex)
            if check_rex:
                score['enable']['match'] += 1
                score['enable']['reasons'].append(f"'--{opt} {check_rex.pattern}'")
        opt, cmd = f'{xtype}_cmd_enable', item['run_cmd']
        rex = self.__filter_regex[opt]
        if rex:
            score['enable']['tried'] += 1
            check_rex = filter_using_regexp(cmd, rex)
            if check_rex:
                score['enable']['match'] += 1
                score['enable']['reasons'].append(f"'--{opt} {check_rex.pattern}'")
        if score['enable']['tried'] > 0:
            if score['enable']['tried'] == score['enable']['match']:
                debug('Debug-1',
                      f"Keep {xtype} '{fqn()}' since matches: " \
                      + ' AND '.join(score['enable']['reasons']))
                keepit()
            else:
                # none of the enable matches
                debug('Debug-1',
                      f"Reject {xtype} '{fqn()}' since name did not match " \
                      "the enable pattern(s)")
            return self
        # If we match any disable, we dont take it
        tried = 0
        opt = f'{xtype}_name_disable'
        rex = self.__filter_regex[opt]
        if rex:
            tried += 1
            check_rex = filter_using_regexp(name, rex)
            if check_rex:
                debug('Debug-1', f"Reject {xtype} '{fqn()}' since name matches '--{opt} {check_rex.pattern}'")
                return self
        opt = f'{xtype}_cmd_disable'
        rex = self.__filter_regex[opt]
        if rex:
            tried += 1
            check_rex = filter_using_regexp(cmd, rex)
            if check_rex:
                debug('Debug-1', f"Reject {xtype} '{fqn()}' since run_cmd matches '--{opt} {no_keep.pattern}'")
                return self
        if 0 == tried:
            debug('Debug-1', f"Keep {xtype} '{fqn()}' since no --{xtype}_{{name,cmd}}_disable specified")
        else:
            debug('Debug-1', f"Keep {xtype} '{fqn()}' since did not match any --{xtype}_{{name,cmd}}_disable filter")
        keepit()
        return self

    def __filter_build_or_test(self, keeps, item):
        return self.__filter_build_or_test_using_and_logic(keeps, item)

    def __filter_build(self, keep_builds, build):
        """
        Apply build filter(s) and update keep_builds.
        :param keep_builds: list of builds to keep.
        :param build: build to consider.
        :return: self
        """
        assert type(build) is self._Build
        return self.__filter_build_or_test(keep_builds, build)

    def __filter_test(self, keep_tests, test):
        """
        Apply tes filter(s) and update keep_builds.
        :param keep_tests: list of tests to keep.
        :param test: test to consider.
        :return: self
        """
        assert type(test) is self._Test
        return self.__filter_build_or_test(keep_tests, test)

    def __flatten(self):
        """
        Flatten configuration and apply {test,build}_{cmd,name}_{enable,disable}.
        Map dvrun.Build,Test to Makefile._Build,_Test, respectively.
        :return: self
        """
        # Flatten config to _Build and _Test objects.
        for config in self.__configs:
            for build in config.builds:
                self.__all_builds.append(self._Build(config, build))
        for build in self.__all_builds:
            for test in build.build.tests:
                self.__all_tests.append(self._Test(build, test))
        # Apply filters
        keeps = []
        for build in self.__all_builds:
            self.__filter_build(keeps, build)
        # Mark discarded builds no longer valid
        for build in self.__all_builds:
            if build not in keeps: build.is_valid = False
        self.__all_builds = keeps
        keeps = []
        for test in self.__all_tests:
            self.__filter_test(keeps, test)
        self.__all_tests = keeps
        self.__prune_empty_builds()
        if reduce(
                lambda a, b: a or b,
                map(
                    lambda k: self.__filter_counts[k]['considered'] > self.__filter_counts[k]['valid'],
                    ['test', 'build']
                )
        ):
            warn('Filter-2',
                 self.__filter_counts['build']['valid'],
                 self.__filter_counts['build']['considered'],
                 self.__filter_counts['test']['valid'],
                 self.__filter_counts['test']['considered'],
                 )
        return self.__keep_max_tests().__after_test_filter_cb()

    def __after_test_filter_cb(self):
        if self.__after_test_filter is not None:
            self.__after_test_filter(self.all_tests)
        return self

    def __get_test_order(self):
        """
        Python is so lame... doesn't have separate namespace for vars and methods!
        Process test-order file(s) to extract/update self.__test_order.
        :return: self
        """
        fname = self.options['--test_order']
        if not fname: return self
        for fi in as_list(fname):
            lnum = 0
            with open(fi) as ifs:
                for line in ifs:
                    lnum += 1
                    line = re.sub('#.*', '', line).strip()
                    if 1 > len(line): continue
                    for e in line.split():
                        try:
                            re.compile(e)
                            self.__test_order.append(e)
                        except:
                            warn('TestOrder-1', fi, lnum, e)
        return self

    def __keep_max_tests(self):
        nn = self.options['--max_tests']
        if nn is None: return self
        try:
            nn = int(nn)
        except ValueError:
            early_termination_error('Arg-1', nn, '--max-tests')
        orig_nn = len(self.__all_tests)
        if nn < orig_nn:
            self.__all_tests = self.__all_tests[0:nn]
            self.__reset_filter_counts().__prune_empty_builds()
            warn('MaxTests-1', orig_nn, nn)
        return self

    def __prune_empty_builds(self):
        """
        Go through __all_tests and prune builds with no tests.
        :return: self
        """
        test_cnt_by_build_id = {}
        for build in self.__all_builds:
            test_cnt_by_build_id[id(build)] = 0
        for test in self.__all_tests:
            build_id = id(test.build)
            if build_id in test_cnt_by_build_id:
                test_cnt_by_build_id[id(test.build)] += 1
        keep_builds = []
        for build in self.__all_builds:
            if 0 < test_cnt_by_build_id[id(build)]:
                keep_builds.append(build)
            else:
                self.__filter_counts['build']['valid'] -= 1
        self.__all_builds = keep_builds
        return self

    def __get_rules(self, rules, sp=' ' * 4):
        assert isinstance(rules, list)
        return f' \\\n{sp}'.join(rules)

    def __get_path_to(self, prog):
        return os.path.abspath(os.path.join(os.path.dirname(sys.argv[0]), prog))

    def __add_runstatus(self, ofs):
        run_status = self.__get_path_to('et-dvrun-status')
        run_root = self.RUN_ROOTDIR_ENV
        ofs \
            .println('\n# Show status as we progress') \
            .println(f'ETDV_STATUS := {run_status}') \
            .println(
            f'FLOCK := $(if $(wildcard /bin/flock),/bin/flock -x ${{{run_root}}})' '\n' \
            f'{self.STATUS_NOW_CMD}' ' := ${FLOCK} ' \
            '${ETDV_STATUS} status $(shell ${ETDV_STATUS} now) ' \
            f'${{{run_root}}}'
        ) \
            .println('ETDV_STATUS_DETAILS := ${ETDV_STATUS} progress ' + f'${{{run_root}}}')
        return self

    def __add_killall(self, nloop=2, dur=2):
        killall = os.path.realpath(os.path.join(self.outdir, 'killall'))
        with OStream(open(killall, 'w')) as ofs:
            ofs.println("#!/bin/bash")
            cmd = f"{self.KILL_ALL} {os.path.realpath(self.outdir)} >/dev/null 2>&1"
            for i in range(1, nloop + 1):
                ofs.println(cmd)
                if i < nloop: ofs.println(f'sleep {dur}')
        os.chmod(killall, 0o755)
        return self

    def __get_recipe(self, subtarget):
        recipe = \
            "\t-${MAKE} -C ${ETDV_RUNROOT} " f'{subtarget}' " ETDV_SHOW_FAIL=${ETDV_SHOW_FAIL}\n"
        if (subtarget == "all_test_targets"): recipe += "\tet-dvrun-account4notrun\n"
        if self.options['--status']:
            recipe += "\t-${MAKE} -C ${ETDV_RUNROOT} status_details\n"
        else:
            recipe += "\t${ETDV_STATUS_NOW}\n"
        if subtarget == "all_test_targets" and self.options['--rof']:
            recipe += "\t${MAKE} -C ${ETDV_RUNROOT} run_rof\n"
        return recipe

    def __keep_all_status(self, xpass=1):
        """
        Objective is to not remake pass build/test.
        On 1st pass, we record status and value.
        On 2nd pass, we recreate status == ? for those.
        :param xpass: 1 or 2.
        :return: None
        """
        if 1 == xpass:
            self.__status_values = []
            for status in glob.glob(os.path.join(self.outdir, '**', 'status'), recursive=True):
                val = read_file(status)
                self.__status_values.append({'f': status, 'v': val})
                debug('Debug-1', f'{status}: status={val.split()[0]}')
        else:
            for fv in self.__status_values:
                status, val = fv['f'], fv['v']
                write_file(status, val)
                debug('Debug-1', f'{status}: restore status=={val.split()[0]}')
            self.__status_values = None  # cleanup

    def __keep_pass_status(self, xpass=1):
        """
        Objective is to not remake pass build/test.
        On 1st pass, we record which status are pass.
        On 2nd pass, we recreate status == pass for those.
        :param xpass: 1 or 2.
        :return: None
        """
        if 1 == xpass:
            self.__status_pass_files = []
            for status in glob.glob(os.path.join(self.outdir, '**', 'status'), recursive=True):
                if read_file(status).startswith('pass'):
                    self.__status_pass_files.append(status)
                    debug('Debug-1', f'{status}: keep status pass')
        else:
            for status in self.__status_pass_files:
                write_file(status, 'pass')
                debug('Debug-1', f'{status}: restore status==pass')
            self.__status_pass_files = None  # cleanup

    def __to_makefile(self, mkfname='Makefile'):
        os.makedirs(os.path.realpath(self.outdir), exist_ok=True)
        #remove .killed_by_zkmof
        delete_file(os.path.join(self.outdir, '.killed_by_zkmof'))
        if self.options['--keep_all']:
            self.__keep_all_status(xpass=1)
        elif self.options['--keep_pass']:
            self.__keep_pass_status(xpass=1)
        ofname = os.path.join(self.outdir, mkfname)
        metrix = self.__create_get_run_id()
        if metrix is not None:
            steps_to_run = '0x03f'
            create_run_id = """
.PHONY: create_metrix_run_id
create_metrix_run_id:
""" "\t@" f'{self.__prefix_path(metrix)}' ' ${NTESTS}'
        else:
            steps_to_run = '0x02f'
            create_run_id = ""
        info('Makefile-2', ofname)
        with OStream(open(ofname, 'w')) as ofs:
            ofs.println('# Generated on {}\n# from:'.format(ddmmmyyyy_hhmmss()))
            [ofs.println(f) for f in [f'#    {config.filename}' for config in self.__configs]]
            ofs \
                .println(f"# by host: {exec('/bin/hostname -s')}") \
                .println('\n# Clear suffix rules: we dont use here\n.SUFFIXES:') \
                .write_args(self.__write_env, self.RUN_ROOTDIR_ENV, eq=' := ', reqd=False)
            self \
                .__add_runstatus(ofs)
            ofs \
                .println("""
ETDV_MAKE_PPID = $$PPID
ETDV_RUNROOT_UP = $(realpath ${ETDV_RUNROOT}/..)
# Tee logs
ETDV_TEE_LOGS ?= 0
# Show testname fail
ETDV_SHOW_FAIL ?= 0
# Log script output
ifeq ("0", "${ETDV_TEE_LOGS}")
ALL_STEPS_LOG ?= > ${@D}/.all_steps.log 2>&1
else
ALL_STEPS_LOG ?= | tee ${@D}/.all_steps.log 2>&1
endif
""") \
                .println("# Which all_steps to run (default: all of them, except metrix)") \
                .println(f"ALL_STEPS_TO_RUN ?= {steps_to_run}") \
                .println(create_run_id) \
                .println('\n.PHONY: all_tests all_test_targets all_builds all_build_targets status_now status_details') \
                .println(f"all_tests:\n{self.__get_recipe('all_test_targets')}") \
                .println(f"all_builds:\n{self.__get_recipe('all_build_targets')}") \
                .println("status_now: ;\n\t${ETDV_STATUS_NOW}") \
                .println("\nstatus_details: ;\n\t@echo '# Status details'\n\t@${ETDV_STATUS_DETAILS}") \
                .println("\n.PHONY: run_rof") \
                .println("run_rof: ;") \
                .println("\t@echo '# Run rof (${ETDV_RUNROOT_UP}/rof/run_dvrun) ...'") \
                .println("\tet-dvrun-rof ${ETDV_RUNROOT_UP}/rof.json") \
                .println("\t(cd ${ETDV_RUNROOT_UP}/rof ; ./run_dvrun)") \
                .println('\n# build targets\n#')
            #﻿et-dvrun-rof tmp/rof_test/rof.json && ( cd tmp/rof_test/rof ; ./run_dvrun )
            build_rules = [self.__add_build(ofs, build) for build in self.__all_builds]
            num_builds = len(build_rules)
            ofs.println('\n# Test targets\n#')
            test_rules = [self.__add_test(ofs, test) for test in self.__all_tests]
            num_tests = len(test_rules)
            test_rules = [rule for rule in test_rules if rule is not None]
            test_rules = self.__order_test_rules(test_rules)
            test_rules = self.__get_rules(test_rules)
            build_rules = self.__get_rules(build_rules)
            ofs \
                .println('\nALL_BUILD_TARGETS := ' f'{build_rules}') \
                .println('\nALL_TEST_TARGETS := ' f'{test_rules}') \
                .println('\nNTESTS := ' f'{num_tests}') \
                .println("\nall_test_targets: ${ALL_TEST_TARGETS} ;") \
                .println("\nall_build_targets: ${ALL_BUILD_TARGETS} ;")
            self.__add_clean_rules(ofs)
        if self.options['--keep_all']:
            self.__keep_all_status(xpass=2)
        elif self.options['--keep_pass']:
            self.__keep_pass_status(xpass=2)
        return self.__create_status(num_builds, num_tests)

    def __order_test_rules(self, test_rules):
        if 1 > len(self.__test_order): return test_rules
        # test rules form: test.config.build.test
        # __test_order: test | config.build.test
        new_order = []
        ixs = []
        for i in self.__test_order:
            # regexp
            rex = re.compile(r'^test\.\w+\.\w+\.' + i + '$')
            found = list(filter(rex.match, test_rules))
            for f in found:
                ix = test_rules.index(f)
                if ix not in ixs: ixs.append(ix)
        for ix in ixs: new_order.append(test_rules[ix])
        debug('Debug-1', f'__order_test_rules found: {new_order}')
        # need to remove all at once, otherwise index change with each 'del'
        # so lame ass way...
        i = 0
        for e in test_rules:
            if i not in ixs: new_order.append(test_rules[i])
            i += 1
        return new_order

    def __create_status(self, num_builds, num_tests):
        write_istatus(self.outdir, n={'builds': num_builds, 'tests': num_tests})
        return self

    def __add_clean_rules(self, ofs):
        ofs \
            .println("\n# clean targets\n#") \
            .println('.PHONY: clean clean.builds clean.tests') \
            .println('clean: clean.builds clean.tests ;') \
            .println('clean.builds: ${ALL_BUILD_TARGETS:%=clean.%} ;') \
            .println('clean.tests:  ${ALL_TEST_TARGETS:%=clean.%} ;')
        [self.__add_clean_rule(ofs, bt) for bt in self.__all_builds + self.__all_tests]
        return self

    def __add_clean_rule(self, ofs, build_or_test):
        if build_or_test.rule is None: return self
        rule = f'clean.{build_or_test.rule[0]}'
        dir = os.path.dirname(build_or_test.log)
        xcept = basename_parts(build_or_test.log)[0]
        ofs \
            .println('\n.PHONY: ' f'{rule}') \
            .println(f'{rule}:\n\t@et-dvrun-cleanup {dir} {xcept},.all_steps')
        return self

    @staticmethod
    def get_target_name(item):
        return Makefile.the_one().__get_target_name(item)

    def __get_target_name(self, item):
        if isinstance(item, Makefile._Build):
            name = f"{item.config.config_name}.{item.build['name']}"
        else:
            assert isinstance(item, Makefile._Test)
            build = self.__get_target_name(item.build)
            name = f"{build}.{item.test['name']}"
        return name

    REQUIRED_ENV = ['PATH', 'VERDI_HOME', 'VCS_HOME', 'SNPSLMD_LICENSE_FILE', 'REPOROOT']

    RM_SYMLINKS = ['PATH', 'LD_LIBRARY_PATH']

    def __write_env(self, ofs, envs=None, pfx='export ', eq='=', reqd=True):
        if envs is None:
            envs = []
        elif isinstance(envs, str):
            envs = [envs]
        if reqd: envs += self.REQUIRED_ENV
        envs = unique(envs)
        for env in envs:
            val = getenv(env)
            if env in self.RM_SYMLINKS:
                # Let's remove symlinks
                val = ':'.join([os.path.realpath(dir) for dir in val.split(':')])
            if val is None:
                ofs.println(f"# {pfx}{env}{eq}<no_value>")
            else:
                if os.path.exists(val):
                    val = os.path.realpath(val)
                ofs.println(f"{pfx}{env}{eq}{val}")
        return ofs

    def __prefix_path(self, path):
        """
        Prefix path with ${RUN_ROOTDIR_ENV}/...
        :param path: path to prefix.
        :return: prefixed path.
        """
        path = os.path.realpath(path)
        abs_run_dir = os.path.realpath(self.outdir)
        return f'${{{self.RUN_ROOTDIR_ENV}}}' + path[len(abs_run_dir):]

    def __get_check_cmd(self, build_or_test):
        PREDEFINED = ((r'\$TARGET(\W|$)', r'${@}\1'), (r'\$LOG(\W|$)', r'${<}.log\1'))
        cmd = build_or_test['check_cmd']
        if cmd is None: return None
        cmd = multi_line_strip(cmd)
        for patt_repl in PREDEFINED:
            cmd = re.sub(patt_repl[0], patt_repl[1], cmd)
        cmd = '\t' + re.sub(r'\n', '\n\t', cmd)
        return cmd

    def __add_rule(self, mk_ofs, create_fn, build_or_test, xtype, dep=''):
        rule = self.__get_target_name(build_or_test)
        script_fname = create_fn(build_or_test, rule)
        rule = f'{xtype}.{rule}'
        script_fname = self.__prefix_path(script_fname)
        check_cmd = self.__get_check_cmd(build_or_test)
        rule = self.__add_rule_recipe(mk_ofs, build_or_test, xtype, dep, rule, script_fname, check_cmd)
        mk_ofs.println(f'\t@${{{self.STATUS_NOW_CMD}}}')
        return rule

    RECIPE_SCRIPT_TEMPLATE = """#!/bin/bash @opts@
if [ "x" == "x${1}" ] || [ "x" == "x${2}" ] || [ "x" == "x${3}" ]; then
    echo "Usage: ETDV_RUNROOT ALL_STEPS_TO_RUN ETDV_TEE_LOGS ETDV_MAKE_PPID"
    exit 1
fi
ETDV_RUNROOT="${1}"
steps_to_run=${2}
tee_logs=${3}
make_pid=${4}
target="@target@"
script="@script@"
script_dir=`dirname $script`
script_log=${script}.log
killedmsg="${script_dir}/.killedmsg"
spinlockfn=""
if [ `/bin/hostname -s | /bin/fgrep -ic 'et-aws-'` -gt 0 ]; then
    spinlockfn="${script_dir}/.slacjobid"
    et-dvrun-spinlock-request ${script} ${spinlockfn}
fi
if [ $((0x01 & $steps_to_run)) -gt 0 ]; then
    export ET_SLURM_OPTS="@slurm_time@ @slurm_opts@"
    rm -f ${script_log} ${killedmsg}
    if [ "0" == "${tee_logs}" ]; then
        @slurm@ @timeit@ ${script} > ${script_log} 2>&1
    else
        @slurm@ @timeit@ ${script} | tee ${script_log} 2>&1
    fi
    if [ -r ${killedmsg} ]; then
        cat ${killedmsg} >> ${script_log}
    fi
fi
if [ "x" != "x${spinlockfn}" ]; then
    et-dvrun-spinlock-release ${spinlockfn}
fi
if [ $((0x02 & $steps_to_run)) -gt 0 ]; then
    true; #do something in case no check
    @check@
fi
if [ $((0x04 & $steps_to_run)) -gt 0 ]; then
    if [ ! -f ${target} ]; then
        sleep 60  #in case nfs/noac not set; wait for file to appear
        if [ ! -f ${target} ]; then
            echo unknown > ${target}
        fi
    fi
fi
if [ -f ${script_dir}/../../../../.killed_by_zkmof ]; then
    echo fail process_killed_by_zkmof > ${target}
fi
do_gzip="0"
if [ $((0x08 & $steps_to_run)) -gt 0 ]; then
    # case of .gz BEFORE metrix (anomoly)
    if [ -e ${script_log}.gz ]; then
        gunzip -f ${script_log}.gz
        do_gzip="1"
    fi
    et-dvrun-metrix create ${script_log}
fi
if [ $((0x010 & $steps_to_run)) -gt 0 ]; then
    et-dvrun-metrix upload ${script_log}
fi
testme_post=\"@testme_post@\"
if [ $((0x020 & $steps_to_run)) -gt 0 ]; then
    if [ "x" != "x${testme_post}" ]; then
        ${testme_post} ${script}
    else
        et-dvrun-gzip-or-rm-log @onpass@ ${target} ${script_log}
    fi
fi
if [ "${do_gzip}" == "1" ]; then
    if [ -e ${script_log} ]; then
        gzip -f ${script_log}
    fi
fi
if [ @zkmof@ -gt 0 ]; then
    et-dvrun-kill-make-on-fail ${target} ${make_pid}
fi
"""

    def __subst_check_cmd(self, check_cmd):
        if check_cmd is None: return None
        check_cmd = re.sub(r'^@', '', check_cmd.strip())
        check_cmd = re.sub(r'\$(\(|{)@(}|\))', '${target}', check_cmd)
        check_cmd = re.sub(r'\$(\(|{)<(}|\))', '${script}', check_cmd)
        check_cmd = re.sub(r'\$(\(|{)@D(}|\))', '`dirname ${target}`', check_cmd)
        # we dont do any SHOW_FAIL in the script: its in makefile
        check_cmd = re.sub(r'\$(\(|{)(ETDV_)?SHOW_FAIL(}|\))', '0', check_cmd)
        return check_cmd

    def __merge_slurm_opts(self, opts):
        """
        Merge slurm opts, since command line can have -L and attribute similarly.
        BUT: slurm does not aggregate, so we need to do it.
        :param opts: combined command and user opts
        :return: merged slurm options
        """
        opts2 = opts.split()
        if 1 > len(opts2): return ''
        merged = []
        lic_opts = []
        while 0 < len(opts2):
            opt = opts2.pop(0)
            if opt == '-L':
                lic_opts.append(opts2.pop(0))
            else:
                m = re.match('\-\-licenses=(.+)', opt)
                if m:
                    lic_opts.append(m.group(1))
                else:
                    merged.append(opt)
                    if 0 < len(opts2):
                        if opts2[0][0] != '-':
                            merged.append(opts2.pop(0))
        opts = ' '.join(merged)
        lic_opts = unique(flatten([e.split(',') for e in lic_opts]))
        if 0 < len(lic_opts):
            opts += f" -L {','.join(lic_opts)}"
        return opts

    def __create_recipe_script(self, script_fname, check_cmd, target, xtype=None, build_or_test=None):
        steps_fname = os.path.join(os.path.dirname(script_fname), ".all_steps")
        steps_fname = steps_fname.replace(f'${{{self.RUN_ROOTDIR_ENV}}}', getenv(self.RUN_ROOTDIR_ENV))
        slurm_time = ''
        if self.slurm:
            user_slurm_opts = build_or_test['slurmopts']
            if xtype is 'build':
                slurm = 'et-slurm-vcscompiler'
            else:
                slurm = 'et-slurm-vcsruntime'
                user_slurm_opts = user_slurm_opts or build_or_test.build.build['test_slurmopts']
            resv = self.slurm_reservation
            if resv:
                slurm_opts = f'--reservation={resv}'
            else:
                slurm_opts = ''
            sopts = self.slurm_opts
            if sopts:
                sopts = sopts.replace("'", '')
                slurm_opts += f" {sopts}"
            if user_slurm_opts:
                slurm_opts += f" {user_slurm_opts}"
            if xtype is not 'build' and self.options['--timeout'] and "0" != self.options['--timeout']:
                slurm_time = f"--time={self.options['--timeout']}"
        else:
            slurm, slurm_opts = '', ''
        zkmof = (self.options['--zkmof'] and '1') or '0'
        contents = template_ize(self.RECIPE_SCRIPT_TEMPLATE,
                                opts='-x',  # no -e: since we want to continue even on error to other steps
                                target=target,
                                script=script_fname,
                                check=self.__subst_check_cmd(check_cmd),
                                onpass=self.options['--onpass'],
                                testme_post=self.options['--testme_post'],
                                timeit=TIME_IT,
                                slurm=slurm,
                                slurm_opts=self.__merge_slurm_opts(slurm_opts),
                                slurm_time=slurm_time,
                                zkmof=zkmof
                                )
        info('Script-1', steps_fname, 'recipe in makefile')
        with open(steps_fname, 'w+t') as ofs:
            ofs.write(contents)
        os.chmod(steps_fname, 0o755)
        return os.path.basename(steps_fname)

    # returns != 0 if pass not matched
    CHECK_FAIL = """\t@if [ ! -e ${@} ]; then echo fail > ${@} ; fi
\t@if [ ${ETDV_SHOW_FAIL} -gt 0 ]; then egrep -iq "^pass" ${@} ; fi"""

    def __add_rule_recipe(self, mk_ofs, build_or_test, xtype, dep, rule, script_fname, check_cmd):
        status = os.path.join(os.path.dirname(script_fname), 'status')
        recipe_fname = self.__create_recipe_script(script_fname, check_cmd, status, xtype, build_or_test=build_or_test)
        if self.options['--znibc'] is None:
            dashi = '-'
        else:
            dashi = ''
        mk_ofs \
            .println(f'\n.PHONY: {rule}') \
            .println(f'{rule}: {status} ;') \
            .println(f'.PRECIOUS: {status}') \
            .println(f'{status}: {script_fname} {dep}') \
            .println(f'\t{dashi}${{<D}}/{recipe_fname}' \
                     f' ${{{self.RUN_ROOTDIR_ENV}}}' \
                     ' ${ALL_STEPS_TO_RUN}'
                     ' ${ETDV_TEE_LOGS}'
                     ' ${ALL_STEPS_LOG}'
                     ' ${ETDV_MAKE_PPID}'
                     ) \
            .println(self.CHECK_FAIL)
        build_or_test.rule = [rule, status]
        build_or_test.log = f'{script_fname}.log'
        return rule

    def __add_build(self, mk_ofs, build):
        return self.__add_rule(mk_ofs, self.__create_build_script, build, 'build')

    def __get_filename(self, subdir, target_nm, name=None):
        """
        Generate script filename.
        :param subdir: build|test
        :param target_nm: dot separated (makefile) target: config.build(.test)?
        :param name: actual base filename.
        :return: absolute script filename
        """
        config, build, test = (target_nm.split('.') + [None])[:3]
        if subdir == 'builds':
            assert test is None
            parts = [config, build, 'build']
        elif subdir == 'tests':
            assert test is not None
            parts = [config, build, 'tests', test]
        else:
            assert False
        filename = os.path.join(self.outdir, *parts)
        if name is not None: filename = os.path.join(filename, name)
        return os.path.realpath(filename)

    ACTUAL = {'builds': 'buildme', 'tests': 'testme'}
    KILL_ALL = os.path.realpath(os.path.join(sys.path[1], 'et-dvrun-kill-all'))
    LOG_MONITOR = os.path.realpath(os.path.join(sys.path[1], 'et-dvrun-log-monitor'))
    TIMEOUT_MONITOR = os.path.realpath(os.path.join(sys.path[1], 'et-dvrun-timeout-monitor'))

    def __create_script(self, subdir, target_nm, commands,
                        env=None, preamble=None, build_rundir=None, tags=None):
        script_fname = self.__get_filename(subdir, target_nm, self.ACTUAL[subdir])
        kill_fname = os.path.join('${ETDV_RUNDIR}', 'killme')
        log_fname = os.path.join('${ETDV_RUNDIR}', f'{self.ACTUAL[subdir]}.log')
        os.makedirs(os.path.realpath(os.path.dirname(script_fname)), exist_ok=True)
        self.__write_seed(os.path.dirname(script_fname))
        info('Script-1', script_fname, subdir[:-1])
        if tags:
            tags_meta = f"#TAGS: {','.join(as_list(tags))}"
        else:
            tags_meta = ''
        if build_rundir is not None:
            if True:
                build_rundir = """
# build is relative to test
function get_build_rundir() {
    echo "$( cd "${ETDV_RUNDIR}/../../build" && /bin/pwd )"
}
export ETDV_BUILD_RUNDIR=$(get_build_rundir)

"""
            else:
                build_rundir = f'export ETDV_BUILD_RUNDIR={build_rundir}\n'
            timeout = self.options['--timeout'] or 0
        else:
            build_rundir = ''
            timeout = 0  # builds have no timeout
        #                       f"{self.TIMEOUT_MONITOR} {log_fname} $$ {timeout} &\n"
        if self.slurm:
            timeout_cmd = ''
        else:
            timeout_cmd = f"{self.TIMEOUT_MONITOR} {log_fname} $$ {timeout} &\n"
        with OStream(open(script_fname, 'w')) as sh_ofs:
            sh_ofs \
                .println("#!/bin/bash -e\n\n# Show commands before we execute them\nset -x\n") \
                .println(tags_meta) \
                .write("""
# Return directory of this script.
function get_rundir() {
    echo "$( cd "$( dirname "${BASH_SOURCE[0]}" )" && /bin/pwd )"
}
export ETDV_RUNDIR=$(get_rundir)
"""
                       ) \
                .write(build_rundir) \
                .write_args(self.__write_env, env) \
                .write(f"\npidfn={kill_fname}\n"
                       """
echo '#!/bin/bash' > $pidfn
"""
                       f"echo {self.KILL_ALL} $$ >> $pidfn"
                       """
/bin/chmod +x $pidfn

function cleanup() {
   /bin/rm -f $pidfn
}
function bad_signal() {
    cleanup
    echo "ERROR: bad_signal${1}"
    exit 0  #dont want to trigger make fail
}
set +x  #disable verbose
trap cleanup EXIT
for s in ABRT BUS FPE USR1 SEGV USR2 PIPE ALRM TERM
do
    trap "bad_signal $s" $s
done
#Get the hostname for our metrix
echo "metrixHostname:`/bin/hostname -s`"
echo "metrixBegin:`/bin/date`"
echo "SLURM_JOBID=${SLURM_JOBID:-n/a}"
set -x  #enable verbose

# Set OK status unless we (re)set later
status=0

# Monitor this log for bad things and kill if we find them.
# We also set test timeout (mins) as last argument.
"""
                       f"{self.LOG_MONITOR} {log_fname} $$ {self.options['--zlmb']} 0 &\n"
                       f"{timeout_cmd}"
                       )
            if preamble is not None:
                sh_ofs.println(
                    "\n# Preamble from config file\n" f'{preamble}\n'
                )
            commands = multi_line_strip(commands)
            sh_ofs.println(
                # python concatenates adjacent string...
                "\n#\n# Commands from config here\n#\n" f'{commands}' "\n#\n# end of commands\n"
                "#\n# NOTE: it is possible command(s) above set status when they exit.\n"
                "# It is recommended that command(s) do NOT exit with != 0 status, since it will\n"
                "# trigger fail/exit of our outermost make, unless run with -i.\n#\n"
                "cleanup\nexit $status\n"
            )
        os.chmod(script_fname, 0o755)
        return script_fname

    def __create_build_script(self, build, target_nm):
        fname = self.__create_script(
            'builds',
            target_nm,
            commands=build['run_cmd'],
            env=build.config.env,
            preamble=build['preamble']
        )
        build.rundir = os.path.dirname(fname)
        return fname

    def __create_test_script(self, test, target_nm):
        build_rundir = test.build.rundir
        return self.__create_script(
            'tests',
            target_nm,
            env=test.build.config.env,
            commands=test['run_cmd'],
            preamble=test['preamble'],
            build_rundir=build_rundir,
            tags=test['tags']
        )

    def __write_seed(self, dir):
        fn = os.path.join(dir, '.etdv.randomseed')
        write_file(fn, etdv_randint())
        return self

    def __add_test(self, mk_ofs, test):
        test_name = self.__get_target_name(test)
        path = os.path.dirname(test.build.log)
        build_dep = os.path.join(path, 'status')
        return self.__add_rule(mk_ofs, self.__create_test_script, test, 'test', dep=build_dep)
