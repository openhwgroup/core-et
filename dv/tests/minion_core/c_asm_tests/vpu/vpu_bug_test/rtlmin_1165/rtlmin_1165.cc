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
      0xffc00001,    // -signaling NaN        
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00000,    // -quiet NaN            

      // vpu register f3
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            
      0xcb000000,    // -8388608.0            
      0x00000000,    // zero                  

      // vpu register f4
      0x007fffff,    // 1.1754942E-38         
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  

      // vpu register f5
      0xffc00001,    // -signaling NaN        
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb8c4b40,    // -18388608.0           
      0x80000000,    // -zero                 
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            

      // vpu register f6
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0x7fc00001,    // signaling NaN         
      0x00000000,    // zero                  

      // vpu register f7
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0xffc00000,    // -quiet NaN            
      0x7fc00000,    // quiet NaN             
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x00000000,    // zero                  

      // vpu register f8
      0x7fc00001,    // signaling NaN         
      0x7fc00001,    // signaling NaN         
      0x7f800000,    // inf                   
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           
      0xff800000,    // -inf                  
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  

      // vpu register f9
      0x80011111,    // -9.7958E-41           
      0xffc00001,    // -signaling NaN        
      0x7f800000,    // inf                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x007fffff,    // 1.1754942E-38         
      0x80011111,    // -9.7958E-41           

      // vpu register f10
      0x4b000000,    // 8388608.0             
      0xff800000,    // -inf                  
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x80000000,    // -zero                 
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0xffc00000,    // -quiet NaN            

      // vpu register f11
      0x7fc00001,    // signaling NaN         
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f12
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0x80011111,    // -9.7958E-41           
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f13
      0x4b8c4b40,    // 18388608.0            
      0xffc00001,    // -signaling NaN        
      0x4b8c4b40,    // 18388608.0            
      0x007fffff,    // 1.1754942E-38         
      0x7f800000,    // inf                   
      0x80011111,    // -9.7958E-41           
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f14
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00000,    // -quiet NaN            
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000000,    // zero                  
      0x00000000,    // zero                  
      0xff800000,    // -inf                  
      0x4b8c4b40,    // 18388608.0            
      0x00011111,    // 9.7958E-41            

      // vpu register f15
      0x80000001,    // -1.4E-45 (-denorm)    
      0x80000000,    // -zero                 
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0x80000000,    // -zero                 
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f16
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0x7fc00001,    // signaling NaN         
      0x00000000,    // zero                  
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0xcb8c4b40,    // -18388608.0           

      // vpu register f17
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         
      0xffc00000,    // -quiet NaN            
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0x7f800000,    // inf                   

      // vpu register f18
      0x00011111,    // 9.7958E-41            
      0x3f800000,    // 1.0                   
      0xcb000000,    // -8388608.0            
      0x80000000,    // -zero                 
      0x7fc00000,    // quiet NaN             
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0xcb8c4b40,    // -18388608.0           

      // vpu register f19
      0x4b8c4b40,    // 18388608.0            
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0xff800000,    // -inf                  
      0xffc00000,    // -quiet NaN            
      0x7fc00001,    // signaling NaN         
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f20
      0x80000001,    // -1.4E-45 (-denorm)    
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00011111,    // 9.7958E-41            
      0x00000000,    // zero                  
      0xcb8c4b40,    // -18388608.0           
      0x4b8c4b40,    // 18388608.0            
      0x80000000,    // -zero                 
      0x4b8c4b40,    // 18388608.0            

      // vpu register f21
      0x7fc00001,    // signaling NaN         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            
      0x4b000000,    // 8388608.0             

      // vpu register f22
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 
      0xffc00001,    // -signaling NaN        
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x7fc00000,    // quiet NaN             

      // vpu register f23
      0xbf800000,    // -1.0                  
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0x4b8c4b40,    // 18388608.0            
      0x7fc00000,    // quiet NaN             
      0x3f800000,    // 1.0                   
      0xcb8c4b40,    // -18388608.0           

      // vpu register f24
      0x4b8c4b40,    // 18388608.0            
      0x7f800000,    // inf                   
      0x007fffff,    // 1.1754942E-38         
      0x00000000,    // zero                  
      0x7f800000,    // inf                   
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0x7fc00001,    // signaling NaN         

      // vpu register f25
      0x4b000000,    // 8388608.0             
      0x80011111,    // -9.7958E-41           
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00001,    // signaling NaN         

      // vpu register f26
      0xcb8c4b40,    // -18388608.0           
      0xbf028f5c,    // -0.51                 
      0x80011111,    // -9.7958E-41           
      0x7fc00001,    // signaling NaN         
      0xffc00001,    // -signaling NaN        
      0x7f800000,    // inf                   
      0x00000000,    // zero                  
      0xbf028f5c,    // -0.51                 

      // vpu register f27
      0x00000001,    // 1.4E-45 (+denorm)     
      0x4b000000,    // 8388608.0             
      0x7fc00000,    // quiet NaN             
      0x3f028f5c,    // 0.51                  
      0x4b8c4b40,    // 18388608.0            
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            

      // vpu register f28
      0xcb8c4b40,    // -18388608.0           
      0xcb000000,    // -8388608.0            
      0x007fffff,    // 1.1754942E-38         
      0xffc00001,    // -signaling NaN        
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x7fc00000,    // quiet NaN             
      0xbf800000,    // -1.0                  

      // vpu register f29
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000000,    // -zero                 
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0x7f800000,    // inf                   

      // vpu register f30
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0x3f800000,    // 1.0                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0xcb8c4b40,    // -18388608.0           
      0x7fc00001,    // signaling NaN         
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            

      // vpu register f31
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 
      0x00000001,    // 1.4E-45 (+denorm)     
      0x007fffff,    // 1.1754942E-38         
      0xff800000,    // -inf                  
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
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
         "li x1,  0x802cd1a0\n"
         "li x2,  0x80025fc0\n"
         "li x3,  0x80cb2700\n"
         "li x4,  0x80c51c20\n"
         "li x5,  0x80b2b280\n"
         "li x6,  0x80e6f7e0\n"
         "li x7,  0x803a40e0\n"
         "li x8,  0x8096a4c0\n"
         "li x9,  0x80238700\n"
         "li x10,  0x8005bb00\n"
         "li x11,  0x801eed80\n"
         "li x12,  0x80fd0f60\n"
         "li x13,  0x808cdd60\n"
         "li x14,  0x802783c0\n"
         "li x15,  0x80483960\n"
         "li x16,  0x804e9ca0\n"
         "li x17,  0x80b55f40\n"
         "li x18,  0x80c175c0\n"
         "li x19,  0x80d07d20\n"
         "li x20,  0x80dcf800\n"
         "li x21,  0x80495fe0\n"
         "li x22,  0x802abb20\n"
         "li x23,  0x80992100\n"
         "li x24,  0x8033d860\n"
         "li x25,  0x80903240\n"
         "li x26,  0x80a38d40\n"
         "li x27,  0x80a2f7a0\n"
         "li x28,  0x80731d00\n"
         "li x29,  0x800810e0\n"
         "li x30,  0x80c878c0\n"
         "li x31,  0x802797a0\n"

         //"fgh.ps f30, f1(x31)\n"                               // opcode=0x51f09f0b
         "fcmov.ps f14, f31, f17, f19\n"                       // opcode=0x9d1fa73f
         "maskpopc x10, m6\n"                                  // opcode=0x5203057b
         "fle.pi f3, f19, f30\n"                               // opcode=0xa7e981fb
         "fmsub.ps f31, f26, f8, f13, rtz\n"                   // opcode=0x6a8d1fdb
         "feq.s x24, f14, f21\n"                               // opcode=0xa1572c53
         //"fgh.ps f11, f1(x3)\n"                                // opcode=0x5030958b
         "fsra.pi f19, f6, f20\n"                              // opcode=0x0f4359fb
         //"fsw.ps f20, 73(x24)\n"                               // opcode=0x554c648b
         "fadd.s f12, f17, f11, dyn\n"                         // opcode=0x00b8f653
         "fcvt.ps.f16 f19, f3\n"                               // opcode=0xd0a189fb
         "frcp.ps f5, f12\n"                                   // opcode=0x587602fb
         "fmin.s f8, f19, f12\n"                               // opcode=0x28c98453
         //"fg32b.ps f13, x1(x18)\n"                             // opcode=0x0920968b
         "faddi.pi f19, f19, 149\n"                            // opcode=0x255989bf
         //"fg32h.ps f15, x0(x13)\n"                             // opcode=0x10d0178b
         "fmin.s f3, f17, f4\n"                                // opcode=0x284881d3
         //"fsw.ps f5, 109(x23)\n"                               // opcode=0xa65be68b
         "fcmov.ps f31, f15, f18, f31\n"                       // opcode=0xfd27afbf
         "fsgnjn.ps f19, f24, f23\n"                           // opcode=0x217c19fb
         "feq.pi f29, f19, f18\n"                              // opcode=0xa729aefb
         "fcvt.f16.ps f12, f29\n"                              // opcode=0xd89e867b
         "flem.ps m0, f10, f12\n"                              // opcode=0xa0c5407b
         //"fcvt.pw.ps f10, f20, rne\n"                          // opcode=0xc00a057b
         "feq.s x21, f6, f17\n"                                // opcode=0xa1132ad3
         "cubesgntc.ps f18, f28, f11\n"                        // opcode=0x88be397b
         "fmin.ps f17, f9, f21\n"                              // opcode=0x295488fb
         "fcvt.ps.pw f24, f29, dyn\n"                          // opcode=0xd00efc7b
         "fclass.s x29, f11\n"                                 // opcode=0xe0059ed3
         //"fgw.ps f14, f0(x22)\n"                               // opcode=0x6160170b
         "fcvt.w.s x24, f23, dyn\n"                            // opcode=0xc00bfc53
         "cubeface.ps f4, f4, f20\n"                           // opcode=0x8942027b
         "fsat8.pi f26, f10\n"                                 // opcode=0x06053d7b
         "for.pi f16, f17, f20\n"                              // opcode=0x0748e87b
         "fmulh.pi f13, f10, f29\n"                            // opcode=0x17d526fb
         "fmul.pi f20, f30, f13\n"                             // opcode=0x16df0a7b
         //"fscw.ps f26, f0(x25)\n"                              // opcode=0xe1901d0b
         "fbci.pi f18, 610641\n"                               // opcode=0x9515195f
         "flt.pi f17, f15, f2\n"                               // opcode=0xa62798fb
         "fminu.pi f4, f27, f30\n"                             // opcode=0x2feda27b
         "fcvt.ps.pwu f3, f30, rne\n"                          // opcode=0xd01f01fb
         "fadd.ps f16, f27, f30, dyn\n"                        // opcode=0x01edf87b
         "fand.pi f4, f21, f7\n"                               // opcode=0x067af27b
         "maskxor m1, m1, m0\n"                                // opcode=0x6600c0fb
         "flt.pi f20, f14, f18\n"                              // opcode=0xa7271a7b
         "fmax.ps f14, f28, f2\n"                              // opcode=0x282e177b
         "fnmadd.s f20, f22, f29, f29, dyn\n"                  // opcode=0xe9db7a4f
         "fnmsub.s f15, f25, f21, f10, dyn\n"                  // opcode=0x515cf7cb
         "fxor.pi f30, f6, f27\n"                              // opcode=0x07b34f7b
         "fcmov.ps f11, f17, f4, f31\n"                        // opcode=0xfc48a5bf
         "fbci.pi f23, 193182\n"                               // opcode=0x2f29ebdf
         "fclass.s x9, f14\n"                                  // opcode=0xe00714d3
         //"flw f10, 101(x3)\n"                                  // opcode=0x2e51a507
         "flem.ps m4, f23, f20\n"                              // opcode=0xa14bc27b
         //"fsq2 f5, 24(x20)\n"                                  // opcode=0xb85a5c27
         //"fscw.ps f9, f1(x8)\n"                                // opcode=0xe080948b
         "maskpopc x28, m4\n"                                  // opcode=0x52020e7b
         "feq.pi f30, f4, f30\n"                               // opcode=0xa7e22f7b
         "fpackreph.pi f30, f9\n"                              // opcode=0x26049f7b
         "fand.pi f21, f6, f22\n"                              // opcode=0x07637afb
         //"fscw.ps f6, f1(x16)\n"                               // opcode=0xe100930b
         //"flw.ps f9, 111(x14)\n"                               // opcode=0x56f7248b
         "flt.pi f17, f6, f20\n"                               // opcode=0xa74318fb
         "fle.s x20, f7, f28\n"                                // opcode=0xa1c38a53
         "fbcx.ps f12, x26\n"                                  // opcode=0x000d360b
         "feq.s x20, f19, f26\n"                               // opcode=0xa1a9aa53
         "fsrli.pi f14, f8, 14\n"                              // opcode=0x4ee4577b
         "fmul.s f3, f26, f25, dyn\n"                          // opcode=0x119d71d3
         "fmaxu.pi f14, f4, f4\n"                              // opcode=0x2e42377b
         "fclass.s x21, f12\n"                                 // opcode=0xe0061ad3
         "fcvt.f11.ps f26, f4\n"                               // opcode=0xd8820d7b
         "maskpopc x5, m6\n"                                   // opcode=0x520302fb
         //"fsw.ps f2, 60(x10)\n"                                // opcode=0xb2256e0b
         "fmadd.s f26, f15, f28, f29, dyn\n"                   // opcode=0xe9c7fd43

         //"fbc.ps f14, 57(x25)\n"                               // opcode=0xd39c870b
         "fcvt.ps.f11 f12, f3\n"                               // opcode=0xd091867b
         "fsgnjx.ps f7, f12, f8\n"                             // opcode=0x208623fb
         "fclass.s x6, f21\n"                                  // opcode=0xe00a9353
         //"fsc32h.ps f30, x1(x16)\n"                            // opcode=0x91009f0b
         ////RTLMIN-5479"fsin.ps f10, f23\n"                                  // opcode=0x586b857b
         "fcvt.wu.s x22, f14, dyn\n"                           // opcode=0xc0177b53
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
