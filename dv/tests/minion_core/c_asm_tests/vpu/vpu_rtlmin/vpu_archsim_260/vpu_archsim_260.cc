/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "macros.h"
#include "cacheops.h"
#include "tensors.h"
#include "minion.h"


//// Macros

#define VSNIP_RSV           : : [operands] "r" (operands), [m_11] "r" (m_11), [m_12] "r" (m_12), [m_13] "r" (m_13), [m_14] "r" (m_14), [m_15] "r" (m_15) : "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
#define TSNIP_RSV          : : [x31_enc] "r" (x31_enc), [csr_enc] "r" (csr_enc) : "x3", "x4", "x5", "x6", "x7", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "x31"
#define RESET_FCSR            __asm__ __volatile__ ( "csrw    fcsr, zero\n" VSNIP_RSV);
#define RESET_TENSOR_COOP     __asm__ __volatile__ ( "csrw    0x804, zero\n" VSNIP_RSV);

#define ECALL                 __asm__ __volatile__ ( "ecall\n" VSNIP_RSV);
#define MRET                  __asm__ __volatile__ ( "mret\n" VSNIP_RSV);

#define TENSOR_WAIT_TL0       __asm__ __volatile__ ( "csrwi   0x830, 0x0\n" VSNIP_RSV);
#define TENSOR_WAIT_TL1       __asm__ __volatile__ ( "csrwi   0x830, 0x1\n" VSNIP_RSV);
#define TENSOR_WAIT_COP       __asm__ __volatile__ ( "csrwi   0x830, 0x6\n" VSNIP_RSV);
#define TENSOR_WAIT_FMA       __asm__ __volatile__ ( "csrwi   0x830, 0x7\n" VSNIP_RSV);
#define TENSOR_WAIT_STR       __asm__ __volatile__ ( "csrwi   0x830, 0x8\n" VSNIP_RSV);
#define TENSOR_WAIT_RED       __asm__ __volatile__ ( "csrwi   0x830, 0x9\n" VSNIP_RSV);
#define TENSOR_WAIT_QNT       __asm__ __volatile__ ( "csrwi   0x830, 0xa\n" VSNIP_RSV);
#define SET_TENSOR_MASK    \
   __asm__ __volatile__ ( \
         "li x5, 0xFFFF\n" \
         "csrrw zero, 0x805, x5\n" \
         VSNIP_RSV);

#define SET_SCP_CTL    \
   __asm__ __volatile__ ( \
         "csrrs	x0, 0x7e0, 3\n" \
         VSNIP_RSV);


inline void __attribute__((always_inline)) tensor_load_vpu (uint64_t     use_tmask,
                                                        uint64_t     use_coop,
                                                        uint64_t dst_start,
                                                        void* vaddr,
                                                        uint64_t num_lines,
                                                        uint64_t stride,
                                                        uint64_t id)
{
   uint64_t  csr_enc = (((uint64_t)use_tmask & 1) << 63) |
                      (((uint64_t)use_coop & 1)  << 62) |
                      (((uint64_t)0x0 & 0x7) << 59)    |
                      ((dst_start & 0x3F) << 53)        |
                      (((uint64_t)0x0 & 0x1) << 52)          |
                      (((uint64_t)vaddr & 0xFFFFFFFFFFC0ULL))      |
                      (((uint64_t)0x0 & 0x3) << 4)             |
                      ((num_lines & 0xF));
   uint64_t  x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x83f, %[csr_enc]\n"
         TSNIP_RSV
   );
}

inline void __attribute__((always_inline)) tensor_load_i8_vpu (uint64_t     use_tmask,
                                                        uint64_t     use_coop,
                                                        uint64_t dst_start,
                                                        void* vaddr,
                                                        uint64_t offset,
                                                        uint64_t num_lines,
                                                        uint64_t stride,
                                                        uint64_t id)
{
   uint64_t  csr_enc = (((uint64_t)use_tmask & 1) << 63) |
                      (((uint64_t)use_coop & 1)  << 62) |
                      (((uint64_t)0x1 & 0x7) << 59)    |
                      ((dst_start & 0x3F) << 53)        |
                      (((uint64_t)0x0 & 0x1) << 52)          |
                      (((uint64_t)vaddr & 0xFFFFFFFFFFC0ULL))      |
                      ((offset & 0x3) << 4)             |
                      ((num_lines & 0xF));
   uint64_t  x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x83f, %[csr_enc]\n"
         TSNIP_RSV
   );
}

inline void __attribute__((always_inline)) tensor_load_i16_vpu (uint64_t     use_tmask,
                                                        uint64_t     use_coop,
                                                        uint64_t dst_start,
                                                        void* vaddr,
                                                        uint64_t offset,
                                                        uint64_t num_lines,
                                                        uint64_t stride,
                                                        uint64_t id)
{
   uint64_t  csr_enc = (((uint64_t)use_tmask & 1) << 63) |
                      (((uint64_t)use_coop & 1)  << 62) |
                      (((uint64_t)0x2 & 0x7) << 59)    |
                      ((dst_start & 0x3F) << 53)        |
                      (((uint64_t)0x0 & 0x1) << 52)          |
                      (((uint64_t)vaddr & 0xFFFFFFFFFFC0ULL))      |
                      ((offset & 0x1) << 5)             |
                      (((uint64_t)0x0 & 0x1) << 4)             |
                      ((num_lines & 0xF));
   uint64_t  x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x83f, %[csr_enc]\n"
         TSNIP_RSV
   );
}

inline void __attribute__((always_inline)) tensor_load_t8_vpu (uint64_t     use_tmask,
                                                        uint64_t     use_coop,
                                                        uint64_t dst_start,
                                                        void* vaddr,
                                                        uint64_t offset,
                                                        uint64_t num_lines,
                                                        uint64_t stride,
                                                        uint64_t id)
{
   uint64_t  csr_enc = (((uint64_t)use_tmask & 1) << 63) |
                      (((uint64_t)use_coop & 1)  << 62) |
                      (((uint64_t)0x5 & 0x7) << 59)    |
                      ((dst_start & 0x3F) << 53)        |
                      (((uint64_t)0x0 & 0x1) << 52)          |
                      (((uint64_t)vaddr & 0xFFFFFFFFFFC0ULL))      |
                      ((offset & 0x3) << 4)             |
                      ((num_lines & 0xF));
   uint64_t  x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x83f, %[csr_enc]\n"
         TSNIP_RSV
   );
}

inline void __attribute__((always_inline)) tensor_load_t16_vpu (uint64_t     use_tmask,
                                                        uint64_t     use_coop,
                                                        uint64_t dst_start,
                                                        void* vaddr,
                                                        uint64_t offset,
                                                        uint64_t num_lines,
                                                        uint64_t stride,
                                                        uint64_t id)
{
   uint64_t  csr_enc = (((uint64_t)use_tmask & 1) << 63) |
                      (((uint64_t)use_coop & 1)  << 62) |
                      (((uint64_t)0x6 & 0x7) << 59)    |
                      ((dst_start & 0x3F) << 53)        |
                      (((uint64_t)0x0 & 0x1) << 52)          |
                      (((uint64_t)vaddr & 0xFFFFFFFFFFC0ULL))      |
                      ((offset & 0x1) << 5)             |
                      (((uint64_t)0x0 & 0x1) << 4)             |
                      ((num_lines & 0xF));
   uint64_t  x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x83f, %[csr_enc]\n"
         TSNIP_RSV
   );
}

inline void __attribute__((always_inline)) tensor_load_t32_vpu (uint64_t     use_tmask,
                                                        uint64_t     use_coop,
                                                        uint64_t dst_start,
                                                        void* vaddr,
                                                        uint64_t num_lines,
                                                        uint64_t stride,
                                                        uint64_t id)
{
   uint64_t  csr_enc = (((uint64_t)use_tmask & 1) << 63) |
                      (((uint64_t)use_coop & 1)  << 62) |
                      (((uint64_t)0x6 & 0x7) << 59)    |
                      ((dst_start & 0x3F) << 53)        |
                      (((uint64_t)0x0 & 0x1) << 52)          |
                      (((uint64_t)vaddr & 0xFFFFFFFFFFC0ULL))      |
                      (((uint64_t)0x0 & 0x3) << 4)             |
                      ((num_lines & 0xF));
   uint64_t  x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x83f, %[csr_enc]\n"
         TSNIP_RSV
   );
}


inline void __attribute__((always_inline)) tensor_load_setup_b_vpu (
                                                        uint64_t     use_coop,
                                                        void* vaddr,
                                                        uint64_t num_lines,
                                                        uint64_t stride,
                                                        uint64_t id)
{
   uint64_t  csr_enc = (((uint64_t)0x0 & 1) << 63) |
                      (((uint64_t)use_coop & 1)  << 62) |
                      (((uint64_t)0x0 & 0x7) << 59)    |
                      (((uint64_t)0x0 & 0x3F) << 53)        |
                      (((uint64_t)0x1 & 0x1) << 52)          |
                      (((uint64_t)vaddr & 0xFFFFFFFFFFC0ULL))      |
                      (((uint64_t)0x0 & 0x3) << 4)             |
                      ((num_lines & 0xF));
   uint64_t  x31_enc = (stride & 0xFFFFFFFFFFC0ULL) | (id & 0x1);

   __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x83f, %[csr_enc]\n"
         TSNIP_RSV
   );
}

/////////////////////////////////////////////////////
inline void __attribute__((always_inline)) tensor_store_scp_vpu (uint64_t entry_stride,
                                                            uint64_t start_scp_entry,
                                                            uint64_t Arows,
                                                            uint64_t scp,
                                                            void* vaddr,
                                                            uint64_t stride)
{
   uint64_t  csr_enc = ((entry_stride & 0x3) << 62)      |
                      ((start_scp_entry & 0x3F) << 56)  |
                      ((Arows & 0xF) << 51)             |
                      ((scp & 0x1) << 48)               |
                      (((uint64_t)vaddr & 0xFFFFFFFFFFC0ULL))
                      ;
   uint64_t  x31_enc = (stride & 0xFFFFFFFFFFC0UL);

   __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x87f, %[csr_enc]\n"
         TSNIP_RSV
   );
}

inline void __attribute__((always_inline)) tensor_store_vpu(uint64_t reg_stride,
                                                         uint64_t start_reg,
                                                         uint64_t cols,
                                                         uint64_t Arows,
                                                         uint64_t coop_store,
                                                         uint64_t scp,
                                                         void* vaddr,
                                                         uint64_t stride)
{
   uint64_t  csr_enc = ((reg_stride & 0x3) << 62)      |
                      ((start_reg & 0x1F) << 57)      |
                      ((cols & 0x3) << 55)            |
                      ((Arows & 0xF) << 51)           |
                      ((coop_store & 0x3) << 49)      |
                      ((scp & 0x1) << 48)             |
                      (((uint64_t)vaddr & 0xFFFFFFFFFFF0ULL))    ;
   uint64_t  x31_enc = (stride & 0xFFFFFFFFFF0UL);

   __asm__ __volatile__ (
         "add x31, zero, %[x31_enc]\n"
         "csrw 0x87f, %[csr_enc]\n"
         TSNIP_RSV
   );
}
inline void __attribute__((always_inline)) tensor_fma16_vpu(uint64_t use_tmask, uint64_t b_num_cols, uint64_t a_num_rows, uint64_t a_num_cols, uint64_t offset, uint64_t ten_b_in_mem, uint64_t scp_loc_b, uint64_t scp_loc_a, uint64_t first_pass) {
   uint64_t  csr_enc = ((use_tmask & 1) << 63) |
                      ((b_num_cols & 0x3) << 55)         |
                      ((a_num_rows & 0xF) << 51)        |
                      ((a_num_cols & 0xF) << 47)        |
                      ((offset & 0xF) << 43)            |
                      ((ten_b_in_mem & 0xFF) << 20)        |
                      ((scp_loc_b & 0xFF) << 12)        |
                      ((scp_loc_a & 0xFF) << 4)         |
                      ((0x1 & 0x7) << 1)             |
                      (first_pass & 1);

   uint64_t  x31_enc = 0 ;
   __asm__ __volatile__ (
         "csrw 0x801, %[csr_enc]\n"
         TSNIP_RSV
   );
}
inline void __attribute__((always_inline)) tensor_fma32_vpu(uint64_t use_tmask, uint64_t b_num_cols, uint64_t a_num_rows, uint64_t a_num_cols, uint64_t offset, uint64_t ten_b_in_mem, uint64_t scp_loc_b, uint64_t scp_loc_a, uint64_t first_pass) {
   uint64_t  csr_enc = (((uint64_t)use_tmask & 1) << 63) |
                      ((b_num_cols & 0x3) << 55)         |
                      ((a_num_rows & 0xF) << 51)        |
                      ((a_num_cols & 0xF) << 47)        |
                      ((offset & 0xF) << 43)            |
                      ((ten_b_in_mem & 0xFF) << 20)        |
                      ((scp_loc_b & 0xFF) << 12)        |
                      ((scp_loc_a & 0xFF) << 4)         |
                      ((0x0 & 0x7) << 1)             |
                      ((uint64_t)first_pass & 1);

   uint64_t  x31_enc = 0 ;
   __asm__ __volatile__ (
         "csrw 0x801, %[csr_enc]\n"
         TSNIP_RSV
   );
}

inline void __attribute__((always_inline)) tensor_ima_vpu(uint64_t use_tmask, uint64_t b_num_cols, uint64_t a_num_rows, uint64_t a_num_cols, uint64_t offset, uint64_t to_vrf, uint64_t u_b, uint64_t u_a, uint64_t ten_b_in_mem, uint64_t scp_loc_b, uint64_t scp_loc_a, uint64_t first_pass) {
   uint64_t  csr_enc = ((use_tmask & 1) << 63) |
                      ((b_num_cols & 0x3) << 55)         |
                      ((a_num_rows & 0xF) << 51)        |
                      ((a_num_cols & 0xF) << 47)        |
                      ((offset & 0xF) << 43)            |
                      ((to_vrf & 0x1) << 23)        |
                      ((u_b & 0x1) << 22)        |
                      ((u_a & 0x1) << 21)        |
                      ((ten_b_in_mem & 0x1) << 20)        |
                      ((scp_loc_b & 0xFF) << 12)        |
                      ((scp_loc_a & 0xFF) << 4)         |
                      ((0x3 & 0x7) << 1)             |
                      (first_pass & 1);

   uint64_t  x31_enc = 0 ;
   __asm__ __volatile__ (
         "csrw 0x801, %[csr_enc]\n"
         TSNIP_RSV
   );
}


inline void __attribute__((always_inline)) tensor_reduce_send_vpu(uint64_t start_reg, uint64_t num_reg, uint64_t partnerID) {
   uint64_t  csr_enc = ((start_reg & 0x1F) << 57)        |
                      ((num_reg & 0xFF) << 16)          |
                      ((partnerID & 0x1FFF) << 3)        |
                      ((0x0 & 0x3));

   uint64_t  x31_enc = 0 ;
   __asm__ __volatile__ (
         "csrw 0x800, %[csr_enc]\n"
         TSNIP_RSV
   );
}
inline void __attribute__((always_inline)) tensor_reduce_rcv_vpu(uint64_t start_reg, uint64_t operation, uint64_t num_reg, uint64_t partnerID) {
   uint64_t  csr_enc = ((start_reg & 0x1F) << 57)        |
                      ((operation & 0xF) << 24)          |
                      ((num_reg & 0xFF) << 16)          |
                      ((partnerID & 0x1FFF) << 3)        |
                      ((0x1 & 0x3));

   uint64_t  x31_enc = 0 ;
   __asm__ __volatile__ (
         "csrw 0x800, %[csr_enc]\n"
         TSNIP_RSV
   );
}

inline void __attribute__((always_inline)) tensor_reduce_auto_vpu(uint64_t start_reg, uint64_t operation, uint64_t num_reg, uint64_t depth) {
   uint64_t  csr_enc = ((start_reg & 0x1F) << 57)        |
                      ((operation & 0xF) << 24)          |
                      ((num_reg & 0xFF) << 16)          |
                      ((depth & 0xF) << 3)        |
                      ((0x3 & 0x3));

   uint64_t  x31_enc = 0 ;
   __asm__ __volatile__ (
         "csrw 0x800, %[csr_enc]\n"
         TSNIP_RSV
   );
}

inline void __attribute__((always_inline)) tensor_reduce_bcast_vpu(uint64_t start_reg, uint64_t operation, uint64_t num_reg, uint64_t depth) {
   uint64_t  csr_enc = ((start_reg & 0x1F) << 57)        |
                      ((operation & 0xF) << 24)          |
                      ((num_reg & 0xFF) << 16)          |
                      ((depth & 0xF) << 3)        |
                      ((0x2 & 0x3));

   uint64_t  x31_enc = 0 ;
   __asm__ __volatile__ (
         "csrw 0x800, %[csr_enc]\n"
         TSNIP_RSV
   );
}


inline void __attribute__((always_inline)) tensor_quant_vpu(uint64_t start_reg, uint64_t col, uint64_t row, uint64_t scp_loc, uint64_t transf9, uint64_t transf8, uint64_t transf7, uint64_t transf6, uint64_t transf5, uint64_t transf4, uint64_t transf3, uint64_t transf2, uint64_t transf1, uint64_t transf0 ) {
   uint64_t  csr_enc = ((start_reg  & 0x1F) << 57)       |
                      ((col 	   & 0x3) << 55)        |
                      ((row    	   & 0xF) << 51)        |
                      ((scp_loc    & 0x3F) << 45)       |
                      ((transf9    & 0xF) << 36)        |
                      ((transf8    & 0xF) << 32)        |
                      ((transf7    & 0xF) << 28)        |
                      ((transf6    & 0xF) << 24)        |
                      ((transf5    & 0xF) << 20)        |
                      ((transf4    & 0xF) << 16)        |
                      ((transf3    & 0xF) << 12)        |
                      ((transf2    & 0xF) << 8)         |
                      ((transf1    & 0xF) << 4)         |
                      ((transf0    & 0xF) << 0);

   uint64_t  x31_enc = 0 ;
   __asm__ __volatile__ (
         "csrw 0x806, %[csr_enc]\n"
         TSNIP_RSV
   );
}


