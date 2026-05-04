/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$
* @version $Release$
* @date $Date$
* @author 
*
* @brief tb.h Test Bench interface
*
* SW for various SW <-> SV synchronization
*/

#ifndef __TB_H
#define __TB_H

#ifdef __cplusplus
extern "C"
{
#endif

/* =============================================================================
 * EXPORTED DEFINITIONS
 * =============================================================================
 */ 


/*-------------------------------------------------------------------------*//**
 * @DEFINITION   TB_TC_PASS
 *
 * @BRIEF        TC Pass status
 *
 * @DESCRIPTION  TC Pass status.
 *
 *//*------------------------------------------------------------------------ */
#define TB_TC_PASS      0x0000


/*-------------------------------------------------------------------------*//**
 * @DEFINITION   TB_TC_FAIL
 *
 * @BRIEF        TC Fail status
 *
 * @DESCRIPTION  TC Fail status.
 *
 *//*------------------------------------------------------------------------ */
#define TB_TC_FAIL      0x000F

#define TB_PRINT                    0x01
#define TB_STOP                     0x02
#define TB_ERROR                    0x03
#define TB_SPY                      0x04
#define TB_TBCFG                    0x05
#define TB_SYNC                     0x06
#define TB_BUFF                     0x07
#define TB_RANDOM                   0x08
#define TB_RUNNING_CPUS             0x09
#define TB_TC_PARAM_RANDOM          0x10
#define TB_TC_PARAM_DEFAULT         0x11



/* =============================================================================
 * EXPORTED TYPES
 * =============================================================================
 */


/*-------------------------------------------------------------------------*//**
 * @TYPE         TB_t
 *
 * @BRIEF        TB block
 *
 * @DESCRIPTION  struct which ia interface between SW and TB
 *               Main communication method block.
 *               Contains configuration for other components 
 *               ( tb2sw, sw2tb, spy and printf ).
 *
 *//*------------------------------------------------------------------------ */ 

typedef struct tb
{
    uint64_t     cpuId;               // 0x0000
    uint64_t     tcStatus;
    uint64_t     masterTcStatus;
    uint64_t     dummy1;
    uint64_t     msgQueueAddress;     // 0x0010
    uint64_t     msgQueueSize;
    uint64_t     spyAddress;
    uint64_t     spySize;
    uint64_t     printfAddress;
    uint64_t     printfSize;
    uint64_t     printModeSelect;
    uint64_t     dummy2;
    uint64_t     tcParamAddress;      // 0x0030
    uint64_t     tcParamSelect;       // Filled by TB
} TB_t;



/*-------------------------------------------------------------------------*//**
 * @TYPE         TB_TC_STATUS_t
 *
 * @BRIEF        TC status enumeration
 *
 * @DESCRIPTION  TC status definitions
 *
 *//*------------------------------------------------------------------------ */ 
typedef enum tb_tc_status
{
    TB_TC_NOT_READY      = 0,
	TB_TC_STARTED,
	TB_TC_INITIALIZED,
	TB_TC_RUNNING        = 0xAAAA0000,
	TB_TC_FATAL_ERROR    = 0xEEEE0000,
	TB_TC_FINISH         = 0xFFFF0000
} TB_TC_STATUS_t;


/*-------------------------------------------------------------------------*//**
 * @TYPE         TB_CLOCK_ID_e
 *
 * @BRIEF        Soc clock enumeration
 *
 * @DESCRIPTION  Listing several key clocks in SoC which need to be measured
 *
 *//*------------------------------------------------------------------------ */ 
typedef enum tb_clock_id
{
    TB_EXT_100MHZ      = 0,
	TB_EXT_24MHZ,
	TB_EXT,
	TB_PLL0,
	TB_PLL1,
	TB_PLL2,
	TB_PLL3,
	TB_PLL4,
	TB_GOLD,
	TB_SILVER,
    TB_BRONZE,
    
} TB_CLOCK_ID_e;


/*-------------------------------------------------------------------------*//**
 * @TYPE         TB_buff_t
 *
 * @BRIEF        TB block
 *
 * @DESCRIPTION  struct initialization of buffer
 *
 *//*------------------------------------------------------------------------ */ 

typedef struct tb_buff
{
    uint64_t     Addr;
    uint64_t     Size;
    uint64_t     FillGet;
    uint64_t     Val;
} TB_buff_t;


/* =============================================================================
 * EXPORTED VARIABLES
 * =============================================================================
 */ 
 
 /*-------------------------------------------------------------------------*//**
 * @VARIABLE     pTB
 *
 * @BRIEF        Pointer to CPU TB struct
 *
 * @DESCRIPTION  Pointer to CPU TB struct.
 *
 *//*------------------------------------------------------------------------ */ 
 extern TB_t *pTB;
 
 
 /* =============================================================================
 * EXPORTED FUNCTIONS
 * =============================================================================
 */ 
 
 

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_init
 *
 * @BRIEF         Test Bench initialization
 *
 * @param[in]     void
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   Carries out Test Bench initialization.
 *                In case of problems, function never returns.
 *                CPU life completes in CPU_Dead
 *
 *//*------------------------------------------------------------------------ */
extern void tb_init(void); 


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_finish
 *
 * @BRIEF         Test Bench Finish Test Case regulary.
 *
 * @param[in]     (uint32_t) status -> TC pass/Fail Status
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   Test Case regulary completes here.
 *                Status is passed to TB and CPU loops for ever.
 *                Function Never Returns.
 *
 *//*------------------------------------------------------------------------ */
extern void tb_finish( uint32_t status );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_fatalError
 *
 * @BRIEF         Test Bench Fatal Error detected
 *
 * @param[in]     (uint32_t) error -> TC Error Code to be reported
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   Test Case completes here in case of fatal error.
 *                Error is passed to TB and CPU loops for ever.
 *                Function Never Returns.
 *
 *//*------------------------------------------------------------------------ */
extern void tb_fatalError( uint32_t error );



/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_setTCStatus
 *
 * @BRIEF         Test Bench Status set
 *
 * @param[in]     (TB_TC_STATUS_t) status -> TC Status to be set
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   Pass current TC status to TB.
 *
 *//*------------------------------------------------------------------------ */
extern void tb_setTCStatus( TB_TC_STATUS_t status );




/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_printf
 *
 * @BRIEF         TB print string parsing and output.  
 *
 * @param[in]     printf style VA list
 *
 * @RETURNS       int
 *
 * @DESCRIPTION   Send printf args to be parsed  and displayed by TB
 *                Current limit to 15 VA elements
 *
 *//*------------------------------------------------------------------------ */
extern int tb_printf(char *format, ...);


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_getRandom( char *format, ... )
 *
 * @BRIEF         Get random values from TB. Used when randomized values
 *                not needed in verilog part of the test
 *
 * @param[in]     printf style VA list
 *
 * @RETURNS       int64
 *
 * @DESCRIPTION   TB parse format and VA list, and generate random number. 
 *                Current limit to 16 VA elements
 *                Accepted string formats:
 *                  "MEMBERS: 1 3 4 %d %d", val1, val2
 *                  "RNG: 0x134, %d", maxRng
 *
 *//*------------------------------------------------------------------------ */
extern uint64_t tb_getRandom( char *format, ... );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_getTcParamPtr( char *format, ... )
 *
 * @BRIEF         Fill tcParam area
 *
 * @param[in]     tcParam data type: Random/Default
 *
 * @RETURNS       void *
 *
 * @DESCRIPTION   TB parse tcParam.h and fill random/default data
 *
 *//*------------------------------------------------------------------------ */
void *tb_getTcParamPtr( uint64_t tcParamType );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_runningCPUs( uint64_t runningCPUs )
 *
 * @BRIEF         Send list of running CPU's to TB
 *
 * @param[in]     runningCPUs  uin64_t. Each bit represents one running CPU in test
 *
 * @RETURNS       void *
 *
 * @DESCRIPTION   tcMain parse tcConfig.h and pass arg to TB.
 *
 *//*------------------------------------------------------------------------ */
void tb_runningCPUs( uint64_t running_CPUs );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_measureClock( uint64_t clockID, uint64_t address )
 *
 * @BRIEF         Measure clock frequency in Soc
 *
 * @param[in]     SOC_CLOCK_e clockID
 * @param[in]     uint64_t address
 *
 * @RETURNS       void
 *
 * @DESCRIPTION  Measure frequency of a wire in SoC.
 *               This function takes 2 arguments:
 *               - clockID - ID of a SoC clock to be measured
 *               - address - target address to which measured 
 *                 frequency will be written to
 *
 *
 *//*------------------------------------------------------------------------ */
void tb_measureClock( uint64_t clockID, uint64_t address );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_genuine_boot( uint64_t action  )
 *
 * @BRIEF         assert/deassert genuine_boot_pad
 *
 * @param[in]     uint64_t action
 *
 * @RETURNS       void
 *
 * @DESCRIPTION  action = 0 -> deassert genuine_boot_pad
 *               action = 1 -> assert genuine_boot_pad
 *
 *
 *//*------------------------------------------------------------------------ */
void tb_genuine_boot(uint64_t action );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_external_clk_sel( uint64_t action  )
 *
 * @BRIEF         select external clock
 *
 * @param[in]     uint64_t strap_in
 *
 * @RETURNS       void
 *
 * @DESCRIPTION  strap_in = 0 -> 100Mhz
 *               strap_in = 1 -> 24Mhz osc
 *               strap_in = 2 -> External 1GHz
 *
 *
 *//*------------------------------------------------------------------------ */
void tb_external_clk_sel(uint64_t strap_in );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_sendSoCaddr( uint64_t SoCbaseAddr )
 *
 * @BRIEF         send message to verilog component
 *
 * @param[in]     uint64_t SoCbaseAddr
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   FUTURE:
 *
 *
 *//*------------------------------------------------------------------------ */
void tb_sendSoCaddr( uint64_t SoCbaseAddr );

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_sendPadsAddr( uint64_t padsBaseAddr );
 *
 * @BRIEF         send message to verilog component
 *
 * @param[in]     uint64_t SoCbaseAddr
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   FUTURE:
 *
 *
 *//*------------------------------------------------------------------------ */
void tb_sendPadsAddr( uint64_t padsBaseAddr );

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_spy( uint64_t spyVal )
 *
 * @BRIEF         
 *
 * @param[in]     uint64_t spyVal
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   FUTURE:
 *
 *
 *//*------------------------------------------------------------------------ */
void tb_spy( uint64_t spyVal );

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_sync( uint64_t syncVal )
 *
 * @BRIEF         
 *
 * @param[in]     uint64_t syncVal
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   FUTURE:
 *
 *
 *//*------------------------------------------------------------------------ */
void tb_sync( uint64_t syncVal );

/* ------------------------------------------------------------------------*//**
 * @FUNCTION      tb_buff( uint64_t *buff );
 *
 * @BRIEF         
 *
 * @param[in]     uint64_t *buff
 *
 * @RETURNS       void
 *
 * @DESCRIPTION   FUTURE:
 *
 *
 *//*------------------------------------------------------------------------ */
void tb_buff( uint64_t *buff );


#ifdef __cplusplus
}
#endif

#endif	/* __TB_H */


/*****     <EOF>     *****/
