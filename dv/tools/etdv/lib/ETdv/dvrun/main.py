# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re
import signal
import sys
from collections import OrderedDict
from functools import reduce
from os import (path, makedirs)
import os

from ETdv.colortext import ColorText
from ETdv.commander import Commander
from ETdv.dvrun.configfile import ConfigurationFile
from ETdv.dvrun.dvrun import DvRun
from ETdv.dvrun.messaging \
    import (Messaging,
            info, warn, error, debug, print_message_counts,
            early_termination_error, EarlyTerminationException)
from ETdv.dvrun.random_gen import RandomGen
from ETdv.dvrun.simfile import Simfile
from ETdv.dvrun.util import cmdline_as_string
from ETdv.tool import Tool
from ETdv.util import (write_file, getenv, as_list, split_uniq, exec, method_name, is_int)
from ETdv.yargparse import YargParse


class Main:
    VERSION = 'r2.54'
    PROGNAME = DvRun.NAME
    HELP_OPTS = ['-h', '--help']
    ETDV_PY_BUG = getenv("ETDV_PY_BUG")

    __the_one = None

    @staticmethod
    def the_one():
        return Main.__the_one

    @staticmethod
    def after_test_filter(callme):
        from ETdv.dvrun.makefile import Makefile
        return Makefile.the_one().set_after_test_filter(callme)

    @staticmethod
    def get_cmdline():
        return cmdline_as_string(Main.get_cmdline_args())

    @staticmethod
    def get_cmdline_args():
        return Main.the_one().__argv

    def __init__(self, argv, start_msg=True):
        assert Main.__the_one is None  # hack singleton
        Main.__the_one = self
        self.__argv = argv[1:]  # drop progname
        self.__tools = OrderedDict()
        self.__arg_parser = None
        self.__messaging = None
        self.__configurations = None
        self.__make = None
        self.__start_msg = start_msg
        self.__rerun_on_fail = None
        self.rof_signature_exclude = []
        self.rof_testcnt_by_signature = {'.*':1}
        self.__metrix_opts = {
            '--project': None,
            '--group': None,
            '--detail': None
        }

    def check_rof_config(self, builds, tests):
        if self.do_rerun_on_fail(): self.__rerun_on_fail.check_config(builds, tests)
        return

    def set_option(self, opt_nm, val=[]):
        return self.__arg_parser.set_option(opt_nm, val)

    def has_option(self, option):
        return YargParse.has_option(self.__argv, option)

    def parse_args(self):
        self.__arg_parser = YargParse()
        self.__tools.update({
            DvRun.NAME: DvRun(self.__arg_parser),
            'make': self._Make(self.__arg_parser)
            # todo: re-enable later
            # Vcs.NAME: Vcs(self._arg_parser),
            # Verilator.NAME: Verilator(self._arg_parser)
        })
        if 1 > len(self.__argv) or YargParse.has_option(self.__argv, Main.HELP_OPTS):
            print('\n' + self._usage(self.__arg_parser), file=sys.stderr, flush=True)
            sys.exit(1)
        if self.has_option('--version'):
            print(f'{Main.PROGNAME}: {Main.VERSION}')
            sys.exit(0)
        self.__modify_opts().__arg_parser.parse(self.__argv)
        return self

    __SLURMOPTS = '--slurmopts'

    # Massage options for slurm
    def __modify_opts(self):
        if getenv('B4C_SLURM_OPTS') is None: return self
        b4c_slurm_opts = getenv('B4C_SLURM_OPTS').split()
        #lose the --noslurm if it exists
        nargv = list(filter(lambda a: a != '--noslurm', self.__argv))
        if self.__SLURMOPTS in nargv:
            i = nargv.index(self.__SLURMOPTS)
            assert 0 <= i
            slurmopts = nargv[i+1]
            del nargv[i:i+2]
        else: slurmopts = ''
        assert self.__SLURMOPTS not in nargv
        for opt in b4c_slurm_opts: slurmopts += f' {opt}'
        for e in [self.__SLURMOPTS, slurmopts]: nargv.append(e)
        self.__argv = nargv
        return self

    def parse_configurations(self):
        """
        Parse configuration file(s) and simfiles.
        :return: ConfigurationFile(s) or None.
        """
        config_files = self.options['--config']
        if config_files is not None:
            config_files = as_list(config_files)
        else:
            #use default config path
            default_config = getenv('ETDV_DEFAULT_CONFIG')
            if not default_config or not os.path.exists(default_config):
                early_termination_error('Makefile-3', default_config)
            config_files = as_list(default_config)
            warn('Makefile-1', default_config)
        if self.has_option('--zdcf'):
            self.__configurations = self.__generate_debuggable_configs(config_files)
        else:
            self.__configurations = [ConfigurationFile(fname) for fname in config_files]
        sim_files = self.options['--simfile']
        if sim_files is not None:
            sim_files = as_list(sim_files)
            for fname in sim_files: Simfile.process_simfile(fname)
            self.__configurations = self.__configurations or []
            self.__configurations += Simfile.get_configurations()
        return self.__to_json().__configurations

    __USER_CONFIG = 'user_config'

    def __generate_debuggable_configs(self, config_files):
        rootdir = path.join(self.get_makefile_dir(), self.__USER_CONFIG)
        makedirs(rootdir, exist_ok=True)
        do_write = True  #set False: allows us to hack/what-if the generated user_confg*.py files
        rng = range(1,len(config_files)+1)
        for configi in rng:
            fname = config_files[configi-1]
            if do_write: ConfigurationFile(fname, rootdir=rootdir, configi=configi)
        #generate sideband files
        user_config_p1 = "def user_config(i):\n" + \
                         "".join([f"  if i is {i}:\n    {self.__USER_CONFIG}{i}()\n" for i in rng])
        user_config_p2 = "".join([f"from {self.__USER_CONFIG}{i} import {self.__USER_CONFIG}{i}\n" for i in rng])
        user_configs = [self.__USER_CONFIG] + [f"{self.__USER_CONFIG}{i}" for i in rng]
        if do_write:
            top_config = path.join(rootdir, "user_config.py")
            debug("Cfg-11", top_config, "Generating top/main for debugging config file(s)")
            write_file(top_config, f"{user_config_p2}\n{user_config_p1}\n")
            init = path.join(rootdir, "__init__.py")
            debug("Cfg-11", init, "Generating index for debugging config file(s)")
            write_file(init, f"__all__ = {str(user_configs)}\n")
        import sys
        sys.path.insert(0, rootdir)
        return [ConfigurationFile(None, configi=i) for i in rng]

    def __to_json(self):
        json_filename = self.options['--json']
        if json_filename is None: return self
        odata = [d.to_dict() for d in self.__configurations]
        info('Json-1', json_filename)
        from ETdv.jsonwriter import JsonWriter
        JsonWriter(json_filename, odata).write()
        warn('Json-2')
        exit(0)

    def process_options(self):
        """
        Process any target independent options.
        TODO: refactor some of the option processing in Makefile target to here,
        since tags/tests are independent and not related to Makefile target.
        :return: self
        """
        if self.has_option('--timeout'):
            n = self.options['--timeout']
            if not is_int(n) or 0 > int(n):
                early_termination_error('Timeout-1', n)
        if self.do_rerun_on_fail():
            if self.has_option('--json'):
                early_termination_error('Rof-1')
            # we're ok to check just project, since all checked later (group+project+detail)
            if not self.has_option("--project"):
                early_termination_error('Rof-4')
            from ETdv.dvrun.rof import Rof
            self.__rerun_on_fail = Rof(self)
        return self \
            .__init_random() \
            .__slurm_check() \
            .__check_metrix()

    def do_rerun_on_fail(self):
        return self.has_option('--rof')

    def __init_random(self):
        if self.has_option('--seed'):
            try:
                val = self.options['--seed']
                seed = int(val)
                RandomGen(seed)
            except ValueError:
                early_termination_error('RandomGen-1', f'{val}: --seed option is not int')
        else:
            RandomGen()
        return self

    def __slurm_check(self):
        if self.has_option('--slurm') and self.has_option('--noslurm'): early_termination_error('Slurm-1')
        if self.has_option('--noslurm'):
            info('Slurm-2', 'is not')
        else:
            info('Slurm-2', 'is')
        return self

    def __check_metrix(self):
        for k in self.__metrix_opts:
            if self.has_option(k):
                val = self.options[k]
                if k == '--group': val = split_uniq(as_list(val))
                self.__metrix_opts[k] = val
        # all or none
        cnt = reduce(lambda a, b: a + b, map(lambda a: (a and 1) or 0, self.__metrix_opts.values()))
        # debug('Debug-1', f'cnt={cnt}, vals={self.__metrix_opts}')
        if 0 < cnt:
            if 3 != cnt: early_termination_error('Metrix-1')
            if '@' == self.metrix_opts['--detail'][0]:
                cmd = self.metrix_opts['--detail'][1:].replace("'", '')
                val = exec(cmd)
                debug('Debug-1', f'{method_name(self)}: exec({cmd}) returns: {val}')
                self.metrix_opts['--detail'] = val
        return self

    @property
    def collect_metrix(self):
        return self.metrix_opts['--detail'] is not None

    @property
    def metrix_opts(self):
        return self.__metrix_opts

    @property
    def options(self):
        return self.__arg_parser

    @property
    def positional_args(self):
        return self.__arg_parser.positional

    def start_messaging(self):
        logfile = self.options['--logfile']
        if self.__start_msg:
            self.__messaging = Messaging(self.options['--verbose']).the_one()
        else:
            self.__messaging = Messaging.the_one()
            assert self.__messaging
        try:
            info('Log-1', logfile)
            self.__messaging.add_logfile(logfile)
        except IOError as err:
            error('File-2', logfile, 'create', err.strerror)
        return self

    def _usage(self, arg_parser):
        HELP = Main.HELP_OPTS
        usage = 'Usage: {} [{}] options...\n\n'.format(Main.PROGNAME, '|'.join(HELP))
        usage += arg_parser.usage_options()
        return usage

    def cerror(self, msg):
        """
        Error messaging until Messaging() enabled.
        """
        msg = '{}: {}'.format(Main.PROGNAME, str(msg))
        print(ColorText.red(msg), file=sys.stderr, flush=True)

    METHODS_BY_SVR = {
        'I': info,
        'W': warn,
        'E': error
    }

    MESSAGE_CODE_REX = re.compile(r'\s+\((\w+\-\d+)\)\s*$')

    STDERR_WHITELIST = r'^(metrix:|\++)\s+'

    def __subprocess_msg(self, message, is_stderr=False, use_stderr_whitelist=False):
        """
        Process (intercepted) line from subprocess pipe.
        If message matches template, then we strip message body and send to our
        handler.
        :param message: raw message.
        :param is_stderr: True if message was sent to stderr.
        :param use_stderr_whitelist: if is_stderr and message matches STDERR_WHITELIST then
        redirect to stdout instead.  Especially when 'sh -x' prints commands to stderr as
        does 'time command' too.
        """
        message = ColorText.uncolorize(message)
        if re.match(r'.\-\d\d:\d\d:\d\d:\s+', message):
            svr = message[0]
            if self.METHODS_BY_SVR.get(svr) is not None:
                message = message[12:]
                match = self.MESSAGE_CODE_REX.search(message)
                code = match.group(1)
                message = message[:-len(match.group(0))]
                self.METHODS_BY_SVR[svr](code, message)
                return  # all done
        elif is_stderr and use_stderr_whitelist and re.match(self.STDERR_WHITELIST, message):
            is_stderr = False
        if is_stderr:
            error(None, message)
        else:
            info(None, message)

    def __py_bug(self, tmpfn, cmd, full=True, done=False):
        wrapper = path.realpath(path.join(self.get_makefile_dir(), '.fix_pybug'))
        if full:
            #communicate wrapper back to et-dvrun wrapper.
            write_file(tmpfn, wrapper)
            full_cmd = f"#!/bin/csh -fxe\n"
            full_cmd += '# Python threads unreliable: https://bugs.python.org/issue18748\n'
            full_cmd += '# So we mitigate by not running make directly w/in et-dvrun.\n'
            full_cmd += '# And use this wrapper instead.\n'
            full_cmd += f"{cmd}\n"
            mode = 'w+t'
        else:
            full_cmd = f"{cmd}\n"
            mode = 'a+t'
        write_file(wrapper, full_cmd, mode)
        if done: sys.exit(0)

    def __run_command(self, command, intercept_msgs=False, except_plus=False):
        debug('Cmd-1', command)
        if intercept_msgs:
            return Commander(command,
                             stdout=lambda msg: self.__subprocess_msg(msg),
                             stderr=lambda msg: self.__subprocess_msg(msg, True, except_plus))
        else:
            return Commander(command)

    def sigint_handler(self, loop=2, dur=2):
        error('Signal-1', 'SIGINT')
        if self.__make is not None:
            self.__make.send_signal()
            self.__make = None
            cmd = path.realpath(path.join(self.get_makefile_dir(), 'killall'))
            # self._run_command(f'et-dvrun-kill-all {self._get_makefile_dir()}')
            import subprocess
            subprocess.run(cmd.split())
            raise EarlyTerminationException()

    def get_makefile_dir(self):
        return path.realpath(path.join(self.options['--runroot'], 'makefile'))

    def __get_testcount(self):
        tc = None
        mkfile = path.join(self.get_makefile_dir(), 'Makefile')
        with open(mkfile) as ifs:
            for line in ifs:
                m = re.match(r'\w*NTESTS.+(\d+)', line)
                if m:
                    tc = int(m.group(1))
                    break
        return tc

    def run_make(self):
        cmd = f'make -C {self.get_makefile_dir()}'
        ntests = self.__get_testcount()
        if self.collect_metrix:
            xcmd = cmd
            if self.has_option('-n'): xcmd += ' -n'
            xcmd += f' create_metrix_run_id'
            if self.ETDV_PY_BUG:
                self.__py_bug(self.ETDV_PY_BUG, xcmd)
            else:
                info('Make-1', xcmd)
                self.__make = self.__run_command(xcmd, intercept_msgs=True)
                exit_code = self.__make.run()
                if 0 != int(exit_code): raise SystemExit(exit_code)
        if self.has_option('-j'): cmd += f" -j {self.options['-j']}"
        for opt in 'skni':
            if self.has_option(f'-{opt}'): cmd += f' -{opt}'
        cmd += ' ETDV_SHOW_FAIL=1 -k'
        if self.__do_clean(): cmd += ' clean'
        cmd += ' status_now all_tests'
        tee_logs = 1 == ntests and self.has_option('--noquiet')
        if tee_logs:
            cmd += ' ETDV_TEE_LOGS=1'
        if not self.__messaging.lvl_ok('I'): self.__messaging.level = 'I'
        if self.ETDV_PY_BUG:
            self.__py_bug(self.ETDV_PY_BUG, cmd, not self.collect_metrix, True)
        else:
            info('Make-1', cmd)
            self.__make = self.__run_command(cmd, intercept_msgs=True, except_plus=tee_logs)
            exit_code = self.__make.run()
            raise SystemExit(exit_code)

    def __do_clean(self):
        do_clean = not self.has_option('--noclean')
        if do_clean and (self.has_option('--keep_all') or self.has_option('--keep_pass')):
            info('NoClean-1')
            do_clean = False
        return do_clean

    class _Make(Tool):
        NAME = 'make'

        def __init__(self, arg_parser):
            super().__init__(self.NAME, arg_parser)

        def add_options(self):
            opt_group = self.arg_parser().add_group(
                tool=self.NAME,
                name='make',
                title='{}: Options'.format(self.NAME))
            opt_group \
                .add('--make', help='Generate makefile and run it too') \
                .add('--noclean', help='Do not add \'clean\' target when running make') \
                .add('-j njobs', help='Run njobs in parallel') \
                .add('-s', help='Do not echo recipes') \
                .add('-n', help='Do not actually run any commands; just print them') \
                .add('-k', help='Keep going when some targets cannot be made') \
                .add('-i', help='Ignore errors from commands')


def main(argv=sys.argv, start_msg=True):
    minx = Main(argv, start_msg)
    # ' '.join(argv[1:-1])
    try:
        minx \
            .parse_args() \
            .start_messaging() \
            .process_options()
        if 0 < len(minx.positional_args):
            early_termination_error('Args-1', ' '.join(minx.positional_args))
        info('Rev-1', Main.PROGNAME, Main.VERSION)
        # todo: always do makefile for now
        signal.signal(signal.SIGINT, lambda sig, frame: minx.sigint_handler())
        # test-ctrl-c: signal.pause()
        warn('Cmd-2', f"{Main.PROGNAME} {cmdline_as_string(argv[1:])}")
        warn('Git-1', exec('et-dvrun-git-sha'))
        RandomGen.the_one().write_seed(minx.options['--runroot'])
        from ETdv.dvrun.makefile import Makefile
        Makefile.create(minx).process()
        if minx.has_option('--make'):
            minx.run_make()
        print_message_counts()
    except YargParse.ArgException as err:
        minx.cerror(str(err))
        sys.exit(1)
    except EarlyTerminationException as err:
        sys.exit(1)
    except SystemExit as code:
        sys.exit(code)


if __name__ == '__main__':
    main()
