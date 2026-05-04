/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"
#include "minion.h"
#include "tensors.h"


#define SHIRE_0 0UL
#define ESR_SHIRE_REGS ((0x1UL)<<15)
#define ESR_SHIRE_COOP_MODE_ADDRESS (ESR_SHIRE_REGS | 0x52UL)


inline __attribute__((always_inline)) void tensorWait(long id) {
    __asm__ __volatile__
        (
         " csrw 0x830, %[id]\n"
         :
         : [id] "r" (id)
         : "memory"
        );
}

uint32_t mcause_val = 0x0;

inline __attribute__((naked)) void mtrap_vec(void)
{

  uint64_t *mcause_val_ptr = (uint64_t*)(&mcause_val);
   __asm__ __volatile__ (
         ".align 12\n\t"
         "mtrap_vec%=: csrr s1, mtval\n\t"
         "csrr s2, mcause\n\t"
         "csrr s3, mepc\n\t"
         "addi s3, s3, 4\n\t"
         "sd s2, 0(%[mcause_val])\n\t"
         "csrw mepc, s3\n\t"
         "mret\n\t"
         ::[mcause_val] "r" (mcause_val_ptr):
   );
  
  if(mcause_val != 2) {
    C_TEST_FAIL;
  }
}

inline void set_mtrap_vec(void)
{
    void (*mtvec_ptr)(void) = &mtrap_vec;
    __asm__ __volatile__ (
        "csrw mtval, t0\n\t"
        "csrw stval, t0\n\t"
        "mv t0, %[mtvec]\n\t"
        "csrw mtvec, t0\n\t"
        :
        :[mtvec] "r" (mtvec_ptr)
        :
  );
}

int main() {

  uint64_t tid = get_thread_id();
  if (tid == 0) {
      // Setup machine trap vector
      set_mtrap_vec();
      excl_mode(1);
/**
#ifdef L1D_SPLIT_MODE
      mcache_control(1,0,0);
#endif
#ifdef L1D_SCP_MODE
      mcache_control(1,0,0);
      mcache_control(1,1,0);
#endif
**/
      setup_l1dcache_split_or_scp();  // PRM-8; VERIF-3295
      excl_mode(0);
  } else {
      delay(30);
  }

  C_TEST_START;
  int status = 0;

  setup_cache_scp();

  uint32_t data[256];
  for(uint32_t i = 0; i < 256; i++) {
    data[i] = i;
  }
  uint64_t data_array = (uint64_t) data;


  if ( tid % 2 == 0 ) {
    /////////////////////////////// Set ESR_SHIRE_COOP_MODE ///////////////////////////////
    write_esr(PP_SUPERVISOR, SHIRE_0, REGION_OTHER, ESR_SHIRE_COOP_MODE_ADDRESS, 1);
    if(read_esr(PP_SUPERVISOR, SHIRE_0, REGION_OTHER, ESR_SHIRE_COOP_MODE_ADDRESS) != 1) {
      status = 1;
    }

    tensor_mask(0x0, 0x02);
    tensor_coop(0x03, 0x01, 0);

    tensor_load(1, true, 0, 0, 0, data_array, 0, 0, 0, 0);
    tensorWait(TENSOR_LOAD_WAIT_0);
    if ((mcause_val != 0) && (tid < 64)) {
      status = 1;
    }
    FENCE;

    ////////////////////////////// Reset ESR_SHIRE_COOP_MODE //////////////////////////////
#if 1
    write_esr(PP_SUPERVISOR, SHIRE_0, REGION_OTHER, ESR_SHIRE_COOP_MODE_ADDRESS, 0);
    if(read_esr(PP_SUPERVISOR, SHIRE_0, REGION_OTHER, ESR_SHIRE_COOP_MODE_ADDRESS) != 0) {
      status = 1;
    }

    tensor_mask(0x0, 0x02);
    tensor_coop(0x03, 0x01, 0);

    tensor_load(1, true, 0, 0, 0, data_array, 0, 0, 0, 0);
    tensorWait(TENSOR_LOAD_WAIT_0);
    if(mcause_val != 2) {
      status = 1;
    }
    FENCE;
    mcause_val = 0;
#endif
    /////////////////////////////// Set ESR_SHIRE_COOP_MODE ///////////////////////////////
    write_esr(PP_SUPERVISOR, SHIRE_0, REGION_OTHER, ESR_SHIRE_COOP_MODE_ADDRESS, 1);
    if(read_esr(PP_SUPERVISOR, SHIRE_0, REGION_OTHER, ESR_SHIRE_COOP_MODE_ADDRESS) != 1) {
      status = 1;
    }

    tensor_mask(0x0, 0x02);
    tensor_coop(0x03, 0x01, 0);

    tensor_load(1, true, 0, 0, 0, data_array, 0, 0, 0, 0);
    tensorWait(TENSOR_LOAD_WAIT_0);
    if ((mcause_val != 0) && (tid < 64)) {
      status = 1;
    }
    FENCE;
  }

  if(status) {
    C_TEST_FAIL;
  } else {
    C_TEST_PASS;
  }
}
