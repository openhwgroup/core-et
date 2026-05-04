/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <inttypes.h>
#include <stdexcept>



int main()
{
   static uint32_t operands[16]  __attribute__ ((aligned (16)))= {
      0x11111111,
      0xF0F0F0F0,
      0x00000000,
      0x0F0F0F0F,
      0x01010101,
      0xFFFFFFFF,
      0xFFFF0000,
      0x0000FFFF,
      0xFF00FF00,
      0x00FF00FF,
      0x10101010,
      0xFFFFFFFF,
      0x00000000,
      0xFF000000,
      0x000000FF,
      0x000FF000
   };
   __asm__ __volatile__ (

         "flw.ps ft0, 0(%[operands])\n"
         "flw.ps ft1, 32(%[operands])\n"

         "li t5, 0xc001\n"

         // machine trap-handler base address
         "la t0, trap_vector\n"
         "csrw mtvec,t0\n"

         "addi t3, t3, 24\n"
         "fremu.pi   ft6,ft0,ft1\n"       //fdiv.pi
         "fmv.x.s t6, f6\n"
         "bne	t6,t5,fail\n"


         "call pass\n"


         ".align 12\n" //trap vector aligned to 4k... restriction of Xtvec CSRs
         "trap_vector:\n"
         "csrrw sp, mscratch, sp\n"                  // mscratch <- sp
         "csrr a1, mcause\n"                         // a1 <- mcause
         "li a0, 0x1e\n" 
         //  bne a1,a0,fail                        # check cause is m-code
         "bgez a1, .Lhandle_trap_in_machine_mode\n"  // handle trap in machine mode


         ".Lhandle_trap_in_machine_mode:\n"
         // Preserve the registers.  Compute the address of the trap handler

         "csrr a2, mepc\n"                    // a2   <- mepc
         "addi a3, a2,4\n"                    // a3   <- mepc+4
         "csrw mepc, a3\n"                    // mepc <- a3                
         "csrrw t0, mscratch, x0\n"           // t0   <- user sp

         // decode instructions. should decode instruction bits, 
         // using predefined t6 instead
         "la t4, mcode_table\n"  // address of mcode_table
         "add t1, t3, t4\n"
         "lw t0, 0(t1)\n"
         "jr t0\n"               // jump to mcode_table


         "mcode_table:\n"
         ".word mcode_fcvt_s_l\n"
         ".word mcode_fcvt_s_lu\n"
         ".word mcode_fcvt_l_s\n"
         ".word mcode_fcvt_lu_s\n"
         ".word mcode_fdiv_pi\n"
         ".word mcode_fdivu_pi\n"
         ".word mcode_fremu_pi\n"
         ".word mcode_frem_pi\n"
         ".word mcode_fdiv_s\n"
         ".word mcode_fdiv_ps\n"
         ".word mcode_fsqrt_s\n"
         ".word mcode_fsqrt_ps\n"
         ".word mcode_instmcode_ps\n"


         "mcode_fcvt_s_l:\n"
         "fmv.s.x f0, t5\n"
         "call Lmret\n"       // machine mode return


         "mcode_fcvt_s_lu:\n"
         "fmv.s.x f1, t5\n"
         "call Lmret\n"       // machine mode return

         "mcode_fcvt_l_s:\n"
         "fmv.s.x f2, t5\n"
         "call Lmret\n"       // machine mode return

         "mcode_fcvt_lu_s:\n"
         "fmv.s.x f3, t5\n"
         "call Lmret\n"       // machine mode return

         "mcode_fdiv_pi:\n"
         "fmv.s.x f4, t5\n"
         "call Lmret\n"       // machine mode return

         "mcode_fdivu_pi:\n"
         "fmv.s.x f5, t5\n"
         "call Lmret\n"       // machine mode return  

         "mcode_fremu_pi:\n"
         "fmv.s.x f6, t5\n"
         "call Lmret\n"       // machine mode return  

         "mcode_frem_pi:\n"
         "fmv.s.x f7, t5\n"
         "call Lmret\n"       // machine mode return  

         "mcode_fdiv_s:\n"
         "fmv.s.x f8, t5\n"
         "call Lmret\n"       // machine mode return  

         "mcode_fdiv_ps:\n"
         "fmv.s.x f9, t5\n"
         "call Lmret\n"       // machine mode return  

         "mcode_fsqrt_s:\n"
         "fmv.s.x f10, t5\n"
         "call Lmret\n"       // machine mode return  

         "mcode_fsqrt_ps:\n"
         "fmv.s.x f11, t5\n"
         "call Lmret\n"       // machine mode return  

         "mcode_instmcode_ps:\n"
         "fmv.s.x f12, t5\n"
         "call Lmret\n"       // machine mode return  


         "Lmret:\n"
         // Go back where we came.
         "csrrw sp, mscratch, sp\n"
         "mret\n"


         "fail:\n"
         "slli	a0,t3,0x1\n"
         "ori	a0,a0,1\n"
         "wfi\n"

         "pass:\n"
         //  ecall





         // Asm output results to C code
         : 
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7","f8","f9","f10","f11","f12", "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7","ft8","ft9","ft10","ft11", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "a0", "a1", "a2", "a3", "x0"
            );


   C_TEST_PASS;
   return 0;
}
