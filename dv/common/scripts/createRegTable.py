#!/usr/bin/python
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import os
import argparse
import array
import argparse
import re
REPOROOT = os.getenv('REPOROOT')
RTLROOT = os.getenv('RTLROOT')
assert REPOROOT is not None, "setenv REPOROOT must be defined"
assert RTLROOT is not None, "setenv RTLROOT must be defined"
env = ['REPOROOT']

parser = argparse.ArgumentParser()
parser.add_argument('-file', type=argparse.FileType('r'), nargs='+')
parser.add_argument('-name', help = "name of created file")
args=parser.parse_args()

string_file = str(args.file)
new_name = str(args.name)
name_str_aux2 = string_file.split("/inc/")
name_str_aux = name_str_aux2[1].split(".h'")
name_str = name_str_aux[0]
substring = "DW"
substring = "DWC"
#print(name_str_aux2[1])
#print(name_str_aux[0])

#if substring in name_str:
#  ip_aux = name_str.split("b_")
#  print(ip_aux)
#  ip = str(ip_aux[1])
#  ip_name = "aux"
#  ip_name = str(ip)
#else:
#  ip_name = name_str



#effective=open(RTLROOT + "/mesh/noc_io_spio_2x5_2l_1spio_p1_1spio_sp_main_1spio_p0_v2/effective.log", "r")
f=open(REPOROOT + "/dv/common/sw/ip/inc/" + new_name + "_regTest.h","w+")


f.write("/*------------------------------------------------------------------------- \n\
*------------------------------------------------------------------------- \n\
*/\n\
\n\
/**\n\
* @file "+ new_name +"_regTest.h \n\
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
 *  @Filename       "+ new_name +"_regTest.h\n\
 *\n\
 *  @Description    IPs register table \n\
 *\n\
 *//*======================================================================== */\n\
\n\
#include \"" + name_str + ".h\"\n\
\n\
\n\
REGTEST_t "+ new_name +"Regs[] =\n\
\n\
{\n\
\n")

def str_to_write(wstr):
    data_print = wstr.split()
    ajust_data = '{0[0]:<3}{0[1]:<50}{0[2]:<30}{0[3]:<50}{0[4]:<50}{0[5]:<35}{0[6]:<5}'.format(data_print)
    f.write(ajust_data + "\n")
    return

i=0
j=0
reg_array=0
offset_size=0

for info in args.file:

  intro_string = "/* regAddress                               regSize              resetValue                                     bitMask                               regName */\n"

  str_to_write(intro_string)
  f.write("\n")
  searchString = ""
  line = info.readlines()
  for i in range (0, len(line)):
    checkword = line[i].split(" ")
    if len(checkword)>4:
      checkword_aux = checkword[1]+" "+checkword[2]+" "+checkword[3]
      if checkword_aux == "Register template referenced:":
        table_content = line[i+1].split(" ")
        offset = table_content[1]
        register_name_search = re.search("\:\:(.*?) \*\/", line[i])
        register_name_aux2 = register_name_search.group()
        register_name_aux = register_name_aux2.split("::")
        n = len(register_name_aux)
        register_name = register_name_aux[n-1].split(" ")
        reg_name = register_name[0]
        table_content = line[i+5].split(" ")
        reset_value = table_content[1]
        regsize_aux = len(table_content[2])
        if regsize_aux == 6:
          regsize= "REGTEST_SIZE_8_BIT"
          offset_size = 1
        elif regsize_aux == 8:
          regsize= "REGTEST_SIZE_16_BIT"
          offset_size = 2
        elif regsize_aux == 13:
          regsize= "REGTEST_SIZE_32_BIT"
          offset_size = 4
        else:
          regsize= "REGTEST_SIZE_64_BIT"
          offset_size = 8
        table_content = line[i+8].split(" ")
        rw_mask = table_content[1]
        for j in range (i, len(line)):
          check_struct = line[j]
          if((check_struct.find(reg_name))>0):
            m = check_struct.find(reg_name)
            found_string = re.search("\[(.*?)\]",check_struct)
            if(found_string):
              repet_number_aux = check_struct.split("[")
              repet_number_aux = repet_number_aux[1].split("];")
              repet_number = repet_number_aux[0]
              repet_number_int=int(repet_number, 16)
              #print("repet_number_int je = ",repet_number_int)
              reg_array = 1
        if(reg_array==1):
          for a in range(0, repet_number_int):
            struct_arr = "    { " + offset + "+" +str(offset_size*a)+ ", " + regsize + ", " + reset_value + ", " + rw_mask + ", " + "\"" + reg_name + "[" + str(a) + "]\", " + "},"
            str_to_write(struct_arr)
            f.write("\n")
        if(reg_array==0):
          struct_arr = "    { " + offset + ", " + regsize + ", " + reset_value + ", " + rw_mask + ", " + "\"" + reg_name + "\", " + "},"
          str_to_write(struct_arr)
          f.write("\n")
        reg_array=0
  outro_string = "{     REGTEST_END_OF_LIST,                  0,                   0,                                             0,                                   0,     }\n"

f.write("   /* End List Delimiter */\n\
\n")
str_to_write(outro_string)
f.write("\n\
};  \n\
\n")


