/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file tcRegtest.c
* @version $Release$
* @date $Date$
* @author
*
* @brief
*
* Setup SoC to enable TC run
/**
 *  @Component      tcRegtest
 *
 *  @Filename       dv/common/sw/src/tcRegtest.c
 *
 *  @Description    Code to run Register Access Test.
 *                  Register Set to access is defined in Test Case folder
 *
 *//*======================================================================== */

#include "cpu.h"
#include "soc.h"
#include "tb.h"
#include "api.h"

  
#include "regtest.h"
#include "tcRegtest.h"

#ifdef pSHIRE_TESTS
#include "pshire_common.h"
#endif
/* Register Access Test Function */
/*==================== Function Separator =============================*/
et_status_t tcRegtest_TestFunction( void )
{

    et_status_t    est;
    uint32_t       i;
    uint32_t       error;
    
    error = 0;
    est   = ET_PASS;
#ifdef pSHIRE_TESTS
pcie_ss_power_up();
/*    uint32_t     *pRegs32;
    printDbg( "\tEnable plls..\n");
    pllMovellus_setClockFreq( pShire_pll_handle, EXTERNAL100MHZ, PLL_PSHIRE_OUTPUT);
    pRegs32 = (uint32_t *)(R_PCIE_ESR_BASEADDR + PSHIRE_PSHIRE_CTRL_OFFSET);
    pRegs32[0] = (1 << PSHIRE_PSHIRE_CTRL_PLL0_BYP_LSB);
    printDbg( "\tRelease power_up_rst..\n"); */
    /* Deassert PCIe cold reset */
/*    pRegs32 = (uint32_t *)(R_PCIE_ESR_BASEADDR + PSHIRE_PSHIRE_RESET_OFFSET);
    pRegs32[0] = (1 << PSHIRE_PSHIRE_RESET_PWR_UP_RSTN_LSB); */
#endif

#if CPU==1
    /* Release maxion PLLs from reset */
    et_cru_reset( cru_handle, RESET_COLD_MAX, RELEASE);
    et_cru_reset( cru_handle, RESET_WARM_MAX_UNCORE, RELEASE);
    et_cru_reset( cru_handle, RESET_N_MAX_PLL_UNCORE, RELEASE);
    et_cru_reset( cru_handle, RESET_N_MAX_PLL_CORE, RELEASE);
#endif


#ifdef EMMC_TEST
    #pragma message "This is pu_emmc regtest."
    et_cru_reset( cru_handle, RESET_N_EMMC_SEQUENCE, RELEASE);
#endif



    printDbg( "\tTest Started on CPU Id: %d\n\n", CPU );


    /* Test Reset Values */
    printDbg( "Test Reset Values ....\n" );
#ifdef REGISTER_WAIVERS
    #pragma message "WAIVERS regtest_resetValues_with_waivers"
    error = regtest_resetValues_with_waivers( regBaseAddress, pRegTable, listRegisterWaivers );
#else 
    error = regtest_resetValues( regBaseAddress, pRegTable);
#endif

    if( error == 0 ) {
        printDbg( "\t\t\tPass\n" );
    }
    else {
        est = ET_FAIL;
        printDbg( "\t\t\tXXX   FAILED   XXX\n" );
        printDbg( "\t\t\tError Count : %d\n", error );
    }


//  #ifdef REGTEST_RUN_RW_TEST
    /* Test Register Access */
    printDbg( "Test Register Access ....\n" );
    error = regtest_access( regBaseAddress, pRegTable );

    if( error == 0 ) {
        printDbg( "\t\t\tPass\n" );
    }
    else {
        est = ET_FAIL;
        printDbg( "\t\t\tXXX   FAILED   XXX\n" );
        printDbg( "\t\t\tError Count : %d\n", error );
    }
//  #endif

//  #ifdef REGTEST_DUMP
    /* Dump register Values */
    printDbg( "Dump Register Values ....\n" );
    regtest_dump( regBaseAddress, pRegTable );
//  #endif

    printDbg( "\n\tTest Completed on CPU Id: %d\n", CPU );


    return( est );

} /* tcRegtest_TestFunction() */



/*     <EOF>     */

