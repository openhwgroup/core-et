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

} /* tb_init() */



/*==================== Function Separator =============================*/
void tb_finish( uint32_t status )
{

} /* tb_finish() */



/*==================== Function Separator =============================*/
#define tb_fatalError(...)
/*void tb_fatalError( uint32_t error )
{

} */



/*==================== Function Separator =============================*/
void tb_setTCStatus( TB_TC_STATUS_t status )
{

} /* tb_setTCStatus() */


/*==================== Function Separator =============================*/
#define tb_printf(...)
/*{
   return 1;
} */



/*==================== Function Separator =============================*/
uint64_t tb_getRandom( char *format, ... )
{
   return 0;
} /* tb_getRandom() */



/*==================== Function Separator =============================*/
void *tb_getTcParamPtr( uint64_t tcParamType )
{
 return (void*)1 ;
} /* tb_getTcParamPtr() */



/*==================== Function Separator =============================*/
void tb_runningCPUs( uint64_t runningCPUs )
{

} /* tb_runningCPUs() */



/*==================== Function Separator =============================*/
void tb_spy( uint64_t spyVal )
{


} /* tb_spy() */



/*==================== Function Separator =============================*/
void tb_sync(uint64_t syncVal)
{

} /* tb_Sync() */



/*==================== Function Separator =============================*/
void tb_buff(uint64_t *buff)
{

} /* tb_buff() */


/*==================== Function Separator =============================*/
void tb_measureClock( uint64_t clockID, uint64_t address )
{

} /* tb_measureClock() */



/*==================== Function Separator =============================*/
void tb_sendSoCaddr( uint64_t SoCbaseAddr )
{


} /* tb_sendSoCaddr() */


/*==================== Function Separator =============================*/
void tb_write( uint64_t Id, uint64_t Param )
{


} /* tb_write() */



/*****     <EOF>     *****/
