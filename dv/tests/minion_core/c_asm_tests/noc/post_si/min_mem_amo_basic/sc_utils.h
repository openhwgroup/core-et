/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _SC_COP_SM_
#define _SC_COP_SM_

#include "fcc.h"
#include "sc_esr.h"
#include "shire_sync.h"

#ifndef FCC_COUNT_NUM
   #define FCC_COUNT_NUM 0
#endif

#define MAX_NUM_SHIRES 33

#define SC_L2_INV 1
#define SC_L2_FLUSH 2
#define SC_L2_EVICT 3
#define SC_L3_INV 4
#define SC_L3_FLUSH 5
#define SC_L3_EVICT 6
#define SC_CLEAR_SCP 9

#define SC_COP_SM_RESET 1
#define SC_COP_SM_ALL_INV 2
#define SC_COP_SM_IDLE 4
#define SC_COP_SM_ACTIVE 8
#define SC_COP_SM_CB_INV 16
#define SC_COP_SM_DBG 32
#define SC_COP_SM_SYNC 64

#define SC_CMD(OPCODE) ((((OPCODE) & 0xf) << 8) | 1)
#define SC_GET_COP_SM_STATE(ADDR) ((*(ADDR) >> 24) && 0xff)

#define ESR_SC_IDX_COP_SM_CTL_SHIRE0 0x80F1E030

inline uint64_t* get_esr_sc_idx_cop_sm_ctl_addr(uint64_t shire_id) {
   if (shire_id < MAX_NUM_SHIRES) {
      return (uint64_t*) (ESR_SC_IDX_COP_SM_CTL_SHIRE0 + (shire_id << 22));
   }
   else {
      C_TEST_FAIL;
      return (uint64_t*) 0xbadc0de;
   }
}

inline void wait_idx_cop_sm(uint64_t shire_id) {
   FENCE;
   uint64_t* esr_sc_idx_cop_sm_ctl = get_esr_sc_idx_cop_sm_ctl_addr(shire_id);

   uint64_t cop_sm_state = SC_GET_COP_SM_STATE(esr_sc_idx_cop_sm_ctl);
   while (cop_sm_state == SC_COP_SM_ACTIVE) {
      cop_sm_state = SC_GET_COP_SM_STATE(esr_sc_idx_cop_sm_ctl);
   }
}

void flush_l3(uint64_t shire_id) {
   uint64_t* esr_sc_idx_cop_sm_ctl = get_esr_sc_idx_cop_sm_ctl_addr(shire_id);
   uint64_t  cmd = SC_CMD(SC_L3_FLUSH);
   *esr_sc_idx_cop_sm_ctl = cmd;
   wait_idx_cop_sm(shire_id);
}

inline void set_local_active_harts(uint8_t* array_mask) {
   uint64_t minions_mask = MINION_MASK;
   uint8_t th0 = (THREAD_MASK & 0x1);
   uint8_t th1 = ((THREAD_MASK >> 1) & 0x1);
   for (int i = 0; i < NUM_HART_PER_SHIRE; i+=2) {
      uint64_t tmp = minions_mask & 0x1;
      *array_mask = tmp * th0;
      array_mask++;
      *array_mask = tmp * th1;
      array_mask++;
      minions_mask >>= 1;
   }
}

void disable_l3(uint64_t hart_id, const uint64_t local_active_hart){
   // L3 must be disabled by one single HART, so we need to make sure all the other HARTS
   // are quiet. To achieve this, we will rely on the FCC mechanism.
   uint64_t local_hart_id = (hart_id & 0x3F);
   uint64_t shire_id = hart_id >> 6;

   if (local_hart_id == local_active_hart) {
      // Receive an FCC credit from each local active HART
      uint64_t local_active_num_harts = get_num_local_active_harts();
      for (uint64_t i = 1; i < local_active_num_harts; i++) {
         wait_for_credit(FCC_COUNT_NUM);
      }

      flush_l3(shire_id);
      set_esr_sc_remote_l3_enable(shire_id, 0);

      // Send an FCC credit to each local active HART
      uint8_t local_active_harts_array[NUM_HART_PER_SHIRE];
      set_local_active_harts(local_active_harts_array);
      for (int i = 0; i < NUM_HART_PER_SHIRE; i++) {
         if (local_active_harts_array[i]) {
            uint64_t mid = i >> 1;
            uint64_t tid = i & 0x1;
            give_credit(shire_id, mid, tid, FCC_COUNT_NUM);
         }
      }
   }
   else {
      const uint64_t local_active_minion_id = local_active_hart >> 1;
      const uint64_t local_active_thread_id = local_active_hart & 0x1;
      give_credit(shire_id, local_active_minion_id, local_active_thread_id, FCC_COUNT_NUM);
   }
   wait_for_credit(FCC_COUNT_NUM);
}
#endif
