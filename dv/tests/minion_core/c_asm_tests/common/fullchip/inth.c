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
#include "minion.h"


/* ============================================================================
* GLOBAL VARIABLES DECLARATIONS AND DEFINITONS
* =============================================================================
*/

PLIC_API_t *plic_handle;
PLIC_API_t *plicPu_handle;

const char *excp_name[EXCP_NUMBER]={ "INSTRUCTION ADDRESS MISALIGNED", "ACCESS FAULT", "ILLEGAL INSTRUCTION", "BREAKPOINT", "LOAD ADDRESS MISALIGNED", "LOAD ACCESS FAULT",
                                     "STORE/AMO ADDRESS MISALIGNED","STORE/AMO ACCESS FAULT", "USER ECALL EXCEPTION", "HS ECALL EXCEPTION", "VS ECALL EXCEPTION","MACHINE ECALL EXCEPTION",
                                     "INSTRUCTION PAGE FAULT", "LOAD PAGE FAULT","", "STORE/AMO PAGE FAULT","","","","","","","","","", "FETCH_BUS_ERROR", "FETCH_ECC_ERROR",
                                     "LOAD_PAGE_SPLIT_FAULT", "STORE_PAGE_SPLIT_FAULT", "BUS_ERROR", "MCODE_INSTRUCTION", "TXFMA_OFF" };
                                     
/* ============================================================================
* LOCAL TYPES AND DEFINITIONS
* =============================================================================
*/
#define tbHandlerId 68
//int tb_printf(char *format, ...){}

/* ============================================================================
* LOCAL VARIABLES DECLARATIONS
* =============================================================================
*/
   /* array of interrupt handlers */
/*__attribute__ ((section ("sys_nvm")))*/  INTH_HANDLER_t inth_irqHandler[INTH_NUMBER_OF_INT + 1]; 
/*__attribute__ ((section ("sys_nvm")))*/  INTH_HANDLER_t inth_excptHandler[EXCP_NUMBER] = { 0 };

  /* array of interrupt counters */
/*__attribute__ ((section ("sys_nvm")))*/  uint64_t inth_irqCount[INTH_NUMBER_OF_INT + 1];
/*__attribute__ ((section ("sys_nvm")))*/  uint64_t inth_excptCount[EXCP_NUMBER] = { 0 };
                                           uint64_t inth_L1_Count[L1_IRQ_NUMBER] = { 0 };
/* ============================================================================
* LOCAL FUNCTIONS PROTOTYPES
* =============================================================================
*/
void excpt_handler(uint64_t excpNo);
void interrupt_handler(uint64_t irqNo);
//void __irq inth_mTrapIsr( void ) __attribute__ ((section (".Mmode_trap_code")));
__attribute__((aligned(4096))) void __irq inth_mTrapIsr( void );
/* ============================================================================
* FUNCTIONS
* =============================================================================
*/
inline uint64_t __attribute__((always_inline))  csr_mie_read( )         
{                                  
    uint64_t ret;                  
    __asm__ __volatile__(            
        "csrr %[ret], mie\n"  
        : [ret] "=r" (ret)
        : 
        : 
    );              
    return ret;                           
}                                  
inline uint64_t __attribute__((always_inline))  csr_mtval_read( )         
{                                  
    uint64_t ret;                  
    __asm__ __volatile__(            
        "csrr %[ret], mtval\n"  
        : [ret] "=r" (ret)
        : 
        : 
    );              
    return ret;                           
}
inline uint64_t __attribute__((always_inline))  csr_mcause_read( )         
{                                  
    uint64_t ret;                  
    __asm__ __volatile__(            
        "csrr %[ret], mcause\n"  
        : [ret] "=r" (ret)
        : 
        : 
    );              
    return ret;                           
}
inline void __attribute__((always_inline)) csr_mie_write(uint64_t val )
{                                  
    __asm__ __volatile__(            
        "csrw mie, %[val]\n"  
        : 
        : [val] "r" (val)
        : 
    );              
}

inline uint64_t __attribute__((always_inline))  csr_mstatus_read( )         
{                                  
    uint64_t ret;                  
    __asm__ __volatile__(            
        "csrr %[ret], mstatus\n"  
        : [ret] "=r" (ret)
        : 
        : 
    );              
    return ret;                           
}                                  

inline void __attribute__((always_inline)) csr_mstatus_write(uint64_t val )
{                                  
    __asm__ __volatile__(            
        "csrw mstatus, %[val]\n"  
        : 
        : [val] "r" (val)
        : 
    );              
}

