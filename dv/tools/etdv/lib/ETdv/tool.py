# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

from ETdv.yargparse import YargParse


class Tool:
    def __init__(self, name, arg_parser):
        """
        Create tool object.
        :param name: name of tool.
        """
        self._name = name
        self._arg_parser = arg_parser
        self.add_options()

    @property
    def name(self):
        return self._name

    def arg_parser(self):
        return self._arg_parser

    def add_options(self):
        raise AssertionError("subclass must implement")


# basic tests
#if __name__ == '__main__':
#    arg_parser = YargParse()
#    dvrun_tool = DvRun(arg_parser)
#    vcs_tool = VcsMx(arg_parser)
#    verilator_tool = Verilator(arg_parser)
#    print(arg_parser.usage_options())
#    pass

