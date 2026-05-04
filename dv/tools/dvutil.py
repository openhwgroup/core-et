# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


"""
Utilities for creating etdv.py config files.
"""

import logging
import os
import re
import sys
import random

# Import a few useful things to access some internal state
from ETdv.dvrun.config import get_user_arg
from ETdv.dvrun.main import (Main)
from ETdv.util import (as_list,flatten)
from ETdv.dvrun.config import new_build
from ETdv.dvrun.config import add_build
from ETdv.dvrun.config import new_test
from ETdv.dvrun.messaging import (info)
from ETdv.dvrun.messaging import (warn)

random.seed()

REPOROOT = os.getenv('REPOROOT')
sys.path.append(f'{REPOROOT}/dv/common/scripts')

def __get_options():
    return Main.the_one().options

def get_uarg_as_dict():
    """
    Collect --uarg key[=val],... into dict.
    :return: {key: val|None, ...}
    """
    user_args = {}
    for uarg in flatten([v.split(',') for v in get_user_arg()]):
        kv = uarg.split('=') + [None]  #add no value in case we just have option
        user_args[kv[0]] = kv[1]
    return user_args

def get_option_value(opt_nm):
    """
    Return value of option passed on command line.
    :param opt_nm: option name, e.g. '--test_name_enable'.
    :return: value or None.
    """
    return __get_options()[opt_nm]

def has_option(opt_nm):
    """
    Check if opt_nm passed on command line.
    :param opt_nm: option name, e.g. '--test_name_enable'.
    :return: True or False.
    """
    return get_option_value(opt_nm) is not None

def get_test_names():
    """
    Get values of --test_name_enable (or -t) options.
    :return: list of test names or [].
    """
    names = has_option('--test_name_enable') and get_option_value('--test_name_enable')
    if not names: names = []
    else: names = as_list(names.split(','))
    return names

def test_uarg(rex):
    """
    Look for user argument (passed using --uarg option to et-dvrun).
    Return true if match
    :param rex: match user argument.
    :return: True or False.
    """
    rex = re.compile(rex)
    for arg in get_user_arg():
        match = rex.match(arg)
        if match: return True
    return False

def get_uarg(rex):
    """
    Look for user argument (passed using --uarg option to et-dvrun).
    Return match to rex or None if no match.
    :param rex: match user argument.
    :return: match or None.
    """
    rex = re.compile(rex)
    for arg in get_user_arg():
        match = rex.match(arg)
        if match: return match
    return None

def get_uarg_str(rex):
    """
    Look for user argument (passed using --uarg option to et-dvrun).
    Return match string to rex or None if no match.
    :param rex: match user argument.
    :return: match or None.
    """
    uarg = get_uarg(rex)
    if uarg is None: return None
    return uarg.group(1)

def get_cm_uarg():
    """
    Return arg of '--uarg CM=arg'.
    :return: arg or None.
    """
    expect = r'^CM=(.+)'
    uarg = get_uarg(expect)
    if uarg is None: return None
    return uarg.group(1)

def add_cm_args(xtype, args=''):
    """
    Append -cm_dir and -cm options if '--uarg CM=arg' passed on et-dvrun
    command line.
    :param xtype: one of build|test.
    :param args: argument string to append to.
    :return: args + appended cm-options (or arg if no options).
    """
    assert xtype in ['build', 'test']
    cm = get_cm_uarg()
    cm = get_uarg_str(r'^CM=(.+)')
    if cm is not None:
        # See Wiki Esperanto VCS Code Coverage Options for documentation of default coverage options
        if cm == "default" :
            if xtype in ['build'] :
                cm = "line+cond+tgl+fsm+branch+assert -cm_cond std+allops+for+tf+ports+obs -cm_libs yv -cm_line contassign -cm_seqnoconst -cm_tgl portsonly"
            if xtype in ['test'] :
                cm = "line+cond+tgl+fsm+branch+assert"
            args += ' -cm_dir ${ETDV_RUNDIR}/' f'{xtype} -cm {cm} -cm_log /dev/null '
        elif cm == "func" :
            args += ' -cm_dir ${ETDV_RUNDIR}/' f'{xtype} -cm_log /dev/null '
        else :
            args += ' -cm_dir ${ETDV_RUNDIR}/' f'{xtype} -cm {cm} -cm_log /dev/null '
        if xtype in ['build'] :
            args += ' +define+EVL_COVERAGE_ON=1 +define+ET_COVERAGE=1 '

    # specify a cm hierarchy file
    hier = get_uarg_str(r'^cm_hier=(.+)')
    if hier is not None:
        if xtype in ['build'] :
            if cm is not None:
                if (cm != "func") :
                    args += f' -cm_hier {hier} '
    return args

