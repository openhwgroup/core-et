# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Shared config.
"""
import sys, subprocess, re
from ETdv.util import exec_with_stderr
from ETdv.logger import ConsoleLogger
from ETdv.messages import DetailedMessages
from ETdv.constraineddict import ConstrainedKeyDict


class Common:
    CONFIG = {
        'features': {}  # in license.json now
        ,
        'message_verbosity': 'D'
    }

    CONFIG['lmstat'] = {
        'cmd_common': 'source /home/cad/.cadcshrc; /home/cad/tools/ri/AscentLint/bin/lmstat -a'
        ,
        'features': CONFIG['features']
        ,
        'ssh_cmd': 'ssh sc-dv01'
        ,
    }
    CONFIG['slurm'] = {
        'features': CONFIG['features']
        ,
        #fyi: 21-dec-20020: drop sc-master since me thinks IT screwed somethin up
        #'ssh_cmd': ['ssh sc-slurm', 'ssh sc-master']  # list of clusters
        'ssh_cmd': ['ssh sc-dv01']  # list of clusters
        ,
    }
    _MESSAGES = {
        'RunCmd-1': 'Command: {}'
        ,
    }
    _LOGGER = ConsoleLogger(messages=DetailedMessages(_MESSAGES), level=CONFIG['message_verbosity'])

    _COMMON_MSGS = {
        '1': 'Unexpected exception: {}'
        ,
        '2': 'Parser error: {}'
        ,
        '3': 'Unexpected stderr: {}'
    }

    def __init__(self, messages=None, config=None, message_pfx=None, allowed_keys=None, ssh=False):
        self._LOGGER.messages.update(messages)
        for k, v in self._COMMON_MSGS.items():
            self._LOGGER.messages.update({f'{message_pfx}-{k}': v})
        self.__config = ConstrainedKeyDict(init=config, valid_keys=allowed_keys)
        self.__message_pfx = message_pfx
        self.__ssh = ssh

    def _error(self, code, *args):
        self._LOGGER.error(f'{self.__message_pfx}-{code}', *args)

    def _debug(self, code, *args):
        self._LOGGER.debug(f'{self.__message_pfx}-{code}', *args)

    # stupid-ass sc-master issues banner on login to stderr
    def __get_rid_of_banner(self, s):
        news = ""
        for line in s.split('\n'):
            if 0 < len(line) and '#' != line[0]:
                news += f'{line}\n'
        if 0 < len(news) and '\n' == news[-1]: news = news[0:-1]
        return news

    # Since ssh-cmd can be multiple clusters, use this method for just one at time
    def __run_cmd2(self, xcmd, cmd, args=None, return_after_exec=False):
        self.__stdout, self.__stderr = [], []
        self.__exit_code = 1
        ncmd = f'{xcmd} {cmd}'
        self._LOGGER.debug('RunCmd-1', ncmd)
        try:
            # print(f"DEBUG1")
            cp = exec_with_stderr(ncmd)
            # print(f"DEBUG2:{cp}")
            self.__exit_code = cp.returncode
            self.__stdout = cp.stdout
            self.__stderr = self.__get_rid_of_banner(cp.stderr)
            if return_after_exec: return self.exit_code
            if 0 < len(self.stderr):
                raise (Exception(self.stderr))
            if 0 == self.exit_code:
                if args is not None and not isinstance(args, list): args = [args]
                self._parse_output(args=args)
        except self._ParseException as parse_err:
            self._error(2, parse_err)
        except Exception as err:
            self._error(3, err)
        except:
            self._error(1, sys.exc_info()[0])
        return self.exit_code

    def _run_cmd_cb(self, cmd, cb=None):
        return self._run_cmd(cmd, None, False, cb)

    # Works with __run_cmd2 above
    def _run_cmd(self, cmd, args=None, return_after_exec=False, cb=None, host=None):
        self.__stdout, self.__stderr = [], []
        self.__exit_code = 1
        if host:
            xcmd = f"ssh {host}"
        else:
            xcmd = (self.ssh and self.config['ssh_cmd']) or ""
        if isinstance(xcmd, list):
            #NOTE: quite the hack to run across multiple hosts when host=None
            for xxcmd in xcmd:
                assert "ssh" == xxcmd.split()[0]
                ecode = self.__run_cmd2(xxcmd, cmd, args, return_after_exec)
                # todo: stuff based on result
                if cb is not None:
                    cb(xxcmd.split()[-1], ecode)
        else:
            return self.__run_cmd2(xcmd, cmd, args, return_after_exec)

    def _run_cmd_simple(self, cmd, args=None, host=None):
        if host: cmd = f"ssh {host} {cmd}"
        return self._run_cmd(cmd=cmd, args=args, return_after_exec=True)

    def _parse_output(self, args):
        assert False, "Subclass must implement"

    def _check_feature(self, feature):
        assert feature in self.config['features'], f'{feature}: feature not supported'
        return feature

    @property
    def config(self):
        return self.__config

    @property
    def ssh(self):
        return self.__ssh

    @property
    def stdout(self):
        return self.__stdout

    @property
    def stderr(self):
        return self.__stderr

    @property
    def exit_code(self):
        return self.__exit_code

    class _ParseException(Exception):
        def __init__(self, message):
            super().__init__(message)
