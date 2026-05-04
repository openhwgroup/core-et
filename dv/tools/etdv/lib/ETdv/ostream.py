# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Wrapper around stream to make more conventional (and chain-able)
"""


class OStream:
    def __init__(self, ostr):
        self._os = ostr

    def write(self, s):
        self._os.write(s)
        return self

    def print(self, s):
        if s is None: return self
        return self.write(s)

    def println(self, s):
        if s is None: return self
        return self.print(s + '\n')

    def write_args(self, func, *args, **kwargs):
        func(self, *args, **kwargs)
        return self

    def close(self):
        if hasattr(self, '_os') and self._os:
            self._os.close()

    def __del__(self):
        self.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        return self._os.__exit__(exc_type, exc_val, exc_tb)
