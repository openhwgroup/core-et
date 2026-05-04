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
* GLOBAL VARIABLES DECLARATIONS AND DEFINITONS
* =============================================================================
*/

PLIC_API_t *plic_handle;
PLIC_API_t *plicPu_handle;


const char *excp_name[EXCP_NUMBER] = {
                "INSTRUCTION ADDRESS MISALIGNED",
                "INSTRUCTION ACCESS FAULT",
                "ILLEGAL INSTRUCTION",
                "BREAKPOINT",
                "LOAD ADDRESS MISALIGNED",
                "LOAD ACCESS FAULT",
                "STORE/AMO ADDRESS MISALIGNED",
                "STORE/AMO ACCESS FAULT",
                "USER ECALL EXCEPTION",
                "SUPERVSOR ECALL EXCEPTION",
                "RESERVED",
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
                "INSTRUCTION_BUS_ERROR",
                "INSTRUCTION_ECC_ERROR",
                "LOAD_SPLIT_PAGE_FAULT",
                "STORE_SPLIT_PAGE_FAULT",
                "RESERVED",
                "MCODE_INSTRUCTION",
                "RESERVED"
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
                "BAD IPI REDIRECT INTERRUPT",
                "RESERVED",
                "RESERVED",
                "INSTRUCTION CACHE ECC COUNTER OVERFLOW INTERRUPT",
                "RESERVED",
                "RESERVED",
                "RESERVED",
                "BUS ERROR INTERRUPT",
};


INTH_HANDLER_t inth_irqHandler[INTH_NUMBER_OF_INT + 1] = { 0 };
INTH_HANDLER_t inth_excptHandler[EXCP_NUMBER] = { 0 };
INTH_HANDLER_t inth_L1_HandlerM[L1_IRQ_NUMBER] = { 0 };

uint64_t inth_irqCount[INTH_NUMBER_OF_INT + 1] = { 0 };
uint64_t inth_excptCount[EXCP_NUMBER] = { 0 };
uint64_t inth_L1_Count[L1_IRQ_NUMBER] = { 0 };
                                           
uint64_t initDone[2] = { 0 };

extern void tc_finish( et_status_t est );

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

    uint64_t rdVal;
    uint64_t wrVal;

    uint32_t target = 0;

    API_IP_PARAMS_t IpParams;
    et_status_t est = ET_PASS;

    plic_init();

    /* Opening PLICs */
    IpParams.baseAddress = R_SP_PLIC_BASEADDR;
    plic_handle = (PLIC_API_t *)plic_open( 0, &IpParams );
    IpParams.baseAddress = R_PU_PLIC_BASEADDR;
    plicPu_handle = (PLIC_API_t *)plic_open( 1, &IpParams );

    rdVal = CSRregRead( mstatus );
    wrVal = rdVal | ENABLE_GLOBAL_INTERRUPTS;
    CSRregWrite( mstatus, wrVal );

    /* Enabling interrupts to the SP */
    rdVal = CSRregRead( mie );
    wrVal = rdVal | ENABLE_ALL_INTERRUPTS;
    CSRregWrite( mie, wrVal );

    /* Enablig PU PLIC interrupts which are going to the SPIO PLIC */
    est = inth_enableInterrupt( SPIO_PLIC_PU_PLIC0_INTR_ID, target );
    est = plic_setPriority( plic_handle, SPIO_PLIC_PU_PLIC0_INTR_ID, ( INTH_DEFAULT_PRIORITY - 1 ) );
    est = inth_enableInterrupt( SPIO_PLIC_PU_PLIC1_INTR_ID, 1 );
    est = plic_setPriority( plic_handle, SPIO_PLIC_PU_PLIC1_INTR_ID, ( INTH_DEFAULT_PRIORITY - 1 ) );
    FENCE;
    
    inth_setup_trap_vectors();
    initDone[0]++;

    return est;

}  /* inth_init() */

void inth_setup_trap_vectors(void)
{
  uint64_t addr;
  /* Jumping to the trap routine */
  addr = (uint64_t)&(inth_mTrapIsr);
  CSRregWrite( mtvec, addr );
  
  /* Jumping to the trap routine  S mode */
  addr = (uint64_t)&(inth_sTrapIsr);
  CSRregWrite( stvec, addr );
}

