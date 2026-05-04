/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$
* @version $Release$
* @date $Date$
* @author 
*
* @brief tcMain.c Main function for all TCs
*
* Setup SoC to enable TC run
*/

/**
 *  @Component    ET
 *
 *  @Filename     et.h
 *
 *  @Description  Contains Defines used globally
 *                error codes
 *                typedef
 *
 *//*======================================================================== */

#ifndef __ET_H
#define __ET_H



#ifdef __cplusplus
extern "C"
{
#endif


/*=============================================================================
 * EXPORTED DEFINITIONS
 *=============================================================================*/

#define         ET_PASS              0
#define         ET_OK                0
#define         ET_FAIL              0xF


/*=============================================================================
 * EXPORTED TYPES
 *=============================================================================*/

typedef  uint32_t            et_status_t;

typedef void                 *et_handle_t;


/*=============================================================================
 * EXPORTED VARIABLES
 *=============================================================================*/



/*=============================================================================
 * LOCAL  FUNCTIONS/MACROS
 *=============================================================================*/
#define __setBitMask( pos ) ( ((1u << (pos)) - 1) | (1u << (pos)) )

#define __getMask( range )  (  __setBitMask((range##__SHIFT) + (range##__WIDTH) - 1 ) &  \
                               ~(__setBitMask(range##__SHIFT) >> 1) )
 

#define ___getMask( range ) (  __setBitMask((range##_LSB) + (range##_WIDTH) - 1 ) &  \
                               ~(__setBitMask(range##_LSB) >> 1) )


#define mretToSmode __asm__ __volatile__ \
                    ("la t0, 1f\n"    \
                    "csrw mepc, t0\n" \
                    "mret\n"          \
                    "1:\n"            \
                    ::: "t0")         \
/*=============================================================================
 * EXPORTED FUNCTIONS/MACROS
 *=============================================================================*/

#define setBitField( var, regBitField, value ) ((var) = ((var) & ~__getMask(regBitField)) |  \
                                               ( ((value) << (regBitField##__SHIFT)) & __getMask(regBitField)) )

#define getBitField( var, regBitField )        ( ((var) & __getMask(regBitField)) >> (regBitField##__SHIFT) ) 


#define set_bitField( var, regBitField, value ) ((var) = ((var) & ~___getMask(regBitField)) |  \
                                               ( ((value) << (regBitField##_LSB)) & ___getMask(regBitField)) )

#define get_bitField( var, regBitField )        ( ((var) & ___getMask(regBitField)) >> (regBitField##_LSB) ) 

 


#ifdef __cplusplus
}
#endif

#endif    /* __ET_H */



/*****     <EOF>     *****/
