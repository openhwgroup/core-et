/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

/**
*   Component:     INTH - Interrupt (IRQ) handler library component
*
*   Filename:      inth.c
*
*   Description:   Interrupt handler functions
*
* =============================================================================
*/

/* ============================================================================
* STANDARD INCLUDE FILES
* =============================================================================
*/

/* ============================================================================
* PROJECT SPECIFIC INCLUDE FILES
* =============================================================================
*/

#include "cpu.h"
#include "soc.h"
#include "macros.h"
#include "inth.h"
#include "plic_api.h"
#include "tb.h"

/* ============================================================================
* GLOBAL VARIABLES DECLARATIONS
* =============================================================================
*/

PLIC_API_t *plicPu_handle;

const char *excp_name[EXCP_NUMBER]= {
                "INSTRUCTION ADDRESS MISALIGNED",
                "INSTRUCTION ACCESS FAULT",
                "ILLEGAL INSTRUCTION",
                "BREAKPOINT",
                "LOAD ADDRESS MISALIGNED",
                "LOAD ACCESS FAULT",
                "STORE/AMO ADDRESS MISALIGNED",
                "STORE/AMO ACCESS FAULT",
                "USER ECALL EXCEPTION",
                "SUPERVISOR ECALL EXCEPTION",
                "VS ECALL EXCEPTION",
                "MACHINE ECALL EXCEPTION",
                "INSTRUCTION PAGE FAULT",
                "LOAD PAGE FAULT",
                "RESERVED",
                "STORE/AMO PAGE FAULT",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "FETCH_BUS_ERROR",
                "FETCH_ECC_ERROR",
                "LOAD_PAGE_SPLIT_FAULT",
                "STORE_PAGE_SPLIT_FAULT",
                "BUS_ERROR",
                "MCODE_INSTRUCTION",
                "TXFMA_OFF"
};

const char *interrupt_name[L1_IRQ_NUMBER] = {
                "USER SOFTWARE INTERRUPT",
                "SUPERVISOR SOFTWARE INTERRUPT",
                "RESERVED",
                "MACHINE SOFTWARE INTERRUPT",
                "USER TIMER INTERRUPT",
                "SUPERVISOR TIMER INTERRUPT",
                "RESERVED",
                "MACHINE TIMER INTERRUPT",
                "USER EXTERNAL INTERRUPT",
                "SUPERVISOR EXTERNAL INTERRUPT",
                "RESERVED",
                "MACHINE EXTERNAL INTERRUPT",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "RESERVED",
};

extern void tc_finish( et_status_t est );
/* ============================================================================
* LOCAL TYPES AND DEFINITIONS
* =============================================================================
*/

/* ============================================================================
* LOCAL VARIABLES DECLARATIONS
* =============================================================================
*/
   /* array of interrupt handlers */
/*__attribute__ ((section ("sys_nvm")))*/  INTH_HANDLER_t inth_irqHandler[INTH_NUMBER_OF_INT + 1] = { 0 };
/*__attribute__ ((section ("sys_nvm")))*/  INTH_HANDLER_t inth_excptHandler[EXCP_NUMBER] = { 0 };
                                           INTH_HANDLER_t inth_L1_HandlerM[L1_IRQ_NUMBER] = { 0 };

  /* array of interrupt counters */
/*__attribute__ ((section ("sys_nvm")))*/  uint64_t inth_irqCount[INTH_NUMBER_OF_INT + 1] = { 0 };
/*__attribute__ ((section ("sys_nvm")))*/  uint64_t inth_excptCount[EXCP_NUMBER] = { 0 };
                                           uint64_t inth_L1_Count[L1_IRQ_NUMBER] = { 0 };
/* ============================================================================
* LOCAL FUNCTIONS PROTOTYPES
* =============================================================================
*/
void excpt_handler(uint64_t excpNo);
void interrupt_handler(uint64_t irqNo);
void __irq inth_mTrapIsr( void ) __attribute__ ((section (".Mmode_trap_code")));
void __irq_s inth_sTrapIsr( void ) __attribute__ ((section (".Smode_trap_code")));
/* ============================================================================
* FUNCTIONS
* =============================================================================
*/

