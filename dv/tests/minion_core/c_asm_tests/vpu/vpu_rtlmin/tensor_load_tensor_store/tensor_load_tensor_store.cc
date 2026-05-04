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
#define SET_SHIRE_COOP        __asm__ __volatile__ ( "li    x30, 1\n" \
                                                     "li    x31, 0x80740290\n" \
                                                     "sd    x30, 0(x31)\n" \
                                                     "fence" \
                                                     : \
                                                     : \
                                                     : "x30", "x31" \
                                                   );
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
			 0xcec9c2d, /// 0x0
			 0x65c933a7, /// 0x4
			 0x8b907598, /// 0x8
			 0xa402f986, /// 0xc
			 0xaf7b11d1, /// 0x10
			 0x36aa0d7a, /// 0x14
			 0x8f9e4074, /// 0x18
			 0x9516a607, /// 0x1c
			 0xfee4c217, /// 0x20
			 0xcd65b50d, /// 0x24
			 0x4f2c2e3, /// 0x28
			 0xb6ac8f64, /// 0x2c
			 0xeb677bac, /// 0x30
			 0x23ffda03, /// 0x34
			 0xcca226b, /// 0x38
			 0x53a8d98c, /// 0x3c
			 0xf3f7caae, /// 0x40
			 0x3f6c6856, /// 0x44
			 0xf05672ef, /// 0x48
			 0x5fa41767, /// 0x4c
			 0xf0d0e3e0, /// 0x50
			 0xbd462836, /// 0x54
			 0x4301be7b, /// 0x58
			 0xf33f341b, /// 0x5c
			 0x8be5edd, /// 0x60
			 0x177678d5, /// 0x64
			 0xefda739, /// 0x68
			 0x8c53945d, /// 0x6c
			 0x530055d0, /// 0x70
			 0xf54e8061, /// 0x74
			 0x5b77038, /// 0x78
			 0x3b406e08, /// 0x7c
			 0xf0dc74cd, /// 0x80
			 0xadc0c867, /// 0x84
			 0x5030cd41, /// 0x88
			 0x2dcbf9b, /// 0x8c
			 0x247ab0b6, /// 0x90
			 0x1194c456, /// 0x94
			 0x180080fe, /// 0x98
			 0x193fdf7c, /// 0x9c
			 0x5959e440, /// 0xa0
			 0x22f0695e, /// 0xa4
			 0x4ad42e0d, /// 0xa8
			 0x1b4a2b88, /// 0xac
			 0x8ecd4491, /// 0xb0
			 0xc985deb, /// 0xb4
			 0x91c1d00e, /// 0xb8
			 0xe81230a3, /// 0xbc
			 0x11faf2a6, /// 0xc0
			 0x29735bcf, /// 0xc4
			 0x1cfb885a, /// 0xc8
			 0x935a3634, /// 0xcc
			 0xb755456d, /// 0xd0
			 0x1ad46d3, /// 0xd4
			 0x9238976c, /// 0xd8
			 0x9e4d94f7, /// 0xdc
			 0x456f1413, /// 0xe0
			 0xf8e4c61, /// 0xe4
			 0xdf658f2, /// 0xe8
			 0x18608699, /// 0xec
			 0x22e9ae64, /// 0xf0
			 0x6f12a8a, /// 0xf4
			 0x5a5cf11b, /// 0xf8
			 0x25830e52, /// 0xfc
			 0xe7ff5589, /// 0x100
			 0xcfec8983, /// 0x104
			 0x1999d3dc, /// 0x108
			 0x1def1199, /// 0x10c
			 0x658c8b69, /// 0x110
			 0x8e71e60c, /// 0x114
			 0xe0f4df87, /// 0x118
			 0xa5cf48f9, /// 0x11c
			 0xe9e46663, /// 0x120
			 0xee3383c1, /// 0x124
			 0x284a667, /// 0x128
			 0xa664825d, /// 0x12c
			 0x67282d39, /// 0x130
			 0x52262bca, /// 0x134
			 0xa07771d6, /// 0x138
			 0x83430dbd, /// 0x13c
			 0xd6c4e433, /// 0x140
			 0x7e00a64e, /// 0x144
			 0xbc6b1a81, /// 0x148
			 0xaa9d1f62, /// 0x14c
			 0x1cbdeec5, /// 0x150
			 0xb400e84, /// 0x154
			 0x21d62ed4, /// 0x158
			 0xd529932e, /// 0x15c
			 0x6b86037b, /// 0x160
			 0x7968aacb, /// 0x164
			 0x3e3c8bc3, /// 0x168
			 0x63f50f58, /// 0x16c
			 0x6d6d2b07, /// 0x170
			 0xd55ca62b, /// 0x174
			 0xfe6d1846, /// 0x178
			 0xd7753e27, /// 0x17c
			 0xb9adc727, /// 0x180
			 0x87325f14, /// 0x184
			 0x7df57bc6, /// 0x188
			 0xa041c46e, /// 0x18c
			 0x5b106d56, /// 0x190
			 0x8cfe981e, /// 0x194
			 0x60a7ac2b, /// 0x198
			 0xd934d06f, /// 0x19c
			 0xfc9e31b3, /// 0x1a0
			 0xed9244ec, /// 0x1a4
			 0x96db7e1c, /// 0x1a8
			 0xd224267e, /// 0x1ac
			 0xc0c559cc, /// 0x1b0
			 0x2ecfddd, /// 0x1b4
			 0x6020fb48, /// 0x1b8
			 0xfaf8f3ac, /// 0x1bc
			 0x8d6885d6, /// 0x1c0
			 0x793eb9ea, /// 0x1c4
			 0x5b980285, /// 0x1c8
			 0xa1c80c81, /// 0x1cc
			 0xf07ba39e, /// 0x1d0
			 0x9991095c, /// 0x1d4
			 0x91268f29, /// 0x1d8
			 0x1dfe4b1d, /// 0x1dc
			 0x782598cb, /// 0x1e0
			 0xed435840, /// 0x1e4
			 0x50838a1f, /// 0x1e8
			 0x5502ec27, /// 0x1ec
			 0x7b9fe7b7, /// 0x1f0
			 0x2fd4b69e, /// 0x1f4
			 0x3032aec0, /// 0x1f8
			 0xff710219, /// 0x1fc
			 0xe32e0d99, /// 0x200
			 0x80a4632c, /// 0x204
			 0xf073c928, /// 0x208
			 0xeb4f0449, /// 0x20c
			 0x99790507, /// 0x210
			 0x14508bd2, /// 0x214
			 0x1d389bc9, /// 0x218
			 0x34bab25f, /// 0x21c
			 0x8e8dae3a, /// 0x220
			 0xc7eeef8d, /// 0x224
			 0xf1c2f604, /// 0x228
			 0x966cf221, /// 0x22c
			 0xec0057ab, /// 0x230
			 0x317f484f, /// 0x234
			 0xc72d96fd, /// 0x238
			 0xaabda79f, /// 0x23c
			 0x3277901d, /// 0x240
			 0x6a334aec, /// 0x244
			 0x7b807e14, /// 0x248
			 0xb9a4b6da, /// 0x24c
			 0x3a31a79, /// 0x250
			 0x3681ca16, /// 0x254
			 0x8a8b7795, /// 0x258
			 0xa0b128c6, /// 0x25c
			 0xd5ece79c, /// 0x260
			 0x6da8ecbb, /// 0x264
			 0x7926e539, /// 0x268
			 0x16f04c98, /// 0x26c
			 0xeca46e45, /// 0x270
			 0xe423be5a, /// 0x274
			 0x4b343c85, /// 0x278
			 0xd416aa4d, /// 0x27c
			 0xdec733e9, /// 0x280
			 0x92171586, /// 0x284
			 0x4351ff3d, /// 0x288
			 0x1d5909be, /// 0x28c
			 0x354203c1, /// 0x290
			 0x1c035bc5, /// 0x294
			 0xed6ab03c, /// 0x298
			 0x6ec83540, /// 0x29c
			 0x793402c3, /// 0x2a0
			 0xf133f5e2, /// 0x2a4
			 0x4d6e4232, /// 0x2a8
			 0x1d8ec04e, /// 0x2ac
			 0xab2d3b2a, /// 0x2b0
			 0x39fef720, /// 0x2b4
			 0x66ce5fce, /// 0x2b8
			 0x6e64140, /// 0x2bc
			 0x6ff63614, /// 0x2c0
			 0x4cfa7c0f, /// 0x2c4
			 0xcd83c5d3, /// 0x2c8
			 0x12052887, /// 0x2cc
			 0x3b9ea32d, /// 0x2d0
			 0x6af9d083, /// 0x2d4
			 0x94ec0048, /// 0x2d8
			 0xc895d78c, /// 0x2dc
			 0xba0cb2aa, /// 0x2e0
			 0x44f5fd3d, /// 0x2e4
			 0xbaab0148, /// 0x2e8
			 0x17fabbc5, /// 0x2ec
			 0xfc14ef1f, /// 0x2f0
			 0xe392f671, /// 0x2f4
			 0x76f2f8f8, /// 0x2f8
			 0x450c387f, /// 0x2fc
			 0x2a325200, /// 0x300
			 0x13628c62, /// 0x304
			 0x96916fdd, /// 0x308
			 0xf5167fa0, /// 0x30c
			 0x60dca93d, /// 0x310
			 0x2c6ff853, /// 0x314
			 0x19220d78, /// 0x318
			 0xa0a7948b, /// 0x31c
			 0xec766964, /// 0x320
			 0x748b6c00, /// 0x324
			 0x24cae50, /// 0x328
			 0x993fb21a, /// 0x32c
			 0x2e55411b, /// 0x330
			 0x549ec2d5, /// 0x334
			 0x8562fc75, /// 0x338
			 0x6d51eec8, /// 0x33c
			 0xe3efddb1, /// 0x340
			 0x275149d4, /// 0x344
			 0x43c64f6f, /// 0x348
			 0xd9e4b96f, /// 0x34c
			 0x91b5d631, /// 0x350
			 0xf705f418, /// 0x354
			 0x9d5a4629, /// 0x358
			 0x803b0355, /// 0x35c
			 0x1ba7e97a, /// 0x360
			 0xdac67026, /// 0x364
			 0x7474b7f3, /// 0x368
			 0x53a8e792, /// 0x36c
			 0xb514e1a8, /// 0x370
			 0x38b644ec, /// 0x374
			 0xb2951f55, /// 0x378
			 0x1a805366, /// 0x37c
			 0xa35a6d46, /// 0x380
			 0x1ecea0ea, /// 0x384
			 0xfc614ee7, /// 0x388
			 0x4f6878db, /// 0x38c
			 0xa1307024, /// 0x390
			 0x80e1b0d3, /// 0x394
			 0xe50e56a1, /// 0x398
			 0xaa61f22f, /// 0x39c
			 0xb339b3e0, /// 0x3a0
			 0xf96315d0, /// 0x3a4
			 0x64cd335c, /// 0x3a8
			 0xc6ac7e75, /// 0x3ac
			 0x5c4bf42b, /// 0x3b0
			 0xfda9193f, /// 0x3b4
			 0xdc4f2b3, /// 0x3b8
			 0xf0ebf17, /// 0x3bc
			 0x3e85b3ea, /// 0x3c0
			 0xfa1a6c25, /// 0x3c4
			 0x871d7599, /// 0x3c8
			 0xbb8fae98, /// 0x3cc
			 0x2cadd76e, /// 0x3d0
			 0x89712dd3, /// 0x3d4
			 0x52f2f5a8, /// 0x3d8
			 0x486108d3, /// 0x3dc
			 0xb7bdcea, /// 0x3e0
			 0x2c9a52c5, /// 0x3e4
			 0x50a9bc2f, /// 0x3e8
			 0x8cc6472e, /// 0x3ec
			 0xf8e9ff4, /// 0x3f0
			 0x773e836f, /// 0x3f4
			 0xa6a6043a, /// 0x3f8
			 0x8db87dc1, /// 0x3fc
			 0x8a4c80d9, /// 0x400
			 0x4afe29fe, /// 0x404
			 0xd8dba879, /// 0x408
			 0x38818031, /// 0x40c
			 0x4fa971ec, /// 0x410
			 0x6f9f141d, /// 0x414
			 0x50ca74d3, /// 0x418
			 0xc5d82fa0, /// 0x41c
			 0x511af222, /// 0x420
			 0xd1877a56, /// 0x424
			 0x35ec4727, /// 0x428
			 0xf80e59b5, /// 0x42c
			 0xa4272e41, /// 0x430
			 0x5b6830b2, /// 0x434
			 0xddf162d2, /// 0x438
			 0x1477c44e, /// 0x43c
			 0xcc8c5878, /// 0x440
			 0x5ac95a25, /// 0x444
			 0x595dcb3b, /// 0x448
			 0xe04c7c6a, /// 0x44c
			 0xf7a25c41, /// 0x450
			 0x45d06eee, /// 0x454
			 0x8203527d, /// 0x458
			 0x5a9fba0, /// 0x45c
			 0x29bc8b1c, /// 0x460
			 0xe0f6ce5e, /// 0x464
			 0x8e2d9270, /// 0x468
			 0x7e503e50, /// 0x46c
			 0x6c41187, /// 0x470
			 0x5e62d18c, /// 0x474
			 0xf229e0b8, /// 0x478
			 0x1ca0c5e2, /// 0x47c
			 0xb167a399, /// 0x480
			 0xb4d46295, /// 0x484
			 0x3f0cda69, /// 0x488
			 0xe9323b1c, /// 0x48c
			 0x1784b8bd, /// 0x490
			 0xf3e800d7, /// 0x494
			 0xb90e4477, /// 0x498
			 0x7c3115dd, /// 0x49c
			 0xa155810a, /// 0x4a0
			 0x52801290, /// 0x4a4
			 0x5b432955, /// 0x4a8
			 0x8aaa5d6d, /// 0x4ac
			 0xade7a5a7, /// 0x4b0
			 0x809e3ffc, /// 0x4b4
			 0x3ca0abb3, /// 0x4b8
			 0xaaa11bc1, /// 0x4bc
			 0xe1ef1ce, /// 0x4c0
			 0x858ccc99, /// 0x4c4
			 0xeaabe10c, /// 0x4c8
			 0x928055ac, /// 0x4cc
			 0x19ce05a5, /// 0x4d0
			 0x444f8313, /// 0x4d4
			 0xcfe76743, /// 0x4d8
			 0x329a6411, /// 0x4dc
			 0x5c814afd, /// 0x4e0
			 0xd4801e65, /// 0x4e4
			 0x9236fb71, /// 0x4e8
			 0x612f8b0b, /// 0x4ec
			 0x964992e3, /// 0x4f0
			 0x8a384e29, /// 0x4f4
			 0x4f937062, /// 0x4f8
			 0x3c29af8a, /// 0x4fc
			 0x19607b85, /// 0x500
			 0xa3845265, /// 0x504
			 0x5bca9609, /// 0x508
			 0x4c9dd962, /// 0x50c
			 0xf387b8c2, /// 0x510
			 0x9c2a87ed, /// 0x514
			 0x3c8d316b, /// 0x518
			 0x76131ce4, /// 0x51c
			 0x7ba2dc7e, /// 0x520
			 0xf09d7727, /// 0x524
			 0xcf91e50f, /// 0x528
			 0x60f058a0, /// 0x52c
			 0x4eb7dea1, /// 0x530
			 0xb8eb8ef8, /// 0x534
			 0x40b92b4e, /// 0x538
			 0x20378459, /// 0x53c
			 0xf4d844bd, /// 0x540
			 0x15a11b22, /// 0x544
			 0x4d183e71, /// 0x548
			 0x2d10d9d6, /// 0x54c
			 0x4e25d1b7, /// 0x550
			 0x85181f4, /// 0x554
			 0xa220f653, /// 0x558
			 0x144f2f8e, /// 0x55c
			 0x175393b2, /// 0x560
			 0x233a2336, /// 0x564
			 0xcde1f897, /// 0x568
			 0x8ddd9e6c, /// 0x56c
			 0x633c4d8d, /// 0x570
			 0x2da7163b, /// 0x574
			 0x9dde7d16, /// 0x578
			 0x98990ebb, /// 0x57c
			 0x4c44efae, /// 0x580
			 0xbd167159, /// 0x584
			 0x35b73354, /// 0x588
			 0x3bf4eb17, /// 0x58c
			 0xcbbe1b7e, /// 0x590
			 0x61e545fa, /// 0x594
			 0x1619cae3, /// 0x598
			 0x4736e1, /// 0x59c
			 0x93a75185, /// 0x5a0
			 0x7e563e31, /// 0x5a4
			 0xe24bc05f, /// 0x5a8
			 0x839b7266, /// 0x5ac
			 0x7e90e33a, /// 0x5b0
			 0x9ff7c764, /// 0x5b4
			 0xd155fd5a, /// 0x5b8
			 0xa8334234, /// 0x5bc
			 0xc84a10fb, /// 0x5c0
			 0x1e0c4045, /// 0x5c4
			 0x6346b0fa, /// 0x5c8
			 0xe61b671d, /// 0x5cc
			 0x5bd66c6b, /// 0x5d0
			 0xbbfcc4b6, /// 0x5d4
			 0x68e1ea41, /// 0x5d8
			 0x2d1c57db, /// 0x5dc
			 0x5e0aaeed, /// 0x5e0
			 0x904e9c6a, /// 0x5e4
			 0x594c31fc, /// 0x5e8
			 0x96360a33, /// 0x5ec
			 0x6d103c39, /// 0x5f0
			 0x46c737ab, /// 0x5f4
			 0x5762bc70, /// 0x5f8
			 0xe5ee33b4, /// 0x5fc
			 0xe96ab399, /// 0x600
			 0xad424c03, /// 0x604
			 0x76f443ea, /// 0x608
			 0xbfe0223e, /// 0x60c
			 0xd92d62e5, /// 0x610
			 0x2c42110e, /// 0x614
			 0xbe3fe244, /// 0x618
			 0x214dfbc8, /// 0x61c
			 0x721dcf3, /// 0x620
			 0x752df53, /// 0x624
			 0x7d8c0b82, /// 0x628
			 0xb5a12a8a, /// 0x62c
			 0x456872bd, /// 0x630
			 0x33077281, /// 0x634
			 0xc3627086, /// 0x638
			 0xa0f4baac, /// 0x63c
			 0x5f333a7e, /// 0x640
			 0x425adb3a, /// 0x644
			 0xd7e8cfef, /// 0x648
			 0x44eb30a9, /// 0x64c
			 0xc39d848c, /// 0x650
			 0x9be04b32, /// 0x654
			 0xc580571, /// 0x658
			 0x7d3a7573, /// 0x65c
			 0xa0ac46c, /// 0x660
			 0x93508caa, /// 0x664
			 0x8f375fee, /// 0x668
			 0x564e6a2b, /// 0x66c
			 0x69dc944a, /// 0x670
			 0xb8230a3, /// 0x674
			 0x37c56b23, /// 0x678
			 0x47c66a0f, /// 0x67c
			 0x63e5ffcf, /// 0x680
			 0x44103858, /// 0x684
			 0x1b5b7b, /// 0x688
			 0x1d1b4678, /// 0x68c
			 0x13a9ae27, /// 0x690
			 0x85f20e1d, /// 0x694
			 0x97db1d8c, /// 0x698
			 0xc097a70b, /// 0x69c
			 0xd3bfe3f, /// 0x6a0
			 0x480b3aca, /// 0x6a4
			 0xbace881e, /// 0x6a8
			 0x33bf5495, /// 0x6ac
			 0xd43f7dfb, /// 0x6b0
			 0x70ea0326, /// 0x6b4
			 0x716a6586, /// 0x6b8
			 0xce7ae982, /// 0x6bc
			 0x91b1f0e6, /// 0x6c0
			 0x9093d123, /// 0x6c4
			 0x9e29682b, /// 0x6c8
			 0x6bef359b, /// 0x6cc
			 0x559c765b, /// 0x6d0
			 0x5fc46346, /// 0x6d4
			 0x8b43266b, /// 0x6d8
			 0x5af48a61, /// 0x6dc
			 0x360bf5c8, /// 0x6e0
			 0xf03f3b82, /// 0x6e4
			 0x6235ea86, /// 0x6e8
			 0xd59a6176, /// 0x6ec
			 0x8eeeab7d, /// 0x6f0
			 0x67a40d8d, /// 0x6f4
			 0x4bc249f5, /// 0x6f8
			 0xc9b1e04b, /// 0x6fc
			 0x4e025330, /// 0x700
			 0x34946a61, /// 0x704
			 0x8d71811f, /// 0x708
			 0x4411bcf8, /// 0x70c
			 0x1bb346e6, /// 0x710
			 0xf9939e1c, /// 0x714
			 0x9104c388, /// 0x718
			 0xb3bfe26b, /// 0x71c
			 0x5f473025, /// 0x720
			 0x6444eefb, /// 0x724
			 0xa1735a7a, /// 0x728
			 0xf62eaee4, /// 0x72c
			 0x883b0768, /// 0x730
			 0xc06243b8, /// 0x734
			 0x34630310, /// 0x738
			 0x34f7910a, /// 0x73c
			 0x29bfa35b, /// 0x740
			 0xaae3f27f, /// 0x744
			 0xe9cd64b, /// 0x748
			 0xb5e7e2cb, /// 0x74c
			 0x46306987, /// 0x750
			 0x3045d61b, /// 0x754
			 0x18441eb7, /// 0x758
			 0xf26b912c, /// 0x75c
			 0xf23f2a6b, /// 0x760
			 0x3251e674, /// 0x764
			 0x714e78fe, /// 0x768
			 0x21e68bbf, /// 0x76c
			 0xe282e6bf, /// 0x770
			 0x103d9536, /// 0x774
			 0xa0774d37, /// 0x778
			 0x6216311a, /// 0x77c
			 0x9096f375, /// 0x780
			 0x27dfd88e, /// 0x784
			 0x2b552cfa, /// 0x788
			 0x28aeb23f, /// 0x78c
			 0x80f6d4eb, /// 0x790
			 0x209d586d, /// 0x794
			 0x234e6864, /// 0x798
			 0xd838c971, /// 0x79c
			 0xdf437b55, /// 0x7a0
			 0x6f2e2748, /// 0x7a4
			 0x7f69c318, /// 0x7a8
			 0x97968ec8, /// 0x7ac
			 0xe461c6d, /// 0x7b0
			 0x97bc4a11, /// 0x7b4
			 0xd5ce1dd, /// 0x7b8
			 0xdd88e276, /// 0x7bc
			 0xac65fb78, /// 0x7c0
			 0xf5d1228e, /// 0x7c4
			 0x33ac94b2, /// 0x7c8
			 0x1e29f404, /// 0x7cc
			 0x7c27f919, /// 0x7d0
			 0x40622186, /// 0x7d4
			 0x813b2b72, /// 0x7d8
			 0xb739d5e9, /// 0x7dc
			 0x926765b0, /// 0x7e0
			 0xcf2218c2, /// 0x7e4
			 0x2f03c442, /// 0x7e8
			 0xd0a7f2b5, /// 0x7ec
			 0x481eee43, /// 0x7f0
			 0x267f647, /// 0x7f4
			 0x18685dc4, /// 0x7f8
			 0xd8651382, /// 0x7fc
			 0xd75fae49, /// 0x800
			 0x4e7571f9, /// 0x804
			 0xe0b7d522, /// 0x808
			 0xe15038a0, /// 0x80c
			 0x77985ddb, /// 0x810
			 0x6100bf66, /// 0x814
			 0xdebeda44, /// 0x818
			 0xeebc6db4, /// 0x81c
			 0xdb689bbe, /// 0x820
			 0x269bcc10, /// 0x824
			 0x94f407c2, /// 0x828
			 0xcdc44283, /// 0x82c
			 0xfb281217, /// 0x830
			 0xeaf41385, /// 0x834
			 0x77de3e89, /// 0x838
			 0xa2746d63, /// 0x83c
			 0xec82515f, /// 0x840
			 0xd073d660, /// 0x844
			 0xdb4d0e1e, /// 0x848
			 0x7d697567, /// 0x84c
			 0xb35e3035, /// 0x850
			 0x96d3415b, /// 0x854
			 0xd81de377, /// 0x858
			 0xbfc560fe, /// 0x85c
			 0x1f1446e5, /// 0x860
			 0xbdbb508f, /// 0x864
			 0xdd91ca1b, /// 0x868
			 0xd33ce8c0, /// 0x86c
			 0x5ce75674, /// 0x870
			 0x24ef3cbb, /// 0x874
			 0x19bead48, /// 0x878
			 0x35c204eb, /// 0x87c
			 0x8a00d87b, /// 0x880
			 0x6f3a93d9, /// 0x884
			 0xa9e7d286, /// 0x888
			 0xd8b0d7d4, /// 0x88c
			 0x3723a65f, /// 0x890
			 0x940efddb, /// 0x894
			 0xd06f31b6, /// 0x898
			 0xd64e0f9, /// 0x89c
			 0x701f522e, /// 0x8a0
			 0x92c6c3e6, /// 0x8a4
			 0xeeb21e6b, /// 0x8a8
			 0x8155024f, /// 0x8ac
			 0x9be0e5eb, /// 0x8b0
			 0xf3389b08, /// 0x8b4
			 0xef95585b, /// 0x8b8
			 0x660f2dd3, /// 0x8bc
			 0x4ad4c746, /// 0x8c0
			 0xac88bd34, /// 0x8c4
			 0x9454130a, /// 0x8c8
			 0x933f2df9, /// 0x8cc
			 0x8d077067, /// 0x8d0
			 0xdd574a45, /// 0x8d4
			 0xcb4fd0eb, /// 0x8d8
			 0x2bef1f6a, /// 0x8dc
			 0x7619de9d, /// 0x8e0
			 0x51021a3a, /// 0x8e4
			 0x7afa7e79, /// 0x8e8
			 0x36c319a7, /// 0x8ec
			 0xb61a58d2, /// 0x8f0
			 0x89aa9927, /// 0x8f4
			 0x3b8cc9bc, /// 0x8f8
			 0xdf1c0040, /// 0x8fc
			 0x4a403a54, /// 0x900
			 0xf934bd0b, /// 0x904
			 0xc45d5054, /// 0x908
			 0xe76bbce1, /// 0x90c
			 0x2b243da5, /// 0x910
			 0x4e2e8277, /// 0x914
			 0x79eb2680, /// 0x918
			 0xf80e2a3e, /// 0x91c
			 0xcdb0f8a5, /// 0x920
			 0x569244be, /// 0x924
			 0x6336efed, /// 0x928
			 0x307d2826, /// 0x92c
			 0xe757673f, /// 0x930
			 0x10b7d041, /// 0x934
			 0x3176977c, /// 0x938
			 0x4f274912, /// 0x93c
			 0x765e85a5, /// 0x940
			 0x4843663b, /// 0x944
			 0xdea77064, /// 0x948
			 0xfb912da5, /// 0x94c
			 0x98d1702c, /// 0x950
			 0x13e3242a, /// 0x954
			 0x36c546a8, /// 0x958
			 0x310c2458, /// 0x95c
			 0x3d551605, /// 0x960
			 0xb9a000f7, /// 0x964
			 0xdd1db8b2, /// 0x968
			 0x1f9b51d0, /// 0x96c
			 0x614cbd6, /// 0x970
			 0xaca1533e, /// 0x974
			 0xd72f7a9c, /// 0x978
			 0x2b7919b0, /// 0x97c
			 0x6b7ddb7d, /// 0x980
			 0x37cdf744, /// 0x984
			 0xf1c3d40f, /// 0x988
			 0x8900be00, /// 0x98c
			 0x330f915d, /// 0x990
			 0xd3b6aaa8, /// 0x994
			 0xc7b2bc42, /// 0x998
			 0x6d9a6dc4, /// 0x99c
			 0xea37e2b3, /// 0x9a0
			 0x4fd34629, /// 0x9a4
			 0xf4c7d6f, /// 0x9a8
			 0xfbec4275, /// 0x9ac
			 0x7a0dd269, /// 0x9b0
			 0x73eb02c4, /// 0x9b4
			 0x30461a91, /// 0x9b8
			 0x137fb1a7, /// 0x9bc
			 0xe1b5ce1c, /// 0x9c0
			 0x44079eb0, /// 0x9c4
			 0x63eca9f8, /// 0x9c8
			 0x6a2857d5, /// 0x9cc
			 0xb4ee307d, /// 0x9d0
			 0x2632d483, /// 0x9d4
			 0x5c8190d, /// 0x9d8
			 0x181fec48, /// 0x9dc
			 0xe9f5dd05, /// 0x9e0
			 0x386fd8a5, /// 0x9e4
			 0x101cdf44, /// 0x9e8
			 0xd05e52ea, /// 0x9ec
			 0x3ef9caa1, /// 0x9f0
			 0xb3906cb0, /// 0x9f4
			 0xdd0ee267, /// 0x9f8
			 0x3a4fff0e, /// 0x9fc
			 0x981687a6, /// 0xa00
			 0x349e2742, /// 0xa04
			 0xd9c97337, /// 0xa08
			 0xcc2390bd, /// 0xa0c
			 0x4b844fa, /// 0xa10
			 0xbf181d24, /// 0xa14
			 0x187d716f, /// 0xa18
			 0xc46791c0, /// 0xa1c
			 0x7f649124, /// 0xa20
			 0xf444f372, /// 0xa24
			 0x57a43d38, /// 0xa28
			 0xa6e14562, /// 0xa2c
			 0x2006b8b4, /// 0xa30
			 0x7449dda0, /// 0xa34
			 0x6a406d60, /// 0xa38
			 0x77b583bd, /// 0xa3c
			 0x26f3ee98, /// 0xa40
			 0xea4f5067, /// 0xa44
			 0x478789c1, /// 0xa48
			 0x5ef59664, /// 0xa4c
			 0xa3360cd, /// 0xa50
			 0x2a8932b2, /// 0xa54
			 0xce666cf1, /// 0xa58
			 0x4e2bdb24, /// 0xa5c
			 0xfb9a8f2d, /// 0xa60
			 0xac4ece32, /// 0xa64
			 0xd7ea250, /// 0xa68
			 0x82a8f248, /// 0xa6c
			 0x80adaf3b, /// 0xa70
			 0xbcfda413, /// 0xa74
			 0x94b9fc64, /// 0xa78
			 0x421a2251, /// 0xa7c
			 0x497e08f0, /// 0xa80
			 0xbdcb4d6b, /// 0xa84
			 0x4270dce1, /// 0xa88
			 0x85ca6a79, /// 0xa8c
			 0x3f85756c, /// 0xa90
			 0x51207683, /// 0xa94
			 0x3fa1cb97, /// 0xa98
			 0x38160140, /// 0xa9c
			 0xcf1c71d1, /// 0xaa0
			 0xfd8a260d, /// 0xaa4
			 0x5ce174ff, /// 0xaa8
			 0x944ea451, /// 0xaac
			 0xaf8443ba, /// 0xab0
			 0x2ec7a599, /// 0xab4
			 0x955d52ce, /// 0xab8
			 0xfa520a06, /// 0xabc
			 0x60fab527, /// 0xac0
			 0xb87d67c2, /// 0xac4
			 0x5f2350b4, /// 0xac8
			 0x73d3e8bd, /// 0xacc
			 0xeaa3d357, /// 0xad0
			 0xa8b87035, /// 0xad4
			 0xe5e57b01, /// 0xad8
			 0x6b8604c8, /// 0xadc
			 0x48706352, /// 0xae0
			 0x9d1e0719, /// 0xae4
			 0x8910fb, /// 0xae8
			 0x86d68eea, /// 0xaec
			 0xa6a16535, /// 0xaf0
			 0x15c007ad, /// 0xaf4
			 0xa199bf8e, /// 0xaf8
			 0x4740d5c8, /// 0xafc
			 0xf059cc5a, /// 0xb00
			 0xddfa0810, /// 0xb04
			 0xec67e59, /// 0xb08
			 0x7eb97cd0, /// 0xb0c
			 0x5b4d06b9, /// 0xb10
			 0x4407effd, /// 0xb14
			 0xcd4c4f26, /// 0xb18
			 0x9ae8e67d, /// 0xb1c
			 0x8779bd67, /// 0xb20
			 0xc68b0621, /// 0xb24
			 0x2fc8d361, /// 0xb28
			 0xf30c021d, /// 0xb2c
			 0xa8212bec, /// 0xb30
			 0x3c2b90f6, /// 0xb34
			 0x3cd7e75e, /// 0xb38
			 0x52211456, /// 0xb3c
			 0xabe695ea, /// 0xb40
			 0x70247156, /// 0xb44
			 0x383c62fe, /// 0xb48
			 0xaaff3033, /// 0xb4c
			 0x4dd38cf2, /// 0xb50
			 0x6bf24fe7, /// 0xb54
			 0xa5c2560a, /// 0xb58
			 0x8274f0c3, /// 0xb5c
			 0xc91afc57, /// 0xb60
			 0x7b8bd071, /// 0xb64
			 0xdee44698, /// 0xb68
			 0x689b9525, /// 0xb6c
			 0x368383e4, /// 0xb70
			 0x21fb3339, /// 0xb74
			 0xafd320dc, /// 0xb78
			 0x5f8bd607, /// 0xb7c
			 0xf13c5cec, /// 0xb80
			 0x48945f21, /// 0xb84
			 0x7701ab59, /// 0xb88
			 0x6f43a9db, /// 0xb8c
			 0xc059e1ad, /// 0xb90
			 0x3601fe2e, /// 0xb94
			 0x702ec25c, /// 0xb98
			 0xde96de98, /// 0xb9c
			 0xf3ac0a47, /// 0xba0
			 0xdb288d0a, /// 0xba4
			 0xf6507fb6, /// 0xba8
			 0x88d93e22, /// 0xbac
			 0x4cc40375, /// 0xbb0
			 0x3f1d1a66, /// 0xbb4
			 0x3769d37e, /// 0xbb8
			 0x47d033a5, /// 0xbbc
			 0x93567189, /// 0xbc0
			 0x6af370bc, /// 0xbc4
			 0x55a42c98, /// 0xbc8
			 0xc6a5e13, /// 0xbcc
			 0xa4a79f64, /// 0xbd0
			 0xeaecec31, /// 0xbd4
			 0x72245c28, /// 0xbd8
			 0xebc07dd6, /// 0xbdc
			 0x4425db8, /// 0xbe0
			 0x73e54c57, /// 0xbe4
			 0x3f252af9, /// 0xbe8
			 0x7a5cfc93, /// 0xbec
			 0xe870cb4e, /// 0xbf0
			 0x7ca0fcda, /// 0xbf4
			 0xdda95afb, /// 0xbf8
			 0xebd42f65, /// 0xbfc
			 0xa180bd9d, /// 0xc00
			 0x1705e79, /// 0xc04
			 0xc5b70588, /// 0xc08
			 0x7ccbfd7e, /// 0xc0c
			 0x3d59f037, /// 0xc10
			 0xa9b6d536, /// 0xc14
			 0xcc8b7366, /// 0xc18
			 0x359c598a, /// 0xc1c
			 0x7a71e895, /// 0xc20
			 0x46440025, /// 0xc24
			 0x699670f, /// 0xc28
			 0x70e8bf73, /// 0xc2c
			 0x5fca20d9, /// 0xc30
			 0xb133b659, /// 0xc34
			 0xfa206a3a, /// 0xc38
			 0x87100c67, /// 0xc3c
			 0x5958bce3, /// 0xc40
			 0x20a2a9d3, /// 0xc44
			 0xb3737991, /// 0xc48
			 0xabf54246, /// 0xc4c
			 0xae200bd, /// 0xc50
			 0xd55c0e15, /// 0xc54
			 0x954038fe, /// 0xc58
			 0x85add95f, /// 0xc5c
			 0xb30673ee, /// 0xc60
			 0xfcb38415, /// 0xc64
			 0xa81e68cc, /// 0xc68
			 0x3c9c7d56, /// 0xc6c
			 0xc70545aa, /// 0xc70
			 0xb7fa2395, /// 0xc74
			 0x45c42dd9, /// 0xc78
			 0x5879ec54, /// 0xc7c
			 0x3384fa96, /// 0xc80
			 0x8f11a33, /// 0xc84
			 0x94973fe6, /// 0xc88
			 0x8fc7b6be, /// 0xc8c
			 0x579e3f06, /// 0xc90
			 0xa5650ccb, /// 0xc94
			 0x15abd30e, /// 0xc98
			 0x273eb370, /// 0xc9c
			 0x6ba7ea5a, /// 0xca0
			 0x2e25627c, /// 0xca4
			 0x103e7a2d, /// 0xca8
			 0x5691e9f7, /// 0xcac
			 0x425b074a, /// 0xcb0
			 0x212a0c8e, /// 0xcb4
			 0xd83aba3e, /// 0xcb8
			 0x9b701fbe, /// 0xcbc
			 0xbcbdca9e, /// 0xcc0
			 0x45534ab5, /// 0xcc4
			 0x16666f23, /// 0xcc8
			 0xd66beb3a, /// 0xccc
			 0xc661379e, /// 0xcd0
			 0x25cb934e, /// 0xcd4
			 0x578ffb15, /// 0xcd8
			 0x62b5423b, /// 0xcdc
			 0xfd43e574, /// 0xce0
			 0x1f962317, /// 0xce4
			 0x5919a1f1, /// 0xce8
			 0x15cfb96b, /// 0xcec
			 0x609bf25c, /// 0xcf0
			 0xd3323d25, /// 0xcf4
			 0x81ff6597, /// 0xcf8
			 0x43099dc4, /// 0xcfc
			 0xf288d137, /// 0xd00
			 0x60694732, /// 0xd04
			 0x2289bf9f, /// 0xd08
			 0x72a8bcbb, /// 0xd0c
			 0x8fab17ed, /// 0xd10
			 0xbe7a5a73, /// 0xd14
			 0x4f021fbb, /// 0xd18
			 0xdad4d998, /// 0xd1c
			 0xa51101d1, /// 0xd20
			 0x2b8daae7, /// 0xd24
			 0x7432d8ba, /// 0xd28
			 0x1e82d527, /// 0xd2c
			 0xa4d5b46c, /// 0xd30
			 0xef2c9359, /// 0xd34
			 0xf9353da5, /// 0xd38
			 0x6c586eb, /// 0xd3c
			 0xa30ac933, /// 0xd40
			 0x21e6e00c, /// 0xd44
			 0xf1ceef66, /// 0xd48
			 0xce847866, /// 0xd4c
			 0x81774d0b, /// 0xd50
			 0x98473dac, /// 0xd54
			 0x662567a5, /// 0xd58
			 0x2f0535a6, /// 0xd5c
			 0x54d927f8, /// 0xd60
			 0x87b70134, /// 0xd64
			 0x4b6c895, /// 0xd68
			 0x67ef4790, /// 0xd6c
			 0x1229ecb1, /// 0xd70
			 0xddc8bd89, /// 0xd74
			 0x828c3c01, /// 0xd78
			 0x348b57d5, /// 0xd7c
			 0xde6d58f5, /// 0xd80
			 0x6e94516c, /// 0xd84
			 0xa8efebea, /// 0xd88
			 0x690d52, /// 0xd8c
			 0xe6c94d06, /// 0xd90
			 0xa9e80f1e, /// 0xd94
			 0x45ed5c15, /// 0xd98
			 0x76c11f3, /// 0xd9c
			 0x2fc0a687, /// 0xda0
			 0xf5d547a0, /// 0xda4
			 0x36924219, /// 0xda8
			 0x1e874a7f, /// 0xdac
			 0x13799c0b, /// 0xdb0
			 0xfe9c2de5, /// 0xdb4
			 0xf96cb2bd, /// 0xdb8
			 0xe4da9ab5, /// 0xdbc
			 0xee7180b6, /// 0xdc0
			 0x73b752c7, /// 0xdc4
			 0x532f0b26, /// 0xdc8
			 0x5fe2910a, /// 0xdcc
			 0x3c53cba2, /// 0xdd0
			 0x9e65348, /// 0xdd4
			 0x9ed16258, /// 0xdd8
			 0x6faa3ee2, /// 0xddc
			 0xd0e6a4f7, /// 0xde0
			 0x7668e0ad, /// 0xde4
			 0xc7ac67ed, /// 0xde8
			 0xaa6f564a, /// 0xdec
			 0xf545aa1a, /// 0xdf0
			 0xcc115d1e, /// 0xdf4
			 0xf10c264f, /// 0xdf8
			 0x7f416c66, /// 0xdfc
			 0xffd21227, /// 0xe00
			 0xbeccfeb5, /// 0xe04
			 0xff4e6638, /// 0xe08
			 0x4120d65d, /// 0xe0c
			 0x8b8b046a, /// 0xe10
			 0x4d943785, /// 0xe14
			 0x7080de1b, /// 0xe18
			 0x754352ba, /// 0xe1c
			 0x1419c0e3, /// 0xe20
			 0x11db4985, /// 0xe24
			 0x30060bd5, /// 0xe28
			 0xd187af34, /// 0xe2c
			 0x74931418, /// 0xe30
			 0xe5eb2b17, /// 0xe34
			 0xdcc72226, /// 0xe38
			 0x78e4ba68, /// 0xe3c
			 0x1e9d0f5a, /// 0xe40
			 0x1f7d82f8, /// 0xe44
			 0xef79c9b4, /// 0xe48
			 0xcb853428, /// 0xe4c
			 0x81e55f36, /// 0xe50
			 0x94dd30a7, /// 0xe54
			 0xbf300a43, /// 0xe58
			 0xd71fb196, /// 0xe5c
			 0x5b4ba758, /// 0xe60
			 0x7d1a76c8, /// 0xe64
			 0x7e1edd30, /// 0xe68
			 0x83f119f7, /// 0xe6c
			 0xeb443dd6, /// 0xe70
			 0x59da486b, /// 0xe74
			 0xbd29c9d6, /// 0xe78
			 0xf8d69a4, /// 0xe7c
			 0xf2b335ea, /// 0xe80
			 0x9cb799c6, /// 0xe84
			 0x8b77408b, /// 0xe88
			 0x8cad0def, /// 0xe8c
			 0x3239641, /// 0xe90
			 0x8416df5, /// 0xe94
			 0x78f4da39, /// 0xe98
			 0xad99ad18, /// 0xe9c
			 0x395efd1, /// 0xea0
			 0x6907f5e1, /// 0xea4
			 0x3cdf8503, /// 0xea8
			 0x7313b43c, /// 0xeac
			 0x4d0662ee, /// 0xeb0
			 0x1e748d70, /// 0xeb4
			 0xa62885cf, /// 0xeb8
			 0x36c3ae9c, /// 0xebc
			 0xa6e560da, /// 0xec0
			 0x1a6b8142, /// 0xec4
			 0xb3bed6b1, /// 0xec8
			 0xb44a7b9d, /// 0xecc
			 0x374bfa88, /// 0xed0
			 0x64c0b666, /// 0xed4
			 0x787f2279, /// 0xed8
			 0xd1fe491f, /// 0xedc
			 0xc0d0f279, /// 0xee0
			 0x6016867e, /// 0xee4
			 0x5cf46d03, /// 0xee8
			 0x9059b52f, /// 0xeec
			 0xa2f73ef6, /// 0xef0
			 0x89eff5fb, /// 0xef4
			 0x7f7d11eb, /// 0xef8
			 0x43179858, /// 0xefc
			 0xd87ff87d, /// 0xf00
			 0x936560b4, /// 0xf04
			 0x44166042, /// 0xf08
			 0x7fe176c6, /// 0xf0c
			 0xd4ebadd7, /// 0xf10
			 0xd3fe133b, /// 0xf14
			 0x839af5a0, /// 0xf18
			 0x20387354, /// 0xf1c
			 0xdaf7ee54, /// 0xf20
			 0xeed22e1a, /// 0xf24
			 0x86f33e49, /// 0xf28
			 0xae08d83, /// 0xf2c
			 0x46e3fc81, /// 0xf30
			 0x8dd425df, /// 0xf34
			 0xdda814d2, /// 0xf38
			 0x78147c48, /// 0xf3c
			 0xac28727f, /// 0xf40
			 0xf76ce710, /// 0xf44
			 0x55aa5d7a, /// 0xf48
			 0x1f20b4e6, /// 0xf4c
			 0x4f77a62, /// 0xf50
			 0x12e0a804, /// 0xf54
			 0x60ab097c, /// 0xf58
			 0x731c81f5, /// 0xf5c
			 0x4b6a3038, /// 0xf60
			 0x13dbe795, /// 0xf64
			 0x9b5e58ef, /// 0xf68
			 0x6562d7e5, /// 0xf6c
			 0xe996b974, /// 0xf70
			 0xa1a55bf8, /// 0xf74
			 0x7820669c, /// 0xf78
			 0xde975bdb, /// 0xf7c
			 0x15c64608, /// 0xf80
			 0x788c5844, /// 0xf84
			 0x56265f86, /// 0xf88
			 0xacc31007, /// 0xf8c
			 0x9cc90c9c, /// 0xf90
			 0x8893bbc9, /// 0xf94
			 0x9addffea, /// 0xf98
			 0x916a2ded, /// 0xf9c
			 0x93607c95, /// 0xfa0
			 0x3c713f6d, /// 0xfa4
			 0xf95f8cf5, /// 0xfa8
			 0x2c7936e4, /// 0xfac
			 0xce48a04c, /// 0xfb0
			 0x2569d398, /// 0xfb4
			 0x93bf3698, /// 0xfb8
			 0xc85021d6, /// 0xfbc
			 0x993cfde6, /// 0xfc0
			 0xea64795e, /// 0xfc4
			 0xad0d61aa, /// 0xfc8
			 0xa5479bbc, /// 0xfcc
			 0xb225ecf8, /// 0xfd0
			 0x9262336e, /// 0xfd4
			 0xc85ae409, /// 0xfd8
			 0xfac72077, /// 0xfdc
			 0xb3f5dde1, /// 0xfe0
			 0x3532826d, /// 0xfe4
			 0xd804af62, /// 0xfe8
			 0x86d7493a, /// 0xfec
			 0x7bd31dc9, /// 0xff0
			 0x69c24154, /// 0xff4
			 0xe5c32bfb, /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x770ee150, /// 0x0
			 0x4e015972, /// 0x4
			 0x5e1bf303, /// 0x8
			 0x5f8ca58f, /// 0xc
			 0x64d16fef, /// 0x10
			 0x23feb118, /// 0x14
			 0x2b9a8f01, /// 0x18
			 0xfaa16829, /// 0x1c
			 0xb2bb5327, /// 0x20
			 0xdd8050cf, /// 0x24
			 0xaa14cd0f, /// 0x28
			 0xb5c63b13, /// 0x2c
			 0xcfd29782, /// 0x30
			 0xd21c437a, /// 0x34
			 0x102bbab5, /// 0x38
			 0x78d2cd96, /// 0x3c
			 0xc8600f48, /// 0x40
			 0x664b2298, /// 0x44
			 0x7f3f79f7, /// 0x48
			 0xd534d5f3, /// 0x4c
			 0x4511bdf5, /// 0x50
			 0xa6067de2, /// 0x54
			 0x2931c14e, /// 0x58
			 0x22eef531, /// 0x5c
			 0xa0249e54, /// 0x60
			 0xbaabce4d, /// 0x64
			 0xa795efba, /// 0x68
			 0xa7ec8dbe, /// 0x6c
			 0xb5a2fc9c, /// 0x70
			 0x5a530dd9, /// 0x74
			 0x8e6383ea, /// 0x78
			 0xf906903f, /// 0x7c
			 0x303a37e0, /// 0x80
			 0x739aa611, /// 0x84
			 0xf2c287ea, /// 0x88
			 0x8b5842f, /// 0x8c
			 0xd40d6fe, /// 0x90
			 0x9417b13, /// 0x94
			 0xc080beb1, /// 0x98
			 0x370958fb, /// 0x9c
			 0xe36470f, /// 0xa0
			 0x32ee6f6c, /// 0xa4
			 0xd8c8bd8, /// 0xa8
			 0xcf862cc3, /// 0xac
			 0xc11e3991, /// 0xb0
			 0xd8ece3d3, /// 0xb4
			 0xc862be34, /// 0xb8
			 0xb04b18c7, /// 0xbc
			 0x2c3f0a32, /// 0xc0
			 0x43fd2472, /// 0xc4
			 0x324483d7, /// 0xc8
			 0x3c019a70, /// 0xcc
			 0x455d73f8, /// 0xd0
			 0x2e6431f8, /// 0xd4
			 0x3499e417, /// 0xd8
			 0x14d79b4f, /// 0xdc
			 0x49aa67d6, /// 0xe0
			 0x47d93967, /// 0xe4
			 0xb34faf39, /// 0xe8
			 0x72ef885b, /// 0xec
			 0xee9d117e, /// 0xf0
			 0xd2b5ace0, /// 0xf4
			 0xe42c18b6, /// 0xf8
			 0xb1a57146, /// 0xfc
			 0xcdf8755f, /// 0x100
			 0x24ceda61, /// 0x104
			 0x48c57e2d, /// 0x108
			 0xf58eaa06, /// 0x10c
			 0xde6ecb87, /// 0x110
			 0xa90807e9, /// 0x114
			 0x223c4743, /// 0x118
			 0xd4e4e84b, /// 0x11c
			 0x73721d1c, /// 0x120
			 0xc2dbc20a, /// 0x124
			 0xa66e52d8, /// 0x128
			 0xe73f5a86, /// 0x12c
			 0xdc2572bd, /// 0x130
			 0x7f269371, /// 0x134
			 0xe995238, /// 0x138
			 0x3f6acd4, /// 0x13c
			 0x8c5928d7, /// 0x140
			 0x854a914e, /// 0x144
			 0x3598f6dc, /// 0x148
			 0xc5321cb0, /// 0x14c
			 0x494d3a68, /// 0x150
			 0xbe654b8d, /// 0x154
			 0xe9e4124, /// 0x158
			 0xf4936701, /// 0x15c
			 0xbbf21a18, /// 0x160
			 0x649ea505, /// 0x164
			 0xab3a794, /// 0x168
			 0x672717f8, /// 0x16c
			 0xbb315168, /// 0x170
			 0x597cb6eb, /// 0x174
			 0xd81bd54a, /// 0x178
			 0xfefd134e, /// 0x17c
			 0xd4d51006, /// 0x180
			 0x4e86922a, /// 0x184
			 0xaa1f404b, /// 0x188
			 0x315ba57b, /// 0x18c
			 0xf8177455, /// 0x190
			 0x8ef77d63, /// 0x194
			 0x4f4c87af, /// 0x198
			 0x34024b2c, /// 0x19c
			 0xd8109b57, /// 0x1a0
			 0x8b23333d, /// 0x1a4
			 0x7a1e9958, /// 0x1a8
			 0x6a4066ec, /// 0x1ac
			 0x6d78da8c, /// 0x1b0
			 0x23b83471, /// 0x1b4
			 0xdee82406, /// 0x1b8
			 0xe56da8d0, /// 0x1bc
			 0x7ee2f667, /// 0x1c0
			 0xa74bacc0, /// 0x1c4
			 0x619a3788, /// 0x1c8
			 0x1597ff69, /// 0x1cc
			 0x15cab3bb, /// 0x1d0
			 0x4b269a5a, /// 0x1d4
			 0x8096235e, /// 0x1d8
			 0xa280ec82, /// 0x1dc
			 0xd2787af9, /// 0x1e0
			 0x1b6fd8aa, /// 0x1e4
			 0xca08ac67, /// 0x1e8
			 0x462c64cc, /// 0x1ec
			 0xfdea6f5c, /// 0x1f0
			 0x3e380e3, /// 0x1f4
			 0x73446c49, /// 0x1f8
			 0x28222f68, /// 0x1fc
			 0x794e4bf4, /// 0x200
			 0xd628b94, /// 0x204
			 0x2c1181c4, /// 0x208
			 0x5c12350e, /// 0x20c
			 0x6bcd95a8, /// 0x210
			 0xe035928f, /// 0x214
			 0x9ebcd3ff, /// 0x218
			 0x6cd0a2b9, /// 0x21c
			 0x491ef5d6, /// 0x220
			 0x886bac1e, /// 0x224
			 0x193f3b9e, /// 0x228
			 0x8b3b4373, /// 0x22c
			 0x6e9cf146, /// 0x230
			 0x77f41ef5, /// 0x234
			 0x50d9ba64, /// 0x238
			 0xae9a9c25, /// 0x23c
			 0x1209c1be, /// 0x240
			 0x49052e68, /// 0x244
			 0x39612030, /// 0x248
			 0x1731c365, /// 0x24c
			 0xf61b98cf, /// 0x250
			 0x310c3fd5, /// 0x254
			 0xc877a06b, /// 0x258
			 0xe5c36db9, /// 0x25c
			 0x10ec6fc7, /// 0x260
			 0x4e375adb, /// 0x264
			 0x77f44a65, /// 0x268
			 0xcd2fb8d3, /// 0x26c
			 0x609dae57, /// 0x270
			 0x9e6bebad, /// 0x274
			 0xe3cda49c, /// 0x278
			 0xb70bb229, /// 0x27c
			 0x4ddcca75, /// 0x280
			 0x57098fa2, /// 0x284
			 0x46723b59, /// 0x288
			 0xf454d0e7, /// 0x28c
			 0x4ee595a2, /// 0x290
			 0x6cb7993e, /// 0x294
			 0xbc896614, /// 0x298
			 0x3939126, /// 0x29c
			 0xb60a95c, /// 0x2a0
			 0x209f1d8b, /// 0x2a4
			 0x11801cb0, /// 0x2a8
			 0x7c840918, /// 0x2ac
			 0x34e582f, /// 0x2b0
			 0xd7573164, /// 0x2b4
			 0x82f14f56, /// 0x2b8
			 0x5471e2, /// 0x2bc
			 0xb2882d24, /// 0x2c0
			 0x71387286, /// 0x2c4
			 0x2b454738, /// 0x2c8
			 0xf13f977a, /// 0x2cc
			 0xf191dc92, /// 0x2d0
			 0x65ebb2c9, /// 0x2d4
			 0x28cea66f, /// 0x2d8
			 0x5e80d4a0, /// 0x2dc
			 0x2d83bf60, /// 0x2e0
			 0xfa67abda, /// 0x2e4
			 0x6df0f5e4, /// 0x2e8
			 0x56056480, /// 0x2ec
			 0x32319c63, /// 0x2f0
			 0x1b95fec0, /// 0x2f4
			 0xd9c17bb6, /// 0x2f8
			 0x39aee52, /// 0x2fc
			 0x97563667, /// 0x300
			 0xfb98b23, /// 0x304
			 0x266bfee2, /// 0x308
			 0x28f3f055, /// 0x30c
			 0x947faf01, /// 0x310
			 0xf9bb4eb9, /// 0x314
			 0xd04956bb, /// 0x318
			 0xfde55f8f, /// 0x31c
			 0x7a401ae4, /// 0x320
			 0x598317f0, /// 0x324
			 0x840b9423, /// 0x328
			 0x3301eeff, /// 0x32c
			 0x3dfc280e, /// 0x330
			 0x776ee836, /// 0x334
			 0x65104808, /// 0x338
			 0xa5aabaa7, /// 0x33c
			 0x9a2104ca, /// 0x340
			 0xdc074ba1, /// 0x344
			 0x9f849098, /// 0x348
			 0x289d0b1c, /// 0x34c
			 0xc3137e05, /// 0x350
			 0x1ae00bca, /// 0x354
			 0x9a2d02da, /// 0x358
			 0x5effbc0f, /// 0x35c
			 0x903514d8, /// 0x360
			 0x57b4683b, /// 0x364
			 0xc4e85081, /// 0x368
			 0xf1d648e4, /// 0x36c
			 0xcb8846cd, /// 0x370
			 0xc57346da, /// 0x374
			 0x516b0f5c, /// 0x378
			 0x396f3251, /// 0x37c
			 0xfa677dd9, /// 0x380
			 0xf19759b5, /// 0x384
			 0x6be1bdb6, /// 0x388
			 0x11b5d84, /// 0x38c
			 0x3e012a91, /// 0x390
			 0xaf87ba43, /// 0x394
			 0xb248ad46, /// 0x398
			 0x895dd3b4, /// 0x39c
			 0xe9453b5a, /// 0x3a0
			 0x8ff180ba, /// 0x3a4
			 0x161ff8bc, /// 0x3a8
			 0xe2cb3e52, /// 0x3ac
			 0x15831b14, /// 0x3b0
			 0x6e4af761, /// 0x3b4
			 0x1aabb67, /// 0x3b8
			 0xab56eebe, /// 0x3bc
			 0x69a116d4, /// 0x3c0
			 0x76e98c0, /// 0x3c4
			 0x928bd331, /// 0x3c8
			 0x56016f33, /// 0x3cc
			 0x1f2a832b, /// 0x3d0
			 0xd61a43a8, /// 0x3d4
			 0xf9608a1b, /// 0x3d8
			 0x8c1fd0ab, /// 0x3dc
			 0x873fd728, /// 0x3e0
			 0xba28f4bf, /// 0x3e4
			 0x8e9dadcc, /// 0x3e8
			 0x81cee9c1, /// 0x3ec
			 0x90fd3f35, /// 0x3f0
			 0x50373d98, /// 0x3f4
			 0x29a0f8e4, /// 0x3f8
			 0xde616e1f, /// 0x3fc
			 0xbadde580, /// 0x400
			 0xfbdd5c68, /// 0x404
			 0xe156811e, /// 0x408
			 0xfacbd4d7, /// 0x40c
			 0xf351700c, /// 0x410
			 0x7a7fce7e, /// 0x414
			 0x3e706480, /// 0x418
			 0x6b219dc6, /// 0x41c
			 0x917a9ae7, /// 0x420
			 0x296ce210, /// 0x424
			 0xb9bd98ee, /// 0x428
			 0xd970cb9a, /// 0x42c
			 0x8c00efc4, /// 0x430
			 0x8bc690c6, /// 0x434
			 0xbd9d87e7, /// 0x438
			 0x1d138b80, /// 0x43c
			 0x14a73ab5, /// 0x440
			 0x8ab1ae6e, /// 0x444
			 0x5a8274, /// 0x448
			 0xc2953e8, /// 0x44c
			 0x64284984, /// 0x450
			 0xa44973e6, /// 0x454
			 0xfae34109, /// 0x458
			 0x594f9909, /// 0x45c
			 0x83d0377, /// 0x460
			 0x71afa75, /// 0x464
			 0x23d8ca77, /// 0x468
			 0x53d07e9a, /// 0x46c
			 0x7a53e396, /// 0x470
			 0x2c832a3a, /// 0x474
			 0xd64ec737, /// 0x478
			 0xeb20fa3b, /// 0x47c
			 0xea8ec168, /// 0x480
			 0xdc28b8ff, /// 0x484
			 0x9b0ff21b, /// 0x488
			 0x6cf5633b, /// 0x48c
			 0x28401185, /// 0x490
			 0x573f261b, /// 0x494
			 0x766e2bc1, /// 0x498
			 0xf68be3a7, /// 0x49c
			 0xe3d7571d, /// 0x4a0
			 0x19c1b4c0, /// 0x4a4
			 0xf00eebe2, /// 0x4a8
			 0xce2b1899, /// 0x4ac
			 0x100b63e2, /// 0x4b0
			 0x71d01401, /// 0x4b4
			 0x11c30515, /// 0x4b8
			 0x958df169, /// 0x4bc
			 0xa7891950, /// 0x4c0
			 0xf9a60519, /// 0x4c4
			 0xee43d9cb, /// 0x4c8
			 0x54b49d07, /// 0x4cc
			 0x3b70ed5f, /// 0x4d0
			 0xde6c7468, /// 0x4d4
			 0x15b468e4, /// 0x4d8
			 0xd637ed40, /// 0x4dc
			 0x3ef61c2d, /// 0x4e0
			 0x60d83116, /// 0x4e4
			 0xe7ff8ace, /// 0x4e8
			 0x48b8bda8, /// 0x4ec
			 0x49631b92, /// 0x4f0
			 0xfc87e1f7, /// 0x4f4
			 0x614fffc4, /// 0x4f8
			 0xd95c2589, /// 0x4fc
			 0xddd052f9, /// 0x500
			 0x21d28d1e, /// 0x504
			 0xf0d75977, /// 0x508
			 0x825f8506, /// 0x50c
			 0x6d3f2fdd, /// 0x510
			 0x344228ce, /// 0x514
			 0x8067f83a, /// 0x518
			 0xe6652f6e, /// 0x51c
			 0xbb0a3d1c, /// 0x520
			 0xeddb9872, /// 0x524
			 0xf0712c5a, /// 0x528
			 0x4a841e04, /// 0x52c
			 0xac3891e8, /// 0x530
			 0xbae091d0, /// 0x534
			 0x7189eef4, /// 0x538
			 0x52bb9cf5, /// 0x53c
			 0xf3681a96, /// 0x540
			 0x3f7742f5, /// 0x544
			 0x2fc26045, /// 0x548
			 0x8df09434, /// 0x54c
			 0xf82ec854, /// 0x550
			 0x3fb3416d, /// 0x554
			 0x7acfcf7f, /// 0x558
			 0xa30321c7, /// 0x55c
			 0x1b5ec733, /// 0x560
			 0x3097a632, /// 0x564
			 0x7a8b673d, /// 0x568
			 0x8c3d4d46, /// 0x56c
			 0x32d12c65, /// 0x570
			 0xdf5384d3, /// 0x574
			 0x1efb90d5, /// 0x578
			 0x15ce46a7, /// 0x57c
			 0xf7a05661, /// 0x580
			 0x89398925, /// 0x584
			 0xe58d88bc, /// 0x588
			 0x71dce669, /// 0x58c
			 0xf52b1ebb, /// 0x590
			 0x4a100c34, /// 0x594
			 0x277a6ae4, /// 0x598
			 0x1c42f44c, /// 0x59c
			 0xf0e33b9e, /// 0x5a0
			 0x5703d71d, /// 0x5a4
			 0x21e51a4, /// 0x5a8
			 0xf89d8af8, /// 0x5ac
			 0x8cb834f0, /// 0x5b0
			 0x89ca598d, /// 0x5b4
			 0x13814761, /// 0x5b8
			 0x92d1ef4, /// 0x5bc
			 0xe106fa67, /// 0x5c0
			 0x2f0e023b, /// 0x5c4
			 0x30a853c8, /// 0x5c8
			 0xfc617622, /// 0x5cc
			 0x94292e3a, /// 0x5d0
			 0xbedb1171, /// 0x5d4
			 0xa5fbc26a, /// 0x5d8
			 0x9cf87fab, /// 0x5dc
			 0x276a5c96, /// 0x5e0
			 0xdf2014fe, /// 0x5e4
			 0x61a1e6bc, /// 0x5e8
			 0x2bf48afb, /// 0x5ec
			 0x2088e4dc, /// 0x5f0
			 0x7e9b5084, /// 0x5f4
			 0x42abe6ef, /// 0x5f8
			 0x5dc7724f, /// 0x5fc
			 0x7912bfb, /// 0x600
			 0xb59975, /// 0x604
			 0xb894fc2f, /// 0x608
			 0x85156201, /// 0x60c
			 0x636b9748, /// 0x610
			 0x66173cb8, /// 0x614
			 0x59dcc19c, /// 0x618
			 0xb890b19c, /// 0x61c
			 0xf99b4202, /// 0x620
			 0xbef94503, /// 0x624
			 0xe427b29c, /// 0x628
			 0xa6356459, /// 0x62c
			 0x18bbf390, /// 0x630
			 0x2ed1c055, /// 0x634
			 0x8dd3f31a, /// 0x638
			 0xac0163f7, /// 0x63c
			 0x801989f7, /// 0x640
			 0xb138f3c0, /// 0x644
			 0xd505b2eb, /// 0x648
			 0x26cc0f60, /// 0x64c
			 0x616c3669, /// 0x650
			 0x846222f4, /// 0x654
			 0x20092d2, /// 0x658
			 0x76644dc0, /// 0x65c
			 0xdc7d8e17, /// 0x660
			 0xae087918, /// 0x664
			 0xa4cf340e, /// 0x668
			 0x800b118a, /// 0x66c
			 0xc02c8b1a, /// 0x670
			 0x99a35fc5, /// 0x674
			 0x38e84e39, /// 0x678
			 0x3ba89a3f, /// 0x67c
			 0x8362ca48, /// 0x680
			 0x85a1238c, /// 0x684
			 0xf78959e9, /// 0x688
			 0xe59889d9, /// 0x68c
			 0x8b5dd209, /// 0x690
			 0x51ed3425, /// 0x694
			 0xbb1852d6, /// 0x698
			 0xd6429f9e, /// 0x69c
			 0x243b0848, /// 0x6a0
			 0xb375f7c4, /// 0x6a4
			 0x13f7917d, /// 0x6a8
			 0xb90fcc40, /// 0x6ac
			 0xd81650cb, /// 0x6b0
			 0x1a11dbad, /// 0x6b4
			 0xc90c068, /// 0x6b8
			 0xe50dd335, /// 0x6bc
			 0x52430a39, /// 0x6c0
			 0x633e92ef, /// 0x6c4
			 0x20bc674f, /// 0x6c8
			 0x5295a323, /// 0x6cc
			 0xe7ca6e2e, /// 0x6d0
			 0x1e61caee, /// 0x6d4
			 0x17227462, /// 0x6d8
			 0xe6621103, /// 0x6dc
			 0x851cfba, /// 0x6e0
			 0xe2082920, /// 0x6e4
			 0x60b75cd0, /// 0x6e8
			 0xbf38774b, /// 0x6ec
			 0x8ccfbe1c, /// 0x6f0
			 0xcaadf35c, /// 0x6f4
			 0x8d5d606c, /// 0x6f8
			 0xf0b1b70e, /// 0x6fc
			 0xe1d36876, /// 0x700
			 0x83073601, /// 0x704
			 0x61105258, /// 0x708
			 0xf78168c0, /// 0x70c
			 0x8389748c, /// 0x710
			 0xb1ab72ed, /// 0x714
			 0xb975681c, /// 0x718
			 0xa04d98a1, /// 0x71c
			 0xba32c27f, /// 0x720
			 0xa426f8c1, /// 0x724
			 0xa70bd321, /// 0x728
			 0x10ab3b1d, /// 0x72c
			 0xb3b2e61, /// 0x730
			 0x5310431, /// 0x734
			 0x9d0766c2, /// 0x738
			 0xded24fa2, /// 0x73c
			 0xee16e10d, /// 0x740
			 0xa0f9a3b8, /// 0x744
			 0xf618b7e1, /// 0x748
			 0x3d7d94bc, /// 0x74c
			 0x52d16949, /// 0x750
			 0xc1aad065, /// 0x754
			 0x13421b25, /// 0x758
			 0xf306a1e4, /// 0x75c
			 0xe51161ac, /// 0x760
			 0xeb3da218, /// 0x764
			 0xbefd6236, /// 0x768
			 0x4da31c83, /// 0x76c
			 0x2be22d85, /// 0x770
			 0x69477918, /// 0x774
			 0x905f7512, /// 0x778
			 0xdbe285c9, /// 0x77c
			 0x3762c6dd, /// 0x780
			 0x8845bdd1, /// 0x784
			 0x679f4b8d, /// 0x788
			 0x2b2f5d63, /// 0x78c
			 0xc45f3000, /// 0x790
			 0x9d8e01b6, /// 0x794
			 0x7074171, /// 0x798
			 0x6139ede3, /// 0x79c
			 0xd176755d, /// 0x7a0
			 0xb98cf921, /// 0x7a4
			 0xd46735ab, /// 0x7a8
			 0x13348ab4, /// 0x7ac
			 0xc2672489, /// 0x7b0
			 0xc26bc2b2, /// 0x7b4
			 0x50088b5, /// 0x7b8
			 0x40218d12, /// 0x7bc
			 0xbaeb187a, /// 0x7c0
			 0x5b267934, /// 0x7c4
			 0x9e78cc56, /// 0x7c8
			 0xb2e4abe0, /// 0x7cc
			 0x98f92b33, /// 0x7d0
			 0xdbe17cbc, /// 0x7d4
			 0xb30b9043, /// 0x7d8
			 0x12fd56a2, /// 0x7dc
			 0xbce2ebb6, /// 0x7e0
			 0x437e0f09, /// 0x7e4
			 0xb474829, /// 0x7e8
			 0xe031a2ad, /// 0x7ec
			 0x6eb19f1a, /// 0x7f0
			 0x13568ac9, /// 0x7f4
			 0x79555e74, /// 0x7f8
			 0x18a6e01a, /// 0x7fc
			 0x543ee7a3, /// 0x800
			 0x5e115fd7, /// 0x804
			 0xb134afd5, /// 0x808
			 0x91141878, /// 0x80c
			 0x50e74538, /// 0x810
			 0xf9bd12c7, /// 0x814
			 0xfbdd307c, /// 0x818
			 0x149e7240, /// 0x81c
			 0xce75c20, /// 0x820
			 0x3dad827, /// 0x824
			 0x4d585450, /// 0x828
			 0xdf1b02d1, /// 0x82c
			 0x4f44f418, /// 0x830
			 0x255ebad6, /// 0x834
			 0x892500d4, /// 0x838
			 0x1e0fa987, /// 0x83c
			 0xc6506a71, /// 0x840
			 0x80214f5e, /// 0x844
			 0x5bb8fbf7, /// 0x848
			 0x23c05970, /// 0x84c
			 0x9e380fd, /// 0x850
			 0x5bf1838a, /// 0x854
			 0x7e4c849d, /// 0x858
			 0x9328aa1, /// 0x85c
			 0xe2f3de5d, /// 0x860
			 0xa3ebec3b, /// 0x864
			 0x2c8acb46, /// 0x868
			 0x13bf069d, /// 0x86c
			 0xf046bf9d, /// 0x870
			 0xb3d470b, /// 0x874
			 0xe696e0e4, /// 0x878
			 0x92e4127, /// 0x87c
			 0x9f10b225, /// 0x880
			 0x8948f7f5, /// 0x884
			 0xffc1240d, /// 0x888
			 0xe57e7562, /// 0x88c
			 0xa8f4cc00, /// 0x890
			 0xe67d2120, /// 0x894
			 0x391aa47e, /// 0x898
			 0xac35d17d, /// 0x89c
			 0x1a0ac5a, /// 0x8a0
			 0x6ec233f, /// 0x8a4
			 0xc0845e88, /// 0x8a8
			 0x57999237, /// 0x8ac
			 0xa2ef3bbb, /// 0x8b0
			 0x57ca4649, /// 0x8b4
			 0xe53e457d, /// 0x8b8
			 0x834bc50a, /// 0x8bc
			 0x793f9d5e, /// 0x8c0
			 0xe7c3740c, /// 0x8c4
			 0x1e2174ff, /// 0x8c8
			 0x6a6c1d6a, /// 0x8cc
			 0x2ac6d602, /// 0x8d0
			 0xb7ec73fd, /// 0x8d4
			 0x710becaa, /// 0x8d8
			 0xdc444ef3, /// 0x8dc
			 0x481bba4f, /// 0x8e0
			 0x36bc2ac2, /// 0x8e4
			 0xffb5f349, /// 0x8e8
			 0x263204cc, /// 0x8ec
			 0xadbb6a42, /// 0x8f0
			 0xca15767d, /// 0x8f4
			 0x7f9978f1, /// 0x8f8
			 0x845d490, /// 0x8fc
			 0x2244c0bc, /// 0x900
			 0xac47dc31, /// 0x904
			 0xab9609f2, /// 0x908
			 0x90188191, /// 0x90c
			 0x1ec93792, /// 0x910
			 0x7ce2fa20, /// 0x914
			 0xec1a879b, /// 0x918
			 0x9c9b2a8d, /// 0x91c
			 0xda6d008d, /// 0x920
			 0x73e4b54c, /// 0x924
			 0x9d49e51e, /// 0x928
			 0x7761ca96, /// 0x92c
			 0x39b7eb82, /// 0x930
			 0x1484f6e3, /// 0x934
			 0x30d258b2, /// 0x938
			 0x5d5111e3, /// 0x93c
			 0x754c910a, /// 0x940
			 0x1f0c6274, /// 0x944
			 0x76d57c4f, /// 0x948
			 0x9d90e059, /// 0x94c
			 0xd8533e74, /// 0x950
			 0xfd47bb09, /// 0x954
			 0x5cc30cf9, /// 0x958
			 0xb560460f, /// 0x95c
			 0xb226500f, /// 0x960
			 0x7ccb25ab, /// 0x964
			 0x3ac944f4, /// 0x968
			 0xf16f1341, /// 0x96c
			 0x6c229dce, /// 0x970
			 0x7111a47, /// 0x974
			 0xf2170b45, /// 0x978
			 0xac3e8f91, /// 0x97c
			 0x46b7ad0e, /// 0x980
			 0x536d106f, /// 0x984
			 0x20211f3a, /// 0x988
			 0xa7003db3, /// 0x98c
			 0xee8547be, /// 0x990
			 0xbc0a7603, /// 0x994
			 0x446b2ff3, /// 0x998
			 0x1137c75b, /// 0x99c
			 0xfaf4e782, /// 0x9a0
			 0xd9dbf1e0, /// 0x9a4
			 0x16c75c4b, /// 0x9a8
			 0x29b6af9f, /// 0x9ac
			 0x5b72139f, /// 0x9b0
			 0xc8dae9c6, /// 0x9b4
			 0xbaca94fb, /// 0x9b8
			 0xec8c3cb1, /// 0x9bc
			 0xd807543f, /// 0x9c0
			 0xd969ad42, /// 0x9c4
			 0x43d5ec3, /// 0x9c8
			 0xace221fd, /// 0x9cc
			 0xdb75a445, /// 0x9d0
			 0xc72af462, /// 0x9d4
			 0x8564191a, /// 0x9d8
			 0x982ae4d6, /// 0x9dc
			 0x2b355b8b, /// 0x9e0
			 0xf7315a67, /// 0x9e4
			 0xa8a3fda7, /// 0x9e8
			 0x2424f8c8, /// 0x9ec
			 0x6792b90, /// 0x9f0
			 0x42e154fe, /// 0x9f4
			 0x68276816, /// 0x9f8
			 0xf17212c, /// 0x9fc
			 0x3b5f03b7, /// 0xa00
			 0x320d29ff, /// 0xa04
			 0xf9561f09, /// 0xa08
			 0x93aa3577, /// 0xa0c
			 0x8e9125d5, /// 0xa10
			 0x784264f3, /// 0xa14
			 0x1be5364, /// 0xa18
			 0x4b915837, /// 0xa1c
			 0x422e9451, /// 0xa20
			 0x46d9e890, /// 0xa24
			 0x5f1ab19c, /// 0xa28
			 0x92f2f9f7, /// 0xa2c
			 0xbf0ef861, /// 0xa30
			 0x432cafbd, /// 0xa34
			 0x5fe605d1, /// 0xa38
			 0x91b4681a, /// 0xa3c
			 0x8a3725aa, /// 0xa40
			 0x271e9756, /// 0xa44
			 0xbeb6c939, /// 0xa48
			 0xd6fdaa58, /// 0xa4c
			 0xebad5c1b, /// 0xa50
			 0x6cd3fcc1, /// 0xa54
			 0x2e2700b9, /// 0xa58
			 0x494a3ed1, /// 0xa5c
			 0x19edf943, /// 0xa60
			 0xd6652f84, /// 0xa64
			 0xfca84210, /// 0xa68
			 0xb2abf21c, /// 0xa6c
			 0x9708504f, /// 0xa70
			 0x87fa05e3, /// 0xa74
			 0xa601e274, /// 0xa78
			 0xa1bc5cd4, /// 0xa7c
			 0x97be409e, /// 0xa80
			 0xbe10e9cb, /// 0xa84
			 0x2567a96b, /// 0xa88
			 0x3c8295c1, /// 0xa8c
			 0xb96a5538, /// 0xa90
			 0x65033461, /// 0xa94
			 0xfa929716, /// 0xa98
			 0x5b2522bc, /// 0xa9c
			 0xba0d87e1, /// 0xaa0
			 0x7998f383, /// 0xaa4
			 0x5f99715e, /// 0xaa8
			 0x992033e0, /// 0xaac
			 0xb9834afc, /// 0xab0
			 0xc80ed0ca, /// 0xab4
			 0xd9096734, /// 0xab8
			 0xb5d18f2e, /// 0xabc
			 0xa62f7a9d, /// 0xac0
			 0x466631d0, /// 0xac4
			 0x43c5f4a7, /// 0xac8
			 0xaac5810, /// 0xacc
			 0x499050b7, /// 0xad0
			 0x6fe94888, /// 0xad4
			 0xdacfa86f, /// 0xad8
			 0x86bae007, /// 0xadc
			 0x9d1c4971, /// 0xae0
			 0x2d5ab27c, /// 0xae4
			 0xd5d47d9e, /// 0xae8
			 0x594a5cd3, /// 0xaec
			 0xa5e0e4ae, /// 0xaf0
			 0x279fc14d, /// 0xaf4
			 0x6ee7e985, /// 0xaf8
			 0xb196ec1f, /// 0xafc
			 0x100e9fcb, /// 0xb00
			 0x26490d6d, /// 0xb04
			 0x5d437316, /// 0xb08
			 0xd664b467, /// 0xb0c
			 0x326019c2, /// 0xb10
			 0xd4fc55ba, /// 0xb14
			 0xe673bd01, /// 0xb18
			 0x27aeae89, /// 0xb1c
			 0xc00c1c24, /// 0xb20
			 0x1d1491ed, /// 0xb24
			 0xa0d0430e, /// 0xb28
			 0x5c979f42, /// 0xb2c
			 0x61d931ba, /// 0xb30
			 0x6583aed9, /// 0xb34
			 0xc879cc3b, /// 0xb38
			 0x37e1c91a, /// 0xb3c
			 0x4808b93f, /// 0xb40
			 0xe23f5a71, /// 0xb44
			 0x9c53ef8c, /// 0xb48
			 0x78a0157c, /// 0xb4c
			 0x2449b883, /// 0xb50
			 0xfc5cddc2, /// 0xb54
			 0x8101c6, /// 0xb58
			 0xb58f8670, /// 0xb5c
			 0x120e89e, /// 0xb60
			 0x602501ee, /// 0xb64
			 0x686e1db3, /// 0xb68
			 0x9f90dbd2, /// 0xb6c
			 0x71c2abd9, /// 0xb70
			 0x988d0eda, /// 0xb74
			 0x947e7d91, /// 0xb78
			 0x5d256035, /// 0xb7c
			 0xf4f63f3, /// 0xb80
			 0x9ab4000d, /// 0xb84
			 0x547c3109, /// 0xb88
			 0x9682ff8, /// 0xb8c
			 0x61d47b6c, /// 0xb90
			 0xe54e5f0c, /// 0xb94
			 0x68a143cb, /// 0xb98
			 0x1d307d5c, /// 0xb9c
			 0xfefb2314, /// 0xba0
			 0x6d6b3718, /// 0xba4
			 0xf8dae2a4, /// 0xba8
			 0x317dc986, /// 0xbac
			 0x3dbde43b, /// 0xbb0
			 0x3ba64930, /// 0xbb4
			 0x20c92dcd, /// 0xbb8
			 0x6a6117a2, /// 0xbbc
			 0xa0ccfff0, /// 0xbc0
			 0xb19336b2, /// 0xbc4
			 0xd55a9226, /// 0xbc8
			 0xf7950cf0, /// 0xbcc
			 0xd7a5505a, /// 0xbd0
			 0x79938548, /// 0xbd4
			 0x67ef08a7, /// 0xbd8
			 0xbbe214c2, /// 0xbdc
			 0x5cd1fb4c, /// 0xbe0
			 0x12039370, /// 0xbe4
			 0xe8cd426e, /// 0xbe8
			 0x36f53fa1, /// 0xbec
			 0xba0803b3, /// 0xbf0
			 0x1e1a3cc8, /// 0xbf4
			 0xb554a916, /// 0xbf8
			 0x48a14c72, /// 0xbfc
			 0xdbc25f60, /// 0xc00
			 0xb9ad6a88, /// 0xc04
			 0x5d69817a, /// 0xc08
			 0x27d04a0b, /// 0xc0c
			 0x7a734c84, /// 0xc10
			 0xcd24847, /// 0xc14
			 0xe9d5d8f3, /// 0xc18
			 0x22bfb48d, /// 0xc1c
			 0x4c6d013e, /// 0xc20
			 0xbea3de8a, /// 0xc24
			 0x937f1ae0, /// 0xc28
			 0xf45cc8ad, /// 0xc2c
			 0x48b8391e, /// 0xc30
			 0x698fbad6, /// 0xc34
			 0x28f8c5dd, /// 0xc38
			 0xeb44b65e, /// 0xc3c
			 0xe818d7d5, /// 0xc40
			 0x5c5e9d1b, /// 0xc44
			 0xe28a9685, /// 0xc48
			 0x1e705bac, /// 0xc4c
			 0x45fe860e, /// 0xc50
			 0xf8a4f2ef, /// 0xc54
			 0x40e0e72d, /// 0xc58
			 0x32a0f74c, /// 0xc5c
			 0x79a4837b, /// 0xc60
			 0xacfd64f3, /// 0xc64
			 0x841b24fc, /// 0xc68
			 0x579a14f2, /// 0xc6c
			 0xa0cf1392, /// 0xc70
			 0x1822552a, /// 0xc74
			 0x9f8c2fa2, /// 0xc78
			 0x9998183a, /// 0xc7c
			 0x71944943, /// 0xc80
			 0x8fe4e910, /// 0xc84
			 0x8f46e5bc, /// 0xc88
			 0xa31116c0, /// 0xc8c
			 0x88e557a8, /// 0xc90
			 0x70ccb532, /// 0xc94
			 0x6afcdea9, /// 0xc98
			 0xe189e22a, /// 0xc9c
			 0x968aa144, /// 0xca0
			 0x44acae69, /// 0xca4
			 0x9dd2cc1f, /// 0xca8
			 0x3d716aa9, /// 0xcac
			 0x7993a843, /// 0xcb0
			 0xc2677fd8, /// 0xcb4
			 0x9dcee436, /// 0xcb8
			 0x5dce7fb6, /// 0xcbc
			 0xe76ede23, /// 0xcc0
			 0x3f8a2591, /// 0xcc4
			 0x155bd987, /// 0xcc8
			 0xa9d58879, /// 0xccc
			 0x5c6ab5ac, /// 0xcd0
			 0xc3ca9701, /// 0xcd4
			 0x96554b60, /// 0xcd8
			 0xbe8d4d79, /// 0xcdc
			 0xccd375ef, /// 0xce0
			 0x121eb2ba, /// 0xce4
			 0x7143b230, /// 0xce8
			 0xfb0c5151, /// 0xcec
			 0x3b746c3f, /// 0xcf0
			 0xfcd6148e, /// 0xcf4
			 0xfffd5f03, /// 0xcf8
			 0x96879873, /// 0xcfc
			 0x77c9444d, /// 0xd00
			 0xf4d7c0d6, /// 0xd04
			 0xbb75d680, /// 0xd08
			 0xa1fc3b83, /// 0xd0c
			 0x4a9c00d8, /// 0xd10
			 0x1d48e1a5, /// 0xd14
			 0x21a6c8c9, /// 0xd18
			 0xe35d84e9, /// 0xd1c
			 0xdd11d737, /// 0xd20
			 0xa1693426, /// 0xd24
			 0x9de6899, /// 0xd28
			 0x21f952c8, /// 0xd2c
			 0xdb1c20be, /// 0xd30
			 0x5e8af3b9, /// 0xd34
			 0xfaa27789, /// 0xd38
			 0x7c86b8f9, /// 0xd3c
			 0x7078a355, /// 0xd40
			 0x91e883d5, /// 0xd44
			 0x45be6961, /// 0xd48
			 0x7904405b, /// 0xd4c
			 0x4f74b2a7, /// 0xd50
			 0x555852c1, /// 0xd54
			 0x8759e3fc, /// 0xd58
			 0x8208a775, /// 0xd5c
			 0xe6a1cec2, /// 0xd60
			 0x8d8f8f0b, /// 0xd64
			 0xa7b1bf74, /// 0xd68
			 0x36981e06, /// 0xd6c
			 0x232f6552, /// 0xd70
			 0xa0acc4e5, /// 0xd74
			 0xd3c5137f, /// 0xd78
			 0x63103b56, /// 0xd7c
			 0x7351599c, /// 0xd80
			 0x42531261, /// 0xd84
			 0x80794514, /// 0xd88
			 0x9251bd75, /// 0xd8c
			 0x13b241bd, /// 0xd90
			 0x3c211a97, /// 0xd94
			 0x570eb7ed, /// 0xd98
			 0x55a4ff01, /// 0xd9c
			 0xf9ede8f1, /// 0xda0
			 0xe56d1a55, /// 0xda4
			 0x42c42d2a, /// 0xda8
			 0xfd0d9993, /// 0xdac
			 0xae7df031, /// 0xdb0
			 0xa200224a, /// 0xdb4
			 0xea9c6f5d, /// 0xdb8
			 0xced1d85d, /// 0xdbc
			 0xb16d0faa, /// 0xdc0
			 0x24670572, /// 0xdc4
			 0xa567b7ad, /// 0xdc8
			 0x74412368, /// 0xdcc
			 0xe603c186, /// 0xdd0
			 0x4f28e019, /// 0xdd4
			 0xbc85af35, /// 0xdd8
			 0xc1f26603, /// 0xddc
			 0xa19a86cd, /// 0xde0
			 0xcd58273e, /// 0xde4
			 0xf3a48dfd, /// 0xde8
			 0xb0d20509, /// 0xdec
			 0xcbeb4a8f, /// 0xdf0
			 0x5cf9a528, /// 0xdf4
			 0x4f552b1f, /// 0xdf8
			 0xea5a57ea, /// 0xdfc
			 0x26edb10e, /// 0xe00
			 0xda00325e, /// 0xe04
			 0x8245b638, /// 0xe08
			 0x63e24715, /// 0xe0c
			 0x6ba09ad7, /// 0xe10
			 0x4afd9453, /// 0xe14
			 0xf0617093, /// 0xe18
			 0x558c2920, /// 0xe1c
			 0x550cf56a, /// 0xe20
			 0x3b21cefb, /// 0xe24
			 0xd40c7593, /// 0xe28
			 0x52efcb01, /// 0xe2c
			 0xf9e68fbd, /// 0xe30
			 0xa09abeef, /// 0xe34
			 0x5cb31d25, /// 0xe38
			 0xb392c66e, /// 0xe3c
			 0x1659df1b, /// 0xe40
			 0x9f78071b, /// 0xe44
			 0xfc03fa2e, /// 0xe48
			 0x7b70397a, /// 0xe4c
			 0x174b258e, /// 0xe50
			 0x84eae440, /// 0xe54
			 0x710ba609, /// 0xe58
			 0x9e2e25df, /// 0xe5c
			 0x237b0606, /// 0xe60
			 0xef3f2f7c, /// 0xe64
			 0x733467c8, /// 0xe68
			 0x8ac03971, /// 0xe6c
			 0xa8580c31, /// 0xe70
			 0x4dd6d054, /// 0xe74
			 0x56aa30da, /// 0xe78
			 0xdac96ea7, /// 0xe7c
			 0xadf7bd30, /// 0xe80
			 0xca38935e, /// 0xe84
			 0xa4d61d38, /// 0xe88
			 0x1141578d, /// 0xe8c
			 0x6a0772b7, /// 0xe90
			 0x9c219223, /// 0xe94
			 0x851ea4df, /// 0xe98
			 0x114a1b60, /// 0xe9c
			 0x8ee94f70, /// 0xea0
			 0xc6758cfe, /// 0xea4
			 0xb39e6d0b, /// 0xea8
			 0x735a06dc, /// 0xeac
			 0xf31ea3d0, /// 0xeb0
			 0xb60990cc, /// 0xeb4
			 0xbeee031f, /// 0xeb8
			 0xb14225f5, /// 0xebc
			 0x8b1c3a90, /// 0xec0
			 0x96189ce0, /// 0xec4
			 0x5b7231d4, /// 0xec8
			 0xf8b17c3b, /// 0xecc
			 0xc77a808f, /// 0xed0
			 0xffc62345, /// 0xed4
			 0x6e8801b2, /// 0xed8
			 0xbcbed131, /// 0xedc
			 0x6fa48308, /// 0xee0
			 0x67ebaafa, /// 0xee4
			 0x6115aff8, /// 0xee8
			 0xd7e4fc1, /// 0xeec
			 0xe64f816f, /// 0xef0
			 0x3cf05d8, /// 0xef4
			 0x1486ba8b, /// 0xef8
			 0x879a2f9d, /// 0xefc
			 0xd14a8371, /// 0xf00
			 0xde76a980, /// 0xf04
			 0x57933d81, /// 0xf08
			 0x629a3b7, /// 0xf0c
			 0x4be0ee88, /// 0xf10
			 0x42036e7f, /// 0xf14
			 0xd32b6c4c, /// 0xf18
			 0xa47c39d1, /// 0xf1c
			 0xced6f721, /// 0xf20
			 0x5d739341, /// 0xf24
			 0x16dfd6b6, /// 0xf28
			 0x60ef1a46, /// 0xf2c
			 0xf506a5af, /// 0xf30
			 0xd4921222, /// 0xf34
			 0x8af705f5, /// 0xf38
			 0x8bcadb3, /// 0xf3c
			 0x2ada3a63, /// 0xf40
			 0xbd61484e, /// 0xf44
			 0xaa4cd96e, /// 0xf48
			 0x914d25a1, /// 0xf4c
			 0x5bf92b3d, /// 0xf50
			 0x22edc34a, /// 0xf54
			 0x86b0579b, /// 0xf58
			 0xddc62af9, /// 0xf5c
			 0x9d9eb898, /// 0xf60
			 0x3dc94f55, /// 0xf64
			 0xf1d4d1bb, /// 0xf68
			 0x13f3ad89, /// 0xf6c
			 0x1fc2c870, /// 0xf70
			 0x1a34274d, /// 0xf74
			 0xc28ef0a, /// 0xf78
			 0x53bf7a40, /// 0xf7c
			 0x84a8955c, /// 0xf80
			 0x532d1d30, /// 0xf84
			 0xde89fe08, /// 0xf88
			 0xe3e94423, /// 0xf8c
			 0x70639275, /// 0xf90
			 0xc38d8669, /// 0xf94
			 0xf6ee7d19, /// 0xf98
			 0x9bb147c5, /// 0xf9c
			 0x1fe2f7a8, /// 0xfa0
			 0xdbf5baca, /// 0xfa4
			 0x534beef0, /// 0xfa8
			 0x8bacd64e, /// 0xfac
			 0xc6c0a0c2, /// 0xfb0
			 0xa48c15b3, /// 0xfb4
			 0x3be4d517, /// 0xfb8
			 0x345a37d8, /// 0xfbc
			 0x658d6ae1, /// 0xfc0
			 0x63602c54, /// 0xfc4
			 0x80f0d43a, /// 0xfc8
			 0x9d5c0d6e, /// 0xfcc
			 0xa5dd5fe2, /// 0xfd0
			 0xb92e0b52, /// 0xfd4
			 0x24a9a3e6, /// 0xfd8
			 0xc98d6da7, /// 0xfdc
			 0xeffdcf7c, /// 0xfe0
			 0x363c2c2f, /// 0xfe4
			 0x9d426cd2, /// 0xfe8
			 0xbb61c57a, /// 0xfec
			 0x3ce9f5aa, /// 0xff0
			 0x607a4b27, /// 0xff4
			 0xcd04157e, /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x14
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x2c
			 0x33333333,                                                  // 4 random values                             /// 0x30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x38
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x3c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x44
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x4c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x70
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x80
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x84
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x88
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x94
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x98
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xa0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xa4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xa8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xb4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xcc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xd4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xd8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xe0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xe4
			 0xbf028f5c,                                                  // -0.51                                       /// 0xe8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xf8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xfc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x100
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x104
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x108
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x10c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x110
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x114
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x118
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x11c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x120
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x124
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x128
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x12c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x130
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x134
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x138
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x13c
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x140
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x144
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x148
			 0x3f028f5c,                                                  // 0.51                                        /// 0x14c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x150
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x154
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x158
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x15c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x160
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x164
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x168
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x16c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x170
			 0x55555555,                                                  // 4 random values                             /// 0x174
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x178
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x17c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x180
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x184
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x188
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x18c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x190
			 0xff800000,                                                  // -inf                                        /// 0x194
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x198
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x19c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x1a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x1a4
			 0x7fc00001,                                                  // signaling NaN                               /// 0x1a8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x1ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x1b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x1b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x1b8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x1bc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x1c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x1c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x1c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1cc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x1d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x1d4
			 0x55555555,                                                  // 4 random values                             /// 0x1d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x1dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x1e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x1e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x1e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x1ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x1f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x1f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x1fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x200
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x204
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x208
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x20c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x210
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x214
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x218
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x21c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x220
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x224
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x228
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x22c
			 0x3f028f5c,                                                  // 0.51                                        /// 0x230
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x234
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x238
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x23c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x240
			 0xbf028f5c,                                                  // -0.51                                       /// 0x244
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x248
			 0xbf028f5c,                                                  // -0.51                                       /// 0x24c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x250
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x254
			 0xbf028f5c,                                                  // -0.51                                       /// 0x258
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x25c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x260
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x264
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x268
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x26c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x270
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x274
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x278
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x27c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x280
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x284
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x288
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x28c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x290
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x294
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x298
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x29c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x2a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x2a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x2b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x2b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x2bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x2c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x2c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x2cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x2d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x2d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x2d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x2dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x2e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x2e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x2f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x2fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x300
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x304
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x308
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x30c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x310
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x314
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x318
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x31c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x320
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x324
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x328
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x32c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x330
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x334
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x338
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x33c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x340
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x344
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x348
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x34c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x350
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x354
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x358
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x35c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x360
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x364
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x368
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x36c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x370
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x374
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x378
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x37c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x380
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x384
			 0xcb000000,                                                  // -8388608.0                                  /// 0x388
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x38c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x390
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x394
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x398
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x39c
			 0x7f800000,                                                  // inf                                         /// 0x3a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x3a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x3ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x3b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x3b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x3b8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x3c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x3c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x3c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x3cc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x3d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x3d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x3dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x3e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x3e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3e8
			 0x7f800000,                                                  // inf                                         /// 0x3ec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x3f0
			 0x7f800000,                                                  // inf                                         /// 0x3f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x3f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x3fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x400
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x404
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x408
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x40c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x410
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x414
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x418
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x41c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x420
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x424
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x428
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x42c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x430
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x434
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x438
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x43c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x440
			 0x00000000,                                                  // zero                                        /// 0x444
			 0x80000000,                                                  // -zero                                       /// 0x448
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x44c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x450
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x454
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x458
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x45c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x460
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x464
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x468
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x46c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x470
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x474
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x478
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x47c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x480
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x484
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x488
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x48c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x490
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x494
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x498
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x49c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x4a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x4a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x4ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x4b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x4b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x4b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x4c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x4c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x4cc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x4d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x4dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x4e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x4e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x4e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x4ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x4f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x4f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x4f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x4fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x500
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x504
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x508
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x50c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x510
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x514
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x518
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x51c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x520
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x524
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x528
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x52c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x530
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x534
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x538
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x53c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x540
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x544
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x548
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x54c
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x550
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x554
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x558
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x55c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x560
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x564
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x568
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x56c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x570
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x574
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x578
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x57c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x580
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x584
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x588
			 0xffc00001,                                                  // -signaling NaN                              /// 0x58c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x590
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x594
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x598
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x59c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x5a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x5a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x5ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x5b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x5b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x5bc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x5c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x5c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x5c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x5cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x5d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5d8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x5dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x5e0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x5e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x5e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x5ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x5f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x5f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x5f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x5fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x600
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x604
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x608
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x60c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x610
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x614
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x618
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x61c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x620
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x624
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x628
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x62c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x630
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x634
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x638
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x63c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x640
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x644
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x648
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x64c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x650
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x654
			 0xcb000000,                                                  // -8388608.0                                  /// 0x658
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x65c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x660
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x664
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x668
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x66c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x670
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x674
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x678
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x67c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x680
			 0x3f028f5c,                                                  // 0.51                                        /// 0x684
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x688
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x68c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x690
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x694
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x698
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x69c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x6a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x6a4
			 0x33333333,                                                  // 4 random values                             /// 0x6a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x6ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x6b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x6b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x6b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x6bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x6c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x6c4
			 0x3f028f5c,                                                  // 0.51                                        /// 0x6c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6d0
			 0xff800000,                                                  // -inf                                        /// 0x6d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x6d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x6dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x6e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x6e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x6ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x6f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x6f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x6fc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x700
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x704
			 0x33333333,                                                  // 4 random values                             /// 0x708
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x70c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x710
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x714
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x718
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x71c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x720
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x724
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x728
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x72c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x730
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x734
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x738
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x73c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x740
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x744
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x748
			 0x55555555,                                                  // 4 random values                             /// 0x74c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x750
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x754
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x758
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x75c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x760
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x764
			 0xff800000,                                                  // -inf                                        /// 0x768
			 0x80000000,                                                  // -zero                                       /// 0x76c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x770
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x774
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x778
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x77c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x780
			 0x55555555,                                                  // 4 random values                             /// 0x784
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x788
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x78c
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x790
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x794
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x798
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x79c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x7a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x7a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x7ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x7b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x7b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x7bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7c0
			 0xffc00001,                                                  // -signaling NaN                              /// 0x7c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x7c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x7cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x7d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x7d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x7dc
			 0x7fc00001,                                                  // signaling NaN                               /// 0x7e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x7e4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x7ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x7f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x7f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x800
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x804
			 0x55555555,                                                  // 4 random values                             /// 0x808
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x80c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x810
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x814
			 0xbf028f5c,                                                  // -0.51                                       /// 0x818
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x81c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x820
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x824
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x828
			 0xbf028f5c,                                                  // -0.51                                       /// 0x82c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x830
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x834
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x838
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x83c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x840
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x844
			 0xcb000000,                                                  // -8388608.0                                  /// 0x848
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x84c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x850
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x854
			 0x4b000000,                                                  // 8388608.0                                   /// 0x858
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x85c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x860
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x864
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x868
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x86c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x870
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x874
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x878
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x87c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x880
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x884
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x888
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x88c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x890
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x894
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x898
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x89c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x8a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x8a4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x8a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x8ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x8b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x8b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x8b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x8bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x8c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x8c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x8c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x8cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x8d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x8d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x8d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x8e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x8e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x8ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x8f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x8f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x8fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x900
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x904
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x908
			 0x3f028f5c,                                                  // 0.51                                        /// 0x90c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x910
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x914
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x918
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x91c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x920
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x924
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x928
			 0xffc00001,                                                  // -signaling NaN                              /// 0x92c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x930
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x934
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x938
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x93c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x940
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x944
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x948
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x94c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x950
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x954
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x958
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x95c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x960
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x964
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x968
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x96c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x970
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x974
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x978
			 0xff800000,                                                  // -inf                                        /// 0x97c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x980
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x984
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x988
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x98c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x990
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x994
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x998
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x99c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x9a0
			 0xbf028f5c,                                                  // -0.51                                       /// 0x9a4
			 0xff800000,                                                  // -inf                                        /// 0x9a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x9b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x9b4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x9bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x9c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x9c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x9c8
			 0xcb000000,                                                  // -8388608.0                                  /// 0x9cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x9d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x9d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x9dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x9e0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x9e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x9e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9ec
			 0x00000000,                                                  // zero                                        /// 0x9f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x9f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x9f8
			 0x4b000000,                                                  // 8388608.0                                   /// 0x9fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa00
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xa04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xa08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xa0c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xa10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xa14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xa18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa1c
			 0x00000000,                                                  // zero                                        /// 0xa20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xa24
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xa30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa34
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xa38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xa3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xa44
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa50
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xa54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xa5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa60
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa64
			 0x7f800000,                                                  // inf                                         /// 0xa68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa6c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xa70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xa78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xa7c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xa80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xa84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa88
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xa8c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xa90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xa94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xa98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa9c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xaa0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xaa4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xaa8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xab0
			 0xff800000,                                                  // -inf                                        /// 0xab4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xab8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xabc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xac0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xac4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xac8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xacc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xad0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xad4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xad8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xadc
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xae0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xae4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xae8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xaf0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xaf4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xaf8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xafc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xb00
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb08
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb10
			 0x4b000000,                                                  // 8388608.0                                   /// 0xb14
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xb18
			 0x4b000000,                                                  // 8388608.0                                   /// 0xb1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xb20
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xb24
			 0x55555555,                                                  // 4 random values                             /// 0xb28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xb30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xb38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xb3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xb40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb44
			 0x7fc00001,                                                  // signaling NaN                               /// 0xb48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xb4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xb54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xb58
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xb60
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xb64
			 0x3f028f5c,                                                  // 0.51                                        /// 0xb68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb6c
			 0x80000000,                                                  // -zero                                       /// 0xb70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb74
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xb78
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xb7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xb80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xb84
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xb88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb90
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0xb94
			 0x55555555,                                                  // 4 random values                             /// 0xb98
			 0x4b000000,                                                  // 8388608.0                                   /// 0xb9c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xba0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xba4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xba8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xbb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xbb4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xbb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xbbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xbc0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xbc4
			 0xcb000000,                                                  // -8388608.0                                  /// 0xbc8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xbcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xbd4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xbd8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xbdc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbe0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xbe4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbe8
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xbec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xbf0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xbf4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0xbf8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xbfc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xc00
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xc04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xc08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xc0c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xc10
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc14
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc18
			 0x33333333,                                                  // 4 random values                             /// 0xc1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xc20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xc24
			 0x55555555,                                                  // 4 random values                             /// 0xc28
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc34
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xc3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xc44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xc48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xc4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xc50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xc54
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc58
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xc5c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xc60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xc64
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xc68
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xc6c
			 0x7f800000,                                                  // inf                                         /// 0xc70
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xc74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xc78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xc7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xc80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xc88
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xc8c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xc90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xc94
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xc98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xc9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xca0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xca4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xca8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xcac
			 0x4b000000,                                                  // 8388608.0                                   /// 0xcb0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xcb4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xcb8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xcbc
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xcc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xccc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcd0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xcd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcd8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xcdc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xce0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xce4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xce8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xcec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xcf0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xcf4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xcf8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xcfc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xd00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xd08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xd0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xd10
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xd14
			 0x80000000,                                                  // -zero                                       /// 0xd18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd1c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xd24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xd2c
			 0x7f800000,                                                  // inf                                         /// 0xd30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xd34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xd38
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xd3c
			 0x7f800000,                                                  // inf                                         /// 0xd40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xd48
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xd4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xd50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xd54
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xd58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xd5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xd60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xd64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xd6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xd70
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xd74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xd78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xd7c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xd80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xd84
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd88
			 0x33333333,                                                  // 4 random values                             /// 0xd8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xd90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xd94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xd9c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xda0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xda4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xda8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xdb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xdb8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdbc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xdc0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xdc4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xdc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xdcc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xdd0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xdd4
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xdd8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xddc
			 0x55555555,                                                  // 4 random values                             /// 0xde0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xde4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xde8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdec
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xdf0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xdf4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdf8
			 0xff800000,                                                  // -inf                                        /// 0xdfc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xe04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xe0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xe14
			 0x3f028f5c,                                                  // 0.51                                        /// 0xe18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xe20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xe28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xe2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xe30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xe34
			 0x33333333,                                                  // 4 random values                             /// 0xe38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xe3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xe40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xe44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xe48
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xe4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xe50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe54
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xe58
			 0x00000000,                                                  // zero                                        /// 0xe5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xe64
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xe68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xe6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xe70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe74
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xe78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xe7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xe84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xe90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xe94
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xe98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xe9c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xea0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xea4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xea8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xeac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xeb0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xeb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xeb8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xebc
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xec0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xec4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xec8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xecc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xed0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xed4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xed8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xedc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xee0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xee4
			 0x7fc00001,                                                  // signaling NaN                               /// 0xee8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xeec
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xef0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xef4
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xef8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xefc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xf00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xf04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf08
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xf0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xf10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xf18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xf20
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xf24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xf28
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xf30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xf34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xf38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xf44
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xf48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xf4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xf54
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xf58
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xf5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xf60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xf64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xf68
			 0xffc00001,                                                  // -signaling NaN                              /// 0xf6c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xf78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xf7c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xf84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xf8c
			 0x7fc00001,                                                  // signaling NaN                               /// 0xf90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xf94
			 0x7fc00001,                                                  // signaling NaN                               /// 0xf98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xf9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xfa0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xfa4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfa8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xfac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xfb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xfb4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xfb8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xfbc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xfc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xfc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xfc8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xfcc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xfd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfd4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xfd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xfdc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xfe0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xfe4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xfe8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xff0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xff4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x14
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x18
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x1c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x24
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x28
			 0x7f800000,                                                  // inf                                         /// 0x2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x34
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x38
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4c
			 0x33333333,                                                  // 4 random values                             /// 0x50
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x58
			 0xff800000,                                                  // -inf                                        /// 0x5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x60
			 0x4b000000,                                                  // 8388608.0                                   /// 0x64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x80
			 0xcb000000,                                                  // -8388608.0                                  /// 0x84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x94
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa4
			 0xffc00001,                                                  // -signaling NaN                              /// 0xa8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xb0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xbc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xc0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xc4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xc8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xd4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xd8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xdc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xe0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xe8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x100
			 0x80000000,                                                  // -zero                                       /// 0x104
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x108
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x10c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x110
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x114
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x118
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x11c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x120
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x124
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x128
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x12c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x130
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x134
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x138
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x13c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x140
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x144
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x148
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x14c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x150
			 0xffc00001,                                                  // -signaling NaN                              /// 0x154
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x158
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x15c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x160
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x164
			 0xbf028f5c,                                                  // -0.51                                       /// 0x168
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x16c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x170
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x174
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x178
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x17c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x180
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x184
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x188
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x18c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x190
			 0xbf028f5c,                                                  // -0.51                                       /// 0x194
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x198
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x19c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x1a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x1a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x1ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x1b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x1b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x1bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x1c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x1c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x1c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x1d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x1dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x1e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x1e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x1e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x1ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x1f0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x1f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x1fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x200
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x204
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x208
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x20c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x210
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0x214
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x218
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x21c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x220
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x224
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x228
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x22c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x230
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x234
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x238
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x23c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x240
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x244
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x248
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x24c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x250
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x254
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x258
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x25c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x260
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x264
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x268
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x26c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x270
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x274
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x278
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x27c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x280
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x284
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x288
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x28c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x290
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x294
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x298
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x29c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x2a0
			 0x80000000,                                                  // -zero                                       /// 0x2a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x2b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x2b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x2b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x2bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x2c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x2c8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x2cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x2d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x2d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x2dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x2e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2e8
			 0x3f028f5c,                                                  // 0.51                                        /// 0x2ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x2f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x2fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x300
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x304
			 0xff800000,                                                  // -inf                                        /// 0x308
			 0xbf028f5c,                                                  // -0.51                                       /// 0x30c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x310
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x314
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x318
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x31c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x320
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x324
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x328
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x32c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x330
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x334
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x338
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x33c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x340
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x344
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x348
			 0x55555555,                                                  // 4 random values                             /// 0x34c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x350
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x354
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x358
			 0x4b000000,                                                  // 8388608.0                                   /// 0x35c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x360
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x364
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x368
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x36c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x370
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x374
			 0x7f800000,                                                  // inf                                         /// 0x378
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x37c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x380
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x384
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x388
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x38c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x390
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x394
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x398
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x39c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x3a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x3ac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3b0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x3b8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x3bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x3c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x3c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x3c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x3d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x3d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x3dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x3e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x3e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3e8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x3f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x3f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x3fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x400
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x404
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x408
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x40c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x410
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x414
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x418
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x41c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x420
			 0xbf028f5c,                                                  // -0.51                                       /// 0x424
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x428
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x42c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x430
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x434
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x438
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x43c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x440
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x444
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x448
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x44c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x450
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x454
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x458
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x45c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x460
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x464
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x468
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x46c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x470
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x474
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x478
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x47c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x480
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x484
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x488
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x48c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x490
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x494
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x498
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x49c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x4a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x4a4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x4a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x4ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x4b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x4b4
			 0x55555555,                                                  // 4 random values                             /// 0x4b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x4bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x4c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x4c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x4c8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x4cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x4d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x4d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x4dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x4e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x4e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x4e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x4ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x4f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x4f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x4fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x500
			 0x4b000000,                                                  // 8388608.0                                   /// 0x504
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x508
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x50c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x510
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x514
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x518
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x51c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x520
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x524
			 0x7f800000,                                                  // inf                                         /// 0x528
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x52c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x530
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x534
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x538
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x53c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x540
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x544
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x548
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x54c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x550
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x554
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x558
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x55c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x560
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x564
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x568
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x56c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x570
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x574
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x578
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x57c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x580
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x584
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x588
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x58c
			 0xffc00001,                                                  // -signaling NaN                              /// 0x590
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x594
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x598
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x59c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x5a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x5a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x5ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x5b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x5b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5b8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x5c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x5cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x5d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x5d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x5d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x5dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x5e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x5e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x5e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x5ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x5f4
			 0xcb000000,                                                  // -8388608.0                                  /// 0x5f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x5fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x600
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x604
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x608
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x60c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x610
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x614
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x618
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x61c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x620
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x624
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x628
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0x62c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x630
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x634
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x638
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x63c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x640
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x644
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x648
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x64c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x650
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x654
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x658
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x65c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x660
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x664
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x668
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x66c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x670
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x674
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x678
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x67c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x680
			 0x80000000,                                                  // -zero                                       /// 0x684
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x688
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x68c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x690
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x694
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x698
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x69c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x6a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x6a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x6ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x6b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x6b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x6bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x6c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x6c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x6cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x6d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x6d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x6d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x6dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x6e0
			 0x55555555,                                                  // 4 random values                             /// 0x6e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x6e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x6ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x6f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x6f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x700
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x704
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x708
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x70c
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x710
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x714
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x718
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x71c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x720
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x724
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x728
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x72c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x730
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x734
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x738
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x73c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x740
			 0x80000000,                                                  // -zero                                       /// 0x744
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x748
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x74c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x750
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x754
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x758
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x75c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x760
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x764
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x768
			 0x55555555,                                                  // 4 random values                             /// 0x76c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x770
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x774
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x778
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x77c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x780
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x784
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x788
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x78c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x790
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x794
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x798
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x79c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x7a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x7ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x7b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x7b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x7bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x7c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x7c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x7cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x7d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x7d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x7d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x7dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x7e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x7e8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x7ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x7f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x7f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x800
			 0xffc00001,                                                  // -signaling NaN                              /// 0x804
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x808
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x80c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x810
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x814
			 0x00000000,                                                  // zero                                        /// 0x818
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x81c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x820
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x824
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x828
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x82c
			 0x55555555,                                                  // 4 random values                             /// 0x830
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x834
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x838
			 0x55555555,                                                  // 4 random values                             /// 0x83c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x840
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x844
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x848
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x84c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x850
			 0x33333333,                                                  // 4 random values                             /// 0x854
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x858
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x85c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x860
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x864
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x868
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x86c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x870
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x874
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x878
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x87c
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x880
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x884
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x888
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x88c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x890
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x894
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x898
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x89c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8a0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x8a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x8a8
			 0xffc00001,                                                  // -signaling NaN                              /// 0x8ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x8b4
			 0x55555555,                                                  // 4 random values                             /// 0x8b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x8c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x8c4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x8c8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x8cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x8d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x8d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x8d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x8dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x8e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x8e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x8e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x8ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x8f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x8f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x8f8
			 0xff800000,                                                  // -inf                                        /// 0x8fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x900
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x904
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x908
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x90c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0x910
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x914
			 0x7f800000,                                                  // inf                                         /// 0x918
			 0xcb000000,                                                  // -8388608.0                                  /// 0x91c
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x920
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0x924
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x928
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x92c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x930
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x934
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x938
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x93c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x940
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x944
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x948
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x94c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x950
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x954
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x958
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x95c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x960
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x964
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x968
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x96c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x970
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x974
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x978
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x97c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x980
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x984
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x988
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x98c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x990
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x994
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x998
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x99c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x9a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x9a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0x9ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x9b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x9b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x9b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x9bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0x9c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x9c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x9cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x9d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0x9d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9d8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x9dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x9e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x9e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x9f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x9f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x9f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x9fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xa00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xa04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xa08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xa0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xa10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xa18
			 0x3f028f5c,                                                  // 0.51                                        /// 0xa1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa20
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xa24
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xa28
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xa2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xa34
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xa38
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xa3c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xa40
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xa44
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xa48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xa4c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xa50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xa58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa5c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xa60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xa64
			 0x55555555,                                                  // 4 random values                             /// 0xa68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xa6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xa78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xa7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xa84
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xa88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xa8c
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xa90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xa94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xa98
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xaa0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xaa4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xaa8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xab0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xab4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xab8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xabc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xac0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xac4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xac8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xacc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xad0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xad4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xad8
			 0x3f028f5c,                                                  // 0.51                                        /// 0xadc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xae0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xae4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xae8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xaec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xaf0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xaf4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaf8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xafc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xb00
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xb04
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xb08
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xb10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xb14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xb18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xb1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xb2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xb30
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xb34
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb38
			 0xffc00001,                                                  // -signaling NaN                              /// 0xb3c
			 0x33333333,                                                  // 4 random values                             /// 0xb40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0xb44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xb48
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xb4c
			 0x33333333,                                                  // 4 random values                             /// 0xb50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb54
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xb58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xb5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xb60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xb68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xb6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xb70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xb78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xb7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xb88
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xb94
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb98
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xb9c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xba0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xba4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xba8
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xbac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbb4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbb8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xbbc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbc4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbc8
			 0x7fc00001,                                                  // signaling NaN                               /// 0xbcc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xbd0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xbdc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xbe0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xbe4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xbe8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xbf0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xbf4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xbf8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbfc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xc00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xc08
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xc0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc10
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc14
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xc18
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc1c
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xc20
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xc24
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xc28
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xc2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xc30
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xc38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xc3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc40
			 0x80011111,                                                  // -9.7958E-41                                 /// 0xc44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xc48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xc50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xc58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xc60
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0xc64
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xc68
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xc6c
			 0x3f028f5c,                                                  // 0.51                                        /// 0xc70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xc74
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xc78
			 0xff800000,                                                  // -inf                                        /// 0xc7c
			 0xff800000,                                                  // -inf                                        /// 0xc80
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xc88
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xc94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc98
			 0x3f028f5c,                                                  // 0.51                                        /// 0xc9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xca0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xca4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xca8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xcac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xcb0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xcb4
			 0x7f800000,                                                  // inf                                         /// 0xcb8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xcbc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xcc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xcc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xccc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xcd4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xcd8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xcdc
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xce0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xce4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xce8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xcec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xcf0
			 0xcb000000,                                                  // -8388608.0                                  /// 0xcf4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xcf8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xcfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xd00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xd04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xd08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xd10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd14
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xd18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xd1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xd20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xd24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xd28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xd2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xd30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xd34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xd38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0xd3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xd44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xd48
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xd50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xd54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd58
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd5c
			 0xbf028f5c,                                                  // -0.51                                       /// 0xd60
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xd68
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xd6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xd70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd74
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xd78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xd80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xd84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd88
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd94
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xd98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xda0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xda4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xda8
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xdac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xdb0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xdb4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xdb8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xdbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xdc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xdc4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xdc8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdd0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 0xdd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xddc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xde0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xde4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xde8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xdec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xdf0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdf4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xdf8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xdfc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xe00
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xe04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xe08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xe0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xe14
			 0x55555555,                                                  // 4 random values                             /// 0xe18
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xe1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0xe24
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xe28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0xe2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe34
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xe38
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xe3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xe44
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xe48
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xe4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xe50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xe54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xe58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xe5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 0xe60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xe64
			 0xff800000,                                                  // -inf                                        /// 0xe68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xe6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xe74
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xe78
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xe7c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xe80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xe84
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xe88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xe8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0xe94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xe98
			 0x3f028f5c,                                                  // 0.51                                        /// 0xe9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xea0
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xea4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xea8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xeac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xeb0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xeb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xeb8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xebc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xec0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0xec4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xec8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xecc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xed0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xed4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xed8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xedc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xee0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xee4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xee8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xeec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xef0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xef4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0xef8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xefc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xf00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xf04
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0xf0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf14
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0xf18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xf1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xf20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xf24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xf28
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xf30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xf34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0xf38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xf3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xf40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xf48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xf4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xf50
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xf54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xf58
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xf5c
			 0xcb000000,                                                  // -8388608.0                                  /// 0xf60
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf64
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xf68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xf6c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xf70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xf78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xf7c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xf84
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0xf88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xf90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xf94
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xf98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xf9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xfa0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xfa4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xfa8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfac
			 0xff800000,                                                  // -inf                                        /// 0xfb0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xfb4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xfb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xfbc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xfc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xfcc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xfd0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xfd8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfdc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xfe0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfe4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xfe8
			 0x80000000,                                                  // -zero                                       /// 0xfec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xff0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xff4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xff8
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xc
			 0xffc00001,                                                  // -signaling NaN                              /// 0x10
			 0x00000000,                                                  // zero                                        /// 0x14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x1c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x20
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x38
			 0xcb000000,                                                  // -8388608.0                                  /// 0x3c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x40
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x44
			 0x55555555,                                                  // 4 random values                             /// 0x48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x54
			 0xffc00001,                                                  // -signaling NaN                              /// 0x58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0x60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x68
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x78
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x88
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xa4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xa8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xb0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xb8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xbc
			 0xbf028f5c,                                                  // -0.51                                       /// 0xc0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xd0
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xd4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xd8
			 0x4b000000,                                                  // 8388608.0                                   /// 0xdc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0xe4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xf0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf8
			 0x4b000000,                                                  // 8388608.0                                   /// 0xfc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x100
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x104
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x108
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x10c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x110
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x114
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x118
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x11c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x120
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x124
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x128
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x12c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x130
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x134
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x138
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x13c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0x140
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x144
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x148
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x14c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x150
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x154
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x158
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x15c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x160
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0x164
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x168
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x16c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x170
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x174
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x178
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x17c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x180
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x184
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x188
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x18c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x190
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x194
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x198
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x19c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x1a4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x1ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x1b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x1b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x1b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x1bc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x1c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x1c4
			 0x3f028f5c,                                                  // 0.51                                        /// 0x1c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x1cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x1d0
			 0x55555555,                                                  // 4 random values                             /// 0x1d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x1d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x1dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x1e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x1e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x1e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x1ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x1f0
			 0x33333333,                                                  // 4 random values                             /// 0x1f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x1f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x1fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x200
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x204
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x208
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x20c
			 0x00000000,                                                  // zero                                        /// 0x210
			 0x80000000,                                                  // -zero                                       /// 0x214
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x218
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x21c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x220
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x224
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x228
			 0x0e000003,                                                  // Trailing 1s:                                /// 0x22c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x230
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x234
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x238
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x23c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x240
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x244
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x248
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x24c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x250
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x254
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x258
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x25c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x260
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x264
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x268
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x26c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x270
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x274
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x278
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x27c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x280
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x284
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x288
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x28c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x290
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x294
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x298
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x29c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x2a4
			 0x7fc00001,                                                  // signaling NaN                               /// 0x2a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x2ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2b0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x2b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x2bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0x2c0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x2c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x2c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x2cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x2d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x2d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x2dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x2e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x2e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x2e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x2ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x2f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x2f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x2f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0x2fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x300
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x304
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x308
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x30c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x310
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0x314
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x318
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x31c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x320
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x324
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x328
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x32c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x330
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x334
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x338
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x33c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x340
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x344
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x348
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x34c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x350
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x354
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x358
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x35c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x360
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x364
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0x368
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x36c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x370
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x374
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0x378
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x37c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x380
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x384
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x388
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x38c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x390
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x394
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x398
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x39c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3a4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x3a8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x3ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x3b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x3b8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x3bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x3c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x3c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x3c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3cc
			 0xcb000000,                                                  // -8388608.0                                  /// 0x3d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x3d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x3d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x3dc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0x3e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x3ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x3f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x3f4
			 0x3f028f5c,                                                  // 0.51                                        /// 0x3f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x3fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x400
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x404
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x408
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x40c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x410
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x414
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x418
			 0x55555555,                                                  // 4 random values                             /// 0x41c
			 0xffc00001,                                                  // -signaling NaN                              /// 0x420
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x424
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x428
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x42c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x430
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x434
			 0x0c600000,                                                  // Leading 1s:                                 /// 0x438
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x43c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x440
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x444
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x448
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x44c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x450
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x454
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x458
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x45c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x460
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0x464
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0x468
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x46c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x470
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x474
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x478
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x47c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x480
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x484
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0x488
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0x48c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x490
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x494
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x498
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0x49c
			 0xff800000,                                                  // -inf                                        /// 0x4a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x4a4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x4a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x4ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x4b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0x4b8
			 0x00000000,                                                  // zero                                        /// 0x4bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x4c4
			 0x4b000000,                                                  // 8388608.0                                   /// 0x4c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x4cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0x4d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x4dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4e0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x4e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x4e8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x4f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x4f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x4f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x4fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x500
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x504
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x508
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x50c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x510
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x514
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x518
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x51c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x520
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x524
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x528
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x52c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x530
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x534
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x538
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x53c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x540
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x544
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x548
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0x54c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x550
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x554
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x558
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x55c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0x560
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0x564
			 0xbf028f5c,                                                  // -0.51                                       /// 0x568
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0x56c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x570
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x574
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x578
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x57c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x580
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x584
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x588
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x58c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x590
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x594
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x598
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x59c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x5a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0x5a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x5a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x5ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x5b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x5b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5b8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x5c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0x5c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x5c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0x5d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x5d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x5d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x5e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x5e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x5ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x5f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x5f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x5f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x5fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x600
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0x604
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x608
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0x60c
			 0x7f800000,                                                  // inf                                         /// 0x610
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x614
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x618
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x61c
			 0x80000000,                                                  // -zero                                       /// 0x620
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x624
			 0x00000000,                                                  // zero                                        /// 0x628
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0x62c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x630
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x634
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0x638
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x63c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x640
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x644
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x648
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0x64c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x650
			 0x7fc00001,                                                  // signaling NaN                               /// 0x654
			 0xffc00001,                                                  // -signaling NaN                              /// 0x658
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x65c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0x660
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x664
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0x668
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0x66c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x670
			 0x80000000,                                                  // -zero                                       /// 0x674
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0x678
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x67c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x680
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x684
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x688
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x68c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x690
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x694
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x698
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x69c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x6a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6a4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x6a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x6b0
			 0x4b000000,                                                  // 8388608.0                                   /// 0x6b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6b8
			 0xffc00001,                                                  // -signaling NaN                              /// 0x6bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x6c0
			 0x33333333,                                                  // 4 random values                             /// 0x6c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 0x6c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x6cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x6d0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x6d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0x6d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x6dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x6e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x6e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x6e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x6ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x6f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x6f4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x6f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x6fc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x700
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x704
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x708
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x70c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0x710
			 0x0e000001,                                                  // Trailing 1s:                                /// 0x714
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x718
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x71c
			 0xff800000,                                                  // -inf                                        /// 0x720
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x724
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x728
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x72c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x730
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x734
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x738
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x73c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x740
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x744
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x748
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0x74c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x750
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x754
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x758
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0x75c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0x760
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x764
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0x768
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0x76c
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x770
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x774
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x778
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0x77c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0x780
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x784
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x788
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x78c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0x790
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x794
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x798
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0x79c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0x7a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0x7a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0x7a8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x7ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0x7b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7b4
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x7b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x7c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0x7c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x7cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0x7d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x7d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x7dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x7e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0x7e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x7e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x7ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x7f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x7f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x7f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x7fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0x800
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x804
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x808
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x80c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0x810
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0x814
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x818
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x81c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x820
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x824
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x828
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0x82c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0x830
			 0x0c400000,                                                  // Leading 1s:                                 /// 0x834
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x838
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x83c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x840
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x844
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x848
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0x84c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0x850
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0x854
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0x858
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0x85c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0x860
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x864
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0x868
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x86c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x870
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0x874
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0x878
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0x87c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x880
			 0xffc00001,                                                  // -signaling NaN                              /// 0x884
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x888
			 0x7f800000,                                                  // inf                                         /// 0x88c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x890
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0x894
			 0x7f800000,                                                  // inf                                         /// 0x898
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x89c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0x8a0
			 0x33333333,                                                  // 4 random values                             /// 0x8a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0x8a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0x8ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x8b0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0x8b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0x8b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0x8bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x8c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x8c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0x8c8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x8cc
			 0x7fc00001,                                                  // signaling NaN                               /// 0x8d0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0x8d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0x8d8
			 0x00000000,                                                  // zero                                        /// 0x8dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 0x8e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x8e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0x8ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x8f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0x8f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0x8f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x8fc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0x900
			 0x33333333,                                                  // 4 random values                             /// 0x904
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x908
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0x90c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0x910
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x914
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0x918
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0x91c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x920
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0x924
			 0x7f800000,                                                  // inf                                         /// 0x928
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0x92c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0x930
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0x934
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0x938
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x93c
			 0x0c700000,                                                  // Leading 1s:                                 /// 0x940
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0x944
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x948
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x94c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0x950
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x954
			 0x0e000007,                                                  // Trailing 1s:                                /// 0x958
			 0x33333333,                                                  // 4 random values                             /// 0x95c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0x960
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0x964
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x968
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x96c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x970
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x974
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0x978
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x97c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x980
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x984
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0x988
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x98c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0x990
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0x994
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x998
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0x99c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0x9a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0x9a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x9ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0x9b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x9b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0x9b8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0x9c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0x9c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x9cc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0x9d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0x9d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0x9dc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0x9e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0x9e4
			 0x4b000000,                                                  // 8388608.0                                   /// 0x9e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0x9ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0x9f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0x9f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0x9f8
			 0x3f028f5c,                                                  // 0.51                                        /// 0x9fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xa00
			 0x33333333,                                                  // 4 random values                             /// 0xa04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xa08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0xa0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xa10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xa14
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xa18
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xa20
			 0xff800000,                                                  // -inf                                        /// 0xa24
			 0xbf028f5c,                                                  // -0.51                                       /// 0xa28
			 0x7fc00001,                                                  // signaling NaN                               /// 0xa2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xa30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xa34
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xa38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xa40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa44
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa48
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0xa4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0xa50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xa54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xa58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xa60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xa64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xa68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xa70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xa74
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xa78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0xa7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0xa80
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xa84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa88
			 0xbf028f5c,                                                  // -0.51                                       /// 0xa8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xa90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xa94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xa98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xa9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xaa0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xaa4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xaa8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0xaac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xab0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xab4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xab8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xabc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xac0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xac4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xac8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0xacc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xad0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xad4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xad8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xadc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0xae0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0xae4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xae8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xaec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xaf0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0xaf4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0xaf8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xafc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb00
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xb08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xb0c
			 0xffc00001,                                                  // -signaling NaN                              /// 0xb10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xb14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xb18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xb1c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb20
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xb24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xb28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xb2c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xb30
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xb34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0xb3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xb40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xb44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xb48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xb4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xb50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xb54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb58
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xb5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xb60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0xb64
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xb68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb6c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xb70
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xb74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb78
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xb7c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xb80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xb88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xb8c
			 0x7f800000,                                                  // inf                                         /// 0xb90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xb94
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xb98
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xb9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xba0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xba4
			 0x33333333,                                                  // 4 random values                             /// 0xba8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xbac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xbb0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0xbb4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xbb8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xbbc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbc0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0xbc4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0xbc8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xbcc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xbd0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbd4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xbd8
			 0xffc00001,                                                  // -signaling NaN                              /// 0xbdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbe0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xbe4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbe8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xbec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xbf0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xbf4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0xbf8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xbfc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0xc00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xc04
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0xc08
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xc0c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xc10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xc14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0xc18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xc1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xc20
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xc24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xc28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0xc2c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xc30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xc38
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc3c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xc44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xc48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0xc4c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xc50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xc54
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xc58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xc5c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0xc60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xc64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xc68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0xc6c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xc70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xc74
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xc78
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xc7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xc80
			 0x7fc00001,                                                  // signaling NaN                               /// 0xc84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0xc88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xc8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0xc90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xc94
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xc98
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xc9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xca0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xca4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xca8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0xcac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xcb0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xcb4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xcb8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcbc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcc0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xcc4
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xcc8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xccc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xcd0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xcd4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xcd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xcdc
			 0x80000000,                                                  // -zero                                       /// 0xce0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xce4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0xce8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xcec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xcf0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0xcf4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xcf8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xcfc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0xd00
			 0x55555555,                                                  // 4 random values                             /// 0xd04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xd0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xd14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xd18
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0xd1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xd20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xd28
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xd2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xd30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xd34
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xd38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd3c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xd40
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xd44
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd48
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xd4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xd54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xd58
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xd5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0xd68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0xd6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0xd70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xd74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0xd78
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xd7c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xd80
			 0xcb000000,                                                  // -8388608.0                                  /// 0xd84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xd88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xd8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0xd90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0xd94
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0xd98
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xd9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xda0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0xda4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xda8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xdac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0xdb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0xdb4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xdb8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xdbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xdc0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xdc4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xdc8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdcc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdd0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0xdd8
			 0x0c600000,                                                  // Leading 1s:                                 /// 0xddc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xde0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xde4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xde8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xdec
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xdf0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0xdf4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xdf8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0xdfc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0xe00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xe04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0xe08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0xe10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0xe14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xe18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xe1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xe20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xe24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe28
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe2c
			 0xffc00001,                                                  // -signaling NaN                              /// 0xe30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0xe34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe38
			 0xffc00001,                                                  // -signaling NaN                              /// 0xe3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe40
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0xe44
			 0x55555555,                                                  // 4 random values                             /// 0xe48
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0xe4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe50
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe58
			 0x0c700000,                                                  // Leading 1s:                                 /// 0xe5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0xe60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xe64
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xe68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xe6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xe70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0xe74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xe78
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xe7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xe80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0xe84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0xe88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0xe8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xe90
			 0x0e000007,                                                  // Trailing 1s:                                /// 0xe94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0xe98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xe9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 0xea0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0xea4
			 0x33333333,                                                  // 4 random values                             /// 0xea8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xeac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0xeb0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0xeb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xeb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xebc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0xec0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xec4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xec8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xecc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xed0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xed4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xed8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0xedc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xee0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xee4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xee8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0xeec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0xef0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xef4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0xef8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xefc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0xf00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0xf04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xf08
			 0xbf028f5c,                                                  // -0.51                                       /// 0xf0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xf10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf14
			 0x0e000003,                                                  // Trailing 1s:                                /// 0xf18
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xf1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0xf20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0xf24
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0xf28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0xf2c
			 0x00011111,                                                  // 9.7958E-41                                  /// 0xf30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0xf34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0xf3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf44
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0xf48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xf4c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xf50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xf54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0xf58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xf5c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0xf60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xf64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0xf68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0xf6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0xf70
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xf74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0xf78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0xf7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0xf80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0xf84
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0xf88
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0xf8c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xf90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0xf94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0xf98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0xf9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0xfa0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0xfa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0xfa8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0xfac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0xfb0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xfb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfb8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0xfbc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0xfc0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0xfc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0xfc8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xfcc
			 0x3f028f5c,                                                  // 0.51                                        /// 0xfd0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0xfd4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0xfd8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0xfdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0xfe0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0xfe4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0xfe8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0xfec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0xff0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0xff4
			 0xcb000000,                                                  // -8388608.0                                  /// 0xff8
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
			 0x41b80000,
			 0x41300000,
			 0x41980000,
			 0x41800000,
			 0x41a80000,
			 0x41400000,
			 0x41880000,
			 0x41400000,

			 // vpu register f3
			 0x41b00000,
			 0x40400000,
			 0x41c80000,
			 0x41c00000,
			 0x41200000,
			 0x41100000,
			 0x41e80000,
			 0x41600000,

			 // vpu register f4
			 0x41800000,
			 0x41e80000,
			 0x41300000,
			 0x41200000,
			 0x41300000,
			 0x41900000,
			 0x41100000,
			 0x40e00000,

			 // vpu register f5
			 0x41c00000,
			 0x41500000,
			 0x41e80000,
			 0x40a00000,
			 0x40a00000,
			 0x41d00000,
			 0x40400000,
			 0x41b00000,

			 // vpu register f6
			 0x41d00000,
			 0x41700000,
			 0x41800000,
			 0x41d00000,
			 0x40e00000,
			 0x41d80000,
			 0x41b80000,
			 0x3f800000,

			 // vpu register f7
			 0x41b00000,
			 0x40400000,
			 0x41d00000,
			 0x40800000,
			 0x40000000,
			 0x41d00000,
			 0x41f80000,
			 0x41f00000,

			 // vpu register f8
			 0x40400000,
			 0x40e00000,
			 0x41100000,
			 0x41a00000,
			 0x42000000,
			 0x41800000,
			 0x41b80000,
			 0x41a00000,

			 // vpu register f9
			 0x41500000,
			 0x41e80000,
			 0x41b00000,
			 0x41e00000,
			 0x40800000,
			 0x41f00000,
			 0x41600000,
			 0x41980000,

			 // vpu register f10
			 0x41800000,
			 0x41b80000,
			 0x42000000,
			 0x41f00000,
			 0x40a00000,
			 0x40c00000,
			 0x41200000,
			 0x3f800000,

			 // vpu register f11
			 0x41880000,
			 0x41000000,
			 0x41300000,
			 0x41e00000,
			 0x41d00000,
			 0x41c00000,
			 0x41b80000,
			 0x40e00000,

			 // vpu register f12
			 0x41700000,
			 0x41a00000,
			 0x41500000,
			 0x40800000,
			 0x40c00000,
			 0x41400000,
			 0x41400000,
			 0x41100000,

			 // vpu register f13
			 0x40a00000,
			 0x41200000,
			 0x41980000,
			 0x41100000,
			 0x41900000,
			 0x41000000,
			 0x41b80000,
			 0x41880000,

			 // vpu register f14
			 0x41e00000,
			 0x41000000,
			 0x41f80000,
			 0x40400000,
			 0x41c80000,
			 0x40800000,
			 0x41900000,
			 0x41880000,

			 // vpu register f15
			 0x40c00000,
			 0x40e00000,
			 0x3f800000,
			 0x40400000,
			 0x41b00000,
			 0x41980000,
			 0x40000000,
			 0x41400000,

			 // vpu register f16
			 0x40800000,
			 0x40e00000,
			 0x41f00000,
			 0x41a80000,
			 0x41b80000,
			 0x40000000,
			 0x41700000,
			 0x41d80000,

			 // vpu register f17
			 0x41a80000,
			 0x41800000,
			 0x40e00000,
			 0x41800000,
			 0x41500000,
			 0x41b80000,
			 0x41a00000,
			 0x41f00000,

			 // vpu register f18
			 0x40e00000,
			 0x42000000,
			 0x41b00000,
			 0x41a80000,
			 0x40a00000,
			 0x41300000,
			 0x41800000,
			 0x41e80000,

			 // vpu register f19
			 0x41b00000,
			 0x41000000,
			 0x3f800000,
			 0x41500000,
			 0x41b80000,
			 0x41000000,
			 0x41e80000,
			 0x41200000,

			 // vpu register f20
			 0x41c00000,
			 0x41900000,
			 0x41c80000,
			 0x41c00000,
			 0x3f800000,
			 0x41d80000,
			 0x41500000,
			 0x41700000,

			 // vpu register f21
			 0x41c80000,
			 0x41b80000,
			 0x40a00000,
			 0x41f80000,
			 0x41f00000,
			 0x40800000,
			 0x41000000,
			 0x41900000,

			 // vpu register f22
			 0x40e00000,
			 0x41300000,
			 0x3f800000,
			 0x41000000,
			 0x41600000,
			 0x41600000,
			 0x41f00000,
			 0x41f80000,

			 // vpu register f23
			 0x41800000,
			 0x41800000,
			 0x41c80000,
			 0x40a00000,
			 0x40000000,
			 0x41d00000,
			 0x41c80000,
			 0x41a80000,

			 // vpu register f24
			 0x41e00000,
			 0x40c00000,
			 0x41500000,
			 0x40c00000,
			 0x41400000,
			 0x41200000,
			 0x41100000,
			 0x41400000,

			 // vpu register f25
			 0x41400000,
			 0x40a00000,
			 0x41c00000,
			 0x40800000,
			 0x41900000,
			 0x41800000,
			 0x41a80000,
			 0x41a80000,

			 // vpu register f26
			 0x41900000,
			 0x41e00000,
			 0x3f800000,
			 0x41800000,
			 0x41f00000,
			 0x41900000,
			 0x41400000,
			 0x41980000,

			 // vpu register f27
			 0x41200000,
			 0x40e00000,
			 0x41400000,
			 0x41500000,
			 0x40c00000,
			 0x41e00000,
			 0x41e00000,
			 0x41800000,

			 // vpu register f28
			 0x41e00000,
			 0x40800000,
			 0x41300000,
			 0x40400000,
			 0x41b80000,
			 0x41300000,
			 0x3f800000,
			 0x41500000,

			 // vpu register f29
			 0x41400000,
			 0x41e80000,
			 0x41600000,
			 0x41000000,
			 0x40a00000,
			 0x40800000,
			 0x41d80000,
			 0x41c00000,

			 // vpu register f30
			 0x41a80000,
			 0x41e80000,
			 0x3f800000,
			 0x41f80000,
			 0x41200000,
			 0x41c00000,
			 0x41e80000,
			 0x41e00000,

			 // vpu register f31
			 0x41200000,
			 0x40a00000,
			 0x41c80000,
			 0x41f80000,
			 0x40800000,
			 0x41300000,
			 0x40400000,
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
    SET_SHIRE_COOP;
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

///////////////////////////////////////tensor_store_vpu
        "li x5,  0xe0000000000000 \n"          //   reg_stride [63:62] = 0x0,  start_reg [61:57] = 0x0,  cols [56:55] = 0x1,  Arows [54:51] = 0xc,  coop_store [50:49] = 0x0,  scp [48:48] = 0x0,
        "add x5, x14, zero\n"
        "csrrw zero,  0x87f , x5\n"  //  tensor_store_vpu
        "csrwi   0x830, 0x8\n"  // wait on tensor_store_vpu
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