inline void __attribute__((always_inline)) csr_mtvec_write(uint64_t val )
{                                  
    __asm__ __volatile__(            
        "csrw mtvec, %[val]\n"  
        : 
        : [val] "r" (val)
        : 
    );              
}
/*==================== Function Separator =============================*/
et_status_t inth_init(void)
{
    uint64_t addr;
    uint64_t rdVal;
    uint64_t wrVal;
    uint64_t arrCnt[2];
    uint64_t arrHandler[2];
    
    API_IP_PARAMS_t IpParams;
    et_status_t est = ET_OK;

    plic_init();

    /* Opening PLICs*/
    IpParams.baseAddress = R_SP_PLIC_BASEADDR;
    plic_handle = (PLIC_API_t *)plic_open( 0, &IpParams );
    IpParams.baseAddress = R_PU_PLIC_BASEADDR;
    plicPu_handle = (PLIC_API_t *)plic_open( 1, &IpParams );

    /* Enabling interrupts to the Minion */
    rdVal = csr_mie_read( );
    wrVal = rdVal | MEIE;
    csr_mie_write( wrVal );

    rdVal = csr_mstatus_read( );
    wrVal = rdVal | MIE;
    csr_mstatus_write( wrVal );
    
    /* Jumping to the trap routine */
    addr = (uint64_t)(inth_mTrapIsr);
    csr_mtvec_write( addr );

    /* Clear Local Variables */
    arrCnt[0] = inth_irqCount[0]; 
    arrCnt[1] = INTH_NUMBER_OF_INT + 1;
    //tb_write( tbHandlerId, arrCnt );
    
    arrHandler[0] = inth_irqHandler[0];
    arrHandler[1] = INTH_NUMBER_OF_INT + 1;
    //tb_write( tbHandlerId, arrHandler );

    return est;

}  /* inth_init() */


/*==================== Function Separator =============================*/
et_status_t inth_setupInterrupt(  uint64_t              intSource,
                                    INTH_HANDLER_t      intHandler,
                                    INTH_PRIORITY_t       intPriority)
{
    et_status_t est = ET_OK;
    uint64_t L1_irq;
    
    /* Set priority for PU PLIC */
    if( intSource > PU_PLIC_IRQ_OFFSET ){
       intSource -= PU_PLIC_IRQ_OFFSET;
       est = plic_setPriority( plicPu_handle, (uint64_t)intSource, (uint32_t)intPriority );
    }
    /* Set priority for SPIO PLIC */
    else
       est = plic_setPriority( plic_handle, (uint64_t)intSource, (uint32_t)intPriority );

    inth_irqHandler[intSource] = intHandler;
} /* inth_setupInterrupt() */


/*==================== Function Separator =============================*/
et_status_t inth_setupException( uint64_t              intSource,
                                    INTH_HANDLER_t     intHandler)
{
    et_status_t est = ET_OK;

    /* reset counter and install handler */
    inth_excptCount[intSource] = 0;
 
    /* Install interrupt handler if not  null */
    if( intHandler )
        inth_excptHandler[intSource] = intHandler;
    else 
        est = ET_FAIL;
        
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
    uint8_t   id;
    volatile uint64_t  wrVal;
    uint64_t  L1id;

    if( intSource < SP_L1_IRQ_OFFSET ) {
  	    /* Enabling PU PLIC interrupts */
        if( intSource > PU_PLIC_IRQ_OFFSET ){
           intSource -= PU_PLIC_IRQ_OFFSET;
           id  = 0;
           est = plic_enableInterrupt( intSource, target, id );
        }
        /* Enabling SPIO PLIC interrupts */
        else{
           id = 1;
           est = plic_enableInterrupt( intSource, target, id );
        }
    }
    else {
        L1id   = intSource - SP_L1_IRQ_OFFSET;
        wrVal  = CSRregRead( mie );
        wrVal |= ( 1 << L1id );
        CSRregWrite( mie, wrVal );
    }    

    return est;

} /* inth_enableInterrupt() */


/*==================== Function Separator =============================*/
et_status_t inth_disableInterrupt( uint64_t source, uint32_t target )
{
    et_status_t est = ET_OK;
    uint64_t  wrVal;
    uint64_t  L1id;
    uint8_t   id;

    /* Disabling PU PLIC interrupts */
    if( source > PU_PLIC_IRQ_OFFSET ){
       source -= PU_PLIC_IRQ_OFFSET;
       id  = 0;
       est = plic_disableInterrupt( source, target, id );
    }
    /* Disabling SPIO PLIC interrupts */
    else{
       id = 1;
       est = plic_disableInterrupt( source, target, id );        
    }

    return est;

} /* inth_disableInterrupt() */

