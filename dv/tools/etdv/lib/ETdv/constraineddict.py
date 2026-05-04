# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
dict with limited keys
"""


class MyKeyError(BaseException):
    def __init__(self, key, valid_keys):
        super().__init__(f"{key}: invalid key.  Valid keys: {'|'.join(valid_keys)}")


class ConstrainedKeyDict(dict):
    def __init__(self, valid_keys=None, init=None, **kwargs):
        assert valid_keys is not None and isinstance(valid_keys, list)
        if init is not None: super().__init__(init)
        super().update(kwargs)
        self._valid_keys = valid_keys
        for key in self.keys():
            if key not in self._valid_keys: raise MyKeyError(key, self._valid_keys)

    def __getitem__(self, key):
        if key in self._valid_keys: return super().__getitem__(key)
        raise MyKeyError(key, self._valid_keys)

    def __setitem__(self, key, value):
        if key in self._valid_keys:
            super().__setitem__(key, value)
            return self
        raise MyKeyError(key, self._valid_keys)


if __name__ == '__main__':
    mydict = ConstrainedKeyDict(valid_keys=['dog', 'cat'])
    mydict['cat'] = 6
    mydict['cat']
