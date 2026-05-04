# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Convert .sim to .etdv.py
"""
import re, os, copy
from ETdv.dvrun.messaging import (info, warn, debug, error)
from ETdv.util import (getenv, flatten, basename_parts, as_list, replace_vals, unique, delete_file)
from ETdv.dvrun.simfile import Simfile

assert False, "This file is obsolete"

class XSimToPy:
    class _Globals:
        def __init__(self, init=None):
            self.options = (init and copy.deepcopy(init.options)) or {}
            self.pre_sim = (init and copy.deepcopy(init.pre_sim)) or {}
            self.post_sim = (init and copy.deepcopy(init.post_sim)) or {}

    def __init__(self, sim_fname, globals=None):
        self.__globals = self._Globals(globals)
        self.__sim_fname = os.path.realpath(sim_fname)
        self.__options = self.__globals.options
        self.__local_opts = {}
        self.__included = []
        self.__pre_sim = self.__globals.pre_sim
        self.__post_sim = self.__globals.post_sim
        self.__line_number = 0
        self.__tests = []
        self.__testn = 0
        self.__uniq_test_names = {}

    @property
    def sim_fname(self):
        return self.__sim_fname

    @property
    def __basename(self):
        return basename_parts(self.sim_fname)[0]

    @property
    def __next_testn(self):
        self.__testn += 1
        return self.__testn

    def __setopt(self, k, v, map=None):
        map = map or self.__options
        if k == 'tag':
            if k not in map: map[k] = []
            map[k].append(v)
        else:
            map[k] = v

    def __clr_presim(self):
        self.__pre_sim = {'cmd': None, 'args': []}

    def __clr_postsim(self):
        self.__post_sim = {'cmd': None, 'args': []}

    def __clear_local_opts(self):
        self.__local_opts = {}

    def parse(self):
        def sub_env(val):
            return replace_vals(val)

        def set_local_opt(k, v):
            self.__setopt(k, v, self.__local_opts)

        info('Simfile-1', self.sim_fname)
        with open(self.sim_fname) as ifs:
            for line in ifs:
                self.__line_number += 1
                line = re.sub(r'\s*=\s*', '=', line)
                line = re.sub('#.*', '', line).strip()
                test_name = None
                if 0 < len(line):
                    eles = re.sub(r'\s+', ' ', line).split(' ')
                    has_test = False
                    self.__tag_opt = []
                    while 0 < len(eles):
                        ele = eles.pop(0)
                        if ele == 'include':
                            incl_fname = os.path.join(os.path.dirname(self.sim_fname), eles.pop(0))
                            debug('Debug-1', f'{self.__line_number}: added include: {incl_fname}')
                            included = XSimToPy(incl_fname, self.__globals).parse()
                            included = {
                                'globals': copy.deepcopy(self.__globals),
                                'fname': incl_fname
                            }
                            self.__included.append(included)
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
                        val = sub_env(val)
                        if key == 'test':
                            has_test = True
                            test_name = val
                        elif key == 'preSimCmd':
                            self.__clr_presim()
                            self.__pre_sim['cmd'] = val
                            self.__pre_sim['args'] = eles
                            break  # for
                        elif key == 'postSimCmd':
                            self.__clr_postsim()
                            self.__post_sim['cmd'] = ele.split('=')[-1]
                            break  # line processing
                        else:
                            if has_test:
                                set_local_opt(key, val)
                            else:
                                self.__setopt(key, val)
                    if has_test:
                        self \
                            .__add_test(build_name=test_name) \
                            .__clear_local_opts()
        return self

    def __replace(self, opts):
        for key, val in opts.items():
            if key != 'tag':
                opts[key] = self.__replace_vals(val, opts)
            else:
                opts[key] = [self.__replace_vals(v, opts) for v in opts[key]]

    def __add_test(self, build_name):
        merged = {**self.__options, **self.__local_opts}
        self.__replace(merged)
        test = {
            'options': merged,
            'line': self.__line_number
        }
        if 0 < len(self.__pre_sim) and self.__pre_sim['cmd'] is not None:
            test['pre'] = copy.deepcopy(self.__pre_sim)
            if 1 > len(test['pre']['args']): test['pre'].pop('args', None)
        if 0 < len(self.__post_sim) and self.__post_sim['cmd'] is not None:
            test['post'] = copy.deepcopy(self.__post_sim)
            if 1 > len(test['post']['args']): test['post'].pop('args', None)
        self.__tests.append(test)
        return self

    TEST_FUNC = "add_test"

    def __test_to_py(self, ofs, test):
        if 'tag' in test['options']:
            tag = unique(flatten(test['options']['tag']))
        else:
            tag = []
        test['options']['tag'] = tag
        test['name'] = self.__get_testname(test)
        where = f"#{self.sim_fname}:{test['line']}"
        build = self.get_build_name(self.sim_fname)
        test.pop('line', None)
        ofs.write(f'\n{where}\nbuild:{build}\noptions = {test}\n{self.TEST_FUNC}(options)\n')
        return self

    def __get_testname(self, test):
        options = test['options']
        if 'alias' in options:
            alias = options['alias']
            test_name = self.__replace_vals(alias, options)
            test_name = re.sub(r'\W+', '_', test_name)
        else:
            test_name = f'{self.__basename}_{self.__next_testn}'
        if test_name in self.__uniq_test_names: test_name += f'_{self.__next_testn}'
        self.__uniq_test_names[test_name] = 1
        return test_name

    def __replace_vals(self, val, merged):
        return replace_vals(val, lambda key: (key in merged and merged[key]) or key)

    def print(self):
        ofn = f'{self.sim_fname}.etdv.py'
        delete_file(ofn)
        with open(ofn, 'w+t') as ofs:
            for test in self.__tests:
                self.__test_to_py(ofs, test)
        for test in self.__included:
            test.print(ofn)

    @staticmethod
    def get_make_dir(simfile):
        return Simfile.rm_simlists(os.path.dirname(os.path.realpath(simfile)))

    @staticmethod
    def get_build_name(simfile):
        mk_dir = XSimToPy.get_make_dir(simfile)
        repo = os.path.dirname(getenv('REPOROOT'))
        name = mk_dir.replace(repo,'')
        if name[0] == '/': name = name[1:]
        return re.sub(r'/+', '_', name)

    @staticmethod
    def main(simfiles):
        from ETdv.dvrun.messaging import Messaging
        Messaging('I')
        for sim in simfiles:
            XSimToPy(sim).parse().print()


if __name__ == '__main__':
    import glob
    XSimToPy.main(glob.glob('/Users/kpfalzer/projects/etdv/test/simfiles/**/*.sim', recursive=True))
