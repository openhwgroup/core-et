/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file regtest.C
* @version $Release$ 
* @date $Date$
* @author 
*
* @brief 
*
* Setup SoC to enable TC run 
*/ 
/**
 *  @Component      REGTEST
 *
 *  @Filename       dv/comon/sw/utils/src/regtest.c
 *
 *  @Description    The REGTEST component contains utilities neede for 
 *                  register access tests
 *
 *//*======================================================================== */


/* =============================================================================
 * STANDARD INCLUDE FILES
 * =============================================================================
 */ 
#include "cpu.h"
#include "soc.h"
//#include "tb.h"
#include "regtest.h" 
 
 
/* =============================================================================
 * GLOBAL VARIABLES DECLARATIONS
 * =============================================================================
 */


/* =============================================================================
 * LOCAL TYPES AND DEFINITIONS
 * =============================================================================
 */


/* =============================================================================
 * LOCAL VARIABLES DECLARATIONS
 * =============================================================================
 */  

 
 /* =============================================================================
 * LOCAL FUNCTIONS PROTOTYPES
 * =============================================================================
 */


/* =============================================================================
 * FUNCTIONS
 * =============================================================================
 */

/*==================== Function Separator =============================*/
uint32_t regtest_resetValues_with_waivers( uint64_t baseAddress, REGTEST_t *pRegsTable, uint32_t* pListRegisterWaivers )
{

    uint32_t error;
    uint32_t regIndex;
    uint64_t regVal;
    uint64_t resetVal;
    uint8_t  *pReg8;
    uint16_t *pReg16;
    uint32_t *pReg32;
    uint32_t *pReg64;
    uint32_t *pReg64Upper;
  
    uint32_t *pRegWaiver;
    uint8_t  toWaive = 0 ;

    
    REGTEST_t *pRegs;

    pRegs    = pRegsTable;
    regIndex = 0;
    error    = 0;
    printDbg("Value of base addres = %x", baseAddress);

    while( pRegs->regAddress != REGTEST_END_OF_LIST ) {
        #define END_OF_WAIVERS_LIST 0xFFFFFFFF
        pRegWaiver = (uint32_t*)pListRegisterWaivers; 
        while (*pRegWaiver != (uint32_t) END_OF_WAIVERS_LIST) {
            if (*pRegWaiver == pRegs->regAddress) {
                printDbg("Waiving register: %0s", pRegs->regName);
                toWaive = 1;
                break;
            }
            pRegWaiver++;
        }

        if (toWaive == 1) {
            pRegs ++;
            regIndex ++;
            continue;
        }
        
        /* Check if we should red & compare register reset */
        if( pRegs->resetValue == RESET_VALUE_NA ) {
            pRegs ++;
            regIndex ++;
            continue;
        }
//        printDbg("pRegs->resetValue: %h\n", pRegs->resetValue);
        switch( pRegs->regSize ) {
            case REGTEST_SIZE_8_BIT:
                pReg8 = (uint8_t *)(baseAddress + pRegs->regAddress);
                regVal = *pReg8;
                resetVal = pRegs->resetValue & 0xFF;
                break;
            case REGTEST_SIZE_16_BIT:
                pReg16 = (uint16_t *)(baseAddress + pRegs->regAddress);
                regVal = *pReg16;
                resetVal = pRegs->resetValue & 0xFFFF;
                break;
            case REGTEST_SIZE_32_BIT:
                pReg32 = (uint32_t *)(baseAddress + pRegs->regAddress);
                regVal = *pReg32;
                resetVal = pRegs->resetValue;
                break;
            case REGTEST_SIZE_64_BIT:
                pReg64 = (uint32_t *)(baseAddress + pRegs->regAddress);
                pReg64Upper = (uint32_t *)(baseAddress + pRegs->regAddress + 4);                
                regVal = (*pReg64Upper);
                regVal = (regVal << 32);
                regVal = regVal | (*pReg64);
                resetVal = pRegs->resetValue;
//                pReg64 = (uint64_t *)(baseAddress + pRegs->regAddress);
//                regVal = *pReg64;
//                resetVal = pRegs->resetValue;
                break;
            default:
                return( REGTEST_ERROR );
        } 

         if( regVal != resetVal ) {        
            printDbg("ERROR READING: Register name: %0s \n\t\tregAddress: 0x%h\n\t\tresetVal(expected): 0x%h\n\t\tregVal(provided): 0x%h\n", pRegs->regName, pRegs->regAddress, resetVal, regVal);
            error ++;
       }

        pRegs ++;
        regIndex ++;
           
    }

    return( error );

}

