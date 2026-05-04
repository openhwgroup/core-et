/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

int main() {

   static uint32_t operands[256] __attribute__ ((aligned (16))) = {

      // vpu register f0
      0x80011111,    // -9.7958E-41           
      0x80000000,    // -zero                 
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            

      // vpu register f1
      0x7fc00000,    // quiet NaN             
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45              
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0xff800000,    // -inf                  

      // vpu register f2
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  
      0x80000001,    // -1.4E-45              
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 

      // vpu register f3
      0x00000001,    // 1.4E-45               
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0x80000000,    // -zero                 
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   
      0x7fc00000,    // quiet NaN             

      // vpu register f4
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0x7fc00000,    // quiet NaN             
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x4b000000,    // 8388608.0             

      // vpu register f5
      0xffc00000,    // -quiet NaN            
      0x7f800000,    // inf                   
      0x7f800000,    // inf                   
      0x00000000,    // zero                  
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0xffc00001,    // -signaling NaN        

      // vpu register f6
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        
      0xffc00001,    // -signaling NaN        

      // vpu register f7
      0x00000001,    // 1.4E-45               
      0x4b8c4b40,    // 18388608.0            
      0xff800000,    // -inf                  
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   

      // vpu register f8
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45              
      0x80011111,    // -9.7958E-41           
      0x80000000,    // -zero                 
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            

      // vpu register f9
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0x00000000,    // zero                  
      0xff800000,    // -inf                  
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0x80000001,    // -1.4E-45              
      0x7f800000,    // inf                   

      // vpu register f10
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0x80000000,    // -zero                 

      // vpu register f11
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0x007fffff,    // 1.1754942E-38         
      0x00000000,    // zero                  
      0xbf800000,    // -1.0                  

      // vpu register f12
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x00000000,    // zero                  
      0x00000001,    // 1.4E-45               
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x00000000,    // zero                  
      0xffc00000,    // -quiet NaN            

      // vpu register f13
      0x3f800000,    // 1.0                   
      0xff800000,    // -inf                  
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45               
      0xffc00000,    // -quiet NaN            
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0x80000000,    // -zero                 

      // vpu register f14
      0x00000001,    // 1.4E-45               
      0xbf028f5c,    // -0.51                 
      0xffc00000,    // -quiet NaN            
      0x80000000,    // -zero                 
      0xcb000000,    // -8388608.0            
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            

      // vpu register f15
      0x4b8c4b40,    // 18388608.0            
      0xffc00001,    // -signaling NaN        
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  
      0x00000001,    // 1.4E-45               

      // vpu register f16
      0x00000001,    // 1.4E-45               
      0x80000001,    // -1.4E-45              
      0x80000001,    // -1.4E-45              
      0x80000000,    // -zero                 
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0xffc00000,    // -quiet NaN            
      0x00011111,    // 9.7958E-41            

      // vpu register f17
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0x00000001,    // 1.4E-45               
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45               
      0xbf800000,    // -1.0                  

      // vpu register f18
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45               
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45               
      0x7f800000,    // inf                   
      0xffc00000,    // -quiet NaN            
      0x4b8c4b40,    // 18388608.0            

      // vpu register f19
      0x80000001,    // -1.4E-45              
      0x4b8c4b40,    // 18388608.0            
      0xbf800000,    // -1.0                  
      0x80000001,    // -1.4E-45              
      0x3f800000,    // 1.0                   
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0x00000000,    // zero                  

      // vpu register f20
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45               
      0xffc00000,    // -quiet NaN            
      0x80000000,    // -zero                 
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45               

      // vpu register f21
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0x4b000000,    // 8388608.0             
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0x00000000,    // zero                  

      // vpu register f22
      0x00000000,    // zero                  
      0xbf028f5c,    // -0.51                 
      0x00000000,    // zero                  
      0x3f800000,    // 1.0                   
      0x80000000,    // -zero                 
      0x3f800000,    // 1.0                   
      0x80000000,    // -zero                 
      0xffc00001,    // -signaling NaN        

      // vpu register f23
      0xbf800000,    // -1.0                  
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0x80000000,    // -zero                 
      0x80000000,    // -zero                 
      0xcb8c4b40,    // -18388608.0           

      // vpu register f24
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   

      // vpu register f25
      0x00000000,    // zero                  
      0xcb000000,    // -8388608.0            
      0x7f800000,    // inf                   
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45               
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0xffc00001,    // -signaling NaN        

      // vpu register f26
      0x7fc00001,    // signaling NaN         
      0x7fc00001,    // signaling NaN         
      0x007fffff,    // 1.1754942E-38         
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0xffc00000,    // -quiet NaN            

      // vpu register f27
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            

      // vpu register f28
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             
      0xbf800000,    // -1.0                  
      0xff800000,    // -inf                  
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            
      0x80000000,    // -zero                 
      0xffc00000,    // -quiet NaN            

      // vpu register f29
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0xffc00001,    // -signaling NaN        
      0xffc00001,    // -signaling NaN        
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0x00000000,    // zero                  

      // vpu register f30
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   
      0x80000001,    // -1.4E-45              
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0xcb8c4b40,    // -18388608.0           

      // vpu register f31
      0x80000000,    // -zero                 
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   
      0x80000000,    // -zero                 
      0x7fc00001,    // signaling NaN         
      0x3f800000     // 1.0                   
   };

   __asm__ __volatile__ (
         // ptr to memory where th ematrices are stored
         "li x10,  0x805f9600\n"

         // return status 
         "li x11,  0xFFFFFFFF\n"
         "li x12,  0xFFFFFFFF\n"
         "li x13,  0xFFFFFFFF\n"
         "li x14,  0xFFFFFFFF\n"

         // for eviction/flush
         "li x15,  0x0C0000000000000F\n"   // way0 16 lines
         "li x16,  0x0C0000000000004F\n"   // way1 16 lines
         "li x17,  0x0C0000000000008F\n"   // way2 16 lines
         "li x18,  0x0C000000000000CF\n"   // way3 16 lines

         "li x26,  0x805f960b\n"
         "li x27,  0x0\n"
         "li x28,  0x1\n"
         "li x29,  0x0048000000000007\n"
         "li x31,  0x40\n"

         "li x3, 0xffffffffffffffff\n"
         "mova.m.x x3\n"

         // load from operands array into fp regs
         "flw.ps f0,  0 (%[operands])\n"
         "flw.ps f1,  32 (%[operands])\n"
         "flw.ps f2,  64 (%[operands])\n"
         "flw.ps f3,  96 (%[operands])\n"
         "flw.ps f4,  128 (%[operands])\n"
         "flw.ps f5,  160 (%[operands])\n"
         "flw.ps f6,  192 (%[operands])\n"
         "flw.ps f7,  224 (%[operands])\n"
         "flw.ps f8,  256 (%[operands])\n"
         "flw.ps f9,  288 (%[operands])\n"
         "flw.ps f10,  320 (%[operands])\n"
         "flw.ps f11,  352 (%[operands])\n"
         "flw.ps f12,  384 (%[operands])\n"
         "flw.ps f13,  416 (%[operands])\n"
         "flw.ps f14,  448 (%[operands])\n"
         "flw.ps f15,  480 (%[operands])\n"
         "flw.ps f16,  512 (%[operands])\n"
         "flw.ps f17,  544 (%[operands])\n"
         "flw.ps f18,  576 (%[operands])\n"
         "flw.ps f19,  608 (%[operands])\n"
         "flw.ps f20,  640 (%[operands])\n"
         "flw.ps f21,  672 (%[operands])\n"
         "flw.ps f22,  704 (%[operands])\n"
         "flw.ps f23,  736 (%[operands])\n"
         "flw.ps f24,  768 (%[operands])\n"
         "flw.ps f25,  800 (%[operands])\n"
         "flw.ps f26,  832 (%[operands])\n"
         "flw.ps f27,  864 (%[operands])\n"
         "flw.ps f28,  896 (%[operands])\n"
         "flw.ps f29,  928 (%[operands])\n"
         "flw.ps f30,  960 (%[operands])\n"
         "flw.ps f31,  992 (%[operands])\n"
         "fence\n"

         // store fp reg values to designated mem location (matrix location)
         "fsw.ps f0,  0    (x10)\n"    // for tensor ops
         "fsw.ps f1,  32   (x10)\n"    // for tensor ops
         "fsw.ps f2,  64   (x10)\n"    // for tensor ops
         "fsw.ps f3,  96   (x10)\n"    // for tensor ops
         "fsw.ps f4,  128  (x10)\n"    // for tensor ops
         "fsw.ps f5,  160  (x10)\n"    // for tensor ops
         "fsw.ps f6,  192  (x10)\n"    // for tensor ops
         "fsw.ps f7,  224  (x10)\n"    // for tensor ops
         "fsw.ps f8,  256  (x10)\n"    // for tensor ops
         "fsw.ps f9,  288  (x10)\n"    // for tensor ops
         "fsw.ps f10,  320 (x10)\n"    // for tensor ops
         "fsw.ps f11,  352 (x10)\n"    // for tensor ops
         "fsw.ps f12,  384 (x10)\n"    // for tensor ops
         "fsw.ps f13,  416 (x10)\n"    // for tensor ops
         "fsw.ps f14,  448 (x10)\n"    // for tensor ops
         "fsw.ps f15,  480 (x10)\n"    // for tensor ops
         "fsw.ps f16,  512 (x10)\n"    // for tensor ops
         "fsw.ps f17,  544 (x10)\n"    // for tensor ops
         "fsw.ps f18,  576 (x10)\n"    // for tensor ops
         "fsw.ps f19,  608 (x10)\n"    // for tensor ops
         "fsw.ps f20,  640 (x10)\n"    // for tensor ops
         "fsw.ps f21,  672 (x10)\n"    // for tensor ops
         "fsw.ps f22,  704 (x10)\n"    // for tensor ops
         "fsw.ps f23,  736 (x10)\n"    // for tensor ops
         "fsw.ps f24,  768 (x10)\n"    // for tensor ops
         "fsw.ps f25,  800 (x10)\n"    // for tensor ops
         "fsw.ps f26,  832 (x10)\n"    // for tensor ops
         "fsw.ps f27,  864 (x10)\n"    // for tensor ops
         "fsw.ps f28,  896 (x10)\n"    // for tensor ops
         "fsw.ps f29,  928 (x10)\n"    // for tensor ops
         "fsw.ps f30,  960 (x10)\n"    // for tensor ops
         "fsw.ps f31,  992 (x10)\n"    // for tensor ops
         "fence\n"
         /// now evict all those cachelines out to memory
         // Evict all cachelines
         "li   x11, 0x3400000000000000\n" // Evict SW
         "li   x12, 0x1000000000000\n"    // 1 way advance
         "li   x13, 0x4000000000000\n"    // Clear way
         "addi x14, zero, 16\n"       // 16 sets
         "loop_esw:\n"
         "csrw 0x51f, x11\n"          // Evict SW (way 0)
         "add  x11, x11, x12\n"
         "csrw 0x51f, x11\n"          // Evict SW (way 1)
         "add  x11, x11, x12\n"
         "csrw 0x51f, x11\n"          // Evict SW (way 2)
         "add  x11, x11, x12\n"
         "csrw 0x51f, x11\n"          // Evict SW (way 3)
         "add  x11, x11, 64\n"         // Move to next set
         "xor  x11, x11, x13\n"         // Move to way 0
         "addi x14, x14, -1\n"         // Inc counter
         "bne  x14, zero, loop_esw\n"

         //"csrrw x11, 0x9c0, x15\n"   // evict lines
         //"csrrw x12, 0x9c0, x16\n"   // evict lines
         //"csrrw x13, 0x9c0, x17\n"   // evict lines
         //"csrrw x14, 0x9c0, x18\n"   // evict lines
         "fence\n"

         //  zero out the fp regs
         "fcvt.s.w f0,   zero\n"   
         "fcvt.s.w f1,   zero\n"   
         "fcvt.s.w f2,   zero\n"   
         "fcvt.s.w f3,   zero\n"   
         "fcvt.s.w f4,   zero\n"   
         "fcvt.s.w f5,   zero\n"   
         "fcvt.s.w f6,   zero\n"   
         "fcvt.s.w f7,   zero\n"   
         "fcvt.s.w f8,   zero\n"   
         "fcvt.s.w f9,   zero\n"   
         "fcvt.s.w f10,  zero\n"   
         "fcvt.s.w f11,  zero\n"   
         "fcvt.s.w f12,  zero\n"   
         "fcvt.s.w f13,  zero\n"   
         "fcvt.s.w f14,  zero\n"   
         "fcvt.s.w f15,  zero\n"   
         "fcvt.s.w f16,  zero\n"   
         "fcvt.s.w f17,  zero\n"   
         "fcvt.s.w f18,  zero\n"   
         "fcvt.s.w f19,  zero\n"   
         "fcvt.s.w f20,  zero\n"   
         "fcvt.s.w f21,  zero\n"   
         "fcvt.s.w f22,  zero\n"   
         "fcvt.s.w f23,  zero\n"   
         "fcvt.s.w f24,  zero\n"   
         "fcvt.s.w f25,  zero\n"   
         "fcvt.s.w f26,  zero\n"   
         "fcvt.s.w f27,  zero\n"   
         "fcvt.s.w f28,  zero\n"   
         "fcvt.s.w f29,  zero\n"   
         "fcvt.s.w f30,  zero\n"   
         "fcvt.s.w f31,  zero\n"   
         "fence\n"
         /*
         // sanity check to see if the stores successfully wrote to the memory, by loading from that memory location
         // these values should be the exact same as before
         "flw.ps f0 ,  0    (x10)\n"    // for tensor ops
         "flw.ps f1 ,  32   (x10)\n"    // for tensor ops
         "flw.ps f2 ,  64   (x10)\n"    // for tensor ops
         "flw.ps f3 ,  96   (x10)\n"    // for tensor ops
         "flw.ps f4 ,  128  (x10)\n"    // for tensor ops
         "flw.ps f5 ,  160  (x10)\n"    // for tensor ops
         "flw.ps f6 ,  192  (x10)\n"    // for tensor ops
         "flw.ps f7 ,  224  (x10)\n"    // for tensor ops
         "flw.ps f8 ,  256  (x10)\n"    // for tensor ops
         "flw.ps f9 ,  288  (x10)\n"    // for tensor ops
         "flw.ps f10,  320 (x10)\n"    // for tensor ops
         "flw.ps f11,  352 (x10)\n"    // for tensor ops
         "flw.ps f12,  384 (x10)\n"    // for tensor ops
         "flw.ps f13,  416 (x10)\n"    // for tensor ops
         "flw.ps f14,  448 (x10)\n"    // for tensor ops
         "flw.ps f15,  480 (x10)\n"    // for tensor ops
         "flw.ps f16,  512 (x10)\n"    // for tensor ops
         "flw.ps f17,  544 (x10)\n"    // for tensor ops
         "flw.ps f18,  576 (x10)\n"    // for tensor ops
         "flw.ps f19,  608 (x10)\n"    // for tensor ops
         "flw.ps f20,  640 (x10)\n"    // for tensor ops
         "flw.ps f21,  672 (x10)\n"    // for tensor ops
         "flw.ps f22,  704 (x10)\n"    // for tensor ops
         "flw.ps f23,  736 (x10)\n"    // for tensor ops
         "flw.ps f24,  768 (x10)\n"    // for tensor ops
         "flw.ps f25,  800 (x10)\n"    // for tensor ops
         "flw.ps f26,  832 (x10)\n"    // for tensor ops
         "flw.ps f27,  864 (x10)\n"    // for tensor ops
         "flw.ps f28,  896 (x10)\n"    // for tensor ops
         "flw.ps f29,  928 (x10)\n"    // for tensor ops
         "flw.ps f30,  960 (x10)\n"    // for tensor ops
         "flw.ps f31,  992 (x10)\n"    // for tensor ops
         "fence\n"
         /// now evict all those cachelines out to memory
         "csrrw x11, 0x9c0, x15\n"   // evict lines
         "csrrw x12, 0x9c0, x16\n"   // evict lines
         "csrrw x13, 0x9c0, x17\n"   // evict lines
         "csrrw x14, 0x9c0, x18\n"   // evict lines
         "fence\n"
         */        
         "csrrw x11, 0x810, x28\n"   // set scratchpad control     
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
         "fence\n"
         "csrrw x12, 0x83F, x26\n"   // tensorload to scratchpad     
         "fence\n"
         "csrrw x13, 0x830, x27\n"   // tensorwait for tensor load to finish
         "fence\n"
         "csrrw x14, 0x801, x29\n"     // tima    
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x3", "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x26", "x27", "x28", "x29", "x30", "x31", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
            );
   C_TEST_PASS;
   return 0;
}
