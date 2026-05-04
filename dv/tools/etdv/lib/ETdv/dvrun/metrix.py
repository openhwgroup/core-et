# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Miscellaneous subcommands to deal with metrics database.
"""

import os, sys, json, re, time
from urllib.parse import urlencode
from urllib.request import Request, urlopen
from ETdv.util import (read_file, write_file, split_filename, eprint, basename_parts,
                       hostname, username, rm_empty_values, has_env, getenv)
from ETdv.dvrun.random_gen import RANDOM_SEED_FNAME

if has_env('ETDV_HOST'):
    HOST = getenv('ETDV_HOST')
else:
    HOST = 'http://sc-dv01:3002'

__TIMEOUT = 60  # sec
__NLOOP = 10
__SLEEP = 10  # sec

__STATS_MULT = {
    'ms': 1.0e-03,
    'us': 1.0e-06,
    'ns': 1.0e-09,
    'ps': 1.0e-12,
    'fs': 1.0e-15,
    'seconds': 1.0,
    'kb': 1024, #bad: kb should be kB/KB
    'KB': 1024
}


def _post_request(resource, data):
    """
    :param resource: resource in controller (e.g. 'runs')
    :param data: dict of values.
    :return: dict of POST response.
    """
    assert type(data) is dict
    url = f'{HOST}/{resource}.json'
    request = Request(url, urlencode(data).encode())
    # Seem to get exceptions here.  Lets be stubborn
    for i in range(0, __NLOOP):
        try:
            r = json.loads(urlopen(request, timeout=__TIMEOUT).read().decode())
            return r
        except Exception as e:
            eprint(f'metrix._post_request (iter={i}): ' + str(e))
        time.sleep(__SLEEP)
    return None


def _string_ok(s):
    return 256 > len(s)


def _text_ok(t):
    return 65536 > len(t)


def _create_host_stat(stats):
    """
    Create new HostStats record.
    :param stats: dist of stats.
    :return: id or None.
    """
    stats = stats or {}
    record = {}
    for xfrom, xto in [['username', 'user'], ['hostname', 'name']]:
        if xfrom in stats: record[xto] = stats[xfrom]
    more = {k: v for k, v in stats.items() if k in ['memsize', 'cputime', 'elapsed']}
    data = {**record, **more}

    # TODO: if we add more keys; make sure to update 5
    if 5 != len(data): return 1

    response = _post_request('host_stats', data)
    return response['id']


def _trim_string(kv, str_keys):
    """
    Trim string values in kv (dict) for those keys in str_keys.
    :param kv: dict to process.
    :param str_keys: trim values only for these keys.
    :return: new dict with updated string vals.
    """
    assert type(kv) is dict
    assert type(str_keys) is list
    return {**kv, **{k: v[:255] for k, v in kv.items() if k in str_keys}}


def _create_build(**kwargs):
    data = _trim_string(kwargs, ['config', 'status'])
    response = _post_request('sim_builds', data)
    return response['id']


def _create_test(**kwargs):
    kwargs = rm_empty_values(kwargs)  # in case of no tags
    data = _trim_string(kwargs, ['name', 'signature', 'status', 'tags'])
    response = _post_request('sim_tests', data)
    return response['id']


def _get_run_id(build_dir):
    fn = os.path.join(build_dir, '..', '..', '..', '.run_id')
    run_id = read_file(fn)
    return run_id


def _get_build_id(test_dir):
    fn = os.path.join(test_dir, '..', '..', 'build', '.build_id')
    build_id = read_file(fn)
    return build_id


def _add_result_common(log):
    target_dir = os.path.dirname(log)
    target = os.path.join(target_dir, 'status')
    status = read_file(target)
    return [target_dir, status]


# ﻿e=814.15 s=63.00 u=1599.90 m=4657364kb
__METRIX = re.compile(r'metrix:\s+e=(\S+)\s+s=(\S+)\s+u=(\S+)\s+m=(\d+)(\S+)', re.IGNORECASE)

__SIMTIME = (
    re.compile(r'(time):\s+(\d+)\s+(\w+)', re.IGNORECASE),
    re.compile(r'.*finish at simulation time\s+(\d+)', re.IGNORECASE)
)

__HOSTNAME = re.compile(r'metrixHostname:\s*(\S+)', re.IGNORECASE)


def _get_stats_from_log(fname, do_simtime=False):
    # use short hostname, since otherwise might hang on network
    stats = {'username': username()}
    if fname.endswith('.gz'):
        import gzip
        open_fn = gzip.open
    else:
        open_fn = open
    with open_fn(fname, 'rt') as ifs:
        for line in ifs:
            line = line.strip()
            if 'hostname' not in stats:
                m = __HOSTNAME.search(line)
                if m:
                    stats['hostname'] = m.group(1)
            m = None
            if 'cputime' not in stats:
                m = __METRIX.match(line)
                if m:
                    stats['cputime'] = float(m.group(2)) + float(m.group(3))
                    unit = m.group(5).lower()
                    assert unit in __STATS_MULT
                    stats['memsize'] = __STATS_MULT[unit] * float(m.group(4))
                    stats['elapsed'] = m.group(1)
            if not m and do_simtime:
                # we always prefer this simtime (with units)
                m = __SIMTIME[0].match(line)
                if m:
                    unit = m.group(3).lower()
                    assert unit in __STATS_MULT
                    stats['simtime'] = __STATS_MULT[unit] * float(m.group(2))
                elif not m and 'simtime' not in stats:
                    m = __SIMTIME[1].match(line)
                    if m:
                        stats['simtime'] = float(m.group(1))  # unit-less
    if 'hostname' not in stats:
        stats['hostname'] = hostname(opts='-s')
    defaults = {'memsize': 0, 'cputime': 0, 'elapsed': 0}
    return {**defaults, **stats}


def _get_host_stats(log, do_simtime=False):
    stats = _get_stats_from_log(log, do_simtime=do_simtime)
    return stats


def _add_build_result(log):
    metrix = _read_metrix(log)
    host_stat_id = _create_host_stat(metrix['host_stats'])
    target_dir = os.path.dirname(log)
    run_id = _get_run_id(target_dir) or 1
    if host_stat_id is None: return
    build_id = _create_build(
        run_id=run_id,
        config=metrix['config'],
        status=metrix['status'],
        signature=metrix['signature'],
        host_stat_id=host_stat_id
    )
    fn = os.path.join(target_dir, '.build_id')
    write_file(fn, build_id)


def _add_test_result(log):
    metrix = _read_metrix(log)
    host_stat_id = _create_host_stat(metrix['host_stats'])
    target_dir = os.path.dirname(log)
    build_id = _get_build_id(target_dir) or 1
    if host_stat_id is None: return
    test_id = _create_test(
        name=metrix['name'],
        status=metrix['status'],
        host_stat_id=host_stat_id,
        sim_build_id=build_id,
        signature=metrix['signature'],
        simtime=metrix.get('simtime', 0.0),
        cps=metrix['cps'],
        tags=_get_tags_from_testme(target_dir)
    )


TAGS = re.compile(r'#TAGS:\s+(\S+)')


def _get_tags_from_testme(target_dir, testme='testme'):
    fn = os.path.join(target_dir, testme)
    if not os.path.exists(fn): return None
    tags = None
    with open(fn, 'rt') as ifs:
        for line in ifs:
            m = TAGS.match(line.strip())
            if m:
                tags = m.group(1)
                break
    return tags


def _get_status_signature(xstatus):
    if xstatus is None: return None, None
    m = re.match(r'(\S+)(\s+(.+))?', xstatus)
    if m:
        status, signature = m.group(1), m.group(3) or ''
    else:
        status, signature = 'unknown', ''
    return status, signature


def _create_dict(**kvs):
    return rm_empty_values(kvs)


def _get_random_seed():
    runroot = getenv('ETDV_RUNROOT')
    if runroot is None: return 0
    # at this point, ETDV_RUNROOT points to /.../makefile; but file one up
    fname = os.path.abspath(os.path.join(runroot, '..', RANDOM_SEED_FNAME))
    return int(read_file(fname) or 0)


def _get_test_metrix(log):
    target_dir, status = _add_result_common(log)
    status, signature = _get_status_signature(status)
    name = os.path.basename(target_dir)
    stats = _get_host_stats(log, do_simtime=True)
    cps = float(None or 0)  # todo
    return _create_dict(
        name=name,
        status=status,
        signature=signature,
        simtime=stats.get('simtime', 0.0),
        tags=_get_tags_from_testme(target_dir),
        host_stats=stats,
        cps=cps
    )


def _get_build_metrix(log):
    target_dir, status = _add_result_common(log)
    status, signature = _get_status_signature(status)
    config = '.'.join(split_filename(log)[-4:-2])
    stats = _get_host_stats(log)
    return _create_dict(
        config=config,
        status=status,
        signature=signature,
        host_stats=stats
    )


__METRIX_FNAME = '.metrix.json'


def _get_metrix_fname(log):
    return os.path.join(os.path.dirname(log), __METRIX_FNAME)


def _read_metrix(log):
    ifn = _get_metrix_fname(log)
    with open(ifn, 'r') as infile:
        metrix = json.load(infile)
    return metrix


def _create_metrix(argv):
    assert 1 == len(argv)
    log = argv[0]
    xtype = basename_parts(log)[0]
    assert xtype in ['buildme', 'testme']
    if xtype == 'buildme':
        metrix = _get_build_metrix(log)
    else:
        metrix = _get_test_metrix(log)
    ofn = _get_metrix_fname(log)
    with open(ofn, 'w') as outfile:
        json.dump(metrix, outfile)


def _add_result(argv):
    """
    Process result and post to database.
    :param argv: script pathname (to testme or buildme)
    :return: None
    """
    assert 1 == len(argv)
    log = argv[0]
    xtype = basename_parts(log)[0]
    assert xtype in ['buildme', 'testme']
    if xtype == 'buildme':
        _add_build_result(log)
    else:
        _add_test_result(log)


def create_run(project=None, brief=None, detail=None, ntests=None, rndseed=None):
    assert _string_ok(brief) and _text_ok(detail) and _string_ok(project)
    ntests = ntests or 0
    try:
        response = _post_request('runs',
                                 {
                                     'brief': brief,
                                     'detail': detail,
                                     'project': project,
                                     'expected': ntests,
                                     'rndseed': rndseed
                                 })
        return response['id']
    except:
        bad = sys.exc_info()[0]
    return None


def main(argv):
    program = os.path.basename(argv[0])
    if 2 > len(argv):
        eprint(f'Error: {program}: requires subcommand')
        sys.exit(1)
    subcmd = argv[1]
    argv = argv[2:]
    if subcmd == 'upload':
        _add_result(argv)
    elif subcmd == 'create':
        _create_metrix(argv)
    else:
        assert False, f'{subcmd}: invalid subcommand'