def add_xprop_args(xtype, args):
    """
    Append xprop args to the build command if '--uarg xprop' passed on et-dvrun
    command line.
    :param xtype: one of build|test.
    :param args: argument string to append to.
    :return: args + appended cm-options (or arg if no options).
    """
    if get_uarg('xprop') :
        args = args + ' -xprop=tmerge +define+XPROP=1'
    return args

def repo_is_bisted():
    """
    Check to see if the repo is bisted.
    Returns True if `define ET_BIST_INSERTION 1 in $RTLROOT/inc/dft_defines.vh
    """
    from os import getenv
    RTLROOT = getenv('RTLROOT')
    assert RTLROOT is not None, "setenv RTLROOT must be defined"

    with open(RTLROOT+'/inc/dft_defines.vh') as myfile:
       dft_defines = myfile.read()
       if 'define ET_BIST_INSERTION       1' in dft_defines:
          return True
       elif 'define ET_BIST_INSERTION       0' in dft_defines:
          return False
       else:
          logging.error("$REPOROOT/dv/dvutil.py: Expected ET_BIST_INSERTION in $RTLROOT/inc/dft_defines.vh")

def add_gatesim_args(xtype):
    gatesimopts = []
    flist_args = ' GATE_NETLIST_FILES=\' '
    env_args = ''
    def_args = ''
    sim_args = ''
    mkdef_args = ''
    shire = ''
    if get_uarg('gatesim'):
        gatesimopts = get_uarg_str(r'^gatesim=(.+)').split(',')
        for opts in gatesimopts:
            b_arg = opts.split('=')
            if b_arg[0] in gatesim_db:
                b_arg_orig = b_arg[0]
                if b_arg[0] in ['PG_MINSHIRE']:
                    b_arg[0] = re.sub(r'PG_', '', b_arg[0])
                env_args += ' export ' + b_arg[0] + '_GATE_NETLIST_PATH=' + gatesim_db[b_arg_orig] + '; '
                def_args += ' +define+' + b_arg[0] + '_GATESIM_ENABLE'
                mkdef_args += ' ET_GATESIM_ENABLE=1 ' +  b_arg[0] + '_GATESIM_ENABLE=1'
                flist_args += gatesim_db[b_arg_orig] + ' '
                if len(b_arg) > 1:
                    if b_arg[0] in ['MINSHIRE','PG_MINSHIRE']:
                        shire = 'minionshire'
                        mkdef_args += '=' + b_arg[1]
                        def_args += '=' + b_arg[1]
                    else:
                        if ( '0x' in b_arg[1] ):
                            logging.error(" " + b_arg[1] + " is illegal verilog format for " + opts)
                            exit(1)
                        if not ( re.match(r"\d+", b_arg[1]) ):
                            logging.error(" " + b_arg[1] + " is illegal format for " + opts + " Example usage " + b_arg[0] + "=4'h3 or " + b_arg[0] + "=3")
                            exit(1)
                        b_arg_fix = re.sub(r"\d+\\*'h", "0x", b_arg[1])
                        mkdef_args += '=' + b_arg_fix
                        if shire in ['minionshire']:
                            b_arg[1] = re.sub(r"\\*'", "\\'", b_arg[1])
                        else:
                            b_arg[1] = re.sub(r"\\*'", "\\\\'", b_arg[1])
                        def_args += '=' + b_arg[1]
                def_args += ' '
                mkdef_args += ' '
            else:
                logging.error("Illegal module name '" +opts + "' passed to --uarg gatesim")
                logging.error("Make sure that module '" +opts + "' is included in dv/common/scripts/gate_netlist_db.py")
                exit(1)
    if xtype in ['envvar']:
        return env_args
    if xtype in ['flist']:
        flist_args += ' \''
        return flist_args
    if xtype in ['build']:
        def_args += ' +define+ET_LIBS_USE_STD_CELLS +define+ET_USE_ASIC_MEMS '
        if shire in ['minionshire']:
            def_args += ' +define+ET_PTPX_SIM +vcs+initreg+config+$REPOROOT/dv/tools/ms_gatesim_cfg.txt '
        elif b_arg[0] in ['DCU', 'PSHIRE_SS', 'PSHIRE_ET']:
            def_args += ' '          
        elif b_arg[0] in ['SOCTOP']:
            def_args += ' +define+REDEF_SNPS_CELLS '
        else:
            def_args += ' +vcs+initreg+random'
            #def_args += ' +vcs+initreg+config+$REPOROOT/dv/tools/io_gatesim_cfg.txt '
        return def_args
    if xtype in ['makedef']:
        if b_arg[0] in ['IOSHIRE_WRAPPER']:
            mkdef_args += ' ET_REG_DELAY_ONLY=0 '
        return mkdef_args
    if xtype in ['simvar']:
        sim_args += ' +init_mem_data=0 +et_gatesim_enable '
        if shire in ['minionshire']:
            sim_args += ' +vcs+initreg+config+$REPOROOT/dv/tools/ms_gatesim_cfg.txt '
        elif b_arg[0] in ['DCU', 'PSHIRE_SS', 'PSHIRE_ET', 'SOCTOP']:
            sim_args += '  '
        else:
            sim_args += ' +vcs+initreg+random'
            #sim_args += ' +vcs+initreg+config+$REPOROOT/dv/tools/io_gatesim_cfg.txt '
        return sim_args

