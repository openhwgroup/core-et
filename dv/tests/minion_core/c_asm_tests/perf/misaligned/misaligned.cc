/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include "et_test_common.h"

int main()
{
   volatile long long int __attribute__((aligned(4096))) data[128];
   C_TEST_START;

   // Test
   __asm__ __volatile__ (

         //
         // LOAD MISALIGNED CHECKS
         //

         // Enables all elements of FPU
         "mov.m.x m0, zero, 0xff\n"

         // Inits 128 bytes of data
         "addi a1, %[data], 0\n"
         "addi a2, zero, 144\n"
         "addi a3, zero, 0\n"
         "loop_init:\n"
         "sb   a3, 0(a1)\n"
         "addi a3, a3, 1\n"
         "addi a1, a1, 1\n"
         "add  a2, a2, -1\n"
         "bne  a2, zero, loop_init\n"

         // Stores in FP registers the contents
         "addi a1, %[data], 0\n"
         "flw.ps f16, 0(a1)\n"
         "flw.ps f17, 16(a1)\n"
         "flw.ps f18, 32(a1)\n"
         "flw.ps f19, 48(a1)\n"
         "flw.ps f20, 64(a1)\n"
         "flw.ps f21, 80(a1)\n"
         "flw.ps f22, 96(a1)\n"
         "flw.ps f23, 112(a1)\n"

         // 128 byte access
         "addi a1, %[data], 0\n"
         "addi a2, zero, 128\n"
         "loop_lb:\n"
         "lb   a3, 0(a1)\n"
         "addi a1, a1, 1\n"
         "add  a2, a2, -1\n"
         "bne  a2, zero, loop_lb\n"

         // 127 halfword access
         "addi a1, %[data], 0\n"
         "addi a2, zero, 127\n"
         "loop_lh:\n"
         "lh   a3, 0(a1)\n"
         "addi a1, a1, 1\n"
         "add  a2, a2, -1\n"
         "bne  a2, zero, loop_lh\n"

         // 125 word access
         "addi a1, %[data], 0\n"
         "addi a2, zero, 125\n"
         "loop_lw:\n"
         "lw   a3, 0(a1)\n"
         "addi a1, a1, 1\n"
         "add  a2, a2, -1\n"
         "bne  a2, zero, loop_lw\n"

         // 121 dword access
         "addi a1, %[data], 0\n"
         "addi a2, zero, 121\n"
         "loop_ld:\n"
         "ld   a3, 0(a1)\n"
         "addi a1, a1, 1\n"
         "add  a2, a2, -1\n"
         "bne  a2, zero, loop_ld\n"

         // 112 128b access
         "addi a1, %[data], 0\n"
         "addi a2, zero, 112\n"
         "loop_flw_ps:\n"
         "flw.ps f0, 0(a1)\n"
         "addi a1, a1, 1\n"
         "add  a2, a2, -1\n"
         "bne  a2, zero, loop_flw_ps\n"

         //
         // STORE MISALIGNED CHECKS
         //

         "lui  a1, 0xdeadb\n"
         "ori  a1, a1, 0x7ab\n" // Generates 0xdeadb7af x 2 to a3
         "slli a3, a1, 32\n"
         "srli a1, a3, 32\n"
         "or   a3, a3, a1\n"

         // 127 hword stores
         "addi a1, %[data], 0\n" // Pointer for stores
         "addi a2, %[data], 0\n" // Pointer for load check
         "addi a4, zero, 127\n"  // Loop counter
         "loop_sh:\n"
         "sh   a3, 0(a1)\n"
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "flw.ps f0, 0(a2)\n"    // Checks everything is ok
         "flw.ps f1, 16(a2)\n"
         "flw.ps f2, 32(a2)\n"
         "flw.ps f3, 48(a2)\n"
         "flw.ps f4, 64(a2)\n"
         "flw.ps f5, 80(a2)\n"
         "flw.ps f6, 96(a2)\n"
         "flw.ps f7, 112(a2)\n"
         "fsw.ps f16, 0(a2)\n"   // Restores to original memory contents
         "fsw.ps f17, 16(a2)\n"
         "fsw.ps f18, 32(a2)\n"
         "fsw.ps f19, 48(a2)\n"
         "fsw.ps f20, 64(a2)\n"
         "fsw.ps f21, 80(a2)\n"
         "fsw.ps f22, 96(a2)\n"
         "fsw.ps f23, 112(a2)\n"
         "addi a1, a1, 1\n"
         "add  a4, a4, -1\n"
         "bne  a4, zero, loop_sh\n"

         // 125 word stores
         "addi a1, %[data], 0\n" // Pointer for stores
         "addi a2, %[data], 0\n" // Pointer for load check
         "addi a4, zero, 125\n"  // Loop counter
         "loop_sw:\n"
         "sw   a3, 0(a1)\n"
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "flw.ps f0, 0(a2)\n"    // Checks everything is ok
         "flw.ps f1, 16(a2)\n"
         "flw.ps f2, 32(a2)\n"
         "flw.ps f3, 48(a2)\n"
         "flw.ps f4, 64(a2)\n"
         "flw.ps f5, 80(a2)\n"
         "flw.ps f6, 96(a2)\n"
         "flw.ps f7, 112(a2)\n"
         "fsw.ps f16, 0(a2)\n"   // Restores to original memory contents
         "fsw.ps f17, 16(a2)\n"
         "fsw.ps f18, 32(a2)\n"
         "fsw.ps f19, 48(a2)\n"
         "fsw.ps f20, 64(a2)\n"
         "fsw.ps f21, 80(a2)\n"
         "fsw.ps f22, 96(a2)\n"
         "fsw.ps f23, 112(a2)\n"
         "addi a1, a1, 1\n"
         "add  a4, a4, -1\n"
         "bne  a4, zero, loop_sw\n"

         // 121 dword stores
         "addi a1, %[data], 0\n" // Pointer for stores
         "addi a2, %[data], 0\n" // Pointer for load check
         "addi a4, zero, 121\n"  // Loop counter
         "loop_sd:\n"
         "sd   a3, 0(a1)\n"
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "flw.ps f0, 0(a2)\n"    // Checks everything is ok
         "flw.ps f1, 16(a2)\n"
         "flw.ps f2, 32(a2)\n"
         "flw.ps f3, 48(a2)\n"
         "flw.ps f4, 64(a2)\n"
         "flw.ps f5, 80(a2)\n"
         "flw.ps f6, 96(a2)\n"
         "flw.ps f7, 112(a2)\n"
         "fsw.ps f16, 0(a2)\n"   // Restores to original memory contents
         "fsw.ps f17, 16(a2)\n"
         "fsw.ps f18, 32(a2)\n"
         "fsw.ps f19, 48(a2)\n"
         "fsw.ps f20, 64(a2)\n"
         "fsw.ps f21, 80(a2)\n"
         "fsw.ps f22, 96(a2)\n"
         "fsw.ps f23, 112(a2)\n"
         "addi a1, a1, 1\n"
         "add  a4, a4, -1\n"
         "bne  a4, zero, loop_sd\n"

         // 112 128b stores
         "addi a1, %[data], 0\n" // Pointer for stores
         "addi a2, %[data], 0\n" // Pointer for load check
         "addi a4, zero, 112\n"  // Loop counter
         "loop_fsw_ps:\n"
         "fsw.ps f7, 0(a1)\n"
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "addi zero, zero, 0\n"  // We don't want to use bypasses to read the data, we want to read the cache
         "flw.ps f0, 0(a2)\n"    // Checks everything is ok
         "flw.ps f1, 16(a2)\n"
         "flw.ps f2, 32(a2)\n"
         "flw.ps f3, 48(a2)\n"
         "flw.ps f4, 64(a2)\n"
         "flw.ps f5, 80(a2)\n"
         "flw.ps f6, 96(a2)\n"
         "flw.ps f7, 112(a2)\n"
         "fsw.ps f16, 0(a2)\n"   // Restores to original memory contents
         "fsw.ps f17, 16(a2)\n"
         "fsw.ps f18, 32(a2)\n"
         "fsw.ps f19, 48(a2)\n"
         "fsw.ps f20, 64(a2)\n"
         "fsw.ps f21, 80(a2)\n"
         "fsw.ps f22, 96(a2)\n"
         "fsw.ps f23, 112(a2)\n"
         "addi a1, a1, 1\n"
         "add  a4, a4, -1\n"
         "bne  a4, zero, loop_fsw_ps\n"
         : 
         : [data] "r" (data)
         : "a1", "a2", "a3"
            );

   C_TEST_PASS;
   return 0;
}
