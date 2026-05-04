/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

/**
 *  @Component    INTH
 *
 *  @Filename     inth.h
 *
 *  @Description  Contains Interrupt handler functions allowing
 *                the registering of interrupt handlers for specific vectors.
 *                This file also contains primary ARM interrupt handler.
 *
 *                Available Functions:
 *                   INTH_Init             - Initialises all interrupt handlers
 *                   INTH_SetupInterrupt   - Register and set up interupt handler
 *                   INTH_EnableInterrupt  - Enable interrupt in interrupt controller
 *                   INTH_DisableInterrupt - Disable interrupt in interrupt controller
 *                   INTH_GetIntCount      - Get counter value for an interrupt
 *                   INTH_GetIntHandler    - Get interupt handler for an interrupt
 *                   INTH_ClearIntCount    - Reset counter for an interrupt
 *
 *//*======================================================================== */
#ifndef __INTH_H
#define __INTH_H


/* ============================================================================
* INCLUDE FILES (only if necessary)
* =============================================================================
*/


#ifdef __cplusplus
extern "C"
{
#endif


/* ============================================================================
* EXPORTED DEFINITIONS
* =============================================================================
*/

// Needed for riscv64-GCC
// Add GCC define and encapsulate
#define __irq __attribute__((interrupt ("machine")))  // Needed for GCC. Could be supervisor or user
#define __irq_s __attribute__((interrupt ("supervisor")))

#define __weak __attribute__((weak))


/*-------------------------------------------------------------------------*//**
 * @DEFINITION   INTH_NUMBER_OF_INT
 *
 * @BRIEF        defines the number of interrupts
 *
 * @DESCRIPTION  number of interrupts. It is used to size the following arrays:
 *                 - interrupt counter
 *                 - interrupt fuction association
 *
 *//*------------------------------------------------------------------------ */




/* ============================================================================
* EXPORTED TYPES
* =============================================================================
*/

/*-------------------------------------------------------------------------*//**
 * @TYPE         INTH_HANDLER_t
 *
 * @BRIEF        function pointer to TC interrupt handlers
 *
 * @DESCRIPTION  A function pointer to TC interrupt handlers.
 *               This is not real interrupt handler, but User wrapper which is
 *               called from real interrupt service routine.
 *               The functions should
 *               be defined in the following format:
 *
 *               void PREFIX_IntHandler( uint64_t irqNo );
 *
 *//*------------------------------------------------------------------------ */
typedef void (*INTH_HANDLER_t)( uint64_t );



/*-------------------------------------------------------------------------*//**
 * @TYPE         INTH_TYPE_t
 *
 * @BRIEF        interrupt Type
 *
 * @DESCRIPTION  interrupt Type ( IRQ/FIQ )
 *
 *//*------------------------------------------------------------------------ */
typedef enum inth_type
{
    INTH_TYPE_IRQ_EDGE = 0,
    INTH_TYPE_IRQ_LEVEL,
    INTH_TYPE_FIQ_EDGE,
    ITNH_TYPE_FIQ_LEVEL
} INTH_TYPE_t;



/*-------------------------------------------------------------------------*//**
 * @DEFINITION  INTH_DEFAULT_INTERRUPT_TYPE
 *
 * @BRIEF        default interrupt type
 *
 * @DESCRIPTION  default interrupt type (IRQ or FIQ)
 *
 *//*------------------------------------------------------------------------ */
#define INTH_DEFAULT_INTERRUPT_TYPE     INTH_TYPE_IRQ_LEVEL



/*-------------------------------------------------------------------------*//**
 * @TYPE         INTH_PRIORITY_t
 *
 * @BRIEF        interrupt priority
 *
 * @DESCRIPTION  interrupt priority
 *
 *//*------------------------------------------------------------------------ */
typedef uint32_t INTH_PRIORITY_t;



/*-------------------------------------------------------------------------*//**
 * @DEFINITION   INTH_DEFAULT_PRIORITY
 *
 * @BRIEF        defines the default priority
 *
 * @DESCRIPTION  default priority to apply to the interrupt
 *
 *//*------------------------------------------------------------------------ */
#define INTH_DEFAULT_PRIORITY     7

/*-------------------------------------------------------------------------*//**
 * @DEFINITION   CSR fields
 *
 * @BRIEF        defines for CSR register fields
 *
 * @DESCRIPTION  ddefines for CSR register fields to enable i set interrupts
 *

*//*------------------------------------------------------------------------ */
#define MEIE                      0x800
#define MIE                       0x8
#define SEIE                      0x200
#define SIE                       0x2
#define SSIP                      0x2
#define MEIE_AND_SEIE_ENABLE      0xA00
#define MIE_AND_SIE_ENABLE        0xA
#define CLEAR_MPP_AND_xPx_BITS    0x17f0ul
#define SET_MSTATUS_FOR_S_MODE    0x1008
#define ENABLE_ALL_INTERRUPTS     0x890BBB
#define ENABLE_GLOBAL_INTERRUPTS  0xB

#define MSIE_BIT_POSITION         3
#define MTIE_BIT_POSITION         7
/* ============================================================================
* EXPORTED VARIABLES
* =============================================================================
*/

/* ============================================================================
* EXPORTED FUNCTIONS/MACROS
* =============================================================================
*/


//extern void inth_printError( void );
/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_setup_trap_vectors
 *
 * @BRIEF         Setup mtvec and stvec
 *
 * @RETURNS       Nothing
 *
 * @DESCRIPTION   See BRIEF
 *
 * @METHOD
 *
 * @NOTES         This function is called between test launches in SP
 *                manufacturing tests
 *
 *//*------------------------------------------------------------------------ */

extern  void inth_setup_trap_vectors(void);


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_init
 *
 * @BRIEF         Interrupt Handler initialisation
 *
 * @RETURNS       et_status_t
 *                     et_OK                       No errors occured
 *                     other                          Error.
 *
 * @DESCRIPTION   Carries out Interrupt handler initialisation for all interrupt
 *                vectors
 *
 * @METHOD        Clear arrays of interrupt counter and interrupt function association
 *
 * @NOTES         This function should not be called directly by test cases.
 *
 *//*------------------------------------------------------------------------ */

extern et_status_t inth_init( void );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_setupInterrupt
 *
 * @BRIEF         Install the specified interrupt handler
 *
 * @param[in]     intSource (Interrupt source for which to install interrupt)
 * @param[in]     intHandler (interrupt handler for specified source (NULL is valid input))
 * @param[in]     intPriority (Specifies interrupt priority)
 *
 * @RETURNS       et_status_t
 *                     et_OK                       No errors occured
 *                     other                          Error.
 *
 * @DESCRIPTION   Installs the specified interrupt handler for the specified
 *                interrupt type and
 *                configure interrupt controler specific stuff
 *
 * @METHOD        Installs handler in array with offset on vector.
 *
 *
 *//*------------------------------------------------------------------------ */

extern et_status_t inth_setupInterrupt( uint64_t             intSource,
                                        INTH_HANDLER_t       intHandler,
                                        INTH_PRIORITY_t      intPriority);


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_setupException
 *
 * @BRIEF         Install the specified exception handler
 *
 * @param[in]     intSource (Interrupt source for which to install interrupt)
 * @param[in]     intHandler (interrupt handler for specified source (NULL is valid input))
 * @param[in]     intPriority (Specifies interrupt priority)
 *
 * @RETURNS       et_status_t
 *                     et_OK                       No errors occured
 *                     other                          Error.
 *
 * @DESCRIPTION   Installs the specified exception handler for the specified
 *                interrupt type and
 *                configure exception controler specific stuff
 *
 * @METHOD        Installs handler in array with offset on vector.
 *
 *
 *//*------------------------------------------------------------------------ */

  extern et_status_t inth_setupException( uint64_t          intSource,
                                          INTH_HANDLER_t    intHandler);

  /* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_disableException
 *
 * @BRIEF         Remove the specified exception handler
 *
 * @param[in]     intSource (Interrupt source for which to install interrupt)
 *
 * @RETURNS       et_status_t
 *                     et_OK                       No errors occured
 *                     other                          Error.
 *
 * @DESCRIPTION   Remove the exception handler from the list that
 *                holds them
 *
 * @METHOD        Remove handler in array with offset on vector.
 *
 *
 *//*------------------------------------------------------------------------ */

extern et_status_t inth_disableException( uint64_t              intSource);

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_freeInterrupt
 *
 * @BRIEF         Remove interrupt handler
 *
 * @param[in]     intSource (Interrupt source for which to install interrupt)
 *
 * @RETURNS       et_status_t
 *                     et_OK                       No errors occured
 *                     other                          Error.
 *
 * @DESCRIPTION   Remove interrupt handler for the specified
 *                interrupt type
 *                Disable interrupt before removing handler
 *
 *//*------------------------------------------------------------------------ */

extern et_status_t inth_freeInterrupt( uint32_t source );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_enableInterrupt
 *
 * @BRIEF         Enable the specified interrupt
 *
 * @param[in]     intSource (Interrupt source for which to enable interrupt)
 *
 * @RETURNS       et_status_t
 *                     RET_OK                    No errors occured
 *
 * @DESCRIPTION   Enable the specified interrupt
 *
 * @METHOD        Call Interrupt Controler enable.
 *
 *//*------------------------------------------------------------------------ */

extern et_status_t inth_enableInterrupt( uint64_t intSource, uint32_t target );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_disableInterrupt
 *
 * @BRIEF         Disable the specified interrupt
 *
 * @param[in]     intSource (Interrupt source for which to disable interrupt)
 *
 * @RETURNS       et_status_t
 *                     RET_OK                    No errors occured
 *
 * @DESCRIPTION   Disable the specified interrupt
 *
 * @METHOD        Call Interrupt Controler disable.
 *
 *//*------------------------------------------------------------------------ */

extern et_status_t inth_disableInterrupt( uint64_t source, uint32_t target);


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_getIntCount
 *
 * @BRIEF         Gives the number of interrupts
 *
 * @param[in]     intSource (Interrupt source for which to get count value)
 *
 * @RETURNS       uint32_t
 *                     Number of processed interrupts for selected interrupt source
 *
 * @DESCRIPTION   Gives the number of interrupts for a given type that occured
 *                since the interrupt handler was registered or count cleared
 *
 * @METHOD        Checks input parameter
 *                Read value from array  with source as the offset
 *
 *//*------------------------------------------------------------------------ */

extern uint32_t inth_getIntCount( uint64_t intSource );

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_getExcptCount
 *
 * @BRIEF         Gives the number of exceptions
 *
 * @param[in]     intSource (Exception source for which to get count value)
 *
 * @RETURNS       uint32_t
 *                     Number of processed exceptions for selected interrupt source
 *
 * @DESCRIPTION   Gives the number of exceptions for a given type that occured
 *                since the exception handler was registered or count cleared
 *
 * @METHOD        Checks input parameter
 *                Read value from array  with source as the offset
 *
 *//*------------------------------------------------------------------------ */

extern uint32_t inth_getExcptCount( uint64_t intSource );

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_getIntHandler
 *
 * @BRIEF         Gives interrupt handler address
 *
 * @param[in]     intSource (Interrupt source for which to get handler address)
 *
 * @RETURNS       INTH_HANDLER_t
 *                     Address of User interrupt Handler Routine attached
 *                     to selected interrupt Source
 *
 * @DESCRIPTION   Gives the interrupt handler address for a specific interrupt type
 *
 * @METHOD        Checks input parameter
 *                Read value from array  with source as the offset
 *
 *//*------------------------------------------------------------------------ */

extern INTH_HANDLER_t inth_GetIntHandler( uint64_t intSource );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_clearIntCount
 *
 * @BRIEF         Reset interrupt count to 0
 *
 * @param[in]     intSource (Interrupt source for which to clear count value)
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   Reset interrupt count to 0
 *
 * @METHOD        Checks input parameter
 *                Set count for specified register to 0
 *
 *//*------------------------------------------------------------------------ */
extern void inth_clearIntCount( uint16_t intSource);

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_getPendingInterrupt
 *
 * @BRIEF         get pending Number for selected source
 *
 * @RETURNS       uint64_t
 *
 * @DESCRIPTION   get pending Number for selected source
 *
 *//*------------------------------------------------------------------------ */
extern uint64_t inth_getPendingInterrupt( uint64_t source );

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_getPriority
 *
 * @BRIEF         get pending Number for selected source
 *
 * @RETURNS       uint64_t
 *
 * @DESCRIPTION   get pending Number for selected source
 *
 *//*------------------------------------------------------------------------ */
extern uint64_t inth_getPriority( uint64_t source );

void __irq inth_fiqIsr( void );
void __irq inth_swIrqIsr( void );
void inth_printError( void );

#ifdef SP_MANUFACTURING_TESTS
/* ------------------------------------------------------------------------*//**
 * @FUNCTION      inth_reset_state_and_priv
 *
 * @BRIEF         Remove all the Int/Exc handlers and jumps to M-mode
 *
 * @RETURNS       et_status_t
 *
 * @DESCRIPTION   Interrups setted by inth_init are not removed. Cleanup functio
 *                This function is intended to be used on Bring effort
 *
 *//*------------------------------------------------------------------------ */
void  inth_reset_state_and_priv(void);
#endif


#ifdef __cplusplus
}
#endif

#endif       /* __INTH_H */

/*      <EOF>     */
