/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SELFCHK_TFMA_H_
#define _ET_SELFCHK_TFMA_H_

#include "selfchecking_common.h"

#define TFMA32    0
#define TFMA16A32 1
#define TIMA8A32  3

#define sext(t1) (int32_t) (( int8_t) (t1))
#define zext(t1) (int32_t) ((uint8_t) (t1))

#define FMADD_S(C, C0, A, B) \
            __asm__ volatile("fmadd.s %[FC], %[FA], %[FB], %[FC0], dyn" \
               : [FC]  "=f" (C)     \
               : [FA]  "f"  (A),    \
                 [FB]  "f"  (B),    \
                 [FC0] "f"  (C0)   \
               : /* No clobbers */ \
            );

#define FMUL_S(C, A, B) \
            __asm__ volatile("fmul.s %[FC], %[FA], %[FB], dyn" \
               : [FC]  "=f" (C)     \
               : [FA]  "f"  (A),    \
                 [FB]  "f"  (B)     \
               : /* No clobbers */ \
            );

#include "../../softfloat/fpu.h"

extern "C" int tfma_selfchecking(tensor_data* snippet_info);

#endif
