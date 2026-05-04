/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
 *  @Component     API
 *
 *  @Filename     api.h
 *
 *  @Description  Contains definition for API interfaces
 *
 *//*======================================================================== */

#ifndef __API_H
#define __API_H


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


/*-------------------------------------------------------------------------*//**
 * @DEFINITION   API MODE & STATUS
 *
 * @BRIEF        defines common stuff needed for each API
 *
 * @DESCRIPTION  
 *
 *//*------------------------------------------------------------------------ */
#define API_MODE_NOT_READY    0
#define API_MODE_POLING       1
#define API_MODE_IRQ          2
#define API_MODE_DMA          3

#define API_STATUS_IDLE       0
#define API_STATUS_RUN        1
#define API_STATUS_COMPLETED  2
#define API_STATUS_ERROR      3

#define API_NON_BLOCKING      0xFFFFFFFF


/* ============================================================================
* EXPORTED TYPES
* =============================================================================
*/

/*-------------------------------------------------------------------------*//**
 * @TYPE         API_CFG_t
 *
 * @BRIEF        Structure holding runtime parameters for API
 *
 * @DESCRIPTION  Run time pararameters needed for proper data transfer 
 *
 *//*------------------------------------------------------------------------ */
typedef struct api_cfg
{
    uint32_t   mode;
    uint32_t   irq;
    uint32_t   irqMask;
    uint32_t   drq;
    void       (*userIsr)(uint32_t irq );
    /* Buffer */
    void      *pBuff;
    uint32_t   size;
    uint32_t   count;
    /* Run Time */
    uint32_t   status;
    uint32_t   transferCount;
    uint32_t   errorStatus;
    uint32_t   irqErrorStatus;
    uint32_t   dmaErrorStatus;

} API_CFG_t;



/*-------------------------------------------------------------------------*//**
 * @TYPE         API_IP_PARAMS_t
 *
 * @BRIEF        Structure used to pass IP integration specific parameters
 *
 * @DESCRIPTION  Pass Base Address, IRQ and DRQ to IP API
 *
 *//*------------------------------------------------------------------------ */
typedef struct api_ip_params
{
    uint32_t   baseAddress;
    uint32_t   txMode;
    uint32_t   txIrq;
    uint32_t   txDrq;
    uint32_t   rxMode;
    uint32_t   rxIrq;
    uint32_t   rxDrq;
} API_IP_PARAMS_t;


/* ============================================================================
* EXPORTED VARIABLES
* =============================================================================
*/

/* ============================================================================
* EXPORTED FUNCTIONS/MACROS
* =============================================================================
*/



#ifdef __cplusplus
}
#endif

#endif    /* __API_H */

/*      <EOF>     */

