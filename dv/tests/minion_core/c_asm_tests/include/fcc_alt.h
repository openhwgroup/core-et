/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef __FCC_ALT_H
#define __FCC_ALT_H

#include "esr.h"
#include "macros.h"

#ifndef NUM_SHIRES
#define NUM_SHIRES 33
#endif

#ifndef MIN_PER_SHIRE
#define MIN_PER_SHIRE 32
#endif

#ifndef THREADS_PER_MIN
#define THREADS_PER_MIN 2
#endif

#define CREDIT_COUNTER_PER_HART 2
#define CREDINC_BASE 0x18

//-------------------------------------------------------------------------------------------------
//
// FUNCTION: give_credit
//
//   This function is used to increment the credit counter of specified harts.
//
//   shire_id           : Specifies the shire ID of the targeted HART, or all shires if negative
//   minion_id          : Specifies the minion number of the targeted HART inside its shire,
//                        or all Minions if negative
//   thread_id          : Specifies the thread number of the targeted HART inside the Minion,
//                        or all threads if negative
//   credit_counter_num : The credit counter to increment (each HART has 2)
//
inline int8_t __attribute__((always_inline)) give_credit(int shire_id, int minion_id, int thread_id, int credit_counter_num) {
   uint64_t shire_mask;
   uint64_t minion_mask;
   uint64_t thread_mask;

   // Check that inputs are valid, otherwise jump to fail
   if ((shire_id           > NUM_SHIRES             ) ||
       (minion_id          > MIN_PER_SHIRE          ) ||
       (thread_id          > THREADS_PER_MIN        ) ||
       (credit_counter_num > CREDIT_COUNTER_PER_HART))
   {
      return -1;
   }

   shire_mask  = (shire_id  < 0) ? 0xFFFFFFFFULL         :(uint64_t)1 << shire_id;
   minion_mask = (minion_id < 0) ? 0xFFFFFFFFFFFFFFFFULL :(uint64_t)1 << minion_id;
   thread_mask = (thread_id < 0) ? 0x3                   :(uint64_t)1 << thread_id;

   for (uint64_t s = 0; s < NUM_SHIRES; ++s) {
      if ((shire_mask >> s) & 1) {
         for (uint64_t t = 0; t < THREADS_PER_MIN; ++t) {
            if ((thread_mask >> t) & 1) {
               write_esr(PP_USER, s, REGION_OTHER, (1 << 15) | CREDINC_BASE | (t << 1) | credit_counter_num, minion_mask);
            }
         }
      }
   }
   return 0;
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: give_credit_local
//
//   This function is used to increment the credit counter of specified harts in the local shire
//
//   minion_id          : Specifies the minion number of the targeted HART inside its shire,
//                        or all Minions if negative
//   thread_id          : Specifies the thread number of the targeted HART inside the Minion,
//                        or all threads if negative
//   credit_counter_num : The credit counter to increment (each HART has 2)
//
inline int8_t __attribute__((always_inline)) give_credit_local(int minion_id, int thread_id, int credit_counter_num) {
   uint64_t minion_mask;
   uint64_t thread_mask;

   // Check that inputs are valid, otherwise jump to fail
   if ((minion_id          > MIN_PER_SHIRE          ) ||
       (thread_id          > THREADS_PER_MIN        ) ||
       (credit_counter_num > CREDIT_COUNTER_PER_HART))
   {
      return -1;
   }

   minion_mask = (minion_id < 0) ? 0xFFFFFFFFFFFFFFFFULL :(uint64_t)1 << minion_id;
   thread_mask = (thread_id < 0) ? 0x3                   :(uint64_t)1 << thread_id;

   for (uint64_t t = 0; t < THREADS_PER_MIN; ++t) {
      if ((thread_mask >> t) & 1) {
         write_esr(PP_USER, 0xFF, REGION_OTHER, (1 << 15) | CREDINC_BASE | (t << 1) | credit_counter_num, minion_mask);
      }
   }
   return 0;
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: wait_for_credit
//
//   This function is used to read a given credit counter. Blocks until a credit becomes available
//
//   credit_counter_num : The credit counter to wait on (0 or 1)
//
inline int8_t __attribute__((always_inline)) wait_for_credit(int credit_counter_num) {

   // Check that inputs are valid, otherwise jump to fail
    if (unlikely(credit_counter_num > CREDIT_COUNTER_PER_HART-1)) {
      return -1;
   }

   asm volatile ("csrw fcc, %[counter_num]\n" : : [counter_num] "r" (credit_counter_num));
   return 0;
}


//-------------------------------------------------------------------------------------------------
//
// FUNCTION: get_credit_count_nb
//
//   This function is used to read a given credit counter without non-blocking
//
//   credit_counter_num : The credit counter to read its count from (0 or 1)
//
inline int64_t __attribute__((always_inline)) get_credit_count_nb(int credit_counter_num) {
   uint64_t credit_count;

   // Check that inputs are valid, otherwise jump to fail
   if (unlikely(credit_counter_num > CREDIT_COUNTER_PER_HART-1)) {
      return -1;
   }

   asm volatile ("csrr %[credit_count], fccnb\n" : [credit_count] "=r" (credit_count));

   return (credit_count >> (16 * credit_counter_num)) & 0xFFFFULL;
}


#endif // ! __FCC_H
