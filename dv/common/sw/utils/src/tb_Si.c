/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$
* @version $Release$
* @date $Date$
* @author
*
* @brief tb.c Test Bench interface
*
* SW for various SW <-> SV synchronization
*/



/* =============================================================================
 * STANDARD INCLUDE FILES
 * =============================================================================
 */
#include "cpu.h"
#include "soc.h"

#include <stdarg.h>

#include "tb.h"

#include "macros.h"


//#include "api.h"
//#include "spy.h"


/* =============================================================================
 * GLOBAL VARIABLES DECLARATIONS
 * =============================================================================
 */

TB_t *pTB;



/* =============================================================================
 * LOCAL TYPES AND DEFINITIONS
 * =============================================================================
 */



/* =============================================================================
 * LOCAL VARIABLES DECLARATIONS
 * =============================================================================
 */

uint64_t *pTb_Id;
uint64_t *pTb_Param;

uint64_t noPrintf;

uint64_t *printArgs;



/* =============================================================================
 * LOCAL FUNCTIONS DECLARATIONS
 * =============================================================================
 */

void tb_write( uint64_t Id, uint64_t Param );



/* =============================================================================
 * EXTERNAL VARIABLES DECLARATIONS
 * =============================================================================
 */

// tb Area

/* From Scatter File  */

extern uint64_t __tb_Id;
extern uint64_t __tb_Param;
extern uint64_t __tb_base_address;
//extern uint64_t __msg_base_address;
extern uint64_t __spy_base_address;
extern uint64_t __tcParam_base_address;
extern uint64_t __tb_end_address;
//extern uint64_t __msg_end_address;
extern uint64_t __spy_end_address;
//extern uint64_t __tcParam_end_address;
extern uint64_t __soc_end_address;


// From Soc.h
extern uint32_t thisCpu;


 /* =============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 * =============================================================================
 */



/* =============================================================================
 * FUNCTIONS
 * =============================================================================
 */



/*==================== Function Separator =============================*/
void tb_init(void)
{

#if 0
    et_status_t est;
    uint32_t err;
    uint64_t addr;

    /* Set Global Pointers */
    pTB = (TB_t *)&__tb_base_address;
    /* Init tbId/Param  pointers */
    pTb_Id    = &__tb_Id;
//    pTb_Param = &__tb_Param;

    pTb_Id[0]    = 0;
    pTb_Id[1]    = 0;
    pTb_Id[2]    = 0xCCCC;


    /* Set CPU Id */
    pTB->cpuId = CPU;

    /* Configure tb from scatter file */
/*
    pTB->msgQueueAddress = (uint64_t)&__msg_base_address;
    pTB->msgQueueSize    = (uint64_t)(&__msg_end_address) - (uint64_t)(&__msg_base_address);
*/
    pTB->msgQueueAddress = 0;
    pTB->msgQueueSize    = 0;

    pTB->spyAddress      = (uint64_t)&__spy_base_address;
    pTB->spySize         = (uint64_t)(&__spy_end_address) - (uint64_t)(&__spy_base_address);

    pTB->printfAddress   = 0;
    pTB->printfSize      = 0;
    pTB->printModeSelect = 0;   //FUTURE: add Enums. UVM printf

    /* Set TC param area */
    pTB->tcParamAddress  = ( void *)&__tcParam_base_address;
    pTB->tcParamSelect   = TB_TC_PARAM_RANDOM;

    addr = (uint64_t)&__soc_end_address;
    addr += 0x10;
    addr &= ~0xF;

    printArgs = (uint64_t *)addr;

    /* Set TC status as started */
    pTB->tcStatus = TB_TC_STARTED;
    __asm__ __volatile__ ("fence\n");

    /* Set master TC status as started */
    pTB->masterTcStatus = TB_TC_STARTED;
    __asm__ __volatile__ ("fence\n");

    noPrintf  = 0;

    printDbg( "Completed tb_init()\n" );
#endif

} /* tb_init() */



/*==================== Function Separator =============================*/
void tb_finish( uint32_t status )
{

#if 0
    uint32_t i;
    uint32_t error;
    TB_t *pSlaveTB;

    pTB->tcStatus = TB_TC_FINISH | ( status & 0x0000FFFF );
    __asm__ __volatile__ ("fence\n");

    error = ( status & 0xFFFF ) ? 1 : 0;

    /* All Slaves Completed, Stop Simulation */
    i = ( error ) ? TB_TC_FAIL : TB_TC_PASS;
    pTB->masterTcStatus = TB_TC_FINISH | i;


   /* Send Error status to stop Simulation */
   tb_write( TB_STOP , status & 0xFFFF );

   /* Stop BEMU */
   if( error ) {
      C_TEST_FAIL;
   }
   else {
      C_TEST_PASS;
   }


    __asm__ __volatile__ ("fence\n");

    while( 1 ) {
        WFI;
        printDbg( "Unexpected Wakeup while TC is finished !!!\n" );
    }
#endif

} /* tb_finish() */



/*==================== Function Separator =============================*/
void tb_fatalError( uint32_t error )
{

#if 0
    TB_t *pMasterTB;

    pTB->tcStatus = TB_TC_FATAL_ERROR | ( error & 0x0000FFFF );
    __asm__ __volatile__ ("fence\n");

    /* At least one CPU detected Fatal Error */
    /* So stop simulation immediatelly */
    /* FUTURE: Use some msg  or irq to master CPU */

        while( 1 ) {
//        __WFI();
//        printDbg( "Unexpected Wakeup while TC has Fatal Error !!!\n" );
    }
#endif

} /* tb_fatalError() */



