/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

int main() {
   *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

   volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

      // vpu register f0
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            

      // vpu register f1
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             

      // vpu register f2
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         

      // vpu register f3
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            

      // vpu register f4
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   

      // vpu register f5
      0x80011111,    // -9.7958E-41           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         

      // vpu register f6
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 

      // vpu register f7
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   

      // vpu register f8
      0x80011111,    // -9.7958E-41           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           

      // vpu register f9
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            

      // vpu register f10
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         

      // vpu register f11
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            

      // vpu register f12
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f13
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f14
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            

      // vpu register f15
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           

      // vpu register f16
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x4b000000,    // 8388608.0             
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  

      // vpu register f17
      0x4b8c4b40,    // 18388608.0            
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  

      // vpu register f18
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         

      // vpu register f19
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  

      // vpu register f20
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   

      // vpu register f21
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           

      // vpu register f22
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            

      // vpu register f23
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  

      // vpu register f24
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            

      // vpu register f25
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            

      // vpu register f26
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  
      0xcb8c4b40,    // -18388608.0           

      // vpu register f27
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  

      // vpu register f28
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  

      // vpu register f29
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             

      // vpu register f30
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0x4b000000,    // 8388608.0             

      // vpu register f31
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
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
         "li x0,  0x804cf860\n"
         "li x1,  0x80e8f460\n"
         "li x2,  0x80010000\n"
         "li x3,  0x80ff29a0\n"
         "li x4,  0x801c3a60\n"
         "li x5,  0x80a42680\n"
         "li x6,  0x80aa4f80\n"
         "li x7,  0x806d3380\n"
         "li x8,  0x806df700\n"
         "li x9,  0x805d9ee0\n"
         "li x10,  0x80d37f80\n"
         "li x11,  0x8041d160\n"
         "li x12,  0x80232600\n"
         "li x13,  0x801db520\n"
         "li x14,  0x806e38e0\n"
         "li x15,  0x8047a800\n"
         "li x16,  0x803117e0\n"
         "li x17,  0x80b656a0\n"
         "li x18,  0x80e4e700\n"
         "li x19,  0x80192380\n"
         "li x20,  0x8010b5c0\n"
         "li x21,  0x803f71e0\n"
         "li x22,  0x8073de20\n"
         "li x23,  0x8045e120\n"
         "li x24,  0x802b13a0\n"
         "li x25,  0x805413c0\n"
         "li x26,  0x80807400\n"
         "li x27,  0x8011d0a0\n"
         "li x28,  0x801a4840\n"
         "li x29,  0x800e33c0\n"
         "li x30,  0x8070bdc0\n"
         "li x31,  0x80332d20\n"
        // "fsw.ps f0,   0 (x0)\n"
        // "fsw.ps f0,   32 (x1)\n"
        // "fsw.ps f0,   64 (x2)\n"
        // "fsw.ps f0,  0 (%[operands])\n"    // for tensor ops
        // "fsw.ps f1,   0 (x1)\n"
        // "fsw.ps f1,   32 (x2)\n"
        // "fsw.ps f1,   64 (x3)\n"
        // "fsw.ps f1,  32 (%[operands])\n"    // for tensor ops
        // "fsw.ps f2,   0 (x2)\n"
        // "fsw.ps f2,   32 (x3)\n"
        // "fsw.ps f2,   64 (x4)\n"
        // "fsw.ps f2,  64 (%[operands])\n"    // for tensor ops
        // "fsw.ps f3,   0 (x3)\n"
        // "fsw.ps f3,   32 (x4)\n"
        // "fsw.ps f3,   64 (x5)\n"
        // "fsw.ps f3,  96 (%[operands])\n"    // for tensor ops
        // "fsw.ps f4,   0 (x4)\n"
        // "fsw.ps f4,   32 (x5)\n"
        // "fsw.ps f4,   64 (x6)\n"
        // "fsw.ps f4,  128 (%[operands])\n"    // for tensor ops
        // "fsw.ps f5,   0 (x5)\n"
        // "fsw.ps f5,   32 (x6)\n"
        // "fsw.ps f5,   64 (x7)\n"
        // "fsw.ps f5,  160 (%[operands])\n"    // for tensor ops
        // "fsw.ps f6,   0 (x6)\n"
        // "fsw.ps f6,   32 (x7)\n"
        // "fsw.ps f6,   64 (x8)\n"
        // "fsw.ps f6,  192 (%[operands])\n"    // for tensor ops
        // "fsw.ps f7,   0 (x7)\n"
        // "fsw.ps f7,   32 (x8)\n"
        // "fsw.ps f7,   64 (x9)\n"
        // "fsw.ps f7,  224 (%[operands])\n"    // for tensor ops
        // "fsw.ps f8,   0 (x8)\n"
        // "fsw.ps f8,   32 (x9)\n"
        // "fsw.ps f8,   64 (x10)\n"
        // "fsw.ps f8,  256 (%[operands])\n"    // for tensor ops
        // "fsw.ps f9,   0 (x9)\n"
        // "fsw.ps f9,   32 (x10)\n"
        // "fsw.ps f9,   64 (x11)\n"
        // "fsw.ps f9,  288 (%[operands])\n"    // for tensor ops
        // "fsw.ps f10,   0 (x10)\n"
        // "fsw.ps f10,   32 (x11)\n"
        // "fsw.ps f10,   64 (x12)\n"
        // "fsw.ps f10,  320 (%[operands])\n"    // for tensor ops
        // "fsw.ps f11,   0 (x11)\n"
        // "fsw.ps f11,   32 (x12)\n"
        // "fsw.ps f11,   64 (x13)\n"
        // "fsw.ps f11,  352 (%[operands])\n"    // for tensor ops
        // "fsw.ps f12,   0 (x12)\n"
        // "fsw.ps f12,   32 (x13)\n"
        // "fsw.ps f12,   64 (x14)\n"
        // "fsw.ps f12,  384 (%[operands])\n"    // for tensor ops
        // "fsw.ps f13,   0 (x13)\n"
        // "fsw.ps f13,   32 (x14)\n"
        // "fsw.ps f13,   64 (x15)\n"
        // "fsw.ps f13,  416 (%[operands])\n"    // for tensor ops
        // "fsw.ps f14,   0 (x14)\n"
        // "fsw.ps f14,   32 (x15)\n"
        // "fsw.ps f14,   64 (x16)\n"
        // "fsw.ps f14,  448 (%[operands])\n"    // for tensor ops
        // "fsw.ps f15,   0 (x15)\n"
        // "fsw.ps f15,   32 (x16)\n"
        // "fsw.ps f15,   64 (x17)\n"
        // "fsw.ps f15,  480 (%[operands])\n"    // for tensor ops
        // "fsw.ps f16,   0 (x16)\n"
        // "fsw.ps f16,   32 (x17)\n"
        // "fsw.ps f16,   64 (x18)\n"
        // "fsw.ps f16,  512 (%[operands])\n"    // for tensor ops
        // "fsw.ps f17,   0 (x17)\n"
        // "fsw.ps f17,   32 (x18)\n"
        // "fsw.ps f17,   64 (x19)\n"
        // "fsw.ps f17,  544 (%[operands])\n"    // for tensor ops
        // "fsw.ps f18,   0 (x18)\n"
        // "fsw.ps f18,   32 (x19)\n"
        // "fsw.ps f18,   64 (x20)\n"
        // "fsw.ps f18,  576 (%[operands])\n"    // for tensor ops
        // "fsw.ps f19,   0 (x19)\n"
        // "fsw.ps f19,   32 (x20)\n"
        // "fsw.ps f19,   64 (x21)\n"
        // "fsw.ps f19,  608 (%[operands])\n"    // for tensor ops
        // "fsw.ps f20,   0 (x20)\n"
        // "fsw.ps f20,   32 (x21)\n"
        // "fsw.ps f20,   64 (x22)\n"
        // "fsw.ps f20,  640 (%[operands])\n"    // for tensor ops
        // "fsw.ps f21,   0 (x21)\n"
        // "fsw.ps f21,   32 (x22)\n"
        // "fsw.ps f21,   64 (x23)\n"
        // "fsw.ps f21,  672 (%[operands])\n"    // for tensor ops
        // "fsw.ps f22,   0 (x22)\n"
        // "fsw.ps f22,   32 (x23)\n"
        // "fsw.ps f22,   64 (x24)\n"
        // "fsw.ps f22,  704 (%[operands])\n"    // for tensor ops
        // "fsw.ps f23,   0 (x23)\n"
        // "fsw.ps f23,   32 (x24)\n"
        // "fsw.ps f23,   64 (x25)\n"
        // "fsw.ps f23,  736 (%[operands])\n"    // for tensor ops
        // "fsw.ps f24,   0 (x24)\n"
        // "fsw.ps f24,   32 (x25)\n"
        // "fsw.ps f24,   64 (x26)\n"
        // "fsw.ps f24,  768 (%[operands])\n"    // for tensor ops
        // "fsw.ps f25,   0 (x25)\n"
        // "fsw.ps f25,   32 (x26)\n"
        // "fsw.ps f25,   64 (x27)\n"
        // "fsw.ps f25,  800 (%[operands])\n"    // for tensor ops
        // "fsw.ps f26,   0 (x26)\n"
        // "fsw.ps f26,   32 (x27)\n"
        // "fsw.ps f26,   64 (x28)\n"
        // "fsw.ps f26,  832 (%[operands])\n"    // for tensor ops
        // "fsw.ps f27,   0 (x27)\n"
        // "fsw.ps f27,   32 (x28)\n"
        // "fsw.ps f27,   64 (x29)\n"
        // "fsw.ps f27,  864 (%[operands])\n"    // for tensor ops
        // "fsw.ps f28,   0 (x28)\n"
        // "fsw.ps f28,   32 (x29)\n"
        // "fsw.ps f28,   64 (x30)\n"
        // "fsw.ps f28,  896 (%[operands])\n"    // for tensor ops
        // "fsw.ps f29,   0 (x29)\n"
        // "fsw.ps f29,   32 (x30)\n"
        // "fsw.ps f29,   64 (x31)\n"
        // "fsw.ps f29,  928 (%[operands])\n"    // for tensor ops
        // "fsw.ps f30,   0 (x30)\n"
        // "fsw.ps f30,   32 (x31)\n"
        // "fsw.ps f30,   64 (x0)\n"
        // "fsw.ps f30,  960 (%[operands])\n"    // for tensor ops
        // "fsw.ps f31,   0 (x31)\n"
        // "fsw.ps f31,   32 (x0)\n"
        // "fsw.ps f31,   64 (x1)\n"
        // "fsw.ps f31,  992 (%[operands])\n"    // for tensor ops
         "fminu.pi f22, f27, f20\n"                            // opcode=0x2f4dab7b
         "fsub.s f22, f25, f20, dyn\n"                         // opcode=0x094cfb53
         "fsgnjx.ps f24, f30, f22\n"                           // opcode=0x216f2c7b
         "fsub.s f24, f28, f28, dyn\n"                         // opcode=0x09ce7c53
         "fbci.pi f20, 333976\n"                               // opcode=0x51898a5f
         //"flq2 f20, 0 (x31)\n"                                 // opcode=0xb0afda07
         "fsrl.pi f27, f23, f20\n"                             // opcode=0x074bddfb
         "fmv.x.s x27, f23\n"                                  // opcode=0xe00b8dd3
         "fsub.s f22, f25, f22, dyn\n"                         // opcode=0x096cfb53
         "fnmadd.s f23, f23, f21, f23, dyn\n"                  // opcode=0xb95bfbcf
         "flt.ps f25, f30, f26\n"                              // opcode=0xa1af1cfb
         "fnot.pi f21, f23\n"                                  // opcode=0x060baafb
         "maskpopcz x23, m1\n"                                 // opcode=0x54008bfb
         "fmadd.ps f25, f26, f23, f24, dyn\n"                  // opcode=0xc17d7cdb
         "fandi.pi f21, f24, 330\n"                            // opcode=0x54ac1abf
         "flt.s x30, f31, f20\n"                               // opcode=0xa14f9f53
         "fexp.ps f20, f27\n"                                  // opcode=0x584d8a7b
         "fsgnjn.ps f22, f24, f30\n"                           // opcode=0x21ec1b7b
         "fnmadd.ps f27, f29, f30, f25, dyn\n"                 // opcode=0xcfeefddb
         "fmin.s f23, f23, f23\n"                              // opcode=0x297b8bd3
         "flt.pi f28, f23, f28\n"                              // opcode=0xa7cb9e7b
         "masknot m5, m0\n"                                    // opcode=0x660022fb
         "fbcx.ps f29, x29\n"                                  // opcode=0x000ebe8b
         "fsrli.pi f20, f27, 5\n"                              // opcode=0x4e5dda7b
         //RTLMIN-5479"frsq.ps f23, f22\n"                                  // opcode=0x588b0bfb
         "maskand m7, m3, m4\n"                                // opcode=0x6641f3fb
         "feq.s x29, f23, f30\n"                               // opcode=0xa1ebaed3
         "flt.pi f27, f28, f22\n"                              // opcode=0xa76e1dfb
         "fsrai.pi f29, f20, 2\n"                              // opcode=0x4e2a7efb
         "fltu.pi f25, f30, f25\n"                             // opcode=0xa79f3cfb
         "fle.s x31, f23, f23\n"                               // opcode=0xa17b8fd3
         "fsrl.pi f26, f30, f20\n"                             // opcode=0x074f5d7b
         "fmv.x.s x31, f29\n"                                  // opcode=0xe00e8fd3
         "fcmov.ps f27, f30, f29, f27\n"                       // opcode=0xdddf2dbf
         "fadd.s f30, f25, f24, rmm\n"                         // opcode=0x018ccf53
         "fsat8.pi f21, f20\n"                                 // opcode=0x060a3afb
         "fsll.pi f26, f26, f21\n"                             // opcode=0x075d1d7b
         //"fsq2 f28, 0 (x22)\n"                                 // opcode=0x41cb56a7
         "ffrc.ps f23, f24\n"                                  // opcode=0x582c0bfb
         //"fsq2 f28, 0 (x30)\n"                                 // opcode=0x29cf5127
         "fmax.pi f26, f22, f26\n"                             // opcode=0x2fab1d7b
         "maskand m7, m5, m5\n"                                // opcode=0x6652f3fb
         "maskpopc x21, m2\n"                                  // opcode=0x52010afb
         "fmax.ps f29, f31, f28\n"                             // opcode=0x29cf9efb
         "feqm.ps m0, f21, f27\n"                              // opcode=0xa1bae07b
         "fltm.pi m2, f22, f28\n"                              // opcode=0x3fcb017b
         "maskpopc x23, m6\n"                                  // opcode=0x52030bfb
         "fsetm.pi m5, f20\n"                                  // opcode=0xe00a52fb
         "fnot.pi f30, f24\n"                                  // opcode=0x060c2f7b
         "fclass.s x21, f25\n"                                 // opcode=0xe00c9ad3
         "fsgnj.s f29, f25, f24\n"                             // opcode=0x218c8ed3
         "fsgnj.ps f30, f31, f22\n"                            // opcode=0x216f8f7b
         "fmin.s f28, f30, f26\n"                              // opcode=0x29af0e53
         "fsgnjx.s f20, f21, f25\n"                            // opcode=0x219aaa53
         "flt.s x30, f27, f31\n"                               // opcode=0xa1fd9f53
         "fcmovm.ps f31, f20, f31\n"                           // opcode=0x01fa0ff7
         "fmin.pi f25, f31, f21\n"                             // opcode=0x2f5f8cfb
         "fsub.ps f28, f21, f21, dyn\n"                        // opcode=0x095afe7b
         "feqm.ps m6, f29, f20\n"                              // opcode=0xa14ee37b
         "frcp.ps f27, f30\n"                                  // opcode=0x587f0dfb
         "maskpopcz x27, m1\n"                                 // opcode=0x54008dfb
         "fnmsub.s f31, f22, f23, f27, rdn\n"                  // opcode=0xd97b2fcb
         "mova.m.x x21\n"                                      // opcode=0xd60a907b
         "fcmovm.ps f20, f26, f26\n"                           // opcode=0x01ad0a77
         "fminu.pi f20, f22, f21\n"                            // opcode=0x2f5b2a7b
         "fminu.pi f28, f23, f20\n"                            // opcode=0x2f4bae7b
         "fnmsub.s f28, f27, f23, f23, dyn\n"                  // opcode=0xb97dfe4b
         "fandi.pi f29, f20, 29\n"                            // opcode=0xec1a1ebf
         "frcp.ps f23, f23\n"                                  // opcode=0x587b8bfb
         "fsrli.pi f25, f27, 31\n"                             // opcode=0x4ffddcfb
         //"fbc.ps f25, 61(x26)\n"                               // opcode=0x1bdd0c8b
         //"fbc.ps f28, 124(x21)\n"                              // opcode=0x9fca8e0b
         "fsgnj.s f22, f28, f26\n"                             // opcode=0x21ae0b53
         "maskpopc x30, m5\n"                                  // opcode=0x52028f7b
         "fsetm.pi m5, f23\n"                                  // opcode=0xe00bd2fb
         "fsgnjx.s f24, f28, f29\n"                            // opcode=0x21de2c53
         "fsgnj.ps f27, f22, f31\n"                            // opcode=0x21fb0dfb
         "fadd.s f29, f29, f28, dyn\n"                         // opcode=0x01cefed3
         "fmvs.x.ps x23, f31, 7\n"                             // opcode=0xe07fabfb
         "fsgnjx.ps f27, f21, f22\n"                           // opcode=0x216aadfb
         "fcmov.ps f30, f25, f25, f23\n"                       // opcode=0xbd9caf3f
         "fnmsub.s f31, f29, f30, f22, dyn\n"                  // opcode=0xb1eeffcb
         "flt.pi f28, f28, f25\n"                              // opcode=0xa79e1e7b
         "feq.s x23, f31, f31\n"                               // opcode=0xa1ffabd3
         "maskor m0, m6, m5\n"                                 // opcode=0x6653607b
         "fsub.ps f26, f21, f21, dyn\n"                        // opcode=0x095afd7b
         "fand.pi f21, f30, f20\n"                             // opcode=0x074f7afb
         //RTLMIN-5479"frsq.ps f29, f22\n"                                  // opcode=0x588b0efb
         "cubesgnsc.ps f30, f31, f24\n"                        // opcode=0x898faf7b
         "feq.s x21, f31, f24\n"                               // opcode=0xa18faad3
         "fclass.ps f20, f28\n"                                // opcode=0xe00e1a7b
         "fsub.pi f31, f20, f20\n"                             // opcode=0x0f4a0ffb
         "fsrl.pi f28, f25, f26\n"                             // opcode=0x07acde7b
         "maskpopcz x23, m2\n"                                 // opcode=0x54010bfb
         "fxor.pi f21, f25, f29\n"                             // opcode=0x07dccafb
         //RTLMIN-5479"fsin.ps f31, f28\n"                                  // opcode=0x586e0ffb
         "for.pi f25, f29, f31\n"                              // opcode=0x07feecfb
         "fand.pi f24, f21, f27\n"                             // opcode=0x07bafc7b
         "ffrc.ps f20, f24\n"                                  // opcode=0x582c0a7b
         "fmul.s f24, f26, f30, dyn\n"                         // opcode=0x11ed7c53
         "fsra.pi f21, f28, f27\n"                             // opcode=0x0fbe5afb
         "fnot.pi f29, f21\n"                                  // opcode=0x060aaefb
         "maskand m4, m0, m2\n"                                // opcode=0x6620727b
         "fnmsub.s f25, f28, f24, f21, rdn\n"                  // opcode=0xa98e2ccb
         "fmin.pi f24, f21, f29\n"                             // opcode=0x2fda8c7b
         "fround.ps f24, f21, dyn\n"                           // opcode=0x581afc7b
         "fsub.s f22, f26, f21, dyn\n"                         // opcode=0x095d7b53
         "fltu.pi f22, f30, f23\n"                             // opcode=0xa77f3b7b
         "fle.pi f27, f31, f21\n"                              // opcode=0xa75f8dfb
         "fmsub.ps f20, f26, f22, f25, dyn\n"                  // opcode=0xcb6d7a5b
         "fsatu8.pi f30, f31\n"                                // opcode=0x061fbf7b
         "fmul.ps f23, f23, f23, rup\n"                        // opcode=0x117bbbfb
         "fmadd.s f25, f26, f31, f21, dyn\n"                   // opcode=0xa9fd7cc3
         "masknot m1, m1\n"                                    // opcode=0x6600a0fb
         //"fbc.ps f30, 40(x20)\n"                               // opcode=0x6a8a0f0b
         "feq.ps f29, f27, f29\n"                              // opcode=0xa1ddaefb
         "fsgnjx.s f25, f31, f23\n"                            // opcode=0x217facd3
         "fsll.pi f29, f20, f27\n"                             // opcode=0x07ba1efb
         "fmin.s f21, f21, f27\n"                              // opcode=0x29ba8ad3
         "fltu.pi f23, f20, f27\n"                             // opcode=0xa7ba3bfb
         "fmul.ps f29, f28, f26, dyn\n"                        // opcode=0x11ae7efb
         "fexp.ps f23, f30\n"                                  // opcode=0x584f0bfb
         "fmv.x.s x24, f24\n"                                  // opcode=0xe00c0c53
         "feq.pi f25, f27, f21\n"                              // opcode=0xa75dacfb
         "fmsub.s f29, f30, f23, f25, dyn\n"                   // opcode=0xc97f7ec7
         "fmul.s f22, f28, f27, dyn\n"                         // opcode=0x11be7b53
         "fbcx.ps f29, x27\n"                                  // opcode=0x000dbe8b
         "fnmadd.s f20, f23, f26, f28, dyn\n"                  // opcode=0xe1abfa4f
         //"fsq2 f24, 0 (x23)\n"                                 // opcode=0xc58bdf27
         //RTLMIN-5479"fsin.ps f31, f29\n"                                  // opcode=0x586e8ffb
         "fle.ps f29, f29, f23\n"                              // opcode=0xa17e8efb
         "fsetm.pi m3, f30\n"                                  // opcode=0xe00f51fb
         "fmin.ps f26, f30, f29\n"                             // opcode=0x29df0d7b
         "fmax.pi f23, f22, f23\n"                             // opcode=0x2f7b1bfb
         "feqm.ps m0, f22, f22\n"                              // opcode=0xa16b607b
         "maskxor m1, m0, m4\n"                                // opcode=0x664040fb
         "feq.s x26, f24, f25\n"                               // opcode=0xa19c2d53
         "fmv.x.s x26, f27\n"                                  // opcode=0xe00d8d53
         "fsgnj.s f24, f20, f27\n"                             // opcode=0x21ba0c53
         "fsetm.pi m0, f31\n"                                  // opcode=0xe00fd07b
         "fcmov.ps f27, f30, f21, f25\n"                       // opcode=0xcd5f2dbf
         "cubesgnsc.ps f30, f27, f27\n"                        // opcode=0x89bdaf7b
         "for.pi f21, f26, f27\n"                              // opcode=0x07bd6afb
         //RTLMIN-5479"frsq.ps f22, f22\n"                                  // opcode=0x588b0b7b
         "flt.ps f20, f20, f27\n"                              // opcode=0xa1ba1a7b
         //RTLMIN-5479"frsq.ps f24, f30\n"                                  // opcode=0x588f0c7b
         "maskpopc x21, m6\n"                                  // opcode=0x52030afb
         "fltu.pi f22, f20, f20\n"                             // opcode=0xa74a3b7b
         "mova.m.x x25\n"                                      // opcode=0xd60c907b
         "fsrli.pi f28, f22, 16\n"                             // opcode=0x4f0b5e7b
         //"flq2 f24, 0 (x30)\n"                                 // opcode=0x41af5c07
         "fsub.s f23, f30, f28, dyn\n"                         // opcode=0x09cf7bd3
         "fsgnjn.s f30, f31, f31\n"                            // opcode=0x21ff9f53
         "fmv.s.x f22, x27\n"                                  // opcode=0xf00d8b53
         "fcmov.ps f20, f27, f29, f21\n"                       // opcode=0xadddaa3f
         "fsgnjx.ps f22, f24, f28\n"                           // opcode=0x21cc2b7b
         "fltm.ps m4, f20, f30\n"                              // opcode=0xa1ea527b
         "fsra.pi f29, f21, f22\n"                             // opcode=0x0f6adefb
         "cubeface.ps f28, f31, f23\n"                         // opcode=0x897f8e7b
         "fltm.pi m4, f24, f20\n"                              // opcode=0x3f4c027b
         "fcmov.ps f22, f30, f29, f27\n"                       // opcode=0xdddf2b3f
         "cubesgnsc.ps f23, f29, f26\n"                        // opcode=0x89aeabfb
         "fsgnjx.s f27, f28, f29\n"                            // opcode=0x21de2dd3
         "fbci.ps f20, 855426\n"                               // opcode=0xd0d82a1f
         "fsgnj.s f26, f26, f26\n"                             // opcode=0x21ad0d53
         "fcmov.ps f29, f24, f26, f22\n"                       // opcode=0xb5ac2ebf
         "ffrc.ps f25, f24\n"                                  // opcode=0x582c0cfb
         "maskand m0, m3, m5\n"                                // opcode=0x6651f07b
         "feq.pi f31, f21, f30\n"                              // opcode=0xa7eaaffb
         "fsgnjx.s f29, f23, f23\n"                            // opcode=0x217baed3
         "fmv.x.s x21, f25\n"                                  // opcode=0xe00c8ad3
         "fsgnj.ps f24, f24, f27\n"                            // opcode=0x21bc0c7b
         "fsll.pi f22, f26, f22\n"                             // opcode=0x076d1b7b
         "fcmovm.ps f20, f23, f22\n"                           // opcode=0x016b8a77
         "feq.pi f23, f22, f20\n"                              // opcode=0xa74b2bfb
         "fxor.pi f28, f23, f20\n"                             // opcode=0x074bce7b
         "fmul.ps f26, f21, f29, dyn\n"                        // opcode=0x11dafd7b
         "fsub.s f23, f29, f22, dyn\n"                         // opcode=0x096efbd3
         "fandi.pi f28, f23, 87\n"                            // opcode=0xf5bb9e3f
         "feqm.ps m0, f30, f24\n"                              // opcode=0xa18f607b
         "fpackrepb.pi f27, f29\n"                             // opcode=0x260e8dfb
         "feq.s x22, f28, f24\n"                               // opcode=0xa18e2b53
         "fsra.pi f28, f28, f26\n"                             // opcode=0x0fae5e7b
         "fsetm.pi m1, f23\n"                                  // opcode=0xe00bd0fb
         "fcmovm.ps f24, f29, f28\n"                           // opcode=0x01ce8c77
         "fsra.pi f22, f28, f28\n"                             // opcode=0x0fce5b7b
         "flem.ps m3, f31, f22\n"                              // opcode=0xa16fc1fb
         "fandi.pi f20, f20, 61\n"                            // opcode=0xa55a1a3f
         "fsgnjx.ps f27, f31, f27\n"                           // opcode=0x21bfadfb
         "fmulh.pi f29, f22, f23\n"                            // opcode=0x177b2efb
         "fnot.pi f20, f26\n"                                  // opcode=0x060d2a7b
         "fsgnj.ps f22, f30, f26\n"                            // opcode=0x21af0b7b
         "masknot m1, m7\n"                                    // opcode=0x6603a0fb
         "fmaxu.pi f21, f31, f31\n"                            // opcode=0x2fffbafb
         "fsub.pi f23, f24, f29\n"                             // opcode=0x0fdc0bfb
         "fnmsub.ps f26, f29, f25, f27, dyn\n"                 // opcode=0xdd9efd5b
         "fsrli.pi f31, f31, 31\n"                             // opcode=0x4fffdffb
         "fsll.pi f26, f26, f27\n"                             // opcode=0x07bd1d7b
         "ffrc.ps f28, f22\n"                                  // opcode=0x582b0e7b
         "fle.ps f22, f29, f25\n"                              // opcode=0xa19e8b7b
         "fround.ps f22, f28, dyn\n"                           // opcode=0x581e7b7b
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fbcx.ps f23, x21\n"                                  // opcode=0x000abb8b
         "fsub.pi f29, f31, f25\n"                             // opcode=0x0f9f8efb
         "fsra.pi f22, f22, f23\n"                             // opcode=0x0f7b5b7b
         "fclass.ps f31, f26\n"                                // opcode=0xe00d1ffb
         "fsat8.pi f25, f21\n"                                 // opcode=0x060abcfb
         "fmvs.x.ps x27, f25, 0\n"                             // opcode=0xe00cadfb
         "flt.ps f20, f24, f26\n"                              // opcode=0xa1ac1a7b
         "fmul.ps f26, f30, f21, dyn\n"                        // opcode=0x115f7d7b
         "fpackrepb.pi f21, f30\n"                             // opcode=0x260f0afb
         "maskor m6, m5, m4\n"                                 // opcode=0x6642e37b
         "feq.ps f28, f23, f21\n"                              // opcode=0xa15bae7b
         "cubesgntc.ps f26, f24, f29\n"                        // opcode=0x89dc3d7b
         "fmadd.ps f28, f31, f26, f31, rup\n"                  // opcode=0xf9afbe5b
         //"fbc.ps f21, 54(x27)\n"                               // opcode=0xf36d8a8b
         "fmul.s f23, f20, f28, dyn\n"                         // opcode=0x11ca7bd3
         "maskor m6, m7, m4\n"                                 // opcode=0x6643e37b
         "fbci.pi f25, 502889\n"                               // opcode=0x7ac69cdf
         "cubeface.ps f28, f20, f30\n"                         // opcode=0x89ea0e7b
         "fadd.s f22, f26, f26, dyn\n"                         // opcode=0x01ad7b53
         "feq.pi f23, f31, f26\n"                              // opcode=0xa7afabfb
         "maskpopc x28, m2\n"                                  // opcode=0x52010e7b
         "feq.ps f22, f31, f29\n"                              // opcode=0xa1dfab7b
         "fle.s x28, f31, f31\n"                               // opcode=0xa1ff8e53
         "flem.ps m1, f20, f23\n"                              // opcode=0xa17a40fb
         "fmulhu.pi f21, f30, f27\n"                           // opcode=0x17bf1afb
         "feq.ps f30, f28, f30\n"                              // opcode=0xa1ee2f7b
         "fltm.pi m4, f21, f22\n"                              // opcode=0x3f6a827b
         "maskor m5, m2, m0\n"                                 // opcode=0x660162fb
         "fslli.pi f27, f21, 15\n"                             // opcode=0x4efa9dfb
         "fmsub.s f23, f20, f23, f24, dyn\n"                   // opcode=0xc17a7bc7
         "fle.s x26, f22, f27\n"                               // opcode=0xa1bb0d53
         "fadd.pi f22, f20, f22\n"                             // opcode=0x076a0b7b
         "fmin.ps f30, f27, f24\n"                             // opcode=0x298d8f7b
         "fadd.ps f28, f20, f22, dyn\n"                        // opcode=0x016a7e7b
         "fmax.ps f27, f22, f24\n"                             // opcode=0x298b1dfb
         "fminu.pi f30, f26, f30\n"                            // opcode=0x2fed2f7b
         "fsub.ps f27, f23, f22, dyn\n"                        // opcode=0x096bfdfb
         "flt.pi f25, f23, f25\n"                              // opcode=0xa79b9cfb
         "fnmadd.s f28, f30, f23, f23, dyn\n"                  // opcode=0xb97f7e4f
         "fmul.pi f27, f23, f28\n"                             // opcode=0x17cb8dfb
         "fsrl.pi f26, f29, f29\n"                             // opcode=0x07dedd7b
         "fmin.s f30, f25, f25\n"                              // opcode=0x299c8f53
         "fle.s x29, f21, f26\n"                               // opcode=0xa1aa8ed3
         "fpackrepb.pi f31, f28\n"                             // opcode=0x260e0ffb
         "fsat8.pi f21, f28\n"                                 // opcode=0x060e3afb
         "fmvs.x.ps x27, f23, 3\n"                             // opcode=0xe03badfb
         "fpackreph.pi f28, f21\n"                             // opcode=0x260a9e7b
         "maskor m1, m2, m4\n"                                 // opcode=0x664160fb
         "fnmadd.s f27, f21, f20, f24, dyn\n"                  // opcode=0xc14afdcf
         "fclass.ps f29, f30\n"                                // opcode=0xe00f1efb
         "li x2, 1448\n"
         "mova.m.x x5\n"
         "feq.pi f24, f23, f22\n"                              // opcode=0xa76bac7b
         "fcmov.ps f27, f22, f30, f30\n"                       // opcode=0xf5eb2dbf
         "mova.x.m x22\n"                                      // opcode=0xd6000b7b
         "for.pi f31, f25, f20\n"                              // opcode=0x074ceffb
         "flog.ps f27, f25\n"                                  // opcode=0x583c8dfb
         "fsgnj.ps f24, f26, f25\n"                            // opcode=0x219d0c7b
         "fsub.ps f27, f21, f24, dyn\n"                        // opcode=0x098afdfb
         //"fsq2 f24, 0 (x25)\n"                                 // opcode=0x598cd627
         "fmin.s f22, f24, f26\n"                              // opcode=0x29ac0b53
         "flem.ps m7, f27, f28\n"                              // opcode=0xa1cdc3fb
         "mova.x.m x27\n"                                      // opcode=0xd6000dfb
         "fsrl.pi f28, f22, f23\n"                             // opcode=0x077b5e7b
         //"flq2 f30, 0 (x29)\n"                                 // opcode=0xbfcedf07
         "feq.ps f29, f27, f21\n"                              // opcode=0xa15daefb
         "fle.ps f30, f30, f21\n"                              // opcode=0xa15f0f7b
         "fnot.pi f25, f25\n"                                  // opcode=0x060cacfb
         "fsub.pi f27, f29, f25\n"                             // opcode=0x0f9e8dfb
         "fmulhu.pi f31, f26, f26\n"                           // opcode=0x17ad1ffb
         "fsgnj.s f30, f31, f30\n"                             // opcode=0x21ef8f53
         "fmul.s f20, f29, f24, dyn\n"                         // opcode=0x118efa53
         "feq.s x30, f23, f23\n"                               // opcode=0xa17baf53
         "mova.x.m x20\n"                                      // opcode=0xd6000a7b
         "cubeface.ps f29, f27, f27\n"                         // opcode=0x89bd8efb
         "fpackrepb.pi f25, f28\n"                             // opcode=0x260e0cfb
         "fsrai.pi f26, f23, 2\n"                              // opcode=0x4e2bfd7b
         //"fsq2 f27, 0 (x27)\n"                                 // opcode=0x1fbdd927
         "fle.ps f28, f31, f24\n"                              // opcode=0xa18f8e7b
         //"fbc.ps f21, 83(x27)\n"                               // opcode=0xf53d8a8b
         "fbci.ps f25, 264874\n"                               // opcode=0x40aaac9f
         "mova.x.m x26\n"                                      // opcode=0xd6000d7b
         "fmvs.x.ps x25, f22, 6\n"                             // opcode=0xe06b2cfb
         "fclass.s x26, f24\n"                                 // opcode=0xe00c1d53
         "fexp.ps f30, f27\n"                                  // opcode=0x584d8f7b
         "fmul.pi f26, f23, f29\n"                             // opcode=0x17db8d7b
         "fcmovm.ps f29, f31, f27\n"                           // opcode=0x01bf8ef7
         "fsgnjx.s f26, f21, f27\n"                            // opcode=0x21baad53
         "fmul.pi f21, f26, f22\n"                             // opcode=0x176d0afb
         "fnmadd.s f27, f23, f21, f25, dyn\n"                  // opcode=0xc95bfdcf
         "fnmadd.ps f24, f28, f24, f23, dyn\n"                 // opcode=0xbf8e7c5b
         "feq.ps f27, f26, f20\n"                              // opcode=0xa14d2dfb
         "cubesgntc.ps f26, f28, f21\n"                        // opcode=0x895e3d7b
         "fle.ps f26, f31, f27\n"                              // opcode=0xa1bf8d7b
         "fsgnjx.s f27, f24, f25\n"                            // opcode=0x219c2dd3
         "fsgnjn.s f30, f29, f27\n"                            // opcode=0x21be9f53
         "fmsub.ps f26, f30, f28, f26, dyn\n"                  // opcode=0xd3cf7d5b
         "fsat8.pi f25, f28\n"                                 // opcode=0x060e3cfb
         "faddi.pi f31, f29, 9\n"                            // opcode=0xb45e8fbf
         "fmul.ps f28, f23, f23, dyn\n"                        // opcode=0x117bfe7b
         "fsgnjn.ps f23, f28, f26\n"                           // opcode=0x21ae1bfb
         "fxor.pi f24, f26, f23\n"                             // opcode=0x077d4c7b
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "maskand m0, m2, m7\n"                                // opcode=0x6671707b
         "fsub.ps f22, f25, f30, rtz\n"                        // opcode=0x09ec9b7b
         "fclass.s x26, f30\n"                                 // opcode=0xe00f1d53
         "fand.pi f26, f28, f21\n"                             // opcode=0x075e7d7b
         "fnmsub.ps f29, f28, f26, f30, dyn\n"                 // opcode=0xf5ae7edb
         "fnmadd.ps f28, f24, f28, f27, rtz\n"                 // opcode=0xdfcc1e5b
         "fsub.s f24, f28, f25, dyn\n"                         // opcode=0x099e7c53
         "fnmadd.s f26, f31, f21, f23, dyn\n"                  // opcode=0xb95ffd4f
         "fmadd.ps f20, f27, f27, f25, dyn\n"                  // opcode=0xc9bdfa5b
         "feq.s x20, f20, f30\n"                               // opcode=0xa1ea2a53
         "fltu.pi f22, f29, f20\n"                             // opcode=0xa74ebb7b
         "fmvs.x.ps x21, f26, 1\n"                             // opcode=0xe01d2afb
         "fmul.ps f25, f30, f27, dyn\n"                        // opcode=0x11bf7cfb
         "frcp.ps f26, f24\n"                                  // opcode=0x587c0d7b
         "fmin.pi f31, f22, f31\n"                             // opcode=0x2ffb0ffb
         "fmadd.ps f27, f21, f30, f23, dyn\n"                  // opcode=0xb9eafddb
         "fadd.pi f31, f20, f20\n"                             // opcode=0x074a0ffb
         "mova.x.m x23\n"                                      // opcode=0xd6000bfb
         "fmsub.ps f22, f27, f22, f30, dyn\n"                  // opcode=0xf36dfb5b
         "fsat8.pi f27, f31\n"                                 // opcode=0x060fbdfb
         "cubesgntc.ps f24, f30, f28\n"                        // opcode=0x89cf3c7b
         "fadd.ps f26, f28, f28, rup\n"                        // opcode=0x01ce3d7b
         "faddi.pi f31, f29, 504\n"                            // opcode=0x7d8e8fbf
         "flog.ps f21, f25\n"                                  // opcode=0x583c8afb
         "fnot.pi f20, f26\n"                                  // opcode=0x060d2a7b
         "fbci.pi f28, 450099\n"                               // opcode=0x6de33e5f
         "fmsub.ps f21, f25, f30, f20, dyn\n"                  // opcode=0xa3ecfadb
         "fclass.s x22, f26\n"                                 // opcode=0xe00d1b53
         "ffrc.ps f31, f20\n"                                  // opcode=0x582a0ffb
         "fmsub.s f21, f28, f20, f29, dyn\n"                   // opcode=0xe94e7ac7
         "ffrc.ps f27, f21\n"                                  // opcode=0x582a8dfb
         "fsgnjx.s f24, f20, f21\n"                            // opcode=0x215a2c53
         "fmulh.pi f27, f29, f26\n"                            // opcode=0x17aeadfb
         "for.pi f21, f25, f26\n"                              // opcode=0x07aceafb
         "fadd.s f21, f23, f28, dyn\n"                         // opcode=0x01cbfad3
         "cubesgntc.ps f25, f24, f28\n"                        // opcode=0x89cc3cfb
         "maskpopcz x27, m4\n"                                 // opcode=0x54020dfb
         "fltu.pi f31, f28, f27\n"                             // opcode=0xa7be3ffb
         "fadd.pi f30, f26, f31\n"                             // opcode=0x07fd0f7b
         "fsgnjn.s f24, f28, f22\n"                            // opcode=0x216e1c53
         "fsgnjn.ps f22, f29, f23\n"                           // opcode=0x217e9b7b
         "feq.pi f21, f22, f27\n"                              // opcode=0xa7bb2afb
         "flem.ps m3, f29, f29\n"                              // opcode=0xa1dec1fb
         "fclass.ps f29, f30\n"                                // opcode=0xe00f1efb
         "mova.m.x x30\n"                                      // opcode=0xd60f107b
         "fltm.ps m1, f31, f20\n"                              // opcode=0xa14fd0fb
         "maskand m3, m7, m1\n"                                // opcode=0x6613f1fb
         "fltm.pi m6, f23, f28\n"                              // opcode=0x3fcb837b
         "fnmsub.s f23, f29, f28, f29, dyn\n"                  // opcode=0xe9cefbcb
         "fnmadd.ps f27, f30, f27, f22, dyn\n"                 // opcode=0xb7bf7ddb
         "li x2, 3981\n"
         "mova.m.x x5\n"
         "feq.ps f21, f28, f26\n"                              // opcode=0xa1ae2afb
         "feq.s x23, f30, f29\n"                               // opcode=0xa1df2bd3
         "fround.ps f27, f26, dyn\n"                           // opcode=0x581d7dfb
         "fsll.pi f30, f29, f22\n"                             // opcode=0x076e9f7b
         "fle.pi f20, f23, f26\n"                              // opcode=0xa7ab8a7b
         "fsrli.pi f27, f22, 21\n"                             // opcode=0x4f5b5dfb
         "maskand m1, m3, m0\n"                                // opcode=0x6601f0fb
         "fle.ps f29, f25, f27\n"                              // opcode=0xa1bc8efb
         "fsetm.pi m4, f22\n"                                  // opcode=0xe00b527b
         "fmin.pi f30, f30, f31\n"                             // opcode=0x2fff0f7b
         "fslli.pi f29, f20, 19\n"                             // opcode=0x4f3a1efb
         "fpackrepb.pi f30, f24\n"                             // opcode=0x260c0f7b
         "fminu.pi f25, f26, f30\n"                            // opcode=0x2fed2cfb
         "fmul.s f24, f29, f31, dyn\n"                         // opcode=0x11fefc53
         "fsatu8.pi f24, f28\n"                                // opcode=0x061e3c7b
         "fsgnj.s f29, f26, f30\n"                             // opcode=0x21ed0ed3
         "fmadd.ps f22, f21, f26, f23, dyn\n"                  // opcode=0xb9aafb5b
         "fltm.pi m7, f28, f24\n"                              // opcode=0x3f8e03fb
         "fxor.pi f22, f23, f31\n"                             // opcode=0x07fbcb7b
         "fltm.ps m3, f22, f29\n"                              // opcode=0xa1db51fb
         "fmv.x.s x31, f20\n"                                  // opcode=0xe00a0fd3
         "flt.ps f31, f30, f25\n"                              // opcode=0xa19f1ffb
         //"fbc.ps f23, 90(x29)\n"                               // opcode=0xfdae8b8b
         "cubesgnsc.ps f25, f29, f28\n"                        // opcode=0x89ceacfb
         "cubeface.ps f23, f20, f29\n"                         // opcode=0x89da0bfb
         "fbcx.ps f26, x31\n"                                  // opcode=0x000fbd0b
         "fsgnjn.ps f22, f26, f22\n"                           // opcode=0x216d1b7b
         "maskor m7, m3, m6\n"                                 // opcode=0x6661e3fb
         "fmsub.s f25, f30, f29, f24, rup\n"                   // opcode=0xc1df3cc7
         "fltu.pi f28, f26, f30\n"                             // opcode=0xa7ed3e7b
         "mova.x.m x23\n"                                      // opcode=0xd6000bfb
         //RTLMIN-5479"frsq.ps f22, f25\n"                                  // opcode=0x588c8b7b
         "cubesgnsc.ps f28, f24, f24\n"                        // opcode=0x898c2e7b
         //RTLMIN-5479"fsin.ps f24, f20\n"                                  // opcode=0x586a0c7b
         "fminu.pi f20, f25, f20\n"                            // opcode=0x2f4caa7b
         "fsrli.pi f30, f28, 23\n"                             // opcode=0x4f7e5f7b
         "fltm.pi m6, f27, f29\n"                              // opcode=0x3fdd837b
         "fsrli.pi f30, f27, 20\n"                             // opcode=0x4f4ddf7b
         "cubesgntc.ps f23, f23, f23\n"                        // opcode=0x897bbbfb
         "cubesgnsc.ps f26, f24, f29\n"                        // opcode=0x89dc2d7b
         "flt.pi f30, f25, f25\n"                              // opcode=0xa79c9f7b
         "fsetm.pi m3, f22\n"                                  // opcode=0xe00b51fb
         "fsat8.pi f29, f25\n"                                 // opcode=0x060cbefb
         "fsgnj.s f21, f22, f31\n"                             // opcode=0x21fb0ad3
         "fsgnjx.s f21, f28, f28\n"                            // opcode=0x21ce2ad3
         "fminu.pi f26, f28, f20\n"                            // opcode=0x2f4e2d7b
         "frcp.ps f22, f26\n"                                  // opcode=0x587d0b7b
         "cubesgnsc.ps f28, f29, f24\n"                        // opcode=0x898eae7b
         "maskpopcz x23, m2\n"                                 // opcode=0x54010bfb
         "fadd.ps f25, f27, f26, dyn\n"                        // opcode=0x01adfcfb
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         //"flq2 f31, 0 (x25)\n"                                 // opcode=0xceccdf87
         "maskxor m5, m0, m4\n"                                // opcode=0x664042fb
         "faddi.pi f24, f31, 94\n"                            // opcode=0x952f8c3f
         "fbcx.ps f31, x25\n"                                  // opcode=0x000cbf8b
         "feq.s x27, f27, f26\n"                               // opcode=0xa1adadd3
         "faddi.pi f29, f20, 32\n"                            // opcode=0xd40a0ebf
         "fmadd.s f23, f31, f26, f20, dyn\n"                   // opcode=0xa1affbc3
         "fsgnj.ps f21, f28, f29\n"                            // opcode=0x21de0afb
         "fltu.pi f21, f25, f21\n"                             // opcode=0xa75cbafb
         "fxor.pi f20, f28, f31\n"                             // opcode=0x07fe4a7b
         "fnmsub.s f28, f21, f25, f27, dyn\n"                  // opcode=0xd99afe4b
         //"fbc.ps f29, 98(x27)\n"                               // opcode=0x7e2d8e8b
         "fmv.x.s x25, f21\n"                                  // opcode=0xe00a8cd3
         "fsgnj.ps f22, f20, f24\n"                            // opcode=0x218a0b7b
         "fmin.s f24, f30, f25\n"                              // opcode=0x299f0c53
         "fltm.ps m7, f21, f23\n"                              // opcode=0xa17ad3fb
         "fnmadd.ps f23, f29, f22, f26, dyn\n"                 // opcode=0xd76efbdb
         "fandi.pi f22, f24, 63\n"                            // opcode=0xbdbc1b3f
         "fmaxu.pi f21, f26, f23\n"                            // opcode=0x2f7d3afb
         "fandi.pi f22, f27, 142\n"                            // opcode=0x24ed9b3f
         "fmv.s.x f30, x25\n"                                  // opcode=0xf00c8f53
         "fle.pi f20, f24, f25\n"                              // opcode=0xa79c0a7b
         "feqm.ps m6, f21, f23\n"                              // opcode=0xa17ae37b
         "maskpopcz x29, m5\n"                                 // opcode=0x54028efb
         "fnot.pi f25, f31\n"                                  // opcode=0x060facfb
         "fsll.pi f26, f21, f31\n"                             // opcode=0x07fa9d7b
         "fsra.pi f21, f21, f25\n"                             // opcode=0x0f9adafb
         "fmulhu.pi f23, f29, f29\n"                           // opcode=0x17de9bfb
         "cubesgnsc.ps f30, f24, f31\n"                        // opcode=0x89fc2f7b
         "fsrli.pi f22, f20, 20\n"                             // opcode=0x4f4a5b7b
         "fmul.ps f26, f30, f20, dyn\n"                        // opcode=0x114f7d7b
         "fsgnjx.ps f29, f30, f21\n"                           // opcode=0x215f2efb
         "fnmadd.s f29, f28, f27, f29, dyn\n"                  // opcode=0xe9be7ecf
         "fmulh.pi f24, f28, f30\n"                            // opcode=0x17ee2c7b
         "fxor.pi f23, f27, f28\n"                             // opcode=0x07cdcbfb
         "fnmadd.ps f24, f29, f28, f25, dyn\n"                 // opcode=0xcfcefc5b
         "fmadd.s f25, f21, f20, f22, dyn\n"                   // opcode=0xb14afcc3
         "fmvs.x.ps x20, f25, 4\n"                             // opcode=0xe04caa7b
         "cubefaceidx.ps f27, f25, f28\n"                      // opcode=0x89cc9dfb
         "fmul.ps f30, f24, f28, dyn\n"                        // opcode=0x11cc7f7b
         "fandi.pi f31, f30, 455\n"                            // opcode=0x747f1fbf
         "fltm.ps m2, f23, f30\n"                              // opcode=0xa1ebd17b
         "faddi.pi f26, f24, 7\n"                            // opcode=0xe4bc0d3f
         "fsgnj.s f22, f31, f28\n"                             // opcode=0x21cf8b53
         "fsrli.pi f30, f28, 30\n"                             // opcode=0x4fee5f7b
         "cubefaceidx.ps f26, f27, f26\n"                      // opcode=0x89ad9d7b
         "fand.pi f27, f23, f21\n"                             // opcode=0x075bfdfb
         "fmvz.x.ps x21, f23, 4\n"                             // opcode=0xe04b8afb
         "for.pi f24, f23, f27\n"                              // opcode=0x07bbec7b
         "fmax.pi f25, f31, f23\n"                             // opcode=0x2f7f9cfb
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fmin.pi f26, f29, f30\n"                             // opcode=0x2fee8d7b
         "faddi.pi f28, f22, 137\n"                            // opcode=0x249b0e3f
         "fslli.pi f31, f31, 1\n"                              // opcode=0x4e1f9ffb
         "fmulh.pi f23, f23, f26\n"                            // opcode=0x17ababfb
         "fpackreph.pi f20, f23\n"                             // opcode=0x260b9a7b
         "for.pi f25, f30, f29\n"                              // opcode=0x07df6cfb
         "fmin.pi f20, f30, f26\n"                             // opcode=0x2faf0a7b
         "maskpopc x23, m2\n"                                  // opcode=0x52010bfb
         "fmulhu.pi f30, f25, f29\n"                           // opcode=0x17dc9f7b
         "fnot.pi f27, f20\n"                                  // opcode=0x060a2dfb
         "frcp.ps f22, f23\n"                                  // opcode=0x587b8b7b
         "fsub.pi f29, f28, f28\n"                             // opcode=0x0fce0efb
         "fnot.pi f25, f25\n"                                  // opcode=0x060cacfb
         "for.pi f22, f26, f26\n"                              // opcode=0x07ad6b7b
         "fltu.pi f28, f24, f31\n"                             // opcode=0xa7fc3e7b
         "frcp.ps f30, f20\n"                                  // opcode=0x587a0f7b
         //"fsq2 f23, 0 (x30)\n"                                 // opcode=0x1d7f5ea7
         "fmul.s f31, f22, f28, dyn\n"                         // opcode=0x11cb7fd3
         "ffrc.ps f20, f28\n"                                  // opcode=0x582e0a7b
         "fxor.pi f24, f21, f22\n"                             // opcode=0x076acc7b
         "fmulhu.pi f27, f22, f24\n"                           // opcode=0x178b1dfb
         "fadd.pi f23, f24, f24\n"                             // opcode=0x078c0bfb
         "fmvz.x.ps x27, f21, 2\n"                             // opcode=0xe02a8dfb
         //"fsq2 f31, 0 (x22)\n"                                 // opcode=0x93fb5da7
         "fmsub.ps f29, f22, f27, f28, dyn\n"                  // opcode=0xe3bb7edb
         "for.pi f25, f28, f28\n"                              // opcode=0x07ce6cfb
         "fmax.ps f22, f30, f26\n"                             // opcode=0x29af1b7b
         "cubeface.ps f24, f30, f20\n"                         // opcode=0x894f0c7b
         "maskpopcz x27, m2\n"                                 // opcode=0x54010dfb
         "maskor m5, m0, m6\n"                                 // opcode=0x666062fb
         "fsgnjx.s f30, f26, f27\n"                            // opcode=0x21bd2f53
         "fsgnj.ps f31, f20, f26\n"                            // opcode=0x21aa0ffb
         "fsgnjn.ps f22, f20, f24\n"                           // opcode=0x218a1b7b
         "fcmovm.ps f23, f26, f22\n"                           // opcode=0x016d0bf7
         "fnmsub.s f22, f22, f28, f21, dyn\n"                  // opcode=0xa9cb7b4b
         "fsrai.pi f23, f24, 13\n"                             // opcode=0x4edc7bfb
         "ffrc.ps f23, f21\n"                                  // opcode=0x582a8bfb
         //RTLMIN-5479"fsin.ps f26, f23\n"                                  // opcode=0x586b8d7b
         "fsub.s f29, f28, f20, dyn\n"                         // opcode=0x094e7ed3
         "maskpopc x29, m4\n"                                  // opcode=0x52020efb
         "fcmovm.ps f23, f21, f31\n"                           // opcode=0x01fa8bf7
         "fcmovm.ps f30, f23, f22\n"                           // opcode=0x016b8f77
         //"fsq2 f30, 0 (x26)\n"                                 // opcode=0xafed5127
         "fmin.s f30, f30, f25\n"                              // opcode=0x299f0f53
         "masknot m6, m2\n"                                    // opcode=0x6601237b
         "fmsub.ps f25, f31, f23, f25, dyn\n"                  // opcode=0xcb7ffcdb
         "fmax.ps f22, f23, f28\n"                             // opcode=0x29cb9b7b
         //"flq2 f24, 0 (x22)\n"                                 // opcode=0x1a8b5c07
         "feq.pi f22, f22, f21\n"                              // opcode=0xa75b2b7b
         "fsub.pi f21, f24, f22\n"                             // opcode=0x0f6c0afb
         "li x2, 0xf0f0\n"
         "mova.m.x x5\n"
         "feq.pi f26, f26, f25\n"                              // opcode=0xa79d2d7b
         "ffrc.ps f31, f27\n"                                  // opcode=0x582d8ffb
         "fsrli.pi f22, f21, 3\n"                              // opcode=0x4e3adb7b
         "fround.ps f30, f31, dyn\n"                           // opcode=0x581fff7b
         "cubesgnsc.ps f22, f23, f25\n"                        // opcode=0x899bab7b
         "fnmsub.s f30, f20, f20, f26, dyn\n"                  // opcode=0xd14a7f4b
         "fmadd.ps f22, f20, f31, f31, dyn\n"                  // opcode=0xf9fa7b5b
         "fle.s x31, f29, f25\n"                               // opcode=0xa19e8fd3
         "fmul.pi f23, f24, f30\n"                             // opcode=0x17ec0bfb
         "fmv.x.s x22, f30\n"                                  // opcode=0xe00f0b53
         "fsrl.pi f25, f20, f20\n"                             // opcode=0x074a5cfb
         "fmul.s f27, f21, f31, dyn\n"                         // opcode=0x11fafdd3
         //"fbc.ps f25, 66(x25)\n"                               // opcode=0x442c8c8b
         "fltu.pi f22, f31, f27\n"                             // opcode=0xa7bfbb7b
         "fmvz.x.ps x23, f25, 4\n"                             // opcode=0xe04c8bfb
         "fmax.ps f24, f25, f23\n"                             // opcode=0x297c9c7b
         "fbci.ps f30, 72164\n"                                // opcode=0x119e4f1f
         "fadd.s f30, f25, f23, dyn\n"                         // opcode=0x017cff53
         "fsll.pi f28, f31, f29\n"                             // opcode=0x07df9e7b
         "fnmadd.s f28, f26, f26, f29, dyn\n"                  // opcode=0xe9ad7e4f
         "fsgnjn.ps f23, f27, f21\n"                           // opcode=0x215d9bfb
         "fbci.pi f20, 798355\n"                               // opcode=0xc2e93a5f
         "fmax.ps f28, f30, f22\n"                             // opcode=0x296f1e7b
         "faddi.pi f29, f31, 52\n"                            // opcode=0xd54f8ebf
         //RTLMIN-5479"frsq.ps f20, f30\n"                                  // opcode=0x588f0a7b
         "fmax.ps f29, f23, f27\n"                             // opcode=0x29bb9efb
         "fandi.pi f29, f23, 33\n"                            // opcode=0xec5b9ebf
         "fnmadd.ps f30, f21, f20, f27, dyn\n"                 // opcode=0xdf4aff5b
         //"flq2 f20, 0 (x29)\n"                                 // opcode=0xf21eda07
         "cubeface.ps f23, f22, f27\n"                         // opcode=0x89bb0bfb
         "faddi.pi f24, f21, 43\n"                            // opcode=0xecfa8c3f
         "fmadd.s f21, f29, f31, f26, dyn\n"                   // opcode=0xd1fefac3
         "feq.ps f31, f27, f23\n"                              // opcode=0xa17daffb
         "fandi.pi f27, f23, 253\n"                            // opcode=0x3ddb9dbf
         "fle.s x29, f24, f25\n"                               // opcode=0xa19c0ed3
         "flog.ps f24, f27\n"                                  // opcode=0x583d8c7b
         "fbcx.ps f26, x30\n"                                  // opcode=0x000f3d0b
         "fmsub.ps f29, f26, f24, f21, dyn\n"                  // opcode=0xab8d7edb
         "fmin.ps f21, f20, f27\n"                             // opcode=0x29ba0afb
         "fmul.pi f29, f31, f28\n"                             // opcode=0x17cf8efb
         "fpackreph.pi f21, f24\n"                             // opcode=0x260c1afb
         "fmsub.s f26, f28, f27, f27, dyn\n"                   // opcode=0xd9be7d47
         "fsub.pi f20, f22, f22\n"                             // opcode=0x0f6b0a7b
         "fmul.ps f30, f28, f28, rdn\n"                        // opcode=0x11ce2f7b
         "fsetm.pi m0, f29\n"                                  // opcode=0xe00ed07b
         "fsll.pi f25, f31, f20\n"                             // opcode=0x074f9cfb
         "fltm.pi m5, f28, f30\n"                              // opcode=0x3fee02fb
         "fxor.pi f21, f22, f30\n"                             // opcode=0x07eb4afb
         "mova.m.x x29\n"                                      // opcode=0xd60e907b
         "maskor m5, m6, m6\n"                                 // opcode=0x666362fb
         "li x2, 3051\n"
         "mova.m.x x5\n"
         "fltu.pi f26, f29, f28\n"                             // opcode=0xa7cebd7b
         "fsub.pi f24, f25, f29\n"                             // opcode=0x0fdc8c7b
         "fsat8.pi f28, f31\n"                                 // opcode=0x060fbe7b
         "fslli.pi f30, f23, 10\n"                             // opcode=0x4eab9f7b
         "cubeface.ps f20, f29, f28\n"                         // opcode=0x89ce8a7b
         "mova.m.x x25\n"                                      // opcode=0xd60c907b
         "flt.pi f28, f29, f26\n"                              // opcode=0xa7ae9e7b
         //"fsq2 f21, 0 (x30)\n"                                 // opcode=0xe55f5da7
         "maskxor m7, m3, m4\n"                                // opcode=0x6641c3fb
         "fnmadd.s f23, f21, f27, f27, dyn\n"                  // opcode=0xd9bafbcf
         "fmaxu.pi f31, f26, f30\n"                            // opcode=0x2fed3ffb
         "fand.pi f29, f21, f21\n"                             // opcode=0x075afefb
         "fandi.pi f31, f23, 183\n"                            // opcode=0x2d7b9fbf
         "fsrl.pi f27, f26, f31\n"                             // opcode=0x07fd5dfb
         //"flq2 f25, 0 (x25)\n"                                 // opcode=0x34bcdc87
         "fbci.pi f21, 523508\n"                               // opcode=0x7fcf4adf
         //"flq2 f23, 0 (x27)\n"                                 // opcode=0x6adddb87
         //"fbc.ps f21, 22(x28)\n"                               // opcode=0x616e0a8b
         "fmsub.ps f23, f21, f21, f29, dyn\n"                  // opcode=0xeb5afbdb
         "fsatu8.pi f23, f28\n"                                // opcode=0x061e3bfb
         "fpackrepb.pi f30, f29\n"                             // opcode=0x260e8f7b
         "fle.s x22, f28, f22\n"                               // opcode=0xa16e0b53
         "fmadd.ps f23, f31, f26, f31, dyn\n"                  // opcode=0xf9affbdb
         "fsgnjx.ps f24, f24, f22\n"                           // opcode=0x216c2c7b
         "fsrli.pi f26, f22, 4\n"                              // opcode=0x4e4b5d7b
         "feq.pi f22, f26, f26\n"                              // opcode=0xa7ad2b7b
         "fsrai.pi f30, f27, 28\n"                             // opcode=0x4fcdff7b
         "fsatu8.pi f20, f24\n"                                // opcode=0x061c3a7b
         "for.pi f28, f31, f26\n"                              // opcode=0x07afee7b
         "fnmadd.ps f30, f30, f23, f23, dyn\n"                 // opcode=0xbf7f7f5b
         "ffrc.ps f23, f28\n"                                  // opcode=0x582e0bfb
         "cubesgnsc.ps f30, f31, f25\n"                        // opcode=0x899faf7b
         "fmvz.x.ps x26, f26, 2\n"                             // opcode=0xe02d0d7b
         "fmv.s.x f30, x29\n"                                  // opcode=0xf00e8f53
         "maskpopc x24, m2\n"                                  // opcode=0x52010c7b
         "feqm.ps m0, f25, f20\n"                              // opcode=0xa14ce07b
         "fsll.pi f26, f30, f29\n"                             // opcode=0x07df1d7b
         "cubeface.ps f26, f21, f26\n"                         // opcode=0x89aa8d7b
         "fmv.x.s x21, f30\n"                                  // opcode=0xe00f0ad3
         "fpackrepb.pi f23, f20\n"                             // opcode=0x260a0bfb
         "maskand m0, m0, m3\n"                                // opcode=0x6630707b
         "fsgnjn.ps f22, f28, f23\n"                           // opcode=0x217e1b7b
         "fpackrepb.pi f31, f25\n"                             // opcode=0x260c8ffb
         "fsgnjx.ps f30, f20, f30\n"                           // opcode=0x21ea2f7b
         "fsat8.pi f20, f24\n"                                 // opcode=0x060c3a7b
         "fle.ps f22, f21, f21\n"                              // opcode=0xa15a8b7b
         "flog.ps f25, f26\n"                                  // opcode=0x583d0cfb
         "fround.ps f28, f20, dyn\n"                           // opcode=0x581a7e7b
         "fbcx.ps f22, x31\n"                                  // opcode=0x000fbb0b
         "faddi.pi f23, f22, 229\n"                            // opcode=0x3c5b0bbf
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fmv.x.s x20, f26\n"                                  // opcode=0xe00d0a53
         "fnot.pi f29, f31\n"                                  // opcode=0x060faefb
         "frcp.ps f26, f29\n"                                  // opcode=0x587e8d7b
         "fsrli.pi f22, f29, 29\n"                             // opcode=0x4fdedb7b
         "fadd.s f31, f20, f26, dyn\n"                         // opcode=0x01aa7fd3
         "fsub.s f23, f22, f28, dyn\n"                         // opcode=0x09cb7bd3
         "fmadd.ps f22, f25, f20, f29, dyn\n"                  // opcode=0xe94cfb5b
         "fcmovm.ps f22, f31, f26\n"                           // opcode=0x01af8b77
         "flt.ps f25, f22, f28\n"                              // opcode=0xa1cb1cfb
         "fsgnjx.ps f20, f24, f20\n"                           // opcode=0x214c2a7b
         "fsll.pi f25, f22, f20\n"                             // opcode=0x074b1cfb
         "fpackreph.pi f22, f26\n"                             // opcode=0x260d1b7b
         //RTLMIN-5479"frsq.ps f31, f29\n"                                  // opcode=0x588e8ffb
         "mova.x.m x22\n"                                      // opcode=0xd6000b7b
         "fmin.pi f20, f24, f21\n"                             // opcode=0x2f5c0a7b
         "fclass.ps f30, f26\n"                                // opcode=0xe00d1f7b
         "feq.pi f29, f29, f28\n"                              // opcode=0xa7ceaefb
         "fsgnj.s f28, f21, f28\n"                             // opcode=0x21ca8e53
         "fltm.pi m3, f24, f22\n"                              // opcode=0x3f6c01fb
         "fltu.pi f26, f30, f22\n"                             // opcode=0xa76f3d7b
         "flem.ps m2, f23, f23\n"                              // opcode=0xa17bc17b
         "fsgnj.ps f31, f31, f23\n"                            // opcode=0x217f8ffb
         "flog.ps f31, f20\n"                                  // opcode=0x583a0ffb
         "fmin.ps f28, f25, f23\n"                             // opcode=0x297c8e7b
         "fmv.s.x f29, x29\n"                                  // opcode=0xf00e8ed3
         "fsub.ps f21, f30, f27, dyn\n"                        // opcode=0x09bf7afb
         "fbcx.ps f25, x22\n"                                  // opcode=0x000b3c8b
         "fnmsub.s f26, f21, f30, f29, dyn\n"                  // opcode=0xe9eafd4b
         "fltu.pi f26, f31, f22\n"                             // opcode=0xa76fbd7b
         "cubefaceidx.ps f22, f22, f26\n"                      // opcode=0x89ab1b7b
         "fsub.ps f23, f28, f29, dyn\n"                        // opcode=0x09de7bfb
         "fmadd.s f31, f21, f27, f24, dyn\n"                   // opcode=0xc1baffc3
         "maskxor m0, m5, m0\n"                                // opcode=0x6602c07b
         "fsetm.pi m3, f22\n"                                  // opcode=0xe00b51fb
         "fsgnjx.s f28, f20, f31\n"                            // opcode=0x21fa2e53
         "fmax.s f26, f26, f27\n"                              // opcode=0x29bd1d53
         "fsgnjx.s f25, f20, f24\n"                            // opcode=0x218a2cd3
         "fsrl.pi f26, f22, f23\n"                             // opcode=0x077b5d7b
         "fsgnjx.s f29, f25, f24\n"                            // opcode=0x218caed3
         "masknot m0, m5\n"                                    // opcode=0x6602a07b
         "fnmsub.s f21, f24, f28, f25, dyn\n"                  // opcode=0xc9cc7acb
         "fcmov.ps f27, f24, f24, f23\n"                       // opcode=0xbd8c2dbf
         "fminu.pi f28, f27, f24\n"                            // opcode=0x2f8dae7b
         "fbci.pi f25, 570391\n"                               // opcode=0x8b417cdf
         "flog.ps f21, f27\n"                                  // opcode=0x583d8afb
         "fsub.ps f27, f21, f26, dyn\n"                        // opcode=0x09aafdfb
         "fmadd.s f27, f24, f28, f28, dyn\n"                   // opcode=0xe1cc7dc3
         "fmul.s f20, f30, f20, dyn\n"                         // opcode=0x114f7a53
         "fmin.pi f20, f22, f27\n"                             // opcode=0x2fbb0a7b
         "fbci.pi f28, 163982\n"                               // opcode=0x2808ee5f
         "li x2, 1546\n"
         "mova.m.x x5\n"
         "feq.ps f28, f25, f29\n"                              // opcode=0xa1dcae7b
         "maskpopc x21, m0\n"                                  // opcode=0x52000afb
         "fbci.ps f23, 916189\n"                               // opcode=0xdfaddb9f
         //"fbc.ps f27, 18(x22)\n"                               // opcode=0xe92b0d8b
         "ffrc.ps f29, f30\n"                                  // opcode=0x582f0efb
         "fsra.pi f25, f30, f26\n"                             // opcode=0x0faf5cfb
         "feqm.ps m1, f21, f28\n"                              // opcode=0xa1cae0fb
         "feqm.ps m1, f20, f29\n"                              // opcode=0xa1da60fb
         "fnmsub.ps f26, f27, f31, f26, dyn\n"                 // opcode=0xd5fdfd5b
         "fltm.ps m1, f29, f27\n"                              // opcode=0xa1bed0fb
         "fmaxu.pi f31, f26, f20\n"                            // opcode=0x2f4d3ffb
         "flog.ps f28, f23\n"                                  // opcode=0x583b8e7b
         "fsetm.pi m3, f24\n"                                  // opcode=0xe00c51fb
         "fsub.s f26, f28, f30, dyn\n"                         // opcode=0x09ee7d53
         //"fbc.ps f31, 109(x23)\n"                              // opcode=0x46db8f8b
         "fsgnj.ps f28, f27, f24\n"                            // opcode=0x218d8e7b
         "cubesgnsc.ps f28, f22, f28\n"                        // opcode=0x89cb2e7b
         "fclass.s x24, f26\n"                                 // opcode=0xe00d1c53
         "fsrl.pi f27, f30, f29\n"                             // opcode=0x07df5dfb
         "cubesgnsc.ps f26, f29, f27\n"                        // opcode=0x89bead7b
         //"fsq2 f25, 0 (x22)\n"                                 // opcode=0xa59b5c27
         "fnmadd.s f23, f20, f20, f28, rtz\n"                  // opcode=0xe14a1bcf
         "fandi.pi f27, f21, 61\n"                            // opcode=0xf41a9dbf
         //"flq2 f22, 0 (x27)\n"                                 // opcode=0xb78ddb07
         "fandi.pi f23, f20, 144\n"                            // opcode=0x250a1bbf
         "fsat8.pi f31, f26\n"                                 // opcode=0x060d3ffb
         //RTLMIN-5479"fsin.ps f20, f31\n"                                  // opcode=0x586f8a7b
         "feq.s x23, f23, f29\n"                               // opcode=0xa1dbabd3
         "masknot m7, m6\n"                                    // opcode=0x660323fb
         "fsrl.pi f25, f25, f27\n"                             // opcode=0x07bcdcfb
         "fnmadd.ps f26, f24, f21, f20, rmm\n"                 // opcode=0xa75c4d5b
         "fbci.pi f26, 812572\n"                               // opcode=0xc661cd5f
         "fsra.pi f24, f24, f22\n"                             // opcode=0x0f6c5c7b
         "fle.ps f20, f30, f27\n"                              // opcode=0xa1bf0a7b
         //RTLMIN-5479"frsq.ps f20, f20\n"                                  // opcode=0x588a0a7b
         "fmaxu.pi f24, f31, f30\n"                            // opcode=0x2fefbc7b
         "cubesgntc.ps f23, f22, f23\n"                        // opcode=0x897b3bfb
         "frcp.ps f25, f29\n"                                  // opcode=0x587e8cfb
         "frcp.ps f25, f27\n"                                  // opcode=0x587d8cfb
         "fsgnjn.ps f28, f22, f25\n"                           // opcode=0x219b1e7b
         "fsgnj.ps f23, f28, f31\n"                            // opcode=0x21fe0bfb
         "fmin.s f30, f31, f25\n"                              // opcode=0x299f8f53
         "fltm.pi m3, f22, f27\n"                              // opcode=0x3fbb01fb
         "fmulhu.pi f28, f20, f28\n"                           // opcode=0x17ca1e7b
         "fsra.pi f21, f27, f27\n"                             // opcode=0x0fbddafb
         "fmadd.s f30, f24, f30, f30, dyn\n"                   // opcode=0xf1ec7f43
         "cubesgnsc.ps f23, f22, f25\n"                        // opcode=0x899b2bfb
         "masknot m1, m4\n"                                    // opcode=0x660220fb
         //"fsq2 f31, 0 (x31)\n"                                 // opcode=0x0dffdb27
         "maskxor m3, m1, m1\n"                                // opcode=0x6610c1fb
         "li x2, 0xf0f0\n"
         "mova.m.x x5\n"
         "fclass.ps f25, f31\n"                                // opcode=0xe00f9cfb
         "fle.pi f31, f27, f21\n"                              // opcode=0xa75d8ffb
         //"fsq2 f29, 0 (x31)\n"                                 // opcode=0xd3dfd0a7
         "fbci.ps f20, 124970\n"                               // opcode=0x1e82aa1f
         "fnmsub.ps f28, f22, f29, f25, dyn\n"                 // opcode=0xcddb7e5b
         "feqm.ps m7, f28, f23\n"                              // opcode=0xa17e63fb
         "fsrli.pi f24, f26, 13\n"                             // opcode=0x4edd5c7b
         "mova.m.x x26\n"                                      // opcode=0xd60d107b
         "feq.s x27, f20, f21\n"                               // opcode=0xa15a2dd3
         "maskpopcz x29, m0\n"                                 // opcode=0x54000efb
         "fadd.pi f21, f25, f27\n"                             // opcode=0x07bc8afb
         "fnot.pi f25, f27\n"                                  // opcode=0x060dacfb
         "fmin.s f23, f25, f26\n"                              // opcode=0x29ac8bd3
         "fsrli.pi f21, f29, 18\n"                             // opcode=0x4f2edafb
         "fmvz.x.ps x29, f28, 2\n"                             // opcode=0xe02e0efb
         "flt.ps f29, f23, f24\n"                              // opcode=0xa18b9efb
         "flt.pi f23, f22, f23\n"                              // opcode=0xa77b1bfb
         //RTLMIN-5479"frsq.ps f21, f31\n"                                  // opcode=0x588f8afb
         "fpackrepb.pi f21, f26\n"                             // opcode=0x260d0afb
         "fsgnjx.s f20, f25, f20\n"                            // opcode=0x214caa53
         "maskpopcz x28, m0\n"                                 // opcode=0x54000e7b
         "fmul.ps f28, f20, f22, dyn\n"                        // opcode=0x116a7e7b
         "fmin.ps f30, f20, f30\n"                             // opcode=0x29ea0f7b
         "fcmovm.ps f26, f24, f29\n"                           // opcode=0x01dc0d77
         "fmul.ps f30, f22, f22, rup\n"                        // opcode=0x116b3f7b
         "fmax.s f31, f25, f22\n"                              // opcode=0x296c9fd3
         "fmvz.x.ps x31, f29, 6\n"                             // opcode=0xe06e8ffb
         "feq.pi f20, f29, f25\n"                              // opcode=0xa79eaa7b
         "fadd.ps f29, f30, f25, dyn\n"                        // opcode=0x019f7efb
         "flem.ps m2, f28, f23\n"                              // opcode=0xa17e417b
         "fmul.s f25, f29, f29, dyn\n"                         // opcode=0x11defcd3
         "fle.s x26, f23, f21\n"                               // opcode=0xa15b8d53
         "fsub.s f31, f20, f26, dyn\n"                         // opcode=0x09aa7fd3
         "fbcx.ps f29, x29\n"                                  // opcode=0x000ebe8b
         "flt.s x22, f21, f20\n"                               // opcode=0xa14a9b53
         "fmin.ps f28, f28, f23\n"                             // opcode=0x297e0e7b
         "fsgnjn.s f29, f20, f24\n"                            // opcode=0x218a1ed3
         "fle.ps f31, f21, f28\n"                              // opcode=0xa1ca8ffb
         "fsgnj.ps f24, f28, f22\n"                            // opcode=0x216e0c7b
         //"flq2 f28, 0 (x29)\n"                                 // opcode=0x3e9ede07
         "fadd.pi f24, f26, f31\n"                             // opcode=0x07fd0c7b
         "feq.s x26, f31, f21\n"                               // opcode=0xa15fad53
         //RTLMIN-5479"frsq.ps f24, f30\n"                                  // opcode=0x588f0c7b
         "fmvz.x.ps x22, f30, 2\n"                             // opcode=0xe02f0b7b
         "flem.ps m6, f21, f21\n"                              // opcode=0xa15ac37b
         "fmin.pi f26, f29, f26\n"                             // opcode=0x2fae8d7b
         "fbci.ps f20, 276445\n"                               // opcode=0x437dda1f
         "fsgnjx.ps f20, f27, f24\n"                           // opcode=0x218daa7b
         "fcmovm.ps f27, f27, f28\n"                           // opcode=0x01cd8df7
         "fnmadd.ps f27, f20, f30, f29, rtz\n"                 // opcode=0xefea1ddb
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fbci.ps f24, 914481\n"                               // opcode=0xdf431c1f
         "fmadd.ps f26, f31, f27, f30, dyn\n"                  // opcode=0xf1bffd5b
         "fsub.ps f27, f28, f21, dyn\n"                        // opcode=0x095e7dfb
         "masknot m3, m7\n"                                    // opcode=0x6603a1fb
         "fnot.pi f26, f31\n"                                  // opcode=0x060fad7b
         "flt.ps f28, f25, f28\n"                              // opcode=0xa1cc9e7b
         "fltm.ps m0, f23, f23\n"                              // opcode=0xa17bd07b
         "fadd.ps f22, f22, f26, dyn\n"                        // opcode=0x01ab7b7b
         "mova.m.x x27\n"                                      // opcode=0xd60d907b
         "fmulh.pi f25, f30, f26\n"                            // opcode=0x17af2cfb
         //"flq2 f21, 0 (x21)\n"                                 // opcode=0xd7dada87
         "fbcx.ps f21, x27\n"                                  // opcode=0x000dba8b
         "feq.s x21, f21, f20\n"                               // opcode=0xa14aaad3
         "fmax.ps f20, f22, f21\n"                             // opcode=0x295b1a7b
         "fmadd.s f24, f23, f22, f20, dyn\n"                   // opcode=0xa16bfc43
         //RTLMIN-5479"fsin.ps f27, f30\n"                                  // opcode=0x586f0dfb
         "for.pi f28, f23, f24\n"                              // opcode=0x078bee7b
         "maskpopcz x21, m4\n"                                 // opcode=0x54020afb
         "fsgnj.s f24, f20, f31\n"                             // opcode=0x21fa0c53
         "fbcx.ps f27, x30\n"                                  // opcode=0x000f3d8b
         "masknot m3, m4\n"                                    // opcode=0x660221fb
         "cubesgntc.ps f25, f27, f26\n"                        // opcode=0x89adbcfb
         "flt.pi f27, f28, f25\n"                              // opcode=0xa79e1dfb
         "fnmsub.s f30, f22, f28, f22, dyn\n"                  // opcode=0xb1cb7f4b
         "fsgnj.ps f28, f31, f20\n"                            // opcode=0x214f8e7b
         "fclass.s x31, f28\n"                                 // opcode=0xe00e1fd3
         "fclass.s x27, f20\n"                                 // opcode=0xe00a1dd3
         "fadd.ps f26, f26, f23, dyn\n"                        // opcode=0x017d7d7b
         "fsgnjx.ps f21, f29, f25\n"                           // opcode=0x219eaafb
         "fclass.s x23, f21\n"                                 // opcode=0xe00a9bd3
         "fadd.s f26, f21, f29, dyn\n"                         // opcode=0x01dafd53
         "fnmsub.s f22, f24, f28, f24, dyn\n"                  // opcode=0xc1cc7b4b
         "fbci.ps f26, 202901\n"                               // opcode=0x31895d1f
         "fsgnjx.ps f22, f26, f29\n"                           // opcode=0x21dd2b7b
         "frcp.ps f29, f25\n"                                  // opcode=0x587c8efb
         "fbci.ps f30, 973299\n"                               // opcode=0xed9f3f1f
         "fltm.ps m7, f25, f25\n"                              // opcode=0xa19cd3fb
         "maskpopcz x26, m1\n"                                 // opcode=0x54008d7b
         "fsub.ps f27, f28, f25, dyn\n"                        // opcode=0x099e7dfb
         "fslli.pi f29, f31, 7\n"                              // opcode=0x4e7f9efb
         "fmin.s f22, f26, f25\n"                              // opcode=0x299d0b53
         "ffrc.ps f22, f27\n"                                  // opcode=0x582d8b7b
         "fsrl.pi f20, f28, f26\n"                             // opcode=0x07ae5a7b
         "fnot.pi f28, f24\n"                                  // opcode=0x060c2e7b
         "fsgnjn.s f29, f31, f24\n"                            // opcode=0x218f9ed3
         "fmvz.x.ps x30, f24, 1\n"                             // opcode=0xe01c0f7b
         "fmin.s f27, f26, f26\n"                              // opcode=0x29ad0dd3
         "fsub.s f27, f30, f29, dyn\n"                         // opcode=0x09df7dd3
         "fle.ps f26, f31, f20\n"                              // opcode=0xa14f8d7b
         "fnot.pi f31, f24\n"                                  // opcode=0x060c2ffb
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fltm.ps m2, f30, f25\n"                              // opcode=0xa19f517b
         "fle.s x31, f30, f29\n"                               // opcode=0xa1df0fd3
         "fle.ps f29, f31, f28\n"                              // opcode=0xa1cf8efb
         "fsetm.pi m0, f22\n"                                  // opcode=0xe00b507b
         "fmul.ps f26, f29, f24, dyn\n"                        // opcode=0x118efd7b
         "fsgnjx.ps f29, f27, f27\n"                           // opcode=0x21bdaefb
         "fle.ps f21, f20, f22\n"                              // opcode=0xa16a0afb
         "fsat8.pi f23, f22\n"                                 // opcode=0x060b3bfb
         "fsub.pi f26, f25, f31\n"                             // opcode=0x0ffc8d7b
         "maskpopc x20, m6\n"                                  // opcode=0x52030a7b
         "maskand m1, m5, m1\n"                                // opcode=0x6612f0fb
         "fmax.pi f28, f29, f30\n"                             // opcode=0x2fee9e7b
         "fmsub.ps f25, f21, f21, f29, dyn\n"                  // opcode=0xeb5afcdb
         "fsrai.pi f30, f21, 7\n"                              // opcode=0x4e7aff7b
         "feqm.ps m4, f26, f23\n"                              // opcode=0xa17d627b
         "fmax.s f31, f25, f28\n"                              // opcode=0x29cc9fd3
         "fsgnjn.ps f20, f23, f23\n"                           // opcode=0x217b9a7b
         "fsub.pi f27, f31, f30\n"                             // opcode=0x0fef8dfb
         "fsll.pi f31, f21, f20\n"                             // opcode=0x074a9ffb
         "fnmsub.ps f26, f24, f30, f22, dyn\n"                 // opcode=0xb5ec7d5b
         "fadd.s f31, f30, f23, dyn\n"                         // opcode=0x017f7fd3
         "fnmadd.ps f29, f23, f23, f20, dyn\n"                 // opcode=0xa77bfedb
         "fmax.s f22, f31, f30\n"                              // opcode=0x29ef9b53
         "fsat8.pi f24, f23\n"                                 // opcode=0x060bbc7b
         "fmvz.x.ps x24, f21, 6\n"                             // opcode=0xe06a8c7b
         "fmin.pi f23, f22, f27\n"                             // opcode=0x2fbb0bfb
         "fmax.ps f26, f31, f31\n"                             // opcode=0x29ff9d7b
         "feq.pi f28, f31, f27\n"                              // opcode=0xa7bfae7b
         "feq.ps f31, f20, f20\n"                              // opcode=0xa14a2ffb
         "cubesgntc.ps f23, f31, f31\n"                        // opcode=0x89ffbbfb
         "cubeface.ps f24, f24, f25\n"                         // opcode=0x899c0c7b
         "maskpopcz x26, m6\n"                                 // opcode=0x54030d7b
         "fslli.pi f29, f28, 5\n"                              // opcode=0x4e5e1efb
         //"flq2 f25, 0 (x25)\n"                                 // opcode=0x38acdc87
         "fnmsub.s f27, f20, f25, f29, dyn\n"                  // opcode=0xe99a7dcb
         "fbci.ps f27, 472212\n"                               // opcode=0x73494d9f
         "frcp.ps f29, f24\n"                                  // opcode=0x587c0efb
         "fsrai.pi f25, f26, 1\n"                              // opcode=0x4e1d7cfb
         "fltu.pi f25, f24, f20\n"                             // opcode=0xa74c3cfb
         "fsll.pi f31, f30, f31\n"                             // opcode=0x07ff1ffb
         "fsgnj.s f29, f23, f24\n"                             // opcode=0x218b8ed3
         "fminu.pi f31, f28, f21\n"                            // opcode=0x2f5e2ffb
         "fsub.ps f28, f21, f25, dyn\n"                        // opcode=0x099afe7b
         "fslli.pi f21, f25, 5\n"                              // opcode=0x4e5c9afb
         "flt.pi f25, f27, f23\n"                              // opcode=0xa77d9cfb
         "fsub.pi f24, f27, f31\n"                             // opcode=0x0ffd8c7b
         "fmax.ps f30, f26, f28\n"                             // opcode=0x29cd1f7b
         "fmax.pi f20, f25, f31\n"                             // opcode=0x2ffc9a7b
         //"fsq2 f21, 0 (x26)\n"                                 // opcode=0x615d5227
         "fsub.s f24, f24, f23, dyn\n"                         // opcode=0x097c7c53
         "li x2, 284\n"
         "mova.m.x x5\n"
         "fmin.s f22, f31, f21\n"                              // opcode=0x295f8b53
         "fmin.pi f24, f22, f21\n"                             // opcode=0x2f5b0c7b
         "maskand m6, m2, m7\n"                                // opcode=0x6671737b
         //"fsq2 f22, 0 (x26)\n"                                 // opcode=0x8b6d5ba7
         "fmvz.x.ps x22, f22, 0\n"                             // opcode=0xe00b0b7b
         "fnot.pi f21, f28\n"                                  // opcode=0x060e2afb
         "feqm.ps m0, f26, f23\n"                              // opcode=0xa17d607b
         "fsub.ps f26, f21, f31, dyn\n"                        // opcode=0x09fafd7b
         //RTLMIN-5479"frsq.ps f22, f21\n"                                  // opcode=0x588a8b7b
         "fandi.pi f22, f27, 49\n"                            // opcode=0xbcdd9b3f
         "fmv.x.s x25, f29\n"                                  // opcode=0xe00e8cd3
         "mova.m.x x23\n"                                      // opcode=0xd60b907b
         "fpackreph.pi f29, f27\n"                             // opcode=0x260d9efb
         "mova.m.x x27\n"                                      // opcode=0xd60d907b
         "cubefaceidx.ps f30, f31, f30\n"                      // opcode=0x89ef9f7b
         "fmv.x.s x23, f21\n"                                  // opcode=0xe00a8bd3
         "maskxor m5, m2, m2\n"                                // opcode=0x662142fb
         "feq.pi f28, f20, f20\n"                              // opcode=0xa74a2e7b
         "fbcx.ps f31, x28\n"                                  // opcode=0x000e3f8b
         "flt.pi f29, f24, f20\n"                              // opcode=0xa74c1efb
         "fmaxu.pi f28, f21, f30\n"                            // opcode=0x2feabe7b
         "fmsub.ps f29, f28, f26, f28, dyn\n"                  // opcode=0xe3ae7edb
         "fmsub.ps f24, f25, f31, f23, dyn\n"                  // opcode=0xbbfcfc5b
         "feqm.ps m5, f31, f21\n"                              // opcode=0xa15fe2fb
         "fsgnjx.s f23, f30, f31\n"                            // opcode=0x21ff2bd3
         "faddi.pi f29, f24, 71\n"                            // opcode=0x8dbc0ebf
         "maskor m2, m6, m7\n"                                 // opcode=0x6673617b
         "fexp.ps f24, f31\n"                                  // opcode=0x584f8c7b
         "fmax.ps f22, f30, f22\n"                             // opcode=0x296f1b7b
         "for.pi f25, f24, f25\n"                              // opcode=0x079c6cfb
         "feq.pi f24, f25, f31\n"                              // opcode=0xa7fcac7b
         "flt.s x20, f27, f20\n"                               // opcode=0xa14d9a53
         "fsrl.pi f31, f31, f23\n"                             // opcode=0x077fdffb
         "fsra.pi f24, f30, f26\n"                             // opcode=0x0faf5c7b
         "fmadd.s f28, f27, f24, f23, dyn\n"                   // opcode=0xb98dfe43
         "flog.ps f24, f25\n"                                  // opcode=0x583c8c7b
         "fsgnj.s f31, f24, f20\n"                             // opcode=0x214c0fd3
         "fadd.ps f28, f20, f31, dyn\n"                        // opcode=0x01fa7e7b
         "fmvs.x.ps x28, f24, 5\n"                             // opcode=0xe05c2e7b
         //"fsq2 f25, 0 (x30)\n"                                 // opcode=0x499f50a7
         "fbci.ps f23, 895061\n"                               // opcode=0xda855b9f
         "fsgnjx.s f22, f20, f30\n"                            // opcode=0x21ea2b53
         "ffrc.ps f24, f23\n"                                  // opcode=0x582b8c7b
         "mova.x.m x30\n"                                      // opcode=0xd6000f7b
         "cubesgntc.ps f31, f23, f28\n"                        // opcode=0x89cbbffb
         //"fbc.ps f21, 0(x30)\n"                                // opcode=0x580f0a8b
         "fadd.ps f22, f27, f21, dyn\n"                        // opcode=0x015dfb7b
         "fbcx.ps f22, x24\n"                                  // opcode=0x000c3b0b
         "fmsub.s f31, f28, f27, f31, dyn\n"                   // opcode=0xf9be7fc7
         "fsgnjn.s f28, f23, f23\n"                            // opcode=0x217b9e53
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fsub.pi f26, f20, f27\n"                             // opcode=0x0fba0d7b
         "fexp.ps f23, f21\n"                                  // opcode=0x584a8bfb
         "fmulhu.pi f30, f20, f20\n"                           // opcode=0x174a1f7b
         "flt.pi f25, f23, f26\n"                              // opcode=0xa7ab9cfb
         "feq.ps f23, f26, f26\n"                              // opcode=0xa1ad2bfb
         "fsrai.pi f21, f24, 14\n"                             // opcode=0x4eec7afb
         "fle.s x26, f29, f26\n"                               // opcode=0xa1ae8d53
         "fsra.pi f21, f27, f24\n"                             // opcode=0x0f8ddafb
         "fnmadd.ps f25, f22, f25, f23, dyn\n"                 // opcode=0xbf9b7cdb
         "fsetm.pi m3, f23\n"                                  // opcode=0xe00bd1fb
         "cubeface.ps f23, f25, f25\n"                         // opcode=0x899c8bfb
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
