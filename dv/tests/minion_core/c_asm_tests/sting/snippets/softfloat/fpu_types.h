/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _FPU_TYPES_H
#define _FPU_TYPES_H

#include "platform.h"
#include "softfloat_types.h"

// Arithmetic types
typedef struct { uint16_t v; } float11_t;
typedef struct { uint16_t v; } float10_t;

#define isSubnormalF11UI( a ) ((((a) & 0x7C0) == 0) && ((a) & 0x03F))
#define softfloat_zeroExpSigF11UI( a ) (0)

#define isSubnormalF10UI( a ) ((((a) & 0x3E0) == 0) && ((a) & 0x01F))
#define softfloat_zeroExpSigF10UI( a ) (0)

#endif // _FPU_TYPES_H