uint32_t regtest_resetValues( uint64_t baseAddress, REGTEST_t *pRegsTable )
{ 

    uint32_t error;
    uint32_t regIndex;
    uint64_t regVal;
    uint64_t resetVal;
    uint8_t  *pReg8;
    uint16_t *pReg16;
    uint32_t *pReg32;
    uint32_t *pReg64;
    uint32_t *pReg64Upper;
    
    REGTEST_t *pRegs;

    pRegs    = pRegsTable;
    regIndex = 0;
    error    = 0;
    printDbg("Value of base addres = %x", baseAddress);

    while( pRegs->regAddress != REGTEST_END_OF_LIST ) {
        /* Check if we should red & compare register reset */
        if( pRegs->resetValue == RESET_VALUE_NA ) {
            pRegs ++;
            regIndex ++;
            continue;
        }
//        printDbg("pRegs->resetValue: %h\n", pRegs->resetValue);
        switch( pRegs->regSize ) {
            case REGTEST_SIZE_8_BIT:
                pReg8 = (uint8_t *)(baseAddress + pRegs->regAddress);
                regVal = *pReg8;
                resetVal = pRegs->resetValue & 0xFF;
                break;
            case REGTEST_SIZE_16_BIT:
                pReg16 = (uint16_t *)(baseAddress + pRegs->regAddress);
                regVal = *pReg16;
                resetVal = pRegs->resetValue & 0xFFFF;
                break;
            case REGTEST_SIZE_32_BIT:
                pReg32 = (uint32_t *)(baseAddress + pRegs->regAddress);
                regVal = *pReg32;
                resetVal = pRegs->resetValue;
                break;
            case REGTEST_SIZE_64_BIT:
                pReg64 = (uint32_t *)(baseAddress + pRegs->regAddress);
                pReg64Upper = (uint32_t *)(baseAddress + pRegs->regAddress + 4);                
                regVal = (*pReg64Upper);
                regVal = (regVal << 32);
                regVal = regVal | (*pReg64);
                resetVal = pRegs->resetValue;
//                pReg64 = (uint64_t *)(baseAddress + pRegs->regAddress);
//                regVal = *pReg64;
//                resetVal = pRegs->resetValue;
                break;
            default:
                return( REGTEST_ERROR );
        } 

         if( regVal != resetVal ) {        
            printDbg("ERROR READING: Register name: %0s \n\t\tregAddress: 0x%h\n\t\tresetVal(expected): 0x%h\n\t\tregVal(provided): 0x%h\n", pRegs->regName, pRegs->regAddress, resetVal, regVal);
            error ++;
       }

        pRegs ++;
        regIndex ++;
           
    }

    return( error );

} /* regtest_resetValues() */


