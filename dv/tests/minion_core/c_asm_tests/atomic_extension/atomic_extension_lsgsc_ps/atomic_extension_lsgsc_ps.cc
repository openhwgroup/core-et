/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

// Test: Atomic extension: Local & Global Vector Coherent Memory Operations:
// Local:
// FLWL.PS, FSWL.PS, FGWL.PS, FSCWL.PS
// (missing: FGBL.PS, FGHL.PS, FSCBL.PS, FSCHL.PS)
// Global:
// FLWG.PS, FSWG.PS, FGWG.PS, FSCWG.PS
// (missing: FGBG.PS, FGHG.PS, FSCBG.PS, FSCHG.PS)
//



#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "minion.h"
#include "et_test_common.h"
#include "cacheops.h"



int main()
{

   static uint32_t operands[48]  __attribute__ ((aligned (64)))= {
      0x00000000,           
      0x00000001,          
      0x00000002,           
      0x00000003,           
      0x00000004,
      0x00000005,
      0x00000006,
      0x00000007,
      0x00000008,                 
      0x00000009,                
      0x0000000a,                   
      0x0000000b,                 
      0x0000000c,
      0x0000000d,
      0x0000000e,
      0x0000000f,
      0x00000010,                
      0x00000012,                 
      0x00000013,                
      0x00000014,   
      0x00000015,
      0x00000016,
      0x00000017,
      0x00000018,
      0x00000019,
      0x0000001a,
      0x0000001b,
      0x0000001c,
      0x0000001d,
      0x0000001e,
      0x0000001f,
      0x00000020,
      0x00000021,                
      0x00000022,                 
      0x00000023,                
      0x00000024,   
      0x00000025,
      0x00000026,
      0x00000027,
      0x00000028,
      0x00000029,
      0x0000002a,
      0x0000002b,
      0x0000002c,
      0x0000002d,
      0x0000002e,
      0x0000002f,
      0x00000030 
   };

   static uint64_t err_cnt[1] __attribute__ ((aligned (64))) = {0};


   //volatile char __attribute__((aligned(4096))) ptr[4096];
   C_TEST_START;

   // (CYE) only run the following code in hart_id==0 (in order to do self-check):
   uint64_t hart_id = get_hart_id();

   if ((hart_id % 2) == 0) {    // change to use % modulo: easy to port to Minion #1, #2, #3, etc.

   #ifdef POSTSI_UST
     #ifdef GLOBAL_VEC_COH_MEM_OPS
       // need to initialize the 2 cachline address (%[operands]) in L3$ (by flush from L1->L3):
       // flush_va(uint64_t use_tmask, uint64_t dst, uint64_t addr, uint64_t num_lines, uint64_t stride, uint64_t id, uint64_t warl) 
       flush_va(0, 2, (uint64_t)operands, 1, 64, 0, 0);
       WAIT_CACHEOPS; 
     #endif
   #endif

   for (int i=1; i<10; i++) {  
      //int r = 999*i + 34*i;

      // Test
      __asm__ __volatile__ (
            "addi t4, %[operands], 0\n"     // t4 is the first  64-byte of operands[]
            "addi t3, t4, 64\n"             // t3 is the second 64-byte of operands[]
            "li   t1, 0x6831732103AC3FDE\n"
            "li   t6, 0x8000000000\n" // should have an offset role for gsc inst, changing from 0 to 0x8000000000
            "li   a2, 0xff\n"
            "mova.m.x a2\n"
            "li   s11, 0\n"   // error_count
            "fence \n"

            "addi t0, %[operands], 96\n"
            "addi t2, %[operands], 32\n"
            "sd t0, (t2)\n"
            "addi t0, %[operands], 100\n"
            "addi t2, %[operands], 36\n"
            "sd t0, (t2)\n"
            "addi t0, %[operands], 104\n"
            "addi t2, %[operands], 40\n"
            "sd t0, (t2)\n"
            "addi t0, %[operands], 108\n"
            "addi t2, %[operands], 44\n"
            "sd t0, (t2)\n"
            "addi t0, %[operands], 112\n"
            "addi t2, %[operands], 48\n"
            "sd t0, (t2)\n"
            "addi t0, %[operands], 116\n"
            "addi t2, %[operands], 52\n"
            "sd t0, (t2)\n"
            "addi t0, %[operands], 120\n"
            "addi t2, %[operands], 56\n"
            "sd t0, (t2)\n"

            "addi t0, %[operands], 124\n"
            "addi t2, %[operands], 60\n"
            "sd t0, (t2)\n"

            "flw.ps f0, 32(%[operands])\n"
            "flw.ps f1, 0(%[operands])\n"
            "fence\n"

            //---------- local (L2) vector coherent memory ops ----------------
         #ifdef LOCAL_VEC_COH_MEM_OPS
            "call flwlPS_local_scalar_test%=\n"
            "call flwlPS_local_scalar_selfcheck%=\n"  // (CYE) self-check "flwl.ps" return data
            "call delay%=\n"
//            "addi t6, t6, 1\n" // unnecessary increment, causes misalignment for gsc and trap vector not set

            "call fswlPS_local_scalar_test%=\n"
            "call selfcheck_flwlPS_vs_fswlPS_local_scalar%=\n"  // (CYE) self-check result of "fswl.ps"
            "call delay%=\n"
//            "addi t6, t6, 1\n"

            "call fscwlPS_local_scalar_test%=\n"
            "call delay%=\n"
            //"addi t6, t6, 32\n" // (CYE) changed to use same address for both fgwl.ps and fscwl.ps (in order to do self-check)

            "call fgwlPS_local_scalar_test%=\n"
            "call selfcheck_fgwlPS_vs_fscwlPS_local_scalar%=\n" // (CYE) self-check result of "fscwl.ps" using "fgwl.ps" 
            "call delay%=\n"
            "addi t6, t6, 32\n" //L/G GSC have to be 32-byte aligned  
         #endif // ifdef LOCAL_VEC_COH_MEM_OPS


            //---------- global (L3) vector coherent memory ops ----------------
         #ifdef GLOBAL_VEC_COH_MEM_OPS
            "call flwgPS_global_scalar_test%=\n"
            "call flwgPS_global_scalar_selfcheck%=\n"  // (CYE) self-check "flwg.ps" return data
            "call delay%=\n"
//            "addi t6, t6, 1\n"

            "call fswgPS_global_scalar_test%=\n"
            "call selfcheck_flwgPS_vs_fswgPS_global_scalar%=\n" // (CYE) self-check result of "fswg.ps"
            "call delay%=\n"
//            "addi t6, t6, 1\n"

            "call fscwgPS_global_scalar_test%=\n"
            "call delay%=\n"
            //"addi t6, t6, 32\n" // (CYE) changed to use same address for both fgwg.ps and fscwg.ps (in order to do self-check)

            "call fgwgPS_global_scalar_test%=\n"
            "call selfcheck_fgwgPS_vs_fscwgPS_local_scalar%=\n" // (CYE) self-check result of "fscwg.ps" using "fgwg.ps"
            "call delay%=\n"
            "addi t6, t6, 32\n"
         #endif // ifdef GLOBAL_VEC_COH_MEM_OPS

            "call end%=\n"


            //---------- local (L2) vector coherent memory ops ----------------
         #ifdef LOCAL_VEC_COH_MEM_OPS

            // FLWL.PS
            "flwlPS_local_scalar_test%=:\n"
            //"div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            //
            "flw.ps f2, (t3)\n"  // (CYE) first: save the data in f2 (assuming flw function is correct)
            //
            // (CYE) better to Evict_VA this cline (t3) out of L1D$ (to avoid different data in L1 and L2/L3); 
            "addi t0, zero, 0\n"      // not use tmask 
            "addi t1, zero, 1\n"      // evict_va from L1->Mem // change to: destination=L2 (in order to run in Post-Si Bringup Phase 1) 
            "slli t1, t1, 58\n"       // evict_va destination
            "or   t0, t0, t1\n"
            "addi t1, t3, 0\n"        // virtual address VA[47:0]
            "slli t1, t1, 6\n"
            "srli t1, t1, 6\n"        // zero out va[5:0]
            "or   t0, t0, t1\n"
            "addi t0, t0, 0\n"        // NumLines = 1
            "csrw evict_va, t0\n"     // CSR evict_va  // 0x89f
            "addi t1, zero, 6\n"
            "csrw tensor_wait, t1\n"  // Wait_CacheOp  // 0x830
            "fence\n"
            //
            "flwl.ps f1, (t3)\n"
            "nop\n"
            "ret\n"

            // (CYE) self-check flwl.ps returned data (in f1): compare f1 (returned data) and f2 (expected value)
            "flwlPS_local_scalar_selfcheck%=:\n"
            "feqm.ps m1, f1, f2\n"  // compare f1 vs f2; result in m1[7:0] (expecting 0xff)
            "mova.x.m s10\n"        // write into int reg s10 (note: m1 is on s10[15:8])
            "srli s10, s10, 8\n"    // shift-right-8bit: now m1 is on s10[7:0]
            "li s9, 0xff\n"         // expected value of compare result m1
            "bne s9, s10, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"    // error_count
            "2:\n"  
            "nop\n"
            "ret\n"


            // FSWL.PS
            "fswlPS_local_scalar_test%=:\n"
            //"div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "flw.ps  f3, 32(t4)\n" // 0-63(t4) are in L1
            "fswl.ps f3, (t3)\n" 
            "nop\n"
            "ret\n"

            // (CYE) self-check fswl.ps result: by doing a flwl.ps, and compare return data (f4) with fswl.ps write data (f3);
            "selfcheck_flwlPS_vs_fswlPS_local_scalar%=:\n"
            "flwl.ps f4, (t3)\n" 
            "feqm.ps m1, f4, f3\n"  // compare f4 vs f3; result in m1[7:0] (expecting 0xff)
            "mova.x.m s10\n"        // write into int reg s10 (note: m1 is on s10[15:8])
            "srli s10, s10, 8\n"    // shift-right-8bit: now m1 is on s10[7:0]
            "li s9, 0xff\n"         // expected value of compare result m1
            "bne s9, s10, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"    // error_count
            "2:\n"  
            "nop\n"
            "ret\n"


            // FSCWL.PS
            "fscwlPS_local_scalar_test%=:\n"
            //"div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "flw.ps  f3, 32(t4)\n"
            "fscwl.ps f3, f0(t6)\n"
            "nop\n"
            "ret\n"

            // FGWL.PS
            "fgwlPS_local_scalar_test%=:\n"
            //"div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "fgwl.ps f1, f0(t6)\n"
            "nop\n"
            "ret\n"

            // (CYE) self-check: use "fgwl.ps" return data (f1) to check against "fscwl.ps" write data (f3):
            "selfcheck_fgwlPS_vs_fscwlPS_local_scalar%=:\n"
            "feqm.ps m1, f1, f3\n"  // compare f1 vs f3; result in m1[7:0] (expecting 0xff)
            "mova.x.m s10\n"        // write into int reg s10 (note: m1 is on s10[15:8])
            "srli s10, s10, 8\n"    // shift-right-8bit: now m1 is on s10[7:0]
            "li s9, 0xff\n"         // expected value of compare result m1
            "bne s9, s10, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"    // error_count
            "2:\n"  
            "nop\n"
            "ret\n"

         #endif // ifdef LOCAL_VEC_COH_MEM_OPS


            //---------- global (L3) vector coherent memory ops ----------------
         #ifdef GLOBAL_VEC_COH_MEM_OPS
            
            // FLWG.PS
            "flwgPS_global_scalar_test%=:\n"
            //"div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            //
            "flw.ps f2, (t3)\n"  // (CYE) first: save the data in f2 (assuming flw function is correct)
            //
            // (CYE) better to Evict_VA this cline (t3) out of L1D$ (to avoid different data in L1 and L2/L3); 
            "addi t0, zero, 0\n"      // not use tmask 
            "addi t1, zero, 2\n"      // evict_va from L1->Mem // change to: destination=L3 (in order to run in Post-Si Bringup Phase 2) 
            "slli t1, t1, 58\n"       // evict_va destination 
            "or   t0, t0, t1\n"
            "addi t1, t3, 0\n"        // virtual address VA[47:0]
            "slli t1, t1, 6\n"
            "srli t1, t1, 6\n"        // zero out va[5:0]
            "or   t0, t0, t1\n"
            "addi t0, t0, 0\n"        // NumLines = 1
            "csrw evict_va, t0\n"     // CSR evict_va  // 0x89f
            "addi t1, zero, 6\n"
            "csrw tensor_wait, t1\n"  // Wait_CacheOp  // 0x830
            "fence\n"
            //
            "flwg.ps f1, (t3)\n"
            "nop\n"
            "ret\n"

            // (CYE) self-check flwg.ps returned data (in f1): compare f1 (returned data) and f2 (expected value)
            "flwgPS_global_scalar_selfcheck%=:\n"
            "feqm.ps m1, f1, f2\n"  // compare f1 vs f2; result in m1[7:0] (expecting 0xff)
            "mova.x.m s10\n"        // write into int reg s10 (note: m1 is on s10[15:8])
            "srli s10, s10, 8\n"    // shift-right-8bit: now m1 is on s10[7:0]
            "li s9, 0xff\n"         // expected value of compare result m1
            "bne s9, s10, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"    // error_count
            "2:\n"  
            "nop\n"
            "ret\n"


            // FSWG.PS
            "fswgPS_global_scalar_test%=:\n"
            //"div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "flw.ps  f3, 32(t4)\n"
            "fswg.ps f3, (t3)\n"
            "nop\n"
            "ret\n"

            // (CYE) self-check fswg.ps result: by doing a flwg.ps, and compare return data (f4) with fswg.ps write data (f3); 
            "selfcheck_flwgPS_vs_fswgPS_global_scalar%=:\n"
            "flwg.ps f4, (t3)\n"
            "feqm.ps m1, f4, f3\n"  // compare f4 vs f3; result in m1[7:0] (expecting 0xff)
            "mova.x.m s10\n"        // write into int reg s10 (note: m1 is on s10[15:8])
            "srli s10, s10, 8\n"    // shift-right-8bit: now m1 is on s10[7:0]
            "li s9, 0xff\n"         // expected value of compare result m1
            "bne s9, s10, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"    // error_count
            "2:\n"  
            "nop\n"
            "ret\n"


            // FSCWG.PS
            "fscwgPS_global_scalar_test%=:\n"
            //"div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "flw.ps  f3, 32(t4)\n"
            "fscwg.ps f3, f0(t6)\n"
            "nop\n"
            "ret\n"

            // FGWG.PS
            "fgwgPS_global_scalar_test%=:\n"
            //"div t4, t1, t6\n"
            //"li t2, 0x4400\n"
            "fgwg.ps f1, f0(t6)\n"
            "nop\n"
            "ret\n"

            // (CYE) self-check: use "fgwg.ps" return data (f1) to check against "fscwg.ps" write data (f3):
            "selfcheck_fgwgPS_vs_fscwgPS_local_scalar%=:\n"
            "feqm.ps m1, f1, f3\n"  // compare f1 vs f3; result in m1[7:0] (expecting 0xff)
            "mova.x.m s10\n"        // write into int reg s10 (note: m1 is on s10[15:8])
            "srli s10, s10, 8\n"    // shift-right-8bit: now m1 is on s10[7:0]
            "li s9, 0xff\n"         // expected value of compare result m1
            "bne s9, s10, 1f\n"
            "j 2f\n"
            "1: \n"
            "addi s11, s11, 1\n"    // error_count
            "2:\n"  
            "nop\n"
            "ret\n"

         #endif // ifdef GLOBAL_VEC_COH_MEM_OPS


            "delay%=: \n"
            "li t5, 32\n"
            "loop_delay%=: \n"
            "add t5, t5, -1\n"
            "bne t5, zero, loop_delay%=\n"
            "ret\n"

            "end%=: \n"
            "nop\n"
            :
            : [operands] "r" (operands)//, "n" (r)
            : "t0", "t1", "t2", "t3", "t4", "t5", "t6", "f0", "f1", "f2", "f3", "f4", "a2", "s9", "s10", "s11"
      );


      // check s11 -> err_cnt:
      __asm__ __volatile__ (
          //"bgtz s11, L2\n"
          //"j L3\n"
          //"L2:\n"
            "sw s11, 0(%[err_cnt])\n"
          //"L3:\n"
            :
            : [err_cnt] "r" (err_cnt)
            : "s11", "memory"
      );

      if (err_cnt[0] > 0) {
         et_printf("Test self-check: err_cnt > 0\n"); 
         C_TEST_FAIL;
      }
      //else {
      //   et_printf("Test self-check: no_err\n"); 
      //}


   } // for()

   } // if (hart_id == 0)

 
   C_TEST_PASS;
   return 0;
}
