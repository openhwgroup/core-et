/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#include "trace.h"
#include "sting_lib.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#ifndef _ET_SELFCHK_COMMON_H_
#define _ET_SELFCHK_COMMON_H_

//#define DEBUG 1

#ifdef DEBUG
   #include "../../et_printf.h"
#else
   #define et_printf(...)
#endif

#define pwrtwo(x) (1 << (x))

#define PASS 0
#define NOT_PASS 1

#define SKIP 12
#define COMPARE 27

#define L1SCP_LINE_SIZE_BYTES 64
#define L2SCP_LINE_SIZE_BYTES 64
#define L1SCP_NUM_LINES 48
#define L2SCP_NUM_LINES 0xA000 /*(0x280000/L2SCP_LINE_SIZE_BYTES)*/
#define LANES_PER_VPU_REG 8
#define BYTES_PER_LANE 4
#define VPURF_NUM_REGS 32

#define C_TEST_FAIL \
   __asm__ __volatile__ ( \
         "fence\n"\
         "lui a7, 0x50BAD\n" \
         "csrw validation0, a7\n" \
         "wfi\n" \
         : : : "a7");

typedef struct tensor_data {
   uint64_t csr_enc;       // snp_info[0]
   uint64_t tensor_mask;   // snp_info[8]
   uint64_t x31_val;       // snp_info[16]
   uint64_t pre_data_ptr;  // snp_info[24]
   uint64_t post_data_ptr; // snp_info[32]
   uint64_t pre_vpurf_ptr; // snp_info[40]
   uint64_t tl_tenb_csr;   // snp_info[48]
   uint64_t minion_id;     // snp_info[56]
   uint64_t pre_tenc_ptr;  // snp_info[64]
   uint64_t post_tenc_ptr; // snp_info[72]
   uint64_t ignore_2;      // snp_info[80]
   uint64_t ignore_3;      // snp_info[88]
   uint64_t ignore_4;      // snp_info[96]
   uint64_t ignore_5;      // snp_info[104]
   uint64_t ignore_6;      // snp_info[112]
   uint64_t ignore_7;      // snp_info[120]
} tensor_data;

typedef struct tensor_reduce_data {
   uint64_t csr_enc;           // snp_info[0]
   uint64_t ignore_0;          // snp_info[8]
   uint64_t ignore_1;          // snp_info[16]
   uint64_t prtnr_reduce_data; // snp_info[24]
   uint64_t post_vpurf_ptr;    // snp_info[32]
   uint64_t pre_vpurf_ptr;     // snp_info[40]
   uint64_t ignore_2;          // snp_info[48]
   uint64_t minion_id;         // snp_info[56]
   uint64_t ignore_3;          // snp_info[64]
   uint64_t ignore_4;          // snp_info[72]
   uint64_t ignore_5;          // snp_info[80]
   uint64_t ignore_6;          // snp_info[88]
   uint64_t ignore_7;          // snp_info[96]
   uint64_t ignore_8;          // snp_info[104]
   uint64_t ignore_9;          // snp_info[112]
   uint64_t ignore_10;         // snp_info[120]
} tensor_reduce_data;

inline uint_fast64_t sm_get_bits(uint64_t value, int head, int tail) {
   // we want to extract k-bits
   uint_fast64_t k = (head - tail) + 1;
   uint_fast64_t mask = 1;
   mask = mask << k;
   mask = mask - 1;
   uint_fast64_t sh_val = value;
   sh_val = sh_val  >> tail;
   return (mask & sh_val);
}

int compare_vpurf(uint64_t hid, uint64_t* vpurf_a, uint64_t* vpurf_b);
void print_vpurf_fs1(uint64_t hid, uint64_t* vpurf_a, uint64_t fs1);
void print_vpurf_fd(uint64_t hid, uint64_t* vpurf_a, uint64_t fd);
void print_tquant_l1scp(uint64_t hid, uint64_t* l1scp, uint64_t line, uint64_t start);
void print_mem_w(uint64_t hid, uint64_t addr);

#endif
