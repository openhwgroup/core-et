/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _CLEAR_L2_SCP_
#define _CLEAR_L2_SCP_ 1

inline void wait_sc_cop(volatile uint64_t* addr) {
   const uint64_t SC_COP_SM_ACTIVE = 8;
   uint64_t cop_sm_state;
   uint64_t mask;
   do {
      __asm__ __volatile__(
         "ld %[cop_sm_state], 0(%[addr])\n"
         "srli %[cop_sm_state], %[cop_sm_state], 24\n"
         "li %[mask], 255\n"
         "and %[cop_sm_state], %[cop_sm_state], %[mask]\n"
         : [cop_sm_state] "=r" (cop_sm_state),
           [mask] "=r" (mask)
         : [addr] "r" (addr)
         :
      );
   } while (cop_sm_state == SC_COP_SM_ACTIVE);
}

void sc_pv_clear_l2_scratchpad(uint64_t sid) {
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
      /* Lead code */

      uint64_t sid_mask = sid << 22;

      uint64_t sc_cop_enc = 2305;
   
      volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_BCAST = (uint64_t*) (0x80F1E030 + sid_mask);
   
      // Trigger a Shire Cache Clear L2 SCP cache op
      *ESR_SC_IDX_COP_SM_CTL_BCAST = sc_cop_enc;
   
      FENCE;
   
      // Wait for the cacheop to be done
      volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_B0 = (uint64_t*) (0x80F00030 + sid_mask);
      volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_B1 = (uint64_t*) (0x80F02030 + sid_mask);
      volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_B2 = (uint64_t*) (0x80F04030 + sid_mask);
      volatile uint64_t* ESR_SC_IDX_COP_SM_CTL_B3 = (uint64_t*) (0x80F06030 + sid_mask);
      wait_sc_cop(ESR_SC_IDX_COP_SM_CTL_B0);
      wait_sc_cop(ESR_SC_IDX_COP_SM_CTL_B1);
      wait_sc_cop(ESR_SC_IDX_COP_SM_CTL_B2);
      wait_sc_cop(ESR_SC_IDX_COP_SM_CTL_B3);

      // Send FCC credit to the followers
      uint64_t* fcc_0 = (uint64_t*) 0x803400C0;
      uint64_t* fcc_1 = (uint64_t*) 0x803400D0;
   
      *fcc_0 = 0xFFFFFFFF;
      *fcc_1 = 0xFFFFFFFF;
   }

   /* Wait for and FCC Credit comming from the Lead HART */
   __asm__ __volatile__(
      "csrrwi x0, fcc, 0\n"
   );
}
#endif