/*==================== Function Separator =============================*/
et_status_t inth_init(void)
{
    uint64_t addr;
    uint64_t rdVal;
    uint64_t wrVal;
    et_status_t est = ET_PASS;

#ifndef MAXION_SA
    API_IP_PARAMS_t IpParams;

    plic_init();

    /* Opening PLICs*/
    IpParams.baseAddress = R_PU_PLIC_BASEADDR;
    plicPu_handle = (PLIC_API_t *)plic_open( 1, &IpParams );
#endif

    /* Enabling Maxion interrupts except machine Sw int and mtimer int*/
    rdVal = CSRregRead( mie );
    wrVal = (rdVal | ENABLE_ALL_INTERRUPTS) & ~(0x1u<<MSIE_BIT_POSITION | 0x1u<<MTIE_BIT_POSITION);
    CSRregWrite( mie, wrVal );

    rdVal = CSRregRead( mstatus );
    wrVal = rdVal | ENABLE_GLOBAL_INTERRUPTS;
    CSRregWrite( mstatus, wrVal );


    /* Jumping to the trap routine */
    addr = (uint64_t)&(inth_mTrapIsr);
    CSRregWrite( mtvec, addr );

    /* Jumping to the trap routine S mode */
    addr = (uint64_t)&(inth_sTrapIsr);
    CSRregWrite( stvec, addr );

    return est;

}  /* inth_init() */


/*==================== Function Separator =============================*/
et_status_t inth_setupInterrupt( uint64_t             intSource,
                                        INTH_HANDLER_t      intHandler,
                                        INTH_PRIORITY_t      intPriority)
{
    et_status_t est = ET_OK;
    uint64_t L1_irq;

    /* reset counter and install handler */
    inth_irqCount[intSource] = 0;

    /* Install interrupt handler if not  null */
    if( intHandler ) {
        if( intSource < SP_L1_IRQ_OFFSET) {
            inth_irqHandler[intSource] = intHandler;
        }
        else {
            L1_irq = intSource - SP_L1_IRQ_OFFSET;
            inth_L1_HandlerM[L1_irq] = intHandler;
         }
    }

   if( intSource < SP_L1_IRQ_OFFSET ){
       /* Set priority for PU PLIC */
       est = plic_setPriority( plicPu_handle, intSource, (uint32_t)intPriority );
   }

   return est;

} /* inth_setupInterrupt() */


/*==================== Function Separator =============================*/
et_status_t inth_setupException( uint64_t              intSource,
                                    INTH_HANDLER_t     intHandler)
{

    et_status_t est = ET_OK;

        /* reset counter and install handler */
        inth_excptCount[intSource] = 0;
        /* Install interrupt handler if not  null */
        if( intHandler ) {
            inth_excptHandler[intSource] = intHandler;
        }
        else {
            est = ET_FAIL;
        }

    return est;

} /* inth_setupException() */

#if 0
/*==================== Function Separator =============================*/
et_status_t inth_freeInterrupt( uint64_t source, uint32_t target)
{
    et_status_t est;

    est = ET_OK;

    /* Disable Interrupts */
    est = inth_disableInterrupt( source, target );

    /* Clear Interrupt Handler */
    inth_irqHandler[source] = NULL;

    return est;

} /* inth_freeInterrupt() */
#endif

/*==================== Function Separator =============================*/
et_status_t inth_enableInterrupt( uint64_t intSource, uint32_t target )
{

  et_status_t est = ET_OK;
  uint64_t  wrVal;
  uint64_t  L1id;
  uint8_t   puId = 0;

#ifndef MAXION_SA
  if( intSource < SP_L1_IRQ_OFFSET ) {
     /* Enabling PU PLIC interrupts */
     est = plic_enableInterrupt( intSource, target, puId);
  }
  else {
        L1id   = intSource - SP_L1_IRQ_OFFSET;
        wrVal  = CSRregRead( mie );
        wrVal |= ( 0x1u << L1id );
        CSRregWrite( mie, wrVal );
  }
#else
  L1id   = (intSource - SP_L1_IRQ_OFFSET) | ((uint8_t) target & puId);
  wrVal  = CSRregRead( mie );
  wrVal |= ( 0x1u << L1id );
  CSRregWrite( mie, wrVal );
#endif

  return est;

} /* inth_enableInterrupt() */


