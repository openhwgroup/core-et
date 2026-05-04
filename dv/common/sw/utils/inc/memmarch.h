/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$
* @version $Release$
* @date $Date$
* @author 
*
* @brief memmarch.h Standard memmarch tests
*
*/


#ifndef __MEMMARCH_H
#define __MEMMARCH_H

#ifdef __cplusplus
extern
{
#endif


//void mm_printError( const char* str, uint64_t addr, uint64_t val, uint32_t mask);

uint32_t mm_march9seq8( uint64_t address, uint64_t size, uint8_t mask );

uint32_t mm_march9seq16(  uint64_t address, uint32_t size, uint16_t mask );

uint32_t mm_march9seq32(  uint64_t address, uint32_t size, uint32_t mask );

uint32_t mm_march9seq64(  uint64_t address, uint32_t size, uint64_t mask );

uint32_t mm_march9seq( uint64_t address, uint32_t sizeInBytes, uint8_t logDataWidth);

uint32_t mm_matsPlusseq8( uint64_t address, uint32_t size );

uint32_t mm_matsPlusseq16( uint64_t address, uint32_t size );

uint32_t mm_matsPlusseq32( uint64_t address, uint32_t size );

uint32_t mm_matsPlusseq64( uint64_t address, uint32_t size );

uint32_t mm_matsPlusseq( uint64_t address, uint32_t sizeInBytes, uint8_t logDataWidth);

uint32_t mm_marchCseq8( uint64_t address, uint32_t size );

uint32_t mm_marchCseq16( uint64_t address, uint32_t size );

uint32_t mm_marchCseq32( uint64_t address, uint32_t size );

uint32_t mm_marchCseq64( uint64_t address, uint32_t size );

uint32_t mm_marchCseq( uint64_t address, uint32_t sizeInBytes, uint8_t logDataWidth);


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      memmarch_march9
 *
 * @BRIEF         Memmory march 9 test
 *
 * @param[in]     startAddress -> start address of memory
 *                sizeInBytes -> size
 *                log2Banks ->number of banks
 *                dataWidth -> Data width in bytes
 *
 * @RETURNS        uint32_t -> Number of errors in memory module
 *
 * @DESCRIPTION   Memmory march 9 test
 *
 *//*------------------------------------------------------------------------ */
extern uint32_t memmarch_march9( uint64_t startAddress, uint32_t sizeInBytes,int log2Banks, uint32_t dataWidth  );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      memmarch_marchC
 *
 * @BRIEF         Memmory march C test
 *
 * @param[in]     startAddress -> start address of memory
 *                sizeInBytes -> size
 *
 * @RETURNS        uint32_t -> Number of errors in memory module
 *
 * @DESCRIPTION   Memmory march C test
 *
 *//*------------------------------------------------------------------------ */
extern uint32_t memmarch_marchC( uint64_t startAddress, uint32_t sizeInBytes, uint32_t dataWidth );


/* ------------------------------------------------------------------------*//**
 * @FUNCTION      memmarch_matsplus
 *
 * @BRIEF         Memmory MATS+ test
 *
 * @param[in]     startAddress -> start address of memory
 *                sizeInBytes -> size
 *
 * @RETURNS        uint32_t -> Number of errors in memory module
 *
 * @DESCRIPTION   Memmory MATS+ test
 *
 *//*------------------------------------------------------------------------ */
extern uint32_t memmarch_matsPlus( uint64_t startAddress, uint32_t sizeInBytes, uint32_t dataWidth );


#ifdef __cplusplus
}
#endif

#endif	/* __MEMMARCH_H */
