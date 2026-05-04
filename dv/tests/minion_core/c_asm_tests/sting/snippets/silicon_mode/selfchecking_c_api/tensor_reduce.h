/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SELFCHK_TR_H_
#define _ET_SELFCHK_TR_H_

#include "selfchecking_common.h"

#define TR_SND   0b00
#define TR_RCV   0b01
#define TR_BCAST 0b10
#define TR_AUTO  0b11

#define FADD 0x0ULL
#define FSUB 0x1ULL
#define FMAX 0x2ULL
#define FMIN 0x3ULL
#define IADD 0x4ULL
#define ISUB 0x5ULL
#define IMAX 0x6ULL
#define IMIN 0x7ULL
#define FGET 0x8ULL

extern "C" int tr_selfchecking(tensor_data* snippet_info);

#endif
