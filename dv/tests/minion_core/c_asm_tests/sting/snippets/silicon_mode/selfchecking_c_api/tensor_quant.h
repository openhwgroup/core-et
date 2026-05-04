/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


#ifndef _ET_SELFCHK_TQUANT_H_
#define _ET_SELFCHK_TQUANT_H_

#include "selfchecking_common.h"

#define LAST 0
#define INT32_TO_FP32 1
#define FP32_TO_INT32 2
#define RELU 3
#define INT32_ADD_ROW 4
#define INT32_ADD_COL 5
#define FP32_MUL_ROW 6
#define FP32_MUL_COL 7
#define SATINT8 8
#define SATUINT8 9
#define PACK_128B 10

#define MAX(A, B) ((A) > (B)) ? (A) : (B)
#define MIN(A, B) ((A) < (B)) ? (A) : (B)

#define TQUANT_LOOP(HID, ROWS, COLS, FSTART, C, A, TRANS) \
   for (int i = 0; i < (ROWS); i++) { \
      for (int j = 0; j < (COLS); j+=8) { \
          uint32_t reg = ((FSTART) + (i*2) + (j/8)) % 32; \
          uint32_t nlanes = (j+8) > (COLS) ? (COLS) - j : 8; \
          /*print_vpurf_fs1(HID, (uint64_t*)A, reg);*/ \
          for (int lane = 0; lane < nlanes; lane++) { \
             C[reg][lane] = TRANS(A[reg][lane]); \
          } \
          /*print_vpurf_fd(HID, (uint64_t*)C, reg);*/ \
      } \
   }

#define TQUANT_ROWS_LOOP(HID, ROWS, COLS, FSTART, LINE, A, B, TRANS) \
   for (int i = 0; i < (ROWS); i++) { \
      for (int j = 0; j < (COLS); j+=8) { \
          uint32_t reg = ((FSTART) + (i*2) + (j/8)) % 32; \
          uint32_t nlanes = (j+8) > (COLS) ? (COLS) - j : 8; \
          /*print_tquant_l1scp(HID, (uint64_t*) B, LINE, j);*/ \
          /*print_vpurf_fs1(HID, (uint64_t*)A, reg);*/ \
          for (int lane = 0; lane < nlanes; lane++) { \
             A[reg][lane] = TRANS(A[reg][lane], B[(LINE)][j + lane]); \
          } \
          /*print_vpurf_fd(HID, (uint64_t*)A, reg);*/ \
      } \
   }

#define TQUANT_COLS_LOOP(HID, ROWS, COLS, FSTART, LINE, A, B, TRANS) \
   for (int i = 0; i < (ROWS); i++) { \
      for (int j = 0; j < (COLS); j+=8) { \
          uint32_t reg = ((FSTART) + (i*2) + (j/8)) % 32; \
          uint32_t nlanes = (j+8) > (COLS) ? (COLS) - j : 8; \
          /*print_tquant_l1scp(HID, (uint64_t*) B, LINE, i);*/ \
          /*print_vpurf_fs1(HID, (uint64_t*)A, reg);*/ \
          for (int lane = 0; lane < nlanes; lane++) { \
             A[reg][lane] = TRANS(A[reg][lane], B[(LINE)][i]); \
          } \
          /*print_vpurf_fd(HID, (uint64_t*)A, reg);*/ \
      } \
   }

extern "C" int tquant_selfchecking(tensor_data* snp_info);
#endif
