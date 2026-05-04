#!/usr/bin/env python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import subprocess
import re
import json

class Disasm:
    def __init__(self, rvc, norvc):
        self.__re = re.compile(r"^\s*([0-9a-fA-F]+):\s+([0-9a-fA-F]+)\s+(.*)")
        self.__cache = {}
        for i, fname in enumerate((rvc, norvc)):
            subprocess.run(['riscv64-unknown-elf-as', fname], stdout=None, stderr=None, check=True)
            res = subprocess.run(['riscv64-unknown-elf-objdump', '-d','-z'], stdout=subprocess.PIPE, stderr=None, 
                                 check=True, universal_newlines = True).stdout.split("\n")
            exp_ad = 0
            for l in res:
                m = self.__re.match(l)
                if m:
                    ad = int(m.group(1), 16)
                    encoding = int(m.group(2), 16)
                    disasm = m.group(3)
                    
                    # same disasm for illegals
                    if disasm == "unimp" or disasm==f"{encoding:#x}":
                        disasm="illegal"
                    
                    if ad == exp_ad:
                        exp_ad = exp_ad + 2 + i*2
                        if encoding in self.__cache:
                            assert self.__cache[encoding] == disasm, f"multiple disasm for {encoding:x} => {disasm}  vs {self.__cache[encoding]}"
                        else:
                            self.__cache[encoding] = disasm
                    elif ad > exp_ad:
                        raise Exception(f"unexpected ad {ad:x} vs {exp_ad:x} in {fname}")
    def get(self, value):
        try:
            return self.__cache[value]
        except:
            raise Exception ("encoding {value:x} not found!")
        
rvc=[]
norvc=[]

#read table
with open('test.log','r') as f:
    for l in f:
        if l.startswith('TABLE:'):
            tab, x, y = l.split(" ", maxsplit=2)
            if tab == 'TABLE:':
                rvc.append(int(x,16))
                norvc.append(int(y,16))

for fname, data, word in [ ('rvc.S', rvc, '.half'),  ('norvc.S', norvc, '.word') ]:
    with open(fname,'w') as f:
        f.writelines([f'{word} {i:#x}\n' for i in data])

disasm = Disasm('rvc.S', 'norvc.S')

reserved ={}
for c, nc in zip(rvc, norvc):
    d_c = disasm.get(c)
    d_nc = disasm.get(nc)
    
    if d_c == "illegal" and d_nc != "illegal":
        reserved[f"{c:x}"] =  d_nc

with open('reserved.json', 'w') as f:
    json.dump(reserved, f)
