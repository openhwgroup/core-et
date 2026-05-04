# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
We'll work with Snps Execution Manager (eman)
"""

from ETdv.dvrun.messaging import (info, early_termination_error)
from ETdv.util import (basename_parts, ddmmmyyyy_hhmmss)
from os import (path, makedirs)


class EMan:
    def __init__(self, main):
        assert not EMan.__the_one
        self._main = main
        self._configs = None
        self._outdir = None
        EMan.__the_one = self

    def process(self):
        self._configs = self._main.parse_configurations()
        if self._configs is None: early_termination_error('Eman-1')
        if self._outdir is None:
            self._outdir = path.join(self._main.options['--rootdir'], 'emgr')
        self.to_emc()

    @property
    def outdir(self):
        return self._outdir

    @staticmethod
    def create(main):
        return EMan(main)

    @staticmethod
    def the_one():
        return EMan.__the_one

    def to_emc(self):
        [self._to_emc(config) for config in self._configs]

    # Private
    __the_one = None

    def _to_emc(self, config, sfx='.emc'):
        """
        Save configurations in .emc format.
        :param config: config file to convert.
        :return: self
        """
        makedirs(path.abspath(self.outdir), exist_ok=True)
        ofname = basename_parts(config.filename)[0]
        ofname = path.join(self.outdir, f'{ofname}{sfx}')
        info('Cfg-8', ofname)
        with open(ofname, 'w') as ofs:
            ofs.write('# Generated on {}\n# from {}\n\n'.format(ddmmmyyyy_hhmmss(), path.abspath(config.filename)))
            ofs.write(f"config_name: {config.config_name}\n")
            [self._dump_build(build, ofs) for build in config.builds]

    INDENT = ' ' * 4

    def _dump_build(self, build, ofs):
        REQUIRED = ['name', 'run_cmd']
        ofs.write(f"\nbuilds:\n  - name: {build['name']}\n")
        ofs.write(f"{EMan.INDENT}run_cmd: {build['run_cmd']}\n")
        [ofs.write(f"{EMan.INDENT}{k}: {v}") for k, v in build.config.items() if k not in REQUIRED]
        [self._dump_test(build['name'], test, ofs) for test in build.tests]

    def _dump_test(self, build_name, test, ofs):
        REQUIRED = ['name', 'run_cmd']
        ofs.write(f"\ntests:\n  - name: {test['name']}\n")
        ofs.write(f"{EMan.INDENT}build: {build_name}\n")
        ofs.write(f"{EMan.INDENT}run_cmd: {test['run_cmd']}\n")
        [ofs.write(f"{EMan.INDENT}{k}: {v}") for k, v in test.config.items() if k not in REQUIRED]
