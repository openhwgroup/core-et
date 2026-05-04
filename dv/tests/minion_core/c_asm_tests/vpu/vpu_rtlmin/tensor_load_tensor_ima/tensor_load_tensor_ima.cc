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
			 0x745333c9, /// 0x0
			 0x2756473e, /// 0x4
			 0x9b1e85e8, /// 0x8
			 0x600fa981, /// 0xc
			 0x2c784e68, /// 0x10
			 0x61f7d99c, /// 0x14
			 0x39904ed1, /// 0x18
			 0xb23e27f8, /// 0x1c
			 0x65987105, /// 0x20
			 0x52e4dd23, /// 0x24
			 0xba0ffd77, /// 0x28
			 0x304e0c4c, /// 0x2c
			 0x4b24c092, /// 0x30
			 0x9fedba28, /// 0x34
			 0x316d997a, /// 0x38
			 0xb8627155, /// 0x3c
			 0x3e82ea5b, /// 0x40
			 0x92c642f0, /// 0x44
			 0x198ad128, /// 0x48
			 0x859badd7, /// 0x4c
			 0x5df0b469, /// 0x50
			 0x6a29882, /// 0x54
			 0x7246d642, /// 0x58
			 0x404aac7c, /// 0x5c
			 0x655af0b1, /// 0x60
			 0x534b7526, /// 0x64
			 0x72eac861, /// 0x68
			 0xfe1a2008, /// 0x6c
			 0xb772f574, /// 0x70
			 0x45847090, /// 0x74
			 0x9b6b78df, /// 0x78
			 0xacb070a9, /// 0x7c
			 0xfb31c362, /// 0x80
			 0x9ecb34b, /// 0x84
			 0x2a3f2fad, /// 0x88
			 0xf556f39d, /// 0x8c
			 0x7e38d9e3, /// 0x90
			 0x42366913, /// 0x94
			 0x291e720e, /// 0x98
			 0xc8111708, /// 0x9c
			 0x85d07e03, /// 0xa0
			 0xf82bda48, /// 0xa4
			 0x8c88a5dc, /// 0xa8
			 0xe113dbe, /// 0xac
			 0xf34308fa, /// 0xb0
			 0x2d22ddf4, /// 0xb4
			 0x7ec545db, /// 0xb8
			 0x3529ab67, /// 0xbc
			 0x2135c0ad, /// 0xc0
			 0xacb2d164, /// 0xc4
			 0xc252defe, /// 0xc8
			 0x42e7be60, /// 0xcc
			 0xafd40420, /// 0xd0
			 0x4ae1a492, /// 0xd4
			 0x66e2ee38, /// 0xd8
			 0xd828fe02, /// 0xdc
			 0xda9c2b40, /// 0xe0
			 0x991408b8, /// 0xe4
			 0xfc74970c, /// 0xe8
			 0x8157a96b, /// 0xec
			 0x388b89f4, /// 0xf0
			 0x257092b5, /// 0xf4
			 0xb1f9c236, /// 0xf8
			 0x4a9480d2, /// 0xfc
			 0x61e59a3d, /// 0x100
			 0x54db55ec, /// 0x104
			 0x54f3792e, /// 0x108
			 0x7f071a5, /// 0x10c
			 0xb6a21cae, /// 0x110
			 0x54ace39, /// 0x114
			 0x5371d8d5, /// 0x118
			 0x7e52444b, /// 0x11c
			 0xafcebb54, /// 0x120
			 0xa4c19744, /// 0x124
			 0x766f3bf, /// 0x128
			 0x90646997, /// 0x12c
			 0x3d575ec4, /// 0x130
			 0x2f26e8c8, /// 0x134
			 0xa307e7b4, /// 0x138
			 0xbd6a25f4, /// 0x13c
			 0x4f885b0, /// 0x140
			 0x9ec08284, /// 0x144
			 0xc794cbc9, /// 0x148
			 0x5a62f8e, /// 0x14c
			 0x1853d1b6, /// 0x150
			 0x5a6f17c6, /// 0x154
			 0x5baf4f75, /// 0x158
			 0x225aaac2, /// 0x15c
			 0xaedd4ccb, /// 0x160
			 0xedd7de5a, /// 0x164
			 0x2e558ad1, /// 0x168
			 0xce4173d, /// 0x16c
			 0xad093a8b, /// 0x170
			 0xe99338f9, /// 0x174
			 0x8e423c83, /// 0x178
			 0x52d763d3, /// 0x17c
			 0xee8ee1bc, /// 0x180
			 0xd2f06ef4, /// 0x184
			 0x2bc68fde, /// 0x188
			 0x19d0999c, /// 0x18c
			 0xab08fd3a, /// 0x190
			 0x9c376b9e, /// 0x194
			 0xbd6bf84a, /// 0x198
			 0xb8044c86, /// 0x19c
			 0x972f70c8, /// 0x1a0
			 0x1c9fdc93, /// 0x1a4
			 0x1b5c33f7, /// 0x1a8
			 0xa0ed6d88, /// 0x1ac
			 0x403d8a47, /// 0x1b0
			 0xb44aa68f, /// 0x1b4
			 0x37cba1f5, /// 0x1b8
			 0x1a96a467, /// 0x1bc
			 0xb81bd26b, /// 0x1c0
			 0xca87b930, /// 0x1c4
			 0x26c3f958, /// 0x1c8
			 0xd99bdbdf, /// 0x1cc
			 0x7dbbcdd6, /// 0x1d0
			 0xc379fdbb, /// 0x1d4
			 0x88d538d3, /// 0x1d8
			 0x404b4787, /// 0x1dc
			 0xb3598ade, /// 0x1e0
			 0xdeeac0bd, /// 0x1e4
			 0x4065c2b0, /// 0x1e8
			 0x98ccfbe6, /// 0x1ec
			 0x7bd9b240, /// 0x1f0
			 0xb2580986, /// 0x1f4
			 0x38effd13, /// 0x1f8
			 0xe40438ae, /// 0x1fc
			 0x5f01f079, /// 0x200
			 0x873ff6b9, /// 0x204
			 0x1c3b5768, /// 0x208
			 0x6a055718, /// 0x20c
			 0x5aaa4faa, /// 0x210
			 0x12f440, /// 0x214
			 0xa801ff43, /// 0x218
			 0x5d17517a, /// 0x21c
			 0xda249d01, /// 0x220
			 0xbd1e00bc, /// 0x224
			 0x8b8be10, /// 0x228
			 0x7ab5d3ea, /// 0x22c
			 0xb00a8a6e, /// 0x230
			 0x9134ffae, /// 0x234
			 0x4b219e65, /// 0x238
			 0xacaf64f0, /// 0x23c
			 0xb820b6b7, /// 0x240
			 0xd401e3dc, /// 0x244
			 0x4a5bb88a, /// 0x248
			 0x85639022, /// 0x24c
			 0x114d098c, /// 0x250
			 0xcc293ca3, /// 0x254
			 0x852b052a, /// 0x258
			 0xb9805db3, /// 0x25c
			 0x1cf713f6, /// 0x260
			 0x937ad08f, /// 0x264
			 0xadc1a84e, /// 0x268
			 0x96f8db7c, /// 0x26c
			 0x566a6270, /// 0x270
			 0xf61c620a, /// 0x274
			 0xb74ed0dd, /// 0x278
			 0x5ed9e9f9, /// 0x27c
			 0xa2486905, /// 0x280
			 0x58182e7a, /// 0x284
			 0x5dc981a6, /// 0x288
			 0xc21dacc0, /// 0x28c
			 0x4818db7, /// 0x290
			 0xada9b6ae, /// 0x294
			 0x860a3d7a, /// 0x298
			 0xad4a41af, /// 0x29c
			 0x459aede3, /// 0x2a0
			 0x43a0cc57, /// 0x2a4
			 0xe70d79d6, /// 0x2a8
			 0x909cf4fb, /// 0x2ac
			 0xb6612b63, /// 0x2b0
			 0x72c6d816, /// 0x2b4
			 0x4eec57a1, /// 0x2b8
			 0x15abe516, /// 0x2bc
			 0xd706d040, /// 0x2c0
			 0xf4325682, /// 0x2c4
			 0xdb080fb, /// 0x2c8
			 0x86408a52, /// 0x2cc
			 0x45172f24, /// 0x2d0
			 0xb35570e3, /// 0x2d4
			 0xdc3a89cb, /// 0x2d8
			 0x6a2d2418, /// 0x2dc
			 0x248fcd6c, /// 0x2e0
			 0xb4e9e374, /// 0x2e4
			 0x2099ea1a, /// 0x2e8
			 0xe0acaf62, /// 0x2ec
			 0xe0b99998, /// 0x2f0
			 0x124bcdcf, /// 0x2f4
			 0xba7b5334, /// 0x2f8
			 0xb3fb4556, /// 0x2fc
			 0x188c6d2, /// 0x300
			 0x55fbf7a, /// 0x304
			 0x15ef48b8, /// 0x308
			 0xc6341674, /// 0x30c
			 0xd23e9068, /// 0x310
			 0xfcd1da03, /// 0x314
			 0xbcfc534f, /// 0x318
			 0xe2ee8674, /// 0x31c
			 0x663533b5, /// 0x320
			 0x422a5479, /// 0x324
			 0x6e1201fd, /// 0x328
			 0x1b72f698, /// 0x32c
			 0xcf05d998, /// 0x330
			 0xf2e23db8, /// 0x334
			 0x390eec56, /// 0x338
			 0x6cf62061, /// 0x33c
			 0x644f1796, /// 0x340
			 0xefe6a4de, /// 0x344
			 0x7bb17a72, /// 0x348
			 0x23607ec2, /// 0x34c
			 0xf4302d93, /// 0x350
			 0x20021a41, /// 0x354
			 0x23319b4c, /// 0x358
			 0xf46a75c5, /// 0x35c
			 0x476042a3, /// 0x360
			 0xd036a061, /// 0x364
			 0x5e8c474f, /// 0x368
			 0x823ddbca, /// 0x36c
			 0xfbf88d44, /// 0x370
			 0x9b79f07c, /// 0x374
			 0xb80fb1b2, /// 0x378
			 0x4f392648, /// 0x37c
			 0x5a0e8cf3, /// 0x380
			 0x8e36b35b, /// 0x384
			 0xab012a9a, /// 0x388
			 0xbd12ce80, /// 0x38c
			 0x3408100a, /// 0x390
			 0x74357653, /// 0x394
			 0x73970cfc, /// 0x398
			 0xa58b9413, /// 0x39c
			 0x960d5ebd, /// 0x3a0
			 0x887dcd2e, /// 0x3a4
			 0x56ae1262, /// 0x3a8
			 0xc6ba1efb, /// 0x3ac
			 0xccab6504, /// 0x3b0
			 0x15e779ab, /// 0x3b4
			 0xd9c6f691, /// 0x3b8
			 0xced8598f, /// 0x3bc
			 0x3ed0bf6b, /// 0x3c0
			 0x246ffbb5, /// 0x3c4
			 0x861e426c, /// 0x3c8
			 0x5fbd54f1, /// 0x3cc
			 0xce52f9dd, /// 0x3d0
			 0xddbd6134, /// 0x3d4
			 0x5a97204d, /// 0x3d8
			 0xbfddee60, /// 0x3dc
			 0x6ca6c2ee, /// 0x3e0
			 0x616c3fea, /// 0x3e4
			 0x1b535350, /// 0x3e8
			 0xcf9248da, /// 0x3ec
			 0x1076e440, /// 0x3f0
			 0xb98f9775, /// 0x3f4
			 0x9c335751, /// 0x3f8
			 0x9c4ca7d, /// 0x3fc
			 0xc3c4ea42, /// 0x400
			 0x3549c04f, /// 0x404
			 0x69998954, /// 0x408
			 0xb87f12b, /// 0x40c
			 0x630369fb, /// 0x410
			 0x7eb7cf50, /// 0x414
			 0x6cd41379, /// 0x418
			 0x1f3ce76, /// 0x41c
			 0x205e3dc9, /// 0x420
			 0xd63b2a81, /// 0x424
			 0xdf413b4a, /// 0x428
			 0x948f96f9, /// 0x42c
			 0x91867348, /// 0x430
			 0x4e2b0932, /// 0x434
			 0x55baae5e, /// 0x438
			 0xa7ebec68, /// 0x43c
			 0x9d7780c1, /// 0x440
			 0x245d65d5, /// 0x444
			 0xdbd23167, /// 0x448
			 0x2919c546, /// 0x44c
			 0x88994b2c, /// 0x450
			 0x8512ec7b, /// 0x454
			 0x13080993, /// 0x458
			 0xdb652acd, /// 0x45c
			 0x7f357058, /// 0x460
			 0x2ecfba18, /// 0x464
			 0x10058740, /// 0x468
			 0x9556d8ae, /// 0x46c
			 0xfbf4b101, /// 0x470
			 0xd9f6dc0d, /// 0x474
			 0xd12eeb44, /// 0x478
			 0x128960bc, /// 0x47c
			 0xd34c4fee, /// 0x480
			 0x157ce181, /// 0x484
			 0x9fe282f3, /// 0x488
			 0x30534235, /// 0x48c
			 0x67d1c671, /// 0x490
			 0x60e3d324, /// 0x494
			 0xa89ef450, /// 0x498
			 0xf3ae6eca, /// 0x49c
			 0x10cc98c7, /// 0x4a0
			 0xec353a78, /// 0x4a4
			 0x8361a113, /// 0x4a8
			 0x6639ce11, /// 0x4ac
			 0xf374a13c, /// 0x4b0
			 0x6b018b7a, /// 0x4b4
			 0x373877a3, /// 0x4b8
			 0x621332ee, /// 0x4bc
			 0xd15d1fc5, /// 0x4c0
			 0x644aab9c, /// 0x4c4
			 0xa749a910, /// 0x4c8
			 0xd16cc2cd, /// 0x4cc
			 0xb9e71e23, /// 0x4d0
			 0x8d8cf213, /// 0x4d4
			 0xa107551b, /// 0x4d8
			 0x8ed6aaf6, /// 0x4dc
			 0x7646e239, /// 0x4e0
			 0xcc0db667, /// 0x4e4
			 0x1c81546b, /// 0x4e8
			 0x459714a5, /// 0x4ec
			 0x679473e8, /// 0x4f0
			 0xf8bd0bce, /// 0x4f4
			 0xe6ff1a78, /// 0x4f8
			 0x95c4449e, /// 0x4fc
			 0xf64d0e4e, /// 0x500
			 0x2f0ae217, /// 0x504
			 0x32cf4187, /// 0x508
			 0x5708bbb1, /// 0x50c
			 0x9b5e1f0f, /// 0x510
			 0x3151af67, /// 0x514
			 0x57121239, /// 0x518
			 0x947cbad5, /// 0x51c
			 0x19f11734, /// 0x520
			 0x5a58215f, /// 0x524
			 0xe21b2a4d, /// 0x528
			 0x7a84e287, /// 0x52c
			 0x51b0b335, /// 0x530
			 0x7492a0ba, /// 0x534
			 0x4af05ad5, /// 0x538
			 0xa84705b8, /// 0x53c
			 0x633dc482, /// 0x540
			 0xc596e53, /// 0x544
			 0xb990e748, /// 0x548
			 0x2f9e8383, /// 0x54c
			 0xde9f89a1, /// 0x550
			 0x62bb0713, /// 0x554
			 0xe55d53ae, /// 0x558
			 0xcade5c56, /// 0x55c
			 0xb95ae30e, /// 0x560
			 0x5a64ad7a, /// 0x564
			 0x4b397ee8, /// 0x568
			 0x5bd4bc82, /// 0x56c
			 0x9ed8648e, /// 0x570
			 0x1239e9b, /// 0x574
			 0x3030e310, /// 0x578
			 0xa7ac0aad, /// 0x57c
			 0xf3194af8, /// 0x580
			 0xf502c3ab, /// 0x584
			 0x64eecb06, /// 0x588
			 0x41b01bc1, /// 0x58c
			 0xcf9e3908, /// 0x590
			 0x4c8fe6cb, /// 0x594
			 0xb12bb38e, /// 0x598
			 0x9f6e1006, /// 0x59c
			 0x10615bbb, /// 0x5a0
			 0xff32ef99, /// 0x5a4
			 0x7126506d, /// 0x5a8
			 0x15885d8a, /// 0x5ac
			 0xaae81184, /// 0x5b0
			 0xabec3e, /// 0x5b4
			 0x9d0d6b67, /// 0x5b8
			 0x7a39442b, /// 0x5bc
			 0x98a7081d, /// 0x5c0
			 0xc0571f60, /// 0x5c4
			 0x4b3d1114, /// 0x5c8
			 0x7a4c9826, /// 0x5cc
			 0xd2b97432, /// 0x5d0
			 0x24d0488f, /// 0x5d4
			 0xc17d6a58, /// 0x5d8
			 0x9b813fc7, /// 0x5dc
			 0x4ae44e22, /// 0x5e0
			 0x523d8746, /// 0x5e4
			 0x50db9453, /// 0x5e8
			 0xc26b8996, /// 0x5ec
			 0xf528dbda, /// 0x5f0
			 0xccbea0f4, /// 0x5f4
			 0x72bfd002, /// 0x5f8
			 0x4d318a71, /// 0x5fc
			 0x60451485, /// 0x600
			 0x5300478e, /// 0x604
			 0x31a8b7e7, /// 0x608
			 0x77eb9148, /// 0x60c
			 0xc3932af5, /// 0x610
			 0x677c3fe0, /// 0x614
			 0x67e8cc3d, /// 0x618
			 0x8413dbb2, /// 0x61c
			 0x3df990a7, /// 0x620
			 0x8750063f, /// 0x624
			 0x95a7b0a0, /// 0x628
			 0xfdf90f74, /// 0x62c
			 0x49666627, /// 0x630
			 0x19ee6d51, /// 0x634
			 0x2772b6fc, /// 0x638
			 0x1b4f139b, /// 0x63c
			 0x6ef4a420, /// 0x640
			 0xc6636274, /// 0x644
			 0x5646908a, /// 0x648
			 0xbad548f4, /// 0x64c
			 0xf8e94485, /// 0x650
			 0x4b99fad8, /// 0x654
			 0xbaa51fe3, /// 0x658
			 0xecff0ef8, /// 0x65c
			 0x71527f5, /// 0x660
			 0x89d60652, /// 0x664
			 0x9b20bce0, /// 0x668
			 0x46454400, /// 0x66c
			 0x2f8d9041, /// 0x670
			 0x40a3ba66, /// 0x674
			 0x9e9a0e64, /// 0x678
			 0xa3ade949, /// 0x67c
			 0x5396ce3a, /// 0x680
			 0x6f16f47d, /// 0x684
			 0xc348008d, /// 0x688
			 0xc8bc6993, /// 0x68c
			 0x55da438a, /// 0x690
			 0x349bbca4, /// 0x694
			 0x4f4a8d8d, /// 0x698
			 0x19281bd7, /// 0x69c
			 0xa56b10c0, /// 0x6a0
			 0x802c2819, /// 0x6a4
			 0xe9d46532, /// 0x6a8
			 0xa4f98996, /// 0x6ac
			 0x48474e2b, /// 0x6b0
			 0xc6905771, /// 0x6b4
			 0x71f6bd39, /// 0x6b8
			 0xc6ef9041, /// 0x6bc
			 0xb21f612c, /// 0x6c0
			 0x4fd03128, /// 0x6c4
			 0x8d5eac69, /// 0x6c8
			 0x913e64e3, /// 0x6cc
			 0x9ee097e2, /// 0x6d0
			 0x7537e608, /// 0x6d4
			 0x2f0634e, /// 0x6d8
			 0xb358d39c, /// 0x6dc
			 0x7e5f47e8, /// 0x6e0
			 0xb9881c36, /// 0x6e4
			 0xe7ee4edc, /// 0x6e8
			 0x5177f390, /// 0x6ec
			 0xc82fecb1, /// 0x6f0
			 0xa5025ea7, /// 0x6f4
			 0xbe34a434, /// 0x6f8
			 0x3ec960e9, /// 0x6fc
			 0x9913a1f1, /// 0x700
			 0xc42dd91b, /// 0x704
			 0x2f243194, /// 0x708
			 0x8eb0a3ff, /// 0x70c
			 0x9fbac85, /// 0x710
			 0x535ddf07, /// 0x714
			 0xa5257a51, /// 0x718
			 0xe0369b11, /// 0x71c
			 0x134666fc, /// 0x720
			 0xb21e5bea, /// 0x724
			 0x88ff4642, /// 0x728
			 0xfb8a8b2d, /// 0x72c
			 0xe34b3c04, /// 0x730
			 0xa7b1a3b9, /// 0x734
			 0xd5a88592, /// 0x738
			 0x502763ca, /// 0x73c
			 0x653fbc62, /// 0x740
			 0x2a0f9d4a, /// 0x744
			 0x2cb34b4d, /// 0x748
			 0x51896cc2, /// 0x74c
			 0x749eaf9a, /// 0x750
			 0x28b69b48, /// 0x754
			 0xfbff481e, /// 0x758
			 0xc8b4c345, /// 0x75c
			 0x95504b0b, /// 0x760
			 0x48ad7ca7, /// 0x764
			 0x61f1c31a, /// 0x768
			 0xb63ecc2c, /// 0x76c
			 0x98647a72, /// 0x770
			 0x33a5827c, /// 0x774
			 0x5b7437ce, /// 0x778
			 0xfe8e15d3, /// 0x77c
			 0x18488120, /// 0x780
			 0x5f733c1, /// 0x784
			 0x519ba0b3, /// 0x788
			 0xc99d9ed4, /// 0x78c
			 0xb6173ce3, /// 0x790
			 0xbe85303, /// 0x794
			 0x9ada9a4a, /// 0x798
			 0x8856cdfb, /// 0x79c
			 0x72d73c6f, /// 0x7a0
			 0xae7d469e, /// 0x7a4
			 0xa28bcc49, /// 0x7a8
			 0x45a6c48a, /// 0x7ac
			 0xa772f2bd, /// 0x7b0
			 0x12edb6d6, /// 0x7b4
			 0x2a7c44f6, /// 0x7b8
			 0x9d1b97c5, /// 0x7bc
			 0xecd9cf8b, /// 0x7c0
			 0xd65cc1d3, /// 0x7c4
			 0xb6975793, /// 0x7c8
			 0x39311cd9, /// 0x7cc
			 0x7a3cb3bd, /// 0x7d0
			 0x69b02bd, /// 0x7d4
			 0xe106d31d, /// 0x7d8
			 0x5f3d0162, /// 0x7dc
			 0xa30158be, /// 0x7e0
			 0x62b33c29, /// 0x7e4
			 0xb6164ec0, /// 0x7e8
			 0x9d51a54a, /// 0x7ec
			 0x7bfda1ca, /// 0x7f0
			 0x2ff5ea5c, /// 0x7f4
			 0xb2ae216d, /// 0x7f8
			 0x9ccff67, /// 0x7fc
			 0x730d8274, /// 0x800
			 0x6316aefd, /// 0x804
			 0x1fd5eb94, /// 0x808
			 0xb0ce8c59, /// 0x80c
			 0x1a9e91d6, /// 0x810
			 0x33f9380f, /// 0x814
			 0xde1d3163, /// 0x818
			 0xfa5e98e2, /// 0x81c
			 0xec91e1ed, /// 0x820
			 0x71b8d391, /// 0x824
			 0xf2277be7, /// 0x828
			 0x3b52df48, /// 0x82c
			 0x1e5b43c7, /// 0x830
			 0x3d1e09bd, /// 0x834
			 0x78048343, /// 0x838
			 0x7fd22bbf, /// 0x83c
			 0x8256b454, /// 0x840
			 0x7a678415, /// 0x844
			 0x1da64da8, /// 0x848
			 0x896d91d5, /// 0x84c
			 0x5c48c1f6, /// 0x850
			 0x337d955, /// 0x854
			 0x375cdc30, /// 0x858
			 0xb2ae842, /// 0x85c
			 0xab81490c, /// 0x860
			 0x1167dff0, /// 0x864
			 0x780b508c, /// 0x868
			 0x27761c7, /// 0x86c
			 0x9126b950, /// 0x870
			 0xd7007f4d, /// 0x874
			 0x927b7bcb, /// 0x878
			 0xa7cbd006, /// 0x87c
			 0x68c0a40f, /// 0x880
			 0xf034d8ea, /// 0x884
			 0x686d8c30, /// 0x888
			 0x354890fc, /// 0x88c
			 0x67079db7, /// 0x890
			 0x7e00970d, /// 0x894
			 0x6075cae6, /// 0x898
			 0xf04530ba, /// 0x89c
			 0x22c9ab99, /// 0x8a0
			 0xb3cb80bf, /// 0x8a4
			 0x88335bea, /// 0x8a8
			 0xa66faab2, /// 0x8ac
			 0xf58bad29, /// 0x8b0
			 0xa76173a1, /// 0x8b4
			 0xc6f7f205, /// 0x8b8
			 0x5733050e, /// 0x8bc
			 0xbbea09ec, /// 0x8c0
			 0x3f849764, /// 0x8c4
			 0x59d6ea7b, /// 0x8c8
			 0x9aec6690, /// 0x8cc
			 0x74aa1cd0, /// 0x8d0
			 0xc2a33a5f, /// 0x8d4
			 0x81c35298, /// 0x8d8
			 0x5a4a786c, /// 0x8dc
			 0x14584fd8, /// 0x8e0
			 0x76cbce3a, /// 0x8e4
			 0x5a9d9499, /// 0x8e8
			 0x5c0fd2a9, /// 0x8ec
			 0x5fbef92e, /// 0x8f0
			 0xdbbfefe, /// 0x8f4
			 0x9d98e36b, /// 0x8f8
			 0xd783d478, /// 0x8fc
			 0x1f593b07, /// 0x900
			 0xaa852529, /// 0x904
			 0x6d44bebc, /// 0x908
			 0x93b0a328, /// 0x90c
			 0x5c875f73, /// 0x910
			 0x414c0620, /// 0x914
			 0x6d42dd8d, /// 0x918
			 0xca5553d9, /// 0x91c
			 0xe161ab6c, /// 0x920
			 0x30235cbb, /// 0x924
			 0x3468e638, /// 0x928
			 0x4d097871, /// 0x92c
			 0x577d0c86, /// 0x930
			 0xd7c66f73, /// 0x934
			 0x865b2201, /// 0x938
			 0xbb60ec72, /// 0x93c
			 0x83a94fd5, /// 0x940
			 0xa7c71a6d, /// 0x944
			 0x5af0a264, /// 0x948
			 0x597b1dee, /// 0x94c
			 0x294200f3, /// 0x950
			 0x5ca5a5da, /// 0x954
			 0xca0ab55f, /// 0x958
			 0xeb6a8c0c, /// 0x95c
			 0x9e95c2a5, /// 0x960
			 0xbd9878e, /// 0x964
			 0xbca2df1e, /// 0x968
			 0xd7ec3992, /// 0x96c
			 0x8d0293e7, /// 0x970
			 0x786c0725, /// 0x974
			 0x7567cb1b, /// 0x978
			 0x5d394be2, /// 0x97c
			 0xc20f3bbd, /// 0x980
			 0x5d8d399c, /// 0x984
			 0x1032ea80, /// 0x988
			 0x594ded57, /// 0x98c
			 0x626d81c3, /// 0x990
			 0xd3d8f8f1, /// 0x994
			 0x57955800, /// 0x998
			 0xf4fab5c, /// 0x99c
			 0xfb4e4c, /// 0x9a0
			 0x85205401, /// 0x9a4
			 0x55b3fc92, /// 0x9a8
			 0x8a30a171, /// 0x9ac
			 0xff6b74d9, /// 0x9b0
			 0x38597265, /// 0x9b4
			 0xf2da64fe, /// 0x9b8
			 0x55e04cce, /// 0x9bc
			 0x73aae06f, /// 0x9c0
			 0x49593a92, /// 0x9c4
			 0xdfb7112, /// 0x9c8
			 0xadd5552d, /// 0x9cc
			 0x404c8e5b, /// 0x9d0
			 0x258648e3, /// 0x9d4
			 0x9460d64e, /// 0x9d8
			 0xb4b16eff, /// 0x9dc
			 0x957c996f, /// 0x9e0
			 0x9a9116d6, /// 0x9e4
			 0x8ba6a892, /// 0x9e8
			 0x7027b331, /// 0x9ec
			 0xc9376fcb, /// 0x9f0
			 0x466b7ff1, /// 0x9f4
			 0xc76cb729, /// 0x9f8
			 0x12a360cd, /// 0x9fc
			 0x11cdf415, /// 0xa00
			 0xf910a694, /// 0xa04
			 0xe21966f5, /// 0xa08
			 0x5b3d6861, /// 0xa0c
			 0xedd00254, /// 0xa10
			 0xf793038b, /// 0xa14
			 0xadc45a3d, /// 0xa18
			 0xae20915d, /// 0xa1c
			 0x51b4e33b, /// 0xa20
			 0xa7d4be0e, /// 0xa24
			 0xcd97e9ec, /// 0xa28
			 0x7a03c9c9, /// 0xa2c
			 0x9dc51657, /// 0xa30
			 0x989b4208, /// 0xa34
			 0xbfb24c93, /// 0xa38
			 0x8596106b, /// 0xa3c
			 0x4faaa97b, /// 0xa40
			 0x4d2125dd, /// 0xa44
			 0x1fbcf5e5, /// 0xa48
			 0xd9ba0864, /// 0xa4c
			 0xfdaa57f, /// 0xa50
			 0x2aa3fba6, /// 0xa54
			 0xf6b927c, /// 0xa58
			 0x3fc80189, /// 0xa5c
			 0x63d471d1, /// 0xa60
			 0x10a1f373, /// 0xa64
			 0x12bc115a, /// 0xa68
			 0x19c5946b, /// 0xa6c
			 0x8cccab6, /// 0xa70
			 0x2c3daa89, /// 0xa74
			 0x166b9474, /// 0xa78
			 0xec09c103, /// 0xa7c
			 0x1ad0958b, /// 0xa80
			 0x5a90d205, /// 0xa84
			 0x9d4cf45e, /// 0xa88
			 0x418d2c54, /// 0xa8c
			 0x630ceed3, /// 0xa90
			 0x18d5414e, /// 0xa94
			 0xb85900d3, /// 0xa98
			 0x355edab4, /// 0xa9c
			 0xcc6daa50, /// 0xaa0
			 0x7334ceb3, /// 0xaa4
			 0x1b6a1417, /// 0xaa8
			 0xde9bed5f, /// 0xaac
			 0xa48c7428, /// 0xab0
			 0x50260b22, /// 0xab4
			 0xc3f2c10c, /// 0xab8
			 0x26e30378, /// 0xabc
			 0x34df9c1f, /// 0xac0
			 0xa9c4ae2, /// 0xac4
			 0xdca863ff, /// 0xac8
			 0x79ef2a53, /// 0xacc
			 0xc6160a22, /// 0xad0
			 0x505978c3, /// 0xad4
			 0x966e5da6, /// 0xad8
			 0xc65abce1, /// 0xadc
			 0xf8aba0d4, /// 0xae0
			 0x6a526355, /// 0xae4
			 0x13ffa974, /// 0xae8
			 0xffcc2cb1, /// 0xaec
			 0xaf751e77, /// 0xaf0
			 0xcacacf52, /// 0xaf4
			 0x4e061f2d, /// 0xaf8
			 0xf5f3131a, /// 0xafc
			 0xd8a9d509, /// 0xb00
			 0x3e7cb2a8, /// 0xb04
			 0x9ff6a568, /// 0xb08
			 0x2c063424, /// 0xb0c
			 0xe29f146, /// 0xb10
			 0xddc77a9c, /// 0xb14
			 0x77f67919, /// 0xb18
			 0xef3e6705, /// 0xb1c
			 0x5134c838, /// 0xb20
			 0x3b5f6050, /// 0xb24
			 0x4b98b2c5, /// 0xb28
			 0xbe688cd2, /// 0xb2c
			 0x123d1455, /// 0xb30
			 0x15e5a932, /// 0xb34
			 0x33e6378f, /// 0xb38
			 0xbc6d73bd, /// 0xb3c
			 0x71a7592e, /// 0xb40
			 0x23241b3f, /// 0xb44
			 0x67257771, /// 0xb48
			 0x97d53e99, /// 0xb4c
			 0xbb8807c9, /// 0xb50
			 0xcb5e816c, /// 0xb54
			 0xc89fbcc5, /// 0xb58
			 0x2c257a3f, /// 0xb5c
			 0x5a21cc68, /// 0xb60
			 0x4931cc68, /// 0xb64
			 0xddf7f678, /// 0xb68
			 0x5e14020f, /// 0xb6c
			 0xc02a7529, /// 0xb70
			 0xb59c0ff0, /// 0xb74
			 0x8a469ebd, /// 0xb78
			 0xea3384ae, /// 0xb7c
			 0xf4aaa766, /// 0xb80
			 0xf10756a0, /// 0xb84
			 0xbb2c364c, /// 0xb88
			 0xc359b31, /// 0xb8c
			 0x84e473bc, /// 0xb90
			 0xa2a64e2a, /// 0xb94
			 0x633b7cc8, /// 0xb98
			 0x13a1742a, /// 0xb9c
			 0x7af7b8a8, /// 0xba0
			 0x951d2034, /// 0xba4
			 0xc411b6de, /// 0xba8
			 0x677546fd, /// 0xbac
			 0xe0ec4bbe, /// 0xbb0
			 0xf62467a2, /// 0xbb4
			 0x3342961, /// 0xbb8
			 0x9e64df20, /// 0xbbc
			 0x4ecdd3f3, /// 0xbc0
			 0x60054191, /// 0xbc4
			 0x7c191053, /// 0xbc8
			 0x71147717, /// 0xbcc
			 0x459d599d, /// 0xbd0
			 0x521da938, /// 0xbd4
			 0x25fcf277, /// 0xbd8
			 0xe2588f2a, /// 0xbdc
			 0x4c929843, /// 0xbe0
			 0xb04cc905, /// 0xbe4
			 0xacea4d58, /// 0xbe8
			 0xbc709ab5, /// 0xbec
			 0x8e7dcec9, /// 0xbf0
			 0xaf633e1f, /// 0xbf4
			 0x4081db36, /// 0xbf8
			 0x9bad5c55, /// 0xbfc
			 0x90a41a53, /// 0xc00
			 0xeeaaae8e, /// 0xc04
			 0xc6b4c329, /// 0xc08
			 0x271f2166, /// 0xc0c
			 0x5dee3379, /// 0xc10
			 0x1018dec1, /// 0xc14
			 0x852a4d53, /// 0xc18
			 0xea9a2d6a, /// 0xc1c
			 0xc51f2a4d, /// 0xc20
			 0x719f8460, /// 0xc24
			 0x7a788f65, /// 0xc28
			 0xc3cdecab, /// 0xc2c
			 0xbb7e3650, /// 0xc30
			 0xd721957d, /// 0xc34
			 0xed2cc7ab, /// 0xc38
			 0x553de28a, /// 0xc3c
			 0x1e1d62f7, /// 0xc40
			 0xf93f761b, /// 0xc44
			 0x2c68f0ca, /// 0xc48
			 0x116b7a53, /// 0xc4c
			 0xbe725fd6, /// 0xc50
			 0xe8d9ed07, /// 0xc54
			 0xd6a0f927, /// 0xc58
			 0x962ac8fa, /// 0xc5c
			 0x8861ef52, /// 0xc60
			 0x919218de, /// 0xc64
			 0x19b763cf, /// 0xc68
			 0xeb3c5295, /// 0xc6c
			 0x5d17330b, /// 0xc70
			 0x3049866a, /// 0xc74
			 0x47fd0fe5, /// 0xc78
			 0x35c1b032, /// 0xc7c
			 0x71f387fa, /// 0xc80
			 0xd1d632da, /// 0xc84
			 0xe2370d73, /// 0xc88
			 0x292fbd9c, /// 0xc8c
			 0x8e80aab4, /// 0xc90
			 0x15e1c920, /// 0xc94
			 0xee7d768f, /// 0xc98
			 0xb289c3b1, /// 0xc9c
			 0x934fba78, /// 0xca0
			 0x37750c11, /// 0xca4
			 0x81cfa112, /// 0xca8
			 0x479ea5f3, /// 0xcac
			 0xcef02bab, /// 0xcb0
			 0x3dd755da, /// 0xcb4
			 0x348a92ff, /// 0xcb8
			 0xad5a57fd, /// 0xcbc
			 0xead6f104, /// 0xcc0
			 0x59cb4b6d, /// 0xcc4
			 0x2bb04daf, /// 0xcc8
			 0x7e0227bb, /// 0xccc
			 0x33b998dc, /// 0xcd0
			 0x4998be3f, /// 0xcd4
			 0xcd946d87, /// 0xcd8
			 0x1d2beff8, /// 0xcdc
			 0x16b3d789, /// 0xce0
			 0x3661d43d, /// 0xce4
			 0x42a25df8, /// 0xce8
			 0x12569a0a, /// 0xcec
			 0xe89c9e85, /// 0xcf0
			 0x1360c956, /// 0xcf4
			 0x9d8edcde, /// 0xcf8
			 0xb30663fa, /// 0xcfc
			 0x36701296, /// 0xd00
			 0x5c420bd2, /// 0xd04
			 0xb24d7904, /// 0xd08
			 0x339f4f63, /// 0xd0c
			 0x70d88aaf, /// 0xd10
			 0xcf973313, /// 0xd14
			 0x2e6d6f55, /// 0xd18
			 0x1ed03f3, /// 0xd1c
			 0x79d84e34, /// 0xd20
			 0xe428b1d4, /// 0xd24
			 0x5229c8ff, /// 0xd28
			 0x1989ef59, /// 0xd2c
			 0x282f4e38, /// 0xd30
			 0xd56278b3, /// 0xd34
			 0x2a1d8ff1, /// 0xd38
			 0x2868d426, /// 0xd3c
			 0x6318130c, /// 0xd40
			 0x2716fb9c, /// 0xd44
			 0x5ba027c, /// 0xd48
			 0xaae253f9, /// 0xd4c
			 0xf8d8fe36, /// 0xd50
			 0x9c720366, /// 0xd54
			 0xd10841cb, /// 0xd58
			 0x8e21d6e7, /// 0xd5c
			 0xb3bdd258, /// 0xd60
			 0xee6ab6ed, /// 0xd64
			 0x8e957e53, /// 0xd68
			 0xabbbce10, /// 0xd6c
			 0xc12097a, /// 0xd70
			 0x7e96a70, /// 0xd74
			 0x28534bca, /// 0xd78
			 0x69ab09a4, /// 0xd7c
			 0xa0b95de2, /// 0xd80
			 0x7d0ea5d3, /// 0xd84
			 0x6b352a06, /// 0xd88
			 0xe77ed724, /// 0xd8c
			 0xa0cbb367, /// 0xd90
			 0x334275b4, /// 0xd94
			 0xf01e9fb4, /// 0xd98
			 0x7022988b, /// 0xd9c
			 0xec0c0b87, /// 0xda0
			 0xc243d2b9, /// 0xda4
			 0x3e7190b0, /// 0xda8
			 0x7202369b, /// 0xdac
			 0xcf09ec0e, /// 0xdb0
			 0xe92cdd58, /// 0xdb4
			 0x8605d0ef, /// 0xdb8
			 0x518c4e0, /// 0xdbc
			 0x77ec689, /// 0xdc0
			 0x2985bbcd, /// 0xdc4
			 0x5d61addc, /// 0xdc8
			 0x72d78e92, /// 0xdcc
			 0x709d9538, /// 0xdd0
			 0x6167ed1b, /// 0xdd4
			 0xebb0d86, /// 0xdd8
			 0x55f07c12, /// 0xddc
			 0x235eef7c, /// 0xde0
			 0x1b502b4, /// 0xde4
			 0xa7da7a4d, /// 0xde8
			 0xecf3287c, /// 0xdec
			 0x3505f74f, /// 0xdf0
			 0xbec3e3df, /// 0xdf4
			 0x54e5ed2a, /// 0xdf8
			 0x56489ed9, /// 0xdfc
			 0xe51a2759, /// 0xe00
			 0x2a2bf8e7, /// 0xe04
			 0x2e1ecd84, /// 0xe08
			 0x573c762c, /// 0xe0c
			 0xbf780bc6, /// 0xe10
			 0x58865eaf, /// 0xe14
			 0x35285492, /// 0xe18
			 0x7d908135, /// 0xe1c
			 0xd949435f, /// 0xe20
			 0xf093abab, /// 0xe24
			 0xd79f2be7, /// 0xe28
			 0xd0822c25, /// 0xe2c
			 0x1f49e743, /// 0xe30
			 0xe0ded10b, /// 0xe34
			 0xd10d48ba, /// 0xe38
			 0xdc675bb7, /// 0xe3c
			 0x53cd96b3, /// 0xe40
			 0xe7f0ce61, /// 0xe44
			 0x8fa0907b, /// 0xe48
			 0xddf1ead0, /// 0xe4c
			 0x7e89ca8e, /// 0xe50
			 0xd3371025, /// 0xe54
			 0xa9843d6b, /// 0xe58
			 0xbe44938e, /// 0xe5c
			 0x28b442ab, /// 0xe60
			 0x6b10d08f, /// 0xe64
			 0x649f567e, /// 0xe68
			 0x972cfd17, /// 0xe6c
			 0x97260b58, /// 0xe70
			 0x6b0cefdd, /// 0xe74
			 0x38202d06, /// 0xe78
			 0x9f145597, /// 0xe7c
			 0xae96ab1e, /// 0xe80
			 0x8a767b38, /// 0xe84
			 0x71ec1a89, /// 0xe88
			 0x97667c6a, /// 0xe8c
			 0x4df970c9, /// 0xe90
			 0x6075c20d, /// 0xe94
			 0xa9321c7f, /// 0xe98
			 0xa88b5ae8, /// 0xe9c
			 0xec313e10, /// 0xea0
			 0x69d0cfbf, /// 0xea4
			 0x1942191a, /// 0xea8
			 0x4866eba6, /// 0xeac
			 0xef3b6ce5, /// 0xeb0
			 0xca485ff3, /// 0xeb4
			 0x5e5ca579, /// 0xeb8
			 0x2664b789, /// 0xebc
			 0x36c927f0, /// 0xec0
			 0xb253dfe1, /// 0xec4
			 0xc0420af6, /// 0xec8
			 0x4b3a367b, /// 0xecc
			 0x3cb86f7, /// 0xed0
			 0xb44d440f, /// 0xed4
			 0x92e40f0f, /// 0xed8
			 0xdee1e236, /// 0xedc
			 0x43db28ef, /// 0xee0
			 0x4c2988ee, /// 0xee4
			 0x3c4013b5, /// 0xee8
			 0x81a68aae, /// 0xeec
			 0x63a373e3, /// 0xef0
			 0x81d5a23d, /// 0xef4
			 0x5eb19a76, /// 0xef8
			 0x16acf4b5, /// 0xefc
			 0x45c4e5f1, /// 0xf00
			 0x7922a1bc, /// 0xf04
			 0x17c34d79, /// 0xf08
			 0x876e795f, /// 0xf0c
			 0x89b39f4b, /// 0xf10
			 0xd06be409, /// 0xf14
			 0x27f54dde, /// 0xf18
			 0x7ebae011, /// 0xf1c
			 0xdce77404, /// 0xf20
			 0xca6426c6, /// 0xf24
			 0xa5dbc855, /// 0xf28
			 0x8c3c203d, /// 0xf2c
			 0x5f735f64, /// 0xf30
			 0x3501e34b, /// 0xf34
			 0x14de5756, /// 0xf38
			 0xe978563c, /// 0xf3c
			 0xd8b3a6ca, /// 0xf40
			 0x92b9274d, /// 0xf44
			 0xfa43f09c, /// 0xf48
			 0x197218e4, /// 0xf4c
			 0xb9c691f3, /// 0xf50
			 0xe3eb56fe, /// 0xf54
			 0x6f70d42b, /// 0xf58
			 0x66d0d76b, /// 0xf5c
			 0x3a8c42d4, /// 0xf60
			 0x41db5740, /// 0xf64
			 0xb6879353, /// 0xf68
			 0x628ee8c5, /// 0xf6c
			 0x55ced73f, /// 0xf70
			 0x4d3c1741, /// 0xf74
			 0xa9c660a7, /// 0xf78
			 0x53a18ed5, /// 0xf7c
			 0x8d149fc2, /// 0xf80
			 0x555cc4fe, /// 0xf84
			 0x7bf28ad1, /// 0xf88
			 0x9c6ed54a, /// 0xf8c
			 0x377470bb, /// 0xf90
			 0x77cf5c8a, /// 0xf94
			 0x2fc2c013, /// 0xf98
			 0x66205f9e, /// 0xf9c
			 0xccff898, /// 0xfa0
			 0xf77f50cf, /// 0xfa4
			 0x9231b5f7, /// 0xfa8
			 0x1bead79c, /// 0xfac
			 0xb75a532f, /// 0xfb0
			 0xa8ef1350, /// 0xfb4
			 0x363f3c5b, /// 0xfb8
			 0x6841d4d6, /// 0xfbc
			 0xaa827090, /// 0xfc0
			 0x1a60d5d5, /// 0xfc4
			 0xe7ab34ef, /// 0xfc8
			 0x66acd482, /// 0xfcc
			 0x4b24d48f, /// 0xfd0
			 0xa32e4341, /// 0xfd4
			 0x65fb766, /// 0xfd8
			 0x5c918171, /// 0xfdc
			 0x499f65a6, /// 0xfe0
			 0xb6691ebf, /// 0xfe4
			 0xd278ee64, /// 0xfe8
			 0x1bb8f94c, /// 0xfec
			 0xe400420a, /// 0xff0
			 0x8ca68453, /// 0xff4
			 0x385feac2, /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x10
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x28
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x40
			 0x80000000,                                                  // -zero                                       /// 0x44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x50
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x54
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x58
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x5c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x64
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x6c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x8c
			 0x4b000000,                                                  // 8388608.0                                   /// 0x90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x98
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xa0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xa8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xb0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xb4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xc8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xd0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xd8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xdc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xe0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xe4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xe8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xf0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xf8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xfc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x100
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x104
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x108
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x10c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x110
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x114
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x118
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x11c
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x120
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x124
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x128
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x12c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x130
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x134
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x138
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x13c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x140
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x144
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x148
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x14c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x150
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x154
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x158
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x15c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x160
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x164
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x168
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x16c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x170
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x174
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x178
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x17c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x180
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x184
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x188
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x18c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x190
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x194
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x198
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x19c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x1a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x1a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x1ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x1b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x1b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x1b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x1bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x1c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x1c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x1c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x1cc
			 0xbf028f5c,                                                  // -0.51                                       /// 0x1d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x1d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x1d8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x1dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x1e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x1e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x1ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x1f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x1fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x200
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x204
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x208
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x20c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x210
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x214
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x218
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x21c
			 0xbf028f5c,                                                  // -0.51                                       /// 0x220
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x224
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x228
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x22c
			 0x7fc00001,                                                  // signaling NaN                               /// 0x230
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x234
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x238
			 0x33333333,                                                  // 4 random values                             /// 0x23c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x240
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x244
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x248
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x24c
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x250
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x254
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x258
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x25c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x260
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x264
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x268
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x26c
			 0x4b000000,                                                  // 8388608.0                                   /// 0x270
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x274
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x278
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x27c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x280
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x284
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x288
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x28c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x290
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x294
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x298
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x29c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x2a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x2a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x2ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2b0
			 0x00000000,                                                  // zero                                        /// 0x2b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x2c0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x2c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x2c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x2d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2d4
			 0x33333333,                                                  // 4 random values                             /// 0x2d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x2dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x2e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x2ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x2f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x2f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x2f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x2fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x300
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x304
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x308
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x30c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x310
			 0x4b000000,                                                  // 8388608.0                                   /// 0x314
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x318
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x31c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x320
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x324
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x328
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x32c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x330
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x334
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x338
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x33c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x340
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x344
			 0x55555555,                                                  // 4 random values                             /// 0x348
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x34c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x350
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x354
			 0xffc00001,                                                  // -signaling NaN                              /// 0x358
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x35c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x360
			 0x80000000,                                                  // -zero                                       /// 0x364
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x368
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x36c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x370
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x374
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x378
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x37c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x380
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x384
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x388
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x38c
			 0xffc00001,                                                  // -signaling NaN                              /// 0x390
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x394
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x398
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x39c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3a0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x3a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x3a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x3ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x3b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x3b4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x3bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x3c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x3cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x3d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x3d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x3e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x3ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x3f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x3f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x3fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x400
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x404
			 0x80000000,                                                  // -zero                                       /// 0x408
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x40c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x410
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x414
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x418
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x41c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x420
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x424
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x428
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x42c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x430
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x434
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x438
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x43c
			 0x7fc00001,                                                  // signaling NaN                               /// 0x440
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x444
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x448
			 0x7f800000,                                                  // inf                                         /// 0x44c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x450
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x454
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x458
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x45c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x460
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x464
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x468
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x46c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x470
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x474
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x478
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x47c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x480
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x484
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x488
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x48c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x490
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x494
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x498
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x49c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x4a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x4a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x4ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x4b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x4b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x4bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x4c0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x4cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x4d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x4d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x4dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x4e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x4e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x4f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x4f4
			 0x33333333,                                                  // 4 random values                             /// 0x4f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x4fc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x500
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x504
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x508
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x50c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x510
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x514
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x518
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x51c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x520
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x524
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x528
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x52c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x530
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x534
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x538
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x53c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x540
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x544
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x548
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x54c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x550
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x554
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x558
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x55c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x560
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x564
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x568
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x56c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x570
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x574
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x578
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x57c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x580
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x584
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x588
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x58c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x590
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x594
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x598
			 0x3f028f5c,                                                  // 0.51                                        /// 0x59c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x5a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x5a4
			 0xffc00001,                                                  // -signaling NaN                              /// 0x5a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x5ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x5b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x5b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x5b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x5bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x5c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x5c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x5c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x5cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x5d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x5d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x5d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x5e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x5e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x5ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x5f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x600
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x604
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x608
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x60c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x610
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x614
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x618
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x61c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x620
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x624
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x628
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x62c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x630
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x634
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x638
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x63c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x640
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x644
			 0xffc00001,                                                  // -signaling NaN                              /// 0x648
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x64c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x650
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x654
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x658
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x65c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x660
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x664
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x668
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x66c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x670
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x674
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x678
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x67c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x680
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x684
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x688
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x68c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x690
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x694
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x698
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x69c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x6a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x6a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x6a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x6ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x6b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x6b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x6bc
			 0xcb000000,                                                  // -8388608.0                                  /// 0x6c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x6c4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x6c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x6cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x6d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x6d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x6d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x6dc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x6e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x6e4
			 0x3f028f5c,                                                  // 0.51                                        /// 0x6e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x6ec
			 0x3f028f5c,                                                  // 0.51                                        /// 0x6f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x6f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x6fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x700
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x704
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x708
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x70c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x710
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x714
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x718
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x71c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x720
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x724
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x728
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x72c
			 0x7f800000,                                                  // inf                                         /// 0x730
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x734
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x738
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x73c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x740
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x744
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x748
			 0x4b000000,                                                  // 8388608.0                                   /// 0x74c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x750
			 0x00000000,                                                  // zero                                        /// 0x754
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x758
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x75c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x760
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x764
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x768
			 0x3f028f5c,                                                  // 0.51                                        /// 0x76c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x770
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x774
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x778
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x77c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x780
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x784
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x788
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x78c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x790
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x794
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x798
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x79c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x7a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x7a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x7a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x7b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x7b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x7b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x7c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x7c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x7c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x7d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x7d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x7dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x7e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x7e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x7ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x7f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x7f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x7f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x7fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x800
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x804
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x808
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x80c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x810
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x814
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x818
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x81c
			 0xffc00001,                                                  // -signaling NaN                              /// 0x820
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x824
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x828
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x82c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x830
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x834
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x838
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x83c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x840
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x844
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x848
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x84c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x850
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x854
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x858
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x85c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x860
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x864
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x868
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x86c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x870
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x874
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x878
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x87c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x880
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x884
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x888
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x88c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x890
			 0x00000000,                                                  // zero                                        /// 0x894
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x898
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x89c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x8a4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x8a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x8ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x8b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x8b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x8c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x8c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x8c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x8cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x8d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x8d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x8d8
			 0xcb000000,                                                  // -8388608.0                                  /// 0x8dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x8e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x8e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x8e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x8ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x8f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x8f4
			 0x4b000000,                                                  // 8388608.0                                   /// 0x8f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x8fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x900
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x904
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x908
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x90c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x910
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x914
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x918
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x91c
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x920
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x924
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x928
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x92c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x930
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x934
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x938
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x93c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x940
			 0x4b000000,                                                  // 8388608.0                                   /// 0x944
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x948
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x94c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x950
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x954
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x958
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x95c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x960
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x964
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x968
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x96c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x970
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x974
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x978
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x97c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x980
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x984
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x98c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x990
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x994
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x998
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x99c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x9a4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x9a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x9b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x9b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x9bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x9c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x9cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x9d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9d8
			 0x00000000,                                                  // zero                                        /// 0x9dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x9e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x9ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x9f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xa00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xa08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xa0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xa10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xa14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xa1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa20
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xa24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xa28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xa2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa30
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xa34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xa38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xa3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xa44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xa50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xa54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xa58
			 0xffc00001,                                                  // -signaling NaN                              /// 0xa5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xa60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xa64
			 0x55555555,                                                  // 4 random values                             /// 0xa68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xa70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa74
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xa78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xa7c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xa80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xa88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xa8c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xa90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xa94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xa98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xa9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xaa0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xaa4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xaa8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xaac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xab0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xab4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xab8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xabc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xac0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xac4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xac8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xacc
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xad0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xad4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xad8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xadc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xae0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xae4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xae8
			 0x4b000000,                                                  // 8388608.0                                   /// 0xaec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xaf0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xaf4
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xaf8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xafc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb00
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xb04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xb08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xb10
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xb14
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xb18
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xb1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xb20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xb24
			 0xffc00001,                                                  // -signaling NaN                              /// 0xb28
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb2c
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xb30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xb34
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xb38
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xb3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xb40
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xb44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xb48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xb4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xb60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xb64
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xb68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xb6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xb70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xb74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb78
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xb7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xb80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xb84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xb88
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xb98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xb9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xba0
			 0x00000000,                                                  // zero                                        /// 0xba4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xba8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xbac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xbb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbb4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xbb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xbbc
			 0x33333333,                                                  // 4 random values                             /// 0xbc0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xbc4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xbc8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbcc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xbd0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xbd4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xbd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xbdc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xbe0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbe4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xbe8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xbec
			 0xcb000000,                                                  // -8388608.0                                  /// 0xbf0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xbf4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xbf8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xbfc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xc00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xc04
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xc08
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xc0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xc14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xc18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc20
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xc24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xc28
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xc2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xc30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xc34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xc38
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xc3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xc40
			 0xffc00001,                                                  // -signaling NaN                              /// 0xc44
			 0xffc00001,                                                  // -signaling NaN                              /// 0xc48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xc50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xc54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xc58
			 0x7fc00001,                                                  // signaling NaN                               /// 0xc5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xc60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xc64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xc68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xc70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc74
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xc78
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xc7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xc80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xc84
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xc8c
			 0xff800000,                                                  // -inf                                        /// 0xc90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xc94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xc98
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xc9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xca0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xca4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xca8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xcac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xcb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xcb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xcb8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xcbc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xcc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xcc4
			 0xffc00001,                                                  // -signaling NaN                              /// 0xcc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xccc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xcd0
			 0x3f028f5c,                                                  // 0.51                                        /// 0xcd4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xcd8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xcdc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xce0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xce4
			 0xff800000,                                                  // -inf                                        /// 0xce8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcec
			 0x55555555,                                                  // 4 random values                             /// 0xcf0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xcf4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcf8
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xcfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xd00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xd04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xd08
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd0c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd10
			 0x3f028f5c,                                                  // 0.51                                        /// 0xd14
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xd18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xd1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xd20
			 0x7fc00001,                                                  // signaling NaN                               /// 0xd24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xd28
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xd2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xd30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xd3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xd44
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xd48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xd4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xd50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xd5c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xd64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xd68
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xd6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xd70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xd74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xd78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xd80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd84
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xd88
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xd94
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xd98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xda0
			 0x3f028f5c,                                                  // 0.51                                        /// 0xda4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xda8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xdac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xdb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xdb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xdbc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xdc4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xdc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xdcc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdd0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdd4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdd8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xddc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xde0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xde4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xde8
			 0x7f800000,                                                  // inf                                         /// 0xdec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdf0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xdf4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdf8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdfc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xe04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xe08
			 0x7f800000,                                                  // inf                                         /// 0xe0c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe10
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xe14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xe18
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xe1c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe28
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xe2c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0xe30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xe34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xe38
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xe3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xe40
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xe44
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xe48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xe4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xe54
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xe58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xe5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xe64
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe68
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xe6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xe7c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xe80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xe84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xe88
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe8c
			 0x33333333,                                                  // 4 random values                             /// 0xe90
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xe94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xe98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xe9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xea0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xea4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xea8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xeac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xeb0
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xeb4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xeb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xebc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xec0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xec4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xec8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xecc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xed0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xed4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xed8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xedc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xee0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xee4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xee8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xeec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xef0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xef4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xef8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xefc
			 0xbf028f5c,                                                  // -0.51                                       /// 0xf00
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xf04
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xf08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xf0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xf10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xf14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xf20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf24
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xf28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xf2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xf34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xf38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xf3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xf40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf44
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xf50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xf54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xf58
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xf5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xf60
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xf64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xf68
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xf6c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xf70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xf74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xf7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xf80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xf84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xf88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xf90
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf94
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf98
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xf9c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfa0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xfa4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xfa8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xfac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xfb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xfb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfb8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xfbc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xfc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xfc4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xfc8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xfcc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfd0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xfd4
			 0xcb000000,                                                  // -8388608.0                                  /// 0xfd8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xfdc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xfe0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfe4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xfe8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xfec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xff0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xff4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x0
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x8
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xc
			 0x4b000000,                                                  // 8388608.0                                   /// 0x10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x20
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x24
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x3c
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x6c
			 0x55555555,                                                  // 4 random values                             /// 0x70
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x80
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x84
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x88
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x90
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x98
			 0xff800000,                                                  // -inf                                        /// 0x9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xa8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xb8
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xc0
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xc4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xd0
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xd4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xd8
			 0x80000000,                                                  // -zero                                       /// 0xdc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xe4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xe8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xf0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xf4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xf8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x100
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x104
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x108
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x10c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x110
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x114
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x118
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x11c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x120
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x124
			 0x7fc00001,                                                  // signaling NaN                               /// 0x128
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x12c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x130
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x134
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x138
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x13c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x140
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x144
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x148
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x14c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x150
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x154
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x158
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x15c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x160
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x164
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x168
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x16c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x170
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x174
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x178
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x17c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x180
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x184
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x188
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x18c
			 0x4b000000,                                                  // 8388608.0                                   /// 0x190
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x194
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x198
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x19c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x1a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x1a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1ac
			 0xff800000,                                                  // -inf                                        /// 0x1b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x1bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x1c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x1c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x1c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x1cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x1d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x1d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x1d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x1dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x1e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x1e4
			 0xbf028f5c,                                                  // -0.51                                       /// 0x1e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x1ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x1f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x1f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x200
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x204
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x208
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x20c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x210
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x214
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x218
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x21c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x220
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x224
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x228
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x22c
			 0x3f028f5c,                                                  // 0.51                                        /// 0x230
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x234
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x238
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x23c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x240
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x244
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x248
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x24c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x250
			 0x55555555,                                                  // 4 random values                             /// 0x254
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x258
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x25c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x260
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x264
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x268
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x26c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x270
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x274
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x278
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x27c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x280
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x284
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x288
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x28c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x290
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x294
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x298
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x29c
			 0x00000000,                                                  // zero                                        /// 0x2a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x2a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x2ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x2b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x2b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2b8
			 0x3f028f5c,                                                  // 0.51                                        /// 0x2bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x2c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x2c8
			 0xbf028f5c,                                                  // -0.51                                       /// 0x2cc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x2d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x2d4
			 0x00000000,                                                  // zero                                        /// 0x2d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x2dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x2e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x2e4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x2ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x2f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2f4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x2f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x2fc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x300
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x304
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x308
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x30c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x310
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x314
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x318
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x31c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x320
			 0x00000000,                                                  // zero                                        /// 0x324
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x328
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x32c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x330
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x334
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x338
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x33c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x340
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x344
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x348
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x34c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x350
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x354
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x358
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x35c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x360
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x364
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x368
			 0x7fc00001,                                                  // signaling NaN                               /// 0x36c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x370
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x374
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x378
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x37c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x380
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x384
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x388
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x38c
			 0xbf028f5c,                                                  // -0.51                                       /// 0x390
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x394
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x398
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x39c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x3a0
			 0x4b000000,                                                  // 8388608.0                                   /// 0x3a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3a8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3b0
			 0x00000000,                                                  // zero                                        /// 0x3b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x3b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x3c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x3c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x3d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x3d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x3e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x3e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x3e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x3ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x3f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x400
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x404
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x408
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x40c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x410
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x414
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x418
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x41c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x420
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x424
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x428
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x42c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x430
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x434
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x438
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x43c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x440
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x444
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x448
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x44c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x450
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x454
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x458
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x45c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x460
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x464
			 0x4b000000,                                                  // 8388608.0                                   /// 0x468
			 0xffc00001,                                                  // -signaling NaN                              /// 0x46c
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x470
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x474
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x478
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x47c
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x480
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x484
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x488
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x48c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x490
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x494
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x498
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x49c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x4ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x4b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x4b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x4bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x4c0
			 0x4b000000,                                                  // 8388608.0                                   /// 0x4c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x4cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x4d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x4dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x4e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x4e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x4ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4f0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x4f4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x4f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x4fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x500
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x504
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x508
			 0x55555555,                                                  // 4 random values                             /// 0x50c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x510
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x514
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x518
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x51c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x520
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x524
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x528
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x52c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x530
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x534
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x538
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x53c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x540
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x544
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x548
			 0x7f800000,                                                  // inf                                         /// 0x54c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x550
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x554
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x558
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x55c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x560
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x564
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x568
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x56c
			 0x7fc00001,                                                  // signaling NaN                               /// 0x570
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x574
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x578
			 0xbf028f5c,                                                  // -0.51                                       /// 0x57c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x580
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x584
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x588
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x58c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x590
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x594
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x598
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x59c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x5a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x5a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x5a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5b0
			 0x7fc00001,                                                  // signaling NaN                               /// 0x5b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x5c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x5c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x5cc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x5d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x5dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x5e0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x5e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x5e8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x5ec
			 0xff800000,                                                  // -inf                                        /// 0x5f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x5f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x600
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x604
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x608
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x60c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x610
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x614
			 0x4b000000,                                                  // 8388608.0                                   /// 0x618
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x61c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x620
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x624
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x628
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x62c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x630
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x634
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x638
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x63c
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x640
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x644
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x648
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x64c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x650
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x654
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x658
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x65c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x660
			 0x7fc00001,                                                  // signaling NaN                               /// 0x664
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x668
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x66c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x670
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x674
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x678
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x67c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x680
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x684
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x688
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x68c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x690
			 0x7fc00001,                                                  // signaling NaN                               /// 0x694
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x698
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x69c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x6a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x6a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6ac
			 0x3f028f5c,                                                  // 0.51                                        /// 0x6b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x6b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x6b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x6bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x6c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x6c8
			 0x7f800000,                                                  // inf                                         /// 0x6cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x6d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x6dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x6e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x6e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x6e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6ec
			 0x7f800000,                                                  // inf                                         /// 0x6f0
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x6f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x6f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x700
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x704
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x708
			 0xcb000000,                                                  // -8388608.0                                  /// 0x70c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x710
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x714
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x718
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x71c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x720
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x724
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x728
			 0x3f028f5c,                                                  // 0.51                                        /// 0x72c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x730
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x734
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x738
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x73c
			 0x55555555,                                                  // 4 random values                             /// 0x740
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x744
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x748
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x74c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x750
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x754
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x758
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x75c
			 0xbf028f5c,                                                  // -0.51                                       /// 0x760
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x764
			 0x80000000,                                                  // -zero                                       /// 0x768
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x76c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x770
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x774
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x778
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x77c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x780
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x784
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x788
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x78c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x790
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x794
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x798
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x79c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x7a0
			 0x3f028f5c,                                                  // 0.51                                        /// 0x7a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x7a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x7ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x7b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x7b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x7c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x7c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x7c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7cc
			 0x80000000,                                                  // -zero                                       /// 0x7d0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x7d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x7dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x7e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x7e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x7e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x7f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x7f4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x7f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x7fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x800
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x804
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x808
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x80c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x810
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x814
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x818
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x81c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x820
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x824
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x828
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x82c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x830
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x834
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x838
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x83c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x840
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x844
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x848
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x84c
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x850
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x854
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x858
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x85c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x860
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x864
			 0xbf028f5c,                                                  // -0.51                                       /// 0x868
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x86c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x870
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x874
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x878
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x87c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x880
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x884
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x888
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x88c
			 0x7f800000,                                                  // inf                                         /// 0x890
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x894
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x898
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x89c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x8a0
			 0x33333333,                                                  // 4 random values                             /// 0x8a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x8ac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x8b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x8b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x8bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x8c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x8d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x8d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x8d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x8e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x8ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x8f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x8f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x8f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x900
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x904
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x908
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x90c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x910
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x914
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x918
			 0x80000000,                                                  // -zero                                       /// 0x91c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x920
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x924
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x928
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x92c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x930
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x934
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x938
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x93c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x940
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x944
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x948
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x94c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x950
			 0x3f028f5c,                                                  // 0.51                                        /// 0x954
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x958
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x95c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x960
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x964
			 0x3f028f5c,                                                  // 0.51                                        /// 0x968
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x96c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x970
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x974
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x978
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x97c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x980
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x984
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x988
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x98c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x990
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x994
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x998
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x99c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x9a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x9a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x9a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x9b4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x9b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x9bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x9c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x9c4
			 0x3f028f5c,                                                  // 0.51                                        /// 0x9c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x9cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x9d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x9d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x9dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x9e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x9f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x9f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x9fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xa00
			 0x4b000000,                                                  // 8388608.0                                   /// 0xa04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xa08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xa0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xa10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xa18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xa1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa2c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xa30
			 0x33333333,                                                  // 4 random values                             /// 0xa34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xa38
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xa40
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xa44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xa48
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xa4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xa50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xa54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xa5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xa60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xa64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa6c
			 0xbf028f5c,                                                  // -0.51                                       /// 0xa70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xa74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xa78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xa7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xa80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xa84
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xa88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xa8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xa90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xa94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xa98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xa9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xaa0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xaa4
			 0x3f028f5c,                                                  // 0.51                                        /// 0xaa8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xaac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xab0
			 0x33333333,                                                  // 4 random values                             /// 0xab4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xab8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xabc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xac0
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xac4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xac8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xacc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xad0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xad4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xad8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xadc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xae0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xae4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xae8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xaec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xaf0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xaf4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xaf8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xafc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xb00
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xb04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xb08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xb10
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xb14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xb18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xb1c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xb24
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xb28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xb2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xb30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xb34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb38
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xb3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xb40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xb44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xb48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xb50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xb58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xb60
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xb68
			 0x7f800000,                                                  // inf                                         /// 0xb6c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xb70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb74
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb78
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xb7c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xb80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xb84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xb88
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xb8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xb90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xb94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xb98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xb9c
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xba0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xba4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xba8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xbac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xbb0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xbb4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xbb8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xbbc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xbc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xbc4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xbc8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbd4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xbd8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xbdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbe0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xbe4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbe8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xbf0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbf4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xbf8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbfc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xc00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc04
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xc08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xc10
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xc14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xc18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xc1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xc20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xc24
			 0x80000000,                                                  // -zero                                       /// 0xc28
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xc2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xc30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xc34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xc40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xc44
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xc48
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xc4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xc50
			 0x7fc00001,                                                  // signaling NaN                               /// 0xc54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xc58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xc5c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xc60
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xc64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc68
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xc6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc70
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xc74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xc78
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xc7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xc80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xc84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xc88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc8c
			 0x55555555,                                                  // 4 random values                             /// 0xc90
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xc9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xca0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xca4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xca8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xcb0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xcb8
			 0x00000000,                                                  // zero                                        /// 0xcbc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xcc0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xcc8
			 0xbf028f5c,                                                  // -0.51                                       /// 0xccc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xcd0
			 0x7fc00001,                                                  // signaling NaN                               /// 0xcd4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xcd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xce0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xce4
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xce8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xcec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xcf0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xcf4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcf8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xcfc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xd00
			 0x80000000,                                                  // -zero                                       /// 0xd04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xd08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xd0c
			 0x55555555,                                                  // 4 random values                             /// 0xd10
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xd14
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xd18
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xd1c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd20
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xd24
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xd28
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xd2c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xd30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xd34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xd38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xd3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0xd40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xd44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xd48
			 0xbf028f5c,                                                  // -0.51                                       /// 0xd4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xd54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xd58
			 0x4b000000,                                                  // 8388608.0                                   /// 0xd5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd60
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xd64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd6c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xd74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd78
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xd7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xd80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xd84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xd88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xd8c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xd94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0xd98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xd9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xda0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xda4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xda8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xdb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xdb8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xdbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xdc0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xdc4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xdcc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xdd0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdd4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdd8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xddc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xde0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xde4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xde8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xdec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xdf0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xdf4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xdf8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xdfc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xe00
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe04
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xe08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xe0c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xe10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xe14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe18
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xe1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xe20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xe24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xe28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xe2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xe30
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xe34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xe38
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xe3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xe40
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xe44
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xe48
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xe4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xe50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xe54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xe58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xe5c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xe60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xe68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xe70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xe74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xe7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe80
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xe88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xe8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe90
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xe94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xe9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xea0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xea4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xea8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xeac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xeb0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xeb4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xeb8
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xebc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xec0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xec4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xec8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xecc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xed0
			 0x4b000000,                                                  // 8388608.0                                   /// 0xed4
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xed8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xedc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xee0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xee4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xee8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xeec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xef0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xef4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xef8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xefc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xf04
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xf08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xf10
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xf14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf1c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xf20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf24
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xf28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xf2c
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xf30
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf34
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xf38
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xf3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xf40
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xf44
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xf48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf4c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xf50
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xf54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf58
			 0x00000000,                                                  // zero                                        /// 0xf5c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xf60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xf64
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xf68
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xf6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xf70
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xf74
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xf7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xf80
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xf84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xf88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xf90
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xf94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xf9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xfa0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfa4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xfa8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xfac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xfb0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xfb4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xfb8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xfbc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xfc0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xfc8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xfcc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xfd0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xfd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xfd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfdc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xfe0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xfe4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xfe8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xff0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xff4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x24
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x40
			 0xbf028f5c,                                                  // -0.51                                       /// 0x44
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x48
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x6c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x7c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x80
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x8c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x90
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x94
			 0x33333333,                                                  // 4 random values                             /// 0x98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xa0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xa4
			 0x55555555,                                                  // 4 random values                             /// 0xa8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xb0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xb4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xbc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc0
			 0x00000000,                                                  // zero                                        /// 0xc4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xcc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xd4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xd8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xdc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xe0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xe4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xe8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xf0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xf4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xf8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xfc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x100
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x104
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x108
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x10c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x110
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x114
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x118
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x11c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x120
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x124
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x128
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x12c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x130
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x134
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x138
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x13c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x140
			 0x3f028f5c,                                                  // 0.51                                        /// 0x144
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x148
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x14c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x150
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x154
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x158
			 0x33333333,                                                  // 4 random values                             /// 0x15c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x160
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x164
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x168
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x16c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x170
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x174
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x178
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x17c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x180
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x184
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x188
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x18c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x190
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x194
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x198
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x19c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x1a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x1a4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x1a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x1b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x1b4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x1b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x1bc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x1c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x1c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x1c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x1cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x1d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x1d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x1dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x1e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x1e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x1ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x1f0
			 0x4b000000,                                                  // 8388608.0                                   /// 0x1f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x1f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x1fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x200
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x204
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x208
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x20c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x210
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x214
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x218
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x21c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x220
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x224
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x228
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x22c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x230
			 0x33333333,                                                  // 4 random values                             /// 0x234
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x238
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x23c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x240
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x244
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x248
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x24c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x250
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x254
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x258
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x25c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x260
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x264
			 0x55555555,                                                  // 4 random values                             /// 0x268
			 0xff800000,                                                  // -inf                                        /// 0x26c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x270
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x274
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x278
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x27c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x280
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x284
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x288
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x28c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x290
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x294
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x298
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x29c
			 0xcb000000,                                                  // -8388608.0                                  /// 0x2a0
			 0x55555555,                                                  // 4 random values                             /// 0x2a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x2ac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x2b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x2b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x2b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x2bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x2c0
			 0xffc00001,                                                  // -signaling NaN                              /// 0x2c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x2d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x2d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x2d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x2e0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x2e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x2ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x2f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x2f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x2fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x300
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x304
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x308
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x30c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x310
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x314
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x318
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x31c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x320
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x324
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x328
			 0x55555555,                                                  // 4 random values                             /// 0x32c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x330
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x334
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x338
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x33c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x340
			 0x3f028f5c,                                                  // 0.51                                        /// 0x344
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x348
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x34c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x350
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x354
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x358
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x35c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x360
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x364
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x368
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x36c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x370
			 0xbf028f5c,                                                  // -0.51                                       /// 0x374
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x378
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x37c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x380
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x384
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x388
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x38c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x390
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x394
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x398
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x39c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x3a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x3a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x3b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x3b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x3bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x3c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x3c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x3cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x3d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x3d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x3d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x3e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x3e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x3e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x3f0
			 0x3f028f5c,                                                  // 0.51                                        /// 0x3f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x3f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x400
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x404
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x408
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x40c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x410
			 0x33333333,                                                  // 4 random values                             /// 0x414
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x418
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x41c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x420
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x424
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x428
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x42c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x430
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x434
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x438
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x43c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x440
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x444
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x448
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x44c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x450
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x454
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x458
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x45c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x460
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x464
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x468
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x46c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x470
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x474
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x478
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x47c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x480
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x484
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x488
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x48c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x490
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x494
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x498
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x49c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x4a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x4a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x4a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x4ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x4bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x4c0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x4c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x4c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x4cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x4d0
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x4d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x4d8
			 0xcb000000,                                                  // -8388608.0                                  /// 0x4dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x4e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x4e4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x4ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x4f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x4f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x4fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x500
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x504
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x508
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x50c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x510
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x514
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x518
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x51c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x520
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x524
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x528
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x52c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x530
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x534
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x538
			 0xff800000,                                                  // -inf                                        /// 0x53c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x540
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x544
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x548
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x54c
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x550
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x554
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x558
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x55c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x560
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x564
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x568
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x56c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x570
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x574
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x578
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x57c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x580
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x584
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x588
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x58c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x590
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x594
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x598
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x59c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x5a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x5a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x5a8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x5ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x5b0
			 0x7f800000,                                                  // inf                                         /// 0x5b4
			 0xff800000,                                                  // -inf                                        /// 0x5b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x5bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x5c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x5c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x5cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x5d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x5dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x5e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x5e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x5e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x5f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x5f8
			 0x33333333,                                                  // 4 random values                             /// 0x5fc
			 0x7f800000,                                                  // inf                                         /// 0x600
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x604
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x608
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x60c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x610
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x614
			 0xff800000,                                                  // -inf                                        /// 0x618
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x61c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x620
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x624
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x628
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x62c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x630
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x634
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x638
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x63c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x640
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x644
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x648
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x64c
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x650
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x654
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x658
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x65c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x660
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x664
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x668
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x66c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x670
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x674
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x678
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x67c
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x680
			 0x80000000,                                                  // -zero                                       /// 0x684
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x688
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x68c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x690
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x694
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x698
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x69c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x6a0
			 0xff800000,                                                  // -inf                                        /// 0x6a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x6a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x6ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x6b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x6b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x6c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x6c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x6d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x6d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x6dc
			 0x3f028f5c,                                                  // 0.51                                        /// 0x6e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x6e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x6e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x6ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x6f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x6f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x6fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x700
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x704
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x708
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x70c
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x710
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x714
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x718
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x71c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x720
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x724
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x728
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x72c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x730
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x734
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x738
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x73c
			 0x3f028f5c,                                                  // 0.51                                        /// 0x740
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x744
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x748
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x74c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x750
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x754
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x758
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x75c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x760
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x764
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x768
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x76c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x770
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x774
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x778
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x77c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x780
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x784
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x788
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x78c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x790
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x794
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x798
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x79c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x7a0
			 0xffc00001,                                                  // -signaling NaN                              /// 0x7a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x7a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x7ac
			 0xcb000000,                                                  // -8388608.0                                  /// 0x7b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x7b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x7b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x7bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x7c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x7c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x7cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x7d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x7d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x7d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x7dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x7e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x7e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x7f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x7f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x800
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x804
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x808
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x80c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x810
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x814
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x818
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x81c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x820
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x824
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x828
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x82c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x830
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x834
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x838
			 0xff800000,                                                  // -inf                                        /// 0x83c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x840
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x844
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x848
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x84c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x850
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x854
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x858
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x85c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x860
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x864
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x868
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x86c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x870
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x874
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x878
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x87c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x880
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x884
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x888
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x88c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x890
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x894
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x898
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x89c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x8a4
			 0x55555555,                                                  // 4 random values                             /// 0x8a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x8ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x8b4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x8b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x8bc
			 0x80000000,                                                  // -zero                                       /// 0x8c0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x8c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x8c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x8cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x8d0
			 0xbf028f5c,                                                  // -0.51                                       /// 0x8d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x8e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x8e8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x8ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8f4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x8f8
			 0x00000000,                                                  // zero                                        /// 0x8fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x900
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x904
			 0x33333333,                                                  // 4 random values                             /// 0x908
			 0x7f800000,                                                  // inf                                         /// 0x90c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x910
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x914
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x918
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x91c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x920
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x924
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x928
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x92c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x930
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x934
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x938
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x93c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x940
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x944
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x948
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x94c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x950
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x954
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x958
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x95c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x960
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x964
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x968
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x96c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x970
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x974
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x978
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x97c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x980
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x984
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x988
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x98c
			 0x55555555,                                                  // 4 random values                             /// 0x990
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x994
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x998
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x99c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x9a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x9b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x9b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x9bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x9c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x9cc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x9d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x9d4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x9dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9e0
			 0x3f028f5c,                                                  // 0.51                                        /// 0x9e4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x9ec
			 0xff800000,                                                  // -inf                                        /// 0x9f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x9f8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x9fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xa00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xa08
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xa0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xa10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa14
			 0x80000000,                                                  // -zero                                       /// 0xa18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa1c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xa20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xa24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xa28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xa2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa30
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa34
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xa38
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xa3c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xa40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xa44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xa50
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xa54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa60
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xa64
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa68
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xa6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xa70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xa74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xa78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xa7c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xa80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa84
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xa88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xa8c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa90
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xa94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xa98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xa9c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xaa0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaa4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xaa8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xaac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xab0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xab4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xab8
			 0x55555555,                                                  // 4 random values                             /// 0xabc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xac0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xac4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xac8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xacc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xad0
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xad4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xad8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xadc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xae0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xae4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xae8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xaec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xaf0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaf4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xaf8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xafc
			 0x7fc00001,                                                  // signaling NaN                               /// 0xb00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb04
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xb08
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xb0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xb10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xb14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xb18
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xb1c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xb20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xb28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xb2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xb34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xb3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xb40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xb48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xb4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xb50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xb54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xb58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xb5c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xb60
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb64
			 0xff800000,                                                  // -inf                                        /// 0xb68
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xb6c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xb70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb74
			 0x7fc00001,                                                  // signaling NaN                               /// 0xb78
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xb7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xb80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xb84
			 0xcb000000,                                                  // -8388608.0                                  /// 0xb88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xb90
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xb94
			 0x00000000,                                                  // zero                                        /// 0xb98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xba0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xba4
			 0x33333333,                                                  // 4 random values                             /// 0xba8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xbac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xbb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xbb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbbc
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xbc0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbc4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xbc8
			 0xff800000,                                                  // -inf                                        /// 0xbcc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xbd0
			 0x33333333,                                                  // 4 random values                             /// 0xbd4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xbd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbe0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xbe4
			 0x7f800000,                                                  // inf                                         /// 0xbe8
			 0x33333333,                                                  // 4 random values                             /// 0xbec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xbf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xbf4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xbf8
			 0xffc00001,                                                  // -signaling NaN                              /// 0xbfc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xc00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xc04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xc08
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xc0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xc10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xc14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xc18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xc1c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xc28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xc2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xc30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xc38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xc3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xc44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc48
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xc4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc50
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xc54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xc58
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xc5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xc60
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xc64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc68
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xc6c
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xc70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc74
			 0x33333333,                                                  // 4 random values                             /// 0xc78
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xc7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xc80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xc84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xc88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc8c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xc90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xc94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xc98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xc9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xca0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xca4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xca8
			 0x3f028f5c,                                                  // 0.51                                        /// 0xcac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xcb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xcb4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xcb8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xcbc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcc0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xcc4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xcc8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xccc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xcd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xcd8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcdc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xce0
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xce4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xce8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xcf0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcf4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcf8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xcfc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xd00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd04
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xd08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xd0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xd10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xd14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xd18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xd1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xd20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xd24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xd28
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xd30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xd38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xd40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xd48
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xd4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xd50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xd58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xd5c
			 0x4b000000,                                                  // 8388608.0                                   /// 0xd60
			 0xbf028f5c,                                                  // -0.51                                       /// 0xd64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xd6c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xd70
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xd74
			 0x7fc00001,                                                  // signaling NaN                               /// 0xd78
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xd80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xd84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xd88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd8c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xd94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd9c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xda0
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xda4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xda8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xdac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xdb4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xdb8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xdbc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xdc0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdc4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xdc8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xdcc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdd0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xdd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xdd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xddc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xde0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xde4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xde8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xdec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xdf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdf4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xdf8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xdfc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xe00
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xe04
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xe08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xe0c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xe10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xe14
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xe18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xe1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xe24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xe28
			 0x80000000,                                                  // -zero                                       /// 0xe2c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xe34
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xe38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xe3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xe40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xe44
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xe48
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xe4c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xe50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xe54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xe58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xe5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe60
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xe64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xe68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xe70
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xe78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xe7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xe80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xe88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xe8c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xe90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xe94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xe98
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xe9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xea0
			 0xbf028f5c,                                                  // -0.51                                       /// 0xea4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xea8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xeac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xeb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xeb4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xeb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xebc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xec0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xec4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xec8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xecc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xed0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xed4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xed8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xedc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xee0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xee4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xee8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xeec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xef0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xef4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xef8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xefc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf00
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xf04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xf08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xf0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xf10
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xf14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xf18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xf20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xf28
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xf2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xf30
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xf34
			 0x00000000,                                                  // zero                                        /// 0xf38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xf3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xf40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xf4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xf50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xf54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xf58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xf64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xf68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xf6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf70
			 0x55555555,                                                  // 4 random values                             /// 0xf74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xf7c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xf88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xf8c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xf98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf9c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xfa0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xfa4
			 0x4b000000,                                                  // 8388608.0                                   /// 0xfa8
			 0x33333333,                                                  // 4 random values                             /// 0xfac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xfb0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfb4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfb8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xfbc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xfc0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xfc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xfc8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xfcc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfd0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xfd4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfd8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xfdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xfe0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xfe4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfe8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xfec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xff0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xff4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x10
			 0x33333333,                                                  // 4 random values                             /// 0x14
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x1c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x24
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x28
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x34
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x4c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x80
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x88
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x90
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x94
			 0xbf028f5c,                                                  // -0.51                                       /// 0x98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xa0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xa8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xac
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xbc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xc0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xc4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc8
			 0x4b000000,                                                  // 8388608.0                                   /// 0xcc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xd0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xd8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xdc
			 0xcb000000,                                                  // -8388608.0                                  /// 0xe0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xe4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xec
			 0xff800000,                                                  // -inf                                        /// 0xf0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xf4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xfc
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x100
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x104
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x108
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x10c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x110
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x114
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x118
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x11c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x120
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x124
			 0x80000000,                                                  // -zero                                       /// 0x128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x12c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x130
			 0x7f800000,                                                  // inf                                         /// 0x134
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x138
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x13c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x140
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x144
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x148
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x14c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x150
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x154
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x158
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x15c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x160
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x164
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x168
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x16c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x170
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x174
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x178
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x17c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x180
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x184
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x188
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x18c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x190
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x194
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x198
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x19c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x1a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x1a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x1a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x1ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x1b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x1b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x1b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x1bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x1c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x1c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x1c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x1cc
			 0x80000000,                                                  // -zero                                       /// 0x1d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x1e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x1e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x1e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x1ec
			 0x55555555,                                                  // 4 random values                             /// 0x1f0
			 0xffc00001,                                                  // -signaling NaN                              /// 0x1f4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x1f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x200
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x204
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x208
			 0x33333333,                                                  // 4 random values                             /// 0x20c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x210
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x214
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x218
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x21c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x220
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x224
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x228
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x22c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x230
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x234
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x238
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x23c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x240
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x244
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x248
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x24c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x250
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x254
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x258
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x25c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x260
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x264
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x268
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x26c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x270
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x274
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x278
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x27c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x280
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x284
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x288
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x28c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x290
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x294
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x298
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x29c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x2a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x2a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x2a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x2ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x2b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x2b4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x2bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x2c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x2c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x2c8
			 0x7fc00001,                                                  // signaling NaN                               /// 0x2cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x2d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x2d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x2d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x2e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x2e4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x2e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x2f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x2f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x2f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x2fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x300
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x304
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x308
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x30c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x310
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x314
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x318
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x31c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x320
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x324
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x328
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x32c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x330
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x334
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x338
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x33c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x340
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x344
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x348
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x34c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x350
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x354
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x358
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x35c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x360
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x364
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x368
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x36c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x370
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x374
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x378
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x37c
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x380
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x384
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x388
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x38c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x390
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x394
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x398
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x39c
			 0x4b000000,                                                  // 8388608.0                                   /// 0x3a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x3a4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x3a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x3ac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x3b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x3b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x3b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x3bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x3c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x3c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x3c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x3cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x3d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x3d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x3d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x3dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x3e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3e4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x3ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x3f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x3fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x400
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x404
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x408
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x40c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x410
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x414
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x418
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x41c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x420
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x424
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x428
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x42c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x430
			 0x00000000,                                                  // zero                                        /// 0x434
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x438
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x43c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x440
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x444
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x448
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x44c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x450
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x454
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x458
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x45c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x460
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x464
			 0x33333333,                                                  // 4 random values                             /// 0x468
			 0x4b000000,                                                  // 8388608.0                                   /// 0x46c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x470
			 0x00000000,                                                  // zero                                        /// 0x474
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x478
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x47c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x480
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x484
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x488
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x48c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x490
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x494
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x498
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x49c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x4a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x4a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4a8
			 0xff800000,                                                  // -inf                                        /// 0x4ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x4b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x4bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4c4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4c8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x4cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4d0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x4d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x4d8
			 0x33333333,                                                  // 4 random values                             /// 0x4dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4e0
			 0xffc00001,                                                  // -signaling NaN                              /// 0x4e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x4e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x4ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x4f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x4f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x500
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x504
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x508
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x50c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x510
			 0x7f800000,                                                  // inf                                         /// 0x514
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x518
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x51c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x520
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x524
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x528
			 0x3f028f5c,                                                  // 0.51                                        /// 0x52c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x530
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x534
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x538
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x53c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x540
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x544
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x548
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x54c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x550
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x554
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x558
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x55c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x560
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x564
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x568
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x56c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x570
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x574
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x578
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x57c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x580
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x584
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x588
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x58c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x590
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x594
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x598
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x59c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x5a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x5a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x5ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x5b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x5b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x5b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x5bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5c0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x5c8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x5cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x5d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x5d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x5d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x5dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x5e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x5e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x5ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x5f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x5f4
			 0x33333333,                                                  // 4 random values                             /// 0x5f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x5fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x600
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x604
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x608
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x60c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x610
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x614
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x618
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x61c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x620
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x624
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x628
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x62c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x630
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x634
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x638
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x63c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x640
			 0x33333333,                                                  // 4 random values                             /// 0x644
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x648
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x64c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x650
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x654
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x658
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x65c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x660
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x664
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x668
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x66c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x670
			 0x80000000,                                                  // -zero                                       /// 0x674
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x678
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x67c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x680
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x684
			 0x00000000,                                                  // zero                                        /// 0x688
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x68c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x690
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x694
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x698
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x69c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x6a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x6a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x6a8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x6b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x6b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x6bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x6c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x6c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x6cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6d0
			 0x4b000000,                                                  // 8388608.0                                   /// 0x6d4
			 0x7fc00001,                                                  // signaling NaN                               /// 0x6d8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x6dc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x6e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x6e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x6e8
			 0xbf028f5c,                                                  // -0.51                                       /// 0x6ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x6f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x6f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x700
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x704
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x708
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x70c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x710
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x714
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x718
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x71c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x720
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x724
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x728
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x72c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x730
			 0xff800000,                                                  // -inf                                        /// 0x734
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x738
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x73c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x740
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x744
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x748
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x74c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x750
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x754
			 0xbf028f5c,                                                  // -0.51                                       /// 0x758
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x75c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x760
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x764
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x768
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x76c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x770
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x774
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x778
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x77c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x780
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x784
			 0x7fc00001,                                                  // signaling NaN                               /// 0x788
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x78c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x790
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x794
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x798
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x79c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x7a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x7a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x7a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x7ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x7b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x7b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x7b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x7bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x7c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x7c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x7cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x7d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x7d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x7d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x7dc
			 0x4b000000,                                                  // 8388608.0                                   /// 0x7e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x7e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x7e8
			 0xffc00001,                                                  // -signaling NaN                              /// 0x7ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7f0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x7f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x7f8
			 0x7fc00001,                                                  // signaling NaN                               /// 0x7fc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x800
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x804
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x808
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x80c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x810
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x814
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x818
			 0xbf028f5c,                                                  // -0.51                                       /// 0x81c
			 0x33333333,                                                  // 4 random values                             /// 0x820
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x824
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x828
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x82c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x830
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x834
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x838
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x83c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x840
			 0x00000000,                                                  // zero                                        /// 0x844
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x848
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x84c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x850
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x854
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x858
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x85c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x860
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x864
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x868
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x86c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x870
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x874
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x878
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x87c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x880
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x884
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x888
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x88c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x890
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x894
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x898
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x89c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x8a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x8a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x8ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x8b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x8bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x8c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x8c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x8c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x8cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x8d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x8d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x8d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x8dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x8e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x8e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x8e8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x8ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x8f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x8f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x8f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x8fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x900
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x904
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x908
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x90c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x910
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x914
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x918
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x91c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x920
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x924
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x928
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x92c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x930
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x934
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x938
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x93c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x940
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x944
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x948
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x94c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x950
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x954
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x958
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x95c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x960
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x964
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x968
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x96c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x970
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x974
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x978
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x97c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x980
			 0xffc00001,                                                  // -signaling NaN                              /// 0x984
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x988
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x98c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x990
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x994
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x998
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x99c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x9a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x9a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x9ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x9b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x9b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x9b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x9bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9c8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x9cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x9d0
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x9d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x9d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9e8
			 0x4b000000,                                                  // 8388608.0                                   /// 0x9ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x9f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x9f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x9f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xa04
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xa08
			 0xcb000000,                                                  // -8388608.0                                  /// 0xa0c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa10
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xa14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xa18
			 0x4b000000,                                                  // 8388608.0                                   /// 0xa1c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xa20
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xa24
			 0xbf028f5c,                                                  // -0.51                                       /// 0xa28
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xa2c
			 0x55555555,                                                  // 4 random values                             /// 0xa30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xa34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xa38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xa3c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xa40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xa44
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xa48
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa4c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xa54
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xa58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xa5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xa60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xa64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xa68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xa6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa70
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xa74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xa78
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xa7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xa80
			 0xffc00001,                                                  // -signaling NaN                              /// 0xa84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xa88
			 0x4b000000,                                                  // 8388608.0                                   /// 0xa8c
			 0x00000000,                                                  // zero                                        /// 0xa90
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xa94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xa98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xa9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaa0
			 0x3f028f5c,                                                  // 0.51                                        /// 0xaa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xaa8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xaac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xab0
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xab4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xab8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xabc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xac0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xac4
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xac8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xacc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xad0
			 0x7f800000,                                                  // inf                                         /// 0xad4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xad8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xadc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xae0
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xae4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xae8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xaec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xaf0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xaf4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xaf8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xafc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xb00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xb04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xb08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xb0c
			 0x7f800000,                                                  // inf                                         /// 0xb10
			 0x55555555,                                                  // 4 random values                             /// 0xb14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xb18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xb1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb24
			 0x7fc00001,                                                  // signaling NaN                               /// 0xb28
			 0x00000000,                                                  // zero                                        /// 0xb2c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xb30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb34
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xb38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xb3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xb40
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xb4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xb50
			 0x00000000,                                                  // zero                                        /// 0xb54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xb5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xb60
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xb6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb70
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xb74
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xb78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xb7c
			 0x55555555,                                                  // 4 random values                             /// 0xb80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb84
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xb88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xb94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xb98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xb9c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xba0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xba4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xba8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xbb0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xbb4
			 0x7f800000,                                                  // inf                                         /// 0xbb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xbbc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xbc0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xbc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xbc8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbcc
			 0x80000000,                                                  // -zero                                       /// 0xbd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xbd4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xbd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xbdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xbe0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbe4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0xbe8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xbec
			 0xffc00001,                                                  // -signaling NaN                              /// 0xbf0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xbf4
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xbf8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xbfc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xc04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xc08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xc0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xc18
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc1c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xc20
			 0xcb000000,                                                  // -8388608.0                                  /// 0xc24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xc28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xc30
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xc34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xc38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xc3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xc40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xc44
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xc48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xc50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xc54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xc58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xc5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xc60
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xc64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xc68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xc74
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xc80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xc84
			 0x00000000,                                                  // zero                                        /// 0xc88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc8c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc94
			 0x7f800000,                                                  // inf                                         /// 0xc98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc9c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xca0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xca4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xca8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xcb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xcb4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xcbc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xcc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcc8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xccc
			 0x00000000,                                                  // zero                                        /// 0xcd0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xcd4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xcd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xcdc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xce0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xce4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xce8
			 0xcb000000,                                                  // -8388608.0                                  /// 0xcec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xcf0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcf4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xcf8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xcfc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xd00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xd04
			 0x55555555,                                                  // 4 random values                             /// 0xd08
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xd10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xd18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xd1c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xd20
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd24
			 0x00000000,                                                  // zero                                        /// 0xd28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xd30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xd34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xd38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xd3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xd40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xd44
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd4c
			 0xffc00001,                                                  // -signaling NaN                              /// 0xd50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xd54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xd58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xd5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xd60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xd68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xd6c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xd70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xd74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xd78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xd7c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xd80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xd84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xd88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xd8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xd94
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xd98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xda0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xda4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xda8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xdac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xdb0
			 0xbf028f5c,                                                  // -0.51                                       /// 0xdb4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xdb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xdbc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xdc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdc4
			 0x7f800000,                                                  // inf                                         /// 0xdc8
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xdcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xdd0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xdd4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xdd8
			 0x55555555,                                                  // 4 random values                             /// 0xddc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xde0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xde4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xde8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xdf0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xdf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdf8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xdfc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xe00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xe04
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xe08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xe0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xe10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xe14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xe1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xe24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xe2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xe30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe34
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xe38
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xe3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xe40
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xe44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xe48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xe4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xe54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xe58
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xe5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe60
			 0x80000000,                                                  // -zero                                       /// 0xe64
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xe68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xe70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xe74
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xe7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xe80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xe84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xe88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xe8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xe90
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe94
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xe98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xe9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xea0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xea4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xea8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xeac
			 0xcb000000,                                                  // -8388608.0                                  /// 0xeb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xeb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xeb8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xebc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xec0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xec4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xec8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xecc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xed0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xed4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xed8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xedc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xee0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xee4
			 0x4b000000,                                                  // 8388608.0                                   /// 0xee8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xeec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xef0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xef4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xef8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xefc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xf00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xf04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xf08
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xf10
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xf14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xf18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xf1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf24
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xf28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xf2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xf30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xf34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xf38
			 0x3f028f5c,                                                  // 0.51                                        /// 0xf3c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xf40
			 0xcb000000,                                                  // -8388608.0                                  /// 0xf44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xf4c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xf50
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xf54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xf58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf64
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xf68
			 0xffc00001,                                                  // -signaling NaN                              /// 0xf6c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xf70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xf74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xf7c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xf80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xf84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xf88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xf8c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xf90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xf94
			 0xff800000,                                                  // -inf                                        /// 0xf98
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf9c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xfa0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xfa4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xfa8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xfb4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xfb8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xfbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xfc0
			 0x55555555,                                                  // 4 random values                             /// 0xfc4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfc8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xfcc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xfd0
			 0x4b000000,                                                  // 8388608.0                                   /// 0xfd4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xfd8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xfdc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfe0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xfe4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xfe8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xff0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xff4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xff8
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
			 0x41d80000,
			 0x41d00000,
			 0x41e80000,
			 0x41e80000,
			 0x41c00000,
			 0x40400000,
			 0x41300000,
			 0x41c80000,

			 // vpu register f3
			 0x41b80000,
			 0x41800000,
			 0x41600000,
			 0x41a80000,
			 0x41700000,
			 0x41c00000,
			 0x41980000,
			 0x41600000,

			 // vpu register f4
			 0x41200000,
			 0x41c00000,
			 0x3f800000,
			 0x41980000,
			 0x41100000,
			 0x41700000,
			 0x41600000,
			 0x41a00000,

			 // vpu register f5
			 0x41d80000,
			 0x41300000,
			 0x3f800000,
			 0x40a00000,
			 0x41a80000,
			 0x41980000,
			 0x41b80000,
			 0x40a00000,

			 // vpu register f6
			 0x40400000,
			 0x41a80000,
			 0x40e00000,
			 0x41f00000,
			 0x41400000,
			 0x41500000,
			 0x41c80000,
			 0x41d80000,

			 // vpu register f7
			 0x41880000,
			 0x41b00000,
			 0x41600000,
			 0x41800000,
			 0x41500000,
			 0x41880000,
			 0x41f80000,
			 0x41980000,

			 // vpu register f8
			 0x41a80000,
			 0x41200000,
			 0x42000000,
			 0x41980000,
			 0x41c80000,
			 0x41300000,
			 0x41d00000,
			 0x41400000,

			 // vpu register f9
			 0x41300000,
			 0x41d00000,
			 0x41700000,
			 0x41200000,
			 0x41b80000,
			 0x40400000,
			 0x41500000,
			 0x40c00000,

			 // vpu register f10
			 0x41500000,
			 0x41d80000,
			 0x41000000,
			 0x40c00000,
			 0x41900000,
			 0x41880000,
			 0x41c80000,
			 0x40000000,

			 // vpu register f11
			 0x41880000,
			 0x41980000,
			 0x41700000,
			 0x41b00000,
			 0x41800000,
			 0x41b00000,
			 0x41d00000,
			 0x41200000,

			 // vpu register f12
			 0x41e80000,
			 0x41a80000,
			 0x41a00000,
			 0x41f00000,
			 0x41e00000,
			 0x41b80000,
			 0x41e80000,
			 0x41980000,

			 // vpu register f13
			 0x41d80000,
			 0x41900000,
			 0x40e00000,
			 0x41300000,
			 0x41a00000,
			 0x41100000,
			 0x41800000,
			 0x41b80000,

			 // vpu register f14
			 0x41a80000,
			 0x40000000,
			 0x41e80000,
			 0x41200000,
			 0x41600000,
			 0x41b80000,
			 0x41d00000,
			 0x41d00000,

			 // vpu register f15
			 0x41100000,
			 0x41800000,
			 0x40a00000,
			 0x40c00000,
			 0x41880000,
			 0x41600000,
			 0x41d80000,
			 0x41900000,

			 // vpu register f16
			 0x41200000,
			 0x42000000,
			 0x3f800000,
			 0x41100000,
			 0x41b80000,
			 0x40e00000,
			 0x41d80000,
			 0x41900000,

			 // vpu register f17
			 0x41980000,
			 0x41b80000,
			 0x41800000,
			 0x41600000,
			 0x42000000,
			 0x40000000,
			 0x41e80000,
			 0x40c00000,

			 // vpu register f18
			 0x41200000,
			 0x40a00000,
			 0x41300000,
			 0x41700000,
			 0x41800000,
			 0x40c00000,
			 0x40800000,
			 0x41f00000,

			 // vpu register f19
			 0x41a80000,
			 0x41f00000,
			 0x41000000,
			 0x41400000,
			 0x3f800000,
			 0x41200000,
			 0x41f00000,
			 0x41500000,

			 // vpu register f20
			 0x41a80000,
			 0x41f00000,
			 0x41b80000,
			 0x41880000,
			 0x40a00000,
			 0x41980000,
			 0x41800000,
			 0x40400000,

			 // vpu register f21
			 0x41980000,
			 0x41e80000,
			 0x41a00000,
			 0x41a80000,
			 0x41b80000,
			 0x41b00000,
			 0x41600000,
			 0x41d00000,

			 // vpu register f22
			 0x3f800000,
			 0x40400000,
			 0x40a00000,
			 0x41000000,
			 0x41d80000,
			 0x41880000,
			 0x41880000,
			 0x40800000,

			 // vpu register f23
			 0x41000000,
			 0x41900000,
			 0x40e00000,
			 0x41000000,
			 0x41000000,
			 0x40800000,
			 0x41600000,
			 0x41900000,

			 // vpu register f24
			 0x41f80000,
			 0x40400000,
			 0x40800000,
			 0x41e00000,
			 0x41d80000,
			 0x41e80000,
			 0x41b80000,
			 0x41f00000,

			 // vpu register f25
			 0x41500000,
			 0x40a00000,
			 0x40800000,
			 0x41700000,
			 0x40400000,
			 0x41200000,
			 0x41b80000,
			 0x41a00000,

			 // vpu register f26
			 0x40a00000,
			 0x41200000,
			 0x3f800000,
			 0x41a00000,
			 0x41200000,
			 0x41880000,
			 0x41980000,
			 0x41500000,

			 // vpu register f27
			 0x40a00000,
			 0x41900000,
			 0x3f800000,
			 0x40e00000,
			 0x41100000,
			 0x41b00000,
			 0x41c80000,
			 0x41980000,

			 // vpu register f28
			 0x40400000,
			 0x41100000,
			 0x41600000,
			 0x41800000,
			 0x40400000,
			 0x41a80000,
			 0x41700000,
			 0x40000000,

			 // vpu register f29
			 0x41100000,
			 0x3f800000,
			 0x41c00000,
			 0x41b00000,
			 0x41100000,
			 0x40e00000,
			 0x41f00000,
			 0x41900000,

			 // vpu register f30
			 0x41300000,
			 0x41200000,
			 0x41800000,
			 0x41e80000,
			 0x41d80000,
			 0x41980000,
			 0x41d00000,
			 0x41c00000,

			 // vpu register f31
			 0x41b80000,
			 0x41a00000,
			 0x40800000,
			 0x41600000,
			 0x41d80000,
			 0x41c80000,
			 0x40800000,
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

