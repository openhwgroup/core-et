# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Check log.
"""

import re, os
from ETdv.util import eprint

BUILD_CHECK = {
    'bad': r'(\*\s*No rule to make target)|(g\+\+:\s*error)|(make.+\*\*\*.+error\s+\d+)|(BUILD\s+FAILED)',
    'good': r'(\s*up\s*to\s*date)' \
            '|(BUILD\s+PASSED)' \
            '|(The\s*design\s*hasn.*t\s*changed\s*and\s*need\s*not\s*be\s*recompiled)'
}

TEST_CHECK = {
    'bad': r'(Aborted\s+.core\s+dumped.)|((^|\W)(error|fail)\W)|(TEST\s+FAILED)|(An\s+unexpected\s+termination)',
    'good': r'((INFO\s+testMain:\s+)?TEST\s+PASSED)|(TEST\s+HAS\s+PASSED)',
    # srun: error: Ignoring job_complete for job 616084 because our job ID is 616117
    'bad_allow': r'(srun.+error.+Invalid\s+user\s+id.+No\s+error)|(srun.+error.+ignoring)'
}


def _trim_line(line):
    return re.sub(r'^\s+', '', line)

"""
Provide base class for more advanced log checkers.
"""
class CheckLogBase:
    def __init__(self):
        self.__status_filename = None
        self.__success = True

    def checker(self, log, check, status=None, flags=re.IGNORECASE, type=None):
        assert False, "subclass must implement"

    @property
    def status_filename(self):
        return self.__status_filename

    @status_filename.setter
    def status_filename(self, value):
        assert self.status_filename is None
        self.__status_filename = value

    @property
    def success(self):
        return self.__success

    @success.setter
    def success(self, value):
        self.__success &= value


class CheckLog(CheckLogBase):
    def __init__(self, build_check=None, test_check=None):
        CheckLogBase.__init__(self)  #need to explicitly call base constructor...
        if build_check is None: build_check = BUILD_CHECK
        if test_check is None: test_check = TEST_CHECK
        self.__build_check = build_check
        self.__test_check = test_check

    def checker(self, log, check, status=None, flags=re.IGNORECASE, type=None):
        """
        Check that log does not match any of bad and does match good.
        :param log: log to check.
        :param check: map with bad and good.
        :param status: status file to write.
        :param flags: pass flags to re.search.
        :return: True on pass, else False
        """
        type = (type and f'({type}) ') or ''
        status = self.status_filename or status
        val = 'fail'
        has_bad = False
        is_illegal_bin = False
        skip_line = True
        ifs = open(log, mode='rb')
        for line in ifs:
            line = line.decode('utf-8', errors='ignore')
            if is_illegal_bin:
                if skip_line :
                    skip_line = False
                    continue
                else:
                    val = f'{val[:-1]}: {_trim_line(line)}'
                    break
            if check['bad'] and re.search(check['bad'], line, flags):
                if 'bad_allow' in check and re.search(check['bad_allow'], line, flags):
                    pass  # do nothing
                else:
                    val = f'fail {type}{_trim_line(line)}'
                    has_bad = True
                    # For illegal bin we need second line after [FCIBH] error also. [GPDV-191]
                    if re.search(r'\[FCIBH\]', val, flags):
                        is_illegal_bin = True
                        continue
                    break
            else:
                m = re.search(r'ERROR: bad_signal(\w+)', line, flags)
                if m:
                    val = f'fail_{m.group(1)} {_trim_line(line)}'
                    break
                elif check['good'] is not None and re.search(check['good'], line, flags):
                    val = 'pass'
        ifs.close()
        # Add case where we only checked for bad
        ok = val == 'pass' or (check['good'] is None and not has_bad)
        if ok: val = 'pass'
        if status is not None:
            with open(status, 'w') as ofs:
                ofs.write(val)
        self.success = ok
        return ok

    def __build(self, log, status):
        return self.checker(log, self.__build_check, status)

    def __test(self, log, status):
        return self.checker(log, self.__test_check, status)

    def process(self, argv=None, log=None, check=None, type=None):
        """
        Much overloaded method to process log.
        But... backwards compatible
        Either specify argv triplet or log & check
        :param argv: [type,log,status] triplet
        :param log: logfile
        :param check: check: contains regex for keys: 'good', 'bad' and optional 'bad_allow'
        :return: success or self (depending on usn argv or latter form)
        """
        if argv is not None:
            assert log is None and check is None
            xtype, xlog, status = argv
            m = re.match(r'build|test', xtype)
            assert m is not None, f"'{xtype}' is invalid, must be build|test"
            if type == 'build':
                stat = self.__build(xlog, status)
            else:
                stat = self.__test(xlog, status)
            return stat
        else:
            return self.__xprocess(log=log, check=check, type=type)

    def __xprocess(self, log, check, type):
        """
        Process log using specific test_checker.
        :param log: filename to process.
        :param check: contains regex for keys: 'good', 'bad' and optional 'bad_allow'
                      or: is instance of CheckLogBase.
        :return: self
        """
        assert self.status_filename, "expected status_filename set"
        if self.success:
            # only continue is we are successful
            if isinstance(check, CheckLogBase):
                self.success = check.checker(log=log, check=None, type=type, status=self.status_filename)
            else:
                self.success = self.checker(log=log, check=check, type=type)
        return self


def _usage_error(prognm):
    eprint(f"Usage: {prognm} build|test log_file status_file die_on_fail")
    exit(1)


def log_checker(status=None):
    """
    More general purpose log checker.
    :param status: status file to update
    :return: checker instance to use for chained process(log=..., check=...)
    """
    checker = CheckLog()
    checker.status_filename = status
    return checker


def main(argv, build_check=None, test_check=None):
    """
    Process log_file to generate status_file.
    :param argv: [build|test, log_file, status_file, die_on_fail]
    :param build_check: {'good': regexp, 'bad': regexp, ('bad_allow': regexp)?}
    :param test_check: {'good': regexp, 'bad': regexp, ('bad_allow': regexp)?}
    """
    prognm = os.path.basename(argv.pop(0))
    argc = len(argv)
    if 4 != argc: _usage_error(prognm)
    checker = CheckLog(build_check, test_check)  # use default build and test checker regexp.
    status = checker.process(argv[:3])
    if not status and '0' != argv[-1]: exit(1)
    exit(0)
