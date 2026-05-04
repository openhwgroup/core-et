/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ML_OPS_DISABLED_H_
#define _ML_OPS_DISABLED_H_

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#define ITERATIONS 16*10

#define MSIP_CAUSE               ((1ULL<<63) | 3)
#define ILLEGAL_INST_CAUSE       0x2

#define TRIGGER_MSIP_AD (0x80F40090 | (0xFF<<22))
#define CLEAR_MSIP_AD   (0x80F40098 | (0xFF<<22))
#define TRIGGER_MSIP do { *((volatile uint64_t*) TRIGGER_MSIP_AD ) = 1; } while(0)
#define CLEAR_MSIP   do { *((volatile uint64_t*) CLEAR_MSIP_AD   ) = 1; } while(0)
#define TRIGGER_MSIP_T2 do { *((volatile uint64_t*) TRIGGER_MSIP_AD ) = 4; } while(0)
#define CLEAR_MSIP_T2   do { *((volatile uint64_t*) CLEAR_MSIP_AD   ) = 4; } while(0)

#define NEW_TRIGGER_MSIP(hart_id) do { *((volatile uint64_t*) TRIGGER_MSIP_AD ) = ((uint64_t)0x1 << hart_id); } while(0)
#define NEW_CLEAR_MSIP(hart_id)   do { *((volatile uint64_t*) CLEAR_MSIP_AD   ) = ((uint64_t)0x1 << hart_id); } while(0)

#define CREDIT_INC_T0C0_AD        0x803400C0
#define CRED_INC_T0_C0 do { *((volatile uint64_t*) CREDIT_INC_T0C0_AD   ) = 1; } while(0)
#define CRED_INC_T2_C0 do { *((volatile uint64_t*) CREDIT_INC_T0C0_AD   ) = 2; } while(0)

#define NEW_CRED_INC_LOCAL(min_id) do { *((volatile uint64_t*) CREDIT_INC_T0C0_AD) = ((uint64_t)0x1 << min_id); } while(0)

#define MINION_FEATURE_ADDR      0x80F40000
#define ALL_ONES                 0xFFFFFFFFFFFFFFFF

#define TRAP_ON_GFX_BIT          (1<<0)
#define TRAP_ON_ML_BIT           (1<<1)
#define TRAP_ON_UCACHEOP_BIT     (1<<2)
#define DISABLE_USCP_BIT         (1<<3)
#define DISABLE_MTHREADING_BIT   (1<<4)
#define DISABLE_ULOCK_UNLOCK_BIT (1<<5)

#define CSR_MASK                 ~(0x1fff<<7)
#define TENSOR_REDUCE_INST	 (0x80079073 & CSR_MASK) 
#define TENSOR_FMA_INST		 (0x80179073 & CSR_MASK) 
#define TENSOR_CONV_SIZE_INST    (0x80271073 & CSR_MASK) 
#define TENSOR_CONV_CTRL_INST	 (0x80351073 & CSR_MASK)
#define TENSOR_COOP_INST	 (0x80479073 & CSR_MASK) 
#define TENSOR_MASK_INST      	 (0x805027f3 & CSR_MASK) 
#define TENSOR_QUANT_INST 	 (0x80681073 & CSR_MASK) 
#define TENSOR_ERROR_INST        (0x808027f3 & CSR_MASK) 
#define FLB_INST 		 (0x82001073 & CSR_MASK)
#define FCC_INST		 (0x82101073 & CSR_MASK) 
#define STALL_INST 		 (0x82201073 & CSR_MASK)
#define TENSOR_WAIT_INST	 (0x83035073 & CSR_MASK) 
#define TENSOR_LOAD_L2SCP_INST	 (0x85f31073 & CSR_MASK) 
#define FCCNB_INST		 (0xcc0027f3 & CSR_MASK) 
#define TENSOR_LOAD_INST         (0x83f79073 & CSR_MASK) 
#define TENSOR_STORE_INST	 (0x87f79073 & CSR_MASK)


EXTERN_C void  __attribute__((aligned(4096))) __attribute__ ((interrupt)) trap_handler();
EXTERN_C void  __attribute__((aligned(4096))) __attribute__ ((interrupt)) trap_handler_t2();

inline unsigned int get_hart_id() __attribute__((always_inline));
void test_t0(unsigned int hart_id);
void test_t2(unsigned int hart_id);




#endif
