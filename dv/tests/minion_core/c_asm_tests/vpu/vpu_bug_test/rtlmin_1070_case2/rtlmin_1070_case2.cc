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
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   
      0x80000000,    // -zero                 
      0xffc00001,    // -signaling NaN        
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            

      // vpu register f3
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x80011111,    // -9.7958E-41           
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            

      // vpu register f4
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   
      0x00000000,    // zero                  
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f5
      0x3f028f5c,    // 0.51                  
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0x7fc00001,    // signaling NaN         
      0xbf028f5c,    // -0.51                 
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   

      // vpu register f6
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0x4b000000,    // 8388608.0             
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0xffc00001,    // -signaling NaN        
      0x7f800000,    // inf                   

      // vpu register f7
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  

      // vpu register f8
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf028f5c,    // -0.51                 
      0x7fc00000,    // quiet NaN             
      0x00000000,    // zero                  
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           

      // vpu register f9
      0xff800000,    // -inf                  
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x3f800000,    // 1.0                   
      0x7fc00000,    // quiet NaN             
      0xbf800000,    // -1.0                  
      0xffc00001,    // -signaling NaN        
      0xffc00000,    // -quiet NaN            

      // vpu register f10
      0x3f800000,    // 1.0                   
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xff800000,    // -inf                  
      0x4b000000,    // 8388608.0             
      0x80000000,    // -zero                 
      0x80000000,    // -zero                 

      // vpu register f11
      0x4b000000,    // 8388608.0             
      0x7f800000,    // inf                   
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7f800000,    // inf                   
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  

      // vpu register f12
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  
      0x3f028f5c,    // 0.51                  
      0x4b000000,    // 8388608.0             
      0x4b8c4b40,    // 18388608.0            
      0xff800000,    // -inf                  
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f13
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            
      0x3f800000,    // 1.0                   
      0x7f800000,    // inf                   
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x4b8c4b40,    // 18388608.0            

      // vpu register f14
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            
      0x7f800000,    // inf                   
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           

      // vpu register f15
      0x00011111,    // 9.7958E-41            
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0x00000001,    // 1.4E-45 (+denorm)     
      0xff800000,    // -inf                  

      // vpu register f16
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0xffc00000,    // -quiet NaN            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           

      // vpu register f17
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00001,    // signaling NaN         
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         
      0x7fc00000,    // quiet NaN             

      // vpu register f18
      0x3f028f5c,    // 0.51                  
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0x7f800000,    // inf                   
      0x80000000,    // -zero                 
      0x3f800000,    // 1.0                   

      // vpu register f19
      0x7f800000,    // inf                   
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x80000000,    // -zero                 
      0xbf800000,    // -1.0                  
      0xff800000,    // -inf                  
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  

      // vpu register f20
      0x00000000,    // zero                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0x7fc00001,    // signaling NaN         
      0xcb8c4b40,    // -18388608.0           
      0x7fc00001,    // signaling NaN         
      0x80000000,    // -zero                 
      0x00000000,    // zero                  

      // vpu register f21
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0x7fc00001,    // signaling NaN         
      0x80000000,    // -zero                 
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   
      0x7f800000,    // inf                   
      0x007fffff,    // 1.1754942E-38         

      // vpu register f22
      0x4b000000,    // 8388608.0             
      0xffc00000,    // -quiet NaN            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb8c4b40,    // -18388608.0           
      0xff800000,    // -inf                  
      0xff800000,    // -inf                  
      0xffc00001,    // -signaling NaN        
      0xffc00000,    // -quiet NaN            

      // vpu register f23
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80011111,    // -9.7958E-41           
      0xcb000000,    // -8388608.0            

      // vpu register f24
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0x7fc00000,    // quiet NaN             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7fc00001,    // signaling NaN         
      0x4b000000,    // 8388608.0             
      0xbf028f5c,    // -0.51                 

      // vpu register f25
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0xff800000,    // -inf                  
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0x3f800000,    // 1.0                   

      // vpu register f26
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0xff800000,    // -inf                  

      // vpu register f27
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0xbf028f5c,    // -0.51                 
      0x7fc00001,    // signaling NaN         
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            

      // vpu register f28
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0x80011111,    // -9.7958E-41           
      0x00011111,    // 9.7958E-41            

      // vpu register f29
      0x80000001,    // -1.4E-45 (-denorm)    
      0x7f800000,    // inf                   
      0xcb000000,    // -8388608.0            
      0x00000000,    // zero                  
      0x7fc00001,    // signaling NaN         
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f800000,    // 1.0                   
      0x80011111,    // -9.7958E-41           

      // vpu register f30
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   
      0x80000000,    // -zero                 
      0x80000000,    // -zero                 
      0x00011111,    // 9.7958E-41            

      // vpu register f31
      0x7fc00001,    // signaling NaN         
      0xff800000,    // -inf                  
      0xbf800000,    // -1.0                  
      0x3f028f5c,    // 0.51                  
      0x80011111,    // -9.7958E-41           
      0x00000000,    // zero                  
      0xffc00000,    // -quiet NaN            
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
         "li x1,  0x80f33520\n"
         "li x2,  0x803593a0\n"
         "li x3,  0x8000a240\n"
         "li x4,  0x80a770c0\n"
         "li x5,  0x80ab97e0\n"
         "li x6,  0x80c56880\n"
         "li x7,  0x80dc9d60\n"
         "li x8,  0x80b36b60\n"
         "li x9,  0x80c85060\n"
         "li x10,  0x80604060\n"
         "li x11,  0x80fdef00\n"
         "li x12,  0x8051b8c0\n"
         "li x13,  0x809906c0\n"
         "li x14,  0x80a54b60\n"
         "li x15,  0x80cc16e0\n"
         "li x16,  0x8063b400\n"
         "li x17,  0x8027b700\n"
         "li x18,  0x805f06a0\n"
         "li x19,  0x802d2ca0\n"
         "li x20,  0x8007e3c0\n"
         "li x21,  0x80daa1e0\n"
         "li x22,  0x802ed460\n"
         "li x23,  0x80515d00\n"
         "li x24,  0x80261fe0\n"
         "li x25,  0x8094a7e0\n"
         "li x26,  0x807cbda0\n"
         "li x27,  0x8002e920\n"
         "li x28,  0x807f5b20\n"
         "li x29,  0x8027fcc0\n"
         "li x30,  0x80d5a100\n"
         "li x31,  0x8019a400\n"
         "fbcx.ps f6, x29\n"                                   // opcode=0x000eb30b
         "fmulhu.pi f25, f7, f4\n"                             // opcode=0x16439cfb
         "fbcx.ps f29, x18\n"                                  // opcode=0x00093e8b
         "cubefaceidx.ps f24, f10, f23\n"                      // opcode=0x89751c7b
         "fsgnj.s f10, f25, f16\n"                             // opcode=0x210c8553
         "fle.s x13, f10, f9\n"                                // opcode=0xa09506d3
         "cubesgntc.ps f22, f16, f29\n"                        // opcode=0x89d83b7b
         "fsrli.pi f6, f16, 26\n"                              // opcode=0x4fa8537b
         "fbcx.ps f27, x22\n"                                  // opcode=0x000b3d8b
         "fadd.s f3, f13, f23, dyn\n"                          // opcode=0x0176f1d3
         "masknot m6, m5\n"                                    // opcode=0x6602a37b
         "fbci.ps f31, 732556\n"                               // opcode=0xb2d8cf9f
         "fmadd.ps f9, f9, f30, f16, dyn\n"                    // opcode=0x81e4f4db
         "fsetm.pi m1, f6\n"                                   // opcode=0xe00350fb
         "faddi.pi f9, f31, 76\n"                             // opcode=0xac4f84bf
         "cubesgnsc.ps f16, f27, f20\n"                        // opcode=0x894da87b
         "fadd.ps f31, f25, f21, dyn\n"                        // opcode=0x015cfffb
         "fsgnj.s f12, f24, f23\n"                             // opcode=0x217c0653
         "mova.m.x x10\n"                                      // opcode=0xd605107b
         "fsll.pi f30, f29, f29\n"                             // opcode=0x07de9f7b
         "maskpopc x4, m7\n"                                   // opcode=0x5203827b
         "for.pi f29, f11, f14\n"                              // opcode=0x06e5eefb
         "fsgnjx.ps f19, f20, f27\n"                           // opcode=0x21ba29fb
         "flt.ps f25, f14, f29\n"                              // opcode=0xa1d71cfb
         "fclass.ps f15, f15\n"                                // opcode=0xe00797fb
         "fsgnjn.s f9, f24, f23\n"                             // opcode=0x217c14d3
         "fle.ps f13, f24, f4\n"                               // opcode=0xa04c06fb
         "fsub.pi f14, f24, f17\n"                             // opcode=0x0f1c077b
         "fsgnj.s f2, f7, f18\n"                               // opcode=0x21238153
         "for.pi f31, f2, f23\n"                               // opcode=0x07716ffb
         "fround.ps f30, f9, dyn\n"                            // opcode=0x5814ff7b
         "fround.ps f4, f19, dyn\n"                            // opcode=0x5819f27b
         "faddi.pi f31, f17, 23\n"                            // opcode=0xe5b88fbf
         "fsub.ps f13, f8, f3, dyn\n"                          // opcode=0x083476fb
         //"fsw.ps f2, 83(x5)\n"                                 // opcode=0xd422e98b
         "fmsub.ps f26, f20, f25, f31, dyn\n"                  // opcode=0xfb9a7d5b
         "maskor m2, m3, m4\n"                                 // opcode=0x6641e17b
         "ffrc.ps f16, f17\n"                                  // opcode=0x5828887b
         "fsetm.pi m3, f24\n"                                  // opcode=0xe00c51fb
         //"flw.ps f25, 49(x20)\n"                               // opcode=0x2b1a2c8b
         "fle.ps f27, f13, f7\n"                               // opcode=0xa0768dfb
         "cubefaceidx.ps f10, f4, f21\n"                       // opcode=0x8952157b
         "fsgnjn.ps f23, f16, f2\n"                            // opcode=0x20281bfb
         "ffrc.ps f27, f13\n"                                  // opcode=0x58268dfb
         "fpackreph.pi f23, f2\n"                              // opcode=0x26011bfb
         "maskpopc x16, m1\n"                                  // opcode=0x5200887b
         "fmulh.pi f20, f31, f12\n"                            // opcode=0x16cfaa7b
         "fmulhu.pi f10, f8, f2\n"                             // opcode=0x1624157b
         "feq.s x28, f22, f7\n"                                // opcode=0xa07b2e53
         "maskpopcz x11, m6\n"                                 // opcode=0x540305fb
         "maskpopcz x25, m5\n"                                 // opcode=0x54028cfb
         "fsrli.pi f25, f7, 11\n"                              // opcode=0x4eb3dcfb
         //"fsw f28, 127(x21)\n"                                 // opcode=0xb7caafa7
         "flt.s x30, f24, f8\n"                                // opcode=0xa08c1f53
         "fsub.pi f22, f13, f3\n"                              // opcode=0x0e368b7b
         "maskxor m7, m4, m0\n"                                // opcode=0x660243fb
         "fclass.s x2, f13\n"                                  // opcode=0xe0069153
         "fmadd.s f13, f12, f15, f29, dyn\n"                   // opcode=0xe8f676c3
         "maskpopcz x11, m3\n"                                 // opcode=0x540185fb
         "fmadd.s f16, f6, f21, f21, dyn\n"                    // opcode=0xa9537843
         "fmulh.pi f3, f14, f11\n"                             // opcode=0x16b721fb
         "fsrl.pi f12, f3, f8\n"                               // opcode=0x0681d67b
         "fsgnjx.ps f18, f17, f29\n"                           // opcode=0x21d8a97b
         "ffrc.ps f5, f9\n"                                    // opcode=0x582482fb
         "fltu.pi f13, f9, f23\n"                              // opcode=0xa774b6fb
         "fle.ps f12, f13, f26\n"                              // opcode=0xa1a6867b
         "fsrai.pi f19, f15, 30\n"                             // opcode=0x4fe7f9fb
         "fmsub.s f13, f8, f19, f15, dyn\n"                    // opcode=0x793476c7
         "fandi.pi f28, f10, 54\n"                            // opcode=0xeda51e3f
         "fpackrepb.pi f6, f10\n"                              // opcode=0x2605037b
         "fround.ps f15, f5, dyn\n"                            // opcode=0x5812f7fb
         "fsub.ps f8, f16, f11, dyn\n"                         // opcode=0x08b8747b
         "fadd.ps f5, f26, f30, dyn\n"                         // opcode=0x01ed72fb
         "feq.pi f26, f6, f16\n"                               // opcode=0xa7032d7b
         "fle.ps f12, f22, f21\n"                              // opcode=0xa15b067b
         "fnot.pi f19, f12\n"                                  // opcode=0x060629fb
         "ffrc.ps f19, f3\n"                                   // opcode=0x582189fb
         "feq.s x19, f25, f8\n"                                // opcode=0xa08ca9d3
         "fsgnjn.s f26, f28, f28\n"                            // opcode=0x21ce1d53
         "masknot m6, m7\n"                                    // opcode=0x6603a37b
         "fpackreph.pi f8, f25\n"                              // opcode=0x260c947b
         "fadd.s f15, f6, f14, dyn\n"                          // opcode=0x00e377d3
         "fsgnjn.ps f6, f23, f18\n"                            // opcode=0x212b937b
         "fltu.pi f10, f30, f6\n"                              // opcode=0xa66f357b
         "fmulh.pi f18, f22, f6\n"                             // opcode=0x166b297b
         //"fsw.ps f9, 27(x15)\n"                                // opcode=0x0097ed8b
         "fsgnjn.s f22, f12, f13\n"                            // opcode=0x20d61b53
         "cubesgnsc.ps f24, f30, f7\n"                         // opcode=0x887f2c7b
         "mova.x.m x12\n"                                      // opcode=0xd600067b
         "cubesgntc.ps f9, f6, f7\n"                           // opcode=0x887334fb
         "cubesgnsc.ps f23, f12, f5\n"                         // opcode=0x88562bfb
         "fltu.pi f16, f27, f15\n"                             // opcode=0xa6fdb87b
         "fadd.ps f27, f18, f30, dyn\n"                        // opcode=0x01e97dfb
         "fandi.pi f26, f30, 473\n"                            // opcode=0x759f1d3f
         "fsgnjn.s f14, f9, f6\n"                              // opcode=0x20649753
         "fltu.pi f9, f12, f7\n"                               // opcode=0xa67634fb
         "flt.pi f20, f16, f23\n"                              // opcode=0xa7781a7b
         "maskpopc x22, m1\n"                                  // opcode=0x52008b7b
         "fxor.pi f28, f15, f11\n"                             // opcode=0x06b7ce7b
         "fandi.pi f31, f19, 465\n"                            // opcode=0x75199fbf
         "flt.s x13, f15, f7\n"                                // opcode=0xa07796d3
         "fmadd.ps f31, f14, f5, f13, dyn\n"                   // opcode=0x68577fdb
         "fbci.ps f6, 762999\n"                                // opcode=0xba47731f
         "fbci.pi f30, 490740\n"                               // opcode=0x77cf4f5f
         "cubesgnsc.ps f15, f22, f10\n"                        // opcode=0x88ab27fb
         "fmul.s f29, f19, f29, dyn\n"                         // opcode=0x11d9fed3
         "flt.pi f17, f25, f12\n"                              // opcode=0xa6cc98fb
         "fmulh.pi f25, f19, f16\n"                            // opcode=0x1709acfb
         "fswizz.ps f30, f27, 171\n"                           // opcode=0xe75dbf7b
         "flt.ps f28, f7, f8\n"                                // opcode=0xa0839e7b
         "fcmov.ps f11, f13, f18, f20\n"                       // opcode=0xa526a5bf
         "cubefaceidx.ps f22, f9, f14\n"                       // opcode=0x88e49b7b
         "fbci.pi f9, 640979\n"                                // opcode=0x9c7d34df
         "fltm.ps m7, f29, f25\n"                              // opcode=0xa19ed3fb
         "fsub.s f26, f26, f7, dyn\n"                          // opcode=0x087d7d53
         "fsub.s f9, f4, f5, dyn\n"                            // opcode=0x085274d3
         "fmsub.ps f4, f24, f31, f31, dyn\n"                   // opcode=0xfbfc725b
         "fsrl.pi f17, f24, f27\n"                             // opcode=0x07bc58fb
         "fltm.pi m0, f15, f5\n"                               // opcode=0x3e57807b
         "fmsub.ps f21, f8, f18, f8, dyn\n"                    // opcode=0x43247adb
         "maskpopc x3, m4\n"                                   // opcode=0x520201fb
         "flt.ps f4, f14, f4\n"                                // opcode=0xa047127b
         "fsgnjn.ps f9, f26, f5\n"                             // opcode=0x205d14fb
         "maskpopcz x17, m3\n"                                 // opcode=0x540188fb
         //"flw.ps f30, 82(x18)\n"                               // opcode=0x55292f0b
         "fmul.ps f14, f7, f25, dyn\n"                         // opcode=0x1193f77b
         "fcmovm.ps f19, f26, f13\n"                           // opcode=0x00dd09f7
         "fbci.ps f18, 59460\n"                                // opcode=0x0e84491f
         //"flw.ps f26, 61(x18)\n"                               // opcode=0x2bd92d0b
         "flt.ps f25, f9, f18\n"                               // opcode=0xa1249cfb
         "fclass.s x12, f27\n"                                 // opcode=0xe00d9653
         "fadd.pi f14, f29, f25\n"                             // opcode=0x079e877b
         "fclass.ps f30, f16\n"                                // opcode=0xe0081f7b
         "cubefaceidx.ps f26, f31, f14\n"                      // opcode=0x88ef9d7b
         //"flw f6, 46(x18)\n"                                   // opcode=0x22e92307
         "fsll.pi f20, f18, f26\n"                             // opcode=0x07a91a7b
         "fbci.ps f31, 750728\n"                               // opcode=0xb7488f9f
         "fltm.pi m4, f26, f5\n"                               // opcode=0x3e5d027b
         //"flw f8, 45(x7)\n"                                    // opcode=0x8ad3a407
         "mova.m.x x23\n"                                      // opcode=0xd60b907b
         "cubefaceidx.ps f11, f7, f20\n"                       // opcode=0x894395fb
         "fand.pi f8, f31, f13\n"                              // opcode=0x06dff47b
         "maskxor m2, m5, m2\n"                                // opcode=0x6622c17b
         "maskor m4, m7, m5\n"                                 // opcode=0x6653e27b
         "fmulh.pi f30, f10, f25\n"                            // opcode=0x17952f7b
         //"flw f5, 32(x26)\n"                                   // opcode=0xa20d2287
         "fadd.pi f6, f16, f2\n"                               // opcode=0x0628037b
         "fltm.pi m1, f23, f9\n"                               // opcode=0x3e9b80fb
         "fmul.ps f12, f23, f30, dyn\n"                        // opcode=0x11ebf67b
         "cubesgntc.ps f29, f7, f30\n"                         // opcode=0x89e3befb
         "feq.pi f18, f19, f22\n"                              // opcode=0xa769a97b
         "fmul.s f26, f3, f11, dyn\n"                          // opcode=0x10b1fd53
         "maskand m5, m2, m5\n"                                // opcode=0x665172fb
         "fswizz.ps f19, f24, 209\n"                           // opcode=0xe7ac19fb
         "maskpopc x13, m5\n"                                  // opcode=0x520286fb
         "fmul.ps f20, f25, f26, rne\n"                        // opcode=0x11ac8a7b
         "feq.ps f15, f18, f17\n"                              // opcode=0xa11927fb
         //"fsw f5, 50(x19)\n"                                   // opcode=0x0259a927
         "fadd.ps f29, f8, f20, dyn\n"                         // opcode=0x01447efb
         "fnot.pi f2, f15\n"                                   // opcode=0x0607a17b
         "fand.pi f20, f5, f27\n"                              // opcode=0x07b2fa7b
         "fnot.pi f28, f13\n"                                  // opcode=0x0606ae7b
         "fmulhu.pi f17, f21, f30\n"                           // opcode=0x17ea98fb
         "fxor.pi f27, f16, f26\n"                             // opcode=0x07a84dfb
         "flem.ps m4, f6, f2\n"                                // opcode=0xa023427b
         "fbci.ps f2, 940527\n"                                // opcode=0xe59ef11f
         "fsub.pi f19, f29, f15\n"                             // opcode=0x0efe89fb
         "fsub.s f10, f3, f9, dyn\n"                           // opcode=0x0891f553
         "fslli.pi f30, f15, 30\n"                             // opcode=0x4fe79f7b
         "fmsub.s f28, f16, f11, f11, rup\n"                   // opcode=0x58b83e47
         "fnot.pi f17, f20\n"                                  // opcode=0x060a28fb
         "fsrai.pi f10, f4, 27\n"                              // opcode=0x4fb2757b
         "fltm.pi m6, f6, f31\n"                               // opcode=0x3ff3037b
         "cubeface.ps f4, f24, f29\n"                          // opcode=0x89dc027b
         "fmsub.ps f13, f24, f5, f26, dyn\n"                   // opcode=0xd25c76db
         "maskand m4, m3, m3\n"                                // opcode=0x6631f27b
         "fsgnj.ps f19, f20, f10\n"                            // opcode=0x20aa09fb
         "fsll.pi f2, f18, f17\n"                              // opcode=0x0719117b
         "fmsub.ps f5, f4, f8, f17, dyn\n"                     // opcode=0x8a8272db
         "fsgnjx.ps f29, f6, f25\n"                            // opcode=0x21932efb
         "fsgnjn.ps f24, f4, f10\n"                            // opcode=0x20a21c7b
         "fmul.pi f24, f8, f22\n"                              // opcode=0x17640c7b
         //"flw.ps f14, 57(x20)\n"                               // opcode=0xc39a270b
         "fadd.s f20, f24, f24, dyn\n"                         // opcode=0x018c7a53
         "fsgnj.ps f24, f8, f5\n"                              // opcode=0x20540c7b
         "fclass.s x21, f30\n"                                 // opcode=0xe00f1ad3
         "fcmovm.ps f4, f31, f5\n"                             // opcode=0x005f8277
         "fmul.pi f26, f18, f8\n"                              // opcode=0x16890d7b
         "fbci.ps f21, 550340\n"                               // opcode=0x865c4a9f
         "fltm.pi m5, f9, f12\n"                               // opcode=0x3ec482fb
         "fxor.pi f23, f27, f30\n"                             // opcode=0x07edcbfb
         "fmsub.s f27, f14, f18, f26, dyn\n"                   // opcode=0xd1277dc7
         "fle.pi f8, f5, f13\n"                                // opcode=0xa6d2847b
         //"flw.ps f20, 96(x28)\n"                               // opcode=0x7e0e2a0b
         "flt.ps f7, f2, f17\n"                                // opcode=0xa11113fb
         "fsrl.pi f22, f22, f31\n"                             // opcode=0x07fb5b7b
         "fsll.pi f16, f29, f15\n"                             // opcode=0x06fe987b
         "fsub.ps f31, f15, f17, dyn\n"                        // opcode=0x0917fffb
         "cubefaceidx.ps f23, f13, f10\n"                      // opcode=0x88a69bfb
         "fle.ps f30, f5, f19\n"                               // opcode=0xa1328f7b
         "fsub.s f27, f29, f2, rne\n"                          // opcode=0x082e8dd3
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fsll.pi f2, f2, f6\n"                                // opcode=0x0661117b
         "fandi.pi f30, f25, 87\n"                            // opcode=0x94bc9f3f
         "flem.ps m5, f5, f26\n"                               // opcode=0xa1a2c2fb
         //"fbc.ps f21, 86(x25)\n"                               // opcode=0xdd6c8a8b
         //"flw.ps f8, 92(x19)\n"                                // opcode=0x05c9a40b
         "fsrl.pi f9, f14, f16\n"                              // opcode=0x070754fb
         "fsrai.pi f9, f27, 18\n"                              // opcode=0x4f2df4fb
         "fmulh.pi f11, f8, f6\n"                              // opcode=0x166425fb
         "fsrai.pi f6, f26, 30\n"                              // opcode=0x4fed737b
         "fpackrepb.pi f20, f14\n"                             // opcode=0x26070a7b
         "fsetm.pi m5, f26\n"                                  // opcode=0xe00d52fb
         "fsgnjx.s f18, f13, f13\n"                            // opcode=0x20d6a953
         "fround.ps f15, f27, dyn\n"                           // opcode=0x581df7fb
         "fltu.pi f3, f18, f8\n"                               // opcode=0xa68931fb
         "fbci.pi f9, 496196\n"                                // opcode=0x792444df
         "fsub.ps f12, f3, f30, dyn\n"                         // opcode=0x09e1f67b
         "flem.ps m4, f7, f15\n"                               // opcode=0xa0f3c27b
         "fcmovm.ps f21, f25, f22\n"                           // opcode=0x016c8af7
         "fmul.pi f12, f12, f2\n"                              // opcode=0x1626067b
         "fclass.ps f26, f14\n"                                // opcode=0xe0071d7b
         "fsgnjn.s f13, f20, f15\n"                            // opcode=0x20fa16d3
         "fmul.ps f4, f24, f5, dyn\n"                          // opcode=0x105c727b
         "mova.x.m x17\n"                                      // opcode=0xd60008fb
         "fsll.pi f25, f5, f21\n"                              // opcode=0x07529cfb
         "fsrai.pi f9, f21, 26\n"                              // opcode=0x4faaf4fb
         "maskor m1, m1, m0\n"                                 // opcode=0x6600e0fb
         "fclass.s x7, f14\n"                                  // opcode=0xe00713d3
         "fsgnjn.ps f7, f2, f10\n"                             // opcode=0x20a113fb
         "cubesgnsc.ps f28, f6, f31\n"                         // opcode=0x89f32e7b
         "fltm.ps m1, f11, f26\n"                              // opcode=0xa1a5d0fb
         "fsub.pi f6, f22, f18\n"                              // opcode=0x0f2b037b
         "fsgnj.ps f25, f20, f22\n"                            // opcode=0x216a0cfb
         "fclass.s x27, f25\n"                                 // opcode=0xe00c9dd3
         "for.pi f20, f17, f4\n"                               // opcode=0x0648ea7b
         "fpackreph.pi f2, f29\n"                              // opcode=0x260e917b
         "cubesgntc.ps f2, f23, f10\n"                         // opcode=0x88abb17b
         "fslli.pi f4, f18, 20\n"                              // opcode=0x4f49127b
         "fadd.pi f5, f11, f15\n"                              // opcode=0x06f582fb
         "feqm.ps m7, f29, f17\n"                              // opcode=0xa11ee3fb
         "fbci.pi f24, 614008\n"                               // opcode=0x95e78c5f
         "fsrl.pi f7, f22, f15\n"                              // opcode=0x06fb53fb
         "fmsub.s f31, f18, f23, f25, dyn\n"                   // opcode=0xc9797fc7
         "fsll.pi f7, f22, f4\n"                               // opcode=0x064b13fb
         "fmul.s f3, f21, f12, dyn\n"                          // opcode=0x10caf1d3
         "fpackrepb.pi f2, f4\n"                               // opcode=0x2602017b
         "fmul.s f25, f14, f23, dyn\n"                         // opcode=0x11777cd3
         //"fsw f2, 45(x17)\n"                                   // opcode=0x9228a6a7
         "mova.x.m x10\n"                                      // opcode=0xd600057b
         "fnot.pi f28, f10\n"                                  // opcode=0x06052e7b
         "fsgnjx.s f11, f24, f10\n"                            // opcode=0x20ac25d3
         "li x2, 1270\n"
         "mova.m.x x5\n"
         "fmul.ps f17, f29, f30, dyn\n"                        // opcode=0x11eef8fb
         "fpackrepb.pi f12, f5\n"                              // opcode=0x2602867b
         "maskxor m7, m6, m1\n"                                // opcode=0x661343fb
         "fnot.pi f27, f5\n"                                   // opcode=0x0602adfb
         "fbci.pi f11, 284975\n"                               // opcode=0x4592f5df
         "fmadd.s f12, f2, f17, f12, dyn\n"                    // opcode=0x61117643
         "fsub.pi f3, f29, f9\n"                               // opcode=0x0e9e81fb
         "fmulh.pi f13, f28, f29\n"                            // opcode=0x17de26fb
         "fltm.pi m0, f4, f25\n"                               // opcode=0x3f92007b
         "fround.ps f24, f30, dyn\n"                           // opcode=0x581f7c7b
         "fswizz.ps f11, f25, 65\n"                            // opcode=0xe68c95fb
         "fltu.pi f16, f25, f2\n"                              // opcode=0xa62cb87b
         "fsgnjx.ps f8, f10, f31\n"                            // opcode=0x21f5247b
         "feq.ps f28, f31, f18\n"                              // opcode=0xa12fae7b
         "fsrai.pi f7, f16, 19\n"                              // opcode=0x4f3873fb
         "fmul.ps f16, f28, f29, dyn\n"                        // opcode=0x11de787b
         "cubesgntc.ps f4, f17, f14\n"                         // opcode=0x88e8b27b
         "faddi.pi f25, f7, 93\n"                             // opcode=0x95138cbf
         "fsrl.pi f29, f9, f22\n"                              // opcode=0x0764defb
         "fbci.ps f30, 648103\n"                               // opcode=0x9e3a7f1f
         "fround.ps f5, f25, dyn\n"                            // opcode=0x581cf2fb
         //"fsw.ps f23, 96(x24)\n"                               // opcode=0x677c600b
         "faddi.pi f7, f6, 168\n"                              // opcode=0x2c8303bf
         "fsub.pi f4, f6, f25\n"                               // opcode=0x0f93027b
         "fsll.pi f18, f2, f18\n"                              // opcode=0x0721197b
         "fmul.pi f18, f24, f26\n"                             // opcode=0x17ac097b
         "fltu.pi f10, f16, f4\n"                              // opcode=0xa648357b
         "fbcx.ps f20, x3\n"                                   // opcode=0x0001ba0b
         "cubesgnsc.ps f4, f17, f4\n"                          // opcode=0x8848a27b
         "for.pi f25, f23, f16\n"                              // opcode=0x070becfb
         "flem.ps m1, f25, f14\n"                              // opcode=0xa0ecc0fb
         "cubefaceidx.ps f14, f14, f21\n"                      // opcode=0x8957177b
         "fsra.pi f12, f31, f16\n"                             // opcode=0x0f0fd67b
         "fmulh.pi f18, f6, f11\n"                             // opcode=0x16b3297b
         "ffrc.ps f7, f3\n"                                    // opcode=0x582183fb
         "fcmovm.ps f4, f17, f4\n"                             // opcode=0x00488277
         "cubesgntc.ps f23, f11, f21\n"                        // opcode=0x8955bbfb
         "fmadd.ps f17, f18, f24, f15, dyn\n"                  // opcode=0x798978db
         "fsgnjx.s f28, f16, f5\n"                             // opcode=0x20582e53
         "fltm.pi m1, f19, f23\n"                              // opcode=0x3f7980fb
         "fsgnjn.s f24, f13, f10\n"                            // opcode=0x20a69c53
         "fsll.pi f7, f23, f22\n"                              // opcode=0x076b93fb
         "maskpopcz x20, m6\n"                                 // opcode=0x54030a7b
         "fbci.pi f29, 703584\n"                               // opcode=0xabc60edf
         "for.pi f14, f30, f26\n"                              // opcode=0x07af677b
         "for.pi f12, f20, f19\n"                              // opcode=0x073a667b
         "fsgnjn.ps f6, f26, f11\n"                            // opcode=0x20bd137b
         "maskand m4, m5, m5\n"                                // opcode=0x6652f27b
         "fnot.pi f8, f17\n"                                   // opcode=0x0608a47b
         "fclass.ps f22, f18\n"                                // opcode=0xe0091b7b
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "fmulhu.pi f22, f27, f5\n"                            // opcode=0x165d9b7b
         "fand.pi f18, f17, f30\n"                             // opcode=0x07e8f97b
         "flem.ps m5, f23, f28\n"                              // opcode=0xa1cbc2fb
         "fbcx.ps f24, x11\n"                                  // opcode=0x0005bc0b
         "fsgnjn.s f14, f31, f19\n"                            // opcode=0x213f9753
         "fclass.ps f12, f13\n"                                // opcode=0xe006967b
         "maskand m0, m4, m7\n"                                // opcode=0x6672707b
         "flem.ps m2, f2, f29\n"                               // opcode=0xa1d1417b
         "fadd.ps f29, f8, f25, dyn\n"                         // opcode=0x01947efb
         "maskor m6, m7, m1\n"                                 // opcode=0x6613e37b
         "fle.ps f13, f28, f11\n"                              // opcode=0xa0be06fb
         "ffrc.ps f3, f12\n"                                   // opcode=0x582601fb
         "cubesgnsc.ps f11, f2, f24\n"                         // opcode=0x898125fb
         "fpackrepb.pi f10, f9\n"                              // opcode=0x2604857b
         "flem.ps m2, f5, f13\n"                               // opcode=0xa0d2c17b
         //"fsw.ps f6, 103(x30)\n"                               // opcode=0x466f638b
         "fbci.pi f13, 645908\n"                               // opcode=0x9db146df
         "fmul.s f4, f14, f16, dyn\n"                          // opcode=0x11077253
         "fxor.pi f8, f18, f7\n"                               // opcode=0x0679447b
         "for.pi f7, f13, f12\n"                               // opcode=0x06c6e3fb
         "fmul.ps f2, f14, f25, dyn\n"                         // opcode=0x1197717b
         "fround.ps f22, f6, dyn\n"                            // opcode=0x58137b7b
         "maskpopcz x18, m1\n"                                 // opcode=0x5400897b
         //"flw.ps f13, 3(x9)\n"                                 // opcode=0x4034a68b
         "fmul.s f26, f2, f7, dyn\n"                           // opcode=0x10717d53
         "fle.pi f16, f2, f24\n"                               // opcode=0xa781087b
         "mova.m.x x3\n"                                       // opcode=0xd601907b
         //"fsw f10, 107(x16)\n"                                 // opcode=0x16a825a7
         "fandi.pi f10, f26, 25\n"                            // opcode=0xb55d153f
         "mova.m.x x10\n"                                      // opcode=0xd605107b
         //"flw.ps f21, 63(x24)\n"                               // opcode=0xe3fc2a8b
         "cubesgntc.ps f16, f9, f18\n"                         // opcode=0x8924b87b
         "fswizz.ps f27, f29, 191\n"                           // opcode=0xe77efdfb
         "mova.x.m x28\n"                                      // opcode=0xd6000e7b
         //"fbc.ps f21, 10(x5)\n"                                // opcode=0xf0a28a8b
         "mova.x.m x15\n"                                      // opcode=0xd60007fb
         //"fsw.ps f8, 24(x20)\n"                                // opcode=0xd08a6c0b
         "fle.pi f2, f17, f13\n"                               // opcode=0xa6d8817b
         "feq.s x30, f29, f26\n"                               // opcode=0xa1aeaf53
         "fsgnjn.ps f29, f11, f2\n"                            // opcode=0x20259efb
         "fpackreph.pi f26, f5\n"                              // opcode=0x26029d7b
         "fltm.pi m5, f9, f29\n"                               // opcode=0x3fd482fb
         "flt.pi f15, f3, f26\n"                               // opcode=0xa7a197fb
         "fle.pi f31, f21, f14\n"                              // opcode=0xa6ea8ffb
         "fsgnjx.ps f27, f16, f24\n"                           // opcode=0x21882dfb
         "fsra.pi f21, f5, f20\n"                              // opcode=0x0f42dafb
         "mova.x.m x24\n"                                      // opcode=0xd6000c7b
         "fsub.ps f12, f12, f31, dyn\n"                        // opcode=0x09f6767b
         "flt.pi f31, f10, f18\n"                              // opcode=0xa7251ffb
         "fsgnj.s f14, f13, f15\n"                             // opcode=0x20f68753
         "li x2, 2822\n"
         "mova.m.x x5\n"
         //"fbc.ps f7, 69(x12)\n"                                // opcode=0xc456038b
         "fsll.pi f15, f18, f14\n"                             // opcode=0x06e917fb
         "fcmov.ps f16, f8, f3, f5\n"                          // opcode=0x2c34283f
         //"flw.ps f3, 97(x21)\n"                                // opcode=0x5e1aa18b
         "fsgnj.ps f25, f24, f17\n"                            // opcode=0x211c0cfb
         "fpackreph.pi f21, f5\n"                              // opcode=0x26029afb
         "fcmov.ps f9, f30, f20, f17\n"                        // opcode=0x8d4f24bf
         "fmsub.ps f6, f13, f13, f10, dyn\n"                   // opcode=0x52d6f35b
         "fle.pi f12, f11, f2\n"                               // opcode=0xa625867b
         "fsll.pi f23, f12, f26\n"                             // opcode=0x07a61bfb
         "fpackrepb.pi f6, f13\n"                              // opcode=0x2606837b
         "masknot m3, m0\n"                                    // opcode=0x660021fb
         "fnot.pi f8, f7\n"                                    // opcode=0x0603a47b
         "cubesgnsc.ps f29, f26, f19\n"                        // opcode=0x893d2efb
         "fandi.pi f19, f14, 61\n"                            // opcode=0x8d1719bf
         "fmul.ps f24, f13, f30, dyn\n"                        // opcode=0x11e6fc7b
         "fsgnjx.ps f2, f21, f25\n"                            // opcode=0x219aa17b
         "fsgnjn.ps f14, f4, f5\n"                             // opcode=0x2052177b
         "flem.ps m7, f21, f4\n"                               // opcode=0xa04ac3fb
         "feq.ps f22, f16, f14\n"                              // opcode=0xa0e82b7b
         "fclass.ps f15, f23\n"                                // opcode=0xe00b97fb
         "maskpopc x19, m4\n"                                  // opcode=0x520209fb
         "fsgnjn.s f12, f18, f31\n"                            // opcode=0x21f91653
         "fbci.ps f27, 26135\n"                                // opcode=0x06617d9f
         "fslli.pi f23, f9, 28\n"                              // opcode=0x4fc49bfb
         "cubefaceidx.ps f10, f11, f13\n"                      // opcode=0x88d5957b
         "for.pi f21, f23, f8\n"                               // opcode=0x068beafb
         "fbci.ps f30, 949096\n"                               // opcode=0xe7b68f1f
         "fmul.ps f4, f6, f12, dyn\n"                          // opcode=0x10c3727b
         "fmulhu.pi f27, f25, f2\n"                            // opcode=0x162c9dfb
         "fnot.pi f24, f2\n"                                   // opcode=0x06012c7b
         "fbcx.ps f9, x23\n"                                   // opcode=0x000bb48b
         "fadd.s f20, f9, f16, rmm\n"                          // opcode=0x0104ca53
         "fmulh.pi f21, f7, f14\n"                             // opcode=0x16e3aafb
         //"flw f24, 51(x20)\n"                                  // opcode=0x433a2c07
         "fpackrepb.pi f20, f22\n"                             // opcode=0x260b0a7b
         "maskxor m5, m5, m0\n"                                // opcode=0x6602c2fb
         "fsra.pi f12, f10, f13\n"                             // opcode=0x0ed5567b
         "fround.ps f25, f8, dyn\n"                            // opcode=0x58147cfb
         "fsub.ps f17, f16, f5, dyn\n"                         // opcode=0x085878fb
         "maskpopc x30, m0\n"                                  // opcode=0x52000f7b
         "fle.pi f12, f10, f17\n"                              // opcode=0xa715067b
         //"flw.ps f22, 44(x4)\n"                                // opcode=0x92c22b0b
         "fmulh.pi f14, f20, f5\n"                             // opcode=0x165a277b
         "maskpopcz x14, m6\n"                                 // opcode=0x5403077b
         "fsub.pi f2, f11, f4\n"                               // opcode=0x0e45817b
         //"flw f18, 50(x23)\n"                                  // opcode=0x7b2ba907
         "flem.ps m7, f5, f18\n"                               // opcode=0xa122c3fb
         "fsgnjx.ps f8, f10, f6\n"                             // opcode=0x2065247b
         "fbcx.ps f7, x18\n"                                   // opcode=0x0009338b
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
         "fsra.pi f30, f3, f19\n"                              // opcode=0x0f31df7b
         "cubefaceidx.ps f24, f31, f5\n"                       // opcode=0x885f9c7b
         "cubeface.ps f22, f12, f16\n"                         // opcode=0x89060b7b
         "fltm.pi m3, f14, f4\n"                               // opcode=0x3e4701fb
         "fround.ps f12, f11, rup\n"                           // opcode=0x5815b67b
         "fcmovm.ps f27, f15, f22\n"                           // opcode=0x01678df7
         "fle.pi f16, f14, f26\n"                              // opcode=0xa7a7087b
         "fsgnjx.ps f13, f14, f21\n"                           // opcode=0x215726fb
         "fle.ps f26, f16, f11\n"                              // opcode=0xa0b80d7b
         //"fbc.ps f25, 69(x14)\n"                               // opcode=0xac570c8b
         "fsgnjn.ps f22, f3, f14\n"                            // opcode=0x20e19b7b
         "ffrc.ps f20, f8\n"                                   // opcode=0x58240a7b
         "fmsub.ps f6, f19, f31, f8, dyn\n"                    // opcode=0x43f9f35b
         "fand.pi f13, f19, f12\n"                             // opcode=0x06c9f6fb
         "maskor m3, m2, m5\n"                                 // opcode=0x665161fb
         "fclass.s x12, f20\n"                                 // opcode=0xe00a1653
         "fnot.pi f16, f13\n"                                  // opcode=0x0606a87b
         "fsgnjn.s f25, f24, f27\n"                            // opcode=0x21bc1cd3
         "maskpopcz x3, m1\n"                                  // opcode=0x540081fb
         "cubeface.ps f17, f20, f23\n"                         // opcode=0x897a08fb
         "maskpopcz x4, m1\n"                                  // opcode=0x5400827b
         "masknot m2, m6\n"                                    // opcode=0x6603217b
         "fsll.pi f7, f11, f19\n"                              // opcode=0x073593fb
         "fclass.s x7, f15\n"                                  // opcode=0xe00793d3
         "masknot m3, m5\n"                                    // opcode=0x6602a1fb
         "fsgnjx.ps f15, f28, f4\n"                            // opcode=0x204e27fb
         "fadd.pi f19, f21, f20\n"                             // opcode=0x074a89fb
         "maskpopcz x17, m2\n"                                 // opcode=0x540108fb
         "fmul.pi f16, f26, f14\n"                             // opcode=0x16ed087b
         "fle.ps f29, f22, f6\n"                               // opcode=0xa06b0efb
         "fcmov.ps f4, f12, f17, f25\n"                        // opcode=0xcd16223f
         "fle.s x20, f25, f26\n"                               // opcode=0xa1ac8a53
         "masknot m1, m4\n"                                    // opcode=0x660220fb
         "maskpopcz x10, m3\n"                                 // opcode=0x5401857b
         "fmadd.s f9, f3, f11, f28, dyn\n"                     // opcode=0xe0b1f4c3
         "fxor.pi f29, f9, f24\n"                              // opcode=0x0784cefb
         "fbcx.ps f4, x29\n"                                   // opcode=0x000eb20b
         "fsgnjn.ps f9, f3, f3\n"                              // opcode=0x203194fb
         "flt.pi f14, f9, f29\n"                               // opcode=0xa7d4977b
         "fmadd.ps f4, f21, f9, f4, rup\n"                     // opcode=0x209ab25b
         "fsetm.pi m4, f7\n"                                   // opcode=0xe003d27b
         "fandi.pi f29, f11, 317\n"                            // opcode=0x4dd59ebf
         "flt.ps f23, f16, f24\n"                              // opcode=0xa1881bfb
         "masknot m3, m3\n"                                    // opcode=0x6601a1fb
         "cubesgntc.ps f26, f21, f27\n"                        // opcode=0x89babd7b
         "fsgnjx.s f25, f23, f16\n"                            // opcode=0x210bacd3
         "feq.s x20, f15, f20\n"                               // opcode=0xa147aa53
         "fmul.pi f13, f20, f14\n"                             // opcode=0x16ea06fb
         "fmulhu.pi f4, f21, f3\n"                             // opcode=0x163a927b
         "for.pi f11, f9, f25\n"                               // opcode=0x0794e5fb
         "li x5, 0xffffffff\n"
         "mova.m.x x5\n"
         "feqm.ps m0, f13, f24\n"                              // opcode=0xa186e07b
         "mova.x.m x20\n"                                      // opcode=0xd6000a7b
         "fmsub.s f29, f6, f4, f26, rmm\n"                     // opcode=0xd0434ec7
         "fpackrepb.pi f4, f27\n"                              // opcode=0x260d827b
         "mova.m.x x10\n"                                      // opcode=0xd605107b
         "flt.ps f30, f17, f19\n"                              // opcode=0xa1389f7b
         "feqm.ps m0, f15, f4\n"                               // opcode=0xa047e07b
         "fadd.pi f30, f11, f24\n"                             // opcode=0x07858f7b
         "fnot.pi f29, f20\n"                                  // opcode=0x060a2efb
         "fltm.ps m3, f9, f22\n"                               // opcode=0xa164d1fb
         "maskand m5, m4, m4\n"                                // opcode=0x664272fb
         "fle.s x25, f20, f26\n"                               // opcode=0xa1aa0cd3
         "maskand m3, m1, m3\n"                                // opcode=0x6630f1fb
         "fadd.ps f7, f27, f15, dyn\n"                         // opcode=0x00fdf3fb
         "fmadd.ps f5, f27, f18, f14, dyn\n"                   // opcode=0x712df2db
         "flt.s x3, f2, f12\n"                                 // opcode=0xa0c111d3
         "mova.m.x x12\n"                                      // opcode=0xd606107b
         "cubefaceidx.ps f28, f31, f2\n"                       // opcode=0x882f9e7b
         "fsetm.pi m3, f17\n"                                  // opcode=0xe008d1fb
         "maskpopcz x12, m3\n"                                 // opcode=0x5401867b
         "fle.pi f21, f10, f18\n"                              // opcode=0xa7250afb
         "fsub.pi f17, f13, f6\n"                              // opcode=0x0e6688fb
         "fbci.pi f4, 841550\n"                                // opcode=0xcd74e25f
         "fxor.pi f15, f23, f8\n"                              // opcode=0x068bc7fb
         "fswizz.ps f31, f27, 27\n"                            // opcode=0xe63dbffb
         "fslli.pi f12, f19, 31\n"                             // opcode=0x4ff9967b
         //"fsw.ps f12, 60(x7)\n"                                // opcode=0xfac3ee0b
         "fand.pi f28, f20, f5\n"                              // opcode=0x065a7e7b
         "fmadd.ps f2, f28, f24, f2, dyn\n"                    // opcode=0x118e715b
         "feq.ps f30, f3, f3\n"                                // opcode=0xa031af7b
         //"fsw.ps f31, 50(x8)\n"                                // opcode=0x3bf4690b
         "fbcx.ps f9, x27\n"                                   // opcode=0x000db48b
         "fsll.pi f13, f18, f3\n"                              // opcode=0x063916fb
         "flt.s x26, f30, f10\n"                               // opcode=0xa0af1d53
         "fsrl.pi f27, f5, f29\n"                              // opcode=0x07d2ddfb
         "fsrl.pi f27, f12, f16\n"                             // opcode=0x07065dfb
         "feqm.ps m1, f19, f9\n"                               // opcode=0xa099e0fb
         "fltm.ps m0, f15, f26\n"                              // opcode=0xa1a7d07b
         "maskand m6, m0, m7\n"                                // opcode=0x6670737b
         "fsetm.pi m3, f6\n"                                   // opcode=0xe00351fb
         "fsrl.pi f3, f13, f9\n"                               // opcode=0x0696d1fb
         "fsub.ps f12, f13, f2, dyn\n"                         // opcode=0x0826f67b
         "maskand m6, m3, m6\n"                                // opcode=0x6661f37b
         "fsgnjx.ps f24, f22, f14\n"                           // opcode=0x20eb2c7b
         "fmadd.s f5, f3, f23, f8, dyn\n"                      // opcode=0x4171f2c3
         "fsgnjx.s f18, f23, f20\n"                            // opcode=0x214ba953
         "maskpopcz x28, m0\n"                                 // opcode=0x54000e7b
         "fmul.ps f31, f12, f11, rmm\n"                        // opcode=0x10b64ffb
         "feq.ps f4, f21, f6\n"                                // opcode=0xa06aa27b
         "fslli.pi f5, f11, 11\n"                              // opcode=0x4eb592fb
         "li x2, 0xf0f0\n"
         "mova.m.x x5\n"
         "fsgnj.s f6, f17, f15\n"                              // opcode=0x20f88353
         "fle.pi f25, f26, f18\n"                              // opcode=0xa72d0cfb
         "fadd.ps f7, f26, f16, dyn\n"                         // opcode=0x010d73fb
         "fmulhu.pi f20, f19, f14\n"                           // opcode=0x16e99a7b
         "fmul.pi f18, f21, f16\n"                             // opcode=0x170a897b
         "fpackreph.pi f10, f4\n"                              // opcode=0x2602157b
         "fclass.ps f28, f8\n"                                 // opcode=0xe0041e7b
         "fltm.pi m0, f29, f17\n"                              // opcode=0x3f1e807b
         //"flw.ps f26, 24(x2)\n"                                // opcode=0x99812d0b
         "fbci.pi f19, 875678\n"                               // opcode=0xd5c9e9df
         "fxor.pi f13, f17, f18\n"                             // opcode=0x0728c6fb
         "fnot.pi f9, f30\n"                                   // opcode=0x060f24fb
         "feq.ps f13, f19, f21\n"                              // opcode=0xa159a6fb
         "mova.m.x x18\n"                                      // opcode=0xd609107b
         "fmadd.ps f12, f13, f28, f19, dyn\n"                  // opcode=0x99c6f65b
         "fxor.pi f25, f8, f13\n"                              // opcode=0x06d44cfb
         "fbcx.ps f25, x26\n"                                  // opcode=0x000d3c8b
         "fand.pi f23, f21, f28\n"                             // opcode=0x07cafbfb
         "fsra.pi f6, f30, f2\n"                               // opcode=0x0e2f537b
         "feq.pi f12, f16, f12\n"                              // opcode=0xa6c8267b
         "fnot.pi f27, f7\n"                                   // opcode=0x0603adfb
         "fadd.pi f10, f27, f6\n"                              // opcode=0x066d857b
         "fcmovm.ps f20, f29, f3\n"                            // opcode=0x003e8a77
         "fltm.pi m0, f9, f6\n"                                // opcode=0x3e64807b
         //"fsw f20, 39(x9)\n"                                   // opcode=0x2b44a3a7
         "fclass.s x5, f31\n"                                  // opcode=0xe00f92d3
         "fmadd.s f12, f22, f23, f27, rne\n"                   // opcode=0xd97b0643
         "fadd.s f15, f27, f11, dyn\n"                         // opcode=0x00bdf7d3
         "fmulhu.pi f12, f16, f17\n"                           // opcode=0x1718167b
         "fnot.pi f31, f5\n"                                   // opcode=0x0602affb
         "maskpopcz x15, m6\n"                                 // opcode=0x540307fb
         "cubeface.ps f21, f30, f4\n"                          // opcode=0x884f0afb
         "fbci.pi f9, 613980\n"                                // opcode=0x95e5c4df
         "fclass.ps f28, f21\n"                                // opcode=0xe00a9e7b
         "mova.m.x x15\n"                                      // opcode=0xd607907b
         "cubesgnsc.ps f26, f19, f8\n"                         // opcode=0x8889ad7b
         "maskxor m4, m0, m3\n"                                // opcode=0x6630427b
         "ffrc.ps f4, f6\n"                                    // opcode=0x5823027b
         "fxor.pi f15, f11, f7\n"                              // opcode=0x0675c7fb
         "fsrli.pi f17, f17, 3\n"                              // opcode=0x4e38d8fb
         "cubefaceidx.ps f18, f31, f25\n"                      // opcode=0x899f997b
         "fclass.ps f27, f5\n"                                 // opcode=0xe0029dfb
         "fltm.ps m2, f2, f11\n"                               // opcode=0xa0b1517b
         "fsgnj.ps f9, f19, f2\n"                              // opcode=0x202984fb
         "fcmovm.ps f18, f9, f22\n"                            // opcode=0x01648977
         "cubefaceidx.ps f5, f29, f24\n"                       // opcode=0x898e92fb
         "fmulhu.pi f30, f14, f20\n"                           // opcode=0x17471f7b
         "maskpopc x21, m1\n"                                  // opcode=0x52008afb
         "fmadd.ps f6, f19, f4, f19, dyn\n"                    // opcode=0x9849f35b
         "fle.pi f23, f12, f15\n"                              // opcode=0xa6f60bfb
         "li x2, 2946\n"
         "mova.m.x x5\n"
         "fltu.pi f23, f20, f20\n"                             // opcode=0xa74a3bfb
         "fmul.ps f21, f24, f12, dyn\n"                        // opcode=0x10cc7afb
         //"fsw f19, 13(x18)\n"                                  // opcode=0x593926a7
         "fsgnjx.s f17, f26, f6\n"                             // opcode=0x206d28d3
         "faddi.pi f20, f7, 81\n"                             // opcode=0xc4d38a3f
         //"flw f30, 66(x18)\n"                                  // opcode=0x14292f07
         "feqm.ps m5, f31, f17\n"                              // opcode=0xa11fe2fb
         "maskxor m4, m1, m0\n"                                // opcode=0x6600c27b
         "fslli.pi f16, f31, 10\n"                             // opcode=0x4eaf987b
         "fbci.pi f12, 656206\n"                               // opcode=0xa034e65f
         "fswizz.ps f27, f17, 16\n"                            // opcode=0xe6288dfb
         "fsgnj.ps f4, f29, f23\n"                             // opcode=0x217e827b
         //"fsw f16, 51(x2)\n"                                   // opcode=0x430129a7
         "fsgnjn.s f16, f22, f12\n"                            // opcode=0x20cb1853
         "ffrc.ps f17, f15\n"                                  // opcode=0x582788fb
         "fround.ps f23, f20, dyn\n"                           // opcode=0x581a7bfb
         "flem.ps m0, f12, f10\n"                              // opcode=0xa0a6407b
         "fmsub.s f5, f31, f28, f18, dyn\n"                    // opcode=0x91cff2c7
         "flt.s x24, f28, f8\n"                                // opcode=0xa08e1c53
         "fnot.pi f14, f15\n"                                  // opcode=0x0607a77b
         "flt.s x4, f8, f18\n"                                 // opcode=0xa1241253
         "cubeface.ps f14, f8, f20\n"                          // opcode=0x8944077b
         "fltm.ps m5, f21, f18\n"                              // opcode=0xa12ad2fb
         "fmul.pi f2, f17, f15\n"                              // opcode=0x16f8817b
         "fsra.pi f19, f22, f28\n"                             // opcode=0x0fcb59fb
         "fsgnjx.s f9, f21, f30\n"                             // opcode=0x21eaa4d3
         "fround.ps f10, f18, dyn\n"                           // opcode=0x5819757b
         "maskand m3, m3, m2\n"                                // opcode=0x6621f1fb
         "fmsub.s f29, f6, f22, f20, dyn\n"                    // opcode=0xa1637ec7
         "ffrc.ps f29, f14\n"                                  // opcode=0x58270efb
         "feq.pi f15, f15, f13\n"                              // opcode=0xa6d7a7fb
         "fltm.pi m4, f5, f24\n"                               // opcode=0x3f82827b
         "fandi.pi f28, f21, 412\n"                            // opcode=0x65ca9e3f
         "fltm.pi m1, f26, f15\n"                              // opcode=0x3efd00fb
         "fltu.pi f7, f10, f27\n"                              // opcode=0xa7b533fb
         "fsll.pi f23, f26, f2\n"                              // opcode=0x062d1bfb
         "fbci.ps f31, 751567\n"                               // opcode=0xb77cff9f
         "fbcx.ps f26, x7\n"                                   // opcode=0x0003bd0b
         "fslli.pi f2, f30, 11\n"                              // opcode=0x4ebf117b
         "fmulhu.pi f17, f25, f22\n"                           // opcode=0x176c98fb
         "fmul.ps f17, f17, f28, dyn\n"                        // opcode=0x11c8f8fb
         "fclass.s x17, f15\n"                                 // opcode=0xe00798d3
         "fmulhu.pi f8, f3, f29\n"                             // opcode=0x17d1947b
         //"fbc.ps f14, 49(x23)\n"                               // opcode=0xab1b870b
         "fmul.pi f6, f26, f19\n"                              // opcode=0x173d037b
         "fsgnjn.s f5, f26, f22\n"                             // opcode=0x216d12d3
         //"fbc.ps f4, 33(x24)\n"                                // opcode=0x221c020b
         "maskand m6, m4, m1\n"                                // opcode=0x6612737b
         "fsub.s f12, f22, f15, dyn\n"                         // opcode=0x08fb7653
         "fsgnj.ps f9, f9, f31\n"                              // opcode=0x21f484fb
         "li x5, 0xffffffffffffffff\n"
         "mova.m.x x5\n"
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
