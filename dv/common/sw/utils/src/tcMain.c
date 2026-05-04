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

#include "spio_misc_esr.h"
#include "et_cru_api.h"





/*=============================================================================
   GLOBAL VARIABLES DECLARATIONS
  =============================================================================*/


uint32_t thisCpu;
uint64_t runningCPUs;


/*=============================================================================
 * LOCAL TYPES AND DEFINITIONS
 *=============================================================================*/

#ifndef POSTSI_UST
#ifdef QUICK_BOOT
  #ifdef POSTSI_UST
    #undef QUICK_BOOT
  #else
    #ifndef NO_TB
      #define NO_TB
    #endif
  #endif
#endif
#endif

/*=============================================================================
 * LOCAL VARIABLES DECLARATIONS
 *=============================================================================*/


/*=============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *=============================================================================*/
void tc_finish( et_status_t est );
/*=============================================================================
 * FUNCTIONS
 *=============================================================================*/

#ifndef POSTSI_UST
#ifndef MAXION_SA
void quickBoot( void )
{

    et_status_t      est;

    Reset_Manager *pRM;
    Spio_misc_esr *miscEsr;

  #ifdef L1_CACHE_ON
    miscEsr = ( Spio_misc_esr *)R_SP_MISC_BASEADDR;
    miscEsr->SP_BYPASS_CACHE &= ~3;
    FENCE;
  #endif

    pRM = ( Reset_Manager *)R_SP_CRU_BASEADDR;

    pRM->rm_memshire_warm = 0xff;
    pRM->rm_memshire_cold = 0x1;

  #ifdef MAX_RESET_RELEASE
    pRM->rm_max_cold = 1;
    pRM->rm_max = 0x3;
    pRM->rm_max_warm = 0x01; // Maxion warm uncore reset

    soc_setupMaxionShireChannel();

    pRM->rm_max_warm = (MAX_RESET_RELEASE << 4) | 0x1;
  #endif

   #ifdef PSHIRE_RESET_RELEASE
    pRM->rm_pshire_cold = 1;
    pRM->rm_pshire_warm = 1;
  #endif

#ifndef SKIP_CONFIG_NOC_CLOCKS
  #ifdef CONFIG_NOC_CLOCKS
    soc_setup_noc_clocks(0);
  #else
    soc_setup_noc_clocks(1);
  #endif
#endif

  #ifdef MINION_RESET_RELEASE
    pRM->rm_minion = 3;   // Release cold reset
    pRM->rm_minion_warm_b = (MINION_RESET_RELEASE >> 32 & 0x3);
    pRM->rm_minion_warm_a = (MINION_RESET_RELEASE & 0x00FFFFFFFF);

    soc_setupShireChannels((uint64_t)MINION_RESET_RELEASE);
  #endif

  #ifdef RUN_SP_TEST
    /* Run only first Test Case */
    est = tcListFunctions[0]();
    tc_finish( est );
  #else
    #ifndef RELEASE_FC_RESETS
    C_TEST_PASS;
    WFI;
    #endif
  #endif

} /* quickBoot() */
#endif // MAXION_SA
#endif //POSTSI_UST


