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
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         

      // vpu register f1
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb8c4b40,    // -18388608.0           

      // vpu register f2
      0xcb8c4b40,    // -18388608.0           
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            

      // vpu register f3
      0x4b000000,    // 8388608.0             
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f4
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f5
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           

      // vpu register f6
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  

      // vpu register f7
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f8
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         

      // vpu register f9
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f10
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         

      // vpu register f11
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           

      // vpu register f12
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            

      // vpu register f13
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   

      // vpu register f14
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           
      0x4b000000,    // 8388608.0             

      // vpu register f15
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           

      // vpu register f16
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  

      // vpu register f17
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            

      // vpu register f18
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         

      // vpu register f19
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 

      // vpu register f20
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            

      // vpu register f21
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f22
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            

      // vpu register f23
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           

      // vpu register f24
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            

      // vpu register f25
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f26
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f27
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  

      // vpu register f28
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f29
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            

      // vpu register f30
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 

      // vpu register f31
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
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
         "li x0,  0x80b54f00\n"
         "li x1,  0x80a51e40\n"
         "li x2,  0x8004fd00\n"
         "li x3,  0x80389280\n"
         "li x4,  0x805a3000\n"
         "li x5,  0x807e18c0\n"
         "li x6,  0x8080b960\n"
         "li x7,  0x80ba8520\n"
         "li x8,  0x80ef1ee0\n"
         "li x9,  0x808c44c0\n"
         "li x10,  0x80c48840\n"
         "li x11,  0x80bff100\n"
         "li x12,  0x80d91380\n"
         "li x13,  0x80d705a0\n"
         "li x14,  0x804f8480\n"
         "li x15,  0x80d14220\n"
         "li x16,  0x8013dca0\n"
         "li x17,  0x80208280\n"
         "li x18,  0x8028f1e0\n"
         "li x19,  0x800fa100\n"
         "li x20,  0x803b5220\n"
         "li x21,  0x80efc5e0\n"
         "li x22,  0x80374580\n"
         "li x23,  0x80e48b60\n"
         "li x24,  0x8085b8e0\n"
         "li x25,  0x805c4fa0\n"
         "li x26,  0x80489120\n"
         "li x27,  0x80ab4440\n"
         "li x28,  0x80b616a0\n"
         "li x29,  0x80e157e0\n"
         "li x30,  0x80e71b60\n"
         "li x31,  0x809d0bc0\n"
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
         "flog.ps f24, f31\n"                                  // opcode=0x583f8c7b
         "flt.pi f28, f31, f26\n"                              // opcode=0xa7af9e7b
         "ffrc.ps f20, f21\n"                                  // opcode=0x582a8a7b
         "fnot.pi f21, f21\n"                                  // opcode=0x060aaafb
         "fsgnj.ps f20, f25, f27\n"                            // opcode=0x21bc8a7b
         "maskand m2, m1, m7\n"                                // opcode=0x6670f17b
         //RTLMIN-5479"fsin.ps f27, f30\n"                                  // opcode=0x586f0dfb
         "maskor m5, m3, m1\n"                                 // opcode=0x6611e2fb
         "fltu.pi f28, f22, f23\n"                             // opcode=0xa77b3e7b
         "fsub.ps f23, f22, f23, dyn\n"                        // opcode=0x097b7bfb
         "fmaxu.pi f31, f22, f28\n"                            // opcode=0x2fcb3ffb
         "flq2 f27, 0 (x26)\n"                                 // opcode=0x081d5d87
         "flog.ps f26, f31\n"                                  // opcode=0x583f8d7b
         "fclass.s x20, f21\n"                                 // opcode=0xe00a9a53
         "mova.m.x x21\n"                                      // opcode=0xd60a907b
         "fle.s x27, f20, f29\n"                               // opcode=0xa1da0dd3
         "maskpopc x27, m1\n"                                  // opcode=0x52008dfb
         "fbc.ps f30, 58(x25)\n"                               // opcode=0xa3ac8f0b
         "fround.ps f26, f26, dyn\n"                           // opcode=0x581d7d7b
         "maskor m1, m6, m4\n"                                 // opcode=0x664360fb
         "flt.ps f27, f27, f20\n"                              // opcode=0xa14d9dfb
         "feq.pi f24, f22, f27\n"                              // opcode=0xa7bb2c7b
         "flt.pi f27, f25, f20\n"                              // opcode=0xa74c9dfb
         "fandi.pi f21, f22, 94\n"                            // opcode=0x952b1abf
         "maskxor m1, m6, m3\n"                                // opcode=0x663340fb
         "fsgnjx.ps f30, f31, f23\n"                           // opcode=0x217faf7b
         "fle.s x30, f31, f20\n"                               // opcode=0xa14f8f53
         "fbci.ps f21, 121785\n"                               // opcode=0x1dbb9a9f
         "maskpopc x25, m1\n"                                  // opcode=0x52008cfb
         "fmul.s f28, f28, f30, dyn\n"                         // opcode=0x11ee7e53
         "fmv.x.s x31, f27\n"                                  // opcode=0xe00d8fd3
         "mova.m.x x22\n"                                      // opcode=0xd60b107b
         "masknot m3, m7\n"                                    // opcode=0x6603a1fb
         "fpackreph.pi f29, f21\n"                             // opcode=0x260a9efb
         "fpackreph.pi f20, f27\n"                             // opcode=0x260d9a7b
         "fsrl.pi f21, f21, f28\n"                             // opcode=0x07cadafb
         "fmax.ps f31, f23, f28\n"                             // opcode=0x29cb9ffb
         "fmsub.s f30, f23, f29, f31, dyn\n"                   // opcode=0xf9dbff47
         "feqm.ps m5, f23, f28\n"                              // opcode=0xa1cbe2fb
         "fmv.x.s x27, f28\n"                                  // opcode=0xe00e0dd3
         "feqm.ps m0, f24, f28\n"                              // opcode=0xa1cc607b
         "fsat8.pi f26, f30\n"                                 // opcode=0x060f3d7b
         "feqm.ps m0, f30, f23\n"                              // opcode=0xa17f607b
         "fcmov.ps f26, f25, f26, f24\n"                       // opcode=0xc5acad3f
         "flt.pi f27, f24, f20\n"                              // opcode=0xa74c1dfb
         "fadd.pi f31, f30, f24\n"                             // opcode=0x078f0ffb
         "fadd.pi f31, f29, f28\n"                             // opcode=0x07ce8ffb
         "fbcx.ps f29, x24\n"                                  // opcode=0x000c3e8b
         "flem.ps m1, f26, f30\n"                              // opcode=0xa1ed40fb
         "ffrc.ps f24, f21\n"                                  // opcode=0x582a8c7b
         "fnmsub.ps f20, f31, f25, f27, dyn\n"                 // opcode=0xdd9ffa5b
         "fcmovm.ps f22, f31, f23\n"                           // opcode=0x017f8b77
         "fle.pi f20, f23, f25\n"                              // opcode=0xa79b8a7b
         "fsrl.pi f31, f28, f20\n"                             // opcode=0x074e5ffb
         "fsgnj.s f26, f20, f27\n"                             // opcode=0x21ba0d53
         "fmsub.ps f21, f25, f23, f22, rup\n"                  // opcode=0xb37cbadb
         "fsgnjx.s f27, f26, f23\n"                            // opcode=0x217d2dd3
         "maskand m1, m5, m5\n"                                // opcode=0x6652f0fb
         "fslli.pi f31, f30, 1\n"                              // opcode=0x4e1f1ffb
         "frcp.ps f21, f21\n"                                  // opcode=0x587a8afb
         "fclass.s x25, f21\n"                                 // opcode=0xe00a9cd3
         "fnmsub.ps f21, f20, f23, f29, dyn\n"                 // opcode=0xed7a7adb
         "flt.pi f28, f31, f20\n"                              // opcode=0xa74f9e7b
         "fxor.pi f28, f21, f31\n"                             // opcode=0x07face7b
         "fsll.pi f20, f27, f22\n"                             // opcode=0x076d9a7b
         "fslli.pi f31, f28, 6\n"                              // opcode=0x4e6e1ffb
         "fbcx.ps f30, x29\n"                                  // opcode=0x000ebf0b
         "maskxor m1, m5, m6\n"                                // opcode=0x6662c0fb
         "maskand m6, m4, m2\n"                                // opcode=0x6622737b
         "fsub.pi f22, f28, f30\n"                             // opcode=0x0fee0b7b
         "fadd.pi f31, f20, f30\n"                             // opcode=0x07ea0ffb
         "fsgnjx.s f25, f22, f27\n"                            // opcode=0x21bb2cd3
         "fnmsub.ps f21, f24, f29, f30, dyn\n"                 // opcode=0xf5dc7adb
         "fle.s x21, f27, f20\n"                               // opcode=0xa14d8ad3
         "maskand m3, m3, m5\n"                                // opcode=0x6651f1fb
         "fsll.pi f23, f26, f20\n"                             // opcode=0x074d1bfb
         "maskxor m7, m3, m2\n"                                // opcode=0x6621c3fb
         "fclass.ps f22, f25\n"                                // opcode=0xe00c9b7b
         "faddi.pi f24, f27, 56\n"                            // opcode=0xedcd8c3f
         "fexp.ps f27, f24\n"                                  // opcode=0x584c0dfb
         "fnmadd.ps f27, f31, f29, f30, dyn\n"                 // opcode=0xf7dffddb
         "fadd.ps f31, f27, f29, dyn\n"                        // opcode=0x01ddfffb
         "maskand m1, m4, m4\n"                                // opcode=0x664270fb
         "fltm.pi m2, f24, f29\n"                              // opcode=0x3fdc017b
         "fsrli.pi f25, f22, 9\n"                              // opcode=0x4e9b5cfb
         "flt.pi f25, f25, f21\n"                              // opcode=0xa75c9cfb
         "fbci.pi f22, 489030\n"                               // opcode=0x77646b5f
         "fmadd.ps f28, f22, f30, f23, dyn\n"                  // opcode=0xb9eb7e5b
         "feq.ps f27, f21, f25\n"                              // opcode=0xa19aadfb
         "fbcx.ps f31, x31\n"                                  // opcode=0x000fbf8b
         "feq.s x29, f20, f28\n"                               // opcode=0xa1ca2ed3
         "fsrl.pi f21, f23, f25\n"                             // opcode=0x079bdafb
         "fmax.pi f29, f26, f25\n"                             // opcode=0x2f9d1efb
         "fsra.pi f28, f24, f25\n"                             // opcode=0x0f9c5e7b
         "feq.ps f20, f20, f25\n"                              // opcode=0xa19a2a7b
         "masknot m1, m7\n"                                    // opcode=0x6603a0fb
         "fmax.s f21, f24, f20\n"                              // opcode=0x294c1ad3
         "fnot.pi f30, f21\n"                                  // opcode=0x060aaf7b
         "fle.ps f30, f22, f25\n"                              // opcode=0xa19b0f7b
         "maskpopcz x25, m6\n"                                 // opcode=0x54030cfb
         "fsgnj.s f26, f25, f21\n"                             // opcode=0x215c8d53
         "fmax.pi f31, f25, f31\n"                             // opcode=0x2ffc9ffb
         "maskpopc x28, m6\n"                                  // opcode=0x52030e7b
         "fsll.pi f31, f22, f23\n"                             // opcode=0x077b1ffb
         "feqm.ps m3, f20, f22\n"                              // opcode=0xa16a61fb
         "fxor.pi f21, f28, f25\n"                             // opcode=0x079e4afb
         "fadd.ps f26, f24, f22, dyn\n"                        // opcode=0x016c7d7b
         "fsgnjn.s f28, f23, f29\n"                            // opcode=0x21db9e53
         "fclass.ps f29, f31\n"                                // opcode=0xe00f9efb
         "fmin.pi f22, f20, f25\n"                             // opcode=0x2f9a0b7b
         "fmvs.x.ps x23, f30, 2\n"                             // opcode=0xe02f2bfb
         "fmvs.x.ps x28, f23, 0\n"                             // opcode=0xe00bae7b
         "fmadd.s f21, f21, f21, f20, dyn\n"                   // opcode=0xa15afac3
         "for.pi f29, f25, f29\n"                              // opcode=0x07dceefb
         "fsrli.pi f25, f22, 25\n"                             // opcode=0x4f9b5cfb
         "fmv.s.x f21, x24\n"                                  // opcode=0xf00c0ad3
         "fmadd.ps f22, f23, f27, f23, rne\n"                  // opcode=0xb9bb8b5b
         "fmul.pi f29, f23, f31\n"                             // opcode=0x17fb8efb
         "fnmadd.s f30, f28, f25, f23, dyn\n"                  // opcode=0xb99e7f4f
         "fmul.pi f30, f30, f28\n"                             // opcode=0x17cf0f7b
         "fmvs.x.ps x25, f26, 4\n"                             // opcode=0xe04d2cfb
         "maskpopc x21, m5\n"                                  // opcode=0x52028afb
         "maskor m3, m3, m5\n"                                 // opcode=0x6651e1fb
         "cubefaceidx.ps f28, f22, f21\n"                      // opcode=0x895b1e7b
         "fmvz.x.ps x24, f29, 4\n"                             // opcode=0xe04e8c7b
         "fnmsub.ps f25, f26, f29, f30, dyn\n"                 // opcode=0xf5dd7cdb
         "fclass.s x21, f27\n"                                 // opcode=0xe00d9ad3
         "feq.ps f27, f31, f22\n"                              // opcode=0xa16fadfb
         "fsub.ps f28, f29, f23, dyn\n"                        // opcode=0x097efe7b
         "fsub.s f21, f27, f28, dyn\n"                         // opcode=0x09cdfad3
         "fmul.ps f21, f27, f29, dyn\n"                        // opcode=0x11ddfafb
         "fadd.pi f29, f29, f26\n"                             // opcode=0x07ae8efb
         "fmsub.s f29, f23, f24, f29, dyn\n"                   // opcode=0xe98bfec7
         "flt.s x29, f23, f25\n"                               // opcode=0xa19b9ed3
         "fadd.s f27, f24, f24, dyn\n"                         // opcode=0x018c7dd3
         "fmadd.ps f24, f24, f24, f22, dyn\n"                  // opcode=0xb18c7c5b
         "feqm.ps m7, f30, f27\n"                              // opcode=0xa1bf63fb
         "flt.ps f31, f26, f30\n"                              // opcode=0xa1ed1ffb
         "fsrl.pi f27, f21, f22\n"                             // opcode=0x076addfb
         "cubefaceidx.ps f31, f26, f30\n"                      // opcode=0x89ed1ffb
         "cubesgntc.ps f28, f21, f20\n"                        // opcode=0x894abe7b
         "fslli.pi f25, f31, 2\n"                              // opcode=0x4e2f9cfb
         "fmadd.ps f26, f27, f24, f21, dyn\n"                  // opcode=0xa98dfd5b
         "fsub.pi f28, f29, f27\n"                             // opcode=0x0fbe8e7b
         "fbci.pi f20, 91259\n"                                // opcode=0x1647ba5f
         "fmulhu.pi f27, f26, f23\n"                           // opcode=0x177d1dfb
         "masknot m5, m5\n"                                    // opcode=0x6602a2fb
         "fmax.ps f25, f26, f23\n"                             // opcode=0x297d1cfb
         "fsrli.pi f22, f21, 30\n"                             // opcode=0x4feadb7b
         "frcp.ps f27, f31\n"                                  // opcode=0x587f8dfb
         "feq.ps f27, f21, f21\n"                              // opcode=0xa15aadfb
         "fbci.ps f22, 362234\n"                               // opcode=0x586fab1f
         "fpackrepb.pi f27, f24\n"                             // opcode=0x260c0dfb
         "fmsub.s f31, f25, f21, f24, dyn\n"                   // opcode=0xc15cffc7
         "maskxor m5, m2, m7\n"                                // opcode=0x667142fb
         "fround.ps f21, f28, dyn\n"                           // opcode=0x581e7afb
         "fmadd.ps f23, f29, f25, f24, dyn\n"                  // opcode=0xc19efbdb
         "fnot.pi f22, f27\n"                                  // opcode=0x060dab7b
         "fmul.s f31, f20, f30, dyn\n"                         // opcode=0x11ea7fd3
         "flog.ps f29, f22\n"                                  // opcode=0x583b0efb
         "fsgnjn.s f31, f20, f28\n"                            // opcode=0x21ca1fd3
         "fmin.ps f26, f25, f26\n"                             // opcode=0x29ac8d7b
         "maskand m0, m0, m3\n"                                // opcode=0x6630707b
         "fsrai.pi f26, f26, 18\n"                             // opcode=0x4f2d7d7b
         "for.pi f30, f24, f20\n"                              // opcode=0x074c6f7b
         "flog.ps f24, f25\n"                                  // opcode=0x583c8c7b
         //RTLMIN-5479"frsq.ps f25, f20\n"                                  // opcode=0x588a0cfb
         "fmadd.s f20, f22, f24, f21, dyn\n"                   // opcode=0xa98b7a43
         "cubesgnsc.ps f30, f23, f25\n"                        // opcode=0x899baf7b
         "fsrai.pi f21, f20, 16\n"                             // opcode=0x4f0a7afb
         "fandi.pi f28, f21, 67\n"                            // opcode=0xa5ba9e3f
         "fadd.pi f25, f23, f28\n"                             // opcode=0x07cb8cfb
         "cubefaceidx.ps f26, f27, f27\n"                      // opcode=0x89bd9d7b
         "fbc.ps f26, 72(x25)\n"                               // opcode=0xec8c8d0b
         "flt.s x27, f23, f20\n"                               // opcode=0xa14b9dd3
         "fnmsub.ps f22, f30, f31, f31, dyn\n"                 // opcode=0xfdff7b5b
         "fminu.pi f23, f27, f25\n"                            // opcode=0x2f9dabfb
         "flog.ps f27, f21\n"                                  // opcode=0x583a8dfb
         "fmsub.ps f24, f25, f26, f26, dyn\n"                  // opcode=0xd3acfc5b
         "fmax.pi f30, f30, f26\n"                             // opcode=0x2faf1f7b
         "fandi.pi f31, f25, 17\n"                            // opcode=0xcd1c9fbf
         "ffrc.ps f21, f23\n"                                  // opcode=0x582b8afb
         "fsra.pi f23, f23, f31\n"                             // opcode=0x0ffbdbfb
         "fpackreph.pi f31, f26\n"                             // opcode=0x260d1ffb
         "fand.pi f25, f31, f24\n"                             // opcode=0x078ffcfb
         "fsra.pi f21, f27, f20\n"                             // opcode=0x0f4ddafb
         "fmvs.x.ps x23, f22, 6\n"                             // opcode=0xe06b2bfb
         "fsrai.pi f22, f24, 9\n"                              // opcode=0x4e9c7b7b
         "cubeface.ps f23, f31, f20\n"                         // opcode=0x894f8bfb
         "cubesgnsc.ps f30, f27, f25\n"                        // opcode=0x899daf7b
         "fsat8.pi f31, f21\n"                                 // opcode=0x060abffb
         "fsrl.pi f27, f31, f25\n"                             // opcode=0x079fddfb
         "fmv.x.s x25, f30\n"                                  // opcode=0xe00f0cd3
         "fmadd.ps f20, f25, f26, f31, dyn\n"                  // opcode=0xf9acfa5b
         "mova.x.m x29\n"                                      // opcode=0xd6000efb
         "maskand m6, m6, m2\n"                                // opcode=0x6623737b
         "fbc.ps f30, 93(x25)\n"                               // opcode=0xaddc8f0b
         "fsgnjn.s f20, f26, f25\n"                            // opcode=0x219d1a53
         "fsetm.pi m3, f24\n"                                  // opcode=0xe00c51fb
         "fmulhu.pi f25, f28, f23\n"                           // opcode=0x177e1cfb
         "fmax.pi f29, f22, f26\n"                             // opcode=0x2fab1efb
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "feq.ps f26, f20, f24\n"                              // opcode=0xa18a2d7b
         //RTLMIN-5479"fsin.ps f27, f30\n"                                  // opcode=0x586f0dfb
         "maskand m1, m2, m4\n"                                // opcode=0x664170fb
         "fadd.s f28, f26, f26, rdn\n"                         // opcode=0x01ad2e53
         "maskor m7, m7, m4\n"                                 // opcode=0x6643e3fb
         "cubefaceidx.ps f23, f23, f20\n"                      // opcode=0x894b9bfb
         "maskpopc x22, m4\n"                                  // opcode=0x52020b7b
         "fadd.ps f30, f23, f30, dyn\n"                        // opcode=0x01ebff7b
         "fle.s x26, f29, f31\n"                               // opcode=0xa1fe8d53
         "maskpopcz x20, m0\n"                                 // opcode=0x54000a7b
         "fmax.s f21, f27, f23\n"                              // opcode=0x297d9ad3
         "fsrai.pi f26, f22, 23\n"                             // opcode=0x4f7b7d7b
         "cubefaceidx.ps f30, f30, f24\n"                      // opcode=0x898f1f7b
         "fmadd.s f24, f21, f23, f28, dyn\n"                   // opcode=0xe17afc43
         "fnmadd.ps f27, f28, f25, f28, dyn\n"                 // opcode=0xe79e7ddb
         "for.pi f25, f23, f26\n"                              // opcode=0x07abecfb
         "fsgnjn.ps f25, f27, f27\n"                           // opcode=0x21bd9cfb
         "fclass.ps f24, f31\n"                                // opcode=0xe00f9c7b
         "fmul.pi f29, f20, f22\n"                             // opcode=0x176a0efb
         "fpackrepb.pi f23, f20\n"                             // opcode=0x260a0bfb
         "cubesgntc.ps f25, f25, f20\n"                        // opcode=0x894cbcfb
         "maskpopc x29, m7\n"                                  // opcode=0x52038efb
         "for.pi f23, f30, f28\n"                              // opcode=0x07cf6bfb
         "fmin.pi f20, f31, f27\n"                             // opcode=0x2fbf8a7b
         "feq.s x23, f27, f20\n"                               // opcode=0xa14dabd3
         "fsgnj.ps f30, f29, f29\n"                            // opcode=0x21de8f7b
         "fadd.ps f26, f31, f20, rdn\n"                        // opcode=0x014fad7b
         "fmaxu.pi f28, f30, f23\n"                            // opcode=0x2f7f3e7b
         "flog.ps f22, f30\n"                                  // opcode=0x583f0b7b
         "maskpopcz x31, m7\n"                                 // opcode=0x54038ffb
         "fnmsub.s f29, f21, f30, f31, dyn\n"                  // opcode=0xf9eafecb
         "fmulh.pi f27, f21, f26\n"                            // opcode=0x17aaadfb
         "mova.m.x x29\n"                                      // opcode=0xd60e907b
         "fmul.s f30, f20, f23, dyn\n"                         // opcode=0x117a7f53
         "fbci.pi f29, 314916\n"                               // opcode=0x4ce24edf
         "fmul.s f20, f23, f27, dyn\n"                         // opcode=0x11bbfa53
         "flt.s x24, f29, f20\n"                               // opcode=0xa14e9c53
         "fltm.pi m2, f29, f27\n"                              // opcode=0x3fbe817b
         "fmvs.x.ps x22, f21, 1\n"                             // opcode=0xe01aab7b
         "fxor.pi f20, f24, f26\n"                             // opcode=0x07ac4a7b
         "faddi.pi f20, f26, 294\n"                            // opcode=0x4c6d0a3f
         "fadd.s f31, f25, f23, dyn\n"                         // opcode=0x017cffd3
         "maskxor m4, m7, m6\n"                                // opcode=0x6663c27b
         "maskor m7, m5, m6\n"                                 // opcode=0x6662e3fb
         "fmadd.s f24, f30, f24, f21, rne\n"                   // opcode=0xa98f0c43
         //RTLMIN-5479"frsq.ps f23, f23\n"                                  // opcode=0x588b8bfb
         "fmvz.x.ps x30, f21, 7\n"                             // opcode=0xe07a8f7b
         "fsub.s f30, f20, f21, rtz\n"                         // opcode=0x095a1f53
         "feq.pi f27, f31, f27\n"                              // opcode=0xa7bfadfb
         "fsat8.pi f26, f21\n"                                 // opcode=0x060abd7b
         "li x2, 340\n"
         "mova.m.x x5\n"
         "fround.ps f20, f20, dyn\n"                           // opcode=0x581a7a7b
         "cubefaceidx.ps f24, f21, f21\n"                      // opcode=0x895a9c7b
         "flog.ps f31, f24\n"                                  // opcode=0x583c0ffb
         "fmul.ps f28, f23, f27, dyn\n"                        // opcode=0x11bbfe7b
         "faddi.pi f30, f20, 77\n"                            // opcode=0x941a0f3f
         "fnmadd.s f21, f21, f22, f30, dyn\n"                  // opcode=0xf16afacf
         "fclass.ps f24, f23\n"                                // opcode=0xe00b9c7b
         "flq2 f25, 0 (x20)\n"                                 // opcode=0x4c0a5c87
         "maskand m1, m1, m0\n"                                // opcode=0x6600f0fb
         "fbcx.ps f26, x26\n"                                  // opcode=0x000d3d0b
         "fsrli.pi f20, f27, 27\n"                             // opcode=0x4fbdda7b
         "fnmsub.s f31, f31, f27, f31, dyn\n"                  // opcode=0xf9bfffcb
         "maskpopcz x31, m6\n"                                 // opcode=0x54030ffb
         "fexp.ps f27, f27\n"                                  // opcode=0x584d8dfb
         "fcmovm.ps f23, f29, f20\n"                           // opcode=0x014e8bf7
         "fnmadd.s f27, f25, f21, f27, dyn\n"                  // opcode=0xd95cfdcf
         "maskpopc x25, m3\n"                                  // opcode=0x52018cfb
         "fsrli.pi f31, f24, 16\n"                             // opcode=0x4f0c5ffb
         "fsgnj.s f27, f21, f22\n"                             // opcode=0x216a8dd3
         "fmsub.ps f26, f26, f23, f25, dyn\n"                  // opcode=0xcb7d7d5b
         "fmvz.x.ps x24, f22, 7\n"                             // opcode=0xe07b0c7b
         "fsgnjx.ps f24, f24, f26\n"                           // opcode=0x21ac2c7b
         "fsgnj.s f30, f22, f26\n"                             // opcode=0x21ab0f53
         "maskor m4, m7, m5\n"                                 // opcode=0x6653e27b
         "fsub.pi f20, f26, f30\n"                             // opcode=0x0fed0a7b
         "fltm.pi m1, f31, f20\n"                              // opcode=0x3f4f80fb
         "fle.s x31, f31, f23\n"                               // opcode=0xa17f8fd3
         "fle.s x31, f25, f23\n"                               // opcode=0xa17c8fd3
         "maskpopc x29, m2\n"                                  // opcode=0x52010efb
         "fnmadd.ps f22, f27, f24, f21, dyn\n"                 // opcode=0xaf8dfb5b
         "feqm.ps m7, f31, f25\n"                              // opcode=0xa19fe3fb
         "fsll.pi f25, f21, f26\n"                             // opcode=0x07aa9cfb
         "fcmovm.ps f27, f30, f22\n"                           // opcode=0x016f0df7
         "fsgnjx.ps f20, f27, f21\n"                           // opcode=0x215daa7b
         "fslli.pi f28, f24, 5\n"                              // opcode=0x4e5c1e7b
         "fsq2 f28, 0 (x29)\n"                                 // opcode=0xd5cede27
         //RTLMIN-5479"fsin.ps f28, f23\n"                                  // opcode=0x586b8e7b
         "faddi.pi f29, f29, 375\n"                            // opcode=0x5d7e8ebf
         "fsgnjn.ps f25, f25, f20\n"                           // opcode=0x214c9cfb
         "fsub.ps f29, f31, f30, dyn\n"                        // opcode=0x09effefb
         "fmadd.ps f26, f30, f24, f31, dyn\n"                  // opcode=0xf98f7d5b
         "fsq2 f22, 0 (x29)\n"                                 // opcode=0xed6ed4a7
         "fsgnj.ps f20, f20, f27\n"                            // opcode=0x21ba0a7b
         "fle.s x29, f21, f25\n"                               // opcode=0xa19a8ed3
         "maskor m3, m7, m6\n"                                 // opcode=0x6663e1fb
         "fbci.pi f28, 363670\n"                               // opcode=0x58c96e5f
         "fnot.pi f28, f22\n"                                  // opcode=0x060b2e7b
         "fsgnjn.s f28, f25, f25\n"                            // opcode=0x219c9e53
         "flt.s x23, f27, f25\n"                               // opcode=0xa19d9bd3
         "fadd.s f24, f28, f25, rtz\n"                         // opcode=0x019e1c53
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "feqm.ps m2, f22, f28\n"                              // opcode=0xa1cb617b
         "fsrl.pi f30, f30, f20\n"                             // opcode=0x074f5f7b
         "fsatu8.pi f28, f27\n"                                // opcode=0x061dbe7b
         "fmax.pi f20, f21, f31\n"                             // opcode=0x2ffa9a7b
         "fsgnjx.s f23, f26, f22\n"                            // opcode=0x216d2bd3
         "fle.s x22, f21, f29\n"                               // opcode=0xa1da8b53
         "fmadd.s f29, f25, f30, f22, rdn\n"                   // opcode=0xb1ecaec3
         "fmaxu.pi f30, f26, f26\n"                            // opcode=0x2fad3f7b
         "fpackrepb.pi f29, f21\n"                             // opcode=0x260a8efb
         "cubefaceidx.ps f27, f22, f26\n"                      // opcode=0x89ab1dfb
         "fnmadd.ps f20, f22, f24, f24, dyn\n"                 // opcode=0xc78b7a5b
         "fsrl.pi f21, f24, f23\n"                             // opcode=0x077c5afb
         "flt.s x29, f29, f25\n"                               // opcode=0xa19e9ed3
         "fltm.pi m7, f22, f21\n"                              // opcode=0x3f5b03fb
         "fsra.pi f30, f25, f30\n"                             // opcode=0x0fecdf7b
         "fminu.pi f21, f29, f26\n"                            // opcode=0x2faeaafb
         "fslli.pi f23, f24, 1\n"                              // opcode=0x4e1c1bfb
         "fsgnjx.s f27, f29, f22\n"                            // opcode=0x216eadd3
         "fbci.ps f29, 880512\n"                               // opcode=0xd6f80e9f
         "fsatu8.pi f31, f22\n"                                // opcode=0x061b3ffb
         "fadd.s f31, f22, f27, rup\n"                         // opcode=0x01bb3fd3
         "feq.pi f22, f21, f21\n"                              // opcode=0xa75aab7b
         "fmax.pi f25, f20, f31\n"                             // opcode=0x2ffa1cfb
         "fsgnj.ps f31, f29, f28\n"                            // opcode=0x21ce8ffb
         //RTLMIN-5479"frsq.ps f22, f25\n"                                  // opcode=0x588c8b7b
         "fle.s x25, f31, f27\n"                               // opcode=0xa1bf8cd3
         "fmul.pi f22, f31, f26\n"                             // opcode=0x17af8b7b
         "fandi.pi f22, f30, 52\n"                            // opcode=0xa4cf1b3f
         "fsll.pi f28, f26, f24\n"                             // opcode=0x078d1e7b
         "fmv.s.x f25, x23\n"                                  // opcode=0xf00b8cd3
         "fsgnjx.s f24, f23, f26\n"                            // opcode=0x21abac53
         "flt.pi f27, f30, f27\n"                              // opcode=0xa7bf1dfb
         "fsgnjn.s f22, f30, f25\n"                            // opcode=0x219f1b53
         "fmv.x.s x22, f30\n"                                  // opcode=0xe00f0b53
         "fclass.ps f27, f27\n"                                // opcode=0xe00d9dfb
         "fle.ps f29, f21, f25\n"                              // opcode=0xa19a8efb
         "fslli.pi f25, f21, 4\n"                              // opcode=0x4e4a9cfb
         "fltu.pi f30, f26, f25\n"                             // opcode=0xa79d3f7b
         "flt.pi f22, f24, f21\n"                              // opcode=0xa75c1b7b
         "fandi.pi f27, f22, 220\n"                            // opcode=0x35cb1dbf
         "feq.s x20, f26, f27\n"                               // opcode=0xa1bd2a53
         "fsatu8.pi f20, f24\n"                                // opcode=0x061c3a7b
         "maskpopc x24, m6\n"                                  // opcode=0x52030c7b
         "fcmovm.ps f24, f27, f22\n"                           // opcode=0x016d8c77
         "fbci.ps f21, 535028\n"                               // opcode=0x829f4a9f
         "fmin.s f22, f31, f31\n"                              // opcode=0x29ff8b53
         "for.pi f22, f22, f26\n"                              // opcode=0x07ab6b7b
         "maskpopcz x28, m2\n"                                 // opcode=0x54010e7b
         "fle.ps f31, f23, f31\n"                              // opcode=0xa1fb8ffb
         "fnmadd.ps f30, f23, f21, f26, dyn\n"                 // opcode=0xd75bff5b
         "li x2, 1973\n"
         "mova.m.x x5\n"
         "fmvz.x.ps x30, f20, 7\n"                             // opcode=0xe07a0f7b
         "fsgnjn.s f27, f31, f27\n"                            // opcode=0x21bf9dd3
         "faddi.pi f29, f22, 23\n"                            // opcode=0x84bb0ebf
         "flt.pi f23, f30, f29\n"                              // opcode=0xa7df1bfb
         "fnot.pi f21, f25\n"                                  // opcode=0x060caafb
         "fmul.pi f25, f25, f30\n"                             // opcode=0x17ec8cfb
         "fsq2 f20, 0 (x24)\n"                                 // opcode=0xf34c5ca7
         "fmv.s.x f24, x30\n"                                  // opcode=0xf00f0c53
         "fsgnjn.s f21, f30, f22\n"                            // opcode=0x216f1ad3
         "fbci.ps f23, 823170\n"                               // opcode=0xc8f82b9f
         "fmul.pi f25, f23, f28\n"                             // opcode=0x17cb8cfb
         "fmulhu.pi f28, f24, f26\n"                           // opcode=0x17ac1e7b
         "mova.m.x x27\n"                                      // opcode=0xd60d907b
         "fbcx.ps f26, x31\n"                                  // opcode=0x000fbd0b
         "fmul.ps f22, f22, f28, dyn\n"                        // opcode=0x11cb7b7b
         "fsgnjx.ps f24, f27, f27\n"                           // opcode=0x21bdac7b
         "masknot m0, m7\n"                                    // opcode=0x6603a07b
         "fsub.ps f26, f31, f20, dyn\n"                        // opcode=0x094ffd7b
         "fnot.pi f25, f23\n"                                  // opcode=0x060bacfb
         "fnot.pi f22, f23\n"                                  // opcode=0x060bab7b
         "fminu.pi f28, f23, f23\n"                            // opcode=0x2f7bae7b
         "fsgnjx.ps f20, f26, f22\n"                           // opcode=0x216d2a7b
         "flt.s x24, f25, f29\n"                               // opcode=0xa1dc9c53
         "fmv.s.x f21, x25\n"                                  // opcode=0xf00c8ad3
         "cubesgnsc.ps f27, f26, f30\n"                        // opcode=0x89ed2dfb
         "mova.m.x x27\n"                                      // opcode=0xd60d907b
         "fclass.s x31, f26\n"                                 // opcode=0xe00d1fd3
         "fsra.pi f23, f25, f25\n"                             // opcode=0x0f9cdbfb
         "fsat8.pi f22, f30\n"                                 // opcode=0x060f3b7b
         "fpackrepb.pi f28, f31\n"                             // opcode=0x260f8e7b
         "cubesgntc.ps f24, f25, f30\n"                        // opcode=0x89ecbc7b
         "feqm.ps m7, f25, f29\n"                              // opcode=0xa1dce3fb
         "fxor.pi f24, f23, f30\n"                             // opcode=0x07ebcc7b
         "fsq2 f24, 0 (x23)\n"                                 // opcode=0xc38bde27
         "fmvz.x.ps x22, f25, 4\n"                             // opcode=0xe04c8b7b
         "fandi.pi f27, f23, 314\n"                            // opcode=0x4dab9dbf
         "fbc.ps f22, 62(x26)\n"                               // opcode=0x63ed0b0b
         "fsgnjx.ps f26, f25, f31\n"                           // opcode=0x21fcad7b
         "for.pi f26, f26, f24\n"                              // opcode=0x078d6d7b
         "fle.ps f24, f26, f21\n"                              // opcode=0xa15d0c7b
         "flt.ps f25, f28, f28\n"                              // opcode=0xa1ce1cfb
         "frcp.ps f22, f20\n"                                  // opcode=0x587a0b7b
         "fsll.pi f21, f21, f26\n"                             // opcode=0x07aa9afb
         "fmin.ps f23, f22, f25\n"                             // opcode=0x299b0bfb
         "fround.ps f25, f21, dyn\n"                           // opcode=0x581afcfb
         "fmulhu.pi f31, f30, f30\n"                           // opcode=0x17ef1ffb
         "fnmsub.s f31, f20, f25, f21, dyn\n"                  // opcode=0xa99a7fcb
         "fmv.s.x f26, x22\n"                                  // opcode=0xf00b0d53
         "fsrai.pi f31, f21, 14\n"                             // opcode=0x4eeafffb
         "fmul.s f24, f31, f21, rne\n"                         // opcode=0x115f8c53
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fnmadd.s f30, f30, f28, f20, dyn\n"                  // opcode=0xa1cf7f4f
         "fmin.ps f29, f24, f31\n"                             // opcode=0x29fc0efb
         "maskxor m6, m2, m2\n"                                // opcode=0x6621437b
         "fltu.pi f23, f21, f23\n"                             // opcode=0xa77abbfb
         "fnmsub.s f22, f20, f25, f30, dyn\n"                  // opcode=0xf19a7b4b
         "fpackreph.pi f31, f24\n"                             // opcode=0x260c1ffb
         "fcmov.ps f26, f21, f31, f29\n"                       // opcode=0xedfaad3f
         "fcmovm.ps f22, f21, f26\n"                           // opcode=0x01aa8b77
         "fslli.pi f20, f25, 27\n"                             // opcode=0x4fbc9a7b
         "fexp.ps f27, f25\n"                                  // opcode=0x584c8dfb
         "fmv.s.x f28, x27\n"                                  // opcode=0xf00d8e53
         "fmv.x.s x25, f31\n"                                  // opcode=0xe00f8cd3
         "fsub.s f21, f27, f25, rtz\n"                         // opcode=0x099d9ad3
         "fmadd.s f24, f25, f27, f31, dyn\n"                   // opcode=0xf9bcfc43
         "fmsub.ps f24, f29, f27, f23, dyn\n"                  // opcode=0xbbbefc5b
         "fltu.pi f27, f30, f25\n"                             // opcode=0xa79f3dfb
         "mova.m.x x31\n"                                      // opcode=0xd60f907b
         "fsub.pi f28, f21, f24\n"                             // opcode=0x0f8a8e7b
         "fmin.s f24, f27, f22\n"                              // opcode=0x296d8c53
         "fmulh.pi f31, f30, f21\n"                            // opcode=0x175f2ffb
         "flt.pi f29, f30, f24\n"                              // opcode=0xa78f1efb
         "feq.s x25, f22, f30\n"                               // opcode=0xa1eb2cd3
         "fsrl.pi f29, f24, f23\n"                             // opcode=0x077c5efb
         "flt.pi f24, f24, f31\n"                              // opcode=0xa7fc1c7b
         "fsub.pi f31, f22, f26\n"                             // opcode=0x0fab0ffb
         "maskor m2, m4, m6\n"                                 // opcode=0x6662617b
         "mova.x.m x26\n"                                      // opcode=0xd6000d7b
         "fmv.s.x f30, x24\n"                                  // opcode=0xf00c0f53
         "cubefaceidx.ps f22, f26, f30\n"                      // opcode=0x89ed1b7b
         "fsub.ps f21, f28, f25, dyn\n"                        // opcode=0x099e7afb
         "cubesgnsc.ps f30, f25, f20\n"                        // opcode=0x894caf7b
         "flt.ps f29, f29, f20\n"                              // opcode=0xa14e9efb
         "flt.pi f21, f22, f24\n"                              // opcode=0xa78b1afb
         "fmsub.ps f24, f27, f23, f31, dyn\n"                  // opcode=0xfb7dfc5b
         "fadd.ps f21, f27, f29, dyn\n"                        // opcode=0x01ddfafb
         "fadd.ps f20, f21, f27, dyn\n"                        // opcode=0x01bafa7b
         "fmin.pi f29, f26, f26\n"                             // opcode=0x2fad0efb
         "cubefaceidx.ps f26, f24, f29\n"                      // opcode=0x89dc1d7b
         "flt.s x23, f24, f29\n"                               // opcode=0xa1dc1bd3
         "fsub.s f20, f27, f23, dyn\n"                         // opcode=0x097dfa53
         "fsgnj.s f21, f21, f20\n"                             // opcode=0x214a8ad3
         "fadd.pi f22, f30, f21\n"                             // opcode=0x075f0b7b
         "fsgnjn.ps f29, f23, f27\n"                           // opcode=0x21bb9efb
         "fcmovm.ps f26, f29, f30\n"                           // opcode=0x01ee8d77
         "fsat8.pi f31, f21\n"                                 // opcode=0x060abffb
         "fmul.s f31, f31, f27, dyn\n"                         // opcode=0x11bfffd3
         "fmvs.x.ps x20, f30, 4\n"                             // opcode=0xe04f2a7b
         "fsrli.pi f24, f22, 0\n"                              // opcode=0x4e0b5c7b
         "fmin.ps f26, f25, f25\n"                             // opcode=0x299c8d7b
         "fmax.ps f27, f28, f21\n"                             // opcode=0x295e1dfb
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fsetm.pi m2, f23\n"                                  // opcode=0xe00bd17b
         "flt.pi f25, f21, f24\n"                              // opcode=0xa78a9cfb
         "fmul.s f24, f23, f20, dyn\n"                         // opcode=0x114bfc53
         "fsgnjx.ps f24, f28, f24\n"                           // opcode=0x218e2c7b
         "fadd.s f24, f31, f26, dyn\n"                         // opcode=0x01affc53
         "flog.ps f20, f21\n"                                  // opcode=0x583a8a7b
         "fbc.ps f28, 102(x30)\n"                              // opcode=0xa66f0e0b
         //RTLMIN-5479"frsq.ps f21, f24\n"                                  // opcode=0x588c0afb
         "fmvz.x.ps x20, f29, 2\n"                             // opcode=0xe02e8a7b
         "fsat8.pi f26, f27\n"                                 // opcode=0x060dbd7b
         "maskxor m4, m2, m5\n"                                // opcode=0x6651427b
         "fsub.pi f20, f31, f24\n"                             // opcode=0x0f8f8a7b
         "cubefaceidx.ps f21, f30, f30\n"                      // opcode=0x89ef1afb
         "fmsub.ps f23, f30, f30, f22, dyn\n"                  // opcode=0xb3ef7bdb
         //RTLMIN-5479"fsin.ps f22, f30\n"                                  // opcode=0x586f0b7b
         "fsrai.pi f29, f27, 31\n"                             // opcode=0x4ffdfefb
         "maskpopcz x22, m0\n"                                 // opcode=0x54000b7b
         "fsat8.pi f23, f29\n"                                 // opcode=0x060ebbfb
         "fsgnjn.ps f26, f20, f29\n"                           // opcode=0x21da1d7b
         "fpackrepb.pi f26, f30\n"                             // opcode=0x260f0d7b
         "mova.m.x x29\n"                                      // opcode=0xd60e907b
         "fmin.s f26, f26, f24\n"                              // opcode=0x298d0d53
         "fcmov.ps f31, f21, f29, f25\n"                       // opcode=0xcddaafbf
         "fpackreph.pi f20, f21\n"                             // opcode=0x260a9a7b
         "fsgnjn.ps f25, f22, f25\n"                           // opcode=0x219b1cfb
         "cubesgntc.ps f23, f23, f30\n"                        // opcode=0x89ebbbfb
         "fmin.pi f24, f27, f31\n"                             // opcode=0x2ffd8c7b
         "fle.ps f28, f31, f20\n"                              // opcode=0xa14f8e7b
         "fmax.ps f23, f31, f20\n"                             // opcode=0x294f9bfb
         "flt.ps f21, f28, f23\n"                              // opcode=0xa17e1afb
         "fmvs.x.ps x30, f25, 7\n"                             // opcode=0xe07caf7b
         "fsra.pi f26, f31, f28\n"                             // opcode=0x0fcfdd7b
         "fbci.pi f31, 26129\n"                                // opcode=0x06611fdf
         "fmvz.x.ps x24, f29, 0\n"                             // opcode=0xe00e8c7b
         "fcmovm.ps f23, f23, f29\n"                           // opcode=0x01db8bf7
         "fsgnj.ps f24, f24, f28\n"                            // opcode=0x21cc0c7b
         "fmulh.pi f25, f21, f22\n"                            // opcode=0x176aacfb
         "flt.s x24, f25, f30\n"                               // opcode=0xa1ec9c53
         "frcp.ps f25, f20\n"                                  // opcode=0x587a0cfb
         "fsetm.pi m0, f31\n"                                  // opcode=0xe00fd07b
         "maskor m2, m2, m5\n"                                 // opcode=0x6651617b
         "fmul.s f24, f21, f23, dyn\n"                         // opcode=0x117afc53
         "fclass.s x20, f28\n"                                 // opcode=0xe00e1a53
         "fsra.pi f22, f29, f27\n"                             // opcode=0x0fbedb7b
         "feqm.ps m6, f24, f29\n"                              // opcode=0xa1dc637b
         "fmul.pi f31, f30, f27\n"                             // opcode=0x17bf0ffb
         "fsub.pi f29, f22, f30\n"                             // opcode=0x0feb0efb
         "fnot.pi f23, f28\n"                                  // opcode=0x060e2bfb
         "fandi.pi f27, f27, 84\n"                            // opcode=0x948d9dbf
         "maskand m2, m1, m5\n"                                // opcode=0x6650f17b
         "li x2, 0xf0f0\n"
         "mova.m.x x5\n"
         "masknot m7, m3\n"                                    // opcode=0x6601a3fb
         "faddi.pi f26, f31, 32\n"                            // opcode=0x854f8d3f
         "fmax.pi f30, f31, f26\n"                             // opcode=0x2faf9f7b
         "feq.pi f25, f23, f22\n"                              // opcode=0xa76bacfb
         "cubesgntc.ps f23, f21, f30\n"                        // opcode=0x89eabbfb
         "cubesgnsc.ps f30, f29, f21\n"                        // opcode=0x895eaf7b
         "fpackrepb.pi f22, f25\n"                             // opcode=0x260c8b7b
         "fsub.pi f21, f22, f21\n"                             // opcode=0x0f5b0afb
         "cubesgntc.ps f25, f20, f31\n"                        // opcode=0x89fa3cfb
         "fsetm.pi m6, f24\n"                                  // opcode=0xe00c537b
         "flt.ps f28, f20, f23\n"                              // opcode=0xa17a1e7b
         "cubeface.ps f20, f20, f26\n"                         // opcode=0x89aa0a7b
         "fsra.pi f26, f22, f21\n"                             // opcode=0x0f5b5d7b
         "flt.ps f29, f28, f27\n"                              // opcode=0xa1be1efb
         "maskand m2, m7, m7\n"                                // opcode=0x6673f17b
         "fclass.s x29, f20\n"                                 // opcode=0xe00a1ed3
         "fbc.ps f23, 104(x29)\n"                              // opcode=0x3e8e8b8b
         "fandi.pi f23, f28, 83\n"                            // opcode=0xc4fe1bbf
         "maskand m0, m3, m2\n"                                // opcode=0x6621f07b
         "fsub.ps f26, f29, f23, dyn\n"                        // opcode=0x097efd7b
         "fmaxu.pi f28, f28, f26\n"                            // opcode=0x2fae3e7b
         "fmin.pi f25, f27, f28\n"                             // opcode=0x2fcd8cfb
         "fbc.ps f22, 99(x20)\n"                               // opcode=0x3e3a0b0b
         //RTLMIN-5479"fsin.ps f26, f26\n"                                  // opcode=0x586d0d7b
         "fmul.s f26, f26, f23, dyn\n"                         // opcode=0x117d7d53
         "fnmsub.ps f29, f22, f21, f29, dyn\n"                 // opcode=0xed5b7edb
         "fsub.ps f25, f25, f29, dyn\n"                        // opcode=0x09dcfcfb
         "fsrli.pi f27, f20, 10\n"                             // opcode=0x4eaa5dfb
         "for.pi f31, f29, f31\n"                              // opcode=0x07feeffb
         "fadd.ps f26, f31, f23, rdn\n"                        // opcode=0x017fad7b
         "fltm.ps m7, f20, f27\n"                              // opcode=0xa1ba53fb
         "fclass.ps f26, f29\n"                                // opcode=0xe00e9d7b
         "maskpopcz x27, m1\n"                                 // opcode=0x54008dfb
         "feq.s x30, f23, f21\n"                               // opcode=0xa15baf53
         "fnot.pi f21, f25\n"                                  // opcode=0x060caafb
         "fmax.s f27, f24, f31\n"                              // opcode=0x29fc1dd3
         "ffrc.ps f24, f21\n"                                  // opcode=0x582a8c7b
         "fle.pi f24, f31, f26\n"                              // opcode=0xa7af8c7b
         "fcmov.ps f24, f29, f28, f21\n"                       // opcode=0xadceac3f
         "fpackrepb.pi f26, f20\n"                             // opcode=0x260a0d7b
         "fsat8.pi f29, f23\n"                                 // opcode=0x060bbefb
         "fmsub.ps f26, f26, f20, f26, dyn\n"                  // opcode=0xd34d7d5b
         "fmulh.pi f26, f27, f20\n"                            // opcode=0x174dad7b
         "fbci.ps f29, 934575\n"                               // opcode=0xe42afe9f
         "cubeface.ps f31, f27, f25\n"                         // opcode=0x899d8ffb
         "fmax.ps f20, f25, f24\n"                             // opcode=0x298c9a7b
         "fnmsub.ps f31, f26, f27, f25, dyn\n"                 // opcode=0xcdbd7fdb
         "fmax.s f26, f23, f31\n"                              // opcode=0x29fb9d53
         "fltu.pi f27, f27, f30\n"                             // opcode=0xa7edbdfb
         "fand.pi f22, f28, f29\n"                             // opcode=0x07de7b7b
         "li x2, 2299\n"
         "mova.m.x x5\n"
         "fsgnj.s f23, f20, f31\n"                             // opcode=0x21fa0bd3
         "fxor.pi f22, f23, f29\n"                             // opcode=0x07dbcb7b
         "faddi.pi f20, f31, 239\n"                            // opcode=0x3cff8a3f
         "fmsub.ps f29, f28, f26, f20, dyn\n"                  // opcode=0xa3ae7edb
         "fmv.s.x f24, x27\n"                                  // opcode=0xf00d8c53
         "fsq2 f31, 0 (x26)\n"                                 // opcode=0x1dfd53a7
         "fsgnjx.s f27, f26, f21\n"                            // opcode=0x215d2dd3
         "fmadd.ps f26, f23, f24, f27, dyn\n"                  // opcode=0xd98bfd5b
         "fmul.pi f27, f28, f20\n"                             // opcode=0x174e0dfb
         "frcp.ps f28, f26\n"                                  // opcode=0x587d0e7b
         "flt.pi f27, f29, f24\n"                              // opcode=0xa78e9dfb
         "fbci.ps f31, 1037686\n"                              // opcode=0xfd576f9f
         "fmadd.s f20, f24, f29, f25, dyn\n"                   // opcode=0xc9dc7a43
         "flt.s x24, f21, f31\n"                               // opcode=0xa1fa9c53
         "fsat8.pi f28, f29\n"                                 // opcode=0x060ebe7b
         "fmvs.x.ps x22, f21, 3\n"                             // opcode=0xe03aab7b
         "frcp.ps f23, f26\n"                                  // opcode=0x587d0bfb
         "fclass.ps f28, f31\n"                                // opcode=0xe00f9e7b
         "fnot.pi f21, f24\n"                                  // opcode=0x060c2afb
         "fadd.pi f31, f23, f23\n"                             // opcode=0x077b8ffb
         "mova.m.x x31\n"                                      // opcode=0xd60f907b
         "flq2 f25, 0 (x20)\n"                                 // opcode=0xe92a5c87
         "fsgnjn.s f24, f21, f26\n"                            // opcode=0x21aa9c53
         "fbcx.ps f24, x24\n"                                  // opcode=0x000c3c0b
         "fmulh.pi f28, f24, f21\n"                            // opcode=0x175c2e7b
         "maskxor m3, m1, m2\n"                                // opcode=0x6620c1fb
         "cubefaceidx.ps f28, f25, f28\n"                      // opcode=0x89cc9e7b
         "flog.ps f30, f29\n"                                  // opcode=0x583e8f7b
         "fsetm.pi m3, f30\n"                                  // opcode=0xe00f51fb
         "fltu.pi f23, f30, f30\n"                             // opcode=0xa7ef3bfb
         "fmadd.s f30, f27, f26, f22, dyn\n"                   // opcode=0xb1adff43
         "fmul.ps f25, f27, f31, dyn\n"                        // opcode=0x11fdfcfb
         "frcp.ps f24, f31\n"                                  // opcode=0x587f8c7b
         "masknot m3, m2\n"                                    // opcode=0x660121fb
         "fltm.ps m2, f26, f29\n"                              // opcode=0xa1dd517b
         "flt.pi f29, f20, f24\n"                              // opcode=0xa78a1efb
         "fsrli.pi f25, f21, 23\n"                             // opcode=0x4f7adcfb
         "fmadd.s f29, f25, f22, f26, dyn\n"                   // opcode=0xd16cfec3
         "fmax.ps f24, f27, f20\n"                             // opcode=0x294d9c7b
         "fexp.ps f31, f26\n"                                  // opcode=0x584d0ffb
         "fsgnj.s f30, f22, f28\n"                             // opcode=0x21cb0f53
         "fsat8.pi f26, f29\n"                                 // opcode=0x060ebd7b
         "fnot.pi f28, f28\n"                                  // opcode=0x060e2e7b
         "fbci.ps f20, 15128\n"                                // opcode=0x03b18a1f
         "feqm.ps m5, f29, f31\n"                              // opcode=0xa1fee2fb
         "fle.s x29, f20, f31\n"                               // opcode=0xa1fa0ed3
         "fnmadd.s f23, f28, f25, f25, dyn\n"                  // opcode=0xc99e7bcf
         "fmsub.ps f23, f22, f26, f30, dyn\n"                  // opcode=0xf3ab7bdb
         "fmadd.s f21, f27, f20, f31, dyn\n"                   // opcode=0xf94dfac3
         "fsetm.pi m2, f29\n"                                  // opcode=0xe00ed17b
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "mova.m.x x22\n"                                      // opcode=0xd60b107b
         "fadd.ps f21, f26, f30, dyn\n"                        // opcode=0x01ed7afb
         "fmin.pi f21, f30, f28\n"                             // opcode=0x2fcf0afb
         "fmax.pi f27, f29, f23\n"                             // opcode=0x2f7e9dfb
         "fsub.ps f20, f31, f23, dyn\n"                        // opcode=0x097ffa7b
         "frcp.ps f31, f30\n"                                  // opcode=0x587f0ffb
         "faddi.pi f30, f30, 66\n"                            // opcode=0x752f0f3f
         "fxor.pi f22, f20, f27\n"                             // opcode=0x07ba4b7b
         "fltm.pi m5, f21, f20\n"                              // opcode=0x3f4a82fb
         "fmv.x.s x30, f25\n"                                  // opcode=0xe00c8f53
         "flem.ps m0, f29, f27\n"                              // opcode=0xa1bec07b
         "fbci.pi f27, 461227\n"                               // opcode=0x709abddf
         "fadd.s f24, f25, f22, dyn\n"                         // opcode=0x016cfc53
         "fsub.s f30, f24, f29, dyn\n"                         // opcode=0x09dc7f53
         "fsrl.pi f31, f20, f23\n"                             // opcode=0x077a5ffb
         "fadd.pi f21, f23, f25\n"                             // opcode=0x079b8afb
         "fsat8.pi f30, f31\n"                                 // opcode=0x060fbf7b
         "fclass.s x31, f22\n"                                 // opcode=0xe00b1fd3
         "fcmov.ps f29, f28, f25, f31\n"                       // opcode=0xfd9e2ebf
         "maskor m7, m4, m3\n"                                 // opcode=0x663263fb
         "flt.pi f21, f23, f28\n"                              // opcode=0xa7cb9afb
         "fsll.pi f27, f29, f20\n"                             // opcode=0x074e9dfb
         "cubefaceidx.ps f31, f21, f29\n"                      // opcode=0x89da9ffb
         "fsatu8.pi f30, f22\n"                                // opcode=0x061b3f7b
         "fsgnj.s f31, f26, f24\n"                             // opcode=0x218d0fd3
         "fsll.pi f24, f27, f24\n"                             // opcode=0x078d9c7b
         "masknot m2, m0\n"                                    // opcode=0x6600217b
         "fmvz.x.ps x22, f20, 3\n"                             // opcode=0xe03a0b7b
         //RTLMIN-5479"fsin.ps f24, f29\n"                                  // opcode=0x586e8c7b
         "feqm.ps m2, f31, f26\n"                              // opcode=0xa1afe17b
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
