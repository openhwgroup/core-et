# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Interface to lmstat
"""

import sys, re
from ETdv.constraineddict import ConstrainedKeyDict
from ETdv.commander import Commander
from ETdv.licmon.common import Common

_CONFIG = Common.CONFIG['lmstat']

_MESSAGES = {
    'LmStat-4': '{}: (ecode: {}): {} issued, {} used'
}


class LmStat(Common):
    CONFIG_KEYS = ['cmd_common', 'features', 'ssh_cmd']

    def __init__(self, config=None, ssh=False):
        self.__lmstat_result = None
        super().__init__(messages=_MESSAGES, config=config or _CONFIG,
                         message_pfx="LmStat", allowed_keys=self.CONFIG_KEYS, ssh=ssh)

    # Users of VCSRuntime_Net:  (Total of 25 licenses issued;  Total of 23 licenses in use)
    __DETAILS = r'users\s+of\s+(\S+):.+total\s+of\s+(\d+)\s+license.?\s+issued' \
                '.+total\s+of\s+(\d+)\s+license.?\s+in\s+use'

    def __run_lmstat(self, feature):
        self.__feature = self._check_feature(feature).lower()
        if self.__lmstat_result is None:
            cmd = self.config['cmd_common']
            self._run_cmd(cmd)  # calls parse_output
        inuse, issued = 0, 0
        for m in re.finditer(self.__DETAILS, self.__lmstat_result, re.IGNORECASE):
            if self.__feature == m.group(1).lower():
                inuse += int(m.group(3))
                issued += int(m.group(2))
        self.__inuse, self.__issued = inuse, issued
        return self.exit_code, self.__inuse, self.__issued

    def _parse_output(self, args):
        # Look for the magic string
        assert args is None, 'expected args is None'
        self.__lmstat_result = self.stdout

    def __get_usage(self, feature):
        """
        Run lmstat to query feature details.
        :param feature: feature name.
        :param ssh: True to use ssh.
        :return: triplet: lmstat_exit_code, in_use, issued
        """
        ecode, in_use, issue = self.__run_lmstat(feature=feature)
        self._debug(4, feature, ecode, issue, in_use)
        return ecode, in_use, issue

    def get_usages(self):
        self.__lmstat_result = None
        usages = {}
        for feature in self.config['features']:
            ecode, inuse, issued = self.__get_usage(feature)
            usages[feature] = dict(
                ecode=ecode,
                inuse=inuse,
                issued=issued
            )
        return usages


if __name__ == '__main__':
    usages = LmStat(ssh=True).get_usages()
    pass