/*==================== Function Separator =============================*/
et_status_t inth_setupInterrupt(  uint64_t              intSource,
                                   INTH_HANDLER_t       intHandler,
                                    INTH_PRIORITY_t       intPriority)
{
    et_status_t est = ET_PASS;
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
    else
       return ET_FAIL;

   if( intSource < SP_L1_IRQ_OFFSET ){
       /* Set priority for PU PLIC */
       if( intSource >= PU_PLIC_IRQ_OFFSET ){
          intSource = intSource - PU_PLIC_IRQ_OFFSET;
          est = plic_setPriority( plicPu_handle, intSource, (uint32_t)intPriority );
       }
       /* Set priority for SPIO PLIC */
       else
          est = plic_setPriority( plic_handle, intSource, (uint32_t)intPriority );
   }

   return est;

} /* inth_setupInterrupt() */


/*==================== Function Separator =============================*/
et_status_t inth_setupException( uint64_t              intSource,
                                    INTH_HANDLER_t     intHandler)
{
    et_status_t est = ET_PASS;

    /* reset counter and install handler */
    inth_excptCount[intSource] = 0;

    /* Install interrupt handler if not  null */
    if( intHandler )
        inth_excptHandler[intSource] = intHandler;
    else
        est = ET_FAIL;

    return est;

} /* inth_setupException() */

/*==================== Function Separator =============================*/
et_status_t inth_disableException( uint64_t           intSource )
{
    et_status_t est = ET_PASS;

    /* reset counter and install handler */
    inth_excptCount[intSource] = 0;

    est = ET_FAIL ? !inth_excptHandler[intSource] : ET_PASS;
    
    /* Remove handler */
    inth_excptHandler[intSource] = NULL;   

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
    et_status_t est = ET_PASS;
    uint64_t  wrVal;
    uint64_t  L1id;
    uint8_t   id;
    uint32_t  targetMode;
      
    /* Choosing targets. Depends on Mode (1==S mode, 0==M mode) 
       Overwriting target instead of changing 100 tests. FUTURE: Clean code one day!!! */
    if( initDone[0] == 1 &&  pSoC != NULL )
        targetMode = (uint32_t)pSoC -> ChooseMode;
    else
        targetMode = target;
    
    if( intSource < SP_L1_IRQ_OFFSET ){
       /* Enabling PU PLIC interrupts */
       if( intSource >= PU_PLIC_IRQ_OFFSET ){
          intSource -= PU_PLIC_IRQ_OFFSET;
          id  = 0;
          est = plic_enableInterrupt( intSource, targetMode, id );
       }
       /* Enabling SPIO PLIC interrupts */
       else{
          id = 1;
          est = plic_enableInterrupt( intSource, targetMode, id );
       }
    }
    else{
       L1id   = intSource - SP_L1_IRQ_OFFSET;
       wrVal  = CSRregRead( mie );
       wrVal |= ( 0x1u << L1id );
       CSRregWrite( mie, wrVal );
    }

    return est;

} /* inth_enableInterrupt() */


/*==================== Function Separator =============================*/
et_status_t inth_disableInterrupt( uint64_t source, uint32_t target )
{
    et_status_t est = ET_PASS;
    uint64_t  wrVal;
    uint64_t  L1id;
    uint8_t   id;
    uint32_t  targetMode;
    
    /* Choosing taregets. Depends on Mode (1==S mode, 0==M mode) 
       Overwriting target instead of changing 100 tests. FUTURE: Clean code one day!!! */
    if( initDone[0] == 1  &&  pSoC != NULL )
        targetMode = (uint32_t)pSoC -> ChooseMode;
    else
        targetMode = target;
    
    if( source < SP_L1_IRQ_OFFSET ){
       /* Disabling PU PLIC interrupts */
       if( source >= PU_PLIC_IRQ_OFFSET ){
          source -= PU_PLIC_IRQ_OFFSET;
          id  = 0;
          est = plic_disableInterrupt( source, targetMode, id );
       }
       /* Disabling SPIO PLIC interrupts */
       else{
          id = 1;
          est = plic_disableInterrupt( source, targetMode, id );
       }
    }
    else{
       L1id   = source - SP_L1_IRQ_OFFSET;
       wrVal  = CSRregRead( mie );
       wrVal &= ( ~( 0x1u << L1id ) );
       CSRregWrite( mie, wrVal );
    }

    return est;

} /* inth_disableInterrupt() */

