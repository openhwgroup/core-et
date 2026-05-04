/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file mem_test.c
* @version 1.3
* @date 01/11/2019
* @author HDL Design House
*
* @brief mem_test.c Standard memtest
*
*/

#include "cpu.h"
#include "soc.h"
#include "mem_test.h"

/*==================== Function Separator =============================*/
void memtest_copyStruct( MEMTEST_PARAMS_t *src, MEMTEST_PARAMS_t *dst )
{

    dst->startAddress     = src->startAddress;
    dst->size             = src->size;
    dst->busDataWidth     = src->busDataWidth;
    dst->numberOfAccesses = src->numberOfAccesses;
    dst->patternType      = src->patternType;
    dst->pattern          = src->pattern;
    dst->mem_name         = src->mem_name;

} /* memtest_copyStruct() */



/*==================== Function Separator =============================*/
uint32_t memtest_dataRoll( MEMTEST_PARAMS_t *pMtParam, uint32_t exitOnFirstError )
{

    uint64_t *addr64;
    uint64_t patt;
    uint32_t size;
    uint32_t i;
    uint32_t error;
    uint64_t rd64;

    patt  = (uint64_t)( pMtParam -> pattern );
    size  = (pMtParam -> busDataWidth) / 64;
    error = 0;

    for( i = 0; i < size; i++ ){
       addr64  = (uint64_t *)(pMtParam->startAddress) + 8 * i;
       *addr64 = patt;
       rd64    = (uint64_t) * addr64;
           if( rd64 != patt ){
                error ++;
                if( exitOnFirstError )
                     return error;
           }
     }

     return error;

} /* memtest_dataTest() */


/*==================== Function Separator =============================*/
uint32_t memtest_variableWidthAccess( MEMTEST_PARAMS_t *pMtParam, uint32_t exitOnFirstError )
{

    uint8_t  *addr8;
    uint16_t *addr16;
    uint32_t *addr32;
    uint64_t *addr64;
    uint32_t i;
    uint32_t error;
    uint8_t  rd8;
    uint16_t rd16;
    uint32_t rd32;
    uint64_t rd64;
    uint8_t  val8;
    uint16_t val16;
    uint32_t val32;
    uint64_t val64;

    /* MaxBootRam does not support sub-32b writes */
    uint32_t MaxBootId =(uint32_t)( (( pMtParam->startAddress >= (uint64_t)R_MX_ROM_BASEADDR ) && ( pMtParam->startAddress < ((uint64_t)R_MX_ROM_BASEADDR + (uint64_t)R_MX_ROM_SIZE))));

    /* Start address calculation for variable Size test */
    /* Addresses in use are concatanated */
    /* numberOfAccesses is max width count */
    if ( MaxBootId ){
         addr32 = (uint32_t *)pMtParam->startAddress;
         addr64 = (uint64_t *)(((uint32_t)addr32 + pMtParam->numberOfAccesses * 2 + 0x8) & ~0x03 );
    }
    else{
        addr8  = (uint8_t  *)pMtParam->startAddress;
        addr16 = (uint16_t *)((uint64_t)addr8  + pMtParam->numberOfAccesses * 8 );
        addr32 = (uint32_t *)((uint64_t)addr16 + pMtParam->numberOfAccesses * 4 );
        addr64 = (uint64_t *)(((uint64_t)addr32 + pMtParam->numberOfAccesses * 2 + 0x8) & ~0x03 );
    }

    error  = 0;

   if ( !MaxBootId ){
       /* Check 8 Bit Access */
       /* Write Data */
       val8 = (uint8_t)pMtParam->pattern;
       for( i = 0; i < pMtParam->numberOfAccesses * 8; i ++ ){
          switch( pMtParam->patternType ){
            case MEMTEST_PATTERN_RAMP_UP:
                val8 += 1;
                break;
            case MEMTEST_PATTERN_RAMP_DOWN:
                val8 -= 1;
                break;
          }
          addr8[i] = val8;
       }

       /* Read & Verify Data */
       val8 = (uint8_t)pMtParam->pattern;
       for( i = 0; i < pMtParam->numberOfAccesses * 8; i ++ ){
          switch( pMtParam->patternType ){
            case MEMTEST_PATTERN_RAMP_UP:
                val8 += 1;
                break;
            case MEMTEST_PATTERN_RAMP_DOWN:
                val8 -= 1;
                break;
          }
          rd8 = addr8[i];
          if( rd8 != val8 ){
            error ++;
            if( exitOnFirstError )
                return error;
          }
       }

       /* Check 16 Bit Access */
       /* Write Data */
       val16 = (uint16_t)pMtParam->pattern;
       for( i = 0; i < pMtParam->numberOfAccesses * 4; i ++ ){
          switch( pMtParam->patternType ){
            case MEMTEST_PATTERN_RAMP_UP:
                val16 += 1;
                break;
            case MEMTEST_PATTERN_RAMP_DOWN:
                val16 -= 1;
                break;
          }
          addr16[i] = val16;
       }

       /* Read & Verify Data */
       val16 = (uint16_t)pMtParam->pattern;
       for( i = 0; i < pMtParam->numberOfAccesses * 4; i ++ ){
          switch( pMtParam->patternType ){
            case MEMTEST_PATTERN_RAMP_UP:
                val16 += 1;
                break;
            case MEMTEST_PATTERN_RAMP_DOWN:
                val16 -= 1;
                break;
          }
          rd16 = addr16[i];
          if( rd16 != val16 ){
            error ++;
            if( exitOnFirstError )
                return error;
          }
       }
    }

    /* Check 32 Bit Access */
    /* Write Data */
    val32 = (uint32_t)pMtParam->pattern;
    for( i = 0; i < pMtParam->numberOfAccesses * 2; i ++ ){
        switch( pMtParam->patternType ){
            case MEMTEST_PATTERN_RAMP_UP:
                val32 += 1;
                break;
            case MEMTEST_PATTERN_RAMP_DOWN:
                val32 -= 1;
                break;
        }
        addr32[i] = val32;
    }

    /* Read & Verify Data */
    val32 = (uint32_t)pMtParam->pattern;
    for( i = 0; i < pMtParam->numberOfAccesses * 2; i ++ ){
        switch( pMtParam->patternType ){
            case MEMTEST_PATTERN_RAMP_UP:
                val32 += 1;
                break;
            case MEMTEST_PATTERN_RAMP_DOWN:
                val32 -= 1;
                break;
        }
        rd32 = addr32[i];
        if( rd32 != val32 ){
            error ++;
            if( exitOnFirstError )
	            return error;
        }
    }

    /* Check 64 Bit Access */
    /* Write Data */
    val64 = (uint64_t)pMtParam->pattern;
    for( i = 0; i < pMtParam->numberOfAccesses; i ++ ){
        switch( pMtParam->patternType )
	{
            case MEMTEST_PATTERN_RAMP_UP:
                val64 += 1;
                break;
            case MEMTEST_PATTERN_RAMP_DOWN:
                val64 -= 1;
                break;
        }
        addr64[i] = val64;
   }

    /* Read & Verify Data */
    val64 = (uint64_t)pMtParam->pattern;
    for( i = 0; i < pMtParam->numberOfAccesses; i ++ ){
        switch( pMtParam->patternType ){
            case MEMTEST_PATTERN_RAMP_UP:
                val64 += 1;
                break;
            case MEMTEST_PATTERN_RAMP_DOWN:
                val64 -= 1;
                break;
        }
        rd64 = addr64[i];
        if( rd64 != val64 ){
            error ++;
            if( exitOnFirstError )
                return error;
        }
    }

    return error;

} /* memtest_variableWidthAccess() */


