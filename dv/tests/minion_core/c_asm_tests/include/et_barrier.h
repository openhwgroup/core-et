/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


// This barrier assumes that:
//  * Active shires have consecutive shire id
//  * Expected number of active harts per shire is 16 (one neigh)
//  * Shire 0 is enabled
// TODO: active_num_shires left for compatibility, consider removing in erbium
inline void _soc_fcc_barrier(uint64_t active_num_shires, uint64_t delay) {

   // Send and FCC Credit to H0 in Shire 0
   uint64_t* fcc_h0 = (uint64_t*) (0x803400C0);
   *fcc_h0 = 0x1;

   uint64_t hid = get_hart_id();
   if (hid == 0) {
      for (uint64_t i = 0; i < (16*active_num_shires); i++) {
         // Wait for a FCC credit comming from the rest of the HARTs in the SoC
         __asm__ __volatile__(
            "csrrwi x0, fcc, 0\n"
         );
      }

      // Delay using NOPs
      for (uint64_t i = 0; i < delay; i++) {
         NOP;
      }

      for (uint64_t sid = 0; sid < active_num_shires; sid++) {
         uint64_t* fcc_0 = (uint64_t*) (0x803400C0);
         *fcc_0 = 0xFF; // 16 harts with id=0
         uint64_t* fcc_1 = (uint64_t*) (0x803400D0);
         *fcc_1 = 0xFF; // 16 harts with id=1
      }
   }

   /* Wait for and FCC Credit comming from S0:N0:M0:T0 */
   __asm__ __volatile__(
      "csrrwi x0, fcc, 0\n"
   );
}

void soc_fcc_barrier(uint64_t active_num_shires) {
   _soc_fcc_barrier(active_num_shires, 0);
}

void soc_fcc_barrier_with_delay(uint64_t active_num_shires, uint64_t delay) {
   _soc_fcc_barrier(active_num_shires, delay);
}
