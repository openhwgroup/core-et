#!/usr/bin/env python3

import random
import os

NUM_TESTS = 10
NUM_COMPS = 1024

seeds = [random.getrandbits(64) for _ in range(NUM_TESTS)]


for idx, seed in enumerate(seeds):
   elf_dir = f"screen_{idx}"
   cmd = f"rm -rf {elf_dir}; ../../mtg.py -thr 0x3 -min 0xffffffff -shi 0x3ffffffff -c rand.py -s {seed} -ncomps {NUM_COMPS} -ps -sa {elf_dir} >> {idx}.log 2>&1"
   os.system(f'echo "MTG GIT commit hash:" > {idx}.log')
   os.system(f"git rev-parse --verify HEAD >> {idx}.log")
   os.system(f'echo "{cmd}" >> {idx}.log')
   os.system(cmd)
   os.system(f"mv base 2176t_screen_{idx}.elf")