/*==================== Function Separator =============================*/
uint64_t inth_getPendingInterrupt( uint64_t source )
{
    uint64_t pendingValue;

    if( source >= PU_PLIC_IRQ_OFFSET ){
        source -= PU_PLIC_IRQ_OFFSET;
        pendingValue = plic_getPendingInterrupt( plicPu_handle, source );
    }
    else
        pendingValue = plic_getPendingInterrupt( plic_handle, source );

    return pendingValue;

} /* inth_getPendingInterrupt() */

/*==================== Function Separator =============================*/
uint64_t inth_getPriority( uint64_t source )
{
    uint64_t priorityValue;

    if( source >= PU_PLIC_IRQ_OFFSET ){
        source -= PU_PLIC_IRQ_OFFSET;
        priorityValue = plic_getPriority( plicPu_handle, source );
    }
    else
        priorityValue = plic_getPriority( plic_handle, source );

    return priorityValue;

} /* inth_getPendingInterrupt() */


/*==================== Function Separator =============================*/
uint32_t inth_getIntCount( uint64_t   intSource )
{
    uint32_t ret;

    /* FUTURE:Check Input Value and add Spy error */

    /* return count value */
    ret = (uint32_t) inth_irqCount[intSource];

    return ret;

} /* inth_getIntCount() */


