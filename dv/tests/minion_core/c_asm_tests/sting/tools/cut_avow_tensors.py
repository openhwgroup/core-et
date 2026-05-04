#!/usr/bin/python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import re
from enum import Enum

class PARSER_STATE(Enum):
   FIND_FETCH_PC = 1
   FIND_TENSOR_INSNS = 2
   PROCESS_TENSOR_INSNS = 3
   READ_NEW_LINE = 4
   DONE = 5

pc  = 0x8100000000
cyc = 0

full_sysemu = open('sysemu.log', 'r') 
cut_sysemu = open('avow.sysemu.log', 'w')

def is_fetch_pc(line):
   if ("Fetched" in line):
      return True
   else:
      return False

tensor_pattern = re.compile("csr.+tensor")
def is_tensor(line):
   global tensor_pattern
   if (bool(re.search(tensor_pattern, line))):
      return True
   else:
      return False

cyc_pattern = re.compile("^.+: DEBUG")
hart_pattern = re.compile("\[H.+T.\]")
def format_avow(line):
   global cyc
   global cyc_pattern
   global hart_pattern
   line = re.sub(cyc_pattern, str(cyc) + ": DEBUG", line)
   line = re.sub(hart_pattern, "[H0 S0:N0:C0:T0]", line, count=1)
   return line

fetch_pattern = re.compile("PC.+:")
def format_avow_pc(line):
   global pc
   global fetch_pattern
   line = re.sub(fetch_pattern, "PC "+str(hex(pc))+":", line)
   return format_avow(line)



state = PARSER_STATE.FIND_FETCH_PC

fetch = ""

for line in full_sysemu:

   if (state == PARSER_STATE.FIND_FETCH_PC):
      if (is_fetch_pc(line)):
         state = PARSER_STATE.FIND_TENSOR_INSNS
         fetch = line
 
   elif (state == PARSER_STATE.FIND_TENSOR_INSNS):
      if (is_tensor(line)):
         state = PARSER_STATE.PROCESS_TENSOR_INSNS
         cut_sysemu.write(format_avow_pc(fetch))
         cut_sysemu.write(format_avow(line))
      else:
         state = PARSER_STATE.FIND_FETCH_PC

   elif (state == PARSER_STATE.PROCESS_TENSOR_INSNS):
      if (is_fetch_pc(line)):
         state = PARSER_STATE.FIND_TENSOR_INSNS
         fetch = line
         pc = pc + 4
         cyc = cyc + 1
      else:
         cut_sysemu.write(format_avow(line))

full_sysemu.close()
cut_sysemu.close()
