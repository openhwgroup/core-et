/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
int main_l3_mixed() { printf("hi from main_l3_mixed"); return 0; }
int main_l3_read_miss() { printf("hi from main_l3_read_miss"); return 0; }
int main_gfx_ops_disabled() { printf("hi from main_gfx_ops_disabled"); return 0; }
int main_sparse_nn() { printf("hi from main_sparse_nn"); return 0; }
int main_matmul_max_fc() { printf("hi from main_matmul_max_fc"); return 0; }
#define NKERNELS 3
const kernel_info_t  kernels[NKERNELS+1] = {
  { main_l3_mixed, "main_l3_mixed", USES_ONE_MINION },
  { main_l3_read_miss, "main_l3_read_miss", USES_ONE_THREAD },
  { main_gfx_ops_disabled, "main_gfx_ops_disabled", USES_ONE_THREAD_T0 },
  { 0, "", 0 }
};
#define NKERNELS_FC 2
const kernel_info_t  kernels_fc[NKERNELS_FC+1] = {
  { main_sparse_nn, "main_sparse_nn", USES_ALL_SHIRES },
  { main_matmul_max_fc, "main_matmul_max_fc", USES_ALL_SHIRES_T0 },
  { 0, "", 0 }
};
