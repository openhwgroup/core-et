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
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           

      // vpu register f3
      0x4b000000,    // 8388608.0             
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x7fc00000,    // quiet NaN             
      0xffc00000,    // -quiet NaN            
      0x7fc00000,    // quiet NaN             

      // vpu register f4
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   

      // vpu register f5
      0x7fc00001,    // signaling NaN         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            

      // vpu register f6
      0x3f800000,    // 1.0                   
      0x80011111,    // -9.7958E-41           
      0x00000000,    // zero                  
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             

      // vpu register f7
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0xbf800000,    // -1.0                  
      0x7f800000,    // inf                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  

      // vpu register f8
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0x7fc00000,    // quiet NaN             

      // vpu register f9
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0xff800000,    // -inf                  
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0xffc00001,    // -signaling NaN        

      // vpu register f10
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000000,    // zero                  
      0x4b000000,    // 8388608.0             
      0x7fc00000,    // quiet NaN             
      0x00000000,    // zero                  
      0xffc00001,    // -signaling NaN        

      // vpu register f11
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0x7f800000,    // inf                   
      0xcb8c4b40,    // -18388608.0           
      0x80000000,    // -zero                 
      0x7fc00001,    // signaling NaN         
      0x80011111,    // -9.7958E-41           
      0xbf028f5c,    // -0.51                 

      // vpu register f12
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0xff800000,    // -inf                  
      0xffc00001,    // -signaling NaN        
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  

      // vpu register f13
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 
      0x7fc00001,    // signaling NaN         
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf028f5c,    // -0.51                 

      // vpu register f14
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0xffc00000,    // -quiet NaN            

      // vpu register f15
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0xff800000,    // -inf                  
      0x7f800000,    // inf                   
      0x3f800000,    // 1.0                   
      0xffc00001,    // -signaling NaN        
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            

      // vpu register f16
      0x3f800000,    // 1.0                   
      0x007fffff,    // 1.1754942E-38         
      0xffc00000,    // -quiet NaN            
      0xcb8c4b40,    // -18388608.0           
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0xff800000,    // -inf                  

      // vpu register f17
      0x007fffff,    // 1.1754942E-38         
      0x7f800000,    // inf                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0xff800000,    // -inf                  
      0xcb8c4b40,    // -18388608.0           

      // vpu register f18
      0x4b000000,    // 8388608.0             
      0xbf800000,    // -1.0                  
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0xff800000,    // -inf                  
      0xcb8c4b40,    // -18388608.0           
      0x7fc00000,    // quiet NaN             

      // vpu register f19
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 

      // vpu register f20
      0x7f800000,    // inf                   
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f21
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b000000,    // 8388608.0             
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   

      // vpu register f22
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0xff800000,    // -inf                  
      0x7fc00001,    // signaling NaN         
      0x80000000,    // -zero                 
      0xbf028f5c,    // -0.51                 
      0x4b8c4b40,    // 18388608.0            

      // vpu register f23
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  
      0xffc00001,    // -signaling NaN        
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x3f028f5c,    // 0.51                  
      0xcb8c4b40,    // -18388608.0           

      // vpu register f24
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0x00000000,    // zero                  
      0xcb8c4b40,    // -18388608.0           
      0xffc00001,    // -signaling NaN        
      0xffc00001,    // -signaling NaN        
      0xff800000,    // -inf                  
      0x4b000000,    // 8388608.0             

      // vpu register f25
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0xffc00000,    // -quiet NaN            
      0x7f800000,    // inf                   

      // vpu register f26
      0xffc00001,    // -signaling NaN        
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0x7fc00000,    // quiet NaN             
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            

      // vpu register f27
      0x80011111,    // -9.7958E-41           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x7fc00001,    // signaling NaN         
      0x4b000000,    // 8388608.0             

      // vpu register f28
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00001,    // -signaling NaN        
      0x80000000,    // -zero                 

      // vpu register f29
      0x00000000,    // zero                  
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0x7f800000,    // inf                   
      0x00011111,    // 9.7958E-41            
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  

      // vpu register f30
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0x7fc00000,    // quiet NaN             
      0x007fffff,    // 1.1754942E-38         
      0x7fc00001,    // signaling NaN         
      0x7fc00000,    // quiet NaN             
      0x00000000,    // zero                  
      0xbf028f5c,    // -0.51                 

      // vpu register f31
      0x80011111,    // -9.7958E-41           
      0x4b8c4b40,    // 18388608.0            
      0xffc00001,    // -signaling NaN        
      0x3f800000,    // 1.0                   
      0x7fc00001,    // signaling NaN         
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        
      0x3f800000     // 1.0                   
   };
   C_TEST_START; 
   __asm__ __volatile__ (
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "flw.ps f15,  480 (%[operands])\n"
         "faddi.pi f26, f15, 102\n"  // opcode=0x1c678d3f
         "nop\n"
         "nop\n"
         "nop\n"
         "csrr x5, fflags\n" 
         // Asm output results to C code
         :
         // Asm input data from C code
         : [operands] "r" (operands)
         // Registers used in Asm so C knows they change
         : "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19",  "f15"
         );
   C_TEST_PASS;
   return 0;
}
