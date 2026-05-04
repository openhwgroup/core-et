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
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x00000000,    // zero                  
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            
      0x7f800000,    // inf                   

      // vpu register f3
      0xbf028f5c,    // -0.51                 
      0xff800000,    // -inf                  
      0x7fc00001,    // signaling NaN         
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0x80000000,    // -zero                 
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   

      // vpu register f4
      0x00011111,    // 9.7958E-41            
      0x7fc00000,    // quiet NaN             
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             
      0x7fc00001,    // signaling NaN         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 

      // vpu register f5
      0x00000000,    // zero                  
      0x80000000,    // -zero                 
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        

      // vpu register f6
      0x4b8c4b40,    // 18388608.0            
      0x80000000,    // -zero                 
      0xffc00000,    // -quiet NaN            
      0x3f800000,    // 1.0                   
      0x80000000,    // -zero                 
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         

      // vpu register f7
      0xbf028f5c,    // -0.51                 
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0x00000000,    // zero                  
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            

      // vpu register f8
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  
      0xff800000,    // -inf                  
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0x7fc00001,    // signaling NaN         

      // vpu register f9
      0x3f028f5c,    // 0.51                  
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x00000000,    // zero                  
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   

      // vpu register f10
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x7fc00001,    // signaling NaN         
      0xcb000000,    // -8388608.0            
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           

      // vpu register f11
      0x80000000,    // -zero                 
      0x00011111,    // 9.7958E-41            
      0xff800000,    // -inf                  
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  

      // vpu register f12
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00001,    // -signaling NaN        
      0x7f800000,    // inf                   
      0x80000000,    // -zero                 
      0x00000000,    // zero                  
      0xbf800000,    // -1.0                  
      0x80000000,    // -zero                 
      0x3f800000,    // 1.0                   

      // vpu register f13
      0x7f800000,    // inf                   
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   

      // vpu register f14
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7f800000,    // inf                   
      0x80000000,    // -zero                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             
      0x7f800000,    // inf                   

      // vpu register f15
      0x007fffff,    // 1.1754942E-38         
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         

      // vpu register f16
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  

      // vpu register f17
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0x4b000000,    // 8388608.0             
      0xff800000,    // -inf                  

      // vpu register f18
      0x7fc00001,    // signaling NaN         
      0xcb8c4b40,    // -18388608.0           
      0x7fc00001,    // signaling NaN         
      0x4b8c4b40,    // 18388608.0            
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  

      // vpu register f19
      0xffc00001,    // -signaling NaN        
      0x80000000,    // -zero                 
      0x7fc00000,    // quiet NaN             
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0x4b8c4b40,    // 18388608.0            
      0x7f800000,    // inf                   
      0x00000000,    // zero                  

      // vpu register f20
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0x3f800000,    // 1.0                   
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x80000000,    // -zero                 
      0xcb000000,    // -8388608.0            
      0x7f800000,    // inf                   

      // vpu register f21
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           
      0x7fc00001,    // signaling NaN         
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         
      0x4b000000,    // 8388608.0             
      0xff800000,    // -inf                  
      0x4b8c4b40,    // 18388608.0            

      // vpu register f22
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   
      0x007fffff,    // 1.1754942E-38         
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00001,    // -signaling NaN        
      0xffc00000,    // -quiet NaN            

      // vpu register f23
      0x4b000000,    // 8388608.0             
      0x7fc00000,    // quiet NaN             
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0xff800000,    // -inf                  
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 

      // vpu register f24
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0x007fffff,    // 1.1754942E-38         
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0xff800000,    // -inf                  

      // vpu register f25
      0xff800000,    // -inf                  
      0x80011111,    // -9.7958E-41           
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0xff800000,    // -inf                  
      0x007fffff,    // 1.1754942E-38         
      0xffc00000,    // -quiet NaN            
      0x7fc00000,    // quiet NaN             

      // vpu register f26
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00000,    // -quiet NaN            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000000,    // -zero                 

      // vpu register f27
      0x80011111,    // -9.7958E-41           
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             
      0xffc00001,    // -signaling NaN        
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0x7fc00001,    // signaling NaN         

      // vpu register f28
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0xff800000,    // -inf                  
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xff800000,    // -inf                  
      0xffc00000,    // -quiet NaN            

      // vpu register f29
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             

      // vpu register f30
      0x4b8c4b40,    // 18388608.0            
      0x4b000000,    // 8388608.0             
      0xff800000,    // -inf                  
      0x007fffff,    // 1.1754942E-38         
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            
      0xff800000,    // -inf                  
      0xffc00000,    // -quiet NaN            

      // vpu register f31
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
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
         "li x1,  0x80cd0300\n"
         "li x2,  0x80c5de60\n"
         "li x3,  0x80178b40\n"
         "li x4,  0x80ca07c0\n"
         "li x5,  0x80241ca0\n"
         "li x6,  0x803f7460\n"
         "li x7,  0x809f4e60\n"
         "li x8,  0x802b38e0\n"
         "li x9,  0x80cef500\n"
         "li x10,  0x809539c0\n"
         "li x11,  0x808ca6e0\n"
         "li x12,  0x80c18520\n"
         "li x13,  0x807b1da0\n"
         "li x14,  0x8092f000\n"
         "li x15,  0x80febd80\n"
         "li x16,  0x807f1ca0\n"
         "li x17,  0x80bfe100\n"
         "li x18,  0x80d39d80\n"
         "li x19,  0x80280620\n"
         "li x20,  0x802f6220\n"
         "li x21,  0x8085dfc0\n"
         "li x22,  0x80fa9ae0\n"
         "li x23,  0x80b5a900\n"
         "li x24,  0x80acc080\n"
         "li x25,  0x8030d600\n"
         "li x26,  0x80c71b80\n"
         "li x27,  0x80f65b00\n"
         "li x28,  0x809321e0\n"
         "li x29,  0x802f8ee0\n"
         "li x30,  0x8055d300\n"
         "li x31,  0x80362ca0\n"
         "fsub.pi f9, f9, f17\n"                               // opcode=0x0f1484fb
         "fadd.pi f14, f14, f13\n"                             // opcode=0x06d7077b
         "fsub.pi f15, f7, f10\n"                              // opcode=0x0ea387fb
         "fadd.pi f14, f25, f31\n"                             // opcode=0x07fc877b
         "fsub.pi f6, f14, f31\n"                              // opcode=0x0ff7037b
         "fadd.pi f31, f31, f17\n"                             // opcode=0x071f8ffb
         "fsub.pi f3, f28, f21\n"                              // opcode=0x0f5e01fb
         "fsub.pi f22, f22, f19\n"                             // opcode=0x0f3b0b7b
         "fsub.pi f24, f9, f27\n"                              // opcode=0x0fb48c7b
         "fadd.pi f31, f3, f2\n"                               // opcode=0x06218ffb
         "fsub.pi f9, f23, f17\n"                              // opcode=0x0f1b84fb
         "fadd.pi f29, f8, f26\n"                              // opcode=0x07a40efb
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
