# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from ETdv.dvrun.configuration import (Configuration, ConfigurationFile)
from ETdv.dvrun.build import Build
from ETdv.util import is_type


class Task:
    """
    Encapsulate a collection of Build.
    Task is same as Session (snps ExecMan).
    This is a singleton object: i.e., only one instance of Task throughout process lifecycle.
    """

    def __init__(self, config=None):
        if Task.__the_one:
            raise AssertionError('Cannot create another singleton')
        self._config = Configuration.create(config)
        self._builds = []
        self._config_files = []
        Task.__the_one = self

    @staticmethod
    def the_one():
        return Task.__the_one

    def add_build(self, build):
        is_type(build, Build)
        self._builds.append(build)

    def add_config_file(self, config):
        is_type(config, ConfigurationFile)
        self._config_files.append(config)

    @property
    def builds(self):
        """
        Get collection of Build, in order of add().
        :return: list of Build.
        """
        return self._builds

    @property
    def current_build(self):
        return self.builds[-1] if len(self.builds) else None

    @property
    def config_files(self):
        return self._config_files

    # Private methods

    __the_one = None

