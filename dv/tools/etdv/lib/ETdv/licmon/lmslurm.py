# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

"""
Daemon to control slurm and flexlm licenses.
"""
import os, sys, re
from time import sleep
from ETdv.util import eprint, exec_with_stderr
from ETdv.licmon.common import Common
from ETdv.licmon.lmstat import LmStat
from ETdv.licmon.slurm import Slurm


class LmSlurm:
    @staticmethod
    def main(args):
        LmSlurm(args=args).run()

    _MESSAGES = {
        'LmSlurm-1': "slurm is not tracking feature '{}'"
        ,
        'LmSlurm-2': "no (lmstat) licenses issued for feature '{}'"
        ,
        'LmSlurm-3': "slurm query for feature '{}' exit code is {}"
        ,
        'LmSlurm-4': "{} has unexpected value {}"
        ,
        'LmSlurm-5': "{} slurm penalty for feature '{}' from {} to {}"
        ,
        'LmSlurm-6': "{}: could not set initial slurm count to {}.  Exit now!"
        ,
        'LmSlurm-7': "{}: could not create initial reservation.  Exit now!"
        ,
        'LmSlurm-8': "{}: available slurm licenses changed from {} to {}"
        ,
        'LmSlurm-9': "{}: could read file"
        ,
        'LmSlurm-10': "{}: error while processing: {}"
        ,
        'LmSlurm-11': "{}: reading licenses.json file ..."
        ,
        'LmSlurm-12': "vcs_lic_status reports regression overage: {}"
        ,
        'LmSlurm-13': "set priority {} for jobid={}"
        ,
    }

    __B4C_LICENSES = ['vcsruntime_net']
    __B4C_RESERVE  = 'b4c_reserve'
    __B4C_QOS = 'b4c-push'
    __B4C_HI_PRI   = 199

    def __init__(self, args):
        self.__license = args.l
        self.__period = args.p
        self.__verbose = args.v
        self.__ssh = args.s
        self.__lmstat = LmStat(ssh=self.__ssh)
        self.__slurm = Slurm(ssh=self.__ssh)
        self.__logger = Common._LOGGER
        self.__logger.level = self.__verbose
        self.__logger.messages.update(self._MESSAGES)
        self.__diff_acctg = {}
        self.__b4c_last_jobid = {}
        for l in self.__B4C_LICENSES: self.__b4c_last_jobid[l] = 0

    @property
    def lmstat(self):
        return self.__lmstat

    @property
    def slurm(self):
        return self.__slurm

    def __info(self, code, *args):
        self.__logger.info(f'LmSlurm-{code}', *args)

    def __warn(self, code, *args):
        self.__logger.warn(f'LmSlurm-{code}', *args)

    def __error(self, code, *args):
        self.__logger.error(f'LmSlurm-{code}', *args)

    def __debug(self, code, *args):
        self.__logger.debug(f'LmSlurm-{code}', *args)

    def __read_licenses(self):
        import json
        lics = None
        fname = os.path.realpath(self.__license)
        self.__debug(11, fname)
        try:
            lics = json.load(open(fname, 'rt'))
        except FileNotFoundError as ex:
            self.__error(9, fname)
            exit(1)
        except Exception as ex:
            self.__warn(10, fname, ex)
        return lics

    def __update_features(self, lics):
        self.slurm.config['features'] = lics
        self.lmstat.config['features'] = lics

    def __update(self):
        lics = self.__read_licenses()
        if lics is None: return
        self.__update_features(lics)
        avail = self.lmstat.get_usages()
        used = self.slurm.get_used_license_counts()
        for feature, usage in avail.items():
            if feature in used:
                self.__reconcile(feature=feature,
                                 avail=usage,
                                 used=used[feature],
                                 lic_avail_pcnt=self.slurm.config['features'][feature]
                                 )
            else:
                self.__warn(1, feature)

    def __update_b4c(self):
        for license in self.__B4C_LICENSES:
            pending_jobs = self.slurm.get_pending(self.__B4C_QOS)
            last_id = self.__b4c_last_jobid[license]
            ids_to_set = list(filter(lambda i: i > last_id, pending_jobs))
            if (0 < len(ids_to_set)):
                self.__b4c_last_jobid[license] = ids_to_set[-1]
                self.__info(13, self.__B4C_HI_PRI, ids_to_set)
                self.slurm.modify_priority(ids_to_set, self.__B4C_HI_PRI)

    # Since we mirror, need to account for usages in other slurm controllers
    def __get_other_slurm_used(self, used, host):
        other_used = 0
        for h in filter(lambda hh: hh != host, used['used'].keys()):
            other_used += used['used'][h]
        return other_used

    # verif-3586
    # INFO: Total license consumed by Regressions = 288 of out Total=280
    __VCSRUNTIME_LIC_STATUS = "/home/eng/dvtools/release/vcs_lic_stat/latest/vcs_lic_status"
    # detect: Total license consumed by Regressions = nnn of out Total=mmm
    __TLCONSUME_PATT = re.compile(
        r'consumed\s+by\s+regressions\s*=\s*(\d+)\s+of\s+out\s+total\s*=\s*(\d+)',
        re.IGNORECASE
    )

    def __vcsruntime_penalty(self):
        cmd = f"ssh sc-dv01 {self.__VCSRUNTIME_LIC_STATUS}"
        try:
            cp = exec_with_stderr(cmd)
            if 0 != cp.returncode: return 0
            if 0 < len(cp.stderr): return 0
            m = self.__TLCONSUME_PATT.search(cp.stdout)
            if m is None: return 0
            consumed = int(m.group(1))
            total = int(m.group(2))
            diff = consumed - total
            if 0 > diff: diff = 0
            return diff
        except:
            return 0

    def __reconcile(self, feature, lic_avail_pcnt, avail, used):
        """
        Update accounting reservation to cover any difference between what
        slurm reports used and lmstat shows used.
        :param feature: license feature.
        :param avail: lmstat (actual) details.
        :param used: slurm details.
        :return: None
        """
        if False and feature == "vcsruntime_net":   #disabled once slurm enabled for all licenses
            extra_penalty = self.__vcsruntime_penalty()
            if 0 < extra_penalty: self.__warn(12, extra_penalty)
        else:
            extra_penalty = 0
        total_issued = (0 == avail['ecode']) and avail['issued']
        if total_issued:
            # used keys: ecode,total,used
            bad = list(filter(lambda x: x != 0, (used['ecode'].values())))
            if 0 != len(bad):
                self.__warn(3, feature, bad)
                return
            all_hosts = used['used'].keys()
            create_reservation = feature not in self.__diff_acctg
            if create_reservation:
                self.__diff_acctg[feature] = {}
                for host in all_hosts:
                    self.__diff_acctg[feature][host] = {
                        'penalty_acctg': 0,
                        'slurm_can_use': 0
                    }
                    if not self.slurm.create_lm_acctg_reservation(feature, host):
                        self.__error(7, f"{host}:{feature}")
                        exit(1)
            all_slurm_used = self.__get_other_slurm_used(used, '_')
            for host in all_hosts:
                acctg = self.__diff_acctg[feature][host]
                lic_avail_mult = lic_avail_pcnt / 100.0
                count = int(total_issued * lic_avail_mult) or 1
                if create_reservation or (count != acctg['slurm_can_use']):
                    self.__info(8, f"{host}:{feature}", acctg['slurm_can_use'], count)
                    if not self.slurm.update_license_count(feature, count, host):
                        self.__error(6, feature, count)
                        exit(1)
                    acctg['slurm_can_use'] = count
                slurm_can_use = count
                actual_used = avail['inuse']
                acct_diff = actual_used - all_slurm_used - (avail['issued'] - slurm_can_use)
                if acct_diff < 0: acct_diff = 0
                acct_diff += self.__get_other_slurm_used(used, host)
                #
                # verif-3586
                acct_diff += extra_penalty

                # can't reserve more than we are issued
                if acct_diff > slurm_can_use: acct_diff = slurm_can_use
                prev_diff = acctg['penalty_acctg']
                if acct_diff != prev_diff:
                    if acct_diff > prev_diff:
                        self.__warn(5, 'increase', f"{host}:{feature}", prev_diff, acct_diff)
                    else:
                        self.__info(5, 'decrease', f"{host}:{feature}", prev_diff, acct_diff)
                    if self.slurm.update_lmstat_accounting(feature, acct_diff, host):
                        acctg['penalty_acctg'] = acct_diff
        else:
            self.__warn(2, feature)

    def run(self, lvl=0):
        while True:
            if True: self.__update()
            if False: self.__update_b4c()
            sleep(self.__period)


def main(ssh=False):
    import argparse, sys

    parser = argparse.ArgumentParser(
        description='Manage licenses for slurm.',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-p', type=int, default=30,
                        metavar='period', help='period (secs) to query flexlm')
    parser.add_argument('-v', type=str, default='I', choices=['I', 'W', 'D'],
                        help='message severity')
    parser.add_argument('-s', action='store_true',
                        help='use ssh for lmstat and slurm commands')
    parser.add_argument('-l', type=str, metavar='license', required=True,
                        help='license.json')
    args = parser.parse_args()
    args.s = args.s or ssh
    try:
        LmSlurm.main(args)
    except:
        eprint(sys.exc_info()[0])
        exit(1)
    exit(0)


if __name__ == '__main__':
    main(ssh=True)
