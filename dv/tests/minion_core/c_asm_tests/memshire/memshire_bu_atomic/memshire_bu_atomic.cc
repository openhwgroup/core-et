/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

//
// Description : This test does a series of memshire atomic and partial write transactions
//
// The steps are as follows:
// 1) The expected results of atomic operations is calculated
// 2) There are 4 atomic transactions to fill the memshire "atomic cache"
// 3) These atomic transactions are checked
// 4) There are some partial writes to the same address as the first atomic transaction
// 5) There is a 5th atomic transaction, which causes the address line with partial writes from step 4 to be evicted to memory
// 6) The results of all the atomic and partial writes are checked
// 
//

#include "macros.h"
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"
#include <inttypes.h>
#include "common.h"
#include "tensors.h"
#include "esr.h"
#include "shire_cache_rand.h"
#include "fcc.h"
#include <cstdio>
#include "print_dbg.h"


#define ESR_SC_REQQ_CTL              0x1

                                                                           

// Preload random data (~1M)
//extern unsigned char _binary_blob_bin_start;

inline void amo_write(uint64_t *addr, uint64_t write_data) {
   __asm__ __volatile__(
      "amoswapg.d x0, %[write_data], (%[addr])\n"
      :
      : [write_data] "r" (write_data),
        [addr] "r" (addr)
      :
   );
}

inline uint64_t amo_cmpswp(uint64_t *addr, uint64_t write_val, uint64_t cmp_val) {
   uint64_t old_data;
   __asm__ __volatile__(
      "add x31, x0, %[cmp_val]\n"
      "amocmpswapg.d %[old_data], %[write_val], (%[addr])\n"
      : [old_data] "=r" (old_data)
      : [addr] "r" (addr), [write_val] "r" (write_val), [cmp_val] "r" (cmp_val)
      : "x31"
   );
   return old_data;
}

//evicting "to l3" really acts like evicting "to mem" when the remote_l3_enable = 0
// if this test is exhibiting weird behavior or is failing, check that the L3s are actually bypassed!
inline void __attribute__((always_inline)) evict_l1_to_l3() {

   for (int w = 0; w < 8; ++w) {
     evict_sw(false, 0x2, w, 0, 15, 0);
     //evict_sw(false, to_Mem, w, 0, 15, 0);
   }
   WAIT_CACHEOPS;
}

uint8_t check_master(void)
{ 
  uint64_t hart_id = get_hart_id();
          
  if( (hart_id & 1) == 0) {
     uint64_t minion_id = ((hart_id>>1) & 0x1F);
     uint64_t shire_id  = ((hart_id>>6) & 0x3F);
     if (shire_id == 32 && minion_id == 0) return 1;  
     else return 0;
  }   
  return 0;
}

uint64_t lock[64/sizeof(uint64_t)] __attribute__((aligned(64))) = {0}; // full cache line
#define CMPSWP_LOCK amo_cmpswp(&lock[0], 1, 0)
#define FREE_LOCK amo_write(&lock[0], 0)

inline void SetLock() {
   while (CMPSWP_LOCK == 1); // spin until the lock seems free
}

inline void UnsetLock() {
   FREE_LOCK;
}

template<typename... Args>
int my_printf(char* fmt, Args... args) {
   unsigned int hid;
   __asm__ __volatile__ (
       "csrr %[hid], hartid\n"
     : [hid] "=r" (hid)
     :
     :
   );
   SetLock();
   print_dbg("[H%d] ", hid);
   print_dbg(fmt, args...);
   __asm__ __volatile__ ("fence iorw, iorw");
   UnsetLock();
   return 0;
}

