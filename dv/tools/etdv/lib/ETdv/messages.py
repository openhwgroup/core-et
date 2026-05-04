# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from functools import reduce
from datetime import datetime
from ETdv.timer import Timer


class Messages:
    """
    Singleton object to handle messages.
    """

    def __init__(self, messages):
        assert isinstance(messages, dict)
        if Messages.__the_one is None:
            self._msg_cnt_by_severity = {'I': 0, 'W': 0, 'E': 0, 'F': 0, 'D': 0}
            self._severities = self._msg_cnt_by_severity.keys()
            self._msg_by_code = messages
            Messages.__the_one = self
        elif messages is not None:
            Messages.add_messages(messages)

    def add_messages(self, messages):
        if messages is not None:
            assert isinstance(messages, dict), "messages is not dict"
            Messages.the_one()._msg_by_code.update(messages)

    @property
    def msg_cnt_by_severity(self):
        return self._msg_cnt_by_severity

    @staticmethod
    def the_one():
        return Messages.__the_one

    def message(self, severity, code, *args):
        """
        Create message string.
        :param severity: one of 'IWEFD'
        :param code: code as key into msg_by_code
        :param args: arguments corresponding to format {} placeholders.
        :return: formatted message.
        """
        assert severity in self._severities
        self._msg_cnt_by_severity[severity] += 1
        if code is None:
            return args[0]
        else:
            return self._msg_by_code[code].format(*args)

    def has_errors(self, svr=['E', 'F']):
        return 0 < reduce(lambda x, y: x + y, map(lambda k: self.msg_cnt_by_severity[k], svr))

    # Private methods

    __the_one = None


class DetailedMessages(Messages):
    @staticmethod
    def _xformatter(s, e, m, c):
        if c is None:
            return '{}-{}: {}'.format(s, e, m)
        else:
            return '{}-{}: {} ({})'.format(s, e, m, c)

    FORMATTER = lambda s, e, m, c: DetailedMessages._xformatter(s, e, m, c)

    NOW_FORMATTER = lambda s, e, m, c: DetailedMessages._xformatter(
        s,
        datetime.now().strftime('%d%b%y-%H:%M:%S').lower(),
        m, c)

    def __init__(self, messages=None, formatter=NOW_FORMATTER):
        if DetailedMessages.__the_one is None:
            super().__init__(messages)
            self._formatter = formatter
            self._timer = Timer()
            DetailedMessages.__the_one = self
        else:
            self.update(messages)

    def update(self, messages):
        super().add_messages(messages)

    @staticmethod
    def the_one():
        return DetailedMessages.__the_one

    @staticmethod
    def elapsed():
        return DetailedMessages.the_one()._timer.elapsed()

    def message(self, severity, code, *args):
        msg = super().message(severity, code, *args)
        elapsed = self.elapsed()
        return self._formatter(severity, elapsed, msg, code)

    # Private methods

    __the_one = None


# Test
if __name__ == '__main__':
    DetailedMessages({1: 'from messages: hello {}'})
    msg = DetailedMessages.the_one().message('I', 1, 'world')
    # expect assert: DetailedMessages({2: 'ekk {}'})
    print(msg)
    pass
