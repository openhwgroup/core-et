/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"

#include "kernel.h"

#ifndef LOOP_SIZE
#define LOOP_SIZE 100*1024*1024
#endif

#ifndef INNER_LOOP_SIZE
#define INNER_LOOP_SIZE 20
#endif

#ifndef SKIP_CHECK
#define SKIP_CHECK 0
#endif

#define PASS 0

int main() {
   uint64_t res = vpu_ps_power_virus(LOOP_SIZE, INNER_LOOP_SIZE, (bool)SKIP_CHECK);
   if (res == PASS) { C_TEST_PASS; }
   else { C_TEST_FAIL; }
   return 0;
}
