# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re, os
from ETdv.util import (has_env, has_errors, as_list)
from ETdv.constraineddict import MyKeyError
from ETdv.dvrun.messaging import (debug, debug2, info, warn, error, early_termination_error)
from ETdv.dvrun import config
from ETdv.dvrun.simfile import Simfile


class ConfigurationFile:
    """
    A Configuration from a file.
    A Config contains one or more builds and their associated tests.
    """

    def __init__(self, fname, rootdir=None, configi=0, force_cwd=False, as_simfile=None):
        self.__filename = fname
        self.__force_cwd = force_cwd
        if as_simfile:
            assert isinstance(as_simfile, dict)
            self.__builds = as_simfile['builds']
            self.__config_name = as_simfile['config_name']
            self.__env = as_simfile['env']
            self.__sanity_check()
        else:
            config._current = self
            config._builds = []  # reset every time we process a config
            self.__builds = None
            self.__config_name = None
            self.__env = None
            self.__process(rootdir, configi)

    def to_dict(self):
        return {
            'filename': os.path.realpath(self.filename),
            'name': self.config_name,
            'builds': (self.builds and [b.to_dict() for b in self.builds]) or [],
            'env': self.env or []
        }

    @property
    def filename(self):
        return self.__filename

    @property
    def builds(self):
        return self.__builds

    @property
    def config_name(self):
        return self.__config_name

    @property
    def env(self):
        return self.__env

    IMPORTS = """
from ETdv.util import (getenv, has_env, setenv)
from ETdv.dvrun import config
from ETdv.dvrun.config import (add_build, new_build, new_test, get_config_filename, get_user_arg)
from ETdv.dvrun.random_gen import (etdv_randint, etdv_random, etdv_random_seed)
"""

    def __update_from_config(self):
        self.__builds = config._builds
        self.__config_name = config.name
        self.__env = config.env
        from ETdv.dvrun.main import Main
        Main.the_one().rof_signature_exclude = config.rof_signature_exclude
        Main.the_one().rof_testcnt_by_signature = config.rof_testcnt_by_signature

    def __reset(self):
        config.builds = []
        config.current = None
        config.name = None
        config.env = []
        config.rof_signature_exclude = []
        config.rof_testcnt_by_signature = {'.*': 1}
        config.CWD = None

    SIM_INCLUDE_REX = re.compile(r"\s*(sim_include\s*\(\s*'([^']+)'\s*\))")

    def __get_sim_include_contents(self, parent_dir, incl_fname, did_incl):
        if not parent_dir:
            parent_dir = os.path.dirname(os.path.realpath(self.filename))
        fname = os.path.join(parent_dir, incl_fname)
        try:
            debug('SimIncl-1', fname)
            if fname in did_incl:
                early_termination_error('SimIncl-2', fname)
            did_incl[fname] = True
            with open(fname, 'r') as ifs:
                contents = ifs.read()
            ncontents = self.__include_sim_include(contents, did_incl, os.path.dirname(fname))
            ncontents = f"\nsim_mark_include('begin', '{fname}')\n{ncontents}\nsim_mark_include('end', '{fname}')\n"
            return ncontents
        except IOError as err:
            early_termination_error('File-2', fname, 'read', err)

    def __include_sim_include(self, contents, did_incl=None, parent_dir=None):
        """
        Process contents and inline sim_include clauses.
        :param contents: contents to process.
        :return: contents with inlined sim_includes.
        """
        if not did_incl: did_incl = {}
        while True:
            m = self.SIM_INCLUDE_REX.search(contents)
            if not m: break
            find = m.group(1)
            replace = self.__get_sim_include_contents(parent_dir, m.group(2), did_incl)
            contents = contents.replace(find, replace + '\n')
        return contents

    def __find_vars_to_export(self, contents):
        """
        Since .sim introduce env adhoc, find ones which look like env var,
        check they are env and then export them.
        :param contents: search here.
        :return: unqiue env to add
        """
        did = {}
        add_to_env = ['REPOROOT']
        for cand in re.finditer(r'(\$[\{\(]?(\w+)[\)\}]?)', contents):
            nm = cand.group(2)
            if nm not in did:
                if has_env(nm):
                    add_to_env.append(nm)
                did[nm] = True
        return add_to_env

    def __modify_config_contents(self, contents):
        """
        Replace contents of config file with reference to config.xxx scopes.
        :return: modified contents.
        """
        contents = re.sub(r'(^|\W)config_name(\W)', r'\1config.name\2', contents)
        contents = re.sub(r'(^|\W)CWD($|\W)', r'\1config.CWD\2', contents)
        contents = re.sub(r'(^|\W)env(\W)', r'\1config.env\2', contents)
        contents = re.sub(r'(^|\W)rof_signature_exclude(\W)', r'\1config.rof_signature_exclude\2', contents)
        contents = re.sub(r'(^|\W)rof_testcnt_by_signature(\W)', r'\1config.rof_testcnt_by_signature\2', contents)
        contents = self.__include_sim_include(contents)
        add_env = self.__find_vars_to_export(contents)
        contents = 'from ETdv.dvrun import config\n' f'for e in {add_env}: config.env.append(e)\n' + contents
        debug2('SimToDvPy-3', self.filename, contents)
        return contents

    def __process(self, rootdir, configi):

        __my_globals = {}

        # MODULE = "<class 'module'>"
        # add back in any modules we have here, so config can import them again?
        # [xglobals.update({k: v}) for k, v in globals().items() if str(type(v)) == MODULE]

        try:
            from ETdv.dvrun.main import Main
            self.__reset()
            if 0 < configi:
                if rootdir is not None:
                    outfname = os.path.join(rootdir, f"user_config{configi}.py")
                    with open(self.filename, 'r') as file:
                        info('Cfg-1x', self.filename, outfname)
                        contents = self.__modify_config_contents(file.read())
                    with open(outfname, 'w') as ofs:
                        ofs.write(f"{self.IMPORTS}\n")
                        ofs.write(f"#From config file: {self.filename}\n")
                        ofs.write(f"\ndef user_config{configi}():\n")
                        indent = " "*2
                        for line in contents.splitlines(): ofs.write(f"{indent}{line}\n")
                    return
                else:
                    from user_config import user_config
                    user_config(configi)
            else:
                with open(self.filename, 'r') as file:
                    info('Cfg-1', self.filename)
                    contents = self.IMPORTS + self.__modify_config_contents(file.read())
                    exec(contents, __my_globals)
            self.__update_from_config()
            self.__sanity_check().__check_cwd(config.CWD)
        except MyKeyError as err:
            early_termination_error('Cfg-11', self.filename, err)
        except IOError as err:
            early_termination_error('File-2', self.filename, 'read', err)

    def __check_cwd(self, cwd):
        def xwarn(code, *args):
            warn(code, os.path.abspath(self.filename), *args)

        if cwd is None:
            pass  # no longer require: xwarn('Cfg-9')
        else:
            cfg_fname = os.path.abspath(self.filename)
            cwd = os.path.abspath(cwd)
            if not os.path.samefile(os.path.dirname(cfg_fname), cwd):
                xwarn('Cfg-10', cwd)
        return self

    def __sanity_check(self):
        from ETdv.dvrun.build import Build
        from ETdv.dvrun.test import Test

        def xerror(code, *args):
            error(code, self.filename, *args)

        def name_ok(name, rex='[\w\-]+$'):
            """
            Test if name matches regex.
            :param name: name to test.
            :param rex: rex to use.
            :return: None if no match, else a matcher object.
            """
            return name is not None and re.match(rex, name)

        def check_name(name, xtype):
            if name_ok(name) is None:
                xerror('Name-1', name, xtype)

        if self.__config_name is None: xerror('Cfg-2')
        check_name(self.__config_name, 'config_name')
        if 1 > len(self.__builds): xerror('Cfg-3')
        [xerror('Env-1', var) for var in self.__env if not has_env(var)]
        for build in self.__builds:
            assert isinstance(build, Build)
            [xerror('Cfg-4', attr) for attr in ['name', 'run_cmd'] if build[attr] is None]
            check_name(build['name'], 'build')
            # if 1 > len(build.tests): xerror('Cfg-5', build['name'])
            if 1 > len(build.tests): warn('Cfg-5', self.filename, build['name'])
            for test in build.tests:
                assert isinstance(test, Test)
                [xerror('Cfg-6', test['name'], attr) for attr in ['name', 'run_cmd'] if test[attr] is None]
                check_name(test['name'], 'test')
        if has_errors(): early_termination_error('Cfg-7', self.filename)
        return self


if __name__ == '__main__':
    from ETdv.dvrun.messaging import Messaging

    Messaging('I')
    cfg = ConfigurationFile('/Users/kpfalzer/projects/etdv/test/pipe_unit_test.py')
    pass
