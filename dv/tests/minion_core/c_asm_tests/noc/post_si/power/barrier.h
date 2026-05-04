/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


uint64_t barrier_count[64/sizeof(uint64_t)] __attribute__((aligned(64))) = {0}; // full cache line

uint64_t global_read(uint64_t*addr) {
   uint64_t ret;
   __asm__ __volatile__(
      "amoorg.d %[ret], x0, (%[addr])\n"
      : [ret] "=r" (ret)
      : [addr] "r" (addr)
      :
   );
   return ret;
}

uint64_t global_incr(uint64_t *addr) {
   uint64_t ret;
   uint64_t add_value = 1;
   __asm__ __volatile__(
      "amoaddg.w %[ret], %[add_value], (%[addr])\n"
      : [ret] "=r" (ret)
      : [add_value] "r" (add_value),
        [addr] "r" (addr)
      :
   );
   return ret;
}

void global_write(uint64_t *addr, uint64_t write_data) {
   __asm__ __volatile__(
      "amoswapg.w x0, %[write_data], (%[addr])\n"
      :
      : [write_data] "r" (write_data),
        [addr] "r" (addr)
      :
   );
}

void soc_barrier(uint64_t active_num_shires) {
   // Fast Local Barrier to ensure only one HART triggers the Shire Cache Operation
   int flb_enc;
   __asm__ __volatile__(
      "li %[flb_enc], -8326528131590895620\n"
      "csrrw %[flb_enc], flb0, %[flb_enc]\n"
      : [flb_enc] "=r" (flb_enc)
      :
      :
   );
   
   if (flb_enc != 0) {

      // Global AMO Barrier
      uint64_t prev_val = global_incr(barrier_count);

      // If I am the last one
      if (prev_val == (active_num_shires - 1)) {
         // reset the barrier
         global_write(barrier_count, 0);

         for (uint64_t sid = 0; sid < active_num_shires; sid++) {
            // Send FCC credit to the followers
            uint64_t* fcc_0 = (uint64_t*) (0x803400C0 + (sid << 22));
            uint64_t* fcc_1 = (uint64_t*) (0x803400D0 + (sid << 22));
   
            *fcc_0 = 0xFFFFFFFF;
            *fcc_1 = 0xFFFFFFFF;
         }
      }
   }

   /* Wait for and FCC Credit comming from the Lead HART */
   __asm__ __volatile__(
      "csrrwi x0, fcc, 0\n"
   );
}
