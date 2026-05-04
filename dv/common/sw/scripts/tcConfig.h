/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$
* @version $Release$
* @date $Date$
* @author 
*
* @brief tcMain.c Main function for all TCs
*
* Setup SoC to enable TC run
*/
/**
 *  @Component    Test Case Configuration
 *
 *  @Filename     tcConfig.h
 *
 *  @Description  Contains Test Case Specific configuration
 *                - Test Case Description
 *                - Test Case Functions lists
 *                - SOC Configuration for TC
 *                - Running CPU list
 *
 *//*======================================================================== */

#ifndef __TCCONFIG_H
#define __TCCONFIG_H


/*=============================================================================
 * Test Case Description
 *=============================================================================*/

/* 

   uart_echo test case
 
*/



#ifdef __cplusplus
extern "C"
{
#endif

/*=============================================================================
 * Verification Requirements Covered in TC
 *=============================================================================*/
uint32_t vreq_covered[] = 
{    
    VREQ_END_OF_LIST
};

/*=============================================================================
 * Test Case Functions list
 *=============================================================================*/

extern et_status_t uart_echo_TestFunction( void );



/* List of Running TC Functions */
const et_status_t ( *tcListFunctions[] )( void ) = 
{
    uart_echo_TestFunction,
    NULL   /* !!!  End of List delimiter DO NOT MODIFY !!! */
};


/* List of Running TC names */
const char *tcListNames[] =
{
    { "uart_echo access test" },
    NULL   /* !!!  End of List delimiter DO NOT MODIFY !!! */
};

 
/*=============================================================================
 *  List of Running CPU's for This Test Case
 *=============================================================================*/

const  uint32_t tcRunningCPUs[] =
{
    spMin,
    NULL   // !!!  End of List delimiter DO NOT MODIFY !!!
};
 

/*==============================================================================
 * SOC Configuration
 * =============================================================================*/

#ifdef 0
    SOC_CFG_t *pSocCfg = NULL;          // Use Default
//#else
    SOC_CFG_t *pSocCfg = &tcSocCfg;     // Use TC Specific
#endif

 /* 
  *    TBD.
  *  
 SOC_CFG_t tcSocCfg = 
 {
     // Create Specific one for this TC
 };
*/


#ifdef __cplusplus
}
#endif

#endif    /* __TCCONFIG_H */



/*      <EOF>     */
