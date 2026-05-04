/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

   volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

      // vpu register f0
      0x0,    // -0.51                 
      0x4,    // -signaling NaN        
      0x8,    // -quiet NaN            
      0xc,    // -18388608.0           
      0x10,    // 9.7958E-41            
      0x14,    // signaling NaN         
      0x18,    // -zero                 
      0x1c,    // -quiet NaN            

      // vpu register f1
      0x80000000,    // -zero                 
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  

      // vpu register f2
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0xcb8c4b40    // -18388608.0           

   };
   C_TEST_START;
   __asm__ __volatile__ (
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "flw.ps f22,  0 (%[operands])\n"
         "flw.ps f26,  32 (%[operands])\n"
         "flw.ps f29,  64 (%[operands])\n"
         "li x26,  0x805f9668\n"
         "fsw.ps f26,   0 (x26)\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "nop\n"
         "famominl.pi f29, f22(x26)\n"  // opcode=0x2fab4e8b
         "flw.ps f21, 88(x26)\n"        // opcode=0xfd8d2a8b
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f21", "f22", "f26", "f29"
            );
   C_TEST_PASS;
   return 0;
}
