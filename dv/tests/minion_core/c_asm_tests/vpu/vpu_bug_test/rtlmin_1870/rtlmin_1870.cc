/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include "cacheops.h"
#include "tensors.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

//Other ESR
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

int main() {

	C_TEST_START;

   *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions

	// Avoid using x2 and x8. x2 is stack pointer. Assembler does not like if we use x8 and forbid it from using x8 - it errors out
	volatile uint32_t m_11[256] __attribute__ ((aligned (4096))) = {
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f800000,                                                  // inf                                        
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[256] __attribute__ ((aligned (4096))) = {
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x33333333,                                                  // 4 random values                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000000,                                                  // -zero                                      
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000000,                                                  // -zero                                      
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[256] __attribute__ ((aligned (4096))) = {
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0xffc00001,                                                  // -signaling NaN                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[256] __attribute__ ((aligned (4096))) = {
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f800000,                                                  // inf                                        
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff800000,                                                  // -inf                                       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[256] __attribute__ ((aligned (4096))) = {
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f800000,                                                  // inf                                        
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f800000,                                                  // inf                                        
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x55555555,                                                  // 4 random values                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000000,                                                  // -zero                                      
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000000,                                                  // zero                                       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_16[256] __attribute__ ((aligned (4096))) = {
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c600000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_17[256] __attribute__ ((aligned (4096))) = {
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0xff800000,                                                  // -inf                                       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000,                                                  // inf                                        
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xff800000,                                                  // -inf                                       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c700000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x55555555,                                                  // 4 random values                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_18[256] __attribute__ ((aligned (4096))) = {
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x3f028f5c,                                                  // 0.51                                       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000,                                                  // inf                                        
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x55555555,                                                  // 4 random values                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_19[256] __attribute__ ((aligned (4096))) = {
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000000,                                                  // -zero                                      
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x33333333,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff800000,                                                  // -inf                                       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80011111,                                                  // -9.7958E-41                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b000000,                                                  // 8388608.0                                  
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
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
        C_TEST_START;
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
		"fadd.ps f24, f15, f10, rne\n"                        // opcode=0x00a7fc7b
		"masknot m2, m7\n"                                    // opcode=0x6603a17b
		"fmulhu.pi f20, f11, f11\n"                           // opcode=0x16b59a7b
		"fmv.s.x f27, x18\n"                                  // opcode=0xf0090dd3
		"fsgnjx.s f25, f14, f13\n"                            // opcode=0x20d72cd3
		"fsgnj.ps f23, f15, f16\n"                            // opcode=0x21078bfb
		"fsw f16, -746(x13)\n"                                // opcode=0xd106ab27
		"fmul.s f26, f16, f11, rne\n"                         // opcode=0x10b87d53
		"fadd.ps f0, f13, f13, rne\n"                         // opcode=0x00d6b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.pi f20, f16, f19\n"                             // opcode=0x07380a7b
		"fmul.ps f28, f17, f15, rne\n"                        // opcode=0x10f8fe7b
		"fcmovm.ps f24, f15, f13\n"                           // opcode=0x00d78c77
		"maskpopcz x21, m7\n"                                 // opcode=0x54038afb
		"fbcx.ps f28, x11\n"                                  // opcode=0x0005be0b
		"flem.ps m4, f18, f15\n"                              // opcode=0xa0f9427b
		"fsgnjx.s f21, f12, f13\n"                            // opcode=0x20d62ad3
		"fxor.pi f30, f12, f11\n"                             // opcode=0x06b64f7b
		"fmaxu.pi f24, f17, f11\n"                            // opcode=0x2eb8bc7b
		"fmin.ps f26, f12, f19\n"                             // opcode=0x29360d7b
		"fclass.ps f21, f14\n"                                // opcode=0xe0071afb
		"fltm.pi m4, f12, f14\n"                              // opcode=0x3ee6027b
		"fand.pi f21, f10, f11\n"                             // opcode=0x06b57afb
		"fbcx.ps f25, x13\n"                                  // opcode=0x0006bc8b
		"fadd.s f28, f12, f18, rne\n"                         // opcode=0x01267e53
		"fnmadd.ps f23, f17, f10, f11, rne\n"                 // opcode=0x5ea8fbdb
		"fmin.pi f21, f18, f17\n"                             // opcode=0x2f190afb
		"fbc.ps f28, 813(x16)\n"                              // opcode=0x32d80e0b
		"cubeface.ps f0, f16, f18\n"                          // opcode=0x8928007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.s f31, f16, f15\n"                            // opcode=0x20f82fd3
		"fandi.pi f30, f13, 6\n"                              // opcode=0x04d69f3f
		"fsgnj.ps f22, f17, f15\n"                            // opcode=0x20f88b7b
		"fmulh.pi f22, f15, f19\n"                            // opcode=0x1737ab7b
		"fnmadd.ps f30, f18, f18, f13, rne\n"                 // opcode=0x6f297f5b
		"fbcx.ps f21, x14\n"                                  // opcode=0x00073a8b
		"fclass.s x0, f12\n"                                  // opcode=0xe0061053
		"fsub.pi f20, f11, f17\n"                             // opcode=0x0f158a7b
		"for.pi f24, f19, f15\n"                              // opcode=0x06f9ec7b
		"fsw f17, 1736(x19)\n"                                // opcode=0x6d19a427
		"cubesgntc.ps f28, f16, f11\n"                        // opcode=0x88b83e7b
		"flw f20, 70(x13)\n"                                 // opcode=0xfba6aa07
		"fsgnj.s f21, f13, f13\n"                             // opcode=0x20d68ad3
		"fle.pi f23, f11, f11\n"                              // opcode=0xa6b58bfb
		"fsub.s f23, f11, f18, rne\n"                         // opcode=0x0925fbd3
		"mov.m.x m3, x17, 107\n"                              // opcode=0x57a8f1fb
		"fsgnjn.ps f31, f15, f18\n"                           // opcode=0x21279ffb
		"fadd.s f28, f13, f14, rne\n"                         // opcode=0x00e6fe53
		"fsub.pi f21, f13, f18\n"                             // opcode=0x0f268afb
		"fsgnjx.s f21, f18, f17\n"                            // opcode=0x21192ad3
		"for.pi f0, f19, f13\n"                               // opcode=0x06d9e07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbcx.ps f25, x11\n"                                  // opcode=0x0005bc8b
		"fmv.s.x f31, x12\n"                                  // opcode=0xf0060fd3
		"fsgnj.s f0, f18, f18\n"                              // opcode=0x21290053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnot.pi f20, f12\n"                                  // opcode=0x06062a7b
		"fsgnjn.s f22, f12, f13\n"                            // opcode=0x20d61b53
		"fmin.ps f21, f18, f11\n"                             // opcode=0x28b90afb
		"fxor.pi f0, f14, f19\n"                              // opcode=0x0737407b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.x.s x20, f11\n"                                  // opcode=0xe0058a53
		"flog.ps f24, f17\n"                                  // opcode=0x58388c7b
		"mov.m.x m4, x15, 32\n"                               // opcode=0x5687827b
		"flt.s x23, f16, f18\n"                               // opcode=0xa1281bd3
		"flem.ps m5, f11, f15\n"                              // opcode=0xa0f5c2fb
		"feq.s x31, f15, f12\n"                               // opcode=0xa0c7afd3
		"maskpopcz x24, m0\n"                                 // opcode=0x54000c7b
		"fmax.pi f21, f11, f10\n"                             // opcode=0x2ea59afb
		"fbc.ps f23, 32(x19)\n"                            // opcode=0xb4198b8b
		"fltm.pi m0, f19, f10\n"                              // opcode=0x3ea9807b
		"flw f27, 87(x11)\n"                                  // opcode=0x0575ad87
		"fmax.pi f28, f12, f16\n"                             // opcode=0x2f061e7b
		"fmul.ps f29, f18, f18, rne\n"                        // opcode=0x11297efb
		"feq.pi f21, f10, f15\n"                              // opcode=0xa6f52afb
		"fadd.pi f23, f17, f17\n"                             // opcode=0x07188bfb
		"fmul.s f20, f19, f12, rne\n"                         // opcode=0x10c9fa53
		"fmadd.ps f30, f12, f13, f15, rne\n"                  // opcode=0x78d67f5b
		"fmvs.x.ps x20, f19, 5\n"                             // opcode=0xe059aa7b
		"feq.s x25, f16, f10\n"                               // opcode=0xa0a82cd3
		"fsub.pi f20, f13, f14\n"                             // opcode=0x0ee68a7b
		"flw f27, 202(x16)\n"                                // opcode=0xf3682d87
		"mov.m.x m1, x13, 49\n"                               // opcode=0x56c6b0fb
		"flq2 f26, 0 (x14)\n"                                 // opcode=0x1b375d07
		"fmax.ps f30, f10, f17\n"                             // opcode=0x29151f7b
		"fpackreph.pi f27, f13\n"                             // opcode=0x26069dfb
		"flw.ps f31, 96(x16)\n"                             // opcode=0x4a882f8b
		"fsetm.pi m2, f19\n"                                  // opcode=0xe009d17b
		"fsat8.pi f31, f18\n"                                 // opcode=0x06093ffb
		"faddi.pi f27, f11, 28\n"                             // opcode=0x0d958dbf
		"fsrli.pi f22, f11, 11\n"                             // opcode=0x4f65db7b
		"feq.ps f22, f15, f17\n"                              // opcode=0xa117ab7b
		"fsrl.pi f22, f11, f12\n"                             // opcode=0x06c5db7b
		"flw.ps f0, 64(x17)\n"                              // opcode=0xd2e8a00b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopc x22, m3\n"                                  // opcode=0x52018b7b
		"fmsub.s f22, f11, f18, f19, rne\n"                   // opcode=0x9925fb47
		"fsetm.pi m0, f11\n"                                  // opcode=0xe005d07b
		"fle.s x24, f17, f12\n"                               // opcode=0xa0c88c53
		"fsll.pi f21, f17, f19\n"                             // opcode=0x07389afb
		"fpackrepb.pi f24, f17\n"                             // opcode=0x26088c7b
		"fltu.pi f24, f12, f19\n"                             // opcode=0xa7363c7b
		"fnmadd.ps f26, f17, f16, f11, rne\n"                 // opcode=0x5f08fd5b
		"fandi.pi f27, f17, 401\n"                            // opcode=0xcc289dbf
		"fmv.s.x f26, x11\n"                                  // opcode=0xf0058d53
		"fsetm.pi m6, f19\n"                                  // opcode=0xe009d37b
		"fltm.ps m1, f18, f12\n"                              // opcode=0xa0c950fb
		"fcmov.ps f29, f13, f12, f14\n"                       // opcode=0x74c6aebf
		"fbci.pi f28, 345216\n"                               // opcode=0xa8901e5f
		"flq2 f28, 0 (x18)\n"                                 // opcode=0x19495e07
		"fandi.pi f23, f18, 203\n"                            // opcode=0x65691bbf
		"flem.ps m5, f10, f13\n"                              // opcode=0xa0d542fb
		"fslli.pi f24, f11, 12\n"                             // opcode=0x4f959c7b
		"fand.pi f24, f16, f11\n"                             // opcode=0x06b87c7b
		"maskpopc x26, m0\n"                                  // opcode=0x52000d7b
		"ffrc.ps f31, f17\n"                                  // opcode=0x58288ffb
		"fadd.s f29, f14, f14, rne\n"                         // opcode=0x00e77ed3
		"fpackreph.pi f29, f17\n"                             // opcode=0x26089efb
		"masknot m4, m1\n"                                    // opcode=0x6600a27b
		"fsq2 f16, 0 (x10)\n"                                 // opcode=0x2d055f27
		"fsw.ps f17, 1695(x14)\n"                             // opcode=0x69176f8b
		"fnmadd.ps f24, f16, f17, f16, rne\n"                 // opcode=0x87187c5b
		"fadd.ps f0, f19, f12, rne\n"                         // opcode=0x00c9f07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.ps f22, f10, f17, f12, rne\n"                 // opcode=0x67157b5b
		//RTLMIN-5479"fsin.ps f22, f13\n"                                  // opcode=0x58668b7b
		"fsgnjn.ps f23, f19, f13\n"                           // opcode=0x20d99bfb
		"fpackrepb.pi f22, f10\n"                             // opcode=0x26050b7b
		"fmvs.x.ps x28, f18, 5\n"                             // opcode=0xe0592e7b
		"fcmov.ps f29, f10, f10, f16\n"                       // opcode=0x84a52ebf
		"fsll.pi f28, f19, f16\n"                             // opcode=0x07099e7b
		"maskpopc.rast x23, m7, m0, 7\n"                      // opcode=0x5f8b8bfb
		"frcp.ps f28, f14\n"                                  // opcode=0x58770e7b
		"fmax.ps f28, f16, f16\n"                             // opcode=0x29081e7b
		"fsrli.pi f28, f12, 12\n"                             // opcode=0x4f965e7b
		"fmv.s.x f23, x16\n"                                  // opcode=0xf0080bd3
		"fmul.ps f21, f14, f10, rne\n"                        // opcode=0x10a77afb
		"fmv.s.x f0, x12\n"                                   // opcode=0xf0060053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnot.pi f0, f16\n"                                   // opcode=0x0608207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flq2 f21, 0 (x12)\n"                                 // opcode=0x9e165a87
		"fsgnjx.ps f26, f18, f16\n"                           // opcode=0x21092d7b
		"fmsub.s f24, f13, f17, f14, rne\n"                   // opcode=0x7116fc47
		"fnmsub.ps f27, f11, f18, f19, rne\n"                 // opcode=0x9d25fddb
		"frcp.ps f24, f13\n"                                  // opcode=0x58768c7b
		"flw f30, 169(x19)\n"                                 // opcode=0x0a99af07
		"fclass.s x24, f17\n"                                 // opcode=0xe0089c53
		"fmsub.ps f28, f19, f17, f19, rne\n"                  // opcode=0x9b19fe5b
		"fcmov.ps f22, f14, f16, f15\n"                       // opcode=0x7d072b3f
		"fmsub.s f0, f17, f13, f11, rne\n"                    // opcode=0x58d8f047
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m4, x15, 94\n"                               // opcode=0x5777d27b
		"fsgnjx.s f20, f17, f10\n"                            // opcode=0x20a8aa53
		"fmsub.s f25, f17, f13, f14, rne\n"                   // opcode=0x70d8fcc7
		"masknot m5, m7\n"                                    // opcode=0x6603a2fb
		"fpackrepb.pi f22, f14\n"                             // opcode=0x26070b7b
		"faddi.pi f21, f11, 366\n"                            // opcode=0xb5d58abf
		"fsgnj.s f29, f11, f12\n"                             // opcode=0x20c58ed3
		"flq2 f20, 0 (x10)\n"                                 // opcode=0x74455a07
		"maskand m7, m0, m2\n"                                // opcode=0x662073fb
		"fle.pi f30, f13, f16\n"                              // opcode=0xa7068f7b
		"fltu.pi f28, f13, f11\n"                             // opcode=0xa6b6be7b
		"fsat8.pi f0, f16\n"                                  // opcode=0x0608307b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.s f30, f12, f11\n"                              // opcode=0x28b61f53
		"fmv.s.x f24, x14\n"                                  // opcode=0xf0070c53
		"fbcx.ps f0, x11\n"                                   // opcode=0x0005b00b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flq2 f0, 0 (x14)\n"                                  // opcode=0x74975007
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m5, f19, f13\n"                              // opcode=0x3ed982fb
		"flt.s x26, f19, f16\n"                               // opcode=0xa1099d53
		"faddi.pi f31, f11, 336\n"                            // opcode=0xac058fbf
		"fmv.s.x f22, x19\n"                                  // opcode=0xf0098b53
		"fmax.pi f25, f15, f10\n"                             // opcode=0x2ea79cfb
		//RTLMIN-5479"frsq.ps f23, f16\n"                                  // opcode=0x58880bfb
		"fsrli.pi f24, f12, 14\n"                             // opcode=0x4fc65c7b
		"maskpopcz x20, m4\n"                                 // opcode=0x54020a7b
		"fmulhu.pi f23, f12, f14\n"                           // opcode=0x16e61bfb
		"fmin.ps f24, f15, f16\n"                             // opcode=0x29078c7b
		"fltm.pi m0, f10, f16\n"                              // opcode=0x3f05007b
		"fsatu8.pi f23, f18\n"                                // opcode=0x06193bfb
		"cubeface.ps f25, f11, f19\n"                         // opcode=0x89358cfb
		"fnmsub.s f27, f16, f10, f14, rne\n"                  // opcode=0x70a87dcb
		"maskxor m5, m4, m3\n"                                // opcode=0x663242fb
		"feq.ps f0, f12, f16\n"                               // opcode=0xa106207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"frcp.ps f21, f16\n"                                  // opcode=0x58780afb
		"fexp.ps f27, f15\n"                                  // opcode=0x58478dfb
		"fsq2 f12, 0 (x11)\n"                                 // opcode=0x2ac5d227
		"fexp.ps f24, f10\n"                                  // opcode=0x58450c7b
		"ffrc.ps f30, f12\n"                                  // opcode=0x58260f7b
		"fsatu8.pi f20, f11\n"                                // opcode=0x0615ba7b
		"faddi.pi f29, f15, 423\n"                            // opcode=0xd4e78ebf
		"fswizz.ps f20, f11, 101\n"                           // opcode=0xe795aa7b
		"ffrc.ps f29, f16\n"                                  // opcode=0x58280efb
		"fsub.s f31, f15, f17, rne\n"                         // opcode=0x0917ffd3
		"fandi.pi f23, f13, 293\n"                            // opcode=0x94b69bbf
		"flem.ps m6, f14, f16\n"                              // opcode=0xa107437b
		"fltm.ps m1, f10, f14\n"                              // opcode=0xa0e550fb
		"fsw f15, -501(x14)\n"                                // opcode=0xe0f725a7
		"frcp.ps f28, f14\n"                                  // opcode=0x58770e7b
		"cubefaceidx.ps f21, f16, f18\n"                      // opcode=0x89281afb
		"flog.ps f25, f15\n"                                  // opcode=0x58378cfb
		"fbc.ps f26, 674(x11)\n"                              // opcode=0x2a258d0b
		"fmvs.x.ps x22, f16, 2\n"                             // opcode=0xe0282b7b
		"maskand m3, m7, m5\n"                                // opcode=0x6653f1fb
		"fmax.s f28, f13, f14\n"                              // opcode=0x28e69e53
		"feq.s x20, f15, f16\n"                               // opcode=0xa107aa53
		"fpackreph.pi f25, f19\n"                             // opcode=0x26099cfb
		"fltm.pi m5, f10, f18\n"                              // opcode=0x3f2502fb
		"flt.pi f29, f14, f16\n"                              // opcode=0xa7071efb
		"fslli.pi f24, f13, 1\n"                              // opcode=0x4e269c7b
		"fsgnj.s f0, f12, f17\n"                              // opcode=0x21160053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fsatu8.pi f29, f12\n"                                // opcode=0x06163efb
		"fslli.pi f30, f13, 6\n"                              // opcode=0x4ec69f7b
		"fcmov.ps f29, f16, f12, f11\n"                       // opcode=0x5cc82ebf
		"fnmadd.ps f29, f17, f11, f19, rne\n"                 // opcode=0x9eb8fedb
		"fmul.pi f27, f15, f16\n"                             // opcode=0x17078dfb
		"flw.ps f0, 32(x15)\n"                             // opcode=0x8b47a00b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.ps f24, f11, f19\n"                           // opcode=0x21359c7b
		"feq.pi f0, f19, f15\n"                               // opcode=0xa6f9a07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.x.s x31, f16\n"                                  // opcode=0xe0080fd3
		"flt.pi f29, f14, f12\n"                              // opcode=0xa6c71efb
		"maskpopcz x24, m4\n"                                 // opcode=0x54020c7b
		"fmul.s f29, f16, f10, rne\n"                         // opcode=0x10a87ed3
		"fxor.pi f25, f13, f12\n"                             // opcode=0x06c6ccfb
		"maskor m2, m3, m2\n"                                 // opcode=0x6621e17b
		"fmaxu.pi f26, f18, f15\n"                            // opcode=0x2ef93d7b
		"maskor m3, m4, m2\n"                                 // opcode=0x662261fb
		"fbci.ps f30, 278379\n"                               // opcode=0x87ed6f1f
		"fsrli.pi f31, f14, 13\n"                             // opcode=0x4fb75ffb
		"cubesgntc.ps f23, f13, f18\n"                        // opcode=0x8926bbfb
		"fmax.ps f28, f15, f19\n"                             // opcode=0x29379e7b
		"feqm.ps m3, f10, f14\n"                              // opcode=0xa0e561fb
		"fmin.s f28, f10, f16\n"                              // opcode=0x29050e53
		"mov.m.x m6, x14, 56\n"                               // opcode=0x56e7137b
		"fsgnjx.s f24, f17, f15\n"                            // opcode=0x20f8ac53
		"fmin.ps f20, f12, f18\n"                             // opcode=0x29260a7b
		"fsetm.pi m1, f14\n"                                  // opcode=0xe00750fb
		"fadd.ps f31, f13, f16, rne\n"                        // opcode=0x0106fffb
		"cubeface.ps f20, f17, f18\n"                         // opcode=0x89288a7b
		"fsatu8.pi f24, f14\n"                                // opcode=0x06173c7b
		"feq.pi f0, f15, f13\n"                               // opcode=0xa6d7a07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.pi f28, f12, f14\n"                             // opcode=0x16e60e7b
		"fle.ps f26, f17, f17\n"                              // opcode=0xa1188d7b
		"flem.ps m4, f15, f15\n"                              // opcode=0xa0f7c27b
		"fnmadd.s f20, f12, f16, f14, rne\n"                  // opcode=0x71067a4f
		"fltu.pi f21, f19, f17\n"                             // opcode=0xa719bafb
		"fltm.ps m5, f15, f10\n"                              // opcode=0xa0a7d2fb
		"fmax.s f31, f16, f14\n"                              // opcode=0x28e81fd3
		"fmvs.x.ps x27, f15, 1\n"                             // opcode=0xe017adfb
		"fmv.x.s x21, f14\n"                                  // opcode=0xe0070ad3
		"fand.pi f29, f14, f16\n"                             // opcode=0x07077efb
		"fpackrepb.pi f22, f14\n"                             // opcode=0x26070b7b
		"fltm.pi m3, f15, f19\n"                              // opcode=0x3f3781fb
		//RTLMIN-5479"frsq.ps f29, f12\n"                                  // opcode=0x58860efb
		"feq.pi f21, f16, f15\n"                              // opcode=0xa6f82afb
		"fsub.pi f25, f17, f14\n"                             // opcode=0x0ee88cfb
		"fmin.ps f23, f17, f14\n"                             // opcode=0x28e88bfb
		"flt.pi f25, f10, f12\n"                              // opcode=0xa6c51cfb
		"fslli.pi f21, f17, 11\n"                             // opcode=0x4f689afb
		"fclass.ps f20, f18\n"                                // opcode=0xe0091a7b
		"fexp.ps f30, f17\n"                                  // opcode=0x58488f7b
		"li x5, 0x198\n"                                      // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fmul.pi f24, f10, f14\n"                             // opcode=0x16e50c7b
		"fsrai.pi f27, f13, 8\n"                              // opcode=0x4f16fdfb
		"fsq2 f14, 0 (x18)\n"                                 // opcode=0xb0e95ba7
		"maskpopc.rast x22, m1, m1, 7\n"                      // opcode=0x5f988b7b
		"fnmadd.s f29, f11, f16, f12, rne\n"                  // opcode=0x6105fecf
		"cubefaceidx.ps f27, f14, f13\n"                      // opcode=0x88d71dfb
		"fpackrepb.pi f20, f13\n"                             // opcode=0x26068a7b
		"fltu.pi f21, f11, f17\n"                             // opcode=0xa715bafb
		"fmv.s.x f21, x16\n"                                  // opcode=0xf0080ad3
		"fmin.pi f31, f13, f19\n"                             // opcode=0x2f368ffb
		"maskor m0, m0, m3\n"                                 // opcode=0x6630607b
		"fmul.ps f24, f11, f16, rne\n"                        // opcode=0x1105fc7b
		"fbc.ps f28, 130(x11)\n"                             // opcode=0xf7e58e0b
		"flog.ps f25, f11\n"                                  // opcode=0x58358cfb
		"mov.m.x m0, x14, 83\n"                               // opcode=0x5747707b
		"fsq2 f16, 0 (x13)\n"                                 // opcode=0x9506dba7
		"fcmov.ps f23, f16, f12, f15\n"                       // opcode=0x7cc82bbf
		"fsgnj.s f0, f10, f12\n"                              // opcode=0x20c50053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubefaceidx.ps f26, f18, f16\n"                      // opcode=0x89091d7b
		"ffrc.ps f26, f13\n"                                  // opcode=0x58268d7b
		"fcmovm.ps f21, f18, f13\n"                           // opcode=0x00d90af7
		"flt.ps f21, f15, f10\n"                              // opcode=0xa0a79afb
		"fadd.pi f21, f13, f11\n"                             // opcode=0x06b68afb
		"fclass.s x28, f18\n"                                 // opcode=0xe0091e53
		"fsetm.pi m2, f19\n"                                  // opcode=0xe009d17b
		"fnmadd.ps f31, f17, f16, f13, rne\n"                 // opcode=0x6f08ffdb
		"fbci.pi f25, 442920\n"                               // opcode=0xd8451cdf
		"fsetm.pi m6, f12\n"                                  // opcode=0xe006537b
		"cubesgntc.ps f23, f18, f13\n"                        // opcode=0x88d93bfb
		"fmul.ps f29, f12, f10, rne\n"                        // opcode=0x10a67efb
		"fsw.ps f15, 1476(x11)\n"                            // opcode=0xa2f5ee0b
		"fclass.s x31, f16\n"                                 // opcode=0xe0081fd3
		"mov.m.x m5, x11, 10\n"                               // opcode=0x5625c2fb
		"fbci.ps f23, 102302\n"                               // opcode=0x31f3db9f
		"fmul.s f21, f16, f19, rne\n"                         // opcode=0x11387ad3
		//RTLMIN-5479"frsq.ps f26, f16\n"                                  // opcode=0x58880d7b
		"fsw f13, 788(x11)\n"                                // opcode=0xced5a627
		"fle.s x28, f19, f18\n"                               // opcode=0xa1298e53
		"flt.ps f29, f15, f10\n"                              // opcode=0xa0a79efb
		"flog.ps f20, f12\n"                                  // opcode=0x58360a7b
		"fmul.pi f26, f18, f18\n"                             // opcode=0x17290d7b
		"fsgnjx.ps f27, f16, f12\n"                           // opcode=0x20c82dfb
		"fmul.pi f29, f12, f16\n"                             // opcode=0x17060efb
		"fsll.pi f20, f11, f11\n"                             // opcode=0x06b59a7b
		"fnmadd.s f26, f16, f10, f12, rne\n"                  // opcode=0x60a87d4f
		"fmax.pi f24, f14, f16\n"                             // opcode=0x2f071c7b
		"maskand m1, m4, m7\n"                                // opcode=0x667270fb
		"fsra.pi f23, f19, f10\n"                             // opcode=0x0ea9dbfb
		"mov.m.x m7, x11, 105\n"                              // opcode=0x57a5a3fb
		"fltm.ps m7, f11, f18\n"                              // opcode=0xa125d3fb
		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fmvz.x.ps x0, f14, 2\n"                              // opcode=0xe027007b
		"fclass.s x26, f16\n"                                 // opcode=0xe0081d53
		"fclass.ps f0, f11\n"                                 // opcode=0xe005907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.pi f28, f16, f18\n"                             // opcode=0x17280e7b
		"flw.ps f24, 32(x19)\n"                             // opcode=0xdc69ac0b
		"fadd.pi f24, f11, f19\n"                             // opcode=0x07358c7b
		"mov.m.x m1, x11, 26\n"                               // opcode=0x5665d0fb
		"fsub.s f27, f19, f18, rne\n"                         // opcode=0x0929fdd3
		"fsub.pi f0, f13, f13\n"                              // opcode=0x0ed6807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.ps f30, f11, f19\n"                           // opcode=0x2135af7b
		"fsgnj.s f29, f12, f17\n"                             // opcode=0x21160ed3
		"maskpopc.rast x26, m4, m2, 3\n"                      // opcode=0x5eaa0d7b
		"faddi.pi f24, f10, 40\n"                             // opcode=0x15050c3f
		"fmul.s f26, f10, f12, rne\n"                         // opcode=0x10c50d53
		"fmulh.pi f27, f19, f19\n"                            // opcode=0x1739adfb
		"fcmov.ps f24, f17, f11, f19\n"                       // opcode=0x9cb8ac3f
		"flt.s x23, f16, f11\n"                               // opcode=0xa0b81bd3
		"fmax.s f28, f11, f13\n"                              // opcode=0x28d59e53
		"fnmadd.s f0, f14, f16, f12, rne\n"                   // opcode=0x6107704f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmsub.s f25, f14, f12, f11, rne\n"                  // opcode=0x58c77ccb
		"fnmadd.ps f27, f13, f16, f14, rne\n"                 // opcode=0x7706bddb
		"fnot.pi f26, f14\n"                                  // opcode=0x06072d7b
		"flt.s x23, f19, f12\n"                               // opcode=0xa0c99bd3
		"fsgnjx.s f26, f10, f18\n"                            // opcode=0x21252d53
		"fmaxu.pi f0, f17, f11\n"                             // opcode=0x2eb8b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmadd.ps f30, f17, f16, f15, rne\n"                  // opcode=0x7908ff5b
		"fsat8.pi f22, f13\n"                                 // opcode=0x0606bb7b
		"fsub.ps f23, f17, f16, rne\n"                        // opcode=0x0908fbfb
		"fnmsub.s f26, f12, f14, f17, rne\n"                  // opcode=0x88e62d4b
		"for.pi f28, f17, f16\n"                              // opcode=0x0708ee7b
		"fmaxu.pi f21, f15, f13\n"                            // opcode=0x2ed7bafb
		"fsgnjx.ps f28, f11, f12\n"                           // opcode=0x20c5ae7b
		"fmv.s.x f30, x18\n"                                  // opcode=0xf0090f53
		"fbci.pi f26, 79747\n"                                // opcode=0x26f06d5f
		"faddi.pi f0, f16, 410\n"                             // opcode=0xcd48003f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.ps f28, f15, f16\n"                              // opcode=0xa107ae7b
		"flq2 f23, 0 (x13)\n"                                 // opcode=0xe5f6db87
		"fmadd.ps f27, f11, f10, f19, rne\n"                  // opcode=0x98a5fddb
		"fround.ps f22, f11, rne\n"                           // opcode=0x5815fb7b
		"maskpopc.rast x20, m7, m6, 2\n"                      // opcode=0x5ee38a7b
		"fsatu8.pi f28, f14\n"                                // opcode=0x06173e7b
		"fbci.pi f26, 307236\n"                               // opcode=0x96049d5f
		"fnmadd.s f0, f17, f16, f10, rne\n"                   // opcode=0x5108f04f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fround.ps f22, f15, rne\n"                           // opcode=0x5817fb7b
		"fminu.pi f20, f13, f11\n"                            // opcode=0x2eb6aa7b
		"cubesgnsc.ps f27, f10, f14\n"                        // opcode=0x88e52dfb
		"frcp.ps f26, f19\n"                                  // opcode=0x58798d7b
		"fsra.pi f23, f16, f18\n"                             // opcode=0x0f285bfb
		"fnot.pi f29, f16\n"                                  // opcode=0x06082efb
		"fsub.ps f26, f14, f13, rne\n"                        // opcode=0x08d77d7b
		"li x5, 0xfb8\n"                                      // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fle.ps f31, f10, f13\n"                              // opcode=0xa0d50ffb
		"feq.pi f29, f16, f12\n"                              // opcode=0xa6c82efb
		"fnmsub.ps f27, f17, f10, f19, rne\n"                 // opcode=0x9ca8fddb
		"flog.ps f29, f19\n"                                  // opcode=0x58398efb
		"fltu.pi f30, f13, f17\n"                             // opcode=0xa716bf7b
		"maskpopcz x28, m0\n"                                 // opcode=0x54000e7b
		"fsgnjx.s f26, f12, f17\n"                            // opcode=0x21162d53
		"fmadd.ps f30, f19, f15, f16, rne\n"                  // opcode=0x80f9ff5b
		"fmax.ps f28, f15, f17\n"                             // opcode=0x29179e7b
		"fxor.pi f25, f14, f13\n"                             // opcode=0x06d74cfb
		"fsub.ps f31, f12, f13, rne\n"                        // opcode=0x08d67ffb
		"fbc.ps f29, 601(x14)\n"                             // opcode=0xda770e8b
		"feq.pi f25, f16, f16\n"                              // opcode=0xa7082cfb
		"fmax.pi f23, f16, f12\n"                             // opcode=0x2ec81bfb
		"cubesgnsc.ps f27, f16, f10\n"                        // opcode=0x88a82dfb
		"fandi.pi f24, f11, 224\n"                            // opcode=0x74059c3f
		"fsub.ps f23, f19, f17, rne\n"                        // opcode=0x0919fbfb
		"fbci.pi f27, 82449\n"                                // opcode=0x28422ddf
		"fmv.s.x f30, x16\n"                                  // opcode=0xf0080f53
		"fmulhu.pi f25, f11, f16\n"                           // opcode=0x17059cfb
		"fcmovm.ps f27, f11, f13\n"                           // opcode=0x00d58df7
		"fmin.ps f28, f11, f11\n"                             // opcode=0x28b58e7b
		//RTLMIN-5479"frsq.ps f29, f13\n"                                  // opcode=0x58868efb
		"fsub.ps f20, f10, f15, rne\n"                        // opcode=0x08f57a7b
		"fsll.pi f0, f16, f19\n"                              // opcode=0x0738107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"for.pi f26, f13, f10\n"                              // opcode=0x06a6ed7b
		"fsat8.pi f0, f17\n"                                  // opcode=0x0608b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.s f22, f16, f13, rne\n"                         // opcode=0x08d87b53
		"fandi.pi f24, f15, 398\n"                            // opcode=0xc5c79c3f
		"fcmov.ps f23, f12, f18, f18\n"                       // opcode=0x95262bbf
		"fnmadd.s f25, f18, f15, f11, rne\n"                  // opcode=0x58f97ccf
		"frcp.ps f28, f18\n"                                  // opcode=0x58790e7b
		"fclass.ps f22, f10\n"                                // opcode=0xe0051b7b
		"fnot.pi f24, f16\n"                                  // opcode=0x06082c7b
		"masknot m7, m1\n"                                    // opcode=0x6600a3fb
		"feq.pi f24, f13, f16\n"                              // opcode=0xa706ac7b
		"frcp.ps f31, f12\n"                                  // opcode=0x58760ffb
		"fsgnjx.s f23, f11, f12\n"                            // opcode=0x20c5abd3
		"fltm.pi m4, f12, f17\n"                              // opcode=0x3f16027b
		"fslli.pi f29, f14, 13\n"                             // opcode=0x4fa71efb
		"flw f29, 83(x14)\n"                                  // opcode=0x05372e87
		"cubeface.ps f30, f10, f11\n"                         // opcode=0x88b50f7b
		"faddi.pi f28, f14, 304\n"                            // opcode=0x9c070e3f
		"fsq2 f10, 0 (x14)\n"                                 // opcode=0x30a75ca7
		"flw.ps f27, 32(x11)\n"                              // opcode=0x18f5ad8b
		"fbci.pi f31, 150377\n"                               // opcode=0x496d3fdf
		"cubeface.ps f21, f18, f10\n"                         // opcode=0x88a90afb
		"fmadd.ps f21, f14, f17, f12, rne\n"                  // opcode=0x61177adb
		"fadd.pi f22, f12, f16\n"                             // opcode=0x07060b7b
		"fmv.s.x f20, x11\n"                                  // opcode=0xf0058a53
		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"cubefaceidx.ps f21, f12, f13\n"                      // opcode=0x88d61afb
		"fsub.s f26, f19, f10, rne\n"                         // opcode=0x08a9fd53
		"mov.m.x m6, x19, 29\n"                               // opcode=0x5679a37b
		"fmadd.s f22, f12, f12, f16, rne\n"                   // opcode=0x80c67b43
		"maskpopc x22, m0\n"                                  // opcode=0x52000b7b
		"feq.ps f22, f11, f17\n"                              // opcode=0xa115ab7b
		"fsrai.pi f23, f17, 0\n"                              // opcode=0x4e18fbfb
		"fsw.ps f17, 1086(x18)\n"                             // opcode=0x43196f0b
		"fmsub.s f25, f11, f18, f15, rne\n"                   // opcode=0x7925bcc7
		"fltm.ps m6, f18, f11\n"                              // opcode=0xa0b9537b
		"faddi.pi f22, f12, 428\n"                            // opcode=0xd5860b3f
		"fmul.pi f31, f11, f17\n"                             // opcode=0x17158ffb
		"fmin.ps f30, f19, f14\n"                             // opcode=0x28e98f7b
		"fmin.pi f28, f11, f15\n"                             // opcode=0x2ef58e7b
		"maskor m6, m1, m2\n"                                 // opcode=0x6620e37b
		"fsgnjn.s f22, f18, f15\n"                            // opcode=0x20f91b53
		"flog.ps f31, f16\n"                                  // opcode=0x58380ffb
		"fle.ps f24, f16, f17\n"                              // opcode=0xa1180c7b
		"fsrli.pi f31, f17, 8\n"                              // opcode=0x4f08dffb
		"fsrai.pi f22, f16, 11\n"                             // opcode=0x4f787b7b
		"frcp.ps f22, f16\n"                                  // opcode=0x58780b7b
		"fmulhu.pi f24, f19, f19\n"                           // opcode=0x17399c7b
		"feq.pi f21, f14, f15\n"                              // opcode=0xa6f72afb
		"fsatu8.pi f0, f17\n"                                 // opcode=0x0618b07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackreph.pi f20, f14\n"                             // opcode=0x26071a7b
		"fbci.pi f25, 510514\n"                               // opcode=0xf9465cdf
		"fsll.pi f22, f13, f11\n"                             // opcode=0x06b69b7b
		"fsw f15, 1393(x16)\n"                               // opcode=0xa8f827a7
		"fmin.ps f21, f10, f17\n"                             // opcode=0x29150afb
		"for.pi f30, f17, f11\n"                              // opcode=0x06b8ef7b
		"fclass.s x20, f15\n"                                 // opcode=0xe0079a53
		"fmadd.s f23, f18, f12, f10, rne\n"                   // opcode=0x50c97bc3
		"fslli.pi f28, f15, 7\n"                              // opcode=0x4ef79e7b
		"fmulhu.pi f31, f12, f16\n"                           // opcode=0x17061ffb
		"fsetm.pi m6, f11\n"                                  // opcode=0xe005d37b
		"fbci.ps f30, 15678\n"                                // opcode=0x07a7df1f
		"fmulhu.pi f27, f11, f12\n"                           // opcode=0x16c59dfb
		"fadd.ps f20, f12, f16, rne\n"                        // opcode=0x01067a7b
		"feqm.ps m6, f14, f16\n"                              // opcode=0xa107637b
		"fclass.ps f29, f15\n"                                // opcode=0xe0079efb
		"fnmadd.s f30, f17, f10, f18, rne\n"                  // opcode=0x90a8ff4f
		"flq2 f20, 0 (x19)\n"                                 // opcode=0x16a9da07
		"fpackreph.pi f28, f18\n"                             // opcode=0x26091e7b
		"fle.s x30, f18, f10\n"                               // opcode=0xa0a90f53
		"fmadd.ps f26, f17, f13, f17, rne\n"                  // opcode=0x88d8fd5b
		"feqm.ps m7, f19, f18\n"                              // opcode=0xa129e3fb
		"for.pi f0, f19, f12\n"                               // opcode=0x06c9e07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopcz x24, m2\n"                                 // opcode=0x54010c7b
		"flq2 f21, 0 (x12)\n"                                 // opcode=0x7ee65a87
		"feqm.ps m3, f14, f10\n"                              // opcode=0xa0a761fb
		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fmvs.x.ps x24, f17, 2\n"                             // opcode=0xe028ac7b
		"fmulhu.pi f25, f10, f14\n"                           // opcode=0x16e51cfb
		"fpackrepb.pi f25, f12\n"                             // opcode=0x26060cfb
		"fmin.s f30, f16, f16\n"                              // opcode=0x29080f53
		"maskand m7, m7, m3\n"                                // opcode=0x6633f3fb
		"fmsub.ps f27, f13, f18, f16, rne\n"                  // opcode=0x8326fddb
		"fsgnjn.s f22, f19, f19\n"                            // opcode=0x21399b53
		"fsatu8.pi f30, f15\n"                                // opcode=0x0617bf7b
		"flog.ps f31, f15\n"                                  // opcode=0x58378ffb
		"fmin.pi f24, f12, f19\n"                             // opcode=0x2f360c7b
		"fclass.s x20, f11\n"                                 // opcode=0xe0059a53
		"flt.pi f0, f11, f14\n"                               // opcode=0xa6e5907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feqm.ps m2, f14, f16\n"                              // opcode=0xa107617b
		"fcmov.ps f21, f17, f18, f17\n"                       // opcode=0x8d28aabf
		"ffrc.ps f31, f14\n"                                  // opcode=0x58270ffb
		"fsgnjn.s f28, f15, f18\n"                            // opcode=0x21279e53
		"fmvz.x.ps x20, f18, 5\n"                             // opcode=0xe0590a7b
		"fswizz.ps f31, f14, 90\n"                            // opcode=0xe7675ffb
		"fnmadd.ps f30, f14, f12, f12, rne\n"                 // opcode=0x66c77f5b
		"fclass.s x22, f16\n"                                 // opcode=0xe0081b53
		"fbci.ps f29, 10394\n"                                // opcode=0x05134e9f
		"fsrai.pi f29, f14, 10\n"                             // opcode=0x4f577efb
		"fsat8.pi f23, f19\n"                                 // opcode=0x0609bbfb
		"faddi.pi f24, f18, 209\n"                            // opcode=0x6c390c3f
		"fsll.pi f28, f14, f13\n"                             // opcode=0x06d71e7b
		"fadd.s f26, f17, f19, rne\n"                         // opcode=0x0138fd53
		"cubefaceidx.ps f23, f10, f16\n"                      // opcode=0x89051bfb
		//RTLMIN-5479"fsin.ps f27, f11\n"                                  // opcode=0x58658dfb
		"fsrl.pi f25, f15, f14\n"                             // opcode=0x06e7dcfb
		"maskpopc x26, m4\n"                                  // opcode=0x52020d7b
		"fsll.pi f23, f12, f11\n"                             // opcode=0x06b61bfb
		"fmvs.x.ps x24, f10, 3\n"                             // opcode=0xe0352c7b
		"fcmov.ps f21, f17, f19, f16\n"                       // opcode=0x8538aabf
		"fmin.pi f31, f15, f14\n"                             // opcode=0x2ee78ffb
		//RTLMIN-5479"fsin.ps f29, f19\n"                                  // opcode=0x58698efb
		"fminu.pi f23, f14, f15\n"                            // opcode=0x2ef72bfb
		"fandi.pi f30, f10, 133\n"                            // opcode=0x44a51f3f
		"fmax.s f21, f17, f14\n"                              // opcode=0x28e89ad3
		"maskxor m7, m0, m3\n"                                // opcode=0x663043fb
		"fadd.s f21, f13, f15, rne\n"                         // opcode=0x00f6fad3
		"maskor m7, m2, m3\n"                                 // opcode=0x663163fb
		"fcmovm.ps f25, f19, f17\n"                           // opcode=0x01198cf7
		"fsw.ps f10, 1593(x17)\n"                            // opcode=0x9ca8e38b
		"fmin.ps f29, f10, f11\n"                             // opcode=0x28b50efb
		//RTLMIN-5479"fsin.ps f28, f15\n"                                  // opcode=0x58678e7b
		"maskpopcz x30, m4\n"                                 // opcode=0x54020f7b
		"fcmov.ps f22, f17, f18, f12\n"                       // opcode=0x6528ab3f
		"fslli.pi f20, f16, 1\n"                              // opcode=0x4e381a7b
		"flt.ps f30, f11, f16\n"                              // opcode=0xa1059f7b
		"fcmovm.ps f20, f17, f19\n"                           // opcode=0x01388a77
		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"feq.pi f24, f11, f13\n"                              // opcode=0xa6d5ac7b
		"fsra.pi f31, f14, f18\n"                             // opcode=0x0f275ffb
		"ffrc.ps f30, f10\n"                                  // opcode=0x58250f7b
		"maskpopcz x21, m7\n"                                 // opcode=0x54038afb
		"fsrai.pi f20, f18, 1\n"                              // opcode=0x4e297a7b
		"fmulhu.pi f29, f13, f17\n"                           // opcode=0x17169efb
		"fmax.pi f22, f11, f10\n"                             // opcode=0x2ea59b7b
		"fmul.s f22, f17, f11, rne\n"                         // opcode=0x10b8fb53
		"cubesgntc.ps f24, f19, f13\n"                        // opcode=0x88d9bc7b
		"fbci.pi f22, 341232\n"                               // opcode=0xa69e0b5f
		"feq.pi f23, f14, f10\n"                              // opcode=0xa6a72bfb
		"fnot.pi f30, f19\n"                                  // opcode=0x0609af7b
		"fsgnjn.ps f29, f14, f17\n"                           // opcode=0x21171efb
		"fnmadd.s f27, f10, f10, f15, rne\n"                  // opcode=0x78a57dcf
		"feqm.ps m1, f16, f11\n"                              // opcode=0xa0b860fb
		"for.pi f21, f12, f12\n"                              // opcode=0x06c66afb
		"fsrl.pi f24, f15, f15\n"                             // opcode=0x06f7dc7b
		"cubeface.ps f30, f12, f17\n"                         // opcode=0x89160f7b
		"cubesgntc.ps f22, f14, f19\n"                        // opcode=0x89373b7b
		"frcp.ps f22, f17\n"                                  // opcode=0x58788b7b
		"fsrli.pi f30, f14, 12\n"                             // opcode=0x4f875f7b
		"fle.ps f0, f19, f18\n"                               // opcode=0xa129807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.s f28, f19, f12, rne\n"                         // opcode=0x00c9fe53
		"flq2 f28, 0 (x15)\n"                                 // opcode=0x5bd7de07
		"fmin.pi f31, f18, f17\n"                             // opcode=0x2f190ffb
		"fbci.pi f28, 516928\n"                               // opcode=0xfc680e5f
		"fmax.pi f28, f17, f17\n"                             // opcode=0x2f189e7b
		"masknot m1, m4\n"                                    // opcode=0x660220fb
		"fltm.ps m0, f12, f11\n"                              // opcode=0xa0b6507b
		"fltm.pi m1, f17, f15\n"                              // opcode=0x3ef880fb
		"fmadd.ps f23, f13, f11, f18, rne\n"                  // opcode=0x90b6fbdb
		"maskpopc.rast x28, m6, m0, 7\n"                      // opcode=0x5f8b0e7b
		"fmax.s f24, f10, f14\n"                              // opcode=0x28e51c53
		"maskpopcz x22, m0\n"                                 // opcode=0x54000b7b
		"fle.ps f27, f10, f19\n"                              // opcode=0xa1350dfb
		"fmsub.s f30, f10, f10, f16, rne\n"                   // opcode=0x80a57f47
		"fsgnjn.ps f28, f10, f12\n"                           // opcode=0x20c51e7b
		"fclass.ps f28, f15\n"                                // opcode=0xe0079e7b
		"fmax.s f28, f17, f14\n"                              // opcode=0x28e89e53
		"fsgnjn.s f29, f14, f15\n"                            // opcode=0x20f71ed3
		"fsatu8.pi f29, f13\n"                                // opcode=0x0616befb
		"fltm.pi m4, f14, f14\n"                              // opcode=0x3ee7027b
		"fsrai.pi f20, f17, 9\n"                              // opcode=0x4f28fa7b
		"fmsub.s f23, f16, f16, f19, rne\n"                   // opcode=0x99087bc7
		"fnmadd.ps f21, f12, f11, f18, rne\n"                 // opcode=0x96b67adb
		"fmsub.ps f26, f15, f11, f19, rne\n"                  // opcode=0x9ab7fd5b
		"fsrai.pi f29, f10, 11\n"                             // opcode=0x4f657efb
		"fadd.s f21, f19, f17, rne\n"                         // opcode=0x01198ad3
		"mov.m.x m0, x13, 95\n"                               // opcode=0x5776f07b
		"fadd.ps f25, f13, f12, rne\n"                        // opcode=0x00c6fcfb
		"li x5, 0xf6e\n"                                      // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"flt.pi f28, f18, f19\n"                              // opcode=0xa7391e7b
		"flt.pi f21, f12, f14\n"                              // opcode=0xa6e61afb
		"fsrl.pi f0, f18, f11\n"                              // opcode=0x06b9507b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskand m5, m6, m0\n"                                // opcode=0x660372fb
		"fnot.pi f20, f12\n"                                  // opcode=0x06062a7b
		"fminu.pi f23, f13, f10\n"                            // opcode=0x2ea6abfb
		"flq2 f0, 0 (x16)\n"                                  // opcode=0x25a85007
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.ps f29, f17, f13\n"                             // opcode=0x28d88efb
		"fmv.x.s x20, f11\n"                                  // opcode=0xe0058a53
		"fsgnjx.s f24, f15, f14\n"                            // opcode=0x20e7ac53
		"fslli.pi f20, f15, 13\n"                             // opcode=0x4fb79a7b
		"fnot.pi f21, f13\n"                                  // opcode=0x0606aafb
		"fsra.pi f24, f13, f11\n"                             // opcode=0x0eb6dc7b
		"fmsub.ps f20, f11, f18, f17, rne\n"                  // opcode=0x8b25fa5b
		"fsub.ps f24, f11, f18, rne\n"                        // opcode=0x0925fc7b
		"fsgnjx.s f29, f19, f16\n"                            // opcode=0x2109aed3
		"fmsub.s f24, f10, f13, f10, rne\n"                   // opcode=0x50d57c47
		"feqm.ps m7, f11, f14\n"                              // opcode=0xa0e5e3fb
		"fmax.pi f27, f17, f17\n"                             // opcode=0x2f189dfb
		//RTLMIN-5479"fsin.ps f27, f10\n"                                  // opcode=0x58650dfb
		"fsatu8.pi f28, f18\n"                                // opcode=0x06193e7b
		"maskpopcz x31, m7\n"                                 // opcode=0x54038ffb
		"feqm.ps m0, f11, f14\n"                              // opcode=0xa0e5e07b
		"fadd.pi f20, f12, f13\n"                             // opcode=0x06d60a7b
		"flq2 f28, 0 (x19)\n"                                 // opcode=0x8b79de07
		"fsra.pi f22, f10, f17\n"                             // opcode=0x0f155b7b
		"maskpopc.rast x30, m6, m1, 2\n"                      // opcode=0x5e970f7b
		"fcmovm.ps f26, f17, f17\n"                           // opcode=0x01188d77
		"for.pi f31, f14, f13\n"                              // opcode=0x06d76ffb
		"maskpopc x20, m7\n"                                  // opcode=0x52038a7b
		"maskpopc.rast x28, m7, m2, 6\n"                      // opcode=0x5fa78e7b
		"fbci.pi f31, 125134\n"                               // opcode=0x3d19dfdf
		"fmaxu.pi f20, f14, f10\n"                            // opcode=0x2ea73a7b
		"fmulh.pi f20, f17, f16\n"                            // opcode=0x1708aa7b
		"fltm.pi m7, f18, f14\n"                              // opcode=0x3ee903fb
		"fmvs.x.ps x29, f11, 1\n"                             // opcode=0xe015aefb
		"fbci.pi f0, 10117\n"                                 // opcode=0x04f0a05f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmsub.s f31, f14, f14, f15, rne\n"                  // opcode=0x78e77fcb
		"feqm.ps m0, f15, f13\n"                              // opcode=0xa0d7e07b
		"fmax.pi f27, f18, f17\n"                             // opcode=0x2f191dfb
		"fmv.s.x f27, x14\n"                                  // opcode=0xf0070dd3
		"fsrli.pi f25, f17, 10\n"                             // opcode=0x4f48dcfb
		"fmulh.pi f26, f17, f11\n"                            // opcode=0x16b8ad7b
		"fadd.s f28, f15, f11, rne\n"                         // opcode=0x00b7fe53
		"fadd.ps f29, f11, f10, rne\n"                        // opcode=0x00a5fefb
		"fnmsub.s f24, f11, f12, f14, rne\n"                  // opcode=0x70c5fc4b
		"frcp.ps f25, f10\n"                                  // opcode=0x58750cfb
		"fmin.pi f26, f13, f14\n"                             // opcode=0x2ee68d7b
		"flq2 f20, 0 (x10)\n"                                 // opcode=0x18355a07
		"flt.s x26, f14, f13\n"                               // opcode=0xa0d71d53
		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fmv.x.s x26, f13\n"                                  // opcode=0xe0068d53
		"fsub.ps f25, f18, f10, rne\n"                        // opcode=0x08a97cfb
		"fround.ps f31, f14, rne\n"                           // opcode=0x58177ffb
		"fsgnjn.ps f25, f16, f12\n"                           // opcode=0x20c81cfb
		"fsat8.pi f25, f17\n"                                 // opcode=0x0608bcfb
		"fmulhu.pi f26, f16, f13\n"                           // opcode=0x16d81d7b
		"fmadd.s f23, f10, f16, f15, rne\n"                   // opcode=0x79053bc3
		"cubeface.ps f0, f10, f18\n"                          // opcode=0x8925007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.s f25, f10, f13, rne\n"                         // opcode=0x10d57cd3
		"maskpopcz x29, m6\n"                                 // opcode=0x54030efb
		"fsrl.pi f26, f12, f10\n"                             // opcode=0x06a65d7b
		"ffrc.ps f29, f19\n"                                  // opcode=0x58298efb
		"fadd.s f23, f12, f11, rne\n"                         // opcode=0x00b67bd3
		"ffrc.ps f31, f10\n"                                  // opcode=0x58250ffb
		"ffrc.ps f27, f14\n"                                  // opcode=0x58270dfb
		"flt.s x20, f17, f18\n"                               // opcode=0xa1289a53
		"cubefaceidx.ps f23, f16, f14\n"                      // opcode=0x88e81bfb
		"fsub.s f30, f14, f17, rne\n"                         // opcode=0x09177f53
		"maskpopc x0, m6\n"                                   // opcode=0x5203007b
		"fle.pi f30, f14, f11\n"                              // opcode=0xa6b70f7b
		"fnmsub.ps f23, f14, f18, f12, rne\n"                 // opcode=0x65277bdb
		"fpackreph.pi f26, f16\n"                             // opcode=0x26081d7b
		"fle.s x28, f10, f11\n"                               // opcode=0xa0b50e53
		"fbci.ps f24, 478080\n"                               // opcode=0xe9700c1f
		"fmsub.ps f31, f16, f17, f14, rne\n"                  // opcode=0x73187fdb
		"fle.pi f22, f14, f17\n"                              // opcode=0xa7170b7b
		"cubesgnsc.ps f0, f10, f12\n"                         // opcode=0x88c5207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackrepb.pi f23, f17\n"                             // opcode=0x26088bfb
		"fsat8.pi f22, f11\n"                                 // opcode=0x0605bb7b
		"fnmsub.s f21, f12, f12, f13, rne\n"                  // opcode=0x68c67acb
		"fsetm.pi m3, f16\n"                                  // opcode=0xe00851fb
		"fnmsub.ps f30, f17, f14, f10, rne\n"                 // opcode=0x54e88f5b
		"cubesgntc.ps f30, f19, f10\n"                        // opcode=0x88a9bf7b
		"fand.pi f26, f13, f19\n"                             // opcode=0x0736fd7b
		"ffrc.ps f0, f15\n"                                   // opcode=0x5827807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcmovm.ps f28, f12, f14\n"                           // opcode=0x00e60e77
		"cubesgntc.ps f23, f15, f16\n"                        // opcode=0x8907bbfb
		"feqm.ps m5, f18, f12\n"                              // opcode=0xa0c962fb
		"maskxor m2, m5, m1\n"                                // opcode=0x6612c17b
		"faddi.pi f0, f19, 385\n"                             // opcode=0xc439803f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrli.pi f0, f18, 12\n"                              // opcode=0x4f89507b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.ps f31, f10, f11\n"                              // opcode=0xa0b50ffb
		"fpackrepb.pi f24, f12\n"                             // opcode=0x26060c7b
		"fsrai.pi f30, f19, 1\n"                              // opcode=0x4e39ff7b
		"fbcx.ps f21, x15\n"                                  // opcode=0x0007ba8b
		"fnmsub.s f30, f15, f19, f14, rne\n"                  // opcode=0x7137ff4b
		"flt.s x26, f11, f10\n"                               // opcode=0xa0a59d53
		"maskand m3, m6, m1\n"                                // opcode=0x661371fb
		"maskpopc.rast x24, m0, m1, 7\n"                      // opcode=0x5f9c0c7b
		"cubesgnsc.ps f28, f13, f13\n"                        // opcode=0x88d6ae7b
		"li x5, 0x615\n"                                      // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fmax.ps f31, f14, f16\n"                             // opcode=0x29071ffb
		"fmsub.ps f30, f17, f19, f13, rne\n"                  // opcode=0x6b38ff5b
		"fsrl.pi f26, f11, f13\n"                             // opcode=0x06d5dd7b
		"fmvz.x.ps x22, f17, 3\n"                             // opcode=0xe0388b7b
		"fsub.pi f23, f17, f14\n"                             // opcode=0x0ee88bfb
		"fadd.pi f22, f18, f17\n"                             // opcode=0x07190b7b
		"fbci.pi f25, 374627\n"                               // opcode=0xb6ec6cdf
		"fsub.s f27, f17, f19, rne\n"                         // opcode=0x0938fdd3
		"fmsub.ps f21, f13, f17, f10, rne\n"                  // opcode=0x5316fadb
		"fbcx.ps f24, x18\n"                                  // opcode=0x00093c0b
		"fsll.pi f20, f10, f17\n"                             // opcode=0x07151a7b
		"fbci.ps f23, 205627\n"                               // opcode=0x64676b9f
		"fmvz.x.ps x22, f14, 7\n"                             // opcode=0xe0770b7b
		"fnot.pi f23, f17\n"                                  // opcode=0x0608abfb
		"fminu.pi f27, f14, f19\n"                            // opcode=0x2f372dfb
		"fsw f14, 1454(x17)\n"                                // opcode=0x5ae8a727
		"fcmovm.ps f31, f16, f16\n"                           // opcode=0x01080ff7
		"ffrc.ps f29, f14\n"                                  // opcode=0x58270efb
		"fadd.ps f0, f12, f10, rne\n"                         // opcode=0x00a6707b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.s f20, f19, f15\n"                            // opcode=0x20f9aa53
		"fnmadd.s f29, f11, f14, f10, rne\n"                  // opcode=0x50e5fecf
		"feq.s x25, f11, f12\n"                               // opcode=0xa0c5acd3
		"masknot m5, m0\n"                                    // opcode=0x660022fb
		"fmin.ps f30, f19, f10\n"                             // opcode=0x28a98f7b
		"faddi.pi f0, f17, 78\n"                              // opcode=0x25c8803f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m1, f12\n"                                  // opcode=0xe00650fb
		"fsw f14, 1968(x17)\n"                               // opcode=0x84e8a827
		"for.pi f23, f18, f17\n"                              // opcode=0x07196bfb
		"maskpopc.rast x26, m2, m7, 7\n"                      // opcode=0x5ffd0d7b
		"fcmovm.ps f30, f11, f19\n"                           // opcode=0x01358f77
		"fle.pi f27, f11, f14\n"                              // opcode=0xa6e58dfb
		"fsra.pi f24, f18, f16\n"                             // opcode=0x0f095c7b
		"maskpopc x29, m7\n"                                  // opcode=0x52038efb
		"flt.ps f28, f12, f10\n"                              // opcode=0xa0a61e7b
		"fsat8.pi f25, f13\n"                                 // opcode=0x0606bcfb
		"fxor.pi f23, f13, f13\n"                             // opcode=0x06d6cbfb
		"fsrai.pi f21, f14, 13\n"                             // opcode=0x4fa77afb
		"maskpopc.rast x22, m5, m6, 7\n"                      // opcode=0x5fea8b7b
		"masknot m4, m1\n"                                    // opcode=0x6600a27b
		"fmax.ps f26, f16, f16\n"                             // opcode=0x29081d7b
		"fnmsub.ps f30, f14, f16, f10, rne\n"                 // opcode=0x55077f5b
		"fclass.s x31, f17\n"                                 // opcode=0xe0089fd3
		"fsw f14, 1706(x16)\n"                               // opcode=0x94e82b27
		"fminu.pi f29, f11, f11\n"                            // opcode=0x2eb5aefb
		"fsat8.pi f28, f16\n"                                 // opcode=0x06083e7b
		"feqm.ps m0, f19, f13\n"                              // opcode=0xa0d9e07b
		"fsgnjx.ps f28, f19, f18\n"                           // opcode=0x2129ae7b
		"maskand m0, m2, m4\n"                                // opcode=0x6641707b
		"fsq2 f14, 0 (x16)\n"                                 // opcode=0xc2e85b27
		"fsgnjn.ps f30, f14, f17\n"                           // opcode=0x21171f7b



		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		//RTLMIN-5479"fsin.ps f31, f14\n"                                  // opcode=0x58670ffb
		"fround.ps f21, f18, rne\n"                           // opcode=0x58197afb
		"flw.ps f26, 96(x10)\n"                            // opcode=0x94c52d0b
		"fnmsub.s f27, f13, f15, f18, rne\n"                  // opcode=0x90f6fdcb
		"fmulh.pi f24, f14, f16\n"                            // opcode=0x17072c7b
		"flt.pi f23, f16, f10\n"                              // opcode=0xa6a81bfb
		"feq.ps f26, f16, f11\n"                              // opcode=0xa0b82d7b
		"fbcx.ps f26, x10\n"                                  // opcode=0x00053d0b
		"fsetm.pi m6, f10\n"                                  // opcode=0xe005537b
		"feqm.ps m1, f12, f15\n"                              // opcode=0xa0f660fb
		"maskand m2, m1, m3\n"                                // opcode=0x6630f17b
		"fmin.pi f22, f17, f14\n"                             // opcode=0x2ee88b7b
		"maskand m6, m0, m0\n"                                // opcode=0x6600737b
		"masknot m6, m7\n"                                    // opcode=0x6603a37b
		"fnmadd.s f20, f16, f14, f12, rne\n"                  // opcode=0x60e87a4f
		"fnot.pi f27, f15\n"                                  // opcode=0x0607adfb
		"flt.ps f20, f16, f16\n"                              // opcode=0xa1081a7b
		"ffrc.ps f21, f11\n"                                  // opcode=0x58258afb
		"for.pi f25, f15, f15\n"                              // opcode=0x06f7ecfb
		"fmsub.s f26, f19, f10, f10, rne\n"                   // opcode=0x50a9cd47
		"fcmovm.ps f0, f16, f18\n"                            // opcode=0x01280077
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.s f25, f12, f18\n"                            // opcode=0x21262cd3
		"masknot m5, m1\n"                                    // opcode=0x6600a2fb
		"maskand m6, m6, m2\n"                                // opcode=0x6623737b
		"fxor.pi f23, f15, f13\n"                             // opcode=0x06d7cbfb
		"fnmadd.ps f21, f16, f18, f16, rne\n"                 // opcode=0x87287adb
		"fmaxu.pi f28, f19, f18\n"                            // opcode=0x2f29be7b
		"fnmsub.s f29, f19, f19, f17, rne\n"                  // opcode=0x8939fecb
		"fmin.ps f27, f10, f17\n"                             // opcode=0x29150dfb
		"fadd.ps f23, f14, f19, rne\n"                        // opcode=0x01377bfb
		"fsub.s f21, f14, f16, rne\n"                         // opcode=0x09077ad3
		"fpackrepb.pi f20, f12\n"                             // opcode=0x26060a7b
		"fandi.pi f30, f13, 165\n"                            // opcode=0x54a69f3f
		"fsub.ps f25, f11, f16, rne\n"                        // opcode=0x0905fcfb
		"fltm.ps m4, f16, f15\n"                              // opcode=0xa0f8527b
		"fmaxu.pi f26, f11, f10\n"                            // opcode=0x2ea5bd7b
		"cubesgnsc.ps f20, f10, f14\n"                        // opcode=0x88e52a7b
		"fsll.pi f28, f12, f10\n"                             // opcode=0x06a61e7b
		"fmvs.x.ps x20, f10, 7\n"                             // opcode=0xe0752a7b
		"feq.pi f25, f19, f14\n"                              // opcode=0xa6e9acfb <-------
		"fsub.ps f0, f14, f14, rne\n"                         // opcode=0x08e7707b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"frsq.ps f28, f17\n"                                  // opcode=0x58888e7b
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
        //"nop\n"
		"fltm.ps m0, f16, f19\n"                              // opcode=0xa138507b
		"fbcx.ps f23, x15\n"                                  // opcode=0x0007bb8b
		"fslli.pi f24, f15, 0\n"                              // opcode=0x4e179c7b
		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fle.pi f26, f18, f12\n"                              // opcode=0xa6c90d7b
		"cubesgntc.ps f29, f17, f16\n"                        // opcode=0x8908befb
		"fsw.ps f13, 1267(x17)\n"                            // opcode=0xb0d8e68b
		"maskpopc.rast x25, m3, m2, 0\n"                      // opcode=0x5e258cfb
		"fadd.s f27, f12, f11, rne\n"                         // opcode=0x00b67dd3
		//RTLMIN-5479"frsq.ps f28, f13\n"                                  // opcode=0x58868dfb
        
        
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