'''
get_conf_files:
Get all configuration file(s) recursively from directory specified.
This is implemented as a generator.
'''
def get_conf_files(dir : str) -> str :
    for root, dirs, files in os.walk("{0}".format(dir), topdown=False):
        for name in files:
            filename = os.path.join(root, name)
            if (filename.endswith('.conf')):
                yield filename

'''
get_sting_test_name:
Get a symbolic/unique testname. Currently this is derived from the name of the file (without the .conf extension)
at the leaf level and an intermediate level after the riscv directory if one exists.
'''
RISCV_DIR = 'riscv'
def get_sting_test_name(prefix: str, conf_f : str) -> str :
    unique_test_name_with_dir = ''
    # First split on "/" symbol and get the last element
    file_subpath_list = conf_f.split('/')
    if (file_subpath_list[-2] != RISCV_DIR):
        if (file_subpath_list[-3] == RISCV_DIR):
            unique_test_name_with_dir = file_subpath_list[-2] + "_"
        elif (file_subpath_list[-5] == RISCV_DIR):
            unique_test_name_with_dir = file_subpath_list[-4] + "_" + file_subpath_list[-3] + "_"
        else:
            print("ERROR - Unexpected hierarchy in regress.d needs to be understood, Prefix: {0}, Conf: {1}".format(prefix, conf_f))
            sys.exit(1)

    filename = conf_f.split('/')[-1]

    # Then split on "." and split the name and extension
    name_fields = filename.split('.')
    return prefix + unique_test_name_with_dir + name_fields[0]


#
# Return a string of arguments to add to verilog simulation command
#
# Example:
# --uarg sv_sim_args="+foo -bar"
#
def get_user_sv_sim_args():
  """
  get_user_sv_sim_args:
  parse uargs for system verilog simulation args. Gather then into a string
  return: all user system verilog simulation args
  """
  sv_sim_args=""
  usr_args=get_user_arg()
  usr_sim_args = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('sv_sim_args=')]
  if usr_sim_args:
     for p in usr_sim_args:
       sv_sim_args += ' ' + p
  return sv_sim_args