int main() {

	uint64_t mid, tid;
	
	mid = get_minion_id();
	tid = get_thread_id();
	if (mid != 0 && tid != 0) { C_TEST_FAIL; return -1;};
	volatile uint32_t m_11[1024] __attribute__ ((aligned (4096))) = {
			 0xa001a000,                      
			 0xa003a002,                      
			 0xa005a004,                      
			 0xa007a006,                      
			 0xa009a008,                      
			 0xa00ba00a,                      
			 0xa00da00c,                      
			 0xa00fa00e,                      
			 0xa011a010,                      
			 0xa013a012,                      
			 0xa015a014,                      
			 0xa017a016,                      
			 0xa019a018,                      
			 0xa01ba01a,                      
			 0xa01da01c,                      
			 0xa01fa01e,                      
			 0xa021a020,                      
			 0xa023a022,                      
			 0xa025a024,                      
			 0xa027a026,                      
			 0xa029a028,                      
			 0xa02ba02a,                      
			 0xa02da02c,                      
			 0xa02fa02e,                      
			 0xa031a030,                      
			 0xa033a032,                      
			 0xa035a034,                      
			 0xa037a036,                      
			 0xa039a038,                      
			 0xa03ba03a,                      
			 0xa03da03c,                      
			 0xa03fa03e,                 
			 0xa041a040,                      
			 0xa043a042,                      
			 0xa045a044,                      
			 0xa047a046,                      
			 0xa049a048,                      
			 0xa04ba04a,                      
			 0xa04da04c,                      
			 0xa04fa04e,                 
			 0xa051a050,                      
			 0xa053a052,                      
			 0xa055a054,                      
			 0xa057a056,                      
			 0xa059a058,                      
			 0xa05ba05a,                      
			 0xa05da05c,                      
			 0xa05fa05e,                 
			 0xa061a060,                      
			 0xa063a062,                      
			 0xa065a064,                      
			 0xa067a066,                      
			 0xa069a068,                      
			 0xa06ba06a,                      
			 0xa06da06c,                      
			 0xa06fa06e,                 
			 0xa071a070,                      
			 0xa073a072,                      
			 0xa075a074,                      
			 0xa077a076,                      
			 0xa079a078,                      
			 0xa07ba07a,                      
			 0xa07da07c,                      
			 0xa07fa07e,                 
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x100
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x104
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x108
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x10c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x110
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x114
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x118
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x11c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x120
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x124
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x128
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x12c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x130
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x134
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x138
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x13c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x140
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x144
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x148
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x14c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x150
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x154
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x158
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x15c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x160
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x164
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x168
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x16c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x170
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x174
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x178
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x17c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x180
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x184
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x188
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x18c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x190
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x194
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x198
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x19c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x1a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1a4
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x1a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x1b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x1b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x1bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x1c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x1c4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x1c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x1cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x1d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x1d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x1dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1e0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x1e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x1e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x1ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x1f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x1f8
			 0x7fc00001,                                                  // signaling NaN                               /// 0x1fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x200
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x204
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x208
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x20c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x210
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x214
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x218
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x21c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x220
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x224
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x228
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x22c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x230
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x234
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x238
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x23c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x240
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x244
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x248
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x24c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x250
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x254
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x258
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x25c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x260
			 0x3f028f5c,                                                  // 0.51                                        /// 0x264
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x268
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x26c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x270
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x274
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x278
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x27c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x280
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x284
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x288
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x28c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x290
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x294
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x298
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x29c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x2a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x2a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x2a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x2ac
			 0xbf028f5c,                                                  // -0.51                                       /// 0x2b0
			 0x33333333,                                                  // 4 random values                             /// 0x2b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x2b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x2bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x2c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x2c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x2c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x2d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x2d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x2d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x2e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x2ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x2f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x2f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x2f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x2fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x300
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x304
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x308
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x30c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x310
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x314
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x318
			 0xbf028f5c,                                                  // -0.51                                       /// 0x31c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x320
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x324
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x328
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x32c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x330
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x334
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x338
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x33c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x340
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x344
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x348
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x34c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x350
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x354
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x358
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x35c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x360
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x364
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x368
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x36c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x370
			 0xbf028f5c,                                                  // -0.51                                       /// 0x374
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x378
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x37c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x380
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x384
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x388
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x38c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x390
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x394
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x398
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x39c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x3a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3a8
			 0x7f800000,                                                  // inf                                         /// 0x3ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3b0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x3b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x3b8
			 0xffc00001,                                                  // -signaling NaN                              /// 0x3bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x3c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x3c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x3c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3cc
			 0x80000000,                                                  // -zero                                       /// 0x3d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x3d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x3dc
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x3e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x3e8
			 0xcb000000,                                                  // -8388608.0                                  /// 0x3ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x3f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x3f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x3fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x400
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x404
			 0xffc00001,                                                  // -signaling NaN                              /// 0x408
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x40c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x410
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x414
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x418
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x41c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x420
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x424
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x428
			 0x3f028f5c,                                                  // 0.51                                        /// 0x42c
			 0xffc00001,                                                  // -signaling NaN                              /// 0x430
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x434
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x438
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x43c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x440
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x444
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x448
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x44c
			 0x80000000,                                                  // -zero                                       /// 0x450
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x454
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x458
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x45c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x460
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x464
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x468
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x46c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x470
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x474
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x478
			 0x55555555,                                                  // 4 random values                             /// 0x47c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x480
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x484
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x488
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x48c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x490
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x494
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x498
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x49c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x4a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4a8
			 0xffc00001,                                                  // -signaling NaN                              /// 0x4ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x4b0
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x4b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x4b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x4bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4c0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x4c4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x4c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x4cc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x4d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x4d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x4dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x4e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x4e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x4e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x4ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x4f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x4f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x4f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x4fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x500
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x504
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x508
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x50c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x510
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x514
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x518
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x51c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x520
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x524
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x528
			 0x3f028f5c,                                                  // 0.51                                        /// 0x52c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x530
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x534
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x538
			 0x00000000,                                                  // zero                                        /// 0x53c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x540
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x544
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x548
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x54c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x550
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x554
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x558
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x55c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x560
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x564
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x568
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x56c
			 0x4b000000,                                                  // 8388608.0                                   /// 0x570
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x574
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x578
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x57c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x580
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x584
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x588
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x58c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x590
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x594
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x598
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x59c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x5a0
			 0x55555555,                                                  // 4 random values                             /// 0x5a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x5a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x5b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x5b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x5bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x5c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x5cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x5d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x5d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x5d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x5dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x5e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x5e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x5e8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x5f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x5f4
			 0x7fc00001,                                                  // signaling NaN                               /// 0x5f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x5fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x600
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x604
			 0x80000000,                                                  // -zero                                       /// 0x608
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x60c
			 0x7f800000,                                                  // inf                                         /// 0x610
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x614
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x618
			 0x7f800000,                                                  // inf                                         /// 0x61c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x620
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x624
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x628
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x62c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x630
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x634
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x638
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x63c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x640
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x644
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x648
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x64c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x650
			 0x3f028f5c,                                                  // 0.51                                        /// 0x654
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x658
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x65c
			 0x33333333,                                                  // 4 random values                             /// 0x660
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x664
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x668
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x66c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x670
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x674
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x678
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x67c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x680
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x684
			 0x33333333,                                                  // 4 random values                             /// 0x688
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x68c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x690
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x694
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x698
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x69c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x6a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x6ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x6b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x6b4
			 0xffc00001,                                                  // -signaling NaN                              /// 0x6b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x6bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x6c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x6c4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x6c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x6cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x6d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x6e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x6e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x6ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x6f0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x6f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x6f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x6fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x700
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x704
			 0x7f800000,                                                  // inf                                         /// 0x708
			 0x80000000,                                                  // -zero                                       /// 0x70c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x710
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x714
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x718
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x71c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x720
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x724
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x728
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x72c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x730
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x734
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x738
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x73c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x740
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x744
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x748
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x74c
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x750
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x754
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x758
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x75c
			 0x00000000,                                                  // zero                                        /// 0x760
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x764
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x768
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x76c
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x770
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x774
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x778
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x77c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x780
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x784
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x788
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x78c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x790
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x794
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x798
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x79c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x7a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x7a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7a8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x7ac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x7b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x7b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x7bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x7c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x7cc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x7d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x7d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x7d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x7dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x7e0
			 0x7fc00001,                                                  // signaling NaN                               /// 0x7e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x7e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x7ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x7f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x7f4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x7f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7fc
			 0xa001a000,                      
			 0xa003a002,                      
			 0xa005a004,                      
			 0xa007a006,                      
			 0xa009a008,                      
			 0xa00ba00a,                      
			 0xa00da00c,                      
			 0xa00fa00e,                      
			 0xa011a010,                      
			 0xa013a012,                      
			 0xa015a014,                      
			 0xa017a016,                      
			 0xa019a018,                      
			 0xa01ba01a,                      
			 0xa01da01c,                      
			 0xa01fa01e,                      
			 0xa021a020,                      
			 0xa023a022,                      
			 0xa025a024,                      
			 0xa027a026,                      
			 0xa029a028,                      
			 0xa02ba02a,                      
			 0xa02da02c,                      
			 0xa02fa02e,                      
			 0xa031a030,                      
			 0xa033a032,                      
			 0xa035a034,                      
			 0xa037a036,                      
			 0xa039a038,                      
			 0xa03ba03a,                      
			 0xa03da03c,                      
			 0xa03fa03e,                 
			 0xa041a040,                      
			 0xa043a042,                      
			 0xa045a044,                      
			 0xa047a046,                      
			 0xa049a048,                      
			 0xa04ba04a,                      
			 0xa04da04c,                      
			 0xa04fa04e,                 
			 0xa051a050,                      
			 0xa053a052,                      
			 0xa055a054,                      
			 0xa057a056,                      
			 0xa059a058,                      
			 0xa05ba05a,                      
			 0xa05da05c,                      
			 0xa05fa05e,                 
			 0xa061a060,                      
			 0xa063a062,                      
			 0xa065a064,                      
			 0xa067a066,                      
			 0xa069a068,                      
			 0xa06ba06a,                      
			 0xa06da06c,                      
			 0xa06fa06e,                 
			 0xa071a070,                      
			 0xa073a072,                      
			 0xa075a074,                      
			 0xa077a076,                      
			 0xa079a078,                      
			 0xa07ba07a,                      
			 0xa07da07c,                      
			 0xa07fa07e,                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x900
			 0xbf028f5c,                                                  // -0.51                                       /// 0x904
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x908
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x90c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x910
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x914
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x918
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x91c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x920
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x924
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x928
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x92c
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x930
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x934
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x938
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x93c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x940
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x944
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x948
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x94c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x950
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x954
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x958
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x95c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x960
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x964
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x968
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x96c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x970
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x974
			 0x80000000,                                                  // -zero                                       /// 0x978
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x97c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x980
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x984
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x988
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x98c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x990
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x994
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x998
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x99c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x9a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x9a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x9ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x9b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x9c0
			 0xbf028f5c,                                                  // -0.51                                       /// 0x9c4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x9cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x9d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x9dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x9e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x9e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x9e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x9ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x9f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x9f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x9fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xa00
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xa04
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xa08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xa0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xa10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xa14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa18
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xa1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xa20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xa24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xa28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa2c
			 0xcb000000,                                                  // -8388608.0                                  /// 0xa30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xa34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xa38
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa3c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xa40
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xa44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xa48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xa4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xa54
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xa58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xa5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xa64
			 0x33333333,                                                  // 4 random values                             /// 0xa68
			 0x4b000000,                                                  // 8388608.0                                   /// 0xa6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xa74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xa78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xa7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xa80
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xa84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xa88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xa8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xa90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xa94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xa98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xa9c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaa0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xaa4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xaa8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaac
			 0x7fc00001,                                                  // signaling NaN                               /// 0xab0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xab4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xab8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xabc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xac0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xac4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xac8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xacc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xad0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xad4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xad8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xadc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xae0
			 0x7fc00001,                                                  // signaling NaN                               /// 0xae4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xae8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xaec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xaf0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaf4
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xaf8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xafc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xb00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xb08
			 0xffc00001,                                                  // -signaling NaN                              /// 0xb0c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xb18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xb1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb20
			 0x80000000,                                                  // -zero                                       /// 0xb24
			 0x80000000,                                                  // -zero                                       /// 0xb28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xb2c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xb30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb34
			 0xff800000,                                                  // -inf                                        /// 0xb38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xb3c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xb40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xb48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xb4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xb50
			 0x80000000,                                                  // -zero                                       /// 0xb54
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xb58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xb5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xb64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xb68
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xb74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xb78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xb7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xb84
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xb88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xb8c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xb90
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xb98
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xb9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xba0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xba4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xba8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xbac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xbb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xbb4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xbb8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xbbc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xbc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xbc4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbc8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xbcc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xbd0
			 0x55555555,                                                  // 4 random values                             /// 0xbd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xbd8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xbdc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xbe0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xbe4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xbe8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xbec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xbf0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xbf4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xbf8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbfc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xc04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xc0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xc10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xc14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xc18
			 0xffc00001,                                                  // -signaling NaN                              /// 0xc1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xc24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xc28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xc30
			 0xffc00001,                                                  // -signaling NaN                              /// 0xc34
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xc38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xc40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc44
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xc48
			 0x33333333,                                                  // 4 random values                             /// 0xc4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xc54
			 0x55555555,                                                  // 4 random values                             /// 0xc58
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xc5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xc64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xc68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xc6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xc74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xc80
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xc84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xc8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xc90
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xc94
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xc9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xca0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xca4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xca8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xcb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xcb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xcbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xcc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xcc4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xcc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xccc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xcd0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xcd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xcd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xcdc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xce0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xce4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xce8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xcf0
			 0x00000000,                                                  // zero                                        /// 0xcf4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xcf8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xcfc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xd00
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xd04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xd08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xd14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xd18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xd24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xd28
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd2c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xd30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xd34
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xd38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xd3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xd40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xd48
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xd4c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xd50
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xd54
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xd58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xd64
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xd6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xd70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xd74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xd78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xd7c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xd84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd88
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xd90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xd94
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xda0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xda4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xda8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xdac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xdb0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xdb4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xdb8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xdbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xdc0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdc4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xdc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xdcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xdd0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdd4
			 0xff800000,                                                  // -inf                                        /// 0xdd8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xddc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xde0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xde4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xde8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xdec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xdf0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdf4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xdf8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xdfc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xe00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xe04
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xe08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xe0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xe10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xe14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xe18
			 0x33333333,                                                  // 4 random values                             /// 0xe1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xe20
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xe28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xe2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xe30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xe34
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xe38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xe40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xe44
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xe48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xe50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xe54
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xe58
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xe5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xe60
			 0x3f028f5c,                                                  // 0.51                                        /// 0xe64
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xe6c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xe70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xe74
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xe78
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xe7c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xe80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xe84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xe8c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xe94
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xe98
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xe9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xea0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xea4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xea8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xeac
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xeb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xeb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xeb8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xebc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xec0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xec4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xec8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xecc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xed0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xed4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xed8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xedc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xee0
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xee4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xee8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xeec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xef0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xef4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xef8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xefc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xf04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xf08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xf0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xf10
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xf14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xf18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xf1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xf20
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xf24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf28
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xf2c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xf34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xf44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xf48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xf4c
			 0x3f028f5c,                                                  // 0.51                                        /// 0xf50
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xf54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xf58
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xf60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xf64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xf68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xf6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xf70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xf74
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xf78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xf7c
			 0x7f800000,                                                  // inf                                         /// 0xf80
			 0xbf028f5c,                                                  // -0.51                                       /// 0xf84
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xf88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xf8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xf90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xf94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf98
			 0x33333333,                                                  // 4 random values                             /// 0xf9c
			 0xbf028f5c,                                                  // -0.51                                       /// 0xfa0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xfa4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xfa8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xfac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xfb0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xfb4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xfb8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xfbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xfc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xfc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xfcc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xfd0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xfd4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xfd8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xfdc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfe0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfe4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xfe8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xfec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xff0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xff4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x8a0bc87a, /// 0x0
			 0xd0fc3a10, /// 0x4
			 0x578e48ad, /// 0x8
			 0x7bef38b4, /// 0xc
			 0x43792090, /// 0x10
			 0xda9330f1, /// 0x14
			 0xdffc077a, /// 0x18
			 0x227b5011, /// 0x1c
			 0x2314d01d, /// 0x20
			 0x43a14600, /// 0x24
			 0xf537eb17, /// 0x28
			 0x9e331cf5, /// 0x2c
			 0x9db0785b, /// 0x30
			 0xdd27e83e, /// 0x34
			 0x35c61e0c, /// 0x38
			 0x4fb1bc03, /// 0x3c
			 0xbf14e764, /// 0x40
			 0x3992481a, /// 0x44
			 0xa506ca10, /// 0x48
			 0x861dcbe1, /// 0x4c
			 0xceb5b1a, /// 0x50
			 0x51db56a9, /// 0x54
			 0x10134011, /// 0x58
			 0x628d53cf, /// 0x5c
			 0x739a5daa, /// 0x60
			 0x7a432afb, /// 0x64
			 0x21ae84fb, /// 0x68
			 0xa80911fc, /// 0x6c
			 0x36bf3a6d, /// 0x70
			 0xae116884, /// 0x74
			 0x51918e2c, /// 0x78
			 0x61a9793a, /// 0x7c
			 0xe39f35ff, /// 0x80
			 0xc9872946, /// 0x84
			 0x19e2d6b8, /// 0x88
			 0x8fe46c87, /// 0x8c
			 0x7c7bac1a, /// 0x90
			 0x85ece2aa, /// 0x94
			 0xd59bd66a, /// 0x98
			 0x5a6e136a, /// 0x9c
			 0xb9a6262a, /// 0xa0
			 0xfe47ded2, /// 0xa4
			 0xe01ada91, /// 0xa8
			 0xc2ba7138, /// 0xac
			 0x3f3a3f94, /// 0xb0
			 0xe4c8cf63, /// 0xb4
			 0x664f2f1a, /// 0xb8
			 0xe3537c2e, /// 0xbc
			 0x6fb2e64c, /// 0xc0
			 0x5c9ba7d8, /// 0xc4
			 0x488cca59, /// 0xc8
			 0x4016244, /// 0xcc
			 0xab6202ef, /// 0xd0
			 0xb04deb3a, /// 0xd4
			 0xa05bac1b, /// 0xd8
			 0x553383a9, /// 0xdc
			 0xcc988c4e, /// 0xe0
			 0x575bbb39, /// 0xe4
			 0x5569826, /// 0xe8
			 0xd8ff10a2, /// 0xec
			 0x2b47277b, /// 0xf0
			 0x285dd8b1, /// 0xf4
			 0x258e86df, /// 0xf8
			 0x4adc4b49, /// 0xfc
			 0xc7198aa, /// 0x100
			 0xefed1063, /// 0x104
			 0x9267031, /// 0x108
			 0x4de02074, /// 0x10c
			 0xaf2f9252, /// 0x110
			 0x55206940, /// 0x114
			 0xa57d7aeb, /// 0x118
			 0x4c17f365, /// 0x11c
			 0xcc2aa5eb, /// 0x120
			 0x3181d675, /// 0x124
			 0xe7c5277f, /// 0x128
			 0x3a166a9a, /// 0x12c
			 0xf58ec4b9, /// 0x130
			 0x5bb6ddd2, /// 0x134
			 0xb00b5c65, /// 0x138
			 0x95a631d0, /// 0x13c
			 0xc5ada847, /// 0x140
			 0x16662c3, /// 0x144
			 0x971c1b4, /// 0x148
			 0x711019f6, /// 0x14c
			 0x5dc97818, /// 0x150
			 0x5537b8b, /// 0x154
			 0x64057820, /// 0x158
			 0x8baa7c8b, /// 0x15c
			 0xea36c7ca, /// 0x160
			 0xea5782f7, /// 0x164
			 0x91434028, /// 0x168
			 0x479b616f, /// 0x16c
			 0xb0bd8251, /// 0x170
			 0x9c38abc4, /// 0x174
			 0xafb4ab4a, /// 0x178
			 0xfc70f08d, /// 0x17c
			 0x297ded24, /// 0x180
			 0x724b373, /// 0x184
			 0xb06d23b3, /// 0x188
			 0x11d8ba04, /// 0x18c
			 0xc720ec23, /// 0x190
			 0xe81c4aa0, /// 0x194
			 0x4b3913b6, /// 0x198
			 0x82a8c064, /// 0x19c
			 0x9e50f128, /// 0x1a0
			 0xf2fb4ea9, /// 0x1a4
			 0x9781d61, /// 0x1a8
			 0xbdb8e5a0, /// 0x1ac
			 0xa2815c7, /// 0x1b0
			 0x547e67e0, /// 0x1b4
			 0xdf6d1c5b, /// 0x1b8
			 0xf423247a, /// 0x1bc
			 0x98f42c3e, /// 0x1c0
			 0x26aea8ef, /// 0x1c4
			 0xd0be569a, /// 0x1c8
			 0xbaf34bdf, /// 0x1cc
			 0xa74a7e5b, /// 0x1d0
			 0x9f6321c0, /// 0x1d4
			 0xeb2cbef0, /// 0x1d8
			 0x61024cc8, /// 0x1dc
			 0xe14e3d0c, /// 0x1e0
			 0x65093bcf, /// 0x1e4
			 0xd58e309c, /// 0x1e8
			 0xbd305092, /// 0x1ec
			 0x6385fa37, /// 0x1f0
			 0xfe257263, /// 0x1f4
			 0x69bb1369, /// 0x1f8
			 0xa67f7b6, /// 0x1fc
			 0x1fc2f654, /// 0x200
			 0x63b5edde, /// 0x204
			 0xa633be1d, /// 0x208
			 0x7160f087, /// 0x20c
			 0xb3b5d9ee, /// 0x210
			 0x91c934b6, /// 0x214
			 0x99347215, /// 0x218
			 0x46a192ec, /// 0x21c
			 0x58505754, /// 0x220
			 0x5159a363, /// 0x224
			 0xc9c370af, /// 0x228
			 0x51e9cfed, /// 0x22c
			 0x3abda800, /// 0x230
			 0x9c816921, /// 0x234
			 0xfe91a023, /// 0x238
			 0xea947d08, /// 0x23c
			 0x81f7e6cc, /// 0x240
			 0x243c906c, /// 0x244
			 0x4961ea51, /// 0x248
			 0x58c646ba, /// 0x24c
			 0x619fdc70, /// 0x250
			 0x256118f7, /// 0x254
			 0xe31d8fee, /// 0x258
			 0xe0db764, /// 0x25c
			 0xd1e4a9ec, /// 0x260
			 0x4a8be06f, /// 0x264
			 0xeebbe333, /// 0x268
			 0x97cb6e77, /// 0x26c
			 0x7c2a6c22, /// 0x270
			 0x26e60355, /// 0x274
			 0x451eb458, /// 0x278
			 0x2b91935c, /// 0x27c
			 0x9d4719d6, /// 0x280
			 0xffb7b6a9, /// 0x284
			 0xe8aaeb85, /// 0x288
			 0xc9880b01, /// 0x28c
			 0x20792abc, /// 0x290
			 0x76aa1b5f, /// 0x294
			 0x372981aa, /// 0x298
			 0x3ed19716, /// 0x29c
			 0x2eccddaa, /// 0x2a0
			 0xc7bf4f9b, /// 0x2a4
			 0xa130a9d1, /// 0x2a8
			 0x70243821, /// 0x2ac
			 0x5587eacd, /// 0x2b0
			 0xa6fbce47, /// 0x2b4
			 0xb79cff28, /// 0x2b8
			 0xe95e7d21, /// 0x2bc
			 0xe0da4e8c, /// 0x2c0
			 0xce545234, /// 0x2c4
			 0x4652a1a8, /// 0x2c8
			 0x7fcd7342, /// 0x2cc
			 0xe91ed5a8, /// 0x2d0
			 0xfd619248, /// 0x2d4
			 0xeb05218f, /// 0x2d8
			 0x44f54f8e, /// 0x2dc
			 0x67ad9d37, /// 0x2e0
			 0x25574c92, /// 0x2e4
			 0xf6ad6f0b, /// 0x2e8
			 0x61caa2f0, /// 0x2ec
			 0x7f080579, /// 0x2f0
			 0x74b680ab, /// 0x2f4
			 0x3331f281, /// 0x2f8
			 0xaaf2bcac, /// 0x2fc
			 0x26fdabf0, /// 0x300
			 0xf0604592, /// 0x304
			 0xe56adf3e, /// 0x308
			 0xbfab534c, /// 0x30c
			 0xaf9e2faa, /// 0x310
			 0x17ded805, /// 0x314
			 0x4fd24811, /// 0x318
			 0x632ba25f, /// 0x31c
			 0x4b5988c0, /// 0x320
			 0xe3c8841b, /// 0x324
			 0x3cd28237, /// 0x328
			 0x4f336ed9, /// 0x32c
			 0x38518267, /// 0x330
			 0xaf7d1c40, /// 0x334
			 0xb020d94b, /// 0x338
			 0xab6bfcd3, /// 0x33c
			 0x27861913, /// 0x340
			 0x3da01aec, /// 0x344
			 0xbc088967, /// 0x348
			 0xc54ef964, /// 0x34c
			 0x8a588b09, /// 0x350
			 0xc97746f2, /// 0x354
			 0x2ba97616, /// 0x358
			 0xa5d80a44, /// 0x35c
			 0x212d8c84, /// 0x360
			 0x102f27cd, /// 0x364
			 0x13828296, /// 0x368
			 0x89a822d1, /// 0x36c
			 0xdd3be895, /// 0x370
			 0x5dc9098a, /// 0x374
			 0x38c5d138, /// 0x378
			 0x4b6d8199, /// 0x37c
			 0xebad4a59, /// 0x380
			 0x9003f512, /// 0x384
			 0x14f978ea, /// 0x388
			 0x63e9f480, /// 0x38c
			 0x371634d6, /// 0x390
			 0xc437482a, /// 0x394
			 0x3e1a2bd1, /// 0x398
			 0x2a90ad94, /// 0x39c
			 0x4fdae2b3, /// 0x3a0
			 0xd1295e91, /// 0x3a4
			 0x1b821477, /// 0x3a8
			 0xeca513d4, /// 0x3ac
			 0x254cacb9, /// 0x3b0
			 0xe1fbe251, /// 0x3b4
			 0x72b02300, /// 0x3b8
			 0xd6be9920, /// 0x3bc
			 0x385910a6, /// 0x3c0
			 0xb44cee0, /// 0x3c4
			 0xa38289d4, /// 0x3c8
			 0xeef3787c, /// 0x3cc
			 0xb33d7f09, /// 0x3d0
			 0x13ab73f3, /// 0x3d4
			 0x49868328, /// 0x3d8
			 0xd1872d45, /// 0x3dc
			 0xe0ed2a72, /// 0x3e0
			 0xb7a3d89, /// 0x3e4
			 0xdd7f1c85, /// 0x3e8
			 0x7fdaece8, /// 0x3ec
			 0x45182d54, /// 0x3f0
			 0x51a3e8df, /// 0x3f4
			 0xece8af1d, /// 0x3f8
			 0xf3e323a6, /// 0x3fc
			 0x8bcf2a2a, /// 0x400
			 0x1192d220, /// 0x404
			 0x960f835d, /// 0x408
			 0x1f0785a1, /// 0x40c
			 0x80d4f50a, /// 0x410
			 0x1772df3, /// 0x414
			 0xdfbbe54a, /// 0x418
			 0x8f0419bf, /// 0x41c
			 0x48c7feef, /// 0x420
			 0x2a9129f5, /// 0x424
			 0x26dfdb6b, /// 0x428
			 0x74b3cc68, /// 0x42c
			 0x131113, /// 0x430
			 0x765a9605, /// 0x434
			 0x12838c16, /// 0x438
			 0x7b35eb07, /// 0x43c
			 0xbf1962f8, /// 0x440
			 0x81200165, /// 0x444
			 0xd8441e44, /// 0x448
			 0x6e6994e9, /// 0x44c
			 0x2400324d, /// 0x450
			 0x1ed565b4, /// 0x454
			 0x4e6acc36, /// 0x458
			 0x488b02ab, /// 0x45c
			 0x197a54ef, /// 0x460
			 0x71cf86d5, /// 0x464
			 0xa261070, /// 0x468
			 0x1b0f2f3c, /// 0x46c
			 0x275a147c, /// 0x470
			 0x2a974350, /// 0x474
			 0xdade0ad6, /// 0x478
			 0xb4b49324, /// 0x47c
			 0xb0803fa1, /// 0x480
			 0x1965253f, /// 0x484
			 0xfeda55f3, /// 0x488
			 0xae066e8f, /// 0x48c
			 0x74a07b0f, /// 0x490
			 0x1a91fd38, /// 0x494
			 0x40c4fffd, /// 0x498
			 0xd46d9fc8, /// 0x49c
			 0xd6ea9396, /// 0x4a0
			 0xb76c831d, /// 0x4a4
			 0xb263d164, /// 0x4a8
			 0x39d7158a, /// 0x4ac
			 0x74d44fa4, /// 0x4b0
			 0x31b66420, /// 0x4b4
			 0x46096162, /// 0x4b8
			 0xa890b934, /// 0x4bc
			 0xd06667ed, /// 0x4c0
			 0x4355c9fb, /// 0x4c4
			 0x792f097, /// 0x4c8
			 0xdd0e9dd1, /// 0x4cc
			 0xb3c54b66, /// 0x4d0
			 0x263ccc6c, /// 0x4d4
			 0x2ac0e330, /// 0x4d8
			 0xaed21a55, /// 0x4dc
			 0x9ce19d4a, /// 0x4e0
			 0x76e879b8, /// 0x4e4
			 0xbfc02ff4, /// 0x4e8
			 0x9120fa95, /// 0x4ec
			 0xfc907f42, /// 0x4f0
			 0x3ab8dbf6, /// 0x4f4
			 0x7d959d31, /// 0x4f8
			 0x30236350, /// 0x4fc
			 0xf93eb60c, /// 0x500
			 0x1d20ff8c, /// 0x504
			 0xd324a3ec, /// 0x508
			 0x2cc31b28, /// 0x50c
			 0x980e4251, /// 0x510
			 0x5f70a187, /// 0x514
			 0xbc652d4b, /// 0x518
			 0x7643dc50, /// 0x51c
			 0x788e2bbd, /// 0x520
			 0x41bbd1d1, /// 0x524
			 0x6a3935c8, /// 0x528
			 0x214350b6, /// 0x52c
			 0xeea7d092, /// 0x530
			 0xe36f674b, /// 0x534
			 0x2a117325, /// 0x538
			 0xf70efa3d, /// 0x53c
			 0x586b4152, /// 0x540
			 0x2ed4deef, /// 0x544
			 0x83cc1ae9, /// 0x548
			 0x4e34711, /// 0x54c
			 0x2be04fee, /// 0x550
			 0xc74ae8f6, /// 0x554
			 0xa63b53e0, /// 0x558
			 0xfb40035e, /// 0x55c
			 0x833d1017, /// 0x560
			 0x76c11b53, /// 0x564
			 0x50006cf5, /// 0x568
			 0x55519acd, /// 0x56c
			 0xa6b3831c, /// 0x570
			 0xcf21eadd, /// 0x574
			 0xbf51fa7, /// 0x578
			 0xfb7484fb, /// 0x57c
			 0xc53b638b, /// 0x580
			 0x88fe297c, /// 0x584
			 0x1f2a455f, /// 0x588
			 0x8b65fc97, /// 0x58c
			 0xee47bb55, /// 0x590
			 0xb503e879, /// 0x594
			 0x72bdde92, /// 0x598
			 0x62731970, /// 0x59c
			 0xb8a3855a, /// 0x5a0
			 0x2d537687, /// 0x5a4
			 0xd6f6c749, /// 0x5a8
			 0x1356b28e, /// 0x5ac
			 0xa49099b8, /// 0x5b0
			 0x4c3adbb2, /// 0x5b4
			 0x6f36d3cb, /// 0x5b8
			 0x2075a72f, /// 0x5bc
			 0xc66a1f63, /// 0x5c0
			 0x7bb3d9ab, /// 0x5c4
			 0x42ffacf6, /// 0x5c8
			 0xe9f7f4ba, /// 0x5cc
			 0x119b76fc, /// 0x5d0
			 0xab00c48, /// 0x5d4
			 0x6598737f, /// 0x5d8
			 0x1bf6ea42, /// 0x5dc
			 0x2db4f2ff, /// 0x5e0
			 0x15f5b046, /// 0x5e4
			 0xdeb08b86, /// 0x5e8
			 0xcad6bf2e, /// 0x5ec
			 0x852505af, /// 0x5f0
			 0xa8444d43, /// 0x5f4
			 0xc74651a8, /// 0x5f8
			 0xe6ba5525, /// 0x5fc
			 0x9355a889, /// 0x600
			 0x78db9b35, /// 0x604
			 0xd6c4ac7d, /// 0x608
			 0xe87ab431, /// 0x60c
			 0xda6678d9, /// 0x610
			 0xb4b3a17b, /// 0x614
			 0x8041c175, /// 0x618
			 0x7a61e17f, /// 0x61c
			 0xb49feee3, /// 0x620
			 0xd51653e9, /// 0x624
			 0xf3906c39, /// 0x628
			 0xe7ae2020, /// 0x62c
			 0x49bd58a8, /// 0x630
			 0x991074a1, /// 0x634
			 0x73eb8fcd, /// 0x638
			 0x42184986, /// 0x63c
			 0xaee72b3f, /// 0x640
			 0x938adf30, /// 0x644
			 0x7bc84ed9, /// 0x648
			 0x17bb361, /// 0x64c
			 0x9a576814, /// 0x650
			 0x1a2050cc, /// 0x654
			 0xb18fba57, /// 0x658
			 0xa3aafe25, /// 0x65c
			 0x84cd4b64, /// 0x660
			 0xbb6982f2, /// 0x664
			 0x239637b0, /// 0x668
			 0x34295eff, /// 0x66c
			 0x4bbd6767, /// 0x670
			 0x6959baa0, /// 0x674
			 0x7179a648, /// 0x678
			 0x2863fef8, /// 0x67c
			 0x4dff6c72, /// 0x680
			 0x66d50e76, /// 0x684
			 0x7493487b, /// 0x688
			 0x32f18c3, /// 0x68c
			 0x6108e265, /// 0x690
			 0x67008074, /// 0x694
			 0xe8f836a4, /// 0x698
			 0x5f3f4f40, /// 0x69c
			 0xa4c9ed88, /// 0x6a0
			 0xc7a2302a, /// 0x6a4
			 0x1768730b, /// 0x6a8
			 0x540f0d0a, /// 0x6ac
			 0x50f11387, /// 0x6b0
			 0x5d30057b, /// 0x6b4
			 0xa5030817, /// 0x6b8
			 0x174ceb83, /// 0x6bc
			 0x7c994335, /// 0x6c0
			 0x67792b42, /// 0x6c4
			 0xfad6a660, /// 0x6c8
			 0x431f87c4, /// 0x6cc
			 0x6b7cdd1a, /// 0x6d0
			 0x8f4b16b, /// 0x6d4
			 0x58d48b07, /// 0x6d8
			 0xc35a9534, /// 0x6dc
			 0x92c5d069, /// 0x6e0
			 0x64648537, /// 0x6e4
			 0x990e8ed9, /// 0x6e8
			 0xbb87624a, /// 0x6ec
			 0x2c223eba, /// 0x6f0
			 0x431998bd, /// 0x6f4
			 0x851085d9, /// 0x6f8
			 0x2011f580, /// 0x6fc
			 0x1b590e3f, /// 0x700
			 0xa5cd00f3, /// 0x704
			 0x42ddaddf, /// 0x708
			 0x4fb5b7d8, /// 0x70c
			 0x9515196f, /// 0x710
			 0x66dfab12, /// 0x714
			 0x8726883c, /// 0x718
			 0xa3c4b0e1, /// 0x71c
			 0x12314c64, /// 0x720
			 0xbbbb4b74, /// 0x724
			 0xdd1bf3d1, /// 0x728
			 0xf3f5c77b, /// 0x72c
			 0x86395dbc, /// 0x730
			 0xc343e055, /// 0x734
			 0x34933be9, /// 0x738
			 0x97a025b, /// 0x73c
			 0xa9ab9614, /// 0x740
			 0x97baa6f4, /// 0x744
			 0xbc9ed9bf, /// 0x748
			 0x69d048d0, /// 0x74c
			 0xb8a7ec9e, /// 0x750
			 0xd91fab47, /// 0x754
			 0xea9a61e8, /// 0x758
			 0x1a726fdc, /// 0x75c
			 0x3d50bec1, /// 0x760
			 0x9a38400a, /// 0x764
			 0x271721a3, /// 0x768
			 0x2b2a4d64, /// 0x76c
			 0x5cd91597, /// 0x770
			 0x8344e1a1, /// 0x774
			 0xcc28cc74, /// 0x778
			 0x5e1b9fa8, /// 0x77c
			 0x34c6a39a, /// 0x780
			 0x844439f8, /// 0x784
			 0x3a264101, /// 0x788
			 0xb37e263a, /// 0x78c
			 0x320f165e, /// 0x790
			 0x44537126, /// 0x794
			 0x8f5daa18, /// 0x798
			 0xe9e8a045, /// 0x79c
			 0xf9cbde5f, /// 0x7a0
			 0x311fbb4d, /// 0x7a4
			 0x67886c22, /// 0x7a8
			 0x8b1d6d12, /// 0x7ac
			 0x4a95599a, /// 0x7b0
			 0x41fafe0b, /// 0x7b4
			 0x58c5cb03, /// 0x7b8
			 0x51d26faa, /// 0x7bc
			 0x54f89040, /// 0x7c0
			 0x2fe219cc, /// 0x7c4
			 0xddfb2ea8, /// 0x7c8
			 0xe77efe6b, /// 0x7cc
			 0x92a4ea7f, /// 0x7d0
			 0x467ed93c, /// 0x7d4
			 0x9d43b1dc, /// 0x7d8
			 0xa9bc81d8, /// 0x7dc
			 0xb1a6fc66, /// 0x7e0
			 0xf72bec89, /// 0x7e4
			 0x9066d5f0, /// 0x7e8
			 0xd3a86cab, /// 0x7ec
			 0xd8aef08a, /// 0x7f0
			 0x1eae0885, /// 0x7f4
			 0xa4976907, /// 0x7f8
			 0xf37b4a24, /// 0x7fc
			 0x47a9c1cd, /// 0x800
			 0x270f5c70, /// 0x804
			 0xa4f75649, /// 0x808
			 0xb4dfa34c, /// 0x80c
			 0xb2e59427, /// 0x810
			 0x343e144d, /// 0x814
			 0x3fd4f22, /// 0x818
			 0x4df3d6fe, /// 0x81c
			 0x333f2576, /// 0x820
			 0x37f07030, /// 0x824
			 0xec02fc89, /// 0x828
			 0xb20b62d0, /// 0x82c
			 0x767488ec, /// 0x830
			 0xcac3b0c7, /// 0x834
			 0x52ceb8a0, /// 0x838
			 0x3d8e3916, /// 0x83c
			 0xe969485c, /// 0x840
			 0x783af661, /// 0x844
			 0x168acf07, /// 0x848
			 0x546f4a5, /// 0x84c
			 0xe0c3303e, /// 0x850
			 0xedd48aa7, /// 0x854
			 0xdf585160, /// 0x858
			 0xf7bc2dc3, /// 0x85c
			 0xcae3216c, /// 0x860
			 0xee7ebd1f, /// 0x864
			 0x396035a8, /// 0x868
			 0xac266d25, /// 0x86c
			 0xce74edc5, /// 0x870
			 0xabd7d5dc, /// 0x874
			 0x3ab32519, /// 0x878
			 0xf49c6fc0, /// 0x87c
			 0xa37b3d, /// 0x880
			 0xd3010447, /// 0x884
			 0x949de148, /// 0x888
			 0x872aea98, /// 0x88c
			 0xd5b5c617, /// 0x890
			 0xc7c6e671, /// 0x894
			 0x90e4ea71, /// 0x898
			 0xe79658d8, /// 0x89c
			 0x173071c, /// 0x8a0
			 0xb6eceaa6, /// 0x8a4
			 0xcd4146c2, /// 0x8a8
			 0xa4207261, /// 0x8ac
			 0x37fc54f6, /// 0x8b0
			 0xe584732b, /// 0x8b4
			 0x233ac767, /// 0x8b8
			 0xecca9ea4, /// 0x8bc
			 0xfd19f80, /// 0x8c0
			 0x3948c7a5, /// 0x8c4
			 0x1efb1d1c, /// 0x8c8
			 0x59363023, /// 0x8cc
			 0x13e1942c, /// 0x8d0
			 0x66a9b7b0, /// 0x8d4
			 0xeb68d5ef, /// 0x8d8
			 0x78378116, /// 0x8dc
			 0x41c0157d, /// 0x8e0
			 0x6177121b, /// 0x8e4
			 0xaf67153d, /// 0x8e8
			 0xacc5c258, /// 0x8ec
			 0x7040d9b8, /// 0x8f0
			 0x64417928, /// 0x8f4
			 0x828cc724, /// 0x8f8
			 0x673a677d, /// 0x8fc
			 0x8ffb0392, /// 0x900
			 0x8e101914, /// 0x904
			 0xf11dfee0, /// 0x908
			 0x4927f6f8, /// 0x90c
			 0xe28485b3, /// 0x910
			 0xf2809cf, /// 0x914
			 0xd3cd0366, /// 0x918
			 0xb0a98dc5, /// 0x91c
			 0x99c6b015, /// 0x920
			 0x2f2f838e, /// 0x924
			 0x289bfe58, /// 0x928
			 0x8e4c196e, /// 0x92c
			 0x1aa32b90, /// 0x930
			 0x8fb16a97, /// 0x934
			 0xebd01f1b, /// 0x938
			 0x519a1604, /// 0x93c
			 0xef356480, /// 0x940
			 0xd25b1c71, /// 0x944
			 0x19a56d11, /// 0x948
			 0x3001886c, /// 0x94c
			 0xf103d759, /// 0x950
			 0x671fd1e5, /// 0x954
			 0xa355463c, /// 0x958
			 0x5971d55e, /// 0x95c
			 0xe8fc0e0b, /// 0x960
			 0xcf8d073f, /// 0x964
			 0x8343f097, /// 0x968
			 0x70449309, /// 0x96c
			 0x3bb5fff8, /// 0x970
			 0x403c6af6, /// 0x974
			 0xa3253efc, /// 0x978
			 0x319e6245, /// 0x97c
			 0xaa00532, /// 0x980
			 0xc0e4f733, /// 0x984
			 0xefcae1f0, /// 0x988
			 0xd19ae008, /// 0x98c
			 0xb449aeeb, /// 0x990
			 0x5e584e, /// 0x994
			 0x28bb2c98, /// 0x998
			 0x81859811, /// 0x99c
			 0x837dc963, /// 0x9a0
			 0x4a93d622, /// 0x9a4
			 0x15acb762, /// 0x9a8
			 0x828dd186, /// 0x9ac
			 0x1c9b4952, /// 0x9b0
			 0x7b87cd65, /// 0x9b4
			 0x235026c2, /// 0x9b8
			 0xe878d4a4, /// 0x9bc
			 0x6e93e489, /// 0x9c0
			 0x61749121, /// 0x9c4
			 0xef1618d2, /// 0x9c8
			 0x3c40189a, /// 0x9cc
			 0x1029efc6, /// 0x9d0
			 0x8ba2a9b5, /// 0x9d4
			 0x1bde8dbb, /// 0x9d8
			 0x8f3841b7, /// 0x9dc
			 0xb6e284e2, /// 0x9e0
			 0x37ff27fe, /// 0x9e4
			 0x4c2aaca3, /// 0x9e8
			 0x3b47c398, /// 0x9ec
			 0x95a07c60, /// 0x9f0
			 0xb63f2ded, /// 0x9f4
			 0xbef6a7eb, /// 0x9f8
			 0xe6e514bf, /// 0x9fc
			 0x491dbc9c, /// 0xa00
			 0x30be9086, /// 0xa04
			 0x3acc1595, /// 0xa08
			 0xb912fa2b, /// 0xa0c
			 0x9f0a866d, /// 0xa10
			 0x1ea328ae, /// 0xa14
			 0xf5beec22, /// 0xa18
			 0x58d7ea77, /// 0xa1c
			 0x397093ff, /// 0xa20
			 0xe7391eb, /// 0xa24
			 0xcb01e273, /// 0xa28
			 0x420d498b, /// 0xa2c
			 0xb251dd20, /// 0xa30
			 0x87e45fcf, /// 0xa34
			 0x2783a6e2, /// 0xa38
			 0xfb58af78, /// 0xa3c
			 0x14aca2ef, /// 0xa40
			 0xa7d850e9, /// 0xa44
			 0x82f0c7a, /// 0xa48
			 0xf7fd3606, /// 0xa4c
			 0x32041f76, /// 0xa50
			 0x2f0f1e25, /// 0xa54
			 0xac303f73, /// 0xa58
			 0x7e698479, /// 0xa5c
			 0x12e94191, /// 0xa60
			 0xf93f0937, /// 0xa64
			 0xe5e58c8f, /// 0xa68
			 0x4f64e321, /// 0xa6c
			 0x60c947ec, /// 0xa70
			 0x53980a60, /// 0xa74
			 0xf210dfff, /// 0xa78
			 0x61ee9f93, /// 0xa7c
			 0xf9e1d5c3, /// 0xa80
			 0x122587ae, /// 0xa84
			 0x37393077, /// 0xa88
			 0xc9f19099, /// 0xa8c
			 0xe23fb146, /// 0xa90
			 0x917130f, /// 0xa94
			 0x207e0fa6, /// 0xa98
			 0x2b3d270e, /// 0xa9c
			 0xeb944ea3, /// 0xaa0
			 0xc29a37f5, /// 0xaa4
			 0x9f5e91c6, /// 0xaa8
			 0xfedaaca1, /// 0xaac
			 0xecbcd1b5, /// 0xab0
			 0xa381a5e6, /// 0xab4
			 0x38395633, /// 0xab8
			 0x54cbcc29, /// 0xabc
			 0x3c0f9f3e, /// 0xac0
			 0x45b4c64, /// 0xac4
			 0xa8749cb, /// 0xac8
			 0x8c86d066, /// 0xacc
			 0xd96fee83, /// 0xad0
			 0x50685ce2, /// 0xad4
			 0x9e7f6f17, /// 0xad8
			 0xa466cffb, /// 0xadc
			 0xee86df01, /// 0xae0
			 0x6cf827a6, /// 0xae4
			 0x37645af4, /// 0xae8
			 0xa6bfb777, /// 0xaec
			 0xc523234, /// 0xaf0
			 0x7cbe46de, /// 0xaf4
			 0xb0716dc6, /// 0xaf8
			 0xfc8fc63e, /// 0xafc
			 0x2f8720f3, /// 0xb00
			 0xdeb6fe13, /// 0xb04
			 0x1e90b3b8, /// 0xb08
			 0xb17f9468, /// 0xb0c
			 0xc579bf07, /// 0xb10
			 0x63e234ae, /// 0xb14
			 0x99a2387, /// 0xb18
			 0x945ae95d, /// 0xb1c
			 0xdd6029e0, /// 0xb20
			 0x4a9b26b7, /// 0xb24
			 0x2c92cd81, /// 0xb28
			 0xd3bb8c08, /// 0xb2c
			 0x9706d15e, /// 0xb30
			 0xf21360b1, /// 0xb34
			 0x3c2908a0, /// 0xb38
			 0x3a7e19fb, /// 0xb3c
			 0x17b0cc6f, /// 0xb40
			 0x5fb07b30, /// 0xb44
			 0xe46086cc, /// 0xb48
			 0x391222e2, /// 0xb4c
			 0x44da3f0b, /// 0xb50
			 0x57f1ebe4, /// 0xb54
			 0x8e95c336, /// 0xb58
			 0x203398fc, /// 0xb5c
			 0xf180e0, /// 0xb60
			 0x187bf5e1, /// 0xb64
			 0x9a80de09, /// 0xb68
			 0xa401b008, /// 0xb6c
			 0x4f8f7a70, /// 0xb70
			 0x5ee2b121, /// 0xb74
			 0x1eea1e12, /// 0xb78
			 0x2f26040f, /// 0xb7c
			 0x9b7b8135, /// 0xb80
			 0x554e5212, /// 0xb84
			 0xa15cbabb, /// 0xb88
			 0xdc476cf1, /// 0xb8c
			 0xf8687440, /// 0xb90
			 0xb6c6a43b, /// 0xb94
			 0x5a0bc60e, /// 0xb98
			 0x710078a4, /// 0xb9c
			 0xfbe78477, /// 0xba0
			 0xa5abdf51, /// 0xba4
			 0xe8df0895, /// 0xba8
			 0x7f57d0b2, /// 0xbac
			 0x5701429c, /// 0xbb0
			 0xd67d19c4, /// 0xbb4
			 0xe37f666, /// 0xbb8
			 0x47e01f5a, /// 0xbbc
			 0x9eebffaf, /// 0xbc0
			 0xdaf97757, /// 0xbc4
			 0x754dc475, /// 0xbc8
			 0x86a49c6e, /// 0xbcc
			 0x2f522090, /// 0xbd0
			 0x7491540, /// 0xbd4
			 0x69f535b8, /// 0xbd8
			 0x2f4c97ce, /// 0xbdc
			 0x7d61d6f3, /// 0xbe0
			 0x48097086, /// 0xbe4
			 0x6fddd6a6, /// 0xbe8
			 0x63b495d2, /// 0xbec
			 0xa0fa63a5, /// 0xbf0
			 0xad2c38cd, /// 0xbf4
			 0xb4232daa, /// 0xbf8
			 0x1fe1b1d0, /// 0xbfc
			 0x22d18a6d, /// 0xc00
			 0xcafb51e0, /// 0xc04
			 0xf5ff0a40, /// 0xc08
			 0x2aae2f43, /// 0xc0c
			 0x69ff5ef5, /// 0xc10
			 0xed3861c0, /// 0xc14
			 0xcfca96b7, /// 0xc18
			 0xb3d10789, /// 0xc1c
			 0x52b4b210, /// 0xc20
			 0x175cf5f6, /// 0xc24
			 0xc672c653, /// 0xc28
			 0x7c43bcbb, /// 0xc2c
			 0x56d5cfdb, /// 0xc30
			 0x5c96c57a, /// 0xc34
			 0x94755dec, /// 0xc38
			 0xc105f582, /// 0xc3c
			 0x2301a3e1, /// 0xc40
			 0x9a662937, /// 0xc44
			 0xcabfc6bd, /// 0xc48
			 0xfdf93912, /// 0xc4c
			 0x2b7355eb, /// 0xc50
			 0x48c2a7ef, /// 0xc54
			 0xe1ce7ec2, /// 0xc58
			 0xdaf56d1f, /// 0xc5c
			 0x8e89d879, /// 0xc60
			 0xb6ba7712, /// 0xc64
			 0x18ddc356, /// 0xc68
			 0xbae6a033, /// 0xc6c
			 0x100e3501, /// 0xc70
			 0xc278d802, /// 0xc74
			 0x69b935e3, /// 0xc78
			 0x503215eb, /// 0xc7c
			 0x39342d13, /// 0xc80
			 0x45ebd169, /// 0xc84
			 0x729fdee6, /// 0xc88
			 0x9950b50b, /// 0xc8c
			 0x989350b0, /// 0xc90
			 0xd45938e3, /// 0xc94
			 0x76b4ae5, /// 0xc98
			 0x7f570b47, /// 0xc9c
			 0x3c114622, /// 0xca0
			 0x477a3716, /// 0xca4
			 0xe8322f15, /// 0xca8
			 0x29defcec, /// 0xcac
			 0x578591b8, /// 0xcb0
			 0x85680f21, /// 0xcb4
			 0x8c7a6b5f, /// 0xcb8
			 0x166a14b6, /// 0xcbc
			 0xee2492c6, /// 0xcc0
			 0xabc15d03, /// 0xcc4
			 0x21bbf724, /// 0xcc8
			 0xe069a90c, /// 0xccc
			 0xb45328c9, /// 0xcd0
			 0xbc33b09, /// 0xcd4
			 0xcb0b9782, /// 0xcd8
			 0xb26df54, /// 0xcdc
			 0x482158d, /// 0xce0
			 0xc29fc2e5, /// 0xce4
			 0x59bb07bd, /// 0xce8
			 0x47394527, /// 0xcec
			 0xbea520c5, /// 0xcf0
			 0x4abb5321, /// 0xcf4
			 0x57c766b3, /// 0xcf8
			 0x45b96336, /// 0xcfc
			 0x4c8ab4d6, /// 0xd00
			 0x7578c65f, /// 0xd04
			 0xd51006bc, /// 0xd08
			 0xf61ff707, /// 0xd0c
			 0xc3e03563, /// 0xd10
			 0xbbebe778, /// 0xd14
			 0xd8e5ddbf, /// 0xd18
			 0x6e4aa0ea, /// 0xd1c
			 0x35bbf42c, /// 0xd20
			 0xfa08cb62, /// 0xd24
			 0xf0d469e, /// 0xd28
			 0x5daed70d, /// 0xd2c
			 0x51f70c91, /// 0xd30
			 0x46cca5aa, /// 0xd34
			 0xf83b759c, /// 0xd38
			 0x6dfb18e8, /// 0xd3c
			 0xec37a63c, /// 0xd40
			 0x216a0a53, /// 0xd44
			 0xba5e8f1b, /// 0xd48
			 0xcaf7c6ef, /// 0xd4c
			 0x37e7723b, /// 0xd50
			 0x42829df2, /// 0xd54
			 0xedc05584, /// 0xd58
			 0x3b3b1dec, /// 0xd5c
			 0xdb919a9c, /// 0xd60
			 0x89ecfbe9, /// 0xd64
			 0x2dd130d1, /// 0xd68
			 0x34f9cecc, /// 0xd6c
			 0xc97a1928, /// 0xd70
			 0xa8cfce29, /// 0xd74
			 0x37bfddd1, /// 0xd78
			 0x50cacf58, /// 0xd7c
			 0x8094e7b8, /// 0xd80
			 0x7747965d, /// 0xd84
			 0x49a6dd44, /// 0xd88
			 0x52182b46, /// 0xd8c
			 0x6b71e817, /// 0xd90
			 0x613a64ec, /// 0xd94
			 0xc096394a, /// 0xd98
			 0xa173cd32, /// 0xd9c
			 0x5397f5ed, /// 0xda0
			 0x57ed7b65, /// 0xda4
			 0x2dc63935, /// 0xda8
			 0x819df059, /// 0xdac
			 0xb9854d70, /// 0xdb0
			 0xb8a34ac9, /// 0xdb4
			 0xc105f0e1, /// 0xdb8
			 0x2426728d, /// 0xdbc
			 0x3d0e2ff, /// 0xdc0
			 0x8eca748c, /// 0xdc4
			 0x145e8988, /// 0xdc8
			 0xba26443e, /// 0xdcc
			 0xa682fb58, /// 0xdd0
			 0x3f73486c, /// 0xdd4
			 0xd323e45a, /// 0xdd8
			 0x757ea7c2, /// 0xddc
			 0xd4d2d126, /// 0xde0
			 0xb3ca1cac, /// 0xde4
			 0x453fe265, /// 0xde8
			 0x52405472, /// 0xdec
			 0x160ec099, /// 0xdf0
			 0xc64a0741, /// 0xdf4
			 0x8db5707d, /// 0xdf8
			 0x84be9b00, /// 0xdfc
			 0xe0d2d3aa, /// 0xe00
			 0xe8a849be, /// 0xe04
			 0x1124843f, /// 0xe08
			 0xb714f0ce, /// 0xe0c
			 0x5034c56c, /// 0xe10
			 0x78015624, /// 0xe14
			 0x3c661da, /// 0xe18
			 0xd93d630e, /// 0xe1c
			 0x26b4ed9e, /// 0xe20
			 0x18b9dab0, /// 0xe24
			 0x3cfb7a4b, /// 0xe28
			 0xd34a2a68, /// 0xe2c
			 0x30ddfd59, /// 0xe30
			 0xbfc8a4e, /// 0xe34
			 0xc5087245, /// 0xe38
			 0x4f1b335a, /// 0xe3c
			 0x94f6921a, /// 0xe40
			 0x99110007, /// 0xe44
			 0xd1f8cc97, /// 0xe48
			 0x3b9a2e20, /// 0xe4c
			 0x39b1d8af, /// 0xe50
			 0xc843941, /// 0xe54
			 0xcea06002, /// 0xe58
			 0x89400c7, /// 0xe5c
			 0x90f4ed87, /// 0xe60
			 0x4c30cba3, /// 0xe64
			 0x41ed2dde, /// 0xe68
			 0x37993853, /// 0xe6c
			 0x393d1661, /// 0xe70
			 0xe6f1f82e, /// 0xe74
			 0xafa6cbae, /// 0xe78
			 0xb8e06781, /// 0xe7c
			 0x76d08306, /// 0xe80
			 0x50303d5, /// 0xe84
			 0x56eb490c, /// 0xe88
			 0xae3a0b86, /// 0xe8c
			 0xb3c8cfd, /// 0xe90
			 0x17c79fbe, /// 0xe94
			 0x43dcab4d, /// 0xe98
			 0xff383e26, /// 0xe9c
			 0x2748c6be, /// 0xea0
			 0xb65336f, /// 0xea4
			 0x11f8b42a, /// 0xea8
			 0x311edd43, /// 0xeac
			 0x743a52b4, /// 0xeb0
			 0xc1eadfab, /// 0xeb4
			 0xcd7f21bc, /// 0xeb8
			 0x9263d1de, /// 0xebc
			 0xb1982db5, /// 0xec0
			 0x3bbe3475, /// 0xec4
			 0x39e0ce3e, /// 0xec8
			 0xf6ef25b, /// 0xecc
			 0xc54ebd72, /// 0xed0
			 0xb674ecc6, /// 0xed4
			 0xeacaeeaf, /// 0xed8
			 0x3a951ec8, /// 0xedc
			 0xa29f0460, /// 0xee0
			 0xf64c0d80, /// 0xee4
			 0x965bda16, /// 0xee8
			 0xcea90db1, /// 0xeec
			 0xf80c5470, /// 0xef0
			 0x6ca43379, /// 0xef4
			 0x9bc7cadb, /// 0xef8
			 0x95432eb0, /// 0xefc
			 0x3c875ed5, /// 0xf00
			 0x9395d6e2, /// 0xf04
			 0xe09882bb, /// 0xf08
			 0x65d5045f, /// 0xf0c
			 0x5e7e0d03, /// 0xf10
			 0xab19374e, /// 0xf14
			 0x79bdce77, /// 0xf18
			 0x33fe7ed0, /// 0xf1c
			 0x1f58e330, /// 0xf20
			 0xbb6a606b, /// 0xf24
			 0xcb24b3b4, /// 0xf28
			 0xb914c07d, /// 0xf2c
			 0x149f45d3, /// 0xf30
			 0x661735e, /// 0xf34
			 0x7fb0b8ba, /// 0xf38
			 0xd2f35bc6, /// 0xf3c
			 0xd4d47900, /// 0xf40
			 0x1568a8d0, /// 0xf44
			 0xb37a63f6, /// 0xf48
			 0x7abf382a, /// 0xf4c
			 0x40b57549, /// 0xf50
			 0x1ff7d678, /// 0xf54
			 0x80433c4a, /// 0xf58
			 0xc656c394, /// 0xf5c
			 0x5215b1e8, /// 0xf60
			 0x79a72545, /// 0xf64
			 0x2e3ac160, /// 0xf68
			 0xf7b2a3ae, /// 0xf6c
			 0xa67837b6, /// 0xf70
			 0x57fcf4e6, /// 0xf74
			 0xe48fea70, /// 0xf78
			 0x4f24809a, /// 0xf7c
			 0xaac76ec7, /// 0xf80
			 0xedf2b905, /// 0xf84
			 0xf075c1ef, /// 0xf88
			 0xc010e28c, /// 0xf8c
			 0x27c4a64d, /// 0xf90
			 0x8e8d7765, /// 0xf94
			 0x5a78d8bd, /// 0xf98
			 0x887c0e25, /// 0xf9c
			 0x5bda6f15, /// 0xfa0
			 0x83c7e4fa, /// 0xfa4
			 0x72002fce, /// 0xfa8
			 0xc0e776a5, /// 0xfac
			 0xeb733f13, /// 0xfb0
			 0xcae713b8, /// 0xfb4
			 0xe94895bc, /// 0xfb8
			 0xd55b69c3, /// 0xfbc
			 0xce7c56bf, /// 0xfc0
			 0xc327ec66, /// 0xfc4
			 0x6c98f612, /// 0xfc8
			 0x7cea512c, /// 0xfcc
			 0x2c3ffcb2, /// 0xfd0
			 0x23dc9a8b, /// 0xfd4
			 0x5dd39371, /// 0xfd8
			 0x8cdbc03c, /// 0xfdc
			 0xb3eb8b28, /// 0xfe0
			 0x451c32f, /// 0xfe4
			 0x59f68569, /// 0xfe8
			 0x28dc18c1, /// 0xfec
			 0x732a92a6, /// 0xff0
			 0x31298b39, /// 0xff4
			 0xb681989c, /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x5247d5e7, /// 0x0
			 0x83c00fe, /// 0x4
			 0xa4f350c1, /// 0x8
			 0x53da466f, /// 0xc
			 0x66637e24, /// 0x10
			 0x16e91d9c, /// 0x14
			 0x87750b2b, /// 0x18
			 0x2d2150dc, /// 0x1c
			 0x1f917819, /// 0x20
			 0x5f25aaa5, /// 0x24
			 0xdc295f02, /// 0x28
			 0xf30cc5bf, /// 0x2c
			 0x2a6474fe, /// 0x30
			 0x91f8f35e, /// 0x34
			 0x276cd1ac, /// 0x38
			 0x4e09b45d, /// 0x3c
			 0x7eb0ad96, /// 0x40
			 0xa063ae42, /// 0x44
			 0x8a5779c4, /// 0x48
			 0x41d8527e, /// 0x4c
			 0xdb1249a9, /// 0x50
			 0x5992cd6a, /// 0x54
			 0x4c5cb59f, /// 0x58
			 0x67cbc819, /// 0x5c
			 0xef9d45a2, /// 0x60
			 0x6c80f449, /// 0x64
			 0xe32127ed, /// 0x68
			 0x1c463d4b, /// 0x6c
			 0xaf2dd3f7, /// 0x70
			 0x498a411f, /// 0x74
			 0x5f8b1848, /// 0x78
			 0x8369ccc1, /// 0x7c
			 0xcabc2d15, /// 0x80
			 0x6e193542, /// 0x84
			 0xc13be9fd, /// 0x88
			 0x647bb839, /// 0x8c
			 0xc17add8f, /// 0x90
			 0x31949d4d, /// 0x94
			 0x84d6a06d, /// 0x98
			 0x4b2e1c22, /// 0x9c
			 0x61d6ef8d, /// 0xa0
			 0x97c58f6b, /// 0xa4
			 0x2cba01d, /// 0xa8
			 0x51b15202, /// 0xac
			 0x2cc87b9f, /// 0xb0
			 0xada087e6, /// 0xb4
			 0x38bf651, /// 0xb8
			 0xbd0aa34b, /// 0xbc
			 0xf43df641, /// 0xc0
			 0xc96ce5f8, /// 0xc4
			 0x36b5f78d, /// 0xc8
			 0xeb5b0c3f, /// 0xcc
			 0xab935f2a, /// 0xd0
			 0xe1f247e9, /// 0xd4
			 0x62ea4176, /// 0xd8
			 0x19350077, /// 0xdc
			 0xc44a9f20, /// 0xe0
			 0x35acc69d, /// 0xe4
			 0x3f11436e, /// 0xe8
			 0xb641af0b, /// 0xec
			 0x919b5e1c, /// 0xf0
			 0x44538676, /// 0xf4
			 0xa1f3cf4c, /// 0xf8
			 0xad63840a, /// 0xfc
			 0x9c32d7ef, /// 0x100
			 0xd8116712, /// 0x104
			 0x66b2b743, /// 0x108
			 0x753d444e, /// 0x10c
			 0x470e5233, /// 0x110
			 0x72bce64c, /// 0x114
			 0xf2b7b360, /// 0x118
			 0xf827f830, /// 0x11c
			 0x3511ae5d, /// 0x120
			 0x7142aa0e, /// 0x124
			 0x2c3553d4, /// 0x128
			 0xa26a50e8, /// 0x12c
			 0xcdc77ec2, /// 0x130
			 0xa77ef488, /// 0x134
			 0x161ac182, /// 0x138
			 0xd82810d8, /// 0x13c
			 0xf65f35ea, /// 0x140
			 0xe4544bb8, /// 0x144
			 0xffc1b124, /// 0x148
			 0x7ac141e0, /// 0x14c
			 0xab2d5df0, /// 0x150
			 0xb2902542, /// 0x154
			 0xfcf3ec7e, /// 0x158
			 0x3bcf6d94, /// 0x15c
			 0x4ff80af6, /// 0x160
			 0xf53df1b6, /// 0x164
			 0x7c4df4b5, /// 0x168
			 0xb9dc7ded, /// 0x16c
			 0xf3f04cac, /// 0x170
			 0x2c8da684, /// 0x174
			 0x95f040b7, /// 0x178
			 0x7ba7d860, /// 0x17c
			 0x3e7797a6, /// 0x180
			 0x85848e94, /// 0x184
			 0x8431e2ef, /// 0x188
			 0xbcccdc4a, /// 0x18c
			 0xd27992e7, /// 0x190
			 0x54ccfa1c, /// 0x194
			 0x9548a416, /// 0x198
			 0x865ece7e, /// 0x19c
			 0xa3e5f1d8, /// 0x1a0
			 0x8ca5189b, /// 0x1a4
			 0xf473dbbe, /// 0x1a8
			 0xd33b47e7, /// 0x1ac
			 0x9026b4f4, /// 0x1b0
			 0x463d5fcb, /// 0x1b4
			 0xd8062bde, /// 0x1b8
			 0xe0cb4f2e, /// 0x1bc
			 0xef579ea5, /// 0x1c0
			 0xef930ccc, /// 0x1c4
			 0xda6e199c, /// 0x1c8
			 0x334f02ea, /// 0x1cc
			 0x96d3d940, /// 0x1d0
			 0xb3529206, /// 0x1d4
			 0xc4c0beed, /// 0x1d8
			 0xbdeb4345, /// 0x1dc
			 0x1ad80609, /// 0x1e0
			 0x9d7c5567, /// 0x1e4
			 0xbaa2efe2, /// 0x1e8
			 0xec123649, /// 0x1ec
			 0x66d7fff6, /// 0x1f0
			 0x32bb590a, /// 0x1f4
			 0xde461b70, /// 0x1f8
			 0xf07b8e98, /// 0x1fc
			 0x57f641ab, /// 0x200
			 0x2b94b809, /// 0x204
			 0xf1570bfe, /// 0x208
			 0x87293a74, /// 0x20c
			 0x4faab207, /// 0x210
			 0x7242c821, /// 0x214
			 0xd96e36d0, /// 0x218
			 0x7b4e39d7, /// 0x21c
			 0x2bd72a09, /// 0x220
			 0xe72e32d5, /// 0x224
			 0xa84e7970, /// 0x228
			 0xd927b90b, /// 0x22c
			 0x6aa70f0d, /// 0x230
			 0xfc57e506, /// 0x234
			 0x4f852cad, /// 0x238
			 0xc1b771fe, /// 0x23c
			 0xf6632701, /// 0x240
			 0x9a7ed0e8, /// 0x244
			 0xa772549f, /// 0x248
			 0x9d99a2a0, /// 0x24c
			 0xb8ae914e, /// 0x250
			 0x994bdbb2, /// 0x254
			 0x5a1da932, /// 0x258
			 0x35f0827d, /// 0x25c
			 0x28d09e94, /// 0x260
			 0x7bdb6c74, /// 0x264
			 0xd1532b2d, /// 0x268
			 0x6b8e6c84, /// 0x26c
			 0xc59a9a3, /// 0x270
			 0xbdfe148b, /// 0x274
			 0x4e9ccbc1, /// 0x278
			 0x58f13d70, /// 0x27c
			 0x8a90b051, /// 0x280
			 0x620871c7, /// 0x284
			 0x96fdc58c, /// 0x288
			 0x2cab842d, /// 0x28c
			 0x63d68874, /// 0x290
			 0xa573727f, /// 0x294
			 0x698da8b9, /// 0x298
			 0xaa19eabd, /// 0x29c
			 0xa468ab9a, /// 0x2a0
			 0xd6c61ab1, /// 0x2a4
			 0xc684a193, /// 0x2a8
			 0x2c95be5b, /// 0x2ac
			 0x78315bc8, /// 0x2b0
			 0x42f5e378, /// 0x2b4
			 0xd64e67f2, /// 0x2b8
			 0xb1694740, /// 0x2bc
			 0x21d1ee5, /// 0x2c0
			 0x428c0168, /// 0x2c4
			 0x60040412, /// 0x2c8
			 0x72bdfb, /// 0x2cc
			 0xf9cb4bb1, /// 0x2d0
			 0xa6bc435f, /// 0x2d4
			 0x299e9bc5, /// 0x2d8
			 0xa85e5c0a, /// 0x2dc
			 0x52dceeb2, /// 0x2e0
			 0x9e466813, /// 0x2e4
			 0x26cad325, /// 0x2e8
			 0xae88a533, /// 0x2ec
			 0x16526423, /// 0x2f0
			 0x40a8359, /// 0x2f4
			 0xad498646, /// 0x2f8
			 0x2bb49e30, /// 0x2fc
			 0x3b40511a, /// 0x300
			 0xfc0a9417, /// 0x304
			 0xa21948fc, /// 0x308
			 0x44b9e6, /// 0x30c
			 0x7192c1ef, /// 0x310
			 0xcdd383a8, /// 0x314
			 0x9a4b1079, /// 0x318
			 0xec6c184, /// 0x31c
			 0x81eb4c93, /// 0x320
			 0x49bfa775, /// 0x324
			 0x8b9a8dfc, /// 0x328
			 0xa3ebe6fd, /// 0x32c
			 0x6bd6d522, /// 0x330
			 0xf0925db5, /// 0x334
			 0xefe2f675, /// 0x338
			 0xc01d13c4, /// 0x33c
			 0x764325e8, /// 0x340
			 0x10c10b3f, /// 0x344
			 0xa699e56c, /// 0x348
			 0x23ca2d5d, /// 0x34c
			 0x95272b96, /// 0x350
			 0x77b4cfc3, /// 0x354
			 0x556d2fa4, /// 0x358
			 0xa49d9b16, /// 0x35c
			 0x9fc6b512, /// 0x360
			 0xb2f5dacb, /// 0x364
			 0xbefd113d, /// 0x368
			 0x55ce96d1, /// 0x36c
			 0x5decebe9, /// 0x370
			 0x24a94c54, /// 0x374
			 0xe335595f, /// 0x378
			 0x3d146e70, /// 0x37c
			 0xfb92723e, /// 0x380
			 0xd857646d, /// 0x384
			 0x76755d88, /// 0x388
			 0x49398bee, /// 0x38c
			 0x3a527fff, /// 0x390
			 0xda574e50, /// 0x394
			 0xf0e21516, /// 0x398
			 0xfd81612d, /// 0x39c
			 0xb36bef98, /// 0x3a0
			 0xf1bdfe73, /// 0x3a4
			 0x1e9bc911, /// 0x3a8
			 0x35876606, /// 0x3ac
			 0x9c00f298, /// 0x3b0
			 0xd4edac09, /// 0x3b4
			 0x3b4af95, /// 0x3b8
			 0x73a24404, /// 0x3bc
			 0xe956a3f2, /// 0x3c0
			 0x484013df, /// 0x3c4
			 0xba0b774, /// 0x3c8
			 0x58885b51, /// 0x3cc
			 0x9396cae8, /// 0x3d0
			 0x6c39476c, /// 0x3d4
			 0x7b9fdb27, /// 0x3d8
			 0x765b69d7, /// 0x3dc
			 0x92e2d167, /// 0x3e0
			 0xed0a71c1, /// 0x3e4
			 0xf602f0cf, /// 0x3e8
			 0x8c4f1113, /// 0x3ec
			 0xbeed4593, /// 0x3f0
			 0xde91e6db, /// 0x3f4
			 0x793ca8a8, /// 0x3f8
			 0xc9b6a296, /// 0x3fc
			 0x51e92eaf, /// 0x400
			 0x2df3e41b, /// 0x404
			 0x50ca00b4, /// 0x408
			 0xa141d76a, /// 0x40c
			 0xeee9ff42, /// 0x410
			 0xc39098c5, /// 0x414
			 0x3712de12, /// 0x418
			 0x198492dd, /// 0x41c
			 0xb87f5c20, /// 0x420
			 0x1e57f659, /// 0x424
			 0x6e66155f, /// 0x428
			 0xae5f8c4b, /// 0x42c
			 0xae974e0b, /// 0x430
			 0x2fdb4153, /// 0x434
			 0x345272ab, /// 0x438
			 0x8b513f08, /// 0x43c
			 0x9affd47c, /// 0x440
			 0x8dc5d658, /// 0x444
			 0xfac5b5e6, /// 0x448
			 0xfd34b2d5, /// 0x44c
			 0x673f862b, /// 0x450
			 0x785a8e84, /// 0x454
			 0xc6a0875a, /// 0x458
			 0x28d356c8, /// 0x45c
			 0x46c5160d, /// 0x460
			 0x60c667f5, /// 0x464
			 0x6dd9dc2b, /// 0x468
			 0x5b10abb1, /// 0x46c
			 0x633d3e84, /// 0x470
			 0xc5cf93d7, /// 0x474
			 0xc4ee0b13, /// 0x478
			 0xe199e87e, /// 0x47c
			 0xe4b00d78, /// 0x480
			 0x7a0fddb0, /// 0x484
			 0x733af516, /// 0x488
			 0xf3c7fbc3, /// 0x48c
			 0xc22cfd4b, /// 0x490
			 0xaa9c157b, /// 0x494
			 0x34b8f95, /// 0x498
			 0xf6c03681, /// 0x49c
			 0xe0735565, /// 0x4a0
			 0xfa2accfe, /// 0x4a4
			 0x747f08bb, /// 0x4a8
			 0xb5cb2264, /// 0x4ac
			 0x5fad8e0a, /// 0x4b0
			 0xb61fdd64, /// 0x4b4
			 0x7e73ce56, /// 0x4b8
			 0xbd26caa2, /// 0x4bc
			 0xb9a4fd67, /// 0x4c0
			 0xba452fb0, /// 0x4c4
			 0xee1c40e5, /// 0x4c8
			 0x396d5c33, /// 0x4cc
			 0xcf64e7e0, /// 0x4d0
			 0xc981cc31, /// 0x4d4
			 0xbe52d937, /// 0x4d8
			 0xeeda7435, /// 0x4dc
			 0x702b0133, /// 0x4e0
			 0x6217dd2, /// 0x4e4
			 0x4f509b4a, /// 0x4e8
			 0x2855672a, /// 0x4ec
			 0x4a1bd6d4, /// 0x4f0
			 0xe22dd7da, /// 0x4f4
			 0x878c3a52, /// 0x4f8
			 0x56127b6e, /// 0x4fc
			 0x594d46d4, /// 0x500
			 0xc909eb4c, /// 0x504
			 0xc295af4d, /// 0x508
			 0xdc658dd1, /// 0x50c
			 0x8fbf692a, /// 0x510
			 0x3f30d52b, /// 0x514
			 0x32faa9bd, /// 0x518
			 0x963f375c, /// 0x51c
			 0xe0d02c94, /// 0x520
			 0x2f3c3223, /// 0x524
			 0x67e0ebe2, /// 0x528
			 0x4bfee42b, /// 0x52c
			 0xec9f7c08, /// 0x530
			 0x434102c7, /// 0x534
			 0x6ffe0e9c, /// 0x538
			 0x7ffc5a6c, /// 0x53c
			 0x103dfee3, /// 0x540
			 0xc751c1a4, /// 0x544
			 0x8c9c14e2, /// 0x548
			 0xe323a241, /// 0x54c
			 0x7969b1cf, /// 0x550
			 0xaa2551dd, /// 0x554
			 0xd144f9d, /// 0x558
			 0xe3ede369, /// 0x55c
			 0x32ac0a49, /// 0x560
			 0x91213e1c, /// 0x564
			 0xcf39f443, /// 0x568
			 0x14937e6c, /// 0x56c
			 0xe74688, /// 0x570
			 0xe2426443, /// 0x574
			 0x314bd794, /// 0x578
			 0x6c26cd63, /// 0x57c
			 0x4223584a, /// 0x580
			 0xfb29a46a, /// 0x584
			 0xbb821351, /// 0x588
			 0x3875709, /// 0x58c
			 0xea254bc6, /// 0x590
			 0x529832e, /// 0x594
			 0x9e3799a6, /// 0x598
			 0x98486033, /// 0x59c
			 0x34152727, /// 0x5a0
			 0xfc6d502, /// 0x5a4
			 0xea973c22, /// 0x5a8
			 0xe7a814f5, /// 0x5ac
			 0x82cb264b, /// 0x5b0
			 0x7feb5221, /// 0x5b4
			 0x8ff1188e, /// 0x5b8
			 0x305e0d06, /// 0x5bc
			 0x43ffb2e8, /// 0x5c0
			 0x6e9aeb23, /// 0x5c4
			 0xe7659b79, /// 0x5c8
			 0x41c8bc61, /// 0x5cc
			 0xdb00cc5e, /// 0x5d0
			 0x9248ee74, /// 0x5d4
			 0x8fc53915, /// 0x5d8
			 0xa66ce41, /// 0x5dc
			 0xb679cdbd, /// 0x5e0
			 0xacfa495c, /// 0x5e4
			 0xc63b5a10, /// 0x5e8
			 0x57aef19, /// 0x5ec
			 0x8ff2e107, /// 0x5f0
			 0xdf84eb9b, /// 0x5f4
			 0x699fc9b9, /// 0x5f8
			 0x46f7e2e8, /// 0x5fc
			 0x976d13fb, /// 0x600
			 0xed672cea, /// 0x604
			 0x570ea316, /// 0x608
			 0x3774a8bc, /// 0x60c
			 0xa509f58e, /// 0x610
			 0x94e96f07, /// 0x614
			 0xedca9d31, /// 0x618
			 0x14b2f92a, /// 0x61c
			 0x39c4a965, /// 0x620
			 0xed057fd8, /// 0x624
			 0x8e918ddb, /// 0x628
			 0x1b2aa84f, /// 0x62c
			 0x1f086b8a, /// 0x630
			 0x8271495c, /// 0x634
			 0x6aeba30f, /// 0x638
			 0x27be093, /// 0x63c
			 0xdeccf86b, /// 0x640
			 0xe0e0a20, /// 0x644
			 0xf0c20249, /// 0x648
			 0xfef90594, /// 0x64c
			 0xed08d483, /// 0x650
			 0x7c24d391, /// 0x654
			 0x55054cae, /// 0x658
			 0x3c2ac47a, /// 0x65c
			 0x65fd1645, /// 0x660
			 0xe3756b28, /// 0x664
			 0x3163d6e6, /// 0x668
			 0x416950af, /// 0x66c
			 0x9b572ece, /// 0x670
			 0x26974447, /// 0x674
			 0x55df952a, /// 0x678
			 0xb186e324, /// 0x67c
			 0xa811e308, /// 0x680
			 0x64f771bc, /// 0x684
			 0x9ac7bdc9, /// 0x688
			 0xf0dc2cbc, /// 0x68c
			 0x5f701c50, /// 0x690
			 0x6277182c, /// 0x694
			 0x50b2c4d1, /// 0x698
			 0xc1b6f558, /// 0x69c
			 0x1a6ed81d, /// 0x6a0
			 0x66a9dabe, /// 0x6a4
			 0x4b9bb8cd, /// 0x6a8
			 0xa1aed463, /// 0x6ac
			 0xae811939, /// 0x6b0
			 0xf47d07cf, /// 0x6b4
			 0x81ec816, /// 0x6b8
			 0x2e06d72b, /// 0x6bc
			 0xbb096b5, /// 0x6c0
			 0xd9dcd390, /// 0x6c4
			 0x32b0c7e4, /// 0x6c8
			 0x2e6caa9b, /// 0x6cc
			 0xe7249357, /// 0x6d0
			 0x3a0d83a, /// 0x6d4
			 0xbf816579, /// 0x6d8
			 0xc7b1867d, /// 0x6dc
			 0xae3af9fd, /// 0x6e0
			 0x26be4f30, /// 0x6e4
			 0xaac7df2a, /// 0x6e8
			 0x56dedd70, /// 0x6ec
			 0x59c0a27, /// 0x6f0
			 0x1e9978fa, /// 0x6f4
			 0xda82c0f9, /// 0x6f8
			 0x90a955c3, /// 0x6fc
			 0x99d8adf1, /// 0x700
			 0xc67f8f11, /// 0x704
			 0x4387525f, /// 0x708
			 0x8aaf373d, /// 0x70c
			 0xe581b2cc, /// 0x710
			 0x779e1a8e, /// 0x714
			 0xbec7a453, /// 0x718
			 0xc25d913b, /// 0x71c
			 0xabf549e7, /// 0x720
			 0x20e4e6ad, /// 0x724
			 0x3ce88de5, /// 0x728
			 0xbbcfa651, /// 0x72c
			 0x69f504bf, /// 0x730
			 0xe118d2e7, /// 0x734
			 0x25e1c5b5, /// 0x738
			 0x2dbc6b7f, /// 0x73c
			 0xa600374c, /// 0x740
			 0xffcb59e9, /// 0x744
			 0xb747294d, /// 0x748
			 0x743937c0, /// 0x74c
			 0x24011ddd, /// 0x750
			 0xb7c06c26, /// 0x754
			 0xd15d3e09, /// 0x758
			 0xcc4ebb22, /// 0x75c
			 0xf032886b, /// 0x760
			 0xb0e5e1c4, /// 0x764
			 0x6ae8da4b, /// 0x768
			 0xa94345a6, /// 0x76c
			 0x3479e7c7, /// 0x770
			 0x980f40f7, /// 0x774
			 0xbc662dad, /// 0x778
			 0x6f8b6afe, /// 0x77c
			 0x58e5619c, /// 0x780
			 0x88f8ce2c, /// 0x784
			 0x59e4b9ff, /// 0x788
			 0xeef90626, /// 0x78c
			 0x8eda989d, /// 0x790
			 0x41f8ae70, /// 0x794
			 0xf9c65060, /// 0x798
			 0x31d9591e, /// 0x79c
			 0x89c98040, /// 0x7a0
			 0xefb0ca2a, /// 0x7a4
			 0x5ab8eee9, /// 0x7a8
			 0x4f81e039, /// 0x7ac
			 0x3be1a355, /// 0x7b0
			 0x6c8aa0ef, /// 0x7b4
			 0x452a6b2e, /// 0x7b8
			 0xb50b888, /// 0x7bc
			 0x81d688c9, /// 0x7c0
			 0x441b78d2, /// 0x7c4
			 0x613c9e18, /// 0x7c8
			 0x99d632cc, /// 0x7cc
			 0x2c5d1d6f, /// 0x7d0
			 0xab388f5a, /// 0x7d4
			 0x96c79dc8, /// 0x7d8
			 0x3884b810, /// 0x7dc
			 0xfd5066e7, /// 0x7e0
			 0xd2b718c, /// 0x7e4
			 0x32972ae1, /// 0x7e8
			 0x69702b1f, /// 0x7ec
			 0xe81507ea, /// 0x7f0
			 0x1d5b4e3c, /// 0x7f4
			 0xdfe06bde, /// 0x7f8
			 0x4ecfdccb, /// 0x7fc
			 0x316c29b0, /// 0x800
			 0xb1841e7c, /// 0x804
			 0xfccf0d2e, /// 0x808
			 0xd1ef4987, /// 0x80c
			 0x5be2e84a, /// 0x810
			 0xc099f38b, /// 0x814
			 0x302e7b06, /// 0x818
			 0xa5313769, /// 0x81c
			 0xab4f813e, /// 0x820
			 0x3a030779, /// 0x824
			 0x3ba27f15, /// 0x828
			 0x3a52ff24, /// 0x82c
			 0x90239a6c, /// 0x830
			 0x15965152, /// 0x834
			 0x4bd53d84, /// 0x838
			 0x3d59caa7, /// 0x83c
			 0xe56564d2, /// 0x840
			 0x4623fde4, /// 0x844
			 0x6448aca6, /// 0x848
			 0xe10fd5c4, /// 0x84c
			 0x52dc55de, /// 0x850
			 0x690c4685, /// 0x854
			 0xf30d92a8, /// 0x858
			 0x1a26b4c1, /// 0x85c
			 0x7a822abc, /// 0x860
			 0xc1b9a16, /// 0x864
			 0xbc03e861, /// 0x868
			 0x88bc6589, /// 0x86c
			 0x9b45d5b2, /// 0x870
			 0x754250c6, /// 0x874
			 0x149b3685, /// 0x878
			 0x767752b7, /// 0x87c
			 0xc820eb2e, /// 0x880
			 0xfd35f0ff, /// 0x884
			 0x5f69e123, /// 0x888
			 0xd2b6baa, /// 0x88c
			 0xbab4c35b, /// 0x890
			 0xa22db559, /// 0x894
			 0x67e9bea2, /// 0x898
			 0x16a5f4aa, /// 0x89c
			 0xa305ab06, /// 0x8a0
			 0x80801d23, /// 0x8a4
			 0xc3ef3d2f, /// 0x8a8
			 0xef430a45, /// 0x8ac
			 0x2d28b402, /// 0x8b0
			 0x1cd3adb3, /// 0x8b4
			 0xd789f96a, /// 0x8b8
			 0x22679c30, /// 0x8bc
			 0x7945237b, /// 0x8c0
			 0x77ea022, /// 0x8c4
			 0xc9033484, /// 0x8c8
			 0x333da252, /// 0x8cc
			 0x8e2bb1c7, /// 0x8d0
			 0x25daaf5, /// 0x8d4
			 0xc8d900d0, /// 0x8d8
			 0xc643690f, /// 0x8dc
			 0x462dc417, /// 0x8e0
			 0x839c89a, /// 0x8e4
			 0xd99a9108, /// 0x8e8
			 0x25c98b21, /// 0x8ec
			 0x2232b680, /// 0x8f0
			 0x7b2f112e, /// 0x8f4
			 0xf61a9f19, /// 0x8f8
			 0x79fa437c, /// 0x8fc
			 0x63358686, /// 0x900
			 0x1fc8ba88, /// 0x904
			 0xe4309a6f, /// 0x908
			 0x2838b80f, /// 0x90c
			 0x9637adec, /// 0x910
			 0x915013f7, /// 0x914
			 0xa5c49931, /// 0x918
			 0x8dd7f27, /// 0x91c
			 0xc8b41308, /// 0x920
			 0x42afa420, /// 0x924
			 0x2e98ee4e, /// 0x928
			 0xce2df258, /// 0x92c
			 0xc4ad729b, /// 0x930
			 0xfd11cc7b, /// 0x934
			 0x91a8cbdc, /// 0x938
			 0x409a5261, /// 0x93c
			 0x6bd823dc, /// 0x940
			 0xa3a065cd, /// 0x944
			 0x8aa9ff50, /// 0x948
			 0xe73e85b6, /// 0x94c
			 0xd3efd282, /// 0x950
			 0x6982144b, /// 0x954
			 0x278fe92b, /// 0x958
			 0x91c726fc, /// 0x95c
			 0x4922d077, /// 0x960
			 0xc0b98818, /// 0x964
			 0x24ccda09, /// 0x968
			 0x30e06402, /// 0x96c
			 0xb276e5ae, /// 0x970
			 0x63fa9ab3, /// 0x974
			 0x3c0b5ed, /// 0x978
			 0x871510e8, /// 0x97c
			 0x79d41839, /// 0x980
			 0x848444c6, /// 0x984
			 0xc38256de, /// 0x988
			 0x70c9c104, /// 0x98c
			 0xf96950e5, /// 0x990
			 0xf4c74a5f, /// 0x994
			 0xb64ab9ba, /// 0x998
			 0xa4a734c4, /// 0x99c
			 0x25780e4c, /// 0x9a0
			 0x9ba405f8, /// 0x9a4
			 0x327b6001, /// 0x9a8
			 0xda3fdc89, /// 0x9ac
			 0xb4ecaf76, /// 0x9b0
			 0x238733cc, /// 0x9b4
			 0x2218be3e, /// 0x9b8
			 0xf5dcb2bc, /// 0x9bc
			 0xc9f4f871, /// 0x9c0
			 0x7c582888, /// 0x9c4
			 0xdb179f82, /// 0x9c8
			 0x1c89b463, /// 0x9cc
			 0x5bafedb3, /// 0x9d0
			 0x4ef753aa, /// 0x9d4
			 0x561fbce9, /// 0x9d8
			 0xf56ff8cf, /// 0x9dc
			 0xbcfe74e, /// 0x9e0
			 0x10b740db, /// 0x9e4
			 0xc895d473, /// 0x9e8
			 0x8360f0b6, /// 0x9ec
			 0x76a19340, /// 0x9f0
			 0xf45c4cea, /// 0x9f4
			 0xc58e2769, /// 0x9f8
			 0x676c2be0, /// 0x9fc
			 0xba2500ff, /// 0xa00
			 0x578b75c2, /// 0xa04
			 0xbfeba659, /// 0xa08
			 0x4e80f2c3, /// 0xa0c
			 0xc9051159, /// 0xa10
			 0x26a35a71, /// 0xa14
			 0x4900c9e6, /// 0xa18
			 0x2aa1b3e8, /// 0xa1c
			 0xaa6a9a39, /// 0xa20
			 0x4472619f, /// 0xa24
			 0xb2c054eb, /// 0xa28
			 0x7c942b52, /// 0xa2c
			 0x423a24ff, /// 0xa30
			 0x875bb482, /// 0xa34
			 0x1c0ad142, /// 0xa38
			 0x7c9028ec, /// 0xa3c
			 0xf969b583, /// 0xa40
			 0x39a7ae73, /// 0xa44
			 0xe259109e, /// 0xa48
			 0xfa2640c3, /// 0xa4c
			 0xca4d130d, /// 0xa50
			 0xbe299be4, /// 0xa54
			 0x4ed20357, /// 0xa58
			 0x67fe23bd, /// 0xa5c
			 0x44cf47bf, /// 0xa60
			 0x96981263, /// 0xa64
			 0x55371eea, /// 0xa68
			 0x35b1d535, /// 0xa6c
			 0x97432907, /// 0xa70
			 0x340aa93b, /// 0xa74
			 0xf95f282c, /// 0xa78
			 0x4e640a34, /// 0xa7c
			 0x6b1b0062, /// 0xa80
			 0xade51b0c, /// 0xa84
			 0x7fc10cc4, /// 0xa88
			 0x16ed0866, /// 0xa8c
			 0xa829cd7f, /// 0xa90
			 0x7250132f, /// 0xa94
			 0x5fcae529, /// 0xa98
			 0xd36d6e82, /// 0xa9c
			 0xe96cc7b2, /// 0xaa0
			 0x723eae20, /// 0xaa4
			 0xfd8e6dc4, /// 0xaa8
			 0xcd0169d0, /// 0xaac
			 0x6344cea6, /// 0xab0
			 0xfc0d9a1d, /// 0xab4
			 0xd69fada3, /// 0xab8
			 0xa72c31b0, /// 0xabc
			 0x56a28a44, /// 0xac0
			 0x37d2d191, /// 0xac4
			 0x24727393, /// 0xac8
			 0x5e6fb196, /// 0xacc
			 0xb96c8840, /// 0xad0
			 0xa83329a1, /// 0xad4
			 0x97615c42, /// 0xad8
			 0xb31dffe8, /// 0xadc
			 0xc7077199, /// 0xae0
			 0x840cc5cb, /// 0xae4
			 0x1e6a9c12, /// 0xae8
			 0x119cc361, /// 0xaec
			 0xa01cb8a8, /// 0xaf0
			 0x6307f0a4, /// 0xaf4
			 0x2bf5667e, /// 0xaf8
			 0x43e861e5, /// 0xafc
			 0x1179a638, /// 0xb00
			 0x1038a8fa, /// 0xb04
			 0xddcb5137, /// 0xb08
			 0xeaf52daf, /// 0xb0c
			 0xb08dd7af, /// 0xb10
			 0x4503cca1, /// 0xb14
			 0x45619bc, /// 0xb18
			 0x37a1bbb9, /// 0xb1c
			 0xe0677b97, /// 0xb20
			 0xff4c602e, /// 0xb24
			 0xbb822211, /// 0xb28
			 0xf138388c, /// 0xb2c
			 0x2fc7915e, /// 0xb30
			 0xd29736ef, /// 0xb34
			 0xc69f88f8, /// 0xb38
			 0x40b6c9b7, /// 0xb3c
			 0x178a7ae7, /// 0xb40
			 0xd85978d0, /// 0xb44
			 0x383dd623, /// 0xb48
			 0xb8832259, /// 0xb4c
			 0x5bf4f797, /// 0xb50
			 0x67a417e7, /// 0xb54
			 0x148e0949, /// 0xb58
			 0xb9d03d30, /// 0xb5c
			 0xe2ad6be, /// 0xb60
			 0x8c978eda, /// 0xb64
			 0x37a4f95a, /// 0xb68
			 0x7d2fda8d, /// 0xb6c
			 0x26c746f6, /// 0xb70
			 0xe449abdd, /// 0xb74
			 0xc9ea2202, /// 0xb78
			 0xaf89bac5, /// 0xb7c
			 0x198adde9, /// 0xb80
			 0x6d3999e6, /// 0xb84
			 0x4c5ff6ef, /// 0xb88
			 0xa3706baa, /// 0xb8c
			 0xafc54e61, /// 0xb90
			 0xf68f2fea, /// 0xb94
			 0x5becbc68, /// 0xb98
			 0x81982f7e, /// 0xb9c
			 0x847523dd, /// 0xba0
			 0xe468411e, /// 0xba4
			 0x2e500ef2, /// 0xba8
			 0x41a8dcba, /// 0xbac
			 0x624580a4, /// 0xbb0
			 0x375bfe34, /// 0xbb4
			 0x5b4946ee, /// 0xbb8
			 0x9e15a38a, /// 0xbbc
			 0x1c5738a2, /// 0xbc0
			 0xd164790b, /// 0xbc4
			 0xae2ef41, /// 0xbc8
			 0x210e5332, /// 0xbcc
			 0xf22b100a, /// 0xbd0
			 0x8317eb0a, /// 0xbd4
			 0xda001614, /// 0xbd8
			 0x4e725a12, /// 0xbdc
			 0x17097003, /// 0xbe0
			 0xeff52059, /// 0xbe4
			 0x6f77e28f, /// 0xbe8
			 0x28a4bb9e, /// 0xbec
			 0x37100e7a, /// 0xbf0
			 0xac6b011a, /// 0xbf4
			 0x8186796a, /// 0xbf8
			 0xa0b71376, /// 0xbfc
			 0x509186bd, /// 0xc00
			 0xda21fd7f, /// 0xc04
			 0x3ac3640d, /// 0xc08
			 0xdca40068, /// 0xc0c
			 0x1f4ee7bd, /// 0xc10
			 0x338614db, /// 0xc14
			 0xfcff96d4, /// 0xc18
			 0xa7a60bad, /// 0xc1c
			 0x1583ff1d, /// 0xc20
			 0x370bd3c9, /// 0xc24
			 0xef1fadec, /// 0xc28
			 0xd908b9aa, /// 0xc2c
			 0xd1cb99e0, /// 0xc30
			 0xe0627a3e, /// 0xc34
			 0xb5143a29, /// 0xc38
			 0xfc41eb02, /// 0xc3c
			 0xad9cecff, /// 0xc40
			 0x2ca3ae4c, /// 0xc44
			 0x18b7ff97, /// 0xc48
			 0x969525dc, /// 0xc4c
			 0x60d93cf7, /// 0xc50
			 0x5c2114a8, /// 0xc54
			 0x13f250ed, /// 0xc58
			 0x22eebe6a, /// 0xc5c
			 0xc4915e66, /// 0xc60
			 0x16f0973c, /// 0xc64
			 0x83ae9efe, /// 0xc68
			 0xfded6ac5, /// 0xc6c
			 0xcefa6908, /// 0xc70
			 0x7a0a2fdb, /// 0xc74
			 0xe90205c0, /// 0xc78
			 0x6d2a28bb, /// 0xc7c
			 0x7f1b01fe, /// 0xc80
			 0xfcf7ea13, /// 0xc84
			 0xea6e247e, /// 0xc88
			 0x794cb4f1, /// 0xc8c
			 0x93aced7a, /// 0xc90
			 0x7dcd70cc, /// 0xc94
			 0x43aa8198, /// 0xc98
			 0xcf48e07, /// 0xc9c
			 0x4bab04d6, /// 0xca0
			 0x6d37f415, /// 0xca4
			 0x745c233b, /// 0xca8
			 0xf8cfea3a, /// 0xcac
			 0xaf2630bb, /// 0xcb0
			 0x325b2ce9, /// 0xcb4
			 0xd2d4af6b, /// 0xcb8
			 0xfce195a3, /// 0xcbc
			 0xeedac2, /// 0xcc0
			 0x3b79e7e7, /// 0xcc4
			 0xfaf49413, /// 0xcc8
			 0x3fd033e0, /// 0xccc
			 0x7a2f1e82, /// 0xcd0
			 0x79c2a1c1, /// 0xcd4
			 0x6fb16e81, /// 0xcd8
			 0x59172b0f, /// 0xcdc
			 0x19105bcb, /// 0xce0
			 0xf2779c33, /// 0xce4
			 0x1ef879bc, /// 0xce8
			 0x64f49ae3, /// 0xcec
			 0x7a1e9fd3, /// 0xcf0
			 0x9cbf9b4, /// 0xcf4
			 0x84943077, /// 0xcf8
			 0xe333ef31, /// 0xcfc
			 0x94a47ac0, /// 0xd00
			 0xe4c2876, /// 0xd04
			 0x5e026a85, /// 0xd08
			 0x2148594e, /// 0xd0c
			 0x951077b7, /// 0xd10
			 0xd590e3a2, /// 0xd14
			 0xae5f27c0, /// 0xd18
			 0x772ae608, /// 0xd1c
			 0xa6447eec, /// 0xd20
			 0xc579b5a3, /// 0xd24
			 0x23f2b9b0, /// 0xd28
			 0x6173716f, /// 0xd2c
			 0x69e71e8d, /// 0xd30
			 0x521dcaee, /// 0xd34
			 0xce621e02, /// 0xd38
			 0xe93072f6, /// 0xd3c
			 0xe2162830, /// 0xd40
			 0x8dfc8f63, /// 0xd44
			 0xcd422832, /// 0xd48
			 0x97c05468, /// 0xd4c
			 0x183e9c06, /// 0xd50
			 0x672ea6d0, /// 0xd54
			 0x6b4f5aad, /// 0xd58
			 0x23e2173c, /// 0xd5c
			 0x234f2c4b, /// 0xd60
			 0x5a7c1d73, /// 0xd64
			 0x42e44b0a, /// 0xd68
			 0xf3d34b0c, /// 0xd6c
			 0x42dd9d91, /// 0xd70
			 0x8309808e, /// 0xd74
			 0x45d48f77, /// 0xd78
			 0xb6bab7a2, /// 0xd7c
			 0x269b2537, /// 0xd80
			 0xee08df11, /// 0xd84
			 0xc70d6d60, /// 0xd88
			 0x97d7e621, /// 0xd8c
			 0x1224311c, /// 0xd90
			 0x89050709, /// 0xd94
			 0xfd13ce1c, /// 0xd98
			 0x783da86f, /// 0xd9c
			 0xc43de34, /// 0xda0
			 0x7cd9b95, /// 0xda4
			 0xdf58c872, /// 0xda8
			 0xf43844fc, /// 0xdac
			 0x6b290474, /// 0xdb0
			 0xf8d55c16, /// 0xdb4
			 0xd908a715, /// 0xdb8
			 0xf830f611, /// 0xdbc
			 0xeb3a4de4, /// 0xdc0
			 0x9e442e49, /// 0xdc4
			 0xe69abc64, /// 0xdc8
			 0x3e75b3fe, /// 0xdcc
			 0x28cbf8d, /// 0xdd0
			 0x4bbc2797, /// 0xdd4
			 0xbe1f4ea7, /// 0xdd8
			 0xfa4b4ee9, /// 0xddc
			 0x262d953e, /// 0xde0
			 0x818c5666, /// 0xde4
			 0xeb85ce09, /// 0xde8
			 0x75a54029, /// 0xdec
			 0x4ca8522b, /// 0xdf0
			 0xf4dec15, /// 0xdf4
			 0x8544dc1a, /// 0xdf8
			 0x513957fa, /// 0xdfc
			 0xf71105bb, /// 0xe00
			 0x72af232b, /// 0xe04
			 0x93f7d9a1, /// 0xe08
			 0xb614373c, /// 0xe0c
			 0x1372bf8a, /// 0xe10
			 0x224d55ae, /// 0xe14
			 0xa493a82f, /// 0xe18
			 0x9ff08268, /// 0xe1c
			 0xed66eaef, /// 0xe20
			 0x835cef84, /// 0xe24
			 0xfadc4793, /// 0xe28
			 0x929af769, /// 0xe2c
			 0xcd838bf9, /// 0xe30
			 0xa729c85e, /// 0xe34
			 0x35752690, /// 0xe38
			 0xe52f937a, /// 0xe3c
			 0xd460828d, /// 0xe40
			 0xe7baaccd, /// 0xe44
			 0x27bf4980, /// 0xe48
			 0x1d3ff545, /// 0xe4c
			 0x1ed5cb8a, /// 0xe50
			 0xf5c11e12, /// 0xe54
			 0x79def48f, /// 0xe58
			 0xdc3ebc88, /// 0xe5c
			 0x9ed3d35a, /// 0xe60
			 0x4106c00f, /// 0xe64
			 0xc2b2caaf, /// 0xe68
			 0xc1ecfdb0, /// 0xe6c
			 0x29c6501, /// 0xe70
			 0x1f1b5d07, /// 0xe74
			 0x70b8bbde, /// 0xe78
			 0xef9c58f0, /// 0xe7c
			 0x9ccb7ed5, /// 0xe80
			 0xa400f014, /// 0xe84
			 0xfd3dd35b, /// 0xe88
			 0x23043656, /// 0xe8c
			 0x4e332b51, /// 0xe90
			 0xedae470a, /// 0xe94
			 0xae5ba2d7, /// 0xe98
			 0xeb46fbf7, /// 0xe9c
			 0x68807123, /// 0xea0
			 0x975f39af, /// 0xea4
			 0x9e85236a, /// 0xea8
			 0x3d1f6b5d, /// 0xeac
			 0xaa5fc0d8, /// 0xeb0
			 0x487450dd, /// 0xeb4
			 0xc235481c, /// 0xeb8
			 0x946ff27b, /// 0xebc
			 0x88115ef4, /// 0xec0
			 0x1a011691, /// 0xec4
			 0xbf6c383b, /// 0xec8
			 0x8171e3a7, /// 0xecc
			 0x70b63c30, /// 0xed0
			 0xac9bbbe7, /// 0xed4
			 0xffc36d67, /// 0xed8
			 0x46e8ea77, /// 0xedc
			 0xce8512e, /// 0xee0
			 0xd3653008, /// 0xee4
			 0x8ac2bdc6, /// 0xee8
			 0x525d1307, /// 0xeec
			 0xa74079ee, /// 0xef0
			 0xfe33a40d, /// 0xef4
			 0x345b1e73, /// 0xef8
			 0x55727e4c, /// 0xefc
			 0xfa0d867f, /// 0xf00
			 0x1f6ceca8, /// 0xf04
			 0xa5955654, /// 0xf08
			 0x71b5c9e4, /// 0xf0c
			 0x50870d07, /// 0xf10
			 0x32d94329, /// 0xf14
			 0x6a2f7fef, /// 0xf18
			 0xeb6f38a8, /// 0xf1c
			 0x7371f269, /// 0xf20
			 0x4cc0b3c8, /// 0xf24
			 0x50cfc27b, /// 0xf28
			 0xb51d3cbb, /// 0xf2c
			 0x11353513, /// 0xf30
			 0x40c5ac2c, /// 0xf34
			 0xee3f67e5, /// 0xf38
			 0xe68506f7, /// 0xf3c
			 0xeefbe8d5, /// 0xf40
			 0xbc6063da, /// 0xf44
			 0x132793cd, /// 0xf48
			 0x2621a305, /// 0xf4c
			 0xda2d57d5, /// 0xf50
			 0x66e0be98, /// 0xf54
			 0x39b385f, /// 0xf58
			 0x181b5016, /// 0xf5c
			 0x8932c7de, /// 0xf60
			 0xed01c1ec, /// 0xf64
			 0xa4e02d92, /// 0xf68
			 0xf5dfb041, /// 0xf6c
			 0xec87508, /// 0xf70
			 0xd4656803, /// 0xf74
			 0xa7cb4cf7, /// 0xf78
			 0x64c6e8c, /// 0xf7c
			 0xea6b123b, /// 0xf80
			 0x295d6a35, /// 0xf84
			 0x695ecf73, /// 0xf88
			 0xc531af02, /// 0xf8c
			 0xd65f39e3, /// 0xf90
			 0x1225ae2f, /// 0xf94
			 0xcfdcbe5b, /// 0xf98
			 0x326c074e, /// 0xf9c
			 0x14d2ce29, /// 0xfa0
			 0xc9ad4ba3, /// 0xfa4
			 0xdab1c1bb, /// 0xfa8
			 0xa9903c42, /// 0xfac
			 0xb32aa55, /// 0xfb0
			 0x2877b01a, /// 0xfb4
			 0x2e33df05, /// 0xfb8
			 0x70f215ef, /// 0xfbc
			 0xbd25a718, /// 0xfc0
			 0x58d936e5, /// 0xfc4
			 0x78e3dd05, /// 0xfc8
			 0x7b930295, /// 0xfcc
			 0x73f6da8d, /// 0xfd0
			 0xadb58af8, /// 0xfd4
			 0x47602d8a, /// 0xfd8
			 0x8ac76fd3, /// 0xfdc
			 0x632fd976, /// 0xfe0
			 0x6d839505, /// 0xfe4
			 0xd62889cf, /// 0xfe8
			 0x42553c49, /// 0xfec
			 0x1695a18f, /// 0xff0
			 0xab8ad380, /// 0xff4
			 0x18987027, /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x8eb0a818, /// 0x0
			 0xbd35a17c, /// 0x4
			 0x9df70a11, /// 0x8
			 0x7cd4e8ee, /// 0xc
			 0xddcca202, /// 0x10
			 0x7802c1d1, /// 0x14
			 0xa054645a, /// 0x18
			 0xd4d3bccc, /// 0x1c
			 0x71a0cd51, /// 0x20
			 0x60796bd2, /// 0x24
			 0x8978ee68, /// 0x28
			 0x2f018289, /// 0x2c
			 0xc92dc41f, /// 0x30
			 0xbb0aff24, /// 0x34
			 0x949c40ed, /// 0x38
			 0x52ce38d5, /// 0x3c
			 0x86b649ba, /// 0x40
			 0x42e56aaf, /// 0x44
			 0x572f72bd, /// 0x48
			 0x8e91256b, /// 0x4c
			 0xa8594950, /// 0x50
			 0xd7cf667, /// 0x54
			 0x4fb735af, /// 0x58
			 0xc468dffd, /// 0x5c
			 0xb40ba745, /// 0x60
			 0x2dfd0dab, /// 0x64
			 0xd07ba9b5, /// 0x68
			 0x72b34278, /// 0x6c
			 0xbe36be12, /// 0x70
			 0x1c14cee8, /// 0x74
			 0x7fe9b2a2, /// 0x78
			 0x91904ade, /// 0x7c
			 0xeb6d9876, /// 0x80
			 0x1d3ac417, /// 0x84
			 0x26f3ef02, /// 0x88
			 0x211f2de5, /// 0x8c
			 0x4ff0e523, /// 0x90
			 0x3e9d9158, /// 0x94
			 0x7fa60788, /// 0x98
			 0xb9c1a5a0, /// 0x9c
			 0xb1d2af96, /// 0xa0
			 0x4ec0f12e, /// 0xa4
			 0x42f83d80, /// 0xa8
			 0x90bc6d81, /// 0xac
			 0x7ae21ba9, /// 0xb0
			 0xbcae3f64, /// 0xb4
			 0xc5af1bb0, /// 0xb8
			 0xc3194d90, /// 0xbc
			 0xe63dedcc, /// 0xc0
			 0x79c7a7df, /// 0xc4
			 0x9d7e56ca, /// 0xc8
			 0xb173f9a5, /// 0xcc
			 0xd062372c, /// 0xd0
			 0xa1748389, /// 0xd4
			 0xb41ca65d, /// 0xd8
			 0x43676e2, /// 0xdc
			 0x76c12a3e, /// 0xe0
			 0x97a62b04, /// 0xe4
			 0xa071162, /// 0xe8
			 0x351d2422, /// 0xec
			 0x87ee9ec7, /// 0xf0
			 0x14c1318d, /// 0xf4
			 0xdea43202, /// 0xf8
			 0xca95a894, /// 0xfc
			 0x643b5792, /// 0x100
			 0x5ac8904f, /// 0x104
			 0xc50fe94b, /// 0x108
			 0x89f9f193, /// 0x10c
			 0x64a41786, /// 0x110
			 0x87b89ed7, /// 0x114
			 0x42f7eb23, /// 0x118
			 0x3f7bc1b6, /// 0x11c
			 0xe336171a, /// 0x120
			 0x6469b24f, /// 0x124
			 0xf9890d0a, /// 0x128
			 0x1a727239, /// 0x12c
			 0x9b5c76aa, /// 0x130
			 0xbb459a72, /// 0x134
			 0x1d6032c5, /// 0x138
			 0xc19ee158, /// 0x13c
			 0xef4187d7, /// 0x140
			 0xa4711d49, /// 0x144
			 0x2678bed1, /// 0x148
			 0x6b29c548, /// 0x14c
			 0xa7c25c4b, /// 0x150
			 0x3922d217, /// 0x154
			 0x6b30372c, /// 0x158
			 0xd7c724e2, /// 0x15c
			 0x68323286, /// 0x160
			 0x17006dd8, /// 0x164
			 0xae99910a, /// 0x168
			 0xb6f71ddb, /// 0x16c
			 0x9d1f1ba3, /// 0x170
			 0xededdb72, /// 0x174
			 0x22d759ca, /// 0x178
			 0xb22f1d4f, /// 0x17c
			 0x5b5ac364, /// 0x180
			 0xed41d5e6, /// 0x184
			 0x8402acc, /// 0x188
			 0x1160d54f, /// 0x18c
			 0xb1fa5da1, /// 0x190
			 0x1a922b4b, /// 0x194
			 0x3bcbb44f, /// 0x198
			 0x54fb98ff, /// 0x19c
			 0xa2cb65d8, /// 0x1a0
			 0x323e8a67, /// 0x1a4
			 0x450c28ae, /// 0x1a8
			 0x1815551e, /// 0x1ac
			 0x95f3b45, /// 0x1b0
			 0x6070bb4f, /// 0x1b4
			 0xa2cd75d4, /// 0x1b8
			 0xe8194302, /// 0x1bc
			 0xa75716c3, /// 0x1c0
			 0xce0f57fb, /// 0x1c4
			 0x1ef451b1, /// 0x1c8
			 0x6ddc86a, /// 0x1cc
			 0x8dcf6cd7, /// 0x1d0
			 0x2a9de3bf, /// 0x1d4
			 0x504aba71, /// 0x1d8
			 0xe85719ef, /// 0x1dc
			 0x6e07fc97, /// 0x1e0
			 0x764c50ee, /// 0x1e4
			 0xe39b1857, /// 0x1e8
			 0x58103afe, /// 0x1ec
			 0xf4afe0d9, /// 0x1f0
			 0xee61d848, /// 0x1f4
			 0xf8633d93, /// 0x1f8
			 0xa8e7850, /// 0x1fc
			 0x5e26eba9, /// 0x200
			 0xc47cb1c9, /// 0x204
			 0xe570a600, /// 0x208
			 0x3b7a5e55, /// 0x20c
			 0x98d29d08, /// 0x210
			 0x33b2859b, /// 0x214
			 0x46ed4d60, /// 0x218
			 0x608f8d7, /// 0x21c
			 0x6f2a6332, /// 0x220
			 0xbc6eb8a, /// 0x224
			 0x136611ca, /// 0x228
			 0x996ed3b4, /// 0x22c
			 0xc4b5db37, /// 0x230
			 0x662dee64, /// 0x234
			 0x8838abe0, /// 0x238
			 0xd1842fda, /// 0x23c
			 0xb28449fd, /// 0x240
			 0x5bd43dc1, /// 0x244
			 0xc449bece, /// 0x248
			 0x55869a7, /// 0x24c
			 0x8e49f265, /// 0x250
			 0x5a354162, /// 0x254
			 0xaefe6061, /// 0x258
			 0xc4082c18, /// 0x25c
			 0x86d49705, /// 0x260
			 0x549df50f, /// 0x264
			 0xedb32da4, /// 0x268
			 0x614fbb63, /// 0x26c
			 0x557c5aaf, /// 0x270
			 0x181ec756, /// 0x274
			 0x410933c, /// 0x278
			 0x3dfb5c6c, /// 0x27c
			 0xf34e3c38, /// 0x280
			 0x27fa34ce, /// 0x284
			 0x7ffafb5b, /// 0x288
			 0x61b753f7, /// 0x28c
			 0x42e96169, /// 0x290
			 0xddd17498, /// 0x294
			 0xf0103703, /// 0x298
			 0x43b5538d, /// 0x29c
			 0xa6a9122d, /// 0x2a0
			 0xb457cc9, /// 0x2a4
			 0xc775d5eb, /// 0x2a8
			 0xa2b6c487, /// 0x2ac
			 0xb9cefcf1, /// 0x2b0
			 0x1f9c1bf, /// 0x2b4
			 0xc1fdda13, /// 0x2b8
			 0x72aa478a, /// 0x2bc
			 0xf6e04d03, /// 0x2c0
			 0xee8f85aa, /// 0x2c4
			 0x1459fd53, /// 0x2c8
			 0x2acfe4, /// 0x2cc
			 0x6b952967, /// 0x2d0
			 0x40537a12, /// 0x2d4
			 0x904813aa, /// 0x2d8
			 0xa55cc58f, /// 0x2dc
			 0x4d52714f, /// 0x2e0
			 0xc34084e1, /// 0x2e4
			 0x1205f42b, /// 0x2e8
			 0xe8c5f380, /// 0x2ec
			 0xa756abbe, /// 0x2f0
			 0x6d1a4045, /// 0x2f4
			 0xdfe19609, /// 0x2f8
			 0xf061a59b, /// 0x2fc
			 0x751c9cd4, /// 0x300
			 0xa4d3b492, /// 0x304
			 0xe06db3, /// 0x308
			 0xd26f6637, /// 0x30c
			 0x37291571, /// 0x310
			 0x91eb4ea8, /// 0x314
			 0x72487b95, /// 0x318
			 0x99496989, /// 0x31c
			 0x4c2d6391, /// 0x320
			 0x515584c1, /// 0x324
			 0x27cf809d, /// 0x328
			 0xd3ea5127, /// 0x32c
			 0x3a6cf071, /// 0x330
			 0xbbba2dfa, /// 0x334
			 0xa3aa4cf9, /// 0x338
			 0x3118259d, /// 0x33c
			 0x60174388, /// 0x340
			 0x1144cdc, /// 0x344
			 0x65929d16, /// 0x348
			 0x6dba9f9d, /// 0x34c
			 0x6054189b, /// 0x350
			 0x74020528, /// 0x354
			 0x72f4dc05, /// 0x358
			 0x26ee2be7, /// 0x35c
			 0xa9f52133, /// 0x360
			 0x620fefd7, /// 0x364
			 0x8d090a62, /// 0x368
			 0xbabbd71d, /// 0x36c
			 0x983073cd, /// 0x370
			 0xe38de29b, /// 0x374
			 0x2ddad870, /// 0x378
			 0x74a3bdd6, /// 0x37c
			 0x1b2a3628, /// 0x380
			 0x5c5faab2, /// 0x384
			 0x3ec06eb3, /// 0x388
			 0x1dca6208, /// 0x38c
			 0xf369b8af, /// 0x390
			 0x4f18c891, /// 0x394
			 0xc5c392a, /// 0x398
			 0x8534e59c, /// 0x39c
			 0x44c19d60, /// 0x3a0
			 0x93425213, /// 0x3a4
			 0x50e252ec, /// 0x3a8
			 0x7e044259, /// 0x3ac
			 0x1e323002, /// 0x3b0
			 0xc16d31c9, /// 0x3b4
			 0x7bb29a26, /// 0x3b8
			 0x3a70a8ef, /// 0x3bc
			 0xf095bab8, /// 0x3c0
			 0xe6a52374, /// 0x3c4
			 0x39ecb1fa, /// 0x3c8
			 0x1d97bbe9, /// 0x3cc
			 0x1a627470, /// 0x3d0
			 0x8bd5bd4e, /// 0x3d4
			 0xed836da9, /// 0x3d8
			 0xd9defe4b, /// 0x3dc
			 0x9f6dc671, /// 0x3e0
			 0x6aedbf62, /// 0x3e4
			 0x82a96e2f, /// 0x3e8
			 0x24c98074, /// 0x3ec
			 0xa494deb1, /// 0x3f0
			 0x6e79988c, /// 0x3f4
			 0x5f2498b4, /// 0x3f8
			 0x9a457f12, /// 0x3fc
			 0x8f1b2757, /// 0x400
			 0x55ca7b5e, /// 0x404
			 0x69eb5025, /// 0x408
			 0x3a1affab, /// 0x40c
			 0x11d6385e, /// 0x410
			 0x19f72ca2, /// 0x414
			 0xdfb3c5fc, /// 0x418
			 0xa01bb68d, /// 0x41c
			 0xd3916da, /// 0x420
			 0x11bdb55, /// 0x424
			 0x80bc962e, /// 0x428
			 0x8a1a0502, /// 0x42c
			 0xcafe3d3c, /// 0x430
			 0x4b7440f8, /// 0x434
			 0x9d4fcbc1, /// 0x438
			 0x860ddc7f, /// 0x43c
			 0x8b7b4708, /// 0x440
			 0xdf5d7c71, /// 0x444
			 0x39b7fc34, /// 0x448
			 0x562bfbbc, /// 0x44c
			 0x7878aaab, /// 0x450
			 0xd776f21f, /// 0x454
			 0xe2d9d5c3, /// 0x458
			 0xf99f9c45, /// 0x45c
			 0x45c335bd, /// 0x460
			 0x3dd740e4, /// 0x464
			 0x58c4e942, /// 0x468
			 0x648c09fb, /// 0x46c
			 0x74d6de2a, /// 0x470
			 0x87a89296, /// 0x474
			 0x2886ce4e, /// 0x478
			 0x18540bbf, /// 0x47c
			 0xba89aa58, /// 0x480
			 0x40f19117, /// 0x484
			 0x96c49dc2, /// 0x488
			 0xa12235d1, /// 0x48c
			 0x8cee042, /// 0x490
			 0x71d376e2, /// 0x494
			 0x8493c9c6, /// 0x498
			 0xeef7311e, /// 0x49c
			 0xc5b007b1, /// 0x4a0
			 0x690d042, /// 0x4a4
			 0x7ded4fa9, /// 0x4a8
			 0x569d6afd, /// 0x4ac
			 0x64d5b2be, /// 0x4b0
			 0x7890acb3, /// 0x4b4
			 0x3df599d1, /// 0x4b8
			 0x8a148af5, /// 0x4bc
			 0x8d9a409c, /// 0x4c0
			 0x43311875, /// 0x4c4
			 0xe5fb10e3, /// 0x4c8
			 0x115447a3, /// 0x4cc
			 0x254a6917, /// 0x4d0
			 0x2e4bb053, /// 0x4d4
			 0x9f3a985a, /// 0x4d8
			 0x5e2dfb28, /// 0x4dc
			 0x84b91074, /// 0x4e0
			 0x35fa48d9, /// 0x4e4
			 0x26b2e7a0, /// 0x4e8
			 0xc3f935a9, /// 0x4ec
			 0xa9d3847, /// 0x4f0
			 0xc55850c8, /// 0x4f4
			 0x4c1cbc00, /// 0x4f8
			 0x48493f0, /// 0x4fc
			 0xa6a2007a, /// 0x500
			 0x61c693b3, /// 0x504
			 0x182b3306, /// 0x508
			 0x3c18b761, /// 0x50c
			 0x932ea780, /// 0x510
			 0x8b44244e, /// 0x514
			 0x34c95176, /// 0x518
			 0xf42c9826, /// 0x51c
			 0x26a692d5, /// 0x520
			 0x86d090a3, /// 0x524
			 0xcf38b2ba, /// 0x528
			 0x9a62a894, /// 0x52c
			 0xdcd1d06f, /// 0x530
			 0x1dfbcb86, /// 0x534
			 0x5fd514d, /// 0x538
			 0x226d9e56, /// 0x53c
			 0x51326b10, /// 0x540
			 0x83367d10, /// 0x544
			 0x88ab7ab7, /// 0x548
			 0x569bca22, /// 0x54c
			 0x88fa590b, /// 0x550
			 0xd72f7048, /// 0x554
			 0xd6bc0420, /// 0x558
			 0x32dfc0f5, /// 0x55c
			 0x9156704, /// 0x560
			 0x2fa8f718, /// 0x564
			 0x66ae7f5b, /// 0x568
			 0x1730ccd4, /// 0x56c
			 0x5d63c173, /// 0x570
			 0x7b2eb210, /// 0x574
			 0xe70ed463, /// 0x578
			 0xb2a81356, /// 0x57c
			 0xe6755e05, /// 0x580
			 0x8a53c925, /// 0x584
			 0x3c7d0d61, /// 0x588
			 0x1dcc843e, /// 0x58c
			 0x7c5d2303, /// 0x590
			 0xd626c1a3, /// 0x594
			 0x79db1c48, /// 0x598
			 0xf5787f80, /// 0x59c
			 0x22c241e6, /// 0x5a0
			 0x2d5859b3, /// 0x5a4
			 0x8157e3fd, /// 0x5a8
			 0x67dd1651, /// 0x5ac
			 0x463e2fab, /// 0x5b0
			 0xd8759b84, /// 0x5b4
			 0xca22c702, /// 0x5b8
			 0x8ec967f4, /// 0x5bc
			 0xbba30242, /// 0x5c0
			 0x5b43dea1, /// 0x5c4
			 0x274f06a8, /// 0x5c8
			 0xadb77409, /// 0x5cc
			 0x26bd647, /// 0x5d0
			 0x1563390f, /// 0x5d4
			 0x9eaa7c56, /// 0x5d8
			 0xb2f982d3, /// 0x5dc
			 0xc587ecee, /// 0x5e0
			 0xf4cfe6b3, /// 0x5e4
			 0xcc94c334, /// 0x5e8
			 0xda93c9e1, /// 0x5ec
			 0xeaea0d92, /// 0x5f0
			 0x56b03b84, /// 0x5f4
			 0xc9c84a0b, /// 0x5f8
			 0x305598e6, /// 0x5fc
			 0x514f5464, /// 0x600
			 0x89947218, /// 0x604
			 0x9401ffa1, /// 0x608
			 0xf338a3fb, /// 0x60c
			 0x24832396, /// 0x610
			 0x25ed0b04, /// 0x614
			 0x7ae2b1d9, /// 0x618
			 0xc0fba2aa, /// 0x61c
			 0x21ef6f5a, /// 0x620
			 0x96bf12d7, /// 0x624
			 0x6a5c4210, /// 0x628
			 0xab92b386, /// 0x62c
			 0x9490c7ce, /// 0x630
			 0x5c570401, /// 0x634
			 0x803ae8e9, /// 0x638
			 0xf975d27b, /// 0x63c
			 0xebe82575, /// 0x640
			 0xd8b09436, /// 0x644
			 0x2fa57ed8, /// 0x648
			 0xfe6c7c85, /// 0x64c
			 0x926adf79, /// 0x650
			 0x800710c2, /// 0x654
			 0x23d4436a, /// 0x658
			 0x1a1edc62, /// 0x65c
			 0xc992c737, /// 0x660
			 0x50c3adae, /// 0x664
			 0xb9f12458, /// 0x668
			 0x9ef8f22d, /// 0x66c
			 0x612c76a, /// 0x670
			 0xc918fb60, /// 0x674
			 0xa6207ca4, /// 0x678
			 0x92dea11, /// 0x67c
			 0x973bfb53, /// 0x680
			 0x40fc8740, /// 0x684
			 0xbac6b991, /// 0x688
			 0xc03878, /// 0x68c
			 0xafa1bfb9, /// 0x690
			 0x2c4e92f2, /// 0x694
			 0xf00dab76, /// 0x698
			 0x816789d5, /// 0x69c
			 0xe482568b, /// 0x6a0
			 0x69652211, /// 0x6a4
			 0x4fc23b06, /// 0x6a8
			 0x615a443e, /// 0x6ac
			 0x6a9ea0f8, /// 0x6b0
			 0x46fc1f8c, /// 0x6b4
			 0x7e728037, /// 0x6b8
			 0x66226a16, /// 0x6bc
			 0x83964a11, /// 0x6c0
			 0x5d33be92, /// 0x6c4
			 0xfa60016a, /// 0x6c8
			 0x8a29e18b, /// 0x6cc
			 0x55e6e18b, /// 0x6d0
			 0x23d09a43, /// 0x6d4
			 0xb72069dd, /// 0x6d8
			 0xa53d57b4, /// 0x6dc
			 0x55c1c7d2, /// 0x6e0
			 0x2a3f5dc0, /// 0x6e4
			 0xdf495e76, /// 0x6e8
			 0xffd0c882, /// 0x6ec
			 0xfe6c6fd5, /// 0x6f0
			 0x320da74c, /// 0x6f4
			 0x84057cfc, /// 0x6f8
			 0x7b046573, /// 0x6fc
			 0xef8ef83c, /// 0x700
			 0xf4d45fa8, /// 0x704
			 0xc0cc3c73, /// 0x708
			 0x972b502d, /// 0x70c
			 0x374efb25, /// 0x710
			 0x6a7447b1, /// 0x714
			 0x7ec07a3f, /// 0x718
			 0x4c0452c2, /// 0x71c
			 0xd4f753a9, /// 0x720
			 0x67093255, /// 0x724
			 0x23bd8533, /// 0x728
			 0xfe0f984, /// 0x72c
			 0xf0994765, /// 0x730
			 0xd3f7fb1, /// 0x734
			 0x2b61830c, /// 0x738
			 0x74b76cfe, /// 0x73c
			 0x36bfefe5, /// 0x740
			 0x53733e93, /// 0x744
			 0x4822c77d, /// 0x748
			 0x3b61f355, /// 0x74c
			 0x1f28e477, /// 0x750
			 0xcecb4fd9, /// 0x754
			 0xa8e876c, /// 0x758
			 0xd83bb8a0, /// 0x75c
			 0x84fd98f, /// 0x760
			 0x5d3e8ef4, /// 0x764
			 0xfb857785, /// 0x768
			 0x182f60e1, /// 0x76c
			 0xaef964ea, /// 0x770
			 0x45ff9a91, /// 0x774
			 0x39dc1089, /// 0x778
			 0x33525ad3, /// 0x77c
			 0x9dc6996b, /// 0x780
			 0x28f59e8c, /// 0x784
			 0xa1f5226d, /// 0x788
			 0x553ba7b, /// 0x78c
			 0x5b4b9491, /// 0x790
			 0xf35dca4b, /// 0x794
			 0x57ebab2b, /// 0x798
			 0x5799cb38, /// 0x79c
			 0x4673f7cc, /// 0x7a0
			 0xa0672e8e, /// 0x7a4
			 0x7b7cdaab, /// 0x7a8
			 0xb3eb26ce, /// 0x7ac
			 0x6e700915, /// 0x7b0
			 0xf6322089, /// 0x7b4
			 0x81ae76fe, /// 0x7b8
			 0x620b0e29, /// 0x7bc
			 0xba2d59bb, /// 0x7c0
			 0xea05c0b3, /// 0x7c4
			 0x1d5c6c0c, /// 0x7c8
			 0x5909fa76, /// 0x7cc
			 0x575390aa, /// 0x7d0
			 0xad40eb0f, /// 0x7d4
			 0x3e20ac53, /// 0x7d8
			 0x54612731, /// 0x7dc
			 0xca5abcb7, /// 0x7e0
			 0xcad09cf2, /// 0x7e4
			 0xa0a717c4, /// 0x7e8
			 0x12aff7a1, /// 0x7ec
			 0xa2b4cddf, /// 0x7f0
			 0x53ee99b5, /// 0x7f4
			 0x15810fbc, /// 0x7f8
			 0x62abdc1b, /// 0x7fc
			 0x72943aba, /// 0x800
			 0x7b76931, /// 0x804
			 0xd73347e5, /// 0x808
			 0x5e509a5c, /// 0x80c
			 0x7c69b971, /// 0x810
			 0xac874daa, /// 0x814
			 0x2a295fea, /// 0x818
			 0x2a1f1a82, /// 0x81c
			 0xcbff58a9, /// 0x820
			 0xcc3c5c22, /// 0x824
			 0x9c6369c3, /// 0x828
			 0x83b6c1bb, /// 0x82c
			 0x3f53599f, /// 0x830
			 0x2db5d981, /// 0x834
			 0xe6f8ef10, /// 0x838
			 0xf1f9834f, /// 0x83c
			 0xec3be4d9, /// 0x840
			 0x41b111c3, /// 0x844
			 0x28dd6098, /// 0x848
			 0x945a0618, /// 0x84c
			 0xd3d80ff1, /// 0x850
			 0xcf43c20e, /// 0x854
			 0xe3becc8b, /// 0x858
			 0x83224da8, /// 0x85c
			 0x7370e52c, /// 0x860
			 0x6c7a5f81, /// 0x864
			 0x6ac5145e, /// 0x868
			 0x10c1351b, /// 0x86c
			 0xebc48203, /// 0x870
			 0x3c009fcf, /// 0x874
			 0x926ca013, /// 0x878
			 0x40c66052, /// 0x87c
			 0xa576760e, /// 0x880
			 0xe8c88250, /// 0x884
			 0x5da2977, /// 0x888
			 0x1a52242d, /// 0x88c
			 0x47fdf17b, /// 0x890
			 0xf3137796, /// 0x894
			 0xc2227e3, /// 0x898
			 0x4050ef20, /// 0x89c
			 0x45431b31, /// 0x8a0
			 0xe8f4064b, /// 0x8a4
			 0x9d8222d9, /// 0x8a8
			 0xc259ddfd, /// 0x8ac
			 0x4613bf03, /// 0x8b0
			 0x606b7491, /// 0x8b4
			 0x9a1f0087, /// 0x8b8
			 0x43084463, /// 0x8bc
			 0x3bbe5101, /// 0x8c0
			 0xb250c73f, /// 0x8c4
			 0xd401601, /// 0x8c8
			 0x38867c94, /// 0x8cc
			 0x25ff35db, /// 0x8d0
			 0x5fd0c9c4, /// 0x8d4
			 0xd663ae9, /// 0x8d8
			 0x505df576, /// 0x8dc
			 0xec7e9215, /// 0x8e0
			 0xc1dafde8, /// 0x8e4
			 0xdd6ca468, /// 0x8e8
			 0x3d19bb97, /// 0x8ec
			 0x8ca94208, /// 0x8f0
			 0xb95cf658, /// 0x8f4
			 0x522ccdae, /// 0x8f8
			 0xb2edcbc3, /// 0x8fc
			 0x28dfd5be, /// 0x900
			 0x757714a6, /// 0x904
			 0x5244431a, /// 0x908
			 0xe48bc0c3, /// 0x90c
			 0x49d3aa0, /// 0x910
			 0x2acf3ac3, /// 0x914
			 0x355a499c, /// 0x918
			 0x768f81eb, /// 0x91c
			 0x7c64ed77, /// 0x920
			 0x5d0063f3, /// 0x924
			 0x90b86294, /// 0x928
			 0x846752a, /// 0x92c
			 0x4c244a54, /// 0x930
			 0x78cc1c64, /// 0x934
			 0xbe67d8db, /// 0x938
			 0xb922c8e, /// 0x93c
			 0x1c4ed123, /// 0x940
			 0xd1b3f7bb, /// 0x944
			 0x80a8ed8a, /// 0x948
			 0x88a149c0, /// 0x94c
			 0x6dac82b9, /// 0x950
			 0xb6e0ecb8, /// 0x954
			 0xe15b79de, /// 0x958
			 0x26083492, /// 0x95c
			 0x780b519b, /// 0x960
			 0x12ff3127, /// 0x964
			 0x8c81bffd, /// 0x968
			 0xd50462e1, /// 0x96c
			 0x9fa0fb21, /// 0x970
			 0xf50048da, /// 0x974
			 0x8352f5a3, /// 0x978
			 0x1810ff63, /// 0x97c
			 0xa1f54e93, /// 0x980
			 0x64dd19d1, /// 0x984
			 0xb75e2825, /// 0x988
			 0x26319139, /// 0x98c
			 0x94d3178b, /// 0x990
			 0x29ac0bab, /// 0x994
			 0x6a1291bc, /// 0x998
			 0x38f70bc5, /// 0x99c
			 0xddc9bf65, /// 0x9a0
			 0xf54380e6, /// 0x9a4
			 0x275a4d87, /// 0x9a8
			 0x780a9441, /// 0x9ac
			 0x148f2369, /// 0x9b0
			 0xe938fba6, /// 0x9b4
			 0x4dd71b50, /// 0x9b8
			 0x9b0a4521, /// 0x9bc
			 0xcf8b590c, /// 0x9c0
			 0xc26ebb2d, /// 0x9c4
			 0x9d70a77c, /// 0x9c8
			 0x4558a599, /// 0x9cc
			 0x5d5a884c, /// 0x9d0
			 0xd464bcd4, /// 0x9d4
			 0x5757e90b, /// 0x9d8
			 0x73db6600, /// 0x9dc
			 0xae9d11, /// 0x9e0
			 0xc38bccbc, /// 0x9e4
			 0xaec4ebda, /// 0x9e8
			 0x809ac033, /// 0x9ec
			 0x9c81b8c, /// 0x9f0
			 0x14919cfe, /// 0x9f4
			 0x5f3c1243, /// 0x9f8
			 0x402693fa, /// 0x9fc
			 0x4592f795, /// 0xa00
			 0x229b2508, /// 0xa04
			 0x8c2e2196, /// 0xa08
			 0x5fa7f206, /// 0xa0c
			 0x5b0544f6, /// 0xa10
			 0x20ca35e, /// 0xa14
			 0x77a81915, /// 0xa18
			 0x438512ce, /// 0xa1c
			 0x941b22c, /// 0xa20
			 0x94b50987, /// 0xa24
			 0xfd6cf3d5, /// 0xa28
			 0x5ee57e7e, /// 0xa2c
			 0x35208ca2, /// 0xa30
			 0x2f744338, /// 0xa34
			 0xaeb2d6e2, /// 0xa38
			 0x4c315652, /// 0xa3c
			 0xbd41b751, /// 0xa40
			 0x62c594df, /// 0xa44
			 0x4d92fe0f, /// 0xa48
			 0x2472a47e, /// 0xa4c
			 0x723580aa, /// 0xa50
			 0x87c40e52, /// 0xa54
			 0x90989b89, /// 0xa58
			 0xd97b1996, /// 0xa5c
			 0xc3aeac28, /// 0xa60
			 0x94c7e325, /// 0xa64
			 0xa4094d0f, /// 0xa68
			 0x28a517f0, /// 0xa6c
			 0xb66f147a, /// 0xa70
			 0xf8e257c4, /// 0xa74
			 0x13acf7fa, /// 0xa78
			 0xcc20adb0, /// 0xa7c
			 0x885604d3, /// 0xa80
			 0x3eb3913d, /// 0xa84
			 0xe7b6502c, /// 0xa88
			 0xc13a6f6f, /// 0xa8c
			 0x5755c65a, /// 0xa90
			 0xbf21e56, /// 0xa94
			 0xac353a14, /// 0xa98
			 0xe086b0e8, /// 0xa9c
			 0xdeba8d74, /// 0xaa0
			 0x42457000, /// 0xaa4
			 0xe680fe65, /// 0xaa8
			 0x939e2daf, /// 0xaac
			 0x514817dc, /// 0xab0
			 0xf7a02ea5, /// 0xab4
			 0x75cd5292, /// 0xab8
			 0x43b52238, /// 0xabc
			 0x5603c680, /// 0xac0
			 0x876892d6, /// 0xac4
			 0x28d99689, /// 0xac8
			 0xa8fb4cb2, /// 0xacc
			 0xd89acf3b, /// 0xad0
			 0xc32f9877, /// 0xad4
			 0x3b01ef1c, /// 0xad8
			 0xbd071ee2, /// 0xadc
			 0xc9704a48, /// 0xae0
			 0x21231a46, /// 0xae4
			 0xf3388c09, /// 0xae8
			 0x85daeb06, /// 0xaec
			 0x208e16a2, /// 0xaf0
			 0xdcfff3bd, /// 0xaf4
			 0xe8a7f94d, /// 0xaf8
			 0xcb857af1, /// 0xafc
			 0x7ccb801a, /// 0xb00
			 0xe12ac75a, /// 0xb04
			 0x31f5a558, /// 0xb08
			 0xf890f771, /// 0xb0c
			 0x1a5c0cb6, /// 0xb10
			 0xccfb5e50, /// 0xb14
			 0x565cf73e, /// 0xb18
			 0x3851fc26, /// 0xb1c
			 0xa3d5060a, /// 0xb20
			 0x9e599630, /// 0xb24
			 0xb5ae72a9, /// 0xb28
			 0x2212163, /// 0xb2c
			 0x4024ec1f, /// 0xb30
			 0x5b1a9581, /// 0xb34
			 0x50921b86, /// 0xb38
			 0x713ae3ee, /// 0xb3c
			 0x1f069aee, /// 0xb40
			 0xac74a2dc, /// 0xb44
			 0x2de9be79, /// 0xb48
			 0x77e92728, /// 0xb4c
			 0x1ddddfb1, /// 0xb50
			 0x441cf7d8, /// 0xb54
			 0xb5b58db7, /// 0xb58
			 0x758c82a8, /// 0xb5c
			 0xaa32cfea, /// 0xb60
			 0xf3a32c52, /// 0xb64
			 0x28315573, /// 0xb68
			 0x90e557e0, /// 0xb6c
			 0x14c7ec09, /// 0xb70
			 0xdebfa500, /// 0xb74
			 0xda0d8b6, /// 0xb78
			 0x86526ccd, /// 0xb7c
			 0xa4367f7c, /// 0xb80
			 0x6da0a409, /// 0xb84
			 0xb550f07b, /// 0xb88
			 0xdee4b018, /// 0xb8c
			 0xa058a2e2, /// 0xb90
			 0x229b191, /// 0xb94
			 0x1a74e537, /// 0xb98
			 0xb2054426, /// 0xb9c
			 0xbb7d1904, /// 0xba0
			 0x65d55959, /// 0xba4
			 0xd2e37e06, /// 0xba8
			 0x1119c5ef, /// 0xbac
			 0xc33fddf6, /// 0xbb0
			 0xc2af8648, /// 0xbb4
			 0xabd9af27, /// 0xbb8
			 0x65a1ca74, /// 0xbbc
			 0xce81261, /// 0xbc0
			 0x73b2dcf6, /// 0xbc4
			 0x711cb4ad, /// 0xbc8
			 0xbf7f2ce7, /// 0xbcc
			 0x6bbc8fe6, /// 0xbd0
			 0xcb291f7b, /// 0xbd4
			 0xb0c23035, /// 0xbd8
			 0x1eb47191, /// 0xbdc
			 0xb39f8f78, /// 0xbe0
			 0xf1a2710, /// 0xbe4
			 0x1ac62559, /// 0xbe8
			 0x9a790952, /// 0xbec
			 0xd9fdbfd1, /// 0xbf0
			 0x22ea750a, /// 0xbf4
			 0xa2cf4af2, /// 0xbf8
			 0xc08400c3, /// 0xbfc
			 0x77d9eaa7, /// 0xc00
			 0x3b69440b, /// 0xc04
			 0x96b107e, /// 0xc08
			 0xe068469f, /// 0xc0c
			 0x8fac75fd, /// 0xc10
			 0x2f3955b4, /// 0xc14
			 0x4cc3f13f, /// 0xc18
			 0x39278efe, /// 0xc1c
			 0xc6433337, /// 0xc20
			 0x7f7c7745, /// 0xc24
			 0x9604c6e2, /// 0xc28
			 0x7f51b05c, /// 0xc2c
			 0xbcd7a2df, /// 0xc30
			 0x654657d2, /// 0xc34
			 0x7368eb67, /// 0xc38
			 0x6d28ae3, /// 0xc3c
			 0x46ef4621, /// 0xc40
			 0xfa2fb350, /// 0xc44
			 0x667a27bc, /// 0xc48
			 0x571cfceb, /// 0xc4c
			 0xae8c809e, /// 0xc50
			 0x97cbcff6, /// 0xc54
			 0x8a9be058, /// 0xc58
			 0x588ed27c, /// 0xc5c
			 0x46a84a70, /// 0xc60
			 0xf34ad0f4, /// 0xc64
			 0xc18ee8bc, /// 0xc68
			 0x3ec76a94, /// 0xc6c
			 0xa0925063, /// 0xc70
			 0x63034a88, /// 0xc74
			 0x42ad2730, /// 0xc78
			 0x82d7d90a, /// 0xc7c
			 0x4eb8dd74, /// 0xc80
			 0x1cffa49f, /// 0xc84
			 0x87cfd905, /// 0xc88
			 0x2085dfbc, /// 0xc8c
			 0x7383b868, /// 0xc90
			 0x745fb0b4, /// 0xc94
			 0x21ddb7fc, /// 0xc98
			 0xaf820df2, /// 0xc9c
			 0xf3d9ad97, /// 0xca0
			 0x45bc9363, /// 0xca4
			 0xabed622a, /// 0xca8
			 0x48191944, /// 0xcac
			 0xafcc2284, /// 0xcb0
			 0x2ce540cc, /// 0xcb4
			 0x57044c83, /// 0xcb8
			 0x39d1930f, /// 0xcbc
			 0x4d16cae8, /// 0xcc0
			 0xc0dce6e3, /// 0xcc4
			 0xff9ae11, /// 0xcc8
			 0x9217fac3, /// 0xccc
			 0x5a41d6fb, /// 0xcd0
			 0x5a5bdef, /// 0xcd4
			 0x8b2a39e0, /// 0xcd8
			 0xa999e285, /// 0xcdc
			 0xcb5e4267, /// 0xce0
			 0xfedc4b94, /// 0xce4
			 0xc4c990ba, /// 0xce8
			 0x298238dc, /// 0xcec
			 0x124bcd97, /// 0xcf0
			 0x556c8fed, /// 0xcf4
			 0x8ec9dced, /// 0xcf8
			 0xd164d4f9, /// 0xcfc
			 0xcaacb566, /// 0xd00
			 0x19c2d9a, /// 0xd04
			 0x3e4a99be, /// 0xd08
			 0xc7ae3567, /// 0xd0c
			 0xd5d31e06, /// 0xd10
			 0x1568ece2, /// 0xd14
			 0xdfaa0178, /// 0xd18
			 0x713067d4, /// 0xd1c
			 0xa2e0d2a6, /// 0xd20
			 0x4c68ad92, /// 0xd24
			 0xfffe43ce, /// 0xd28
			 0xc3b4283f, /// 0xd2c
			 0xbbf7b7e1, /// 0xd30
			 0x6e6c632a, /// 0xd34
			 0x8178b2c9, /// 0xd38
			 0xb1b6aa1f, /// 0xd3c
			 0xf2372750, /// 0xd40
			 0xadd1207c, /// 0xd44
			 0xdedeccc2, /// 0xd48
			 0xc6677c6c, /// 0xd4c
			 0x54751a5f, /// 0xd50
			 0x2e1d700f, /// 0xd54
			 0xa911c38b, /// 0xd58
			 0xa303cb3e, /// 0xd5c
			 0x25b47f8c, /// 0xd60
			 0x41f28ea7, /// 0xd64
			 0xaabbc137, /// 0xd68
			 0x843f47f7, /// 0xd6c
			 0x487724cd, /// 0xd70
			 0x7d3522cb, /// 0xd74
			 0x14fdb54c, /// 0xd78
			 0x6f66e4e9, /// 0xd7c
			 0x768cdfad, /// 0xd80
			 0xad97f1e3, /// 0xd84
			 0x50950e46, /// 0xd88
			 0x7bcb3dfb, /// 0xd8c
			 0x328613c8, /// 0xd90
			 0xf75d6aa, /// 0xd94
			 0xeffec47, /// 0xd98
			 0xd4cea9f2, /// 0xd9c
			 0x48687b49, /// 0xda0
			 0x83ef193, /// 0xda4
			 0xeeaedab6, /// 0xda8
			 0x7dd86bd4, /// 0xdac
			 0xc9a1962, /// 0xdb0
			 0xb617c087, /// 0xdb4
			 0x6ee1344f, /// 0xdb8
			 0xbe2b5d0e, /// 0xdbc
			 0xef7f9803, /// 0xdc0
			 0x88d7c31c, /// 0xdc4
			 0x9541becb, /// 0xdc8
			 0x1337d2a, /// 0xdcc
			 0x8adac1ef, /// 0xdd0
			 0x71b4efff, /// 0xdd4
			 0xb696d916, /// 0xdd8
			 0x8dbe86b8, /// 0xddc
			 0x14022258, /// 0xde0
			 0x6c4938a8, /// 0xde4
			 0x24c4708b, /// 0xde8
			 0xd3fc45c5, /// 0xdec
			 0x54ca2904, /// 0xdf0
			 0x6b82caa0, /// 0xdf4
			 0x5066dd4d, /// 0xdf8
			 0x7e34c681, /// 0xdfc
			 0x78504bc4, /// 0xe00
			 0xe9d3fdd6, /// 0xe04
			 0x4b8a7f61, /// 0xe08
			 0x6d52aa94, /// 0xe0c
			 0xc57c06df, /// 0xe10
			 0x723eb63, /// 0xe14
			 0xf95a5318, /// 0xe18
			 0x40e079d9, /// 0xe1c
			 0xd6e9, /// 0xe20
			 0xd7deffbd, /// 0xe24
			 0x34e09d02, /// 0xe28
			 0xaeb000d4, /// 0xe2c
			 0xfd8ac094, /// 0xe30
			 0x778a9f46, /// 0xe34
			 0x252ddfd1, /// 0xe38
			 0x17985bb8, /// 0xe3c
			 0xc911dc60, /// 0xe40
			 0xdc99f04a, /// 0xe44
			 0x85cb85a1, /// 0xe48
			 0x700670af, /// 0xe4c
			 0xd2272825, /// 0xe50
			 0x78ce8bd0, /// 0xe54
			 0x77ccaa3b, /// 0xe58
			 0x9b4efe20, /// 0xe5c
			 0x4b4f7c5d, /// 0xe60
			 0xf60fe4fd, /// 0xe64
			 0xa703aad7, /// 0xe68
			 0x7036863, /// 0xe6c
			 0x5043998c, /// 0xe70
			 0x538610c5, /// 0xe74
			 0xdfb3e18c, /// 0xe78
			 0x25458e5d, /// 0xe7c
			 0x19ccc397, /// 0xe80
			 0xf34d3af1, /// 0xe84
			 0xffb1af72, /// 0xe88
			 0x3cab2db7, /// 0xe8c
			 0x62b57a86, /// 0xe90
			 0x38bb139d, /// 0xe94
			 0x24ae8581, /// 0xe98
			 0xde001bd1, /// 0xe9c
			 0x88d6842c, /// 0xea0
			 0x9d399bd5, /// 0xea4
			 0xa5c3e6a2, /// 0xea8
			 0xf0cb4c48, /// 0xeac
			 0xbaa0a555, /// 0xeb0
			 0xe1353df1, /// 0xeb4
			 0x7426296d, /// 0xeb8
			 0xa0e202ec, /// 0xebc
			 0x8d8f89c7, /// 0xec0
			 0x51ba9309, /// 0xec4
			 0x3f0651ca, /// 0xec8
			 0xf4edeef6, /// 0xecc
			 0xf0d0726f, /// 0xed0
			 0xe8f39f1a, /// 0xed4
			 0x814041fd, /// 0xed8
			 0x2c24b1d9, /// 0xedc
			 0xf7ec74ef, /// 0xee0
			 0x4d63fbd9, /// 0xee4
			 0x9f8c7ece, /// 0xee8
			 0x2d934812, /// 0xeec
			 0x33c52ba4, /// 0xef0
			 0xffb94b21, /// 0xef4
			 0xe569737a, /// 0xef8
			 0x9afbe2b3, /// 0xefc
			 0xfe836f9d, /// 0xf00
			 0x6a57a1e1, /// 0xf04
			 0x31cd559c, /// 0xf08
			 0x9d2aa58c, /// 0xf0c
			 0xff80ae16, /// 0xf10
			 0xd6950522, /// 0xf14
			 0xda30cf0d, /// 0xf18
			 0x90cfdb4d, /// 0xf1c
			 0x9ecd0e0a, /// 0xf20
			 0x6030f1b2, /// 0xf24
			 0xc926036a, /// 0xf28
			 0x2cd5c682, /// 0xf2c
			 0xa5ab8383, /// 0xf30
			 0xd4c58896, /// 0xf34
			 0xb9916562, /// 0xf38
			 0x302d5615, /// 0xf3c
			 0xa8b2027, /// 0xf40
			 0xe15f8210, /// 0xf44
			 0xfc364e48, /// 0xf48
			 0xb0389819, /// 0xf4c
			 0xa801406e, /// 0xf50
			 0x5287f68f, /// 0xf54
			 0xf348187c, /// 0xf58
			 0x99f09780, /// 0xf5c
			 0x98c874e9, /// 0xf60
			 0xecb25783, /// 0xf64
			 0x4da9b1eb, /// 0xf68
			 0x71b693e2, /// 0xf6c
			 0x5618223a, /// 0xf70
			 0x404c9b27, /// 0xf74
			 0xa3560ed8, /// 0xf78
			 0xe9916325, /// 0xf7c
			 0xebb4338e, /// 0xf80
			 0xd19af60d, /// 0xf84
			 0x3041bc1b, /// 0xf88
			 0x7421ccc8, /// 0xf8c
			 0xcfb9bcad, /// 0xf90
			 0xab3e6896, /// 0xf94
			 0xd8059cca, /// 0xf98
			 0xc308c06, /// 0xf9c
			 0x8291c3a9, /// 0xfa0
			 0x3f8b9018, /// 0xfa4
			 0x60a206d7, /// 0xfa8
			 0x28ef9133, /// 0xfac
			 0xcfec5b25, /// 0xfb0
			 0x1d29d208, /// 0xfb4
			 0x479a2664, /// 0xfb8
			 0x974a3b60, /// 0xfbc
			 0x30c50c6b, /// 0xfc0
			 0x9db4e93b, /// 0xfc4
			 0x399ad38a, /// 0xfc8
			 0x754a96a7, /// 0xfcc
			 0xb966716b, /// 0xfd0
			 0x95e05f9, /// 0xfd4
			 0x9b20e152, /// 0xfd8
			 0xe00b77a0, /// 0xfdc
			 0xbf0a0a96, /// 0xfe0
			 0xe9feb541, /// 0xfe4
			 0x87f8340e, /// 0xfe8
			 0x9fbf4377, /// 0xfec
			 0xa2558690, /// 0xff0
			 0xac91b394, /// 0xff4
			 0x305d1d4, /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x38
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x40
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x44
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x50
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x70
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x78
			 0x00000000,                                                  // zero                                        /// 0x7c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x98
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xa0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xa8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xb4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xc8
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xcc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xd0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd4
			 0x7f800000,                                                  // inf                                         /// 0xd8
			 0x80000000,                                                  // -zero                                       /// 0xdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xe0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xe4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xe8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xf0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xf4
			 0x7fc00001,                                                  // signaling NaN                               /// 0xf8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x100
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x104
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x108
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x10c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x110
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x114
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x118
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x11c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x120
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x124
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x128
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x12c
			 0xffc00001,                                                  // -signaling NaN                              /// 0x130
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x134
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x138
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x13c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x140
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x144
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x148
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x14c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x150
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x154
			 0x33333333,                                                  // 4 random values                             /// 0x158
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x15c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x160
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x164
			 0x3f028f5c,                                                  // 0.51                                        /// 0x168
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x16c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x170
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x174
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x178
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x17c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x180
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x184
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x188
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x18c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x190
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x194
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x198
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x19c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x1a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x1ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x1b0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x1b4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x1b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x1c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x1c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x1c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x1cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1d0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1d4
			 0x7f800000,                                                  // inf                                         /// 0x1d8
			 0x7fc00001,                                                  // signaling NaN                               /// 0x1dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x1e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x1e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x1f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1f4
			 0x4b000000,                                                  // 8388608.0                                   /// 0x1f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x1fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x200
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x204
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x208
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x20c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x210
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x214
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x218
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x21c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x220
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x224
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x228
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x22c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x230
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x234
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x238
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x23c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x240
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x244
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x248
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x24c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x250
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x254
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x258
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x25c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x260
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x264
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x268
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x26c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x270
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x274
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x278
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x27c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x280
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x284
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x288
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x28c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x290
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x294
			 0xbf028f5c,                                                  // -0.51                                       /// 0x298
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x29c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x2a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x2a8
			 0x80000000,                                                  // -zero                                       /// 0x2ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x2b8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x2c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x2c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x2c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x2cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x2d0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x2d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x2d8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x2dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x2e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x2e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x2f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x2f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x2fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x300
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x304
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x308
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x30c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x310
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x314
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x318
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x31c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x320
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x324
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x328
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x32c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x330
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x334
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x338
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x33c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x340
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x344
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x348
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x34c
			 0x80000000,                                                  // -zero                                       /// 0x350
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x354
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x358
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x35c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x360
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x364
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x368
			 0x55555555,                                                  // 4 random values                             /// 0x36c
			 0x00000000,                                                  // zero                                        /// 0x370
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x374
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x378
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x37c
			 0x7f800000,                                                  // inf                                         /// 0x380
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x384
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x388
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x38c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x390
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x394
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x398
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x39c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x3a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x3a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x3a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x3ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x3b4
			 0xbf028f5c,                                                  // -0.51                                       /// 0x3b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x3bc
			 0x3f028f5c,                                                  // 0.51                                        /// 0x3c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x3c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x3c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3cc
			 0x80000000,                                                  // -zero                                       /// 0x3d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x3d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x3d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x3dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3e4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3e8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x3f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x3f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3f8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x3fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x400
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x404
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x408
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x40c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x410
			 0x7fc00001,                                                  // signaling NaN                               /// 0x414
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x418
			 0x00000000,                                                  // zero                                        /// 0x41c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x420
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x424
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x428
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x42c
			 0x7f800000,                                                  // inf                                         /// 0x430
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x434
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x438
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x43c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x440
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x444
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x448
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x44c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x450
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x454
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x458
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x45c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x460
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x464
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x468
			 0x33333333,                                                  // 4 random values                             /// 0x46c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x470
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x474
			 0x4b000000,                                                  // 8388608.0                                   /// 0x478
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x47c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x480
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x484
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x488
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x48c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x490
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x494
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x498
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x49c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x4a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x4a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x4a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x4b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x4b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x4c0
			 0x00000000,                                                  // zero                                        /// 0x4c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x4c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x4d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4d8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x4dc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x4e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x4e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x4e8
			 0x33333333,                                                  // 4 random values                             /// 0x4ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x4f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x4f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x4f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x4fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x500
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x504
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x508
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x50c
			 0x7fc00001,                                                  // signaling NaN                               /// 0x510
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x514
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x518
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x51c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x520
			 0x00000000,                                                  // zero                                        /// 0x524
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x528
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x52c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x530
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x534
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x538
			 0xbf028f5c,                                                  // -0.51                                       /// 0x53c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x540
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x544
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x548
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x54c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x550
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x554
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x558
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x55c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x560
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x564
			 0x4b000000,                                                  // 8388608.0                                   /// 0x568
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x56c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x570
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x574
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x578
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x57c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x580
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x584
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x588
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x58c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x590
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x594
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x598
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x59c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x5a0
			 0x7fc00001,                                                  // signaling NaN                               /// 0x5a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x5a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x5b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x5b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x5bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x5c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x5c8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x5cc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x5d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x5d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5dc
			 0x7f800000,                                                  // inf                                         /// 0x5e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x5e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x5ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x5f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x5f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5fc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x600
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x604
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x608
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x60c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x610
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x614
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x618
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x61c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x620
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x624
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x628
			 0x4b000000,                                                  // 8388608.0                                   /// 0x62c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x630
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x634
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x638
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x63c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x640
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x644
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x648
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x64c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x650
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x654
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x658
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x65c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x664
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x668
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x66c
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x670
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x674
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x678
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x67c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x680
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x684
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x688
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x68c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x690
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x694
			 0xff800000,                                                  // -inf                                        /// 0x698
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x69c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x6a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x6a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x6a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x6b0
			 0xffc00001,                                                  // -signaling NaN                              /// 0x6b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x6b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x6bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x6c0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x6c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x6cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x6d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x6d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x6d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x6e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x6e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x6e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x6ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x6f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x6f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x6f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x6fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x700
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x704
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x708
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x70c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x710
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x714
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x718
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x71c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x720
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x724
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x728
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x72c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x730
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x734
			 0xffc00001,                                                  // -signaling NaN                              /// 0x738
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x73c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x740
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x744
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x748
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x74c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x750
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x754
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x758
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x75c
			 0x4b000000,                                                  // 8388608.0                                   /// 0x760
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x764
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x768
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x76c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x770
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x774
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x778
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x77c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x780
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x784
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x788
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x78c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x790
			 0x33333333,                                                  // 4 random values                             /// 0x794
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x798
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x79c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x7a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x7a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x7a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x7ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x7b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x7bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x7c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x7c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x7c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x7cc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x7d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x7d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x7dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x7e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x7e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x7e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x7ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x7f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x7fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x800
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x804
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x808
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x80c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x810
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x814
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x818
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x81c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x820
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x824
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x828
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x82c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x830
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x834
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x838
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x83c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x840
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x844
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x848
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x84c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x850
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x854
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x858
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x85c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x860
			 0x00000000,                                                  // zero                                        /// 0x864
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x868
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x86c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x870
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x874
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x878
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x87c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x880
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x884
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x888
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x88c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x890
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x894
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x898
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x89c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x8a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x8a8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x8b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x8b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x8b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x8c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x8c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8cc
			 0x7fc00001,                                                  // signaling NaN                               /// 0x8d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x8e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x8e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x8e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x8ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x8f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x8f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8f8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x900
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x904
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x908
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x90c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x910
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x914
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x918
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x91c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x920
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x924
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x928
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x92c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x930
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x934
			 0xffc00001,                                                  // -signaling NaN                              /// 0x938
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x93c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x940
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x944
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x948
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x94c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x950
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x954
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x958
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x95c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x960
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x964
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x968
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x96c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x970
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x974
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x978
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x97c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x980
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x984
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x988
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x98c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x990
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x994
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x998
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x99c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9a4
			 0x55555555,                                                  // 4 random values                             /// 0x9a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x9ac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x9b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x9b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x9b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x9bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x9c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x9c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x9c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x9cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x9d0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x9d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x9d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x9dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x9e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x9e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x9ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x9f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x9f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x9f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x9fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xa00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xa04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xa08
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xa0c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa10
			 0x00000000,                                                  // zero                                        /// 0xa14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xa20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xa24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xa28
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xa2c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xa30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xa34
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xa40
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xa44
			 0xff800000,                                                  // -inf                                        /// 0xa48
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xa50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xa54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xa60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xa64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xa68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xa6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xa70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xa74
			 0xcb000000,                                                  // -8388608.0                                  /// 0xa78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xa7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa80
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa8c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xa90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xa94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xa98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xa9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xaa0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaa4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xaa8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xaac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xab0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xab4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xab8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xabc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xac0
			 0x00000000,                                                  // zero                                        /// 0xac4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xac8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xacc
			 0x7f800000,                                                  // inf                                         /// 0xad0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xad4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xad8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xadc
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xae0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xae4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xae8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xaec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xaf0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xaf4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xaf8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xafc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xb00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xb04
			 0x55555555,                                                  // 4 random values                             /// 0xb08
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xb0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb10
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xb14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xb18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xb1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xb20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xb28
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xb2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xb30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb38
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xb3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xb40
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xb44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xb48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xb4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xb54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xb58
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xb5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xb60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xb68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xb6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xb74
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xb78
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xb7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xb80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xb84
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xb8c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xb90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb94
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xb9c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xba0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xba4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xba8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xbac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xbb0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xbb4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xbb8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xbbc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xbc0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xbc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xbcc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xbd0
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xbd4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbd8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbdc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xbe0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xbe4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xbe8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xbec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xbf0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xbf4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xbf8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xbfc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xc00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xc04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xc0c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc10
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xc14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xc18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xc20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xc24
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xc2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xc38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xc3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xc40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xc44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc48
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc4c
			 0x55555555,                                                  // 4 random values                             /// 0xc50
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xc54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc58
			 0x55555555,                                                  // 4 random values                             /// 0xc5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xc60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xc64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xc6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xc70
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xc74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xc78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xc7c
			 0x55555555,                                                  // 4 random values                             /// 0xc80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xc84
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xc88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xc8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xc90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc94
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xc9c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xca0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xca4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xca8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xcac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0xcb0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xcb4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xcbc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xcc8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xccc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcd0
			 0x55555555,                                                  // 4 random values                             /// 0xcd4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xcd8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xcdc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xce0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xce4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xce8
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xcec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcf0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xcf4
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xcf8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xcfc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xd00
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xd04
			 0x7f800000,                                                  // inf                                         /// 0xd08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xd0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd14
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xd18
			 0x80000000,                                                  // -zero                                       /// 0xd1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xd20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xd30
			 0x80000000,                                                  // -zero                                       /// 0xd34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xd3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xd40
			 0x80000000,                                                  // -zero                                       /// 0xd44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xd48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xd50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xd54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xd58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd5c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xd60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xd6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xd70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xd78
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xd7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xd80
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd84
			 0x33333333,                                                  // 4 random values                             /// 0xd88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xd8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xd90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xd94
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xd98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xd9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xda0
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xda4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xda8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdb0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xdb4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdb8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xdbc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdc0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdc8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xdcc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xdd0
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xdd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xdd8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xddc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xde0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xde4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xde8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xdec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xdf0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdf4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xdf8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xdfc
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xe00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xe04
			 0x33333333,                                                  // 4 random values                             /// 0xe08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xe0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xe10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xe14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xe18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xe1c
			 0x3f028f5c,                                                  // 0.51                                        /// 0xe20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xe24
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xe28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xe2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xe30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xe34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xe3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xe40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xe44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xe4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xe54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xe58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xe5c
			 0xffc00001,                                                  // -signaling NaN                              /// 0xe60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe6c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe70
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xe74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xe78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xe7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xe80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xe84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xe8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xe90
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xe94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe98
			 0x7fc00001,                                                  // signaling NaN                               /// 0xe9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xea0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xea4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xea8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xeac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xeb0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xeb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xeb8
			 0x7f800000,                                                  // inf                                         /// 0xebc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xec0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xec4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xec8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xecc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xed0
			 0x4b000000,                                                  // 8388608.0                                   /// 0xed4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xed8
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xedc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xee0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xee4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xee8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xeec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xef0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xef4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xef8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xefc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xf00
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xf04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xf08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xf10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xf14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf18
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xf1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xf20
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xf24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xf28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf30
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xf38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xf3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xf40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xf44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xf48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xf4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xf50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xf54
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xf58
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf5c
			 0x80000000,                                                  // -zero                                       /// 0xf60
			 0x4b000000,                                                  // 8388608.0                                   /// 0xf64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xf68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xf70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xf74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xf78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xf7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xf80
			 0x80000000,                                                  // -zero                                       /// 0xf84
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xf88
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xf8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xf90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xf94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xf98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xf9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xfa0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xfa4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfa8
			 0x3f028f5c,                                                  // 0.51                                        /// 0xfac
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xfb0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfb4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfb8
			 0xcb000000,                                                  // -8388608.0                                  /// 0xfbc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xfc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xfc8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xfcc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xfd0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xfd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xfd8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfdc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xfe0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xfe4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfe8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xff0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xff4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

			 // vpu register f0
			 0x00000000,                      
			 0x00000000,                      
			 0x00000000,                      
			 0x00000000,                      
			 0x00000000,                      
			 0x00000000,                      
			 0x00000000,                      
			 0x00000000,                      

			 // vpu register f1
			 0x00000100,                      
			 0x00000200,                      
			 0x00000300,                      
			 0x00000400,                      
			 0x00000500,                      
			 0x00000600,                      
			 0x00000700,                      
			 0x00000800,                      

			 // vpu register f2
			 0x41700000,
			 0x41200000,
			 0x40c00000,
			 0x40800000,
			 0x41400000,
			 0x41b00000,
			 0x41f00000,
			 0x41b00000,

			 // vpu register f3
			 0x41e00000,
			 0x3f800000,
			 0x41d80000,
			 0x40c00000,
			 0x41200000,
			 0x41b00000,
			 0x41900000,
			 0x40a00000,

			 // vpu register f4
			 0x40000000,
			 0x41f00000,
			 0x41200000,
			 0x3f800000,
			 0x41b00000,
			 0x41100000,
			 0x41b80000,
			 0x41880000,

			 // vpu register f5
			 0x41400000,
			 0x40c00000,
			 0x41100000,
			 0x41300000,
			 0x40e00000,
			 0x42000000,
			 0x41880000,
			 0x41b00000,

			 // vpu register f6
			 0x41200000,
			 0x41b00000,
			 0x41900000,
			 0x40400000,
			 0x41000000,
			 0x3f800000,
			 0x41500000,
			 0x41600000,

			 // vpu register f7
			 0x41b00000,
			 0x41d80000,
			 0x41f00000,
			 0x41c00000,
			 0x41800000,
			 0x41a00000,
			 0x40000000,
			 0x41a00000,

			 // vpu register f8
			 0x42000000,
			 0x41c80000,
			 0x41b00000,
			 0x41e00000,
			 0x41900000,
			 0x40800000,
			 0x41d00000,
			 0x40000000,

			 // vpu register f9
			 0x41900000,
			 0x41200000,
			 0x41900000,
			 0x41800000,
			 0x42000000,
			 0x41e80000,
			 0x41980000,
			 0x41000000,

			 // vpu register f10
			 0x41800000,
			 0x41600000,
			 0x40e00000,
			 0x41b80000,
			 0x41600000,
			 0x41300000,
			 0x41f00000,
			 0x41600000,

			 // vpu register f11
			 0x41500000,
			 0x41a80000,
			 0x41500000,
			 0x41a00000,
			 0x42000000,
			 0x41980000,
			 0x40e00000,
			 0x41f80000,

			 // vpu register f12
			 0x41c80000,
			 0x3f800000,
			 0x41b00000,
			 0x41100000,
			 0x40c00000,
			 0x41700000,
			 0x41b80000,
			 0x41900000,

			 // vpu register f13
			 0x41200000,
			 0x40000000,
			 0x41c80000,
			 0x41e00000,
			 0x40400000,
			 0x42000000,
			 0x41980000,
			 0x41600000,

			 // vpu register f14
			 0x41200000,
			 0x41400000,
			 0x41300000,
			 0x41600000,
			 0x41f00000,
			 0x3f800000,
			 0x40e00000,
			 0x41400000,

			 // vpu register f15
			 0x41800000,
			 0x41300000,
			 0x41600000,
			 0x41e00000,
			 0x41f80000,
			 0x3f800000,
			 0x41a80000,
			 0x41b00000,

			 // vpu register f16
			 0x41700000,
			 0x41100000,
			 0x41d80000,
			 0x41500000,
			 0x41300000,
			 0x40000000,
			 0x41100000,
			 0x41e00000,

			 // vpu register f17
			 0x41300000,
			 0x40000000,
			 0x42000000,
			 0x41a80000,
			 0x41200000,
			 0x41100000,
			 0x41d00000,
			 0x41880000,

			 // vpu register f18
			 0x41a00000,
			 0x40000000,
			 0x41900000,
			 0x41d80000,
			 0x41f80000,
			 0x41400000,
			 0x41b00000,
			 0x41a00000,

			 // vpu register f19
			 0x41980000,
			 0x41200000,
			 0x41800000,
			 0x41600000,
			 0x41200000,
			 0x41880000,
			 0x41980000,
			 0x41400000,

			 // vpu register f20
			 0x41200000,
			 0x41e00000,
			 0x41800000,
			 0x40000000,
			 0x41e80000,
			 0x40400000,
			 0x3f800000,
			 0x40a00000,

			 // vpu register f21
			 0x41f80000,
			 0x42000000,
			 0x40800000,
			 0x40000000,
			 0x41600000,
			 0x41f00000,
			 0x41300000,
			 0x41b00000,

			 // vpu register f22
			 0x41600000,
			 0x41b80000,
			 0x41980000,
			 0x41c00000,
			 0x41e00000,
			 0x3f800000,
			 0x41980000,
			 0x41f80000,

			 // vpu register f23
			 0x41e00000,
			 0x3f800000,
			 0x41200000,
			 0x41880000,
			 0x41000000,
			 0x41c00000,
			 0x41d00000,
			 0x41d80000,

			 // vpu register f24
			 0x41500000,
			 0x41800000,
			 0x41600000,
			 0x41800000,
			 0x41e00000,
			 0x41800000,
			 0x41f80000,
			 0x41d00000,

			 // vpu register f25
			 0x41c80000,
			 0x41700000,
			 0x41700000,
			 0x42000000,
			 0x41b00000,
			 0x41100000,
			 0x41400000,
			 0x41700000,

			 // vpu register f26
			 0x40e00000,
			 0x41d80000,
			 0x41800000,
			 0x41f00000,
			 0x41a00000,
			 0x41c00000,
			 0x40c00000,
			 0x41700000,

			 // vpu register f27
			 0x41900000,
			 0x40a00000,
			 0x41e80000,
			 0x42000000,
			 0x41980000,
			 0x41300000,
			 0x41980000,
			 0x41300000,

			 // vpu register f28
			 0x41500000,
			 0x41a00000,
			 0x41f00000,
			 0x40000000,
			 0x41600000,
			 0x41a80000,
			 0x41800000,
			 0x41300000,

			 // vpu register f29
			 0x41f00000,
			 0x41a00000,
			 0x41d00000,
			 0x41f80000,
			 0x42000000,
			 0x41e00000,
			 0x40400000,
			 0x41d80000,

			 // vpu register f30
			 0x41400000,
			 0x40800000,
			 0x41300000,
			 0x3f800000,
			 0x41500000,
			 0x41c80000,
			 0x41e00000,
			 0x41900000,

			 // vpu register f31
			 0x40a00000,
			 0x42000000,
			 0x40e00000,
			 0x41e80000,
			 0x41900000,
			 0x41c00000,
			 0x41d80000,
			 0x3f800000     // 1.0                   
	};

	C_TEST_START;

	__asm__ __volatile__ (
		"LBL_C_TEST_START:\n"
		"j LBL_SETUP_EV\n"
		".align 12\n"
		"M_MODE_E_H:\n"
		"csrr x5, 0x342\n"      // read mcause
		"li x6, 0xf\n"
		"and x5, x5, x6\n"
		"li x6, 0xb\n"
		"bne x5, x6, LBL_RET_TO_CLR\n"
		"LBL_SETUP_SCP:\n"
		"csrrs	x0, 0x7e0, 1\n"
		"fence\n"
		"csrrs	x0, 0x7e0, 3\n"
		"fence\n"
		"LBL_RET_TO_CLR:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
		"LBL_S_MODE_E_H:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
		"sret\n"
		".align 12\n"
		"LBL_SETUP_EV:\n"
		"la x5, M_MODE_E_H\n"
		"csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
		"la x5, LBL_S_MODE_E_H\n"
		"csrrw x0, 0x105, x5\n"
		"li x5, 0xffffffffffffffff\n"
		"mova.m.x x5\n"
		"mv x11, %[m_11]\n"
		"addi x11, x11, 2047\n"
		"addi x11, x11, 1\n"
		"mv x12, %[m_12]\n"
		"addi x12, x12, 2047\n"
		"addi x12, x12, 1\n"
		"mv x13, %[m_13]\n"
		"addi x13, x13, 2047\n"
		"addi x13, x13, 1\n"
		"mv x14, %[m_14]\n"
		"addi x14, x14, 2047\n"
		"addi x14, x14, 1\n"
		"mv x15, %[m_15]\n"
		"addi x15, x15, 2047\n"
		"addi x15, x15, 1\n"
		"mv x4, %[operands]\n"
		"flw.ps f0,  0 (x4)\n"
		"flw.ps f1,  32 (x4)\n"
		"flw.ps f2,  64 (x4)\n"
		"flw.ps f3,  96 (x4)\n"
		"flw.ps f4,  128 (x4)\n"
		"flw.ps f5,  160 (x4)\n"
		"flw.ps f6,  192 (x4)\n"
		"flw.ps f7,  224 (x4)\n"
		"flw.ps f8,  256 (x4)\n"
		"flw.ps f9,  288 (x4)\n"
		"flw.ps f10,  320 (x4)\n"
		"flw.ps f11,  352 (x4)\n"
		"flw.ps f12,  384 (x4)\n"
		"flw.ps f13,  416 (x4)\n"
		"flw.ps f14,  448 (x4)\n"
		"flw.ps f15,  480 (x4)\n"
		"flw.ps f16,  512 (x4)\n"
		"flw.ps f17,  544 (x4)\n"
		"flw.ps f18,  576 (x4)\n"
		"flw.ps f19,  608 (x4)\n"
		"flw.ps f20,  640 (x4)\n"
		"flw.ps f21,  672 (x4)\n"
		"flw.ps f22,  704 (x4)\n"
		"flw.ps f23,  736 (x4)\n"
		"flw.ps f24,  768 (x4)\n"
		"flw.ps f25,  800 (x4)\n"
		"flw.ps f26,  832 (x4)\n"
		"flw.ps f27,  864 (x4)\n"
		"flw.ps f28,  896 (x4)\n"
		"flw.ps f29,  928 (x4)\n"
		"flw.ps f30,  960 (x4)\n"
		"flw.ps f31,  992 (x4)\n"
		"fence\n"
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_VPU_SEQ_START:\n"
		VSNIP_RSV
	);
	RESET_FCSR;
	RESET_TENSOR_COOP;
	SET_TENSOR_MASK;
	FENCE;
	TENSOR_WAIT_COP;
	ECALL;
	FENCE;
	__asm__ __volatile__ (

///////////////////////////////////////tensor_load_i16_vpu
        "add x5, x11, zero\n"
        "li x31, 0x41\n"
        "li x6,  0x1000000000000000 \n"
        "add x5, x6, x5\n"    //   use_tmask [63:63] = 0x0,  use_coop [62:62] = 0x0,  dst_start [58:53] = 0x0,  offset [5:5] = 0x0,  num_lines [3:0] = 0xb, vaddr = %[m_11]
        "csrrw zero,  0x83f , x5\n"  //  tensor_load_i16_vpu
        "csrwi   0x830, 1\n"  // wait on tensor_load with appropriate id

		VSNIP_RSV
	);
	FENCE;
	mcache_control(0, 0, 0);
	FENCE;
	TENSOR_WAIT_COP;
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