/*==================== Function Separator =============================*/
uint32_t regtest_access( uint64_t baseAddress, REGTEST_t *pRegsTable )
{ 

    uint32_t regIndex;
    uint64_t regVal;
    uint32_t error;
    uint8_t  *pReg8;
    uint16_t *pReg16;
    uint32_t *pReg32;
    uint32_t *pReg64;
    uint32_t *pReg64Upper;
    uint32_t bitUpper;
    uint32_t bit;
    uint32_t regSize;
    uint64_t shiftBit = 0x1;
    uint32_t bitLen;
    REGTEST_t *pRegs;

    pRegs    = pRegsTable;
    regIndex = 0;
    error = 0;

    while( pRegs->regAddress != REGTEST_END_OF_LIST ) {
        switch( pRegs->regSize ) {
            case REGTEST_SIZE_8_BIT:
                pReg8      = (uint8_t *)(baseAddress + pRegs->regAddress);
                regSize = REGTEST_SIZE_8_BIT;
                bitLen = 8;
                break;
            case REGTEST_SIZE_16_BIT:
                pReg16      = (uint16_t *)(baseAddress + pRegs->regAddress);
                regSize = REGTEST_SIZE_16_BIT;
                bitLen = 16;
                break;
            case REGTEST_SIZE_32_BIT:
                pReg32      = (uint32_t *)(baseAddress + pRegs->regAddress);
                regSize = REGTEST_SIZE_32_BIT;
                bitLen = 32;
                break;
            case REGTEST_SIZE_64_BIT:
                pReg64 = (uint32_t *)(baseAddress + pRegs->regAddress);
                pReg64Upper = (uint32_t *)(baseAddress + pRegs->regAddress + 4);
                regSize = REGTEST_SIZE_64_BIT;
                bitLen = 64;
                break;
            default:
                return( REGTEST_ERROR );
        }

        

        if( pRegs->bitMask ) {
            for( bit = 0; bit < bitLen; bit ++ ) {
                if( pRegs->bitMask & (shiftBit << bit) ) {
                    /* Test Direct Access to main register */
                        switch( regSize ) {
                            case REGTEST_SIZE_8_BIT:
                                /* Write (set bit ) to register */
                                *pReg8 = (uint8_t) (shiftBit << bit);
                                /* Verify written data */
                                regVal = (uint32_t)(*pReg8) & pRegs->bitMask;
                                if( regVal != (shiftBit << bit) ) {
                                    printDbg(" ERROR WRITING 1: Register name: %0s \n Register offset: 0x%h\n 0x01 << bit: 0x%h\n Register value: 0x%h\n", pRegs->regName, pRegs->regAddress, (shiftBit << bit), regVal);
                                    error ++;
                                }
                                /* Write 0 to register */
                                *pReg8 = 0;
                                /* Verify written data */
                                regVal = (uint32_t)(*pReg8) & pRegs->bitMask;
                                if( regVal != 0 ) {
                                    printDbg(" ERROR WRITING 0: Register name: %0s \n Register offset: 0x%h\n 0x01 << bit: 0x%h\n Register value: 0x%h\n", pRegs->regName, pRegs->regAddress, 0, regVal);
                                    error ++;
                                }
                                break;
                            case REGTEST_SIZE_16_BIT:
                                /* Write ( set bit ) to register */
                                *pReg16 = (uint16_t) (shiftBit << bit);
                                /* Verify written data */
                                regVal = (uint32_t)(*pReg16) & pRegs->bitMask;
                                if( regVal != (shiftBit << bit) ) {
                                    printDbg(" ERROR WRITING 1: Register name: %0s \n Register offset: 0x%h\n 0x01 << bit: 0x%h\n Register value: 0x%h\n", pRegs->regName, pRegs->regAddress, (shiftBit << bit), regVal);
                                    error ++;
                                }
                                /* Write 0 to register */
                                *pReg16 = 0;
                                /* Verify written data */
                                regVal = (uint32_t)(*pReg16) & pRegs->bitMask;
                                if( regVal != 0 ) {
                                    printDbg(" ERROR WRITING 0: Register name: %0s \n Register offset: 0x%h\n 0x01 << bit: 0x%h\n Register value: 0x%h\n", pRegs->regName, pRegs->regAddress, 0, regVal);
                                    error ++;
                                }
                                break;
                            case REGTEST_SIZE_32_BIT:
                                /* Write ( set bit ) to register */
                                *pReg32 = (uint32_t) (shiftBit << bit);
                                /* Verify written data */
                                regVal = (uint32_t)(*pReg32) & pRegs->bitMask;
                                if( regVal != (shiftBit << bit) ) {
                                    printDbg(" ERROR WRITING 1: Register name: %0s \n Register offset: 0x%h\n 0x01 << bit: 0x%h\n Register value: 0x%h\n", pRegs->regName, pRegs->regAddress, (shiftBit << bit), regVal);
                                    error ++;
                                }
                                /* Write 0 to register */
                                *pReg32 = 0;
                                /* Verify written data */
                                regVal = (uint32_t)(*pReg32) & pRegs->bitMask;
                                if( regVal != 0  ) {
                                    printDbg(" ERROR WRITING 0:Register name: %0s \n Register offset: 0x%h\n 0x01 << bit: 0x%h\n Register value: 0x%h\n", pRegs->regName, pRegs->regAddress, 0, regVal);                                
                                    error ++;
                                }
                                break;
                            case REGTEST_SIZE_64_BIT:
                                /* Write ( set bit ) to register */                                
                                if( bit > 31) {
                                  bitUpper = (bit % 32);
                                  *pReg64Upper = (uint32_t) (shiftBit << bitUpper);
                                  *pReg64 = 0; 
                                }
                                else {
                                  *pReg64 = (uint32_t) (shiftBit << bit);   
                                  *pReg64Upper = 0;                              
                                }
 
                                /* Verify written data */
                                regVal = *pReg64Upper;
                                regVal = (regVal << 32);
                                regVal = regVal | (*pReg64); 
                                regVal = regVal & pRegs->bitMask;
                                if( regVal != (shiftBit << bit) ) {
                                    printDbg(" ERROR WRITING 1: Register name: %0s \n Register offset: 0x%h\n 0x01 << bit: 0x%h\n Register value: 0x%h\n", pRegs->regName, pRegs->regAddress, (shiftBit << bit), regVal);
                                    error ++;
                                }
                                /* Write 0 to register */
                                if( bit > 31) {
                                  bitUpper = (bit % 32);
                                  *pReg64Upper = 0;
                                }
                                else {
                                  *pReg64 = 0;                                 
                                }
                                
                                /* Verify written data */
                                regVal = *pReg64Upper;
                                regVal = (regVal << 32);
                                regVal = regVal | (*pReg64);                                
                                regVal = regVal & pRegs->bitMask;
                                if( regVal != 0 ) {
                                    printDbg(" ERROR WRITING 0: Register name: %0s \n Register offset: 0x%h\n 0x01 << bit: 0x%h\n Register value: 0x%h\n", pRegs->regName, pRegs->regAddress, 0, regVal);
                                    error ++;
                                }
                                break;
                        }
                }
            }
        }

        pRegs ++;
        regIndex ++;
           
    }

    return( error );

} /* regtest_access() */


