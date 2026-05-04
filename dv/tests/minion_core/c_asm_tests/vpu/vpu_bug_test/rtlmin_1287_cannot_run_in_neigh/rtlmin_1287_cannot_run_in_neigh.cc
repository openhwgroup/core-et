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
      0xbf800000,    // -1.0                  
      0x7fc00001,    // signaling NaN         
      0x00011111,    // 9.7958E-41            
      0x4b000000,    // 8388608.0             
      0x007fffff,    // 1.1754942E-38         
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            

      // vpu register f3
      0xffc00001,    // -signaling NaN        
      0xffc00000,    // -quiet NaN            
      0xbf800000,    // -1.0                  
      0x7f800000,    // inf                   
      0xffc00000,    // -quiet NaN            
      0x80000000,    // -zero                 
      0x80011111,    // -9.7958E-41           
      0x7fc00000,    // quiet NaN             

      // vpu register f4
      0xcb8c4b40,    // -18388608.0           
      0x80000001,    // -1.4E-45 (-denorm)    
      0xbf028f5c,    // -0.51                 
      0x3f028f5c,    // 0.51                  
      0xbf028f5c,    // -0.51                 
      0x00000000,    // zero                  
      0xffc00000,    // -quiet NaN            
      0x7fc00001,    // signaling NaN         

      // vpu register f5
      0x3f800000,    // 1.0                   
      0x3f028f5c,    // 0.51                  
      0xffc00001,    // -signaling NaN        
      0x80000000,    // -zero                 
      0x007fffff,    // 1.1754942E-38         
      0x00000001,    // 1.4E-45 (+denorm)     
      0x00000001,    // 1.4E-45 (+denorm)     
      0xffc00000,    // -quiet NaN            

      // vpu register f6
      0xff800000,    // -inf                  
      0x00000000,    // zero                  
      0x00000000,    // zero                  
      0xcb8c4b40,    // -18388608.0           
      0xcb8c4b40,    // -18388608.0           
      0xffc00000,    // -quiet NaN            
      0xffc00001,    // -signaling NaN        
      0x4b000000,    // 8388608.0             

      // vpu register f7
      0xffc00000,    // -quiet NaN            
      0x7fc00001,    // signaling NaN         
      0x00000000,    // zero                  
      0xbf028f5c,    // -0.51                 
      0x7fc00000,    // quiet NaN             
      0xbf800000,    // -1.0                  
      0xcb000000,    // -8388608.0            
      0xcb000000,    // -8388608.0            

      // vpu register f8
      0x007fffff,    // 1.1754942E-38         
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0xffc00001,    // -signaling NaN        
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0xcb8c4b40,    // -18388608.0           

      // vpu register f9
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xcb000000,    // -8388608.0            
      0xcb000000,    // -8388608.0            
      0x7fc00001,    // signaling NaN         
      0x4b8c4b40,    // 18388608.0            
      0x7f800000,    // inf                   
      0x7fc00001,    // signaling NaN         

      // vpu register f10
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0x007fffff,    // 1.1754942E-38         
      0xffc00001,    // -signaling NaN        
      0x00011111,    // 9.7958E-41            
      0x7f800000,    // inf                   
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00000,    // quiet NaN             

      // vpu register f11
      0x3f800000,    // 1.0                   
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0xcb000000,    // -8388608.0            
      0xffc00000,    // -quiet NaN            
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0x80011111,    // -9.7958E-41           

      // vpu register f12
      0x00000001,    // 1.4E-45 (+denorm)     
      0x80000000,    // -zero                 
      0xcb000000,    // -8388608.0            
      0xbf800000,    // -1.0                  
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0x00000000,    // zero                  
      0x4b8c4b40,    // 18388608.0            

      // vpu register f13
      0x4b000000,    // 8388608.0             
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0xffc00000,    // -quiet NaN            
      0x00000000,    // zero                  
      0x7fc00000,    // quiet NaN             
      0x00000001,    // 1.4E-45 (+denorm)     
      0x7fc00001,    // signaling NaN         

      // vpu register f14
      0xcb000000,    // -8388608.0            
      0x80011111,    // -9.7958E-41           
      0xcb8c4b40,    // -18388608.0           
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0xcb8c4b40,    // -18388608.0           
      0x80011111,    // -9.7958E-41           
      0x7f800000,    // inf                   

      // vpu register f15
      0xcb000000,    // -8388608.0            
      0xffc00000,    // -quiet NaN            
      0xcb8c4b40,    // -18388608.0           
      0x00011111,    // 9.7958E-41            
      0xbf028f5c,    // -0.51                 
      0xffc00001,    // -signaling NaN        
      0x80011111,    // -9.7958E-41           
      0x80000000,    // -zero                 

      // vpu register f16
      0xffc00001,    // -signaling NaN        
      0xff800000,    // -inf                  
      0x00011111,    // 9.7958E-41            
      0xbf800000,    // -1.0                  
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    

      // vpu register f17
      0xcb000000,    // -8388608.0            
      0x4b000000,    // 8388608.0             
      0xcb000000,    // -8388608.0            
      0x3f028f5c,    // 0.51                  
      0x7fc00001,    // signaling NaN         
      0x00000000,    // zero                  
      0x3f800000,    // 1.0                   
      0xbf800000,    // -1.0                  

      // vpu register f18
      0x7fc00000,    // quiet NaN             
      0x7fc00001,    // signaling NaN         
      0xff800000,    // -inf                  
      0x007fffff,    // 1.1754942E-38         
      0x4b000000,    // 8388608.0             
      0x80000001,    // -1.4E-45 (-denorm)    
      0x4b000000,    // 8388608.0             
      0x80000000,    // -zero                 

      // vpu register f19
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0x4b8c4b40,    // 18388608.0            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00001,    // -signaling NaN        
      0x007fffff,    // 1.1754942E-38         
      0x00000000,    // zero                  

      // vpu register f20
      0x00011111,    // 9.7958E-41            
      0xffc00001,    // -signaling NaN        
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0x00000000,    // zero                  
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f21
      0xff800000,    // -inf                  
      0x00000001,    // 1.4E-45 (+denorm)     
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0x3f028f5c,    // 0.51                  
      0x80000000,    // -zero                 
      0x00000000,    // zero                  
      0xcb8c4b40,    // -18388608.0           

      // vpu register f22
      0xffc00001,    // -signaling NaN        
      0x3f028f5c,    // 0.51                  
      0x00011111,    // 9.7958E-41            
      0x00000000,    // zero                  
      0xff800000,    // -inf                  
      0x007fffff,    // 1.1754942E-38         
      0xff800000,    // -inf                  
      0xcb000000,    // -8388608.0            

      // vpu register f23
      0x3f800000,    // 1.0                   
      0x7fc00001,    // signaling NaN         
      0x80000000,    // -zero                 
      0xff800000,    // -inf                  
      0xff800000,    // -inf                  
      0xcb8c4b40,    // -18388608.0           
      0x80000000,    // -zero                 
      0x4b8c4b40,    // 18388608.0            

      // vpu register f24
      0xff800000,    // -inf                  
      0x7fc00000,    // quiet NaN             
      0xcb8c4b40,    // -18388608.0           
      0x3f800000,    // 1.0                   
      0x7fc00000,    // quiet NaN             
      0x4b000000,    // 8388608.0             
      0x00011111,    // 9.7958E-41            
      0x00011111,    // 9.7958E-41            

      // vpu register f25
      0xcb8c4b40,    // -18388608.0           
      0xff800000,    // -inf                  
      0xbf028f5c,    // -0.51                 
      0xbf800000,    // -1.0                  
      0x00011111,    // 9.7958E-41            
      0x4b8c4b40,    // 18388608.0            
      0x7f800000,    // inf                   
      0xff800000,    // -inf                  

      // vpu register f26
      0x007fffff,    // 1.1754942E-38         
      0x80000000,    // -zero                 
      0x4b000000,    // 8388608.0             
      0xffc00000,    // -quiet NaN            
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   

      // vpu register f27
      0x80011111,    // -9.7958E-41           
      0x7fc00001,    // signaling NaN         
      0x80000001,    // -1.4E-45 (-denorm)    
      0xff800000,    // -inf                  
      0xbf800000,    // -1.0                  
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0xbf028f5c,    // -0.51                 

      // vpu register f28
      0xbf028f5c,    // -0.51                 
      0x3f800000,    // 1.0                   
      0x80000001,    // -1.4E-45 (-denorm)    
      0x3f028f5c,    // 0.51                  
      0x00000000,    // zero                  
      0x4b8c4b40,    // 18388608.0            
      0xbf028f5c,    // -0.51                 
      0x00000001,    // 1.4E-45 (+denorm)     

      // vpu register f29
      0x4b000000,    // 8388608.0             
      0x4b000000,    // 8388608.0             
      0x4b000000,    // 8388608.0             
      0x4b8c4b40,    // 18388608.0            
      0x7fc00001,    // signaling NaN         
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0xbf028f5c,    // -0.51                 

      // vpu register f30
      0x80000000,    // -zero                 
      0x3f028f5c,    // 0.51                  
      0xff800000,    // -inf                  
      0xffc00000,    // -quiet NaN            
      0xffc00000,    // -quiet NaN            
      0x3f028f5c,    // 0.51                  
      0x3f800000,    // 1.0                   
      0x7fc00001,    // signaling NaN         

      // vpu register f31
      0xbf800000,    // -1.0                  
      0xbf028f5c,    // -0.51                 
      0x00011111,    // 9.7958E-41            
      0x80000001,    // -1.4E-45 (-denorm)    
      0xffc00000,    // -quiet NaN            
      0x80000000,    // -zero                 
      0xcb000000,    // -8388608.0            
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
         "li x1,  0x8056a6c0\n"
         "li x2,  0x808c20a0\n"
         "li x3,  0x80bec820\n"
         "li x4,  0x80347f20\n"
         "li x5,  0x80299c20\n"
         "li x6,  0x80396b60\n"
         "li x7,  0x80a9e7c0\n"
         "li x8,  0x806bb760\n"
         "li x9,  0x80900560\n"
         "li x10,  0x802fa4a0\n"
         "li x11,  0x803c9b40\n"
         "li x12,  0x802aec00\n"
         "li x13,  0x80568d20\n"
         "li x14,  0x80556e80\n"
         "li x15,  0x806447c0\n"
         "li x16,  0x8084dfa0\n"
         "li x17,  0x802e1dc0\n"
         "li x18,  0x8098eb80\n"
         "li x19,  0x807d2920\n"
         "li x20,  0x80e70240\n"
         "li x21,  0x801076c0\n"
         "li x22,  0x805f3800\n"
         "li x23,  0x801330a0\n"
         "li x24,  0x802983a0\n"
         "li x25,  0x80262420\n"
         "li x26,  0x80a73920\n"
         "li x27,  0x80421860\n"
         "li x28,  0x806a38e0\n"
         "li x29,  0x80477660\n"
         "li x30,  0x80238a40\n"
         "li x31,  0x8003fd40\n"

         "fadd.ps f27, f16, f12, dyn\n"                        // opcode=0x00c87dfb
         "fmulhu.pi f8, f7, f9\n"                              // opcode=0x1693947b
         "flw.ps f29, 92(x3)\n"                                // opcode=0xcdc1ae8b
         "flt.pi f28, f22, f31\n"                              // opcode=0xa7fb1e7b
         "fle.s x15, f8, f26\n"                                // opcode=0xa1a407d3
         "ffrc.ps f31, f12\n"                                  // opcode=0x58260ffb
         "feq.s x28, f18, f27\n"                               // opcode=0xa1b92e53
         "fand.pi f26, f8, f2\n"                               // opcode=0x06247d7b
         "fpackrepb.pi f14, f31\n"                             // opcode=0x260f877b
         "maskxor m1, m2, m6\n"                                // opcode=0x666140fb
         "flem.ps m0, f15, f24\n"                              // opcode=0xa187c07b
         "fcmov.ps f21, f26, f15, f26\n"                       // opcode=0xd4fd2abf
         "fle.pi f4, f2, f30\n"                                // opcode=0xa7e1027b
         "maskand m0, m0, m6\n"                                // opcode=0x6660707b
         "fsw.ps f5, 86(x31)\n"                                // opcode=0xf45feb0b
         "fbci.pi f17, 15476\n"                                // opcode=0x03c748df
         "fmul.s f5, f2, f31, dyn\n"                           // opcode=0x11f172d3
         "fmsub.s f29, f20, f24, f20, dyn\n"                   // opcode=0xa18a7ec7
         "fpackrepb.pi f8, f14\n"                              // opcode=0x2607047b
         "feq.s x8, f25, f15\n"                                // opcode=0xa0fca453
         "maskor m6, m3, m5\n"                                 // opcode=0x6651e37b
         "maskand m6, m1, m6\n"                                // opcode=0x6660f37b
         "fxor.pi f16, f4, f24\n"                              // opcode=0x0782487b
         "fmadd.ps f31, f27, f30, f26, dyn\n"                  // opcode=0xd1edffdb
         "fclass.ps f26, f3\n"                                 // opcode=0xe0019d7b
         "fclass.ps f13, f4\n"                                 // opcode=0xe00216fb
         "fsub.ps f5, f23, f4, dyn\n"                          // opcode=0x084bf2fb
         "fsll.pi f12, f12, f5\n"                              // opcode=0x0656167b
         "fbc.ps f20, 93(x26)\n"                               // opcode=0x15dd0a0b
         "feqm.ps m5, f6, f26\n"                               // opcode=0xa1a362fb
         "cubeface.ps f14, f20, f10\n"                         // opcode=0x88aa077b
         "faddi.pi f16, f23, 200\n"                            // opcode=0x348b883f
         "fround.ps f8, f13, dyn\n"                            // opcode=0x5816f47b
         "fswizz.ps f12, f29, 21\n"                            // opcode=0xe62ed67b
         "maskxor m0, m1, m7\n"                                // opcode=0x6670c07b
         "maskpopc x16, m5\n"                                  // opcode=0x5202887b
         "cubefaceidx.ps f11, f7, f16\n"                       // opcode=0x890395fb
         "fltm.ps m4, f20, f23\n"                              // opcode=0xa17a527b
         "fle.s x26, f11, f13\n"                               // opcode=0xa0d58d53
         "fpackreph.pi f7, f6\n"                               // opcode=0x260313fb
         "fadd.pi f21, f20, f13\n"                             // opcode=0x06da0afb
         "fsgnjn.ps f21, f15, f14\n"                           // opcode=0x20e79afb
         "fcmovm.ps f10, f4, f16\n"                            // opcode=0x01020577
         "fsub.s f21, f4, f25, dyn\n"                          // opcode=0x09927ad3
         "fcmovm.ps f15, f25, f17\n"                           // opcode=0x011c87f7
         "fadd.s f3, f11, f5, rne\n"                           // opcode=0x005581d3
         "fbcx.ps f29, x15\n"                                  // opcode=0x0007be8b
         "fpackrepb.pi f12, f7\n"                              // opcode=0x2603867b
         "fsw.ps f8, 75(x13)\n"                                // opcode=0x6c86e58b
         "feq.pi f3, f9, f5\n"                                 // opcode=0xa654a1fb
         "fltu.pi f4, f11, f30\n"                              // opcode=0xa7e5b27b
         "fltm.pi m5, f16, f28\n"                              // opcode=0x3fc802fb
         "feqm.ps m2, f10, f23\n"                              // opcode=0xa175617b       bemu expects m2 = 0x04 - matches
         // adding these 3 nops gets the test to pass, since this gives the older instruction enough time to write to mask reg before the younger mova.m.x instruction overwrits it. 
         //"nop\n"
         //"nop\n"
         //"nop\n"
         "mova.m.x x21\n"                                      // opcode=0xd60a907b       bemu expects m2 = 0x10, m6 = 
         "feqm.ps m6, f27, f20\n"                              // opcode=0xa14de37b       bemu expects 0x40 - matches rtl
         "maskxor m4, m6, m2\n"                                // opcode=0x6623427b
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
