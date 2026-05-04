#!/usr/bin/python
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import random
import sys
import argparse
import os.path
from os import path

def main():

  my_parser = argparse.ArgumentParser(description="tcParam randomizer")
  my_parser.add_argument('--path', action='store', type=str, help='tcParam path', required=True)
  my_parser.add_argument('--seed', action='store', type=int, help='randomization seed', default=0)
  args = my_parser.parse_args()


  file_paths = ["/spMin/tcParam.h", "/ioMax/tcParam.h", "/msMin/tcParam.h"]

  seed = args.seed
  print (seed)
  random.seed(seed)

  for _path in file_paths:

      file_path = args.path + _path

      print (file_path)

      cnt = 0;
      value = []
      defaultValue = []

      if path.exists(file_path):
        print ("File exist: "+file_path)
        f= open(file_path, "r+")

        f1 = f.readlines()
        for line in f1:
          if line.strip():
            info = line.split(" ")
            if info[0] == '@RP':
              cnt = cnt + 1;
              if info[1] == 'KEY':
                value.append(int(0))
                defaultValue.append(int(info[2]) if info[2].find('x') == -1 else int(info[2], 16))
              elif info[1] == 'VAL':
                arr = info[2].split(",")
                index = random.randint(0,len(arr)-1)
                value.append(int(arr[index]) if arr[index].find('x') == -1 else int(arr[index], 16))
                defaultValue.append(int(info[3]) if info[3].find('x') == -1 else int(info[3], 16))
              elif info[1] == 'FIX':
                value.append(int(info[2]) if info[2].find('x') == -1 else int(info[2], 16))
                defaultValue.append(int(info[2]) if info[2].find('x') == -1 else int(info[2], 16))
              elif info[1] == 'RNG':
                range_start = int(info[2]) if info[2].find('x') == -1 else int(info[2], 16)
                range_end = int(info[3]) if info[3].find('x') == -1 else int(info[3], 16)
                value.append(random.randint(range_start,range_end))
                defaultValue.append(int(info[4]) if info[4].find('x') == -1 else int(info[4], 16))
              else:
                f.write("\n//Python parsing error!!! line: %s" % line)
                f.close()
                return 0

        f.write("\n\ntcParam_t tc_param_random = {")
        for i in range(0, len(value)):
            if i == (len(value)-1):
                f.write("%s};" % value[i])
            else:
                f.write("%s, " % value[i])

        f.write("\ntcParam_t tc_param_default = {")
        for i in range(0, len(defaultValue)):
          if i == (len(defaultValue)-1):
            f.write("%s};" % defaultValue[i])
          else:
            f.write("%s, " % defaultValue[i])

        f.write("\n\nextern void* tc_param_random_ptr = &tc_param_random;")
        f.write("\nextern void* tc_param_default_ptr = &tc_param_default;")

        f.close()

      else:
        print ("File not exist: "+file_path)

      print (value)
      print (defaultValue)

  print ("Adding seed to tcConfig.h")
  file_path = args.path + "/tcConfig.h"
  f= open(file_path, "r+")
  fl=f.readlines();
  f.seek(0)
  seed_added_flag=False
  for line in fl:
      if not seed_added_flag and ("extern et_status_t" in line):
          f.write("\nextern uint64_t tc_param_seed = %d;" % seed)
          seed_mod = seed % 2147483648;
          f.write("\n//extern int tc_param_seed = %d;\n\n" % seed_mod)
          seed_added_flag=True
      f.write(line)
  f.close()

  return 0

if __name__ == "__main__":
  main()
  sys.exit(0)
#  rand(sys.argv[1],sys.argv[2])
