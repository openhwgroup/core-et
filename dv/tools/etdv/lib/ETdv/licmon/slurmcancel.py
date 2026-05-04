# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
slurm cancel stale jobs.
A stale job is one slurm thinks is RUNNING, but is not actually found running on node/server.
"""
import re
from time import sleep
from ETdv.util import exec_with_stderr, eprint


def __exec_cmd(cmd, ssh='ssh sc-dv01'):
    r = exec_with_stderr(f'{ssh} {cmd}')
    if 0 != r.returncode and 0 < len(r.stderr):
        eprint(cmd + ' : stderr: ' + r.stderr)
        return None
    return r.stdout


# JobId=202451 JobName=testme/sc_nightly/nightly_sc_unit_4b_4sb/tests/42_sc_wts_addr_focus_03 UserId=cchung(5061) GroupId=eng(5000) MCS_label=N/A Priority=4294707133 Nice=0 Account=dv QOS=normal JobState=COMPLETED Reason=None Dependency=(null) Requeue=1 Restarts=0 BatchFlag=0 Reboot=0 ExitCode=0:0 RunTime=00:02:00 TimeLimit=UNLIMITED TimeMin=N/A SubmitTime=2019-02-21T13:05:26 EligibleTime=2019-02-21T13:05:26 AccrueTime=Unknown StartTime=2019-02-21T13:05:26 EndTime=2019-02-21T13:07:26 Deadline=N/A PreemptTime=None SuspendTime=None SecsPreSuspend=0 LastSchedEval=2019-02-21T13:05:26 Partition=dv AllocNode:Sid=sc-dv02:16316 ReqNodeList=(null) ExcNodeList=(null) NodeList=SC-SERVER04 BatchHost=SC-SERVER04 NumNodes=1 NumCPUs=1 NumTasks=0 CPUs/Task=1 ReqB:S:C:T=0:0:*:* TRES=cpu=1,node=1,billing=1,license/vcsruntime_net@slurmdb=1 Socks/Node=* NtasksPerN:B:S:C=0:0:*:* CoreSpec=* MinCPUsNode=1 MinMemoryNode=0 MinTmpDiskNode=0 Features=(null) DelayBoot=00:00:00 OverSubscribe=OK Contiguous=0 Licenses=vcsruntime_net@slurmdb Network=(null) Command=/bin/time WorkDir=/data/cchung/repo_scratch/nightly_try0/dvrun/makefile Power=

__SCONTROL_SHOW_JOB = re.compile('JobId=(\d+).+JobName=(\S+).+BatchHost=(\S+).+WorkDir=(\S+)')

__QUERY_CMD = 'scontrol -o show job'


def __get_running():
    running = []
    query = __exec_cmd(__QUERY_CMD)
    if query is None: return running
    for line in query.split('\n'):
        # JobState=RUNNING
        if 0 > line.find('JobState=RUNNING'): continue
        match = __SCONTROL_SHOW_JOB.search(line)
        if match is None: continue
        running.append({
            'id': match.group(1),
            'name': match.group(2),
            'host': match.group(3),
            'wdir': match.group(4)
        })
    return running


def __get_command(ele):
    name = ele['name']
    wdir = ele['wdir']
    ix = name.find('/')
    cmd = wdir + name[ix:] + '/' + name[0:ix]
    return cmd


__COMMANDS_ON_HOST = 'ps -auxww'


def __commands_on_host(host):
    return __exec_cmd(__COMMANDS_ON_HOST, ssh='ssh ' + host)


def __slurm_cancel(id):
    cmd = f'scancel {id}'
    print(f'run: {cmd}')
    ##__exec_cmd(cmd)


def __process():
    running_on_host = {}
    running = __get_running()
    for run in running:
        host = run['host']
        if host not in running_on_host:
            running_on_host[host] = __commands_on_host(host)
        cmd = __get_command(run)
        #only care about buildme|testme at this time
        if not re.search(r'buildme|testme', cmd): continue
        on_host = running_on_host[host] or ''
        if 0 > on_host.find(cmd):
            __slurm_cancel(run['id'])


__PERIOD = 5 * 60


def main():
    import datetime
    while True:
        print(str(datetime.datetime.now()))
        __process()
        sleep(__PERIOD)
