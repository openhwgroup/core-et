# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
General purpose utilities
"""

import collections, sys, os, fcntl, time, re, subprocess
from functools import reduce
from os import (environ, path)
from datetime import datetime
from ETdv.messages import Messages


def get_timestamped_suffix(head):
    return './{}_{}'.format(head, ddmmmyy_hhmmss_us()).lower()


def rm_prefix(path, prefix):
    """
    Remove prefix from path.
    :param path: path with prefix.
    :param prefix: prefix to remove.
    :return: path with prefix removed.
    """
    res = path.replace(prefix, '')
    if 0 < len(res) and res[0] == '/': res = res[1:]
    return res


def flock(fname, age=60, debug=False):
    """
    Create exclusive filelock and wait.
    :param fname: filename to create and lock
    :param age: if fname exists and older than age (sec), remove it first
    :return: True when fname is exclusively locked.
    """
    pid = os.getpid()

    def msg(op):
        print(f'DEBUG: util.flock: pid={pid}: {op} lock on: {fname}')

    if debug: msg('request')
    if os.path.exists(fname):
        try:
            mtime = os.path.getmtime(fname)
            now = time.time()
            if now < (mtime + age):
                os.remove(fname)
        except:  # do nothing (in case file rm'd by other)
            pass
    with open(fname, 'w') as fd:
        fcntl.lockf(fd, fcntl.LOCK_EX)
    if debug: msg('got')
    return True


def as_list(item):
    if not isinstance(item, list): item = [item]
    return item


def eprint(text, flush=True):
    """
    Print text to stderr.
    :param text: text to print.
    :param flush: flush.
    """
    print(text, file=sys.stderr, flush=flush)


def is_type(obj, xtype, throw=True):
    """
    Check that object is a subclass of type.
    :param obj: object to check.
    :param xtype: superclass type.
    :param throw: set True to throw AssertionError if not type.
    :return: True if is subclass; else throw Assertion.
    """
    if isinstance(obj, xtype):
        return True
    elif throw:
        raise AssertionError('Expected type \'{}\': found \'{}\''.format(str(xtype), str(type(obj))))
    else:
        return False


def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        return False


def has_env(key):
    """
    Check if environment var exists.
    :param key: name of var.
    :return: True if var exists, else false.
    """
    return key in environ


def getenv(key, default=None):
    """
    Get value of environment var or default if does  not exist.
    :param key: name of var.
    :param default: default value if does not exist
    :return: value or default.
    """
    return environ.get(key, default)


def setenv(key, val=None):
    environ[key] = str(val)


def ddmmmyy_hhmmss_us(lower=True):
    """
    Get current time.
    :param lower: True to convert to lowercase.
    :return: time as string.

    >>> ddmmmyy_hhmmss_us()
    '14Jun18_110522_64507'
    >>> ddmmmyy_hhmmss_us().lower()
    '14jun18_110533_932575'
    """
    now = datetime.now()
    as_str = now.strftime('%d%b%y_%H%M%S_' + str(now.microsecond))
    if lower: as_str = as_str.lower()
    return as_str


def ddmmmyyyy_hhmmss(lower=False):
    """
    Get current time.
    :param lower: True to convert to lowercase.
    :return: time as string.
    """
    now = datetime.now()
    as_str = now.strftime('%d-%b-%Y %H:%M:%S')
    if lower: as_str = as_str.lower()
    return as_str


def pluralize(n, type, cond=lambda x: 1 < x):
    val = f'{n} {type}'
    if cond(n): val += 's'
    return val


def has_errors():
    return Messages.the_one().has_errors()


def basename_parts(fpath):
    """
    Grab base filename from path.
    :param fpath: file pathname.
    :return: 2 tuple: (basename, .ext)
    """
    return path.splitext(path.basename(fpath))


def flatten(x):
    """
    Flatten lists of list of ...
    :param x: list to flatten.
    :return: single list.
    """
    if isinstance(x, collections.Iterable) and not isinstance(x, str):
        return [a for i in x for a in flatten(i)]
    else:
        return [x]


def unique(*args):
    """
    Return list of unique elements.
    :param args: list of elements (incl. other lists).
    :return: list of unique elements (all duplicates removed).
    """
    eles = flatten(args)
    return list(reduce(lambda a, b: set(a) | set(b), [arg for arg in [eles, eles]]))


def multi_line_strip(lines, remove_blanks=True):
    """
    Strip (leading/trailing) spacing from lines(s).
    :param lines: list/tuple or '\n' separated lines.
    :param remove_blanks: set True to remove blank lines.
    :return: line(s) with (leading/trailing) spacing stripped.
    """
    if isinstance(lines, str): lines = lines.split('\n')
    rval = map(lambda l: l.strip(), lines)
    if remove_blanks:
        rval = filter(lambda line: 0 < len(line), rval)
    rval = '\n'.join(rval)
    return rval


def pluck(obj, keys):
    return [obj[key] for key in keys]


def replace_vals(val, lookup=lambda key: getenv(key, f'${{{key}}}')):
    """
    Replace vals of form ${KEY} using lookup function.
    :param val: value to scan and replace ${KEY}.
    :param lookup: function which returns value for key.
    :return:
    """
    if not isinstance(val, str): return val
    did = []
    ix = 0
    while True:
        match = re.search(r'(\$[\{\(]?(\w+)[\)\}]?)', val[ix:])
        if match is None: break
        var_name = match.group(2)
        if var_name not in did:
            env_val = lookup(var_name)
            val = val.replace(match.group(0), env_val)
            did.append(var_name)
        ix += match.regs[0][0] + 1
    return val


def split_uniq(eles, delim=','):
    """
    Split eles (list) using delim.
    :param eles: list of delim-separated items.
    :param delim: delimiter
    :return: list of unique elements.
    """
    if not isinstance(eles, list): eles = [eles]
    return unique(delim.join(eles).split(delim))


def exec(cmd, pp=lambda r: r.replace('\n', ' ').strip()):
    """
    Execute command and return its stdout.
    :param cmd: command to execute.
    :param pp: optional function to process result before return.
    :return: command stdout.
    """
    sp = subprocess.run(cmd.split(), stdout=subprocess.PIPE)
    sp.check_returncode()  # throw CalledProcessError
    out = sp.stdout.decode('ascii')
    if pp is not None: out = pp(out)
    return out


def exec_with_stderr(cmd):
    """
    Execute command and keep separate stdout and stderr.
    :param cmd: command to execute.
    :return: subprocess.CompletedProcess  (with accessors: returncode, stdout, stderr)
    """
    return subprocess.run(cmd.split(), universal_newlines=True,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)


def method_name(inst=None):
    """
    Return current method name.
    :param inst: class instance or None.
    :return: current method name.
    """
    if inst is not None:
        cls = type(inst).__name__ + '.'
    else:
        cls = ''
    return cls + sys._getframe(1).f_code.co_name


def template_ize(template, **kwargs):
    """
    Process template containing markers of form @k@.
    Replace all occurences of @k@ in template with their values kwargs[k].
    :param template: template with markers.
    :param kwargs: dict of replacement vals.
    :return: template with replaced values.
    """
    subst = template
    for k, v in kwargs.items():
        if v is None: v = ''
        subst = re.sub(f'@{k}@', v, subst)
    return subst


def read_file(fn, mode='rt'):
    val = None
    try:
        with open(fn, mode=mode) as ifs:
            val = ifs.read().strip()
    except:
        pass  # do nothing
    return val


def write_file(fn, data, mode='w+t'):
    with open(fn, mode) as ofs:
        if type(data) is not str: data = str(data)
        ofs.write(data)


def split_filename(fn):
    eles = []
    fn = os.path.abspath(fn)
    while True:
        head, tail = os.path.split(fn)
        if tail is not None and 0 < len(tail):
            eles.append(tail)
        else:
            break
        fn = head
    eles.reverse()
    return eles


def hostname(opts='-f'):
    return exec(f'/bin/hostname {opts}')


def username():
    return exec('/usr/bin/id -un')


def rm_empty_values(kvs, empty=[None]):
    return {k: v for k, v in kvs.items() if v not in empty}


def cross_str(a, b, sep='_'):
    """
    Cross a x b using sep as separator.
    :param a: list of str.
    :param b: list of str.
    :param sep: separator.
    :return: list of cross result.
    """
    rval = []
    for ai in as_list(a):
        for bi in as_list(b):
            rval.append(f'{ai}{sep}{bi}')
    return rval


def filter_using_regexp(item, rex):
    """
    Match item against rex (list) using re.search.
    :param item: item.
    :param rex: regex (scalar or list).
    :return: regex which matched or None.
    """
    rex = as_list(rex)
    for r in rex:
        if r.search(item): return r
    return None


def delete_file(fname):
    try:
        os.remove(fname)
    except OSError:
        pass


def escape_list_eles(val):
    if type(val) is list:
        val = '\\ '.join(val)
    return val

class AnyKeyDict:
    def __init__(self, key_eq_fn=lambda k, any_key: k == any_key or k in any_key):
        self.__kvs = []
        self.__key_eq_fn = key_eq_fn

    def __setitem__(self, key, value):
        pos = self.__find(key)[-1]
        if pos:
            del self.__kvs[pos]
        self.__kvs.append([key, value])

    def __getitem__(self, key):
        val, pos = self.__find(key)[1:]
        if pos is None: raise AssertionError(f'No value found for key={key}')
        return val

    def __contains__(self, key):
        return self.__find(key)[0] is not None

    def values(self):
        return [v for k, v in self.__kvs]

    def keys(self):
        return [k for k, v in self.__kvs]

    def __find(self, key):
        pos = 0
        for k, v in self.__kvs:
            if self.__key_eq_fn(key, k): return [k, v, pos]
            pos += 1
        return [None, None, None]


if __name__ == '__main__':
    r = unique([1, 2, 3, 3, 4, 5] + [1, 2, 3])
    s = unique([1, 2, 3, 3, 2, 2, 5])
    t = unique(1, 2, 3)
    pass