/*==================== Function Separator =============================*/
uint64_t inth_getPendingInterrupt( uint64_t source )
{
    uint64_t pendingValue;

    pendingValue = plic_getPendingInterrupt( plicPu_handle, source );

    return pendingValue;

} /* inth_getPendingInterrupt() */

/*==================== Function Separator =============================*/
uint64_t inth_getPriority( uint64_t source )
{
    uint64_t priorityValue;

    priorityValue = plic_getPriority( plicPu_handle, source );

    return priorityValue;

} /* inth_getPendingInterrupt() */


/*==================== Function Separator =============================*/
uint32_t inth_getIntCount( uint64_t   intSource )
{
    uint32_t ret;

    /* FUTURE:Check Input Value and add Spy error */

    /* return count value */
    ret = inth_irqCount[intSource];

    return ret;

} /* inth_getIntCount() */


/*==================== Function Separator =============================*/
uint32_t inth_getExcptCount( uint64_t  excptVector )
{
    uint32_t ret;

    /*FUTURE: Check Input Value and add Spy error */

    /* return count value */
    ret = inth_excptCount[excptVector];

    return ret;

} /* inth_getExcptCount() */


#if 0
/*==================== Function Separator =============================*/
INTH_HANDLER_t inth_getIntHandler( uint16_t intSource )
{

    INTH_HANDLER_t ret;

    /* return Current handler Value */
    ret = inth_irqHandler[intSource];

    return ret;

} /* inth_getIntHandler() */


/*==================== Function Separator =============================*/
void inth_clearIntCount( uint16_t intSource )
{
    /* Clear count value */
    inth_irqCount[intSource] = 0;

} /* inth_clearIntCount() */
#endif

/*==================== Function Separator =============================*/
void __irq inth_mTrapIsr( void )
{
    uint64_t irqNo;
    uint32_t target;
    uint64_t trapVal;
    uint64_t id;
    uint64_t mask = 0x1;
    uint32_t hart_id;
    uint32_t shire_id;

    /* Reading the Interrupt Acknowledge Register to get the interrupt */
    trapVal = csr_mcause_read();
    id      = trapVal >> 63;
    mask    = ~( mask << 63 );
    hart_id = get_hart_id();

    if( (hart_id & 1) == 0) {
       shire_id = ((hart_id>>6) & 0x3F);
    }
    if ( shire_id <= 32){
       target   = 10; //other Minions
       irqNo = plic_getIrqNo( plicPu_handle, target );
    }
    else {
       target = 0; //SP
       irqNo = plic_getIrqNo( plic_handle, target );
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
              /* //printDbg("\n \tHandler for this trap does not exist\n"); */
              break;
    }

  return;

}  /* inth_mTrapIsr() */

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

        /* Reading from the mtval CSR to get better informatio abouth exceptions*/
        mtvalVal = csr_mtval_read();

        /*if( excpNo < EXCP_NUMBER ){
           ////printDbg( " \n\n Exception is %s and we do not have specific handler for it!!!\n Value from CSR mtval is 0x%h\n ", excp_name[excpNo], mtvalVal );
           est = ET_FAIL;
        }
        else{
           ////printDbg( " \n\n *****Handler has got an unknown exception [%d]!!! *****\n Value from CSR mtval is 0x%h\n ", excpNo, mtvalVal );
           est = ET_FAIL;
        }*/

           C_TEST_FAIL;//tb_finish( est );
    }

}
/*==================== Function Separator =============================*/
void interrupt_handler(uint64_t irqNo)
{
    et_status_t est = ET_OK;

    inth_irqCount[irqNo] ++;
    
    if ( irqNo > INTH_NUMBER_OF_INT ){
            //printDbg( "irqNo in handler is %d and it is greater then max numer of interrupts! \n", irqNo );
            //est = ET_FAIL;
           C_TEST_FAIL;//tb_finish( est );
    }

    else{
        /* Handling interrrupts with the specific written function */
        if( inth_irqHandler[irqNo] )
            inth_irqHandler[irqNo]( irqNo );
        else{
            //printDbg( "\n\n***** Doesn't exist spesific handler for this interrupt! *****\n" );
            //est = ET_FAIL;
           C_TEST_FAIL;//tb_finish( est );
        }
    }

}
/*      <EOF>     */

