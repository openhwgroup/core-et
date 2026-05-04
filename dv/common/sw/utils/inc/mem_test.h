/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file mem_test.h
* @version 1.2
* @date 01/03/2019
* @author HDL Design House
*
* @brief mem_test.h Standard mem test
*
*/

#ifndef __MEM_TEST_H
#define __MEM_TEST_H

#ifdef __cplusplus
extern
{
#endif


#define MEMTEST_8_BIT_ACCESS       0x00000001
#define MEMTEST_16_BIT_ACCESS      0x00000002
#define MEMTEST_32_BIT_ACCESS      0x00000004
#define MEMTEST_64_BIT_ACCESS      0x00000008

#define MEMTEST_PATTERN_CONST      0x00000001
#define MEMTEST_PATTERN_RAMP_UP    0x00000002
#define MEMTEST_PATTERN_RAMP_DOWN  0x00000004
#define MEMTEST_PATTERN_RANDOM     0x00000008

  
typedef struct memtest_params 
{
    uint64_t startAddress;
    uint64_t size;
    uint32_t busDataWidth;
    uint32_t numberOfAccesses;
    uint32_t patternType;
    uint32_t pattern;
    char* mem_name;
    
} MEMTEST_PARAMS_t;




/*==================== Function Separator =============================*/
extern uint32_t memtest_dataRoll( MEMTEST_PARAMS_t *pMtParam, uint32_t exitOnFirstError );


/*==================== Function Separator =============================*/
extern uint32_t memtest_variableWidthAccess( MEMTEST_PARAMS_t *pMtParam, uint32_t exitOnFirstError );


/*==================== Function Separator =============================*/
extern uint32_t memtest_addressRange( MEMTEST_PARAMS_t *pMtParam, uint32_t exitOnFirstError, uint32_t checkOnly );

/*==================== Function Separator =============================*/
extern uint32_t memtest_Endian( MEMTEST_PARAMS_t *pMtParam, uint32_t exitOnFirstError );

/*==================== Function Separator =============================*/
extern void memtest_copyStruct( MEMTEST_PARAMS_t *src, MEMTEST_PARAMS_t *dst );



#ifdef __cplusplus
}
#endif

#endif	
/* __MEM_TEST_H */

/*     < EOF >     */

