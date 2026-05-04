#!/usr/bin/python
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import os
import argparse
from shutil import copyfile
REPOROOT = os.getenv('REPOROOT')
assert REPOROOT is not None, "setenv REPOROOT must be defined"
env = ['REPOROOT']
parser = argparse.ArgumentParser()
parser.add_argument("--ipName", "-n", help="set test name")
parser.add_argument("--cpuList", "-c", help="set cpu", default="ioMax", choices = ['ioMax'])
parser.add_argument("--ipBaseAddress", "-b", help="set base address")
parser.add_argument("--regTable", "-t", help="register table")
parser.add_argument("--incFile", "-i", help="Name of file with regTable")
args=parser.parse_args()
ip_name=args.ipName
cpu_name=args.cpuList
ip_baseaddr=args.ipBaseAddress
reg_table=args.regTable
inc_file=args.incFile
os.mkdir(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register')
os.mkdir(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register/'+cpu_name)
os.mkdir(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register/'+cpu_name+'/obj')
os.chdir(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register/'+cpu_name+'/obj')
os.mknod(".gitignore")
os.mkdir(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register/spMin/')
os.mkdir(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register/spMin/obj')
os.chdir(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register/spMin/')

os.mknod("makefile")
############### MAKEFILE FOR SPMIN ###############
f=open("makefile","w")
f.write("include $(REPOROOT)/dv/tests/ioshire/scripts/spMin/makefile.defines\n\
\n\
src    := tt.c\n\
asmsrc :=\n\
\n\
include $(SW_COMMON_DIR)/scripts/makefile_tc\n")
f.close()


os.mknod("tt.c")
###############   TT.C   ###############
f=open("tt.c","w")
f.write("#include \"cpu.h\"\n\
#include \"soc.h\"\n\
\n\
#include \"tb.h\"\n\
//#include \"spy.h\"\n\
#include \"macros.h\"\n\
\n\
/* TT Test Function */\n\
et_status_t tt_TestFunction( void )\n\
{\n\
\n\
    soc_releaseCpuReset(IOMAX0);\n\
    FENCE;\n\
    printDbg(\"\\t\\tMaxion has started ...\" );\n\
    FENCE;\n\
\n\
    return ET_OK;\n\
\n\
} /* tt_Test() */\n\
\n\
/*****     < EOF >     *****/\n")
f.close()

os.chdir(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register/spMin/obj')
os.mknod(".gitignore")
os.chdir(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register/'+cpu_name)
os.mknod("makefile")


############### MAKEFILE ###############
f=open("makefile","w")
f.write("include $(REPOROOT)/dv/tests/ioshire/scripts/ioMax/makefile.defines\n \
\n\
src    := $(REPOROOT)/dv/common/sw/utils/src/tcRegtest.c\n\
asmsrc :=\n\
\n\
include $(SW_COMMON_DIR)/scripts/makefile_tc\n")
f.close()

############### tcRegtest.h ###############
copyfile(REPOROOT + '/dv/common/sw/utils/inc/tcRegtest_template.h', REPOROOT + '/dv/tests/ioshire/max_' + ip_name+ '_register/' + cpu_name + '/tcRegtest.h')
with open(REPOROOT + '/dv/tests/ioshire/max_' + ip_name+ '_register/' + cpu_name + '/tcRegtest.h', 'r') as file :
  filedata = file.read()

filedata = filedata.replace("<put_include_file_name_here>", inc_file)
filedata = filedata.replace("<put_tc_name_here>", ip_name + '_register')
filedata = filedata.replace("<put_IP_base_address_here>", ip_baseaddr)
filedata = filedata.replace("<put_regtest_table_here>", reg_table)

with open(REPOROOT + '/dv/tests/ioshire/max_' + ip_name+ '_register/' + cpu_name + '/tcRegtest.h', 'w') as file :
  file.write(filedata)

############### tcConfig.h ###############
f=open(REPOROOT + '/dv/tests/ioshire/max_' + ip_name + '_register/tcConfig.h',"w")
f.write("/*------------------------------------------------------------------------- \n\
*------------------------------------------------------------------------- \n\
*/\n\
\n\
/**\n\
* @file $Id$ \n\
* @version $Release$ \n\
* @date $Date$ \n\
* @author \n\
*\n\
* @brief "+ip_name+".c Main function for all TCs \n\
*\n\
* Setup SoC to enable TC run \n\
*/ \n\
/** \n\
 *  @Component    Test Case Configuration \n\
 * \n\
 *  @Filename     tcConfig.h \n\
 * \n\
 *  @Description  Contains Test Case Specific configuration\n\
 *                - Test Case Description \n\
 *                - Test Case Functions lists \n\
 *                - SOC Configuration for TC \n\
 *                - Running CPU list \n\
 * \n\
 *//*======================================================================== */ \n\
\n\
#ifndef __TCCONFIG_H \n\
#define __TCCONFIG_H \n\
\n\
\n\
/*============================================================================= \n\
 * Test Case Description \n\
 *=============================================================================*/ \n\
\n\
/* \n\
\n\
   " + ip_name + "_register test case \n\
 \n\
*/\n\
\n\
\n\
\n\
#ifdef __cplusplus \n\
extern \"C\" \n\
{ \n\
#endif \n\
\n\
/*============================================================================= \n\
 * Verification Requirements Covered in TC \n\
 *=============================================================================*/ \n\
#if 0\n\
uint32_t vreq_covered[] =\n\
{\n\
    VREQ_END_OF_LIST\n\
};\n\
#endif\n\
\n\
/*=============================================================================\n\
 * Test Case Functions list\n\
 *=============================================================================*/\n\
\n\
extern et_status_t " + ip_name +"_register_TestFunction( void );\n\
extern et_status_t tt_TestFunction( void );\n\
\n\
\n\
\n\
/* List of Running TC Functions */\n\
const et_status_t ( *tcListFunctions[] )( void ) =\n\
{\n\
// ioMax is CPU 2\n\
#if CPU==2\n\
    "+ ip_name+"_register_TestFunction,\n\
// spMin is CPU 1\n\
#elif CPU==1\n\
    tt_TestFunction,\n\
#else\n\
    #error \"CPU is not defined!\"\n\
#endif\n\
    NULL   /* !!!  End of List delimiter DO NOT MODIFY !!! */\n\
};\n\
\n\
\n\
/* List of Running TC names */\n\
const char *tcListNames[] =\n\
{\n\
// ioMax is CPU 2\n\
#if CPU==2\n\
    { \"Maxion "+ip_name+"_register test\" },\n\
// spMin is CPU 1\n\
#elif CPU==1\n\
    { \"TT test\" },\n\
#else\n\
    #error \"CPU is not defined!\"\n\
#endif\n\
    NULL   /* !!!  End of List delimiter DO NOT MODIFY !!! */\n\
};\n\
\n\
\n\
/*=============================================================================\n\
 *  List of Running CPU's for This Test Case\n\
 *=============================================================================*/\n\
\n\
const  uint32_t tcRunningCPUs[] =\n\
{\n\
    SPMIN,\n\
    IOMAX,\n\
    NULL   // !!!  End of List delimiter DO NOT MODIFY !!!\n\
};\n\
\n\
\n\
/*==============================================================================\n\
 * SOC Configuration\n\
 * =============================================================================*/\n\
\n\
#if 0\n\
    SOC_CFG_t *pSocCfg = NULL;          // Use Default\n\
//#else\n\
    SOC_CFG_t *pSocCfg = &tcSocCfg;     // Use TC Specific\n\
#endif\n\
\n\
 /*\n\
  *    \n\
  *\n\
 SOC_CFG_t tcSocCfg = \n\
 {\n\
     // Create Specific one for this TC\n\
 };\n\
*/\n\
\n\
\n\
#ifdef __cplusplus\n\
}\n\
#endif\n\
\n\
#endif    /* __TCCONFIG_H */\n\
\n\
\n\
\n\
/*      <EOF>     */\n")
f.close()