/*==================== Function Separator =============================*/
uint32_t memtest_addressRange( MEMTEST_PARAMS_t *pMtParam, uint32_t exitOnFirstError, uint32_t checkOnly )
{

    uint64_t *addr64;
    uint64_t startAddr;
    uint64_t stopAddr;
    uint64_t addr;
    uint32_t noAddrBits;
    uint32_t countVal;
    uint32_t pattern;
    uint32_t pattInc;
    uint32_t i, j;
    uint32_t error;
    uint64_t rd64;

    /* Start address calculation for variable Size test */
    startAddr = pMtParam->startAddress;
    stopAddr  = startAddr + pMtParam->size - 1;

    printDbg( "AR:  startAddr: [63:32] 0x%h [31:0] 0x%h  StopAddr: [63:32] 0x%h [31:0] 0x%h\n", startAddr>>32, startAddr, stopAddr>>32, stopAddr );

    /* Calculate number of address bits */
    noAddrBits = 0;
    addr = pMtParam->size;

    while( addr){
        noAddrBits ++;
        addr = (addr >> 1);
    }

    /* Use only lower 24 bits of pattern */
    pattern = pMtParam->pattern & 0x00FFFFFF;

    /* Calculate pattInc */
    switch( pMtParam->patternType ){
        case MEMTEST_PATTERN_RAMP_UP:
            pattInc = 1;
            break;
        case MEMTEST_PATTERN_RAMP_DOWN:
            pattInc = -1;
            break;
        default:
            pattInc = 0;
    }

    addr  = 0;
    error = 0;
    countVal = 4;

    /* Write Sequence */
    if( checkOnly == 0 ) {
        for( i = 5; i < noAddrBits; i ++ ) {
            addr = startAddr + 2*( (uint64_t)0x1 << (uint64_t)(i - 1) );
            if( addr > stopAddr )
                addr = stopAddr + 1 - (countVal * sizeof(uint64_t));

            addr64 = (uint64_t *)addr;
            printDbg( "AR:  WrAddr: 0x%h\n", addr64 );
            for( j = 0; j < countVal; j++ ){
                addr64[j] = (uint64_t)(( i << 24 ) + pattern + pattInc);
            }
        }
    }

    /* Read & Check Sequence */
    for( i = 5; i < noAddrBits; i ++ ) {
        addr = startAddr + 2*((uint64_t)0x1 << (uint64_t)(i - 1) );
        if( addr > stopAddr )
            addr = stopAddr + 1 - (countVal * sizeof(uint64_t));

        addr64 = (uint64_t *)addr;
        printDbg( "AR:  RdAddr: 0x%h\n", addr64 );

        for( j = 0; j < countVal; j++ ){
	        rd64 =  addr64[j];
            if( rd64 != (uint64_t)(( i << 24 ) + pattern + pattInc) ){
                error ++;
                if( exitOnFirstError )
                    return error;
            }
        }
    }

    return error;

} /* memtest_addressRange() */