#
# Return a string of arguments to add to verilog build command
#
# Example:
# --uarg sv_build_args="+define+foo -bar"
#
def get_user_sv_build_args():
  """
  get_user_sv_build_args:
  parse uargs for system verilog build args. Gather then into a string
  return: all user system verilog simulation args
  """
  sv_build_args=""
  usr_args=get_user_arg()
  usr_build_options = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('sv_build_args=')]
  if usr_build_options:
     for p in usr_build_options:
        sv_build_args += ' ' + p
  return sv_build_args

#
# Return a string of arguments to add to test compile
#
# Example:
# --uarg cflags_args="-Dfoo -Dbar"
#
def get_user_cflags_args():
  """
  get_user_cflags_args
  cflags to compile the test differently based on c-defines. Gather them into a string
  return: all user cflags
  """
  cflags_args=""
  usr_args=get_user_arg()
  usr_build_options = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('cflags_args=')]
  if usr_build_options:
     for p in usr_build_options:
        cflags_args += ' ' + p
  return cflags_args

#
# Return a string of arguments to add to testcase makefile command line
#
# Example:
# --uarg tc_make_args="+foo -bar"
#
def get_user_tc_make_args():
  """
  get_user_tc_make_args:
  parse uargs for test case make args. Gather then into a string
  return: all user system verilog simulation args
  """
  tc_make_args=""
  usr_args=get_user_arg()
  usr_make_args = [arg.split('=', 1)[-1] for arg in usr_args if arg.startswith('tc_make_args=')]
  if usr_make_args:
     for p in usr_make_args:
       tc_make_args += ' ' + p
  return tc_make_args

def get_bisted_rtl_build_args():
  """
  get_bisted_rtl_build_args:
  Add flags needed for bisted RTL into a string
  return: build arguments for bisted RTL
  """
  usr_args=get_user_arg()
  sv_build_args=""
  if usr_args.count('use_bisted_rtl') != 0:
    sv_build_args='+define+ET_USE_BIST_RTL +define+ET_BIST_INSERTION=1 +define+ET_USE_ASIC_MEMS '
  return sv_build_args

def get_uvm_phase_debug_args():
  """
  get_uvm_phase_debug_args:
  returns a string with numerous flags for tracing and debugging UVM Phases
  return: string of verilog runtime arguments
  """
  usr_args=get_user_arg()
  sv_sim_args=""
  if usr_args.count('use_uvm_phase_debug') != 0:
    sv_sim_args='+UVM_OBJECTION_TRACE  +UVM_PHASE_TRACE +display_objections'
  return sv_sim_args

def get_force_list_build_args():
  """
  get_force_list_build_args:
  return a string with list of VCS build specific arguments to enable reporting of forces
  return: string of VCS specific runtime arguements
  """
  usr_args=get_user_arg()
  sv_build_args=""
  if usr_args.count('force_list') != 0:
    sv_build_args=' -force_list  -debug_access+f+r+cbk'   # +r+cbk is needed for PLI forces.  +f is for language forces
  return sv_build_args

def get_force_list_sim_args():
  """
  get_force_list_sim_args:
  return a string with list of VCS runtime specific arguments to enable reporting of forces
  return: string of VCS specific runtime arguements
  """
  usr_args=get_user_arg()
  sv_sim_args=""
  if usr_args.count('force_list') != 0:
    sv_sim_args=' -force_list sim_force_list.log '
  return sv_sim_args

def printbuild(myclass, full=1):
    print (f'build_name={myclass.config["name"]}')
    if full:
        print (f'\trun_cmd={myclass.config["run_cmd"]}')

def printtest(myclass, full=1):
    print (f'\t{myclass.config["name"]}')
    if full:
        print (f'\trun_cmd={myclass.config["run_cmd"]}')

