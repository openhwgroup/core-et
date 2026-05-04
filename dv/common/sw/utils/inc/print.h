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
 *  @Component      Print
 *
 *  @Filename       print.h
 *
 *  @Description    print Macro definitions
 *
 *//*======================================================================== */

#ifndef __PRINT_H
#define __PRINT_H

#ifdef __cplusplus
extern "C"
{
#endif

extern int tb_printf(char*, ...); 

// RR: Fix mess
// printDbg Macro


#ifndef NO_PRINT
    #define printDbg(...)            \
        tb_printf( __VA_ARGS__ )
#else
    #define printDbg(...)
#endif


#ifdef __cplusplus
}
#endif

#endif	/* __PRINT_H */


/*     <EOF>     */
