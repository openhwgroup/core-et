# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


#
# How to add a test:
#
#  t(<test_name>, <test_path>, <models_list> [, tags] [, plusargs] [, pre_test] [, post_test])
#
#  test_name        : The name of the ELF created by make. Use an empty string if stimulus is created only by
#                     other means than code run on Minion (e.g. SV/TCL) in which case test_path is used as test name
#  test_path        : The path to the test source and Makefile, relative to $MINION_DIAGS
#  models_list      : The RTL models to run the test on. Choose among neigh/shire/fc/sc
#  tags             : An array of tags used to group multiple tests under a same name.
#                     Note that et-dvrun already has built-in capability to filter tests per name, build,
#                     command with regex so sometimes a tag isnt needed.
#  plusargs         : A string of plusargs to pass to VCS
#  pre_test         : A bash command to execute before the test in the directory where it will be run.
#                     $DIAG_DIR refers to the test's directory (the test_path)
#  post_test        : A bash command to execute at the end of the test in the directory where it ran.
#                     $DIAG_DIR refers to the test's directory (the test_path)
#
# By default, only 1 thread is enabled whatever the build used.
# The following plusargs should be specified in the Plusargs to control the number of threads that will be enabled.
#
# +SHIRE_MASK=0x...    Mask of active shires
# +MINION_MASK=0x...   Mask of active minions per shire
# +THREAD_MASK=0x...   Mask of active threads per minion
#
#

common_test_plusargs = ' +LVDPLL_MODE=RAND +MINION_PRIMARY_CLOCK=RAND '

