/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "perf_rq.h"
using namespace RQTest;
int main(){
  C_TEST_START;
  int32_t minion = get_minion_id();
  if (minion != 0) { C_TEST_PASS; return 0;}
  asm ("csrwi validation3, 0");

  
  TestRunner <LoadHit,
              LoadMiss,
              UCStore,
              UCLoad,
              LoadMissSec,
              Idle> :: run();
 
 C_TEST_PASS;
}
