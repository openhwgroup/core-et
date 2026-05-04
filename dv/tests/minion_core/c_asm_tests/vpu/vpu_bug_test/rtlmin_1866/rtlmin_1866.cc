/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <cstdint>
#include <stdlib.h>
#include <inttypes.h>
#include "macros.h"

int main() {

	volatile uint32_t operands[256] __attribute__ ((aligned (32))) = {

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
			 0x3f800000,                      
			 0x3f800000,                      
			 0x3f800000,                      
			 0x3f800000,                      
			 0x0c7fff80,                      
			 0x3f800000,                      
			 0x3f800000,                      
			 0x3f800000,                      

			 // vpu register f2
			 0x3f800000, 
			 0x3f800000, 
			 0x3f800000,
			 0x3f800000,
			 0x0e0000ff,
			 0x3f800000,
			 0x3f800000,
			 0x3f800000,

			 // vpu register f3
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          

			 // vpu register f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x0c7fc000,                                                  // Leading 1s:                                

			 // vpu register f5
			 0x33333333,                                                  // 4 random values                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                

			 // vpu register f6
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f7
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            

			 // vpu register f8
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            

			 // vpu register f9
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               

			 // vpu register f11
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            

			 // vpu register f12
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             

			 // vpu register f13
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800003,                                                  // none of the mantissa set to +3ulp          

			 // vpu register f14
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f15
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            

			 // vpu register f16
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x3f028f5c,                                                  // 0.51                                       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               

			 // vpu register f17
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             

			 // vpu register f18
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0c7f0000,                                                  // Leading 1s:                                

			 // vpu register f19
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e3fffff,                                                  // Trailing 1s:                               

			 // vpu register f20
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                

			 // vpu register f21
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          

			 // vpu register f22
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          

			 // vpu register f23
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         

			 // vpu register f24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               

			 // vpu register f25
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                

			 // vpu register f26
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               

			 // vpu register f27
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f28
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x80000000,                                                  // -zero                                      
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f29
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               

			 // vpu register f30
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             

			 // vpu register f31
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
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
		"fence\n"
		"fmul.ps f31, f1, f2, dyn\n"    
		"csrr x5, fflags\n"           
        
	// Asm output results to C code
	:
	// Asm input data from C code
	: [operands] "r" (operands)
	// Registers used in Asm so C knows they change
	: "x5", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
	);
         C_TEST_PASS;
	return 0;
}
