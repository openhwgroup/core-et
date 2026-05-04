# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import os
from sys import (stderr)
from ETdv.messages import (Messages, DetailedMessages)
from ETdv.colortext import ColorText


class Logger:
    """
    Log messages for stdout for severity <= 'Warning'.
    Log messages to stderr for severity >= 'Error'.
    Use Messages facility to generate formatted message.
    In case of MultiLogger: be careful to not invoke Messages more than
    once to eliminate double counting of msg_cnt_by_severity.
    """

    def __init__(self, messages, level='I'):
        assert isinstance(messages, Messages)
        self.level = level
        self._messages = messages
        self._lvl_ok_exception = None

    def xprint(self, severity, code, *args, **kargs):
        """
        Test if severity is valid to print.
        If so, then print message and return formatted message.
        :param severity: severity code.
        :param code: message code.
        :param args: arguments to format.
        :param kargs: check_lvl= level to check: default is severity.
        :return: None if not printed (due to severity); else formatted message.
        """
        if 'check_lvl' in kargs:
            check_lvl = kargs['check_lvl']
        else:
            check_lvl = severity
        if not self.lvl_ok(check_lvl):
            return None
        formatted = self.format(severity, code, *args)
        self.message(severity, formatted)
        return formatted

    def eprint(self, *args):
        """
        Print to stderr.
        :param args: arguments to print.
        :return: self
        """
        print(*args, file=stderr, flush=True)
        return self

    def print(self, *args):
        """
        Print to stdout.
        :param args:
        :return: self
        """
        print(*args, flush=True)
        return self

    def format(self, svr, code, *args):
        return self._messages.message(svr, code, *args)

    def message(self, severity, formatted):
        """
        Method to print formatted message.
        :param severity: severity level.
        :param formatted: formatted message.
        :return: self
        """
        if severity not in ('E', 'F'):
            self.print(formatted)
        else:
            self.eprint(formatted)
        return self

    def info(self, code, *args):
        return self.xprint('I', code, *args)

    def warn(self, code, *args):
        return self.xprint('W', code, *args)

    def error(self, code, *args):
        return self.xprint('E', code, *args)

    def fatal(self, code, *args):
        return self.xprint('F', code, *args)

    def debug(self, code, *args):
        """
        Print debug message if current level ok.
        :param code: message code.
        :param args: message arguments.
        """
        return self.xprint('D', code, *args)

    def debug2(self, code, *args):
        """
        Print D2 message if current level ok.
        :param code: message code.
        :param args: message arguments.
        """
        return self.xprint('D', code, *args, check_lvl='D2')

    def debug3(self, code, *args):
        """
        Print D3 message if current level ok.
        :param code: message code.
        :param args: message arguments.
        """
        return self.xprint('D', code, *args, check_lvl='D3')

    @property
    def messages(self):
        return self._messages

    @property
    def level(self):
        return list(Logger.LEVELS)[self._level]

    @level.setter
    def level(self, lvl):
        """
        Set verbosity level.
        :param lvl: symbolic level.
        :return: self
        """
        assert lvl in Logger.LEVELS
        self._level = Logger.LEVELS.index(lvl)
        return self

    LEVELS = ('D3', 'D2', 'D', 'I', 'W', 'E', 'F')

    def lvl_ok(self, to):
        """
        Check if level 'to' is ok to print.
        :param to: symbolic level to check.
        :return: true if ok to print, else false.
        """
        return Logger.LEVELS.index(to) >= self._level


class ConsoleLogger(Logger):
    def __init__(self, messages, level='I'):
        super().__init__(messages, level)

    def format(self, svr, code, *args):
        text = super().format(svr, code, *args)
        # TODO: we dont need this, since message deals with colors.
        # If enabled, we get color codes into logfile
        if False and svr in ('E', 'F'):
            text = ColorText.red(text)
        return text

    def message(self, severity, formatted):
        """
        Method to print formatted message.
        :param severity: severity level.
        :param formatted: formatted message.
        :return: self
        """
        if severity in ('W'):
            self.orange(formatted)
        elif severity in ('E', 'F'):
            self.red(formatted)
        elif severity in ('D'):
            self.blue(formatted)
        else:
            self.print(formatted)
        return self

    def orange(self, text, to=lambda obj, x: obj.print(x)):
        to(self, ColorText.orange(text))
        return self

    def green(self, text, to=lambda obj, x: obj.print(x)):
        to(self, ColorText.green(text))
        return self

    def red(self, text, to=lambda obj, x: obj.eprint(x)):
        to(self, ColorText.red(text))
        return self

    def blue(self, text, to=lambda obj, x: obj.eprint(x)):
        to(self, ColorText.blue(text))
        return self


class FileLogger(Logger):
    """
    Log messages to file.
    """

    def __init__(self, messages, fname, level='I', mode='w'):
        super().__init__(messages, level)
        self._fname = fname
        dir = os.path.dirname(self.file_name)
        if 0 < len(dir):
            os.makedirs(dir, exist_ok=True)
        self._os = open(self.file_name, mode=mode)

    def print(self, *args):
        print(*args, file=self._os, flush=True)

    def eprint(self, *args):
        self.print(*args)

    @property
    def file_name(self):
        return self._fname

    def __del__(self):
        if hasattr(self, '_os') and self._os:
            self._os.close()


class MultiLogger(ConsoleLogger):
    """
    Log messages to any number of Logger.
    """

    def __init__(self, messages, fname=None, level='I'):
        super().__init__(messages, level)
        self._loggers = []
        if fname:
            self.add(self._get_file_logger(fname))

    def _get_file_logger(self, fname):
        return FileLogger(fname=fname, messages=self.messages)

    def add(self, logger):
        if isinstance(logger, str):
            self.add(self._get_file_logger(logger))
        else:
            self._loggers.append(logger)
        return self

    def xprint(self, severity, code, *args, **kargs):
        formatted = super().xprint(severity, code, *args, **kargs)
        if formatted:
            [logger.message(severity, formatted) for logger in self._loggers]


# Basic test
if __name__ == '__main__':
    messages = {1: 'yoyo {}'}
    logger = ConsoleLogger(DetailedMessages(messages))
    logger.error(1, 'world')
    logger.info(1, "cat")
    logger.green('who let the dogs out')
    logger.red('red text')
    logger.warn(1, 'warning message')
    flogger = FileLogger(DetailedMessages(), 'flogger.txt')
    flogger.info(1, 'flogger_info')
    flogger.fatal(1, 'flogger.fatal')
    #
    print('BEGIN: multilogger')
    messages[2] = 'multi2 pvals: p1={} p2={}'
    mlogger = MultiLogger(DetailedMessages())
    mlogger.info(2, 'p1val', 'p2val')
    mlogger.error(1, 'mlogger')
    mlogger.add(FileLogger(messages=DetailedMessages(), fname='flogger.txt'))
    mlogger.info(2, 'p1val##', 'p2val##')
    mlogger.error(1, 'mlogger##')
    pass
