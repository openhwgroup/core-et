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
      0x3ffafafb,    // 1.96078
      0xbffafafb,    // -1.96078
      0x33699116,    // 5.43815e-08
      0xb3699116,    // -5.43815e-08
      0x7f800000,    // inf
      0xff800000,    // -inf
      0x00000000,    // zero
      0x80000000,    // -zero

      // vpu register f1
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             

      // vpu register f2
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           

      // vpu register f3
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 

      // vpu register f4
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  

      // vpu register f5
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0x4b000000,    // 8388608.0             
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  

      // vpu register f6
      0xbf028f5c,    // -0.51                 
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            

      // vpu register f7
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x3f800000,    // 1.0                   
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b000000,    // 8388608.0             

      // vpu register f8
      0x4b8c4b40,    // 18388608.0            
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            

      // vpu register f9
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             

      // vpu register f10
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             

      // vpu register f11
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f12
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            

      // vpu register f13
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 

      // vpu register f14
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            

      // vpu register f15
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b8c4b40,    // 18388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            

      // vpu register f16
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            

      // vpu register f17
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 

      // vpu register f18
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 

      // vpu register f19
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f20
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            

      // vpu register f21
      0x4b8c4b40,    // 18388608.0            
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            

      // vpu register f22
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            

      // vpu register f23
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f24
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           

      // vpu register f25
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             

      // vpu register f26
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   

      // vpu register f27
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f28
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         

      // vpu register f29
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b8c4b40,    // 18388608.0            
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            

      // vpu register f30
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f31
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         
      0x3f800000     // 1.0                   
   };

   C_TEST_START;
   __asm__ __volatile__ (
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "flw.ps f0,  0 (%[operands])\n"
         "flw.ps f1,  0 (%[operands])\n"
         "flw.ps f2,  0 (%[operands])\n"
         "flw.ps f3,  0 (%[operands])\n"
         "flw.ps f4,  0 (%[operands])\n"
         "flw.ps f5,  0 (%[operands])\n"
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
         "fround.ps f28, f0, rtz\n"            
         "fround.ps f28, f2, rdn\n"            
         "fround.ps f28, f3, rup\n"            
         "fround.ps f28, f1, rne\n"            
         "fround.ps f28, f4, rmm\n"            
         "fround.ps f28, f5, dyn\n"            
         /*
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
            "li x0,  0x80f3cc80\n"
            "li x1,  0x809e2800\n"
            "li x2,  0x80a31460\n"
            "li x3,  0x806836a0\n"
            "li x4,  0x80917120\n"
            "li x5,  0x806f1640\n"
            "li x6,  0x80095180\n"
            "li x7,  0x80b77380\n"
            "li x8,  0x809b1d40\n"
            "li x9,  0x80ac16c0\n"
            "li x10,  0x802d66a0\n"
            "li x11,  0x80e0bb20\n"
            "li x12,  0x80572de0\n"
            "li x13,  0x80f61420\n"
            "li x14,  0x80cd15a0\n"
            "li x15,  0x80a6d5a0\n"
            "li x16,  0x8098e9c0\n"
            "li x17,  0x80c61820\n"
            "li x18,  0x80d3a3e0\n"
            "li x19,  0x800da4a0\n"
            "li x20,  0x804bd420\n"
            "li x21,  0x8073e600\n"
            "li x22,  0x804c4100\n"
            "li x23,  0x80229d40\n"
            "li x24,  0x80e56040\n"
            "li x25,  0x80cdcd80\n"
            "li x26,  0x804255c0\n"
            "li x27,  0x801a81a0\n"
            "li x28,  0x8053f0c0\n"
            "li x29,  0x80c4bf60\n"
            "li x30,  0x807165e0\n"
            "li x31,  0x806c0ae0\n"
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
         "fpackrepb.pi f27, f30\n"                             // opcode=0x260f0dfb
         "fmin.s f28, f29, f31\n"                              // opcode=0x29fe8e53
         "fmv.x.s x29, f23\n"                                  // opcode=0xe00b8ed3
         "fmulh.pi f26, f31, f21\n"                            // opcode=0x175fad7b
         "fmax.s f29, f27, f29\n"                              // opcode=0x29dd9ed3
         "fmsub.ps f24, f21, f31, f27, dyn\n"                  // opcode=0xdbfafc5b
         "fsgnjx.s f28, f23, f24\n"                            // opcode=0x218bae53
         "fnmsub.s f25, f29, f31, f28, dyn\n"                  // opcode=0xe1fefccb
         //"fbc.ps f20, 52(x29)\n"                               // opcode=0x234e8a0b
         "cubesgnsc.ps f29, f21, f23\n"                        // opcode=0x897aaefb
         "fmul.s f29, f27, f22, dyn\n"                         // opcode=0x116dfed3
         "frcp.ps f22, f23\n"                                  // opcode=0x587b8b7b
         "fandi.pi f21, f24, 2\n"                              // opcode=0x042c1abf
         "flt.pi f29, f24, f20\n"                              // opcode=0xa74c1efb
         "fmul.pi f28, f26, f28\n"                             // opcode=0x17cd0e7b
         "fmulhu.pi f29, f22, f28\n"                           // opcode=0x17cb1efb
         //RTLMIN-5479"frsq.ps f21, f27\n"                                  // opcode=0x588d8afb
         "maskor m5, m4, m7\n"                                 // opcode=0x667262fb
         "fclass.ps f20, f30\n"                                // opcode=0xe00f1a7b
         "fround.ps f28, f22, dyn\n"                           // opcode=0x581b7e7b
         "cubeface.ps f26, f23, f28\n"                         // opcode=0x89cb8d7b
         "mova.m.x x21\n"                                      // opcode=0xd60a907b
         "mova.x.m x21\n"                                      // opcode=0xd6000afb
         "fmsub.ps f30, f31, f23, f28, dyn\n"                  // opcode=0xe37fff5b
         "fsgnjx.ps f30, f26, f24\n"                           // opcode=0x218d2f7b
         "fsra.pi f20, f30, f31\n"                             // opcode=0x0fff5a7b
         "maskor m7, m1, m4\n"                                 // opcode=0x6640e3fb
         "maskor m5, m3, m1\n"                                 // opcode=0x6611e2fb
         "fmv.s.x f28, x21\n"                                  // opcode=0xf00a8e53
         "fle.ps f26, f31, f26\n"                              // opcode=0xa1af8d7b
         "fcmov.ps f20, f26, f24, f30\n"                       // opcode=0xf58d2a3f
         "fle.s x24, f31, f23\n"                               // opcode=0xa17f8c53
         "maskpopc x31, m2\n"                                  // opcode=0x52010ffb
         "flt.s x25, f30, f27\n"                               // opcode=0xa1bf1cd3
         "fcmovm.ps f20, f25, f23\n"                           // opcode=0x017c8a77
         "fmulh.pi f23, f31, f25\n"                            // opcode=0x179fabfb
         "flt.s x24, f25, f31\n"                               // opcode=0xa1fc9c53
         "fmin.pi f29, f28, f23\n"                             // opcode=0x2f7e0efb
         "feq.pi f28, f28, f28\n"                              // opcode=0xa7ce2e7b
         "fmulh.pi f31, f26, f22\n"                            // opcode=0x176d2ffb
         "fsgnjn.s f31, f21, f24\n"                            // opcode=0x218a9fd3
         "fmax.s f31, f28, f20\n"                              // opcode=0x294e1fd3
         "fclass.s x24, f30\n"                                 // opcode=0xe00f1c53
         "maskor m7, m6, m1\n"                                 // opcode=0x661363fb
         "faddi.pi f29, f21, 987\n"                            // opcode=0xf5ba8ebf
         "fmin.ps f31, f30, f21\n"                             // opcode=0x295f0ffb
         "fsatu8.pi f27, f23\n"                                // opcode=0x061bbdfb
         "fsgnjn.s f31, f31, f21\n"                            // opcode=0x215f9fd3
         "fbci.ps f22, 416579\n"                               // opcode=0x65b43b1f
         "fbci.ps f26, 78111\n"                                // opcode=0x1311fd1f
         "fsgnjx.s f27, f27, f26\n"                            // opcode=0x21adadd3
         "ffrc.ps f25, f24\n"                                  // opcode=0x582c0cfb
         "fsgnj.ps f21, f30, f24\n"                            // opcode=0x218f0afb
         "fxor.pi f28, f24, f30\n"                             // opcode=0x07ec4e7b
         "fmadd.ps f22, f26, f22, f20, rup\n"                  // opcode=0xa16d3b5b
         "fexp.ps f22, f31\n"                                  // opcode=0x584f8b7b
         "fmin.s f29, f23, f31\n"                              // opcode=0x29fb8ed3
         "fsgnjx.ps f27, f31, f22\n"                           // opcode=0x216fadfb
         "fslli.pi f28, f27, 31\n"                             // opcode=0x4ffd9e7b
         "fmvs.x.ps x21, f20, 0\n"                             // opcode=0xe00a2afb
         "fbci.ps f20, 673862\n"                               // opcode=0xa4846a1f
         "flem.ps m1, f24, f20\n"                              // opcode=0xa14c40fb
         "cubesgntc.ps f28, f24, f26\n"                        // opcode=0x89ac3e7b
         "fmul.s f30, f26, f23, dyn\n"                         // opcode=0x117d7f53
         "fsub.s f30, f24, f24, dyn\n"                         // opcode=0x098c7f53
         "fsgnj.ps f29, f26, f23\n"                            // opcode=0x217d0efb
         "fmv.s.x f24, x21\n"                                  // opcode=0xf00a8c53
         "cubesgntc.ps f30, f28, f28\n"                        // opcode=0x89ce3f7b
         "fsgnj.ps f24, f21, f22\n"                            // opcode=0x216a8c7b
         "fmsub.s f21, f23, f21, f30, dyn\n"                   // opcode=0xf15bfac7
         "mova.x.m x20\n"                                      // opcode=0xd6000a7b
         "fnmadd.s f21, f29, f28, f24, dyn\n"                  // opcode=0xc1cefacf
         "fsub.pi f27, f26, f26\n"                             // opcode=0x0fad0dfb
         "fltm.pi m5, f30, f21\n"                              // opcode=0x3f5f02fb
         "feq.s x20, f21, f26\n"                               // opcode=0xa1aaaa53
         "fsub.pi f30, f22, f29\n"                             // opcode=0x0fdb0f7b
         "fsq2 f26, 0 (x30)\n"                                 // opcode=0x27af5e27
         "fmul.s f27, f21, f30, dyn\n"                         // opcode=0x11eafdd3
         "fltm.ps m7, f23, f20\n"                              // opcode=0xa14bd3fb
         "fsrl.pi f27, f21, f24\n"                             // opcode=0x078addfb
         "fpackrepb.pi f23, f24\n"                             // opcode=0x260c0bfb
         "fadd.s f21, f26, f31, dyn\n"                         // opcode=0x01fd7ad3
         "fle.ps f25, f21, f26\n"                              // opcode=0xa1aa8cfb
         "fltm.pi m2, f20, f25\n"                              // opcode=0x3f9a017b
         "flem.ps m1, f22, f21\n"                              // opcode=0xa15b40fb
         "fpackrepb.pi f29, f28\n"                             // opcode=0x260e0efb
         "cubeface.ps f30, f21, f29\n"                         // opcode=0x89da8f7b
         "fround.ps f25, f23, dyn\n"                           // opcode=0x581bfcfb
         "fmul.ps f23, f23, f20, dyn\n"                        // opcode=0x114bfbfb
         "flt.ps f21, f23, f28\n"                              // opcode=0xa1cb9afb
         "fadd.s f24, f20, f31, dyn\n"                         // opcode=0x01fa7c53
         "fcmovm.ps f24, f22, f22\n"                           // opcode=0x016b0c77
         "fslli.pi f22, f21, 30\n"                             // opcode=0x4fea9b7b
         "fbci.pi f20, 1037404\n"                              // opcode=0xfd45ca5f
         "maskxor m5, m0, m2\n"                                // opcode=0x662042fb
         "fbci.ps f26, 834203\n"                               // opcode=0xcba9bd1f
         "fsll.pi f24, f22, f25\n"                             // opcode=0x079b1c7b
         "frcp.ps f25, f23\n"                                  // opcode=0x587b8cfb
         "fbci.pi f22, 761823\n"                               // opcode=0xb9fdfb5f
         //RTLMIN-5479"fsin.ps f20, f31\n"                                  // opcode=0x586f8a7b
         "frcp.ps f29, f28\n"                                  // opcode=0x587e0efb
         "fmin.ps f27, f31, f25\n"                             // opcode=0x299f8dfb
         "ffrc.ps f31, f22\n"                                  // opcode=0x582b0ffb
         "fpackrepb.pi f30, f22\n"                             // opcode=0x260b0f7b
         "fmin.pi f24, f31, f21\n"                             // opcode=0x2f5f8c7b
         "flem.ps m3, f27, f30\n"                              // opcode=0xa1edc1fb
         "fmax.s f29, f27, f26\n"                              // opcode=0x29ad9ed3
         "fmsub.s f21, f25, f21, f30, dyn\n"                   // opcode=0xf15cfac7
         "maskxor m6, m0, m3\n"                                // opcode=0x6630437b
         "fcmov.ps f26, f31, f20, f20\n"                       // opcode=0xa54fad3f
         "fadd.ps f23, f28, f30, dyn\n"                        // opcode=0x01ee7bfb
         "fltu.pi f26, f23, f24\n"                             // opcode=0xa78bbd7b
         "flt.ps f24, f21, f30\n"                              // opcode=0xa1ea9c7b
         "fmvs.x.ps x23, f27, 6\n"                             // opcode=0xe06dabfb
         "fmin.s f30, f31, f26\n"                              // opcode=0x29af8f53
         "fsgnj.s f22, f20, f27\n"                             // opcode=0x21ba0b53
         "maskpopcz x21, m2\n"                                 // opcode=0x54010afb
         "fltm.pi m2, f28, f22\n"                              // opcode=0x3f6e017b
         "fround.ps f24, f27, dyn\n"                           // opcode=0x581dfc7b
         "fmadd.ps f21, f22, f26, f27, dyn\n"                  // opcode=0xd9ab7adb
         "fsrl.pi f24, f28, f24\n"                             // opcode=0x078e5c7b
         "fround.ps f28, f29, dyn\n"                           // opcode=0x581efe7b
         "fround.ps f21, f24, dyn\n"                           // opcode=0x581c7afb
         "fsrli.pi f21, f29, 15\n"                             // opcode=0x4efedafb
         "cubeface.ps f27, f22, f27\n"                         // opcode=0x89bb0dfb
         "fbci.ps f23, 627393\n"                               // opcode=0x992c1b9f
         "fsgnjn.s f20, f25, f29\n"                            // opcode=0x21dc9a53
         "fsub.pi f31, f22, f20\n"                             // opcode=0x0f4b0ffb
         "fle.s x28, f30, f26\n"                               // opcode=0xa1af0e53
         "fadd.pi f28, f24, f29\n"                             // opcode=0x07dc0e7b
         "fclass.ps f21, f26\n"                                // opcode=0xe00d1afb
         "fsrl.pi f27, f24, f24\n"                             // opcode=0x078c5dfb
         "fbcx.ps f21, x26\n"                                  // opcode=0x000d3a8b
         "feq.ps f26, f26, f20\n"                              // opcode=0xa14d2d7b
         "ffrc.ps f20, f21\n"                                  // opcode=0x582a8a7b
         "fmax.s f28, f22, f22\n"                              // opcode=0x296b1e53
         "fxor.pi f21, f21, f22\n"                             // opcode=0x076acafb
         "fle.pi f28, f28, f25\n"                              // opcode=0xa79e0e7b
         "fsra.pi f23, f29, f25\n"                             // opcode=0x0f9edbfb
         "cubesgntc.ps f25, f25, f31\n"                        // opcode=0x89fcbcfb
         "fsat8.pi f20, f27\n"                                 // opcode=0x060dba7b
         "fandi.pi f29, f24, 440\n"                            // opcode=0x6d8c1ebf
         "fmax.ps f23, f24, f23\n"                             // opcode=0x297c1bfb
         "flog.ps f24, f28\n"                                  // opcode=0x583e0c7b
         "feq.ps f27, f23, f30\n"                              // opcode=0xa1ebadfb
         "fsgnjx.ps f30, f27, f27\n"                           // opcode=0x21bdaf7b
         "fpackrepb.pi f26, f26\n"                             // opcode=0x260d0d7b
         "cubeface.ps f27, f28, f22\n"                         // opcode=0x896e0dfb
         "fnot.pi f24, f23\n"                                  // opcode=0x060bac7b
         "fmin.s f23, f21, f31\n"                              // opcode=0x29fa8bd3
         "fle.pi f20, f22, f31\n"                              // opcode=0xa7fb0a7b
         "feq.ps f29, f28, f24\n"                              // opcode=0xa18e2efb
         "fpackreph.pi f21, f28\n"                             // opcode=0x260e1afb
         "fsetm.pi m1, f27\n"                                  // opcode=0xe00dd0fb
         "maskpopc x20, m1\n"                                  // opcode=0x52008a7b
         "fmul.pi f31, f29, f31\n"                             // opcode=0x17fe8ffb
         "fsq2 f27, 0 (x21)\n"                                 // opcode=0x99bad527
         "fbci.pi f20, 127341\n"                               // opcode=0x1f16da5f
         "feq.ps f31, f25, f30\n"                              // opcode=0xa1ecaffb
         "mova.m.x x31\n"                                      // opcode=0xd60f907b
         "mova.x.m x29\n"                                      // opcode=0xd6000efb
         "feq.ps f25, f20, f24\n"                              // opcode=0xa18a2cfb
         "fmin.pi f24, f24, f30\n"                             // opcode=0x2fec0c7b
         "fmsub.s f28, f23, f29, f27, dyn\n"                   // opcode=0xd9dbfe47
         "fclass.s x27, f28\n"                                 // opcode=0xe00e1dd3
         "fmadd.s f20, f30, f25, f21, dyn\n"                   // opcode=0xa99f7a43
         "fmul.ps f29, f26, f23, rdn\n"                        // opcode=0x117d2efb
         "feqm.ps m1, f31, f22\n"                              // opcode=0xa16fe0fb
         "feq.s x26, f31, f23\n"                               // opcode=0xa17fad53
         "fmv.x.s x28, f21\n"                                  // opcode=0xe00a8e53
         "fsrai.pi f27, f29, 31\n"                             // opcode=0x4ffefdfb
         "fsub.s f20, f28, f25, dyn\n"                         // opcode=0x099e7a53
         "fsub.s f23, f23, f29, dyn\n"                         // opcode=0x09dbfbd3
         "frcp.ps f27, f31\n"                                  // opcode=0x587f8dfb
         "for.pi f26, f22, f28\n"                              // opcode=0x07cb6d7b
         "fsatu8.pi f30, f27\n"                                // opcode=0x061dbf7b
         "fadd.ps f26, f30, f21, rdn\n"                        // opcode=0x015f2d7b
         "maskand m6, m4, m0\n"                                // opcode=0x6602737b
         "fmin.s f24, f21, f31\n"                              // opcode=0x29fa8c53
         "fmaxu.pi f30, f25, f26\n"                            // opcode=0x2facbf7b
         "cubesgnsc.ps f26, f22, f25\n"                        // opcode=0x899b2d7b
         "fsll.pi f23, f26, f21\n"                             // opcode=0x075d1bfb
         "fadd.pi f26, f28, f31\n"                             // opcode=0x07fe0d7b
         "fmul.ps f26, f25, f21, dyn\n"                        // opcode=0x115cfd7b
         "fmsub.ps f23, f23, f20, f28, dyn\n"                  // opcode=0xe34bfbdb
         "fcmov.ps f31, f28, f26, f31\n"                       // opcode=0xfdae2fbf
         //RTLMIN-5479"fsin.ps f24, f31\n"                                  // opcode=0x586f8c7b
         "fclass.s x25, f27\n"                                 // opcode=0xe00d9cd3
         "fround.ps f27, f26, dyn\n"                           // opcode=0x581d7dfb
         "fexp.ps f29, f31\n"                                  // opcode=0x584f8efb
         "flog.ps f23, f25\n"                                  // opcode=0x583c8bfb
         "flt.ps f31, f22, f25\n"                              // opcode=0xa19b1ffb
         "fsgnjx.ps f23, f21, f25\n"                           // opcode=0x219aabfb
         "fsll.pi f24, f27, f22\n"                             // opcode=0x076d9c7b
         "fmulh.pi f28, f29, f26\n"                            // opcode=0x17aeae7b
         "fadd.ps f29, f21, f25, dyn\n"                        // opcode=0x019afefb
         "fmadd.s f30, f31, f25, f24, dyn\n"                   // opcode=0xc19fff43
         "fltm.ps m2, f25, f30\n"                              // opcode=0xa1ecd17b
         "fslli.pi f27, f29, 11\n"                             // opcode=0x4ebe9dfb
         "fltm.pi m0, f28, f26\n"                              // opcode=0x3fae007b
         "fltm.ps m4, f28, f27\n"                              // opcode=0xa1be527b
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fxor.pi f20, f29, f26\n"                             // opcode=0x07aeca7b
         "fsq2 f26, 0 (x27)\n"                                 // opcode=0x79add8a7
         "fmulh.pi f31, f29, f29\n"                            // opcode=0x17deaffb
         "flem.ps m3, f31, f30\n"                              // opcode=0xa1efc1fb
         "maskxor m4, m0, m6\n"                                // opcode=0x6660427b
         "fnmsub.ps f30, f30, f22, f30, dyn\n"                 // opcode=0xf56f7f5b
         "fsetm.pi m0, f29\n"                                  // opcode=0xe00ed07b
         "fbc.ps f27, 22(x26)\n"                               // opcode=0xa16d0d8b
         "flog.ps f28, f28\n"                                  // opcode=0x583e0e7b
         "fbc.ps f22, 76(x31)\n"                               // opcode=0x94cf8b0b
         "ffrc.ps f29, f20\n"                                  // opcode=0x582a0efb
         "fmin.ps f21, f30, f27\n"                             // opcode=0x29bf0afb
         "fmin.ps f25, f29, f27\n"                             // opcode=0x29be8cfb
         "fsub.pi f28, f29, f27\n"                             // opcode=0x0fbe8e7b
         "fmaxu.pi f27, f25, f23\n"                            // opcode=0x2f7cbdfb
         "mova.x.m x31\n"                                      // opcode=0xd6000ffb
         "masknot m5, m2\n"                                    // opcode=0x660122fb
         "fexp.ps f30, f20\n"                                  // opcode=0x584a0f7b
         "flt.s x24, f26, f25\n"                               // opcode=0xa19d1c53
         "fsrli.pi f26, f30, 6\n"                              // opcode=0x4e6f5d7b
         "flt.ps f27, f27, f20\n"                              // opcode=0xa14d9dfb
         "fmax.ps f22, f29, f29\n"                             // opcode=0x29de9b7b
         "fsgnj.ps f29, f25, f30\n"                            // opcode=0x21ec8efb
         "maskxor m0, m1, m1\n"                                // opcode=0x6610c07b
         "fbcx.ps f26, x25\n"                                  // opcode=0x000cbd0b
         "fmul.ps f20, f26, f21, dyn\n"                        // opcode=0x115d7a7b
         //RTLMIN-5479"frsq.ps f27, f25\n"                                  // opcode=0x588c8dfb
         "maskand m6, m2, m7\n"                                // opcode=0x6671737b
         "mova.x.m x25\n"                                      // opcode=0xd6000cfb
         "fmv.s.x f20, x20\n"                                  // opcode=0xf00a0a53
         "fnmadd.ps f31, f23, f25, f23, dyn\n"                 // opcode=0xbf9bffdb
         "fmaxu.pi f31, f28, f31\n"                            // opcode=0x2ffe3ffb
         "fsgnjn.ps f24, f25, f26\n"                           // opcode=0x21ac9c7b
         "fxor.pi f22, f25, f25\n"                             // opcode=0x079ccb7b
         "fsub.s f30, f20, f28, dyn\n"                         // opcode=0x09ca7f53
         "cubesgnsc.ps f28, f20, f20\n"                        // opcode=0x894a2e7b
         "fsrli.pi f27, f30, 28\n"                             // opcode=0x4fcf5dfb
         "fsgnj.ps f25, f25, f30\n"                            // opcode=0x21ec8cfb
         "fcmovm.ps f23, f23, f22\n"                           // opcode=0x016b8bf7
         "fltm.pi m1, f23, f21\n"                              // opcode=0x3f5b80fb
         "fmsub.ps f25, f21, f23, f22, dyn\n"                  // opcode=0xb37afcdb
         "fsra.pi f31, f31, f31\n"                             // opcode=0x0fffdffb
         "maskxor m2, m2, m3\n"                                // opcode=0x6631417b
         "fsrai.pi f24, f25, 4\n"                              // opcode=0x4e4cfc7b
         "feq.ps f21, f22, f25\n"                              // opcode=0xa19b2afb
         "fsgnjn.s f28, f30, f31\n"                            // opcode=0x21ff1e53
         "fcmov.ps f28, f28, f30, f28\n"                       // opcode=0xe5ee2e3f
         "fmin.s f24, f24, f20\n"                              // opcode=0x294c0c53
         "fsrl.pi f31, f28, f20\n"                             // opcode=0x074e5ffb
         "feq.s x26, f24, f29\n"                               // opcode=0xa1dc2d53
         "li x2, 3205\n"
         "mova.m.x x5\n"
         "fmsub.ps f27, f30, f24, f28, dyn\n"                  // opcode=0xe38f7ddb
         "cubesgntc.ps f30, f30, f27\n"                        // opcode=0x89bf3f7b
         "fsetm.pi m1, f26\n"                                  // opcode=0xe00d50fb
         "fmin.s f26, f20, f22\n"                              // opcode=0x296a0d53
         "fle.s x24, f29, f31\n"                               // opcode=0xa1fe8c53
         //RTLMIN-5479"frsq.ps f30, f22\n"                                  // opcode=0x588b0f7b
         "flt.ps f24, f30, f26\n"                              // opcode=0xa1af1c7b
         "fxor.pi f24, f21, f20\n"                             // opcode=0x074acc7b
         "fsgnjx.s f25, f30, f21\n"                            // opcode=0x215f2cd3
         "maskpopcz x23, m4\n"                                 // opcode=0x54020bfb
         "fle.ps f28, f27, f25\n"                              // opcode=0xa19d8e7b
         "faddi.pi f28, f23, 945\n"                            // opcode=0xed1b8e3f
         "fmin.pi f29, f30, f24\n"                             // opcode=0x2f8f0efb
         "feq.pi f26, f23, f22\n"                              // opcode=0xa76bad7b
         "cubefaceidx.ps f23, f22, f24\n"                      // opcode=0x898b1bfb
         "fmax.ps f29, f20, f30\n"                             // opcode=0x29ea1efb
         "fsgnjx.ps f23, f29, f29\n"                           // opcode=0x21deabfb
         "flog.ps f22, f28\n"                                  // opcode=0x583e0b7b
         "fsgnj.s f28, f25, f31\n"                             // opcode=0x21fc8e53
         "fsgnj.s f24, f23, f24\n"                             // opcode=0x218b8c53
         "flt.s x20, f21, f31\n"                               // opcode=0xa1fa9a53
         "fminu.pi f24, f30, f21\n"                            // opcode=0x2f5f2c7b
         "fsll.pi f31, f27, f24\n"                             // opcode=0x078d9ffb
         "fmv.x.s x29, f24\n"                                  // opcode=0xe00c0ed3
         "fnmsub.ps f30, f26, f26, f28, dyn\n"                 // opcode=0xe5ad7f5b
         //RTLMIN-5479"frsq.ps f20, f27\n"                                  // opcode=0x588d8a7b
         "fnmadd.ps f20, f21, f24, f20, dyn\n"                 // opcode=0xa78afa5b
         "fnot.pi f24, f24\n"                                  // opcode=0x060c2c7b
         "feq.pi f25, f26, f27\n"                              // opcode=0xa7bd2cfb
         "fnmadd.ps f30, f28, f24, f20, dyn\n"                 // opcode=0xa78e7f5b
         "fnmsub.s f22, f26, f25, f28, rmm\n"                  // opcode=0xe19d4b4b
         //RTLMIN-5479"fsin.ps f29, f22\n"                                  // opcode=0x586b0efb
         "fcmovm.ps f24, f29, f29\n"                           // opcode=0x01de8c77
         "fadd.ps f26, f30, f24, dyn\n"                        // opcode=0x018f7d7b
         "fmin.ps f23, f22, f30\n"                             // opcode=0x29eb0bfb
         "cubesgntc.ps f22, f30, f20\n"                        // opcode=0x894f3b7b
         "feq.s x22, f26, f22\n"                               // opcode=0xa16d2b53
         "fltu.pi f31, f30, f28\n"                             // opcode=0xa7cf3ffb
         "fslli.pi f20, f20, 29\n"                             // opcode=0x4fda1a7b
         "for.pi f25, f30, f27\n"                              // opcode=0x07bf6cfb
         "feq.pi f28, f26, f21\n"                              // opcode=0xa75d2e7b
         "fltm.ps m1, f25, f29\n"                              // opcode=0xa1dcd0fb
         "fmvs.x.ps x30, f22, 2\n"                             // opcode=0xe02b2f7b
         "feq.s x26, f30, f30\n"                               // opcode=0xa1ef2d53
         "fminu.pi f30, f31, f29\n"                            // opcode=0x2fdfaf7b
         "flt.s x31, f21, f21\n"                               // opcode=0xa15a9fd3
         "fmul.pi f24, f23, f28\n"                             // opcode=0x17cb8c7b
         "fmin.ps f26, f28, f22\n"                             // opcode=0x296e0d7b
         "maskpopc x29, m7\n"                                  // opcode=0x52038efb
         "cubesgnsc.ps f24, f30, f31\n"                        // opcode=0x89ff2c7b
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "maskpopc x21, m1\n"                                  // opcode=0x52008afb
         "feqm.ps m6, f23, f22\n"                              // opcode=0xa16be37b
         "fmadd.ps f20, f24, f25, f28, dyn\n"                  // opcode=0xe19c7a5b
         "fnmsub.s f20, f20, f26, f25, dyn\n"                  // opcode=0xc9aa7a4b
         "fexp.ps f31, f22\n"                                  // opcode=0x584b0ffb
         "fnot.pi f22, f25\n"                                  // opcode=0x060cab7b
         "fbcx.ps f25, x26\n"                                  // opcode=0x000d3c8b
         "fsatu8.pi f27, f20\n"                                // opcode=0x061a3dfb
         "fsrl.pi f28, f27, f21\n"                             // opcode=0x075dde7b
         "fandi.pi f20, f30, 132\n"                            // opcode=0x244f1a3f
         "mova.x.m x21\n"                                      // opcode=0xd6000afb
         "fadd.s f22, f26, f25, dyn\n"                         // opcode=0x019d7b53
         "fmsub.ps f20, f31, f25, f22, dyn\n"                  // opcode=0xb39ffa5b
         "fclass.ps f24, f26\n"                                // opcode=0xe00d1c7b
         "fnot.pi f24, f20\n"                                  // opcode=0x060a2c7b
         "fbc.ps f28, 82(x28)\n"                               // opcode=0x752e0e0b
         "fmul.s f27, f31, f21, dyn\n"                         // opcode=0x115ffdd3
         "fsub.s f22, f27, f22, dyn\n"                         // opcode=0x096dfb53
         //RTLMIN-5479"frsq.ps f29, f30\n"                                  // opcode=0x588f0efb
         "cubeface.ps f26, f29, f31\n"                         // opcode=0x89fe8d7b
         "maskor m1, m1, m2\n"                                 // opcode=0x6620e0fb
         "fsgnjx.ps f20, f27, f28\n"                           // opcode=0x21cdaa7b
         "fmin.pi f24, f29, f20\n"                             // opcode=0x2f4e8c7b
         "fsrli.pi f30, f20, 3\n"                              // opcode=0x4e3a5f7b
         "maskor m5, m3, m7\n"                                 // opcode=0x6671e2fb
         "flt.pi f28, f23, f26\n"                              // opcode=0xa7ab9e7b
         "fnmsub.s f31, f27, f23, f22, dyn\n"                  // opcode=0xb17dffcb
         "fmadd.s f24, f31, f31, f26, dyn\n"                   // opcode=0xd1fffc43
         "fbci.pi f30, 540365\n"                               // opcode=0x83ecdf5f
         "fbcx.ps f31, x22\n"                                  // opcode=0x000b3f8b
         "cubefaceidx.ps f27, f22, f20\n"                      // opcode=0x894b1dfb
         "fmulhu.pi f30, f30, f24\n"                           // opcode=0x178f1f7b
         "fmvz.x.ps x29, f25, 6\n"                             // opcode=0xe06c8efb
         "fmax.s f27, f28, f28\n"                              // opcode=0x29ce1dd3
         "fmulhu.pi f29, f23, f25\n"                           // opcode=0x179b9efb
         "mova.m.x x24\n"                                      // opcode=0xd60c107b
         "fnmadd.ps f26, f24, f21, f23, dyn\n"                 // opcode=0xbf5c7d5b
         "flog.ps f20, f23\n"                                  // opcode=0x583b8a7b
         "maskpopc x31, m7\n"                                  // opcode=0x52038ffb
         "flt.s x25, f28, f30\n"                               // opcode=0xa1ee1cd3
         "fsrai.pi f23, f27, 29\n"                             // opcode=0x4fddfbfb
         "cubeface.ps f31, f23, f23\n"                         // opcode=0x897b8ffb
         "fclass.ps f30, f27\n"                                // opcode=0xe00d9f7b
         "cubesgntc.ps f27, f27, f22\n"                        // opcode=0x896dbdfb
         "mova.m.x x20\n"                                      // opcode=0xd60a107b
         "fsub.ps f26, f22, f22, dyn\n"                        // opcode=0x096b7d7b
         "fpackrepb.pi f23, f22\n"                             // opcode=0x260b0bfb
         "fsat8.pi f28, f21\n"                                 // opcode=0x060abe7b
         "flt.s x26, f29, f25\n"                               // opcode=0xa19e9d53
         "flt.pi f24, f30, f26\n"                              // opcode=0xa7af1c7b
         "li x2, 3847\n"
         "mova.m.x x5\n"
         "feq.s x23, f26, f27\n"                               // opcode=0xa1bd2bd3
         "fsatu8.pi f29, f29\n"                                // opcode=0x061ebefb
         "cubefaceidx.ps f26, f31, f31\n"                      // opcode=0x89ff9d7b
         "fpackreph.pi f24, f23\n"                             // opcode=0x260b9c7b
         "fmulhu.pi f21, f24, f25\n"                           // opcode=0x179c1afb
         "fsrli.pi f24, f31, 17\n"                             // opcode=0x4f1fdc7b
         "flem.ps m4, f22, f30\n"                              // opcode=0xa1eb427b
         "fclass.ps f31, f28\n"                                // opcode=0xe00e1ffb
         "cubesgntc.ps f27, f23, f20\n"                        // opcode=0x894bbdfb
         "fsetm.pi m0, f25\n"                                  // opcode=0xe00cd07b
         "fclass.s x30, f28\n"                                 // opcode=0xe00e1f53
         "fmin.s f26, f22, f29\n"                              // opcode=0x29db0d53
         "fbci.ps f21, 418479\n"                               // opcode=0x662afa9f
         "fsrli.pi f28, f26, 31\n"                             // opcode=0x4ffd5e7b
         "fmin.pi f31, f23, f27\n"                             // opcode=0x2fbb8ffb
         "fclass.s x30, f22\n"                                 // opcode=0xe00b1f53
         "fle.s x22, f26, f22\n"                               // opcode=0xa16d0b53
         "fnmsub.s f20, f26, f31, f31, dyn\n"                  // opcode=0xf9fd7a4b
         "fmvz.x.ps x26, f24, 5\n"                             // opcode=0xe05c0d7b
         "fnmadd.ps f21, f23, f30, f23, dyn\n"                 // opcode=0xbfebfadb
         "fslli.pi f27, f31, 12\n"                             // opcode=0x4ecf9dfb
         "maskand m4, m6, m7\n"                                // opcode=0x6673727b
         "fmin.pi f28, f31, f24\n"                             // opcode=0x2f8f8e7b
         "fnmsub.s f29, f30, f27, f30, dyn\n"                  // opcode=0xf1bf7ecb
         "flt.ps f21, f31, f20\n"                              // opcode=0xa14f9afb
         "fround.ps f29, f25, dyn\n"                           // opcode=0x581cfefb
         "fsrai.pi f22, f30, 17\n"                             // opcode=0x4f1f7b7b
         "fmulhu.pi f23, f30, f21\n"                           // opcode=0x175f1bfb
         "flem.ps m6, f21, f26\n"                              // opcode=0xa1aac37b
         "flt.pi f23, f27, f29\n"                              // opcode=0xa7dd9bfb
         "fsat8.pi f20, f23\n"                                 // opcode=0x060bba7b
         "feq.pi f21, f27, f27\n"                              // opcode=0xa7bdaafb
         "fmin.pi f21, f25, f27\n"                             // opcode=0x2fbc8afb
         "cubesgnsc.ps f31, f25, f27\n"                        // opcode=0x89bcaffb
         "fxor.pi f31, f21, f23\n"                             // opcode=0x077acffb
         "flt.ps f30, f26, f21\n"                              // opcode=0xa15d1f7b
         "fclass.ps f24, f31\n"                                // opcode=0xe00f9c7b
         "fmin.ps f25, f29, f31\n"                             // opcode=0x29fe8cfb
         "feq.ps f23, f21, f20\n"                              // opcode=0xa14aabfb
         "fsrli.pi f24, f23, 28\n"                             // opcode=0x4fcbdc7b
         "flt.pi f22, f28, f23\n"                              // opcode=0xa77e1b7b
         "fle.pi f29, f28, f27\n"                              // opcode=0xa7be0efb
         "fminu.pi f25, f25, f27\n"                            // opcode=0x2fbcacfb
         "faddi.pi f21, f20, 261\n"                            // opcode=0x445a0abf
         "fmadd.ps f24, f23, f20, f24, rdn\n"                  // opcode=0xc14bac5b
         "fsat8.pi f30, f20\n"                                 // opcode=0x060a3f7b
         //RTLMIN-5479"fsin.ps f27, f30\n"                                  // opcode=0x586f0dfb
         "fnot.pi f28, f26\n"                                  // opcode=0x060d2e7b
         "mova.x.m x21\n"                                      // opcode=0xd6000afb
         "fslli.pi f27, f31, 1\n"                              // opcode=0x4e1f9dfb
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fsatu8.pi f23, f31\n"                                // opcode=0x061fbbfb
         "fsgnjx.s f31, f23, f30\n"                            // opcode=0x21ebafd3
         "fadd.s f23, f23, f31, dyn\n"                         // opcode=0x01fbfbd3
         "fbcx.ps f31, x25\n"                                  // opcode=0x000cbf8b
         "fsgnjx.ps f27, f21, f20\n"                           // opcode=0x214aadfb
         "fmsub.ps f21, f26, f25, f20, dyn\n"                  // opcode=0xa39d7adb
         "fmin.pi f23, f29, f27\n"                             // opcode=0x2fbe8bfb
         "fsrl.pi f29, f23, f22\n"                             // opcode=0x076bdefb
         "fmin.pi f31, f20, f26\n"                             // opcode=0x2faa0ffb
         "maskpopcz x21, m4\n"                                 // opcode=0x54020afb
         "fbcx.ps f23, x31\n"                                  // opcode=0x000fbb8b
         "fexp.ps f30, f29\n"                                  // opcode=0x584e8f7b
         "fadd.pi f27, f22, f26\n"                             // opcode=0x07ab0dfb
         //RTLMIN-5479"frsq.ps f30, f29\n"                                  // opcode=0x588e8f7b
         "fmulh.pi f27, f22, f30\n"                            // opcode=0x17eb2dfb
         "fmsub.s f23, f20, f30, f20, dyn\n"                   // opcode=0xa1ea7bc7
         "fminu.pi f23, f29, f27\n"                            // opcode=0x2fbeabfb
         "fmulhu.pi f24, f30, f25\n"                           // opcode=0x179f1c7b
         "fsll.pi f30, f20, f28\n"                             // opcode=0x07ca1f7b
         "fclass.ps f25, f26\n"                                // opcode=0xe00d1cfb
         "fsub.ps f20, f24, f29, dyn\n"                        // opcode=0x09dc7a7b
         "faddi.pi f22, f27, 914\n"                            // opcode=0xe52d8b3f
         "fbcx.ps f21, x29\n"                                  // opcode=0x000eba8b
         "fle.pi f26, f29, f24\n"                              // opcode=0xa78e8d7b
         "feq.s x25, f25, f20\n"                               // opcode=0xa14cacd3
         "cubefaceidx.ps f27, f26, f29\n"                      // opcode=0x89dd1dfb
         "fmadd.s f30, f22, f21, f23, dyn\n"                   // opcode=0xb95b7f43
         "fmulhu.pi f20, f31, f26\n"                           // opcode=0x17af9a7b
         "for.pi f27, f30, f22\n"                              // opcode=0x076f6dfb
         "fround.ps f25, f31, dyn\n"                           // opcode=0x581ffcfb
         "fsgnjn.s f21, f23, f25\n"                            // opcode=0x219b9ad3
         "maskand m7, m2, m7\n"                                // opcode=0x667173fb
         "fmvs.x.ps x25, f20, 7\n"                             // opcode=0xe07a2cfb
         "fsgnj.ps f21, f29, f30\n"                            // opcode=0x21ee8afb
         "flt.s x21, f28, f27\n"                               // opcode=0xa1be1ad3
         "fsetm.pi m7, f23\n"                                  // opcode=0xe00bd3fb
         "fsatu8.pi f27, f31\n"                                // opcode=0x061fbdfb
         "fmin.ps f30, f30, f29\n"                             // opcode=0x29df0f7b
         "fpackrepb.pi f30, f22\n"                             // opcode=0x260b0f7b
         "fmulh.pi f20, f30, f31\n"                            // opcode=0x17ff2a7b
         "fnot.pi f28, f20\n"                                  // opcode=0x060a2e7b
         "fnmsub.ps f21, f21, f27, f31, dyn\n"                 // opcode=0xfdbafadb
         "fnot.pi f23, f23\n"                                  // opcode=0x060babfb
         "fbcx.ps f29, x21\n"                                  // opcode=0x000abe8b
         "fmsub.ps f21, f31, f26, f28, dyn\n"                  // opcode=0xe3affadb
         "feqm.ps m3, f22, f27\n"                              // opcode=0xa1bb61fb
         "fnmsub.s f23, f31, f31, f29, rne\n"                  // opcode=0xe9ff8bcb
         "masknot m0, m3\n"                                    // opcode=0x6601a07b
         "fmin.ps f21, f26, f23\n"                             // opcode=0x297d0afb
         "fmin.pi f28, f31, f26\n"                             // opcode=0x2faf8e7b
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         //RTLMIN-5479"frsq.ps f27, f26\n"                                  // opcode=0x588d0dfb
         "fmax.s f27, f20, f23\n"                              // opcode=0x297a1dd3
         "fmax.pi f23, f20, f29\n"                             // opcode=0x2fda1bfb
         "mova.m.x x27\n"                                      // opcode=0xd60d907b
         "fsrl.pi f21, f27, f30\n"                             // opcode=0x07eddafb
         "fmulhu.pi f28, f31, f31\n"                           // opcode=0x17ff9e7b
         "fmsub.s f22, f21, f27, f25, dyn\n"                   // opcode=0xc9bafb47
         "fadd.pi f20, f31, f28\n"                             // opcode=0x07cf8a7b
         "fmul.s f20, f25, f20, dyn\n"                         // opcode=0x114cfa53
         "feq.s x22, f26, f20\n"                               // opcode=0xa14d2b53
         "flt.s x29, f23, f26\n"                               // opcode=0xa1ab9ed3
         "fsat8.pi f24, f31\n"                                 // opcode=0x060fbc7b
         "fsatu8.pi f30, f26\n"                                // opcode=0x061d3f7b
         "fnmsub.s f30, f31, f23, f27, dyn\n"                  // opcode=0xd97fff4b
         "fltm.pi m3, f29, f28\n"                              // opcode=0x3fce81fb
         "fsetm.pi m2, f25\n"                                  // opcode=0xe00cd17b
         "fsgnjx.s f27, f30, f30\n"                            // opcode=0x21ef2dd3
         "feq.s x25, f22, f21\n"                               // opcode=0xa15b2cd3
         "fsgnj.s f27, f27, f29\n"                             // opcode=0x21dd8dd3
         "frcp.ps f29, f21\n"                                  // opcode=0x587a8efb
         "fcmov.ps f21, f28, f26, f28\n"                       // opcode=0xe5ae2abf
         "fnot.pi f26, f29\n"                                  // opcode=0x060ead7b
         "fclass.s x30, f23\n"                                 // opcode=0xe00b9f53
         "fclass.ps f22, f27\n"                                // opcode=0xe00d9b7b
         "fnmadd.ps f22, f31, f26, f20, rdn\n"                 // opcode=0xa7afab5b
         "fxor.pi f29, f20, f31\n"                             // opcode=0x07fa4efb
         "mova.x.m x29\n"                                      // opcode=0xd6000efb
         "flt.ps f29, f27, f24\n"                              // opcode=0xa18d9efb
         "fbci.ps f28, 373224\n"                               // opcode=0x5b1e8e1f
         "fmadd.ps f23, f25, f28, f29, dyn\n"                  // opcode=0xe9ccfbdb
         "fsetm.pi m1, f27\n"                                  // opcode=0xe00dd0fb
         "fsrli.pi f24, f22, 25\n"                             // opcode=0x4f9b5c7b
         "fpackreph.pi f23, f31\n"                             // opcode=0x260f9bfb
         "fltm.pi m4, f31, f25\n"                              // opcode=0x3f9f827b
         "feq.pi f26, f29, f30\n"                              // opcode=0xa7eead7b
         "cubesgnsc.ps f29, f27, f31\n"                        // opcode=0x89fdaefb
         "fnmadd.s f24, f21, f25, f30, dyn\n"                  // opcode=0xf19afc4f
         "flt.s x26, f27, f21\n"                               // opcode=0xa15d9d53
         //RTLMIN-5479"frsq.ps f29, f30\n"                                  // opcode=0x588f0efb
         "fadd.pi f26, f27, f28\n"                             // opcode=0x07cd8d7b
         "ffrc.ps f23, f30\n"                                  // opcode=0x582f0bfb
         "cubefaceidx.ps f24, f27, f28\n"                      // opcode=0x89cd9c7b
         "fmul.ps f26, f30, f25, dyn\n"                        // opcode=0x119f7d7b
         "mova.m.x x30\n"                                      // opcode=0xd60f107b
         "fbcx.ps f31, x31\n"                                  // opcode=0x000fbf8b
         "fmulh.pi f22, f28, f25\n"                            // opcode=0x179e2b7b
         "fnmadd.s f26, f24, f25, f22, dyn\n"                  // opcode=0xb19c7d4f
         "fmaxu.pi f20, f30, f27\n"                            // opcode=0x2fbf3a7b
         "flt.pi f22, f27, f23\n"                              // opcode=0xa77d9b7b
         "frcp.ps f20, f21\n"                                  // opcode=0x587a8a7b
         "li x2, 0xf0f0\n"
         "mova.m.x x5\n"
         "maskxor m0, m7, m6\n"                                // opcode=0x6663c07b
         "maskor m6, m6, m3\n"                                 // opcode=0x6633637b
         "fclass.ps f20, f26\n"                                // opcode=0xe00d1a7b
         "fbci.pi f28, 827420\n"                               // opcode=0xca01ce5f
         "fmv.s.x f27, x24\n"                                  // opcode=0xf00c0dd3
         "feq.pi f25, f24, f26\n"                              // opcode=0xa7ac2cfb
         "fxor.pi f25, f20, f30\n"                             // opcode=0x07ea4cfb
         "fadd.ps f31, f23, f25, rdn\n"                        // opcode=0x019baffb
         "masknot m3, m3\n"                                    // opcode=0x6601a1fb
         "maskand m1, m0, m7\n"                                // opcode=0x667070fb
         "fltm.ps m1, f25, f20\n"                              // opcode=0xa14cd0fb
         "fbci.pi f29, 414833\n"                               // opcode=0x65471edf
         "fnmadd.ps f25, f20, f27, f29, dyn\n"                 // opcode=0xefba7cdb
         "fltu.pi f29, f27, f24\n"                             // opcode=0xa78dbefb
         "flt.ps f27, f29, f31\n"                              // opcode=0xa1fe9dfb
         "fbc.ps f22, 58(x24)\n"                               // opcode=0x63ac0b0b
         "fnmadd.ps f21, f31, f30, f20, dyn\n"                 // opcode=0xa7effadb
         "fsub.pi f30, f27, f27\n"                             // opcode=0x0fbd8f7b
         "maskpopcz x27, m0\n"                                 // opcode=0x54000dfb
         //RTLMIN-5479"frsq.ps f30, f22\n"                                  // opcode=0x588b0f7b
         "flt.ps f22, f24, f31\n"                              // opcode=0xa1fc1b7b
         "fadd.pi f27, f26, f26\n"                             // opcode=0x07ad0dfb
         "fmax.s f23, f26, f22\n"                              // opcode=0x296d1bd3
         "maskpopcz x24, m7\n"                                 // opcode=0x54038c7b
         "maskpopc x27, m4\n"                                  // opcode=0x52020dfb
         "fmulhu.pi f29, f26, f31\n"                           // opcode=0x17fd1efb
         "fbci.pi f28, 16775\n"                                // opcode=0x04187e5f
         "fnmsub.s f24, f30, f26, f31, dyn\n"                  // opcode=0xf9af7c4b
         "fsub.s f28, f24, f24, dyn\n"                         // opcode=0x098c7e53
         "mova.m.x x22\n"                                      // opcode=0xd60b107b
         //RTLMIN-5479"frsq.ps f25, f21\n"                                  // opcode=0x588a8cfb
         "fsgnjn.s f26, f26, f28\n"                            // opcode=0x21cd1d53
         "fmax.pi f24, f22, f31\n"                             // opcode=0x2ffb1c7b
         "fmul.ps f20, f21, f20, dyn\n"                        // opcode=0x114afa7b
         "fltm.ps m1, f31, f25\n"                              // opcode=0xa19fd0fb
         "cubefaceidx.ps f27, f22, f30\n"                      // opcode=0x89eb1dfb
         "fsrl.pi f31, f28, f25\n"                             // opcode=0x079e5ffb
         "fmv.x.s x20, f26\n"                                  // opcode=0xe00d0a53
         "fsub.ps f20, f21, f25, dyn\n"                        // opcode=0x099afa7b
         "fxor.pi f31, f25, f24\n"                             // opcode=0x078ccffb
         "mova.x.m x25\n"                                      // opcode=0xd6000cfb
         "fbci.ps f27, 949778\n"                               // opcode=0xe7e12d9f
         "fbcx.ps f24, x31\n"                                  // opcode=0x000fbc0b
         "for.pi f26, f26, f20\n"                              // opcode=0x074d6d7b
         "for.pi f25, f25, f28\n"                              // opcode=0x07ccecfb
         "maskxor m0, m1, m4\n"                                // opcode=0x6640c07b
         "mova.x.m x22\n"                                      // opcode=0xd6000b7b
         "fmax.s f26, f31, f29\n"                              // opcode=0x29df9d53
         "flt.ps f28, f30, f23\n"                              // opcode=0xa17f1e7b
         "fmv.x.s x23, f29\n"                                  // opcode=0xe00e8bd3
         "li x2, 4027\n"
         "mova.m.x x5\n"
         "cubesgnsc.ps f20, f30, f21\n"                        // opcode=0x895f2a7b
         "fsat8.pi f26, f23\n"                                 // opcode=0x060bbd7b
         "fclass.s x24, f28\n"                                 // opcode=0xe00e1c53
         "fmulh.pi f25, f23, f30\n"                            // opcode=0x17ebacfb
         "fslli.pi f30, f30, 13\n"                             // opcode=0x4edf1f7b
         "fsgnjn.ps f30, f28, f29\n"                           // opcode=0x21de1f7b
         "feqm.ps m0, f24, f24\n"                              // opcode=0xa18c607b
         "fadd.ps f26, f21, f22, dyn\n"                        // opcode=0x016afd7b
         "fsub.s f29, f27, f25, dyn\n"                         // opcode=0x099dfed3
         "feqm.ps m6, f23, f20\n"                              // opcode=0xa14be37b
         "maskxor m5, m5, m5\n"                                // opcode=0x6652c2fb
         "fsetm.pi m7, f22\n"                                  // opcode=0xe00b53fb
         "fsat8.pi f21, f20\n"                                 // opcode=0x060a3afb
         "fnmsub.ps f21, f28, f23, f29, dyn\n"                 // opcode=0xed7e7adb
         "fnmadd.ps f31, f28, f21, f24, dyn\n"                 // opcode=0xc75e7fdb
         "fmax.ps f31, f20, f30\n"                             // opcode=0x29ea1ffb
         "fminu.pi f30, f31, f20\n"                            // opcode=0x2f4faf7b
         "for.pi f23, f28, f29\n"                              // opcode=0x07de6bfb
         "masknot m1, m2\n"                                    // opcode=0x660120fb
         "mova.m.x x29\n"                                      // opcode=0xd60e907b
         "fsrai.pi f24, f28, 18\n"                             // opcode=0x4f2e7c7b
         "ffrc.ps f26, f22\n"                                  // opcode=0x582b0d7b
         "maskand m3, m5, m6\n"                                // opcode=0x6662f1fb
         "fsrl.pi f28, f31, f20\n"                             // opcode=0x074fde7b
         "masknot m0, m1\n"                                    // opcode=0x6600a07b
         "masknot m3, m2\n"                                    // opcode=0x660121fb
         "fmaxu.pi f20, f26, f20\n"                            // opcode=0x2f4d3a7b
         "fclass.s x26, f27\n"                                 // opcode=0xe00d9d53
         "cubefaceidx.ps f23, f22, f25\n"                      // opcode=0x899b1bfb
         "fmsub.ps f22, f27, f20, f25, dyn\n"                  // opcode=0xcb4dfb5b
         "fsetm.pi m0, f22\n"                                  // opcode=0xe00b507b
         "fnmsub.ps f21, f26, f26, f20, dyn\n"                 // opcode=0xa5ad7adb
         "fsat8.pi f20, f31\n"                                 // opcode=0x060fba7b
         //RTLMIN-5479"frsq.ps f27, f27\n"                                  // opcode=0x588d8dfb
         "fclass.ps f31, f27\n"                                // opcode=0xe00d9ffb
         "flt.pi f20, f26, f23\n"                              // opcode=0xa77d1a7b
         "fmulh.pi f29, f30, f23\n"                            // opcode=0x177f2efb
         "for.pi f29, f21, f22\n"                              // opcode=0x076aeefb
         "for.pi f28, f20, f28\n"                              // opcode=0x07ca6e7b
         "fmax.pi f30, f22, f27\n"                             // opcode=0x2fbb1f7b
         "maskxor m5, m0, m7\n"                                // opcode=0x667042fb
         "maskor m3, m1, m5\n"                                 // opcode=0x6650e1fb
         "maskpopc x31, m2\n"                                  // opcode=0x52010ffb
         "feqm.ps m6, f30, f29\n"                              // opcode=0xa1df637b
         "fmadd.s f25, f21, f21, f27, dyn\n"                   // opcode=0xd95afcc3
         "fsrli.pi f25, f29, 14\n"                             // opcode=0x4eeedcfb
         "fnot.pi f20, f29\n"                                  // opcode=0x060eaa7b
         "fxor.pi f26, f20, f31\n"                             // opcode=0x07fa4d7b
         "fmadd.s f25, f21, f26, f27, dyn\n"                   // opcode=0xd9aafcc3
         "fsq2 f30, 0 (x20)\n"                                 // opcode=0xb3ea5ca7
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "cubesgntc.ps f30, f31, f30\n"                        // opcode=0x89efbf7b
         "fsll.pi f26, f28, f30\n"                             // opcode=0x07ee1d7b
         "fltu.pi f29, f26, f25\n"                             // opcode=0xa79d3efb
         "fsatu8.pi f27, f22\n"                                // opcode=0x061b3dfb
         "maskand m7, m1, m0\n"                                // opcode=0x6600f3fb
         "fbcx.ps f24, x27\n"                                  // opcode=0x000dbc0b
         "fmvs.x.ps x20, f27, 2\n"                             // opcode=0xe02daa7b
         "fsra.pi f23, f23, f28\n"                             // opcode=0x0fcbdbfb
         "fbci.pi f27, 673377\n"                               // opcode=0xa4661ddf
         "flem.ps m0, f30, f29\n"                              // opcode=0xa1df407b
         "feqm.ps m7, f24, f25\n"                              // opcode=0xa19c63fb
         "fmin.ps f24, f28, f30\n"                             // opcode=0x29ee0c7b
         "fexp.ps f29, f21\n"                                  // opcode=0x584a8efb
         "fmul.s f30, f22, f25, dyn\n"                         // opcode=0x119b7f53
         "fclass.s x20, f25\n"                                 // opcode=0xe00c9a53
         "fmul.pi f31, f30, f22\n"                             // opcode=0x176f0ffb
         "fbcx.ps f26, x27\n"                                  // opcode=0x000dbd0b
         "fmv.s.x f22, x30\n"                                  // opcode=0xf00f0b53
         "fmin.s f26, f29, f25\n"                              // opcode=0x299e8d53
         "fnmsub.s f30, f22, f20, f31, rne\n"                  // opcode=0xf94b0f4b
         "fcmovm.ps f31, f28, f28\n"                           // opcode=0x01ce0ff7
         "fsq2 f28, 0 (x30)\n"                                 // opcode=0xbfcf5827
         "fsra.pi f24, f24, f28\n"                             // opcode=0x0fcc5c7b
         "fsgnj.s f30, f23, f28\n"                             // opcode=0x21cb8f53
         "flt.pi f22, f20, f27\n"                              // opcode=0xa7ba1b7b
         */
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