int main()
{

   // 
   // These are copied from the fc_global_atomics test
   //
   static uint32_t predicted_data[80]  __attribute__ ((aligned (32)));

   static uint32_t operands[80]  __attribute__ ((aligned (32)))= {
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      0xff800000,//-Inf
      0xc1a4cccd, //-20.6
      0x806ce3ee, // -1.0E-38-0
      0x80000000, // -zero
      0x0,  // zero
      0x006ce3ee, //1.0E-38
      0x40000000, //2
      0x7f800000, // +Inf
      /*0x7f800002, // signaling NaN
      0xff800002, // -signaling NaN
      0x7fc00001, //NANQ
      0xffc00001, //-NANQ
      0x1edcba98,
      0x12345678,
      0x12345678,
      0x1edcba98,
      0x69abcdef,
      0x38765432,
      0x7543654a,
      0x3c342985,
      0x15634579,
      0x78967365,
      0x88888888,
      0x88888888,
      0x77777777,
      0x77777777,
      0x44444444,
      0x44444444,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98*/
   };

   static uint32_t result[640]  __attribute__ ((aligned (64)))= {
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0xfedcba98,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x0fedcba9,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x00fedcba,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x000fedcb,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc,
      0x0000fedc
   };

   // 16 is one cache line
   // we want to use 5 lines so make it 80
   //static uint32_t address[16]  __attribute__ ((aligned (32)))= {
   static int32_t address[80]  __attribute__ ((aligned (32)))= {
      -1024,
      -1020,
      -1016,
      -1012,
      -1008,
      -1004,
      -1000,
      -996,
      -992,
      -988,
      -984,
      -980,
      -976,
      -972,
      -968,
      -964,
      -512,
      -508,
      -504,
      -500,
      -496,
      -492,
      -488,
      -484,
      -480,
      -476,
      -472,
      -468,
      -464,
      -460,
      -456,
      -452,
      0,
      4,
      8,
      12,
      16,
      20,
      24,
      28,
      32,
      36,
      40,
      44,
      48,
      52,
      56,
      60,
      512,
      516,
      520,
      524,
      528,
      532,
      536,
      540,
      544,
      548,
      552,
      556,
      560,
      564,
      568,
      572,
      1024,
      1028,
      1032,
      1036,
      1040,
      1044,
      1048,
      1052,
      1056,
      1060,
      1064,
      1068,
      1072,
      1076,
      1080,
      1084
   };

   static uint32_t w_operands[8]  __attribute__ ((aligned (32)))= {
      0x0e7050c1, // et-soc1
      0xdeadbeef, // everyone uses this
      0xfeedbee5, // save the bees!
      0xf007ba11, // american, not european
      0xba5eba11, // boring!
      0x0f10c07a, // motorsport!
      0x0ddba11,  // that's me
      0x0cab005e  // the end
   };

   // Init marker
   C_TEST_START;

   bool  pass = true;
   uint64_t hart_id = getHartId();

   const int NUM_BANKS = 4;
   const uint8_t sub_region_shire_cache = 0x0;
   const uint32_t esr_addr_sc_reqq_ctl = 0x1;
   const uint32_t esr_sc_idx_cop_sm_ctl= 0x6;
   delay(100);

   // All harts wait for the last hart to be enabled by issuing a CSR FCC. Last hart (34Shires * 64) will enable all the harts by broadcasting to FCC ESR
   if (hart_id == 0) {
     int bank;
     uint64_t sc_reqctl;
     
     for (int s=0;s<34;s++) {
       for (bank = 0; bank < NUM_BANKS; bank++) {
         sc_reqctl = read_esr_new(PP_MACHINE, s, REGION_OTHER, sub_region_shire_cache, esr_addr_sc_reqq_ctl, bank);
       }
      }
     
     if (1) { // setting reqctl
        for (int s=0;s<34;s++) { //DON'T FORGET 32 AND 33!!!
          for (bank = 0; bank < NUM_BANKS; bank++) {
           
            // first, flush all the L3s
            write_esr_new(PP_MACHINE, s, REGION_OTHER, sub_region_shire_cache, esr_sc_idx_cop_sm_ctl,0x601, bank); // flush
            int done=0;
            while (!done) {
              uint64_t status = read_esr_new(PP_MACHINE, s, REGION_OTHER, sub_region_shire_cache, esr_sc_idx_cop_sm_ctl, bank);
              if ( ((status >> 24) & 0xff) == 0x4) done=1;
            }
           
            // disable L3
            sc_reqctl = read_esr_new(PP_MACHINE, s, REGION_OTHER, sub_region_shire_cache, esr_addr_sc_reqq_ctl, bank);
            sc_reqctl &= ~(0x1 << 16) ; // go directly to the memory, do not pass go
            sc_reqctl |= (1 << 14);
            write_esr_new(PP_MACHINE, s, REGION_OTHER, sub_region_shire_cache, esr_addr_sc_reqq_ctl, sc_reqctl,bank);
         }
        }
      }
     
     int a = 0x100000;
     
     a = 0x100000;
     while(a-- != 0);
     // Broadcast to give credits to all the harts
     // Can only broadcast to shires 0-31
     write_esr_new(PP_USER, 32, REGION_OTHER, 0x2, 0x18, 0xffffffffffffffff, 0);
     write_esr_new(PP_USER, 32, REGION_OTHER, 0x2, 0x1a, 0xffffffffffffffff, 0);
     write_esr_new(PP_USER, 33, REGION_OTHER, 0x2, 0x18, 0xffffffffffffffff, 0);
     write_esr_new(PP_USER, 33, REGION_OTHER, 0x2, 0x1a, 0xffffffffffffffff, 0);
     broadcast_req (PP_USER, REGION_OTHER, 0x8018, 0xffffffff, 0xffffffffffffffff);
     broadcast_req (PP_USER, REGION_OTHER, 0x801a, 0xffffffff, 0xffffffffffffffff);
   } else {
     asm volatile ("csrwi 	0x821, 0");
   }
   
   // This sequence is copied from the fc_global_atomics test
   if (!check_master()){  
    
      //////// STEP 2
      /* Wait master to evict result*/
      wait_for_credit(0);

      /* Atomic instruction */
      __asm__ __volatile__ (

         "li x11, 0xff\n"
         "mova.m.x x11\n"

         "addi t0, %[result], 1024\n"

         // line #1 lower half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 0(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"
         // line #1 upper half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 32(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"

         // line #2 lower half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 64(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"
         // line #2 upper half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 96(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"

         // line #3 lower half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 128(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"
         // line #3 upper half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 160(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"

         // line #4 lower half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 192(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"
         // line #4 upper half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 224(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"
         
         :
         : [operands] "r" (operands), [address] "r" (address), [result] "r" (result)
         : "t0", "f0", "f1"
      );

      /* Tell master minion to check data */
      give_credit(32, 0, 0, 0);


      //////// STEP 4

      /* Wait master to evict result*/
      wait_for_credit(1);

      /* Atomic instruction */
      __asm__ __volatile__ (

         "li x11, 0xff\n"
         "mova.m.x x11\n"

         "addi t0, %[result], 1024\n"

         // line #1 lower half
         "flw.ps f0, 0(%[w_operands])\n"
         "flw.ps f1, 0(%[address])\n"
         "fsw.ps f0, -1024(t0)\n"

         :
         : [w_operands] "r" (w_operands), [address] "r" (address), [result] "r" (result)
         : "t0", "f0", "f1"
      );

     

      /* Evict operands from caches*/
      evict_l1_to_l3();


      //////// STEP 5

      /* Atomic instruction */
      __asm__ __volatile__ (

         "li x11, 0xff\n"
         "mova.m.x x11\n"

         "addi t0, %[result], 1024\n"

         // line #5 lower half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 256(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"
         // line #5 upper half
         "flw.ps f0, 0(%[operands])\n"
         "flw.ps f1, 288(%[address])\n"
         "famoaddg.pi f0,f1(t0)\n"

         :
         : [operands] "r" (operands), [address] "r" (address), [result] "r" (result)
         : "t0", "f0", "f1"
      );

      /* Evict operands from caches*/
      evict_l1_to_l3();


      /* Tell master minion to check data */
      give_credit(32, 0, 0, 1);

   }
   /* shire 32*/
   else {

      //////// STEP 1
      int j = 0;
      /* Calculate predicted data */
      for ( int i = 0; i < 80; i ++) {
         j = (i/16)*128 + (i%16);    // this looks weird but it works
         predicted_data[i] = operands[i] + result[j];
      }
      
      /* Evict operands from caches*/
      evict_l1_to_l3();
    
      /* Tell minion 0 to write result to L3*/
      give_credit(0, 0, 0, 0);

    

      //////// STEP 3
      /* Wait credit */
      wait_for_credit(0);

      /* Check data in L3*/
      for ( int i = 0; i < 64; i ++) {
         j = (i/16)*128 + (i%16);    // this looks weird but it works
         if (result[j] != predicted_data[i]) {
           pass = false;
         }
      }

      /* Calculate predicted data */
      /* spoiler alert: it's the same as the w_operands since we're just doing writes */
      for ( int i = 0; i < 8; i ++) {
         predicted_data[i] = w_operands[i];
      }

      /* Evict operands from caches*/
      evict_l1_to_l3();
   
      /* Tell minion 0 to write result to L3*/
      give_credit(0, 0, 0, 1);


      //////// STEP 6
      /* Wait credit */
      wait_for_credit(1);
      

      /* Check data in L3*/
      for ( int i = 0; i < 80; i ++) {
         j = (i/16)*128 + (i%16);    // this looks weird but it works
         if (result[j] != predicted_data[i]) {
           pass = false;
         }
      }
   }

   if (pass == false) {
     //et_printf_long("TEST: FAILED: hart 0x%lx\n", hart_id);
     my_printf("TEST FAILED");
      C_TEST_FAIL;
   } else {
     //et_printf_long("TEST: PASSED: hart 0x%lx\n", hart_id);
     my_printf("TEST PASSED");
      C_TEST_PASS;
   }
   return 0;
}



