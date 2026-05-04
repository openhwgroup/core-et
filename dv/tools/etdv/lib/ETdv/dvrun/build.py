# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from ETdv.constraineddict import ConstrainedKeyDict
from ETdv.dvrun.test import Test


class Build:
    """
    Encapsulate a specific configuration of DUT with a collection of Test.
    """

    VALID_KEYS = ['name',
                  'run_cmd', 'check_cmd', 'slurmopts', 'test_slurmopts',
                  # add rof (rerun on fail) options
                  'rof_run_cmd', 'rof_check_cmd', 'rof_slurmopts', 'rof_test_slurmopts',
                  ]

    def __init__(self, config=None, **kwargs):
        self.__config = ConstrainedKeyDict(Build.VALID_KEYS, config, **kwargs)
        self.__tests = []

    def to_dict(self):
        return {
            **self.config,
            **{'tests': [t.config for t in self.tests]}
        }

    @property
    def tests(self): return self.__tests

    @property
    def config(self): return self.__config

    def __getitem__(self, name):
        """
        Convenience for accessing config items (attributes).
        :param name: attribute name.
        :return: attribute value or None (if attribute does not exist).
        """
        return self.__config.get(name, None)

    def __setitem__(self, key, value):
        """
        Set config attribute value.
        :param key: attribute name.
        :param value: (new) attribute value.
        :return: new attribute value.
        """
        self.__config[key] = value
        return self.__config[key]

    def add_test(self, test):
        assert isinstance(test, Test)
        self.__tests.append(test)
        return self
