# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re

"""
Colored text
"""


# from: https://stackoverflow.com/questions/287871/print-in-terminal-with-colors
class ColorText:
    CEND = '\033[0m'
    CBOLD = '\033[1m'

    CRED = '\033[31m'
    CGREEN = '\033[32m'
    CORANGE = '\033[33m'
    CBLUE = '\033[34m'
    CPURPLE = '\033[35m'
    CCYAN = '\033[36m'
    CLIGHTGREY = '\033[37m'
    CDARKGREY = '\033[90m'
    CLIGHTRED = '\033[91m'
    CLIGHTGREEN = '\033[92m'
    CYELLOW = '\033[93m'
    CLIGHTBLUE = '\033[94m'
    CPINK = '\033[95m'
    CLIGHTCYAN = '\033[96m'

    @staticmethod
    def uncolorize(text):
        return re.sub(r'\033\[\d+m', '', text)  # lose any colorized

    @staticmethod
    def color(color, text):
        return color + text + ColorText.CEND

    @staticmethod
    def green(text):
        return ColorText.color(ColorText.CGREEN, text)

    @staticmethod
    def red(text):
        return ColorText.color(ColorText.CRED, text)

    @staticmethod
    def yellow(text):
        return ColorText.color(ColorText.CYELLOW, text)

    @staticmethod
    def orange(text):
        return ColorText.color(ColorText.CORANGE, text)

    @staticmethod
    def blue(text):
        return ColorText.color(ColorText.CBLUE, text)
