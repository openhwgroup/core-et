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
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0x00000000,    // zero                  
      0x3f028f5c,    // 0.51                  

      // vpu register f3
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0x00000000,    // zero                  
      0x3f028f5c,    // 0.51                  
      0x80000000,    // -zero                 
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f4
      0x4b000000,    // 8388608.0             
      0x7f800000,    // inf                   
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            

      // vpu register f5
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0xcb8c4b40,    // -18388608.0           
      0x7f800000,    // inf                   

      // vpu register f6
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f7
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   
      0xffc00001,    // -signaling NaN        
      0xbf800000,    // -1.0                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xff800000,    // -inf                  
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  

      // vpu register f8
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0x80000000,    // -zero                 
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            

      // vpu register f9
      0x80011111,    // -9.7958E-41           
      0xffc00000,    // -quiet NaN            
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            
      0x80000000,    // -zero                 
      0x80000000,    // -zero                 
      0x7f800000,    // inf                   
      0x4b000000,    // 8388608.0             

      // vpu register f10
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x7fc00000,    // quiet NaN             
      0xbf800000,    // -1.0                  
      0x7fc00000,    // quiet NaN             
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0xcb8c4b40,    // -18388608.0           

      // vpu register f11
      0x3f028f5c,    // 0.51                  
      0xffc00000,    // -quiet NaN            
      0x7f800000,    // inf                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80011111,    // -9.7958E-41           
      0xff800000,    // -inf                  
      0x80000000,    // -zero                 
      0x4b8c4b40,    // 18388608.0            

      // vpu register f12
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0xff800000,    // -inf                  
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            

      // vpu register f13
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        

      // vpu register f14
      0x4b000000,    // 8388608.0             
      0x80000000,    // -zero                 
      0x00000000,    // zero                  
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000000,    // zero                  

      // vpu register f15
      0x7f800000,    // inf                   
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0xff800000,    // -inf                  

      // vpu register f16
      0xffc00000,    // -quiet NaN            
      0x00011111,    // 9.7958E-41            
      0x7f800000,    // inf                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x7fc00001,    // signaling NaN         
      0xbf800000,    // -1.0                  
      0x7fc00000,    // quiet NaN             

      // vpu register f17
      0xbf800000,    // -1.0                  
      0x7fc00000,    // quiet NaN             
      0xffc00001,    // -signaling NaN        
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             
      0x80011111,    // -9.7958E-41           
      0x80000000,    // -zero                 

      // vpu register f18
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0x80000000,    // -zero                 
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0xffc00000,    // -quiet NaN            
      0xcb000000,    // -8388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f19
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  

      // vpu register f20
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x7fc00001,    // signaling NaN         
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         

      // vpu register f21
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  
      0x80011111,    // -9.7958E-41           
      0xff800000,    // -inf                  
      0x7f800000,    // inf                   
      0x00000000,    // zero                  
      0x7fc00001,    // signaling NaN         
      0x7f800000,    // inf                   

      // vpu register f22
      0x7f800000,    // inf                   
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x7fc00001,    // signaling NaN         
      0x7f800000,    // inf                   
      0x4b000000,    // 8388608.0             
      0xffc00001,    // -signaling NaN        

      // vpu register f23
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0x7fc00000,    // quiet NaN             
      0x4b000000,    // 8388608.0             
      0x7fc00000,    // quiet NaN             
      0x80000000,    // -zero                 
      0xcb8c4b40,    // -18388608.0           

      // vpu register f24
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0xcb000000,    // -8388608.0            
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f25
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  

      // vpu register f26
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0xff800000,    // -inf                  

      // vpu register f27
      0x00000000,    // zero                  
      0x7fc00001,    // signaling NaN         
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x7fc00001,    // signaling NaN         

      // vpu register f28
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  
      0xff800000,    // -inf                  

      // vpu register f29
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x7fc00000,    // quiet NaN             
      0xcb8c4b40,    // -18388608.0           
      0x80000000,    // -zero                 
      0x7f800000,    // inf                   
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  

      // vpu register f30
      0x4b000000,    // 8388608.0             
      0xffc00000,    // -quiet NaN            
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            

      // vpu register f31
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000000,    // -zero                 
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
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
         "j SET_UP_EXCEPTION_VECTOR\n"
         ".align 12\n"
         "M_MODE_E_H:\n"
         "csrrw x0, 0x342,x0\n"      // mcause = 0
         "csrrw x0, 0x343,x0\n"      // mtval = 0
         "csrr	x19, 0x341\n"       // x19 = mepc
         "addi	x19,x19,4\n"        // x19+=4
         "csrw	 0x341,x19\n"       // mepc = x19
         "mret\n"                    // return
         ".align 12\n"
         "S_MODE_E_H:\n"
         "csrrw x0, 0x142,x0\n"
         "csrrw x0, 0x143,x0\n"
         "csrr	x19, 0x141\n"
         "addi	x19,x19,4\n"
         "csrw	 0x141,x19\n"
         "sret\n"
         ".align 12\n"
         "SET_UP_EXCEPTION_VECTOR:\n"
         "la x5, M_MODE_E_H\n"
         "csrrw x0, 0x305, x5\n"     // mtvec = M_MODE_E_H
         "la x5, S_MODE_E_H\n"
         "csrrw x0, 0x105, x5\n"
         "VPU_SEQ_START:\n"
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
         "maskpopc x22, m1\n"                                  // opcode=0x52008b7b
         "fsat8.pi f22, f14\n"                                 // opcode=0x06073b7b
         "fmadd.ps f20, f18, f11, f13, dyn\n"                  // opcode=0x68b97a5b
         "masknot m0, m2\n"                                    // opcode=0x6601207b
         "feq.pi f29, f10, f18\n"                              // opcode=0xa7252efb
         "flw.ps f26, 24(x11)\n"                               // opcode=0x7985ad0b
         "maskor m2, m2, m0\n"                                 // opcode=0x6601617b
         "fle.pi f30, f11, f16\n"                              // opcode=0xa7058f7b
         "fsgnj.s f21, f16, f17\n"                             // opcode=0x21180ad3
         "fsgnj.s f21, f18, f12\n"                             // opcode=0x20c90ad3
         "fmsub.s f29, f12, f17, f12, rmm\n"                   // opcode=0x61164ec7
         "maskpopc x30, m6\n"                                  // opcode=0x52030f7b
         "fmadd.ps f30, f14, f17, f14, dyn\n"                  // opcode=0x71177f5b
         "feq.pi f24, f14, f10\n"                              // opcode=0xa6a72c7b
         "fsat8.pi f24, f11\n"                                 // opcode=0x0605bc7b
         "fmadd.ps f0, f19, f18, f16, dyn\n"                   // opcode=0x8129f05b
         "fsgnj.s f25, f13, f13\n"                             // opcode=0x20d68cd3
         "fsgnjx.s f0, f16, f17\n"                             // opcode=0x21182053
         "fltu.pi f0, f14, f17\n"                              // opcode=0xa717307b
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