/*==================== Function Separator =============================*/
uint32_t regtest_dump( uint64_t baseAddress, REGTEST_t *pRegsTable )
{ 

    uint32_t regIndex;
    uint64_t regVal;
    uint8_t  *pReg8;
    uint16_t *pReg16;
    uint32_t *pReg32;
    uint32_t *pReg64;
    uint32_t *pReg64Upper;
    
    REGTEST_t *pRegs;

    pRegs    = pRegsTable;
    regIndex = 0;

    while( pRegs->regAddress != REGTEST_END_OF_LIST ) {
        switch( pRegs->regSize ) {
            case REGTEST_SIZE_8_BIT:
                pReg8 = (uint8_t *)(baseAddress + pRegs->regAddress);
                regVal = *pReg8;
                break;
            case REGTEST_SIZE_16_BIT:
                pReg16 = (uint16_t *)(baseAddress + pRegs->regAddress);
                regVal = *pReg16;
                break;
            case REGTEST_SIZE_32_BIT:
                pReg32 = (uint32_t *)(baseAddress + pRegs->regAddress);
                regVal = *pReg32;
                break;
            case REGTEST_SIZE_64_BIT:
                pReg64 = (uint32_t *)(baseAddress + pRegs->regAddress);
                pReg64Upper = (uint32_t *)(baseAddress + pRegs->regAddress + 4);
                regVal = *pReg64Upper;
                regVal = (regVal << 32);
                regVal = regVal | (*pReg64);
           
//                pReg64 = (uint64_t *)(baseAddress + pRegs->regAddress);
//                regVal = *pReg64;
                break;
            default:
                return( REGTEST_ERROR );
        } 

        printDbg("Register name: %s \n Register offset: 0x%h \n Register value: 0x%h \n", pRegs->regName, pRegs->regAddress, regVal);
        
        pRegs ++;
        regIndex ++;
           
    }

    return( 0 );

} /* regtest_dump() */
 
 /*     <EOF>     */

