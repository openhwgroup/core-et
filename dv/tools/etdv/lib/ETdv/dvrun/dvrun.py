# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from ETdv.tool import Tool
from ETdv.util import get_timestamped_suffix, template_ize


class DvRun(Tool):
    NAME = 'et-dvrun'

    def __init__(self, arg_parser):
        super().__init__(self.NAME, arg_parser)

    def add_options(self):
        opt_group = self.arg_parser().add_group(
            tool=self.NAME,
            name='dvrun',
            title='{}: Options'.format(self.NAME))
        default_root = get_timestamped_suffix(self.NAME)
        opt_group \
            .add('--test_order@* filename', help='Specify test order in filename') \
            .add('--rof', help='Rerun fails using signature processor') \
            .add('--status', help='Run detailed status after make completes') \
            .add('--seed N', help='Seed RandomGen() random number generator with N') \
            .add('--max_tests N', help='Run no more than <N> test(s)') \
            .add('--timeout N', help='Kill test if runtime > N min (0=infinite)', default='0') \
            .add('--noquiet', help='In case of one test, print build and test output to screen') \
            .add('--keep_pass', help='Update status=pass build/test after generating make, so these are not rebuilt') \
            .add('--keep_all', help='Update status=? build/test after generating make, so these are not rebuilt') \
            .add('--config@* filename', help='Specify configuration file (Python syntax). Default is $ETDV_DEFAULT_CONFIG') \
            .add('--json filename', help='Generate raw config into filename (json format)') \
            .add('--runroot  dirname=' + default_root, help='Root directory for all runs') \
            .add('--verbose E|W|I|D|D2|D3', default='W', help='Specify verbosity level') \
            .add('--logfile filename', default='{}.log'.format(self.NAME).lower()) \
            .add('--version', help='Print tool version and exit') \
            .add('--tags@* value', usage='[--tags <val>[,<val>]...]...',
                 help='Only keep tests with specified (comma-separated with no spaces) tag <val>') \
            .add('--force_builds@* value', usage='[--force_builds <val>[,<val>]...]...',
                 help='Force builds instead of using the ones provided in testlist (comma-separated with no spaces)') \
            .add('--tags_exclude@* value', usage='[--tags_exclude <val>[,<val>]...]...',
                 help='Only keep tests without specified (comma-separated with no spaces) tag <val>') \
            .add('--onpass CLEAN|KEEP_LOG|RM_LOG|GZIP_LOG', default='KEEP_LOG',
                 help='If build or test pass: CLEAN, KEEP, RM or GZIP logfile') \
            .add('--testme_post script', help='Execute script after test completion') \
            .add('--zlmb delay', default=f'0', help='Delay (mins) during log-monitor (0: disables log monitor)') \
            .add('--znibc', help='Do not prefix build-cmd in makefile with ignore') \
            .add('--zkmof', help='Kill make on fail') \
            .add('--zdcf', help='Generate explicit config file(s) for debug (using PyCharm)') \
            .add('--uarg@* arg', help='Specify user-level arguments available within python config files' \
                                      ' using method get_user_arg() (returns list of arg)') \
            .add('--group@* name', usage='[--group <name>[,<name]...]...',
                 help='Specify group name(s) to categorize tests to be run.') \
            .add('--project name') \
            .add('--detail content', help='Specify detail content for Run.detail dbase.' \
                                          ' If <content> prefixed with @, as in @content, then execute content and use its stdout as' \
                                          ' value for detail content.')
        return self \
            .__add_filter_options() \
            .__add_slurm_options()

    def __add_slurm_options(self):
        """
        Add slurm options.
        :return: self
        """
        opt_group = self.arg_parser().add_group(
            tool='slurm', name='slurm',
            title='Slurm: Options'
        )
        opt_group \
            .add('--noslurm', help='Do not dispatch using Slurm') \
            .add('--slurm', help='Dispatch using Slurm (default)') \
            .add('--reservation name', help="Allocate resource(s) from Slurm reservation <name>") \
            .add('--slurmopts \'opts\'', help="Additional slurm options (e.g. '--mem=10G')")
        return self

    def __add_filter_options(self):
        """
        Add options for {build,test}_{name,cmd}_{enable,disable}
        :return: self
        """
        TEMPLATE = {
            'usage': '[--@bt@_@nc@_@ed@ <regexp>[,<regexp>]...]...',
            'help': 'Only keep @bt@s with @nc@ @xnot@matching <regexp>'
        }
        # fwd declaration for inner methods
        bt, nc, ed = [None] * 3

        def get_usage():
            return template_ize(TEMPLATE['usage'], bt=bt, nc=nc, ed=ed)

        def get_help():
            if ed == 'enable':
                xnot = ''
            else:
                xnot = 'not '
            return template_ize(TEMPLATE['help'], bt=bt, nc=nc, xnot=xnot)

        opt_group = self.arg_parser().add_group(
            tool='filters',
            name='filters',
            title='build/test filters: Options\n' \
                  '      NOTE: <regexp> is a Python style regular expression\n' \
                  '            enclosed in single quotes (\').\n' \
                  '            re.search() used, so <regexp> does not need to be anchored'
        )
        for bt in ['build', 'test']:
            for nc in ['name', 'cmd']:
                for ed in ['enable', 'disable']:
                    opt_nm = f'--{bt}_{nc}_{ed}'
                    usage = get_usage()
                    if opt_nm == '--test_name_enable':
                        opt_nm = f'-t|{opt_nm}'
                        usage = '[-t|' + usage[1:]
                    opt_group.add(
                        f'{opt_nm}' '@* regexp',
                        usage=usage,
                        help=get_help()
                    )
        return self
