/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SELFCHK_TL_H_
#define _ET_SELFCHK_TL_H_

#include "selfchecking_common.h"

#define TLOAD_TENA  0
#define TLOAD_TENB  0
#define TLOAD_INT8  1
#define TLOAD_INT16 2
#define TLOAD_TRA8  5
#define TLOAD_TRA16 6
#define TLOAD_TRA32 7

#define CHECK 1
#define EXECUTE 2
#define TL_ACTION_WRAPPER(ACTION, RTL_DATA, GOLDEN_DATA) \
   if ((ACTION) == CHECK) { \
      if ((RTL_DATA) != (GOLDEN_DATA)) return NOT_PASS; \
   } \
   else if ((ACTION) == EXECUTE) { \
      (RTL_DATA) = (GOLDEN_DATA); \
   }\
   else { \
      return NOT_PASS; \
   } \

extern "C" int tl_selfchecking(tensor_data* snippet_info);

#endif
