# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from ETdv.tool import Tool


class Verilator(Tool):
    NAME = 'Verilator'

    def __init__(self, arg_parser):
        super().__init__(Verilator.NAME, arg_parser)

    def add_options(self):
        opt_group = self.arg_parser().add_group(
            tool=Verilator.NAME,
            name='verilator',
            title='{}: Options'.format(Verilator.NAME))
        opt_group \
            .add('-sv') \
            .add('--top-module topname', help='Name of top level input module')
