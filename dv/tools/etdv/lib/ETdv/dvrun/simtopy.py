# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Convert .sim to .etdv.py
"""
import re, os, copy
from ETdv.ostream import OStream
from ETdv.dvrun.messaging import (info, warn, debug, debug2, error,
                                  early_termination_error, EarlyTerminationException)
from ETdv.util import (getenv, flatten, basename_parts, as_list, replace_vals,
                       unique, delete_file, rm_prefix, has_env)
from ETdv.dvrun.simfile import Simfile
from ETdv.dvrun import config
from ETdv.dvrun.build import Build
from ETdv.dvrun.test import Test
from ETdv.dvrun.config import add_build


# We'll keep global bookeeping here
class _Bookee:
    __the_one = None

    class State:
        def __init__(self):
            self.config_name = None
            self.build_by_name = {}
            self.builds = []
            self.globals = None
            self.depth = 0

    def __init__(self):
        if _Bookee.__the_one:
            raise AssertionError('Cannot create another singleton')
        self.state = self.State()
        self.uniqi = 0
        self.test_names = {}
        _Bookee.__the_one = self

    def reset(self):
        _Bookee.state = self.State()

    def _close_scope(self):
        assert 0 < self.state.depth
        self.state.depth -= 1
        assert 0 < len(self.state.builds)
        self.state.builds.pop(-1)
        if 0 == self.state.depth:
            self.state.globals = None

    @staticmethod
    def _the_one():
        if not _Bookee.__the_one:
            _Bookee()
        return _Bookee.__the_one


# just like html
SPACE_META = '&nbsp;'


def _split_cmd_args(scmd):
    rex = re.compile(f'(.+)({SPACE_META}.+)?')
    m = rex.match(scmd)
    cmd, args = m.group(1), (m.group(2) or '')
    return [cmd, args]


def _repl_escaped_space(line):
    return re.sub(r'\\\s+', SPACE_META, line)


def __replace_space(line):
    return line.replace(SPACE_META, '\\ ')


def __bookee():
    return _Bookee._the_one()


def __get_state():
    return __bookee().state


def sim_config_name(name):
    if not __get_state().config_name:
        __get_state().config_name = name
        config.name = name


def __uniq_testname(name):
    if name in __bookee().test_names:
        __bookee().uniqi += 1
        name = f'{name}_u{__bookee().uniqi}'
        assert name not in __bookee().test_names
    __bookee().test_names[name] = True
    return name


def __new_build(name, mk_dir):
    clean = ''  # ''clean'  # always need to clean
    run_cmd = f"make -C ${{REPOROOT}}/{mk_dir} {clean} VCS"
    check_cmd = 'et-dvrun-check-log build $LOG $TARGET ${ETDV_SHOW_FAIL}'
    build_dir = os.path.join('${REPOROOT}', mk_dir, 'build', 'vcs')
    return [Build(name=name,
                  run_cmd=run_cmd,
                  check_cmd=check_cmd), build_dir]


def sim_build_name(name, mk_dir):
    """
    Find/create new build.
    :param name: build name.
    :param mk_dir: make directory for build.
    :return: None
    Side effect: push build to builds[-1].
    """
    build_by_name = __get_state().build_by_name
    if name not in build_by_name:
        build = __new_build(name, mk_dir)
        add_build(build[0])
        build_by_name[name] = build
    __get_state().builds.append(build_by_name[name])


def __globals():
    return __get_state().globals


def sim_get_global():
    if 1 > __get_state().depth:
        __get_state().globals = SimToPy.Globals()
    __get_state().depth += 1
    return __get_state().globals


def sim_mark_include(stage, incl):
    # does nothing: since we inline/replace
    debug2('Debug-2', f'sim_mark_include({stage}, {incl})')
    pass


def sim_include(fname):
    # does nothing but marks begin of sim-include
    pass


def __as_exports(map=None):
    if not map: map = __globals()
    return Simfile.get_options_as_exports(map)


def __as_plusargs(map):
    return Simfile.get_opts_as_plusargs(map)


def __args_to_map(args):
    map = {}
    tag = re.search(r'(^|\s+)tag=(.+)', args)
    if tag:
        val = as_list(tag.group(2).replace('tag=','').split(' '))
        map['tag'] = val
        args = args.replace(tag.group(0), '')
    for k, v in [kv.split('=') for kv in args.split(' ')]:
        assert k not in map
        map[k] = v
    return map


def __replace_vals(val, merged):
    return replace_vals(val, lambda key: (key in merged and merged[key]) or 'None')


def __get_testname(merged, test_name):
    if 'alias' in merged:
        alias = merged['alias']
        # Handle case where we included file and alias in global: but its value not defined
        test_name = __replace_vals(alias, merged)
    test_name = __uniq_testname(test_name)
    # dont allow funny chars
    return re.sub(r'\W+', '_', test_name)


__TEST_CHECK_CMD = "et-dvrun-check-log test $LOG $TARGET ${ETDV_SHOW_FAIL}"


def __get_xsim_cmd(cmd, xtype='pre'):
    cmd = as_list(cmd)
    if 2 > len(cmd): cmd.append('')
    script, args = cmd[0], ' '.join(cmd[1:])
    args = __replace_space(args)
    script = Simfile.rm_simlists(script)
    script_cmd_line = f"{script} {args}".replace('\\', ' ')
    ln_cmd = ""
    m = re.match(r'.+/scripts/', script_cmd_line)
    if m:
        ln_cmd = f"/bin/ln -fs {m.group(0)[:-1]}"
    else:
        # cya: the .sim magic continues to astound me
        lndir = '${testPath}/scripts'
        #NOPE: lndir = f"{os.path.dirname(script)}/../../scripts"
        ln_cmd = f"if [ -d {lndir} ]; then /bin/ln -fs {lndir} ; fi"
    #TODO: since we added testPath and testRunDir: we dont need ln ?
    #we need ln_cmd!  ln_cmd = ''
    # We allow precmd to log here, since it is required to run/pass test.
    # We send postCmd into another log to not pollute test run log w/ its nonsense.
    if xtype is 'pre':
        the_cmd = f"{ln_cmd}\n" \
                  f"{script_cmd_line}"
    else:
        post_log = './postCmd.log'
        the_cmd = f"rm -f {post_log}; ( "
        if 0 < len(ln_cmd):
            the_cmd += f"{ln_cmd} && "
        the_cmd += f"{script_cmd_line} ) &> {post_log}"
    xcmd = \
        f"#\n#Make sure to setup {xtype}SimCmd in ETDV_RUNDIR\n#to keep test-related artifacts in the right place\n#\n" \
        "pushd ${ETDV_RUNDIR}\n" \
        f"{the_cmd}\n" \
        "popd\n" \
        f"#End of {xtype}SimCmd\n#\n"
    return xcmd


def __get_presim_cmd(cmd):
    return __get_xsim_cmd(cmd)


def __get_postsim_cmd(cmd):
    return __get_xsim_cmd(cmd, xtype='post')


SIMV_EXTRA_OPTS = ' +vcs+lic+wait'

def sim_add_test(test_args, altname):
    debug2('Debug-2', f'sim_add_test: test_args={test_args}, altname={altname}')
    build, path = __get_state().builds[-1]
    args = __args_to_map(test_args)
    build_exe = args['test']
    del args['test']
    merged = {**__globals(), **args}
    # replace spacing
    merged = {
        k: (type(v) is list and [__replace_space(i) for i in v]) or __replace_space(v)
        for k, v in merged.items()
    }
    run_cmd = __as_exports(merged)
    run_cmd += Simfile.get_options_as_exports({
        'testPath': os.path.dirname(os.path.dirname(path)),
        'testRunDir': '${ETDV_RUNDIR}'
    }, keep = r'.*')
    if 'preSimCmd' in merged: run_cmd += __get_presim_cmd(merged['preSimCmd'])
    run_cmd += "cd ${ETDV_RUNDIR}\n"
    sim_exe = os.path.join(path, build_exe)
    run_cmd += sim_exe
    # simply pass in cmdline args to testme using ${*}
    run_cmd += ' ${*} ' + __as_plusargs(merged) + SIMV_EXTRA_OPTS + '\n'
    if 'postSimCmd' in merged: run_cmd += __get_postsim_cmd(merged['postSimCmd'])
    build.add_test(Test(
        name=__get_testname(merged, altname),
        run_cmd=run_cmd,
        check_cmd=__TEST_CHECK_CMD,
        tags=('tag' in merged and merged['tag']) or []
    ))
    pass  #set bkpt here


class SimToPy:
    class Globals(dict):
        def __init__(self):
            super().__init__()

        def __getitem__(self, key):
            return super().__getitem__(key)

        def __setitem__(self, key, value):
            assert type(value) is str
            if re.search(r'\s', value):
                value = value.split(' ')
            if key == 'tag':
                if key not in self.keys():
                    super().__setitem__(key, as_list(value))
                else:
                    self.__getitem__(key).append(value)
                # keep tag as unique/flat list
                super().__setitem__(key, unique(flatten(self.__getitem__(key))))
            else:
                super().__setitem__(key, value)

        def __del__(self):
            _Bookee._the_one()._close_scope()

    def __init__(self, sim_fname):
        self.__sim_fname = os.path.realpath(sim_fname)
        self.__etdv_fname = f'{self.__sim_fname}.etdv.py'
        self.__ofs = None

    @property
    def sim_fname(self):
        return self.__sim_fname

    @property
    def etdv_fname(self):
        return self.__etdv_fname

    @property
    def config_name(self):
        return basename_parts(self.sim_fname)[0]

    def __preamble(self):
        return \
            "from ETdv.dvrun.simtopy import (sim_config_name, sim_build_name,\n" \
            "                                sim_get_global, sim_include,\n" \
            "                                sim_add_test, sim_mark_include)\n" \
            f"sim_config_name('{self.config_name}')\n" \
            f"sim_build_name('{self.get_build_name(self.sim_fname)}', '{self.get_make_dir(self.sim_fname)}')\n" \
            "gv = sim_get_global()"

    def __postamble(self):
        return ""

    def __prefix_path(self, cmd):
        if cmd[0] is '$': return cmd  #dont prefix if starts with $
        mkdir = self.get_make_dir(self.sim_fname)
        ncmd = os.path.join('${REPOROOT}', mkdir, cmd)
        return ncmd

    def __fix_xsim_cmd(self, scmd):
        """
        Make {pre,post}SimCmd relative to REPOROOT
        :param scmd: cmd and optional args
        :return: amended cmd and options args
        """
        cmd, args = _split_cmd_args(scmd)
        ncmd = self.__prefix_path(cmd)
        return f'{ncmd}{args}'

    def translate(self):
        def println(s):
            return self.__ofs.println(s)

        def alt_name():
            return f'{self.config_name}_l{line_number}'

        info('SimToDvPy-1', self.sim_fname, self.etdv_fname)
        delete_file(self.etdv_fname)
        err_cnt = 0
        try:
            self.__ofs = OStream(open(self.etdv_fname, 'w+t'))
            println(self.__preamble())
            with open(self.sim_fname) as ifs:
                line_number = 0
                for line in ifs:
                    line_number += 1
                    line = re.sub(r'\s*=\s*', '=', line)
                    line = re.sub('#.*', '', line).strip()
                    line = re.sub(r'\s+', ' ', line)
                    line = _repl_escaped_space(line)
                    if 0 < len(line):
                        eles = line.split(' ')
                        ele = eles[0]
                        if ele == 'include':
                            println(f"sim_include('{eles[-1]}.etdv.py')")
                        elif re.match(r'test=.*', ele):
                            xline = self.__fix_test_cmd(line)
                            println(f"sim_add_test('{xline}', altname='{alt_name()}')")
                        else:
                            m = re.match(r'([^=]+)=(.+)', line)
                            if not m:
                                error('SimToDvPy-2', self.sim_fname, line_number, line)
                                err_cnt += 1
                            else:
                                key, val = m.group(1), m.group(2)
                                if re.match(r'(pre|post)SimCmd', key):
                                    val = self.__fix_xsim_cmd(val)
                                println(f"gv['{key}'] = '{val}'")
            println(self.__postamble())
        finally:
            if self.__ofs: self.__ofs.close()
        if err_cnt: raise EarlyTerminationException
        return self

    FIX_XCMD_REX = re.compile('(pre|post)SimCmd=(.+)' f'({SPACE_META}|\s)')

    def __fix_test_cmd(self, line):
        """
        Fully qualify {pre,post}Cmd if necessary.
        :param line: replace {pre,post}Cmd here
        :return: line with {pre,post}Cmd replaced.
        """
        m = re.search(self.FIX_XCMD_REX, line)
        if m:
            xtype, xcmd = m.group(1), m.group(2)
            nxcmd = self.__prefix_path(xcmd)
            line = line.replace(f'{xtype}SimCmd={xcmd}', f'{xtype}SimCmd={nxcmd}')
        return line

    @staticmethod
    def get_make_dir(simfile):
        mk_dir = Simfile.rm_simlists(os.path.dirname(os.path.realpath(simfile)))
        return rm_prefix(mk_dir, os.path.realpath(getenv('REPOROOT')))

    @staticmethod
    def get_build_name(simfile):
        mk_dir = SimToPy.get_make_dir(simfile)
        name = re.sub(r'^/?test/', '', mk_dir)
        return re.sub(r'/+', '_', name)

    @staticmethod
    def xmain(simfile, verbose):
        from ETdv.dvrun.messaging import Messaging
        Messaging(verbose)
        if not has_env('REPOROOT'):
            early_termination_error('Env-1', '', 'REPOROOT')
        for sim in simfile:
            SimToPy(sim).translate()


def main():
    import argparse

    parser = argparse.ArgumentParser(description='Convert .sim to .etdv.py.')
    parser.add_argument('--verbose', type=str, default='I', choices=['I', 'W', 'D'], help='message severity')
    parser.add_argument('simfile', type=str, nargs='+',
                        help='one or more .sim file(s)')
    args = parser.parse_args()
    try:
        SimToPy.xmain(simfile=args.simfile, verbose=args.verbose)
    except EarlyTerminationException:
        exit(1)
    exit(0)
