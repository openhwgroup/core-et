/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */



/*=============================================================================
 * STANDARD INCLUDE FILES
 *=============================================================================*/

#include "cpu.h"
#include "soc.h"
#include "tb.h"
#include "spio_misc_esr.h"
#include "et.h"

/*=============================================================================
   GLOBAL VARIABLES DECLARATIONS
  =============================================================================*/
SOC_t         *pSoC;
ET_CRU_API_t  *cru_handle;
PLL_API_t     *spio_pll0_handle;
PLL_API_t     *spio_pll1_handle;
PLL_API_t     *spio_pll2_handle;
PLL_API_t     *spio_pll3_handle;
PLL_API_t     *spio_pll4_handle;
PLL_API_t     *pShire_pll_handle;


/*=============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 *=============================================================================*/


/*==================== Function Separator =============================*/
void socWait( uint32_t count )
{

} /* socWait() */


/*==================== Function Separator =============================*/
/* Compare measured and reference clock. Allow +-1% tolerance */
et_status_t checkClock( uint64_t measured, uint64_t ref )
{

 return ET_OK;

} /* checkClock() */


/*==================== Function Separator =============================*/
void pSocPrint()
{
} /* pSocPrint() */


#if CPU==1
/*==================== Function Separator =============================*/
et_status_t openPllPeripherals()
{

    return ET_OK;
}
#endif


/*==================== Function Separator =============================*/
#if CPU==1
et_status_t clockConfiguration()
{
    return ET_OK;
} /* clockConfiguration() */
#endif

/*=============================================================================
 * FUNCTIONS
 *=============================================================================*/
void soc_init( void )
{

}  /* soc_init() */

void soc_releaseCpuReset(uint32_t cpuId)
{
}

void soc_setCpuReset(uint32_t cpuId)
{
}

/*****     <EOF>     *****/
