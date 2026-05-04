# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from ETdv.messages import DetailedMessages
from ETdv.logger import MultiLogger
from ETdv.util import pluralize


class Messaging(MultiLogger):
    _MESSAGES = {
        'Arg-1': '{}: argument to {} is invalid',
        'Args-1': 'unexpected argument(s): {}',
        'Cfg-1': '{}: processing config file ...',
        'Cfg-1x': '{}: reading config file into {} ...',
        'Cfg-2': '{}: \'config_name\' must be specified',
        'Cfg-3': '{}: No builds specified',
        'Cfg-4': '{}: build \'{}\' must be specified',
        'Cfg-5': '{}: build \'{}\' has no tests',
        'Cfg-6': '{}: build \'{}\': test \'{}\' must be specified',
        'Cfg-7': '{}: too many error(s)',
        'Cfg-8': '{}: creating eman config file ...',
        'Cfg-9': '{}: expected local variable CWD to be set',
        'Cfg-10': '{}: value of CWD ({}) should be same as directory name of config file',
        'Cfg-11': '{}: {}',
        'Cmd-1': 'running command: {}',
        'Cmd-2': 'cmdline: {}',
        'Debug-1': '{}',
        'Debug-2': '{}',
        'Eman-1': 'eman flow requires --config option(s)',
        'Env-1': '{}: env \'{}\' is not set in current environment',
        'Etr-1': 'Args passed to actual et-dvrun: {}',
        'Etr-2': '{}: could not find an actual config file',
        'Etr-3': '{}: Found more than one matching config files: {}',
        'Ets-1': 'Args passed to actual et-dvrun-status: {}',
        'Ets-2': 'Could not find any makefile dir(s)',
        'Ets-3': 'Found more than one matching makefile dirs: {}',
        'File-1': '{}: could not {} file',
        'File-2': '{}: could not {} file ({})',
        'Filter-1': '--{} \'{}\' option is not a valid regexp',
        'Filter-2': "After filter(s)/tag(s)/prune: keep {} of {} build(s) and {} of {} test(s) (enable '--verbose D' for details)",
        'Git-1': '{}: git HEAD sha',
        'Json-1': '{}: generating JSON dump of (raw) pre-filtered configuration',
        'Json-2': 'Use of --json option pre-empts further processing',
        'Kill-1': 'kill child processes: iteration {} of {} ...',
        'Log-1': '{}: creating logfile ...',
        'Make-1': 'make command: {}',
        'Makefile-1': '--config option(s) not speicified. Using default: {}',
        'Makefile-2': '{}: creating makefile ...',
        'Makefile-3': "Config file {} doesn't exist!",
        'MaxTests-1': '--max_tests: reduced test count from {} to {}',
        'Metrix-1': 'All of --group, --project and --detail must be specified (or none of them)',
        'Msgs-1': '{}',
        'Name-1': '{}: \'{}\': invalid {} name',
        'NoClean-1': '--keep_pass or --keep_all implies --noclean',
        'RandomGen-1': '{}',
        'Rev-1': '{} {}',
        'RmOnFail-1': '{} file/dir(s) removed from failed build/test(s)',
        'Rof-1': 'Do not specify --json with --rof',
        'Rof-2': '{}: No rof_run_cmd property found for {}',
        'Rof-3': '{}: dump json for signature processor',
        'Rof-4': '--rof option requires all of --project,group,detail options',
        'RunStatus-1': '{}',
        'Script-1': "{}: creating '{}' script file ...",
        'Signal-1': "Signal '{}' processing ...",
        'SimCmd-1': '{}:{}: {}: not supported yet',
        'Simfile-1': '{}: processing ...',
        'SimIncl-1': '{}: processing sim_include ...',
        'SimIncl-2': "{}: Infinite loop detected (enable '--verbose D' for details)",
        'SimToDvPy-1': "Translating: {} -> {}",
        'SimToDvPy-2': "{}:{}: syntax error at '{}'",
        'SimToDvPy-3': "{}: after flatten all includes: BEGIN{{\n{}\n}}",
        'Slurm-1': "Both --slurm and --noslurm specified",
        'Slurm-2': "Dispatch {} using Slurm",
        'Term-1': 'Early termination due to previous error(s)',
        'Test-1': '{} valid test(s) found (relative to --tags) of all {} test(s) considered',
        'TestOrder-1': '{}:{}: invalid test name \'{}\'',
        'Timeout-1': '{}: --timeout value must be integer > 0',
    }

    def __init__(self, severity='I', noerror=False):
        if Messaging.__the_one:
            if noerror:
                Messaging.__the_one.level(severity)
            else:
                raise AssertionError('Cannot create another singleton')
        super().__init__(DetailedMessages(Messaging._MESSAGES), level=severity)
        Messaging.__the_one = self

    def add_messages(self, messages):
        assert isinstance(messages, dict)
        self._MESSAGES.update(messages)

    @staticmethod
    def the_one():
        return Messaging.__the_one

    def add_logfile(self, filename):
        return self.add(filename)

    # Private methods

    __the_one = None


def info(code, *args): return Messaging.the_one().info(code, *args)


def warn(code, *args): return Messaging.the_one().warn(code, *args)


def error(code, *args): return Messaging.the_one().error(code, *args)


def fatal(code, *args): return Messaging.the_one().fatal(code, *args)


def debug(code, *args): return Messaging.the_one().debug(code, *args)


def debug2(code, *args): return Messaging.the_one().debug2(code, *args)


def debug3(code, *args): return Messaging.the_one().debug3(code, *args)


def get_message_count_summary(force=False):
    def msg(name):
        cnt = counts[name[0]]
        if force or 0 < cnt:
            return pluralize(cnt, name)
        else:
            return ''

    counts = DetailedMessages.the_one().msg_cnt_by_severity
    return '; '.join(filter(lambda x: 0 < len(x), map(msg, ['Error', 'Warning', 'Info'])))


def print_message_count_summary(force=True, svr_fn=info, incr_svr_cnt='I'):
    """
    Print message count summary.
    :param force: force message (else, only if message count > 0).
    :param svr_fn: severity message function (info, warn, ...)
    :param incr_svr_cnt: increment this message count if we have >0 count(s).
    """
    cnt_msg = get_message_count_summary(force)
    if 0 < len(cnt_msg):
        assert incr_svr_cnt is not None
        DetailedMessages.the_one().msg_cnt_by_severity[incr_svr_cnt] += 1
        cnt_msg = get_message_count_summary(force)
        # force message leve so we can see summary
        Messaging.the_one().level = incr_svr_cnt
        svr_fn('Msgs-1', cnt_msg)


def print_message_counts():
    counts = DetailedMessages.the_one().msg_cnt_by_severity
    svr = None
    if 0 < counts['E']:
        svr, fn = ['E', error]
    elif 0 < counts['W']:
        svr, fn = ['W', warn]
    if svr is not None:
        print_message_count_summary(svr_fn=fn, incr_svr_cnt=svr)


class EarlyTerminationException(BaseException):
    """
    Encapsulate exception used to indicate early program termination.
    """

    def __init__(self):
        error('Term-1')
        print_message_count_summary(force=True, svr_fn=error, incr_svr_cnt='E')


def early_termination_error(code, *args):
    """
    Generate message and then terminate.
    :param code: message code.
    :param args: message format arguments.
    """
    error(code, *args)
    raise EarlyTerminationException()


if __name__ == '__main__':
    Messaging('I')
    info('File-1', 'filename', 'create')
    warn('File-1', 'ANOTHER', 'some')
    error('File-1', 'yada', 'yada')
    debug('File-1', 'debug', 'debug')
    Messaging()
