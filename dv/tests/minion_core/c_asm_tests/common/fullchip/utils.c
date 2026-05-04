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


/*=============================================================================
 * STANDARD INCLUDE FILES
 *=============================================================================*/

#include "cpu.h"
#include "soc.h"

#include "tb.h"

#include "macros.h"

/*=============================================================================
 * LOCAL VARIABLES DECLARATIONS
 *=============================================================================*/


/*=============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *=============================================================================*/

void tc_finish( et_status_t est );

/*=============================================================================
 * LOCAL FUNCTIONS
 *=============================================================================*/

void tc_finish( et_status_t est )
{

  #ifndef NO_TB
    tb_finish( est );
  #else
    if( est ){
        C_TEST_FAIL;
    }
    else{
        C_TEST_PASS;
    }
    WFI;
  #endif

} /* tc_finish() */


/*****     <EOF>     *****/
