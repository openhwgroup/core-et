/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file regtest_comprehensive.h
* @version $Release$ 
* @date $Date$
* @author 
*
* @brief 
*
* Setup SoC to enable TC run 
*/ 
/** 
 *  @Component      REG_TEST_COMPREHENSIVE
 *
 *  @Filename       reg_test_comprehensive.h
 *
 *  @Description    The REG_TEST_COMPREHENSIVEcomponent cotains code for Register Tests
 *
 *//*======================================================================== */

#ifndef __REG_TEST_H
#define __REG_TEST_H

#ifdef __cplusplus
extern "C"
{
#endif

/* =============================================================================
 * EXPORTED DEFINITIONS
 * =============================================================================
 */ 
#define REGTEST_SIZE_8_BIT     1
#define REGTEST_SIZE_16_BIT    2
#define REGTEST_SIZE_32_BIT    4
#define REGTEST_SIZE_64_BIT    8

#define REGTEST_ERROR          0xFFFFFFFF

#define REGTEST_END_OF_LIST    0xEEEEEEEE

#define RESET_VALUE_NA 0xdeadbeef



/* =============================================================================
 * EXPORTED TYPES
 * =============================================================================
 */


/*-------------------------------------------------------------------------*//**
 * @TYPE         REGTEST_COMPREHENSIVE_t
 *
 * @BRIEF        REG Test structure block
 *
 * @DESCRIPTION  struct which defines registers test specific stuff
 *
 *//*------------------------------------------------------------------------ */ 
 typedef struct regtest
{
    uint32_t     regAddress;
    uint32_t     regSize;
    uint64_t     resetValue;
    uint64_t     bitMask;
    char         regName[70];
    uint8_t      skip_reset_check;
    uint8_t      skip_rw_check;
} REGTEST_COMPREHENSIVE_t;



/* =============================================================================
 * EXPORTED VARIABLES
 * =============================================================================
 */ 
 
 
 /* =============================================================================
 * EXPORTED FUNCTIONS
 * =============================================================================
 */ 
extern uint32_t regtest_comprehensive_resetValues( uint64_t baseAddress, REGTEST_COMPREHENSIVE_t *pRegsTable);
extern uint32_t regtest_comprehensive_access( uint64_t baseAddress, REGTEST_COMPREHENSIVE_t *pRegsTables );
extern uint32_t regtest_comprehensive_dump( uint64_t baseAddress, REGTEST_COMPREHENSIVE_t *pRegsTables );



#ifdef __cplusplus
}
#endif

#endif	/* __REG_TEST_H */


/*     <EOF>     */

