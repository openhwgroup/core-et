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
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f1
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           

      // vpu register f2
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             
      0xbf800000,    // -1.0                  

      // vpu register f3
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f4
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f5
      0x3f028f5c,    // 0.51                  
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   

      // vpu register f6
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           

      // vpu register f7
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            

      // vpu register f8
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             

      // vpu register f9
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         

      // vpu register f10
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  

      // vpu register f11
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 

      // vpu register f12
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            

      // vpu register f13
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            

      // vpu register f14
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 

      // vpu register f15
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            

      // vpu register f16
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 

      // vpu register f17
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             

      // vpu register f18
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 

      // vpu register f19
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f20
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             

      // vpu register f21
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   

      // vpu register f22
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 

      // vpu register f23
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            

      // vpu register f24
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           

      // vpu register f25
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 

      // vpu register f26
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  

      // vpu register f27
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0x4b000000,    // 8388608.0             
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           

      // vpu register f28
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f29
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            

      // vpu register f30
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           

      // vpu register f31
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0xbf800000,    // -1.0                  
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
         "li x0,  0x80110560\n"
         "li x1,  0x8099f3c0\n"
         "li x2,  0x80d7d120\n"
         "li x3,  0x8023c9c0\n"
         "li x4,  0x80b77fa0\n"
         "li x5,  0x80d5ef20\n"
         "li x6,  0x80626280\n"
         "li x7,  0x80c9c760\n"
         "li x8,  0x80584a60\n"
         "li x9,  0x800bd520\n"
         "li x10,  0x80d4bc20\n"
         "li x11,  0x802536c0\n"
         "li x12,  0x80dc88e0\n"
         "li x13,  0x80ea76a0\n"
         "li x14,  0x808386c0\n"
         "li x15,  0x8052fba0\n"
         "li x16,  0x8094e1c0\n"
         "li x17,  0x8044fc00\n"
         "li x18,  0x809c2c80\n"
         "li x19,  0x8065aaa0\n"
         "li x20,  0x80ba8400\n"
         "li x21,  0x80a23aa0\n"
         "li x22,  0x80993980\n"
         "li x23,  0x80b6c060\n"
         "li x24,  0x8000e040\n"
         "li x25,  0x801cf320\n"
         "li x26,  0x80043ce0\n"
         "li x27,  0x80b2f7a0\n"
         "li x28,  0x8069af60\n"
         "li x29,  0x8085b5a0\n"
         "li x30,  0x8000b560\n"
         "li x31,  0x805b5260\n"
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
         "fpackrepb.pi f24, f20\n"                             // opcode=0x260a0c7b
         "fbci.ps f31, 913528\n"                               // opcode=0xdf078f9f
         "fnot.pi f22, f29\n"                                  // opcode=0x060eab7b
         "fmax.pi f22, f20, f24\n"                             // opcode=0x2f8a1b7b
         "flt.ps f25, f28, f20\n"                              // opcode=0xa14e1cfb
         "fclass.s x22, f21\n"                                 // opcode=0xe00a9b53
         //RTLMIN-5479"frsq.ps f20, f25\n"                                  // opcode=0x588c8a7b
         "fmvz.x.ps x31, f31, 2\n"                             // opcode=0xe02f8ffb
         "maskxor m3, m2, m5\n"                                // opcode=0x665141fb
         "fclass.s x21, f22\n"                                 // opcode=0xe00b1ad3
         "feq.ps f24, f31, f29\n"                              // opcode=0xa1dfac7b
         "feq.s x21, f22, f30\n"                               // opcode=0xa1eb2ad3
         "fmv.x.s x22, f23\n"                                  // opcode=0xe00b8b53
         "maskpopc x29, m1\n"                                  // opcode=0x52008efb
         "fmvs.x.ps x22, f22, 7\n"                             // opcode=0xe07b2b7b
         /*
            "fsgnj.s f26, f21, f27\n"                             // opcode=0x21ba8d53
            "fround.ps f22, f31, dyn\n"                           // opcode=0x581ffb7b
            "fbcx.ps f29, x22\n"                                  // opcode=0x000b3e8b
            "feq.ps f31, f20, f31\n"                              // opcode=0xa1fa2ffb
            "fmvz.x.ps x26, f26, 1\n"                             // opcode=0xe01d0d7b
            "fmax.s f27, f28, f27\n"                              // opcode=0x29be1dd3
            "fmul.pi f25, f29, f22\n"                             // opcode=0x176e8cfb
            "fpackrepb.pi f27, f23\n"                             // opcode=0x260b8dfb
            "fmulhu.pi f29, f30, f27\n"                           // opcode=0x17bf1efb
            "frcp.ps f20, f23\n"                                  // opcode=0x587b8a7b
            "maskpopc x26, m6\n"                                  // opcode=0x52030d7b
            "fsq2 f30, 0 (x27)\n"                                 // opcode=0xbdedd6a7
            "fsgnjx.s f25, f26, f26\n"                            // opcode=0x21ad2cd3
            "flq2 f24, 0 (x30)\n"                                 // opcode=0x56ff5c07
            "fmaxu.pi f26, f24, f24\n"                            // opcode=0x2f8c3d7b
            "fsetm.pi m7, f24\n"                                  // opcode=0xe00c53fb
            "for.pi f28, f22, f28\n"                              // opcode=0x07cb6e7b
            "fmvz.x.ps x30, f23, 3\n"                             // opcode=0xe03b8f7b
            "fmv.x.s x24, f29\n"                                  // opcode=0xe00e8c53
            "fsgnjn.s f26, f22, f30\n"                            // opcode=0x21eb1d53
            "fsll.pi f28, f21, f23\n"                             // opcode=0x077a9e7b
            "fadd.ps f24, f23, f21, dyn\n"                        // opcode=0x015bfc7b
            "flt.ps f26, f28, f27\n"                              // opcode=0xa1be1d7b
            "fmadd.s f22, f23, f27, f23, dyn\n"                   // opcode=0xb9bbfb43
            "for.pi f30, f26, f27\n"                              // opcode=0x07bd6f7b
            "fxor.pi f22, f20, f24\n"                             // opcode=0x078a4b7b
            "fmv.s.x f28, x30\n"                                  // opcode=0xf00f0e53
            "fxor.pi f26, f23, f23\n"                             // opcode=0x077bcd7b
            "flt.pi f27, f25, f28\n"                              // opcode=0xa7cc9dfb
            "flog.ps f24, f25\n"                                  // opcode=0x583c8c7b
            "fminu.pi f23, f23, f30\n"                            // opcode=0x2febabfb
            "fsgnj.ps f27, f21, f24\n"                            // opcode=0x218a8dfb
            "fmv.s.x f23, x20\n"                                  // opcode=0xf00a0bd3
            "fsq2 f27, 0 (x27)\n"                                 // opcode=0xf7bdd127
            "fand.pi f26, f22, f20\n"                             // opcode=0x074b7d7b
            "fmsub.ps f27, f24, f30, f31, dyn\n"                  // opcode=0xfbec7ddb
            "fmadd.ps f31, f24, f25, f28, dyn\n"                  // opcode=0xe19c7fdb
            "fsub.pi f29, f28, f29\n"                             // opcode=0x0fde0efb
            "cubeface.ps f30, f28, f30\n"                         // opcode=0x89ee0f7b
            "fsrai.pi f25, f28, 13\n"                             // opcode=0x4ede7cfb
            "fexp.ps f24, f28\n"                                  // opcode=0x584e0c7b
            "fadd.pi f28, f25, f28\n"                             // opcode=0x07cc8e7b
            "frcp.ps f28, f24\n"                                  // opcode=0x587c0e7b
            "fsgnj.ps f21, f28, f23\n"                            // opcode=0x217e0afb
            "fcmov.ps f23, f29, f24, f28\n"                       // opcode=0xe58eabbf
            "fcmovm.ps f27, f26, f20\n"                           // opcode=0x014d0df7
            "cubesgnsc.ps f26, f27, f25\n"                        // opcode=0x899dad7b
            "fxor.pi f30, f25, f26\n"                             // opcode=0x07accf7b
            "fsgnj.ps f21, f24, f28\n"                            // opcode=0x21cc0afb
            "fsgnj.s f31, f22, f28\n"                             // opcode=0x21cb0fd3
            "flq2 f27, 0 (x25)\n"                                 // opcode=0x0bdcdd87
            "fle.ps f23, f26, f24\n"                              // opcode=0xa18d0bfb
            "fcmov.ps f25, f30, f24, f25\n"                       // opcode=0xcd8f2cbf
            "fbci.pi f25, 152985\n"                               // opcode=0x25599cdf
            "fclass.s x29, f26\n"                                 // opcode=0xe00d1ed3
            "fmulh.pi f22, f23, f29\n"                            // opcode=0x17dbab7b
            "fsub.pi f28, f24, f23\n"                             // opcode=0x0f7c0e7b
            "fminu.pi f20, f20, f27\n"                            // opcode=0x2fba2a7b
            "fadd.s f22, f29, f27, rne\n"                         // opcode=0x01be8b53
            "fcmovm.ps f27, f20, f25\n"                           // opcode=0x019a0df7
            "fxor.pi f30, f24, f20\n"                             // opcode=0x074c4f7b
            "fsrli.pi f25, f28, 19\n"                             // opcode=0x4f3e5cfb
            "fltm.ps m0, f24, f27\n"                              // opcode=0xa1bc507b
            "fsatu8.pi f29, f28\n"                                // opcode=0x061e3efb
            "fmul.s f25, f31, f24, dyn\n"                         // opcode=0x118ffcd3
            "cubesgntc.ps f29, f21, f27\n"                        // opcode=0x89babefb
            "flem.ps m4, f23, f23\n"                              // opcode=0xa17bc27b
            "fxor.pi f21, f23, f31\n"                             // opcode=0x07fbcafb
            //RTLMIN-5479"frsq.ps f23, f29\n"                                  // opcode=0x588e8bfb
            "fsatu8.pi f26, f20\n"                                // opcode=0x061a3d7b
         "masknot m0, m4\n"                                    // opcode=0x6602207b
         //RTLMIN-5479"frsq.ps f31, f21\n"                                  // opcode=0x588a8ffb
         "flt.s x30, f31, f31\n"                               // opcode=0xa1ff9f53
         "fsll.pi f23, f29, f30\n"                             // opcode=0x07ee9bfb
         "flt.pi f21, f22, f29\n"                              // opcode=0xa7db1afb
         "fand.pi f22, f31, f22\n"                             // opcode=0x076ffb7b
         "fslli.pi f26, f27, 28\n"                             // opcode=0x4fcd9d7b
         "fmax.pi f30, f24, f29\n"                             // opcode=0x2fdc1f7b
         "masknot m0, m3\n"                                    // opcode=0x6601a07b
         "fpackrepb.pi f27, f22\n"                             // opcode=0x260b0dfb
         "fbci.ps f28, 490975\n"                               // opcode=0x77ddfe1f
         "fltu.pi f22, f22, f26\n"                             // opcode=0xa7ab3b7b
         "masknot m5, m1\n"                                    // opcode=0x6600a2fb
         "cubefaceidx.ps f21, f28, f25\n"                      // opcode=0x899e1afb
         "fsq2 f29, 0 (x27)\n"                                 // opcode=0x61ddd3a7
         "maskpopc x20, m0\n"                                  // opcode=0x52000a7b
         "fsatu8.pi f30, f23\n"                                // opcode=0x061bbf7b
         "cubefaceidx.ps f28, f30, f20\n"                      // opcode=0x894f1e7b
         "fmv.x.s x23, f27\n"                                  // opcode=0xe00d8bd3
         "fsgnj.ps f27, f31, f24\n"                            // opcode=0x218f8dfb
         "fexp.ps f22, f27\n"                                  // opcode=0x584d8b7b
         "fxor.pi f25, f28, f26\n"                             // opcode=0x07ae4cfb
         "fnmadd.s f28, f24, f21, f22, dyn\n"                  // opcode=0xb15c7e4f
         "fmin.s f23, f25, f20\n"                              // opcode=0x294c8bd3
         "fsrli.pi f29, f29, 31\n"                             // opcode=0x4ffedefb
         "maskpopcz x31, m5\n"                                 // opcode=0x54028ffb
         "fsub.pi f25, f25, f23\n"                             // opcode=0x0f7c8cfb
         "fmul.pi f22, f22, f21\n"                             // opcode=0x175b0b7b
         "fle.pi f26, f31, f30\n"                              // opcode=0xa7ef8d7b
         "fle.pi f22, f27, f21\n"                              // opcode=0xa75d8b7b
         "fmvs.x.ps x31, f21, 6\n"                             // opcode=0xe06aaffb
         "fnmadd.ps f21, f30, f22, f24, dyn\n"                 // opcode=0xc76f7adb
         "maskor m1, m7, m1\n"                                 // opcode=0x6613e0fb
         "fsub.s f21, f25, f30, dyn\n"                         // opcode=0x09ecfad3
         "fmin.pi f27, f30, f30\n"                             // opcode=0x2fef0dfb
         "fle.s x25, f30, f27\n"                               // opcode=0xa1bf0cd3
         //RTLMIN-5479"frsq.ps f31, f21\n"                                  // opcode=0x588a8ffb
         "fxor.pi f21, f21, f26\n"                             // opcode=0x07aacafb
         "fsrl.pi f26, f23, f30\n"                             // opcode=0x07ebdd7b
         "fpackreph.pi f22, f25\n"                             // opcode=0x260c9b7b
         "fbcx.ps f23, x20\n"                                  // opcode=0x000a3b8b
         "fmadd.ps f23, f26, f29, f20, dyn\n"                  // opcode=0xa1dd7bdb
         "fcmovm.ps f27, f22, f23\n"                           // opcode=0x017b0df7
         "fandi.pi f30, f20, 125\n"                            // opcode=0x1dda1f3f
         "mova.x.m x23\n"                                      // opcode=0xd6000bfb
         "fmvs.x.ps x26, f30, 5\n"                             // opcode=0xe05f2d7b
         "fmax.s f29, f25, f20\n"                              // opcode=0x294c9ed3
         "fsat8.pi f26, f31\n"                                 // opcode=0x060fbd7b
         "fsq2 f22, 0 (x25)\n"                                 // opcode=0xab6cd6a7
         "fmvs.x.ps x28, f20, 7\n"                             // opcode=0xe07a2e7b
         "fsrl.pi f28, f28, f27\n"                             // opcode=0x07be5e7b
         "flq2 f21, 0 (x29)\n"                                 // opcode=0xe32eda87
         "fltm.ps m3, f26, f24\n"                              // opcode=0xa18d51fb
         "fsub.s f26, f30, f27, dyn\n"                         // opcode=0x09bf7d53
         "fnmadd.ps f21, f25, f22, f30, dyn\n"                 // opcode=0xf76cfadb
         "fmulh.pi f31, f22, f22\n"                            // opcode=0x176b2ffb
         "mova.m.x x20\n"                                      // opcode=0xd60a107b
         "feqm.ps m4, f25, f30\n"                              // opcode=0xa1ece27b
         "fsatu8.pi f28, f31\n"                                // opcode=0x061fbe7b
         "fminu.pi f23, f29, f22\n"                            // opcode=0x2f6eabfb
         "fmin.s f21, f28, f23\n"                              // opcode=0x297e0ad3
         "fmulhu.pi f31, f27, f22\n"                           // opcode=0x176d9ffb
         "fmax.s f20, f29, f29\n"                              // opcode=0x29de9a53
         "fminu.pi f29, f24, f21\n"                            // opcode=0x2f5c2efb
         "feq.pi f28, f31, f30\n"                              // opcode=0xa7efae7b
         "masknot m2, m5\n"                                    // opcode=0x6602a17b
         "fsgnjn.ps f20, f27, f25\n"                           // opcode=0x219d9a7b
         "fbci.pi f26, 709290\n"                               // opcode=0xad2aad5f
         "fsat8.pi f25, f20\n"                                 // opcode=0x060a3cfb
         "faddi.pi f23, f28, 638\n"                            // opcode=0x9dee0bbf
         "fsll.pi f25, f27, f30\n"                             // opcode=0x07ed9cfb
         "fmvz.x.ps x25, f23, 5\n"                             // opcode=0xe05b8cfb
         "fsatu8.pi f21, f31\n"                                // opcode=0x061fbafb
         "fsat8.pi f25, f29\n"                                 // opcode=0x060ebcfb
         "fsll.pi f26, f29, f29\n"                             // opcode=0x07de9d7b
         "fsll.pi f28, f29, f23\n"                             // opcode=0x077e9e7b
         "for.pi f25, f31, f21\n"                              // opcode=0x075fecfb
         "fmvs.x.ps x21, f20, 6\n"                             // opcode=0xe06a2afb
         "fmadd.s f27, f26, f20, f29, dyn\n"                   // opcode=0xe94d7dc3
         "fcmov.ps f21, f23, f23, f29\n"                       // opcode=0xed7baabf
         //RTLMIN-5479"frsq.ps f21, f22\n"                                  // opcode=0x588b0afb
         "feq.s x30, f29, f25\n"                               // opcode=0xa19eaf53
         "feq.pi f24, f21, f26\n"                              // opcode=0xa7aaac7b
         "cubefaceidx.ps f24, f30, f21\n"                      // opcode=0x895f1c7b
         "fsgnj.ps f23, f25, f20\n"                            // opcode=0x214c8bfb
         "mova.x.m x27\n"                                      // opcode=0xd6000dfb
         "fmul.ps f30, f25, f22, dyn\n"                        // opcode=0x116cff7b
         "frcp.ps f23, f28\n"                                  // opcode=0x587e0bfb
         "fsgnjn.ps f23, f31, f25\n"                           // opcode=0x219f9bfb
         "fsetm.pi m1, f26\n"                                  // opcode=0xe00d50fb
         "fmin.pi f31, f27, f29\n"                             // opcode=0x2fdd8ffb
         "fmulh.pi f22, f21, f29\n"                            // opcode=0x17daab7b
         "fadd.ps f27, f30, f22, dyn\n"                        // opcode=0x016f7dfb
         "maskor m7, m1, m3\n"                                 // opcode=0x6630e3fb
         "fle.s x29, f26, f21\n"                               // opcode=0xa15d0ed3
         "fle.ps f22, f31, f23\n"                              // opcode=0xa17f8b7b
         "fadd.ps f24, f23, f20, dyn\n"                        // opcode=0x014bfc7b
         "fsat8.pi f25, f28\n"                                 // opcode=0x060e3cfb
         "fsub.pi f21, f20, f22\n"                             // opcode=0x0f6a0afb
         "fclass.ps f29, f21\n"                                // opcode=0xe00a9efb
         "fmul.ps f24, f20, f29, dyn\n"                        // opcode=0x11da7c7b
         "fsgnjx.s f29, f21, f30\n"                            // opcode=0x21eaaed3
         "fle.pi f22, f23, f31\n"                              // opcode=0xa7fb8b7b
         "fmin.s f26, f28, f26\n"                              // opcode=0x29ae0d53
         "fminu.pi f28, f25, f30\n"                            // opcode=0x2fecae7b
         "faddi.pi f27, f20, 175\n"                            // opcode=0x2cfa0dbf
         "fadd.ps f26, f20, f29, dyn\n"                        // opcode=0x01da7d7b
         "fltm.ps m1, f25, f20\n"                              // opcode=0xa14cd0fb
         "fpackreph.pi f23, f22\n"                             // opcode=0x260b1bfb
         //RTLMIN-5479"fsin.ps f23, f25\n"                                  // opcode=0x586c8bfb
         "maskor m0, m4, m2\n"                                 // opcode=0x6622607b
         "fmsub.s f31, f30, f27, f25, dyn\n"                   // opcode=0xc9bf7fc7
         //RTLMIN-5479"fsin.ps f31, f29\n"                                  // opcode=0x586e8ffb
         "fbci.pi f24, 348325\n"                               // opcode=0x550a5c5f
         //RTLMIN-5479"fsin.ps f28, f29\n"                                  // opcode=0x586e8e7b
         "fcmovm.ps f27, f24, f30\n"                           // opcode=0x01ec0df7
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fcmovm.ps f30, f20, f28\n"                           // opcode=0x01ca0f77
         "fsrl.pi f25, f26, f28\n"                             // opcode=0x07cd5cfb
         "fmul.s f23, f20, f25, dyn\n"                         // opcode=0x119a7bd3
         //RTLMIN-5479"frsq.ps f21, f28\n"                                  // opcode=0x588e0afb
         "maskand m6, m4, m4\n"                                // opcode=0x6642737b
         "fbci.ps f20, 109622\n"                               // opcode=0x1ac36a1f
         "fnot.pi f28, f27\n"                                  // opcode=0x060dae7b
         "cubesgntc.ps f23, f21, f22\n"                        // opcode=0x896abbfb
         "fsub.pi f24, f21, f20\n"                             // opcode=0x0f4a8c7b
         "fsat8.pi f28, f23\n"                                 // opcode=0x060bbe7b
         "flq2 f31, 0 (x26)\n"                                 // opcode=0x802d5f87
         "fand.pi f25, f28, f23\n"                             // opcode=0x077e7cfb
         "fmulh.pi f31, f23, f27\n"                            // opcode=0x17bbaffb
         "ffrc.ps f29, f26\n"                                  // opcode=0x582d0efb
         "cubesgnsc.ps f24, f31, f22\n"                        // opcode=0x896fac7b
         "fmv.x.s x22, f30\n"                                  // opcode=0xe00f0b53
         "for.pi f29, f29, f20\n"                              // opcode=0x074eeefb
         "fltm.ps m1, f24, f27\n"                              // opcode=0xa1bc50fb
         "mova.x.m x25\n"                                      // opcode=0xd6000cfb
         "fandi.pi f30, f28, 243\n"                            // opcode=0x3d3e1f3f
         "maskand m0, m4, m2\n"                                // opcode=0x6622707b
         "flt.pi f28, f24, f27\n"                              // opcode=0xa7bc1e7b
         "fsub.ps f26, f28, f24, dyn\n"                        // opcode=0x098e7d7b
         "fbcx.ps f26, x29\n"                                  // opcode=0x000ebd0b
         "feqm.ps m5, f25, f20\n"                              // opcode=0xa14ce2fb
         "fexp.ps f25, f31\n"                                  // opcode=0x584f8cfb
         "fle.ps f30, f20, f21\n"                              // opcode=0xa15a0f7b
         "flem.ps m1, f20, f25\n"                              // opcode=0xa19a40fb
         "fadd.s f22, f30, f29, dyn\n"                         // opcode=0x01df7b53
         "maskand m5, m4, m3\n"                                // opcode=0x663272fb
         "fcmovm.ps f23, f27, f28\n"                           // opcode=0x01cd8bf7
         "fnmadd.s f31, f29, f29, f20, dyn\n"                  // opcode=0xa1deffcf
         "fand.pi f23, f23, f28\n"                             // opcode=0x07cbfbfb
         "fmul.ps f28, f29, f31, dyn\n"                        // opcode=0x11fefe7b
         "fsgnjx.s f31, f25, f28\n"                            // opcode=0x21ccafd3
         //RTLMIN-5479"frsq.ps f28, f23\n"                                  // opcode=0x588b8e7b
         //RTLMIN-5479"fsin.ps f27, f26\n"                                  // opcode=0x586d0dfb
         "ffrc.ps f30, f20\n"                                  // opcode=0x582a0f7b
         "fpackreph.pi f31, f25\n"                             // opcode=0x260c9ffb
         "fmin.s f26, f23, f28\n"                              // opcode=0x29cb8d53
         "fltm.ps m2, f29, f20\n"                              // opcode=0xa14ed17b
         //RTLMIN-5479"fsin.ps f31, f20\n"                                  // opcode=0x586a0ffb
         "fsgnjn.s f31, f25, f26\n"                            // opcode=0x21ac9fd3
         "fmul.s f25, f23, f23, dyn\n"                         // opcode=0x117bfcd3
         "fltm.pi m7, f29, f27\n"                              // opcode=0x3fbe83fb
         "fbcx.ps f23, x26\n"                                  // opcode=0x000d3b8b
         "fsgnjx.s f28, f31, f20\n"                            // opcode=0x214fae53
         "fminu.pi f29, f31, f26\n"                            // opcode=0x2fafaefb
         "fpackrepb.pi f25, f28\n"                             // opcode=0x260e0cfb
         "fpackrepb.pi f27, f23\n"                             // opcode=0x260b8dfb
         "li x2, 3724\n"
         "mova.m.x x5\n"
         "fadd.pi f22, f30, f29\n"                             // opcode=0x07df0b7b
         "fexp.ps f28, f29\n"                                  // opcode=0x584e8e7b
         "fsatu8.pi f31, f26\n"                                // opcode=0x061d3ffb
         "fxor.pi f20, f20, f20\n"                             // opcode=0x074a4a7b
         "fsgnjn.ps f25, f26, f25\n"                           // opcode=0x219d1cfb
         "feqm.ps m3, f27, f24\n"                              // opcode=0xa18de1fb
         "fsub.pi f30, f23, f27\n"                             // opcode=0x0fbb8f7b
         "feqm.ps m0, f20, f28\n"                              // opcode=0xa1ca607b
         "maskand m7, m0, m4\n"                                // opcode=0x664073fb
         "fbci.ps f20, 551927\n"                               // opcode=0x86bf7a1f
         "flt.s x21, f20, f27\n"                               // opcode=0xa1ba1ad3
         "frcp.ps f23, f24\n"                                  // opcode=0x587c0bfb
         "fsub.s f21, f21, f28, dyn\n"                         // opcode=0x09cafad3
         "fltu.pi f25, f30, f22\n"                             // opcode=0xa76f3cfb
         "fnmsub.ps f28, f24, f20, f29, dyn\n"                 // opcode=0xed4c7e5b
         "fmadd.s f28, f21, f22, f21, dyn\n"                   // opcode=0xa96afe43
         "fmsub.s f26, f30, f22, f27, dyn\n"                   // opcode=0xd96f7d47
         "fmvz.x.ps x26, f21, 3\n"                             // opcode=0xe03a8d7b
         "fsra.pi f20, f24, f31\n"                             // opcode=0x0ffc5a7b
         "fsgnj.s f24, f24, f27\n"                             // opcode=0x21bc0c53
         "fadd.ps f28, f31, f28, dyn\n"                        // opcode=0x01cffe7b
         "masknot m5, m1\n"                                    // opcode=0x6600a2fb
         "flt.ps f23, f21, f29\n"                              // opcode=0xa1da9bfb
         "fmsub.ps f28, f21, f21, f29, dyn\n"                  // opcode=0xeb5afe5b
         "fmin.ps f23, f31, f20\n"                             // opcode=0x294f8bfb
         "fmulhu.pi f20, f31, f29\n"                           // opcode=0x17df9a7b
         "maskxor m3, m1, m0\n"                                // opcode=0x6600c1fb
         //RTLMIN-5479"frsq.ps f30, f28\n"                                  // opcode=0x588e0f7b
         "fmvz.x.ps x24, f21, 2\n"                             // opcode=0xe02a8c7b
         "feq.ps f26, f23, f24\n"                              // opcode=0xa18bad7b
         "fslli.pi f28, f21, 27\n"                             // opcode=0x4fba9e7b
         "fcmovm.ps f20, f29, f22\n"                           // opcode=0x016e8a77
         "fsub.s f20, f31, f28, dyn\n"                         // opcode=0x09cffa53
         "mova.x.m x20\n"                                      // opcode=0xd6000a7b
         "mova.m.x x23\n"                                      // opcode=0xd60b907b
         "fsra.pi f31, f26, f24\n"                             // opcode=0x0f8d5ffb
         "fmax.s f25, f29, f24\n"                              // opcode=0x298e9cd3
         "flt.ps f29, f22, f25\n"                              // opcode=0xa19b1efb
         "fnmsub.ps f30, f30, f23, f24, dyn\n"                 // opcode=0xc57f7f5b
         "fmax.ps f21, f20, f27\n"                             // opcode=0x29ba1afb
         "fadd.pi f21, f21, f29\n"                             // opcode=0x07da8afb
         //RTLMIN-5479"fsin.ps f27, f22\n"                                  // opcode=0x586b0dfb
         "fmin.pi f30, f24, f22\n"                             // opcode=0x2f6c0f7b
         "fsub.ps f21, f30, f24, dyn\n"                        // opcode=0x098f7afb
         "fpackrepb.pi f28, f26\n"                             // opcode=0x260d0e7b
         "fnmsub.s f29, f27, f21, f30, dyn\n"                  // opcode=0xf15dfecb
         "fmax.pi f24, f27, f27\n"                             // opcode=0x2fbd9c7b
         "fmul.s f31, f30, f27, dyn\n"                         // opcode=0x11bf7fd3
         "fadd.s f31, f23, f27, dyn\n"                         // opcode=0x01bbffd3
         "fpackreph.pi f25, f27\n"                             // opcode=0x260d9cfb
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fand.pi f22, f23, f28\n"                             // opcode=0x07cbfb7b
         "fmv.x.s x31, f30\n"                                  // opcode=0xe00f0fd3
         "flog.ps f26, f31\n"                                  // opcode=0x583f8d7b
         "flog.ps f25, f23\n"                                  // opcode=0x583b8cfb
         "fbci.pi f24, 187155\n"                               // opcode=0x2db13c5f
         "fsll.pi f26, f24, f29\n"                             // opcode=0x07dc1d7b
         "fmax.ps f22, f24, f26\n"                             // opcode=0x29ac1b7b
         "flt.pi f24, f20, f23\n"                              // opcode=0xa77a1c7b
         "maskpopcz x27, m6\n"                                 // opcode=0x54030dfb
         "fmsub.ps f24, f25, f26, f23, dyn\n"                  // opcode=0xbbacfc5b
         "fmax.ps f24, f21, f26\n"                             // opcode=0x29aa9c7b
         "flem.ps m5, f29, f28\n"                              // opcode=0xa1cec2fb
         "fcmovm.ps f22, f29, f31\n"                           // opcode=0x01fe8b77
         "fmin.ps f21, f30, f28\n"                             // opcode=0x29cf0afb
         "fmul.pi f31, f25, f24\n"                             // opcode=0x178c8ffb
         "fxor.pi f27, f30, f22\n"                             // opcode=0x076f4dfb
         //RTLMIN-5479"frsq.ps f27, f27\n"                                  // opcode=0x588d8dfb
         "frcp.ps f21, f24\n"                                  // opcode=0x587c0afb
         "mova.x.m x27\n"                                      // opcode=0xd6000dfb
         "fround.ps f22, f27, dyn\n"                           // opcode=0x581dfb7b
         "fcmov.ps f29, f20, f28, f28\n"                       // opcode=0xe5ca2ebf
         "fmul.ps f20, f23, f20, dyn\n"                        // opcode=0x114bfa7b
         "flem.ps m3, f27, f23\n"                              // opcode=0xa17dc1fb
         "fmul.s f24, f31, f30, dyn\n"                         // opcode=0x11effc53
         "fsgnjn.s f20, f27, f22\n"                            // opcode=0x216d9a53
         "fsetm.pi m0, f29\n"                                  // opcode=0xe00ed07b
         "cubesgnsc.ps f26, f22, f22\n"                        // opcode=0x896b2d7b
         "fadd.ps f27, f24, f26, dyn\n"                        // opcode=0x01ac7dfb
         "fltm.pi m1, f31, f22\n"                              // opcode=0x3f6f80fb
         "fround.ps f31, f20, dyn\n"                           // opcode=0x581a7ffb
         "fmv.x.s x22, f31\n"                                  // opcode=0xe00f8b53
         "fround.ps f25, f31, dyn\n"                           // opcode=0x581ffcfb
         "fcmov.ps f26, f23, f22, f28\n"                       // opcode=0xe56bad3f
         "fadd.pi f22, f30, f23\n"                             // opcode=0x077f0b7b
         "fmin.s f20, f29, f20\n"                              // opcode=0x294e8a53
         "maskxor m4, m7, m0\n"                                // opcode=0x6603c27b
         "fsgnjx.ps f21, f24, f28\n"                           // opcode=0x21cc2afb
         "fsgnjn.s f30, f25, f24\n"                            // opcode=0x218c9f53
         "fmul.ps f20, f22, f24, rmm\n"                        // opcode=0x118b4a7b
         "for.pi f28, f21, f29\n"                              // opcode=0x07daee7b
         //RTLMIN-5479"fsin.ps f20, f24\n"                                  // opcode=0x586c0a7b
         "fcmov.ps f29, f26, f24, f30\n"                       // opcode=0xf58d2ebf
         "fsub.ps f22, f27, f29, dyn\n"                        // opcode=0x09ddfb7b
         "fltu.pi f30, f31, f29\n"                             // opcode=0xa7dfbf7b
         "fpackrepb.pi f28, f24\n"                             // opcode=0x260c0e7b
         "fclass.ps f26, f26\n"                                // opcode=0xe00d1d7b
         "fmul.ps f29, f30, f21, dyn\n"                        // opcode=0x115f7efb
         "mova.m.x x20\n"                                      // opcode=0xd60a107b
         "fmin.pi f25, f26, f21\n"                             // opcode=0x2f5d0cfb
         "flem.ps m1, f30, f23\n"                              // opcode=0xa17f40fb
         "li x2, 3571\n"
         "mova.m.x x5\n"
         "ffrc.ps f23, f30\n"                                  // opcode=0x582f0bfb
         "fnot.pi f28, f27\n"                                  // opcode=0x060dae7b
         "ffrc.ps f21, f28\n"                                  // opcode=0x582e0afb
         "maskpopc x22, m4\n"                                  // opcode=0x52020b7b
         "fadd.pi f20, f28, f26\n"                             // opcode=0x07ae0a7b
         "fsgnjx.s f20, f21, f24\n"                            // opcode=0x218aaa53
         "fsub.ps f20, f22, f29, dyn\n"                        // opcode=0x09db7a7b
         "fadd.s f31, f23, f27, dyn\n"                         // opcode=0x01bbffd3
         "fmul.s f28, f28, f31, dyn\n"                         // opcode=0x11fe7e53
         "fle.ps f29, f31, f26\n"                              // opcode=0xa1af8efb
         "fcmovm.ps f26, f21, f29\n"                           // opcode=0x01da8d77
         "fmv.x.s x23, f20\n"                                  // opcode=0xe00a0bd3
         "mova.x.m x26\n"                                      // opcode=0xd6000d7b
         "fmax.s f23, f26, f28\n"                              // opcode=0x29cd1bd3
         "maskxor m6, m4, m4\n"                                // opcode=0x6642437b
         "fround.ps f30, f29, dyn\n"                           // opcode=0x581eff7b
         "mova.x.m x28\n"                                      // opcode=0xd6000e7b
         "fmadd.ps f27, f23, f21, f22, dyn\n"                  // opcode=0xb15bfddb
         "fcmov.ps f21, f26, f31, f29\n"                       // opcode=0xedfd2abf
         "fsq2 f29, 0 (x29)\n"                                 // opcode=0xd1ded127
         "fadd.ps f21, f22, f21, dyn\n"                        // opcode=0x015b7afb
         "mova.m.x x29\n"                                      // opcode=0xd60e907b
         "fmaxu.pi f25, f25, f26\n"                            // opcode=0x2facbcfb
         "fnmadd.s f21, f24, f27, f25, dyn\n"                  // opcode=0xc9bc7acf
         "feq.pi f22, f28, f30\n"                              // opcode=0xa7ee2b7b
         "cubefaceidx.ps f20, f26, f28\n"                      // opcode=0x89cd1a7b
         "fsgnj.s f24, f25, f25\n"                             // opcode=0x219c8c53
         "feqm.ps m7, f27, f25\n"                              // opcode=0xa19de3fb
         "fsgnj.s f25, f22, f28\n"                             // opcode=0x21cb0cd3
         "fnot.pi f20, f28\n"                                  // opcode=0x060e2a7b
         "fsgnj.s f21, f27, f20\n"                             // opcode=0x214d8ad3
         "fsetm.pi m7, f25\n"                                  // opcode=0xe00cd3fb
         "fle.s x21, f31, f27\n"                               // opcode=0xa1bf8ad3
         "fclass.ps f24, f24\n"                                // opcode=0xe00c1c7b
         "fsatu8.pi f24, f30\n"                                // opcode=0x061f3c7b
         "cubesgntc.ps f27, f26, f25\n"                        // opcode=0x899d3dfb
         "fand.pi f27, f31, f23\n"                             // opcode=0x077ffdfb
         "fsgnjn.s f28, f29, f31\n"                            // opcode=0x21fe9e53
         "fnmsub.ps f20, f28, f20, f30, dyn\n"                 // opcode=0xf54e7a5b
         "fnmadd.ps f24, f30, f30, f30, dyn\n"                 // opcode=0xf7ef7c5b
         //RTLMIN-5479"fsin.ps f27, f26\n"                                  // opcode=0x586d0dfb
         //RTLMIN-5479"fsin.ps f26, f20\n"                                  // opcode=0x586a0d7b
         "fadd.pi f21, f20, f21\n"                             // opcode=0x075a0afb
         "mova.x.m x31\n"                                      // opcode=0xd6000ffb
         "fexp.ps f31, f24\n"                                  // opcode=0x584c0ffb
         "fsetm.pi m3, f22\n"                                  // opcode=0xe00b51fb
         "fle.ps f29, f23, f28\n"                              // opcode=0xa1cb8efb
         "fle.ps f27, f27, f23\n"                              // opcode=0xa17d8dfb
         "fmin.pi f25, f20, f20\n"                             // opcode=0x2f4a0cfb
         "fmaxu.pi f23, f21, f28\n"                            // opcode=0x2fcabbfb
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fle.ps f30, f29, f23\n"                              // opcode=0xa17e8f7b
         "maskpopcz x21, m3\n"                                 // opcode=0x54018afb
         "flt.ps f25, f21, f28\n"                              // opcode=0xa1ca9cfb
         "fle.pi f29, f21, f27\n"                              // opcode=0xa7ba8efb
         "fpackreph.pi f29, f20\n"                             // opcode=0x260a1efb
         "fsetm.pi m2, f22\n"                                  // opcode=0xe00b517b
         "maskpopc x27, m3\n"                                  // opcode=0x52018dfb
         "fmv.x.s x26, f27\n"                                  // opcode=0xe00d8d53
         "fmsub.s f28, f23, f29, f29, dyn\n"                   // opcode=0xe9dbfe47
         "fltm.ps m1, f30, f29\n"                              // opcode=0xa1df50fb
         "fexp.ps f29, f25\n"                                  // opcode=0x584c8efb
         "feqm.ps m0, f21, f20\n"                              // opcode=0xa14ae07b
         "fpackrepb.pi f20, f28\n"                             // opcode=0x260e0a7b
         "fbci.ps f25, 272683\n"                               // opcode=0x4292bc9f
         "mova.x.m x24\n"                                      // opcode=0xd6000c7b
         "fltm.ps m6, f24, f27\n"                              // opcode=0xa1bc537b
         "fclass.s x31, f21\n"                                 // opcode=0xe00a9fd3
         "fpackreph.pi f22, f23\n"                             // opcode=0x260b9b7b
         "fcmovm.ps f20, f21, f25\n"                           // opcode=0x019a8a77
         "fbcx.ps f31, x30\n"                                  // opcode=0x000f3f8b
         "fclass.ps f23, f25\n"                                // opcode=0xe00c9bfb
         "flq2 f21, 0 (x22)\n"                                 // opcode=0x84ab5a87
         "fclass.ps f24, f22\n"                                // opcode=0xe00b1c7b
         "fnmsub.ps f28, f26, f27, f20, dyn\n"                 // opcode=0xa5bd7e5b
         "fmsub.s f31, f26, f24, f20, dyn\n"                   // opcode=0xa18d7fc7
         "fmaxu.pi f26, f27, f25\n"                            // opcode=0x2f9dbd7b
         "fsgnjx.s f26, f20, f31\n"                            // opcode=0x21fa2d53
         "fpackreph.pi f24, f27\n"                             // opcode=0x260d9c7b
         "fsgnj.ps f22, f21, f26\n"                            // opcode=0x21aa8b7b
         "cubefaceidx.ps f21, f20, f22\n"                      // opcode=0x896a1afb
         "fnmadd.s f25, f29, f28, f29, dyn\n"                  // opcode=0xe9cefccf
         "fcmov.ps f31, f27, f24, f22\n"                       // opcode=0xb58dafbf
         "flem.ps m4, f25, f27\n"                              // opcode=0xa1bcc27b
         "fsgnjx.s f26, f20, f28\n"                            // opcode=0x21ca2d53
         "fpackreph.pi f25, f25\n"                             // opcode=0x260c9cfb
         "fpackrepb.pi f24, f20\n"                             // opcode=0x260a0c7b
         "flog.ps f25, f26\n"                                  // opcode=0x583d0cfb
         "fmax.s f28, f29, f22\n"                              // opcode=0x296e9e53
         "fsll.pi f31, f27, f27\n"                             // opcode=0x07bd9ffb
         "fle.ps f25, f20, f29\n"                              // opcode=0xa1da0cfb
         "mova.x.m x22\n"                                      // opcode=0xd6000b7b
         "feqm.ps m7, f26, f26\n"                              // opcode=0xa1ad63fb
         "fsatu8.pi f24, f27\n"                                // opcode=0x061dbc7b
         "fmadd.ps f28, f23, f28, f20, dyn\n"                  // opcode=0xa1cbfe5b
         "feq.s x20, f26, f25\n"                               // opcode=0xa19d2a53
         "fsub.pi f22, f29, f21\n"                             // opcode=0x0f5e8b7b
         "fle.s x25, f22, f22\n"                               // opcode=0xa16b0cd3
         "fbci.ps f25, 662450\n"                               // opcode=0xa1bb2c9f
         "cubesgntc.ps f28, f24, f20\n"                        // opcode=0x894c3e7b
         "fmadd.s f24, f26, f20, f24, dyn\n"                   // opcode=0xc14d7c43
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fmadd.s f30, f31, f26, f24, dyn\n"                   // opcode=0xc1afff43
         "fmin.ps f25, f31, f22\n"                             // opcode=0x296f8cfb
         "fxor.pi f25, f22, f25\n"                             // opcode=0x079b4cfb
         "fmul.ps f23, f27, f31, dyn\n"                        // opcode=0x11fdfbfb
         "fmvs.x.ps x28, f26, 5\n"                             // opcode=0xe05d2e7b
         "fsgnjn.ps f22, f28, f22\n"                           // opcode=0x216e1b7b
         "fround.ps f23, f28, rmm\n"                           // opcode=0x581e4bfb
         "fnot.pi f25, f20\n"                                  // opcode=0x060a2cfb
         "fsgnjx.ps f31, f21, f20\n"                           // opcode=0x214aaffb
         "fcmovm.ps f29, f29, f23\n"                           // opcode=0x017e8ef7
         "frcp.ps f20, f29\n"                                  // opcode=0x587e8a7b
         "fsgnj.ps f23, f26, f26\n"                            // opcode=0x21ad0bfb
         "fsub.ps f25, f25, f22, rmm\n"                        // opcode=0x096cccfb
         "fadd.pi f25, f20, f23\n"                             // opcode=0x077a0cfb
         "fsgnjx.ps f29, f31, f21\n"                           // opcode=0x215faefb
         "maskpopcz x30, m4\n"                                 // opcode=0x54020f7b
         "maskand m6, m5, m6\n"                                // opcode=0x6662f37b
         "maskpopc x30, m7\n"                                  // opcode=0x52038f7b
         "fsub.s f22, f26, f29, dyn\n"                         // opcode=0x09dd7b53
         "fmin.pi f24, f25, f29\n"                             // opcode=0x2fdc8c7b
         "maskor m2, m3, m1\n"                                 // opcode=0x6611e17b
         "fclass.ps f28, f31\n"                                // opcode=0xe00f9e7b
         "fminu.pi f31, f24, f20\n"                            // opcode=0x2f4c2ffb
         "fmsub.s f31, f30, f26, f23, dyn\n"                   // opcode=0xb9af7fc7
         "fround.ps f30, f31, dyn\n"                           // opcode=0x581fff7b
         "fltu.pi f23, f27, f25\n"                             // opcode=0xa79dbbfb
         "fadd.s f29, f20, f28, dyn\n"                         // opcode=0x01ca7ed3
         "fnmsub.s f22, f30, f26, f29, dyn\n"                  // opcode=0xe9af7b4b
         "fbci.pi f27, 462865\n"                               // opcode=0x71011ddf
         "cubeface.ps f31, f31, f23\n"                         // opcode=0x897f8ffb
         "fsrai.pi f24, f24, 28\n"                             // opcode=0x4fcc7c7b
         "fmv.x.s x25, f23\n"                                  // opcode=0xe00b8cd3
         "flt.pi f21, f26, f28\n"                              // opcode=0xa7cd1afb
         "fminu.pi f27, f31, f22\n"                            // opcode=0x2f6fadfb
         "fmv.x.s x27, f26\n"                                  // opcode=0xe00d0dd3
         "maskxor m1, m1, m4\n"                                // opcode=0x6640c0fb
         "fmaxu.pi f22, f24, f27\n"                            // opcode=0x2fbc3b7b
         "for.pi f21, f27, f23\n"                              // opcode=0x077deafb
         "fsub.ps f23, f24, f26, dyn\n"                        // opcode=0x09ac7bfb
         "fle.ps f24, f31, f31\n"                              // opcode=0xa1ff8c7b
         "fadd.pi f31, f25, f20\n"                             // opcode=0x074c8ffb
         "for.pi f30, f31, f22\n"                              // opcode=0x076fef7b
         "fbci.ps f31, 210588\n"                               // opcode=0x3369cf9f
         "fmin.ps f22, f23, f22\n"                             // opcode=0x296b8b7b
         "fle.pi f26, f24, f26\n"                              // opcode=0xa7ac0d7b
         "fmin.s f21, f21, f28\n"                              // opcode=0x29ca8ad3
         "fbc.ps f28, 97(x26)\n"                               // opcode=0x9e1d0e0b
         "fmax.ps f31, f20, f27\n"                             // opcode=0x29ba1ffb
         "feq.s x24, f26, f29\n"                               // opcode=0xa1dd2c53
         "fsgnj.ps f25, f25, f22\n"                            // opcode=0x216c8cfb
         "li x2, 0xf0f0\n"
         "mova.m.x x5\n"
         "fnmsub.ps f22, f26, f29, f25, dyn\n"                 // opcode=0xcddd7b5b
         "cubeface.ps f29, f27, f27\n"                         // opcode=0x89bd8efb
         "fmul.ps f21, f24, f24, dyn\n"                        // opcode=0x118c7afb
         "fnmsub.s f21, f24, f28, f29, rup\n"                  // opcode=0xe9cc3acb
         "flt.pi f27, f29, f21\n"                              // opcode=0xa75e9dfb
         "fmax.ps f30, f30, f31\n"                             // opcode=0x29ff1f7b
         "fmin.ps f23, f30, f28\n"                             // opcode=0x29cf0bfb
         "fsq2 f30, 0 (x28)\n"                                 // opcode=0xd9ee5e27
         "fmulhu.pi f21, f20, f27\n"                           // opcode=0x17ba1afb
         "maskor m2, m0, m2\n"                                 // opcode=0x6620617b
         "fle.pi f24, f22, f28\n"                              // opcode=0xa7cb0c7b
         "fmvs.x.ps x21, f26, 4\n"                             // opcode=0xe04d2afb
         "fmv.x.s x23, f21\n"                                  // opcode=0xe00a8bd3
         "cubesgntc.ps f23, f22, f30\n"                        // opcode=0x89eb3bfb
         "mova.x.m x27\n"                                      // opcode=0xd6000dfb
         "fbci.ps f27, 504909\n"                               // opcode=0x7b44dd9f
         "fmvz.x.ps x25, f24, 6\n"                             // opcode=0xe06c0cfb
         "fle.s x22, f24, f28\n"                               // opcode=0xa1cc0b53
         "fsrli.pi f30, f25, 30\n"                             // opcode=0x4fecdf7b
         "frcp.ps f30, f22\n"                                  // opcode=0x587b0f7b
         "fnmadd.ps f26, f26, f21, f31, dyn\n"                 // opcode=0xff5d7d5b
         "cubesgnsc.ps f20, f24, f24\n"                        // opcode=0x898c2a7b
         "fnmsub.s f24, f28, f27, f28, dyn\n"                  // opcode=0xe1be7c4b
         "fmsub.ps f23, f21, f31, f27, dyn\n"                  // opcode=0xdbfafbdb
         "fpackrepb.pi f29, f27\n"                             // opcode=0x260d8efb
         "flt.pi f31, f22, f22\n"                              // opcode=0xa76b1ffb
         "maskpopc x21, m2\n"                                  // opcode=0x52010afb
         "fand.pi f28, f25, f29\n"                             // opcode=0x07dcfe7b
         "fbci.ps f20, 365302\n"                               // opcode=0x592f6a1f
         "fandi.pi f22, f31, 267\n"                            // opcode=0x44bf9b3f
         "fmin.s f23, f23, f23\n"                              // opcode=0x297b8bd3
         "faddi.pi f22, f26, 933\n"                            // opcode=0xec5d0b3f
         "maskand m6, m7, m0\n"                                // opcode=0x6603f37b
         "feq.pi f28, f28, f28\n"                              // opcode=0xa7ce2e7b
         "fmax.ps f27, f24, f28\n"                             // opcode=0x29cc1dfb
         "fsetm.pi m7, f31\n"                                  // opcode=0xe00fd3fb
         "fnmsub.s f22, f21, f23, f23, dyn\n"                  // opcode=0xb97afb4b
         "fmul.ps f23, f30, f27, dyn\n"                        // opcode=0x11bf7bfb
         "fle.s x30, f22, f26\n"                               // opcode=0xa1ab0f53
         "fsrl.pi f20, f31, f29\n"                             // opcode=0x07dfda7b
         "fbc.ps f23, 120(x27)\n"                              // opcode=0x278d8b8b
         "fsgnjn.s f30, f27, f21\n"                            // opcode=0x215d9f53
         "fmv.s.x f30, x22\n"                                  // opcode=0xf00b0f53
         "fmadd.s f24, f23, f22, f22, dyn\n"                   // opcode=0xb16bfc43
         "fmsub.s f31, f28, f22, f29, dyn\n"                   // opcode=0xe96e7fc7
         "fmv.s.x f21, x21\n"                                  // opcode=0xf00a8ad3
         "mova.x.m x22\n"                                      // opcode=0xd6000b7b
         "fpackreph.pi f28, f22\n"                             // opcode=0x260b1e7b
         "fmin.ps f31, f24, f25\n"                             // opcode=0x299c0ffb
         "fmax.pi f25, f24, f29\n"                             // opcode=0x2fdc1cfb
         "li x2, 703\n"
         "mova.m.x x5\n"
         "fsgnj.s f20, f27, f27\n"                             // opcode=0x21bd8a53
         "fslli.pi f27, f29, 4\n"                              // opcode=0x4e4e9dfb
         "flem.ps m7, f21, f25\n"                              // opcode=0xa19ac3fb
         "flem.ps m1, f29, f20\n"                              // opcode=0xa14ec0fb
         "flog.ps f24, f29\n"                                  // opcode=0x583e8c7b
         "cubeface.ps f29, f22, f24\n"                         // opcode=0x898b0efb
         "fcmovm.ps f31, f24, f27\n"                           // opcode=0x01bc0ff7
         "cubefaceidx.ps f26, f23, f28\n"                      // opcode=0x89cb9d7b
         "fslli.pi f28, f28, 24\n"                             // opcode=0x4f8e1e7b
         "fcmovm.ps f21, f20, f31\n"                           // opcode=0x01fa0af7
         "fsrl.pi f26, f22, f27\n"                             // opcode=0x07bb5d7b
         "fslli.pi f23, f20, 0\n"                              // opcode=0x4e0a1bfb
         "fmul.pi f26, f24, f26\n"                             // opcode=0x17ac0d7b
         "fadd.pi f31, f25, f23\n"                             // opcode=0x077c8ffb
         "fcmovm.ps f23, f22, f23\n"                           // opcode=0x017b0bf7
         "fround.ps f21, f25, dyn\n"                           // opcode=0x581cfafb
         "fmvs.x.ps x21, f20, 6\n"                             // opcode=0xe06a2afb
         "fmadd.ps f31, f28, f23, f21, dyn\n"                  // opcode=0xa97e7fdb
         "fmaxu.pi f20, f25, f29\n"                            // opcode=0x2fdcba7b
         "flt.s x31, f27, f24\n"                               // opcode=0xa18d9fd3
         "fmul.pi f21, f28, f27\n"                             // opcode=0x17be0afb
         "fmulh.pi f25, f24, f21\n"                            // opcode=0x175c2cfb
         "fcmov.ps f31, f21, f31, f28\n"                       // opcode=0xe5faafbf
         "fsatu8.pi f22, f31\n"                                // opcode=0x061fbb7b
         "feq.ps f21, f30, f23\n"                              // opcode=0xa17f2afb
         "flt.s x27, f23, f21\n"                               // opcode=0xa15b9dd3
         "fxor.pi f28, f20, f31\n"                             // opcode=0x07fa4e7b
         "fsrl.pi f22, f23, f28\n"                             // opcode=0x07cbdb7b
         "fround.ps f23, f27, dyn\n"                           // opcode=0x581dfbfb
         "fltm.ps m5, f29, f31\n"                              // opcode=0xa1fed2fb
         "fxor.pi f26, f29, f26\n"                             // opcode=0x07aecd7b
         "fadd.s f20, f29, f22, dyn\n"                         // opcode=0x016efa53
         "fmin.ps f31, f23, f20\n"                             // opcode=0x294b8ffb
         "fltm.ps m6, f23, f30\n"                              // opcode=0xa1ebd37b
         "fsq2 f21, 0 (x31)\n"                                 // opcode=0x235fdaa7
         "fclass.s x27, f22\n"                                 // opcode=0xe00b1dd3
         "fclass.ps f20, f28\n"                                // opcode=0xe00e1a7b
         "fnmadd.ps f30, f22, f22, f22, dyn\n"                 // opcode=0xb76b7f5b
         "fxor.pi f29, f25, f26\n"                             // opcode=0x07accefb
         "fsub.s f29, f24, f29, dyn\n"                         // opcode=0x09dc7ed3
         "fsatu8.pi f21, f26\n"                                // opcode=0x061d3afb
         "fbci.pi f25, 393584\n"                               // opcode=0x60170cdf
         "for.pi f25, f22, f20\n"                              // opcode=0x074b6cfb
         "fpackreph.pi f21, f22\n"                             // opcode=0x260b1afb
         "feq.s x31, f23, f25\n"                               // opcode=0xa19bafd3
         "mova.x.m x25\n"                                      // opcode=0xd6000cfb
         "fbci.ps f24, 851807\n"                               // opcode=0xcff5fc1f
         "fsll.pi f21, f28, f23\n"                             // opcode=0x077e1afb
         "fcmov.ps f27, f29, f20, f20\n"                       // opcode=0xa54eadbf
         //RTLMIN-5479"fsin.ps f25, f25\n"                                  // opcode=0x586c8cfb
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "flt.ps f22, f28, f23\n"                              // opcode=0xa17e1b7b
         "feq.ps f26, f31, f22\n"                              // opcode=0xa16fad7b
         "fmsub.ps f21, f28, f30, f23, rtz\n"                  // opcode=0xbbee1adb
         "fsat8.pi f20, f21\n"                                 // opcode=0x060aba7b
         "maskxor m0, m6, m2\n"                                // opcode=0x6623407b
         "cubesgntc.ps f28, f21, f20\n"                        // opcode=0x894abe7b
         "fmax.s f21, f22, f26\n"                              // opcode=0x29ab1ad3
         "fmulh.pi f27, f20, f26\n"                            // opcode=0x17aa2dfb
         "for.pi f23, f29, f24\n"                              // opcode=0x078eebfb
         "fsrli.pi f30, f30, 19\n"                             // opcode=0x4f3f5f7b
         "fmax.pi f24, f26, f30\n"                             // opcode=0x2fed1c7b
         "fsatu8.pi f31, f21\n"                                // opcode=0x061abffb
         "fltu.pi f20, f29, f23\n"                             // opcode=0xa77eba7b
         "faddi.pi f29, f27, 270\n"                            // opcode=0x44ed8ebf
         "fadd.s f21, f25, f28, dyn\n"                         // opcode=0x01ccfad3
         "cubefaceidx.ps f22, f22, f22\n"                      // opcode=0x896b1b7b
         "maskpopcz x30, m3\n"                                 // opcode=0x54018f7b
         "fround.ps f24, f23, dyn\n"                           // opcode=0x581bfc7b
         "fnmsub.s f26, f20, f22, f23, dyn\n"                  // opcode=0xb96a7d4b
         "maskpopc x20, m3\n"                                  // opcode=0x52018a7b
         "fmax.pi f21, f23, f29\n"                             // opcode=0x2fdb9afb
         "fltu.pi f31, f20, f21\n"                             // opcode=0xa75a3ffb
         "fclass.s x22, f24\n"                                 // opcode=0xe00c1b53
         "fmulh.pi f20, f24, f29\n"                            // opcode=0x17dc2a7b
         "fmsub.s f30, f27, f31, f30, dyn\n"                   // opcode=0xf1fdff47
         "fmadd.s f29, f31, f29, f25, dyn\n"                   // opcode=0xc9dffec3
         "mova.m.x x20\n"                                      // opcode=0xd60a107b
         "flog.ps f27, f28\n"                                  // opcode=0x583e0dfb
         "fsgnj.ps f20, f22, f28\n"                            // opcode=0x21cb0a7b
         "fround.ps f27, f31, dyn\n"                           // opcode=0x581ffdfb
         "fmsub.ps f30, f25, f21, f31, dyn\n"                  // opcode=0xfb5cff5b
         "fmax.ps f21, f31, f27\n"                             // opcode=0x29bf9afb
         "flog.ps f28, f29\n"                                  // opcode=0x583e8e7b
         "ffrc.ps f27, f20\n"                                  // opcode=0x582a0dfb
         "feqm.ps m1, f26, f28\n"                              // opcode=0xa1cd60fb
         "fmul.pi f26, f25, f23\n"                             // opcode=0x177c8d7b
         "fmulh.pi f21, f30, f30\n"                            // opcode=0x17ef2afb
         "fcmovm.ps f20, f30, f23\n"                           // opcode=0x017f0a77
         "fadd.s f22, f21, f31, dyn\n"                         // opcode=0x01fafb53
         "mova.x.m x21\n"                                      // opcode=0xd6000afb
         "maskor m4, m3, m1\n"                                 // opcode=0x6611e27b
         "fcmov.ps f23, f21, f30, f21\n"                       // opcode=0xadeaabbf
         "fandi.pi f22, f24, 414\n"                            // opcode=0x65ec1b3f
         "fandi.pi f29, f24, 459\n"                            // opcode=0x74bc1ebf
         "fsq2 f26, 0 (x25)\n"                                 // opcode=0x61acd4a7
         "fmin.pi f25, f31, f23\n"                             // opcode=0x2f7f8cfb
         //RTLMIN-5479"fsin.ps f23, f31\n"                                  // opcode=0x586f8bfb
         "fltm.ps m2, f24, f23\n"                              // opcode=0xa17c517b
         "flog.ps f20, f29\n"                                  // opcode=0x583e8a7b
         "fmul.pi f25, f29, f26\n"                             // opcode=0x17ae8cfb
         "li x2, 184\n"
         "mova.m.x x5\n"
         "fsrai.pi f22, f31, 10\n"                             // opcode=0x4eaffb7b
         "frcp.ps f24, f24\n"                                  // opcode=0x587c0c7b
         "fmsub.s f30, f28, f21, f26, dyn\n"                   // opcode=0xd15e7f47
         "fmax.pi f22, f31, f28\n"                             // opcode=0x2fcf9b7b
         "fsatu8.pi f25, f26\n"                                // opcode=0x061d3cfb
         "fcmov.ps f26, f20, f26, f25\n"                       // opcode=0xcdaa2d3f
         "fmvs.x.ps x28, f24, 3\n"                             // opcode=0xe03c2e7b
         "masknot m0, m6\n"                                    // opcode=0x6603207b
         "fsra.pi f21, f28, f22\n"                             // opcode=0x0f6e5afb
         "fclass.s x28, f23\n"                                 // opcode=0xe00b9e53
         "maskxor m0, m6, m4\n"                                // opcode=0x6643407b
         "fnot.pi f20, f29\n"                                  // opcode=0x060eaa7b
         "fmin.ps f24, f25, f29\n"                             // opcode=0x29dc8c7b
         "fminu.pi f31, f24, f29\n"                            // opcode=0x2fdc2ffb
         "fsub.pi f20, f30, f23\n"                             // opcode=0x0f7f0a7b
         "fsat8.pi f28, f22\n"                                 // opcode=0x060b3e7b
         "fmadd.ps f27, f21, f25, f28, dyn\n"                  // opcode=0xe19afddb
         "cubesgnsc.ps f22, f25, f25\n"                        // opcode=0x899cab7b
         "fslli.pi f29, f28, 3\n"                              // opcode=0x4e3e1efb
         "fnmadd.ps f25, f22, f31, f24, dyn\n"                 // opcode=0xc7fb7cdb
         "fround.ps f30, f25, rmm\n"                           // opcode=0x581ccf7b
         "fltu.pi f28, f27, f25\n"                             // opcode=0xa79dbe7b
         "fmulh.pi f30, f27, f29\n"                            // opcode=0x17ddaf7b
         "fsgnjn.s f27, f20, f23\n"                            // opcode=0x217a1dd3
         "fmulh.pi f28, f29, f26\n"                            // opcode=0x17aeae7b
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
