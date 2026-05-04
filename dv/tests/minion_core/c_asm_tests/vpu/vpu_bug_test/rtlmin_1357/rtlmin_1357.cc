/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

   // Avoid using x2 and x8. x2 is stack pointer. Assembler does not like if we use x8 and forbid it from using x8 - it errors out
   static uint32_t m_11[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_12[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_13[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_14[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_15[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_16[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_17[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_18[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_19[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_20[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_21[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_22[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_23[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_24[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_25[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_26[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_27[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_28[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_29[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_30[256] __attribute__ ((aligned (4096))) = {};
   static uint32_t m_31[256] __attribute__ ((aligned (4096))) = {};
   volatile uint32_t operands[256] __attribute__ ((aligned (4096))) = {

      // vpu register f0
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      
      0x00000000,                      

      // vpu register f1
      0x00000100,                      
      0x00000200,                      
      0x00000300,                      
      0x00000400,                      
      0x00000500,                      
      0x00000600,                      
      0x00000700,                      
      0x00000800,                      

      // vpu register f2
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0xffc00000,    // -quiet NaN            
      0xffc00001,    // -signaling NaN        

      // vpu register f3
      0x00000000,    // zero                  
      0x3f028f5c,    // 0.51                  
      0x7fc00000,    // quiet NaN             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x80000000,    // -zero                 
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  

      // vpu register f4
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0xff800000,    // -inf                  

      // vpu register f5
      0xffc00000,    // -quiet NaN            
      0x80000000,    // -zero                 
      0xbf028f5c,    // -0.51                 
      0x7fc00001,    // signaling NaN         
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  

      // vpu register f6
      0x00000000,    // zero                  
      0x7fc00001,    // signaling NaN         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000000,    // -zero                 
      0x00000000,    // zero                  
      0x3f028f5c,    // 0.51                  

      // vpu register f7
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0xbf800000,    // -1.0                  

      // vpu register f8
      0x3f028f5c,    // 0.51                  
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        
      0x7f800000,    // inf                   
      0x7fc00000,    // quiet NaN             
      0x7f800000,    // inf                   
      0x80011111,    // -9.7958E-41           
      0x4b000000,    // 8388608.0             

      // vpu register f9
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0x7fc00001,    // signaling NaN         
      0x7fc00001,    // signaling NaN         
      0x00011111,    // 9.7958E-41            

      // vpu register f10
      0x00011111,    // 9.7958E-41            
      0x7fc00000,    // quiet NaN             
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 

      // vpu register f11
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             

      // vpu register f12
      0x80011111,    // -9.7958E-41           
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0xbf800000,    // -1.0                  
      0x7fc00001,    // signaling NaN         
      0x7fc00001,    // signaling NaN         
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            

      // vpu register f13
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0xff800000,    // -inf                  
      0x80011111,    // -9.7958E-41           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7f800000,    // inf                   
      0x4b8c4b40,    // 18388608.0            
      0x00000000,    // zero                  

      // vpu register f14
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x80000000,    // -zero                 
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         

      // vpu register f15
      0x7fc00001,    // signaling NaN         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             
      0xffc00000,    // -quiet NaN            
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b8c4b40,    // 18388608.0            

      // vpu register f16
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0xffc00001,    // -signaling NaN        
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f17
      0x4b000000,    // 8388608.0             
      0xffc00000,    // -quiet NaN            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7fc00001,    // signaling NaN         
      0x7fc00000,    // quiet NaN             
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0xffc00000,    // -quiet NaN            

      // vpu register f18
      0x7fc00001,    // signaling NaN         
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            

      // vpu register f19
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x7fc00001,    // signaling NaN         
      0xffc00000,    // -quiet NaN            
      0xffc00001,    // -signaling NaN        

      // vpu register f20
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0x80011111,    // -9.7958E-41           
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0xffc00001,    // -signaling NaN        
      0x7fc00001,    // signaling NaN         

      // vpu register f21
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 
      0x80000000,    // -zero                 
      0x3f800000,    // 1.0                   

      // vpu register f22
      0x4b8c4b40,    // 18388608.0            
      0x7f800000,    // inf                   
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x7fc00000,    // quiet NaN             
      0xffc00001,    // -signaling NaN        
      0xff800000,    // -inf                  
      0x3f800000,    // 1.0                   

      // vpu register f23
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         

      // vpu register f24
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  

      // vpu register f25
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            

      // vpu register f26
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000000,    // zero                  
      0xffc00000,    // -quiet NaN            
      0x7fc00000,    // quiet NaN             
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0x7f800000,    // inf                   
      0x00000000,    // zero                  

      // vpu register f27
      0xbf800000,    // -1.0                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0x007fffff,    // 1.1754942E-38         

      // vpu register f28
      0xff800000,    // -inf                  
      0x7f800000,    // inf                   
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x7f800000,    // inf                   
      0x3f028f5c,    // 0.51                  

      // vpu register f29
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  
      0x4b8c4b40,    // 18388608.0            
      0x00000000,    // zero                  
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000000,    // zero                  
      0xffc00000,    // -quiet NaN            

      // vpu register f30
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   

      // vpu register f31
      0xcb8c4b40,    // -18388608.0           
      0x7fc00000,    // quiet NaN             
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0xbf028f5c,    // -0.51                 
      0x80000000,    // -zero                 
      0x3f800000     // 1.0                   
   };

   register uint64_t p_x11 asm("x11") = (uint64_t) m_11; 
   register uint64_t p_x12 asm("x12") = (uint64_t) m_12; 
   register uint64_t p_x13 asm("x13") = (uint64_t) m_13; 
   register uint64_t p_x14 asm("x14") = (uint64_t) m_14; 
   register uint64_t p_x15 asm("x15") = (uint64_t) m_15; 
   register uint64_t p_x16 asm("x16") = (uint64_t) m_16; 
   register uint64_t p_x17 asm("x17") = (uint64_t) m_17; 
   register uint64_t p_x18 asm("x18") = (uint64_t) m_18; 
   register uint64_t p_x19 asm("x19") = (uint64_t) m_19; 
   register uint64_t p_x20 asm("x20") = (uint64_t) m_20; 
   register uint64_t p_x21 asm("x21") = (uint64_t) m_21; 
   register uint64_t p_x22 asm("x22") = (uint64_t) m_22; 
   register uint64_t p_x23 asm("x23") = (uint64_t) m_23; 
   register uint64_t p_x24 asm("x24") = (uint64_t) m_24; 
   register uint64_t p_x25 asm("x25") = (uint64_t) m_25; 
   register uint64_t p_x26 asm("x26") = (uint64_t) m_26; 
   register uint64_t p_x27 asm("x27") = (uint64_t) m_27; 
   register uint64_t p_x28 asm("x28") = (uint64_t) m_28; 
   register uint64_t p_x29 asm("x29") = (uint64_t) m_29; 
   register uint64_t p_x30 asm("x30") = (uint64_t) m_30; 
   register uint64_t p_x31 asm("x31") = (uint64_t) m_31; 
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
         "fsw.ps f0, 0 (x11)\n"
         "fsw.ps f1, 32 (x11)\n"
         "fsw.ps f2, 64 (x11)\n"
         "fsw.ps f3, 96 (x11)\n"
         "fsw.ps f4, 128 (x11)\n"
         "fsw.ps f5, 160 (x11)\n"
         "fsw.ps f6, 192 (x11)\n"
         "fsw.ps f7, 224 (x11)\n"
         "fsw.ps f8, 256 (x11)\n"
         "fsw.ps f9, 288 (x11)\n"
         "fsw.ps f10, 320 (x11)\n"
         "fsw.ps f11, 352 (x11)\n"
         "fsw.ps f12, 384 (x11)\n"
         "fsw.ps f13, 416 (x11)\n"
         "fsw.ps f14, 448 (x11)\n"
         "fsw.ps f15, 480 (x11)\n"
         "fsw.ps f16, 512 (x11)\n"
         "fsw.ps f17, 544 (x11)\n"
         "fsw.ps f18, 576 (x11)\n"
         "fsw.ps f19, 608 (x11)\n"
         "fsw.ps f20, 640 (x11)\n"
         "fsw.ps f21, 672 (x11)\n"
         "fsw.ps f22, 704 (x11)\n"
         "fsw.ps f23, 736 (x11)\n"
         "fsw.ps f24, 768 (x11)\n"
         "fsw.ps f25, 800 (x11)\n"
         "fsw.ps f26, 832 (x11)\n"
         "fsw.ps f27, 864 (x11)\n"
         "fsw.ps f28, 896 (x11)\n"
         "fsw.ps f29, 928 (x11)\n"
         "fsw.ps f30, 960 (x11)\n"
         "fsw.ps f31, 992 (x11)\n"
         "fsw.ps f0, 0 (x12)\n"
         "fsw.ps f1, 32 (x12)\n"
         "fsw.ps f2, 64 (x12)\n"
         "fsw.ps f3, 96 (x12)\n"
         "fsw.ps f4, 128 (x12)\n"
         "fsw.ps f5, 160 (x12)\n"
         "fsw.ps f6, 192 (x12)\n"
         "fsw.ps f7, 224 (x12)\n"
         "fsw.ps f8, 256 (x12)\n"
         "fsw.ps f9, 288 (x12)\n"
         "fsw.ps f10, 320 (x12)\n"
         "fsw.ps f11, 352 (x12)\n"
         "fsw.ps f12, 384 (x12)\n"
         "fsw.ps f13, 416 (x12)\n"
         "fsw.ps f14, 448 (x12)\n"
         "fsw.ps f15, 480 (x12)\n"
         "fsw.ps f16, 512 (x12)\n"
         "fsw.ps f17, 544 (x12)\n"
         "fsw.ps f18, 576 (x12)\n"
         "fsw.ps f19, 608 (x12)\n"
         "fsw.ps f20, 640 (x12)\n"
         "fsw.ps f21, 672 (x12)\n"
         "fsw.ps f22, 704 (x12)\n"
         "fsw.ps f23, 736 (x12)\n"
         "fsw.ps f24, 768 (x12)\n"
         "fsw.ps f25, 800 (x12)\n"
         "fsw.ps f26, 832 (x12)\n"
         "fsw.ps f27, 864 (x12)\n"
         "fsw.ps f28, 896 (x12)\n"
         "fsw.ps f29, 928 (x12)\n"
         "fsw.ps f30, 960 (x12)\n"
         "fsw.ps f31, 992 (x12)\n"
         "fsw.ps f0, 0 (x13)\n"
         "fsw.ps f1, 32 (x13)\n"
         "fsw.ps f2, 64 (x13)\n"
         "fsw.ps f3, 96 (x13)\n"
         "fsw.ps f4, 128 (x13)\n"
         "fsw.ps f5, 160 (x13)\n"
         "fsw.ps f6, 192 (x13)\n"
         "fsw.ps f7, 224 (x13)\n"
         "fsw.ps f8, 256 (x13)\n"
         "fsw.ps f9, 288 (x13)\n"
         "fsw.ps f10, 320 (x13)\n"
         "fsw.ps f11, 352 (x13)\n"
         "fsw.ps f12, 384 (x13)\n"
         "fsw.ps f13, 416 (x13)\n"
         "fsw.ps f14, 448 (x13)\n"
         "fsw.ps f15, 480 (x13)\n"
         "fsw.ps f16, 512 (x13)\n"
         "fsw.ps f17, 544 (x13)\n"
         "fsw.ps f18, 576 (x13)\n"
         "fsw.ps f19, 608 (x13)\n"
         "fsw.ps f20, 640 (x13)\n"
         "fsw.ps f21, 672 (x13)\n"
         "fsw.ps f22, 704 (x13)\n"
         "fsw.ps f23, 736 (x13)\n"
         "fsw.ps f24, 768 (x13)\n"
         "fsw.ps f25, 800 (x13)\n"
         "fsw.ps f26, 832 (x13)\n"
         "fsw.ps f27, 864 (x13)\n"
         "fsw.ps f28, 896 (x13)\n"
         "fsw.ps f29, 928 (x13)\n"
         "fsw.ps f30, 960 (x13)\n"
         "fsw.ps f31, 992 (x13)\n"
         "fsw.ps f0, 0 (x14)\n"
         "fsw.ps f1, 32 (x14)\n"
         "fsw.ps f2, 64 (x14)\n"
         "fsw.ps f3, 96 (x14)\n"
         "fsw.ps f4, 128 (x14)\n"
         "fsw.ps f5, 160 (x14)\n"
         "fsw.ps f6, 192 (x14)\n"
         "fsw.ps f7, 224 (x14)\n"
         "fsw.ps f8, 256 (x14)\n"
         "fsw.ps f9, 288 (x14)\n"
         "fsw.ps f10, 320 (x14)\n"
         "fsw.ps f11, 352 (x14)\n"
         "fsw.ps f12, 384 (x14)\n"
         "fsw.ps f13, 416 (x14)\n"
         "fsw.ps f14, 448 (x14)\n"
         "fsw.ps f15, 480 (x14)\n"
         "fsw.ps f16, 512 (x14)\n"
         "fsw.ps f17, 544 (x14)\n"
         "fsw.ps f18, 576 (x14)\n"
         "fsw.ps f19, 608 (x14)\n"
         "fsw.ps f20, 640 (x14)\n"
         "fsw.ps f21, 672 (x14)\n"
         "fsw.ps f22, 704 (x14)\n"
         "fsw.ps f23, 736 (x14)\n"
         "fsw.ps f24, 768 (x14)\n"
         "fsw.ps f25, 800 (x14)\n"
         "fsw.ps f26, 832 (x14)\n"
         "fsw.ps f27, 864 (x14)\n"
         "fsw.ps f28, 896 (x14)\n"
         "fsw.ps f29, 928 (x14)\n"
         "fsw.ps f30, 960 (x14)\n"
         "fsw.ps f31, 992 (x14)\n"
         "fsw.ps f0, 0 (x15)\n"
         "fsw.ps f1, 32 (x15)\n"
         "fsw.ps f2, 64 (x15)\n"
         "fsw.ps f3, 96 (x15)\n"
         "fsw.ps f4, 128 (x15)\n"
         "fsw.ps f5, 160 (x15)\n"
         "fsw.ps f6, 192 (x15)\n"
         "fsw.ps f7, 224 (x15)\n"
         "fsw.ps f8, 256 (x15)\n"
         "fsw.ps f9, 288 (x15)\n"
         "fsw.ps f10, 320 (x15)\n"
         "fsw.ps f11, 352 (x15)\n"
         "fsw.ps f12, 384 (x15)\n"
         "fsw.ps f13, 416 (x15)\n"
         "fsw.ps f14, 448 (x15)\n"
         "fsw.ps f15, 480 (x15)\n"
         "fsw.ps f16, 512 (x15)\n"
         "fsw.ps f17, 544 (x15)\n"
         "fsw.ps f18, 576 (x15)\n"
         "fsw.ps f19, 608 (x15)\n"
         "fsw.ps f20, 640 (x15)\n"
         "fsw.ps f21, 672 (x15)\n"
         "fsw.ps f22, 704 (x15)\n"
         "fsw.ps f23, 736 (x15)\n"
         "fsw.ps f24, 768 (x15)\n"
         "fsw.ps f25, 800 (x15)\n"
         "fsw.ps f26, 832 (x15)\n"
         "fsw.ps f27, 864 (x15)\n"
         "fsw.ps f28, 896 (x15)\n"
         "fsw.ps f29, 928 (x15)\n"
         "fsw.ps f30, 960 (x15)\n"
         "fsw.ps f31, 992 (x15)\n"
         "fsw.ps f0, 0 (x16)\n"
         "fsw.ps f1, 32 (x16)\n"
         "fsw.ps f2, 64 (x16)\n"
         "fsw.ps f3, 96 (x16)\n"
         "fsw.ps f4, 128 (x16)\n"
         "fsw.ps f5, 160 (x16)\n"
         "fsw.ps f6, 192 (x16)\n"
         "fsw.ps f7, 224 (x16)\n"
         "fsw.ps f8, 256 (x16)\n"
         "fsw.ps f9, 288 (x16)\n"
         "fsw.ps f10, 320 (x16)\n"
         "fsw.ps f11, 352 (x16)\n"
         "fsw.ps f12, 384 (x16)\n"
         "fsw.ps f13, 416 (x16)\n"
         "fsw.ps f14, 448 (x16)\n"
         "fsw.ps f15, 480 (x16)\n"
         "fsw.ps f16, 512 (x16)\n"
         "fsw.ps f17, 544 (x16)\n"
         "fsw.ps f18, 576 (x16)\n"
         "fsw.ps f19, 608 (x16)\n"
         "fsw.ps f20, 640 (x16)\n"
         "fsw.ps f21, 672 (x16)\n"
         "fsw.ps f22, 704 (x16)\n"
         "fsw.ps f23, 736 (x16)\n"
         "fsw.ps f24, 768 (x16)\n"
         "fsw.ps f25, 800 (x16)\n"
         "fsw.ps f26, 832 (x16)\n"
         "fsw.ps f27, 864 (x16)\n"
         "fsw.ps f28, 896 (x16)\n"
         "fsw.ps f29, 928 (x16)\n"
         "fsw.ps f30, 960 (x16)\n"
         "fsw.ps f31, 992 (x16)\n"
         "fsw.ps f0, 0 (x17)\n"
         "fsw.ps f1, 32 (x17)\n"
         "fsw.ps f2, 64 (x17)\n"
         "fsw.ps f3, 96 (x17)\n"
         "fsw.ps f4, 128 (x17)\n"
         "fsw.ps f5, 160 (x17)\n"
         "fsw.ps f6, 192 (x17)\n"
         "fsw.ps f7, 224 (x17)\n"
         "fsw.ps f8, 256 (x17)\n"
         "fsw.ps f9, 288 (x17)\n"
         "fsw.ps f10, 320 (x17)\n"
         "fsw.ps f11, 352 (x17)\n"
         "fsw.ps f12, 384 (x17)\n"
         "fsw.ps f13, 416 (x17)\n"
         "fsw.ps f14, 448 (x17)\n"
         "fsw.ps f15, 480 (x17)\n"
         "fsw.ps f16, 512 (x17)\n"
         "fsw.ps f17, 544 (x17)\n"
         "fsw.ps f18, 576 (x17)\n"
         "fsw.ps f19, 608 (x17)\n"
         "fsw.ps f20, 640 (x17)\n"
         "fsw.ps f21, 672 (x17)\n"
         "fsw.ps f22, 704 (x17)\n"
         "fsw.ps f23, 736 (x17)\n"
         "fsw.ps f24, 768 (x17)\n"
         "fsw.ps f25, 800 (x17)\n"
         "fsw.ps f26, 832 (x17)\n"
         "fsw.ps f27, 864 (x17)\n"
         "fsw.ps f28, 896 (x17)\n"
         "fsw.ps f29, 928 (x17)\n"
         "fsw.ps f30, 960 (x17)\n"
         "fsw.ps f31, 992 (x17)\n"
         "fsw.ps f0, 0 (x18)\n"
         "fsw.ps f1, 32 (x18)\n"
         "fsw.ps f2, 64 (x18)\n"
         "fsw.ps f3, 96 (x18)\n"
         "fsw.ps f4, 128 (x18)\n"
         "fsw.ps f5, 160 (x18)\n"
         "fsw.ps f6, 192 (x18)\n"
         "fsw.ps f7, 224 (x18)\n"
         "fsw.ps f8, 256 (x18)\n"
         "fsw.ps f9, 288 (x18)\n"
         "fsw.ps f10, 320 (x18)\n"
         "fsw.ps f11, 352 (x18)\n"
         "fsw.ps f12, 384 (x18)\n"
         "fsw.ps f13, 416 (x18)\n"
         "fsw.ps f14, 448 (x18)\n"
         "fsw.ps f15, 480 (x18)\n"
         "fsw.ps f16, 512 (x18)\n"
         "fsw.ps f17, 544 (x18)\n"
         "fsw.ps f18, 576 (x18)\n"
         "fsw.ps f19, 608 (x18)\n"
         "fsw.ps f20, 640 (x18)\n"
         "fsw.ps f21, 672 (x18)\n"
         "fsw.ps f22, 704 (x18)\n"
         "fsw.ps f23, 736 (x18)\n"
         "fsw.ps f24, 768 (x18)\n"
         "fsw.ps f25, 800 (x18)\n"
         "fsw.ps f26, 832 (x18)\n"
         "fsw.ps f27, 864 (x18)\n"
         "fsw.ps f28, 896 (x18)\n"
         "fsw.ps f29, 928 (x18)\n"
         "fsw.ps f30, 960 (x18)\n"
         "fsw.ps f31, 992 (x18)\n"
         "fsw.ps f0, 0 (x19)\n"
         "fsw.ps f1, 32 (x19)\n"
         "fsw.ps f2, 64 (x19)\n"
         "fsw.ps f3, 96 (x19)\n"
         "fsw.ps f4, 128 (x19)\n"
         "fsw.ps f5, 160 (x19)\n"
         "fsw.ps f6, 192 (x19)\n"
         "fsw.ps f7, 224 (x19)\n"
         "fsw.ps f8, 256 (x19)\n"
         "fsw.ps f9, 288 (x19)\n"
         "fsw.ps f10, 320 (x19)\n"
         "fsw.ps f11, 352 (x19)\n"
         "fsw.ps f12, 384 (x19)\n"
         "fsw.ps f13, 416 (x19)\n"
         "fsw.ps f14, 448 (x19)\n"
         "fsw.ps f15, 480 (x19)\n"
         "fsw.ps f16, 512 (x19)\n"
         "fsw.ps f17, 544 (x19)\n"
         "fsw.ps f18, 576 (x19)\n"
         "fsw.ps f19, 608 (x19)\n"
         "fsw.ps f20, 640 (x19)\n"
         "fsw.ps f21, 672 (x19)\n"
         "fsw.ps f22, 704 (x19)\n"
         "fsw.ps f23, 736 (x19)\n"
         "fsw.ps f24, 768 (x19)\n"
         "fsw.ps f25, 800 (x19)\n"
         "fsw.ps f26, 832 (x19)\n"
         "fsw.ps f27, 864 (x19)\n"
         "fsw.ps f28, 896 (x19)\n"
         "fsw.ps f29, 928 (x19)\n"
         "fsw.ps f30, 960 (x19)\n"
         "fsw.ps f31, 992 (x19)\n"
         "fsw.ps f0, 0 (x20)\n"
         "fsw.ps f1, 32 (x20)\n"
         "fsw.ps f2, 64 (x20)\n"
         "fsw.ps f3, 96 (x20)\n"
         "fsw.ps f4, 128 (x20)\n"
         "fsw.ps f5, 160 (x20)\n"
         "fsw.ps f6, 192 (x20)\n"
         "fsw.ps f7, 224 (x20)\n"
         "fsw.ps f8, 256 (x20)\n"
         "fsw.ps f9, 288 (x20)\n"
         "fsw.ps f10, 320 (x20)\n"
         "fsw.ps f11, 352 (x20)\n"
         "fsw.ps f12, 384 (x20)\n"
         "fsw.ps f13, 416 (x20)\n"
         "fsw.ps f14, 448 (x20)\n"
         "fsw.ps f15, 480 (x20)\n"
         "fsw.ps f16, 512 (x20)\n"
         "fsw.ps f17, 544 (x20)\n"
         "fsw.ps f18, 576 (x20)\n"
         "fsw.ps f19, 608 (x20)\n"
         "fsw.ps f20, 640 (x20)\n"
         "fsw.ps f21, 672 (x20)\n"
         "fsw.ps f22, 704 (x20)\n"
         "fsw.ps f23, 736 (x20)\n"
         "fsw.ps f24, 768 (x20)\n"
         "fsw.ps f25, 800 (x20)\n"
         "fsw.ps f26, 832 (x20)\n"
         "fsw.ps f27, 864 (x20)\n"
         "fsw.ps f28, 896 (x20)\n"
         "fsw.ps f29, 928 (x20)\n"
         "fsw.ps f30, 960 (x20)\n"
         "fsw.ps f31, 992 (x20)\n"
         "fsw.ps f0, 0 (x21)\n"
         "fsw.ps f1, 32 (x21)\n"
         "fsw.ps f2, 64 (x21)\n"
         "fsw.ps f3, 96 (x21)\n"
         "fsw.ps f4, 128 (x21)\n"
         "fsw.ps f5, 160 (x21)\n"
         "fsw.ps f6, 192 (x21)\n"
         "fsw.ps f7, 224 (x21)\n"
         "fsw.ps f8, 256 (x21)\n"
         "fsw.ps f9, 288 (x21)\n"
         "fsw.ps f10, 320 (x21)\n"
         "fsw.ps f11, 352 (x21)\n"
         "fsw.ps f12, 384 (x21)\n"
         "fsw.ps f13, 416 (x21)\n"
         "fsw.ps f14, 448 (x21)\n"
         "fsw.ps f15, 480 (x21)\n"
         "fsw.ps f16, 512 (x21)\n"
         "fsw.ps f17, 544 (x21)\n"
         "fsw.ps f18, 576 (x21)\n"
         "fsw.ps f19, 608 (x21)\n"
         "fsw.ps f20, 640 (x21)\n"
         "fsw.ps f21, 672 (x21)\n"
         "fsw.ps f22, 704 (x21)\n"
         "fsw.ps f23, 736 (x21)\n"
         "fsw.ps f24, 768 (x21)\n"
         "fsw.ps f25, 800 (x21)\n"
         "fsw.ps f26, 832 (x21)\n"
         "fsw.ps f27, 864 (x21)\n"
         "fsw.ps f28, 896 (x21)\n"
         "fsw.ps f29, 928 (x21)\n"
         "fsw.ps f30, 960 (x21)\n"
         "fsw.ps f31, 992 (x21)\n"
         "fsw.ps f0, 0 (x22)\n"
         "fsw.ps f1, 32 (x22)\n"
         "fsw.ps f2, 64 (x22)\n"
         "fsw.ps f3, 96 (x22)\n"
         "fsw.ps f4, 128 (x22)\n"
         "fsw.ps f5, 160 (x22)\n"
         "fsw.ps f6, 192 (x22)\n"
         "fsw.ps f7, 224 (x22)\n"
         "fsw.ps f8, 256 (x22)\n"
         "fsw.ps f9, 288 (x22)\n"
         "fsw.ps f10, 320 (x22)\n"
         "fsw.ps f11, 352 (x22)\n"
         "fsw.ps f12, 384 (x22)\n"
         "fsw.ps f13, 416 (x22)\n"
         "fsw.ps f14, 448 (x22)\n"
         "fsw.ps f15, 480 (x22)\n"
         "fsw.ps f16, 512 (x22)\n"
         "fsw.ps f17, 544 (x22)\n"
         "fsw.ps f18, 576 (x22)\n"
         "fsw.ps f19, 608 (x22)\n"
         "fsw.ps f20, 640 (x22)\n"
         "fsw.ps f21, 672 (x22)\n"
         "fsw.ps f22, 704 (x22)\n"
         "fsw.ps f23, 736 (x22)\n"
         "fsw.ps f24, 768 (x22)\n"
         "fsw.ps f25, 800 (x22)\n"
         "fsw.ps f26, 832 (x22)\n"
         "fsw.ps f27, 864 (x22)\n"
         "fsw.ps f28, 896 (x22)\n"
         "fsw.ps f29, 928 (x22)\n"
         "fsw.ps f30, 960 (x22)\n"
         "fsw.ps f31, 992 (x22)\n"
         "fsw.ps f0, 0 (x23)\n"
         "fsw.ps f1, 32 (x23)\n"
         "fsw.ps f2, 64 (x23)\n"
         "fsw.ps f3, 96 (x23)\n"
         "fsw.ps f4, 128 (x23)\n"
         "fsw.ps f5, 160 (x23)\n"
         "fsw.ps f6, 192 (x23)\n"
         "fsw.ps f7, 224 (x23)\n"
         "fsw.ps f8, 256 (x23)\n"
         "fsw.ps f9, 288 (x23)\n"
         "fsw.ps f10, 320 (x23)\n"
         "fsw.ps f11, 352 (x23)\n"
         "fsw.ps f12, 384 (x23)\n"
         "fsw.ps f13, 416 (x23)\n"
         "fsw.ps f14, 448 (x23)\n"
         "fsw.ps f15, 480 (x23)\n"
         "fsw.ps f16, 512 (x23)\n"
         "fsw.ps f17, 544 (x23)\n"
         "fsw.ps f18, 576 (x23)\n"
         "fsw.ps f19, 608 (x23)\n"
         "fsw.ps f20, 640 (x23)\n"
         "fsw.ps f21, 672 (x23)\n"
         "fsw.ps f22, 704 (x23)\n"
         "fsw.ps f23, 736 (x23)\n"
         "fsw.ps f24, 768 (x23)\n"
         "fsw.ps f25, 800 (x23)\n"
         "fsw.ps f26, 832 (x23)\n"
         "fsw.ps f27, 864 (x23)\n"
         "fsw.ps f28, 896 (x23)\n"
         "fsw.ps f29, 928 (x23)\n"
         "fsw.ps f30, 960 (x23)\n"
         "fsw.ps f31, 992 (x23)\n"
         "fsw.ps f0, 0 (x24)\n"
         "fsw.ps f1, 32 (x24)\n"
         "fsw.ps f2, 64 (x24)\n"
         "fsw.ps f3, 96 (x24)\n"
         "fsw.ps f4, 128 (x24)\n"
         "fsw.ps f5, 160 (x24)\n"
         "fsw.ps f6, 192 (x24)\n"
         "fsw.ps f7, 224 (x24)\n"
         "fsw.ps f8, 256 (x24)\n"
         "fsw.ps f9, 288 (x24)\n"
         "fsw.ps f10, 320 (x24)\n"
         "fsw.ps f11, 352 (x24)\n"
         "fsw.ps f12, 384 (x24)\n"
         "fsw.ps f13, 416 (x24)\n"
         "fsw.ps f14, 448 (x24)\n"
         "fsw.ps f15, 480 (x24)\n"
         "fsw.ps f16, 512 (x24)\n"
         "fsw.ps f17, 544 (x24)\n"
         "fsw.ps f18, 576 (x24)\n"
         "fsw.ps f19, 608 (x24)\n"
         "fsw.ps f20, 640 (x24)\n"
         "fsw.ps f21, 672 (x24)\n"
         "fsw.ps f22, 704 (x24)\n"
         "fsw.ps f23, 736 (x24)\n"
         "fsw.ps f24, 768 (x24)\n"
         "fsw.ps f25, 800 (x24)\n"
         "fsw.ps f26, 832 (x24)\n"
         "fsw.ps f27, 864 (x24)\n"
         "fsw.ps f28, 896 (x24)\n"
         "fsw.ps f29, 928 (x24)\n"
         "fsw.ps f30, 960 (x24)\n"
         "fsw.ps f31, 992 (x24)\n"
         "fsw.ps f0, 0 (x25)\n"
         "fsw.ps f1, 32 (x25)\n"
         "fsw.ps f2, 64 (x25)\n"
         "fsw.ps f3, 96 (x25)\n"
         "fsw.ps f4, 128 (x25)\n"
         "fsw.ps f5, 160 (x25)\n"
         "fsw.ps f6, 192 (x25)\n"
         "fsw.ps f7, 224 (x25)\n"
         "fsw.ps f8, 256 (x25)\n"
         "fsw.ps f9, 288 (x25)\n"
         "fsw.ps f10, 320 (x25)\n"
         "fsw.ps f11, 352 (x25)\n"
         "fsw.ps f12, 384 (x25)\n"
         "fsw.ps f13, 416 (x25)\n"
         "fsw.ps f14, 448 (x25)\n"
         "fsw.ps f15, 480 (x25)\n"
         "fsw.ps f16, 512 (x25)\n"
         "fsw.ps f17, 544 (x25)\n"
         "fsw.ps f18, 576 (x25)\n"
         "fsw.ps f19, 608 (x25)\n"
         "fsw.ps f20, 640 (x25)\n"
         "fsw.ps f21, 672 (x25)\n"
         "fsw.ps f22, 704 (x25)\n"
         "fsw.ps f23, 736 (x25)\n"
         "fsw.ps f24, 768 (x25)\n"
         "fsw.ps f25, 800 (x25)\n"
         "fsw.ps f26, 832 (x25)\n"
         "fsw.ps f27, 864 (x25)\n"
         "fsw.ps f28, 896 (x25)\n"
         "fsw.ps f29, 928 (x25)\n"
         "fsw.ps f30, 960 (x25)\n"
         "fsw.ps f31, 992 (x25)\n"
         "fsw.ps f0, 0 (x26)\n"
         "fsw.ps f1, 32 (x26)\n"
         "fsw.ps f2, 64 (x26)\n"
         "fsw.ps f3, 96 (x26)\n"
         "fsw.ps f4, 128 (x26)\n"
         "fsw.ps f5, 160 (x26)\n"
         "fsw.ps f6, 192 (x26)\n"
         "fsw.ps f7, 224 (x26)\n"
         "fsw.ps f8, 256 (x26)\n"
         "fsw.ps f9, 288 (x26)\n"
         "fsw.ps f10, 320 (x26)\n"
         "fsw.ps f11, 352 (x26)\n"
         "fsw.ps f12, 384 (x26)\n"
         "fsw.ps f13, 416 (x26)\n"
         "fsw.ps f14, 448 (x26)\n"
         "fsw.ps f15, 480 (x26)\n"
         "fsw.ps f16, 512 (x26)\n"
         "fsw.ps f17, 544 (x26)\n"
         "fsw.ps f18, 576 (x26)\n"
         "fsw.ps f19, 608 (x26)\n"
         "fsw.ps f20, 640 (x26)\n"
         "fsw.ps f21, 672 (x26)\n"
         "fsw.ps f22, 704 (x26)\n"
         "fsw.ps f23, 736 (x26)\n"
         "fsw.ps f24, 768 (x26)\n"
         "fsw.ps f25, 800 (x26)\n"
         "fsw.ps f26, 832 (x26)\n"
         "fsw.ps f27, 864 (x26)\n"
         "fsw.ps f28, 896 (x26)\n"
         "fsw.ps f29, 928 (x26)\n"
         "fsw.ps f30, 960 (x26)\n"
         "fsw.ps f31, 992 (x26)\n"
         "fsw.ps f0, 0 (x27)\n"
         "fsw.ps f1, 32 (x27)\n"
         "fsw.ps f2, 64 (x27)\n"
         "fsw.ps f3, 96 (x27)\n"
         "fsw.ps f4, 128 (x27)\n"
         "fsw.ps f5, 160 (x27)\n"
         "fsw.ps f6, 192 (x27)\n"
         "fsw.ps f7, 224 (x27)\n"
         "fsw.ps f8, 256 (x27)\n"
         "fsw.ps f9, 288 (x27)\n"
         "fsw.ps f10, 320 (x27)\n"
         "fsw.ps f11, 352 (x27)\n"
         "fsw.ps f12, 384 (x27)\n"
         "fsw.ps f13, 416 (x27)\n"
         "fsw.ps f14, 448 (x27)\n"
         "fsw.ps f15, 480 (x27)\n"
         "fsw.ps f16, 512 (x27)\n"
         "fsw.ps f17, 544 (x27)\n"
         "fsw.ps f18, 576 (x27)\n"
         "fsw.ps f19, 608 (x27)\n"
         "fsw.ps f20, 640 (x27)\n"
         "fsw.ps f21, 672 (x27)\n"
         "fsw.ps f22, 704 (x27)\n"
         "fsw.ps f23, 736 (x27)\n"
         "fsw.ps f24, 768 (x27)\n"
         "fsw.ps f25, 800 (x27)\n"
         "fsw.ps f26, 832 (x27)\n"
         "fsw.ps f27, 864 (x27)\n"
         "fsw.ps f28, 896 (x27)\n"
         "fsw.ps f29, 928 (x27)\n"
         "fsw.ps f30, 960 (x27)\n"
         "fsw.ps f31, 992 (x27)\n"
         "fsw.ps f0, 0 (x28)\n"
         "fsw.ps f1, 32 (x28)\n"
         "fsw.ps f2, 64 (x28)\n"
         "fsw.ps f3, 96 (x28)\n"
         "fsw.ps f4, 128 (x28)\n"
         "fsw.ps f5, 160 (x28)\n"
         "fsw.ps f6, 192 (x28)\n"
         "fsw.ps f7, 224 (x28)\n"
         "fsw.ps f8, 256 (x28)\n"
         "fsw.ps f9, 288 (x28)\n"
         "fsw.ps f10, 320 (x28)\n"
         "fsw.ps f11, 352 (x28)\n"
         "fsw.ps f12, 384 (x28)\n"
         "fsw.ps f13, 416 (x28)\n"
         "fsw.ps f14, 448 (x28)\n"
         "fsw.ps f15, 480 (x28)\n"
         "fsw.ps f16, 512 (x28)\n"
         "fsw.ps f17, 544 (x28)\n"
         "fsw.ps f18, 576 (x28)\n"
         "fsw.ps f19, 608 (x28)\n"
         "fsw.ps f20, 640 (x28)\n"
         "fsw.ps f21, 672 (x28)\n"
         "fsw.ps f22, 704 (x28)\n"
         "fsw.ps f23, 736 (x28)\n"
         "fsw.ps f24, 768 (x28)\n"
         "fsw.ps f25, 800 (x28)\n"
         "fsw.ps f26, 832 (x28)\n"
         "fsw.ps f27, 864 (x28)\n"
         "fsw.ps f28, 896 (x28)\n"
         "fsw.ps f29, 928 (x28)\n"
         "fsw.ps f30, 960 (x28)\n"
         "fsw.ps f31, 992 (x28)\n"
         "fsw.ps f0, 0 (x29)\n"
         "fsw.ps f1, 32 (x29)\n"
         "fsw.ps f2, 64 (x29)\n"
         "fsw.ps f3, 96 (x29)\n"
         "fsw.ps f4, 128 (x29)\n"
         "fsw.ps f5, 160 (x29)\n"
         "fsw.ps f6, 192 (x29)\n"
         "fsw.ps f7, 224 (x29)\n"
         "fsw.ps f8, 256 (x29)\n"
         "fsw.ps f9, 288 (x29)\n"
         "fsw.ps f10, 320 (x29)\n"
         "fsw.ps f11, 352 (x29)\n"
         "fsw.ps f12, 384 (x29)\n"
         "fsw.ps f13, 416 (x29)\n"
         "fsw.ps f14, 448 (x29)\n"
         "fsw.ps f15, 480 (x29)\n"
         "fsw.ps f16, 512 (x29)\n"
         "fsw.ps f17, 544 (x29)\n"
         "fsw.ps f18, 576 (x29)\n"
         "fsw.ps f19, 608 (x29)\n"
         "fsw.ps f20, 640 (x29)\n"
         "fsw.ps f21, 672 (x29)\n"
         "fsw.ps f22, 704 (x29)\n"
         "fsw.ps f23, 736 (x29)\n"
         "fsw.ps f24, 768 (x29)\n"
         "fsw.ps f25, 800 (x29)\n"
         "fsw.ps f26, 832 (x29)\n"
         "fsw.ps f27, 864 (x29)\n"
         "fsw.ps f28, 896 (x29)\n"
         "fsw.ps f29, 928 (x29)\n"
         "fsw.ps f30, 960 (x29)\n"
         "fsw.ps f31, 992 (x29)\n"
         "fsw.ps f0, 0 (x30)\n"
         "fsw.ps f1, 32 (x30)\n"
         "fsw.ps f2, 64 (x30)\n"
         "fsw.ps f3, 96 (x30)\n"
         "fsw.ps f4, 128 (x30)\n"
         "fsw.ps f5, 160 (x30)\n"
         "fsw.ps f6, 192 (x30)\n"
         "fsw.ps f7, 224 (x30)\n"
         "fsw.ps f8, 256 (x30)\n"
         "fsw.ps f9, 288 (x30)\n"
         "fsw.ps f10, 320 (x30)\n"
         "fsw.ps f11, 352 (x30)\n"
         "fsw.ps f12, 384 (x30)\n"
         "fsw.ps f13, 416 (x30)\n"
         "fsw.ps f14, 448 (x30)\n"
         "fsw.ps f15, 480 (x30)\n"
         "fsw.ps f16, 512 (x30)\n"
         "fsw.ps f17, 544 (x30)\n"
         "fsw.ps f18, 576 (x30)\n"
         "fsw.ps f19, 608 (x30)\n"
         "fsw.ps f20, 640 (x30)\n"
         "fsw.ps f21, 672 (x30)\n"
         "fsw.ps f22, 704 (x30)\n"
         "fsw.ps f23, 736 (x30)\n"
         "fsw.ps f24, 768 (x30)\n"
         "fsw.ps f25, 800 (x30)\n"
         "fsw.ps f26, 832 (x30)\n"
         "fsw.ps f27, 864 (x30)\n"
         "fsw.ps f28, 896 (x30)\n"
         "fsw.ps f29, 928 (x30)\n"
         "fsw.ps f30, 960 (x30)\n"
         "fsw.ps f31, 992 (x30)\n"
         "fsw.ps f0, 0 (x31)\n"
         "fsw.ps f1, 32 (x31)\n"
         "fsw.ps f2, 64 (x31)\n"
         "fsw.ps f3, 96 (x31)\n"
         "fsw.ps f4, 128 (x31)\n"
         "fsw.ps f5, 160 (x31)\n"
         "fsw.ps f6, 192 (x31)\n"
         "fsw.ps f7, 224 (x31)\n"
         "fsw.ps f8, 256 (x31)\n"
         "fsw.ps f9, 288 (x31)\n"
         "fsw.ps f10, 320 (x31)\n"
         "fsw.ps f11, 352 (x31)\n"
         "fsw.ps f12, 384 (x31)\n"
         "fsw.ps f13, 416 (x31)\n"
         "fsw.ps f14, 448 (x31)\n"
         "fsw.ps f15, 480 (x31)\n"
         "fsw.ps f16, 512 (x31)\n"
         "fsw.ps f17, 544 (x31)\n"
         "fsw.ps f18, 576 (x31)\n"
         "fsw.ps f19, 608 (x31)\n"
         "fsw.ps f20, 640 (x31)\n"
         "fsw.ps f21, 672 (x31)\n"
         "fsw.ps f22, 704 (x31)\n"
         "fsw.ps f23, 736 (x31)\n"
         "fsw.ps f24, 768 (x31)\n"
         "fsw.ps f25, 800 (x31)\n"
         "fsw.ps f26, 832 (x31)\n"
         "fsw.ps f27, 864 (x31)\n"
         "fsw.ps f28, 896 (x31)\n"
         "fsw.ps f29, 928 (x31)\n"
         "fsw.ps f30, 960 (x31)\n"
         "fsw.ps f31, 992 (x31)\n"
         "maskpopc x25, m4\n"                                  // opcode=0x52020cfb
         "feq.ps f24, f13, f18\n"                              // opcode=0xa126ac7b
         "cubesgntc.ps f23, f18, f12\n"                        // opcode=0x88c93bfb
         "fsgnjn.s f30, f18, f15\n"                            // opcode=0x20f91f53
         "fadd.pi f27, f16, f11\n"                             // opcode=0x06b80dfb
         "feq.s x0, f18, f19\n"                                // opcode=0xa1392053
         "fslli.pi f30, f11, 24\n"                             // opcode=0x4f859f7b
         "flt.ps f21, f16, f17\n"                              // opcode=0xa1181afb
         "fnot.pi f21, f10\n"                                  // opcode=0x06052afb
         "maskor m1, m5, m6\n"                                 // opcode=0x6662e0fb
         "fmadd.s f25, f15, f14, f12, dyn\n"                   // opcode=0x60e7fcc3
         "fsw f17, 102(x19)\n"                                 // opcode=0xcf19a327
         "fsat8.pi f25, f11\n"                                 // opcode=0x0605bcfb
         "fswizz.ps f27, f15, 88\n"                            // opcode=0xe6b78dfb
         "flw.ps f25, 110(x17)\n"                              // opcode=0x06e8ac8b
         "fsgnjn.ps f28, f18, f10\n"                           // opcode=0x20a91e7b
         "fsgnj.ps f21, f13, f19\n"                            // opcode=0x21368afb
         "fmulhu.pi f31, f14, f12\n"                           // opcode=0x16c71ffb
         "fmul.s f23, f11, f10, dyn\n"                         // opcode=0x10a5fbd3
         "fslli.pi f29, f12, 28\n"                             // opcode=0x4fc61efb
         "fsub.ps f28, f13, f11, dyn\n"                        // opcode=0x08b6fe7b
         "cubesgnsc.ps f23, f15, f16\n"                        // opcode=0x8907abfb
         "fmulh.pi f22, f13, f17\n"                            // opcode=0x1716ab7b
         "fcmovm.ps f27, f16, f12\n"                           // opcode=0x00c80df7
         "fpackrepb.pi f29, f18\n"                             // opcode=0x26090efb
         "cubeface.ps f31, f19, f16\n"                         // opcode=0x89098ffb
         "fadd.s f29, f12, f19, dyn\n"                         // opcode=0x01367ed3
         "maskor m1, m2, m7\n"                                 // opcode=0x667160fb
         "fsgnj.ps f29, f19, f19\n"                            // opcode=0x21398efb
         "flw f21, 85(x10)\n"                                  // opcode=0xa5552a87
         "fbcx.ps f26, x11\n"                                  // opcode=0x0005bd0b
         "fmsub.ps f26, f14, f14, f15, dyn\n"                  // opcode=0x7ae77d5b
         "fsub.ps f21, f19, f18, dyn\n"                        // opcode=0x0929fafb
         "cubefaceidx.ps f31, f13, f18\n"                      // opcode=0x89269ffb
         "ffrc.ps f31, f17\n"                                  // opcode=0x58288ffb
         "fmadd.ps f23, f14, f18, f16, dyn\n"                  // opcode=0x81277bdb
         "fmsub.s f20, f14, f15, f13, dyn\n"                   // opcode=0x68f77a47
         "fsat8.pi f26, f15\n"                                 // opcode=0x0607bd7b
         "fxor.pi f22, f14, f18\n"                             // opcode=0x07274b7b
         "fsgnjx.ps f25, f15, f17\n"                           // opcode=0x2117acfb
         "fadd.pi f25, f14, f11\n"                             // opcode=0x06b70cfb
         "for.pi f31, f19, f19\n"                              // opcode=0x0739effb
         "fsetm.pi m5, f11\n"                                  // opcode=0xe005d2fb
         "mova.m.x x15\n"                                      // opcode=0xd607907b
         "fandi.pi f29, f19, 14\n"                            // opcode=0xb4a99ebf
         "fle.ps f20, f18, f17\n"                              // opcode=0xa1190a7b
         "masknot m0, m0\n"                                    // opcode=0x6600207b
         "feq.ps f23, f19, f12\n"                              // opcode=0xa0c9abfb
         "fltm.pi m0, f13, f17\n"                              // opcode=0x3f16807b
         "fmul.pi f30, f15, f14\n"                             // opcode=0x16e78f7b
         "fadd.ps f28, f12, f11, dyn\n"                        // opcode=0x00b67e7b
         "fle.s x20, f11, f13\n"                               // opcode=0xa0d58a53
         "fmul.pi f21, f12, f18\n"                             // opcode=0x17260afb
         "fsub.s f20, f14, f10, dyn\n"                         // opcode=0x08a77a53
         "fadd.s f26, f17, f17, dyn\n"                         // opcode=0x0118fd53
         "fsgnjx.ps f22, f14, f10\n"                           // opcode=0x20a72b7b
         "fslli.pi f30, f13, 26\n"                             // opcode=0x4fa69f7b
         "fle.ps f26, f17, f15\n"                              // opcode=0xa0f88d7b
         "fcmov.ps f20, f11, f13, f12\n"                       // opcode=0x64d5aa3f
         "mov.m.x m3, x16, 245\n"                              // opcode=0x57e851fb
         "flw.ps f26, 7(x18)\n"                                // opcode=0x98792d0b
         "fadd.s f24, f17, f13, dyn\n"                         // opcode=0x00d8fc53
         "fsll.pi f23, f15, f19\n"                             // opcode=0x07379bfb
         "flw f27, 41(x18)\n"                                  // opcode=0xaa992d87
         "mov.m.x m2, x14, 230\n"                              // opcode=0x57c7617b
         "cubesgntc.ps f30, f18, f10\n"                        // opcode=0x88a93f7b
         "fsub.s f25, f15, f19, dyn\n"                         // opcode=0x0937fcd3
         "fcmov.ps f22, f13, f11, f15\n"                       // opcode=0x7cb6ab3f
         "fltm.ps m2, f18, f10\n"                              // opcode=0xa0a9517b
         "fmulh.pi f28, f11, f12\n"                            // opcode=0x16c5ae7b
         "fmul.s f24, f15, f13, dyn\n"                         // opcode=0x10d7fc53
         "fsra.pi f20, f12, f12\n"                             // opcode=0x0ec65a7b
         "for.pi f27, f17, f18\n"                              // opcode=0x0728edfb
         "mova.m.x x11\n"                                      // opcode=0xd605907b
         "fand.pi f20, f18, f18\n"                             // opcode=0x07297a7b
         "flw f28, 55(x13)\n"                                  // opcode=0x0376ae07
         "fsll.pi f28, f15, f12\n"                             // opcode=0x06c79e7b
         "fsgnjn.s f24, f10, f14\n"                            // opcode=0x20e51c53
         "fsgnjx.s f27, f14, f16\n"                            // opcode=0x21072dd3
         "fclass.s x23, f15\n"                                 // opcode=0xe0079bd3
         "fround.ps f25, f15, dyn\n"                           // opcode=0x5817fcfb
         "fsrl.pi f25, f16, f19\n"                             // opcode=0x07385cfb
         "cubeface.ps f28, f16, f15\n"                         // opcode=0x88f80e7b
         "fsgnj.s f20, f16, f13\n"                             // opcode=0x20d80a53
         "fsetm.pi m7, f11\n"                                  // opcode=0xe005d3fb
         "fsub.ps f22, f10, f13, dyn\n"                        // opcode=0x08d57b7b
         "fsgnjx.s f23, f18, f18\n"                            // opcode=0x21292bd3
         "faddi.pi f29, f16, 58\n"                            // opcode=0xd5a80ebf
         "mov.m.x m2, x18, 167\n"                              // opcode=0x5749717b
         "fsub.ps f31, f11, f16, dyn\n"                        // opcode=0x0905fffb
         "fltm.ps m2, f10, f14\n"                              // opcode=0xa0e5517b
         "faddi.pi f25, f15, 34\n"                             // opcode=0x0c278cbf
         "fle.ps f28, f15, f18\n"                              // opcode=0xa1278e7b
         "fsrl.pi f22, f14, f10\n"                             // opcode=0x06a75b7b
         "maskxor m5, m1, m0\n"                                // opcode=0x6600c2fb
         "flt.s x31, f14, f15\n"                               // opcode=0xa0f71fd3
         "maskpopc.rast x28, m4, m1, 6\n"                      // opcode=0x5e9a0e7b
         "flt.ps f22, f12, f16\n"                              // opcode=0xa1061b7b
         "feq.ps f26, f19, f14\n"                              // opcode=0xa0e9ad7b
         "fslli.pi f29, f16, 0\n"                              // opcode=0x4e081efb
         "flt.ps f25, f10, f10\n"                              // opcode=0xa0a51cfb
         "fandi.pi f20, f13, 33\n"                            // opcode=0x85569a3f
         "feq.pi f27, f14, f17\n"                              // opcode=0xa7172dfb
         "fbcx.ps f24, x15\n"                                  // opcode=0x0007bc0b
         "fsgnjx.ps f29, f19, f14\n"                           // opcode=0x20e9aefb
         "cubeface.ps f25, f15, f16\n"                         // opcode=0x89078cfb
         "fpackreph.pi f22, f13\n"                             // opcode=0x26069b7b
         "feq.pi f30, f11, f11\n"                              // opcode=0xa6b5af7b
         "fadd.ps f0, f10, f18, dyn\n"                         // opcode=0x0125707b
         "fround.ps f27, f19, dyn\n"                           // opcode=0x5819fdfb
         "for.pi f25, f10, f19\n"                              // opcode=0x07356cfb
         "fxor.pi f25, f10, f16\n"                             // opcode=0x07054cfb
         "fround.ps f23, f17, dyn\n"                           // opcode=0x5818fbfb
         "maskpopcz x25, m0\n"                                 // opcode=0x54000cfb
         "maskxor m1, m3, m6\n"                                // opcode=0x6661c0fb
         "fcmov.ps f23, f17, f13, f17\n"                       // opcode=0x8cd8abbf
         "fbci.ps f26, 275363\n"                               // opcode=0x433a3d1f
         "fswizz.ps f20, f15, 55\n"                            // opcode=0xe667fa7b
         "fsgnj.ps f26, f11, f10\n"                            // opcode=0x20a58d7b
         "fsll.pi f29, f16, f11\n"                             // opcode=0x06b81efb
         "fand.pi f31, f15, f10\n"                             // opcode=0x06a7fffb
         "fmul.ps f22, f19, f18, rtz\n"                        // opcode=0x11299b7b
         "maskand m1, m6, m4\n"                                // opcode=0x664370fb
         "fclass.ps f30, f15\n"                                // opcode=0xe0079f7b
         "feqm.ps m2, f10, f18\n"                              // opcode=0xa125617b
         "fclass.s x26, f17\n"                                 // opcode=0xe0089d53
         "fltu.pi f31, f12, f10\n"                             // opcode=0xa6a63ffb
         "maskor m0, m4, m4\n"                                 // opcode=0x6642607b
         "fsgnj.ps f24, f13, f14\n"                            // opcode=0x20e68c7b
         "fsw f17, 12(x15)\n"                                  // opcode=0xd917a627
         "fsgnj.ps f20, f17, f17\n"                            // opcode=0x21188a7b
         "fadd.pi f28, f19, f11\n"                             // opcode=0x06b98e7b
         "maskpopc.rast x22, m3, m4, 3\n"                      // opcode=0x5e4d8b7b
         "fmul.ps f25, f15, f19, dyn\n"                        // opcode=0x1137fcfb
         "cubesgnsc.ps f23, f16, f14\n"                        // opcode=0x88e82bfb
         "fclass.ps f22, f14\n"                                // opcode=0xe0071b7b
         "fle.s x21, f19, f14\n"                               // opcode=0xa0e98ad3
         "fadd.s f23, f15, f10, dyn\n"                         // opcode=0x00a7fbd3
         "fsgnj.ps f29, f10, f18\n"                            // opcode=0x21250efb
         "fsll.pi f26, f16, f16\n"                             // opcode=0x07081d7b
         "fxor.pi f21, f15, f18\n"                             // opcode=0x0727cafb
         "fle.pi f29, f17, f17\n"                              // opcode=0xa7188efb
         "cubeface.ps f29, f10, f11\n"                         // opcode=0x88b50efb
         "fswizz.ps f30, f15, 112\n"                           // opcode=0xe6e78f7b
         "fsgnj.ps f24, f15, f12\n"                            // opcode=0x20c78c7b
         "mov.m.x m7, x16, 22\n"                               // opcode=0x562863fb
         "fsetm.pi m3, f13\n"                                  // opcode=0xe006d1fb
         "mov.m.x m6, x11, 173\n"                              // opcode=0x5755d37b
         "cubesgnsc.ps f22, f14, f11\n"                        // opcode=0x88b72b7b
         "fmulhu.pi f31, f10, f14\n"                           // opcode=0x16e51ffb
         "fand.pi f24, f19, f15\n"                             // opcode=0x06f9fc7b
         "cubeface.ps f20, f18, f18\n"                         // opcode=0x89290a7b
         "cubesgnsc.ps f30, f11, f18\n"                        // opcode=0x8925af7b
         "fmul.s f25, f15, f10, rmm\n"                         // opcode=0x10a7ccd3
         "fle.s x22, f15, f14\n"                               // opcode=0xa0e78b53
         "feqm.ps m6, f18, f18\n"                              // opcode=0xa129637b
         "cubefaceidx.ps f27, f17, f10\n"                      // opcode=0x88a89dfb
         "cubesgnsc.ps f30, f13, f11\n"                        // opcode=0x88b6af7b
         "fslli.pi f22, f10, 16\n"                             // opcode=0x4f051b7b
         "fle.pi f25, f15, f17\n"                              // opcode=0xa7178cfb
         "fmul.s f27, f11, f18, dyn\n"                         // opcode=0x1125fdd3
         "fsatu8.pi f28, f14\n"                                // opcode=0x06173e7b
         "fpackrepb.pi f30, f10\n"                             // opcode=0x26050f7b
         "fsub.ps f30, f15, f19, dyn\n"                        // opcode=0x0937ff7b
         "maskpopc x24, m6\n"                                  // opcode=0x52030c7b
         "fsgnjx.s f22, f15, f16\n"                            // opcode=0x2107ab53
         "fbc.ps f21, 63(x15)\n"                               // opcode=0xe3f78a8b
         "fsrai.pi f28, f19, 29\n"                             // opcode=0x4fd9fe7b
         "feq.pi f21, f12, f13\n"                              // opcode=0xa6d62afb
         "fsgnjn.s f25, f17, f12\n"                            // opcode=0x20c89cd3
         "fslli.pi f26, f11, 28\n"                             // opcode=0x4fc59d7b
         "cubeface.ps f29, f11, f11\n"                         // opcode=0x88b58efb
         "fsgnjn.ps f26, f18, f16\n"                           // opcode=0x21091d7b
         "fsw f18, 114(x10)\n"                                 // opcode=0x77252927
         "cubeface.ps f0, f19, f14\n"                          // opcode=0x88e9807b
         "mova.x.m x25\n"                                      // opcode=0xd6000cfb
         "maskpopc x20, m7\n"                                  // opcode=0x52038a7b
         "fltu.pi f24, f15, f12\n"                             // opcode=0xa6c7bc7b
         "fsgnjx.s f27, f10, f10\n"                            // opcode=0x20a52dd3
         "fsrl.pi f23, f15, f12\n"                             // opcode=0x06c7dbfb
         "fsra.pi f31, f10, f17\n"                             // opcode=0x0f155ffb
         "flw f30, 7(x13)\n"                                   // opcode=0x7876af07
         "ffrc.ps f28, f12\n"                                  // opcode=0x58260e7b
         "fsll.pi f22, f11, f12\n"                             // opcode=0x06c59b7b
         "fle.ps f30, f12, f10\n"                              // opcode=0xa0a60f7b
         "fpackreph.pi f30, f18\n"                             // opcode=0x26091f7b
         "fmul.s f26, f19, f19, dyn\n"                         // opcode=0x1139fd53
         "faddi.pi f21, f15, 215\n"                            // opcode=0x35778abf
         "fcmovm.ps f25, f11, f17\n"                           // opcode=0x01158cf7
         "fxor.pi f26, f16, f11\n"                             // opcode=0x06b84d7b
         "masknot m1, m4\n"                                    // opcode=0x660220fb
         "fsrli.pi f30, f12, 22\n"                             // opcode=0x4f665f7b
         "fcmov.ps f23, f13, f15, f15\n"                       // opcode=0x7cf6abbf
         "flw f20, 45(x14)\n"                                  // opcode=0x3ad72a07
         "fcmov.ps f0, f15, f16, f12\n"                        // opcode=0x6507a03f
         "fltu.pi f21, f10, f17\n"                             // opcode=0xa7153afb
         "fadd.ps f0, f19, f12, dyn\n"                         // opcode=0x00c9f07b
         "fle.pi f25, f15, f13\n"                              // opcode=0xa6d78cfb
         "fpackreph.pi f24, f15\n"                             // opcode=0x26079c7b
         "faddi.pi f20, f16, 185\n"                            // opcode=0x2d980a3f
         "cubesgntc.ps f0, f11, f11\n"                         // opcode=0x88b5b07b
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "flw.ps f31, 42(x16)\n"                               // opcode=0x3aa82f8b
         "fmsub.ps f28, f19, f10, f13, dyn\n"                  // opcode=0x6aa9fe5b
         "fsrai.pi f31, f17, 14\n"                             // opcode=0x4ee8fffb
         "fbci.ps f23, 734330\n"                               // opcode=0xb347ab9f
         "mova.m.x x18\n"                                      // opcode=0xd609107b
         "maskor m6, m6, m0\n"                                 // opcode=0x6603637b
         "fsatu8.pi f31, f16\n"                                // opcode=0x06183ffb
         "fmul.ps f29, f18, f17, rup\n"                        // opcode=0x11193efb
         "maskxor m4, m3, m2\n"                                // opcode=0x6621c27b
         "fmadd.ps f26, f16, f17, f11, dyn\n"                  // opcode=0x59187d5b
         "masknot m1, m4\n"                                    // opcode=0x660220fb
         "fmulhu.pi f20, f12, f14\n"                           // opcode=0x16e61a7b
         "fcmovm.ps f20, f19, f10\n"                           // opcode=0x00a98a77
         "mova.x.m x26\n"                                      // opcode=0xd6000d7b
         "flw f24, 57(x17)\n"                                  // opcode=0x1398ac07
         "fsll.pi f21, f18, f14\n"                             // opcode=0x06e91afb
         "fsub.ps f20, f15, f16, dyn\n"                        // opcode=0x0907fa7b
         "fle.pi f26, f11, f16\n"                              // opcode=0xa7058d7b
         "fadd.s f24, f14, f16, dyn\n"                         // opcode=0x01077c53
         "fpackrepb.pi f22, f12\n"                             // opcode=0x26060b7b
         "fclass.s x27, f17\n"                                 // opcode=0xe0089dd3
         "fsgnjn.ps f24, f15, f18\n"                           // opcode=0x21279c7b
         "fround.ps f20, f18, dyn\n"                           // opcode=0x58197a7b
         "fmul.ps f28, f16, f10, dyn\n"                        // opcode=0x10a87e7b
         "flt.ps f29, f15, f10\n"                              // opcode=0xa0a79efb
         "fsll.pi f27, f12, f11\n"                             // opcode=0x06b61dfb
         "fsrli.pi f0, f19, 4\n"                               // opcode=0x4e49d07b
         "fmsub.ps f26, f19, f18, f12, dyn\n"                  // opcode=0x6329fd5b
         "fsrli.pi f29, f11, 3\n"                              // opcode=0x4e35defb
         "feq.pi f27, f10, f18\n"                              // opcode=0xa7252dfb
         "fsrai.pi f31, f19, 21\n"                             // opcode=0x4f59fffb
         "fsgnjn.s f29, f13, f10\n"                            // opcode=0x20a69ed3
         "maskand m5, m6, m4\n"                                // opcode=0x664372fb
         "fmulhu.pi f27, f17, f17\n"                           // opcode=0x17189dfb
         "flem.ps m0, f11, f17\n"                              // opcode=0xa115c07b
         "fclass.ps f24, f12\n"                                // opcode=0xe0061c7b
         "flw.ps f30, 97(x13)\n"                               // opcode=0x4e16af0b
         "fsat8.pi f26, f15\n"                                 // opcode=0x0607bd7b
         "fsgnjn.ps f26, f18, f12\n"                           // opcode=0x20c91d7b
         "fsgnjx.ps f24, f17, f11\n"                           // opcode=0x20b8ac7b
         "flw f31, 11(x15)\n"                                  // opcode=0x28b7af87
         "fround.ps f30, f14, dyn\n"                           // opcode=0x58177f7b
         "fmsub.s f31, f13, f16, f12, dyn\n"                   // opcode=0x6106ffc7
         "fmul.ps f25, f15, f18, dyn\n"                        // opcode=0x1127fcfb
         "cubesgnsc.ps f31, f19, f11\n"                        // opcode=0x88b9affb
         "fltu.pi f28, f17, f15\n"                             // opcode=0xa6f8be7b
         "fsetm.pi m5, f14\n"                                  // opcode=0xe00752fb
         "fmulhu.pi f27, f18, f12\n"                           // opcode=0x16c91dfb
         "feq.s x27, f10, f18\n"                               // opcode=0xa1252dd3
         "fltm.ps m7, f19, f19\n"                              // opcode=0xa139d3fb
         "li x5, 975\n"
         "mova.m.x x5\n"
         "fadd.pi f31, f13, f15\n"                             // opcode=0x06f68ffb
         "fnot.pi f27, f16\n"                                  // opcode=0x06082dfb
         "maskpopc.rast x30, m0, m5, 7\n"                      // opcode=0x5edc0f7b
         "cubefaceidx.ps f28, f15, f14\n"                      // opcode=0x88e79e7b
         "maskand m3, m5, m2\n"                                // opcode=0x6622f1fb
         "fsw f17, 107(x15)\n"                                 // opcode=0x9f17a5a7
         "feq.s x29, f12, f18\n"                               // opcode=0xa1262ed3
         "fle.pi f23, f15, f15\n"                              // opcode=0xa6f78bfb
         "feq.s x28, f13, f19\n"                               // opcode=0xa136ae53
         "fclass.ps f20, f13\n"                                // opcode=0xe0069a7b
         "flw f0, 108(x14)\n"                                  // opcode=0xb6c72007
         "maskpopc.rast x20, m7, m6, 5\n"                      // opcode=0x5ee78a7b
         "fbcx.ps f20, x15\n"                                  // opcode=0x0007ba0b
         "fmulhu.pi f0, f11, f18\n"                            // opcode=0x1725907b
         "fsgnjn.s f21, f13, f17\n"                            // opcode=0x21169ad3
         "fnot.pi f22, f11\n"                                  // opcode=0x0605ab7b
         "fmsub.s f23, f18, f10, f18, dyn\n"                   // opcode=0x90a97bc7
         "fsgnjn.s f30, f16, f12\n"                            // opcode=0x20c81f53
         "fsra.pi f29, f18, f14\n"                             // opcode=0x0ee95efb
         "mova.x.m x24\n"                                      // opcode=0xd6000c7b
         "fsw f16, 40(x10)\n"                                  // opcode=0x6b052427
         "fadd.pi f26, f10, f12\n"                             // opcode=0x06c50d7b
         "fpackrepb.pi f22, f16\n"                             // opcode=0x26080b7b
         "fmadd.s f29, f12, f10, f12, dyn\n"                   // opcode=0x60a67ec3
         "fltu.pi f28, f16, f16\n"                             // opcode=0xa7083e7b
         "fsrai.pi f21, f10, 15\n"                             // opcode=0x4ef57afb
         "fmadd.ps f31, f11, f11, f15, dyn\n"                  // opcode=0x78b5ffdb
         "mova.m.x x14\n"                                      // opcode=0xd607107b
         "fmul.pi f26, f17, f18\n"                             // opcode=0x17288d7b
         "fbc.ps f26, 85(x10)\n"                               // opcode=0x2d550d0b
         "fbci.ps f0, 430257\n"                                // opcode=0x690b101f
         "fsgnjx.s f21, f18, f18\n"                            // opcode=0x21292ad3
         "masknot m6, m5\n"                                    // opcode=0x6602a37b
         "fcmovm.ps f29, f19, f14\n"                           // opcode=0x00e98ef7
         "maskpopcz x0, m0\n"                                  // opcode=0x5400007b
         "fand.pi f28, f11, f10\n"                             // opcode=0x06a5fe7b
         "flw f28, 100(x11)\n"                                 // opcode=0xfe45ae07
         "fcmovm.ps f27, f12, f10\n"                           // opcode=0x00a60df7
         "feq.ps f21, f11, f12\n"                              // opcode=0xa0c5aafb
         "fle.ps f27, f14, f18\n"                              // opcode=0xa1270dfb
         "fsgnj.ps f28, f13, f15\n"                            // opcode=0x20f68e7b
         "feq.s x25, f17, f15\n"                               // opcode=0xa0f8acd3
         "cubesgntc.ps f26, f11, f19\n"                        // opcode=0x8935bd7b
         "fsw.ps f16, 36(x14)\n"                               // opcode=0xc307620b
         "flt.pi f25, f12, f13\n"                              // opcode=0xa6d61cfb
         "flw.ps f28, 102(x16)\n"                              // opcode=0xf6682e0b
         "fandi.pi f25, f17, 174\n"                            // opcode=0x2ce89cbf
         "masknot m3, m6\n"                                    // opcode=0x660321fb
         "fmsub.s f21, f19, f12, f10, dyn\n"                   // opcode=0x50c9fac7
         "flw.ps f21, 43(x13)\n"                               // opcode=0x5ab6aa8b
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fnot.pi f26, f14\n"                                  // opcode=0x06072d7b
         "masknot m5, m0\n"                                    // opcode=0x660022fb
         "feq.pi f31, f15, f16\n"                              // opcode=0xa707affb
         "feqm.ps m1, f15, f10\n"                              // opcode=0xa0a7e0fb
         "feqm.ps m4, f15, f10\n"                              // opcode=0xa0a7e27b
         "fsub.s f21, f15, f16, dyn\n"                         // opcode=0x0907fad3
         "fsub.pi f20, f16, f10\n"                             // opcode=0x0ea80a7b
         "fsrai.pi f23, f17, 19\n"                             // opcode=0x4f38fbfb
         "faddi.pi f31, f15, 71\n"                            // opcode=0xbc578fbf
         "cubefaceidx.ps f26, f17, f10\n"                      // opcode=0x88a89d7b
         "fcmov.ps f31, f15, f12, f13\n"                       // opcode=0x6cc7afbf
         "fsgnjn.ps f22, f18, f16\n"                           // opcode=0x21091b7b
         "fsgnjx.s f26, f15, f16\n"                            // opcode=0x2107ad53
         "fclass.s x20, f16\n"                                 // opcode=0xe0081a53
         "fsgnjx.s f21, f14, f14\n"                            // opcode=0x20e72ad3
         "flw f23, 83(x12)\n"                                  // opcode=0x95362b87
         "mov.m.x m3, x14, 91\n"                               // opcode=0x56b731fb
         "fmul.ps f23, f10, f10, dyn\n"                        // opcode=0x10a57bfb
         "fsw.ps f18, 82(x12)\n"                               // opcode=0x1526690b
         "fbci.ps f27, 397425\n"                               // opcode=0x61071d9f
         "fsra.pi f21, f16, f15\n"                             // opcode=0x0ef85afb
         "fmulhu.pi f23, f14, f19\n"                           // opcode=0x17371bfb
         "fsatu8.pi f0, f10\n"                                 // opcode=0x0615307b
         "cubesgnsc.ps f20, f15, f14\n"                        // opcode=0x88e7aa7b
         "fle.ps f29, f10, f19\n"                              // opcode=0xa1350efb
         "flem.ps m1, f15, f13\n"                              // opcode=0xa0d7c0fb
         "fpackrepb.pi f26, f15\n"                             // opcode=0x26078d7b
         "fandi.pi f25, f11, 294\n"                            // opcode=0x4c659cbf
         "fsw f19, 65(x19)\n"                                  // opcode=0x3d39a0a7
         "fsgnj.s f26, f18, f16\n"                             // opcode=0x21090d53
         "fltu.pi f30, f10, f19\n"                             // opcode=0xa7353f7b
         "fsrli.pi f31, f10, 12\n"                             // opcode=0x4ec55ffb
         "masknot m7, m5\n"                                    // opcode=0x6602a3fb
         "fle.s x29, f18, f15\n"                               // opcode=0xa0f90ed3
         "flt.pi f21, f12, f17\n"                              // opcode=0xa7161afb
         "fclass.ps f23, f18\n"                                // opcode=0xe0091bfb
         "fsat8.pi f28, f11\n"                                 // opcode=0x0605be7b
         "feq.pi f21, f10, f11\n"                              // opcode=0xa6b52afb
         "fsatu8.pi f0, f12\n"                                 // opcode=0x0616307b
         "fle.ps f23, f17, f17\n"                              // opcode=0xa1188bfb
         "fround.ps f26, f15, dyn\n"                           // opcode=0x5817fd7b
         "fswizz.ps f25, f19, 50\n"                            // opcode=0xe669acfb
         "maskxor m2, m3, m0\n"                                // opcode=0x6601c17b
         "fadd.ps f21, f12, f19, dyn\n"                        // opcode=0x01367afb
         "fmsub.ps f25, f10, f17, f18, dyn\n"                  // opcode=0x93157cdb
         "cubesgnsc.ps f29, f11, f12\n"                        // opcode=0x88c5aefb
         "maskxor m0, m6, m5\n"                                // opcode=0x6653407b
         "feq.s x20, f10, f13\n"                               // opcode=0xa0d52a53
         "fxor.pi f0, f14, f11\n"                              // opcode=0x06b7407b
         "fsra.pi f28, f18, f18\n"                             // opcode=0x0f295e7b
         "li x5, 3791\n"
         "mova.m.x x5\n"
         "mova.x.m x31\n"                                      // opcode=0xd6000ffb
         "fandi.pi f30, f18, 87\n"                             // opcode=0x15791f3f
         "fclass.s x28, f16\n"                                 // opcode=0xe0081e53
         "fsetm.pi m6, f19\n"                                  // opcode=0xe009d37b
         "fltm.pi m5, f17, f16\n"                              // opcode=0x3f0882fb
         "for.pi f22, f11, f15\n"                              // opcode=0x06f5eb7b
         "fadd.ps f29, f14, f10, dyn\n"                        // opcode=0x00a77efb
         "fmsub.s f25, f13, f15, f16, dyn\n"                   // opcode=0x80f6fcc7
         "fmadd.s f25, f10, f13, f13, rne\n"                   // opcode=0x68d50cc3
         "flw f25, 95(x11)\n"                                  // opcode=0x25f5ac87
         "fsrl.pi f30, f12, f19\n"                             // opcode=0x07365f7b
         "fmulh.pi f31, f13, f14\n"                            // opcode=0x16e6affb
         "fxor.pi f30, f12, f14\n"                             // opcode=0x06e64f7b
         "maskpopc x28, m7\n"                                  // opcode=0x52038e7b
         "fbci.pi f28, 318021\n"                               // opcode=0x4da45e5f
         "fpackreph.pi f31, f15\n"                             // opcode=0x26079ffb
         "fltm.ps m6, f15, f16\n"                              // opcode=0xa107d37b
         "fandi.pi f20, f19, 79\n"                            // opcode=0x94399a3f
         "fswizz.ps f24, f19, 86\n"                            // opcode=0xe6a9ec7b
         "flem.ps m1, f17, f11\n"                              // opcode=0xa0b8c0fb
         "maskand m1, m3, m5\n"                                // opcode=0x6651f0fb
         "fadd.pi f31, f13, f15\n"                             // opcode=0x06f68ffb
         "flw.ps f29, 78(x18)\n"                               // opcode=0x6ce92e8b
         "fxor.pi f26, f18, f19\n"                             // opcode=0x07394d7b
         "fadd.pi f23, f16, f10\n"                             // opcode=0x06a80bfb
         "fsw.ps f12, 18(x14)\n"                               // opcode=0x88c7690b
         "fadd.s f22, f12, f13, rdn\n"                         // opcode=0x00d62b53
         "fsrli.pi f26, f17, 28\n"                             // opcode=0x4fc8dd7b
         "cubesgnsc.ps f23, f12, f16\n"                        // opcode=0x89062bfb
         "fsat8.pi f26, f16\n"                                 // opcode=0x06083d7b
         "flt.s x21, f13, f13\n"                               // opcode=0xa0d69ad3
         "flw.ps f20, 27(x11)\n"                               // opcode=0x31b5aa0b
         "fmul.s f20, f15, f14, rtz\n"                         // opcode=0x10e79a53
         "fle.pi f31, f14, f15\n"                              // opcode=0xa6f70ffb
         "fmulh.pi f26, f17, f17\n"                            // opcode=0x1718ad7b
         "maskpopc.rast x24, m6, m7, 2\n"                      // opcode=0x5e7b0c7b
         "fbcx.ps f22, x19\n"                                  // opcode=0x0009bb0b
         "fsetm.pi m1, f15\n"                                  // opcode=0xe007d0fb
         "feq.ps f27, f15, f18\n"                              // opcode=0xa127adfb
         "fsrai.pi f0, f13, 28\n"                              // opcode=0x4fc6f07b
         "maskor m1, m7, m4\n"                                 // opcode=0x6643e0fb
         "fsetm.pi m2, f18\n"                                  // opcode=0xe009517b
         "fadd.s f20, f14, f16, dyn\n"                         // opcode=0x01077a53
         "fsgnjn.ps f30, f10, f12\n"                           // opcode=0x20c51f7b
         "fadd.pi f31, f18, f15\n"                             // opcode=0x06f90ffb
         "fnot.pi f22, f11\n"                                  // opcode=0x0605ab7b
         "fsat8.pi f24, f12\n"                                 // opcode=0x06063c7b
         "maskpopc x25, m4\n"                                  // opcode=0x52020cfb
         "fsetm.pi m1, f12\n"                                  // opcode=0xe00650fb
         "fsgnjn.s f26, f18, f14\n"                            // opcode=0x20e91d53
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "faddi.pi f30, f11, 506\n"                            // opcode=0x7da58f3f
         "maskpopcz x23, m4\n"                                 // opcode=0x54020bfb
         "fcmovm.ps f31, f15, f17\n"                           // opcode=0x01178ff7
         "fsgnjn.s f22, f11, f14\n"                            // opcode=0x20e59b53
         "fround.ps f27, f18, dyn\n"                           // opcode=0x58197dfb
         "flw.ps f24, 35(x15)\n"                               // opcode=0x2a37ac0b
         "flt.ps f0, f11, f12\n"                               // opcode=0xa0c5907b
         "maskor m7, m0, m5\n"                                 // opcode=0x665063fb
         "fnot.pi f28, f13\n"                                  // opcode=0x0606ae7b
         "fsra.pi f24, f12, f14\n"                             // opcode=0x0ee65c7b
         "fmsub.ps f28, f18, f10, f11, dyn\n"                  // opcode=0x5aa97e5b
         "fsub.pi f27, f10, f18\n"                             // opcode=0x0f250dfb
         "fsrai.pi f22, f12, 27\n"                             // opcode=0x4fb67b7b
         "fsub.s f27, f19, f10, dyn\n"                         // opcode=0x08a9fdd3
         "fslli.pi f31, f19, 4\n"                              // opcode=0x4e499ffb
         "fpackrepb.pi f29, f15\n"                             // opcode=0x26078efb
         "fclass.s x21, f14\n"                                 // opcode=0xe0071ad3
         "masknot m3, m6\n"                                    // opcode=0x660321fb
         "fswizz.ps f24, f15, 222\n"                           // opcode=0xe7b7ec7b
         "fsatu8.pi f20, f19\n"                                // opcode=0x0619ba7b
         "fpackreph.pi f26, f13\n"                             // opcode=0x26069d7b
         "fsrl.pi f31, f18, f16\n"                             // opcode=0x07095ffb
         "fmul.ps f25, f14, f17, dyn\n"                        // opcode=0x11177cfb
         "fsgnj.s f27, f16, f12\n"                             // opcode=0x20c80dd3
         "maskxor m1, m2, m2\n"                                // opcode=0x662140fb
         "fmadd.s f21, f11, f18, f14, dyn\n"                   // opcode=0x7125fac3
         "fsll.pi f25, f12, f16\n"                             // opcode=0x07061cfb
         "maskpopcz x20, m5\n"                                 // opcode=0x54028a7b
         "fsrl.pi f20, f10, f10\n"                             // opcode=0x06a55a7b
         "fclass.s x30, f15\n"                                 // opcode=0xe0079f53
         "fmul.ps f20, f16, f10, dyn\n"                        // opcode=0x10a87a7b
         "fsub.ps f29, f12, f19, rmm\n"                        // opcode=0x09364efb
         "fbci.pi f29, 967059\n"                               // opcode=0xec193edf
         "fsgnjx.s f30, f18, f11\n"                            // opcode=0x20b92f53
         "fpackreph.pi f26, f14\n"                             // opcode=0x26071d7b
         "fround.ps f22, f15, rtz\n"                           // opcode=0x58179b7b
         "fsatu8.pi f20, f15\n"                                // opcode=0x0617ba7b
         "fclass.s x21, f17\n"                                 // opcode=0xe0089ad3
         "fxor.pi f24, f11, f13\n"                             // opcode=0x06d5cc7b
         "fcmov.ps f26, f15, f16, f17\n"                       // opcode=0x8d07ad3f
         "fmulh.pi f24, f13, f17\n"                            // opcode=0x1716ac7b
         "fsgnjn.ps f31, f13, f18\n"                           // opcode=0x21269ffb
         "mova.m.x x11\n"                                      // opcode=0xd605907b
         "fbc.ps f23, 104(x11)\n"                              // opcode=0xfe858b8b
         "fclass.s x22, f13\n"                                 // opcode=0xe0069b53
         "fcmov.ps f22, f19, f14, f14\n"                       // opcode=0x74e9ab3f
         "fpackreph.pi f26, f10\n"                             // opcode=0x26051d7b
         "fsgnjn.s f30, f12, f17\n"                            // opcode=0x21161f53
         "fnot.pi f31, f13\n"                                  // opcode=0x0606affb
         "maskpopcz x23, m5\n"                                 // opcode=0x54028bfb
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fsgnj.ps f26, f14, f10\n"                            // opcode=0x20a70d7b
         "fsgnjn.ps f22, f14, f11\n"                           // opcode=0x20b71b7b
         "fbci.ps f25, 850440\n"                               // opcode=0xcfa08c9f
         "fswizz.ps f21, f16, 97\n"                            // opcode=0xe6c81afb
         "feqm.ps m2, f13, f19\n"                              // opcode=0xa136e17b
         "fmulh.pi f21, f18, f12\n"                            // opcode=0x16c92afb
         "fslli.pi f31, f17, 26\n"                             // opcode=0x4fa89ffb
         "fbc.ps f30, 9(x19)\n"                                // opcode=0xe8998f0b
         "mova.m.x x18\n"                                      // opcode=0xd609107b
         "maskpopcz x24, m7\n"                                 // opcode=0x54038c7b
         "flw f20, 39(x14)\n"                                  // opcode=0x9a772a07
         "cubesgnsc.ps f27, f12, f17\n"                        // opcode=0x89162dfb
         "fbcx.ps f0, x10\n"                                   // opcode=0x0005300b
         "fbci.ps f29, 105421\n"                               // opcode=0x19bcde9f
         "fsrl.pi f20, f10, f16\n"                             // opcode=0x07055a7b
         "cubesgntc.ps f29, f13, f19\n"                        // opcode=0x8936befb
         "fandi.pi f21, f15, 04\n"                            // opcode=0xb4079abf
         "maskxor m6, m3, m7\n"                                // opcode=0x6671c37b
         "cubesgntc.ps f27, f19, f13\n"                        // opcode=0x88d9bdfb
         "fbc.ps f30, 29(x17)\n"                               // opcode=0xe9d88f0b
         "fpackreph.pi f25, f18\n"                             // opcode=0x26091cfb
         "fmsub.ps f21, f15, f16, f15, rne\n"                  // opcode=0x7b078adb
         "fle.ps f30, f12, f18\n"                              // opcode=0xa1260f7b
         "fmul.pi f26, f15, f15\n"                             // opcode=0x16f78d7b
         "fbc.ps f0, 48(x12)\n"                                // opcode=0xcb06000b
         "fcmov.ps f29, f11, f14, f19\n"                       // opcode=0x9ce5aebf
         "fsgnjn.ps f21, f17, f14\n"                           // opcode=0x20e89afb
         "fbcx.ps f0, x12\n"                                   // opcode=0x0006300b
         "fbcx.ps f20, x16\n"                                  // opcode=0x00083a0b
         "cubesgnsc.ps f27, f17, f11\n"                        // opcode=0x88b8adfb
         "fmadd.s f31, f16, f15, f11, rne\n"                   // opcode=0x58f80fc3
         "faddi.pi f31, f11, 59\n"                            // opcode=0xedf58fbf
         "for.pi f22, f14, f13\n"                              // opcode=0x06d76b7b
         "feq.pi f25, f16, f15\n"                              // opcode=0xa6f82cfb
         "fmsub.s f25, f18, f14, f14, dyn\n"                   // opcode=0x70e97cc7
         "fle.ps f23, f12, f16\n"                              // opcode=0xa1060bfb
         "fsgnj.s f27, f18, f14\n"                             // opcode=0x20e90dd3
         "fsetm.pi m7, f15\n"                                  // opcode=0xe007d3fb
         "flw.ps f24, 73(x15)\n"                               // opcode=0x7c97ac0b
         "maskpopcz x26, m3\n"                                 // opcode=0x54018d7b
         "feqm.ps m4, f18, f15\n"                              // opcode=0xa0f9627b
         "flw f21, 107(x18)\n"                                 // opcode=0xe6b92a87
         "cubeface.ps f23, f13, f11\n"                         // opcode=0x88b68bfb
         "fbcx.ps f24, x12\n"                                  // opcode=0x00063c0b
         "fmsub.s f21, f12, f15, f11, dyn\n"                   // opcode=0x58f67ac7
         "fsrl.pi f31, f19, f19\n"                             // opcode=0x0739dffb
         "masknot m6, m0\n"                                    // opcode=0x6600237b
         "fbci.ps f29, 1020454\n"                              // opcode=0xf9226e9f
         "fbcx.ps f23, x17\n"                                  // opcode=0x0008bb8b
         "maskor m0, m2, m6\n"                                 // opcode=0x6661607b
         "li x5, 0xf0f0\n"
         "mova.m.x x5\n"
         "cubesgnsc.ps f27, f18, f10\n"                        // opcode=0x88a92dfb
         "fmadd.s f0, f13, f16, f19, dyn\n"                    // opcode=0x9906f043
         "fsgnj.s f20, f15, f14\n"                             // opcode=0x20e78a53
         "fsll.pi f30, f18, f13\n"                             // opcode=0x06d91f7b
         "fle.pi f27, f16, f19\n"                              // opcode=0xa7380dfb
         "fsetm.pi m5, f13\n"                                  // opcode=0xe006d2fb
         "fadd.ps f22, f15, f11, dyn\n"                        // opcode=0x00b7fb7b
         "fsrli.pi f20, f19, 30\n"                             // opcode=0x4fe9da7b
         "cubefaceidx.ps f21, f13, f15\n"                      // opcode=0x88f69afb
         "fandi.pi f20, f16, 55\n"                             // opcode=0x0d781a3f
         "fpackreph.pi f29, f14\n"                             // opcode=0x26071efb
         "maskpopcz x23, m6\n"                                 // opcode=0x54030bfb
         "cubesgntc.ps f21, f10, f13\n"                        // opcode=0x88d53afb
         "fsgnjn.s f27, f13, f14\n"                            // opcode=0x20e69dd3
         "fbcx.ps f20, x19\n"                                  // opcode=0x0009ba0b
         "for.pi f28, f19, f10\n"                              // opcode=0x06a9ee7b
         "feqm.ps m6, f13, f12\n"                              // opcode=0xa0c6e37b
         "faddi.pi f25, f13, 73\n"                            // opcode=0xf4d68cbf
         "fltm.ps m7, f19, f11\n"                              // opcode=0xa0b9d3fb
         "fsatu8.pi f25, f14\n"                                // opcode=0x06173cfb
         "maskpopc x29, m0\n"                                  // opcode=0x52000efb
         "cubesgnsc.ps f20, f19, f18\n"                        // opcode=0x8929aa7b
         "fnot.pi f24, f17\n"                                  // opcode=0x0608ac7b
         "fpackrepb.pi f22, f17\n"                             // opcode=0x26088b7b
         "fsrli.pi f25, f19, 10\n"                             // opcode=0x4ea9dcfb
         "cubesgnsc.ps f21, f18, f15\n"                        // opcode=0x88f92afb
         "mov.m.x m7, x18, 93\n"                               // opcode=0x56b953fb
         "fltu.pi f23, f15, f16\n"                             // opcode=0xa707bbfb
         "fround.ps f21, f15, dyn\n"                           // opcode=0x5817fafb
         "fclass.ps f25, f16\n"                                // opcode=0xe0081cfb
         "cubesgnsc.ps f31, f14, f17\n"                        // opcode=0x89172ffb
         "fmulh.pi f26, f10, f13\n"                            // opcode=0x16d52d7b
         "fpackrepb.pi f31, f12\n"                             // opcode=0x26060ffb
         "feqm.ps m4, f11, f16\n"                              // opcode=0xa105e27b
         "flem.ps m7, f10, f15\n"                              // opcode=0xa0f543fb
         "fle.pi f30, f12, f13\n"                              // opcode=0xa6d60f7b
         "fmulh.pi f25, f15, f19\n"                            // opcode=0x1737acfb
         "maskand m4, m3, m1\n"                                // opcode=0x6611f27b
         "feq.pi f24, f12, f13\n"                              // opcode=0xa6d62c7b
         "maskand m4, m5, m7\n"                                // opcode=0x6672f27b
         "fsgnj.ps f20, f15, f15\n"                            // opcode=0x20f78a7b
         "cubefaceidx.ps f31, f10, f14\n"                      // opcode=0x88e51ffb
         "flt.pi f31, f15, f15\n"                              // opcode=0xa6f79ffb
         "fround.ps f30, f19, dyn\n"                           // opcode=0x5819ff7b
         "mova.x.m x24\n"                                      // opcode=0xd6000c7b
         "fsat8.pi f28, f13\n"                                 // opcode=0x0606be7b
         "feq.pi f23, f18, f16\n"                              // opcode=0xa7092bfb
         "maskpopcz x29, m2\n"                                 // opcode=0x54010efb
         "masknot m3, m5\n"                                    // opcode=0x6602a1fb
         "fsgnjn.s f29, f15, f15\n"                            // opcode=0x20f79ed3
         "li x5, 660\n"
         "mova.m.x x5\n"
         "flt.pi f22, f14, f11\n"                              // opcode=0xa6b71b7b
         "fmulhu.pi f22, f14, f15\n"                           // opcode=0x16f71b7b
         "flt.s x25, f12, f13\n"                               // opcode=0xa0d61cd3
         "cubesgntc.ps f24, f15, f17\n"                        // opcode=0x8917bc7b
         "fclass.s x27, f11\n"                                 // opcode=0xe0059dd3
         "feq.ps f28, f10, f18\n"                              // opcode=0xa1252e7b
         "fbcx.ps f24, x16\n"                                  // opcode=0x00083c0b
         "fxor.pi f25, f17, f10\n"                             // opcode=0x06a8ccfb
         "fadd.pi f29, f16, f19\n"                             // opcode=0x07380efb
         "flt.pi f21, f13, f15\n"                              // opcode=0xa6f69afb
         "fbc.ps f23, 23(x18)\n"                               // opcode=0xc9790b8b
         "maskpopc x28, m0\n"                                  // opcode=0x52000e7b
         "fsgnjn.ps f29, f16, f14\n"                           // opcode=0x20e81efb
         "fslli.pi f25, f17, 0\n"                              // opcode=0x4e089cfb
         "feqm.ps m1, f13, f11\n"                              // opcode=0xa0b6e0fb
         "fsrli.pi f23, f13, 20\n"                             // opcode=0x4f46dbfb
         "fltm.pi m3, f16, f18\n"                              // opcode=0x3f2801fb
         "fmulh.pi f20, f14, f14\n"                            // opcode=0x16e72a7b
         "maskpopc x21, m1\n"                                  // opcode=0x52008afb
         "fle.ps f27, f12, f12\n"                              // opcode=0xa0c60dfb
         "fsw f10, 40(x11)\n"                                  // opcode=0xa2a5a427
         "fmsub.ps f27, f13, f15, f17, dyn\n"                  // opcode=0x8af6fddb
         "fle.ps f20, f15, f10\n"                              // opcode=0xa0a78a7b
         "fltm.ps m1, f13, f16\n"                              // opcode=0xa106d0fb
         "fle.ps f26, f13, f10\n"                              // opcode=0xa0a68d7b
         "feq.s x20, f15, f15\n"                               // opcode=0xa0f7aa53
         "fpackrepb.pi f29, f17\n"                             // opcode=0x26088efb
         "fmul.s f20, f19, f16, dyn\n"                         // opcode=0x1109fa53
         "fsat8.pi f23, f17\n"                                 // opcode=0x0608bbfb
         "fsrli.pi f0, f10, 9\n"                               // opcode=0x4e95507b
         "feq.ps f20, f15, f13\n"                              // opcode=0xa0d7aa7b
         "fcmovm.ps f29, f10, f15\n"                           // opcode=0x00f50ef7
         "fsgnjn.ps f23, f16, f16\n"                           // opcode=0x21081bfb
         "fsw f16, 19(x15)\n"                                  // opcode=0xf907a9a7
         "fadd.s f30, f13, f19, dyn\n"                         // opcode=0x0136ff53
         "fsetm.pi m3, f13\n"                                  // opcode=0xe006d1fb
         "fbci.pi f20, 405135\n"                               // opcode=0x62e8fa5f
         "maskpopc x24, m4\n"                                  // opcode=0x52020c7b
         "fmulh.pi f31, f16, f11\n"                            // opcode=0x16b82ffb
         "fbc.ps f0, 94(x13)\n"                                // opcode=0xade6800b
         "fsgnjn.ps f25, f14, f10\n"                           // opcode=0x20a71cfb
         "maskpopcz x23, m3\n"                                 // opcode=0x54018bfb
         "fltu.pi f31, f13, f16\n"                             // opcode=0xa706bffb
         "maskpopc x23, m7\n"                                  // opcode=0x52038bfb
         "fbci.ps f28, 650162\n"                               // opcode=0x9ebb2e1f
         "maskand m2, m7, m7\n"                                // opcode=0x6673f17b
         "fsgnj.s f20, f16, f11\n"                             // opcode=0x20b80a53
         "cubefaceidx.ps f30, f11, f17\n"                      // opcode=0x89159f7b
         "for.pi f31, f17, f11\n"                              // opcode=0x06b8effb
         "fsll.pi f30, f19, f13\n"                             // opcode=0x06d99f7b
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "maskpopc x24, m3\n"                                  // opcode=0x52018c7b
         "maskand m4, m7, m7\n"                                // opcode=0x6673f27b
         "maskpopcz x21, m1\n"                                 // opcode=0x54008afb
         "fbcx.ps f28, x18\n"                                  // opcode=0x00093e0b
         "flw f27, 99(x16)\n"                                  // opcode=0xee382d87
         "fltu.pi f30, f12, f19\n"                             // opcode=0xa7363f7b
         "maskpopc x28, m4\n"                                  // opcode=0x52020e7b
         "flw f27, 1(x13)\n"                                   // opcode=0x2816ad87
         "cubefaceidx.ps f22, f16, f11\n"                      // opcode=0x88b81b7b
         "mova.m.x x12\n"                                      // opcode=0xd606107b
         "maskpopcz x26, m5\n"                                 // opcode=0x54028d7b
         "fsgnj.ps f21, f17, f19\n"                            // opcode=0x21388afb
         "maskpopc x0, m7\n"                                   // opcode=0x5203807b
         "fround.ps f21, f12, dyn\n"                           // opcode=0x58167afb
         "feq.s x30, f10, f12\n"                               // opcode=0xa0c52f53
         "for.pi f22, f14, f14\n"                              // opcode=0x06e76b7b
         "fcmov.ps f28, f19, f14, f11\n"                       // opcode=0x5ce9ae3f
         "cubeface.ps f29, f19, f15\n"                         // opcode=0x88f98efb
         "fsgnjx.ps f0, f18, f18\n"                            // opcode=0x2129207b
         "fsatu8.pi f29, f13\n"                                // opcode=0x0616befb
         "fclass.ps f27, f12\n"                                // opcode=0xe0061dfb
         "fnot.pi f31, f11\n"                                  // opcode=0x0605affb
         "fsub.ps f0, f12, f11, rne\n"                         // opcode=0x08b6007b
         "maskpopc.rast x29, m5, m2, 13\n"                     // opcode=0x5fa68efb
         "fmsub.s f23, f12, f11, f12, dyn\n"                   // opcode=0x60b67bc7
         "feq.s x28, f19, f12\n"                               // opcode=0xa0c9ae53
         "fsgnjn.ps f22, f13, f18\n"                           // opcode=0x21269b7b
         "fsw f18, 78(x15)\n"                                  // opcode=0x9527a727
         "flw.ps f26, 15(x16)\n"                               // opcode=0xf0f82d0b
         "cubefaceidx.ps f21, f14, f13\n"                      // opcode=0x88d71afb
         "fsub.ps f27, f11, f15, dyn\n"                        // opcode=0x08f5fdfb
         "fsub.ps f25, f14, f19, dyn\n"                        // opcode=0x09377cfb
         "fswizz.ps f31, f13, 140\n"                           // opcode=0xe716cffb
         "fmulhu.pi f28, f10, f11\n"                           // opcode=0x16b51e7b
         "fxor.pi f26, f19, f19\n"                             // opcode=0x0739cd7b
         "flt.pi f27, f18, f13\n"                              // opcode=0xa6d91dfb
         "fsgnjn.ps f27, f16, f11\n"                           // opcode=0x20b81dfb
         "fsra.pi f22, f17, f10\n"                             // opcode=0x0ea8db7b
         "fmadd.s f21, f14, f17, f15, dyn\n"                   // opcode=0x79177ac3
         "faddi.pi f29, f11, 68\n"                            // opcode=0xa4858ebf
         "feq.s x0, f11, f16\n"                                // opcode=0xa105a053
         "fmsub.s f21, f19, f19, f19, rne\n"                   // opcode=0x9939aac7
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
            );
   C_TEST_PASS;
   return 0;
}