def printbuildstests(builds, buildslist=None, testslist=None):
    # if build arg and test arg both have either full or all
    if any(x in buildslist[0] for x in ['full', 'all']) and any(x in testslist[0] for x in ['full', 'all']):
        for key in sorted(builds):
            printbuild(myclass=builds[key], full=buildslist[0]=='full')
            print ("\tTests: Count=" + str(len(builds[key].tests)))
            for test in builds[key].tests:
                printtest(myclass=test, full=testslist[0]=='full')
            print ()
    # if build arg have either full or all
    elif any(x in buildslist[0] for x in ['full', 'all']):
        for key in sorted(builds):
            printbuild(myclass=builds[key], full=buildslist[0]=='full')
            # if we are here, testlist will have testname rather than key words of full or all
            print ("\tTests:")
            for test in sorted(testslist):
                if test in builds[key].tests:
                    printtest(myclass=test, full=['full'])
            print ()
    elif any(x in testslist[0] for x in ['full', 'all']):
        for key in sorted(buildslist):
            # if we are here, testlist will have key words of full or all
            print ("\tTests:")
            for test in sorted(builds[key].tests):
                printbuild(myclass=builds[key], full=buildslist[0]=='full')
                printtest(myclass=test, full=['full'])
            print ()
    else:
        for key in sorted(buildslist):
            printbuild(myclass=builds[key], full=buildslist[0]=='full')
            # if we are here, testlist will have testname rather than key words of full or all
            print ("\tTests:")
            for test in sorted(testslist):
                if test in builds[key].tests:
                    printtest(myclass=test, full=['full'])
            print ()

def printbuilds(builds, buildslist=None):
    # if build arg and test arg both have either full or all
    if any(x in buildslist[0] for x in ['full', 'all']):
        for key in sorted(builds):
            printbuild(myclass=builds[key], full=buildslist[0]=='full')
    else:
        for build in sorted(buildslist):
            if not build in builds:
                error('Msgs-1',f'Build ' + build + ' is not valid')
                exit(1)
            printbuild(myclass=builds[build], full=1)


def lsbuildstests(builds):
    opt_lstests = 0
    opt_lsbuilds = 0
    testslist = {}
    buildslist = {}
    if get_uarg('lstests'):
        opt_lstests = 1
        testslist = get_uarg_str(r'^lstests=(.+)').split(',')
    if get_uarg('lsbuilds'):
        opt_lsbuilds = 1
        buildslist = get_uarg_str(r'^lsbuilds=(.+)').split(',')
    if opt_lstests and opt_lsbuilds:
        printbuildstests(builds=builds, buildslist=buildslist, testslist=testslist)
        exit(0)
    elif opt_lstests:
        printbuildstests(builds=builds, buildslist=['full'], testslist=testslist)
        exit(0)
    elif opt_lsbuilds:
        printbuilds(builds=builds, buildslist=buildslist)
        exit(0)


dvutil_default_build_check = \
      "@if [ -e $(@D)/vbuild/simv ]; then echo pass > $TARGET; else echo fail > $TARGET; fi"
dvutil_default_test_check = '@et-dvrun-check-log test ${<}.log ${@} ${SHOW_FAIL}'

dvutil_build_list = []

def dvutil_add_build(build_name, build_cmd, check_cmd=dvutil_default_build_check, slurmopts=None):
   """
   Add a build
   Note that the build is run from the dvrun build directory
   build_name = name of the build.  Tests can later be added to builds by build_name
   build_cmd = The command to run
   """
   for b in dvutil_build_list:
      if build_name == b['build_name']:
         warn('et_add_build: build_name ' + build_name + ' already exists')
         break

   # Create a directory for the build
   full_build_cmd = \
       f'cd $ETDV_RUNDIR; \n'

   full_build_cmd += build_cmd

   build_ptr = new_build(name=build_name, run_cmd=full_build_cmd, check_cmd=check_cmd, slurmopts=slurmopts)
   add_build(build_ptr)

   build = {'build_name': build_name,
            'build_cmd': build_cmd,
            'num_tests': 0};
   build['build_ptr'] = build_ptr;
   dvutil_build_list.append(build.copy())
   info('Msgs-1', "Adding build " + build_name + " - Build command: " + full_build_cmd)

