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
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            
      0x80000000,    // -zero                 
      0x7fc00001,    // signaling NaN         
      0xff800000,    // -inf                  

      // vpu register f3
      0x80000000,    // -zero                 
      0x00011111,    // 9.7958E-41            
      0x7fc00001,    // signaling NaN         
      0x00000000,    // zero                  
      0x4b8c4b40,    // 18388608.0            
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        

      // vpu register f4
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0x7fc00000,    // quiet NaN             
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x00000000,    // zero                  

      // vpu register f5
      0x00011111,    // 9.7958E-41            
      0x80000000,    // -zero                 
      0x00011111,    // 9.7958E-41            
      0x80000000,    // -zero                 
      0x00000000,    // zero                  
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f6
      0xbf028f5c,    // -0.51                 
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x00000000,    // zero                  
      0x80000000,    // -zero                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            

      // vpu register f7
      0x80000000,    // -zero                 
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        
      0x4b8c4b40,    // 18388608.0            

      // vpu register f8
      0xff800000,    // -inf                  
      0x3f800000,    // 1.0                   
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00011111,    // 9.7958E-41            

      // vpu register f9
      0x7fc00000,    // quiet NaN             
      0x00000000,    // zero                  
      0x00011111,    // 9.7958E-41            
      0x7f800000,    // inf                   
      0x3f028f5c,    // 0.51                  
      0xffc00000,    // -quiet NaN            
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f10
      0x00000000,    // zero                  
      0x3f028f5c,    // 0.51                  
      0x7fc00001,    // signaling NaN         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0x7fc00000,    // quiet NaN             
      0x7fc00000,    // quiet NaN             
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f11
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0xcb8c4b40,    // -18388608.0           

      // vpu register f12
      0xff800000,    // -inf                  
      0x7fc00001,    // signaling NaN         
      0x7fc00001,    // signaling NaN         
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            
      0x7f800000,    // inf                   

      // vpu register f13
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  
      0x7fc00001,    // signaling NaN         
      0xcb8c4b40,    // -18388608.0           

      // vpu register f14
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f15
      0x80000000,    // -zero                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            
      0x7f800000,    // inf                   
      0x4b000000,    // 8388608.0             
      0x4b000000,    // 8388608.0             
      0x007fffff,    // 1.1754942E-38         

      // vpu register f16
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x3f800000,    // 1.0                   
      0x7fc00000,    // quiet NaN             
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        

      // vpu register f17
      0x4b000000,    // 8388608.0             
      0x7f800000,    // inf                   
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0xffc00000,    // -quiet NaN            
      0x80011111,    // -9.7958E-41           

      // vpu register f18
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            
      0xbf800000,    // -1.0                  
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0x80000001,    // -1.4E-45 (-denorm)    
      0xff800000,    // -inf                  

      // vpu register f19
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000000,    // zero                  
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0x7fc00001,    // signaling NaN         
      0xffc00001,    // -signaling NaN        

      // vpu register f20
      0xffc00000,    // -quiet NaN            
      0xffc00001,    // -signaling NaN        
      0xbf028f5c,    // -0.51                 
      0xff800000,    // -inf                  
      0x4b000000,    // 8388608.0             
      0x3f028f5c,    // 0.51                  
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  

      // vpu register f21
      0x7f800000,    // inf                   
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0x7f800000,    // inf                   
      0x00011111,    // 9.7958E-41            
      0xffc00000,    // -quiet NaN            
      0x7fc00001,    // signaling NaN         
      0xcb8c4b40,    // -18388608.0           

      // vpu register f22
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         
      0x00000000,    // zero                  
      0xffc00001,    // -signaling NaN        
      0x7fc00000,    // quiet NaN             
      0x4b8c4b40,    // 18388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00001,    // -signaling NaN        

      // vpu register f23
      0x4b000000,    // 8388608.0             
      0x7f800000,    // inf                   
      0xffc00001,    // -signaling NaN        
      0x00011111,    // 9.7958E-41            
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0x00011111,    // 9.7958E-41            

      // vpu register f24
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 
      0x7fc00000,    // quiet NaN             
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x7f800000,    // inf                   

      // vpu register f25
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0xcb000000,    // -8388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x007fffff,    // 1.1754942E-38         
      0x7fc00000,    // quiet NaN             
      0x7fc00001,    // signaling NaN         

      // vpu register f26
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 

      // vpu register f27
      0xcb000000,    // -8388608.0            
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0xff800000,    // -inf                  

      // vpu register f28
      0x7fc00000,    // quiet NaN             
      0xffc00001,    // -signaling NaN        
      0xcb000000,    // -8388608.0            
      0x7fc00000,    // quiet NaN             
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00000,    // -quiet NaN            

      // vpu register f29
      0xffc00000,    // -quiet NaN            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x3f800000,    // 1.0                   
      0xffc00001,    // -signaling NaN        
      0x7fc00000,    // quiet NaN             

      // vpu register f30
      0x4b8c4b40,    // 18388608.0            
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  
      0x7fc00001,    // signaling NaN         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0xff800000,    // -inf                  
      0xff800000,    // -inf                  

      // vpu register f31
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0x00011111,    // 9.7958E-41            
      0xff800000,    // -inf                  
      0x4b000000,    // 8388608.0             
      0xffc00001,    // -signaling NaN        
      0x4b000000,    // 8388608.0             
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
         "li x1,  0x80a88e40\n"
         "li x2,  0x803a15c0\n"
         "li x3,  0x804ec280\n"
         "li x4,  0x80d0bc80\n"
         "li x5,  0x80c2fc80\n"
         "li x6,  0x804f4ec0\n"
         "li x7,  0x8033c6c0\n"
         "li x8,  0x80f38780\n"
         "li x9,  0x8004aae0\n"
         "li x10,  0x8030b520\n"
         "li x11,  0x80691720\n"
         "li x12,  0x807a76a0\n"
         "li x13,  0x8045a9e0\n"
         "li x14,  0x809eafa0\n"
         "li x15,  0x80d351c0\n"
         "li x16,  0x80acbd20\n"
         "li x17,  0x80a8d2e0\n"
         "li x18,  0x80472e00\n"
         "li x19,  0x8053bf00\n"
         "li x20,  0x807c41e0\n"
         "li x21,  0x80589e20\n"
         "li x22,  0x800c4740\n"
         "li x23,  0x80fa6620\n"
         "li x24,  0x808c6fe0\n"
         "li x25,  0x80e3cae0\n"
         "li x26,  0x8077f640\n"
         "li x27,  0x804bfa00\n"
         "li x28,  0x809b5740\n"
         "li x29,  0x809cc860\n"
         "li x30,  0x800a0fa0\n"
         "li x31,  0x8018be80\n"
         "fle.pi f4, f31, f18\n"                               // opcode=0xa72f827b
         "fpackrepb.pi f8, f18\n"                              // opcode=0x2609047b


         "cubeface.ps f2, f27, f11\n"                          // opcode=0x88bd817b
         "faddi.pi f24, f10, 511\n"                           // opcode=0xfd050c3f
         "fpackrepb.pi f11, f2\n"                              // opcode=0x260105fb
         "fsll.pi f29, f26, f29\n"                             // opcode=0x07dd1efb
         "fltu.pi f12, f5, f12\n"                              // opcode=0xa6c2b67b
         "fsrli.pi f14, f10, 10\n"                             // opcode=0x4ea5577b
         "fsrai.pi f24, f21, 31\n"                             // opcode=0x4ffafc7b
         "fsrai.pi f23, f12, 16\n"                             // opcode=0x4f067bfb
         "flt.ps f6, f31, f29\n"                               // opcode=0xa1df937b
         //"fsw f17, 50(x23)\n"                                  // opcode=0x7b1ba927
         "ffrc.ps f31, f27\n"                                  // opcode=0x582d8ffb
         "for.pi f16, f25, f11\n"                              // opcode=0x06bce87b
         "fmul.pi f10, f11, f12\n"                             // opcode=0x16c5857b
         "fle.s x15, f23, f3\n"                                // opcode=0xa03b87d3
         "fbci.pi f16, 442383\n"                               // opcode=0x6c00f85f
         //"flw f27, 45(x22)\n"                                  // opcode=0xe2db2d87
         "fswizz.ps f29, f23, 114\n"                           // opcode=0xe6ebaefb
         "fsrl.pi f6, f15, f30\n"                              // opcode=0x07e7d37b
         "cubefaceidx.ps f3, f6, f15\n"                        // opcode=0x88f311fb
         "fmul.s f26, f7, f30, rdn\n"                          // opcode=0x11e3ad53

         "ffrc.ps f27, f2\n"                                   // opcode=0x58210dfb
         "fcmovm.ps f21, f11, f13\n"                           // opcode=0x00d58af7
         "feqm.ps m2, f16, f29\n"                              // opcode=0xa1d8617b
         "maskor m6, m6, m0\n"                                 // opcode=0x6603637b
         "fmsub.s f30, f10, f11, f30, dyn\n"                   // opcode=0xf0b57f47
         "fadd.pi f29, f22, f24\n"                             // opcode=0x078b0efb
         "cubesgntc.ps f22, f10, f25\n"                        // opcode=0x89953b7b
         "fbci.ps f27, 681329\n"                               // opcode=0xa6571d9f
         "fpackreph.pi f15, f16\n"                             // opcode=0x260817fb
         "fpackrepb.pi f12, f20\n"                             // opcode=0x260a067b
         "fsgnj.s f30, f31, f4\n"                              // opcode=0x204f8f53

         "feq.ps f23, f30, f29\n"                              // opcode=0xa1df2bfb

         "ffrc.ps f31, f24\n"                                  // opcode=0x582c0ffb
         "cubesgnsc.ps f3, f5, f21\n"                          // opcode=0x8952a1fb
         "fsgnjx.ps f21, f22, f29\n"                           // opcode=0x21db2afb
         "fsub.s f21, f16, f25, dyn\n"                         // opcode=0x09987ad3
         "fmsub.s f2, f11, f11, f31, dyn\n"                    // opcode=0xf8b5f147
         "fand.pi f12, f9, f7\n"                               // opcode=0x0674f67b
         "fandi.pi f23, f5, 12\n"                              // opcode=0x04c29bbf
         "for.pi f16, f26, f30\n"                              // opcode=0x07ed687b
         "fsetm.pi m4, f25\n"                                  // opcode=0xe00cd27b
         "fandi.pi f16, f31, 150\n"                            // opcode=0x256f983f
         "maskpopc x13, m1\n"                                  // opcode=0x520086fb
         "fsrai.pi f4, f19, 5\n"                               // opcode=0x4e59f27b
         "cubesgntc.ps f27, f7, f13\n"                         // opcode=0x88d3bdfb
         "for.pi f30, f26, f22\n"                              // opcode=0x076d6f7b
         "fsgnjn.s f11, f21, f3\n"                             // opcode=0x203a95d3
         "masknot m6, m7\n"                                    // opcode=0x6603a37b
         "fsrli.pi f20, f31, 9\n"                              // opcode=0x4e9fda7b
         "flt.s x14, f9, f6\n"                                 // opcode=0xa0649753
         "fsgnjx.ps f27, f10, f14\n"                           // opcode=0x20e52dfb
         "fsgnj.s f2, f14, f12\n"                              // opcode=0x20c70153
         "fclass.ps f28, f30\n"                                // opcode=0xe00f1e7b
         "fmulhu.pi f6, f12, f21\n"                            // opcode=0x1756137b
         "fbci.pi f21, 385019\n"                               // opcode=0x5dffbadf
         "fadd.ps f6, f2, f29, dyn\n"                          // opcode=0x01d1737b
         "fxor.pi f7, f22, f2\n"                               // opcode=0x062b43fb
         "fclass.ps f12, f20\n"                                // opcode=0xe00a167b
         "fle.s x15, f14, f22\n"                               // opcode=0xa16707d3
         "fsgnjn.ps f31, f20, f6\n"                            // opcode=0x206a1ffb
         "fbcx.ps f20, x29\n"                                  // opcode=0x000eba0b
         "feqm.ps m2, f13, f9\n"                               // opcode=0xa096e17b
         "fsgnjn.s f19, f16, f10\n"                            // opcode=0x20a819d3
         "maskxor m1, m6, m0\n"                                // opcode=0x660340fb
         //"flw.ps f3, 90(x15)\n"                                // opcode=0x45a7a18b

         "fmsub.s f20, f22, f18, f27, dyn\n"                   // opcode=0xd92b7a47
         "fsgnjn.ps f4, f13, f19\n"                            // opcode=0x2136927b
         "fle.s x11, f23, f31\n"                               // opcode=0xa1fb85d3
         "fmul.ps f5, f25, f15, dyn\n"                         // opcode=0x10fcf2fb
         "fle.s x23, f28, f20\n"                               // opcode=0xa14e0bd3
         "fsll.pi f8, f16, f4\n"                               // opcode=0x0648147b
         //"flw.ps f29, 115(x10)\n"                              // opcode=0x87352e8b
         "fand.pi f10, f15, f17\n"                             // opcode=0x0717f57b
         "fsgnjn.ps f22, f4, f23\n"                            // opcode=0x21721b7b
         "fadd.ps f6, f25, f10, dyn\n"                         // opcode=0x00acf37b
         "masknot m1, m1\n"                                    // opcode=0x6600a0fb
         "ffrc.ps f22, f22\n"                                  // opcode=0x582b0b7b
         "feq.s x20, f8, f13\n"                                // opcode=0xa0d42a53
         "faddi.pi f10, f16, 511\n"                            // opcode=0xfc38053f
         "maskxor m6, m6, m2\n"                                // opcode=0x6623437b
         //"fsw f24, 23(x17)\n"                                  // opcode=0x9988aba7
         "flt.ps f12, f13, f29\n"                              // opcode=0xa1d6967b
         "for.pi f15, f24, f19\n"                              // opcode=0x073c67fb
         "fmul.s f20, f15, f25, dyn\n"                         // opcode=0x1197fa53
         "fcmov.ps f3, f25, f8, f27\n"                         // opcode=0xdc8ca1bf
         "fmul.pi f10, f24, f5\n"                              // opcode=0x165c057b
         "faddi.pi f11, f23, 118\n"                            // opcode=0x1d6b85bf
         "fsetm.pi m6, f9\n"                                   // opcode=0xe004d37b
         "ffrc.ps f16, f23\n"                                  // opcode=0x582b887b
         "flt.pi f4, f20, f25\n"                               // opcode=0xa79a127b
         "fmul.s f17, f2, f17, dyn\n"                          // opcode=0x111178d3
         "fmul.ps f11, f18, f5, dyn\n"                         // opcode=0x105975fb
         "feq.s x21, f5, f9\n"                                 // opcode=0xa092aad3
         "fsrl.pi f8, f16, f22\n"                              // opcode=0x0768547b
         "fmulhu.pi f18, f26, f3\n"                            // opcode=0x163d197b
         "fmulh.pi f28, f4, f20\n"                             // opcode=0x17422e7b
         "fle.pi f10, f4, f7\n"                                // opcode=0xa672057b
         "fsrai.pi f17, f18, 14\n"                             // opcode=0x4ee978fb
         "fle.ps f24, f12, f3\n"                               // opcode=0xa0360c7b
         "fsgnjn.ps f9, f26, f28\n"                            // opcode=0x21cd14fb
         "fswizz.ps f7, f13, 193\n"                            // opcode=0xe78693fb
         "fsub.ps f23, f4, f23, dyn\n"                         // opcode=0x09727bfb
         "fadd.s f29, f8, f27, dyn\n"                          // opcode=0x01b47ed3
         "fmul.s f20, f2, f3, dyn\n"                           // opcode=0x10317a53
         "cubefaceidx.ps f31, f13, f27\n"                      // opcode=0x89b69ffb
         "fle.ps f11, f13, f5\n"                               // opcode=0xa05685fb
         "fsrai.pi f20, f26, 20\n"                             // opcode=0x4f4d7a7b
         "fsetm.pi m3, f11\n"                                  // opcode=0xe005d1fb

         "faddi.pi f16, f7, 511\n"                             // opcode=0xbc83883f
         "fsgnj.ps f26, f18, f9\n"                             // opcode=0x20990d7b
         "fadd.ps f8, f26, f13, dyn\n"                         // opcode=0x00dd747b
         "fmul.s f17, f14, f10, dyn\n"                         // opcode=0x10a778d3
         "cubesgntc.ps f5, f2, f21\n"                          // opcode=0x895132fb
         "fsgnjx.s f15, f14, f31\n"                            // opcode=0x21f727d3
         "fbcx.ps f11, x25\n"                                  // opcode=0x000cb58b
         "fsgnj.ps f5, f8, f26\n"                              // opcode=0x21a402fb
         "fsrli.pi f29, f30, 10\n"                             // opcode=0x4eaf5efb
         "fsll.pi f10, f31, f29\n"                             // opcode=0x07df957b
         "feqm.ps m2, f18, f20\n"                              // opcode=0xa149617b
         "fmsub.s f7, f23, f19, f18, dyn\n"                    // opcode=0x913bf3c7
         "fsgnj.s f24, f26, f12\n"                             // opcode=0x20cd0c53
         "mova.m.x x19\n"                                      // opcode=0xd609907b
         "fpackrepb.pi f13, f16\n"                             // opcode=0x260806fb
         "fpackreph.pi f30, f17\n"                             // opcode=0x26089f7b
         "fadd.s f4, f31, f26, dyn\n"                          // opcode=0x01aff253
         "fsgnj.ps f18, f21, f31\n"                            // opcode=0x21fa897b
         "fxor.pi f9, f19, f20\n"                              // opcode=0x0749c4fb
         "fmul.ps f26, f17, f4, dyn\n"                         // opcode=0x1048fd7b
         "masknot m3, m1\n"                                    // opcode=0x6600a1fb
         "fle.ps f24, f12, f23\n"                              // opcode=0xa1760c7b
         "feq.s x12, f25, f15\n"                               // opcode=0xa0fca653
         "fadd.s f24, f20, f5, dyn\n"                          // opcode=0x005a7c53
         "fsrli.pi f2, f22, 2\n"                               // opcode=0x4e2b517b

         "maskpopcz x12, m0\n"                                 // opcode=0x5400067b
         "flt.s x13, f3, f27\n"                                // opcode=0xa1b196d3
         "fround.ps f27, f25, dyn\n"                           // opcode=0x581cfdfb
         "fmsub.s f18, f6, f13, f15, dyn\n"                    // opcode=0x78d37947
         "fsrai.pi f27, f9, 25\n"                              // opcode=0x4f94fdfb
         "cubesgnsc.ps f19, f17, f22\n"                        // opcode=0x8968a9fb
         "fsra.pi f20, f19, f30\n"                             // opcode=0x0fe9da7b
         "fltu.pi f16, f18, f11\n"                             // opcode=0xa6b9387b
         "feq.s x24, f21, f17\n"                               // opcode=0xa11aac53
         "fpackreph.pi f10, f8\n"                              // opcode=0x2604157b
         "fltu.pi f17, f5, f8\n"                               // opcode=0xa682b8fb
         "fxor.pi f13, f5, f8\n"                               // opcode=0x0682c6fb
         "faddi.pi f15, f31, 307\n"                            // opcode=0x4d3f87bf
         "fbcx.ps f27, x11\n"                                  // opcode=0x0005bd8b
         "fadd.s f30, f25, f21, dyn\n"                         // opcode=0x015cff53

         "fswizz.ps f13, f9, 21\n"                             // opcode=0xe624d6fb
         "fmul.s f24, f14, f20, dyn\n"                         // opcode=0x11477c53
         "cubefaceidx.ps f25, f12, f20\n"                      // opcode=0x89461cfb

         "fsub.pi f2, f2, f8\n"                                // opcode=0x0e81017b
         "fnot.pi f2, f11\n"                                   // opcode=0x0605a17b
         "fsgnjn.ps f6, f14, f28\n"                            // opcode=0x21c7137b
         "fbcx.ps f29, x13\n"                                  // opcode=0x0006be8b

         "masknot m1, m0\n"                                    // opcode=0x660020fb
         "cubeface.ps f12, f16, f15\n"                         // opcode=0x88f8067b
         "fclass.ps f29, f26\n"                                // opcode=0xe00d1efb
         "for.pi f3, f3, f16\n"                                // opcode=0x0701e1fb
         "fsrli.pi f7, f23, 17\n"                              // opcode=0x4f1bd3fb
         "cubesgntc.ps f23, f17, f23\n"                        // opcode=0x8978bbfb
         "fnot.pi f8, f13\n"                                   // opcode=0x0606a47b

         "fmul.s f13, f21, f9, dyn\n"                          // opcode=0x109af6d3
         "maskor m7, m1, m1\n"                                 // opcode=0x6610e3fb
         "fand.pi f19, f2, f18\n"                              // opcode=0x072179fb
         "fcmov.ps f10, f12, f7, f7\n"                         // opcode=0x3c76253f
         "fxor.pi f9, f28, f25\n"                              // opcode=0x079e44fb
         "fsgnjx.ps f18, f30, f30\n"                           // opcode=0x21ef297b
         "fsgnjx.ps f22, f2, f14\n"                            // opcode=0x20e12b7b
         "fsgnjx.s f20, f13, f12\n"                            // opcode=0x20c6aa53

         "maskpopc x21, m1\n"                                  // opcode=0x52008afb
         "fmulh.pi f26, f22, f20\n"                            // opcode=0x174b2d7b
         //"fbc.ps f25, 51(x30)\n"                               // opcode=0x9b3f0c8b
         "fpackrepb.pi f8, f11\n"                              // opcode=0x2605847b
         "fclass.s x10, f12\n"                                 // opcode=0xe0061553
         "fxor.pi f22, f20, f28\n"                             // opcode=0x07ca4b7b
         "fsgnjn.ps f13, f18, f4\n"                            // opcode=0x204916fb
         "fle.s x19, f31, f8\n"                                // opcode=0xa08f89d3
         "feq.ps f12, f22, f30\n"                              // opcode=0xa1eb267b
         "mova.x.m x14\n"                                      // opcode=0xd600077b
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