///////////////////////////////////////tensor_load_vpu
        "add x5, x11, zero\n"
        "li x31, 0x41\n"
        "li x6,  0xb \n"
        "add x5, x6, x5\n"    //   use_tmask [63:63] = 0x0,  use_coop [62:62] = 0x0,  dst_start [58:53] = 0x0,  num_lines [3:0] = 0xb, vaddr = %[m_11]
        "csrrw zero,  0x83f , x5\n"  //  tensor_load_vpu
        "csrwi   0x830, 1\n"  // wait on tensor_load with appropriate id

///////////////////////////////////////tensor_ima_vpu
        "li x5,  0x1bb800000000007 \n"          //   use_tmask [63:63] = 0x0,  b_num_cols [56:55] = 0x3,  a_num_rows [54:51] = 0x7,  a_num_cols [50:47] = 0x7,  offset [46:43] = 0x0,  to_vrf [23:23] = 0x0,  u_b [22:22] = 0x0,  u_a [21:21] = 0x0,  ten_b_in_mem [20:20] = 0x0,  scp_loc_b [19:12] = 0x0,  scp_loc_a [11:4] = 0x0,  reset_tenc [0:0] = 0x1,
        "csrrw zero,  0x801 , x5\n"  //  tensor_ima_vpu
        "csrwi   0x830, 0x7\n"  // wait on tfma/ima


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
