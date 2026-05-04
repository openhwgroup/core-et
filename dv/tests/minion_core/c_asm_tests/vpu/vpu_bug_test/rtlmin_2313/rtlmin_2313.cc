/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include "cacheops.h"
#include "tensors.h"
#include <inttypes.h>
#define GFX_TRAP_ESR_PTR        0x80F40000ULL

volatile uint64_t *gfxTrapEsrPtr        = (uint64_t*)GFX_TRAP_ESR_PTR;  

int main() {

	C_TEST_START;
        *gfxTrapEsrPtr =  (uintptr_t) (0); // Avoid trap for gfx instructions
	// Avoid using x2 and x8. x2 is stack pointer. Assembler does not like if we use x8 and forbid it from using x8 - it errors out
	volatile uint32_t m_11[256] __attribute__ ((aligned (256))) = {
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000000,                                                  // -zero                                      
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f800000,                                                  // inf                                        
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x3f028f5c,                                                  // 0.51                                       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[256] __attribute__ ((aligned (256))) = {
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000000,                                                  // zero                                       
			 0x33333333,                                                  // 4 random values                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x3f028f5c,                                                  // 0.51                                       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000,                                                  // -inf                                       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[256] __attribute__ ((aligned (256))) = {
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x33333333,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000000,                                                  // zero                                       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[256] __attribute__ ((aligned (256))) = {
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x33333333,                                                  // 4 random values                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7f800000,                                                  // inf                                        
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000000,                                                  // -zero                                      
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[256] __attribute__ ((aligned (256))) = {
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7f800000,                                                  // inf                                        
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000000,                                                  // -zero                                      
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff800000,                                                  // -inf                                       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_16[256] __attribute__ ((aligned (256))) = {
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c780000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f800000,                                                  // inf                                        
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_17[256] __attribute__ ((aligned (256))) = {
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000000,                                                  // -zero                                      
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000,                                                  // inf                                        
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff800000,                                                  // -inf                                       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_18[256] __attribute__ ((aligned (256))) = {
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7f800000,                                                  // inf                                        
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000000,                                                  // -zero                                      
			 0xbf028f5c,                                                  // -0.51                                      
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff800000,                                                  // -inf                                       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x33333333,                                                  // 4 random values                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7fc00001,                                                  // signaling NaN                              
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7f800000,                                                  // inf                                        
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_19[256] __attribute__ ((aligned (256))) = {
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000000,                                                  // zero                                       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000000,                                                  // -zero                                      
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
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


		"fsq2 f10, 0 (x18)\n"                                 // opcode=0xcca953a7
		"fsw.ps f13, -1120(x17)\n"                            // opcode=0xbad8e00b
		"fcvt.s.wu f23, x15, rmm\n"                           // opcode=0xd017cbd3
		"fswizz.ps f23, f12, 121\n"                           // opcode=0xe7e63bfb


		"fmul.s f26, f11, f11, dyn\n"                         // opcode=0x10b5fd53
		"fexp.ps f24, f13\n"                                  // opcode=0x58468c7b
		"fadd.s f20, f13, f12, dyn\n"                         // opcode=0x00c6fa53
		"fgb.ps f25, f1(x19)\n"                               // opcode=0x49309c8b




		"cubesgnsc.ps f31, f13, f14\n"                        // opcode=0x88e6affb
		"feq.ps f23, f12, f18\n"                              // opcode=0xa1262bfb
		"flt.s x25, f10, f11\n"                               // opcode=0xa0b51cd3
		"fadd.pi f0, f11, f19\n"                              // opcode=0x0735807b
		"fcvt.sn16.ps f22, f14\n"                             // opcode=0xd9970b7b
		"fsgnjn.ps f30, f15, f11\n"                           // opcode=0x20b79f7b
		"flw.ps f28, 1146(x14)\n"                             // opcode=0x47a72e0b
		"fmadd.s f24, f10, f15, f12, dyn\n"                   // opcode=0x60f57c43
		"fclass.ps f24, f17\n"                                // opcode=0xe0089c7b
		"maskor m2, m1, m6\n"                                 // opcode=0x6660e17b
		"fcvt.s.wu f23, x18, dyn\n"                           // opcode=0xd0197bd3
		"fscb.ps f12, f0(x18)\n"                              // opcode=0xc920160b


		"fand.pi f25, f10, f10\n"                             // opcode=0x06a57cfb
		"fmulhu.pi f28, f19, f19\n"                           // opcode=0x17399e7b
		"fsgnjx.s f31, f17, f12\n"                            // opcode=0x20c8afd3
		"fbc.ps f20, -964(x11)\n"                             // opcode=0xc3c58a0b
		"flt.pi f21, f18, f11\n"                              // opcode=0xa6b91afb
		"fminu.pi f23, f10, f17\n"                            // opcode=0x2f152bfb
		"flt.s x20, f12, f15\n"                               // opcode=0xa0f61a53
		"flt.ps f20, f14, f13\n"                              // opcode=0xa0d71a7b
		"fle.s x21, f10, f13\n"                               // opcode=0xa0d50ad3
		"for.pi f28, f18, f19\n"                              // opcode=0x07396e7b
		"fltm.ps m6, f15, f10\n"                              // opcode=0xa0a7d37b
		"fmaxu.pi f23, f18, f19\n"                            // opcode=0x2f393bfb
		"fnmadd.ps f23, f18, f19, f12, rtz\n"                 // opcode=0x67391bdb
		"fmvs.x.ps x21, f18, 2\n"                             // opcode=0xe0292afb
		"fcvt.f16.ps f27, f12\n"                              // opcode=0xd8960dfb
		"fbcx.ps f22, x15\n"                                  // opcode=0x0007bb0b


		"fnmadd.s f28, f13, f17, f15, dyn\n"                  // opcode=0x7916fe4f
		"fltm.ps m2, f13, f15\n"                              // opcode=0xa0f6d17b
		"flw f26, -421(x13)\n"                                // opcode=0xe5b6ad07
		"fsgnjn.ps f23, f12, f10\n"                           // opcode=0x20a61bfb
		"fcvt.ps.f10 f24, f17\n"                              // opcode=0xd0888c7b
		"feq.pi f24, f18, f12\n"                              // opcode=0xa6c92c7b
		"fcvt.pwu.ps f30, f16, dyn\n"                         // opcode=0xc0187f7b
		"fsgnj.ps f25, f11, f14\n"                            // opcode=0x20e58cfb


		"fmin.pi f28, f18, f10\n"                             // opcode=0x2ea90e7b
		"frcp.ps f21, f10\n"                                  // opcode=0x58750afb
		"mov.m.x m4, x16, 7\n"                                // opcode=0x5618727b
		"flt.pi f29, f13, f11\n"                              // opcode=0xa6b69efb
		"fand.pi f29, f19, f15\n"                             // opcode=0x06f9fefb
		"fnmsub.ps f22, f16, f15, f10, dyn\n"                 // opcode=0x54f87b5b
		"fscw.ps f14, f1(x16)\n"                              // opcode=0xe100970b
		"fcvt.f10.ps f24, f19\n"                              // opcode=0xd8b98c7b
		"fmulhu.pi f22, f12, f16\n"                           // opcode=0x17061b7b
		"cubesgnsc.ps f0, f18, f15\n"                         // opcode=0x88f9207b
		"feq.ps f21, f18, f13\n"                              // opcode=0xa0d92afb
		"frcp.ps f31, f14\n"                                  // opcode=0x58770ffb
		"flw f27, -1322(x14)\n"                               // opcode=0xad672d87
		"mova.m.x x19\n"                                      // opcode=0xd609907b
		"fcvt.w.s x30, f10, dyn\n"                            // opcode=0xc0057f53
		"fsub.pi f23, f11, f11\n"                             // opcode=0x0eb58bfb
		"fsw f12, 890(x17)\n"                                 // opcode=0x36c8ad27


		"fmul.ps f30, f18, f15, dyn\n"                        // opcode=0x10f97f7b
		"fsetm.pi m6, f13\n"                                  // opcode=0xe006d37b
		"fcmovm.ps f22, f19, f15\n"                           // opcode=0x00f98b77
		"fmul.pi f20, f15, f10\n"                             // opcode=0x16a78a7b


		"fround.ps f24, f11, dyn\n"                           // opcode=0x5815fc7b
		"maskpopcz x30, m5\n"                                 // opcode=0x54028f7b
		"fround.ps f26, f11, dyn\n"                           // opcode=0x5815fd7b
		"fmin.s f22, f19, f10\n"                              // opcode=0x28a98b53
		"fmvz.x.ps x26, f17, 5\n"                             // opcode=0xe0588d7b
		"fsgnjx.ps f24, f16, f11\n"                           // opcode=0x20b82c7b


		"fcvt.s.w f25, x17, dyn\n"                            // opcode=0xd008fcd3
		"maskand m4, m6, m7\n"                                // opcode=0x6673727b
		"feq.s x21, f11, f10\n"                               // opcode=0xa0a5aad3
		"fnmsub.s f28, f17, f15, f10, dyn\n"                  // opcode=0x50f8fe4b
		"fsw.ps f18, -251(x17)\n"                             // opcode=0xf128e28b
		"fcvt.wu.s x0, f15, dyn\n"                            // opcode=0xc017f053
		//RTLMIN-5479"frsq.ps f31, f14\n"                                  // opcode=0x58870ffb
		"fmsub.ps f20, f18, f17, f15, dyn\n"                  // opcode=0x7b197a5b
		"flw.ps f29, -408(x13)\n"                             // opcode=0xe686ae8b
		"fsll.pi f0, f14, f18\n"                              // opcode=0x0727107b
		"fcvt.un24.ps f22, f14\n"                             // opcode=0xd9070b7b
		"fsw f16, 354(x14)\n"                                 // opcode=0x17072127
		"fbci.pi f25, 383350\n"                               // opcode=0xbb2edcdf
		"fgh.ps f20, f0(x17)\n"                               // opcode=0x51101a0b
		"mov.m.x m1, x13, 96\n"                               // opcode=0x578680fb
		"fsgnj.s f22, f16, f11\n"                             // opcode=0x20b80b53
		"fnmadd.s f0, f12, f19, f13, dyn\n"                   // opcode=0x6936704f


		"flq2 f26, 0 (x14)\n"                                 // opcode=0x67975d07
		"maskpopc x29, m7\n"                                  // opcode=0x52038efb
		"fsub.ps f29, f13, f17, dyn\n"                        // opcode=0x0916fefb


		"fscb.ps f10, f1(x10)\n"                              // opcode=0xc8a0950b
		"fsch.ps f14, f0(x11)\n"                              // opcode=0xd0b0170b
		"fmadd.ps f30, f14, f18, f19, dyn\n"                  // opcode=0x99277f5b
		"fsatu8.pi f20, f16\n"                                // opcode=0x06183a7b
		"fcvt.f10.ps f26, f15\n"                              // opcode=0xd8b78d7b
		"fltm.pi m0, f19, f10\n"                              // opcode=0x3ea9807b
		"fle.pi f26, f16, f16\n"                              // opcode=0xa7080d7b
		"fbcx.ps f29, x14\n"                                  // opcode=0x00073e8b
		"fsch.ps f11, f1(x17)\n"                              // opcode=0xd110958b

		 
		"fence\n"

		"fsgnj.s f24, f15, f16\n"                             // opcode=0x21078c53
		"fsgnjn.ps f25, f16, f14\n"                           // opcode=0x20e81cfb


		"fbc.ps f26, 1561(x10)\n"                             // opcode=0x61950d0b
		"maskpopcz x26, m0\n"                                 // opcode=0x54000d7b
		"fsgnjx.ps f29, f13, f13\n"                           // opcode=0x20d6aefb
		"fgb.ps f24, f0(x12)\n"                               // opcode=0x48c01c0b
		"fbci.pi f30, 258265\n"                               // opcode=0x7e1b3f5f
		"fbcx.ps f20, x13\n"                                  // opcode=0x0006ba0b
		"fle.ps f22, f17, f13\n"                              // opcode=0xa0d88b7b
		"fmax.ps f31, f14, f10\n"                             // opcode=0x28a71ffb
		"fgb.ps f20, f1(x10)\n"                               // opcode=0x48a09a0b
		"fbc.ps f26, -1229(x12)\n"                            // opcode=0xb3360d0b
		"mova.x.m x23\n"                                      // opcode=0xd6000bfb


		"fsra.pi f20, f18, f10\n"                             // opcode=0x0ea95a7b
		"fsw.ps f13, -1776(x18)\n"                            // opcode=0x90d9680b
		"feq.s x0, f10, f12\n"                                // opcode=0xa0c52053
		"fsch.ps f18, f1(x11)\n"                              // opcode=0xd0b0990b
		"fmadd.ps f25, f18, f17, f12, dyn\n"                  // opcode=0x61197cdb
		"fsgnjx.s f26, f15, f12\n"                            // opcode=0x20c7ad53


		"fsgnj.s f31, f10, f12\n"                             // opcode=0x20c50fd3
		"fsrai.pi f21, f19, 0\n"                              // opcode=0x4e19fafb
		"fsrli.pi f22, f16, 12\n"                             // opcode=0x4f985b7b
		//RTLMIN-5479"frsq.ps f25, f16\n"                                  // opcode=0x58880cfb
		"fnmsub.ps f22, f14, f17, f10, dyn\n"                 // opcode=0x55177b5b
		"ffrc.ps f27, f19\n"                                  // opcode=0x58298dfb
		"fandi.pi f27, f12, 113\n"                            // opcode=0x3c261dbf
		"fmul.pi f28, f19, f15\n"                             // opcode=0x16f98e7b
		"fandi.pi f27, f16, 331\n"                            // opcode=0xa5681dbf
		"fcvt.un10.ps f31, f18\n"                             // opcode=0xd9290ffb
		"maskor m5, m7, m2\n"                                 // opcode=0x6623e2fb


		"fcvt.ps.pwu f25, f10, rtz\n"                         // opcode=0xd0151cfb
		"fsgnjn.ps f24, f19, f12\n"                           // opcode=0x20c99c7b
		"fcvt.un2.ps f26, f17\n"                              // opcode=0xd9788d7b
		"fcvt.ps.f16 f24, f18\n"                              // opcode=0xd0a90c7b
		"fmul.pi f30, f17, f19\n"                             // opcode=0x17388f7b


		"feq.ps f30, f10, f12\n"                              // opcode=0xa0c52f7b
		"fmulh.pi f24, f10, f15\n"                            // opcode=0x16f52c7b
		"fpackrepb.pi f22, f18\n"                             // opcode=0x26090b7b


		"fmul.ps f20, f17, f13, rup\n"                        // opcode=0x10d8ba7b


		"fle.ps f22, f17, f18\n"                              // opcode=0xa1288b7b
		"fcvt.sn16.ps f24, f13\n"                             // opcode=0xd9968c7b
		"feq.s x24, f12, f14\n"                               // opcode=0xa0e62c53
		"fsgnjn.ps f27, f12, f19\n"                           // opcode=0x21361dfb


		"fand.pi f23, f15, f13\n"                             // opcode=0x06d7fbfb


		"fcvt.s.w f26, x11, dyn\n"                            // opcode=0xd005fd53
		"maskand m5, m7, m0\n"                                // opcode=0x6603f2fb
		"fsra.pi f26, f16, f10\n"                             // opcode=0x0ea85d7b
		"fpackrepb.pi f24, f14\n"                             // opcode=0x26070c7b


		"cubesgnsc.ps f30, f17, f15\n"                        // opcode=0x88f8af7b
		"fle.s x0, f15, f13\n"                                // opcode=0xa0d78053
		"fslli.pi f23, f14, 12\n"                             // opcode=0x4f871bfb
		"fandi.pi f22, f10, 258\n"                            // opcode=0x84551b3f
		"fcvt.wu.s x0, f12, dyn\n"                            // opcode=0xc0167053
		"fsgnj.s f23, f17, f18\n"                             // opcode=0x21288bd3
		"flt.s x25, f12, f17\n"                               // opcode=0xa1161cd3

		"fence\n"

		"fcmovm.ps f27, f16, f14\n"                           // opcode=0x00e80df7
		"fmax.s f29, f12, f14\n"                              // opcode=0x28e61ed3
		"fcvt.ps.f10 f27, f18\n"                              // opcode=0xd0890dfb
		"fmul.pi f24, f18, f18\n"                             // opcode=0x17290c7b
		"fmsub.s f25, f15, f13, f10, dyn\n"                   // opcode=0x50d7fcc7


		"fcvt.f16.ps f27, f16\n"                              // opcode=0xd8980dfb
		"mova.x.m x22\n"                                      // opcode=0xd6000b7b
		"fbci.pi f29, 523807\n"                               // opcode=0xffc3eedf
		"feq.s x23, f12, f15\n"                               // opcode=0xa0f62bd3
		"fsub.pi f27, f12, f14\n"                             // opcode=0x0ee60dfb
		"fmul.s f23, f15, f15, dyn\n"                         // opcode=0x10f7fbd3
		"fsub.s f22, f15, f13, dyn\n"                         // opcode=0x08d7fb53
		"fcvt.sn8.ps f26, f11\n"                              // opcode=0xd9b58d7b
		"mova.m.x x15\n"                                      // opcode=0xd607907b
		"fle.pi f24, f10, f10\n"                              // opcode=0xa6a50c7b
		"fsgnj.ps f31, f18, f19\n"                            // opcode=0x21390ffb
		"fsch.ps f19, f1(x14)\n"                              // opcode=0xd0e0998b
		"fnmadd.s f22, f16, f12, f14, dyn\n"                  // opcode=0x70c87b4f
		"flw.ps f0, 310(x17)\n"                               // opcode=0x1368a00b
		"fmul.s f30, f11, f17, dyn\n"                         // opcode=0x1115ff53
		"faddi.pi f30, f15, 219\n"                            // opcode=0x6d678f3f
		"fmin.s f0, f16, f10\n"                               // opcode=0x28a80053
		"fsgnjn.s f26, f12, f17\n"                            // opcode=0x21161d53
		"fsat8.pi f25, f19\n"                                 // opcode=0x0609bcfb
		"fcvt.un2.ps f29, f16\n"                              // opcode=0xd9780efb
		"fsrai.pi f30, f18, 13\n"                             // opcode=0x4fb97f7b
		"fsatu8.pi f0, f14\n"                                 // opcode=0x0617307b
		"fmin.pi f23, f13, f16\n"                             // opcode=0x2f068bfb
		"fscb.ps f18, f0(x14)\n"                              // opcode=0xc8e0190b
		"for.pi f25, f12, f17\n"                              // opcode=0x07166cfb
		"fmv.x.s x26, f16\n"                                  // opcode=0xe0080d53
		"fsetm.pi m7, f11\n"                                  // opcode=0xe005d3fb
		"fcvt.ps.f10 f30, f14\n"                              // opcode=0xd0870f7b
		"fmul.s f26, f19, f12, dyn\n"                         // opcode=0x10c9fd53
		"fsgnjn.ps f29, f12, f14\n"                           // opcode=0x20e61efb
		"fmaxu.pi f22, f15, f17\n"                            // opcode=0x2f17bb7b
		"fpackrepb.pi f29, f13\n"                             // opcode=0x26068efb
		"fltu.pi f26, f16, f15\n"                             // opcode=0xa6f83d7b
		"fclass.s x28, f16\n"                                 // opcode=0xe0081e53


		"mova.x.m x24\n"                                      // opcode=0xd6000c7b
		"cubesgntc.ps f23, f17, f19\n"                        // opcode=0x8938bbfb
		"fmin.pi f22, f13, f14\n"                             // opcode=0x2ee68b7b
		"fgw.ps f21, f0(x19)\n"                               // opcode=0x61301a8b
		"fmax.pi f0, f19, f17\n"                              // opcode=0x2f19907b
		"mova.m.x x14\n"                                      // opcode=0xd607107b

		 
		"fence\n"

		"fcvt.sn16.ps f0, f17\n"                              // opcode=0xd998807b

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fle.pi f24, f11, f17\n"                              // opcode=0xa7158c7b
		"fcvt.un16.ps f26, f15\n"                             // opcode=0xd9178d7b
		"fpackrepb.pi f0, f12\n"                              // opcode=0x2606007b


		"fexp.ps f28, f10\n"                                  // opcode=0x58450e7b
		"fsch.ps f17, f0(x13)\n"                              // opcode=0xd0d0188b
		"fbci.ps f31, 102636\n"                               // opcode=0x321d9f9f
		"fgw.ps f26, f1(x14)\n"                               // opcode=0x60e09d0b


		"fmax.pi f27, f19, f17\n"                             // opcode=0x2f199dfb
		"fcmovm.ps f23, f15, f12\n"                           // opcode=0x00c78bf7
		"fsgnjn.ps f30, f19, f11\n"                           // opcode=0x20b99f7b
		"fltm.pi m4, f15, f19\n"                              // opcode=0x3f37827b
		"fcvt.f11.ps f22, f11\n"                              // opcode=0xd8858b7b
		"cubesgntc.ps f25, f15, f11\n"                        // opcode=0x88b7bcfb


		"fmul.ps f21, f19, f14, dyn\n"                        // opcode=0x10e9fafb
		"fsrl.pi f25, f11, f10\n"                             // opcode=0x06a5dcfb
		"fcvt.wu.s x0, f12, dyn\n"                            // opcode=0xc0167053
		"fbcx.ps f29, x12\n"                                  // opcode=0x00063e8b


		"mov.m.x m7, x12, 11\n"                               // opcode=0x562673fb
		"fmv.x.s x23, f15\n"                                  // opcode=0xe0078bd3
		"fnmadd.s f26, f19, f16, f17, dyn\n"                  // opcode=0x8909fd4f
		"fcvt.w.s x21, f13, dyn\n"                            // opcode=0xc006fad3
		"fcvt.ps.pw f27, f15, dyn\n"                          // opcode=0xd007fdfb
		"fsrli.pi f26, f14, 6\n"                              // opcode=0x4ed75d7b
		"fsw f10, -1519(x10)\n"                               // opcode=0xa0a528a7
		"fsgnjx.s f28, f16, f15\n"                            // opcode=0x20f82e53
		"fmulhu.pi f0, f19, f13\n"                            // opcode=0x16d9907b
		"fround.ps f21, f15, dyn\n"                           // opcode=0x5817fafb
		"fadd.ps f20, f17, f10, dyn\n"                        // opcode=0x00a8fa7b
		"fsetm.pi m1, f18\n"                                  // opcode=0xe00950fb
		"feqm.ps m0, f13, f13\n"                              // opcode=0xa0d6e07b
		"fpackrepb.pi f24, f15\n"                             // opcode=0x26078c7b
		"fmsub.s f25, f17, f14, f12, dyn\n"                   // opcode=0x60e8fcc7
		"fcvt.un10.ps f23, f13\n"                             // opcode=0xd9268bfb
		"mova.x.m x22\n"                                      // opcode=0xd6000b7b
		"fle.s x25, f16, f17\n"                               // opcode=0xa1180cd3
		"fcvt.ps.pw f25, f13, dyn\n"                          // opcode=0xd006fcfb
		"maskor m6, m5, m4\n"                                 // opcode=0x6642e37b
		"fle.pi f24, f18, f19\n"                              // opcode=0xa7390c7b
		"fmaxu.pi f23, f16, f11\n"                            // opcode=0x2eb83bfb
		"cubesgntc.ps f0, f14, f18\n"                         // opcode=0x8927307b
		"fsra.pi f24, f17, f15\n"                             // opcode=0x0ef8dc7b
		"fnmadd.ps f0, f17, f10, f10, dyn\n"                  // opcode=0x56a8f05b
		"fcvt.ps.f11 f23, f14\n"                              // opcode=0xd0970bfb
		"fsatu8.pi f30, f17\n"                                // opcode=0x0618bf7b
		"fscb.ps f13, f0(x14)\n"                              // opcode=0xc8e0168b
		"fsch.ps f13, f1(x17)\n"                              // opcode=0xd110968b
		"ffrc.ps f26, f13\n"                                  // opcode=0x58268d7b
		"flt.s x23, f19, f11\n"                               // opcode=0xa0b99bd3
		"fcvt.un16.ps f23, f14\n"                             // opcode=0xd9170bfb
		"fpackreph.pi f25, f17\n"                             // opcode=0x26089cfb

		"li x5, 0x2cf7cdc6f8066dc\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"maskand m5, m7, m4\n"                                // opcode=0x6643f2fb
		"ffrc.ps f23, f19\n"                                  // opcode=0x58298bfb
		"fcvt.sn8.ps f23, f13\n"                              // opcode=0xd9b68bfb
		"fexp.ps f25, f19\n"                                  // opcode=0x58498cfb
		"fexp.ps f22, f19\n"                                  // opcode=0x58498b7b


		"fmsub.ps f31, f19, f10, f15, dyn\n"                  // opcode=0x7aa9ffdb
		"fcvt.sn16.ps f23, f18\n"                             // opcode=0xd9990bfb
		"fcvt.f11.ps f25, f11\n"                              // opcode=0xd8858cfb
		"fsw f15, -1668(x12)\n"                               // opcode=0x96f62e27
		"flem.ps m6, f13, f10\n"                              // opcode=0xa0a6c37b
		"fmulhu.pi f21, f14, f14\n"                           // opcode=0x16e71afb
		"feq.pi f29, f18, f14\n"                              // opcode=0xa6e92efb


		"fcvt.f10.ps f25, f19\n"                              // opcode=0xd8b98cfb


		"fadd.pi f26, f10, f18\n"                             // opcode=0x07250d7b
		"fcvt.ps.pw f21, f11, dyn\n"                          // opcode=0xd005fafb
		"fmadd.ps f0, f19, f10, f11, dyn\n"                   // opcode=0x58a9f05b
		"fadd.pi f25, f15, f10\n"                             // opcode=0x06a78cfb
		"fmul.pi f30, f13, f16\n"                             // opcode=0x17068f7b
		"fcvt.s.w f27, x16, dyn\n"                            // opcode=0xd0087dd3
		"fsw f17, 575(x14)\n"                                 // opcode=0x23172fa7
		"fgb.ps f21, f1(x13)\n"                               // opcode=0x48d09a8b
		"fsw.ps f15, -1476(x12)\n"                            // opcode=0xa2f66e0b
		"fmax.s f21, f16, f10\n"                              // opcode=0x28a81ad3
		"fmulhu.pi f25, f17, f18\n"                           // opcode=0x17289cfb
		"fmvz.x.ps x24, f16, 3\n"                             // opcode=0xe0380c7b
		"fmax.s f0, f19, f11\n"                               // opcode=0x28b99053


		"fmul.s f28, f13, f19, dyn\n"                         // opcode=0x1136fe53
		"fcvt.un10.ps f21, f19\n"                             // opcode=0xd9298afb
		//RTLMIN-5479"fsin.ps f28, f19\n"                                  // opcode=0x58698e7b
		"fsat8.pi f28, f11\n"                                 // opcode=0x0605be7b
		"faddi.pi f26, f12, 357\n"                            // opcode=0xb4a60d3f
		"fsgnj.ps f25, f16, f11\n"                            // opcode=0x20b80cfb
		"fcvt.f10.ps f21, f17\n"                              // opcode=0xd8b88afb
		"fcvt.pw.ps f29, f11, dyn\n"                          // opcode=0xc005fefb
		"fminu.pi f0, f18, f12\n"                             // opcode=0x2ec9207b
		"fcvt.w.s x20, f17, dyn\n"                            // opcode=0xc008fa53
		"fnmadd.ps f22, f15, f11, f19, dyn\n"                 // opcode=0x9eb7fb5b
		"fcvt.f16.ps f31, f14\n"                              // opcode=0xd8970ffb


		"flq2 f23, 0 (x12)\n"                                 // opcode=0xfc665b87
		"flt.ps f28, f13, f14\n"                              // opcode=0xa0e69e7b
		"cubesgnsc.ps f26, f14, f17\n"                        // opcode=0x89172d7b


		"flog.ps f25, f13\n"                                  // opcode=0x58368cfb


		"fmul.ps f29, f18, f11, dyn\n"                        // opcode=0x10b97efb
		"fcvt.un16.ps f31, f18\n"                             // opcode=0xd9190ffb
		"maskpopcz x21, m7\n"                                 // opcode=0x54038afb
		"fbcx.ps f25, x12\n"                                  // opcode=0x00063c8b
		"fsrli.pi f0, f14, 6\n"                               // opcode=0x4ec7507b
		"fclass.s x21, f12\n"                                 // opcode=0xe0061ad3
		"mov.m.x m4, x15, 100\n"                              // opcode=0x5797827b

		 
		"fence\n"

		"cubefaceidx.ps f30, f16, f15\n"                      // opcode=0x88f81f7b

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.f16.ps f30, f14\n"                              // opcode=0xd8970f7b
		"fcvt.un8.ps f27, f11\n"                              // opcode=0xd9358dfb
		"maskor m0, m4, m3\n"                                 // opcode=0x6632607b


		"for.pi f20, f18, f14\n"                              // opcode=0x06e96a7b
		"flw.ps f23, -1136(x18)\n"                            // opcode=0xb9092b8b
		"mov.m.x m4, x19, 50\n"                               // opcode=0x56c9d27b
		"for.pi f0, f19, f11\n"                               // opcode=0x06b9e07b
		"fmsub.s f26, f18, f17, f15, dyn\n"                   // opcode=0x79197d47
		"fcvt.pwu.ps f29, f19, dyn\n"                         // opcode=0xc019fefb

		 
		"fence\n"



		"fsrl.pi f29, f10, f18\n"                             // opcode=0x07255efb
		"maskpopcz x24, m6\n"                                 // opcode=0x54030c7b
		"mova.m.x x16\n"                                      // opcode=0xd608107b
		"fnmadd.s f22, f14, f11, f10, dyn\n"                  // opcode=0x50b77b4f
		"fmul.pi f25, f17, f15\n"                             // opcode=0x16f88cfb
		"flw.ps f0, 859(x16)\n"                               // opcode=0x35b8200b
		"fsra.pi f28, f14, f17\n"                             // opcode=0x0f175e7b
		"for.pi f27, f17, f12\n"                              // opcode=0x06c8edfb
		"flt.pi f22, f11, f15\n"                              // opcode=0xa6f59b7b
		"fmax.s f23, f18, f10\n"                              // opcode=0x28a91bd3
		"fexp.ps f27, f13\n"                                  // opcode=0x58468dfb
		"fnot.pi f30, f15\n"                                  // opcode=0x0607af7b
		"fsgnjn.ps f25, f10, f11\n"                           // opcode=0x20b51cfb
		"fmadd.ps f23, f13, f17, f15, dyn\n"                  // opcode=0x7916fbdb
		"fle.s x21, f13, f10\n"                               // opcode=0xa0a68ad3
		"feq.s x27, f17, f13\n"                               // opcode=0xa0d8add3
		"flw.ps f25, -783(x19)\n"                             // opcode=0xcf19ac8b
		"feq.s x26, f18, f19\n"                               // opcode=0xa1392d53
		"fsw.ps f19, 1938(x11)\n"                             // opcode=0x7935e90b
		"fle.pi f0, f15, f18\n"                               // opcode=0xa727807b
		"mova.x.m x23\n"                                      // opcode=0xd6000bfb
		"fsub.s f24, f16, f11, dyn\n"                         // opcode=0x08b87c53


		"fmv.s.x f25, x12\n"                                  // opcode=0xf0060cd3
		"flt.ps f31, f10, f15\n"                              // opcode=0xa0f51ffb
		"fmadd.ps f21, f19, f17, f14, dyn\n"                  // opcode=0x7119fadb
		"fmax.s f0, f10, f16\n"                               // opcode=0x29051053
		"fle.s x21, f12, f19\n"                               // opcode=0xa1360ad3






		"fsrai.pi f31, f10, 9\n"                              // opcode=0x4f357ffb
		"fmv.s.x f24, x15\n"                                  // opcode=0xf0078c53
		"fslli.pi f31, f18, 4\n"                              // opcode=0x4e991ffb
		"fcvt.pwu.ps f23, f17, dyn\n"                         // opcode=0xc018fbfb
		"fsat8.pi f29, f15\n"                                 // opcode=0x0607befb


		"fcvt.un2.ps f31, f17\n"                              // opcode=0xd9788ffb
		"fcvt.pw.ps f23, f19, dyn\n"                          // opcode=0xc009fbfb
		"mov.m.x m5, x19, 72\n"                               // opcode=0x572992fb
		"fsat8.pi f31, f19\n"                                 // opcode=0x0609bffb
		"fmaxu.pi f24, f11, f19\n"                            // opcode=0x2f35bc7b
		"fadd.pi f28, f14, f19\n"                             // opcode=0x07370e7b


		"fsq2 f18, 0 (x11)\n"                                 // opcode=0x6325d727
		"fsatu8.pi f20, f18\n"                                // opcode=0x06193a7b
		"fbci.pi f26, 49416\n"                                // opcode=0x18211d5f

		"li x5, 0x2e44f8f8c7fbca93\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmadd.s f27, f14, f19, f19, dyn\n"                   // opcode=0x99377dc3
		"fclass.ps f29, f17\n"                                // opcode=0xe0089efb
		"fmul.pi f21, f10, f12\n"                             // opcode=0x16c50afb


		"fcvt.s.w f20, x15, dyn\n"                            // opcode=0xd007fa53
		"fsra.pi f27, f19, f19\n"                             // opcode=0x0f39ddfb
		"fmadd.ps f31, f10, f17, f18, dyn\n"                  // opcode=0x91157fdb
		"feq.s x0, f18, f17\n"                                // opcode=0xa1192053
		"fround.ps f29, f18, dyn\n"                           // opcode=0x58197efb
		"fcvt.un2.ps f26, f10\n"                              // opcode=0xd9750d7b
		"fand.pi f26, f13, f10\n"                             // opcode=0x06a6fd7b
		"fmax.ps f0, f17, f15\n"                              // opcode=0x28f8907b
		"fmv.s.x f30, x12\n"                                  // opcode=0xf0060f53


		"fcvt.un24.ps f25, f14\n"                             // opcode=0xd9070cfb
		"fcvt.ps.f11 f30, f18\n"                              // opcode=0xd0990f7b
		"fsll.pi f29, f15, f11\n"                             // opcode=0x06b79efb
		//RTLMIN-5479"frsq.ps f27, f13\n"                                  // opcode=0x58868dfb
		"fcvt.ps.f11 f29, f18\n"                              // opcode=0xd0990efb
		"fsll.pi f27, f15, f19\n"                             // opcode=0x07379dfb
		"fsw f18, -957(x13)\n"                                // opcode=0xc526a1a7


		"fcvt.f16.ps f24, f15\n"                              // opcode=0xd8978c7b
		"fcvt.f16.ps f21, f18\n"                              // opcode=0xd8990afb
		"cubesgntc.ps f31, f14, f14\n"                        // opcode=0x88e73ffb
		"fcvt.w.s x28, f15, dyn\n"                            // opcode=0xc007fe53
		"fmv.x.s x31, f15\n"                                  // opcode=0xe0078fd3
		"fsrai.pi f31, f13, 15\n"                             // opcode=0x4ff6fffb


		"fcvt.ps.pw f23, f15, dyn\n"                          // opcode=0xd007fbfb
		"maskpopcz x22, m4\n"                                 // opcode=0x54020b7b


		"fcvt.un16.ps f29, f11\n"                             // opcode=0xd9158efb
		"flw.ps f23, -886(x15)\n"                             // opcode=0xc8a7ab8b
		"fcmovm.ps f21, f19, f10\n"                           // opcode=0x00a98af7
		//RTLMIN-5479"frsq.ps f20, f14\n"                                  // opcode=0x58870a7b
		"flw f31, -1968(x14)\n"                               // opcode=0x85072f87
		"fnmsub.ps f22, f10, f17, f10, dyn\n"                 // opcode=0x55157b5b
		"fmv.s.x f31, x12\n"                                  // opcode=0xf0060fd3
		"fsgnjn.s f25, f17, f15\n"                            // opcode=0x20f89cd3
		"fcvt.un2.ps f20, f10\n"                              // opcode=0xd9750a7b
		"fpackreph.pi f21, f17\n"                             // opcode=0x26089afb
		"feq.s x26, f18, f10\n"                               // opcode=0xa0a92d53
		"fcvt.f10.ps f29, f15\n"                              // opcode=0xd8b78efb
		"fcvt.un8.ps f0, f17\n"                               // opcode=0xd938807b


		"flq2 f0, 0 (x15)\n"                                  // opcode=0x7087d007


		"flem.ps m4, f12, f17\n"                              // opcode=0xa116427b
		"fsra.pi f29, f17, f16\n"                             // opcode=0x0f08defb
		"fgb.ps f28, f1(x17)\n"                               // opcode=0x49109e0b
		"frcp.ps f24, f12\n"                                  // opcode=0x58760c7b
		"flt.s x22, f10, f13\n"                               // opcode=0xa0d51b53
		"fexp.ps f0, f19\n"                                   // opcode=0x5849807b


		"fmadd.s f30, f14, f11, f12, dyn\n"                   // opcode=0x60b77f43
		"fsgnjx.ps f29, f19, f10\n"                           // opcode=0x20a9aefb

		 
		"fence\n"

		"fsrli.pi f27, f16, 6\n"                              // opcode=0x4ec85dfb

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmvz.x.ps x0, f14, 2\n"                              // opcode=0xe027007b
		"fmv.s.x f30, x19\n"                                  // opcode=0xf0098f53
		"feq.pi f26, f18, f16\n"                              // opcode=0xa7092d7b
		"fminu.pi f0, f12, f17\n"                             // opcode=0x2f16207b
		"fcmovm.ps f26, f17, f10\n"                           // opcode=0x00a88d77
		"cubeface.ps f31, f18, f14\n"                         // opcode=0x88e90ffb
		"fscb.ps f10, f1(x14)\n"                              // opcode=0xc8e0950b
		"fnmsub.ps f31, f11, f15, f11, rdn\n"                 // opcode=0x5cf5afdb
		"fcvt.wu.s x28, f10, dyn\n"                           // opcode=0xc0157e53
		"fcvt.ps.pw f30, f17, dyn\n"                          // opcode=0xd008ff7b


		"flq2 f22, 0 (x13)\n"                                 // opcode=0x1ca6db07
		"fminu.pi f20, f12, f10\n"                            // opcode=0x2ea62a7b






		"fgw.ps f30, f1(x12)\n"                               // opcode=0x60c09f0b
		"fsatu8.pi f20, f13\n"                                // opcode=0x0616ba7b
		"fcvt.ps.pwu f24, f11, dyn\n"                         // opcode=0xd015fc7b
		"fcvt.ps.pwu f25, f10, dyn\n"                         // opcode=0xd0157cfb
		"fadd.ps f26, f16, f10, dyn\n"                        // opcode=0x00a87d7b
		"maskpopcz x25, m7\n"                                 // opcode=0x54038cfb
		"fcvt.ps.f11 f27, f18\n"                              // opcode=0xd0990dfb
		"fadd.s f23, f10, f10, dyn\n"                         // opcode=0x00a57bd3
		"fcvt.ps.f11 f0, f15\n"                               // opcode=0xd097807b
		"fexp.ps f27, f18\n"                                  // opcode=0x58490dfb
		"fcvt.s.w f24, x11, dyn\n"                            // opcode=0xd005fc53


		"flw f28, 1790(x15)\n"                                // opcode=0x6fe7ae07
		//RTLMIN-5479"fsin.ps f0, f17\n"                                   // opcode=0x5868807b
		"fclass.s x25, f10\n"                                 // opcode=0xe0051cd3
		"fmin.s f25, f10, f13\n"                              // opcode=0x28d50cd3
		"fsrli.pi f27, f14, 2\n"                              // opcode=0x4e475dfb
		"flt.pi f0, f17, f18\n"                               // opcode=0xa728907b
		"fcvt.un10.ps f29, f19\n"                             // opcode=0xd9298efb
		"fgw.ps f22, f1(x17)\n"                               // opcode=0x61109b0b
		"fround.ps f29, f15, dyn\n"                           // opcode=0x5817fefb
		"frcp.ps f0, f18\n"                                   // opcode=0x5879007b
		"fmin.s f30, f13, f13\n"                              // opcode=0x28d68f53


		"fnmsub.ps f20, f19, f16, f18, dyn\n"                 // opcode=0x9509fa5b
		"fpackrepb.pi f30, f16\n"                             // opcode=0x26080f7b
		"fcvt.un16.ps f23, f13\n"                             // opcode=0xd9168bfb
		"flw f0, -1328(x15)\n"                                // opcode=0xad07a007
		"fsub.ps f22, f17, f18, rup\n"                        // opcode=0x0928bb7b


		"feqm.ps m1, f18, f14\n"                              // opcode=0xa0e960fb
		"fsgnjx.ps f24, f15, f10\n"                           // opcode=0x20a7ac7b
		"fexp.ps f31, f17\n"                                  // opcode=0x58488ffb
		"fpackreph.pi f22, f10\n"                             // opcode=0x26051b7b
		"fsat8.pi f28, f11\n"                                 // opcode=0x0605be7b
		"flt.ps f27, f16, f17\n"                              // opcode=0xa1181dfb
		"fsgnjx.s f24, f19, f19\n"                            // opcode=0x2139ac53
		"fmulh.pi f0, f17, f12\n"                             // opcode=0x16c8a07b
		"fle.s x24, f18, f19\n"                               // opcode=0xa1390c53
		"fadd.pi f29, f14, f19\n"                             // opcode=0x07370efb
		"fsatu8.pi f31, f10\n"                                // opcode=0x06153ffb

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flw f31, -1995(x16)\n"                               // opcode=0x83582f87
		"fclass.s x22, f10\n"                                 // opcode=0xe0051b53
		"fsgnjn.ps f27, f17, f16\n"                           // opcode=0x21089dfb
		"feq.pi f23, f19, f12\n"                              // opcode=0xa6c9abfb
		"fadd.s f31, f11, f13, dyn\n"                         // opcode=0x00d5ffd3
		//RTLMIN-5479"fsin.ps f28, f17\n"                                  // opcode=0x58688e7b
		"flem.ps m4, f10, f18\n"                              // opcode=0xa125427b
		"feqm.ps m5, f19, f10\n"                              // opcode=0xa0a9e2fb
		"for.pi f30, f14, f11\n"                              // opcode=0x06b76f7b
		"maskor m4, m2, m1\n"                                 // opcode=0x6611627b
		"fscb.ps f17, f1(x15)\n"                              // opcode=0xc8f0988b
		"fcvt.pw.ps f31, f11, dyn\n"                          // opcode=0xc005fffb
		"maskpopcz x30, m6\n"                                 // opcode=0x54030f7b
		"fnot.pi f22, f14\n"                                  // opcode=0x06072b7b

		 
		"fence\n"

		"fmulhu.pi f30, f15, f12\n"                           // opcode=0x16c79f7b
		"maskxor m1, m7, m3\n"                                // opcode=0x6633c0fb
		"flw.ps f20, 621(x17)\n"                              // opcode=0x26d8aa0b
		"maskxor m0, m2, m6\n"                                // opcode=0x6661407b
		"fsrl.pi f27, f12, f15\n"                             // opcode=0x06f65dfb
		"fbci.ps f26, 414799\n"                               // opcode=0xca89ed1f
		"fmv.s.x f24, x11\n"                                  // opcode=0xf0058c53
		"fpackreph.pi f22, f10\n"                             // opcode=0x26051b7b
		"feq.ps f26, f17, f17\n"                              // opcode=0xa118ad7b
		"flem.ps m3, f19, f16\n"                              // opcode=0xa109c1fb


		"fsq2 f19, 0 (x18)\n"                                 // opcode=0x73395327
		"fmin.s f0, f12, f17\n"                               // opcode=0x29160053
		"frcp.ps f25, f15\n"                                  // opcode=0x58778cfb
		"fmul.pi f31, f10, f15\n"                             // opcode=0x16f50ffb
		"fsub.ps f28, f13, f19, dyn\n"                        // opcode=0x0936fe7b
		"fgh.ps f20, f0(x17)\n"                               // opcode=0x51101a0b
		"fnmadd.ps f0, f14, f19, f19, dyn\n"                  // opcode=0x9f37705b
		"fmaxu.pi f27, f18, f15\n"                            // opcode=0x2ef93dfb
		"fsgnjx.ps f25, f16, f15\n"                           // opcode=0x20f82cfb
		"fmin.ps f28, f12, f10\n"                             // opcode=0x28a60e7b
		"fcvt.f11.ps f0, f15\n"                               // opcode=0xd887807b
		"fsub.ps f25, f14, f19, dyn\n"                        // opcode=0x09377cfb
		"fsat8.pi f23, f12\n"                                 // opcode=0x06063bfb
		"mova.m.x x16\n"                                      // opcode=0xd608107b
		"fle.pi f23, f19, f19\n"                              // opcode=0xa7398bfb
		"fcvt.un16.ps f28, f16\n"                             // opcode=0xd9180e7b
		"fcvt.ps.f10 f20, f11\n"                              // opcode=0xd0858a7b
		"fmax.s f25, f17, f17\n"                              // opcode=0x29189cd3
		"fsgnj.s f31, f17, f12\n"                             // opcode=0x20c88fd3
		"fsub.s f27, f18, f12, dyn\n"                         // opcode=0x08c97dd3


		"flq2 f20, 0 (x14)\n"                                 // opcode=0xdea75a07
		"mova.x.m x25\n"                                      // opcode=0xd6000cfb
		"fnmsub.ps f28, f18, f10, f10, rmm\n"                 // opcode=0x54a94e5b
		"fcvt.wu.s x23, f14, dyn\n"                           // opcode=0xc0177bd3


		"fpackrepb.pi f23, f19\n"                             // opcode=0x26098bfb

		"fence\n"



		"fsq2 f13, 0 (x18)\n"                                 // opcode=0x76d95027

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fexp.ps f23, f19\n"                                  // opcode=0x58498bfb
		"mova.x.m x25\n"                                      // opcode=0xd6000cfb
		"fadd.ps f31, f18, f19, dyn\n"                        // opcode=0x01397ffb


		"fmul.ps f28, f11, f12, dyn\n"                        // opcode=0x10c5fe7b
		"fcvt.un2.ps f28, f12\n"                              // opcode=0xd9760e7b
		"fcvt.sn8.ps f30, f19\n"                              // opcode=0xd9b98f7b
		"fmvs.x.ps x22, f13, 7\n"                             // opcode=0xe076ab7b
		"fxor.pi f0, f18, f13\n"                              // opcode=0x06d9407b
		"fmulh.pi f23, f14, f15\n"                            // opcode=0x16f72bfb


		"fcvt.sn8.ps f26, f17\n"                              // opcode=0xd9b88d7b
		"fmax.ps f26, f18, f15\n"                             // opcode=0x28f91d7b
		"fsch.ps f13, f0(x14)\n"                              // opcode=0xd0e0168b
		"fsgnj.ps f27, f13, f12\n"                            // opcode=0x20c68dfb
		"fmaxu.pi f26, f11, f18\n"                            // opcode=0x2f25bd7b
		"feq.ps f26, f11, f17\n"                              // opcode=0xa115ad7b
		//RTLMIN-5479"frsq.ps f31, f17\n"                                  // opcode=0x58888ffb


		"cubesgnsc.ps f25, f18, f19\n"                        // opcode=0x89392cfb
		"mov.m.x m3, x12, 3\n"                                // opcode=0x560661fb
		"fmsub.ps f0, f13, f14, f15, dyn\n"                   // opcode=0x7ae6f05b
		"maskpopcz x21, m4\n"                                 // opcode=0x54020afb
		"fxor.pi f30, f15, f12\n"                             // opcode=0x06c7cf7b
		"fcvt.ps.pwu f21, f16, dyn\n"                         // opcode=0xd0187afb
		"fgw.ps f22, f0(x15)\n"                               // opcode=0x60f01b0b
		"fmvz.x.ps x20, f13, 6\n"                             // opcode=0xe0668a7b
		"fpackrepb.pi f21, f17\n"                             // opcode=0x26088afb
		"mova.m.x x13\n"                                      // opcode=0xd606907b
		"fcvt.ps.pwu f20, f12, rmm\n"                         // opcode=0xd0164a7b
		"fle.ps f21, f11, f14\n"                              // opcode=0xa0e58afb
		"fsub.s f27, f10, f15, dyn\n"                         // opcode=0x08f57dd3
		"fcvt.ps.pw f28, f18, dyn\n"                          // opcode=0xd0097e7b
		"fbci.pi f25, 57572\n"                                // opcode=0x1c1c8cdf
		"fxor.pi f29, f13, f14\n"                             // opcode=0x06e6cefb
		"fsgnjx.s f24, f15, f11\n"                            // opcode=0x20b7ac53
		"cubesgnsc.ps f27, f14, f11\n"                        // opcode=0x88b72dfb
		"mov.m.x m2, x16, 104\n"                              // opcode=0x57a8117b
		//RTLMIN-5479"fsin.ps f0, f16\n"                                   // opcode=0x5868007b
		"maskor m2, m7, m1\n"                                 // opcode=0x6613e17b
		"flt.s x20, f15, f10\n"                               // opcode=0xa0a79a53
		"flem.ps m3, f10, f16\n"                              // opcode=0xa10541fb
		"fgw.ps f23, f1(x11)\n"                               // opcode=0x60b09b8b
		"fltm.pi m3, f14, f11\n"                              // opcode=0x3eb701fb
		"fadd.s f31, f11, f10, dyn\n"                         // opcode=0x00a5ffd3


		"fand.pi f26, f17, f15\n"                             // opcode=0x06f8fd7b


		"fcvt.un8.ps f29, f15\n"                              // opcode=0xd9378efb
		"fsrl.pi f20, f18, f11\n"                             // opcode=0x06b95a7b
		"fcvt.ps.pwu f24, f10, dyn\n"                         // opcode=0xd0157c7b
		"fadd.s f20, f17, f10, dyn\n"                         // opcode=0x00a8fa53
		"fmsub.s f20, f14, f16, f17, dyn\n"                   // opcode=0x89077a47
		"feq.ps f20, f14, f17\n"                              // opcode=0xa1172a7b
		"fcvt.pwu.ps f0, f15, rne\n"                          // opcode=0xc017807b

		"li x5, 0x583648288a33dc22\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmul.s f21, f17, f18, dyn\n"                         // opcode=0x1128fad3
		"fscb.ps f12, f1(x19)\n"                              // opcode=0xc930960b
		"fmulhu.pi f21, f15, f17\n"                           // opcode=0x17179afb
		"maskpopc x26, m0\n"                                  // opcode=0x52000d7b
		"feq.s x25, f19, f12\n"                               // opcode=0xa0c9acd3
		"fle.s x28, f18, f11\n"                               // opcode=0xa0b90e53
		"fexp.ps f0, f13\n"                                   // opcode=0x5846807b
		"mova.m.x x10\n"                                      // opcode=0xd605107b
		"cubefaceidx.ps f24, f14, f19\n"                      // opcode=0x89371c7b
		"fexp.ps f31, f16\n"                                  // opcode=0x58480ffb
		"fmax.pi f24, f10, f15\n"                             // opcode=0x2ef51c7b
		"fmul.s f21, f19, f17, dyn\n"                         // opcode=0x1119fad3
		"fgb.ps f31, f0(x17)\n"                               // opcode=0x49101f8b
		"fcvt.un16.ps f21, f18\n"                             // opcode=0xd9190afb
		"fcvt.ps.pwu f29, f17, dyn\n"                         // opcode=0xd018fefb
		"fcvt.w.s x25, f10, rne\n"                            // opcode=0xc0050cd3
		"fcvt.w.s x24, f16, dyn\n"                            // opcode=0xc0087c53
		"fmsub.ps f0, f12, f16, f18, dyn\n"                   // opcode=0x9306705b
		"maskor m7, m0, m4\n"                                 // opcode=0x664063fb
		"feq.pi f22, f18, f14\n"                              // opcode=0xa6e92b7b
		"fmvs.x.ps x20, f18, 2\n"                             // opcode=0xe0292a7b


		"fsgnjx.ps f27, f14, f10\n"                           // opcode=0x20a72dfb
		"maskand m7, m0, m3\n"                                // opcode=0x663073fb
		"fnot.pi f26, f12\n"                                  // opcode=0x06062d7b
		"fcvt.un16.ps f22, f17\n"                             // opcode=0xd9188b7b
		"flt.s x25, f18, f13\n"                               // opcode=0xa0d91cd3
		"fcvt.un10.ps f26, f13\n"                             // opcode=0xd9268d7b


		"fsq2 f19, 0 (x15)\n"                                 // opcode=0x3137daa7




		"fbc.ps f21, 394(x12)\n"                              // opcode=0x18a60a8b
		"fandi.pi f25, f15, 59\n"                             // opcode=0x1d779cbf
		"fmsub.s f26, f19, f10, f12, dyn\n"                   // opcode=0x60a9fd47
		"fbci.pi f22, 12792\n"                                // opcode=0x063f0b5f
		"mov.m.x m1, x16, 105\n"                              // opcode=0x57a820fb
		"fpackreph.pi f24, f18\n"                             // opcode=0x26091c7b


		"fsch.ps f12, f0(x17)\n"                              // opcode=0xd110160b
		"fclass.s x29, f10\n"                                 // opcode=0xe0051ed3
		"fnmsub.ps f27, f15, f14, f17, dyn\n"                 // opcode=0x8ce7fddb
		"fcvt.w.s x26, f14, dyn\n"                            // opcode=0xc0077d53


		"masknot m3, m2\n"                                    // opcode=0x660121fb
		"fsgnj.s f30, f15, f12\n"                             // opcode=0x20c78f53
		"fmulh.pi f0, f12, f10\n"                             // opcode=0x16a6207b


		"fmadd.s f0, f15, f17, f19, dyn\n"                    // opcode=0x9917f043
		"fcvt.s.w f22, x15, dyn\n"                            // opcode=0xd007fb53


		"flt.s x30, f10, f14\n"                               // opcode=0xa0e51f53
		"flt.pi f30, f13, f19\n"                              // opcode=0xa7369f7b
		"fcvt.ps.pw f20, f17, rtz\n"                          // opcode=0xd0089a7b
		"maskor m6, m7, m3\n"                                 // opcode=0x6633e37b
		"fmaxu.pi f30, f13, f18\n"                            // opcode=0x2f26bf7b
		"mova.m.x x11\n"                                      // opcode=0xd605907b

		 
		"fence\n"

		"fmul.s f28, f16, f17, dyn\n"                         // opcode=0x11187e53

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fcmovm.ps f27, f16, f12\n"                           // opcode=0x00c80df7


		"fsub.ps f21, f15, f13, rdn\n"                        // opcode=0x08d7aafb
		"maskor m7, m3, m7\n"                                 // opcode=0x6671e3fb
		"maskpopc x29, m6\n"                                  // opcode=0x52030efb
		"fltm.pi m6, f19, f12\n"                              // opcode=0x3ec9837b
		"fle.pi f28, f11, f19\n"                              // opcode=0xa7358e7b
		//RTLMIN-5479"fsin.ps f22, f16\n"                                  // opcode=0x58680b7b
		"fcvt.pw.ps f26, f17, dyn\n"                          // opcode=0xc008fd7b
		"fcvt.ps.pwu f31, f19, rne\n"                         // opcode=0xd0198ffb
		"fmsub.ps f30, f17, f10, f12, dyn\n"                  // opcode=0x62a8ff5b


		"fcvt.f11.ps f30, f11\n"                              // opcode=0xd8858f7b
		"fmulhu.pi f31, f18, f17\n"                           // opcode=0x17191ffb
		"flw.ps f24, -1174(x16)\n"                            // opcode=0xb6a82c0b
		"fbci.pi f31, 200404\n"                               // opcode=0x61da9fdf
		"feqm.ps m6, f13, f16\n"                              // opcode=0xa106e37b
		"fmv.s.x f26, x17\n"                                  // opcode=0xf0088d53
		"maskpopc x22, m4\n"                                  // opcode=0x52020b7b
		"fmvs.x.ps x30, f16, 7\n"                             // opcode=0xe0782f7b
		"fcvt.ps.pw f25, f14, dyn\n"                          // opcode=0xd0077cfb

		 
		"fence\n"



		"fnmadd.s f20, f17, f14, f12, dyn\n"                  // opcode=0x60e8fa4f
		"feq.pi f22, f19, f14\n"                              // opcode=0xa6e9ab7b
		"fsub.pi f24, f10, f18\n"                             // opcode=0x0f250c7b
		"fmvz.x.ps x30, f15, 0\n"                             // opcode=0xe0078f7b
		"fcvt.ps.pw f27, f16, dyn\n"                          // opcode=0xd0087dfb


		"fcvt.f11.ps f25, f14\n"                              // opcode=0xd8870cfb
		"fsat8.pi f29, f12\n"                                 // opcode=0x06063efb
		"fsgnj.s f30, f13, f11\n"                             // opcode=0x20b68f53
		"fltm.pi m0, f14, f11\n"                              // opcode=0x3eb7007b


		"fle.pi f30, f16, f15\n"                              // opcode=0xa6f80f7b
		"fcmovm.ps f22, f11, f10\n"                           // opcode=0x00a58b77
		"fmul.s f24, f12, f15, dyn\n"                         // opcode=0x10f67c53
		//RTLMIN-5479"fsin.ps f28, f18\n"                                  // opcode=0x58690e7b


		"fmax.s f21, f13, f15\n"                              // opcode=0x28f69ad3
		"fmul.pi f25, f10, f11\n"                             // opcode=0x16b50cfb


		"fnmsub.ps f22, f11, f15, f10, dyn\n"                 // opcode=0x54f5fb5b
		"fcvt.sn8.ps f22, f11\n"                              // opcode=0xd9b58b7b
		"fmv.s.x f25, x10\n"                                  // opcode=0xf0050cd3
		"fnmadd.s f20, f12, f17, f14, rtz\n"                  // opcode=0x71161a4f
		"fsrl.pi f24, f12, f13\n"                             // opcode=0x06d65c7b
		"fmsub.s f22, f18, f15, f13, dyn\n"                   // opcode=0x68f97b47
		"fcvt.ps.f16 f31, f13\n"                              // opcode=0xd0a68ffb


		"fmul.ps f22, f11, f13, dyn\n"                        // opcode=0x10d5fb7b


		"for.pi f24, f18, f14\n"                              // opcode=0x06e96c7b
		"fltu.pi f24, f17, f11\n"                             // opcode=0xa6b8bc7b
		"mova.x.m x20\n"                                      // opcode=0xd6000a7b
		"fle.pi f26, f14, f14\n"                              // opcode=0xa6e70d7b
		"maskor m4, m6, m3\n"                                 // opcode=0x6633627b
		"fcvt.un16.ps f22, f13\n"                             // opcode=0xd9168b7b
		//RTLMIN-5479"fsin.ps f29, f17\n"                                  // opcode=0x58688efb
		"fltm.pi m5, f14, f14\n"                              // opcode=0x3ee702fb

		"li x5, 0xd0434eeb2e124e95\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"masknot m0, m2\n"                                    // opcode=0x6601207b
		"fmadd.s f31, f11, f15, f18, dyn\n"                   // opcode=0x90f5ffc3
		"fmv.s.x f28, x12\n"                                  // opcode=0xf0060e53
		"fsub.ps f0, f13, f14, dyn\n"                         // opcode=0x08e6f07b




		"fsq2 f12, 0 (x13)\n"                                 // opcode=0x74c6d5a7
		"ffrc.ps f28, f10\n"                                  // opcode=0x58250e7b
		"maskand m7, m5, m0\n"                                // opcode=0x6602f3fb
		"feq.s x20, f15, f11\n"                               // opcode=0xa0b7aa53
		"fmax.ps f30, f18, f15\n"                             // opcode=0x28f91f7b
		"fcmov.ps f0, f15, f17, f13\n"                        // opcode=0x6d17a03f
		"cubeface.ps f25, f18, f17\n"                         // opcode=0x89190cfb
		"fpackreph.pi f21, f11\n"                             // opcode=0x26059afb
		"mova.m.x x11\n"                                      // opcode=0xd605907b
		"fmaxu.pi f22, f19, f16\n"                            // opcode=0x2f09bb7b
		"fscb.ps f11, f1(x18)\n"                              // opcode=0xc920958b
		"fmul.s f23, f10, f11, dyn\n"                         // opcode=0x10b57bd3
		"fscw.ps f18, f0(x11)\n"                              // opcode=0xe0b0190b
		"fnmadd.s f27, f18, f17, f14, dyn\n"                  // opcode=0x71197dcf
		"fnmadd.ps f21, f12, f13, f16, dyn\n"                 // opcode=0x86d67adb
		"fpackrepb.pi f25, f15\n"                             // opcode=0x26078cfb
		"fmvs.x.ps x27, f15, 7\n"                             // opcode=0xe077adfb
		"fxor.pi f28, f18, f10\n"                             // opcode=0x06a94e7b
		"fbcx.ps f30, x17\n"                                  // opcode=0x0008bf0b


		"fsat8.pi f31, f15\n"                                 // opcode=0x0607bffb
		"ffrc.ps f21, f18\n"                                  // opcode=0x58290afb
		"fmax.s f28, f11, f13\n"                              // opcode=0x28d59e53
		"fcvt.f10.ps f22, f13\n"                              // opcode=0xd8b68b7b
		"fmax.pi f30, f14, f13\n"                             // opcode=0x2ed71f7b
		"fbci.pi f31, 509380\n"                               // opcode=0xf8b89fdf
		"fcvt.s.w f0, x15, dyn\n"                             // opcode=0xd007f053
		"fcvt.un8.ps f29, f19\n"                              // opcode=0xd9398efb
		"fmaxu.pi f25, f16, f12\n"                            // opcode=0x2ec83cfb
		"flw f25, 1328(x13)\n"                                // opcode=0x5306ac87
		"feqm.ps m1, f17, f11\n"                              // opcode=0xa0b8e0fb
		"fmv.s.x f30, x19\n"                                  // opcode=0xf0098f53


		"maskxor m3, m0, m6\n"                                // opcode=0x666041fb
		"flt.pi f26, f10, f17\n"                              // opcode=0xa7151d7b


		"fscb.ps f12, f0(x11)\n"                              // opcode=0xc8b0160b
		"fcvt.un8.ps f25, f16\n"                              // opcode=0xd9380cfb
		"fbc.ps f20, -613(x12)\n"                             // opcode=0xd9b60a0b
		"maskand m3, m7, m5\n"                                // opcode=0x6653f1fb
		"masknot m7, m4\n"                                    // opcode=0x660223fb
		"fadd.s f0, f11, f15, dyn\n"                          // opcode=0x00f5f053
		"fsll.pi f25, f13, f12\n"                             // opcode=0x06c69cfb
		"fcvt.sn8.ps f29, f17\n"                              // opcode=0xd9b88efb
		"fcvt.f10.ps f0, f11\n"                               // opcode=0xd8b5807b


		"fscb.ps f19, f1(x11)\n"                              // opcode=0xc8b0998b
		"fexp.ps f20, f13\n"                                  // opcode=0x58468a7b
		"fcmovm.ps f27, f18, f12\n"                           // opcode=0x00c90df7

		 
		"fence\n"

		"ffrc.ps f21, f10\n"                                  // opcode=0x58250afb

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fsgnj.s f21, f11, f19\n"                             // opcode=0x21358ad3
		"fcvt.sn8.ps f23, f17\n"                              // opcode=0xd9b88bfb
		"fbci.pi f21, 55395\n"                                // opcode=0x1b0c6adf
		"fbci.pi f23, 411068\n"                               // opcode=0xc8b79bdf
		"fcvt.ps.f10 f23, f10\n"                              // opcode=0xd0850bfb
		"fmax.s f20, f19, f17\n"                              // opcode=0x29199a53
		"mov.m.x m2, x19, 99\n"                               // opcode=0x5789f17b
		"fcvt.pw.ps f22, f17, rdn\n"                          // opcode=0xc008ab7b
		"fxor.pi f0, f17, f14\n"                              // opcode=0x06e8c07b
		"fcvt.ps.pw f20, f19, dyn\n"                          // opcode=0xd009fa7b
		"fsgnjn.ps f28, f17, f11\n"                           // opcode=0x20b89e7b
		"masknot m0, m2\n"                                    // opcode=0x6601207b
		"fltu.pi f22, f19, f15\n"                             // opcode=0xa6f9bb7b
		"fsat8.pi f24, f15\n"                                 // opcode=0x0607bc7b
		"flt.ps f23, f12, f13\n"                              // opcode=0xa0d61bfb
		"fsrl.pi f20, f14, f19\n"                             // opcode=0x07375a7b
		"maskpopc x27, m1\n"                                  // opcode=0x52008dfb
		//RTLMIN-5479"frsq.ps f26, f17\n"                                  // opcode=0x58888d7b
		"fsatu8.pi f30, f13\n"                                // opcode=0x0616bf7b
		"cubefaceidx.ps f25, f13, f19\n"                      // opcode=0x89369cfb
		"fcvt.sn16.ps f28, f13\n"                             // opcode=0xd9968e7b
		"fcvt.ps.f16 f24, f15\n"                              // opcode=0xd0a78c7b
		"fmvs.x.ps x0, f11, 3\n"                              // opcode=0xe035a07b
		"fcvt.wu.s x21, f16, dyn\n"                           // opcode=0xc0187ad3
		"fcmovm.ps f25, f14, f15\n"                           // opcode=0x00f70cf7
		"fnot.pi f24, f13\n"                                  // opcode=0x0606ac7b
		"fltu.pi f29, f17, f19\n"                             // opcode=0xa738befb


		"fcvt.sn16.ps f22, f17\n"                             // opcode=0xd9988b7b
		"fcvt.ps.f16 f0, f16\n"                               // opcode=0xd0a8007b
		"ffrc.ps f20, f19\n"                                  // opcode=0x58298a7b
		"fsrai.pi f23, f17, 15\n"                             // opcode=0x4ff8fbfb
		"fmsub.ps f0, f18, f18, f12, dyn\n"                   // opcode=0x6329705b
		"fnmadd.s f28, f19, f19, f15, dyn\n"                  // opcode=0x7939fe4f
		"fadd.s f0, f12, f16, dyn\n"                          // opcode=0x01067053
		"fsgnjx.s f26, f13, f13\n"                            // opcode=0x20d6ad53
		"fcvt.s.w f25, x10, dyn\n"                            // opcode=0xd0057cd3


		"fcvt.ps.f16 f29, f13\n"                              // opcode=0xd0a68efb
		"fmin.s f20, f12, f10\n"                              // opcode=0x28a60a53
		"fcmov.ps f22, f15, f16, f12\n"                       // opcode=0x6507ab3f
		"fle.pi f20, f11, f11\n"                              // opcode=0xa6b58a7b
		"flt.ps f21, f18, f10\n"                              // opcode=0xa0a91afb
		"fcvt.sn8.ps f25, f10\n"                              // opcode=0xd9b50cfb


		"mov.m.x m2, x18, 116\n"                              // opcode=0x57d9017b
		"for.pi f23, f18, f11\n"                              // opcode=0x06b96bfb
		"fle.ps f25, f11, f11\n"                              // opcode=0xa0b58cfb
		"fclass.ps f27, f10\n"                                // opcode=0xe0051dfb
		"fsub.s f22, f14, f12, rne\n"                         // opcode=0x08c70b53
		"fnmadd.ps f29, f19, f19, f17, dyn\n"                 // opcode=0x8f39fedb
		"fnmadd.ps f22, f18, f14, f17, dyn\n"                 // opcode=0x8ee97b5b
		"fcvt.s.wu f21, x10, dyn\n"                           // opcode=0xd0157ad3

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flt.s x28, f16, f11\n"                               // opcode=0xa0b81e53
		"feqm.ps m4, f14, f17\n"                              // opcode=0xa117627b
		"feq.pi f24, f16, f14\n"                              // opcode=0xa6e82c7b
		"maskor m0, m1, m2\n"                                 // opcode=0x6620e07b
		"feq.s x25, f18, f12\n"                               // opcode=0xa0c92cd3


		"fmul.ps f22, f13, f16, dyn\n"                        // opcode=0x1106fb7b
		"fnmadd.s f29, f13, f12, f18, dyn\n"                  // opcode=0x90c6fecf


		"fsra.pi f30, f17, f14\n"                             // opcode=0x0ee8df7b
		"fbci.pi f0, 356888\n"                                // opcode=0xae43105f
		"fmulh.pi f22, f14, f16\n"                            // opcode=0x17072b7b
		"fcvt.ps.f11 f27, f12\n"                              // opcode=0xd0960dfb
		"fsgnjn.s f30, f16, f13\n"                            // opcode=0x20d81f53
		"mov.m.x m1, x15, 10\n"                               // opcode=0x5627d0fb
		"fscb.ps f11, f1(x15)\n"                              // opcode=0xc8f0958b
		"maskpopc x31, m5\n"                                  // opcode=0x52028ffb
		"fbci.pi f29, 20630\n"                                // opcode=0x0a12dedf
		"fbci.pi f20, 249187\n"                               // opcode=0x79ac7a5f
		"fcvt.f11.ps f23, f17\n"                              // opcode=0xd8888bfb
		"fcvt.pwu.ps f20, f15, dyn\n"                         // opcode=0xc017fa7b


		"frcp.ps f0, f10\n"                                   // opcode=0x5875007b
		"fcvt.ps.pwu f23, f16, dyn\n"                         // opcode=0xd0187bfb
		"fmax.s f27, f14, f19\n"                              // opcode=0x29371dd3
		"fmadd.ps f23, f16, f18, f13, dyn\n"                  // opcode=0x69287bdb
		"fcvt.f16.ps f26, f13\n"                              // opcode=0xd8968d7b

		 
		"fence\n"

		"feq.ps f30, f10, f10\n"                              // opcode=0xa0a52f7b
		"fcvt.f10.ps f22, f13\n"                              // opcode=0xd8b68b7b
		"fnmadd.s f21, f19, f16, f17, rmm\n"                  // opcode=0x8909cacf
		"fandi.pi f30, f14, 380\n"                            // opcode=0xbd971f3f
		"fcvt.un24.ps f21, f15\n"                             // opcode=0xd9078afb
		"maskpopc x30, m3\n"                                  // opcode=0x52018f7b
		"cubesgntc.ps f0, f17, f17\n"                         // opcode=0x8918b07b
		"fcmovm.ps f20, f11, f11\n"                           // opcode=0x00b58a77
		"fcmov.ps f0, f11, f18, f14\n"                        // opcode=0x7525a03f
		"for.pi f31, f10, f11\n"                              // opcode=0x06b56ffb


		"fsq2 f16, 0 (x13)\n"                                 // opcode=0xf306dea7
		"fcvt.f10.ps f21, f11\n"                              // opcode=0xd8b58afb
		"fswizz.ps f25, f12, 61\n"                            // opcode=0xe6f62cfb
		"fcvt.ps.f11 f25, f10\n"                              // opcode=0xd0950cfb
		"fsch.ps f12, f0(x17)\n"                              // opcode=0xd110160b
		"fscb.ps f11, f1(x18)\n"                              // opcode=0xc920958b


		"fcvt.ps.pwu f23, f13, dyn\n"                         // opcode=0xd016fbfb
		"fsub.pi f27, f10, f18\n"                             // opcode=0x0f250dfb
		"fcmovm.ps f29, f11, f19\n"                           // opcode=0x01358ef7
		"fmin.s f22, f12, f14\n"                              // opcode=0x28e60b53
		"feq.s x22, f16, f15\n"                               // opcode=0xa0f82b53
		"fsub.s f24, f19, f16, dyn\n"                         // opcode=0x0909fc53
		"fmin.s f31, f17, f10\n"                              // opcode=0x28a88fd3
		"fsub.s f29, f14, f19, dyn\n"                         // opcode=0x09377ed3
		"fclass.ps f23, f17\n"                                // opcode=0xe0089bfb

		 
		"fence\n"

		"fadd.s f22, f18, f15, rdn\n"                         // opcode=0x00f92b53

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fcvt.f11.ps f21, f11\n"                              // opcode=0xd8858afb
		"fsgnj.ps f0, f14, f15\n"                             // opcode=0x20f7007b
		"maskxor m1, m7, m0\n"                                // opcode=0x6603c0fb
		"flt.ps f24, f16, f14\n"                              // opcode=0xa0e81c7b


		"fcvt.un10.ps f24, f15\n"                             // opcode=0xd9278c7b
		"fandi.pi f28, f16, 321\n"                            // opcode=0xa4381e3f
		"fcvt.s.wu f31, x12, rmm\n"                           // opcode=0xd0164fd3
		"mova.m.x x12\n"                                      // opcode=0xd606107b
		"fnot.pi f30, f13\n"                                  // opcode=0x0606af7b
		"fcvt.pwu.ps f23, f13, dyn\n"                         // opcode=0xc016fbfb
		"fclass.ps f21, f13\n"                                // opcode=0xe0069afb




		"fnot.pi f30, f13\n"                                  // opcode=0x0606af7b


		"flq2 f24, 0 (x10)\n"                                 // opcode=0x4c955c07
		"feq.pi f28, f15, f11\n"                              // opcode=0xa6b7ae7b
		//RTLMIN-5479"frsq.ps f22, f17\n"                                  // opcode=0x58888b7b
		"mova.x.m x23\n"                                      // opcode=0xd6000bfb
		"fcvt.f11.ps f21, f10\n"                              // opcode=0xd8850afb
		"fcvt.ps.pw f23, f14, rne\n"                          // opcode=0xd0070bfb


		"flog.ps f29, f17\n"                                  // opcode=0x58388efb


		"fmul.ps f25, f14, f12, dyn\n"                        // opcode=0x10c77cfb


		"flq2 f24, 0 (x11)\n"                                 // opcode=0xfa15dc07
		"maskor m5, m5, m5\n"                                 // opcode=0x6652e2fb
		"fmvs.x.ps x25, f14, 4\n"                             // opcode=0xe0472cfb
		"fcvt.un2.ps f26, f17\n"                              // opcode=0xd9788d7b


		"fmin.ps f0, f18, f12\n"                              // opcode=0x28c9007b
		"fcvt.ps.f10 f25, f13\n"                              // opcode=0xd0868cfb
		"for.pi f20, f19, f16\n"                              // opcode=0x0709ea7b


		"fsq2 f11, 0 (x10)\n"                                 // opcode=0x7cb55ea7
		"fcvt.pwu.ps f30, f15, dyn\n"                         // opcode=0xc017ff7b
		"fmax.pi f28, f11, f15\n"                             // opcode=0x2ef59e7b
		"fmin.ps f21, f19, f17\n"                             // opcode=0x29198afb


		"fsgnjx.s f27, f18, f13\n"                            // opcode=0x20d92dd3
		"fsll.pi f23, f16, f14\n"                             // opcode=0x06e81bfb
		"fmulhu.pi f20, f12, f15\n"                           // opcode=0x16f61a7b
		"mova.m.x x18\n"                                      // opcode=0xd609107b


		"flq2 f21, 0 (x12)\n"                                 // opcode=0xbc665a87
		"fnmsub.s f0, f11, f14, f15, dyn\n"                   // opcode=0x78e5f04b
		"fmax.pi f28, f15, f16\n"                             // opcode=0x2f079e7b
		"fcvt.w.s x31, f17, dyn\n"                            // opcode=0xc008ffd3
		"fsub.pi f25, f18, f14\n"                             // opcode=0x0ee90cfb
		"fcvt.w.s x23, f15, dyn\n"                            // opcode=0xc007fbd3
		"fsch.ps f10, f0(x19)\n"                              // opcode=0xd130150b
		"flw.ps f21, 616(x13)\n"                              // opcode=0x2686aa8b
		"fadd.ps f30, f10, f14, dyn\n"                        // opcode=0x00e57f7b
		"fcvt.ps.f16 f0, f15\n"                               // opcode=0xd0a7807b
		"fbc.ps f22, 1092(x10)\n"                             // opcode=0x44450b0b
		"fcvt.ps.pwu f0, f18, dyn\n"                          // opcode=0xd019707b


		"flq2 f31, 0 (x18)\n"                                 // opcode=0xa2b95f87
		"fmulh.pi f27, f19, f14\n"                            // opcode=0x16e9adfb
		"fsatu8.pi f21, f18\n"                                // opcode=0x06193afb

		"li x5, 0x706e2e44e3847e66\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"frcp.ps f29, f19\n"                                  // opcode=0x58798efb


		"maskor m2, m3, m1\n"                                 // opcode=0x6611e17b


		"fmul.ps f20, f18, f16, dyn\n"                        // opcode=0x11097a7b
		"maskxor m6, m1, m1\n"                                // opcode=0x6610c37b
		"fsrai.pi f29, f18, 7\n"                              // opcode=0x4ef97efb
		"fltm.pi m3, f19, f15\n"                              // opcode=0x3ef981fb
		"fsrl.pi f28, f10, f10\n"                             // opcode=0x06a55e7b
		"ffrc.ps f0, f10\n"                                   // opcode=0x5825007b
		"fltm.ps m1, f13, f16\n"                              // opcode=0xa106d0fb
		"fsgnjx.ps f29, f19, f17\n"                           // opcode=0x2119aefb
		"fmvz.x.ps x31, f16, 5\n"                             // opcode=0xe0580ffb
		"fnmsub.ps f21, f12, f11, f18, dyn\n"                 // opcode=0x94b67adb
		"cubeface.ps f28, f18, f19\n"                         // opcode=0x89390e7b
		"fmulhu.pi f21, f17, f11\n"                           // opcode=0x16b89afb
		"fmul.pi f31, f11, f15\n"                             // opcode=0x16f58ffb
		"fmin.ps f24, f16, f17\n"                             // opcode=0x29180c7b
		"fcvt.ps.pw f27, f14, dyn\n"                          // opcode=0xd0077dfb
		"fandi.pi f24, f13, 140\n"                            // opcode=0x45969c3f
		"fnot.pi f31, f12\n"                                  // opcode=0x06062ffb
		"fle.ps f29, f11, f11\n"                              // opcode=0xa0b58efb
		"fltm.pi m7, f13, f12\n"                              // opcode=0x3ec683fb
		"cubesgntc.ps f23, f19, f12\n"                        // opcode=0x88c9bbfb
		"fsat8.pi f31, f12\n"                                 // opcode=0x06063ffb
		"fsgnj.s f28, f13, f18\n"                             // opcode=0x21268e53
		"fscb.ps f16, f1(x12)\n"                              // opcode=0xc8c0980b
		"fcvt.ps.f10 f0, f14\n"                               // opcode=0xd087007b


		"for.pi f26, f15, f15\n"                              // opcode=0x06f7ed7b
		"feq.s x24, f11, f12\n"                               // opcode=0xa0c5ac53
		"fle.ps f0, f10, f14\n"                               // opcode=0xa0e5007b
		"flw f31, -1471(x18)\n"                               // opcode=0xa4192f87


		"ffrc.ps f30, f11\n"                                  // opcode=0x58258f7b
		"fsra.pi f24, f13, f16\n"                             // opcode=0x0f06dc7b
		"fle.pi f23, f12, f10\n"                              // opcode=0xa6a60bfb
		"fcvt.pw.ps f25, f14, dyn\n"                          // opcode=0xc0077cfb
		"feqm.ps m7, f15, f15\n"                              // opcode=0xa0f7e3fb
		"fsat8.pi f0, f12\n"                                  // opcode=0x0606307b
		"mova.m.x x13\n"                                      // opcode=0xd606907b
		"maskor m1, m0, m6\n"                                 // opcode=0x666060fb
		"fand.pi f29, f13, f18\n"                             // opcode=0x0726fefb
		"maskor m1, m7, m7\n"                                 // opcode=0x6673e0fb
		"faddi.pi f21, f16, 269\n"                            // opcode=0x85a80abf
		"fslli.pi f23, f13, 13\n"                             // opcode=0x4fa69bfb
		"fle.s x22, f12, f19\n"                               // opcode=0xa1360b53
		"fltu.pi f26, f13, f16\n"                             // opcode=0xa706bd7b
		"frcp.ps f29, f15\n"                                  // opcode=0x58778efb
		"fgh.ps f27, f0(x15)\n"                               // opcode=0x50f01d8b
		"masknot m1, m0\n"                                    // opcode=0x660020fb
		"frcp.ps f21, f16\n"                                  // opcode=0x58780afb
		"fexp.ps f23, f15\n"                                  // opcode=0x58478bfb

		 
		"fence\n"

		"feq.pi f28, f12, f10\n"                              // opcode=0xa6a62e7b

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmax.pi f0, f15, f19\n"                              // opcode=0x2f37907b
		"fclass.s x20, f13\n"                                 // opcode=0xe0069a53
		"fcvt.f11.ps f24, f13\n"                              // opcode=0xd8868c7b
		"fcvt.s.wu f29, x16, dyn\n"                           // opcode=0xd0187ed3
		"fmvz.x.ps x25, f19, 1\n"                             // opcode=0xe0198cfb
		"fxor.pi f24, f13, f12\n"                             // opcode=0x06c6cc7b
		"fmulhu.pi f21, f10, f16\n"                           // opcode=0x17051afb
		"flt.s x25, f10, f13\n"                               // opcode=0xa0d51cd3
		"fsub.s f21, f14, f19, dyn\n"                         // opcode=0x09377ad3
		"fpackreph.pi f26, f14\n"                             // opcode=0x26071d7b
		"fand.pi f23, f11, f13\n"                             // opcode=0x06d5fbfb


		"maskand m2, m6, m7\n"                                // opcode=0x6673717b
		"flw.ps f30, -368(x17)\n"                             // opcode=0xe908af0b
		"fbcx.ps f22, x17\n"                                  // opcode=0x0008bb0b
		"maskpopcz x25, m1\n"                                 // opcode=0x54008cfb
		"fcvt.wu.s x28, f11, dyn\n"                           // opcode=0xc015fe53
		"fcmovm.ps f29, f10, f11\n"                           // opcode=0x00b50ef7
		"fsub.ps f24, f19, f14, dyn\n"                        // opcode=0x08e9fc7b
		"flw.ps f26, 1991(x18)\n"                             // opcode=0x7c792d0b
		"faddi.pi f24, f17, 110\n"                            // opcode=0x35d88c3f
		"fle.ps f29, f13, f16\n"                              // opcode=0xa1068efb
		"fcmov.ps f30, f10, f15, f12\n"                       // opcode=0x64f52f3f
		"fcvt.ps.f10 f0, f17\n"                               // opcode=0xd088807b
		"fbcx.ps f26, x12\n"                                  // opcode=0x00063d0b
		"fmaxu.pi f22, f15, f14\n"                            // opcode=0x2ee7bb7b
		"fcvt.ps.f10 f0, f10\n"                               // opcode=0xd085007b
		"mova.m.x x18\n"                                      // opcode=0xd609107b
		"fgb.ps f25, f0(x12)\n"                               // opcode=0x48c01c8b
		"fxor.pi f27, f10, f18\n"                             // opcode=0x07254dfb

		 
		"fence\n"

		"fmulh.pi f29, f16, f14\n"                            // opcode=0x16e82efb
		"maskpopcz x22, m1\n"                                 // opcode=0x54008b7b
		"fmax.s f27, f14, f11\n"                              // opcode=0x28b71dd3
		"fsetm.pi m2, f17\n"                                  // opcode=0xe008d17b
		"fcvt.wu.s x26, f10, dyn\n"                           // opcode=0xc0157d53
		"fcvt.f10.ps f27, f17\n"                              // opcode=0xd8b88dfb
		"fnot.pi f29, f15\n"                                  // opcode=0x0607aefb
		"feq.pi f28, f14, f12\n"                              // opcode=0xa6c72e7b
		"fcvt.s.w f31, x14, dyn\n"                            // opcode=0xd0077fd3
		"fsrl.pi f28, f14, f11\n"                             // opcode=0x06b75e7b
		"fbci.ps f31, 336234\n"                               // opcode=0xa42d4f9f
		"fmv.x.s x31, f13\n"                                  // opcode=0xe0068fd3
		"fxor.pi f24, f15, f16\n"                             // opcode=0x0707cc7b
		"maskand m4, m6, m2\n"                                // opcode=0x6623727b
		"fcvt.s.wu f31, x18, dyn\n"                           // opcode=0xd0197fd3
		"ffrc.ps f23, f11\n"                                  // opcode=0x58258bfb
		"fcvt.s.wu f21, x13, dyn\n"                           // opcode=0xd016fad3
		"fmv.s.x f21, x16\n"                                  // opcode=0xf0080ad3


		"mova.m.x x13\n"                                      // opcode=0xd606907b
		"fsub.s f20, f14, f16, rup\n"                         // opcode=0x09073a53
		"fcvt.un10.ps f31, f10\n"                             // opcode=0xd9250ffb

		"li x5, 0x587d3579b4b34633\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fcvt.ps.pw f24, f18, dyn\n"                          // opcode=0xd0097c7b
		"fcvt.f10.ps f31, f16\n"                              // opcode=0xd8b80ffb
		"fsgnjx.s f26, f10, f14\n"                            // opcode=0x20e52d53
		"fle.ps f24, f15, f11\n"                              // opcode=0xa0b78c7b
		"fadd.ps f26, f15, f17, dyn\n"                        // opcode=0x0117fd7b
		"maskxor m6, m5, m6\n"                                // opcode=0x6662c37b
		"fmulh.pi f23, f11, f14\n"                            // opcode=0x16e5abfb
		"fbci.pi f28, 499361\n"                               // opcode=0xf3d43e5f
		"fsra.pi f30, f12, f19\n"                             // opcode=0x0f365f7b
		"fclass.s x31, f13\n"                                 // opcode=0xe0069fd3


		"fcvt.un2.ps f0, f10\n"                               // opcode=0xd975007b
		"fmulh.pi f28, f17, f13\n"                            // opcode=0x16d8ae7b
		"fclass.ps f21, f18\n"                                // opcode=0xe0091afb
		"fnmadd.ps f22, f13, f14, f13, dyn\n"                 // opcode=0x6ee6fb5b


		"fmul.ps f30, f11, f12, dyn\n"                        // opcode=0x10c5ff7b
		"fle.ps f21, f13, f11\n"                              // opcode=0xa0b68afb
		"fgb.ps f21, f1(x13)\n"                               // opcode=0x48d09a8b


		"fbci.ps f31, 47813\n"                                // opcode=0x1758af9f
		"fadd.s f30, f19, f13, dyn\n"                         // opcode=0x00d9ff53
		"fcvt.s.w f23, x11, dyn\n"                            // opcode=0xd005fbd3


		"feq.s x29, f11, f13\n"                               // opcode=0xa0d5aed3
		"fmaxu.pi f0, f13, f11\n"                             // opcode=0x2eb6b07b
		"fmsub.ps f28, f18, f19, f16, dyn\n"                  // opcode=0x83397e5b
		"fcvt.ps.f16 f20, f10\n"                              // opcode=0xd0a50a7b
		"fsrl.pi f22, f19, f14\n"                             // opcode=0x06e9db7b
		"fpackreph.pi f27, f18\n"                             // opcode=0x26091dfb
		"fminu.pi f22, f13, f18\n"                            // opcode=0x2f26ab7b
		"fcvt.ps.f10 f24, f12\n"                              // opcode=0xd0860c7b
		"fcvt.pwu.ps f28, f16, rne\n"                         // opcode=0xc0180e7b
		"fscb.ps f17, f1(x10)\n"                              // opcode=0xc8a0988b
		"fcvt.sn8.ps f27, f12\n"                              // opcode=0xd9b60dfb


		"fmin.ps f20, f17, f19\n"                             // opcode=0x29388a7b
		"fnmsub.s f24, f14, f17, f10, rup\n"                  // opcode=0x51173c4b
		"fsgnjx.s f28, f18, f12\n"                            // opcode=0x20c92e53
		"fmadd.s f29, f12, f16, f17, dyn\n"                   // opcode=0x89067ec3
		"fcvt.f10.ps f22, f10\n"                              // opcode=0xd8b50b7b
		"fadd.s f31, f10, f14, dyn\n"                         // opcode=0x00e57fd3
		"fadd.ps f27, f14, f15, dyn\n"                        // opcode=0x00f77dfb
		"fltm.ps m5, f17, f14\n"                              // opcode=0xa0e8d2fb
		"fbci.pi f21, 340513\n"                               // opcode=0xa6443adf
		"cubesgnsc.ps f31, f17, f10\n"                        // opcode=0x88a8affb
		"fcvt.un24.ps f25, f16\n"                             // opcode=0xd9080cfb
		"fgh.ps f22, f0(x19)\n"                               // opcode=0x51301b0b
		"fscb.ps f18, f0(x17)\n"                              // opcode=0xc910190b
		"fnmsub.s f27, f19, f13, f11, dyn\n"                  // opcode=0x58d9fdcb


		"fcvt.ps.pw f31, f15, dyn\n"                          // opcode=0xd007fffb
		"frcp.ps f24, f12\n"                                  // opcode=0x58760c7b
		"fle.ps f23, f16, f19\n"                              // opcode=0xa1380bfb
		"fcvt.ps.f11 f20, f13\n"                              // opcode=0xd0968a7b

		 
		"fence\n"

		"fround.ps f24, f19, dyn\n"                           // opcode=0x5819fc7b

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsrai.pi f23, f11, 11\n"                             // opcode=0x4f65fbfb
		"feqm.ps m7, f11, f15\n"                              // opcode=0xa0f5e3fb
		"fcvt.s.w f21, x11, dyn\n"                            // opcode=0xd005fad3
		"fcvt.wu.s x28, f17, dyn\n"                           // opcode=0xc018fe53
		"fmv.x.s x25, f11\n"                                  // opcode=0xe0058cd3
		"fmsub.ps f30, f15, f10, f11, dyn\n"                  // opcode=0x5aa7ff5b


		"flq2 f31, 0 (x11)\n"                                 // opcode=0x5115df87


		"feqm.ps m7, f17, f10\n"                              // opcode=0xa0a8e3fb
		"fcvt.w.s x0, f16, dyn\n"                             // opcode=0xc0087053
		"ffrc.ps f31, f19\n"                                  // opcode=0x58298ffb
		"fsgnjx.s f31, f17, f15\n"                            // opcode=0x20f8afd3
		"fsrl.pi f20, f12, f12\n"                             // opcode=0x06c65a7b
		"flt.ps f26, f12, f10\n"                              // opcode=0xa0a61d7b
		"fscw.ps f18, f0(x10)\n"                              // opcode=0xe0a0190b
		"cubefaceidx.ps f27, f16, f16\n"                      // opcode=0x89081dfb
		"fcvt.un16.ps f27, f12\n"                             // opcode=0xd9160dfb
		"fmin.ps f24, f10, f12\n"                             // opcode=0x28c50c7b
		"fclass.ps f22, f12\n"                                // opcode=0xe0061b7b
		"feq.s x0, f17, f10\n"                                // opcode=0xa0a8a053
		"mova.m.x x12\n"                                      // opcode=0xd606107b
		"fcvt.pw.ps f21, f19, dyn\n"                          // opcode=0xc009fafb
		"fexp.ps f0, f14\n"                                   // opcode=0x5847007b
		"fscw.ps f18, f1(x10)\n"                              // opcode=0xe0a0990b
		"fmin.pi f30, f12, f15\n"                             // opcode=0x2ef60f7b
		"mova.x.m x27\n"                                      // opcode=0xd6000dfb
		"maskand m3, m3, m3\n"                                // opcode=0x6631f1fb
		"cubesgntc.ps f22, f14, f14\n"                        // opcode=0x88e73b7b
		"cubesgnsc.ps f28, f11, f10\n"                        // opcode=0x88a5ae7b
		"fscb.ps f10, f0(x11)\n"                              // opcode=0xc8b0150b
		"fsub.s f20, f10, f18, dyn\n"                         // opcode=0x09257a53
		"fpackreph.pi f31, f11\n"                             // opcode=0x26059ffb
		"fsatu8.pi f31, f18\n"                                // opcode=0x06193ffb
		"fand.pi f24, f15, f19\n"                             // opcode=0x0737fc7b
		"cubesgntc.ps f27, f10, f15\n"                        // opcode=0x88f53dfb
		"fmaxu.pi f28, f14, f10\n"                            // opcode=0x2ea73e7b
		"fnmsub.s f30, f18, f14, f10, dyn\n"                  // opcode=0x50e97f4b


		"mov.m.x m6, x12, 69\n"                               // opcode=0x5716237b
		"fmvs.x.ps x26, f16, 5\n"                             // opcode=0xe0582d7b
		"fltu.pi f23, f18, f10\n"                             // opcode=0xa6a93bfb
		"feqm.ps m6, f17, f17\n"                              // opcode=0xa118e37b
		"flem.ps m4, f17, f11\n"                              // opcode=0xa0b8c27b
		"fcvt.s.w f31, x10, dyn\n"                            // opcode=0xd0057fd3


		"fandi.pi f27, f18, 282\n"                            // opcode=0x8d491dbf
		"fmin.pi f24, f16, f15\n"                             // opcode=0x2ef80c7b


		"fsq2 f15, 0 (x14)\n"                                 // opcode=0x36f75c27
		"fslli.pi f29, f11, 4\n"                              // opcode=0x4e959efb
		"fltu.pi f24, f11, f12\n"                             // opcode=0xa6c5bc7b
		"mov.m.x m4, x14, 75\n"                               // opcode=0x5727627b
		"fand.pi f26, f15, f14\n"                             // opcode=0x06e7fd7b
		"faddi.pi f27, f10, 443\n"                            // opcode=0xdd650dbf

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fpackrepb.pi f23, f14\n"                             // opcode=0x26070bfb
		"fcmovm.ps f22, f10, f14\n"                           // opcode=0x00e50b77
		"fltu.pi f24, f12, f13\n"                             // opcode=0xa6d63c7b
		"fmvz.x.ps x29, f15, 3\n"                             // opcode=0xe0378efb
		"fsgnj.ps f30, f17, f17\n"                            // opcode=0x21188f7b
		"feq.ps f28, f15, f17\n"                              // opcode=0xa117ae7b
		"fcvt.s.wu f28, x11, dyn\n"                           // opcode=0xd015fe53


		"fmul.pi f23, f18, f16\n"                             // opcode=0x17090bfb
		"ffrc.ps f25, f14\n"                                  // opcode=0x58270cfb
		"flt.s x29, f15, f12\n"                               // opcode=0xa0c79ed3


		"fmsub.ps f26, f19, f10, f11, dyn\n"                  // opcode=0x5aa9fd5b
		"fsrai.pi f27, f17, 3\n"                              // opcode=0x4e68fdfb
		"fmin.ps f22, f18, f14\n"                             // opcode=0x28e90b7b
		"mov.m.x m0, x17, 71\n"                               // opcode=0x5718f07b
		"fltm.ps m6, f17, f11\n"                              // opcode=0xa0b8d37b
		"ffrc.ps f31, f17\n"                                  // opcode=0x58288ffb
		"fsub.s f24, f16, f10, dyn\n"                         // opcode=0x08a87c53
		"fcvt.ps.f16 f27, f14\n"                              // opcode=0xd0a70dfb
		"fcvt.f16.ps f31, f14\n"                              // opcode=0xd8970ffb
		"fsw f11, -481(x10)\n"                                // opcode=0xe0b52fa7
		"fmin.pi f29, f16, f17\n"                             // opcode=0x2f180efb
		"fle.s x24, f17, f18\n"                               // opcode=0xa1288c53
		"fcvt.s.w f29, x12, dyn\n"                            // opcode=0xd0067ed3
		"fcvt.un10.ps f27, f17\n"                             // opcode=0xd9288dfb
		"fsll.pi f29, f14, f15\n"                             // opcode=0x06f71efb
		"fsatu8.pi f29, f10\n"                                // opcode=0x06153efb


		"fscw.ps f10, f0(x15)\n"                              // opcode=0xe0f0150b
		"fcvt.wu.s x23, f15, dyn\n"                           // opcode=0xc017fbd3
		"fmsub.s f27, f13, f19, f19, dyn\n"                   // opcode=0x9936fdc7
		"masknot m3, m2\n"                                    // opcode=0x660121fb
		"fsch.ps f12, f0(x17)\n"                              // opcode=0xd110160b
		"fmadd.s f30, f10, f15, f16, dyn\n"                   // opcode=0x80f57f43
		"fand.pi f26, f15, f15\n"                             // opcode=0x06f7fd7b


		"flq2 f23, 0 (x12)\n"                                 // opcode=0xa9f65b87

		 
		"fence\n"

		"fminu.pi f22, f12, f18\n"                            // opcode=0x2f262b7b
		"fcvt.s.w f22, x15, dyn\n"                            // opcode=0xd007fb53
		"fsgnjn.ps f29, f10, f11\n"                           // opcode=0x20b51efb
		"fcvt.ps.f11 f20, f19\n"                              // opcode=0xd0998a7b
		"fltm.ps m6, f12, f11\n"                              // opcode=0xa0b6537b
		"frcp.ps f23, f11\n"                                  // opcode=0x58758bfb
		"fswizz.ps f22, f14, 116\n"                           // opcode=0xe7d70b7b
		"fsw.ps f13, 675(x19)\n"                              // opcode=0x2ad9e18b
		"fsgnj.s f31, f15, f17\n"                             // opcode=0x21178fd3
		"fcvt.un16.ps f26, f13\n"                             // opcode=0xd9168d7b
		"fmadd.s f23, f17, f16, f11, dyn\n"                   // opcode=0x5908fbc3
		"fcvt.ps.pwu f0, f11, dyn\n"                          // opcode=0xd015f07b




		"fmvs.x.ps x30, f19, 7\n"                             // opcode=0xe079af7b
		"fcvt.un24.ps f25, f11\n"                             // opcode=0xd9058cfb
		"fmv.x.s x20, f12\n"                                  // opcode=0xe0060a53

		 
		"fence\n"



		"flog.ps f29, f19\n"                                  // opcode=0x58398efb

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmv.x.s x28, f15\n"                                  // opcode=0xe0078e53
		"fcvt.s.w f30, x12, dyn\n"                            // opcode=0xd0067f53


		"fmsub.s f22, f18, f10, f15, dyn\n"                   // opcode=0x78a97b47
		"fandi.pi f21, f15, 80\n"                             // opcode=0x2c079abf
		"fmadd.s f30, f13, f19, f15, dyn\n"                   // opcode=0x7936ff43
		"fsrai.pi f26, f19, 10\n"                             // opcode=0x4f59fd7b
		"fltm.pi m1, f17, f11\n"                              // opcode=0x3eb880fb
		"fsat8.pi f27, f14\n"                                 // opcode=0x06073dfb
		"fcvt.s.w f29, x14, dyn\n"                            // opcode=0xd0077ed3
		"fgh.ps f20, f0(x15)\n"                               // opcode=0x50f01a0b
		"fmax.pi f22, f18, f10\n"                             // opcode=0x2ea91b7b
		"fcvt.ps.f10 f29, f18\n"                              // opcode=0xd0890efb
		"maskpopc x28, m1\n"                                  // opcode=0x52008e7b
		"fsw f11, -1432(x17)\n"                               // opcode=0xa6b8a427
		"fcvt.un8.ps f24, f12\n"                              // opcode=0xd9360c7b
		"fmadd.s f29, f12, f17, f17, dyn\n"                   // opcode=0x89167ec3
		"fltm.pi m1, f10, f18\n"                              // opcode=0x3f2500fb
		"fltm.pi m4, f16, f18\n"                              // opcode=0x3f28027b
		"fcvt.un16.ps f24, f14\n"                             // opcode=0xd9170c7b


		"fmin.ps f26, f16, f13\n"                             // opcode=0x28d80d7b
		"maskor m3, m1, m5\n"                                 // opcode=0x6650e1fb
		"maskpopcz x26, m6\n"                                 // opcode=0x54030d7b
		"fmax.ps f27, f12, f11\n"                             // opcode=0x28b61dfb
		"feq.ps f27, f19, f16\n"                              // opcode=0xa109adfb
		"fmvz.x.ps x20, f16, 5\n"                             // opcode=0xe0580a7b
		"fbcx.ps f0, x19\n"                                   // opcode=0x0009b00b
		"fle.pi f20, f12, f18\n"                              // opcode=0xa7260a7b


		"fcvt.sn8.ps f28, f10\n"                              // opcode=0xd9b50e7b
		"fcvt.s.wu f29, x19, dyn\n"                           // opcode=0xd019fed3


		"fcvt.ps.f11 f25, f17\n"                              // opcode=0xd0988cfb
		"fnot.pi f0, f16\n"                                   // opcode=0x0608207b
		"fgw.ps f20, f0(x18)\n"                               // opcode=0x61201a0b
		"cubefaceidx.ps f0, f10, f16\n"                       // opcode=0x8905107b
		"feqm.ps m5, f10, f15\n"                              // opcode=0xa0f562fb


		"fadd.pi f26, f13, f18\n"                             // opcode=0x07268d7b
		"feq.ps f0, f18, f15\n"                               // opcode=0xa0f9207b
		"fcvt.ps.pwu f24, f13, rne\n"                         // opcode=0xd0168c7b
		"fsub.pi f24, f13, f14\n"                             // opcode=0x0ee68c7b


		"for.pi f26, f15, f19\n"                              // opcode=0x0737ed7b
		"fle.pi f30, f12, f13\n"                              // opcode=0xa6d60f7b
		"fnmsub.ps f29, f10, f10, f16, dyn\n"                 // opcode=0x84a57edb


		"flog.ps f21, f11\n"                                  // opcode=0x58358afb
		"fsrai.pi f29, f17, 0\n"                              // opcode=0x4e18fefb
		"fgw.ps f29, f0(x14)\n"                               // opcode=0x60e01e8b
		"fcvt.s.w f20, x19, dyn\n"                            // opcode=0xd009fa53




		"maskxor m2, m3, m5\n"                                // opcode=0x6651c17b
		"flt.pi f30, f19, f19\n"                              // opcode=0xa7399f7b
		"fmin.pi f26, f14, f12\n"                             // opcode=0x2ec70d7b
		"maskand m2, m6, m0\n"                                // opcode=0x6603717b
		"fcvt.wu.s x24, f19, dyn\n"                           // opcode=0xc019fc53

		"li x5, 0xeba2fad7f1ddb667\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fmsub.s f20, f14, f15, f16, dyn\n"                   // opcode=0x80f77a47
		"fsrli.pi f24, f16, 0\n"                              // opcode=0x4e185c7b
		"feq.s x23, f13, f10\n"                               // opcode=0xa0a6abd3
		"fsgnjn.ps f21, f13, f16\n"                           // opcode=0x21069afb
		"fadd.pi f29, f13, f14\n"                             // opcode=0x06e68efb
		"fcvt.ps.f11 f27, f15\n"                              // opcode=0xd0978dfb
		//RTLMIN-5479"fsin.ps f26, f19\n"                                  // opcode=0x58698d7b
		"fcvt.ps.pwu f25, f12, dyn\n"                         // opcode=0xd0167cfb
		"fadd.pi f25, f14, f12\n"                             // opcode=0x06c70cfb


		"fslli.pi f29, f19, 7\n"                              // opcode=0x4ee99efb


		"fsgnj.s f20, f17, f12\n"                             // opcode=0x20c88a53
		"fcvt.un24.ps f20, f17\n"                             // opcode=0xd9088a7b
		"cubesgntc.ps f30, f15, f17\n"                        // opcode=0x8917bf7b
		"fsll.pi f22, f15, f12\n"                             // opcode=0x06c79b7b
		//RTLMIN-5479"fsin.ps f26, f15\n"                                  // opcode=0x58678d7b


		"fcvt.f16.ps f21, f16\n"                              // opcode=0xd8980afb
		"maskor m2, m0, m1\n"                                 // opcode=0x6610617b
		"fswizz.ps f27, f14, 29\n"                            // opcode=0xe6772dfb
		"fsgnj.ps f29, f11, f14\n"                            // opcode=0x20e58efb
		"fscw.ps f19, f1(x10)\n"                              // opcode=0xe0a0998b


		"flem.ps m2, f12, f15\n"                              // opcode=0xa0f6417b
		"fsw f18, 1005(x12)\n"                                // opcode=0x3f2626a7
		"maskpopcz x20, m1\n"                                 // opcode=0x54008a7b
		"masknot m5, m1\n"                                    // opcode=0x6600a2fb
		"fcvt.sn16.ps f21, f19\n"                             // opcode=0xd9998afb
		"fmax.pi f29, f14, f12\n"                             // opcode=0x2ec71efb
		"fsat8.pi f28, f17\n"                                 // opcode=0x0608be7b
		"fcvt.un2.ps f26, f16\n"                              // opcode=0xd9780d7b
		"fsatu8.pi f28, f19\n"                                // opcode=0x0619be7b
		"fcvt.s.w f27, x17, dyn\n"                            // opcode=0xd008fdd3
		"fsll.pi f22, f17, f15\n"                             // opcode=0x06f89b7b
		"fsgnjx.s f30, f18, f12\n"                            // opcode=0x20c92f53
		"fsub.pi f30, f15, f15\n"                             // opcode=0x0ef78f7b
		"fcvt.f11.ps f26, f11\n"                              // opcode=0xd8858d7b
		"cubesgnsc.ps f0, f19, f15\n"                         // opcode=0x88f9a07b
		"fsrl.pi f28, f13, f18\n"                             // opcode=0x0726de7b


		"fmax.pi f30, f15, f16\n"                             // opcode=0x2f079f7b
		"fgb.ps f20, f0(x13)\n"                               // opcode=0x48d01a0b
		"flt.s x28, f17, f13\n"                               // opcode=0xa0d89e53
		"fcvt.ps.f10 f0, f15\n"                               // opcode=0xd087807b
		"fltm.ps m2, f13, f10\n"                              // opcode=0xa0a6d17b
		"fle.s x23, f15, f15\n"                               // opcode=0xa0f78bd3
		"fcmovm.ps f0, f10, f14\n"                            // opcode=0x00e50077
		"flw.ps f21, -996(x10)\n"                             // opcode=0xc1c52a8b
		"fsgnjx.s f30, f19, f12\n"                            // opcode=0x20c9af53
		"fmv.s.x f31, x15\n"                                  // opcode=0xf0078fd3
		"fltm.pi m0, f14, f13\n"                              // opcode=0x3ed7007b
		"flt.s x0, f17, f12\n"                                // opcode=0xa0c89053
		"fltu.pi f22, f16, f16\n"                             // opcode=0xa7083b7b

		 
		"fence\n"

		"faddi.pi f23, f19, 164\n"                            // opcode=0x54898bbf

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsch.ps f13, f0(x10)\n"                              // opcode=0xd0a0168b
		"fsgnjn.ps f24, f13, f14\n"                           // opcode=0x20e69c7b
		"fclass.ps f30, f14\n"                                // opcode=0xe0071f7b
		"fsat8.pi f21, f18\n"                                 // opcode=0x06093afb


		"fle.s x25, f14, f12\n"                               // opcode=0xa0c70cd3
		"fcvt.pw.ps f20, f17, dyn\n"                          // opcode=0xc008fa7b
		"fcvt.ps.pw f27, f17, dyn\n"                          // opcode=0xd008fdfb
		"fmsub.ps f0, f11, f14, f14, dyn\n"                   // opcode=0x72e5f05b
		"maskpopc x24, m1\n"                                  // opcode=0x52008c7b
		"fle.pi f22, f10, f14\n"                              // opcode=0xa6e50b7b
		"fsub.pi f25, f19, f15\n"                             // opcode=0x0ef98cfb
		"for.pi f27, f11, f15\n"                              // opcode=0x06f5edfb
		"fsll.pi f30, f12, f19\n"                             // opcode=0x07361f7b
		"fltm.pi m3, f12, f18\n"                              // opcode=0x3f2601fb
		"fsgnjn.ps f31, f17, f15\n"                           // opcode=0x20f89ffb


		"fandi.pi f0, f12, 333\n"                             // opcode=0xa5b6103f
		"fclass.ps f29, f17\n"                                // opcode=0xe0089efb
		"fnmsub.ps f22, f11, f19, f17, dyn\n"                 // opcode=0x8d35fb5b
		"fslli.pi f23, f19, 8\n"                              // opcode=0x4f199bfb
		"cubeface.ps f23, f17, f18\n"                         // opcode=0x89288bfb
		"fsrai.pi f21, f15, 14\n"                             // opcode=0x4fc7fafb
		"frcp.ps f26, f13\n"                                  // opcode=0x58768d7b
		"cubesgnsc.ps f0, f15, f14\n"                         // opcode=0x88e7a07b
		"fmv.x.s x29, f15\n"                                  // opcode=0xe0078ed3
		"fcmovm.ps f31, f11, f18\n"                           // opcode=0x01258ff7
		"fmsub.ps f30, f13, f12, f15, dyn\n"                  // opcode=0x7ac6ff5b
		"fnot.pi f25, f10\n"                                  // opcode=0x06052cfb
		"maskxor m0, m0, m7\n"                                // opcode=0x6670407b
		"fcvt.un24.ps f26, f15\n"                             // opcode=0xd9078d7b
		"fcmovm.ps f25, f15, f18\n"                           // opcode=0x01278cf7
		"fnmsub.s f28, f18, f18, f11, dyn\n"                  // opcode=0x59297e4b


		"fswizz.ps f24, f13, 29\n"                            // opcode=0xe676bc7b
		"for.pi f31, f19, f14\n"                              // opcode=0x06e9effb


		"fle.ps f25, f11, f17\n"                              // opcode=0xa1158cfb
		"mova.x.m x24\n"                                      // opcode=0xd6000c7b
		"fbci.ps f0, 132301\n"                                // opcode=0x4099b01f
		"feq.pi f20, f10, f19\n"                              // opcode=0xa7352a7b
		"fmax.ps f28, f13, f14\n"                             // opcode=0x28e69e7b
		"fcvt.un2.ps f28, f19\n"                              // opcode=0xd9798e7b

		 
		"fence\n"



		"feqm.ps m4, f18, f12\n"                              // opcode=0xa0c9627b
		"fsrl.pi f25, f13, f12\n"                             // opcode=0x06c6dcfb
		"fsetm.pi m4, f12\n"                                  // opcode=0xe006527b
		"fltm.ps m3, f12, f18\n"                              // opcode=0xa12651fb
		"maskxor m1, m2, m7\n"                                // opcode=0x667140fb
		"fadd.s f26, f18, f15, dyn\n"                         // opcode=0x00f97d53
		"maskxor m0, m3, m1\n"                                // opcode=0x6611c07b
		"fcvt.w.s x23, f19, dyn\n"                            // opcode=0xc009fbd3
		"fcvt.ps.f11 f23, f12\n"                              // opcode=0xd0960bfb
		"fle.ps f23, f16, f16\n"                              // opcode=0xa1080bfb
		"fsrli.pi f29, f19, 7\n"                              // opcode=0x4ee9defb

		"li x5, 0xced0f083f3c7b9ff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmsub.ps f28, f16, f13, f12, dyn\n"                  // opcode=0x62d87e5b
		"fslli.pi f24, f18, 12\n"                             // opcode=0x4f991c7b
		"fmax.ps f20, f14, f10\n"                             // opcode=0x28a71a7b
		"maskand m6, m0, m1\n"                                // opcode=0x6610737b
		"fsgnj.s f28, f11, f12\n"                             // opcode=0x20c58e53


		"fmadd.ps f23, f13, f18, f17, dyn\n"                  // opcode=0x8926fbdb


		"fmax.ps f0, f13, f12\n"                              // opcode=0x28c6907b
		"fnmadd.s f29, f12, f12, f15, dyn\n"                  // opcode=0x78c67ecf
		"fnmadd.ps f23, f13, f19, f13, dyn\n"                 // opcode=0x6f36fbdb
		"fmulhu.pi f28, f16, f10\n"                           // opcode=0x16a81e7b
		"fcvt.f16.ps f21, f10\n"                              // opcode=0xd8950afb
		"fsgnjx.s f29, f18, f15\n"                            // opcode=0x20f92ed3
		"fmax.pi f20, f14, f19\n"                             // opcode=0x2f371a7b
		"fsgnjx.s f22, f18, f11\n"                            // opcode=0x20b92b53
		"fslli.pi f21, f14, 10\n"                             // opcode=0x4f571afb
		"fltu.pi f21, f14, f19\n"                             // opcode=0xa7373afb
		"fclass.s x24, f11\n"                                 // opcode=0xe0059c53
		"fmv.x.s x25, f10\n"                                  // opcode=0xe0050cd3
		"ffrc.ps f27, f14\n"                                  // opcode=0x58270dfb




		"flog.ps f25, f13\n"                                  // opcode=0x58368cfb


		"fsll.pi f24, f17, f12\n"                             // opcode=0x06c89c7b
		"fgb.ps f28, f0(x15)\n"                               // opcode=0x48f01e0b
		"mova.m.x x15\n"                                      // opcode=0xd607907b
		"masknot m1, m5\n"                                    // opcode=0x6602a0fb
		"fcvt.f11.ps f29, f13\n"                              // opcode=0xd8868efb
		"fbc.ps f29, -1391(x12)\n"                            // opcode=0xa9160e8b
		"fcvt.ps.f10 f25, f11\n"                              // opcode=0xd0858cfb
		//RTLMIN-5479"fsin.ps f23, f18\n"                                  // opcode=0x58690bfb
		"fsgnjx.s f24, f12, f10\n"                            // opcode=0x20a62c53
		"flw f28, 998(x11)\n"                                 // opcode=0x3e65ae07
		"cubeface.ps f26, f11, f14\n"                         // opcode=0x88e58d7b
		"feqm.ps m0, f10, f17\n"                              // opcode=0xa115607b
		"fswizz.ps f20, f14, 0\n"                             // opcode=0xe6071a7b
		"maskpopcz x0, m5\n"                                  // opcode=0x5402807b






		"flog.ps f22, f15\n"                                  // opcode=0x58378b7b
		"fltm.pi m6, f15, f11\n"                              // opcode=0x3eb7837b


		"fsatu8.pi f27, f10\n"                                // opcode=0x06153dfb
		"fnmadd.s f0, f19, f11, f15, dyn\n"                   // opcode=0x78b9f04f


		"fcvt.un24.ps f24, f16\n"                             // opcode=0xd9080c7b
		"fmulhu.pi f22, f18, f18\n"                           // opcode=0x17291b7b
		"fnmadd.s f26, f11, f18, f10, dyn\n"                  // opcode=0x5125fd4f
		"fmaxu.pi f31, f13, f11\n"                            // opcode=0x2eb6bffb
		"for.pi f24, f13, f16\n"                              // opcode=0x0706ec7b
		"fsgnjn.ps f20, f17, f19\n"                           // opcode=0x21389a7b
		"fmv.x.s x28, f12\n"                                  // opcode=0xe0060e53
		"fmaxu.pi f22, f15, f13\n"                            // opcode=0x2ed7bb7b
		"fcvt.f16.ps f22, f15\n"                              // opcode=0xd8978b7b
		"fsgnjn.s f29, f13, f12\n"                            // opcode=0x20c69ed3


		"fsll.pi f30, f15, f18\n"                             // opcode=0x07279f7b

		 
		"fence\n"

		"fxor.pi f26, f12, f13\n"                             // opcode=0x06d64d7b

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"maskxor m3, m2, m3\n"                                // opcode=0x663141fb
		"fnmadd.ps f30, f10, f11, f13, dyn\n"                 // opcode=0x6eb57f5b
		"cubesgntc.ps f21, f18, f18\n"                        // opcode=0x89293afb
		//RTLMIN-5479"frsq.ps f27, f18\n"                                  // opcode=0x58890dfb
		"fsw.ps f16, -719(x13)\n"                             // opcode=0xd306e88b
		"fsll.pi f28, f18, f11\n"                             // opcode=0x06b91e7b
		"feq.s x20, f11, f18\n"                               // opcode=0xa125aa53
		"fcmovm.ps f27, f17, f11\n"                           // opcode=0x00b88df7
		"fmin.pi f30, f10, f15\n"                             // opcode=0x2ef50f7b
		"fround.ps f29, f13, dyn\n"                           // opcode=0x5816fefb
		"fslli.pi f22, f15, 12\n"                             // opcode=0x4f879b7b
		"fnmadd.s f30, f12, f14, f17, dyn\n"                  // opcode=0x88e67f4f
		"fle.s x23, f13, f15\n"                               // opcode=0xa0f68bd3
		"fnmadd.ps f21, f12, f10, f12, dyn\n"                 // opcode=0x66a67adb
		"fmax.ps f20, f16, f10\n"                             // opcode=0x28a81a7b
		"maskpopc x31, m3\n"                                  // opcode=0x52018ffb
		"flem.ps m3, f18, f10\n"                              // opcode=0xa0a941fb
		"fcvt.ps.pwu f21, f16, dyn\n"                         // opcode=0xd0187afb
		"fcvt.un24.ps f30, f10\n"                             // opcode=0xd9050f7b
		"fclass.s x0, f10\n"                                  // opcode=0xe0051053


		"fbcx.ps f31, x19\n"                                  // opcode=0x0009bf8b
		"fsub.s f22, f18, f18, dyn\n"                         // opcode=0x09297b53
		"fgw.ps f28, f1(x19)\n"                               // opcode=0x61309e0b
		"fmulhu.pi f25, f11, f12\n"                           // opcode=0x16c59cfb
		"cubesgntc.ps f22, f14, f18\n"                        // opcode=0x89273b7b
		"fnot.pi f24, f11\n"                                  // opcode=0x0605ac7b
		"fsub.pi f30, f14, f10\n"                             // opcode=0x0ea70f7b
		"fcvt.un10.ps f22, f12\n"                             // opcode=0xd9260b7b
		"fadd.ps f21, f17, f12, dyn\n"                        // opcode=0x00c8fafb
		"fcvt.un8.ps f30, f12\n"                              // opcode=0xd9360f7b
		"fnot.pi f29, f11\n"                                  // opcode=0x0605aefb
		"ffrc.ps f21, f17\n"                                  // opcode=0x58288afb
		"fsetm.pi m7, f17\n"                                  // opcode=0xe008d3fb
		"fsw.ps f15, -1568(x12)\n"                            // opcode=0x9ef6600b
		"fsatu8.pi f28, f18\n"                                // opcode=0x06193e7b
		"fsub.pi f30, f17, f14\n"                             // opcode=0x0ee88f7b
		"fclass.s x30, f12\n"                                 // opcode=0xe0061f53
		"fexp.ps f28, f14\n"                                  // opcode=0x58470e7b
		"fbci.pi f31, 5763\n"                                 // opcode=0x02d07fdf
		"fle.ps f23, f15, f13\n"                              // opcode=0xa0d78bfb
		"fcvt.un24.ps f21, f14\n"                             // opcode=0xd9070afb
		"fsw f12, 1566(x10)\n"                                // opcode=0x60c52f27
		"fbci.pi f30, 293990\n"                               // opcode=0x8f8cdf5f
		"fminu.pi f20, f11, f10\n"                            // opcode=0x2ea5aa7b


		"feq.pi f30, f12, f19\n"                              // opcode=0xa7362f7b
		"fxor.pi f26, f14, f16\n"                             // opcode=0x07074d7b
		"maskor m4, m6, m1\n"                                 // opcode=0x6613627b
		"fle.pi f25, f19, f19\n"                              // opcode=0xa7398cfb
		"fcvt.f11.ps f0, f16\n"                               // opcode=0xd888007b
		"fsub.ps f28, f12, f14, dyn\n"                        // opcode=0x08e67e7b

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flt.s x20, f10, f10\n"                               // opcode=0xa0a51a53
		"cubesgnsc.ps f23, f18, f19\n"                        // opcode=0x89392bfb
		"fmadd.s f27, f16, f17, f11, dyn\n"                   // opcode=0x59187dc3




		"fnmsub.ps f0, f19, f16, f18, dyn\n"                  // opcode=0x9509f05b
		"fcvt.ps.f10 f23, f19\n"                              // opcode=0xd0898bfb
		"fpackreph.pi f20, f11\n"                             // opcode=0x26059a7b


		"fmvs.x.ps x26, f12, 5\n"                             // opcode=0xe0562d7b
		"flem.ps m6, f10, f11\n"                              // opcode=0xa0b5437b
		"fsra.pi f24, f10, f12\n"                             // opcode=0x0ec55c7b
		"fcvt.un8.ps f26, f10\n"                              // opcode=0xd9350d7b
		"fsgnjx.s f28, f18, f10\n"                            // opcode=0x20a92e53
		"fsrai.pi f22, f12, 11\n"                             // opcode=0x4f667b7b
		"feq.s x23, f19, f16\n"                               // opcode=0xa109abd3
		"mov.m.x m3, x12, 2\n"                                // opcode=0x560641fb
		"fcvt.ps.f11 f25, f12\n"                              // opcode=0xd0960cfb
		"fpackreph.pi f24, f14\n"                             // opcode=0x26071c7b
		"fmadd.ps f29, f12, f11, f12, dyn\n"                  // opcode=0x60b67edb
		"fsub.s f27, f11, f10, dyn\n"                         // opcode=0x08a5fdd3
		"fpackrepb.pi f29, f10\n"                             // opcode=0x26050efb
		"feq.pi f29, f18, f19\n"                              // opcode=0xa7392efb
		"fsatu8.pi f22, f12\n"                                // opcode=0x06163b7b


		"fcvt.sn16.ps f21, f14\n"                             // opcode=0xd9970afb
		"fsub.s f25, f15, f13, dyn\n"                         // opcode=0x08d7fcd3
		"fmin.ps f27, f16, f13\n"                             // opcode=0x28d80dfb
		"fsrli.pi f29, f12, 3\n"                              // opcode=0x4e765efb
		"maskpopc x0, m3\n"                                   // opcode=0x5201807b
		"fgw.ps f29, f1(x11)\n"                               // opcode=0x60b09e8b


		"fmax.pi f28, f15, f15\n"                             // opcode=0x2ef79e7b
		"fadd.s f31, f16, f18, dyn\n"                         // opcode=0x01287fd3
		"fcvt.ps.f10 f21, f10\n"                              // opcode=0xd0850afb
		"ffrc.ps f28, f16\n"                                  // opcode=0x58280e7b
		"fsrl.pi f27, f15, f15\n"                             // opcode=0x06f7ddfb
		"fmax.ps f20, f11, f11\n"                             // opcode=0x28b59a7b
		"fltm.pi m5, f18, f19\n"                              // opcode=0x3f3902fb
		"maskxor m4, m6, m6\n"                                // opcode=0x6663427b
		"maskpopc x0, m7\n"                                   // opcode=0x5203807b
		"cubeface.ps f24, f13, f11\n"                         // opcode=0x88b68c7b
		"fadd.pi f25, f11, f16\n"                             // opcode=0x07058cfb
		"cubeface.ps f23, f16, f13\n"                         // opcode=0x88d80bfb
		"flt.s x23, f12, f12\n"                               // opcode=0xa0c61bd3
		"fpackrepb.pi f25, f15\n"                             // opcode=0x26078cfb
		"faddi.pi f28, f18, 164\n"                            // opcode=0x54990e3f
		"fnmadd.ps f25, f10, f16, f16, dyn\n"                 // opcode=0x87057cdb
		"fcvt.sn16.ps f27, f12\n"                             // opcode=0xd9960dfb

		 
		"fence\n"

		"fsra.pi f27, f15, f17\n"                             // opcode=0x0f17ddfb
		"fsgnj.s f24, f18, f15\n"                             // opcode=0x20f90c53
		"fsub.pi f29, f19, f13\n"                             // opcode=0x0ed98efb
		"flt.pi f22, f12, f16\n"                              // opcode=0xa7061b7b
		"fsw f18, -1706(x15)\n"                               // opcode=0x9527ab27

		 
		"fence\n"

		"fnot.pi f28, f10\n"                                  // opcode=0x06052e7b

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.un8.ps f28, f13\n"                              // opcode=0xd9368e7b
		"fbci.pi f29, 445446\n"                               // opcode=0xd980dedf
		"fcvt.un8.ps f27, f14\n"                              // opcode=0xd9370dfb
		"fsgnjx.ps f23, f13, f13\n"                           // opcode=0x20d6abfb
		"fmsub.s f27, f19, f18, f19, dyn\n"                   // opcode=0x9929fdc7


		"fscw.ps f15, f0(x19)\n"                              // opcode=0xe130178b
		"fcmovm.ps f30, f17, f14\n"                           // opcode=0x00e88f77
		"fmv.s.x f25, x10\n"                                  // opcode=0xf0050cd3
		"fsetm.pi m1, f13\n"                                  // opcode=0xe006d0fb
		"feq.s x25, f19, f17\n"                               // opcode=0xa119acd3
		"flt.ps f21, f19, f19\n"                              // opcode=0xa1399afb
		"feq.s x0, f13, f14\n"                                // opcode=0xa0e6a053
		"cubefaceidx.ps f26, f12, f13\n"                      // opcode=0x88d61d7b
		"fcvt.f10.ps f28, f10\n"                              // opcode=0xd8b50e7b


		"fmv.s.x f27, x13\n"                                  // opcode=0xf0068dd3
		"fcvt.pwu.ps f29, f17, dyn\n"                         // opcode=0xc018fefb
		"fcvt.ps.f10 f24, f14\n"                              // opcode=0xd0870c7b
		"fsatu8.pi f20, f15\n"                                // opcode=0x0617ba7b
		"fnmsub.s f25, f17, f13, f16, rdn\n"                  // opcode=0x80d8accb
		"fminu.pi f30, f10, f15\n"                            // opcode=0x2ef52f7b
		"fbc.ps f31, -79(x14)\n"                              // opcode=0xfb170f8b
		"fadd.s f20, f11, f12, dyn\n"                         // opcode=0x00c5fa53
		"fsw f17, 692(x10)\n"                                 // opcode=0x2b152a27
		"mova.m.x x12\n"                                      // opcode=0xd606107b
		"fsrli.pi f26, f15, 13\n"                             // opcode=0x4fb7dd7b
		"fclass.s x24, f12\n"                                 // opcode=0xe0061c53
		"fmv.x.s x28, f19\n"                                  // opcode=0xe0098e53
		"fmadd.ps f21, f16, f10, f18, dyn\n"                  // opcode=0x90a87adb
		"fscw.ps f13, f0(x10)\n"                              // opcode=0xe0a0168b
		"feq.ps f23, f14, f15\n"                              // opcode=0xa0f72bfb


		"fadd.pi f27, f18, f18\n"                             // opcode=0x07290dfb


		"flq2 f28, 0 (x11)\n"                                 // opcode=0xba45de07
		"fmulhu.pi f26, f14, f16\n"                           // opcode=0x17071d7b
		"fand.pi f30, f16, f18\n"                             // opcode=0x07287f7b
		"flw.ps f22, 676(x10)\n"                              // opcode=0x2a452b0b
		"fminu.pi f28, f19, f14\n"                            // opcode=0x2ee9ae7b


		"fand.pi f22, f18, f18\n"                             // opcode=0x07297b7b
		"maskxor m2, m2, m0\n"                                // opcode=0x6601417b
		"fmin.ps f30, f10, f10\n"                             // opcode=0x28a50f7b
		"masknot m7, m1\n"                                    // opcode=0x6600a3fb


		"flq2 f30, 0 (x16)\n"                                 // opcode=0x1d585f07
		"fnmsub.ps f29, f14, f13, f17, dyn\n"                 // opcode=0x8cd77edb
		"cubesgntc.ps f28, f19, f16\n"                        // opcode=0x8909be7b
		"fsgnjx.ps f23, f19, f18\n"                           // opcode=0x2129abfb
		"fcvt.f11.ps f24, f12\n"                              // opcode=0xd8860c7b


		"fcvt.sn16.ps f26, f18\n"                             // opcode=0xd9990d7b
		"fmul.pi f25, f16, f18\n"                             // opcode=0x17280cfb
		"maskand m3, m1, m6\n"                                // opcode=0x6660f1fb
		"maskxor m3, m0, m0\n"                                // opcode=0x660041fb
		"fsrl.pi f28, f15, f17\n"                             // opcode=0x0717de7b

		"li x5, 0x6ff169ed8446ccbf\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsgnjx.ps f22, f16, f17\n"                           // opcode=0x21182b7b
		"fmadd.ps f26, f15, f12, f13, dyn\n"                  // opcode=0x68c7fd5b
		"fsll.pi f21, f15, f19\n"                             // opcode=0x07379afb
		"fltm.ps m7, f12, f16\n"                              // opcode=0xa10653fb
		"fsgnjx.s f20, f11, f16\n"                            // opcode=0x2105aa53
		"cubefaceidx.ps f26, f19, f19\n"                      // opcode=0x89399d7b
		"fcmov.ps f24, f16, f12, f13\n"                       // opcode=0x6cc82c3f
		"fcvt.ps.pw f20, f10, dyn\n"                          // opcode=0xd0057a7b
		"frcp.ps f24, f18\n"                                  // opcode=0x58790c7b
		"fcmov.ps f31, f12, f15, f15\n"                       // opcode=0x7cf62fbf
		"fbci.pi f30, 471966\n"                               // opcode=0xe673cf5f
		"fcvt.f10.ps f29, f11\n"                              // opcode=0xd8b58efb




		"fsub.pi f28, f19, f16\n"                             // opcode=0x0f098e7b
		"fxor.pi f31, f12, f17\n"                             // opcode=0x07164ffb
		"fcvt.un24.ps f26, f13\n"                             // opcode=0xd9068d7b
		"fbci.pi f29, 160494\n"                               // opcode=0x4e5ddedf


		"fmin.s f30, f14, f12\n"                              // opcode=0x28c70f53
		"maskand m0, m7, m0\n"                                // opcode=0x6603f07b
		"flt.s x30, f10, f18\n"                               // opcode=0xa1251f53
		"mova.m.x x19\n"                                      // opcode=0xd609907b
		"fand.pi f29, f16, f11\n"                             // opcode=0x06b87efb
		"fcvt.ps.f10 f25, f18\n"                              // opcode=0xd0890cfb
		"fmin.ps f26, f16, f10\n"                             // opcode=0x28a80d7b
		"fnmsub.s f30, f14, f14, f18, dyn\n"                  // opcode=0x90e77f4b
		"fsat8.pi f27, f13\n"                                 // opcode=0x0606bdfb


		"flw.ps f31, -261(x18)\n"                             // opcode=0xefb92f8b
		"fminu.pi f23, f13, f19\n"                            // opcode=0x2f36abfb
		"feq.s x29, f19, f15\n"                               // opcode=0xa0f9aed3
		"fcvt.w.s x23, f14, dyn\n"                            // opcode=0xc0077bd3


		"fmul.ps f30, f11, f12, dyn\n"                        // opcode=0x10c5ff7b
		"maskpopcz x0, m4\n"                                  // opcode=0x5402007b
		"fcvt.wu.s x22, f16, dyn\n"                           // opcode=0xc0187b53
		"fsgnjx.ps f26, f16, f10\n"                           // opcode=0x20a82d7b


		"flog.ps f0, f11\n"                                   // opcode=0x5835807b


		"flq2 f30, 0 (x12)\n"                                 // opcode=0xac165f07
		"fle.ps f23, f15, f13\n"                              // opcode=0xa0d78bfb
		"fmsub.ps f23, f18, f12, f18, dyn\n"                  // opcode=0x92c97bdb
		"fnmsub.ps f0, f16, f10, f18, dyn\n"                  // opcode=0x94a8705b
		"ffrc.ps f29, f14\n"                                  // opcode=0x58270efb
		"maskpopcz x22, m3\n"                                 // opcode=0x54018b7b
		"flt.ps f29, f13, f17\n"                              // opcode=0xa1169efb
		"fmadd.s f20, f19, f17, f17, dyn\n"                   // opcode=0x8919fa43
		"fcvt.f10.ps f29, f14\n"                              // opcode=0xd8b70efb
		"fclass.s x30, f17\n"                                 // opcode=0xe0089f53
		"fcvt.ps.pw f27, f10, dyn\n"                          // opcode=0xd0057dfb
		"fcvt.wu.s x21, f13, dyn\n"                           // opcode=0xc016fad3
		"fswizz.ps f26, f19, 60\n"                            // opcode=0xe6f99d7b
		//RTLMIN-5479"fsin.ps f22, f15\n"                                  // opcode=0x58678b7b


		"fcvt.pwu.ps f0, f18, dyn\n"                          // opcode=0xc019707b

		 
		"fence\n"

		"fcvt.un16.ps f0, f11\n"                              // opcode=0xd915807b

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flt.ps f29, f17, f18\n"                              // opcode=0xa1289efb
		"fsub.pi f29, f17, f15\n"                             // opcode=0x0ef88efb
		"fmadd.s f25, f14, f15, f11, dyn\n"                   // opcode=0x58f77cc3


		"flq2 f20, 0 (x11)\n"                                 // opcode=0x1265da07
		"fcvt.un2.ps f23, f10\n"                              // opcode=0xd9750bfb
		"fsch.ps f12, f1(x12)\n"                              // opcode=0xd0c0960b
		"fmsub.ps f21, f10, f17, f16, dyn\n"                  // opcode=0x83157adb
		"flw f0, -2032(x11)\n"                                // opcode=0x8105a007


		"fsrl.pi f23, f11, f12\n"                             // opcode=0x06c5dbfb
		"fcvt.ps.f16 f24, f18\n"                              // opcode=0xd0a90c7b
		"fmv.s.x f27, x14\n"                                  // opcode=0xf0070dd3


		"fsub.pi f29, f15, f14\n"                             // opcode=0x0ee78efb
		"fpackreph.pi f29, f18\n"                             // opcode=0x26091efb
		"feq.s x29, f15, f10\n"                               // opcode=0xa0a7aed3
		"maskxor m5, m4, m7\n"                                // opcode=0x667242fb


		"fmax.ps f20, f16, f15\n"                             // opcode=0x28f81a7b
		"fgh.ps f23, f0(x18)\n"                               // opcode=0x51201b8b
		"maskpopcz x29, m0\n"                                 // opcode=0x54000efb
		"flem.ps m6, f12, f19\n"                              // opcode=0xa136437b
		"fmul.pi f27, f14, f19\n"                             // opcode=0x17370dfb
		"fswizz.ps f20, f17, 53\n"                            // opcode=0xe6d8aa7b
		"fadd.pi f25, f19, f15\n"                             // opcode=0x06f98cfb
		"mova.m.x x19\n"                                      // opcode=0xd609907b
		"flt.ps f31, f13, f11\n"                              // opcode=0xa0b69ffb
		"fsgnj.s f24, f11, f10\n"                             // opcode=0x20a58c53
		"fsat8.pi f29, f14\n"                                 // opcode=0x06073efb
		"fslli.pi f21, f13, 0\n"                              // opcode=0x4e169afb
		"fsub.ps f21, f10, f10, dyn\n"                        // opcode=0x08a57afb
		"maskand m5, m5, m6\n"                                // opcode=0x6662f2fb
		"fcvt.sn16.ps f29, f18\n"                             // opcode=0xd9990efb
		"flt.s x28, f16, f11\n"                               // opcode=0xa0b81e53
		"fmsub.ps f26, f19, f17, f11, dyn\n"                  // opcode=0x5b19fd5b
		"fltu.pi f31, f11, f18\n"                             // opcode=0xa725bffb
		"fcvt.ps.pwu f31, f12, dyn\n"                         // opcode=0xd0167ffb
		"feq.ps f26, f10, f10\n"                              // opcode=0xa0a52d7b
		"fmax.s f26, f19, f13\n"                              // opcode=0x28d99d53
		"feq.s x24, f12, f13\n"                               // opcode=0xa0d62c53
		"fcvt.un2.ps f21, f19\n"                              // opcode=0xd9798afb
		"fle.pi f24, f12, f10\n"                              // opcode=0xa6a60c7b
		"mova.m.x x11\n"                                      // opcode=0xd605907b
		"fcvt.ps.pw f0, f19, dyn\n"                           // opcode=0xd009f07b
		"fsgnj.ps f30, f13, f13\n"                            // opcode=0x20d68f7b
		"fcvt.un16.ps f29, f14\n"                             // opcode=0xd9170efb
		"fswizz.ps f29, f13, 100\n"                           // opcode=0xe7968efb
		"fadd.s f27, f19, f12, dyn\n"                         // opcode=0x00c9fdd3
		"fcvt.sn8.ps f20, f16\n"                              // opcode=0xd9b80a7b
		"fnmsub.s f31, f15, f16, f16, dyn\n"                  // opcode=0x8107ffcb


		"fmul.ps f31, f14, f15, dyn\n"                        // opcode=0x10f77ffb
		"fmax.ps f28, f14, f15\n"                             // opcode=0x28f71e7b

		 
		"fence\n"



		"flq2 f31, 0 (x15)\n"                                 // opcode=0xf087df87

		"li x5, 0x689a3e5e36da8bf8\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fbc.ps f25, 1491(x18)\n"                             // opcode=0x5d390c8b
		"maskor m7, m2, m6\n"                                 // opcode=0x666163fb
		"cubesgntc.ps f31, f14, f10\n"                        // opcode=0x88a73ffb
		"fcvt.f11.ps f23, f13\n"                              // opcode=0xd8868bfb
		"fcvt.un24.ps f31, f13\n"                             // opcode=0xd9068ffb
		"fcvt.f16.ps f29, f16\n"                              // opcode=0xd8980efb


		"fsw.ps f17, -29(x11)\n"                              // opcode=0xff15e18b
		"fsgnjx.ps f23, f15, f15\n"                           // opcode=0x20f7abfb
		"flw.ps f21, -1629(x12)\n"                            // opcode=0x9a362a8b
		"fmax.s f31, f14, f11\n"                              // opcode=0x28b71fd3
		"fcvt.f10.ps f23, f16\n"                              // opcode=0xd8b80bfb
		"fround.ps f28, f13, dyn\n"                           // opcode=0x5816fe7b


		"fadd.s f21, f19, f10, dyn\n"                         // opcode=0x00a9fad3
		"cubefaceidx.ps f0, f14, f13\n"                       // opcode=0x88d7107b


		"flw f20, 581(x15)\n"                                 // opcode=0x2457aa07
		"fcvt.f16.ps f21, f11\n"                              // opcode=0xd8958afb


		"fle.s x30, f18, f19\n"                               // opcode=0xa1390f53
		"fmadd.s f28, f13, f15, f13, dyn\n"                   // opcode=0x68f6fe43
		"feqm.ps m7, f16, f12\n"                              // opcode=0xa0c863fb
		"mov.m.x m5, x12, 49\n"                               // opcode=0x56c622fb
		"fsat8.pi f26, f10\n"                                 // opcode=0x06053d7b
		"fsrl.pi f24, f12, f17\n"                             // opcode=0x07165c7b
		"fsch.ps f10, f1(x19)\n"                              // opcode=0xd130950b
		"fcvt.f16.ps f30, f14\n"                              // opcode=0xd8970f7b
		//RTLMIN-5479"fsin.ps f29, f14\n"                                  // opcode=0x58670efb
		"fle.s x31, f12, f14\n"                               // opcode=0xa0e60fd3


		"flq2 f29, 0 (x17)\n"                                 // opcode=0x8fb8de87
		"fcvt.un10.ps f24, f13\n"                             // opcode=0xd9268c7b
		"maskor m7, m6, m0\n"                                 // opcode=0x660363fb
		"fcvt.s.wu f25, x17, dyn\n"                           // opcode=0xd018fcd3
		"fltm.ps m4, f17, f17\n"                              // opcode=0xa118d27b
		"fand.pi f28, f17, f16\n"                             // opcode=0x0708fe7b
		"fsat8.pi f20, f11\n"                                 // opcode=0x0605ba7b


		"fcmov.ps f26, f16, f10, f12\n"                       // opcode=0x64a82d3f
		"fsub.s f31, f17, f13, dyn\n"                         // opcode=0x08d8ffd3
		"cubefaceidx.ps f22, f13, f11\n"                      // opcode=0x88b69b7b
		"fsll.pi f25, f18, f13\n"                             // opcode=0x06d91cfb
		"fadd.s f24, f14, f15, dyn\n"                         // opcode=0x00f77c53
		"fbci.pi f25, 459843\n"                               // opcode=0xe0886cdf
		"fmadd.ps f23, f17, f12, f17, dyn\n"                  // opcode=0x88c8fbdb


		"fmax.ps f0, f18, f12\n"                              // opcode=0x28c9107b
		"mov.m.x m6, x10, 78\n"                               // opcode=0x5735537b


		"fmulhu.pi f28, f18, f11\n"                           // opcode=0x16b91e7b
		"fscw.ps f13, f1(x10)\n"                              // opcode=0xe0a0968b
		"fround.ps f25, f10, dyn\n"                           // opcode=0x58157cfb
		"fsat8.pi f29, f14\n"                                 // opcode=0x06073efb
		"fsub.pi f29, f19, f17\n"                             // opcode=0x0f198efb
		"fclass.s x30, f17\n"                                 // opcode=0xe0089f53
		"fcvt.f11.ps f28, f10\n"                              // opcode=0xd8850e7b

		 
		"fence\n"

		"fsrli.pi f21, f15, 7\n"                              // opcode=0x4ef7dafb

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.sn16.ps f23, f19\n"                             // opcode=0xd9998bfb
		"fmsub.s f23, f19, f13, f12, dyn\n"                   // opcode=0x60d9fbc7
		"flw f24, -965(x17)\n"                                // opcode=0xc3b8ac07
		"maskxor m3, m0, m6\n"                                // opcode=0x666041fb
		"fnmadd.ps f31, f10, f16, f11, dyn\n"                 // opcode=0x5f057fdb
		"ffrc.ps f31, f19\n"                                  // opcode=0x58298ffb
		"feq.s x24, f18, f10\n"                               // opcode=0xa0a92c53
		"fmvs.x.ps x30, f19, 0\n"                             // opcode=0xe009af7b
		//RTLMIN-5479"fsin.ps f30, f11\n"                                  // opcode=0x58658f7b
		"cubesgntc.ps f24, f14, f18\n"                        // opcode=0x89273c7b
		"fsgnj.s f24, f15, f12\n"                             // opcode=0x20c78c53
		"fcvt.s.wu f23, x12, rdn\n"                           // opcode=0xd0162bd3
		"cubesgnsc.ps f0, f17, f10\n"                         // opcode=0x88a8a07b
		"fcvt.un2.ps f20, f15\n"                              // opcode=0xd9778a7b
		"fcmovm.ps f0, f12, f17\n"                            // opcode=0x01160077
		//RTLMIN-5479"fsin.ps f22, f16\n"                                  // opcode=0x58680b7b
		"fcvt.w.s x23, f12, dyn\n"                            // opcode=0xc0067bd3
		"fcvt.sn16.ps f25, f14\n"                             // opcode=0xd9970cfb
		"fcvt.pw.ps f28, f13, dyn\n"                          // opcode=0xc006fe7b
		"fcvt.s.w f20, x10, dyn\n"                            // opcode=0xd0057a53
		"fcvt.ps.pwu f27, f13, dyn\n"                         // opcode=0xd016fdfb
		"fcvt.ps.pwu f22, f14, dyn\n"                         // opcode=0xd0177b7b
		"maskand m5, m5, m3\n"                                // opcode=0x6632f2fb
		"flw.ps f30, 1789(x11)\n"                             // opcode=0x6fd5af0b
		"fmax.ps f26, f16, f17\n"                             // opcode=0x29181d7b
		"fcvt.ps.f16 f31, f14\n"                              // opcode=0xd0a70ffb
		"maskpopcz x31, m5\n"                                 // opcode=0x54028ffb
		"fcvt.s.w f21, x13, dyn\n"                            // opcode=0xd006fad3
		"fle.s x24, f12, f18\n"                               // opcode=0xa1260c53
		"fmadd.ps f29, f16, f18, f15, dyn\n"                  // opcode=0x79287edb
		"maskpopc x31, m5\n"                                  // opcode=0x52028ffb
		"fcvt.sn8.ps f27, f17\n"                              // opcode=0xd9b88dfb


		"fmul.s f29, f15, f14, dyn\n"                         // opcode=0x10e7fed3
		"fpackrepb.pi f28, f10\n"                             // opcode=0x26050e7b
		"fcvt.f10.ps f25, f15\n"                              // opcode=0xd8b78cfb
		"fnmadd.ps f31, f15, f16, f18, dyn\n"                 // opcode=0x9707ffdb
		"flt.ps f23, f14, f13\n"                              // opcode=0xa0d71bfb


		"mova.x.m x25\n"                                      // opcode=0xd6000cfb
		"mova.x.m x27\n"                                      // opcode=0xd6000dfb
		"mova.m.x x14\n"                                      // opcode=0xd607107b
		"fmax.pi f31, f19, f12\n"                             // opcode=0x2ec99ffb
		"fcvt.sn16.ps f28, f14\n"                             // opcode=0xd9970e7b
		"masknot m6, m6\n"                                    // opcode=0x6603237b
		"fnmadd.ps f29, f12, f13, f11, dyn\n"                 // opcode=0x5ed67edb
		"feq.s x0, f14, f18\n"                                // opcode=0xa1272053
		"fmax.ps f20, f18, f11\n"                             // opcode=0x28b91a7b
		"fcvt.wu.s x23, f16, dyn\n"                           // opcode=0xc0187bd3
		"ffrc.ps f26, f17\n"                                  // opcode=0x58288d7b
		"fsrli.pi f31, f14, 5\n"                              // opcode=0x4ea75ffb
		"fsatu8.pi f20, f12\n"                                // opcode=0x06163a7b

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.wu.s x20, f12, dyn\n"                           // opcode=0xc0167a53
		"fmax.pi f28, f14, f11\n"                             // opcode=0x2eb71e7b
		"fsw f17, -856(x19)\n"                                // opcode=0xcb19a427
		"maskxor m0, m1, m6\n"                                // opcode=0x6660c07b
		"fminu.pi f29, f14, f11\n"                            // opcode=0x2eb72efb
		"fsub.ps f29, f11, f19, dyn\n"                        // opcode=0x0935fefb


		"fandi.pi f21, f17, 51\n"                             // opcode=0x1c789abf
		"fpackrepb.pi f31, f18\n"                             // opcode=0x26090ffb
		"fmv.x.s x0, f10\n"                                   // opcode=0xe0050053
		"cubesgnsc.ps f25, f12, f12\n"                        // opcode=0x88c62cfb
		//RTLMIN-5479"fsin.ps f30, f16\n"                                  // opcode=0x58680f7b


		"fbcx.ps f21, x13\n"                                  // opcode=0x0006ba8b
		"maskand m4, m6, m3\n"                                // opcode=0x6633727b
		"cubeface.ps f28, f12, f10\n"                         // opcode=0x88a60e7b
		"fsw f10, 1156(x10)\n"                                // opcode=0x48a52227
		"fadd.ps f22, f14, f18, dyn\n"                        // opcode=0x01277b7b
		"fltm.pi m2, f15, f15\n"                              // opcode=0x3ef7817b
		"fbci.ps f31, 389353\n"                               // opcode=0xbe1d3f9f
		"frcp.ps f23, f16\n"                                  // opcode=0x58780bfb
		"fslli.pi f0, f14, 0\n"                               // opcode=0x4e07107b
		"fsrli.pi f21, f11, 6\n"                              // opcode=0x4ec5dafb
		"fsgnj.s f30, f15, f14\n"                             // opcode=0x20e78f53
		"fclass.s x22, f13\n"                                 // opcode=0xe0069b53
		"fltu.pi f22, f13, f17\n"                             // opcode=0xa716bb7b
		"fmv.s.x f21, x18\n"                                  // opcode=0xf0090ad3
		"fround.ps f23, f19, dyn\n"                           // opcode=0x5819fbfb


		"fpackreph.pi f23, f13\n"                             // opcode=0x26069bfb
		"fcvt.pw.ps f21, f10, dyn\n"                          // opcode=0xc0057afb
		"flem.ps m6, f19, f19\n"                              // opcode=0xa139c37b
		"fmaxu.pi f25, f17, f11\n"                            // opcode=0x2eb8bcfb
		"fmax.s f31, f18, f17\n"                              // opcode=0x29191fd3
		"fsll.pi f21, f11, f14\n"                             // opcode=0x06e59afb
		"ffrc.ps f0, f18\n"                                   // opcode=0x5829007b
		"fmv.s.x f30, x15\n"                                  // opcode=0xf0078f53
		"fnmsub.ps f22, f14, f19, f14, dyn\n"                 // opcode=0x75377b5b
		"fadd.pi f27, f18, f14\n"                             // opcode=0x06e90dfb
		"feqm.ps m3, f18, f18\n"                              // opcode=0xa12961fb
		"fpackrepb.pi f26, f13\n"                             // opcode=0x26068d7b
		"fcvt.sn8.ps f0, f13\n"                               // opcode=0xd9b6807b
		"fpackrepb.pi f28, f13\n"                             // opcode=0x26068e7b
		"for.pi f22, f16, f19\n"                              // opcode=0x07386b7b
		"flt.ps f26, f19, f18\n"                              // opcode=0xa1299d7b
		"fcmov.ps f0, f18, f18, f18\n"                        // opcode=0x9529203f


		"flog.ps f24, f14\n"                                  // opcode=0x58370c7b
		"fbci.ps f26, 86912\n"                                // opcode=0x2a700d1f
		"fmax.pi f31, f16, f18\n"                             // opcode=0x2f281ffb


		"flq2 f30, 0 (x11)\n"                                 // opcode=0xe8a5df07
		"fscw.ps f15, f1(x16)\n"                              // opcode=0xe100978b
		"fmadd.ps f0, f18, f19, f18, dyn\n"                   // opcode=0x9139705b

		 
		"fence\n"

		"frcp.ps f21, f18\n"                                  // opcode=0x58790afb

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flw.ps f28, 1978(x15)\n"                             // opcode=0x7ba7ae0b
		"masknot m4, m7\n"                                    // opcode=0x6603a27b
		"fclass.s x29, f18\n"                                 // opcode=0xe0091ed3
		"fsgnj.ps f26, f11, f13\n"                            // opcode=0x20d58d7b

		 
		"fence\n"

		"fsrli.pi f28, f16, 12\n"                             // opcode=0x4f885e7b
		"flem.ps m7, f14, f11\n"                              // opcode=0xa0b743fb
		"fsrl.pi f21, f18, f18\n"                             // opcode=0x07295afb
		"maskxor m3, m2, m2\n"                                // opcode=0x662141fb


		"fmin.ps f21, f19, f17\n"                             // opcode=0x29198afb
		"fcvt.un10.ps f28, f11\n"                             // opcode=0xd9258e7b
		"fclass.s x27, f14\n"                                 // opcode=0xe0071dd3
		"fslli.pi f29, f18, 9\n"                              // opcode=0x4f391efb
		"fxor.pi f0, f13, f10\n"                              // opcode=0x06a6c07b
		"fmul.s f24, f15, f17, dyn\n"                         // opcode=0x1117fc53
		"fand.pi f0, f15, f18\n"                              // opcode=0x0727f07b
		"fcmov.ps f20, f19, f19, f12\n"                       // opcode=0x6539aa3f
		"fmsub.s f26, f10, f11, f17, dyn\n"                   // opcode=0x88b57d47
		"frcp.ps f21, f11\n"                                  // opcode=0x58758afb
		"fbcx.ps f26, x14\n"                                  // opcode=0x00073d0b
		"fmin.pi f0, f15, f12\n"                              // opcode=0x2ec7807b
		"fsw f16, -78(x10)\n"                                 // opcode=0xfb052927
		"fnmadd.s f24, f19, f15, f17, dyn\n"                  // opcode=0x88f9fc4f
		"fmv.x.s x21, f19\n"                                  // opcode=0xe0098ad3
		"maskpopc x26, m2\n"                                  // opcode=0x52010d7b
		"fmulh.pi f21, f17, f19\n"                            // opcode=0x1738aafb
		"flw f0, 560(x16)\n"                                  // opcode=0x23082007


		"fxor.pi f26, f14, f14\n"                             // opcode=0x06e74d7b
		"fnmadd.ps f27, f11, f17, f13, dyn\n"                 // opcode=0x6f15fddb
		"fcvt.s.wu f26, x11, dyn\n"                           // opcode=0xd015fd53
		"fxor.pi f30, f13, f15\n"                             // opcode=0x06f6cf7b


		"fcvt.ps.f10 f28, f17\n"                              // opcode=0xd0888e7b
		"fmax.ps f28, f14, f15\n"                             // opcode=0x28f71e7b
		"mova.m.x x18\n"                                      // opcode=0xd609107b
		"fpackrepb.pi f26, f18\n"                             // opcode=0x26090d7b
		"fclass.s x28, f16\n"                                 // opcode=0xe0081e53
		"cubesgntc.ps f25, f10, f11\n"                        // opcode=0x88b53cfb
		"fminu.pi f22, f17, f17\n"                            // opcode=0x2f18ab7b
		"fsch.ps f10, f1(x16)\n"                              // opcode=0xd100950b
		"fsra.pi f0, f18, f16\n"                              // opcode=0x0f09507b


		"fmsub.ps f29, f11, f14, f12, dyn\n"                  // opcode=0x62e5fedb
		"fswizz.ps f30, f13, 62\n"                            // opcode=0xe6f6df7b
		"maskxor m7, m1, m0\n"                                // opcode=0x6600c3fb


		"flq2 f26, 0 (x14)\n"                                 // opcode=0xa5d75d07
		"fpackreph.pi f31, f19\n"                             // opcode=0x26099ffb


		"fmul.s f0, f16, f13, dyn\n"                          // opcode=0x10d87053
		"fround.ps f29, f10, dyn\n"                           // opcode=0x58157efb
		"fcvt.f16.ps f21, f10\n"                              // opcode=0xd8950afb
		"fcmovm.ps f27, f19, f18\n"                           // opcode=0x01298df7
		"fle.s x23, f16, f15\n"                               // opcode=0xa0f80bd3
		"fcvt.pw.ps f22, f13, dyn\n"                          // opcode=0xc006fb7b

		"li x5, 0xc549787c2bfbc221\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fsq2 f13, 0 (x17)\n"                                 // opcode=0x4ed8d227
		"fmulh.pi f30, f11, f10\n"                            // opcode=0x16a5af7b
		"masknot m1, m2\n"                                    // opcode=0x660120fb
		"fcvt.sn8.ps f28, f10\n"                              // opcode=0xd9b50e7b
		"maskpopc x25, m5\n"                                  // opcode=0x52028cfb
		"mov.m.x m1, x12, 58\n"                               // opcode=0x56e650fb
		"fexp.ps f30, f16\n"                                  // opcode=0x58480f7b
		"fswizz.ps f0, f13, 85\n"                             // opcode=0xe756a07b
		"fnmsub.ps f20, f17, f17, f12, rmm\n"                 // opcode=0x6518ca5b
		"feq.s x0, f15, f17\n"                                // opcode=0xa117a053
		"fsra.pi f31, f11, f12\n"                             // opcode=0x0ec5dffb
		"fcvt.sn16.ps f31, f14\n"                             // opcode=0xd9970ffb
		"fsgnjn.s f30, f12, f10\n"                            // opcode=0x20a61f53


		"for.pi f28, f10, f18\n"                              // opcode=0x07256e7b
		"fsub.ps f24, f19, f19, dyn\n"                        // opcode=0x0939fc7b
		"fcmovm.ps f0, f16, f12\n"                            // opcode=0x00c80077
		"fclass.ps f24, f13\n"                                // opcode=0xe0069c7b


		"fle.pi f31, f10, f12\n"                              // opcode=0xa6c50ffb
		"fbc.ps f25, 1908(x16)\n"                             // opcode=0x77480c8b
		"fmv.x.s x23, f15\n"                                  // opcode=0xe0078bd3
		"fround.ps f30, f16, dyn\n"                           // opcode=0x58187f7b
		"fmin.ps f24, f18, f18\n"                             // opcode=0x29290c7b
		"fmin.ps f0, f19, f15\n"                              // opcode=0x28f9807b
		"fmv.x.s x25, f19\n"                                  // opcode=0xe0098cd3
		"fcvt.f11.ps f24, f17\n"                              // opcode=0xd8888c7b


		"maskor m4, m4, m6\n"                                 // opcode=0x6662627b
		"fsgnjn.s f22, f12, f12\n"                            // opcode=0x20c61b53
		"fswizz.ps f20, f10, 2\n"                             // opcode=0xe6054a7b


		"fclass.ps f31, f10\n"                                // opcode=0xe0051ffb
		"cubefaceidx.ps f20, f11, f14\n"                      // opcode=0x88e59a7b
		"fmin.pi f24, f10, f13\n"                             // opcode=0x2ed50c7b
		"fmvz.x.ps x21, f13, 4\n"                             // opcode=0xe0468afb
		"flem.ps m5, f13, f18\n"                              // opcode=0xa126c2fb
		"fcvt.un2.ps f30, f16\n"                              // opcode=0xd9780f7b
		"fnmadd.s f23, f18, f19, f13, dyn\n"                  // opcode=0x69397bcf
		"fsatu8.pi f23, f19\n"                                // opcode=0x0619bbfb
		"cubefaceidx.ps f23, f16, f10\n"                      // opcode=0x88a81bfb
		"fsw.ps f16, 901(x19)\n"                              // opcode=0x3909e28b
		"fcvt.ps.f11 f21, f17\n"                              // opcode=0xd0988afb
		"fnmsub.ps f29, f13, f13, f15, dyn\n"                 // opcode=0x7cd6fedb
		"fnmsub.ps f23, f16, f12, f18, dyn\n"                 // opcode=0x94c87bdb
		"fnmadd.s f28, f13, f15, f16, dyn\n"                  // opcode=0x80f6fe4f
		"cubeface.ps f31, f19, f16\n"                         // opcode=0x89098ffb
		"fandi.pi f29, f17, 273\n"                            // opcode=0x8c389ebf
		"cubesgntc.ps f0, f15, f18\n"                         // opcode=0x8927b07b
		"fcvt.un2.ps f20, f19\n"                              // opcode=0xd9798a7b
		"fcvt.un10.ps f29, f16\n"                             // opcode=0xd9280efb
		"fmadd.ps f25, f13, f19, f16, dyn\n"                  // opcode=0x8136fcdb
		"flt.pi f29, f13, f10\n"                              // opcode=0xa6a69efb

		 
		"fence\n"

		"fcvt.ps.f10 f24, f19\n"                              // opcode=0xd0898c7b

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fltm.pi m4, f10, f14\n"                              // opcode=0x3ee5027b
		"fmul.s f24, f15, f12, dyn\n"                         // opcode=0x10c7fc53
		"fcvt.un2.ps f24, f14\n"                              // opcode=0xd9770c7b


		"fsq2 f16, 0 (x17)\n"                                 // opcode=0xe308d027
		"fsgnj.ps f25, f14, f18\n"                            // opcode=0x21270cfb


		"fbci.ps f22, 518546\n"                               // opcode=0xfd325b1f
		"flem.ps m4, f19, f12\n"                              // opcode=0xa0c9c27b
		"fcvt.un10.ps f29, f18\n"                             // opcode=0xd9290efb


		"fbci.ps f28, 242452\n"                               // opcode=0x76629e1f
		"fcvt.wu.s x28, f10, dyn\n"                           // opcode=0xc0157e53


		"fcvt.ps.pwu f0, f13, dyn\n"                          // opcode=0xd016f07b
		"fscw.ps f19, f1(x14)\n"                              // opcode=0xe0e0998b
		"cubesgnsc.ps f0, f16, f18\n"                         // opcode=0x8928207b
		"fcvt.f11.ps f21, f12\n"                              // opcode=0xd8860afb
		"fsub.s f27, f16, f15, dyn\n"                         // opcode=0x08f87dd3




		"cubefaceidx.ps f0, f13, f16\n"                       // opcode=0x8906907b
		"fmaxu.pi f22, f10, f11\n"                            // opcode=0x2eb53b7b
		"fcvt.f11.ps f20, f17\n"                              // opcode=0xd8888a7b


		"flog.ps f22, f16\n"                                  // opcode=0x58380b7b
		"for.pi f25, f14, f12\n"                              // opcode=0x06c76cfb
		"fsub.s f20, f10, f14, dyn\n"                         // opcode=0x08e57a53
		"cubefaceidx.ps f31, f19, f12\n"                      // opcode=0x88c99ffb
		"fcvt.ps.f10 f26, f12\n"                              // opcode=0xd0860d7b
		"mov.m.x m1, x19, 15\n"                               // opcode=0x5639e0fb
		"fsw.ps f11, 1232(x13)\n"                             // opcode=0x4cb6e80b
		"fadd.s f31, f14, f14, dyn\n"                         // opcode=0x00e77fd3
		"fround.ps f31, f19, dyn\n"                           // opcode=0x5819fffb
		"cubesgntc.ps f0, f17, f13\n"                         // opcode=0x88d8b07b
		"maskor m5, m4, m4\n"                                 // opcode=0x664262fb
		"fnot.pi f23, f17\n"                                  // opcode=0x0608abfb
		"fsgnjn.s f26, f10, f19\n"                            // opcode=0x21351d53
		"maskpopcz x23, m4\n"                                 // opcode=0x54020bfb
		"fadd.s f23, f14, f12, dyn\n"                         // opcode=0x00c77bd3
		"fcvt.w.s x23, f16, dyn\n"                            // opcode=0xc0087bd3
		"flt.s x31, f16, f11\n"                               // opcode=0xa0b81fd3
		"fnmsub.s f31, f18, f12, f19, dyn\n"                  // opcode=0x98c97fcb
		"fgw.ps f21, f1(x13)\n"                               // opcode=0x60d09a8b
		"cubesgnsc.ps f29, f11, f16\n"                        // opcode=0x8905aefb
		"fsgnjx.s f28, f18, f10\n"                            // opcode=0x20a92e53
		"fcvt.un2.ps f25, f16\n"                              // opcode=0xd9780cfb


		"cubesgnsc.ps f20, f19, f17\n"                        // opcode=0x8919aa7b


		"fpackrepb.pi f24, f19\n"                             // opcode=0x26098c7b
		"fcvt.w.s x31, f11, dyn\n"                            // opcode=0xc005ffd3
		"fsrl.pi f29, f11, f19\n"                             // opcode=0x0735defb
		"fcvt.sn16.ps f0, f18\n"                              // opcode=0xd999007b
		"fcvt.ps.f10 f26, f15\n"                              // opcode=0xd0878d7b
		"fgh.ps f21, f1(x11)\n"                               // opcode=0x50b09a8b
		"fand.pi f0, f13, f12\n"                              // opcode=0x06c6f07b
		"fnmadd.ps f0, f17, f12, f10, dyn\n"                  // opcode=0x56c8f05b
		"fcvt.sn8.ps f24, f18\n"                              // opcode=0xd9b90c7b

		"li x5, 0x8ec8416768c4136c\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmax.s f26, f18, f14\n"                              // opcode=0x28e91d53
		"fgh.ps f22, f0(x13)\n"                               // opcode=0x50d01b0b
		"fmax.pi f22, f15, f19\n"                             // opcode=0x2f379b7b


		"fsq2 f18, 0 (x13)\n"                                 // opcode=0x3326daa7
		"fcvt.pw.ps f29, f16, dyn\n"                          // opcode=0xc0087efb
		"feqm.ps m0, f10, f19\n"                              // opcode=0xa135607b
		"fmax.s f28, f13, f15\n"                              // opcode=0x28f69e53
		"for.pi f24, f14, f15\n"                              // opcode=0x06f76c7b
		"fmulh.pi f30, f14, f12\n"                            // opcode=0x16c72f7b

		 
		"fence\n"

		"fbci.pi f27, 50227\n"                                // opcode=0x18866ddf
		"frcp.ps f31, f15\n"                                  // opcode=0x58778ffb
		"fsgnjn.ps f28, f19, f17\n"                           // opcode=0x21199e7b
		"fsch.ps f17, f1(x17)\n"                              // opcode=0xd110988b
		"fmin.ps f27, f17, f18\n"                             // opcode=0x29288dfb
		"fmin.s f30, f12, f11\n"                              // opcode=0x28b60f53


		"fcvt.ps.pwu f20, f14, dyn\n"                         // opcode=0xd0177a7b
		"frcp.ps f22, f19\n"                                  // opcode=0x58798b7b
		"fnmsub.s f27, f18, f13, f19, dyn\n"                  // opcode=0x98d97dcb
		"faddi.pi f25, f15, 295\n"                            // opcode=0x94e78cbf
		"ffrc.ps f27, f13\n"                                  // opcode=0x58268dfb
		"fle.ps f31, f13, f10\n"                              // opcode=0xa0a68ffb
		"fcvt.f10.ps f31, f16\n"                              // opcode=0xd8b80ffb
		"fcmov.ps f25, f17, f12, f17\n"                       // opcode=0x8cc8acbf
		"fsrli.pi f0, f11, 3\n"                               // opcode=0x4e75d07b


		"fcvt.s.wu f21, x11, dyn\n"                           // opcode=0xd015fad3
		"cubeface.ps f28, f10, f17\n"                         // opcode=0x89150e7b
		"feq.ps f21, f10, f10\n"                              // opcode=0xa0a52afb
		"frcp.ps f23, f10\n"                                  // opcode=0x58750bfb
		"fmv.s.x f30, x15\n"                                  // opcode=0xf0078f53
		"fmv.s.x f30, x18\n"                                  // opcode=0xf0090f53
		"fclass.ps f0, f11\n"                                 // opcode=0xe005907b


		"fsch.ps f13, f0(x11)\n"                              // opcode=0xd0b0168b
		"fmin.ps f30, f13, f16\n"                             // opcode=0x29068f7b
		"fmin.pi f0, f18, f13\n"                              // opcode=0x2ed9007b
		"fsub.s f29, f18, f12, dyn\n"                         // opcode=0x08c97ed3


		"fmvs.x.ps x26, f18, 1\n"                             // opcode=0xe0192d7b
		"fsgnjn.ps f27, f14, f16\n"                           // opcode=0x21071dfb
		"fcvt.wu.s x24, f11, dyn\n"                           // opcode=0xc015fc53
		"fpackreph.pi f25, f19\n"                             // opcode=0x26099cfb


		"fminu.pi f21, f13, f12\n"                            // opcode=0x2ec6aafb


		"fsq2 f14, 0 (x17)\n"                                 // opcode=0xaee8d927
		"fnot.pi f25, f11\n"                                  // opcode=0x0605acfb
		"fmin.ps f27, f10, f19\n"                             // opcode=0x29350dfb
		"fmv.x.s x29, f16\n"                                  // opcode=0xe0080ed3


		"flw.ps f26, 36(x19)\n"                               // opcode=0x0249ad0b
		"fmul.s f28, f11, f18, dyn\n"                         // opcode=0x1125fe53
		"fcvt.f10.ps f22, f15\n"                              // opcode=0xd8b78b7b


		//RTLMIN-5479"fsin.ps f29, f14\n"                                  // opcode=0x58670efb
		"fsrl.pi f24, f12, f15\n"                             // opcode=0x06f65c7b

		 
		"fence\n"

		"fsgnj.ps f24, f18, f18\n"                            // opcode=0x21290c7b

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"fltm.pi m4, f11, f15\n"                              // opcode=0x3ef5827b
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
