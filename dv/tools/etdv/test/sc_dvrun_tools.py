#REPOROOT = getenv('REPOROOT')
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

#assert REPOROOT is not None, "setenv REPOROOT must be defined"
#CWD = getenv('PWD')

import glob
import pathlib
import re
import sys
import os
import random


#sys.path.append(os.path.abspath(REPOROOT + "/dv/tests/shire_cache/regress_lists"))
#try:
#       import sc_dvrun_tools
#except ImportError:
#       print("Could not import sc_dvrun_tools.py")
#       sys.exit()


def flatten(lis):
   """Given a list, possibly nested to any level, return it flattened."""
   new_lis = []
   for item in lis:
      if type(item) == type([]):
         new_lis.extend(flatten(item))
      else:
         new_lis.append(item)
   return new_lis

def one_of(*lis):
   """Given a list, possibly nested to any level, return one of the list entries."""
   return str(random.choice(flatten(lis)))

def get_rand():
   """Returns an 8 digit random number."""
   return (str(random.randint(1,(10**8) -1)))

def sc_add_build(builds, build_name, build_cmd, config_name=None, fns=None):
   assert config_name is not None
   assert fns is not None

   new_build = fns['new_build']
   add_build = fns['add_build']

   #kwp CWD = getenv('PWD')

   build_dir='${ETDV_RUNROOT}/builds/'+config_name+'/'+build_name

   # FIXME Shouldn't need to generate a unique one of these per build. 
   build_check = \
      "@if [ -e "+build_dir+"/vbuild/simv ]; " \
      "then echo pass > $TARGET; " \
      "else echo fail > $TARGET; " \
      "fi"

   # Create a directory per build
   # FIXME if dvrun generates a build directory under build name then 
   # don't create vbuild
   full_build_cmd =  """
      build_dir="""+build_dir+"""
      cd $build_dir
      rm -rf vbuild
      mkdir vbuild
      cd vbuild 
      """ + build_cmd

   # Keep my own copy of all of the builds so that I can sort through them
   # FIXME Consider not duplicating book-keeping.  
   build = {'build_name': build_name,
            'build_cmd': build_cmd,
            'num_tests': 0};

   build_ptr = new_build(name=build_name, run_cmd=full_build_cmd, check_cmd=build_check)
   build['build_ptr'] = build_ptr;
   add_build(build_ptr)
   builds.append(build.copy())


def filter_builds_by_name(builds, names):
   matching_builds = builds
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

def filter_builds_by_cmd(builds, cmds):
   matching_builds = builds
   # If no name regular expression, return unfiltered list
   if cmds is None:
      return matching_builds
   if type(cmds) == type("a"):
      comp_expr = re.compile(names)
      matching_builds = [x for x in builds if re.search(comp_expr, x['build_cmd'])]
      return matching_builds
   if type(cmds) == type([]):
      for expr in cmds:
         comp_expr = re.compile(expr)
         matching_builds = [x for x in builds if re.search(comp_expr, x['build_cmd'])]
      return matching_builds

test_check = \
        "@pass=`fgrep -c 'TEST HAS PASSED' $LOG`; " \
                "if expr \$pass > 0; " \
        "then echo pass > $TARGET; " \
        "else echo fail > $TARGET; " \
        "fi"

def sc_add_test_to_builds(builds, test_name, test_cmd, ntimes=1, build_name_match=None, build_cmd_match=None):
   """
   Add test to builds that match the build name or the build_cmd
   build_name_match = regular expression for matching builds by name.
      Can be a list in which case, the test is added if the build
      matches all of the list entries.
   build_cmd_match = regular expression for matching builds by cmd.
      Can be a list in which case, the test is added if the build
      matches all of the list entries.
   If both name and command regular expressions are applied, then the test is added if both criteria are met.
   test_cmd = The test command to run against the build. 
   ntime = Add the test to the build ntimes.  Generally used with random tests, where each test has a 
      unique random seed.  To help with that, the string "REPLACE_WITH_RAND" if found in test_cmd will
      be replaced with an 8 digit random number.
   """
   matching_builds = builds

   matching_builds = filter_builds_by_name(matching_builds, build_name_match)
   matching_builds = filter_builds_by_cmd(matching_builds, build_cmd_match)

   for build in matching_builds:

      for ii in range(0, ntimes) :
         # uniquify the test_name to prevent them from stomping on each other
         full_test_name = str(build['num_tests'])+"_"+test_name
         print("Adding test " + full_test_name + " to build " + build['build_name'])

         # Add random seeding 
         rand_test_cmd = re.sub(r'REPLACE_WITH_RAND', get_rand(), test_cmd) 
         print(rand_test_cmd)

         # The build isn't local.  It's in a parallel directory structure.  Find it.  
         # FIXME, move this out of the loop once builds are relative. 
         test_dir='${ETDV_RUNROOT}/tests/'+config_name+'/'+build['build_name']+'/'+full_test_name
         build_dir = '${ETDV_BUILD_RUNDIR}/vbuild/'
         full_test_cmd = """ cd """+ test_dir + """
            """ + build_dir + rand_test_cmd

         test = new_test(name=full_test_name, run_cmd=full_test_cmd, check_cmd=test_check)
         build['build_ptr'].add_test(test)
         build['num_tests'] = build['num_tests'] + 1 


