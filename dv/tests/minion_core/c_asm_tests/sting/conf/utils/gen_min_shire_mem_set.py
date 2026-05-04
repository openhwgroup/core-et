#!/usr/bin/python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import sys

def print_mem_set(shire_mask):
    print ("# This is an auto-generated file")
    print ("")
    print ("SNIPPET_MEM_SETS = SNIPPET_MEM_SETS + {")
    print ("    ET_MINION_SHIRE_MEM_SET : [")
    print ("        { SM : ET_DRAM_REGION         WEIGHT : 34 },")
    print ("    ]")
    print ("}")

    print ("if (ET_L2SCP.L2SCP_IN_MEM_SET == 1):")
    print ("    if (ET_MEM_ALIAS.L2SCP == 1) :")
    print ("        SNIPPET_MEM_SETS.ET_MINION_SHIRE_MEM_SET = SNIPPET_MEM_SETS.ET_MINION_SHIRE_MEM_SET + [")
    print ("            { SM : ET_LOCAL_SCP_REGION    WEIGHT : 17 },")
    print ("        ]")
    print ("    else:")
    print ("        SNIPPET_MEM_SETS.ET_MINION_SHIRE_MEM_SET = SNIPPET_MEM_SETS.ET_MINION_SHIRE_MEM_SET + [")
    num_shires = int(bin(shire_mask)[2:].count('1'))
    weight = int(34 / num_shires)
    i = 0
    while (shire_mask != 0):
       if ((shire_mask & 1) == 1):
          print ('            { SM : ET_SCP_SHIRE'+str(i)+'_REGION   WEIGHT : '+str(weight)+' },')
       i = i + 1
       shire_mask = shire_mask >> 1
    print ("        ]")
    print ("    endif")
    print ("endif")


shire_mask = int(sys.argv[1], 0)
print_mem_set(shire_mask)
