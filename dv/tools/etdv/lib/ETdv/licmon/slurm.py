# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Interface to slurm
"""
import re
from ETdv.util import exec_with_stderr
from ETdv.licmon.common import Common

_CONFIG = Common.CONFIG['slurm']

_MESSAGES = {
    'Slurm-4': "{}: reservation not created"
}


class Slurm(Common):
    CONFIG_KEYS = ['manage_issued_pcnt', 'features', 'ssh_cmd']

    def __init__(self, config=None, ssh=False):
        super().__init__(messages=_MESSAGES, config=config or _CONFIG,
                         message_pfx="Slurm", allowed_keys=self.CONFIG_KEYS, ssh=ssh)

    def update_license_count(self, feature=None, count=0, host=None):
        # sacctmgr -i modify resource where name=VCSCompiler_Net set count=4
        self.__feature = self._check_feature(feature).lower()
        cmd = f'sacctmgr -i modify resource where name={feature} set count={count}'
        return 0 == self._run_cmd(cmd, host=host, args='sacct')

    @staticmethod
    def __get_reservation_name(name, pfx='', sfx=''):
        return pfx + re.sub(r'\W+', '_', name).lower() + sfx

    @staticmethod
    def get_slurm_license_name(name):
        return name.lower() + '@slurmdb'

    def __parse_create_res_output(self, name):
        if not re.search(r'reservation\s+created', self.stdout, re.IGNORECASE):
            raise (self._ParseException(f'{name}: could not create reservation'))

    def __parse_update_res_output(self, name):
        if not re.search(r'reservation\s+updated', self.stdout, re.IGNORECASE):
            raise (self._ParseException(f'{name}: could not update reservation'))

    @staticmethod
    def get_lmstat_accounting_name(license):
        return Slurm.__get_reservation_name(license, pfx='__', sfx='_lm_acctg')

    def create_lm_acctg_reservation(self, license, host, count=0, users='slurm'):
        name = self.get_lmstat_accounting_name(license)
        self.delete_reservation(name, host)
        #todo add host below
        return self.create_reservation(name, host,
                                users=users,
                                licenses=f'{self.get_slurm_license_name(license)}:{count}',
                                startTime='now',
                                duration='UNLIMITED',
                                flags='license_only')

    def delete_reservation(self, name, host):
        clause = f'reservationName={name}'
        #NOTE: we'll force this for now, first time through.
        #Since in use above, we delete and create (and create fails if already exists)
        if True: #0 == self._run_cmd_simple(cmd='scontrol show -o ' + clause, host=host):
            self._run_cmd('scontrol delete ' + clause, args='delete_res', host=host)

    def __append_cmd(self, cmd, **args):
        cmd += ' '.join([f'{k}={v}' for k, v in args.items()])
        return cmd

    def create_reservation(self, name, host, **args):
        cmd = 'scontrol create reservation' \
              + f' reservation={name} '
        cmd = self.__append_cmd(cmd, **args)
        return 0 == self._run_cmd(cmd, host=host, args=['create_res', name])

    def __update_reservation(self, name, host, **args):
        cmd = 'scontrol update ' \
              + f'ReservationName={name} '
        cmd = self.__append_cmd(cmd, **args)
        return self._run_cmd(cmd, host=host, args=['update_res', name])

    def update_lmstat_accounting(self, license, count, host):
        """
        Update reservation accounting count.
        :param license: license reservation to update.
        :param count: count to update.
        :return: True on success.
        """
        name = self.get_lmstat_accounting_name(license)
        # licenses=vcsruntime_net@slurmdb:0
        return 0 == self.__update_reservation(name, host=host,
                                              licenses=self.get_slurm_license_name(license) + f':{count}')

    def get_used_license_count(self, feature=None):
        total, used, ecode = {}, {}, {}
        def update(host, ec):
            total[host] = self.__total
            used[host] = self.__used
            ecode[host] = ec
            self.__total, self.__used = 0, 0
        # % scontrol -o show lic=vcscompiler_net@slurmdb
        # LicenseName=vcscompiler_net@slurmdb Total=4 Used=0 Free=4 Remote=yes
        self.__feature = self._check_feature(feature).lower()
        cmd = f'scontrol -o show lic={self.get_slurm_license_name(feature)}'
        self.__total, self.__used = 0, 0
        self._run_cmd_cb(cmd, lambda host, ec : update(host, ec))
        return ecode, total, used

    # Returns {license1: {ecode: {host1: x, host2: y}, total: {host1: a, host2: b}, used: by_host...}
    def get_used_license_counts(self):
        counts = {}
        for feature in self.config['features']:
            ecode, total, used = self.get_used_license_count(feature)
            counts[feature] = dict(
                ecode=ecode, total=total, used=used
            )
        return counts

    def get_pending(self, qos):
        cmd = f'squeue -h -O jobid -S +i -q {qos} -t PENDING'
        self._run_cmd_simple(cmd)
        return [int(i) for i in self.stdout.split()]


    def modify_priority(self, jobids, priority):
        csv = ','.join(map(lambda i: str(i), jobids))
        cmd = f'scontrol update jobid={csv} priority={priority}'
        self._run_cmd_simple(cmd)

    __DETAILS = re.compile(
        r'licensename\s*=\s*(\S+)@.+total\s*=\s*(\d+)\s*used\s*=\s*(\d+)\s*free\s*=\s*(\d+)',
        re.IGNORECASE
    )

    def _parse_output(self, args):
        if args is None:
            self.__parse_scontrol_output()
        elif args[0] == 'sacct':
            self.__parse_sacct_output()
        elif args[0] == 'create_res':
            self.__parse_create_res_output(args[1])
        elif args[0] == 'update_res':
            self.__parse_update_res_output(args[1])
        elif args[0] == 'delete_res':
            pass
        else:
            assert False

    def __parse_sacct_output(self):
        lines = self.stdout
        if not re.search(r'modified server resource', lines, re.IGNORECASE):
            raise (self._ParseException(f'{lines}'))
        pass

    def __parse_scontrol_output(self):
        line = self.stdout
        m = self.__DETAILS.search(line)
        if m is None: raise (self._ParseException(line))
        feature = m.group(1)
        if feature.lower() != self.__feature:
            raise (self._ParseException(f"found feature {feature} != expected {self.__feature}"))
        self.__total = int(m.group(2))
        self.__used = int(m.group(3))


if __name__ == '__main__':
    if True:
        slurm = Slurm(ssh=True)
        x = slurm.get_pending('b4c_reserve', 'vcscompiler_net')
        print(len(x))
        pass

    if False:
        from random import randrange
        slurm = Slurm(ssh=True)
        for feature in slurm.config['features']:
            count = randrange(9999)
            slurm.update_license_count(feature, count)
            ecode, total, used = slurm.get_used_license_count(feature)
            assert total == count, f'found {total}, expected {count}'
            slurm.create_lm_acctg_reservation(feature)
