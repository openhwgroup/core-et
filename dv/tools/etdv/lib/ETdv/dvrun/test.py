# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from ETdv.constraineddict import ConstrainedKeyDict


class Test:
    """
    A single test configuration associated with a Build.
    """

    VALID_KEYS = ['name',
                  'run_cmd', 'check_cmd', 'tags', 'timeout', 'slurmopts',
                  #add rof (rerun on fail) options
                  'rof_run_cmd', 'rof_check_cmd', 'rof_timeout', 'rof_slurmopts'
                  ]

    def __init__(self, config=None, **kwargs):
        self.__config = ConstrainedKeyDict(Test.VALID_KEYS, config, **kwargs)

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
