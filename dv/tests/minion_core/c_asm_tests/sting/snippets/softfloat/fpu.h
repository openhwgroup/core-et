/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef BEMU_FPU_H
#define BEMU_FPU_H

#include "platform.h"
#include "softfloat.h"
#include "fpu_types.h"

// ---------------------------------------------------------------------------
// Esperanto additions to the softfloat floating-point operations.  Similarly
// to softfloat these are added to the global namespace and then wrapped by
// functions with the same name in the fpu namespace.
// ---------------------------------------------------------------------------

float32_t f1632_mulAdd2(float16_t, float16_t, float16_t, float16_t);
float32_t f1632_mulAdd3(float16_t, float16_t, float16_t, float16_t, float32_t);

namespace fpu {

using ::f1632_mulAdd2;
using ::f1632_mulAdd3;

} // namespace fpu

#endif // BEMU_FPU_H
