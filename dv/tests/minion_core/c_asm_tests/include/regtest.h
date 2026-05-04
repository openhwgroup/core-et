/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#define REGTEST_SIZE_8_BIT     1
#define REGTEST_SIZE_16_BIT    2
#define REGTEST_SIZE_32_BIT    4
#define REGTEST_SIZE_64_BIT    8

#define REGTEST_ERROR          0xFFFFFFFF

#define REGTEST_END_OF_LIST    0xEEEEEEEE

#define RESET_VALUE_NA 0xdeadbeef



typedef struct regtest
{
    uint32_t     regAddress;
    uint32_t     regSize;
    uint64_t     resetValue;
    uint64_t     bitMask;
    char         regName[70];
} REGTEST_t;


inline void __attribute__((always_inline)) regtest_checkResetValues(uintptr_t baseAddress, REGTEST_t* registerTable)
{

	uint64_t regVal64;
     
    uint32_t* pReg32;
    uint32_t* pReg32_upper;
    uint16_t* pReg16;
    uint8_t*  pReg8;

    REGTEST_t* pRegs = registerTable;

    while( pRegs->regAddress != REGTEST_END_OF_LIST) {
	/* Check whether we could read & compare register reset */
	if( pRegs->resetValue == RESET_VALUE_NA ) {
	    pRegs ++;
	    continue;
	}
	switch ( pRegs->regSize) {
	    case REGTEST_SIZE_8_BIT:
	        pReg8 = (uint8_t*) (baseAddress + pRegs->regAddress); 
	        if (*pReg8 != (pRegs->resetValue & 0xFF)) C_TEST_FAIL;
	        break;  
	    case REGTEST_SIZE_16_BIT:
	        pReg16 = (uint16_t*) (baseAddress + pRegs->regAddress); 
	        if (*pReg16 != (pRegs->resetValue & 0xFFFF)) C_TEST_FAIL;
	        break;  
	    case REGTEST_SIZE_32_BIT:
	        pReg32 = (uint32_t*) (baseAddress + pRegs->regAddress); 
	        if (*pReg32 != pRegs->resetValue ) C_TEST_FAIL;
	        break;  
	    case REGTEST_SIZE_64_BIT:
	        pReg32 = (uint32_t*) (baseAddress + pRegs->regAddress); 
	        pReg32_upper =  (uint32_t*) (baseAddress + pRegs->regAddress + 4); 
	        regVal64 = *pReg32_upper;
	        regVal64 <<= 32;
	        regVal64 |= *pReg32;
	        if (regVal64 != pRegs->resetValue ) C_TEST_FAIL;
	        break;  
	    default: C_TEST_FAIL; 
	} //end switch         
        pRegs++;
    } //end while
}