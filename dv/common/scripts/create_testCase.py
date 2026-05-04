#!/usr/bin/python
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

import os
import argparse
REPOROOT = os.getenv('REPOROOT')
assert REPOROOT is not None, "setenv REPOROOT must be defined"
env = ['REPOROOT']
parser = argparse.ArgumentParser()
parser.add_argument("--name", "-n", help="set test name")
parser.add_argument("--cpuList", "-c", help="set cpu", default="spMin", choices = ['spMin'])
parser.add_argument("--mainInTc", "-x", action="store_false", default=True)
args=parser.parse_args()
test_name=args.name
cpu_name=args.cpuList
mainInTc=args.mainInTc
os.mkdir(REPOROOT + '/dv/tests/ioshire/' + test_name)
os.mkdir(REPOROOT + '/dv/tests/ioshire/' + test_name + '/'+cpu_name)
os.mkdir(REPOROOT + '/dv/tests/ioshire/' + test_name + '/'+cpu_name+'/obj')
os.chdir(REPOROOT + '/dv/tests/ioshire/' + test_name + '/'+cpu_name+'/obj')
os.mknod(".gitignore")
os.chdir(REPOROOT + '/dv/tests/ioshire/' + test_name + '/'+cpu_name)
os.mknod("makefile")
os.mknod(test_name + ".c")
f=open("makefile","w")
f.write("include $(REPOROOT)/dv/tests/ioshire/scripts/spMin/makefile.defines\n \
\n\
src    := " +test_name +".c\n\
asmsrc :=\n\
\n")
f.close()
if (mainInTc != True):
    f=open("makefile", "a+")
    f.write("MAIN_IN_TC=1\n")
    f.close()
f=open("makefile", "a+")
f.write("\n\
include $(SW_COMMON_DIR)/scripts/makefile_tc\n")
f.close()
f=open(test_name+".c","wb")
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
* @brief "+test_name+".c Main function for all TCs \n\
*\n\
* Setup SoC to enable TC run \n\
*/ \n\
/** \n\
 *  @Component    Test Function Configuration \n\
 * \n\
 *  @Filename     "+test_name+".c \n\
 * \n\
 *  @Description  Contains Test Function\n\
 * \n\
 *//*======================================================================== */ \n\
\n\
\n\
\n\
/* =============================================================================\n\
 * STANDARD INCLUDE FILES\n\
 * =============================================================================\n\
 */ \n\
\n\
#include \"cpu.h\" \n\
#include \"soc.h\" \n\
#include \"macros.h\" \n\
#include \"ioshire_config.h\" \n\
\n\
#include \"tb.h\"\n\
//#include \"spy.h\"\n\
\n\
\n\
/* =============================================================================\n\
 * GLOBAL VARIABLES DECLARATIONS\n\
 * =============================================================================\n\
 */\n\
\n\
/* =============================================================================\n\
 * LOCAL TYPES AND DEFINITIONS\n\
 * =============================================================================\n\
 */\n\
\n\
/* =============================================================================\n\
 * LOCAL VARIABLES DECLARATIONS\n\
 * =============================================================================\n\
 */\n\
\n\
 /* =============================================================================\n\
 * LOCAL FUNCTIONS PROTOTYPES\n\
 * =============================================================================\n\
 *\n\
/* =============================================================================\n\
 * FUNCTIONS\n\
 * =============================================================================\n\
 */\n\
\n\
/*==================== Function Separator =============================*/\n\
\n")
f.close()
if (mainInTc):
    f=open(test_name+".c","a+")
    f.write("/* "+test_name+" Test Function */\n\
et_status_t " +test_name+"_TestFunction( void )\n\
{\n\
\n\
    et_status_t est = ET_FAIL;\n\
    return est;\n\
\n\
} /* "+test_name+"_Test() */\n\
\n\
/*****     < EOF >     *****/\n")
else:
    f=open(test_name+".c","a+")
    f.write("main()\n\
{\n\
    C_TEST_START;\n\
\n\
\n\
    C_TEST_FAIL;\n\
}\n\
/*****     < EOF >     *****/\n")
f.close()
os.chdir(REPOROOT + '/dv/tests/ioshire/' + test_name )
os.mknod("tcConfig.h")
f=open("tcConfig.h","w")
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
* @brief "+test_name+".c Main function for all TCs \n\
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
   " + test_name + " test case \n\
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
extern et_status_t "+test_name+"_TestFunction( void );\n\
\n\
\n\
\n\
/* List of Running TC Functions */\n\
const et_status_t ( *tcListFunctions[] )( void ) = \n\
{\n\
    "+test_name+"_TestFunction,\n\
    NULL   /* !!!  End of List delimiter DO NOT MODIFY !!! */\n\
};\n\
\n\
\n\
/* List of Running TC names */\n\
const char *tcListNames[] =\n\
{\n\
    { \""+test_name+" test\" },\n\
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
    "+cpu_name.upper()+",\n\
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





