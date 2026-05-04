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
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   

      // vpu register f1
      0xbf800000,    // -1.0                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f2
      0x4b8c4b40,    // 18388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   

      // vpu register f3
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f4
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            

      // vpu register f5
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 

      // vpu register f6
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0xbf800000,    // -1.0                  
      0x4b000000,    // 8388608.0             
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  

      // vpu register f7
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         

      // vpu register f8
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            

      // vpu register f9
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            

      // vpu register f10
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b8c4b40,    // 18388608.0            

      // vpu register f11
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   

      // vpu register f12
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0xbf800000,    // -1.0                  

      // vpu register f13
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  

      // vpu register f14
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  

      // vpu register f15
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            

      // vpu register f16
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0xcb8c4b40,    // -18388608.0           

      // vpu register f17
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            

      // vpu register f18
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           

      // vpu register f19
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f20
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f21
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           

      // vpu register f22
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  

      // vpu register f23
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b000000,    // 8388608.0             
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            

      // vpu register f24
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  

      // vpu register f25
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  

      // vpu register f26
      0xbf800000,    // -1.0                  
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         

      // vpu register f27
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 

      // vpu register f28
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   

      // vpu register f29
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f30
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         

      // vpu register f31
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0x3f800000     // 1.0                   
   };
   C_TEST_START;
   __asm__ __volatile__ (
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
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
         "li x0,  0x80565500\n"
         "li x1,  0x80def6a0\n"
         "li x2,  0x80e585a0\n"
         "li x3,  0x80af0620\n"
         "li x4,  0x806ed3c0\n"
         "li x5,  0x80ca8960\n"
         "li x6,  0x80f6dbe0\n"
         "li x7,  0x802a7280\n"
         "li x8,  0x80841900\n"
         "li x9,  0x8023d4e0\n"
         "li x10,  0x803cf7a0\n"
         "li x11,  0x80d81ac0\n"
         "li x12,  0x80d0f4c0\n"
         "li x13,  0x801d0a60\n"
         "li x14,  0x80d51b80\n"
         "li x15,  0x80aca300\n"
         "li x16,  0x80e58f00\n"
         "li x17,  0x807067e0\n"
         "li x18,  0x80e09e60\n"
         "li x19,  0x80440d20\n"
         "li x20,  0x805828c0\n"
         "li x21,  0x803e8aa0\n"
         "li x22,  0x80515520\n"
         "li x23,  0x807f5660\n"
         "li x24,  0x80702e40\n"
         "li x25,  0x80876640\n"
         "li x26,  0x805af3e0\n"
         "li x27,  0x8033db60\n"
         "li x28,  0x803a5c60\n"
         "li x29,  0x802fed60\n"
         "li x30,  0x809c6da0\n"
         "li x31,  0x80643840\n"
         "fsw.ps f0,   0 (x0)\n"
         "fsw.ps f0,   32 (x1)\n"
         "fsw.ps f0,   64 (x2)\n"
         "fsw.ps f0,  0 (%[operands])\n"    // for tensor ops
         "fsw.ps f1,   0 (x1)\n"
         "fsw.ps f1,   32 (x2)\n"
         "fsw.ps f1,   64 (x3)\n"
         "fsw.ps f1,  32 (%[operands])\n"    // for tensor ops
         "fsw.ps f2,   0 (x2)\n"
         "fsw.ps f2,   32 (x3)\n"
         "fsw.ps f2,   64 (x4)\n"
         "fsw.ps f2,  64 (%[operands])\n"    // for tensor ops
         "fsw.ps f3,   0 (x3)\n"
         "fsw.ps f3,   32 (x4)\n"
         "fsw.ps f3,   64 (x5)\n"
         "fsw.ps f3,  96 (%[operands])\n"    // for tensor ops
         "fsw.ps f4,   0 (x4)\n"
         "fsw.ps f4,   32 (x5)\n"
         "fsw.ps f4,   64 (x6)\n"
         "fsw.ps f4,  128 (%[operands])\n"    // for tensor ops
         "fsw.ps f5,   0 (x5)\n"
         "fsw.ps f5,   32 (x6)\n"
         "fsw.ps f5,   64 (x7)\n"
         "fsw.ps f5,  160 (%[operands])\n"    // for tensor ops
         "fsw.ps f6,   0 (x6)\n"
         "fsw.ps f6,   32 (x7)\n"
         "fsw.ps f6,   64 (x8)\n"
         "fsw.ps f6,  192 (%[operands])\n"    // for tensor ops
         "fsw.ps f7,   0 (x7)\n"
         "fsw.ps f7,   32 (x8)\n"
         "fsw.ps f7,   64 (x9)\n"
         "fsw.ps f7,  224 (%[operands])\n"    // for tensor ops
         "fsw.ps f8,   0 (x8)\n"
         "fsw.ps f8,   32 (x9)\n"
         "fsw.ps f8,   64 (x10)\n"
         "fsw.ps f8,  256 (%[operands])\n"    // for tensor ops
         "fsw.ps f9,   0 (x9)\n"
         "fsw.ps f9,   32 (x10)\n"
         "fsw.ps f9,   64 (x11)\n"
         "fsw.ps f9,  288 (%[operands])\n"    // for tensor ops
         "fsw.ps f10,   0 (x10)\n"
         "fsw.ps f10,   32 (x11)\n"
         "fsw.ps f10,   64 (x12)\n"
         "fsw.ps f10,  320 (%[operands])\n"    // for tensor ops
         "fsw.ps f11,   0 (x11)\n"
         "fsw.ps f11,   32 (x12)\n"
         "fsw.ps f11,   64 (x13)\n"
         "fsw.ps f11,  352 (%[operands])\n"    // for tensor ops
         "fsw.ps f12,   0 (x12)\n"
         "fsw.ps f12,   32 (x13)\n"
         "fsw.ps f12,   64 (x14)\n"
         "fsw.ps f12,  384 (%[operands])\n"    // for tensor ops
         "fsw.ps f13,   0 (x13)\n"
         "fsw.ps f13,   32 (x14)\n"
         "fsw.ps f13,   64 (x15)\n"
         "fsw.ps f13,  416 (%[operands])\n"    // for tensor ops
         "fsw.ps f14,   0 (x14)\n"
         "fsw.ps f14,   32 (x15)\n"
         "fsw.ps f14,   64 (x16)\n"
         "fsw.ps f14,  448 (%[operands])\n"    // for tensor ops
         "fsw.ps f15,   0 (x15)\n"
         "fsw.ps f15,   32 (x16)\n"
         "fsw.ps f15,   64 (x17)\n"
         "fsw.ps f15,  480 (%[operands])\n"    // for tensor ops
         "fsw.ps f16,   0 (x16)\n"
         "fsw.ps f16,   32 (x17)\n"
         "fsw.ps f16,   64 (x18)\n"
         "fsw.ps f16,  512 (%[operands])\n"    // for tensor ops
         "fsw.ps f17,   0 (x17)\n"
         "fsw.ps f17,   32 (x18)\n"
         "fsw.ps f17,   64 (x19)\n"
         "fsw.ps f17,  544 (%[operands])\n"    // for tensor ops
         "fsw.ps f18,   0 (x18)\n"
         "fsw.ps f18,   32 (x19)\n"
         "fsw.ps f18,   64 (x20)\n"
         "fsw.ps f18,  576 (%[operands])\n"    // for tensor ops
         "fsw.ps f19,   0 (x19)\n"
         "fsw.ps f19,   32 (x20)\n"
         "fsw.ps f19,   64 (x21)\n"
         "fsw.ps f19,  608 (%[operands])\n"    // for tensor ops
         "fsw.ps f20,   0 (x20)\n"
         "fsw.ps f20,   32 (x21)\n"
         "fsw.ps f20,   64 (x22)\n"
         "fsw.ps f20,  640 (%[operands])\n"    // for tensor ops
         "fsw.ps f21,   0 (x21)\n"
         "fsw.ps f21,   32 (x22)\n"
         "fsw.ps f21,   64 (x23)\n"
         "fsw.ps f21,  672 (%[operands])\n"    // for tensor ops
         "fsw.ps f22,   0 (x22)\n"
         "fsw.ps f22,   32 (x23)\n"
         "fsw.ps f22,   64 (x24)\n"
         "fsw.ps f22,  704 (%[operands])\n"    // for tensor ops
         "fsw.ps f23,   0 (x23)\n"
         "fsw.ps f23,   32 (x24)\n"
         "fsw.ps f23,   64 (x25)\n"
         "fsw.ps f23,  736 (%[operands])\n"    // for tensor ops
         "fsw.ps f24,   0 (x24)\n"
         "fsw.ps f24,   32 (x25)\n"
         "fsw.ps f24,   64 (x26)\n"
         "fsw.ps f24,  768 (%[operands])\n"    // for tensor ops
         "fsw.ps f25,   0 (x25)\n"
         "fsw.ps f25,   32 (x26)\n"
         "fsw.ps f25,   64 (x27)\n"
         "fsw.ps f25,  800 (%[operands])\n"    // for tensor ops
         "fsw.ps f26,   0 (x26)\n"
         "fsw.ps f26,   32 (x27)\n"
         "fsw.ps f26,   64 (x28)\n"
         "fsw.ps f26,  832 (%[operands])\n"    // for tensor ops
         "fsw.ps f27,   0 (x27)\n"
         "fsw.ps f27,   32 (x28)\n"
         "fsw.ps f27,   64 (x29)\n"
         "fsw.ps f27,  864 (%[operands])\n"    // for tensor ops
         "fsw.ps f28,   0 (x28)\n"
         "fsw.ps f28,   32 (x29)\n"
         "fsw.ps f28,   64 (x30)\n"
         "fsw.ps f28,  896 (%[operands])\n"    // for tensor ops
         "fsw.ps f29,   0 (x29)\n"
         "fsw.ps f29,   32 (x30)\n"
         "fsw.ps f29,   64 (x31)\n"
         "fsw.ps f29,  928 (%[operands])\n"    // for tensor ops
         "fsw.ps f30,   0 (x30)\n"
         "fsw.ps f30,   32 (x31)\n"
         "fsw.ps f30,   64 (x0)\n"
         "fsw.ps f30,  960 (%[operands])\n"    // for tensor ops
         "fsw.ps f31,   0 (x31)\n"
         "fsw.ps f31,   32 (x0)\n"
         "fsw.ps f31,   64 (x1)\n"
         "fsw.ps f31,  992 (%[operands])\n"    // for tensor ops
         "maskpopc x31, m3\n"                                  // opcode=0x52018ffb
         "fmaxu.pi f25, f23, f21\n"                            // opcode=0x2f5bbcfb
         "feqm.ps m5, f20, f31\n"                              // opcode=0xa1fa62fb
         "fmin.ps f27, f21, f30\n"                             // opcode=0x29ea8dfb
         "feq.pi f26, f23, f22\n"                              // opcode=0xa76bad7b
         "fbci.pi f31, 206244\n"                               // opcode=0x325a4fdf
         "fsq2 f25, 0 (x29)\n"                                 // opcode=0xcf9ed627
         "fexp.ps f28, f28\n"                                  // opcode=0x584e0e7b
         "fmv.x.s x25, f26\n"                                  // opcode=0xe00d0cd3
         "fsrl.pi f31, f29, f20\n"                             // opcode=0x074edffb
         "fbci.ps f22, 964079\n"                               // opcode=0xeb5efb1f
         "flt.ps f31, f27, f22\n"                              // opcode=0xa16d9ffb
         "flt.ps f26, f23, f20\n"                              // opcode=0xa14b9d7b
         "fltu.pi f20, f29, f26\n"                             // opcode=0xa7aeba7b
         "fmvs.x.ps x30, f20, 2\n"                             // opcode=0xe02a2f7b
         "flt.ps f30, f25, f23\n"                              // opcode=0xa17c9f7b
         "mova.x.m x31\n"                                      // opcode=0xd6000ffb
         "cubesgnsc.ps f25, f25, f29\n"                        // opcode=0x89dcacfb
         "fltu.pi f20, f27, f21\n"                             // opcode=0xa75dba7b
         "fsgnj.s f24, f22, f25\n"                             // opcode=0x219b0c53
         "fmax.pi f21, f28, f27\n"                             // opcode=0x2fbe1afb
         "fnot.pi f24, f21\n"                                  // opcode=0x060aac7b
         "feq.s x25, f24, f24\n"                               // opcode=0xa18c2cd3
         "fmaxu.pi f28, f22, f29\n"                            // opcode=0x2fdb3e7b
         "fmax.s f30, f25, f23\n"                              // opcode=0x297c9f53
         "flt.s x27, f26, f29\n"                               // opcode=0xa1dd1dd3
         "fslli.pi f29, f31, 21\n"                             // opcode=0x4f5f9efb
         "fsrli.pi f27, f30, 24\n"                             // opcode=0x4f8f5dfb
         "fmul.s f22, f26, f24, dyn\n"                         // opcode=0x118d7b53
         "fcmov.ps f28, f28, f20, f25\n"                       // opcode=0xcd4e2e3f
         "fand.pi f27, f27, f25\n"                             // opcode=0x079dfdfb
         "fmv.s.x f28, x25\n"                                  // opcode=0xf00c8e53
         "fnmsub.ps f23, f24, f27, f20, dyn\n"                 // opcode=0xa5bc7bdb
         "frcp.ps f23, f22\n"                                  // opcode=0x587b0bfb
         "masknot m6, m7\n"                                    // opcode=0x6603a37b
         //RTLMIN-5479"frsq.ps f22, f31\n"                                  // opcode=0x588f8b7b
         "fslli.pi f31, f28, 22\n"                             // opcode=0x4f6e1ffb
         "flt.ps f27, f20, f20\n"                              // opcode=0xa14a1dfb
         "flog.ps f30, f25\n"                                  // opcode=0x583c8f7b
         "flt.pi f27, f27, f25\n"                              // opcode=0xa79d9dfb
         "cubesgnsc.ps f22, f30, f30\n"                        // opcode=0x89ef2b7b
         "fand.pi f26, f22, f23\n"                             // opcode=0x077b7d7b
         "ffrc.ps f21, f22\n"                                  // opcode=0x582b0afb
         "fsq2 f25, 0 (x30)\n"                                 // opcode=0x559f5ca7
         "fnmsub.ps f20, f20, f22, f22, dyn\n"                 // opcode=0xb56a7a5b
         "flog.ps f25, f22\n"                                  // opcode=0x583b0cfb
         "fsgnj.s f21, f23, f25\n"                             // opcode=0x219b8ad3
         "fpackrepb.pi f30, f22\n"                             // opcode=0x260b0f7b
         "fmin.pi f23, f25, f24\n"                             // opcode=0x2f8c8bfb
         "fsrli.pi f22, f22, 31\n"                             // opcode=0x4ffb5b7b
         "fle.s x27, f24, f20\n"                               // opcode=0xa14c0dd3
         "for.pi f26, f27, f26\n"                              // opcode=0x07aded7b
         "fsra.pi f31, f27, f31\n"                             // opcode=0x0ffddffb
         "frcp.ps f29, f20\n"                                  // opcode=0x587a0efb
         "fmul.s f29, f21, f24, dyn\n"                         // opcode=0x118afed3
         "fmsub.s f20, f23, f30, f28, dyn\n"                   // opcode=0xe1ebfa47
         "fbci.pi f25, 1045201\n"                              // opcode=0xff2d1cdf
         "cubesgntc.ps f24, f30, f31\n"                        // opcode=0x89ff3c7b
         "fle.ps f28, f29, f31\n"                              // opcode=0xa1fe8e7b
         "cubesgntc.ps f25, f22, f23\n"                        // opcode=0x897b3cfb
         "fmul.pi f21, f26, f23\n"                             // opcode=0x177d0afb
         "for.pi f23, f21, f26\n"                              // opcode=0x07aaebfb
         "feqm.ps m3, f21, f25\n"                              // opcode=0xa19ae1fb
         "fmax.s f24, f27, f28\n"                              // opcode=0x29cd9c53
         "flt.s x25, f21, f25\n"                               // opcode=0xa19a9cd3
         "masknot m6, m4\n"                                    // opcode=0x6602237b
         "fmsub.ps f22, f29, f24, f27, dyn\n"                  // opcode=0xdb8efb5b
         "cubeface.ps f20, f23, f31\n"                         // opcode=0x89fb8a7b
         "fmin.s f23, f25, f31\n"                              // opcode=0x29fc8bd3
         "mova.m.x x30\n"                                      // opcode=0xd60f107b
         "mova.x.m x23\n"                                      // opcode=0xd6000bfb
         "fmax.s f31, f23, f20\n"                              // opcode=0x294b9fd3
         "fadd.pi f29, f27, f28\n"                             // opcode=0x07cd8efb
         "flt.pi f21, f30, f25\n"                              // opcode=0xa79f1afb
         "fmul.ps f27, f25, f22, dyn\n"                        // opcode=0x116cfdfb
         "fltm.ps m1, f31, f28\n"                              // opcode=0xa1cfd0fb
         "faddi.pi f26, f29, 4\n"                              // opcode=0x044e8d3f
         "mova.x.m x28\n"                                      // opcode=0xd6000e7b
         "fmulhu.pi f28, f20, f25\n"                           // opcode=0x179a1e7b
         "flt.ps f27, f24, f25\n"                              // opcode=0xa19c1dfb
         "fnmsub.ps f21, f22, f30, f30, dyn\n"                 // opcode=0xf5eb7adb
         "fsub.s f30, f30, f27, dyn\n"                         // opcode=0x09bf7f53
         "fmvs.x.ps x20, f27, 5\n"                             // opcode=0xe05daa7b
         "fsgnjn.ps f21, f30, f30\n"                           // opcode=0x21ef1afb
         "fnmadd.s f31, f22, f28, f21, dyn\n"                  // opcode=0xa9cb7fcf
         "faddi.pi f22, f25, 52\n"                            // opcode=0xd54c8b3f
         "fnmadd.s f20, f29, f24, f29, dyn\n"                  // opcode=0xe98efa4f
         "fmul.pi f20, f25, f22\n"                             // opcode=0x176c8a7b
         "fmul.pi f26, f29, f20\n"                             // opcode=0x174e8d7b
         "fadd.s f21, f23, f20, dyn\n"                         // opcode=0x014bfad3
         "feq.pi f28, f30, f21\n"                              // opcode=0xa75f2e7b
         "fsrl.pi f26, f23, f30\n"                             // opcode=0x07ebdd7b
         "fsgnjn.s f29, f23, f29\n"                            // opcode=0x21db9ed3
         "fmax.ps f27, f21, f23\n"                             // opcode=0x297a9dfb
         "fle.pi f27, f24, f28\n"                              // opcode=0xa7cc0dfb
         "fle.pi f22, f28, f30\n"                              // opcode=0xa7ee0b7b
         "feq.s x25, f28, f29\n"                               // opcode=0xa1de2cd3
         "flem.ps m5, f23, f29\n"                              // opcode=0xa1dbc2fb
         "flog.ps f28, f31\n"                                  // opcode=0x583f8e7b
         "fsat8.pi f29, f20\n"                                 // opcode=0x060a3efb
         "fbci.ps f21, 735375\n"                               // opcode=0xb388fa9f
         "fmax.ps f30, f21, f30\n"                             // opcode=0x29ea9f7b
         "fmaxu.pi f27, f21, f25\n"                            // opcode=0x2f9abdfb
         "fmax.s f21, f21, f27\n"                              // opcode=0x29ba9ad3
         "fsq2 f21, 0 (x26)\n"                                 // opcode=0x7f5d5da7
         "fsgnj.ps f23, f23, f25\n"                            // opcode=0x219b8bfb
         "cubesgnsc.ps f23, f23, f28\n"                        // opcode=0x89cbabfb
         "fadd.pi f25, f25, f30\n"                             // opcode=0x07ec8cfb
         "fsra.pi f29, f30, f26\n"                             // opcode=0x0faf5efb
         "fnmsub.ps f29, f29, f24, f26, dyn\n"                 // opcode=0xd58efedb
         "fclass.ps f31, f23\n"                                // opcode=0xe00b9ffb
         "maskand m7, m2, m1\n"                                // opcode=0x661173fb
         "fsll.pi f27, f27, f27\n"                             // opcode=0x07bd9dfb
         "maskpopc x21, m6\n"                                  // opcode=0x52030afb
         "maskxor m6, m1, m0\n"                                // opcode=0x6600c37b
         "fcmovm.ps f25, f29, f23\n"                           // opcode=0x017e8cf7
         "fadd.ps f21, f21, f31, dyn\n"                        // opcode=0x01fafafb
         "fsgnjn.s f30, f30, f31\n"                            // opcode=0x21ff1f53
         "masknot m5, m0\n"                                    // opcode=0x660022fb
         "fclass.ps f23, f24\n"                                // opcode=0xe00c1bfb
         "fsgnjn.s f28, f26, f24\n"                            // opcode=0x218d1e53
         "fadd.ps f31, f23, f21, dyn\n"                        // opcode=0x015bfffb
         "fnmadd.s f30, f25, f26, f26, dyn\n"                  // opcode=0xd1acff4f
         "flt.s x24, f26, f30\n"                               // opcode=0xa1ed1c53
         "fmul.pi f28, f24, f31\n"                             // opcode=0x17fc0e7b
         "fbcx.ps f27, x23\n"                                  // opcode=0x000bbd8b
         "fsgnjn.s f31, f21, f21\n"                            // opcode=0x215a9fd3
         "fsat8.pi f24, f24\n"                                 // opcode=0x060c3c7b
         "feq.ps f23, f30, f27\n"                              // opcode=0xa1bf2bfb
         "fmax.s f27, f23, f31\n"                              // opcode=0x29fb9dd3
         "fxor.pi f30, f31, f22\n"                             // opcode=0x076fcf7b
         "fsll.pi f26, f28, f25\n"                             // opcode=0x079e1d7b
         "fslli.pi f30, f27, 2\n"                              // opcode=0x4e2d9f7b
         "fsat8.pi f29, f21\n"                                 // opcode=0x060abefb
         "fmv.x.s x21, f20\n"                                  // opcode=0xe00a0ad3
         "fadd.ps f27, f20, f25, dyn\n"                        // opcode=0x019a7dfb
         "fadd.ps f26, f20, f26, dyn\n"                        // opcode=0x01aa7d7b
         "fclass.s x25, f24\n"                                 // opcode=0xe00c1cd3
         "fbci.pi f31, 438020\n"                               // opcode=0x6af04fdf
         "fsrai.pi f20, f20, 6\n"                              // opcode=0x4e6a7a7b
         "fbcx.ps f21, x28\n"                                  // opcode=0x000e3a8b
         "fadd.s f29, f24, f20, dyn\n"                         // opcode=0x014c7ed3
         "fcmovm.ps f22, f30, f30\n"                           // opcode=0x01ef0b77
         "fcmov.ps f27, f27, f29, f27\n"                       // opcode=0xddddadbf
         "fsgnj.s f25, f27, f30\n"                             // opcode=0x21ed8cd3
         "fmv.s.x f21, x30\n"                                  // opcode=0xf00f0ad3
         "cubesgntc.ps f27, f26, f20\n"                        // opcode=0x894d3dfb
         "ffrc.ps f24, f25\n"                                  // opcode=0x582c8c7b
         "fsrli.pi f31, f27, 6\n"                              // opcode=0x4e6ddffb
         "feqm.ps m6, f30, f20\n"                              // opcode=0xa14f637b
         "mova.x.m x20\n"                                      // opcode=0xd6000a7b
         "fmax.s f20, f28, f30\n"                              // opcode=0x29ee1a53
         "fsub.pi f24, f27, f23\n"                             // opcode=0x0f7d8c7b
         "fmax.pi f29, f27, f22\n"                             // opcode=0x2f6d9efb
         "fnmadd.s f26, f21, f21, f24, dyn\n"                  // opcode=0xc15afd4f
         "fandi.pi f22, f20, 80\n"                            // opcode=0xdd0a1b3f
         "fminu.pi f24, f27, f25\n"                            // opcode=0x2f9dac7b
         "fmul.ps f27, f21, f20, dyn\n"                        // opcode=0x114afdfb
         "fnmadd.ps f22, f30, f22, f29, dyn\n"                 // opcode=0xef6f7b5b
         "fround.ps f21, f20, dyn\n"                           // opcode=0x581a7afb
         "feq.s x20, f22, f27\n"                               // opcode=0xa1bb2a53
         "fexp.ps f27, f22\n"                                  // opcode=0x584b0dfb
         "fsll.pi f25, f26, f31\n"                             // opcode=0x07fd1cfb
         "fmv.x.s x25, f29\n"                                  // opcode=0xe00e8cd3
         "fxor.pi f26, f24, f26\n"                             // opcode=0x07ac4d7b
         "fsq2 f29, 0 (x29)\n"                                 // opcode=0xd3ded4a7
         "fltu.pi f25, f22, f23\n"                             // opcode=0xa77b3cfb
         "fslli.pi f20, f31, 5\n"                              // opcode=0x4e5f9a7b
         "flem.ps m2, f24, f30\n"                              // opcode=0xa1ec417b
         "fmv.x.s x26, f31\n"                                  // opcode=0xe00f8d53
         "fadd.pi f26, f29, f22\n"                             // opcode=0x076e8d7b
         "fsrli.pi f20, f20, 8\n"                              // opcode=0x4e8a5a7b
         "cubesgnsc.ps f21, f23, f21\n"                        // opcode=0x895baafb
         "flt.pi f31, f22, f24\n"                              // opcode=0xa78b1ffb
         "fsrli.pi f25, f28, 25\n"                             // opcode=0x4f9e5cfb
         "fle.s x27, f26, f30\n"                               // opcode=0xa1ed0dd3
         "fpackrepb.pi f21, f25\n"                             // opcode=0x260c8afb
         "flt.pi f22, f30, f30\n"                              // opcode=0xa7ef1b7b
         "fmaxu.pi f27, f22, f29\n"                            // opcode=0x2fdb3dfb
         "flt.pi f29, f25, f28\n"                              // opcode=0xa7cc9efb
         "fmvz.x.ps x28, f27, 0\n"                             // opcode=0xe00d8e7b
         "fadd.ps f29, f25, f31, dyn\n"                        // opcode=0x01fcfefb
         "fclass.ps f28, f21\n"                                // opcode=0xe00a9e7b
         "fsub.pi f20, f30, f30\n"                             // opcode=0x0fef0a7b
         "fle.s x23, f27, f26\n"                               // opcode=0xa1ad8bd3
         "fmv.s.x f23, x21\n"                                  // opcode=0xf00a8bd3
         "fmax.pi f26, f31, f26\n"                             // opcode=0x2faf9d7b
         "fsatu8.pi f27, f21\n"                                // opcode=0x061abdfb
         "flem.ps m0, f26, f24\n"                              // opcode=0xa18d407b
         "maskpopcz x22, m5\n"                                 // opcode=0x54028b7b
         "frcp.ps f30, f20\n"                                  // opcode=0x587a0f7b
         "fadd.ps f24, f23, f29, dyn\n"                        // opcode=0x01dbfc7b
         "fmax.pi f25, f24, f28\n"                             // opcode=0x2fcc1cfb
         "fcmov.ps f31, f23, f21, f26\n"                       // opcode=0xd55bafbf
         "fnmsub.s f29, f26, f25, f24, rdn\n"                  // opcode=0xc19d2ecb
         "maskor m3, m3, m4\n"                                 // opcode=0x6641e1fb
         "feq.ps f21, f31, f24\n"                              // opcode=0xa18faafb
         "fsub.ps f27, f25, f25, dyn\n"                        // opcode=0x099cfdfb
         "fnmadd.ps f27, f27, f28, f24, dyn\n"                 // opcode=0xc7cdfddb
         "fmadd.s f27, f26, f20, f30, dyn\n"                   // opcode=0xf14d7dc3
         "fsub.pi f28, f21, f21\n"                             // opcode=0x0f5a8e7b
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fsrai.pi f20, f20, 6\n"                              // opcode=0x4e6a7a7b
         "fnmadd.s f21, f20, f26, f21, rne\n"                  // opcode=0xa9aa0acf
         //RTLMIN-5479"frsq.ps f31, f27\n"                                  // opcode=0x588d8ffb
         "fmvs.x.ps x27, f22, 3\n"                             // opcode=0xe03b2dfb
         "fbci.ps f27, 39554\n"                                // opcode=0x09a82d9f
         "fmv.s.x f26, x30\n"                                  // opcode=0xf00f0d53
         "feq.ps f29, f30, f21\n"                              // opcode=0xa15f2efb
         "fnmadd.s f26, f24, f28, f24, dyn\n"                  // opcode=0xc1cc7d4f
         "maskxor m1, m2, m5\n"                                // opcode=0x665140fb
         "fmadd.ps f25, f26, f20, f23, dyn\n"                  // opcode=0xb94d7cdb
         "maskor m6, m2, m7\n"                                 // opcode=0x6671637b
         "cubesgntc.ps f20, f25, f20\n"                        // opcode=0x894cba7b
         "fclass.s x26, f28\n"                                 // opcode=0xe00e1d53
         "fmin.ps f30, f23, f20\n"                             // opcode=0x294b8f7b
         "fmul.pi f28, f21, f26\n"                             // opcode=0x17aa8e7b
         "maskxor m3, m7, m6\n"                                // opcode=0x6663c1fb
         "maskpopc x20, m2\n"                                  // opcode=0x52010a7b
         "fsgnjn.ps f29, f28, f20\n"                           // opcode=0x214e1efb
         "fsgnj.s f29, f21, f28\n"                             // opcode=0x21ca8ed3
         "fclass.s x22, f28\n"                                 // opcode=0xe00e1b53
         "fmin.s f24, f24, f30\n"                              // opcode=0x29ec0c53
         "fsra.pi f30, f30, f30\n"                             // opcode=0x0fef5f7b
         "fmax.pi f22, f23, f27\n"                             // opcode=0x2fbb9b7b
         "fmv.s.x f28, x31\n"                                  // opcode=0xf00f8e53
         "fsat8.pi f28, f31\n"                                 // opcode=0x060fbe7b
         "fpackreph.pi f21, f29\n"                             // opcode=0x260e9afb
         //RTLMIN-5479"fsin.ps f26, f28\n"                                  // opcode=0x586e0d7b
         "fadd.pi f31, f21, f25\n"                             // opcode=0x079a8ffb
         "maskor m3, m7, m0\n"                                 // opcode=0x6603e1fb
         "fmadd.s f22, f23, f28, f27, dyn\n"                   // opcode=0xd9cbfb43
         "fadd.pi f27, f26, f20\n"                             // opcode=0x074d0dfb
         "fmul.ps f29, f29, f30, dyn\n"                        // opcode=0x11eefefb
         "fxor.pi f24, f21, f27\n"                             // opcode=0x07bacc7b
         "fmax.s f29, f21, f22\n"                              // opcode=0x296a9ed3
         "fsgnjn.s f23, f21, f24\n"                            // opcode=0x218a9bd3
         "fsrli.pi f22, f24, 17\n"                             // opcode=0x4f1c5b7b
         "fslli.pi f29, f20, 4\n"                              // opcode=0x4e4a1efb
         "fltu.pi f26, f22, f22\n"                             // opcode=0xa76b3d7b
         "maskpopc x30, m4\n"                                  // opcode=0x52020f7b
         "frcp.ps f28, f31\n"                                  // opcode=0x587f8e7b
         "fnot.pi f22, f26\n"                                  // opcode=0x060d2b7b
         "fpackrepb.pi f22, f30\n"                             // opcode=0x260f0b7b
         "fmax.pi f28, f24, f22\n"                             // opcode=0x2f6c1e7b
         "fle.pi f26, f31, f30\n"                              // opcode=0xa7ef8d7b
         "fsat8.pi f27, f26\n"                                 // opcode=0x060d3dfb
         "cubesgnsc.ps f29, f29, f29\n"                        // opcode=0x89deaefb
         "fnmadd.s f21, f24, f25, f20, dyn\n"                  // opcode=0xa19c7acf
         "maskand m3, m7, m3\n"                                // opcode=0x6633f1fb
         "fclass.ps f21, f25\n"                                // opcode=0xe00c9afb
         "fexp.ps f27, f21\n"                                  // opcode=0x584a8dfb
         "li x2, 2765\n"
         "mova.m.x x5\n"
         "fle.ps f29, f22, f22\n"                              // opcode=0xa16b0efb
         "for.pi f24, f28, f27\n"                              // opcode=0x07be6c7b
         "fmv.x.s x29, f26\n"                                  // opcode=0xe00d0ed3
         "maskor m1, m0, m6\n"                                 // opcode=0x666060fb
         "for.pi f21, f20, f28\n"                              // opcode=0x07ca6afb
         "fsgnjn.ps f28, f22, f23\n"                           // opcode=0x217b1e7b
         "fbcx.ps f24, x29\n"                                  // opcode=0x000ebc0b
         "fnot.pi f29, f27\n"                                  // opcode=0x060daefb
         "fsub.ps f20, f28, f31, rmm\n"                        // opcode=0x09fe4a7b
         "fsgnjn.ps f26, f20, f28\n"                           // opcode=0x21ca1d7b
         "fsgnjn.ps f21, f21, f21\n"                           // opcode=0x215a9afb
         "feq.s x27, f23, f23\n"                               // opcode=0xa17badd3
         "fltu.pi f20, f31, f25\n"                             // opcode=0xa79fba7b
         "fmvz.x.ps x22, f26, 1\n"                             // opcode=0xe01d0b7b
         "fsrli.pi f29, f26, 8\n"                              // opcode=0x4e8d5efb
         "fle.ps f21, f21, f28\n"                              // opcode=0xa1ca8afb
         "fcmov.ps f23, f30, f28, f20\n"                       // opcode=0xa5cf2bbf
         "fltm.pi m7, f24, f26\n"                              // opcode=0x3fac03fb
         "fsgnjx.ps f31, f23, f22\n"                           // opcode=0x216baffb
         "maskor m4, m7, m1\n"                                 // opcode=0x6613e27b
         "fmul.ps f24, f24, f25, dyn\n"                        // opcode=0x119c7c7b
         "fmul.s f22, f25, f28, dyn\n"                         // opcode=0x11ccfb53
         "fmul.pi f25, f20, f30\n"                             // opcode=0x17ea0cfb
         "fmvs.x.ps x26, f29, 6\n"                             // opcode=0xe06ead7b
         "ffrc.ps f21, f21\n"                                  // opcode=0x582a8afb
         "frcp.ps f26, f24\n"                                  // opcode=0x587c0d7b
         "fminu.pi f22, f20, f25\n"                            // opcode=0x2f9a2b7b
         "fsat8.pi f28, f31\n"                                 // opcode=0x060fbe7b
         //RTLMIN-5479"fsin.ps f25, f25\n"                                  // opcode=0x586c8cfb
         "fsgnjx.ps f28, f21, f27\n"                           // opcode=0x21baae7b
         "fxor.pi f28, f22, f31\n"                             // opcode=0x07fb4e7b
         "cubesgntc.ps f28, f31, f23\n"                        // opcode=0x897fbe7b
         "fsatu8.pi f25, f21\n"                                // opcode=0x061abcfb
         "fadd.s f28, f31, f30, dyn\n"                         // opcode=0x01effe53
         "fsra.pi f30, f24, f21\n"                             // opcode=0x0f5c5f7b
         "fbcx.ps f23, x21\n"                                  // opcode=0x000abb8b
         "for.pi f27, f23, f25\n"                              // opcode=0x079bedfb
         "fpackreph.pi f26, f29\n"                             // opcode=0x260e9d7b
         "feq.s x31, f24, f20\n"                               // opcode=0xa14c2fd3
         "fmin.ps f24, f25, f25\n"                             // opcode=0x299c8c7b
         "flt.ps f23, f24, f28\n"                              // opcode=0xa1cc1bfb
         "fle.ps f30, f28, f24\n"                              // opcode=0xa18e0f7b
         "fmaxu.pi f31, f23, f27\n"                            // opcode=0x2fbbbffb
         "fmvz.x.ps x27, f23, 1\n"                             // opcode=0xe01b8dfb
         "fcmov.ps f23, f23, f31, f24\n"                       // opcode=0xc5fbabbf
         "fsub.ps f21, f20, f31, dyn\n"                        // opcode=0x09fa7afb
         "for.pi f22, f27, f27\n"                              // opcode=0x07bdeb7b
         "fandi.pi f25, f27, 15\n"                            // opcode=0xccfd9cbf
         "fsrli.pi f20, f25, 28\n"                             // opcode=0x4fccda7b
         "cubesgntc.ps f25, f27, f21\n"                        // opcode=0x895dbcfb
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fnmsub.s f27, f22, f28, f21, dyn\n"                  // opcode=0xa9cb7dcb
         "fmax.ps f28, f27, f21\n"                             // opcode=0x295d9e7b
         "fmul.ps f20, f22, f26, dyn\n"                        // opcode=0x11ab7a7b
         "fmadd.s f23, f20, f22, f31, dyn\n"                   // opcode=0xf96a7bc3
         "flt.s x30, f22, f20\n"                               // opcode=0xa14b1f53
         "fsgnj.s f26, f27, f25\n"                             // opcode=0x219d8d53
         "fsgnj.ps f21, f28, f21\n"                            // opcode=0x215e0afb
         "maskxor m1, m5, m3\n"                                // opcode=0x6632c0fb
         "fpackrepb.pi f22, f30\n"                             // opcode=0x260f0b7b
         "fmax.pi f20, f27, f30\n"                             // opcode=0x2fed9a7b
         "fcmov.ps f29, f22, f27, f24\n"                       // opcode=0xc5bb2ebf
         "ffrc.ps f21, f25\n"                                  // opcode=0x582c8afb
         "fsetm.pi m4, f21\n"                                  // opcode=0xe00ad27b
         "masknot m7, m5\n"                                    // opcode=0x6602a3fb
         "flog.ps f28, f29\n"                                  // opcode=0x583e8e7b
         "fadd.s f27, f30, f25, dyn\n"                         // opcode=0x019f7dd3
         "for.pi f21, f30, f28\n"                              // opcode=0x07cf6afb
         "fmulhu.pi f24, f29, f24\n"                           // opcode=0x178e9c7b
         "fsat8.pi f27, f31\n"                                 // opcode=0x060fbdfb
         "fltm.ps m5, f22, f22\n"                              // opcode=0xa16b52fb
         "fnot.pi f31, f27\n"                                  // opcode=0x060daffb
         "feqm.ps m5, f22, f24\n"                              // opcode=0xa18b62fb
         "fsgnjx.s f20, f28, f29\n"                            // opcode=0x21de2a53
         "mova.m.x x26\n"                                      // opcode=0xd60d107b
         "fmax.s f25, f23, f30\n"                              // opcode=0x29eb9cd3
         "ffrc.ps f31, f26\n"                                  // opcode=0x582d0ffb
         "fnmsub.ps f30, f30, f30, f20, dyn\n"                 // opcode=0xa5ef7f5b
         "mova.m.x x25\n"                                      // opcode=0xd60c907b
         "fandi.pi f25, f26, 33\n"                            // opcode=0xec5d1cbf
         "maskpopc x25, m1\n"                                  // opcode=0x52008cfb
         "fsll.pi f29, f22, f21\n"                             // opcode=0x075b1efb
         "fmvs.x.ps x22, f20, 2\n"                             // opcode=0xe02a2b7b
         "fsgnjx.ps f25, f28, f21\n"                           // opcode=0x215e2cfb
         "cubesgnsc.ps f30, f23, f23\n"                        // opcode=0x897baf7b
         "fmulhu.pi f31, f24, f30\n"                           // opcode=0x17ec1ffb
         "cubesgntc.ps f24, f24, f26\n"                        // opcode=0x89ac3c7b
         "fmv.x.s x24, f27\n"                                  // opcode=0xe00d8c53
         //RTLMIN-5479"fsin.ps f25, f29\n"                                  // opcode=0x586e8cfb
         "fpackrepb.pi f30, f31\n"                             // opcode=0x260f8f7b
         "fmin.ps f20, f30, f22\n"                             // opcode=0x296f0a7b
         "fmaxu.pi f27, f28, f28\n"                            // opcode=0x2fce3dfb
         "maskxor m6, m0, m2\n"                                // opcode=0x6620437b
         "fmin.ps f28, f23, f28\n"                             // opcode=0x29cb8e7b
         "fsub.pi f29, f20, f23\n"                             // opcode=0x0f7a0efb
         "cubesgnsc.ps f24, f25, f22\n"                        // opcode=0x896cac7b
         "fmvz.x.ps x29, f25, 3\n"                             // opcode=0xe03c8efb
         "faddi.pi f23, f27, 48\n"                            // opcode=0xed4d8bbf
         "maskand m7, m5, m5\n"                                // opcode=0x6652f3fb
         "fsgnj.ps f21, f26, f27\n"                            // opcode=0x21bd0afb
         "fpackrepb.pi f30, f29\n"                             // opcode=0x260e8f7b
         "li x2, 703\n"
         "mova.m.x x5\n"
         "fbcx.ps f25, x20\n"                                  // opcode=0x000a3c8b
         "fmul.pi f31, f30, f31\n"                             // opcode=0x17ff0ffb
         "feq.s x22, f30, f21\n"                               // opcode=0xa15f2b53
         "fmv.x.s x28, f28\n"                                  // opcode=0xe00e0e53
         "fmv.s.x f29, x22\n"                                  // opcode=0xf00b0ed3
         "fsub.ps f21, f26, f21, dyn\n"                        // opcode=0x095d7afb
         "fclass.ps f22, f21\n"                                // opcode=0xe00a9b7b
         "fmulhu.pi f24, f21, f30\n"                           // opcode=0x17ea9c7b
         "fmul.s f22, f21, f26, dyn\n"                         // opcode=0x11aafb53
         "fmul.s f26, f25, f22, dyn\n"                         // opcode=0x116cfd53
         "fslli.pi f20, f27, 1\n"                              // opcode=0x4e1d9a7b
         //RTLMIN-5479"frsq.ps f23, f21\n"                                  // opcode=0x588a8bfb
         "ffrc.ps f21, f21\n"                                  // opcode=0x582a8afb
         "masknot m2, m6\n"                                    // opcode=0x6603217b
         "fmin.ps f20, f25, f25\n"                             // opcode=0x299c8a7b
         "fnmsub.s f22, f29, f31, f20, dyn\n"                  // opcode=0xa1fefb4b
         "fmv.s.x f25, x20\n"                                  // opcode=0xf00a0cd3
         "faddi.pi f28, f21, 301\n"                            // opcode=0x4cda8e3f
         "cubefaceidx.ps f28, f22, f28\n"                      // opcode=0x89cb1e7b
         "fcmov.ps f25, f27, f28, f22\n"                       // opcode=0xb5cdacbf
         "fle.ps f21, f21, f21\n"                              // opcode=0xa15a8afb
         "flq2 f27, 0 (x23)\n"                                 // opcode=0xe2dbdd87
         "fmax.ps f23, f21, f23\n"                             // opcode=0x297a9bfb
         "fnot.pi f31, f25\n"                                  // opcode=0x060caffb
         "mova.x.m x23\n"                                      // opcode=0xd6000bfb
         "fcmov.ps f25, f28, f21, f31\n"                       // opcode=0xfd5e2cbf
         "fsrl.pi f26, f30, f26\n"                             // opcode=0x07af5d7b
         "fmulhu.pi f26, f30, f27\n"                           // opcode=0x17bf1d7b
         "fsetm.pi m7, f28\n"                                  // opcode=0xe00e53fb
         "fadd.s f20, f28, f31, dyn\n"                         // opcode=0x01fe7a53
         "flem.ps m3, f28, f20\n"                              // opcode=0xa14e41fb
         "fmax.s f20, f31, f20\n"                              // opcode=0x294f9a53
         "fsub.pi f26, f23, f29\n"                             // opcode=0x0fdb8d7b
         "fltm.ps m7, f20, f26\n"                              // opcode=0xa1aa53fb
         "fle.pi f24, f31, f22\n"                              // opcode=0xa76f8c7b
         "fmv.x.s x23, f21\n"                                  // opcode=0xe00a8bd3
         "fltu.pi f28, f27, f20\n"                             // opcode=0xa74dbe7b
         "fexp.ps f28, f20\n"                                  // opcode=0x584a0e7b
         "masknot m5, m6\n"                                    // opcode=0x660322fb
         "fsrl.pi f26, f21, f23\n"                             // opcode=0x077add7b
         "fmax.pi f24, f27, f28\n"                             // opcode=0x2fcd9c7b
         "maskor m6, m7, m4\n"                                 // opcode=0x6643e37b
         "fsub.pi f20, f31, f31\n"                             // opcode=0x0fff8a7b
         "fslli.pi f25, f25, 31\n"                             // opcode=0x4ffc9cfb
         "frcp.ps f22, f24\n"                                  // opcode=0x587c0b7b
         "fsra.pi f22, f22, f29\n"                             // opcode=0x0fdb5b7b
         "fcmovm.ps f28, f27, f30\n"                           // opcode=0x01ed8e77
         "fle.s x27, f27, f22\n"                               // opcode=0xa16d8dd3
         "fmulhu.pi f20, f24, f26\n"                           // opcode=0x17ac1a7b
         //RTLMIN-5479"fsin.ps f20, f27\n"                                  // opcode=0x586d8a7b
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fsrai.pi f20, f21, 20\n"                             // opcode=0x4f4afa7b
         "mova.x.m x27\n"                                      // opcode=0xd6000dfb
         "fsll.pi f21, f20, f24\n"                             // opcode=0x078a1afb
         "fmul.s f24, f27, f25, dyn\n"                         // opcode=0x119dfc53
         "fle.pi f31, f20, f28\n"                              // opcode=0xa7ca0ffb
         "mova.x.m x30\n"                                      // opcode=0xd6000f7b
         "feq.ps f26, f25, f28\n"                              // opcode=0xa1ccad7b
         "fsgnj.s f23, f25, f29\n"                             // opcode=0x21dc8bd3
         "fsgnjn.s f24, f28, f25\n"                            // opcode=0x219e1c53
         "fpackrepb.pi f31, f27\n"                             // opcode=0x260d8ffb
         "fmax.pi f29, f31, f25\n"                             // opcode=0x2f9f9efb
         "fsrli.pi f23, f24, 16\n"                             // opcode=0x4f0c5bfb
         "fpackrepb.pi f21, f29\n"                             // opcode=0x260e8afb
         "fmaxu.pi f23, f22, f26\n"                            // opcode=0x2fab3bfb
         "fsgnj.ps f31, f30, f27\n"                            // opcode=0x21bf0ffb
         "fmax.s f31, f28, f29\n"                              // opcode=0x29de1fd3
         "fmv.s.x f26, x24\n"                                  // opcode=0xf00c0d53
         "fltm.ps m7, f27, f22\n"                              // opcode=0xa16dd3fb
         "fmulh.pi f29, f22, f21\n"                            // opcode=0x175b2efb
         "fmulhu.pi f31, f30, f30\n"                           // opcode=0x17ef1ffb
         "frcp.ps f25, f28\n"                                  // opcode=0x587e0cfb
         "fmvs.x.ps x26, f23, 4\n"                             // opcode=0xe04bad7b
         //RTLMIN-5479"fsin.ps f23, f28\n"                                  // opcode=0x586e0bfb
         "fmv.x.s x27, f29\n"                                  // opcode=0xe00e8dd3
         "flog.ps f23, f26\n"                                  // opcode=0x583d0bfb
         "fmvs.x.ps x29, f24, 5\n"                             // opcode=0xe05c2efb
         "fsat8.pi f26, f29\n"                                 // opcode=0x060ebd7b
         "fmin.pi f30, f31, f25\n"                             // opcode=0x2f9f8f7b
         "fmax.pi f23, f25, f22\n"                             // opcode=0x2f6c9bfb
         "fand.pi f24, f30, f24\n"                             // opcode=0x078f7c7b
         "fmulhu.pi f22, f25, f28\n"                           // opcode=0x17cc9b7b
         "fadd.s f21, f26, f28, dyn\n"                         // opcode=0x01cd7ad3
         "flt.pi f23, f28, f30\n"                              // opcode=0xa7ee1bfb
         "fbcx.ps f30, x22\n"                                  // opcode=0x000b3f0b
         "fmax.pi f22, f20, f28\n"                             // opcode=0x2fca1b7b
         "fmax.ps f31, f22, f27\n"                             // opcode=0x29bb1ffb
         "faddi.pi f20, f26, 33\n"                             // opcode=0x0c1d0a3f
         "fmin.pi f25, f26, f20\n"                             // opcode=0x2f4d0cfb
         "fsub.s f21, f21, f20, rne\n"                         // opcode=0x094a8ad3
         "fmsub.ps f21, f31, f22, f23, dyn\n"                  // opcode=0xbb6ffadb
         "fsetm.pi m3, f25\n"                                  // opcode=0xe00cd1fb
         "flem.ps m6, f24, f20\n"                              // opcode=0xa14c437b
         "fmin.s f30, f23, f23\n"                              // opcode=0x297b8f53
         "mova.m.x x31\n"                                      // opcode=0xd60f907b
         "fadd.pi f25, f21, f22\n"                             // opcode=0x076a8cfb
         "fsq2 f27, 0 (x26)\n"                                 // opcode=0x89bd5c27
         "maskxor m4, m1, m7\n"                                // opcode=0x6670c27b
         "fltm.pi m4, f26, f29\n"                              // opcode=0x3fdd027b
         "fsgnj.ps f28, f20, f27\n"                            // opcode=0x21ba0e7b
         "fandi.pi f30, f28, 45\n"                            // opcode=0x8c1e1f3f
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fmv.x.s x28, f22\n"                                  // opcode=0xe00b0e53
         "fsub.ps f24, f21, f24, rmm\n"                        // opcode=0x098acc7b
         "fltm.ps m6, f27, f20\n"                              // opcode=0xa14dd37b
         "fsub.s f21, f25, f21, dyn\n"                         // opcode=0x095cfad3
         "cubesgnsc.ps f26, f25, f31\n"                        // opcode=0x89fcad7b
         "mova.m.x x30\n"                                      // opcode=0xd60f107b
         "fnot.pi f23, f28\n"                                  // opcode=0x060e2bfb
         "fand.pi f26, f28, f25\n"                             // opcode=0x079e7d7b
         "fexp.ps f31, f26\n"                                  // opcode=0x584d0ffb
         "fmsub.ps f31, f29, f27, f28, dyn\n"                  // opcode=0xe3beffdb
         "fround.ps f31, f21, dyn\n"                           // opcode=0x581afffb
         "fsgnj.ps f30, f21, f25\n"                            // opcode=0x219a8f7b
         "fle.pi f20, f22, f24\n"                              // opcode=0xa78b0a7b
         "fmvs.x.ps x29, f26, 2\n"                             // opcode=0xe02d2efb
         "fmvz.x.ps x31, f31, 5\n"                             // opcode=0xe05f8ffb
         "fltu.pi f29, f21, f20\n"                             // opcode=0xa74abefb
         "fsra.pi f29, f20, f25\n"                             // opcode=0x0f9a5efb
         "feq.s x31, f25, f21\n"                               // opcode=0xa15cafd3
         "fexp.ps f22, f26\n"                                  // opcode=0x584d0b7b
         "fsetm.pi m4, f30\n"                                  // opcode=0xe00f527b
         "fsgnj.s f25, f30, f30\n"                             // opcode=0x21ef0cd3
         "fminu.pi f21, f30, f24\n"                            // opcode=0x2f8f2afb
         "fand.pi f21, f24, f21\n"                             // opcode=0x075c7afb
         "fmax.ps f30, f20, f22\n"                             // opcode=0x296a1f7b
         "fmulhu.pi f30, f22, f30\n"                           // opcode=0x17eb1f7b
         "fsub.pi f21, f21, f26\n"                             // opcode=0x0faa8afb
         "flog.ps f21, f25\n"                                  // opcode=0x583c8afb
         "cubesgntc.ps f31, f22, f25\n"                        // opcode=0x899b3ffb
         "fbci.pi f27, 993640\n"                               // opcode=0xf2968ddf
         "fmax.s f22, f22, f29\n"                              // opcode=0x29db1b53
         "fclass.s x25, f30\n"                                 // opcode=0xe00f1cd3
         "fmul.pi f26, f28, f28\n"                             // opcode=0x17ce0d7b
         "fle.ps f21, f29, f20\n"                              // opcode=0xa14e8afb
         "fadd.s f20, f29, f20, dyn\n"                         // opcode=0x014efa53
         "for.pi f22, f21, f23\n"                              // opcode=0x077aeb7b
         "flem.ps m1, f31, f31\n"                              // opcode=0xa1ffc0fb
         "fmul.pi f22, f21, f28\n"                             // opcode=0x17ca8b7b
         "maskand m7, m4, m3\n"                                // opcode=0x663273fb
         "fsgnj.ps f31, f30, f22\n"                            // opcode=0x216f0ffb
         "cubeface.ps f24, f31, f22\n"                         // opcode=0x896f8c7b
         "fmadd.ps f27, f22, f21, f22, dyn\n"                  // opcode=0xb15b7ddb
         "fslli.pi f27, f25, 3\n"                              // opcode=0x4e3c9dfb
         "fle.pi f23, f28, f24\n"                              // opcode=0xa78e0bfb
         "for.pi f26, f27, f30\n"                              // opcode=0x07eded7b
         "maskpopc x21, m5\n"                                  // opcode=0x52028afb
         "fminu.pi f28, f31, f25\n"                            // opcode=0x2f9fae7b
         "fltu.pi f24, f25, f23\n"                             // opcode=0xa77cbc7b
         "fsgnjx.s f27, f24, f23\n"                            // opcode=0x217c2dd3
         "fmul.ps f26, f26, f23, dyn\n"                        // opcode=0x117d7d7b
         "fmv.x.s x22, f25\n"                                  // opcode=0xe00c8b53
         "li x2, 0xf0f0\n"
         "mova.m.x x5\n"
         "fadd.s f24, f29, f24, dyn\n"                         // opcode=0x018efc53
         "fmax.ps f22, f28, f27\n"                             // opcode=0x29be1b7b
         "fsub.s f26, f30, f20, dyn\n"                         // opcode=0x094f7d53
         "cubeface.ps f21, f31, f31\n"                         // opcode=0x89ff8afb
         "fsub.pi f23, f31, f27\n"                             // opcode=0x0fbf8bfb
         "fltm.ps m5, f28, f29\n"                              // opcode=0xa1de52fb
         //RTLMIN-5479"fsin.ps f29, f24\n"                                  // opcode=0x586c0efb
         "fround.ps f28, f20, dyn\n"                           // opcode=0x581a7e7b
         "fcmov.ps f25, f27, f20, f25\n"                       // opcode=0xcd4dacbf
         "flt.pi f26, f28, f29\n"                              // opcode=0xa7de1d7b
         "flq2 f21, 0 (x20)\n"                                 // opcode=0x759a5a87
         "maskpopcz x26, m1\n"                                 // opcode=0x54008d7b
         "fmv.x.s x30, f23\n"                                  // opcode=0xe00b8f53
         "fand.pi f26, f29, f22\n"                             // opcode=0x076efd7b
         "fsetm.pi m7, f29\n"                                  // opcode=0xe00ed3fb
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
            );
   C_TEST_PASS;
   return 0;
}