int main( void )
{
#ifdef ULTRASOC_BRINGUP
    WFI;
#else

#ifndef POSTSI_UST
    et_status_t      est;
    uint64_t         i;
#ifdef RUN_MAXION_TEST_ONLY
    Reset_Manager *pRM;
    pRM = (Reset_Manager *)R_SP_CRU_BASEADDR;
#endif

    uint32_t         tcId;
    uint32_t         errorCount;
    uint32_t         reg32;
    uint32_t         *addr;


    C_TEST_START;

  #ifdef QUICK_BOOT
  #ifndef MAXION_SA
    quickBoot();
  #endif
  #endif


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

  #ifndef NO_TB
    /* Initialize TB component */
    tb_init();
  #endif

    /* Default SoC Configuration */
    /*  i = 0; */

    /* Initialize Spy component */
    /*  spy_init(); */

    /* Initialize Msg component */
    /*  msg_init(); */

    /* reset test only */
    #ifndef MAXION_SA
    #ifdef PY_RESET
    soc_init();
    #endif
    #endif

    /* Initialize interrupt Handlers */

    inth_init();


    /*------------------------------------------------
     * Initialize SOC
     *------------------------------------------------*/

    if( thisCpu == MASTER_CPU ) {

      #ifndef NO_TB
        /* Check Runnig CPUs */
        i = 0;
        while( tcRunningCPUs[i] ) {
            runningCPUs |= 1 << tcRunningCPUs[i];
            i ++;
        }

        /* Report to TB list of running CPUs */
        tb_runningCPUs(runningCPUs );
        printDbg(" Running CPUs :  0x%X", runningCPUs );
      #endif

      #ifdef RUN_MAXION_TEST_ONLY
        /* Release all Maxion Reset */

        pRM->rm_max_cold = 1;
        pRM->rm_max = 0x3;
        pRM->rm_max_warm = 0x01; // Maxion warm uncore reset

        soc_setupMaxionShireChannel(); //Will not work in S mode!

        pRM->rm_max_warm = (MAX_RESET_RELEASE << 4) | 0x1;

        printDbg( "Maxion Run Only. Stop SP" );

        tc_finish( est );

      #else  /* RUN_MAXION_TEST_ONLY */

        #ifndef MAXION_SA
          #ifndef NO_TB
            /*------------------------------------------------
             * Perform SoC initialization
             *------------------------------------------------*/
            printDbg( "- Initialize SoC from CPU: %d", thisCpu );
            soc_init();

            #ifdef RELEASE_FC_RESETS
              printDbg( "- Releasing full-chip resets ....");
              quickBoot();
            #endif
          #endif
        #endif

      #endif  /* RUN_MAXION_TEST_ONLY */


    }
    else if (thisCpu == IOMAX) {
      #ifndef MAXION_SA
        soc_init();
      #endif
    }

    /*------------------------------------------------
     * Call Test Case Functions in the Loop
     *------------------------------------------------*/
    while( tcListFunctions[tcId] ) {

        printDbg( "\t\tStarting Test Case Id: %X", tcId );
        if( tcListNames[tcId] ) {
            printDbg( "\t\t\t%s", tcListNames[tcId] );
        }
        else {
            printDbg( "\t\t\tTC Name is missing" );
        }

        /* Inform TB that TC will start shortly */
        /*   tb_setTCStatus( TB_TC_RUNNING + tcId ); */

        /* Run Test Case */
        est = tcListFunctions[tcId]();

        /* Report TC status */
        printDbg( "\n" );
        if( est ) {
            errorCount ++;
            printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" );
            printDbg( "XXX  Test Case %04d                     F A I L E D  !!!!                      XXX", tcId );
            printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" );
        }
        else {
            printDbg( "Finished executing test case %04d on CPU: %d", tcId, CPU );
        }

        /* Increment Id for next test case */
        tcId ++;
    }


    /*------------------------------------------------
     * Check Spy Status
     *------------------------------------------------*/

    /*------------------------------------------------
     * Report Final Test Case Status
     *------------------------------------------------*/
    printDbg( "\t\tFinal result for CPU %d", CPU );
    if( errorCount ) {
        est = ET_FAIL;
        printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" );
        printDbg( "XXX                           !!!!  F A I L E D  !!!!                          XXX" );
        printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" );
    }
    else {
        est = ET_OK;
        printDbg( "**********************************************************************************" );
        printDbg( "***                                  P A S S                                   ***" );
        printDbg( "**********************************************************************************" );
    }

    /*------------------------------------------------
     * Complete TC run
     *------------------------------------------------*/
    tc_finish( est );


    /* This point is never reached */
    return( 0 );
#endif //POSTSI_UST ifndef
#endif //ULTRASOC_BRINGUP


#ifndef POSTSI_UST
#pragma message "POSTSI_UST IS UNDEFINED"
#else
#pragma message "POSTSI_UST IS DEFINED"
#ifndef SLT
#pragma message "SLT is undefined"
#else
#pragma message "SLT is defined"
#endif
#endif

//-- SILICON BRINGUP TESTS EXECUTOR --//
#ifdef POSTSI_UST
    et_status_t est;
    thisCpu     = CPU;
#if CPU==1 //this is service processor
    #include "serial.h"
    SERIAL_init(UART0);
    printDbg("Hello, SP is alive\r\n");
   /* Initialize cru_handle global variable */
    API_IP_PARAMS_t CRU_pIpParams;
    CRU_pIpParams.baseAddress = R_SP_CRU_BASEADDR;
    cru_handle =(ET_CRU_API_t *)et_cru_open(&CRU_pIpParams);
   /* END cru_handle initialization */
#if 0
    #include "pvt_controller.h"
    pvt_init();
    uint64_t delay=0xFFFFF;
    while (--delay) { asm volatile ("nop\n"); };

    pvt_print_all();
#endif

#endif
#if CPU==2
    #include "serial.h"
#ifndef SLT
    SERIAL_init(PU_UART1);
    printDbg("Hello, Maxion is alive!\r\n");
    soc_configure_maxion_pmp();
#endif
#endif
#if CPU==3
    #include "serial.h"
    SERIAL_init(PU_UART1);
    printDbg("Hello, Minion is alive!\r\n");
#endif
#ifndef SLT
    printDbg("Init interrupts");
#endif
    inth_init();
    if (tcListFunctions[0] != NULL) {
#ifndef SLT
        printDbg("C_TEST_START follows");
#endif
        C_TEST_START;
        est = tcListFunctions[0]();
    }
    else {
        printDbg("Empty test case, executing nothing :)\r\n");
    }
#if 0
#if CPU==1
    pvt_print_all();
#endif
#endif

    if (est == ET_PASS) {
#ifndef SLT
      printDbg("TEST PASS\r\n");
#else
#if CPU==2
      if (0 == CSRregRead( mhartid )) {
      printDbg("\n** SLT TEST PASS **\r\n");
      }
#else
      printDbg("\n** SLT TEST PASS **\r\n");
#endif
#endif
     C_TEST_PASS;
    }
    else {
#ifndef SLT
      printDbg("TEST FAIL\r\n");
#else
#if CPU==2
      if (0 ==CSRregRead( mhartid )) {
      printDbg("\n** SLT TEST FAIL **\r\n");
      }
#else
      printDbg("\n** SLT TEST FAIL **\r\n");
#endif
#endif
      C_TEST_FAIL;
    }



    //TEST_END_HERE

#endif //POSTSI_UST


}  /* main() */


/*=============================================================================
 * LOCAL FUNCTIONS
 *=============================================================================*/
void tc_finish( et_status_t est )
{

  #ifndef NO_TB
  #ifdef IOSHIRE_GATESIM
   __asm__ __volatile__ ( \
         "fence\n" \
         "li a7, 0x8100000000\n" \
         "sd a0, (a7)\n" \
         "1:wfi\n" \
         "j 1b\n" \
         : : : "a7");
  #endif
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
