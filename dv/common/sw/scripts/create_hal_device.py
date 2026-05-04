#!/usr/bin/python
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import os
import argparse
import array
import argparse
import re
import sys
REPOROOT = os.getenv('REPOROOT')
RTLROOT = os.getenv('RTLROOT')
assert REPOROOT is not None, "setenv REPOROOT must be defined"
assert RTLROOT is not None, "setenv RTLROOT must be defined"
env = ['REPOROOT']

parser = argparse.ArgumentParser()
parser.add_argument('-file', type=argparse.FileType('r'), nargs='+')
args=parser.parse_args()
#effective=open(RTLROOT + "/mesh/noc_io_spio_2x5_2l_1spio_p1_1spio_sp_main_1spio_p0_v2/effective.log", "r")
f=open("hal_device.h","w+")

f.write("/*------------------------------------------------------------------------- \n\
*------------------------------------------------------------------------- \n\
*/\n\
\n\
/**\n\
* @file create_hal_device.py \n\
* @version $Release$ \n\
* @date $Date$\n\
* @author \n\
*\n\
* @brief \n\
*\n\
* Setup SoC to enable TC run \n\
*/ \n\
/** \n\
 *  @Component      HAL\n\
 *\n\
 *  @Filename       hal_device.h\n\
 *\n\
 *  @Description    The HAL component defines SoC memory map\n\
 *\n\
 *//*======================================================================== */\n\
\n\
\n\
\n\
\n\
#ifndef __HAL_DEVICE_H\n\
#define __HAL_DEVICE_H\n\
\n\
#ifdef __cplusplus\n\
extern\n\
{\n\
#endif\n\
\n\
\n\
/* =============================================================================\n\
 * PROJECT SPECIFIC INCLUDES\n\
 * =============================================================================\n\
 */ \n\
#include \"spio_plic_intr_device.h\"\n\
#include \"pu_plic_intr_device.h\"\n\
#include \"sp_exceptions.h\"\n\
#include \"local_interrupts.h\"\n\
\n\
/* =============================================================================\n\
 * EXPORTED DEFINITIONS\n\
 * =============================================================================\n\
 */ \n\
\n\
/* Interrupt Request map */\n\
\n\
\n\
#define INTH_NUMBER_OF_INT        (SPIO_PLIC_INTR_SRC_CNT + PU_PLIC_INTR_SRC_CNT - 2)\n\
#define PU_PLIC_IRQ_OFFSET        SPIO_PLIC_INTR_SRC_CNT\n\
\n\
/* DMA Request map*/\n\
\n\
\n\
/* Address map */\n")

def str_to_write(wstr):
    data_print = wstr.split()
    ajust_data = '{0[0]:<8}{0[1]:<50}{0[2]:<12}'.format(data_print)
    f.write(ajust_data + "\n")
    return


sys.path.append(RTLROOT + '/ip/netspeed/scripts')
import ioshire_pshire_spec_vlatest

for first_key, rest_data in ioshire_pshire_spec_vlatest.memory_maps_all.items():
  first_word = first_key.replace( '-' , '_')
  prefix = ''
  size_addr_sum = 0
  base_addr_basic = 0
  base_addr = 0
  i=0
  j=0
    
  for second_key, rest_data_aux in rest_data.items():
    #print(second_key)
    if(second_key == 'Base'):
      #print('second_key', second_key)
      base_addr = hex(rest_data_aux)
      #print("base_addr = ", base_addr)
      i=1

    if(second_key == 'End'):
      end_addr = hex(rest_data_aux)
      #print("end_addr = ", end_addr)
      j=1

    if( i & j ):
      base_addr_int=int(base_addr, 16)
      end_addr_int=int(end_addr, 16)
      size_addr = end_addr_int - base_addr_int
      #print("size_addr = ", size_addr)
      
      hex_size_addr = "{0:#0{1}x}".format(size_addr,12)
      hex_base_addr = "{0:#0{1}x}".format(base_addr_int,12)
      print_base_addr = "#define " + first_word.upper() + "_" + "BASEADDR " + hex_base_addr + "\n"
      print_size = "#define " + first_word.upper() + "_" + "SIZE " + hex_size_addr + " \n"
      str_to_write(print_base_addr)
      str_to_write(print_size)
      f.write("\n\n")
      i=0
      j=0


f.write("#ifdef __cplusplus\n\
}\n\
#endif\n\
\n\
#endif  /* __HAL_DEVICE_H */\n\
\n\
\n\
/*****     <EOF>     *****/\n")

