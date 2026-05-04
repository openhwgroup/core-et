/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "et_barrier.h"

#include "kernel.h"

#ifndef LOOP_SIZE
#define LOOP_SIZE 1024*5
#endif

#ifndef INNER_LOOP_SIZE
#define INNER_LOOP_SIZE 20
#endif

#ifndef SKIP_CHECK
#define SKIP_CHECK 0
#endif

#ifndef PING_PING_LOOP_SIZE
#define PING_PING_LOOP_SIZE 8192
#endif

#ifndef IDLE_DELAY
#define IDLE_DELAY 1024*512
#endif

#define PASS 0

int main() {
   uint64_t res = PASS;
   for (int i = 0; i < PING_PING_LOOP_SIZE; i++) {
      res = vpu_ps_power_virus(LOOP_SIZE, INNER_LOOP_SIZE, (bool)SKIP_CHECK);
      if (res != PASS) break;
      soc_fcc_barrier_with_delay(1, IDLE_DELAY);
   }
   if (res == 0) { C_TEST_PASS; }
   else { C_TEST_FAIL; }
   return 0;
}