/*==================== Function Separator =============================*/
void tb_setTCStatus( TB_TC_STATUS_t status )
{

#if 0
    pTB->tcStatus = status;
    __asm__ __volatile__ ("fence\n");
#endif

} /* tb_setTCStatus() */


/*==================== Function Separator =============================*/
int tb_printf(char *format, ...)
{

#if 0
    va_list a_list;

    uint64_t data_ptr_bckp;

    printArgs[0] = (uint64_t)format;
    va_start( a_list, format);

    for( int i = 1; i < 8; i++ ) {
         printArgs[i] = va_arg( a_list, uint64_t );
    }

    tb_write( TB_PRINT, printArgs );

    va_end( a_list );
#endif

    return 0;

} /* tb_printf() */



/*==================== Function Separator =============================*/
int64_t tb_getRandom( char *format, ... )
{

#if 0
    va_list a_list;

    uint64_t data_ptr_bckp;

    printArgs[0] = (uint64_t)format;
    va_start( a_list, format);

    for( int i = 1; i < 16; i++ ) {
         printArgs[i] = va_arg( a_list, uint64_t );
    }

    tb_write( TB_RANDOM, printArgs );

    va_end( a_list );

    return printArgs[0];
#endif
    return 0;

} /* tb_getRandom() */



/*==================== Function Separator =============================*/
void *tb_getTcParamPtr( uint64_t tcParamType )
{

#if 0
    void  *tcParamPtr;


    switch( tcParamType ) {
        case  TB_TC_PARAM_DEFAULT :
            tcParamPtr = pTB->tcParamAddress;
            tb_write( ((uint64_t)thisCpu << 32 ) + TB_TC_PARAM_DEFAULT, tcParamPtr );
            break;
        case  TB_TC_PARAM_RANDOM :
            tcParamPtr = pTB->tcParamAddress;
            tb_write( ((uint64_t)thisCpu << 32 ) + TB_TC_PARAM_RANDOM, tcParamPtr );
            break;
        default:
            tcParamPtr = NULL;
            break;
    }

    return tcParamPtr;
#endif
    return 0;

} /* tb_getTcParamPtr() */



/*==================== Function Separator =============================*/
void tb_runningCPUs( uint64_t runningCPUs )
{

#if 0
    tb_write( TB_RUNNING_CPUS, runningCPUs );
#endif

} /* tb_runningCPUs() */



/*==================== Function Separator =============================*/
void tb_spy( uint64_t spyVal )
{

#if 0
    tb_write( TB_SPY, spyVal );
#endif

} /* tb_spy() */



/*==================== Function Separator =============================*/
void tb_sync(uint64_t syncVal)
{

#if 0
    tb_write( TB_SYNC, syncVal );
#endif

} /* tb_Sync() */



/*==================== Function Separator =============================*/
void tb_buff(uint64_t *buff)
{

#if 0
    tb_write( TB_BUFF, buff );
#endif

} /* tb_buff() */


/*==================== Function Separator =============================*/
void tb_measureClock( uint64_t clockID, uint64_t address )
{

#if 0
    uint64_t clockMeasureDataStruct[2];
    clockMeasureDataStruct[0] = clockID;
    clockMeasureDataStruct[1] = address;

    tb_write(64, &clockMeasureDataStruct);
#endif

} /* tb_measureClock() */



/*==================== Function Separator =============================*/
void tb_sendSoCaddr( uint64_t SoCbaseAddr )
{

#if 0
    uint64_t  clockID;
    uint64_t  clockAddr;
    uint64_t  dataStruct[2];

    printDbg( "\nSoCbaseAddr is : %d\n",  SoCbaseAddr );
    tb_write(67, SoCbaseAddr );

    dataStruct[0] = 0;
    dataStruct[1] = SoCbaseAddr+ 2*sizeof(uint64_t);
    tb_write(64, &dataStruct);

    dataStruct[0] = 1;
    dataStruct[1] = SoCbaseAddr+ 3*sizeof(uint64_t);
    tb_write(64, &dataStruct);

    dataStruct[0] = 2;
    dataStruct[1] = SoCbaseAddr+ 4*sizeof(uint64_t);
    tb_write(64, &dataStruct);
#endif

} /* tb_sendSoCaddr() */


/*==================== Function Separator =============================*/
void tb_write( uint64_t Id, uint64_t Param )
{

#if 0
/*
    uint64_t pData[3];

    switch( Id ) {
        case TB_STOP:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_TBCFG:
            pData[0] = 0;
            pData[1] = pTB;
            pData[2] = 0xABBA;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_SPY:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_SYNC:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        case TB_BUFF:
            pData[0] = Param;
            pTb_Id[1] = (uint64_t)pData;
            break;
        default:
            pTb_Id[1] = Param;

    }
*/

    pTb_Id[1] = Param;
    __asm__ __volatile__ ("fence\n");
    pTb_Id[0] = Id;
    __asm__ __volatile__ ("fence\n");

   /* Clear Id to enable next one  */
    pTb_Id[0] = 0;
    __asm__ __volatile__ ("fence\n");
#endif

} /* tb_write() */



/*****     <EOF>     *****/
