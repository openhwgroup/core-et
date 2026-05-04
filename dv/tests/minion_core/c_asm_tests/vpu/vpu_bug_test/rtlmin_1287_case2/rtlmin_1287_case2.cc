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
      0xffc00001,    // -signaling NaN        
      0xcb000000,    // -8388608.0            
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0x7fc00001,    // signaling NaN         
      0x00000000,    // zero                  
      0x4b8c4b40,    // 18388608.0            

      // vpu register f3
      0x00011111,    // 9.7958E-41            
      0x7f800000,    // inf                   
      0xffc00000,    // -quiet NaN            
      0x7fc00000,    // quiet NaN             
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            

      // vpu register f4
      0xffc00000,    // -quiet NaN            
      0xffc00001,    // -signaling NaN        
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            

      // vpu register f5
      0xffc00000,    // -quiet NaN            
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0xffc00001,    // -signaling NaN        
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            

      // vpu register f6
      0x4b8c4b40,    // 18388608.0            
      0xbf800000,    // -1.0                  
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x7fc00001,    // signaling NaN         

      // vpu register f7
      0x7f800000,    // inf                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            

      // vpu register f8
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         
      0x4b000000,    // 8388608.0             
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0x007fffff,    // 1.1754942E-38         

      // vpu register f9
      0x00000000,    // zero                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  

      // vpu register f10
      0x3f028f5c,    // 0.51                  
      0xbf800000,    // -1.0                  
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b000000,    // 8388608.0             
      0x80000000,    // -zero                 
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f11
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  
      0xffc00000,    // -quiet NaN            
      0xffc00001,    // -signaling NaN        

      // vpu register f12
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x007fffff,    // 1.1754942E-38         
      0xffc00000,    // -quiet NaN            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           

      // vpu register f13
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x80000000,    // -zero                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf800000,    // -1.0                  

      // vpu register f14
      0xff800000,    // -inf                  
      0x00000000,    // zero                  
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  
      0x80000000,    // -zero                 
      0xffc00001,    // -signaling NaN        
      0x7fc00000,    // quiet NaN             
      0xcb8c4b40,    // -18388608.0           

      // vpu register f15
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000000,    // -zero                 
      0x7fc00000,    // quiet NaN             
      0x7f800000,    // inf                   

      // vpu register f16
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x80011111,    // -9.7958E-41           
      0xff800000,    // -inf                  
      0xffc00000,    // -quiet NaN            
      0x4b8c4b40,    // 18388608.0            

      // vpu register f17
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   

      // vpu register f18
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0xcb000000,    // -8388608.0            
      0xffc00000,    // -quiet NaN            
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             

      // vpu register f19
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0x7fc00001,    // signaling NaN         
      0xffc00001,    // -signaling NaN        
      0xffc00001,    // -signaling NaN        
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   

      // vpu register f20
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0x00000000,    // zero                  
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0xffc00000,    // -quiet NaN            
      0xffc00001,    // -signaling NaN        

      // vpu register f21
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00001,    // -signaling NaN        
      0x4b8c4b40,    // 18388608.0            
      0xff800000,    // -inf                  
      0x3f028f5c,    // 0.51                  
      0x00000000,    // zero                  

      // vpu register f22
      0xbf800000,    // -1.0                  
      0x7fc00001,    // signaling NaN         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x4b000000,    // 8388608.0             
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  

      // vpu register f23
      0x00000000,    // zero                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0xff800000,    // -inf                  
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7fc00001,    // signaling NaN         
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f24
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             
      0xffc00001,    // -signaling NaN        
      0xbf800000,    // -1.0                  
      0x80000000,    // -zero                 
      0x007fffff,    // 1.1754942E-38         
      0x007fffff,    // 1.1754942E-38         

      // vpu register f25
      0x4b8c4b40,    // 18388608.0            
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  
      0x007fffff,    // 1.1754942E-38         
      0x7f800000,    // inf                   

      // vpu register f26
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00001,    // -signaling NaN        
      0x00000001,    // 1.4E-45 (+denorm)     
      0xff800000,    // -inf                  
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f27
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            
      0x00000000,    // zero                  
      0x4b8c4b40,    // 18388608.0            
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0x7fc00000,    // quiet NaN             

      // vpu register f28
      0xffc00000,    // -quiet NaN            
      0xcb000000,    // -8388608.0            
      0xffc00001,    // -signaling NaN        
      0x80000000,    // -zero                 
      0xffc00001,    // -signaling NaN        
      0x4b8c4b40,    // 18388608.0            
      0xff800000,    // -inf                  
      0x7fc00001,    // signaling NaN         

      // vpu register f29
      0x3f800000,    // 1.0                   
      0xff800000,    // -inf                  
      0x4b8c4b40,    // 18388608.0            
      0xffc00001,    // -signaling NaN        
      0xbf800000,    // -1.0                  
      0x7f800000,    // inf                   
      0x80011111,    // -9.7958E-41           
      0x00000000,    // zero                  

      // vpu register f30
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0xbf028f5c,    // -0.51                 
      0x7fc00000,    // quiet NaN             
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb000000,    // -8388608.0            

      // vpu register f31
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0x3f028f5c,    // 0.51                  
      0x00000000,    // zero                  
      0x80000000,    // -zero                 
      0x00000000,    // zero                  
      0x3f800000     // 1.0                   
   };
   C_TEST_START;
   *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions
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
         "li x1,  0x8090d460\n"
         "li x2,  0x80f14a00\n"
         "li x3,  0x80fe0800\n"
         "li x4,  0x801d95c0\n"
         "li x5,  0x80aede80\n"
         "li x6,  0x80936240\n"
         "li x7,  0x80adebc0\n"
         "li x8,  0x80ab5080\n"
         "li x9,  0x802ab980\n"
         "li x10,  0x80c33c20\n"
         "li x11,  0x80c038a0\n"
         "li x12,  0x80b61100\n"
         "li x13,  0x80122280\n"
         "li x14,  0x80a87600\n"
         "li x15,  0x80ff6420\n"
         "li x16,  0x80ecc420\n"
         "li x17,  0x80c16640\n"
         "li x18,  0x80a33740\n"
         "li x19,  0x80305e40\n"
         "li x20,  0x8075e040\n"
         "li x21,  0x8049cb40\n"
         "li x22,  0x80e68440\n"
         "li x23,  0x80b3d360\n"
         "li x24,  0x801d22e0\n"
         "li x25,  0x80459320\n"
         "li x26,  0x80c059c0\n"
         "li x27,  0x80b85e80\n"
         "li x28,  0x80639fe0\n"
         "li x29,  0x808605a0\n"
         "li x30,  0x80c0cd20\n"
         "li x31,  0x80636000\n"
         "cubefaceidx.ps f29, f9, f21\n"                       // opcode=0x89549efb
         "cubeface.ps f22, f14, f7\n"                          // opcode=0x88770b7b
         "fmulh.pi f20, f7, f11\n"                             // opcode=0x16b3aa7b
         "fle.ps f21, f30, f24\n"                              // opcode=0xa18f0afb
         "feq.pi f4, f24, f13\n"                               // opcode=0xa6dc227b
         "cubesgnsc.ps f23, f11, f30\n"                        // opcode=0x89e5abfb
         "cubesgntc.ps f11, f14, f6\n"                         // opcode=0x886735fb
         "fmadd.s f15, f22, f31, f16, dyn\n"                   // opcode=0x81fb77c3
         "cubesgntc.ps f6, f8, f21\n"                          // opcode=0x8954337b
         "fcmovm.ps f7, f19, f10\n"                            // opcode=0x00a983f7
         "fsrai.pi f20, f28, 5\n"                              // opcode=0x4e5e7a7b
         "fand.pi f14, f23, f31\n"                             // opcode=0x07fbf77b
         "fsra.pi f16, f13, f27\n"                             // opcode=0x0fb6d87b
         "fle.ps f5, f23, f31\n"                               // opcode=0xa1fb82fb
         "fsub.ps f30, f15, f15, dyn\n"                        // opcode=0x08f7ff7b
         "fbci.ps f16, 432025\n"                               // opcode=0x6979981f
         "fsrli.pi f23, f19, 21\n"                             // opcode=0x4f59dbfb
         //"fbc.ps f22, 63(x27)\n"                               // opcode=0x43fd8b0b
         "fsrli.pi f20, f12, 3\n"                              // opcode=0x4e365a7b
         "fsub.s f26, f11, f6, dyn\n"                          // opcode=0x0865fd53
         "fsll.pi f17, f23, f30\n"                             // opcode=0x07eb98fb
         "fbci.ps f17, 888100\n"                               // opcode=0xd8d2489f
         "fle.ps f22, f18, f6\n"                               // opcode=0xa0690b7b
         "fmadd.ps f31, f13, f31, f17, rup\n"                  // opcode=0x89f6bfdb
         "fsgnj.s f4, f17, f10\n"                              // opcode=0x20a88253
         "fltm.ps m5, f14, f21\n"                              // opcode=0xa15752fb
         "fsgnjn.s f8, f15, f7\n"                              // opcode=0x20779453
         "feq.s x14, f9, f11\n"                                // opcode=0xa0b4a753
         "mova.m.x x23\n"                                      // opcode=0xd60b907b
         "fadd.s f3, f28, f20, dyn\n"                          // opcode=0x014e71d3
         "fpackreph.pi f20, f6\n"                              // opcode=0x26031a7b
         "fpackreph.pi f12, f11\n"                             // opcode=0x2605967b
         "fmul.pi f2, f31, f4\n"                               // opcode=0x164f817b
         //"fsw f21, 104(x8)\n"                                  // opcode=0xcf542427
         "fle.ps f23, f31, f7\n"                               // opcode=0xa07f8bfb
         "fsgnj.ps f26, f11, f25\n"                            // opcode=0x21958d7b
         "fadd.s f2, f15, f12, rne\n"                          // opcode=0x00c78153
         "fclass.ps f11, f20\n"                                // opcode=0xe00a15fb
         //"fbc.ps f25, 53(x5)\n"                                // opcode=0x03528c8b
         "maskxor m2, m0, m7\n"                                // opcode=0x6670417b
         "fsrl.pi f14, f13, f23\n"                             // opcode=0x0776d77b
         //"flw f16, 83(x19)\n"                                  // opcode=0x0d39a807
         "fmul.ps f13, f7, f16, dyn\n"                         // opcode=0x1103f6fb
         "fpackreph.pi f31, f19\n"                             // opcode=0x26099ffb
         "flt.pi f30, f9, f30\n"                               // opcode=0xa7e49f7b
         "fmul.s f11, f5, f19, dyn\n"                          // opcode=0x1132f5d3
         "feqm.ps m1, f29, f8\n"                               // opcode=0xa08ee0fb
         "fclass.ps f7, f3\n"                                  // opcode=0xe00193fb
         "fsrli.pi f30, f20, 17\n"                             // opcode=0x4f1a5f7b
         "fle.s x25, f17, f23\n"                               // opcode=0xa1788cd3
         "flem.ps m5, f28, f27\n"                              // opcode=0xa1be42fb
         "fle.pi f21, f12, f13\n"                              // opcode=0xa6d60afb
         "maskand m4, m5, m3\n"                                // opcode=0x6632f27b
         "fpackreph.pi f18, f12\n"                             // opcode=0x2606197b
         "feqm.ps m5, f28, f26\n"                              // opcode=0xa1ae62fb
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
