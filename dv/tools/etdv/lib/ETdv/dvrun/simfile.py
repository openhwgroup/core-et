# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re, os
from ETdv.dvrun.messaging import (info, warn, debug)
from ETdv.dvrun.build import Build
from ETdv.dvrun.test import Test
from ETdv.util import (getenv, flatten, basename_parts, as_list, replace_vals, escape_list_eles)


class Simfile:
    """
    Object to parse .sim and generate build(s) and test(s) for et-dvrun style.
    The current assumption is that "global" assigns in .sim are not visible within
    included files.
    """

    DEFAULT_ENV = ['BEMU', 'LD_LIBRARY_PATH', 'RISCV', 'TESTROOT', 'TESTSHIRE', 'TESTSTUB', 'RTLROOT']

    __simfiles = []

    # Since multiple .sim can refer to same build, we'll check that we dont create
    # same build across processing all .sim file
    __build_by_name = {}

    @staticmethod
    def get_configurations():
        """
        Process __build_by_name to return Configuration file-like.
        Since there is not a 1:1 map of .sim to config file, since many .sim
        can share a single build, the config-file name is same as the config-name.
        :return: list of ConfigurationFile
        """
        # import here to mitigate circular reference
        from ETdv.dvrun.configfile import ConfigurationFile
        configs = []
        for key, build in Simfile.__build_by_name.items():
            name = key.split('.')[0]
            builds = as_list(build)
            config = ConfigurationFile(name, as_simfile={
                'config_name': name,
                'builds': builds,
                'env': Simfile.DEFAULT_ENV
            })
            configs.append(config)
        return configs

    @staticmethod
    def process_simfile(sim_fname):
        """
        Process .sim file and all of its includes.
        :param sim_fname: .sim file
        :return: list of ConfigurationFile from processed .sim file
        """
        simfiles = Simfile.__simfiles
        simfiles.append(os.path.realpath(sim_fname))
        while 0 < len(simfiles):
            sim_fname = simfiles.pop(0)
            sim_config = Simfile(sim_fname, cwd=os.path.dirname(sim_fname)).process(simfiles)

    def __init__(self, fname, config_name=None, cwd=None, build_check_cmd=None, env=DEFAULT_ENV):
        """
        Read .sim and add build(s) and test(s).
        :param fname: .sim file to process.
        """
        self.__simfile = os.path.realpath(fname)
        if config_name is None:
            config_name = self.__get_config_name()
        self.__config_name = config_name
        self.__cwd = Simfile.rm_simlists(cwd)
        if build_check_cmd is None:
            build_check_cmd = '@et-dvrun-check-log build $LOG $TARGET ${ETDV_SHOW_FAIL}'
        self.__build_check_cmd = build_check_cmd
        self.__pre_sim = {}
        self.__line_number = 0
        self.__options = {'tag': []}
        self.__local_opts = {}
        self.__uniq_test_names = []
        self.__tag_opt = []
        self.__env = env

    @staticmethod
    def get_opts_as_plusargs(options):
        return ' '.join([f'+{k}={escape_list_eles(v)}' for k, v in options.items() if k != 'tag'])

    def _get_opts_as_plusargs(self):
        return self.get_opts_as_plusargs(self.options)

    @staticmethod
    def get_options_as_exports(options, keep=r'[A-Z][A-Z0-9_]+'):
        exports = ""
        for key in [k for k in options.keys() if k != 'tag' and re.match(keep, k)]:
            val = options[key]
            exports += f"export {key}={escape_list_eles(val)}\n"
        return exports

    def __get_options_as_exports(self):
        return self.get_options_as_exports(self.options)

    @property
    def env(self):
        return self.__env

    @property
    def simfile(self):
        return self.__simfile

    @property
    def line_number(self):
        return self.__line_number

    @property
    def options(self):
        return self.__options

    @property
    def builds(self):
        return Simfile.__build_by_name.values()

    @property
    def basename(self):
        return basename_parts(self.simfile)[0]

    @property
    def uniq_test_names(self):
        return self.__uniq_test_names

    @property
    def tags(self):
        return flatten(self.options['tag'] + self.__tag_opt)

    @property
    def config_name(self):
        return self.__config_name

    @staticmethod
    def get_fgrep_cmd(match, cnt=1, predicate='-eq', opts='-ic'):
        return \
            f"@if [ `/bin/fgrep {opts} '{match}' $LOG` {predicate} {cnt} ]; " \
            "then echo pass > $TARGET; else echo fail > $TARGET; fi"

    @property
    def test_check_cmd(self):
        return '@et-dvrun-check-log test $LOG $TARGET ${ETDV_SHOW_FAIL}'

    @property
    def pre_sim(self):
        return self.__pre_sim

    @staticmethod
    def rm_simlists(val):
        # OMG: major hack to add .sim magic...
        val = val.replace('/simlists/', '/')
        val = re.sub(r'/simlists$', '', val)
        return val

    @staticmethod
    def get_presim_cmd(cwd, options, pre_sim):
        script = os.path.realpath(os.path.join(cwd, pre_sim['cmd']))
        script = re.sub(r'\\', '', script)  # there is \\ in .sim file!
        # OMG: major hack to add .sim magic...
        script = Simfile.rm_simlists(script)
        ln_cmd = ""
        m = re.match(r'^.+/scripts/', script)
        if m:
            ln_cmd = f"/bin/ln -fs {m.group(0)[:-1]}\n"
        presim = \
            f"export SEED={options['SEED']}\n" \
            "export testRunDir=${ETDV_RUNDIR}\n" \
            "pushd ${ETDV_RUNDIR}\n" \
            f"{ln_cmd}" \
            f"{script} {' '.join(pre_sim['args'])}\n" \
            "popd\n"
        return presim

    def __get_presim_cmd(self, cwd):
        return self.get_presim_cmd(cwd, self.options, self.pre_sim)

    @staticmethod
    def sub_env(val):
        return replace_vals(val)

    def __replace_vals(self, val, merged):
        return replace_vals(val, lambda key: merged[key])

    def process(self, simfiles):
        # print(f'Info: {simfile}: processing ...')
        info('Simfile-1', self.simfile)
        with open(self.simfile) as ifs:
            self.__clr_presim()
            for line in ifs:
                self.__line_number += 1
                line = re.sub(r'\s*=\s*', '=', line)
                # print(f'DEBUG: line={line}')
                line = re.sub('#.*', '', line).strip()
                test_name = None
                if 0 < len(line):
                    eles = re.sub(r'\s+', ' ', line).split(' ')
                    has_test = False
                    self.__tag_opt = []
                    while 0 < len(eles):
                        ele = eles.pop(0)
                        if ele == 'include':
                            incl_fname = os.path.join(os.path.dirname(self.simfile), eles.pop(0))
                            debug('Debug-1', f'{self.line_number}: added include: {incl_fname}')
                            simfiles.append(incl_fname)
                            break  # while
                        if re.match(r'alias=(.+)', ele):
                            vals = ['alias', ele[6:]]
                        else:
                            vals = ele.split('=')
                        key, val = vals[0], vals[1:]
                        if 2 > len(val):
                            if 1 == len(val):
                                val = val[0]  # scalarize
                            else:
                                # case of: 'tag=nightly riscvtools'
                                key, val = 'tag', key
                        # print(f'DEBUG: {key}={val}')
                        val = Simfile.sub_env(val)
                        if key == 'test':
                            has_test = True
                            test_name = val
                        elif key == 'preSimCmd':
                            self.__clr_presim()
                            # TODO: val could be var as in: preSimCmd=$MINION_DIAGS/fptoint/preTest
                            self.__pre_sim['cmd'] = val
                            self.__pre_sim['args'] = eles
                            break  # for
                        elif key == 'postSimCmd':
                            warn('SimCmd-1', self.simfile, self.line_number, key)
                            break  # line processing
                        else:
                            if has_test:
                                if key == 'tag':
                                    self.__tag_opt.append(val)
                                else:
                                    self.__setopt(key, val)
                                    self.__local_opts[key] = val
                            else:
                                self.__setopt(key, val)
                    if has_test:
                        self.add_test(test_name)
                        self.__remove_local_opts()
        return self

    def add_test(self, build_name):
        """
        Add the test, where the name passed in is actually the build name.
        OMG: who specced out this .sim file syntax?
        :param build_name: the build.
        :return: self
        """
        build = None
        full_build_name = f'{self.config_name}.{build_name}'
        if full_build_name not in Simfile.__build_by_name:
            clean = ''  # 'clean ' # do NOT do make clean VCS
            flock = ''  # f'/bin/flock {self.__cwd} '
            run_cmd = f"{flock}make -C {self.__cwd} {clean}VCS"
            check_cmd = self.__build_check_cmd
            build = Build(
                name=build_name,
                run_cmd=run_cmd,
                check_cmd=check_cmd
            )
            Simfile.__build_by_name[full_build_name] = build
        build = Simfile.__build_by_name[full_build_name]
        run_cmd = f"#\n# From: {self.simfile}:{self.line_number}\n#\n"
        exports = self.__get_options_as_exports()
        if 0 < len(exports):
            run_cmd += "\n# Exports determined from simfile\n" \
                       + exports + "\n#\n"
        if self.pre_sim['cmd'] is not None:
            run_cmd += "\n# preSimCmd {{\n" \
                       + self.__get_presim_cmd(self.__cwd) \
                       + "\n# preSimCmd }}\n#\n"
        run_cmd += "cd ${ETDV_RUNDIR}\n"
        run_cmd += os.path.realpath(
            os.path.join(self.__cwd, 'build', 'vcs', build_name)
        )
        # simply pass in cmdline args to testme using ${*}
        run_cmd += ' ${*} ' + self._get_opts_as_plusargs()
        # always append lnum for consistency
        test_name = self.__get_testname()
        assert (test_name not in self.uniq_test_names), f"duplicate testname: '{test_name}'"
        test = Test(
            name=test_name,
            run_cmd=run_cmd,
            check_cmd=self.test_check_cmd,
            tags=self.tags
        )
        self.uniq_test_names.append(test_name)
        build.add_test(test)
        return self

    def __get_testname(self):
        alias = None
        test_name = f'{self.basename}_l{self.line_number}'
        merged = {**self.options, **self.__local_opts}
        if 'alias' in merged:
            alias = merged['alias']
        else:
            return test_name
        test_name = self.__replace_vals(alias, merged)
        if test_name in self.uniq_test_names: test_name += f'_l{self.line_number}'
        test_name = re.sub(r'\W+', '_', test_name)
        return test_name

    def __setopt(self, k, v):
        if k == 'tag':
            self.__options[k].append(v)
        else:
            self.__options[k] = v

    def __clr_presim(self):
        self.__pre_sim['cmd'] = None
        self.__pre_sim['args'] = []
        self.__pre_sim['tests'] = []

    def __remove_local_opts(self):
        keys = []
        for key in self.__local_opts.keys():
            self.__options.pop(key)
            keys.append(key)
        # hack to not modify local-opts while using its keys
        [self.__local_opts.pop(key) for key in keys]

    @staticmethod
    def get_config_name(simfile):
        reporoot = getenv('REPOROOT')
        assert (reporoot is not None), "setenv REPOROOT required"
        reporoot = os.path.realpath(reporoot)
        config_name = os.path.dirname(simfile)[len(reporoot) + 1:]
        config_name = re.sub(r'^test/?', '', config_name)
        config_name = Simfile.rm_simlists(config_name)
        config_name = re.sub('/', '_', config_name)
        if 1 > len(config_name): config_name = 'my_config_name'
        return config_name

    def __get_config_name(self):
        return self.get_config_name(self.simfile)
