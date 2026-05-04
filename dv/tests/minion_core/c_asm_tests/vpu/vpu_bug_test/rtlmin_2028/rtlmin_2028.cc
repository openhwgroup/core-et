/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include "cacheops.h"
#include "tensors.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

int main() {

	C_TEST_START;
        *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

	// Avoid using x2 and x8. x2 is stack pointer. Assembler does not like if we use x8 and forbid it from using x8 - it errors out
	volatile uint32_t m_11[256] __attribute__ ((aligned (32))) = {
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f800000,                                                  // inf                                        
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000,                                                  // zero                                       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x3f028f5c,                                                  // 0.51                                       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c700000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[256] __attribute__ ((aligned (32))) = {
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x33333333,                                                  // 4 random values                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[256] __attribute__ ((aligned (32))) = {
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000000,                                                  // -zero                                      
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0xff800000,                                                  // -inf                                       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[256] __attribute__ ((aligned (32))) = {
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000000,                                                  // -zero                                      
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[256] __attribute__ ((aligned (32))) = {
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xcb000000,                                                  // -8388608.0                                 
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x55555555,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000000,                                                  // -zero                                      
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_16[256] __attribute__ ((aligned (32))) = {
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0xff800000,                                                  // -inf                                       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0xffc00001,                                                  // -signaling NaN                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000000,                                                  // zero                                       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x33333333,                                                  // 4 random values                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_17[256] __attribute__ ((aligned (32))) = {
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_18[256] __attribute__ ((aligned (32))) = {
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xff800000,                                                  // -inf                                       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x33333333,                                                  // 4 random values                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_19[256] __attribute__ ((aligned (32))) = {
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000000,                                                  // -zero                                      
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000     // 1.0                   
	};
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
			 0x00000100,                      
			 0x00000200,                      
			 0x00000300,                      
			 0x00000400,                      
			 0x00000500,                      
			 0x00000600,                      
			 0x00000700,                      
			 0x00000800,                      

			 // vpu register f2
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f3
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f4
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f5
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f6
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f7
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f8
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f9
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f10
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f11
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f12
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f13
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f14
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f15
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f16
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f17
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f18
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f19
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f20
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f21
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f22
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f23
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f24
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f25
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f26
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f27
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f28
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f29
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f30
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   

			 // vpu register f31
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			0x3f800000,     // 1.0                   
			 0x3f800000     // 1.0                   
	};

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
		"mv x11, %[m_11]\n"
		"mv x12, %[m_12]\n"
		"mv x13, %[m_13]\n"
		"mv x14, %[m_14]\n"
		"mv x15, %[m_15]\n"
		"mv x16, %[m_16]\n"
		"mv x17, %[m_17]\n"
		"mv x18, %[m_18]\n"
		"mv x19, %[m_19]\n"
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
		"fnmsub.ps f30, f14, f19, f10, dyn\n"                 // opcode=0x55377f5b
		"fmv.x.s x20, f16\n"                                  // opcode=0xe0080a53
		"fsgnjx.s f22, f14, f14\n"                            // opcode=0x20e72b53
		"fmax.pi f0, f18, f19\n"                              // opcode=0x2f39107b
		"fmulhu.pi f26, f12, f18\n"                           // opcode=0x17261d7b
		"fexp.ps f21, f11\n"                                  // opcode=0x58458afb
		"fsll.pi f0, f11, f18\n"                              // opcode=0x0725907b
		"fmvz.x.ps x20, f15, 4\n"                             // opcode=0xe0478a7b
		"fcmovm.ps f23, f14, f15\n"                           // opcode=0x00f70bf7
		"fmv.s.x f25, x13\n"                                  // opcode=0xf0068cd3
		"fnmadd.s f22, f16, f14, f18, dyn\n"                  // opcode=0x90e87b4f
		//RTLMIN-5479"fsin.ps f21, f10\n"                                  // opcode=0x58650afb
		"flt.s x20, f12, f13\n"                               // opcode=0xa0d61a53
		"fltm.pi m2, f12, f13\n"                              // opcode=0x3ed6017b
		"fbci.pi f26, 324499\n"                               // opcode=0x9e726d5f
		"fmul.pi f21, f13, f14\n"                             // opcode=0x16e68afb
		"cubeface.ps f30, f19, f17\n"                         // opcode=0x89198f7b
		"feq.s x30, f17, f15\n"                               // opcode=0xa0f8af53
		"fsgnjn.ps f24, f13, f16\n"                           // opcode=0x21069c7b
		"fsrl.pi f31, f13, f16\n"                             // opcode=0x0706dffb
		"fround.ps f27, f13, dyn\n"                           // opcode=0x5816fdfb
		"fbci.ps f20, 338746\n"                               // opcode=0xa5675a1f
		"fand.pi f20, f13, f10\n"                             // opcode=0x06a6fa7b
		"fmadd.ps f21, f19, f13, f11, dyn\n"                  // opcode=0x58d9fadb
		"fmin.ps f29, f10, f14\n"                             // opcode=0x28e50efb
		"faddi.pi f26, f17, 504\n"                            // opcode=0xfd088d3f
		"fminu.pi f23, f19, f15\n"                            // opcode=0x2ef9abfb
		"cubesgntc.ps f27, f16, f11\n"                        // opcode=0x88b83dfb
		"fmul.ps f22, f11, f10, dyn\n"                        // opcode=0x10a5fb7b
		"fmul.ps f30, f10, f15, dyn\n"                        // opcode=0x10f57f7b
		"flem.ps m3, f15, f19\n"                              // opcode=0xa137c1fb
		"fsgnjx.s f28, f17, f16\n"                            // opcode=0x2108ae53
		//RTLMIN-5479"fsin.ps f0, f14\n"                                   // opcode=0x5867007b
		//RTLMIN-5479"fsin.ps f23, f14\n"                                  // opcode=0x58670bfb
		"cubesgnsc.ps f26, f12, f18\n"                        // opcode=0x89262d7b
		"fmin.pi f27, f10, f14\n"                             // opcode=0x2ee50dfb
		"fmin.s f26, f19, f12\n"                              // opcode=0x28c98d53
		"cubesgntc.ps f20, f11, f13\n"                        // opcode=0x88d5ba7b
		"fmin.pi f30, f14, f11\n"                             // opcode=0x2eb70f7b
		"fxor.pi f20, f17, f10\n"                             // opcode=0x06a8ca7b
		"flw.ps f29, -1793(x16)\n"                            // opcode=0x8ff82e8b
		"flt.pi f21, f16, f16\n"                              // opcode=0xa7081afb
		"cubesgnsc.ps f20, f17, f17\n"                        // opcode=0x8918aa7b
		"fnot.pi f23, f10\n"                                  // opcode=0x06052bfb
		"fxor.pi f29, f13, f12\n"                             // opcode=0x06c6cefb
		"cubesgnsc.ps f24, f11, f14\n"                        // opcode=0x88e5ac7b
		"cubeface.ps f27, f13, f19\n"                         // opcode=0x89368dfb
		"fpackreph.pi f24, f13\n"                             // opcode=0x26069c7b
		"cubeface.ps f21, f10, f11\n"                         // opcode=0x88b50afb
		"fmin.ps f24, f15, f13\n"                             // opcode=0x28d78c7b
		"fsrai.pi f26, f10, 14\n"                             // opcode=0x4fd57d7b
		"cubesgntc.ps f24, f10, f17\n"                        // opcode=0x89153c7b
		"mov.m.x m2, x12, 78\n"                               // opcode=0x5736517b
		"feq.s x24, f19, f10\n"                               // opcode=0xa0a9ac53
		"fsgnjn.ps f20, f12, f13\n"                           // opcode=0x20d61a7b
		"fmv.x.s x20, f18\n"                                  // opcode=0xe0090a53
		"fmsub.s f29, f18, f15, f12, dyn\n"                   // opcode=0x60f97ec7
		"fsub.s f30, f18, f13, dyn\n"                         // opcode=0x08d97f53
		"fsub.pi f29, f18, f11\n"                             // opcode=0x0eb90efb
		"cubeface.ps f0, f12, f17\n"                          // opcode=0x8916007b
		"fadd.ps f26, f15, f16, dyn\n"                        // opcode=0x0107fd7b
		"fsatu8.pi f29, f13\n"                                // opcode=0x0616befb
		"fltu.pi f21, f16, f18\n"                             // opcode=0xa7283afb
		"flq2 f22, 0 (x10)\n"                                 // opcode=0x0fd55b07
		"fmsub.ps f31, f15, f18, f11, dyn\n"                  // opcode=0x5b27ffdb
		"fsrli.pi f26, f19, 8\n"                              // opcode=0x4f19dd7b
		"fsq2 f13, 0 (x11)\n"                                 // opcode=0x36d5db27
		"fpackrepb.pi f23, f17\n"                             // opcode=0x26088bfb
		"flw.ps f24, 707(x12)\n"                              // opcode=0x2c362c0b
		"fsrl.pi f24, f13, f19\n"                             // opcode=0x0736dc7b
		"fswizz.ps f0, f17, 127\n"                            // opcode=0xe7f8f07b
		"fpackrepb.pi f24, f14\n"                             // opcode=0x26070c7b
		"feqm.ps m5, f17, f18\n"                              // opcode=0xa128e2fb
		"for.pi f0, f12, f18\n"                               // opcode=0x0726607b
		"fltm.pi m7, f17, f11\n"                              // opcode=0x3eb883fb
		"fsgnjn.ps f24, f14, f11\n"                           // opcode=0x20b71c7b
		"frcp.ps f24, f18\n"                                  // opcode=0x58790c7b
		"fmadd.s f27, f16, f12, f13, dyn\n"                   // opcode=0x68c87dc3
		"fle.s x26, f13, f18\n"                               // opcode=0xa1268d53
		"fmul.ps f24, f10, f18, dyn\n"                        // opcode=0x11257c7b
		"fpackrepb.pi f30, f16\n"                             // opcode=0x26080f7b
		"fsat8.pi f0, f18\n"                                  // opcode=0x0609307b
		"fadd.s f31, f10, f16, dyn\n"                         // opcode=0x01057fd3
		"fltm.pi m1, f15, f18\n"                              // opcode=0x3f2780fb
		"fltm.ps m2, f19, f12\n"                              // opcode=0xa0c9d17b
		"fpackreph.pi f22, f18\n"                             // opcode=0x26091b7b
		"fand.pi f30, f18, f14\n"                             // opcode=0x06e97f7b
		"fexp.ps f30, f10\n"                                  // opcode=0x58450f7b
		"fbc.ps f23, 117(x14)\n"                              // opcode=0x07570b8b
		"fmsub.ps f23, f12, f15, f13, dyn\n"                  // opcode=0x6af67bdb
		"flt.s x30, f14, f16\n"                               // opcode=0xa1071f53
		"fmv.s.x f27, x15\n"                                  // opcode=0xf0078dd3
		"cubesgntc.ps f0, f15, f14\n"                         // opcode=0x88e7b07b
		"fcmovm.ps f28, f10, f19\n"                           // opcode=0x01350e77
		"fadd.ps f23, f10, f15, dyn\n"                        // opcode=0x00f57bfb
		"fbcx.ps f26, x18\n"                                  // opcode=0x00093d0b
		"fnmsub.s f23, f11, f11, f12, rne\n"                  // opcode=0x60b58bcb
		"fcmovm.ps f24, f17, f11\n"                           // opcode=0x00b88c77
		"fsgnjx.ps f25, f15, f12\n"                           // opcode=0x20c7acfb
		//RTLMIN-5479"fsin.ps f0, f15\n"                                   // opcode=0x5867807b
		"fnmadd.s f27, f13, f12, f19, dyn\n"                  // opcode=0x98c6fdcf
		"flog.ps f22, f17\n"                                  // opcode=0x58388b7b
		"fsub.pi f30, f14, f13\n"                             // opcode=0x0ed70f7b
		"fnmadd.s f21, f19, f19, f17, dyn\n"                  // opcode=0x8939facf
		"fsrl.pi f29, f15, f11\n"                             // opcode=0x06b7defb
		"fbci.pi f27, 466796\n"                               // opcode=0xe3ed8ddf
		"flw f27, 519(x14)\n"                                 // opcode=0x20772d87
		"fadd.ps f22, f10, f15, dyn\n"                        // opcode=0x00f57b7b
		"ffrc.ps f24, f10\n"                                  // opcode=0x58250c7b
		"faddi.pi f23, f16, 372\n"                            // opcode=0xbc880bbf
		"feq.ps f26, f11, f19\n"                              // opcode=0xa135ad7b
		"fmvz.x.ps x28, f13, 2\n"                             // opcode=0xe0268e7b
		"fround.ps f23, f13, dyn\n"                           // opcode=0x5816fbfb
		"feq.ps f22, f18, f16\n"                              // opcode=0xa1092b7b
		"fmin.ps f20, f18, f14\n"                             // opcode=0x28e90a7b
		"fminu.pi f20, f13, f17\n"                            // opcode=0x2f16aa7b
		"fsll.pi f26, f13, f16\n"                             // opcode=0x07069d7b
		"cubeface.ps f31, f17, f15\n"                         // opcode=0x88f88ffb
		"fsub.pi f24, f17, f12\n"                             // opcode=0x0ec88c7b
		"fltm.ps m0, f15, f18\n"                              // opcode=0xa127d07b
		"fandi.pi f27, f15, 455\n"                            // opcode=0xe4e79dbf
		"fsrai.pi f27, f12, 10\n"                             // opcode=0x4f567dfb
		"fmv.s.x f29, x19\n"                                  // opcode=0xf0098ed3
		"fcmovm.ps f28, f10, f12\n"                           // opcode=0x00c50e77
		"cubeface.ps f22, f19, f16\n"                         // opcode=0x89098b7b
		"cubefaceidx.ps f27, f18, f19\n"                      // opcode=0x89391dfb
		"fmul.ps f26, f17, f17, dyn\n"                        // opcode=0x1118fd7b
		"fmin.ps f23, f17, f19\n"                             // opcode=0x29388bfb
		"fsgnj.s f22, f16, f18\n"                             // opcode=0x21280b53
		"fcmovm.ps f28, f17, f13\n"                           // opcode=0x00d88e77
		"cubesgntc.ps f28, f15, f13\n"                        // opcode=0x88d7be7b
		"flw.ps f28, -871(x16)\n"                             // opcode=0xc9982e0b
		"fmsub.s f22, f14, f16, f14, dyn\n"                   // opcode=0x71077b47
		"fadd.pi f20, f12, f16\n"                             // opcode=0x07060a7b
		"fclass.s x21, f11\n"                                 // opcode=0xe0059ad3
		"feqm.ps m2, f10, f19\n"                              // opcode=0xa135617b
		"fltm.pi m1, f11, f11\n"                              // opcode=0x3eb580fb
		"fsra.pi f31, f17, f13\n"                             // opcode=0x0ed8dffb
		"feqm.ps m0, f18, f12\n"                              // opcode=0xa0c9607b
		"fsrai.pi f30, f15, 14\n"                             // opcode=0x4fd7ff7b
		"fmaxu.pi f30, f10, f10\n"                            // opcode=0x2ea53f7b
		"flq2 f20, 0 (x12)\n"                                 // opcode=0x00d65a07
		"flem.ps m6, f14, f14\n"                              // opcode=0xa0e7437b
		"fmulh.pi f27, f17, f11\n"                            // opcode=0x16b8adfb
		"fclass.ps f30, f13\n"                                // opcode=0xe0069f7b
		//RTLMIN-5479"fsin.ps f0, f12\n"                                   // opcode=0x5866007b
		"fmul.ps f22, f15, f11, dyn\n"                        // opcode=0x10b7fb7b
		"fmin.pi f25, f12, f11\n"                             // opcode=0x2eb60cfb
		"for.pi f24, f16, f19\n"                              // opcode=0x07386c7b
		"fsgnjn.ps f25, f12, f18\n"                           // opcode=0x21261cfb
		"fnmsub.s f28, f10, f11, f17, dyn\n"                  // opcode=0x88b57e4b
		"ffrc.ps f20, f13\n"                                  // opcode=0x58268a7b
		"fsgnjn.ps f29, f18, f10\n"                           // opcode=0x20a91efb
		"fsatu8.pi f24, f16\n"                                // opcode=0x06183c7b
		"fmul.pi f29, f12, f18\n"                             // opcode=0x17260efb
		"fnmadd.ps f31, f14, f15, f18, dyn\n"                 // opcode=0x96f77fdb
		"fmaxu.pi f21, f14, f18\n"                            // opcode=0x2f273afb
		"fmax.s f24, f11, f12\n"                              // opcode=0x28c59c53
		"fsq2 f18, 0 (x14)\n"                                 // opcode=0x8d275ca7
		"fsub.s f23, f15, f10, dyn\n"                         // opcode=0x08a7fbd3
		"fle.s x27, f19, f12\n"                               // opcode=0xa0c98dd3
		"fmul.pi f29, f17, f11\n"                             // opcode=0x16b88efb
		"flw f0, 1485(x14)\n"                                 // opcode=0x5cd72007
		"fle.pi f30, f14, f15\n"                              // opcode=0xa6f70f7b
		"cubefaceidx.ps f29, f15, f13\n"                      // opcode=0x88d79efb
		"fexp.ps f29, f12\n"                                  // opcode=0x58460efb
		"fmulh.pi f24, f17, f19\n"                            // opcode=0x1738ac7b
		"fsw f15, -1838(x14)\n"                               // opcode=0x8cf72927
		//RTLMIN-5479"frsq.ps f21, f11\n"                                  // opcode=0x58858afb
		"fsll.pi f27, f13, f14\n"                             // opcode=0x06e69dfb
		"fminu.pi f24, f10, f11\n"                            // opcode=0x2eb52c7b
		"mov.m.x m0, x12, 17\n"                               // opcode=0x5646207b
		"mov.m.x m1, x14, 105\n"                              // opcode=0x57a730fb
		"fmin.ps f25, f12, f16\n"                             // opcode=0x29060cfb
		"fadd.s f22, f17, f14, dyn\n"                         // opcode=0x00e8fb53
		"feq.ps f22, f12, f17\n"                              // opcode=0xa1162b7b
		"fsw f15, 1113(x18)\n"                                // opcode=0x44f92ca7
		"fsub.s f27, f14, f18, dyn\n"                         // opcode=0x09277dd3
		"fmsub.s f30, f17, f12, f10, dyn\n"                   // opcode=0x50c8ff47
		"fmsub.ps f0, f12, f13, f10, dyn\n"                   // opcode=0x52d6705b
		"fnmsub.ps f26, f12, f10, f13, dyn\n"                 // opcode=0x6ca67d5b
		"feq.s x27, f12, f18\n"                               // opcode=0xa1262dd3
		"fand.pi f22, f10, f18\n"                             // opcode=0x07257b7b
		"fsw.ps f19, 1159(x19)\n"                             // opcode=0x4939e38b
		"frcp.ps f20, f19\n"                                  // opcode=0x58798a7b
		"fmvs.x.ps x26, f18, 6\n"                             // opcode=0xe0692d7b
		"cubesgnsc.ps f22, f13, f14\n"                        // opcode=0x88e6ab7b
		"fmsub.s f23, f10, f13, f12, dyn\n"                   // opcode=0x60d57bc7
		"fsub.s f22, f16, f13, dyn\n"                         // opcode=0x08d87b53
		"flq2 f29, 0 (x18)\n"                                 // opcode=0x99095e87
		"flt.s x0, f15, f14\n"                                // opcode=0xa0e79053
		"fxor.pi f28, f19, f17\n"                             // opcode=0x0719ce7b
		"fmulh.pi f20, f10, f13\n"                            // opcode=0x16d52a7b
		"frcp.ps f27, f16\n"                                  // opcode=0x58780dfb
		"fnmsub.ps f29, f14, f17, f19, dyn\n"                 // opcode=0x9d177edb
		"fsrli.pi f27, f17, 1\n"                              // opcode=0x4e38ddfb
		"fmul.ps f21, f19, f16, dyn\n"                        // opcode=0x1109fafb
		"fmv.x.s x24, f18\n"                                  // opcode=0xe0090c53
		"fsat8.pi f31, f15\n"                                 // opcode=0x0607bffb
		"fmv.s.x f22, x15\n"                                  // opcode=0xf0078b53
		"fcmovm.ps f26, f11, f14\n"                           // opcode=0x00e58d77

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsgnjx.ps f0, f14, f10\n"                            // opcode=0x20a7207b
		"fsatu8.pi f20, f14\n"                                // opcode=0x06173a7b
		"flw f0, 1101(x18)\n"                                 // opcode=0x44d92007
		"fbc.ps f25, -1513(x16)\n"                            // opcode=0xa1780c8b
		"fslli.pi f28, f18, 13\n"                             // opcode=0x4fb91e7b
		"fsatu8.pi f24, f15\n"                                // opcode=0x0617bc7b
		"fmv.s.x f24, x18\n"                                  // opcode=0xf0090c53
		"cubesgntc.ps f23, f18, f12\n"                        // opcode=0x88c93bfb
		"flt.pi f24, f12, f18\n"                              // opcode=0xa7261c7b
		"fadd.ps f21, f15, f15, dyn\n"                        // opcode=0x00f7fafb
		"fsgnj.s f26, f13, f18\n"                             // opcode=0x21268d53
		"fmv.x.s x25, f17\n"                                  // opcode=0xe0088cd3
		"fmadd.ps f31, f16, f17, f19, dyn\n"                  // opcode=0x99187fdb
		"fnot.pi f29, f18\n"                                  // opcode=0x06092efb
		"fmvz.x.ps x24, f17, 0\n"                             // opcode=0xe0088c7b
		"fmsub.s f27, f18, f16, f13, dyn\n"                   // opcode=0x69097dc7
		"fmul.pi f27, f15, f19\n"                             // opcode=0x17378dfb
		"fle.pi f25, f14, f11\n"                              // opcode=0xa6b70cfb
		"fsgnjn.ps f30, f18, f15\n"                           // opcode=0x20f91f7b
		"feq.pi f25, f12, f15\n"                              // opcode=0xa6f62cfb
		"fmadd.s f22, f14, f15, f17, dyn\n"                   // opcode=0x88f77b43
		"fnmsub.s f25, f14, f11, f18, dyn\n"                  // opcode=0x90b77ccb
		"cubesgntc.ps f26, f15, f17\n"                        // opcode=0x8917bd7b
		"fsub.pi f30, f16, f16\n"                             // opcode=0x0f080f7b
		"fmsub.s f22, f19, f16, f12, dyn\n"                   // opcode=0x6109fb47
		"flw.ps f29, -1329(x18)\n"                            // opcode=0xacf92e8b
		"fmvs.x.ps x25, f18, 4\n"                             // opcode=0xe0492cfb
		"fadd.pi f29, f16, f11\n"                             // opcode=0x06b80efb
		"fsgnjx.ps f23, f12, f15\n"                           // opcode=0x20f62bfb
		"fsw f10, 839(x18)\n"                                 // opcode=0x34a923a7
		"frcp.ps f29, f17\n"                                  // opcode=0x58788efb
		"flt.s x20, f12, f19\n"                               // opcode=0xa1361a53
		"fltu.pi f26, f19, f11\n"                             // opcode=0xa6b9bd7b
		"fmulh.pi f25, f15, f11\n"                            // opcode=0x16b7acfb
		"faddi.pi f25, f18, 432\n"                            // opcode=0xdc190cbf
		"feqm.ps m3, f14, f10\n"                              // opcode=0xa0a761fb
		"for.pi f23, f17, f15\n"                              // opcode=0x06f8ebfb
		"fsq2 f13, 0 (x10)\n"                                 // opcode=0xbed55e27
		"fsq2 f19, 0 (x16)\n"                                 // opcode=0x8b385fa7
		"feq.ps f21, f16, f10\n"                              // opcode=0xa0a82afb
		"fsrli.pi f20, f19, 3\n"                              // opcode=0x4e79da7b
		"fand.pi f27, f17, f12\n"                             // opcode=0x06c8fdfb
		"fswizz.ps f22, f19, 125\n"                           // opcode=0xe7f9bb7b
		"fclass.ps f29, f10\n"                                // opcode=0xe0051efb
		"fmax.pi f31, f14, f13\n"                             // opcode=0x2ed71ffb
		"fnot.pi f27, f19\n"                                  // opcode=0x0609adfb
		"fmulh.pi f28, f16, f19\n"                            // opcode=0x17382e7b
		"fmin.ps f29, f13, f19\n"                             // opcode=0x29368efb
		"faddi.pi f20, f17, 350\n"                            // opcode=0xadd88a3f
		"fmax.pi f0, f10, f17\n"                              // opcode=0x2f15107b

		"li x5, 0x16dd003b4b48f600\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fnot.pi f20, f19\n"                                  // opcode=0x0609aa7b
		"fpackrepb.pi f31, f11\n"                             // opcode=0x26058ffb
		"fsrli.pi f29, f18, 3\n"                              // opcode=0x4e695efb
		"cubesgntc.ps f0, f17, f15\n"                         // opcode=0x88f8b07b
		"flw f25, -1184(x18)\n"                               // opcode=0xb6092c87
		"fnmadd.s f29, f10, f18, f10, dyn\n"                  // opcode=0x51257ecf
		"fltm.ps m2, f13, f13\n"                              // opcode=0xa0d6d17b
		"fnmsub.s f29, f18, f13, f15, dyn\n"                  // opcode=0x78d97ecb
		"fsub.pi f22, f14, f14\n"                             // opcode=0x0ee70b7b
		"fmul.s f24, f13, f14, dyn\n"                         // opcode=0x10e6fc53
		"fltm.ps m6, f13, f18\n"                              // opcode=0xa126d37b
		"fmv.s.x f26, x12\n"                                  // opcode=0xf0060d53
		"flq2 f0, 0 (x14)\n"                                  // opcode=0xd6175007
		"fmax.s f29, f10, f19\n"                              // opcode=0x29351ed3
		"fpackrepb.pi f21, f15\n"                             // opcode=0x26078afb
		"fsub.pi f22, f10, f11\n"                             // opcode=0x0eb50b7b
		"fnmsub.ps f26, f18, f14, f16, dyn\n"                 // opcode=0x84e97d5b
		"fmax.pi f23, f19, f17\n"                             // opcode=0x2f199bfb
		"cubesgnsc.ps f27, f15, f12\n"                        // opcode=0x88c7adfb
		"fmv.x.s x24, f17\n"                                  // opcode=0xe0088c53
		"fmaxu.pi f20, f19, f11\n"                            // opcode=0x2eb9ba7b
		"fltu.pi f24, f17, f12\n"                             // opcode=0xa6c8bc7b
		"fcmovm.ps f22, f10, f19\n"                           // opcode=0x01350b77
		"cubeface.ps f29, f13, f14\n"                         // opcode=0x88e68efb
		"fexp.ps f31, f16\n"                                  // opcode=0x58480ffb
		"fle.ps f31, f17, f17\n"                              // opcode=0xa1188ffb
		"feq.pi f0, f16, f19\n"                               // opcode=0xa738207b
		"fsatu8.pi f21, f19\n"                                // opcode=0x0619bafb
		"fbci.pi f22, 341686\n"                               // opcode=0xa6d6db5f
		"fltm.pi m0, f11, f14\n"                              // opcode=0x3ee5807b
		"fslli.pi f26, f17, 1\n"                              // opcode=0x4e289d7b
		"flw f24, -1116(x13)\n"                               // opcode=0xba46ac07
		"fsq2 f19, 0 (x17)\n"                                 // opcode=0x8b38da27
		"cubeface.ps f0, f11, f12\n"                          // opcode=0x88c5807b
		"fmv.s.x f30, x19\n"                                  // opcode=0xf0098f53
		"fbcx.ps f25, x15\n"                                  // opcode=0x0007bc8b
		"fmax.s f21, f17, f14\n"                              // opcode=0x28e89ad3
		"fmaxu.pi f26, f11, f13\n"                            // opcode=0x2ed5bd7b
		"fsub.pi f27, f14, f10\n"                             // opcode=0x0ea70dfb
		"flog.ps f22, f11\n"                                  // opcode=0x58358b7b
		"fsub.pi f22, f17, f14\n"                             // opcode=0x0ee88b7b
		"fsrl.pi f0, f12, f10\n"                              // opcode=0x06a6507b
		"fminu.pi f26, f16, f19\n"                            // opcode=0x2f382d7b
		"fmul.s f30, f15, f16, dyn\n"                         // opcode=0x1107ff53
		"fsrl.pi f31, f13, f18\n"                             // opcode=0x0726dffb
		"flem.ps m6, f10, f14\n"                              // opcode=0xa0e5437b
		"flt.pi f0, f14, f10\n"                               // opcode=0xa6a7107b
		"fmul.pi f23, f14, f10\n"                             // opcode=0x16a70bfb
		"mov.m.x m2, x18, 93\n"                               // opcode=0x5779317b

//		"csrrs x0, 0x7D1,1\n"                                 // set txfma sleep 2to7 csr 
//		"fence\n"

		"fxor.pi f27, f11, f16\n"                             // opcode=0x0705cdfb

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsub.pi f21, f18, f16\n"                             // opcode=0x0f090afb
		"flem.ps m6, f19, f13\n"                              // opcode=0xa0d9c37b
		"fsetm.pi m0, f11\n"                                  // opcode=0xe005d07b
        
	// Asm output results to C code
	:
	// Asm input data from C code
	: [operands] "r" (operands), [m_11] "r" (m_11), [m_12] "r" (m_12), [m_13] "r" (m_13), [m_14] "r" (m_14), [m_15] "r" (m_15), [m_16] "r" (m_16), [m_17] "r" (m_17), [m_18] "r" (m_18), [m_19] "r" (m_19)
	// Registers used in Asm so C knows they change
	: "x5", "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "x19", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
	);
	C_TEST_PASS;
	return 0;
}
