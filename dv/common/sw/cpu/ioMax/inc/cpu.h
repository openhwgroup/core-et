/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/**
* @file $Id$
* @version $Release$
* @date $Date$
* @author
*
* @brief cpu.h Cpu specific includes
*
*
*/

#ifndef __CPU_H
#define __CPU_H

#ifdef __cplusplus
extern
{
#endif


/* =============================================================================
 * Include files
 * =============================================================================
 */

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "print.h"

#include "et.h"

// #include "inth.h"

/* =============================================================================
 * EXPORTED DEFINITIONS
 * =============================================================================
 */

/*-------------------------------------------------------------------------*//**
 * @DEFINITION   CPU
 *
 * @BRIEF        CPU Define
 *
 * @DESCRIPTION  This define is used To distinguish CPU's in common code and
 *
 *//*------------------------------------------------------------------------ */
#ifndef CPU
  #define CPU     0
#endif


#define REGNAME(x) #x
#define CSR(x) REGNAME(x)


/*-------------------------------------------------------------------------*//**
 * @DEFINITION   CSRregRead
 *
 * @BRIEF        Read CSR register
 *
 * @DESCRIPTION  This macro is used to read CSR register.
 *               reg is name from Minion  CSR Register table
 *
 *//*------------------------------------------------------------------------ */
#define CSRregRead( reg ) (        \
{                                  \
    uint64_t ret;                  \
    __asm__ __volatile(            \
        "csrr %0, " CSR(reg) "\n"  \
        :"=r"(ret)                 \
        :                          \
        :);                        \
    ret;                           \
}                                  \
)

/*-------------------------------------------------------------------------*//**
 * @DEFINITION   CSRregWrite
 *
 * @BRIEF        Write CSR register
 *
 * @DESCRIPTION  This macro is used to write  val to CSR register.
 *               reg is name from Minion  CSR Register table
 *
 *//*------------------------------------------------------------------------ */
#define CSRregWrite( reg, val ) (  \
{                                  \
    __asm__ __volatile(            \
        "csrw " CSR(reg) ", %0\n"  \
        :                          \
        : "r"(val)                 \
        :);                        \
}                                  \
)





/* =============================================================================
 * EXPORTED TYPES
 * =============================================================================
 */



/* =============================================================================
 * EXPORTED VARIABLES
 * =============================================================================
 */



 /* =============================================================================
 * EXPORTED FUNCTIONS
 * =============================================================================
 */



#ifdef __cplusplus
}
#endif

#endif	/* __CPU_H */


/*****     <EOF>     *****/