def filter_builds_by_name(builds, names):
   # If no name regular expression, return unfiltered list
   if names is None:
      return matching_builds
   if type(names) == type("a"):
      comp_expr = re.compile(names)
      matching_builds = [x for x in builds if re.search(comp_expr, x['build_name'])]
      return matching_builds
   if type(names) == type([]):
      for expr in names:
         comp_expr = re.compile(expr)
         matching_builds = [x for x in builds if re.search(comp_expr, x['build_name'])]
      return matching_builds

def get_rand():
   """Returns an 8 digit random number."""
   return (str(random.randint(1,(10**8) -1)))

def dvutil_add_test(test_name, test_cmd, build_name=None, name=None, tags=None, ntimes=1, pre_test=None, post_test=None, check_cmd=dvutil_default_test_check, slurmopts=None):
   """
   Add test to builds that match the build_name
   Note that the test is run from the dvrun test directory

   build_name= regular expression for matching builds by name.
      Can be a list in which case, the test is added if the build
      matches all of the list entries.
   test_cmd = The test command to run against the build.
   ntimes = Add the test to the build ntimes.  Generally used with random tests, where each test has a
      unique random seed.  To help with that, the string "REPLACE_WITH_RAND" if found in test_cmd will
      be replaced with an 8 digit random number.
   """
   assert test_name is not None and test_cmd is not None
   # replace "." with "_" in test_name
   test_name = re.sub(r'\.', '_', test_name)

   matching_builds = dvutil_build_list
   matching_builds = filter_builds_by_name(matching_builds, build_name)

   for build in matching_builds:
      info('Msgs-1', 'Adding test ' + build['build_name'])
      for ii in range(ntimes) :
         full_test_name = str(build['num_tests'])+"_"+test_name

         # If test_cmd is function call it
         # Passing in test_cmd as a function allows test plusargs to be randomly selected independently for
         # each iteration of the test.
         if hasattr(test_cmd, '__call__') :
            rand_test_cmd = test_cmd(build['build_cmd'])
         else :
            rand_test_cmd = test_cmd

         # Add random seeding
         rand_test_cmd = re.sub(r'REPLACE_WITH_RAND', get_rand(), rand_test_cmd)
         rand_test_cmd = re.sub(r'REPLACE_WITH_FULL_TEST_NAME', full_test_name, rand_test_cmd)

         full_test_cmd = \
             f"export testRunDir=$ETDV_RUNDIR \n" \
             'cd ${ETDV_RUNDIR} \n' \
             f"{pre_test if pre_test is not None else ''} \n" \
             f"{rand_test_cmd} \n" \
             f"{post_test if post_test is not None else ''}"

         test = new_test(\
             name=full_test_name,
             run_cmd=full_test_cmd,
             check_cmd=check_cmd,
             slurmopts=slurmopts
         )
      if tags is not None: test['tags'] = tags
      build['build_ptr'].add_test(test)
      build['num_tests'] = build['num_tests'] + 1


#
# Restrict --noslurm from running on certain hosts.  Explicity error to user
#
from ETdv.dvrun.main import Main
import socket
hostname=socket.gethostname()
RESTRICTED_HOSTS=["sc-rtl01","sc-dv01","sc-dv05","sc-slurm","sc-master","sc-jenkins01","sc-vnc","sc-vnc01","sc-vnc02"]

if (Main.the_one().has_option('--noslurm')) and any(hostname in s for s in RESTRICTED_HOSTS):
  logging.error(f' et-dvrun option --noslurm is not supported on restricted host={hostname}.  Please use compute server or remove --noslurm')
  logging.error(f' Restricted hosts: {RESTRICTED_HOSTS} ')
  exit(101);


