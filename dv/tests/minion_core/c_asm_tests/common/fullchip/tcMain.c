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
//#include "spy.h"
//#include "msg.h"

#include "tcConfig.h"

#include "macros.h"




/*=============================================================================
   GLOBAL VARIABLES DECLARATIONS
  =============================================================================*/


uint32_t thisCpu;



/*=============================================================================
 * LOCAL TYPES AND DEFINITIONS
 *=============================================================================*/


/*=============================================================================
 * LOCAL VARIABLES DECLARATIONS
 *=============================================================================*/


/*=============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *=============================================================================*/


/*=============================================================================
 * FUNCTIONS
 *=============================================================================*/
int main( void )
{
    et_status_t      est;
    uint32_t         tcId;
    uint32_t         errorCount;
    uint32_t         reg32;
    uint32_t         *addr;
    uint64_t         i;
    uint64_t         runningCPUs;

    /* Initialize Variables */
    runningCPUs = 0;
    thisCpu     = CPU;

    est           = ET_OK;
    tcId          = 0;
    errorCount    = 0;

    /* TB Start marker */
    C_TEST_START;

    /*------------------------------------------------
     * Initialize TB infrastructure
     *------------------------------------------------*/

    /* Initialize TB component */
    //tb_init();

    /* Default SoC Configuration */
    /*  i = 0; */

    /* Initialize Spy component */
    /*  spy_init(); */

    /* Initialize Msg component */
    /*  msg_init(); */


    /*------------------------------------------------
     * Initialize SOC
     *------------------------------------------------*/


    //if( thisCpu == MASTER_CPU ) {
        /*------------------------------------------------
         * Perform SoC initialization
         *------------------------------------------------*/
      //  printDbg( "- Initialize SoC from CPU: %d", thisCpu );
      //  soc_init();

        /* Check Runnig CPUs */
       // i = 0;
       // while( tcRunningCPUs[i] ) {
       //     runningCPUs |= 1 << tcRunningCPUs[i];
       //     i ++;
      //  }

     /* Report to TB list of running CPUs */
        //tb_runningCPUs(runningCPUs );

    //}


    /*------------------------------------------------
     * Call Test Case Functions in the Loop
     *------------------------------------------------*/
    while( tcListFunctions[tcId] ) {

        /*printDbg( "\t\tStarting Test Case Id: %X", tcId );
        if( tcListNames[tcId] ) {
            printDbg( "\t\t\t%s", tcListNames[tcId] );
        }
        else {
            printDbg( "\t\t\tTC Name is missing" );
        }*/

        /* Inform TB that TC will start shortly */
        /*   tb_setTCStatus( TB_TC_RUNNING + tcId ); */

        /* Run Test Case */
        est = tcListFunctions[tcId]();

        /* Report TC status */
       // printDbg( "\n" );
        if( est ) {
            errorCount ++;
        //    printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" );
        //    printDbg( "XXX  Test Case %04d                     F A I L E D  !!!!                      XXX", tcId );
        //    printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" );
        }
        /*else {
            printDbg( "Finished executing test case %04d on CPU: %d", tcId, CPU );
        }*/

        /* Increment Id for next test case */
        tcId ++;
    }


    /*------------------------------------------------
     * Check Spy Status
     *------------------------------------------------*/

    /*------------------------------------------------
     * Report Final Test Case Status
     *------------------------------------------------*/
    //printDbg( "\t\tFinal result for CPU %d", CPU );
    if( errorCount ) {
        /*est = ET_FAIL;
        printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" );
        printDbg( "XXX                           !!!!  F A I L E D  !!!!                          XXX" );
        printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" );*/
        C_TEST_FAIL; 
    }
    else {
        /*est = ET_OK;
        printDbg( "**********************************************************************************" );
        printDbg( "***                                  P A S S                                   ***" );
        printDbg( "**********************************************************************************" );*/
        C_TEST_PASS; 
    }

    /*------------------------------------------------
     * Complete TC run
     *------------------------------------------------*/
    //tb_finish( est );	


    /* This point is never reached */
    return( 0 );

}  /* main() */



/*=============================================================================
 * LOCAL FUNCTIONS
 *=============================================================================*/


/*****     <EOF>     *****/