/*==================== Function Separator =============================*/
et_status_t inth_disableInterrupt( uint64_t source, uint32_t target )
{

  et_status_t est = ET_OK;
  uint64_t  wrVal;
  uint64_t  L1id;
  uint8_t   puId = 0;

#ifndef MAXION_SA
  if( source < SP_L1_IRQ_OFFSET ){
    /* Disabling PU PLIC interrupts */
    est = plic_disableInterrupt( source, target, puId );
  }
  else{
        L1id   = source - SP_L1_IRQ_OFFSET;
        wrVal  = CSRregRead( mie );
        wrVal &= (~( 0x1u << L1id ));
        CSRregWrite( mie, wrVal );
  }
#else
  L1id   = (source - SP_L1_IRQ_OFFSET) | ((uint8_t) target & puId);
  wrVal  = CSRregRead( mie );
  wrVal &= (~( 0x1u << L1id ));
  CSRregWrite( mie, wrVal );
#endif

    return est;

} /* inth_disableInterrupt() */

/*==================== Function Separator =============================*/
uint64_t inth_getPendingInterrupt( uint64_t source )
{
    uint64_t pendingValue;

#ifndef MAXION_SA
    pendingValue = plic_getPendingInterrupt( plicPu_handle, source );
#else
    pendingValue = source;
#endif

    return pendingValue;

} /* inth_getPendingInterrupt() */

/*==================== Function Separator =============================*/
uint64_t inth_getPriority( uint64_t source )
{
    uint64_t priorityValue;

#ifndef MAXION_SA
    priorityValue = plic_getPriority( plicPu_handle, source );
#else
    priorityValue = source;
#endif

    return priorityValue;

} /* inth_getPendingInterrupt() */

/*==================== Function Separator =============================*/
uint32_t inth_getIntCount( uint64_t   intSource )
{

    uint32_t ret;

    /* Check Input Value and add Spy error */

    /* return count value */
    ret = (uint32_t) inth_irqCount[intSource];

    return ret;

} /* inth_getIntCount() */


/*==================== Function Separator =============================*/
uint32_t inth_getExcptCount( uint64_t  excptVector )
{

    uint32_t ret;

    /* Check Input Value and add Spy error */

    /* return count value */
    ret = (uint32_t) inth_excptCount[excptVector & 0x03];

    return ret;

} /* inth_getExcptCount() */


#if 0
/*==================== Function Separator =============================*/
INTH_HANDLER_t inth_getIntHandler( uint64_t intSource )
{

    INTH_HANDLER_t ret;

    /* return Current handler Value */
    ret = inth_irqHandler[intSource];

    return ret;

} /* inth_getIntHandler() */


/*==================== Function Separator =============================*/
void inth_clearIntCount( uint64_t intSource )
{

    /* Clear count value */
    inth_irqCount[intSource] = 0;

} /* inth_clearIntCount() */
#endif

/*==================== Function Separator =============================*/
void __irq __weak inth_mTrapIsr( void )
{
    uint64_t irqNo;
    uint32_t target;
    uint64_t trapVal;
    uint64_t id;
    uint64_t L1_id;
    uint64_t mask = 0x1;

    /* Reading the Interrupt Acknowledge Register to get the interrupt */
    trapVal = CSRregRead( mcause );
    id      = trapVal >> 63;
    mask    = ~(mask << 63);
    L1_id   = trapVal & mask;

    target  = (uint32_t) CSRregRead( mhartid );
    target  = (target + 1) * 2; // MEIP

#ifndef MAXION_SA
    if(  L1_id != SP_MACHINE_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET )
      irqNo = L1_id;
    else
      irqNo   = plic_getIrqNo( plicPu_handle, target );
#else
    if(  L1_id != SP_MACHINE_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET )
      irqNo = L1_id;
    else
      irqNo = 0;
#endif

    /* Go to the interrupt handler */
    switch (id){
        case 0:
              excpt_handler( trapVal );
              break;
        case 1:
              interrupt_handler( irqNo );
              break;
        default:
              /* printDbg("\n \tHandler for this trap does not exist\n"); */
              break;
    }

#ifndef MAXION_SA
  if( L1_id == SP_MACHINE_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET ){
      plic_setIrqNo( plicPu_handle , (uint32_t)irqNo, target );
    }
#endif

  return;

}  /* inth_mTrapIsr() */