/*==================== Function Separator =============================*/
uint32_t memtest_Endian( MEMTEST_PARAMS_t *pMtParam, uint32_t exitOnFirstError )
{
    uint64_t addrE64;
    uint32_t error;
    uint32_t pattInc;
    uint32_t pattern;
    uint32_t rd32;
    uint64_t rd64;
    uint32_t val32;
    uint64_t val64;
    uint8_t  tmp8;
    uint16_t tmp16;
    uint32_t tmpE32;
    uint64_t tmpE64;
    uint8_t  *ptAddr8;
    uint32_t *ptAddr32;
    uint64_t *ptAddr64;
    uint32_t i;
    int j;


    switch( pMtParam->patternType ){
        case MEMTEST_PATTERN_RAMP_UP:
            pattInc = 1;
            break;
        case MEMTEST_PATTERN_RAMP_DOWN:
            pattInc = -1;
            break;
        default:
            pattInc = 0;
    }

    error = 0;
    rd32  = pMtParam->pattern + pattInc;
    rd64  =(uint64_t)(pMtParam->pattern + pattInc ) * 0x11111111;

    /* read endian 32 */
    for( i = 0; i < pMtParam->numberOfAccesses; i += 4 ){
      val32 = 0;
      ptAddr32  = (uint32_t *)((pMtParam->startAddress) + i );
      *ptAddr32 = (uint32_t)( pMtParam->pattern + pattInc );
      addrE64   = ((pMtParam->startAddress) + i );

      /*read endian 4x8*/
      for( j = 3; j >= 0; j-- ){
          ptAddr8 = (uint8_t *)( addrE64 + j );
          tmp8    =  (uint8_t)( *ptAddr8 );
          tmpE32  = (uint32_t) tmp8;
          tmpE32 <<= ( 8*j );
          val32 |= tmpE32;
      }

      if( rd32 != val32 ){
             if( exitOnFirstError )
                  return error;
      }

      /*read endian 2x16*/
      val32 = 0;
      for(j = 3; j >= 0 ; j -= 2 ){
          ptAddr8 = (uint8_t *)( addrE64 + j );
          tmp16   = (uint16_t)( *ptAddr8 );
          tmp16 <<= 8;
          tmp16 |= *( ptAddr8 - 1 );
          tmpE32 = (uint32_t)tmp16;
          tmpE32 <<= ( 8 * ( j - 1 ));
          val32  |= tmpE32;
      }
      if( rd32 != val32 ){
           error ++;
             if( exitOnFirstError )
                return error;
      }
    }

    /* read endian 64 */
    for( i = 0; i < pMtParam -> numberOfAccesses; i += 8 ){
      val64 = 0;
      ptAddr64  = (uint64_t *)((pMtParam->startAddress) + i );
      *ptAddr64 = (uint64_t)(pMtParam->pattern + pattInc ) * 0x11111111;
      addrE64   = ((pMtParam->startAddress) + i );

      /*read endian 8x8 */
      for( j = 7; j >= 0; j-- ){
          ptAddr8 =(uint8_t *)( addrE64 + j );
          tmp8    = (uint8_t)( *ptAddr8 );
          tmpE64  = (uint64_t)tmp8;
          tmpE64 <<= ( 8 * j );
          val64  |= tmpE64;
      }
      if( rd64 != val64 ){
            error ++;
              if( exitOnFirstError )
                  return error;
      }

      /*read endian 4x16*/
      val64 = 0;
      for( j = 7; j >= 0 ; j -= 2 ){
          ptAddr8 = (uint8_t *)( addrE64 + j );
          tmp16   = (uint16_t)( *ptAddr8 );
          tmp16 <<= 8;
          tmp16  |= *( ptAddr8 - 1 );
          tmpE64  = (uint64_t)tmp16;
          tmpE64 <<= ( 8 * ( j -  1 ) );
          val64  |=tmpE64;
      }
      if( rd64 != val64 ){
           error ++;
             if( exitOnFirstError )
                  return error;
      }
    }

     return error;

}/*  memtest_Endian*/

/*     < EOF >     */

