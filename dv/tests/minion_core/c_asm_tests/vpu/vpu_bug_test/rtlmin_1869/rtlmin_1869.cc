/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#include "macros.h"
#include <cstdint>
#include "cacheops.h"
#include "tensors.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

int main() {

	C_TEST_START;

	// Avoid using x2 and x8. x2 is stack pointer. Assembler does not like if we use x8 and forbid it from using x8 - it errors out
	volatile uint32_t m_11[256] __attribute__ ((aligned (32))) = {
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff800000,                                                  // -inf                                       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x80000000,                                                  // -zero                                      
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[256] __attribute__ ((aligned (32))) = {
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000,                                                  // inf                                        
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x55555555,                                                  // 4 random values                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[256] __attribute__ ((aligned (32))) = {
			 0x55555555,                                                  // 4 random values                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000,                                                  // zero                                       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000000,                                                  // -zero                                      
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[256] __attribute__ ((aligned (32))) = {
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000000,                                                  // -zero                                      
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000000,                                                  // -zero                                      
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000,                                                  // zero                                       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[256] __attribute__ ((aligned (32))) = {
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000,                                                  // zero                                       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x55555555,                                                  // 4 random values                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000,                                                  // inf                                        
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_16[256] __attribute__ ((aligned (32))) = {
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000000,                                                  // -zero                                      
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x55555555,                                                  // 4 random values                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_17[256] __attribute__ ((aligned (32))) = {
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0xff800000,                                                  // -inf                                       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x3f028f5c,                                                  // 0.51                                       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7f800000,                                                  // inf                                        
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_18[256] __attribute__ ((aligned (32))) = {
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000000,                                                  // zero                                       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000000,                                                  // -zero                                      
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000000,                                                  // -zero                                      
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000000,                                                  // -zero                                      
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_19[256] __attribute__ ((aligned (32))) = {
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff800000,                                                  // -inf                                       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
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
		"fmul.ps f20, f18, f16, rne\n"                        // opcode=0x11090a7b
		"cubefaceidx.ps f21, f17, f10\n"                      // opcode=0x88a89afb
		"fnmadd.ps f21, f16, f15, f12, rne\n"                 // opcode=0x66f87adb
		"for.pi f30, f11, f18\n"                              // opcode=0x0725ef7b
		"fsgnjx.s f21, f18, f11\n"                            // opcode=0x20b92ad3
		"cubesgnsc.ps f0, f17, f15\n"                         // opcode=0x88f8a07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulh.pi f30, f18, f14\n"                            // opcode=0x16e92f7b
		"fmulh.pi f25, f12, f19\n"                            // opcode=0x17362cfb
		"fsw f19, -503(x15)\n"                                // opcode=0xe137a4a7
		"maskor m6, m7, m1\n"                                 // opcode=0x6613e37b
		"fmin.ps f24, f15, f14\n"                             // opcode=0x28e78c7b
		"maskpopc x28, m6\n"                                  // opcode=0x52030e7b
		"fadd.s f28, f16, f19, rne\n"                         // opcode=0x01387e53
		"frcp.ps f21, f15\n"                                  // opcode=0x58778afb
		"flt.ps f24, f11, f13\n"                              // opcode=0xa0d59c7b
		"fle.ps f23, f16, f15\n"                              // opcode=0xa0f80bfb
		"fandi.pi f29, f16, 173\n"                            // opcode=0x55a81ebf
		"fmv.s.x f22, x13\n"                                  // opcode=0xf0068b53
		"fminu.pi f23, f18, f15\n"                            // opcode=0x2ef92bfb
		"cubesgnsc.ps f31, f16, f10\n"                        // opcode=0x88a82ffb
		"cubeface.ps f22, f19, f11\n"                         // opcode=0x88b98b7b
		"fmvz.x.ps x20, f16, 0\n"                             // opcode=0xe0080a7b
		"fsub.ps f30, f14, f10, rne\n"                        // opcode=0x08a77f7b
		"fmadd.ps f25, f10, f18, f17, rne\n"                  // opcode=0x89257cdb
		"fandi.pi f20, f19, 296\n"                            // opcode=0x95099a3f
		"fslli.pi f23, f18, 6\n"                              // opcode=0x4ed91bfb
		"fswizz.ps f28, f13, 127\n"                           // opcode=0xe7f6fe7b
		"fsatu8.pi f30, f18\n"                                // opcode=0x06193f7b
		"flem.ps m0, f12, f18\n"                              // opcode=0xa126407b
		"fnmsub.ps f21, f15, f10, f15, rne\n"                 // opcode=0x7ca7fadb
		"fpackreph.pi f31, f15\n"                             // opcode=0x26079ffb
		"fandi.pi f21, f13, 436\n"                            // opcode=0xdc969abf
		"fsgnjx.ps f29, f13, f11\n"                           // opcode=0x20b6aefb
		"fmax.pi f0, f16, f10\n"                              // opcode=0x2ea8107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flq2 f23, 0 (x12)\n"                                 // opcode=0xb0665b87
		"fsub.pi f24, f16, f11\n"                             // opcode=0x0eb80c7b
		"maskand m3, m2, m5\n"                                // opcode=0x665171fb
		"fmvs.x.ps x29, f17, 7\n"                             // opcode=0xe078aefb
		"fsub.s f27, f12, f19, rne\n"                         // opcode=0x09367dd3
		"fbcx.ps f20, x10\n"                                  // opcode=0x00053a0b
		"fsgnjn.ps f0, f19, f10\n"                            // opcode=0x20a9907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"fsin.ps f25, f15\n"                                  // opcode=0x58678cfb
		"cubesgntc.ps f31, f10, f18\n"                        // opcode=0x89253ffb
		"fsgnjx.s f21, f14, f10\n"                            // opcode=0x20a72ad3
		"fltm.ps m7, f15, f13\n"                              // opcode=0xa0d7d3fb
		"fsgnjx.ps f24, f16, f16\n"                           // opcode=0x21082c7b
		"fadd.s f24, f15, f13, rne\n"                         // opcode=0x00d7fc53
		"fsrli.pi f29, f13, 14\n"                             // opcode=0x4fc6defb
		"fsll.pi f27, f14, f16\n"                             // opcode=0x07071dfb
		"cubesgntc.ps f29, f14, f16\n"                        // opcode=0x89073efb
		"fmul.s f0, f16, f14, rne\n"                          // opcode=0x10e87053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrl.pi f0, f11, f14\n"                              // opcode=0x06e5d07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.x.s x21, f12\n"                                  // opcode=0xe0060ad3
		"fmulh.pi f0, f12, f18\n"                             // opcode=0x1726207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.s f0, f15, f17, f12, rne\n"                   // opcode=0x6117f04f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsq2 f18, 0 (x12)\n"                                 // opcode=0xd9265227
		"fsgnjx.s f27, f14, f18\n"                            // opcode=0x21272dd3
		"fslli.pi f30, f18, 14\n"                             // opcode=0x4fc91f7b
		"fmadd.s f30, f18, f18, f16, rne\n"                   // opcode=0x81297f43
		"fsub.s f23, f18, f12, rne\n"                         // opcode=0x08c97bd3
		"fexp.ps f0, f12\n"                                   // opcode=0x5846007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulhu.pi f29, f15, f16\n"                           // opcode=0x17079efb
		"fnmsub.ps f21, f13, f12, f14, rne\n"                 // opcode=0x74c6fadb
		"fsrl.pi f25, f11, f19\n"                             // opcode=0x0735dcfb
		"fmulh.pi f25, f16, f15\n"                            // opcode=0x16f82cfb
		"fslli.pi f28, f17, 12\n"                             // opcode=0x4f989e7b
		"fnmsub.s f24, f13, f11, f11, rne\n"                  // opcode=0x58b6fc4b
		"fmsub.ps f27, f19, f14, f10, rne\n"                  // opcode=0x52e9fddb
		"maskand m7, m7, m3\n"                                // opcode=0x6633f3fb
		"fsrli.pi f30, f18, 8\n"                              // opcode=0x4f195f7b
		"frcp.ps f24, f16\n"                                  // opcode=0x58780c7b
		"fsat8.pi f20, f18\n"                                 // opcode=0x06093a7b
		"fmaxu.pi f22, f14, f15\n"                            // opcode=0x2ef73b7b
		"fmulhu.pi f30, f10, f13\n"                           // opcode=0x16d51f7b
		"feqm.ps m4, f14, f13\n"                              // opcode=0xa0d7627b
		"fmvs.x.ps x24, f16, 1\n"                             // opcode=0xe0182c7b
		"fbcx.ps f20, x16\n"                                  // opcode=0x00083a0b
		"ffrc.ps f27, f13\n"                                  // opcode=0x58268dfb
		"fsrai.pi f25, f12, 6\n"                              // opcode=0x4ed67cfb
		"fsw.ps f10, 1827(x11)\n"                             // opcode=0x72a5e18b
		"fmin.pi f20, f14, f17\n"                             // opcode=0x2f170a7b
		"fsgnjx.s f31, f14, f14\n"                            // opcode=0x20e72fd3
		"fcmov.ps f21, f11, f14, f13\n"                       // opcode=0x6ce5aabf
		"fswizz.ps f26, f14, 121\n"                           // opcode=0xe7e72d7b
		"fsq2 f14, 0 (x17)\n"                                 // opcode=0x1ae8d027
		"fltm.pi m7, f11, f14\n"                              // opcode=0x3ee583fb
		"fmv.s.x f27, x12\n"                                  // opcode=0xf0060dd3
		"cubesgntc.ps f27, f12, f12\n"                        // opcode=0x88c63dfb
		"fmvs.x.ps x0, f19, 1\n"                              // opcode=0xe019a07b
		"fmsub.ps f31, f16, f17, f13, rne\n"                  // opcode=0x6b187fdb
		"mov.m.x m2, x14, 106\n"                              // opcode=0x57a7517b
		"fsatu8.pi f26, f12\n"                                // opcode=0x06163d7b
		"fmaxu.pi f26, f13, f12\n"                            // opcode=0x2ec6bd7b
		"fmvz.x.ps x20, f18, 5\n"                             // opcode=0xe0590a7b
		"fclass.ps f21, f13\n"                                // opcode=0xe0069afb
		"fbcx.ps f23, x14\n"                                  // opcode=0x00073b8b
		"fsub.pi f26, f10, f14\n"                             // opcode=0x0ee50d7b
		"flem.ps m4, f10, f11\n"                              // opcode=0xa0b5427b
		"fand.pi f28, f12, f19\n"                             // opcode=0x07367e7b
		"fmin.ps f25, f14, f15\n"                             // opcode=0x28f70cfb
		"fmadd.s f25, f17, f17, f12, rne\n"                   // opcode=0x6118fcc3
		"for.pi f31, f13, f19\n"                              // opcode=0x0736effb
		"fclass.s x27, f17\n"                                 // opcode=0xe0089dd3
		"fslli.pi f26, f16, 15\n"                             // opcode=0x4ff81d7b
		"maskand m5, m7, m5\n"                                // opcode=0x6653f2fb
		"cubesgnsc.ps f31, f12, f11\n"                        // opcode=0x88b62ffb
		"fnmsub.ps f31, f11, f16, f10, rne\n"                 // opcode=0x5505ffdb
		"fle.ps f29, f13, f13\n"                              // opcode=0xa0d68efb
		"fandi.pi f26, f17, 20\n"                             // opcode=0x0c889d3f
		"fpackrepb.pi f26, f19\n"                             // opcode=0x26098d7b
		"fsw.ps f14, -625(x15)\n"                             // opcode=0xd8e7e78b
		"fmulhu.pi f23, f13, f17\n"                           // opcode=0x17169bfb
		"maskpopc.rast x25, m7, m2, 0\n"                      // opcode=0x5e278cfb
		"fmvs.x.ps x25, f15, 1\n"                             // opcode=0xe017acfb
		"fcmovm.ps f21, f14, f13\n"                           // opcode=0x00d70af7
		"fnmadd.s f22, f17, f15, f15, rne\n"                  // opcode=0x78f8fb4f
		"fmvz.x.ps x23, f16, 5\n"                             // opcode=0xe0580bfb
		"fmv.s.x f24, x11\n"                                  // opcode=0xf0058c53
		"fmax.pi f25, f19, f17\n"                             // opcode=0x2f199cfb
		"fmin.pi f28, f10, f11\n"                             // opcode=0x2eb50e7b
		//RTLMIN-5479"frsq.ps f23, f17\n"                                  // opcode=0x58888bfb
		"fsgnj.ps f24, f10, f16\n"                            // opcode=0x21050c7b
		//RTLMIN-5479"frsq.ps f21, f11\n"                                  // opcode=0x58858afb
		"fand.pi f23, f17, f10\n"                             // opcode=0x06a8fbfb
		"fsgnj.s f0, f18, f19\n"                              // opcode=0x21390053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.ps f27, f12, f18\n"                              // opcode=0xa1262dfb
		"fsrai.pi f21, f15, 8\n"                              // opcode=0x4f07fafb
		"fsrl.pi f0, f16, f15\n"                              // opcode=0x06f8507b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackreph.pi f31, f18\n"                             // opcode=0x26091ffb
		"fslli.pi f22, f17, 12\n"                             // opcode=0x4f989b7b
		"fsll.pi f0, f14, f12\n"                              // opcode=0x06c7107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsw.ps f16, 1021(x11)\n"                             // opcode=0x3f05ee8b
		"fswizz.ps f29, f11, 92\n"                            // opcode=0xe7758efb
		"frcp.ps f25, f11\n"                                  // opcode=0x58758cfb
		"fnmadd.ps f25, f15, f16, f17, rne\n"                 // opcode=0x8f07fcdb
		"fsgnjn.ps f21, f12, f11\n"                           // opcode=0x20b61afb
		"maskpopcz x21, m3\n"                                 // opcode=0x54018afb
		"fle.ps f28, f18, f16\n"                              // opcode=0xa1090e7b
		"fslli.pi f28, f16, 0\n"                              // opcode=0x4e081e7b
		"fnmadd.s f29, f12, f19, f14, rne\n"                  // opcode=0x71367ecf
		"fclass.ps f20, f18\n"                                // opcode=0xe0091a7b
		"fmsub.ps f23, f10, f10, f18, rne\n"                  // opcode=0x92a57bdb
		"fmaxu.pi f25, f10, f16\n"                            // opcode=0x2f053cfb
		"fsgnj.s f28, f18, f13\n"                             // opcode=0x20d90e53
		"fmin.ps f27, f18, f13\n"                             // opcode=0x28d90dfb
		"cubesgnsc.ps f30, f16, f15\n"                        // opcode=0x88f82f7b
		"fmv.x.s x25, f11\n"                                  // opcode=0xe0058cd3
		"fmvs.x.ps x31, f15, 3\n"                             // opcode=0xe037affb
		"fsgnj.ps f27, f15, f15\n"                            // opcode=0x20f78dfb
		"feq.ps f21, f19, f14\n"                              // opcode=0xa0e9aafb
		"fmin.ps f27, f11, f14\n"                             // opcode=0x28e58dfb
		"fsw f15, -58(x18)\n"                                 // opcode=0xfcf92327
		"maskpopcz x24, m4\n"                                 // opcode=0x54020c7b
		"fsgnjn.s f22, f12, f10\n"                            // opcode=0x20a61b53
		"fsgnj.ps f30, f16, f11\n"                            // opcode=0x20b80f7b
		"fcmov.ps f27, f17, f14, f17\n"                       // opcode=0x8ce8adbf
		"fmulhu.pi f26, f15, f15\n"                           // opcode=0x16f79d7b
		"fmaxu.pi f23, f12, f18\n"                            // opcode=0x2f263bfb
		"fmulh.pi f28, f11, f10\n"                            // opcode=0x16a5ae7b
		"fminu.pi f21, f17, f12\n"                            // opcode=0x2ec8aafb
		"fsgnjx.ps f26, f11, f12\n"                           // opcode=0x20c5ad7b
		"fsgnj.s f29, f13, f13\n"                             // opcode=0x20d68ed3
		"flt.pi f25, f10, f11\n"                              // opcode=0xa6b51cfb
		"fmulhu.pi f31, f18, f16\n"                           // opcode=0x17091ffb
		"fbcx.ps f22, x18\n"                                  // opcode=0x00093b0b
		"cubesgntc.ps f30, f14, f19\n"                        // opcode=0x89373f7b
		"fmax.pi f21, f17, f18\n"                             // opcode=0x2f289afb
		"fsgnjx.s f30, f17, f12\n"                            // opcode=0x20c8af53
		"fmv.s.x f30, x18\n"                                  // opcode=0xf0090f53
		"fpackreph.pi f28, f11\n"                             // opcode=0x26059e7b
		"fle.ps f29, f12, f12\n"                              // opcode=0xa0c60efb
		"cubesgnsc.ps f23, f13, f12\n"                        // opcode=0x88c6abfb
		"fnmadd.ps f30, f16, f11, f19, rne\n"                 // opcode=0x9eb87f5b
		"fmul.ps f31, f17, f13, rne\n"                        // opcode=0x10d8fffb
		"fsw f13, -1940(x16)\n"                               // opcode=0x86d82627
		"fnmsub.ps f30, f17, f19, f19, rne\n"                 // opcode=0x9d38ff5b
		"fmvz.x.ps x26, f14, 6\n"                             // opcode=0xe0670d7b
		"flog.ps f29, f10\n"                                  // opcode=0x58350efb
		"fmvs.x.ps x28, f14, 4\n"                             // opcode=0xe0472e7b
		"fsub.ps f27, f11, f13, rne\n"                        // opcode=0x08d5fdfb
		"fmax.ps f27, f14, f15\n"                             // opcode=0x28f71dfb
		"flem.ps m6, f18, f16\n"                              // opcode=0xa109437b
		"fcmovm.ps f22, f11, f18\n"                           // opcode=0x01258b77
		"fadd.ps f0, f19, f11, rne\n"                         // opcode=0x00b9f07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.ps f22, f12, f13\n"                             // opcode=0x28d60b7b
		"flt.ps f25, f18, f15\n"                              // opcode=0xa0f91cfb
		"fnmsub.s f24, f10, f16, f12, rne\n"                  // opcode=0x61057c4b
		"maskor m2, m4, m3\n"                                 // opcode=0x6632617b
		"faddi.pi f22, f12, 419\n"                            // opcode=0xd4660b3f
		"fswizz.ps f28, f16, 54\n"                            // opcode=0xe6d84e7b
		"feq.s x24, f11, f18\n"                               // opcode=0xa125ac53
		"fsetm.pi m1, f15\n"                                  // opcode=0xe007d0fb
		"ffrc.ps f27, f17\n"                                  // opcode=0x58288dfb
		"fsatu8.pi f20, f14\n"                                // opcode=0x06173a7b
		"fclass.s x29, f18\n"                                 // opcode=0xe0091ed3
		"fmsub.s f27, f15, f17, f16, rne\n"                   // opcode=0x8117fdc7
		"cubesgnsc.ps f27, f16, f18\n"                        // opcode=0x89282dfb
		"fsat8.pi f26, f15\n"                                 // opcode=0x0607bd7b
		"fmadd.s f23, f18, f14, f19, rne\n"                   // opcode=0x98e97bc3
		"fmax.s f26, f14, f16\n"                              // opcode=0x29071d53
		"fmax.ps f20, f14, f16\n"                             // opcode=0x29071a7b
		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fsw f10, -196(x13)\n"                                // opcode=0xf2a6ae27
		"fnmsub.s f0, f18, f17, f15, rne\n"                   // opcode=0x7919704b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m6, f11, f13\n"                              // opcode=0x3ed5837b
		"fsgnj.s f29, f11, f14\n"                             // opcode=0x20e58ed3
		"fminu.pi f28, f18, f10\n"                            // opcode=0x2ea92e7b
		"fsatu8.pi f27, f12\n"                                // opcode=0x06163dfb
		"fmax.ps f0, f19, f14\n"                              // opcode=0x28e9907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.s f25, f10, f13, rne\n"                         // opcode=0x00d57cd3
		"fsrli.pi f23, f17, 2\n"                              // opcode=0x4e58dbfb
		"fmulh.pi f31, f13, f15\n"                            // opcode=0x16f6affb
		"fsub.s f20, f11, f16, rne\n"                         // opcode=0x0905fa53
		"fnot.pi f23, f13\n"                                  // opcode=0x0606abfb
		"flem.ps m0, f12, f10\n"                              // opcode=0xa0a6407b
		"fsw.ps f10, 1080(x18)\n"                             // opcode=0x42a96c0b
		"fsgnjn.ps f0, f11, f13\n"                            // opcode=0x20d5907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flq2 f25, 0 (x14)\n"                                 // opcode=0xbff75c87
		"maskor m2, m5, m2\n"                                 // opcode=0x6622e17b
		"fswizz.ps f28, f18, 64\n"                            // opcode=0xe7091e7b
		"cubeface.ps f24, f18, f10\n"                         // opcode=0x88a90c7b
		"fexp.ps f28, f14\n"                                  // opcode=0x58470e7b
		"fsrli.pi f20, f15, 7\n"                              // opcode=0x4ee7da7b
		"fpackrepb.pi f27, f19\n"                             // opcode=0x26098dfb
		"flw.ps f23, 169(x16)\n"                              // opcode=0x0a982b8b
		"fsw.ps f12, -748(x11)\n"                             // opcode=0xd0c5ea0b
		"fbcx.ps f24, x16\n"                                  // opcode=0x00083c0b
		"cubeface.ps f27, f13, f12\n"                         // opcode=0x88c68dfb
		"cubefaceidx.ps f0, f15, f13\n"                       // opcode=0x88d7907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.s f30, f11, f17, rne\n"                         // opcode=0x0915ff53
		"fbcx.ps f27, x18\n"                                  // opcode=0x00093d8b
		"fsw.ps f16, -1416(x19)\n"                            // opcode=0xa709ec0b
		"fmadd.s f0, f18, f14, f18, rne\n"                    // opcode=0x90e97043
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.pi f20, f18, f16\n"                             // opcode=0x2f090a7b
		"fmulh.pi f22, f18, f16\n"                            // opcode=0x17092b7b
		"fmulhu.pi f24, f14, f15\n"                           // opcode=0x16f71c7b
		"fmaxu.pi f28, f18, f14\n"                            // opcode=0x2ee93e7b
		"fsgnjx.ps f20, f13, f18\n"                           // opcode=0x2126aa7b
		"fltm.pi m5, f17, f19\n"                              // opcode=0x3f3882fb
		"fltu.pi f29, f11, f11\n"                             // opcode=0xa6b5befb
		"feq.s x21, f12, f14\n"                               // opcode=0xa0e62ad3
		"fle.pi f28, f16, f11\n"                              // opcode=0xa6b80e7b
		"mov.m.x m3, x13, 57\n"                               // opcode=0x56e6a1fb
		"fswizz.ps f25, f14, 5\n"                             // opcode=0xe6172cfb
		"fand.pi f26, f13, f18\n"                             // opcode=0x0726fd7b
		"flw f29, 1140(x17)\n"                                // opcode=0x4748ae87
		"fsrli.pi f22, f15, 3\n"                              // opcode=0x4e67db7b
		"fsgnj.ps f31, f18, f12\n"                            // opcode=0x20c90ffb
		"fmvz.x.ps x27, f10, 1\n"                             // opcode=0xe0150dfb
		"fbci.ps f27, 104652\n"                               // opcode=0x33199d9f
		"fmulh.pi f24, f17, f12\n"                            // opcode=0x16c8ac7b
		"maskxor m5, m5, m1\n"                                // opcode=0x6612c2fb
		"li x5, 0xcee\n"                                      // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fpackrepb.pi f25, f11\n"                             // opcode=0x26058cfb
		"fbci.ps f28, 147569\n"                               // opcode=0x480e2e1f
		"fsgnj.ps f23, f15, f11\n"                            // opcode=0x20b78bfb
		"fbc.ps f22, 1843(x12)\n"                             // opcode=0x73360b0b
		"fnmadd.s f0, f12, f13, f16, rne\n"                   // opcode=0x80d6704f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.s f25, f12, f19, rne\n"                         // opcode=0x01367cd3
		"fsrli.pi f20, f18, 5\n"                              // opcode=0x4ea95a7b
		"fsw f15, -1556(x15)\n"                               // opcode=0x9ef7a627
		"fsrli.pi f25, f12, 9\n"                              // opcode=0x4f365cfb
		"fsatu8.pi f28, f10\n"                                // opcode=0x06153e7b
		"fle.s x31, f12, f10\n"                               // opcode=0xa0a60fd3
		"fclass.ps f25, f18\n"                                // opcode=0xe0091cfb
		"fmul.pi f20, f12, f14\n"                             // opcode=0x16e60a7b
		"fsrai.pi f29, f16, 6\n"                              // opcode=0x4ed87efb
		"fcmov.ps f29, f13, f16, f13\n"                       // opcode=0x6d06aebf
		"masknot m0, m3\n"                                    // opcode=0x6601a07b
		"for.pi f22, f18, f18\n"                              // opcode=0x07296b7b
		"fmv.x.s x25, f19\n"                                  // opcode=0xe0098cd3
		"fnmadd.ps f23, f13, f10, f19, rne\n"                 // opcode=0x9ea6fbdb
		"fsq2 f11, 0 (x10)\n"                                 // opcode=0x02b55aa7
		"fmv.s.x f20, x17\n"                                  // opcode=0xf0088a53
		"fmsub.s f24, f15, f12, f14, rne\n"                   // opcode=0x70c7bc47
		"for.pi f30, f14, f11\n"                              // opcode=0x06b76f7b
		"feq.s x30, f14, f13\n"                               // opcode=0xa0d72f53
		"fmsub.ps f26, f14, f16, f10, rne\n"                  // opcode=0x53077d5b
		"fand.pi f25, f16, f15\n"                             // opcode=0x06f87cfb
		"maskpopc x30, m7\n"                                  // opcode=0x52038f7b
		"fsgnj.s f28, f11, f18\n"                             // opcode=0x21258e53
		"feqm.ps m7, f13, f19\n"                              // opcode=0xa136e3fb
		"fsq2 f10, 0 (x15)\n"                                 // opcode=0xfca7d3a7
		"fmax.ps f29, f10, f11\n"                             // opcode=0x28b51efb
		"fcmovm.ps f27, f17, f16\n"                           // opcode=0x01088df7
		"fmax.s f0, f19, f16\n"                               // opcode=0x29099053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.s f23, f12, f10, rne\n"                         // opcode=0x00a67bd3
		"fsgnj.s f21, f14, f17\n"                             // opcode=0x21170ad3
		"fadd.ps f27, f10, f17, rne\n"                        // opcode=0x01157dfb
		"fsetm.pi m2, f17\n"                                  // opcode=0xe008d17b
		"fmin.s f0, f10, f15\n"                               // opcode=0x28f50053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.pi f20, f12, f14\n"                             // opcode=0x2ee60a7b
		"flq2 f21, 0 (x16)\n"                                 // opcode=0x52285a87
		"fxor.pi f31, f15, f16\n"                             // opcode=0x0707cffb
		"fsrli.pi f29, f11, 11\n"                             // opcode=0x4f75defb
		"fsatu8.pi f22, f19\n"                                // opcode=0x0619bb7b
		"cubeface.ps f28, f17, f12\n"                         // opcode=0x88c88e7b
		"fsrai.pi f26, f14, 2\n"                              // opcode=0x4e577d7b
		"fbc.ps f25, 1268(x12)\n"                             // opcode=0x4f460c8b
		"flem.ps m3, f16, f13\n"                              // opcode=0xa0d841fb
		"fsatu8.pi f0, f18\n"                                 // opcode=0x0619307b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.ps f20, f13, f19\n"                           // opcode=0x21369a7b
		"fle.pi f0, f10, f14\n"                               // opcode=0xa6e5007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fmin.ps f25, f13, f13\n"                             // opcode=0x28d68cfb
		"fmadd.s f21, f17, f14, f10, rne\n"                   // opcode=0x50e8fac3
		"fmin.s f20, f14, f13\n"                              // opcode=0x28d70a53
		"fsrl.pi f28, f17, f16\n"                             // opcode=0x0708de7b
		"fsra.pi f21, f16, f19\n"                             // opcode=0x0f385afb
		"fsll.pi f20, f18, f19\n"                             // opcode=0x07391a7b
		"fmaxu.pi f27, f18, f13\n"                            // opcode=0x2ed93dfb
		"fsgnj.ps f22, f17, f11\n"                            // opcode=0x20b88b7b
		"fmax.pi f26, f17, f12\n"                             // opcode=0x2ec89d7b
		"maskxor m0, m7, m5\n"                                // opcode=0x6653c07b
		"fcmov.ps f22, f12, f18, f13\n"                       // opcode=0x6d262b3f
		"flog.ps f27, f18\n"                                  // opcode=0x58390dfb
		"fsetm.pi m7, f19\n"                                  // opcode=0xe009d3fb
		"flem.ps m4, f11, f13\n"                              // opcode=0xa0d5c27b
		"fsll.pi f29, f14, f16\n"                             // opcode=0x07071efb
		"fsetm.pi m6, f14\n"                                  // opcode=0xe007537b
		"feq.s x29, f18, f11\n"                               // opcode=0xa0b92ed3
		"for.pi f23, f14, f12\n"                              // opcode=0x06c76bfb
		"frcp.ps f21, f19\n"                                  // opcode=0x58798afb
		"feq.pi f28, f18, f17\n"                              // opcode=0xa7192e7b
		"maskpopc.rast x26, m1, m7, 6\n"                      // opcode=0x5ff08d7b
		"fsra.pi f26, f17, f12\n"                             // opcode=0x0ec8dd7b
		"fmadd.ps f27, f18, f10, f13, rne\n"                  // opcode=0x68a97ddb
		"fsll.pi f22, f11, f17\n"                             // opcode=0x07159b7b
		"cubesgnsc.ps f30, f14, f14\n"                        // opcode=0x88e72f7b
		"flt.ps f0, f13, f17\n"                               // opcode=0xa116907b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.ps f24, f18\n"                                // opcode=0xe0091c7b
		"fnmsub.ps f21, f13, f11, f14, rne\n"                 // opcode=0x74b6fadb
		"fsw.ps f14, -1803(x10)\n"                            // opcode=0x8ee56a8b
		"fsub.ps f21, f19, f17, rne\n"                        // opcode=0x0919fafb
		"masknot m2, m3\n"                                    // opcode=0x6601a17b
		"fmin.ps f24, f11, f10\n"                             // opcode=0x28a58c7b
		"fsw f17, -1401(x11)\n"                               // opcode=0xa915a3a7
		"fmax.pi f29, f13, f14\n"                             // opcode=0x2ee69efb
		"maskpopcz x30, m2\n"                                 // opcode=0x54010f7b
		"fsw.ps f12, 1480(x18)\n"                             // opcode=0x5cc9640b
		"fmax.s f0, f18, f18\n"                               // opcode=0x29291053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.ps f20, f16, f19\n"                              // opcode=0xa1382a7b
		"mov.m.x m2, x19, 32\n"                               // opcode=0x5689917b
		"fbc.ps f30, -1294(x11)\n"                            // opcode=0xaf258f0b
		"fsq2 f11, 0 (x11)\n"                                 // opcode=0x22b5d927
		"fsgnjn.s f26, f13, f18\n"                            // opcode=0x21269d53
		"fsw f14, -1345(x15)\n"                               // opcode=0xaae7afa7
		"fle.ps f26, f13, f12\n"                              // opcode=0xa0c68d7b
		"fand.pi f25, f17, f17\n"                             // opcode=0x0718fcfb
		"maskpopc.rast x21, m4, m4, 7\n"                      // opcode=0x5fca0afb
		"fsrai.pi f25, f19, 5\n"                              // opcode=0x4ea9fcfb
		"cubeface.ps f22, f16, f14\n"                         // opcode=0x88e80b7b
		"flog.ps f24, f10\n"                                  // opcode=0x58350c7b
		"fmadd.s f22, f14, f18, f10, rne\n"                   // opcode=0x51277b43
		"li x5, 0x8d3\n"                                      // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"for.pi f20, f19, f19\n"                              // opcode=0x0739ea7b
		"fsat8.pi f29, f12\n"                                 // opcode=0x06063efb
		"fandi.pi f25, f17, 207\n"                            // opcode=0x65f89cbf
		"fbci.pi f26, 185004\n"                               // opcode=0x5a559d5f
		"fmin.s f23, f18, f14\n"                              // opcode=0x28e90bd3
		"fminu.pi f25, f14, f15\n"                            // opcode=0x2ef72cfb
		"fround.ps f20, f13, rne\n"                           // opcode=0x5816fa7b
		"fmulh.pi f28, f14, f12\n"                            // opcode=0x16c72e7b
		"fslli.pi f26, f14, 7\n"                              // opcode=0x4ee71d7b
		"ffrc.ps f23, f14\n"                                  // opcode=0x58270bfb
		"for.pi f28, f14, f10\n"                              // opcode=0x06a76e7b
		"cubesgntc.ps f23, f10, f16\n"                        // opcode=0x89053bfb
		"faddi.pi f0, f14, 69\n"                              // opcode=0x24b7003f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmaxu.pi f29, f16, f12\n"                            // opcode=0x2ec83efb
		"fltm.pi m4, f16, f19\n"                              // opcode=0x3f38027b
		"fsat8.pi f24, f16\n"                                 // opcode=0x06083c7b
		"fbci.ps f29, 69830\n"                                // opcode=0x2218ce9f
		"fnmadd.ps f27, f15, f17, f17, rne\n"                 // opcode=0x8f17fddb
		"for.pi f29, f12, f19\n"                              // opcode=0x07366efb
		"fmax.ps f0, f10, f13\n"                              // opcode=0x28d5107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.s f23, f12, f12, f12, rne\n"                  // opcode=0x60c67bcf
		"fle.ps f21, f10, f11\n"                              // opcode=0xa0b50afb
		"fmin.ps f22, f19, f17\n"                             // opcode=0x29198b7b
		"fexp.ps f22, f19\n"                                  // opcode=0x58498b7b
		"fltm.ps m7, f15, f15\n"                              // opcode=0xa0f7d3fb
		"flt.pi f22, f17, f12\n"                              // opcode=0xa6c89b7b
		"fltm.pi m5, f15, f11\n"                              // opcode=0x3eb782fb
		"fmsub.s f26, f11, f10, f17, rne\n"                   // opcode=0x88a5fd47
		"fmin.s f28, f10, f12\n"                              // opcode=0x28c50e53
		"fsgnjn.ps f27, f10, f16\n"                           // opcode=0x21051dfb
		"masknot m5, m6\n"                                    // opcode=0x660322fb
		"maskpopc x22, m6\n"                                  // opcode=0x52030b7b
		"fsq2 f10, 0 (x11)\n"                                 // opcode=0x7aa5dc27
		"fsw f18, -1488(x10)\n"                               // opcode=0xa3252827
		//RTLMIN-5479"frsq.ps f0, f16\n"                                   // opcode=0x5888007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulh.pi f22, f16, f12\n"                            // opcode=0x16c82b7b
		"flem.ps m3, f15, f13\n"                              // opcode=0xa0d7c1fb
		"fle.s x26, f16, f11\n"                               // opcode=0xa0b80d53
		"fcmovm.ps f29, f11, f16\n"                           // opcode=0x01058ef7
		"fmax.ps f0, f16, f18\n"                              // opcode=0x2928107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m3, f11, f11\n"                              // opcode=0x3eb581fb
		"feqm.ps m6, f11, f14\n"                              // opcode=0xa0e5e37b
		"fsrai.pi f24, f19, 12\n"                             // opcode=0x4f99fc7b
		"fmv.x.s x0, f16\n"                                   // opcode=0xe0080053
		"fmvz.x.ps x20, f14, 4\n"                             // opcode=0xe0470a7b
		"maskand m7, m6, m5\n"                                // opcode=0x665373fb
		"fadd.s f24, f18, f13, rne\n"                         // opcode=0x00d97c53
		"fsgnjx.ps f25, f10, f14\n"                           // opcode=0x20e52cfb
		"fmadd.s f21, f15, f17, f18, rne\n"                   // opcode=0x9117fac3
		"fltm.ps m6, f18, f18\n"                              // opcode=0xa129537b
		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"maskpopc.rast x31, m1, m5, 1\n"                      // opcode=0x5e588ffb
		"fxor.pi f30, f16, f18\n"                             // opcode=0x07284f7b
		"fsub.ps f29, f10, f17, rne\n"                        // opcode=0x09157efb
		"fmax.ps f24, f10, f17\n"                             // opcode=0x29151c7b
		"fltu.pi f23, f11, f18\n"                             // opcode=0xa725bbfb
		"fpackreph.pi f31, f10\n"                             // opcode=0x26051ffb
		"feqm.ps m0, f19, f19\n"                              // opcode=0xa139e07b
		"fswizz.ps f26, f10, 63\n"                            // opcode=0xe6f57d7b
		"fmvs.x.ps x24, f13, 5\n"                             // opcode=0xe056ac7b
		"fslli.pi f30, f18, 10\n"                             // opcode=0x4f591f7b
		"fminu.pi f26, f15, f19\n"                            // opcode=0x2f37ad7b
		"fsra.pi f26, f15, f11\n"                             // opcode=0x0eb7dd7b
		"fle.pi f25, f12, f14\n"                              // opcode=0xa6e60cfb
		"maskor m3, m2, m4\n"                                 // opcode=0x664161fb
		"maskpopcz x30, m1\n"                                 // opcode=0x54008f7b
		//RTLMIN-5479"frsq.ps f30, f19\n"                                  // opcode=0x58898f7b
		"fmv.x.s x28, f17\n"                                  // opcode=0xe0088e53
		"fmulhu.pi f21, f17, f10\n"                           // opcode=0x16a89afb
		"fandi.pi f22, f19, 507\n"                            // opcode=0xfd799b3f
		"fmax.pi f0, f10, f18\n"                              // opcode=0x2f25107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fminu.pi f27, f14, f12\n"                            // opcode=0x2ec72dfb
		"fnmsub.s f26, f12, f15, f11, rne\n"                  // opcode=0x58f67d4b
		"fle.s x20, f11, f16\n"                               // opcode=0xa1058a53
		"faddi.pi f30, f19, 374\n"                            // opcode=0xbcc98f3f
		"frcp.ps f21, f12\n"                                  // opcode=0x58760afb
		"fbc.ps f20, 380(x13)\n"                              // opcode=0x17c68a0b
		"fmvs.x.ps x23, f10, 6\n"                             // opcode=0xe0652bfb
		"fsra.pi f26, f16, f12\n"                             // opcode=0x0ec85d7b
		"cubesgntc.ps f31, f10, f19\n"                        // opcode=0x89353ffb
		"for.pi f22, f19, f19\n"                              // opcode=0x0739eb7b
		"fmin.s f0, f11, f15\n"                               // opcode=0x28f58053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmsub.s f22, f10, f17, f14, rne\n"                   // opcode=0x71157b47
		"feqm.ps m1, f15, f12\n"                              // opcode=0xa0c7e0fb
		"fsgnjx.ps f0, f12, f14\n"                            // opcode=0x20e6207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsra.pi f23, f11, f10\n"                             // opcode=0x0ea5dbfb
		"fsatu8.pi f21, f14\n"                                // opcode=0x06173afb
		"maskand m1, m0, m5\n"                                // opcode=0x665070fb
		"fmin.ps f27, f12, f14\n"                             // opcode=0x28e60dfb
		"flq2 f28, 0 (x15)\n"                                 // opcode=0x5607de07
		"fle.s x23, f19, f18\n"                               // opcode=0xa1298bd3
		"fround.ps f28, f18, rne\n"                           // opcode=0x58191e7b
		"fmsub.s f24, f18, f14, f11, rne\n"                   // opcode=0x58e97c47
		"masknot m3, m1\n"                                    // opcode=0x6600a1fb
		"fminu.pi f27, f19, f18\n"                            // opcode=0x2f29adfb
		"fpackreph.pi f22, f18\n"                             // opcode=0x26091b7b
		"fpackreph.pi f27, f18\n"                             // opcode=0x26091dfb
		"flw.ps f31, -361(x10)\n"                             // opcode=0xe9752f8b
		"fnmadd.ps f23, f17, f19, f10, rne\n"                 // opcode=0x5738fbdb
		"fround.ps f25, f14, rne\n"                           // opcode=0x58172cfb
		"fbcx.ps f25, x12\n"                                  // opcode=0x00063c8b
		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fnmsub.ps f22, f10, f12, f13, rne\n"                 // opcode=0x6cc57b5b
		"fnmadd.s f29, f16, f15, f16, rne\n"                  // opcode=0x80f87ecf
		"fnmadd.s f21, f15, f14, f19, rne\n"                  // opcode=0x98e7facf
		"fsw.ps f11, 1739(x18)\n"                             // opcode=0x6cb9658b
		"fround.ps f20, f19, rne\n"                           // opcode=0x5819fa7b
		"fsub.ps f28, f18, f19, rne\n"                        // opcode=0x09391e7b
		"flog.ps f24, f12\n"                                  // opcode=0x58360c7b
		"fpackreph.pi f0, f14\n"                              // opcode=0x2607107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fslli.pi f31, f18, 0\n"                              // opcode=0x4e091ffb
		"flw.ps f27, 1160(x12)\n"                             // opcode=0x48862d8b
		"fswizz.ps f28, f10, 12\n"                            // opcode=0xe6350e7b
		"fnmadd.ps f28, f18, f10, f17, rne\n"                 // opcode=0x8ea97e5b
		"flt.ps f21, f15, f10\n"                              // opcode=0xa0a79afb
		"cubefaceidx.ps f21, f16, f12\n"                      // opcode=0x88c81afb
		"fbci.pi f27, 410846\n"                               // opcode=0xc89bcddf
		"fle.s x23, f18, f19\n"                               // opcode=0xa1390bd3
		"fnmsub.ps f31, f16, f10, f10, rne\n"                 // opcode=0x54a87fdb
		"fmin.ps f0, f14, f15\n"                              // opcode=0x28f7007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.s f0, f19, f10, f12, rne\n"                   // opcode=0x60a9f04f
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m4, f13\n"                                  // opcode=0xe006d27b
		"fbci.ps f30, 391594\n"                               // opcode=0xbf354f1f
		"flw.ps f0, 205(x11)\n"                               // opcode=0x0cd5a00b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.ps f28, f16, f12\n"                           // opcode=0x20c82e7b
		"fsrli.pi f21, f12, 4\n"                              // opcode=0x4e965afb
		"fsgnjx.ps f30, f11, f12\n"                           // opcode=0x20c5af7b
		"mov.m.x m3, x11, 70\n"                               // opcode=0x5715d1fb
		"fclass.s x24, f18\n"                                 // opcode=0xe0091c53
		"feq.s x26, f19, f17\n"                               // opcode=0xa119ad53
		"flt.s x29, f13, f10\n"                               // opcode=0xa0a69ed3
		"maskpopcz x24, m5\n"                                 // opcode=0x54028c7b
		"flt.ps f25, f13, f14\n"                              // opcode=0xa0e69cfb
		"fbc.ps f23, -1955(x17)\n"                            // opcode=0x85d88b8b
		"maskor m1, m1, m1\n"                                 // opcode=0x6610e0fb
		//RTLMIN-5479"fsin.ps f23, f14\n"                                  // opcode=0x58670bfb
		"cubesgntc.ps f31, f15, f10\n"                        // opcode=0x88a7bffb
		"fmvs.x.ps x28, f17, 1\n"                             // opcode=0xe018ae7b
		"fcmov.ps f22, f15, f18, f13\n"                       // opcode=0x6d27ab3f
		"fmin.ps f30, f14, f17\n"                             // opcode=0x29170f7b
		"cubesgnsc.ps f21, f10, f18\n"                        // opcode=0x89252afb
		"maskpopc x0, m1\n"                                   // opcode=0x5200807b
		"fcmovm.ps f0, f19, f13\n"                            // opcode=0x00d98077
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"frsq.ps f20, f14\n"                                  // opcode=0x58870a7b
		"fmin.pi f24, f17, f15\n"                             // opcode=0x2ef88c7b
		"fmul.pi f0, f13, f15\n"                              // opcode=0x16f6807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.ps f26, f16, f12, f18, rne\n"                 // opcode=0x96c87d5b
		"flt.pi f24, f11, f12\n"                              // opcode=0xa6c59c7b
		"feq.pi f27, f17, f14\n"                              // opcode=0xa6e8adfb
		"masknot m0, m3\n"                                    // opcode=0x6601a07b
		"fmvs.x.ps x20, f13, 1\n"                             // opcode=0xe016aa7b
		"fmax.pi f24, f17, f12\n"                             // opcode=0x2ec89c7b
		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"feqm.ps m6, f15, f16\n"                              // opcode=0xa107e37b
		"fsat8.pi f25, f17\n"                                 // opcode=0x0608bcfb
		"maskpopc.rast x26, m3, m0, 3\n"                      // opcode=0x5e8d8d7b
		"fswizz.ps f27, f19, 111\n"                           // opcode=0xe7b9fdfb
		"maskpopc.rast x22, m3, m7, 7\n"                      // opcode=0x5ffd8b7b
		"fmul.s f0, f13, f14, rne\n"                          // opcode=0x10e6f053
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m0, f18\n"                                  // opcode=0xe009507b
		"fswizz.ps f22, f15, 109\n"                           // opcode=0xe7b7bb7b
		//RTLMIN-5479"frsq.ps f31, f14\n"                                  // opcode=0x58870ffb
		"fadd.s f23, f12, f17, rne\n"                         // opcode=0x01161bd3
		"feq.ps f0, f12, f17\n"                               // opcode=0xa116207b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrl.pi f20, f10, f12\n"                             // opcode=0x06c55a7b
		"maskor m6, m0, m3\n"                                 // opcode=0x6630637b
		"fsgnjx.s f29, f19, f18\n"                            // opcode=0x2129aed3
		"fclass.s x20, f15\n"                                 // opcode=0xe0079a53
		"fslli.pi f21, f19, 8\n"                              // opcode=0x4f099afb
		"feq.s x28, f13, f19\n"                               // opcode=0xa136ae53
		"frcp.ps f29, f19\n"                                  // opcode=0x58798efb
		"maskand m2, m1, m1\n"                                // opcode=0x6610f17b
		"fsrl.pi f25, f12, f19\n"                             // opcode=0x07365cfb
		"fsrai.pi f29, f12, 0\n"                              // opcode=0x4e167efb
		"fnmsub.ps f0, f13, f19, f15, rne\n"                  // opcode=0x7d36f05b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.s f28, f18, f18\n"                             // opcode=0x21290e53
		"flog.ps f0, f14\n"                                   // opcode=0x5837007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fandi.pi f31, f19, 305\n"                            // opcode=0x9c299fbf
		"fmv.s.x f20, x11\n"                                  // opcode=0xf0058a53
		"feqm.ps m1, f17, f16\n"                              // opcode=0xa108e0fb
		"fmadd.ps f20, f10, f19, f14, rne\n"                  // opcode=0x71357a5b
		"maskxor m4, m5, m2\n"                                // opcode=0x6622c27b
		"fsgnj.ps f23, f19, f18\n"                            // opcode=0x21298bfb
		"fmin.ps f25, f15, f10\n"                             // opcode=0x28a78cfb
		"fand.pi f23, f15, f17\n"                             // opcode=0x0717fbfb
		"fmin.s f31, f16, f15\n"                              // opcode=0x28f80fd3
		"maskor m7, m3, m3\n"                                 // opcode=0x6631e3fb
		"fmvs.x.ps x21, f10, 5\n"                             // opcode=0xe0552afb
		"flt.pi f20, f10, f11\n"                              // opcode=0xa6b51a7b
		"fminu.pi f24, f16, f12\n"                            // opcode=0x2ec82c7b
		"maskpopc.rast x25, m0, m5, 4\n"                      // opcode=0x5f540cfb
		"maskpopc.rast x23, m5, m4, 5\n"                      // opcode=0x5f4a8bfb
		"fsgnjx.s f24, f10, f13\n"                            // opcode=0x20d52c53
		"cubeface.ps f26, f14, f17\n"                         // opcode=0x89170d7b
		"maskxor m2, m4, m4\n"                                // opcode=0x6642417b
		//RTLMIN-5479"frsq.ps f26, f16\n"                                  // opcode=0x58880d7b
		"masknot m7, m7\n"                                    // opcode=0x6603a3fb
		"fsrli.pi f29, f16, 3\n"                              // opcode=0x4e685efb
		"fmsub.ps f31, f14, f11, f14, rne\n"                  // opcode=0x72b77fdb
		"feq.pi f21, f11, f15\n"                              // opcode=0xa6f5aafb
		"maskpopc x31, m2\n"                                  // opcode=0x52010ffb
		"masknot m4, m3\n"                                    // opcode=0x6601a27b
		"fsgnjn.ps f24, f13, f14\n"                           // opcode=0x20e69c7b
		"li x5, 0xc41\n"                                      // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fsrli.pi f28, f14, 13\n"                             // opcode=0x4fb75e7b
		"fbc.ps f0, 239(x17)\n"                               // opcode=0x0ef8800b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrli.pi f29, f17, 5\n"                              // opcode=0x4ea8defb
		"faddi.pi f26, f13, 289\n"                            // opcode=0x94368d3f
		"fnmadd.ps f27, f18, f17, f14, rne\n"                 // opcode=0x77197ddb
		"mov.m.x m0, x18, 74\n"                               // opcode=0x5729507b
		"fcmovm.ps f20, f10, f16\n"                           // opcode=0x01050a77
		"fnot.pi f22, f10\n"                                  // opcode=0x06052b7b
		"fsra.pi f25, f12, f17\n"                             // opcode=0x0f165cfb
		"fmulh.pi f23, f16, f18\n"                            // opcode=0x17282bfb
		"fnmadd.ps f21, f17, f17, f14, rne\n"                 // opcode=0x7718fadb
		"fmsub.s f21, f17, f10, f16, rne\n"                   // opcode=0x80a8fac7
		"fmsub.ps f26, f11, f17, f12, rne\n"                  // opcode=0x6315fd5b
		"fminu.pi f26, f10, f11\n"                            // opcode=0x2eb52d7b
		"maskor m1, m3, m3\n"                                 // opcode=0x6631e0fb
		"flt.pi f24, f16, f15\n"                              // opcode=0xa6f81c7b
		"maskor m6, m3, m2\n"                                 // opcode=0x6621e37b
		"fsq2 f18, 0 (x19)\n"                                 // opcode=0x2529d2a7
		"fsrli.pi f28, f14, 0\n"                              // opcode=0x4e075e7b
		"flt.s x24, f13, f15\n"                               // opcode=0xa0f69c53
		"fsgnjx.s f28, f17, f17\n"                            // opcode=0x2118ae53
		"fle.ps f26, f18, f15\n"                              // opcode=0xa0f90d7b
		"fsub.pi f0, f15, f16\n"                              // opcode=0x0f07807b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flq2 f26, 0 (x10)\n"                                 // opcode=0xbb655d07
		"fmax.ps f27, f18, f18\n"                             // opcode=0x29291dfb
		"fsub.s f30, f15, f12, rne\n"                         // opcode=0x08c7ff53
		"fmax.pi f29, f19, f14\n"                             // opcode=0x2ee99efb
		"fcmov.ps f30, f11, f11, f18\n"                       // opcode=0x94b5af3f
		"fadd.s f31, f10, f16, rne\n"                         // opcode=0x01057fd3
		"fnot.pi f25, f10\n"                                  // opcode=0x06052cfb
		"fadd.pi f21, f11, f16\n"                             // opcode=0x07058afb
		//RTLMIN-5479"fsin.ps f23, f15\n"                                  // opcode=0x58678bfb
		"flt.s x22, f18, f12\n"                               // opcode=0xa0c91b53
		"fmulhu.pi f28, f11, f10\n"                           // opcode=0x16a59e7b
		"fmul.ps f27, f12, f12, rne\n"                        // opcode=0x10c67dfb
		"fclass.ps f23, f11\n"                                // opcode=0xe0059bfb
		"fsub.pi f0, f12, f17\n"                              // opcode=0x0f16007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnot.pi f23, f17\n"                                  // opcode=0x0608abfb
		"fmax.ps f27, f13, f12\n"                             // opcode=0x28c69dfb
		"fswizz.ps f23, f19, 88\n"                            // opcode=0xe7698bfb
		"fmax.s f21, f12, f13\n"                              // opcode=0x28d61ad3
		"feq.pi f0, f13, f18\n"                               // opcode=0xa726a07b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.pi f25, f16, f16\n"                             // opcode=0x2f080cfb
		"feq.ps f26, f18, f12\n"                              // opcode=0xa0c92d7b
		"fbci.ps f22, 405416\n"                               // opcode=0xc5f50b1f
		"fsq2 f15, 0 (x19)\n"                                 // opcode=0xf0f9dea7
		"mov.m.x m0, x14, 73\n"                               // opcode=0x5727207b
		"flq2 f31, 0 (x17)\n"                                 // opcode=0xfc18df87
		"fbcx.ps f27, x15\n"                                  // opcode=0x0007bd8b
		//RTLMIN-5479"fsin.ps f23, f18\n"                                  // opcode=0x58690bfb
		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"flem.ps m4, f12, f11\n"                              // opcode=0xa0b6427b
		"fnmsub.s f30, f17, f18, f14, rne\n"                  // opcode=0x7128ff4b
		"flt.pi f23, f12, f11\n"                              // opcode=0xa6b61bfb
		"fbcx.ps f30, x12\n"                                  // opcode=0x00063f0b
		"fsatu8.pi f21, f10\n"                                // opcode=0x06153afb
		"fsub.s f21, f11, f17, rne\n"                         // opcode=0x0915fad3
		"feqm.ps m4, f15, f17\n"                              // opcode=0xa117e27b
		"fsatu8.pi f31, f13\n"                                // opcode=0x0616bffb
		"masknot m5, m2\n"                                    // opcode=0x660122fb
		"cubesgntc.ps f23, f13, f13\n"                        // opcode=0x88d6bbfb
		"fmul.ps f28, f10, f13, rne\n"                        // opcode=0x10d57e7b
		"fsq2 f10, 0 (x16)\n"                                 // opcode=0x92a85ba7
		"fnmsub.ps f21, f14, f14, f19, rne\n"                 // opcode=0x9ce77adb
		"flt.s x25, f18, f11\n"                               // opcode=0xa0b91cd3
		"fmax.ps f22, f19, f18\n"                             // opcode=0x29299b7b
		"fsub.ps f26, f19, f14, rne\n"                        // opcode=0x08e9fd7b
		"feq.s x29, f11, f13\n"                               // opcode=0xa0d5aed3
		"fadd.pi f28, f14, f15\n"                             // opcode=0x06f70e7b
		"fclass.s x27, f10\n"                                 // opcode=0xe0051dd3
		"fnot.pi f26, f19\n"                                  // opcode=0x0609ad7b
		"fsll.pi f25, f18, f10\n"                             // opcode=0x06a91cfb
		"maskor m0, m5, m4\n"                                 // opcode=0x6642e07b
		"fltu.pi f24, f14, f17\n"                             // opcode=0xa7173c7b
		"fmadd.ps f26, f17, f18, f15, rne\n"                  // opcode=0x7928fd5b
		//RTLMIN-5479"fsin.ps f0, f18\n"                                   // opcode=0x5869007b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnot.pi f21, f16\n"                                  // opcode=0x06082afb
		"fmv.s.x f29, x17\n"                                  // opcode=0xf0088ed3
		"cubesgntc.ps f22, f14, f10\n"                        // opcode=0x88a73b7b
		"fmax.s f30, f14, f19\n"                              // opcode=0x29371f53
		"feq.pi f31, f18, f11\n"                              // opcode=0xa6b92ffb
		"faddi.pi f24, f12, 358\n"                            // opcode=0xb4d60c3f
		"fsgnjn.s f22, f16, f14\n"                            // opcode=0x20e81b53
		"fslli.pi f20, f13, 5\n"                              // opcode=0x4eb69a7b
		"for.pi f20, f11, f14\n"                              // opcode=0x06e5ea7b
		"feq.ps f23, f19, f12\n"                              // opcode=0xa0c9abfb
		"maskand m3, m7, m6\n"                                // opcode=0x6663f1fb
		//RTLMIN-5479"fsin.ps f31, f14\n"                                  // opcode=0x58670ffb
		"fmsub.s f0, f15, f15, f19, rne\n"                    // opcode=0x98f7f047
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f27, -354(x19)\n"                             // opcode=0xe9e9ad8b
		"fnmadd.s f24, f18, f13, f17, rne\n"                  // opcode=0x88d97c4f
		"fmul.pi f28, f13, f13\n"                             // opcode=0x16d68e7b
		"fsrl.pi f25, f14, f11\n"                             // opcode=0x06b75cfb
		"fsgnj.ps f24, f17, f17\n"                            // opcode=0x21188c7b
		"flw.ps f29, 1312(x16)\n"                             // opcode=0x52082e8b
		"fmvz.x.ps x21, f18, 5\n"                             // opcode=0xe0590afb
		"fbc.ps f23, 738(x16)\n"                              // opcode=0x2e280b8b
		"flem.ps m1, f14, f13\n"                              // opcode=0xa0d740fb
		"fle.pi f28, f18, f17\n"                              // opcode=0xa7190e7b
		"feq.s x26, f13, f19\n"                               // opcode=0xa136ad53
		"maskor m1, m6, m7\n"                                 // opcode=0x667360fb
		"li x5, 0xc3f\n"                                      // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fmul.s f21, f11, f17, rne\n"                         // opcode=0x1115fad3
		"fsgnj.ps f29, f19, f17\n"                            // opcode=0x21198efb
		"fmv.s.x f28, x10\n"                                  // opcode=0xf0050e53
		"fsq2 f15, 0 (x18)\n"                                 // opcode=0xcaf95627
		"fmsub.s f21, f12, f19, f18, rne\n"                   // opcode=0x91362ac7
		"maskor m6, m4, m7\n"                                 // opcode=0x6672637b
		"fmadd.ps f28, f17, f14, f11, rne\n"                  // opcode=0x58e8fe5b
		"maskand m2, m7, m3\n"                                // opcode=0x6633f17b
		"flw.ps f27, -993(x18)\n"                             // opcode=0xc1f92d8b
		"fsw f14, -1990(x10)\n"                               // opcode=0x82e52d27
		"fsgnjn.s f20, f12, f16\n"                            // opcode=0x21061a53
		"fpackreph.pi f31, f10\n"                             // opcode=0x26051ffb
		"fsgnj.ps f24, f15, f16\n"                            // opcode=0x21078c7b
		"feq.pi f26, f12, f19\n"                              // opcode=0xa7362d7b
		"fslli.pi f26, f14, 3\n"                              // opcode=0x4e671d7b
		"maskor m6, m3, m1\n"                                 // opcode=0x6611e37b
		"fnot.pi f21, f12\n"                                  // opcode=0x06062afb
		"maskpopc x21, m7\n"                                  // opcode=0x52038afb
		//RTLMIN-5479"fsin.ps f28, f18\n"                                  // opcode=0x58690e7b
		"fnmsub.ps f21, f13, f17, f19, rne\n"                 // opcode=0x9d16fadb
		"fsgnjx.s f29, f18, f16\n"                            // opcode=0x21092ed3
		"fmadd.s f23, f14, f15, f18, rne\n"                   // opcode=0x90f77bc3
		"fsgnjx.s f24, f12, f15\n"                            // opcode=0x20f62c53
		"fsll.pi f0, f16, f13\n"                              // opcode=0x06d8107b
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.ps f31, f12, f13, f16, rne\n"                 // opcode=0x86d67fdb
		"cubesgnsc.ps f22, f15, f13\n"                        // opcode=0x88d7ab7b
		"fsub.s f20, f10, f19, rne\n"                         // opcode=0x09354a53
		"fltm.ps m0, f14, f18\n"                              // opcode=0xa127507b
		"fltm.pi m1, f17, f12\n"                              // opcode=0x3ec880fb
		"feqm.ps m2, f11, f14\n"                              // opcode=0xa0e5e17b
		"fltm.ps m3, f19, f16\n"                              // opcode=0xa109d1fb
		"fmin.s f20, f17, f12\n"                              // opcode=0x28c88a53
		"fle.ps f20, f15, f14\n"                              // opcode=0xa0e78a7b
		"flog.ps f21, f11\n"                                  // opcode=0x58358afb
		"fminu.pi f31, f10, f16\n"                            // opcode=0x2f052ffb
		"maskpopc.rast x28, m5, m0, 2\n"                      // opcode=0x5e828e7b
        
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