void __irq_s __weak inth_sTrapIsr( void )
{
    uint64_t irqNo;
    uint64_t trapVal;
    uint64_t id;
    uint64_t L1_id;
    uint64_t mask = 0x1;
#ifndef MAXION_SA
    uint32_t target;
#endif

    /* Reading the Interrupt Acknowledge Register to get the interrupt */
    trapVal = CSRregRead( scause );
    id      = trapVal >> 63;
    mask    = ~(mask << 63);
    L1_id   = trapVal & mask;

#ifndef MAXION_SA
    target  = ((uint32_t) CSRregRead(mhartid) + 1) * 2 + 1; // FIXME

    if( L1_id != SP_SUPERVISOR_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET )
      irqNo = L1_id;
    else
      irqNo   = plic_getIrqNo( plicPu_handle, target );
#else
    if( L1_id != SP_SUPERVISOR_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET )
      irqNo = L1_id;
    else
      irqNo = 0;
#endif

    /* Go to the interrupt handler */
    switch (id){
        case 0:
              excpt_handler( trapVal );
              break;
        case 1:
              interrupt_handler( irqNo );
              break;
        default:
              /* printDbg("\n \tHandler for this trap does not exist\n"); */
              break;
    }

#ifndef MAXION_SA
    if( L1_id == SP_SUPERVISOR_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET ){
        plic_setIrqNo( plicPu_handle , (uint32_t)irqNo, target );
    }
#endif

    return;
}  /* inth_sTrapIsr() */

/*==================== Function Separator =============================*/
void __irq inth_fiqIsr( void )
{

    while( 1 );

}  /* inth_fiqIsr() */



/*==================== Function Separator =============================*/
void __irq inth_swIrqIsr( void )
{

   while( 1 );

} /* inth_swIrqIsr() */


void inth_printError( void )
{
    printDbg( "\n\n\n");
    printDbg( "\t\tComplete test case status is :\n\n" );
    printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" );
    printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" );
    printDbg( "                              !!!!  F A I L E D  !!!!                             \n" );
    printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" );
    printDbg( "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" );

} /* inth_printError() */

/*==================== Function Separator =============================*/
void excpt_handler(uint64_t excpNo)
{
    uint64_t mtvalVal;

    et_status_t est = ET_OK;

    inth_excptCount[excpNo] ++;

    if( inth_excptHandler[excpNo] ) {
        inth_excptHandler[excpNo]( excpNo );
    }
    else{

        if( excpNo < EXCP_NUMBER ){
           printDbg( " \n\n Exception is %s and we do not have specific handler for it!!!", excp_name[excpNo] );
           est = ET_FAIL;
           (void) excpNo;
        }
        else{
           printDbg( " \n\n *****Handler has got an unknown exception [%d]!!! *****", excpNo );
           est = ET_FAIL;
        }

        /* Reading from the mtval CSR to get better informatio abouth exceptions*/
        if ( excpNo == ILLEGAL_INSTRUCTION || ( excpNo >= LOAD_ADDRESS_MISALIGNED && excpNo <= STORE_ACCESS_FAULT ) || excpNo == LOAD_PAGE_FAULT || excpNo == STORE_PAGE_FAULT ){
             mtvalVal = CSRregRead( mtval );
             printDbg( " \n\t\t\tValue from CSR tval is 0x%h\n ", mtvalVal );
             (void) mtvalVal;
        }

        tb_finish( est );
    }

}
void interrupt_handler(uint64_t irqNo)
{
    et_status_t est = ET_OK;

    inth_irqCount[irqNo] ++;

    if ( irqNo > INTH_NUMBER_OF_INT ){
            printDbg("irqNo in handler is %d and it is greater then max numer of interrupts! \n", irqNo);
            est = ET_FAIL;
            tb_finish( est );
    }

    else{
        /* Handling interrrupts with the specific written function */
        if( inth_irqHandler[irqNo] )
            inth_irqHandler[irqNo]( irqNo );

        else if( inth_L1_HandlerM[irqNo] )
            inth_L1_HandlerM[irqNo]( irqNo );

        else{
           printDbg( "\n\n***** Doesn't exist spesific handler for this interrupt! *****\n" );
           est = ET_FAIL;
           tb_finish( est );
           }
    }

}
/*      <EOF>     */