t('atomics',                    'atomics',                                    ['neigh'    ], tags=['minion_diags', 'b4c_stg1', 'perf_track'], plusargs= '+RECORD=cosim_evt.trace' + common_test_plusargs, post_test='$REPOROOT/dv/common/scripts/run_replay.sh' )
## Charles: run the above "atomics" in "shire" build:
t('atomics',                    'atomics',                                    ['shire'    ], test_name='atomics_in_shire', tags=['minion_diags', 'cye_atomics_in_shire', 'micha'], plusargs= '+RECORD=cosim_evt.trace +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs, post_test='$REPOROOT/dv/common/scripts/run_replay.sh' )

t('t_amocmpswapl_w',            'atomics_with_threads',                       ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('t_amocmpswapg_w',            'atomics_with_threads',                       ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('t_amocmpswapl_d',            'atomics_with_threads',                       ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('t_amocmpswapg_d',            'atomics_with_threads',                       ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )

## Charles: run the above "atomics_xxxx" tests in "shire" build:
t('t_amocmpswapl_w',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapl_w_in_shire', tags=['minion_diags', 'cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('t_amocmpswapl_d',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapl_d_in_shire', tags=['minion_diags', 'cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('t_amocmpswapg_w',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapg_w_in_shire', tags=['minion_diags', 'cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('t_amocmpswapg_d',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapg_d_in_shire', tags=['minion_diags', 'cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
# atomic vector local:
t('t_famoswapl_pi',             'atomics_with_threads',                       ['shire'    ], test_name='t_famoswapl_pi_in_shire',  tags=['minion_diags', 'cye_atomics_in_shire', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
# atomic vector global:
t('t_famoswapg_pi',             'atomics_with_threads',                       ['shire'    ], test_name='t_famoswapg_pi_in_shire',  tags=['minion_diags', 'cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )

## Charles (temp): test modified:
t('t_amocmpswapg_w_v1',         'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapg_w_v1_in_shire',    tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('t_amocmpswapg_w_v2',         'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapg_w_v2_in_shire',    tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('t_amocmpswapg_w_v2',         'atomics_with_threads',                       ['fc2shire' ], test_name='t_amocmpswapg_w_v2_in_fc2shire', tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x100000001 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )

# test t_amocmpswapl_w_in_shire run in single Hart:
t('t_amocmpswapl_w',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapl_w_in_shire_s0m0t0',    tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('t_amocmpswapl_d',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapl_d_in_shire_s0m0t0',    tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('t_amocmpswapg_w',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapg_w_in_shire_s0m0t0',    tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('t_amocmpswapg_d',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapg_d_in_shire_s0m0t0',    tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('t_famoswapl_pi',             'atomics_with_threads',                       ['shire'    ], test_name='t_famoswapl_pi_in_shire_s0m0t0',     tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('t_famoswapg_pi',             'atomics_with_threads',                       ['shire'    ], test_name='t_famoswapg_pi_in_shire_s0m0t0',     tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )

# test modified version:
t('t_amocmpswapl_w_v1',         'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapl_w_v1_in_shire_s0m0t0', tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('t_amocmpswapl_w_v2',         'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapl_w_v2_in_shire_s0m0t0', tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )

# Bug RTLMIN-6469: let test fail without workaround (also enable Cosim/BEMU checker on Atomics in 1-Core test):
t('t_amocmpswapl_w',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapl_w_in_shire_s0m0t0_fail',    tags=['cye_atomics_in_shire'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DNO_WORKAROUND_RTLMIN_6469'" )


t('minion_smoke',               'minion_smoke',                               ['neigh'],     tags=['minion_diags'],        plusargs='+THREAD_MASK=0x3 +MINION_MASK=0x1 ' + common_test_plusargs ) 

## Charles: for postsi_ust: to reduce preloading L1/L2 time: limit to 2 minions (4 harts):
t('t_amocmpswapl_w',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapl_w_in_shire_postsi', tags=['postsi_min_shire_ust'],     plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x00000003 +THREAD_MASK=0x3' + common_test_plusargs )
t('t_amocmpswapl_d',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapl_d_in_shire_postsi', tags=['postsi_min_shire_ust'],     plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x00000003 +THREAD_MASK=0x3' + common_test_plusargs )
t('t_famoswapl_pi',             'atomics_with_threads',                       ['shire'    ], test_name='t_famoswapl_pi_in_shire_postsi',  tags=['postsi_min_shire_ust'],     plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x00000003 +THREAD_MASK=0x3' + common_test_plusargs )
t('t_amocmpswapg_w',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapg_w_in_shire_postsi', tags=['postsi_min_shire_noc_ust'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x00000003 +THREAD_MASK=0x3' + common_test_plusargs )
t('t_amocmpswapg_d',            'atomics_with_threads',                       ['shire'    ], test_name='t_amocmpswapg_d_in_shire_postsi', tags=['postsi_min_shire_noc_ust'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x00000003 +THREAD_MASK=0x3' + common_test_plusargs )
t('t_famoswapg_pi',             'atomics_with_threads',                       ['shire'    ], test_name='t_famoswapg_pi_in_shire_postsi',  tags=['postsi_min_shire_noc_ust'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x00000003 +THREAD_MASK=0x3' + common_test_plusargs )


t('exceptions',                 'exceptions',                                 ['neigh'    ], tags=['minion_diags', 'b4c_stg1', 'postsi_min_shire_ust', 'perf_track'] , plusargs='+RECORD=cosim_evt.trace +ECALL_ENDS_TEST=0' + common_test_plusargs, post_test='$REPOROOT/dv/common/scripts/run_replay.sh' )
t('csr_write',                  'csr_write',                                  ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('csr_write_with_self_check',  'csr_write_with_self_check',                  ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs )
t('gather32_and_scatter32_ps',  'gather32_and_scatter32_ps',                  ['neigh'    ], tags=['minion_diags', 'b4c_stg1'], plusargs= '+RECORD=cosim_evt.trace' + common_test_plusargs, post_test='$REPOROOT/dv/common/scripts/run_replay.sh' )
t('gathers_and_scatters_ps',    'gathers_and_scatters_ps',                    ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('exceptions',                 'exceptions',                                 ['neigh'    ], test_name='exceptions_2T', plusargs='+THREAD_MASK=0x3 +ECALL_ENDS_TEST=0' + common_test_plusargs, tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'])
t('ES-27',                      'jiras/ES-27',                                ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('ES-28',                      'jiras/ES-28',                                ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('hart_esr',                   'hart_esr',                                   ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'], plusargs= common_test_plusargs )
t('rtlmin_6562',                'jiras/rtlmin_6562',                          ['shire'    ], tags=['minion_diags'], plusargs= '+THREAD_MASK=0x3 +MINION_MASK=0xF +trans_timeout=131072 +thread_timeout=5000 ' + common_test_plusargs )
t('RTLMIN-670',                 'jiras/RTLMIN-670',                           ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('RTLMIN-759',                 'jiras/RTLMIN-759',                           ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs )
t('rtlmin_5651',                'jiras/rtlmin_5651',                          ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'] , plusargs='+THREAD_MASK=0x1' + common_test_plusargs )
t('rtlmin-1913',                'jiras/rtlmin-1913',                          ['neigh'    ], tags=['minion_diags', 'postsi_fc_bringup', 'perf_track'] , plusargs='+disable_random_data' + common_test_plusargs )
t('rtlmin_2600',                'jiras/rtlmin_2600',                          ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'] , plusargs='+THREAD_MASK=0x3' + common_test_plusargs )
t('rtlmin_4370',                'jiras/RTLMIN-4370',                          ['neigh'    ], tags=['minion_diags', 'postsi_fc_bringup', 'perf_track'] , plusargs='+allow_bus_err=1' + common_test_plusargs )
t('rtlmin_4814',                'jiras/RTLMIN-4814',                          ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('rtlmin_5705',                'jiras/RTLMIN-5705',                          ['shire'    ], tags=['minion_diags'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0x1FF +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_5842',                'jiras/RTLMIN-5842',                          ['shire'    ], tags=['minion_diags'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
#t('rtlmin_6448',                'jiras/rtlmin_6448',                          ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('amocmpswap',                 'jiras/rtlmin_6469',                          ['neigh'    ], tags=['errata'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('amocmpswap',                 'jiras/rtlmin_6469',                          ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('cacheops',                   'jiras/rtlmin_6469',                          ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('cacheops',                   'jiras/rtlmin_6469',                          ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('tensor',                     'jiras/rtlmin_6469',                          ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('rtlmin_6487',                 'jiras/rtlmin_6487',                         ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' )
t('rtlmin_6488',                 'jiras/rtlmin_6488',                         ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1'  + common_test_plusargs)
t('rtlmin_6489',                 'jiras/rtlmin_6489',                         ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1'  + common_test_plusargs)
t('rtlmin_6520',                 'jiras/rtlmin_6520',                         ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1'  + common_test_plusargs)
t('starve_rq',                   'starvation/starve_rq',                      ['shire'    ], tags=['minion_diags'] , test_count=150, plusargs='+vcs_timelimit=50000 +THREAD_MASK=0x3 +MINION_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_0',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_1',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_2',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_3',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_4',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_5',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_6',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_7',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_8',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
## Charles: following starvation tests rtlmin_6068_9 to xxx_17: Instructions size > 512KB, not suitable for Post-Si NOC_UST (Phase 2) flow;
## Charles: change for POSTSI_UST only: reduce the loop count 250->50 (thus smaller Instructions size), and put them back to Post-Si NOC_UST (Phase 2): 
t('rtlmin_6068_9',              'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_10',             'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_11',             'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_12',             'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_13',             'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_14',             'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_15',             'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_16',             'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('rtlmin_6068_17',             'starvation/rtlmin_6068',                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'postsi_fc_bringup'] ,  plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xff +thread_timeout=3000' + common_test_plusargs )
t('gsc_progress',               'gsc_progress',                               ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs="+THREAD_MASK=0x1" + common_test_plusargs )
t('rvc_comb',                   'rvc_comb',                                   ['neigh'    ], tags=['minion_diags', 'b4c'],  plusargs='+MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('perf_rq',                    'perf_rq',                                    ['shire'    ], tags=['minion_diags'], plusargs= '+MINION_MASK=0x1 +THREAD_MASK=0x3 +SHIRE_MASK=0x1' + common_test_plusargs )
t('loads_and_stores',           'loads_and_stores',                           ['neigh'    ], tags=['minion_diags', 'b4c', 'perf_track'], plusargs= common_test_plusargs )
t('loads_and_stores_bypass',    'loads_and_stores_bypass',                    ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs )
t('loads_and_stores_ps',        'loads_and_stores_ps',                        ['neigh'    ], tags=['minion_diags', 'b4c', 'perf_track'], plusargs= common_test_plusargs )
t('delayed_response',           'delayed_response',                           ['shire'    ], tags=['minion_diags', 'b4c'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xF +THREAD_MASK=0x3' + common_test_plusargs )
t('mcode',                      'mcode',                                      ['neigh', 'cpu_ss_vl', 'cpu_ss_vcs' ], tags=['minion_diags', 'b4c','minion_integration', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= common_test_plusargs )
t('neigh_msgs',                 'neigh_msgs',                                 ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('neigh_msgs_2',               'neigh_msgs_2',                               ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('vmemSV39',                   'vmem/vmemSV39',                              ['neigh'    ], tags=['minion_diags', 'vmem', 'minion_integration'], plusargs= common_test_plusargs )
t('vmemSV48',                   'vmem/vmemSV48',                              ['neigh'    ], tags=['minion_diags', 'vmem', 'minion_integration'], plusargs= common_test_plusargs )
t('pma_cacheops',               'pma/cacheops',                               ['neigh', 'shire'], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('evict_sw',                   'cache_ops/new_impl/evict_sw',                ['neigh'    ], test_name='cacheops_evict_sw',          tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
t('evict_sw',                   'cache_ops/new_impl/evict_sw',                ['neigh'    ], test_name='cacheops_evict_sw_postsi_phs1',          tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('evict_sw',                   'cache_ops/new_impl/evict_sw',                ['neigh'    ], test_name='cacheops_evict_sw_postsi_phs2',          tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('evict_va',                   'cache_ops/new_impl/evict_va',                ['neigh'    ], test_name='cacheops_evict_va',          tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
t('evict_va',                   'cache_ops/new_impl/evict_va',                ['neigh'    ], test_name='cacheops_evict_va_postsi_phs1',          tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('evict_va',                   'cache_ops/new_impl/evict_va',                ['neigh'    ], test_name='cacheops_evict_va_postsi_phs2',          tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('flush_sw',                   'cache_ops/new_impl/flush_sw',                ['neigh'    ], test_name='cacheops_flush_sw',          tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test cacheops_flush_sw, which causes Cosim to fire falsely; hence have to disable Cosim (+CHECKER=0):
t('flush_sw',                   'cache_ops/new_impl/flush_sw',                ['neigh'    ], test_name='cacheops_flush_sw_postsi_phs1',   tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('flush_sw',                   'cache_ops/new_impl/flush_sw',                ['neigh'    ], test_name='cacheops_flush_sw_postsi_phs2',   tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('flush_va',                   'cache_ops/new_impl/flush_va',                ['neigh'    ], test_name='cacheops_flush_va',          tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test cacheops_flush_va, which causes Cosim to fire falsely; hence have to disable Cosim (+CHECKER=0):
t('flush_va',                   'cache_ops/new_impl/flush_va',                ['neigh'    ], test_name='cacheops_flush_va_postsi_phs1',   tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('flush_va',                   'cache_ops/new_impl/flush_va',                ['neigh'    ], test_name='cacheops_flush_va_postsi_phs2',   tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('prefetch',                   'cache_ops/new_impl/prefetch',                ['neigh'    ], test_name='cacheops_prefetch',          tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test below: have to disable Cosim (+CHECKER=0):
t('prefetch',                   'cache_ops/new_impl/prefetch',                ['neigh'    ], test_name='cacheops_prefetch_postsi_phs1',   tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('prefetch',                   'cache_ops/new_impl/prefetch',                ['neigh'    ], test_name='cacheops_prefetch_postsi_phs2',   tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('prefetch',                   'cache_ops/new_impl/prefetch',                ['neigh'    ], test_name='cacheops_prefetch_chicken',  tags=['minion_diags', 'minion_integration', 'perf_track'], plusargs='+min_frontend_clock_gate_disable=1' + common_test_plusargs )
# (CYE) added self-checking in test below: have to disable Cosim (+CHECKER=0):
t('prefetch',                   'cache_ops/new_impl/prefetch',                ['neigh'    ], test_name='cacheops_prefetch_chicken_postsi_phs1',  tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+min_frontend_clock_gate_disable=1 +CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('prefetch',                   'cache_ops/new_impl/prefetch',                ['neigh'    ], test_name='cacheops_prefetch_chicken_postsi_phs2',  tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+min_frontend_clock_gate_disable=1 +CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('lock_va',                    'cache_ops/new_impl/lock_va',                 ['neigh'    ], test_name='cacheops_lock_va',           tags=['minion_diags', 'b4c','minion_integration', 'postsi_min_shire_ust'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('lock_sw',                    'cache_ops/new_impl/lock_sw',                 ['neigh'    ], test_name='cacheops_lock_sw',           tags=['minion_diags', 'b4c','minion_integration'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test below: have to disable Cosim (+CHECKER=0):
t('lock_sw',                    'cache_ops/new_impl/lock_sw',                 ['neigh'    ], test_name='cacheops_lock_sw_postsi',    tags=['minion_diags', 'postsi_min_shire_ust'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('invalidate',                 'cache_ops/new_impl/invalidate',              ['neigh'    ], test_name='cacheops_invalidate',        tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs )
t('evict_sw',                   'cache_ops/new_impl/evict_sw_split',          ['neigh'    ], test_name='cacheops_evict_sw_split',    tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('evict_sw',                   'cache_ops/new_impl/evict_sw_split',          ['neigh'    ], test_name='cacheops_evict_sw_split_postsi_phs1',    tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('evict_sw',                   'cache_ops/new_impl/evict_sw_split',          ['neigh'    ], test_name='cacheops_evict_sw_split_postsi_phs2',    tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('evict_va',                   'cache_ops/new_impl/evict_va_split',          ['neigh'    ], test_name='cacheops_evict_va_split',    tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('evict_va',                   'cache_ops/new_impl/evict_va_split',          ['neigh'    ], test_name='cacheops_evict_va_split_postsi_phs1',    tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('evict_va',                   'cache_ops/new_impl/evict_va_split',          ['neigh'    ], test_name='cacheops_evict_va_split_postsi_phs2',    tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('flush_sw',                   'cache_ops/new_impl/flush_sw_split',          ['neigh'    ], test_name='cacheops_flush_sw_split',    tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test cacheops_flush_sw_split, which causes Cosim to fire falsely; hence have to disable Cosim (+CHECKER=0):
t('flush_sw',                   'cache_ops/new_impl/flush_sw_split',          ['neigh'    ], test_name='cacheops_flush_sw_split_postsi_phs1',    tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('flush_sw',                   'cache_ops/new_impl/flush_sw_split',          ['neigh'    ], test_name='cacheops_flush_sw_split_postsi_phs2',    tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('flush_va',                   'cache_ops/new_impl/flush_va_split',          ['neigh'    ], test_name='cacheops_flush_va_split',    tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test cacheops_flush_va_split, which causes Cosim to fire falsely; hence have to disable Cosim (+CHECKER=0):
t('flush_va',                   'cache_ops/new_impl/flush_va_split',          ['neigh'    ], test_name='cacheops_flush_va_split_postsi_phs1',    tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('flush_va',                   'cache_ops/new_impl/flush_va_split',          ['neigh'    ], test_name='cacheops_flush_va_split_postsi_phs2',    tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('prefetch',                   'cache_ops/new_impl/prefetch_split',          ['neigh'    ], test_name='cacheops_prefetch_split',    tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test below: have to disable Cosim (+CHECKER=0):
t('prefetch',                   'cache_ops/new_impl/prefetch_split',          ['neigh'    ], test_name='cacheops_prefetch_split_postsi_phs1',    tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('prefetch',                   'cache_ops/new_impl/prefetch_split',          ['neigh'    ], test_name='cacheops_prefetch_split_postsi_phs2',    tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('lock_va',                    'cache_ops/new_impl/lock_va_split',           ['neigh'    ], test_name='cacheops_lock_va_split',     tags=['minion_diags', 'postsi_min_shire_ust'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('lock_sw',                    'cache_ops/new_impl/lock_sw_split',           ['neigh'    ], test_name='cacheops_lock_sw_split',     tags=['minion_diags'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test below: have to disable Cosim (+CHECKER=0):
t('lock_sw',                    'cache_ops/new_impl/lock_sw_split',           ['neigh'    ], test_name='cacheops_lock_sw_split_postsi',     tags=['minion_diags', 'postsi_min_shire_ust'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('evict_sw',                   'cache_ops/new_impl/evict_sw_scp',            ['neigh'    ], test_name='cacheops_evict_sw_scp',      tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
t('evict_sw',                   'cache_ops/new_impl/evict_sw_scp',            ['neigh'    ], test_name='cacheops_evict_sw_scp_postsi_phs1',      tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('evict_sw',                   'cache_ops/new_impl/evict_sw_scp',            ['neigh'    ], test_name='cacheops_evict_sw_scp_postsi_phs2',      tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('evict_va',                   'cache_ops/new_impl/evict_va_scp',            ['neigh'    ], test_name='cacheops_evict_va_scp',      tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
t('evict_va',                   'cache_ops/new_impl/evict_va_scp',            ['neigh'    ], test_name='cacheops_evict_va_scp_postsi_phs1',      tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('evict_va',                   'cache_ops/new_impl/evict_va_scp',            ['neigh'    ], test_name='cacheops_evict_va_scp_postsi_phs2',      tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('flush_sw',                   'cache_ops/new_impl/flush_sw_scp',            ['neigh'    ], test_name='cacheops_flush_sw_scp',      tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test cacheops_flush_sw_scp, which causes Cosim to fire falsely; hence have to disable Cosim (+CHECKER=0):
t('flush_sw',                   'cache_ops/new_impl/flush_sw_scp',            ['neigh'    ], test_name='cacheops_flush_sw_scp_postsi_phs1',      tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('flush_sw',                   'cache_ops/new_impl/flush_sw_scp',            ['neigh'    ], test_name='cacheops_flush_sw_scp_postsi_phs2',      tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('flush_va',                   'cache_ops/new_impl/flush_va_scp',            ['neigh'    ], test_name='cacheops_flush_va_scp',      tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test cacheops_flush_va_scp, which causes Cosim to fire falsely; hence have to disable Cosim (+CHECKER=0):
t('flush_va',                   'cache_ops/new_impl/flush_va_scp',            ['neigh'    ], test_name='cacheops_flush_va_scp_postsi_phs1',      tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('flush_va',                   'cache_ops/new_impl/flush_va_scp',            ['neigh'    ], test_name='cacheops_flush_va_scp_postsi_phs2',      tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('prefetch',                   'cache_ops/new_impl/prefetch_scp',            ['neigh'    ], test_name='cacheops_prefetch_scp',      tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test below: have to disable Cosim (+CHECKER=0):
t('prefetch',                   'cache_ops/new_impl/prefetch_scp',            ['neigh'    ], test_name='cacheops_prefetch_scp_postsi_phs1',      tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
t('prefetch',                   'cache_ops/new_impl/prefetch_scp',            ['neigh'    ], test_name='cacheops_prefetch_scp_postsi_phs2',      tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('lock_va',                    'cache_ops/new_impl/lock_va_scp',             ['neigh'    ], test_name='cacheops_lock_va_scp',       tags=['minion_diags', 'b4c','minion_integration', 'postsi_min_shire_ust'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
t('lock_sw',                    'cache_ops/new_impl/lock_sw_scp',             ['neigh'    ], test_name='cacheops_lock_sw_scp',       tags=['minion_diags', 'b4c','minion_integration'], plusargs= common_test_plusargs )
# (CYE) added self-checking in test below: have to disable Cosim (+CHECKER=0):
t('lock_sw',                    'cache_ops/new_impl/lock_sw_scp',             ['neigh'    ], test_name='cacheops_lock_sw_scp_postsi',       tags=['minion_diags', 'postsi_min_shire_ust'], plusargs='+CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )

t('evict_sw',                   'cache_ops/new_impl/evict_sw',                ['neigh'    ], test_name='cacheops_evict_sw_2t',       plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('evict_va',                   'cache_ops/new_impl/evict_va',                ['neigh'    ], test_name='cacheops_evict_va_2t',       plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('flush_sw',                   'cache_ops/new_impl/flush_sw',                ['neigh'    ], test_name='cacheops_flush_sw_2t',       plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('flush_va',                   'cache_ops/new_impl/flush_va',                ['neigh'    ], test_name='cacheops_flush_va_2t',       plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('prefetch',                   'cache_ops/new_impl/prefetch',                ['neigh'    ], test_name='cacheops_prefetch_2t',       plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('lock_va',                    'cache_ops/new_impl/lock_va',                 ['neigh'    ], test_name='cacheops_lock_va_2t',        plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags'] )
t('lock_sw',                    'cache_ops/new_impl/lock_sw',                 ['neigh'    ], test_name='cacheops_lock_sw_2t',        plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags'] )
t('evict_sw',                   'cache_ops/new_impl/evict_sw_split',          ['neigh'    ], test_name='cacheops_evict_sw_split_2t', plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('evict_va',                   'cache_ops/new_impl/evict_va_split',          ['neigh'    ], test_name='cacheops_evict_va_split_2t', plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('flush_sw',                   'cache_ops/new_impl/flush_sw_split',          ['neigh'    ], test_name='cacheops_flush_sw_split_2t', plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('flush_va',                   'cache_ops/new_impl/flush_va_split',          ['neigh'    ], test_name='cacheops_flush_va_split_2t', plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('prefetch',                   'cache_ops/new_impl/prefetch_split',          ['neigh'    ], test_name='cacheops_prefetch_split_2t', plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('lock_va',                    'cache_ops/new_impl/lock_va_split',           ['neigh'    ], test_name='cacheops_lock_va_split_2t',  plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags'] )
t('lock_sw',                    'cache_ops/new_impl/lock_sw_split',           ['neigh'    ], test_name='cacheops_lock_sw_split_2t',  plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags'] )
t('evict_sw',                   'cache_ops/new_impl/evict_sw_scp',            ['neigh'    ], test_name='cacheops_evict_sw_scp_2t',   plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('evict_va',                   'cache_ops/new_impl/evict_va_scp',            ['neigh'    ], test_name='cacheops_evict_va_scp_2t',   plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('flush_sw',                   'cache_ops/new_impl/flush_sw_scp',            ['neigh'    ], test_name='cacheops_flush_sw_scp_2t',   plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('flush_va',                   'cache_ops/new_impl/flush_va_scp',            ['neigh'    ], test_name='cacheops_flush_va_scp_2t',   plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('prefetch',                   'cache_ops/new_impl/prefetch_scp',            ['neigh'    ], test_name='cacheops_prefetch_scp_2t',   plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags', 'perf_track'] )
t('lock_va',                    'cache_ops/new_impl/lock_va_scp',             ['neigh'    ], test_name='cacheops_lock_va_scp_2t',    plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags'] )
t('lock_sw',                    'cache_ops/new_impl/lock_sw_scp',             ['neigh'    ], test_name='cacheops_lock_sw_scp_2t',    plusargs='+THREAD_MASK=0x3' + common_test_plusargs, tags=['minion_diags'] )
t('cop_cover',                  'cache_ops/cop_cover',                        ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('ucache_control_bw',          'cache_ops/ucache_control_bw',                ['shire'    ], tags=['minion_diags'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x1' + common_test_plusargs )
t('cop_dirty_line',             'cache_ops/cop_dirty_line',                   ['neigh'    ], tags=['minion_diags'], plusargs='+CHECKER=0' + common_test_plusargs )
# (CYE) added self-checking in test cop_dirty_line, which causes Cosim to fire falsely; hence disable Cosim (+CHECKER=0):
t('cop_dirty_line',             'cache_ops/cop_dirty_line',                   ['neigh'    ], test_name='cop_dirty_line_postsi',    tags=['minion_diags', 'postsi_min_shire_ust'], plusargs='+CHECKER=0' + common_test_plusargs )
t('cop_dst_start_nop',          'cache_ops/cop_dst_start_nop',                ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_evict_invalidate',       'cache_ops/cop_evict_invalidate',             ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_evictsw',                'cache_ops/cop_evictsw',                      ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_evictva',                'cache_ops/cop_evictva',                      ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_flushsw',                'cache_ops/cop_flushsw',                      ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_flushva',                'cache_ops/cop_flushva',                      ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_lock_nop',               'cache_ops/cop_lock_nop',                     ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_lockva',                 'cache_ops/cop_lockva',                       ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_lockva_akash',           'cache_ops/cop_lockva_akash',                 ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_masked',                 'cache_ops/cop_masked',                       ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_noscp_nop',              'cache_ops/cop_noscp_nop',                    ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_prefetchva',             'cache_ops/cop_prefetchva',                   ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_prefetchva_cover',       'cache_ops/cop_prefetchva_cover',             ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_prefetchva_nop',         'cache_ops/cop_prefetchva_nop',               ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_prefetchva_valid',       'cache_ops/cop_prefetchva_valid',             ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_scp_ctrl',               'cache_ops/cop_scp_ctrl',                     ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_scp_l1prefetch_nop',     'cache_ops/cop_scp_l1prefetch_nop',           ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_unlock_unlocked',        'cache_ops/cop_unlock_unlocked',              ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_unlockva',               'cache_ops/cop_unlockva',                     ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_unlockva',               'cache_ops/cop_unlockva_version2',            ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_unlockva_version2',      'cache_ops/cop_unlockva_version2',            ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_wrap',                   'cache_ops/cop_wrap',                         ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('evict_sw',                   'cache_ops/evict_sw',                         ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('evict_va',                   'cache_ops/evict_va',                         ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('lock_unlock',                'cache_ops/lock_unlock',                      ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_zero_stride',            'cache_ops/cop_zero_stride',                  ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_lock_autoway',           'cache_ops/cop_lock_autoway',                 ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_scp_l23prefetch',        'cache_ops/cop_scp_l23prefetch',              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('cop_lock_setway_locked',     'cache_ops/cop_lock_setway_locked',           ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_lock_pa_exists',         'cache_ops/cop_lock_pa_exists',               ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_lock_way_limit',         'cache_ops/cop_lock_way_limit',               ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_scp_ctrl_dis_unlock',    'cache_ops/cop_scp_ctrl_dis_unlock',          ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('cop_lock_zero',              'cache_ops/cop_lock_zero',                    ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust'], plusargs= common_test_plusargs )
t('cop_scp_ctrl_en_invalidate', 'cache_ops/cop_scp_ctrl_en_invalidate',       ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'] ,plusargs='+CHECKER=0' + common_test_plusargs )
t('cop_scp_evict_flush',        'cache_ops/cop_scp_evict_flush',              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('zero_pos_neg',               'ieee/zero_pos_neg',                          ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('mask_and',                   'mask/mask_and',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('mask_movaxm',                'mask/mask_movaxm',                           ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('mask_movmx',                 'mask/mask_movmx',                            ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('mask_not',                   'mask/mask_not',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('mask_rand',                  'mask/mask_not',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('mask_or',                    'mask/mask_or',                               ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('mask_popc',                  'mask/mask_popc',                             ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('mask_popc_rast',             'mask/mask_popc_rast',                        ['neigh'    ], tags=['minion_diags'], plusargs= '+trap_on_gfx=0' + common_test_plusargs )
t('mask_popcz',                 'mask/mask_popcz',                            ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('mask_xor',                   'mask/mask_xor',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('fsetm_pi',                   'mask/fsetm',                                 ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('fsetm_pi_debug',             'mask/fsetm_vol2',                            ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('ddr_1bank_row_miss',         'perf/ddr_1bank_row_miss',                    ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('fexp',                       'perf/fexp',                                  ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('flog',                       'perf/flog',                                  ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('fma_dep',                    'perf/fma_dep',                               ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('fma_ind',                    'perf/fma_ind',                               ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('frcp',                       'perf/frcp',                                  ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('frcp_ex',                    'perf/frcp',                                  ['neigh'    ], tags=['min_cov_depr'], plusargs= common_test_plusargs ) # takes too long     / bflood: adding tag to stop from getting picked up in coverage run
t('frcpfxp',                    'perf/frcpfxp',                               ['neigh'    ], tags=['min_cov_depr'], plusargs= common_test_plusargs ) # GFX deprioritised  / bflood: adding tag to stop from getting picked up in coverage run
t('frsqrt',                     'perf/frsqrt',                                ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('frsqrt_ex',                  'perf/frsqrt',                                ['neigh'    ], tags=['min_cov_depr'], plusargs= common_test_plusargs ) # takes too long     / bflood: adding tag to stop from getting picked up in coverage run
t('fsin',                       'perf/fsin',                                  ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('idiv',                       'perf/idiv',                                  ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('l1_pref',                    'perf/l1_pref',                               ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('l2_hit',                     'perf/l2_hit',                                ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('l2_miss',                    'perf/l2_miss',                               ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('load_to_use',                'perf/load_to_use',                           ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('misaligned',                 'perf/misaligned',                            ['neigh'    ], tags=['minion_diags', 'b4c', 'perf_track'], plusargs= common_test_plusargs )
t('rsqrt',                      'perf/rsqrt',                                 ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('sqrt',                       'perf/sqrt',                                  ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('st_to_ld_byp',               'perf/st_to_ld_byp',                          ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('texsnd',                     'perf/texsnd',                                ['neigh'    ], tags=['minion_diags'], plusargs= common_test_plusargs )
t('trans',                      'perf/trans',                                 ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('trans_wb',                   'perf/trans_wb',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_fma16a32',            'tensors/tensorFMA16A32',                     ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_fma16a32_acols',      'tensors/tensorFMA16A32',                     ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_fma16a32_acomb',      'tensors/tensorFMA16A32',                     ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_fma16a32_arows',      'tensors/tensorFMA16A32',                     ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_fma32',               'tensors/tensorFMA32',                        ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_ima8a32',             'tensors/tensorIMA8A32',                      ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_fma_invalid',         'tensors/tensorFMAInvalid',                   ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_load',                'tensors/tensor_load',                        ['neigh'    ], tags=['b4c','minion_integration', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_load',                'tensors/tensor_load',                        ['neigh'    ], tags=['minion_diags', 'b4c_stg3','minion_integration', 'perf_track'], plusargs= common_test_plusargs, test_name='tensor_load_cov', diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=48'")
t('tensor_load',                'tensors/tensor_load',                        ['neigh'    ], tags=['postsi_min_shire_noc_ust'], plusargs= common_test_plusargs, test_name='tensor_load_postsi', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DLOOP_SIZE=48'")

t('tensor_load_v1',             'tensors/tensor_load',                        ['neigh'    ], tags=['cye_tensor_load_v1'], plusargs='+MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
#t('tensor_load_v1',             'tensors/tensor_load',                        ['shire'    ], tags=['cye_tensor_load_v1'], test_name='tensor_load_v1_in_shire', plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )

t('neigh_chicken_bits0',        'neigh_chicken_bits/0',                       ['shire'    ], tags=['minion_diags'], plusargs='+MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3 +force_dest_fifo=1 +agent_forced=0 +force_all_agents=0 +dest_fifo=1 +bypass_dcache=1' + common_test_plusargs )
t('neigh_chicken_bits_postsi',  'neigh_chicken_bits_postsi',                  ['shire'    ], tags=['postsi_fc_bringup'], plusargs='+MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3 +force_dest_fifo=1 +agent_forced=0 +force_all_agents=0 +dest_fifo=1 +bypass_dcache=1' + common_test_plusargs )
t('tensor_load_coop',           'tensors/tensor_load_coop',                   ['neigh'    ], tags=['b4c', 'b4c_stg1','minion_integration', 'perf_track'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0xFFFF +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode=1' + common_test_plusargs )
t('tensor_load_coop',           'tensors/tensor_load_coop',                   ['neigh'    ], tags=['minion_diags', 'b4c_stg3','minion_integration', 'perf_track'], test_name='tensor_load_coop_cov', diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=48'", plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0xFFFF +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode=1' + common_test_plusargs )
t('tensor_load_coop',           'tensors/tensor_load_coop',                   ['shire'    ], tags=['postsi_min_shire_noc_ust'], test_name='tensor_load_coop_postsi', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DLOOP_SIZE=48'", plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0xFFFF +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode=1' + common_test_plusargs )
t('tensor_load_coop_interneigh','tensors/tensor_load_coop_interneigh',        ['shire'    ], tags=['b4c','minion_integration'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3 +etlinkReqRdyDelays=short +memDelays=short' + common_test_plusargs )
t('tensor_load_coop_interneigh','tensors/tensor_load_coop_interneigh',        ['shire'    ], tags=['minion_diags', 'b4c_stg3','minion_integration'], test_name='tensor_load_coop_interneigh_cov', diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=48'", plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3 +etlinkReqRdyDelays=short +memDelays=short' + common_test_plusargs )
t('tensor_load_coop_interneigh','tensors/tensor_load_coop_interneigh',        ['shire'    ], tags=['postsi_min_shire_noc_ust'], test_name='tensor_load_coop_interneigh_postsi', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'", plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3 +etlinkReqRdyDelays=short +memDelays=short' + common_test_plusargs )

t('tensor_load_l2scp',          'tensors/tensor_load_l2scp',                  ['shire'    ], tags=['b4c','minion_integration'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_load_l2scp',          'tensors/tensor_load_l2scp',                  ['shire'    ], tags=['minion_diags', 'b4c_stg3','minion_integration'], test_name='tensor_load_l2scp_cov', diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=48'", plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
## for Post-Si bringup: only enable Minion 0 (2-thread):
t('tensor_load_l2scp',          'tensors/tensor_load_l2scp',                  ['shire'    ], test_name='tensor_load_l2scp_postsi', tags=['postsi_min_shire_noc_ust'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_load_l2scp',          'tensors/tensor_load_l2scp',                  ['shire'    ], test_name='tensor_load_l2scp_cov_postsi', tags=['postsi_min_shire_noc_ust'], diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=48'", plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )

t('tensor_reduce',              'tensors/tensor_reduce',                      ['neigh'    ], tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_reduce',              'tensors/tensor_reduce',                      ['neigh'    ], tags=['postsi_min_shire_noc_ust'], test_name='tensor_reduce_postsi', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8'", plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_reduce_snd_rcv',      'tensors/tensor_reduce_snd_rcv',              ['neigh'    ], tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_reduce_snd_rcv',      'tensors/tensor_reduce_snd_rcv',              ['neigh'    ], test_name='tensor_reduce_snd_rcv_postsi', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8'", tags=['postsi_min_shire_noc_ust'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_reduce_auto',         'tensors/tensor_reduce_auto',                 ['neigh'    ], test_name='tensor_reduce_auto_8c',       diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=3'", tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs='+MINION_MASK=0xFF   +THREAD_MASK=0x1' + common_test_plusargs )
t('tensor_reduce_auto',         'tensors/tensor_reduce_auto',                 ['neigh'    ], test_name='tensor_reduce_auto_8c_postsi', diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=3 -DSELF_CHECK -DNUM_MINIONS=8 -DNUM_ITER=20'", tags=['postsi_min_shire_noc_ust'], plusargs='+MINION_MASK=0xFF   +THREAD_MASK=0x1' + common_test_plusargs )
t('tensor_reduce_auto',         'tensors/tensor_reduce_auto',                 ['shire'    ], test_name='tensor_reduce_auto_16c',      diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=4'", tags=['minion_diags','minion_integration'],        plusargs='+MINION_MASK=0xFFFF +THREAD_MASK=0x1' + common_test_plusargs )
# (Charles) test tensor_reduce_auto_16c_postsi requires about 240KB data space (but our raw data is only 64KB); have to move it to Phase 3 (FC):
t('tensor_reduce_auto',         'tensors/tensor_reduce_auto',                 ['shire'    ], test_name='tensor_reduce_auto_16c_postsi', diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=4 -DSELF_CHECK -DNUM_MINIONS=16 -DNUM_ITER=20'", tags=['postsi_fc_bringup'], plusargs='+MINION_MASK=0xFFFF +THREAD_MASK=0x1' + common_test_plusargs )
t('tensor_reduce_broadcast',    'tensors/tensor_reduce_broadcast',            ['neigh'    ], test_name='tensor_reduce_broadcast_8c',  diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=3'", tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs='+MINION_MASK=0xFF   +THREAD_MASK=0x1' + common_test_plusargs )
t('tensor_reduce_broadcast',    'tensors/tensor_reduce_broadcast',            ['neigh'    ], test_name='tensor_reduce_broadcast_8c_postsi', diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=3 -DSELF_CHECK -DNUM_MINIONS=8 -DNUM_ITER=20'", tags=['postsi_min_shire_noc_ust'], plusargs='+MINION_MASK=0xFF   +THREAD_MASK=0x1' + common_test_plusargs )
t('tensor_reduce_broadcast',    'tensors/tensor_reduce_broadcast',            ['shire'    ], test_name='tensor_reduce_broadcast_16c', diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=4'", tags=['minion_diags','minion_integration'],        plusargs='+MINION_MASK=0xFFFF +THREAD_MASK=0x1' + common_test_plusargs )
# (Charles) test tensor_reduce_broadcast_16c_postsi requires about 240KB data space (but our raw data is only 64KB); have to move it to Phase 3 (FC):
t('tensor_reduce_broadcast',    'tensors/tensor_reduce_broadcast',            ['shire'    ], test_name='tensor_reduce_broadcast_16c_postsi', diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=4 -DSELF_CHECK -DNUM_MINIONS=16 -DNUM_ITER=20'", tags=['postsi_fc_bringup'], plusargs='+MINION_MASK=0xFFFF +THREAD_MASK=0x1' + common_test_plusargs )
t('tensor_store_starvation',    'multithread_starvation/tensor_store',        ['neigh'    ], tags=['b4c'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3 +etlinkReqRdyDelays=long +memDelays=long +etlinkStarvationDelay=1' + common_test_plusargs )
t('int_mul_div_starvation',     'multithread_starvation/int_mul_div',         ['neigh'    ], tags=['b4c'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )

t('tensor_reduce_deadlock',              'tensors/tensor_reduce_deadlock',    ['neigh'    ], test_name='tensor_reduce_deadlock', plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x3' + common_test_plusargs )

## Charles: temp: reduce illegal_Op:
t('tensor_reduce',              'tensors/tensor_reduce',                      ['neigh'    ], test_name='tensor_reduce_illegal_op',          diag_compile_opts="EXTRA_CFLAGS='-DREDUCE_ILLEGAL_OP'", tags=['cye_tensor_reduce_illegal_op'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_reduce_snd_rcv',      'tensors/tensor_reduce_snd_rcv',              ['neigh'    ], test_name='tensor_reduce_snd_rcv_illegal_op',  diag_compile_opts="EXTRA_CFLAGS='-DREDUCE_ILLEGAL_OP'",  tags=['cye_tensor_reduce_illegal_op'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_reduce_auto',         'tensors/tensor_reduce_auto',                 ['neigh'    ], test_name='tensor_reduce_auto_8c_illegal_op',  diag_compile_opts="EXTRA_CFLAGS='-DREDUCE_ILLEGAL_OP -DMAX_TREE=3'", tags=['cye_tensor_reduce_illegal_op'], plusargs='+MINION_MASK=0xFF   +THREAD_MASK=0x1' + common_test_plusargs )

t('tensor_broadcast',           'tensors/tensor_broadcast',                   ['neigh'    ], tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_store_coop',          'tensors/tensor_store_coop',                  ['neigh'    ], tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0xFFFF +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode' + common_test_plusargs )
t('tensor_store_coop',          'tensors/tensor_store_coop',                  ['neigh'    ], tags=['postsi_min_shire_noc_ust'], test_name='tensor_store_coop_postsi', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8'", plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0xFFFF +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode' + common_test_plusargs )
t('tensor_store_scp',           'tensors/tensor_store_scp',                   ['neigh'    ], tags=['minion_diags', 'b4c','minion_integration', 'postsi_fc_bringup', 'perf_track'], plusargs= common_test_plusargs )

t('tensor_store_hitfifo',       'tensors/tensor_store_hitfifo',               ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0xFFFF +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode' + common_test_plusargs )
t('tensor_store_hitfifo',       'tensors/tensor_store_hitfifo',               ['neigh'    ], tags=['postsi_min_shire_noc_ust'], test_name='tensor_store_hitfifo_postsi', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8'", plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0xFFFF +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode' + common_test_plusargs )
# (Charles) run in Shire build model: but still 8 Minions (1-Neigh), 
t('tensor_store_hitfifo',       'tensors/tensor_store_hitfifo',               ['shire'    ], test_name='tensor_store_hitfifo_in_shire', tags=['cye'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3 +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode' + common_test_plusargs )

t('tensor_load_mt',             'tensors/tensor_load_mt',                     ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'], plusargs= common_test_plusargs )
t('tensor_multi_op',            'tensors/tensor_multi_op',                    ['shire'    ], tags=['minion_diags', 'b4c','minion_integration'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x1' + common_test_plusargs )
t('tensor_load_setup_b',        'tensors/tensor_load_setup_b',                ['neigh'    ], tags=['minion_diags', 'b4c','minion_integration', 'perf_track'], plusargs='+ECALL_ENDS_TEST=0' + common_test_plusargs )
t('tensor_load_setup_b',        'tensors/tensor_load_setup_b',                ['neigh'    ], tags=['postsi_min_shire_noc_ust'], test_name='tensor_load_setup_b_postsi', diag_compile_opts="EXTRA_CFLAGS+=' -DSELF_CHECK'", plusargs='+ECALL_ENDS_TEST=0' + common_test_plusargs )

# (Charles) temp: 2 shires: s0 and s1 (both compute minions);
t('tensor_reduce_auto',         'tensors/tensor_reduce_auto',                 ['shirex2'  ], test_name='tensor_reduce_auto_s0s1_64c',  diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=6 -DNUM_ITER=12'", tags=['cye_tensor_reduce_auto'], plusargs='+SHIRE_MASK=0x000000003 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3  +trans_timeout=500000 +vc_timeout_wait=2000 +thread_timeout=3000000' + common_test_plusargs )

t('VERIF-2447',                 'cosim_sanity/VERIF-2447',                    ['shire'    ], tags=['minion_diags', 'b4c'], 			plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x3' + common_test_plusargs )
t('fexp_ps',                    'trans/concurrent/fexp_ps',                   ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('flog_ps',                    'trans/concurrent/flog_ps',                   ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('frcp_ps',                    'trans/concurrent/frcp_ps',                   ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('frsqrt_ps',                  'trans/concurrent/frsqrt_ps',                 ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('fsin_ps',                    'trans/concurrent/fsin_ps',                   ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('fdiv_ps',                    'trans/fdiv_ps',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('fexp_ps',                    'trans/fexp_ps',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('flog_ps',                    'trans/flog_ps',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('frcp_ps',                    'trans/frcp_ps',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
#t('frcpfxp_ps',                 'trans/frcpfxp_ps',                           ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= '+trap_on_gfx=0' + common_test_plusargs )
t('frsqrt_ps',                  'trans/frsqrt_ps',                            ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('fsin_ps',                    'trans/fsin_ps',                              ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )

t('atomic_extension_gd',        'atomic_extension/atomic_extension_gd',       ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('atomic_extension_gw',        'atomic_extension/atomic_extension_gw',       ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('atomic_extension_ld',        'atomic_extension/atomic_extension_ld',       ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('atomic_extension_lw',        'atomic_extension/atomic_extension_lw',       ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('atomic_extension_lsgsc_ps',  'atomic_extension/atomic_extension_lsgsc_ps', ['neigh'    ], test_name='atomic_extension_lsgsc_lps', tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLOCAL_VEC_COH_MEM_OPS'" )
t('atomic_extension_lsgsc_ps',  'atomic_extension/atomic_extension_lsgsc_ps', ['neigh'    ], test_name='atomic_extension_lsgsc_gps', tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DGLOBAL_VEC_COH_MEM_OPS'" )
t('amoaddl_w',                  'atomic_extension/amoaddl_w',                 ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('amoandl_w',                  'atomic_extension/amoandl_w',                 ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('amomaxl_w',                  'atomic_extension/amomaxl_w',                 ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('amominl_w',                  'atomic_extension/amominl_w',                 ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('amoorl_w',                   'atomic_extension/amoorl_w',                  ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )
t('amoxorl_w',                  'atomic_extension/amoxorl_w',                 ['neigh'    ], tags=['minion_diags', 'perf_track'], plusargs= common_test_plusargs )

## Charles: run the above tests in "shire" build: also run in 1 hart (otherwise self-check would fail);
t('atomic_extension_gd',        'atomic_extension/atomic_extension_gd',       ['shire'    ], test_name='atomic_extension_gd_in_shire',       tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
# this test atomic_extension_gpi has hard-coded addresses and hence cannot run in Post-Si UST flow:
t('atomic_extension_gpi',       'atomic_extension/atomic_extension_gpi',      ['shire'    ], test_name='atomic_extension_gpi_in_shire',      tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_fc_bringup', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('atomic_extension_gw',        'atomic_extension/atomic_extension_gw',       ['shire'    ], test_name='atomic_extension_gw_in_shire',       tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('atomic_extension_ld',        'atomic_extension/atomic_extension_ld',       ['shire'    ], test_name='atomic_extension_ld_in_shire',       tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('atomic_extension_lw',        'atomic_extension/atomic_extension_lw',       ['shire'    ], test_name='atomic_extension_lw_in_shire',       tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
# this test atomic_extension_lpi has hard-coded addresses and hence cannot run in Post-Si UST flow:
t('atomic_extension_lpi',       'atomic_extension/atomic_extension_lpi',      ['shire'    ], test_name='atomic_extension_lpi_in_shire',      tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_fc_bringup', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('atomic_extension_lsgsc_ps',  'atomic_extension/atomic_extension_lsgsc_ps', ['shire'    ], test_name='atomic_extension_lsgsc_lps_in_shire', tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLOCAL_VEC_COH_MEM_OPS'" )
t('atomic_extension_lsgsc_ps',  'atomic_extension/atomic_extension_lsgsc_ps', ['shire'    ], test_name='atomic_extension_lsgsc_gps_in_shire', tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_noc_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DGLOBAL_VEC_COH_MEM_OPS'" )

t('amoaddl_w',                  'atomic_extension/amoaddl_w',                 ['shire'    ], test_name='amoaddl_w_in_shire',                 tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('amoandl_w',                  'atomic_extension/amoandl_w',                 ['shire'    ], test_name='amoandl_w_in_shire',                 tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('amomaxl_w',                  'atomic_extension/amomaxl_w',                 ['shire'    ], test_name='amomaxl_w_in_shire',                 tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('amominl_w',                  'atomic_extension/amominl_w',                 ['shire'    ], test_name='amominl_w_in_shire',                 tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('amoorl_w',                   'atomic_extension/amoorl_w',                  ['shire'    ], test_name='amoorl_w_in_shire',                  tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('amoxorl_w',                  'atomic_extension/amoxorl_w',                 ['shire'    ], test_name='amoxorl_w_in_shire',                 tags=['minion_diags', 'cye_atomics_in_shire', 'postsi_min_shire_ust', 'perf_track'], plusargs= '+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )

## Charles (temp): run tests in "shire" build but in Shire_1 (or Shire_2,... etc):
## (new) "shire_1" build: see "minion.etdv.py"; it works: 
t('atomic_extension_gw',        'atomic_extension/atomic_extension_gw',       ['shire_1'    ], test_name='atomic_extension_gw_in_shire_1',       tags=['cye_atomics_in_shire_1'],    plusargs= '+SHIRE_MASK=0x2 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
## (new) "fc_shire_1" build: see "minion.etdv.py"; it works:
t('atomic_extension_gw',        'atomic_extension/atomic_extension_gw',       ['fc_shire_1' ], test_name='atomic_extension_gw_in_fc_shire_1',    tags=['cye_atomics_in_fc_shire_1'], plusargs= '+SHIRE_MASK=0x2 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )

t('port_tx_sanity_test',        'ports/port_tx_sanity_test',                  ['shire'    ], tags=['minion_diags', 'b4c','minion_integration'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3 +en_s3_core_resp_chk=0' + common_test_plusargs )
#t('port_tx_full_test',          'ports/port_tx_full_test',                    ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('basic',                      'excl_mode',                                  ['neigh'    ], tags=['minion_diags', 'b4c_stg1'], plusargs= common_test_plusargs )
t('rand_st',                    'noc/rand_st',                                ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1' + common_test_plusargs )
t('power',                      'noc/power',                                  ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1' + common_test_plusargs )
t('scp',                        'noc/scp',                                    ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1' + common_test_plusargs )
t('esr',                        'noc/esr',                                    ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1' + common_test_plusargs ) # test needs update
#t('rbox_test',                  'rbox',                                       ['shiregfx' ], tags=['rbox_diags', 'gfx_b4c'], plusargs='+l1d_sb_en=0 +neigh_sb_en=0 +uc_sb_en=0 +sc_sb_en=0 +SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3 +disableETLinkChecks=1 +dis_sva_dcache_scp_ctrl_EoT_a1=1' + common_test_plusargs )
t('rtlmin_2566',                'jiras/rtlmin_2566',                          ['shire'    ], test_name='rtlmin_2566_chicken', tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3 +min_frontend_clock_gate_disable=1' + common_test_plusargs )
t('rtlmin_2566',                'jiras/rtlmin_2566',                          ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('rtlmin_2612',                'jiras/rtlmin_2612',                          ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
#t('rtlmin_2321',                'jiras/rtlmin_2321',                          ['neigh'    ], tags=['minion_diags', 'postsi_fc_bringup', 'perf_track'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3 +ECALL_ENDS_TEST=0 +l1d_sb_en=0' + common_test_plusargs )
t('rtlmin_2902',                'jiras/rtlmin_2902',                          ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_ust', 'perf_track'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3 +ECALL_ENDS_TEST=0' + common_test_plusargs )
#t('rtlmin_851',                 'jiras/rtlmin_851',                           ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1 +ECALL_ENDS_TEST=0 +l1d_sb_en=0' + common_test_plusargs )                             # Postponed due to RTLMIN-3155      
#t('rtlmin_851',                 'jiras/rtlmin_851',                           ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3 +ECALL_ENDS_TEST=0 +l1d_sb_en=0', test_name="rtlmin_851_2T" + common_test_plusargs )  # Postponed due to RTLMIN-3155 
t('verif_511',                  'jiras/verif_511',                            ['neigh'    ], tags=['vm', 'postsi_fc_bringup', 'perf_track'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1 +ECALL_ENDS_TEST=0 +l1d_sb_en=0' + common_test_plusargs )
t('gsc_bp',                     'gsc_bp',                                     ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3 +ECALL_ENDS_TEST=0 +l1d_sb_en=0' + common_test_plusargs )
#t('bus_err',                     'data_and_fetch_bus_errors',                 ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3 +MONITOR_DISABLE_DELAYED_STORES=1 +ECALL_ENDS_TEST=0 +l1d_sb_en=0 +axi_mem_map_err=1 +disableETLinkChecks=1 +disableAXIChecks=1 +disable_NOC_SB=1  +disableSCSBChecks=1 +l1d_sb_en=0 +axi_mem_map_err=1 +allow_bus_err=1 ' + common_test_plusargs )
# for Post-Si: this diag actually tests breakpoints (and our UST flow uses them too), so it cannot run in Phase_1 or 2; can still keep it in Phase_3/FC; 
#t('te_xcpt',                    'te_xcpt',                                    ['neigh'    ], tags=['minion_diags', 'postsi_fc_bringup', 'perf_track'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3 +ECALL_ENDS_TEST=0 +l1d_sb_en=0 +FESB_FETCH_PROTECTION_DISABLE ' + common_test_plusargs )
#t('switch_modes',               'switch_modes',                               ['neigh'    ], tags=['minion_diags','b4c_stg1'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3 +RECORD=cosim_evt.trace' + common_test_plusargs, post_test='$REPOROOT/dv/common/scripts/run_replay.sh')
t('msip',                       'interrupt/msip',                             ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('meip',                       'interrupt/meip',                             ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('mtip_shire',                 'interrupt/mtip_shire',                       ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('seip',                       'interrupt/seip',                             ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('seip_deleg',                 'interrupt/seip_deleg',                       ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('redirect',                   'redirect',                                   ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3 +ECALL_ENDS_TEST=0 +WFI_ENDS_TEST=0 +CSR_STALL_DONE_COUNT=-1' + common_test_plusargs )
t('int_enable_disable',         'int_enable_disable',                         ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3 +ECALL_ENDS_TEST=0' + common_test_plusargs )
t('flb_amos',                   'flb_amos',                                   ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('mstatus_fs',                 'mstatus_fs',                                 ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust', 'perf_track'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('gsc_sb',                     'gsc_sb',                                     ['neigh'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('gsc_sb_int',                 'gsc_sb_int',                                 ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('flb_stress',                 'flb_stress',                                 ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('shared_csrs',                'shared_csrs',                                ['neigh'    ], tags=['minion_diags', 'perf_track'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('ipi_pc',                     'ipi_pc',                                     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'], plusargs='+THREAD_MASK=0x3 +CSR_STALL_DONE_COUNT=0 +memDelays=long' + common_test_plusargs )
t('ipi_trigger',                'ipi_trigger',                                ['shire'    ], tags=['minion_diags'], plusargs='+THREAD_MASK=0x3 +MINION_MASK=0xFFFFFFFF +CSR_STALL_DONE_COUNT=0 +memDelays=long' + common_test_plusargs )
t('fc_smoke',                   'fc_smoke',                                   ['fc2shire' ], tags=['minion_diags', 'b4c_fc_smoke'], plusargs='+SHIRE_MASK=0x100000001 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('fc_smoke',                   'fc_smoke',                                   ['fc'       ], tags=['minion_diags'], plusargs='+SHIRE_MASK=0x1ffffffff +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('fc_smoke',                   'fc_smoke',                                   ['shire'    ], tags=['minion_diags', 'b4c_stg1'] , plusargs='+RECORD=cosim_evt.trace +SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, post_test='$REPOROOT/dv/common/scripts/run_replay.sh' )
t('fcc',                        'fcc',                                        ['shire'    ], tags=['minion_diags', 'b4c','minion_integration'], plusargs='+MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('fcc_flb',                    'fcc_flb',                                    ['shire'    ], tags=['minion_diags', 'minion_integration', 'postsi_fc_bringup'], plusargs='+MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('fcc_read',                   'fcc_read',                                   ['shire'    ], tags=['minion_diags'], plusargs='+MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('fccnb_sync',                 'fccnb_sync',                                 ['shire'    ], tags=['minion_diags'], plusargs='+MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('fcc_overflow_dt',            'fcc_overflow_dt',                            ['shire'    ], tags=['minion_diags'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )

# CYE: new test: FCC stall across Interrupt:
t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v1', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v2', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLONGER_DELAY_B4_GIVE_CREDIT'" )
t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v0', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DWITHOUT_INTERRUPT'" )

t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v1_th0_fcc1', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DCSRW_FCC1'" )
t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v1_th1_fcc0', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DCSRW_FCC_TH1'" )
t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v1_th1_fcc1', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DCSRW_FCC_TH1 -DCSRW_FCC1'" )

t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v2_th0_fcc1', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLONGER_DELAY_B4_GIVE_CREDIT -DCSRW_FCC1'" )
t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v2_th1_fcc0', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLONGER_DELAY_B4_GIVE_CREDIT -DCSRW_FCC_TH1'" )
t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v2_th1_fcc1', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLONGER_DELAY_B4_GIVE_CREDIT -DCSRW_FCC_TH1 -DCSRW_FCC1'" )

t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v0_th0_fcc1', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DWITHOUT_INTERRUPT -DCSRW_FCC1'" )
t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v0_th1_fcc0', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DWITHOUT_INTERRUPT -DCSRW_FCC_TH1'" )
t('fcc_x_interrupt',            'fcc_x_interrupt',                            ['shire'    ], test_name='fcc_x_interrupt_v0_th1_fcc1', tags=['cye_fcc'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DWITHOUT_INTERRUPT -DCSRW_FCC_TH1 -DCSRW_FCC1'" )


t('neigh_esr',                  'neigh_esr',                                  ['fc2shire' ], tags=['minion_diags'], plusargs='+MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
#t('neigh_esr_postsi',           'neigh_esr_postsi',                           ['fc2shire' ], tags=['postsi_min_shire_noc_ust'], plusargs='+MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('print_dbg_example',                  'print_dbg_example',                ['shire'    ], tags=['minion_diags'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1' + common_test_plusargs ) # superseeded by data_and_fetch_bus_errors
# t('bus_error',                  'bus_error',                                  ['neigh'    ], tags=['minion_diags'], plusargs='+MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3 +allow_bus_err=1' + common_test_plusargs ) # superseeded by data_and_fetch_bus_errors
t('pmu',                        'pmu/pmu',                                    ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1' + common_test_plusargs )
t('pmu_not_used',               'pmu/pmu_not_used',                           ['neigh'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1' + common_test_plusargs )
t('perf_cntr_diag_BRANCHES',    'pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=0' + common_test_plusargs )
t('perf_cntr_diag_RETIRED_INST', 'pmu/perf_cntr_diag',                        ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_RETIRED_INST', 'pmu/perf_cntr_diag',                        ['neigh'    ], test_name = 'pmu_perf_cntr_diag_RETIRED_INST_multicore', tags=['minion_diags', 'pmu'], diag_compile_opts="EXTRA_CFLAGS='-DNUM_MINIONS=8 -DPMU_MULTICORE=1'", plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0xff ' + common_test_plusargs )
t('perf_cntr_diag_MCYCLES',     'pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_MIN_ET_RSP',  'pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_TL_INST',     'pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_TL_OPS',      'pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_TFMA_INST',   'pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_TQUANT_INST', 'pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_TREDUCE_INST','pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], diag_compile_opts="EXTRA_CFLAGS='-DNUM_MINIONS=8'", plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0xff +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_TSTORE_INST', 'pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_TS_OPS',      'pmu/perf_cntr_diag',                         ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_MIN_ICACHE_REQ_RSP', 'pmu/perf_cntr_diag',                  ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_L2_MISS_REQ',  'pmu/perf_cntr_diag',                        ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_L2_MISS_REQ_ovf',  'pmu/perf_cntr_diag',                    ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_L2_EVICT_REQ', 'pmu/perf_cntr_diag',                        ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1 +min_pmu_mon_en=1' + common_test_plusargs )
t('perf_cntr_diag_L2_stress',    'pmu/perf_cntr_diag',                        ['fc2shire' ], tags=['minion_diags', 'pmu'], diag_compile_opts="EXTRA_CFLAGS='-DNUM_MINIONS=32'", plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x3 +MINION_MASK=0xffffffff +min_pmu_mon_en=1 ' + common_test_plusargs )
t('perf_cntr_diag_DCACHE_ACCESS', 'pmu/perf_cntr_diag',                       ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1' + common_test_plusargs )
t('perf_cntr_diag_DCACHE_MISSES', 'pmu/perf_cntr_diag',                       ['neigh'    ], tags=['minion_diags', 'pmu'], plusargs='+THREAD_MASK=0x3 +SHIRE_MASK=0x1 +MINION_MASK=0x1' + common_test_plusargs )
t('pmc_access',                 'perfcounters/pmc_access',                    ['shire'    ], tags=['minion_diags', 'postsi_min_shire_ust'], plusargs='+THREAD_MASK=0x1 +SHIRE_MASK=0x1 +MINION_MASK=0x1' + common_test_plusargs )
t('lock_unlock_enabled',        'minion_feature_esr/lock_unlock_enabled',     ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('lock_unlock_disabled',       'minion_feature_esr/lock_unlock_disabled',    ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('user_scp_enabled',           'minion_feature_esr/user_scp_enabled',        ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('user_scp_disabled',          'minion_feature_esr/user_scp_disabled',       ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('user_cacheops_enabled',      'minion_feature_esr/user_cacheops_enabled',   ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('user_cacheops_disabled',     'minion_feature_esr/user_cacheops_disabled',  ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('ml_ops_enabled',             'minion_feature_esr/ml_ops_enabled',          ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x1 +CSR_STALL_DONE_COUNT=-1' + common_test_plusargs )
t('ml_ops_disabled',            'minion_feature_esr/ml_ops_disabled',         ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x1 +WFI_ENDS_TEST=0' + common_test_plusargs )
t('multi_thread_enabled',       'minion_feature_esr/multi_thread_enabled',    ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('multi_thread_disabled',      'minion_feature_esr/multi_thread_disabled',   ['shire'    ], tags=['minion_diags'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFFFFFFFF +THREAD_MASK=0x3' + common_test_plusargs )
t('gfx_ops_enabled',            'minion_feature_esr/gfx_ops_enabled',         ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('gfx_ops_disabled',           'minion_feature_esr/gfx_ops_disabled',        ['shire'    ], tags=['minion_diags', 'postsi_min_shire_noc_ust'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1' + common_test_plusargs )
t('fcio_b4c_1',                       'fullchip/fcio_b4c_1',                               ['fc_ioshire'], tags=['b4c_fcio_smoke'], plusargs='+SHIRE_MASK=0x500000001 +SP_TESTNAME=fcio_b4c_1 +trans_timeout=50000 +FC_NOC_CLOCK_FREQ=NO_CLOCK_CONFIG' + common_test_plusargs, ioshire_test=True )
#t('mtip',                       'mtip',                                       ['shire' ], tags=['minion_diags'] , plusargs ='+SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x1' + common_test_plusargs )
t('errors',                     'vm_boundary/errors',                         ['neigh'    ], tags=['postponed'] , plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1 +ECALL_ENDS_TEST=0 +l1d_sb_en=0' + common_test_plusargs, test_name='vm_boundary_errors')
t('dll_clk_delays',             'dll_clk_delays',                             ['shire'    ], tags=['dll_clk_delays'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x0 +THREAD_MASK=0x0 +EARLY_TEST_FINISH_AFTER_APB_INJECTOR_DONE +CLK_DLY_CTL=RAND +disable_shire_cache_enable_check' + common_test_plusargs, test_count=10 )
# Test for UC Broadcast
t('uc_broadcast_postsi',        'uc_broadcast_postsi',                        ['shire'],     tags=['postsi_min_shire_noc_ust'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0x00000001 +THREAD_MASK=0x1' + common_test_plusargs)

t('init_rf',                'vpu/init_rf',                                    ['fc'       ], tags=['minion_diags'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )
t('l1_only',                'l1_only',                                        ['shire'    ], tags=['minion_diags'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0x1 +SHIRE_MASK=0x1" + common_test_plusargs )
t('power_vpu_tima_workaround', 'power_vpu/tima/workaround',                   ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )
t('l1scp_selfchk_vpu_tima', 'power_vpu/tima/l1scp_selfchk',                   ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )
t('power_vpu_tima_lite',    'power_vpu/tima/lite',                            ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )
t('power_vpu_bypass',       'power_vpu/bypass',                               ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )
t('power_vpu_bypass_workaround',       'power_vpu/bypass/workaround',         ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )

t('fmax_noc_esr',           'fmax/fmax_noc_esr',                              ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )
t('fmax_noc_amo',           'fmax/fmax_noc_amo',                              ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )

t('sc_noc_power_virus',     'fmax/sc_noc_power_virus',                        ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )
t('sc_noc_power_virus_32gb','fmax/sc_noc_power_virus/32gb',                   ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )
t('ms_power_virus',         'minshire_power_virus',                           ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )
t('ms_ping_pong_power_virus', 'minshire_power_virus/ping_pong',               ['fc'       ], tags=['power_virus'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff" + common_test_plusargs )

#t('fmax_power_loads_and_stores',    'fmax_power/fmax_power_loads_and_stores', ['fc'    ], tags=['minion_diags'], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff " + common_test_plusargs )
t('fmax_power_loads_and_stores',    'fmax_power/fmax_power_loads_and_stores', ['fc'    ], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3ffffffff " + common_test_plusargs )
#t('fmax_power_loads_and_stores',    'fmax_power/fmax_power_loads_and_stores', ['shire'    ], plusargs="+THREAD_MASK=0x3 +MINION_MASK=0xffffffff " + common_test_plusargs )

# Specific tests added which are parameterized
for i in range (4):
    # Allow parameterization to run in every neighborhood. By default it runs 2 thread(s) even though we could change that with THREAD_MASK if needed.
    minion_id = random.randrange(i * 8, (i + 1) * 8)
    minion_mask = 1 << (minion_id)

    shire_id = 0
    shire_mask = 1 << (shire_id)

    s_m_plusargs = f'+SHIRE_MASK={shire_mask:#x} +MINION_MASK={minion_mask:#x}'
    s_m_ext = f's{str(shire_id)}m{str(minion_id)}'

    # postsi_min_shire_param_ust tag
    t('amoaddl_w', 'atomic_extension/amoaddl_w', ['shire'], test_name=f'amoaddl_w_in_shire_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('amoandl_w', 'atomic_extension/amoandl_w', ['shire'], test_name=f'amoandl_w_in_shire_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('amomaxl_w', 'atomic_extension/amomaxl_w', ['shire'], test_name=f'amomaxl_w_in_shire_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('amominl_w', 'atomic_extension/amominl_w', ['shire'], test_name=f'amominl_w_in_shire_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('amoorl_w',  'atomic_extension/amoorl_w',  ['shire'], test_name=f'amoorl_w_in_shire_{s_m_ext}',  tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('amoxorl_w', 'atomic_extension/amoxorl_w', ['shire'], test_name=f'amoxorl_w_in_shire_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('atomic_extension_ld', 'atomic_extension/atomic_extension_ld', ['shire'], test_name=f'atomic_extension_ld_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs )
    t('csr_write_with_self_check', 'csr_write_with_self_check', ['shire'], test_name=f'csr_write_with_self_check_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs)
    t('exceptions', 'exceptions', ['shire'], test_name=f'exceptions_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs)
    t('pmc_access', 'perfcounters/pmc_access', ['shire'], test_name=f'pmc_access_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs)

    t('RTLMIN-759',  'jiras/RTLMIN-759',  ['shire'], test_name=f'rtlmin_759_{s_m_ext}',  tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs}' + common_test_plusargs )
    t('rtlmin_2600', 'jiras/rtlmin_2600', ['shire'], test_name=f'rtlmin_2600_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('rtlmin_2612', 'jiras/rtlmin_2612', ['shire'], test_name=f'rtlmin_2612_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('rtlmin_2902', 'jiras/rtlmin_2902', ['shire'], test_name=f'rtlmin_2902_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('rtlmin_5651', 'jiras/rtlmin_5651', ['shire'], test_name=f'rtlmin_5651_{s_m_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs)

    # postsi_min_shire_noc_param_ust tag
    t('lock_unlock_disabled', 'minion_feature_esr/lock_unlock_disabled', ['shire'], test_name=f'lock_unlock_disabled_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'] , plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('lock_unlock_enabled', 'minion_feature_esr/lock_unlock_enabled', ['shire'], test_name=f'lock_unlock_enabled_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'] , plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('user_cacheops_disabled', 'minion_feature_esr/user_cacheops_disabled', ['shire'], test_name=f'user_cacheops_disabled_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'] , plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('user_cacheops_enabled', 'minion_feature_esr/user_cacheops_enabled', ['shire'], test_name=f'user_cacheops_enabled_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'] , plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('user_scp_disabled', 'minion_feature_esr/user_scp_disabled', ['shire'], test_name=f'user_scp_disabled_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'] , plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('user_scp_enabled', 'minion_feature_esr/user_scp_enabled', ['shire'], test_name=f'user_scp_enabled_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'] , plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('gfx_ops_disabled', 'minion_feature_esr/gfx_ops_disabled', ['shire'], test_name=f'gfx_ops_disabled_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'] , plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('gfx_ops_enabled', 'minion_feature_esr/gfx_ops_enabled', ['shire'], test_name=f'gfx_ops_enabled_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'] , plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('pmu_not_used', 'pmu/pmu_not_used', ['shire'], test_name=f'pmu_not_used_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x3' + common_test_plusargs)
    t('uc_broadcast_postsi', 'uc_broadcast_postsi', ['shire'], test_name=f'uc_broadcast_postsi_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{s_m_plusargs} +THREAD_MASK=0x1' + common_test_plusargs)
    t('tensor_load_setup_b', 'tensors/tensor_load_setup_b', ['shire'], test_name=f'tensor_load_setup_b_postsi_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'], diag_compile_opts="EXTRA_CFLAGS+=' -DSELF_CHECK'", plusargs=f'{s_m_plusargs} +ECALL_ENDS_TEST=0' + common_test_plusargs)
    t('mcode', 'mcode', ['shire'], test_name=f'mcode_{s_m_ext}', tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{s_m_plusargs}' + common_test_plusargs)


## Charles: parameterization: try to run postsi test on different Minion (#0 - #31) on "shire"(#0) build model; 

# for test t_amocmpswapl_{w,d}_in_shire_postsi and t_famoswapl_pi_in_shire_postsi (in postsi_min_shire_ust): 
# Let's run each test on 2 minions (4 harts);

#for i in range (0, 16):    # loop 16 iterations, for all 32 Minions (2 each iteration);
#for i in [0, 15]:          # loop: only for M0 & M1, and M30 & M31:
#    mn_mask = (0x3 << i*2)

#for i in [0, 8, 16, 24]:   # loop: only for M0+M1, M8+M9, M16+M17, M24+M25 
#    mn_mask = (0x3 << i)

#for j in range (0, 16):    # loop 16 iterations, for all 32 Minions (2 each iteration);
#    i = j*2                # i = 0, 2, 4, ..., 30

#for i in range (0, 30):     # loop from 0 to 30
#    mn_mask = (0x3 << i)    # enable 2 Minions, e.g.: M0M1, M1M2, M2M3, ... M30M31
#    sh_mn_mask_plusargs = f'+SHIRE_MASK=0x1 +MINION_MASK={mn_mask:#x}'
#    sh_mn_ext = f's0m{str(i)}m{str(i+1)}'

for i in range (4):    # due to limited VCS license resource: loop through 4 Neigh; 
    mn_id = random.randrange(i * 8, (i + 1) * 8)    # pick a random minion from each of the 4 Neigh
    # since these tests run 2-Core: avoid mn_id=31:
    if (mn_id == 31):
        mn_id = 30
    mn_mask = (0x3 << mn_id)
    sh_id = 0
    sh_mask = (0x1 << sh_id)

    sh_mn_mask_plusargs = f'+SHIRE_MASK=0x1 +MINION_MASK={mn_mask:#x}'
    sh_mn_ext = f's0m{str(mn_id)}m{str(mn_id+1)}'

    # Phase 1: for test t_amocmpswapl_{w,d}_in_shire_postsi and t_famoswapl_pi_in_shire_postsi (in postsi_min_shire_ust): 
    t('t_amocmpswapl_w',            'atomics_with_threads',                   ['shire'    ], test_name=f't_amocmpswapl_w_postsi_{sh_mn_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3' + common_test_plusargs )
    t('t_amocmpswapl_d',            'atomics_with_threads',                   ['shire'    ], test_name=f't_amocmpswapl_d_postsi_{sh_mn_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3' + common_test_plusargs )
    t('t_famoswapl_pi',             'atomics_with_threads',                   ['shire'    ], test_name=f't_famoswapl_pi_postsi_{sh_mn_ext}',  tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3' + common_test_plusargs )

    # Phase 2: for test t_amocmpswapg_{w,d}_in_shire_postsi and t_famoswapg_pi_in_shire_postsi (in postsi_min_shire_noc_ust):
    t('t_amocmpswapg_w',            'atomics_with_threads',                   ['shire'    ], test_name=f't_amocmpswapg_w_postsi_{sh_mn_ext}', tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3' + common_test_plusargs )
    t('t_amocmpswapg_d',            'atomics_with_threads',                   ['shire'    ], test_name=f't_amocmpswapg_d_postsi_{sh_mn_ext}', tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3' + common_test_plusargs )
    t('t_famoswapg_pi',             'atomics_with_threads',                   ['shire'    ], test_name=f't_famoswapg_pi_postsi_{sh_mn_ext}',  tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3' + common_test_plusargs )


## special: some 2-Core (2-Minion) tests can only run with M0M1, M2M3, ... M30M31.
#for k in range (0, 16):     # loop from 0 to 15
#    i = k*2;                # this is minion_id (even number: 0, 2, 4, ... 30)
#    mn_mask = (0x3 << i)    # enable 2 Minions, e.g.: M0M1, M2M3, ... M30M31
#    sh_mn_mask_plusargs = f'+SHIRE_MASK=0x1 +MINION_MASK={mn_mask:#x}'
#    sh_mn_ext = f's0m{str(i)}m{str(i+1)}'
#
for i in range (4):    # due to limited VCS license resource: loop through 4 Neigh; 
    mn_id = random.randrange(i * 8, (i + 1) * 8)    # pick a random minion from each of the 4 Neigh
    # convert odd number to even number:
    mn_id = int(mn_id/2) * 2;
    mn_mask = (0x3 << mn_id)
    sh_id = 0
    sh_mask = (0x1 << sh_id)

    sh_mn_mask_plusargs = f'+SHIRE_MASK=0x1 +MINION_MASK={mn_mask:#x}'
    sh_mn_ext = f's0m{str(mn_id)}m{str(mn_id+1)}'

    t('ml_ops_enabled',             'minion_feature_esr/ml_ops_enabled',      ['shire'    ], test_name=f'ml_ops_enabled_{sh_mn_ext}',         tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +CSR_STALL_DONE_COUNT=-1' + common_test_plusargs )
    t('ml_ops_disabled',            'minion_feature_esr/ml_ops_disabled',     ['shire'    ], test_name=f'ml_ops_disabled_{sh_mn_ext}',        tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +WFI_ENDS_TEST=0' + common_test_plusargs )



## Charles: parameterize: run postsi test on "fc2shire" build model (S0 & S32); note: each runs on 2 minions (4 harts);
#for i in range (0, 16):
#    mn_mask = (0x3 << i*2)
#    # postsi test run on "fc2shire" build model in S0:
#    t('t_amocmpswapl_w',            'atomics_with_threads',                   ['fc2shire' ], test_name='t_amocmpswapl_w_in_fc2shire_postsi_param_s0_m'+str(i*2)+'m'+str(i*2+1),  tags=['postsi_min_shire_param_ust'], plusargs=f'+SHIRE_MASK=0x000000001 +MINION_MASK={mn_mask:#x} +THREAD_MASK=0x3' + common_test_plusargs )
#    # postsi test run on "fc2shire" build model in S32:
#    t('t_amocmpswapl_w',            'atomics_with_threads',                   ['fc2shire' ], test_name='t_amocmpswapl_w_in_fc2shire_postsi_param_s32_m'+str(i*2)+'m'+str(i*2+1), tags=['postsi_min_shire_param_ust'], plusargs=f'+SHIRE_MASK=0x100000000 +MINION_MASK={mn_mask:#x} +THREAD_MASK=0x3' + common_test_plusargs )

## Charles: parameterize: run postsi test on "fc4shire" build model (S0, S1, S2, & S32); each runs on 2 minions (4 harts);
#for i in range (0, 16):
#    mn_mask = (0x3 << i*2)
#    # postsi test run on "fc4shire" build model in S0:
#    t('t_amocmpswapl_w',            'atomics_with_threads',                   ['fc4shire' ], test_name='t_amocmpswapl_w_in_fc4shire_postsi_param_s0_m'+str(i*2)+'m'+str(i*2+1),  tags=['postsi_min_shire_param_ust'], plusargs=f'+SHIRE_MASK=0x000000001 +MINION_MASK={mn_mask:#x} +THREAD_MASK=0x3' + common_test_plusargs )
#    # postsi test run on "fc4shire" build model in S1:
#    t('t_amocmpswapl_w',            'atomics_with_threads',                   ['fc4shire' ], test_name='t_amocmpswapl_w_in_fc4shire_postsi_param_s1_m'+str(i*2)+'m'+str(i*2+1),  tags=['postsi_min_shire_param_ust'], plusargs=f'+SHIRE_MASK=0x000000002 +MINION_MASK={mn_mask:#x} +THREAD_MASK=0x3' + common_test_plusargs )
#    # postsi test run on "fc4shire" build model in S2:
#    t('t_amocmpswapl_w',            'atomics_with_threads',                   ['fc4shire' ], test_name='t_amocmpswapl_w_in_fc4shire_postsi_param_s2_m'+str(i*2)+'m'+str(i*2+1),  tags=['postsi_min_shire_param_ust'], plusargs=f'+SHIRE_MASK=0x000000004 +MINION_MASK={mn_mask:#x} +THREAD_MASK=0x3' + common_test_plusargs )
#    # postsi test run on "fc4shire" build model in S32:
#    t('t_amocmpswapl_w',            'atomics_with_threads',                   ['fc4shire' ], test_name='t_amocmpswapl_w_in_fc4shire_postsi_param_s32_m'+str(i*2)+'m'+str(i*2+1), tags=['postsi_min_shire_param_ust'], plusargs=f'+SHIRE_MASK=0x100000000 +MINION_MASK={mn_mask:#x} +THREAD_MASK=0x3' + common_test_plusargs )



## parameterize postsi_ust tests: 
## Here each test runs one Minion.

#for i in range (0, 32):   # loop: from M0 to M31 
#    mn_mask = (0x1 << i)
#    sh_mn_mask_plusargs = f'+SHIRE_MASK=0x1 +MINION_MASK={mn_mask:#x}'
#    sh_mn_ext = f's0m{str(i)}'

#for i in [0, 8, 16, 24]:   # loop: only for M0, M8, M16, M24 
#    mn_mask = (0x1 << i)
#    sh_mn_mask_plusargs = f'+SHIRE_MASK=0x1 +MINION_MASK={mn_mask:#x}'
#    sh_mn_ext = f's0m{str(i)}'

for i in range (4):    # due to limited VCS license resource: loop through 4 Neigh; 
    mn_id = random.randrange(i * 8, (i + 1) * 8)    # pick a random minion from each of the 4 Neigh
    mn_mask = (1 << mn_id)
    sh_id = 0
    sh_mask = (1 << sh_id)

    sh_mn_mask_plusargs = f'+SHIRE_MASK=0x1 +MINION_MASK={mn_mask:#x}'
    sh_mn_ext = f's0m{str(mn_id)}'

    # Phase 1:
    t('atomic_extension_lsgsc_ps',  'atomic_extension/atomic_extension_lsgsc_ps', ['shire' ], test_name=f'atomic_extension_lsgsc_lps_{sh_mn_ext}', tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLOCAL_VEC_COH_MEM_OPS'" )
    t('atomic_extension_lw',        'atomic_extension/atomic_extension_lw',       ['shire' ], test_name=f'atomic_extension_lw_{sh_mn_ext}',        tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('tensor_load_mt',             'tensors/tensor_load_mt',                     ['shire' ], test_name=f'tensor_load_mt_{sh_mn_ext}',             tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('loads_and_stores_bypass',    'loads_and_stores_bypass',                    ['shire' ], test_name=f'loads_and_stores_bypass_{sh_mn_ext}',    tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('prefetch',                   'cache_ops/new_impl/prefetch',        ['shire' ], test_name=f'cacheops_prefetch_chicken_postsi_phs1_{sh_mn_ext}',  tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +min_frontend_clock_gate_disable=1 +CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
    t('lock_sw',                    'cache_ops/new_impl/lock_sw',         ['shire' ], test_name=f'cacheops_lock_sw_postsi_{sh_mn_ext}',        tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
    t('lock_va',                    'cache_ops/new_impl/lock_va',         ['shire' ], test_name=f'cacheops_lock_va_{sh_mn_ext}',               tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
    t('evict_va',                   'cache_ops/new_impl/evict_va',        ['shire' ], test_name=f'cacheops_evict_va_postsi_phs1_{sh_mn_ext}',  tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
    t('lock_sw',                    'cache_ops/new_impl/lock_sw_split',   ['shire' ], test_name=f'cacheops_lock_sw_split_postsi_{sh_mn_ext}',  tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
    t('cop_dirty_line',             'cache_ops/cop_dirty_line',           ['shire' ], test_name=f'cop_dirty_line_postsi_{sh_mn_ext}',          tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +CHECKER=0' + common_test_plusargs )
    t('cop_lock_zero',              'cache_ops/cop_lock_zero',            ['shire' ], test_name=f'cache_ops_cop_lock_zero_{sh_mn_ext}',        tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )

    t('flush_sw',                   'cache_ops/new_impl/flush_sw',        ['shire' ], test_name=f'cacheops_flush_sw_postsi_phs1_{sh_mn_ext}',  tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
    t('invalidate',                 'cache_ops/new_impl/invalidate',      ['shire' ], test_name=f'cacheops_invalidate_{sh_mn_ext}',            tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('evict_sw',                   'cache_ops/new_impl/evict_sw',        ['shire' ], test_name=f'cacheops_evict_sw_postsi_phs1_{sh_mn_ext}',  tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
    t('lock_sw',                    'cache_ops/new_impl/lock_sw_scp',     ['shire' ], test_name=f'cacheops_lock_sw_scp_postsi_{sh_mn_ext}',    tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
    t('flush_va',                   'cache_ops/new_impl/flush_va',        ['shire' ], test_name=f'cacheops_flush_va_postsi_phs1_{sh_mn_ext}',  tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DPOSTSI_PHASE1'" )
    t('exceptions',                 'exceptions',                         ['shire' ], test_name=f'exceptions_2T_{sh_mn_ext}',                  tags=['postsi_min_shire_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +ECALL_ENDS_TEST=0' + common_test_plusargs )

    # Phase 2:
    t('prefetch',                   'cache_ops/new_impl/prefetch',                ['shire' ], test_name=f'cacheops_prefetch_chicken_postsi_phs2_{sh_mn_ext}', tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +min_frontend_clock_gate_disable=1 +CHECKER=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
    t('evict_va',                   'cache_ops/new_impl/evict_va',                ['shire' ], test_name=f'cacheops_evict_va_postsi_phs2_{sh_mn_ext}',         tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK'" )
    t('atomic_extension_lsgsc_ps',  'atomic_extension/atomic_extension_lsgsc_ps', ['shire' ], test_name=f'atomic_extension_lsgsc_gps_{sh_mn_ext}',   tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DGLOBAL_VEC_COH_MEM_OPS'" )
    t('atomic_extension_gw',        'atomic_extension/atomic_extension_gw',       ['shire' ], test_name=f'atomic_extension_gw_{sh_mn_ext}',          tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )
    t('atomic_extension_gd',        'atomic_extension/atomic_extension_gd',       ['shire' ], test_name=f'atomic_extension_gd_{sh_mn_ext}',          tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs )

    ## (more) parameterize postsi_ust tests: Phase 2: 
    t('tensor_load_l2scp',          'tensors/tensor_load_l2scp',                  ['shire' ], test_name=f'tensor_load_l2scp_cov_postsi_{sh_mn_ext}', tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=48 -DPOSTSI_PARAM'" ) 
    t('tensor_load_setup_b',        'tensors/tensor_load_setup_b',                ['shire' ], test_name=f'tensor_load_setup_b_postsi_{sh_mn_ext}',   tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1 +ECALL_ENDS_TEST=0' + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS+=' -DSELF_CHECK'" ) 
    t('tensor_load',                'tensors/tensor_load',                        ['shire' ], test_name=f'tensor_load_postsi_{sh_mn_ext}',           tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs,  diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DLOOP_SIZE=48 -DPOSTSI_PARAM'" ) 



## param: neigh-based, 8-Minion tests (1-neigh): run on various Neigh:
## (Charles): WIP: test modified but still not yet working for param ... 
t('tensor_reduce_snd_rcv',      'tensors/tensor_reduce_snd_rcv',  ['shire' ], test_name='tensor_reduce_snd_rcv_postsi_s0n0', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8 -DPOSTSI_PARAM'", tags=['cye_postsi_min_shire_noc_param_ust'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_reduce_snd_rcv',      'tensors/tensor_reduce_snd_rcv',  ['shire' ], test_name='tensor_reduce_snd_rcv_postsi_s0n1', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8 -DPOSTSI_PARAM'", tags=['cye_postsi_min_shire_noc_param_ust'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF00 +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_reduce_snd_rcv',      'tensors/tensor_reduce_snd_rcv',  ['shire' ], test_name='tensor_reduce_snd_rcv_postsi_s0n2', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8 -DPOSTSI_PARAM'", tags=['cye_postsi_min_shire_noc_param_ust'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF0000 +THREAD_MASK=0x3' + common_test_plusargs )
t('tensor_reduce_snd_rcv',      'tensors/tensor_reduce_snd_rcv',  ['shire' ], test_name='tensor_reduce_snd_rcv_postsi_s0n3', diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8 -DPOSTSI_PARAM'", tags=['cye_postsi_min_shire_noc_param_ust'], plusargs='+SHIRE_MASK=0x1 +MINION_MASK=0xFF000000 +THREAD_MASK=0x3' + common_test_plusargs )

## param: neigh-based, 8-Minion tests (1-neigh): run on various Neigh:
for ne_id in range (4):    # one of 4 neigh 
    mn_mask = (0xFF << (ne_id*8))
    sh_id = 0
    sh_mask = (0x1 << sh_id)
    sh_mn_mask_plusargs = f'+SHIRE_MASK={sh_mask:#x} +MINION_MASK={mn_mask:#x}'
    sh_mn_ext = f's{str(sh_id)}n{str(ne_id)}'

    t('tensor_reduce_auto',         'tensors/tensor_reduce_auto',      ['shire' ], test_name=f'tensor_reduce_auto_8c_postsi_{sh_mn_ext}',      tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs,  diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=3 -DSELF_CHECK -DNUM_MINIONS=8 -DNUM_ITER=20 -DPOSTSI_PARAM'" )
    t('tensor_store_coop',          'tensors/tensor_store_coop',       ['shire' ], test_name=f'tensor_store_coop_postsi_{sh_mn_ext}',          tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode' + common_test_plusargs,  diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8 -DPOSTSI_PARAM'" ) 
    t('tensor_store_hitfifo',       'tensors/tensor_store_hitfifo',    ['shire' ], test_name=f'tensor_store_hitfifo_postsi_{sh_mn_ext}',       tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode' + common_test_plusargs,  diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DNUM_MINIONS=8'" ) 
    t('tensor_load_coop',           'tensors/tensor_load_coop',        ['shire' ], test_name=f'tensor_load_coop_postsi_{sh_mn_ext}',           tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +etlinkReqRdyDelays=short +memDelays=short +shire_coop_mode=1' + common_test_plusargs,  diag_compile_opts="EXTRA_CFLAGS='-DSELF_CHECK -DLOOP_SIZE=48 -DPOSTSI_PARAM'" ) 
    t('tensor_reduce_broadcast',    'tensors/tensor_reduce_broadcast', ['shire' ], test_name=f'tensor_reduce_broadcast_8c_postsi_{sh_mn_ext}', tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x1' + common_test_plusargs,  diag_compile_opts="EXTRA_CFLAGS='-DMAX_TREE=3 -DSELF_CHECK -DNUM_MINIONS=8 -DNUM_ITER=20 -DPOSTSI_PARAM'" ) 

    ## param: starvation tests: 8-Minion tests (1-neigh): run on various Neigh:
    t('rtlmin_6068_0',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_0_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_1',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_1_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_2',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_2_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_3',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_3_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_4',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_4_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_5',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_5_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_6',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_6_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_7',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_7_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_8',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_8_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_9',              'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_9_{sh_mn_ext}',     tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_10',             'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_10_{sh_mn_ext}',    tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_11',             'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_11_{sh_mn_ext}',    tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_12',             'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_12_{sh_mn_ext}',    tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_13',             'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_13_{sh_mn_ext}',    tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_14',             'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_14_{sh_mn_ext}',    tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_15',             'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_15_{sh_mn_ext}',    tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_16',             'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_16_{sh_mn_ext}',    tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )
    t('rtlmin_6068_17',             'starvation/rtlmin_6068',          ['shire' ], test_name=f'starvation_rtlmin_6068_17_{sh_mn_ext}',    tags=['postsi_min_shire_noc_param_ust'], plusargs=f'{sh_mn_mask_plusargs} +THREAD_MASK=0x3 +thread_timeout=3000' + common_test_plusargs )

#### Vmin tests
t('vpu_3ports_read',      'vmin/vpu_3ports_read',                      ['fc' ], tags=['vmin'], plusargs=f' +SHIRE_MASK=0x3ffffffff +MINION_MASK=0xffffffff +THREAD_MASK=0x3' + common_test_plusargs )


#### MMI bug tests (FV-259)
# tensor quant
t('fv-260_tquant',           'jiras/fv-260/tquant',                     ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('fv-260_tquant_all_combs', 'jiras/fv-260/tquant/all_combs',           ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
# tensor fma
t('fv-260_tfma32',           'jiras/fv-260/tfma/tfma32',                ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('fv-260_tfma16a32',        'jiras/fv-260/tfma/tfma16a32',             ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('fv-260_tima8a32',         'jiras/fv-260/tfma/tima8a32',              ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
# tensor fma tenb
t('fv-260_tfma32_tenb',      'jiras/fv-260/tfma/tfma32/tenb',           ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('fv-260_tfma16a32_tenb',   'jiras/fv-260/tfma/tfma16a32/tenb',        ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
# tensor reduce
t('fv-260_treduce',          'jiras/fv-260/treduce',                    ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x1' + common_test_plusargs )
# tensor op -> SIMD sequence
t('fv-305',                  'jiras/fv-305',                            ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
# tensor reduce -> tensor quant sequence
t('fv-306',                  'jiras/fv-306',                            ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x3' + common_test_plusargs )
# tensor reduce -> tensor op sequence
t('producer_tensor_reduce',  'jiras/fv-306/producer_tensor_reduce',     ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x3' + common_test_plusargs )
# tensor fma -> tensor op sequence
t('producer_tensor_fma',     'jiras/fv-306/producer_tensor_fma',        ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x3' + common_test_plusargs )
# tensor quant -> tensor op sequence
t('producer_tensor_quant',   'jiras/fv-306/producer_tensor_quant',      ['neigh' ], tags=['mmi_bug'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x3 +THREAD_MASK=0x3' + common_test_plusargs )


#### Erbium
erbium_builds=['neigh', 'cpu_ss_vl', 'cpu_ss_vcs']
cpu_ss_builds=['cpu_ss_vl', 'cpu_ss_vcs']
erbium_mask_1h = " +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x1 "
erbium_mask_2h = " +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3 "
erbium_mask_16h = " +SHIRE_MASK=0x1 +MINION_MASK=0xFF +THREAD_MASK=0x3 "

t('playground',                          'erbium/playground',                      erbium_builds, tags=['erbium'], plusargs=erbium_mask_16h + '+verbosity=DEBUG' + common_test_plusargs )
t('erbium_pmp',                          'erbium/erbium_pmp',                      cpu_ss_builds, tags=['erbium'], plusargs=erbium_mask_2h + common_test_plusargs )
t('erbium_pma',                          'erbium/erbium_pma',                      cpu_ss_builds, tags=['erbium'], plusargs=f' +SHIRE_MASK=0x1 +MINION_MASK=0x1 +THREAD_MASK=0x3' + common_test_plusargs )
t('erbium_reduce_invalid_pairs',         'erbium/erbium_reduce_invalid_pairs',     erbium_builds, tags=['erbium'], plusargs=erbium_mask_16h + '+CHECKER=1' + common_test_plusargs )
t('erbium_reduce_invalid_pairs',         'erbium/erbium_reduce_invalid_pairs',     erbium_builds, test_name="erbium_reduce_invalid_pairs_short", tags=['erbium'], plusargs=erbium_mask_16h + '+CHECKER=1' + common_test_plusargs,  diag_compile_opts="EXTRA_CFLAGS='-DSHORT'" )
t('erbium_msg_ext_illegal',              'erbium/erbium_msg_ext_illegal',          cpu_ss_builds, tags=['erbium'], plusargs=erbium_mask_16h + '+CHECKER=1' + common_test_plusargs )
t('erbium_cacheops_invalid_levels',      'erbium/erbium_cacheops_invalid_levels',  erbium_builds, tags=['erbium'], plusargs=erbium_mask_2h + '+CHECKER=1' + common_test_plusargs )
t('erbium_csr_illegal',                  'erbium/erbium_csr_illegal',              erbium_builds, tags=['erbium'], plusargs=erbium_mask_2h + '+CHECKER=1' + common_test_plusargs )
t('erbium_csr_warl',                     'erbium/erbium_csr_warl',                 erbium_builds, tags=['erbium'], plusargs=erbium_mask_2h + '+CHECKER=1' + common_test_plusargs )
t('graphics_isa',                        'erbium/graphics_isa',                    cpu_ss_builds, tags=['erbium'], plusargs=erbium_mask_1h + common_test_plusargs, test_count=10 )
t('erbium_memory_map_sanity_check',      'erbium/erbium_memory_map_sanity_check',  erbium_builds, tags=['erbium'], plusargs=erbium_mask_2h + '+CHECKER=0' + common_test_plusargs )
t('erbium_mtvec',                        'erbium/erbium_mtvec',                    erbium_builds, tags=['erbium'], plusargs=erbium_mask_1h + '+CHECKER=0' + common_test_plusargs )
t('erbium_clint_esrs',                   'erbium/erbium_clint_esrs',               cpu_ss_builds, tags=['erbium'], plusargs=erbium_mask_16h + common_test_plusargs )
t('erbium_clint_mtip',                   'erbium/erbium_clint_mtip',               cpu_ss_builds, tags=['erbium'], plusargs=erbium_mask_16h + '+WFI_ENDS_TEST=0' + common_test_plusargs )
t('erbium_clint_msip',                   'erbium/erbium_clint_msip',               cpu_ss_builds, tags=['erbium'], plusargs=erbium_mask_16h + '+WFI_ENDS_TEST=0' + common_test_plusargs )
t('ipi_redirect',                        'ipi_redirect',                           cpu_ss_builds, tags=['erbium'], plusargs=erbium_mask_16h + '+CSR_STALL_DONE_COUNT=0 +memDelays=long' + common_test_plusargs )
t('erbium_plic_interrupt_injc_test',     'erbium/erbium_plic_interrupt_injc_test', cpu_ss_builds, tags=['erbium'], plusargs=erbium_mask_2h + '+CHECKER=0 +ECALL_ENDS_TEST=0 +WFI_ENDS_TEST=0' + common_test_plusargs )
t('mram_self_check',                     'erbium/mram_self_check',                 erbium_builds, tags=['erbium'], plusargs=erbium_mask_16h + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DFROM_MRAM'" )
# power
t('power_vpu_ps',                        'power_vpu/ps',                           erbium_builds, test_name='power_vpu_ps',                 tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs)
t('power_vpu_ps',                        'power_vpu/ps',                           erbium_builds, test_name='power_vpu_ps_short',           tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=10'" )
t('power_vpu_ps',                        'power_vpu/ps',                           erbium_builds, test_name='power_vpu_ps_lite',            tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DINNER_LOOP_SIZE=1'" )
t('power_vpu_ps',                        'power_vpu/ps',                           erbium_builds, test_name='power_vpu_ps_careless',        tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DSKIP_CHECK=1'" )
t('power_vpu_tima',                      'power_vpu/tima',                         erbium_builds, test_name='power_vpu_tima',               tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs )
t('power_vpu_tima',                      'power_vpu/tima',                         erbium_builds, test_name='power_vpu_tima_short',         tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=1'" )
t('power_vpu_tfma32',                    'power_vpu/tfma32',                       erbium_builds, test_name='power_vpu_tfma32',             tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs )
t('power_vpu_tfma32',                    'power_vpu/tfma32',                       erbium_builds, test_name='power_vpu_tfma32_short',       tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=1'" )
t('power_vpu_ps_ping_pong',              'power_vpu/ps_ping_pong',                 erbium_builds, test_name='power_vpu_ps_ping_pong',       tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs )
t('power_vpu_ps_ping_pong',              'power_vpu/ps_ping_pong',                 erbium_builds, test_name='power_vpu_ps_ping_pong_short', tags=['erbium', 'power_virus'], plusargs=erbium_mask_16h + common_test_plusargs, diag_compile_opts="EXTRA_CFLAGS='-DLOOP_SIZE=10'" )
# CPU Subsystem Verilator tests
t('verilator',                           'erbium/verilator',                       erbium_builds, tags=['erbium'], plusargs=erbium_mask_1h + common_test_plusargs )

