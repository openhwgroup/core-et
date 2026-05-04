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
			 0xff800000,    // -inf                  
			 0xbf028f5c,    // -0.51                 
			 0x3f800000,    // 1.0                   
			 0xbf800000,    // -1.0                  
			 0xffc00000,    // -quiet NaN            
			 0x3f028f5c,    // 0.51                  
			 0xbf800000,    // -1.0                  
			 0xbf028f5c,    // -0.51                 

			 // vpu register f3
			 0x80011111,    // -9.7958E-41           
			 0xcb8c4b40,    // -18388608.0           
			 0x3f800000,    // 1.0                   
			 0x3f800000,    // 1.0                   
			 0x3f800000,    // 1.0                   
			 0x80011111,    // -9.7958E-41           
			 0xffc00001,    // -signaling NaN        
			 0x7fc00001,    // signaling NaN         

			 // vpu register f4
			 0x00000000,    // zero                  
			 0x7fc00001,    // signaling NaN         
			 0xff800000,    // -inf                  
			 0x4b000000,    // 8388608.0             
			 0xbf800000,    // -1.0                  
			 0x4b8c4b40,    // 18388608.0            
			 0xbf028f5c,    // -0.51                 
			 0x3f800000,    // 1.0                   

			 // vpu register f5
			 0x80000000,    // -zero                 
			 0x00000000,    // zero                  
			 0xcb8c4b40,    // -18388608.0           
			 0xbf800000,    // -1.0                  
			 0x7fc00000,    // quiet NaN             
			 0xff800000,    // -inf                  
			 0xbf800000,    // -1.0                  
			 0x7fc00001,    // signaling NaN         

			 // vpu register f6
			 0xbf800000,    // -1.0                  
			 0x80000000,    // -zero                 
			 0xffc00000,    // -quiet NaN            
			 0x3f028f5c,    // 0.51                  
			 0x00011111,    // 9.7958E-41            
			 0x7fc00000,    // quiet NaN             
			 0xbf800000,    // -1.0                  
			 0xffc00001,    // -signaling NaN        

			 // vpu register f7
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x00000000,    // zero                  
			 0x80011111,    // -9.7958E-41           
			 0x00011111,    // 9.7958E-41            
			 0xcb8c4b40,    // -18388608.0           
			 0xffc00001,    // -signaling NaN        
			 0xcb000000,    // -8388608.0            
			 0xff800000,    // -inf                  

			 // vpu register f8
			 0x80000000,    // -zero                 
			 0x3f800000,    // 1.0                   
			 0x80000000,    // -zero                 
			 0xbf800000,    // -1.0                  
			 0x3f800000,    // 1.0                   
			 0x007fffff,    // 1.1754942E-38         
			 0x4b000000,    // 8388608.0             
			 0x00000000,    // zero                  

			 // vpu register f9
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x4b8c4b40,    // 18388608.0            
			 0x3f028f5c,    // 0.51                  
			 0xbf800000,    // -1.0                  
			 0xffc00000,    // -quiet NaN            
			 0x00000000,    // zero                  
			 0x4b8c4b40,    // 18388608.0            
			 0xbf800000,    // -1.0                  

			 // vpu register f10
			 0x7fc00000,    // quiet NaN             
			 0xcb000000,    // -8388608.0            
			 0x4b000000,    // 8388608.0             
			 0x3f800000,    // 1.0                   
			 0x80011111,    // -9.7958E-41           
			 0xbf028f5c,    // -0.51                 
			 0x80000000,    // -zero                 
			 0x4b000000,    // 8388608.0             

			 // vpu register f11
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0xff800000,    // -inf                  
			 0x3f028f5c,    // 0.51                  
			 0x7f800000,    // inf                   
			 0x00011111,    // 9.7958E-41            
			 0x3f028f5c,    // 0.51                  
			 0x4b8c4b40,    // 18388608.0            
			 0x00011111,    // 9.7958E-41            

			 // vpu register f12
			 0x4b000000,    // 8388608.0             
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x4b000000,    // 8388608.0             
			 0xff800000,    // -inf                  
			 0x3f028f5c,    // 0.51                  
			 0xffc00000,    // -quiet NaN            
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x80011111,    // -9.7958E-41           

			 // vpu register f13
			 0x00000000,    // zero                  
			 0x007fffff,    // 1.1754942E-38         
			 0xffc00000,    // -quiet NaN            
			 0xff800000,    // -inf                  
			 0x00000000,    // zero                  
			 0x3f028f5c,    // 0.51                  
			 0x00000000,    // zero                  
			 0x7fc00001,    // signaling NaN         

			 // vpu register f14
			 0x4b8c4b40,    // 18388608.0            
			 0xff800000,    // -inf                  
			 0x80000000,    // -zero                 
			 0xff800000,    // -inf                  
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x00011111,    // 9.7958E-41            
			 0xbf028f5c,    // -0.51                 
			 0x4b8c4b40,    // 18388608.0            

			 // vpu register f15
			 0x7f800000,    // inf                   
			 0x7f800000,    // inf                   
			 0xbf028f5c,    // -0.51                 
			 0x80000000,    // -zero                 
			 0xbf028f5c,    // -0.51                 
			 0x00000000,    // zero                  
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x00011111,    // 9.7958E-41            

			 // vpu register f16
			 0xffc00001,    // -signaling NaN        
			 0xcb000000,    // -8388608.0            
			 0x80011111,    // -9.7958E-41           
			 0xbf028f5c,    // -0.51                 
			 0x4b8c4b40,    // 18388608.0            
			 0xffc00000,    // -quiet NaN            
			 0x00011111,    // 9.7958E-41            
			 0x4b000000,    // 8388608.0             

			 // vpu register f17
			 0x007fffff,    // 1.1754942E-38         
			 0xff800000,    // -inf                  
			 0xbf800000,    // -1.0                  
			 0xbf800000,    // -1.0                  
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0xcb000000,    // -8388608.0            
			 0x3f028f5c,    // 0.51                  
			 0x00000001,    // 1.4E-45 (+denorm)     

			 // vpu register f18
			 0x3f028f5c,    // 0.51                  
			 0xcb8c4b40,    // -18388608.0           
			 0x80000000,    // -zero                 
			 0xbf800000,    // -1.0                  
			 0x007fffff,    // 1.1754942E-38         
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0xff800000,    // -inf                  
			 0xbf800000,    // -1.0                  

			 // vpu register f19
			 0xffc00000,    // -quiet NaN            
			 0x00011111,    // 9.7958E-41            
			 0xbf800000,    // -1.0                  
			 0x7f800000,    // inf                   
			 0xcb000000,    // -8388608.0            
			 0xbf028f5c,    // -0.51                 
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0xcb000000,    // -8388608.0            

			 // vpu register f20
			 0xff800000,    // -inf                  
			 0x80000000,    // -zero                 
			 0xcb8c4b40,    // -18388608.0           
			 0x80011111,    // -9.7958E-41           
			 0xcb000000,    // -8388608.0            
			 0x80000000,    // -zero                 
			 0x7f800000,    // inf                   
			 0x4b8c4b40,    // 18388608.0            

			 // vpu register f21
			 0xffc00000,    // -quiet NaN            
			 0xcb000000,    // -8388608.0            
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x3f028f5c,    // 0.51                  
			 0x80011111,    // -9.7958E-41           
			 0x7fc00000,    // quiet NaN             
			 0x4b000000,    // 8388608.0             
			 0x00000000,    // zero                  

			 // vpu register f22
			 0x7fc00000,    // quiet NaN             
			 0x00000000,    // zero                  
			 0x007fffff,    // 1.1754942E-38         
			 0x4b000000,    // 8388608.0             
			 0x4b000000,    // 8388608.0             
			 0x3f028f5c,    // 0.51                  
			 0x7fc00001,    // signaling NaN         
			 0x4b8c4b40,    // 18388608.0            

			 // vpu register f23
			 0x4b000000,    // 8388608.0             
			 0x7fc00000,    // quiet NaN             
			 0xbf800000,    // -1.0                  
			 0x007fffff,    // 1.1754942E-38         
			 0x4b8c4b40,    // 18388608.0            
			 0x3f800000,    // 1.0                   
			 0x3f028f5c,    // 0.51                  
			 0xffc00001,    // -signaling NaN        

			 // vpu register f24
			 0xbf800000,    // -1.0                  
			 0x4b000000,    // 8388608.0             
			 0xcb000000,    // -8388608.0            
			 0x3f028f5c,    // 0.51                  
			 0x7f800000,    // inf                   
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x007fffff,    // 1.1754942E-38         
			 0xcb000000,    // -8388608.0            

			 // vpu register f25
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x00011111,    // 9.7958E-41            
			 0x7fc00000,    // quiet NaN             
			 0xffc00001,    // -signaling NaN        
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x7f800000,    // inf                   
			 0x007fffff,    // 1.1754942E-38         
			 0xffc00000,    // -quiet NaN            

			 // vpu register f26
			 0x4b000000,    // 8388608.0             
			 0x7fc00000,    // quiet NaN             
			 0xbf800000,    // -1.0                  
			 0x00011111,    // 9.7958E-41            
			 0x00011111,    // 9.7958E-41            
			 0x7f800000,    // inf                   
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0xffc00000,    // -quiet NaN            

			 // vpu register f27
			 0x00011111,    // 9.7958E-41            
			 0x80000000,    // -zero                 
			 0xbf028f5c,    // -0.51                 
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x00011111,    // 9.7958E-41            
			 0xcb000000,    // -8388608.0            
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0xffc00000,    // -quiet NaN            

			 // vpu register f28
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x3f028f5c,    // 0.51                  
			 0x80011111,    // -9.7958E-41           
			 0xff800000,    // -inf                  
			 0xcb8c4b40,    // -18388608.0           
			 0x00000000,    // zero                  
			 0x4b8c4b40,    // 18388608.0            
			 0x80000001,    // -1.4E-45 (-denorm)    

			 // vpu register f29
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x3f800000,    // 1.0                   
			 0xbf800000,    // -1.0                  
			 0x00000000,    // zero                  
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0x7f800000,    // inf                   
			 0x00000000,    // zero                  

			 // vpu register f30
			 0x4b8c4b40,    // 18388608.0            
			 0x80000001,    // -1.4E-45 (-denorm)    
			 0xffc00001,    // -signaling NaN        
			 0x00000000,    // zero                  
			 0x4b8c4b40,    // 18388608.0            
			 0x00000001,    // 1.4E-45 (+denorm)     
			 0xffc00000,    // -quiet NaN            
			 0x80011111,    // -9.7958E-41           

			 // vpu register f31
			 0xffc00001,    // -signaling NaN        
			 0x7fc00001,    // signaling NaN         
			 0x80011111,    // -9.7958E-41           
			 0xcb000000,    // -8388608.0            
			 0x4b8c4b40,    // 18388608.0            
			 0x80000000,    // -zero                 
			 0xffc00000,    // -quiet NaN            
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
		"fmul.s f20, f12, f15, dyn\n"                         // opcode=0x10f67a53
		"maskor m2, m5, m1\n"                                 // opcode=0x6612e17b
		"fsetm.pi m4, f12\n"                                  // opcode=0xe006527b
		"fsat8.pi f0, f17\n"                                  // opcode=0x0608b07b
		"fnot.pi f27, f12\n"                                  // opcode=0x06062dfb
		"fltm.pi m4, f17, f13\n"                              // opcode=0x3ed8827b
		"maskor m7, m7, m7\n"                                 // opcode=0x6673e3fb
		"for.pi f26, f12, f15\n"                              // opcode=0x06f66d7b
		"feq.ps f0, f18, f18\n"                               // opcode=0xa129207b
		"mov.m.x m3, x11, 166\n"                              // opcode=0x5745e1fb
		"fsub.ps f22, f14, f17, dyn\n"                        // opcode=0x09177b7b
		"fcmov.ps f22, f11, f15, f11\n"                       // opcode=0x5cf5ab3f
		"fsgnj.ps f21, f13, f13\n"                            // opcode=0x20d68afb
		"ffrc.ps f20, f17\n"                                  // opcode=0x58288a7b
		//"faddi.pi f25, f18, 897\n"                          // opcode=0xe4190cbf    ERROR Inmm to large, spliting it into 2 sums
		"faddi.pi f25, f18, 500\n"                            // opcode=0xe4190cbf    ERROR spliting into 2 sums
		"faddi.pi f25, f25, 397\n"                            // opcode=0xe4190cbf    ERROR spliting into 2 sums
		"fsat8.pi f24, f10\n"                                 // opcode=0x06053c7b
		"fsll.pi f26, f12, f16\n"                             // opcode=0x07061d7b
		"fsub.ps f30, f18, f12, dyn\n"                        // opcode=0x08c97f7b
		"fxor.pi f23, f15, f10\n"                             // opcode=0x06a7cbfb
		"maskpopc x26, m4\n"                                  // opcode=0x52020d7b
		"fle.ps f31, f18, f13\n"                              // opcode=0xa0d90ffb
		"fsetm.pi m7, f17\n"                                  // opcode=0xe008d3fb
		"fsat8.pi f22, f18\n"                                 // opcode=0x06093b7b
		"masknot m2, m6\n"                                    // opcode=0x6603217b
		"fbci.ps f28, 221555\n"                               // opcode=0x36173e1f
		"fxor.pi f22, f16, f16\n"                             // opcode=0x07084b7b
		"fmsub.ps f23, f15, f15, f12, dyn\n"                  // opcode=0x62f7fbdb
		"fsrl.pi f22, f19, f14\n"                             // opcode=0x06e9db7b
		"fsrl.pi f29, f13, f16\n"                             // opcode=0x0706defb
		"fsgnjx.s f23, f14, f14\n"                            // opcode=0x20e72bd3
		"maskpopcz x0, m0\n"                                  // opcode=0x5400007b
		"cubefaceidx.ps f28, f17, f10\n"                      // opcode=0x88a89e7b
		"fand.pi f26, f11, f12\n"                             // opcode=0x06c5fd7b
		"maskor m3, m5, m4\n"                                 // opcode=0x6642e1fb
		"fltm.ps m5, f10, f18\n"                              // opcode=0xa12552fb
		"fsub.pi f20, f10, f18\n"                             // opcode=0x0f250a7b
		"fsgnj.s f31, f15, f13\n"                             // opcode=0x20d78fd3
		"fltu.pi f25, f17, f15\n"                             // opcode=0xa6f8bcfb
		"fle.s x25, f14, f15\n"                               // opcode=0xa0f70cd3
		"fsat8.pi f21, f19\n"                                 // opcode=0x0609bafb
		"fbci.pi f22, 682495\n"                               // opcode=0xa69ffb5f
		"fadd.ps f24, f15, f19, dyn\n"                        // opcode=0x0137fc7b
		"fsub.ps f21, f14, f10, dyn\n"                        // opcode=0x08a77afb
		"maskxor m2, m6, m2\n"                                // opcode=0x6623417b
		"fmsub.s f30, f19, f14, f18, dyn\n"                   // opcode=0x90e9ff47
		"fltu.pi f25, f15, f15\n"                             // opcode=0xa6f7bcfb
		"maskxor m6, m0, m2\n"                                // opcode=0x6620437b
		"mova.x.m x27\n"                                      // opcode=0xd6000dfb
		"fmul.s f0, f14, f16, dyn\n"                          // opcode=0x11077053
		"ffrc.ps f27, f16\n"                                  // opcode=0x58280dfb
		"fmsub.s f26, f10, f18, f17, dyn\n"                   // opcode=0x89257d47
		"fsat8.pi f28, f16\n"                                 // opcode=0x06083e7b
		"fsgnjx.s f27, f18, f12\n"                            // opcode=0x20c92dd3
		"fpackrepb.pi f21, f14\n"                             // opcode=0x26070afb
		"fmsub.ps f30, f11, f11, f15, dyn\n"                  // opcode=0x7ab5ff5b
		"mova.x.m x0\n"                                       // opcode=0xd600007b
		"fclass.s x28, f19\n"                                 // opcode=0xe0099e53
		"fmulh.pi f21, f15, f14\n"                            // opcode=0x16e7aafb
		"fsgnjn.s f22, f18, f11\n"                            // opcode=0x20b91b53
		"mov.m.x m2, x10, 43\n"                               // opcode=0x5655317b
		"faddi.pi f20, f17, 170\n"                            // opcode=0x2ca88a3f
		"feqm.ps m4, f13, f19\n"                              // opcode=0xa136e27b
		"for.pi f31, f16, f17\n"                              // opcode=0x07186ffb
		"fadd.pi f26, f15, f16\n"                             // opcode=0x07078d7b
		"fsgnj.s f26, f13, f16\n"                             // opcode=0x21068d53
		"fsll.pi f21, f17, f18\n"                             // opcode=0x07289afb
		"fadd.pi f27, f17, f12\n"                             // opcode=0x06c88dfb
		"fsgnjx.s f20, f18, f19\n"                            // opcode=0x21392a53
		"mov.m.x m7, x16, 23\n"                               // opcode=0x562873fb
		"feq.ps f25, f14, f18\n"                              // opcode=0xa1272cfb
		"feqm.ps m2, f17, f12\n"                              // opcode=0xa0c8e17b
		"ffrc.ps f25, f17\n"                                  // opcode=0x58288cfb
		"fsra.pi f21, f17, f12\n"                             // opcode=0x0ec8dafb
		"cubesgnsc.ps f22, f12, f18\n"                        // opcode=0x89262b7b
		"maskxor m1, m2, m2\n"                                // opcode=0x662140fb
		"fadd.s f21, f18, f14, dyn\n"                         // opcode=0x00e97ad3
		"masknot m7, m6\n"                                    // opcode=0x660323fb
		"fsub.ps f20, f11, f17, dyn\n"                        // opcode=0x0915fa7b
		"fclass.s x23, f15\n"                                 // opcode=0xe0079bd3
		"fmadd.s f28, f14, f13, f14, dyn\n"                   // opcode=0x70d77e43
		"flt.ps f20, f15, f11\n"                              // opcode=0xa0b79a7b
		"flt.s x31, f17, f12\n"                               // opcode=0xa0c89fd3
		"fadd.pi f0, f17, f14\n"                              // opcode=0x06e8807b
		"fsgnj.s f26, f11, f11\n"                             // opcode=0x20b58d53
		"fbci.ps f20, 379853\n"                               // opcode=0x5cbcda1f
		"fbci.pi f20, 809480\n"                               // opcode=0xc5a08a5f
		"fswizz.ps f23, f15, 196\n"                           // opcode=0xe787cbfb
		"fxor.pi f27, f19, f17\n"                             // opcode=0x0719cdfb
		"fsgnjn.s f28, f12, f14\n"                            // opcode=0x20e61e53
		"fmul.ps f30, f15, f18, dyn\n"                        // opcode=0x1127ff7b
		"fround.ps f28, f19, dyn\n"                           // opcode=0x5819fe7b
		"fsll.pi f0, f10, f11\n"                              // opcode=0x06b5107b
		"fmul.ps f29, f13, f19, dyn\n"                        // opcode=0x1136fefb
		"fadd.ps f20, f19, f14, dyn\n"                        // opcode=0x00e9fa7b
		"csrr x5, fflags\n"
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
