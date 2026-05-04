# Manage Rerun on fail (rof)
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from os import path
from ETdv.dvrun.main import Main
from ETdv.dvrun.messaging import info, warn


class Rof:
    def __init__(self, main):
        self.__main = main

    __DEFAULT_RUN_CMD = "echo Need to specify rof_run_cmd property for "

    __COPY_OVER_KEYS = ['check_cmd', 'slurmopts', 'test_slurmopts', 'timeout']

    def check_config(self, builds, tests):
        def c(item, type):
            if type == 'build':
                lhs = item.build
            else:
                lhs = item.test
            if item['rof_run_cmd'] is None:
                name = item.full_name
                warn('Rof-2', name, type)
                lhs['rof_run_cmd'] = self.__DEFAULT_RUN_CMD + name
            # copy over
            for k in self.__COPY_OVER_KEYS:
                rofk = f"rof_{k}"
                if item[rofk] is None and item[k] is not None:
                    lhs[rofk] = item[k]

        for build in builds: c(build, 'build')
        for test in tests: c(test, 'test')
        return self.__write_json(builds, tests)

    # The json is:
    # { 'builds': [{'name':...,...},...],
    #   'tests': [{}...],
    #   'rof_signature_exclude': [...],
    #   'runroot': '...',
    #   'cmdline': '...'}
    def __write_json(self, builds, tests):
        import copy
        def clean(item):
            cfg = copy.deepcopy(item)
            for k in ['run_cmd', 'tags'] + self.__COPY_OVER_KEYS:
                if k in cfg: del cfg[k]
            return cfg

        runroot = self.__main.options['--runroot']
        json_fname = path.abspath(path.join(runroot, "rof.json"))
        odata = {
            'runroot': path.dirname(json_fname),
            'cmdline': Main.get_cmdline_args(),
            'rof_signature_exclude': Main.the_one().rof_signature_exclude,
            'rof_testcnt_by_signature': Main.the_one().rof_testcnt_by_signature
        }
        odata['builds'] = [{'full_name': build.full_name, **clean(build.build.config)} for build in builds]
        odata['tests'] = [{'full_name': test.full_name, **clean(test.test.config)} for test in tests]
        info('Rof-3', json_fname)
        from ETdv.jsonwriter import JsonWriter
        JsonWriter(json_fname, odata).write()
        return self