/*==================== Function Separator =============================*/
uint32_t inth_getExcptCount( uint64_t  excptVector )
{
    uint32_t ret;

    /* FUTURE: Check Input Value and add Spy error */

    /* return count value */
    ret = (uint32_t) inth_excptCount[excptVector];

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
void __irq inth_mTrapIsr( void )
{
    uint64_t irqNo;
    uint64_t puIrqNo;
    //uint32_t cpuId;
    uint32_t target;
    uint64_t trapVal;
    uint64_t id;
    uint64_t L1_id;
    uint8_t  puPlicId;
    uint64_t mask = 0x1;

    /* Reading the Interrupt Acknowledge Register to get the interrupt */
    trapVal = CSRregRead( mcause );
    id      = trapVal >> 63;
    mask    = ~(mask << 63);
    L1_id   = trapVal & mask;
    puPlicId = 0;
    
    if( L1_id == SP_MACHINE_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET ){
        target = 0;
        if (pSoC != NULL) { //This check is needed if we use quickBooit, pSoC is uninitialized
           if ( pSoC -> intClaimReverse == 1 ){
             puIrqNo  = plic_getIrqNo( plicPu_handle, target );
           //printDbg( " \n\n The puIrqNo is %d", puIrqNo );
           }
        }     
        irqNo = plic_getIrqNo( plic_handle, target );
        //printDbg( " \n\n The spirqNo is %d", irqNo );
    }
    else if( L1_id == SP_SUPERVISOR_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET ){
        target = 1;
        if (pSoC != NULL) { //This check is needed if we use quickBoot, pSoC is uninitialized
           if ( pSoC -> intClaimReverse == 1 ){
              puIrqNo  = plic_getIrqNo( plicPu_handle, target );
              //printDbg( " \n\n The puIrqNo is %d", puIrqNo );
           }
        }
        irqNo = plic_getIrqNo( plic_handle, target );
        //printDbg( " \n\n The spirqNo is %d", irqNo );
    }   
    else
        irqNo = L1_id; 
        
    /* These two SPIO PLIC irq are outputs of PU PLIC */
    if( irqNo == SPIO_PLIC_PU_PLIC0_INTR_ID || irqNo == SPIO_PLIC_PU_PLIC1_INTR_ID ){
        if (pSoC != NULL) { //Need this if we use quickBoot, pSoC is uninitialized
           if ( pSoC -> intClaimReverse == 0){
              puIrqNo  = plic_getIrqNo( plicPu_handle, target );           
              //printDbg("\n \t The puIrqNo is %d", puIrqNo); 
           }
        }
        else {
           puIrqNo = plic_getIrqNo( plicPu_handle, target);
        }
        irqNo    = puIrqNo + PU_PLIC_IRQ_OFFSET ;
        puPlicId = 1;
    }
     
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

  if( (L1_id == SP_MACHINE_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET) || (L1_id == SP_SUPERVISOR_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET) ){
  
    if( puPlicId == 1 && (L1_id == SP_MACHINE_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET) ){
        plic_setIrqNo( plicPu_handle , (uint32_t) puIrqNo, target );
        plic_setIrqNo( plic_handle , SPIO_PLIC_PU_PLIC0_INTR_ID, target );
    }
    else if( puPlicId == 1 && (L1_id == SP_SUPERVISOR_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET) ){
        plic_setIrqNo( plicPu_handle , (uint32_t) puIrqNo, target );
        plic_setIrqNo( plic_handle , SPIO_PLIC_PU_PLIC1_INTR_ID, target );    
    }
    else
        plic_setIrqNo( plic_handle , (uint32_t) irqNo, target );
  }

  return;

}  /* inth_mTrapIsr() */

void __irq_s inth_sTrapIsr( void )
{
    uint64_t irqNo;
    uint64_t puIrqNo;
    //uint32_t cpuId;
    uint32_t target;
    uint64_t trapVal;
    uint64_t id;
    uint64_t L1_id;
    uint8_t  puPlicId;
    uint64_t mask = 0x1;
    
    /* Reading the Interrupt Acknowledge Register to get the interrupt */
    trapVal = CSRregRead( scause );
    id      = trapVal >> 63;
    mask    = ~(mask << 63);
    L1_id   = trapVal & mask;
    puPlicId = 0;


    
    if( L1_id == SP_MACHINE_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET ){
        target = 0;
        irqNo = plic_getIrqNo( plic_handle, target );
    }
    else if( L1_id == SP_SUPERVISOR_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET ){
        target = 1;
        irqNo = plic_getIrqNo( plic_handle, target );
    }    
    else
        irqNo = L1_id;


    /* These two SPIO PLIC irq are outputs of PU PLIC */
    if( irqNo == SPIO_PLIC_PU_PLIC1_INTR_ID || irqNo == SPIO_PLIC_PU_PLIC0_INTR_ID ){
        puIrqNo  = plic_getIrqNo( plicPu_handle, target );
        irqNo    = puIrqNo + PU_PLIC_IRQ_OFFSET ;
        puPlicId = 1;
    }

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

  if( (L1_id == SP_MACHINE_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET) || (L1_id == SP_SUPERVISOR_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET) ){
    if( puPlicId == 1 && (L1_id == SP_MACHINE_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET) ){
        plic_setIrqNo( plicPu_handle , (uint32_t) puIrqNo, target );
        plic_setIrqNo( plic_handle , SPIO_PLIC_PU_PLIC0_INTR_ID, target );
    }
    else if( puPlicId == 1 && (L1_id == SP_SUPERVISOR_EXTERNAL_IRQ - SP_L1_IRQ_OFFSET) ){
        plic_setIrqNo( plicPu_handle , (uint32_t) puIrqNo, target );
        plic_setIrqNo( plic_handle , SPIO_PLIC_PU_PLIC1_INTR_ID, target );    
    }
    else
        plic_setIrqNo( plic_handle , (uint32_t) irqNo, target );
  }

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
void excpt_handler( uint64_t excpNo )
{
    uint64_t tvalVal;

    et_status_t est = ET_PASS;

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
#ifndef POSTSI_UST
             if( pSoC -> ChooseMode == 1 ){
#else
             extern volatile uint8_t processor_mode;
             if (processor_mode == 1) {
#endif
                 tvalVal = CSRregRead( stval );
                 printDbg( " \n\t\t\tValue from CSR tval is 0x%h\n ", tvalVal );     
                 (void) tvalVal;       
             }    
             else{
                 tvalVal = CSRregRead( mtval );
                 printDbg( " \n\t\t\tValue from CSR tval is 0x%h\n ", tvalVal );  
                 (void) tvalVal;   
             }
        }

        tc_finish( est );
    }


}
void interrupt_handler( uint64_t irqNo ) 
{
    et_status_t est = ET_PASS;

    inth_irqCount[irqNo] ++;
    
    if ( irqNo > INTH_NUMBER_OF_INT + 1 ){
            printDbg( "irqNo in handler is %d and it is greater then max numer of interrupts! \n", irqNo );
            est = ET_FAIL;
            tc_finish( est );
    }
    
    else{
        /* Handling interrrupts with the specific written function */
        if( inth_irqHandler[irqNo] )
            inth_irqHandler[irqNo]( irqNo );

        else if( inth_L1_HandlerM[irqNo] )
            inth_L1_HandlerM[irqNo]( irqNo );

        else{
            printDbg( "\n\n***** Doesn't exist spesific handler for interrupt No %d which is %s! *****\n", irqNo, interrupt_name[irqNo] );
            est = ET_FAIL;
            tc_finish( est );
        }
    }

}

#ifdef SP_MANUFACTURING_TESTS

/*==================== Function Separator =============================*/
/*
 * Interrupt Handler used by `inth_switch_to_mmode`
 * Update mstatus to return in mmode and return to epc+4
 */
static void switch_to_mmode_handler(  uint64_t excpNo )
{
  (void) excpNo;
  // Set mstatus
  uint64_t status = CSRregRead( mstatus );
  status |= 0b11 << 11; // Set MPP to Mmode
  CSRregWrite( mstatus, status );

  // Increment PC
  uint64_t pc = CSRregRead( mepc );
  pc += 0x4;
  CSRregWrite( mepc, pc);
}

/*
 * Routine to return to switch to mmode from any mode
 * This function is intended to be used by `inth_reset_state_and_priv`
 */
static et_status_t inth_switch_to_mmode(void)
{
  et_status_t et = ET_PASS;
  inth_disableException( S_MODE_ECALL_EXCEPTION );
  inth_disableException( M_MODE_ECALL ); // Ignore return value. Just to be sure we can add a new one
  et = inth_setupException( M_MODE_ECALL, switch_to_mmode_handler );
  if (ET_PASS != et) {
      printDbg("Error setting ECALL handler\n\r");
      return ET_FAIL;
  }
  et = inth_setupException( S_MODE_ECALL_EXCEPTION, switch_to_mmode_handler );
  if (ET_PASS != et) {
      printDbg("Error setting ECALL handler\n\r");
      return ET_FAIL;
  }
  __asm__ volatile ("ecall\n"
                    "nop\n");

  // Cleanup and retun
  et += inth_disableException( M_MODE_ECALL );
  et += inth_disableException( S_MODE_ECALL_EXCEPTION );
  return et;
}

/*==================== Function Separator =============================*/
/*
 * Disable all exceptions/Interrupts and returns to M mode
 * this function is intended to be called from the SP bringup tests launcher in 
 * `sp-bringup` repo
 */
void inth_reset_state_and_priv(void)
{
  inth_switch_to_mmode();

  // Remove all xcpt delegations
  CSRregWrite(medeleg, 0);
  
  // Remove all exception handlers
  for (size_t excpNum = 0; excpNum < EXCP_NUMBER; ++excpNum)
  {
    inth_disableException((uint64_t)excpNum);
  }

  // Remove all interrupts
  uint32_t inth_target_mmode = 0;
  uint32_t inth_target_smode = 1;
  for (size_t intNum = 0; intNum < (INTH_NUMBER_OF_INT + 1); ++intNum)
  {
    // Do not remove interrupts set by init
    if ( SPIO_PLIC_PU_PLIC0_INTR_ID != intNum && SPIO_PLIC_PU_PLIC1_INTR_ID != intNum )
      inth_disableInterrupt(intNum, inth_target_mmode);
    
    inth_disableInterrupt(intNum, inth_target_smode);
  }
}
#endif

/*      <EOF>     */

