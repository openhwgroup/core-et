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
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000000,                                                  // -zero                                      
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[256] __attribute__ ((aligned (32))) = {
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x55555555,                                                  // 4 random values                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            
			 0x00000000,                                                  // zero                                       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000000,                                                  // -zero                                      
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[256] __attribute__ ((aligned (32))) = {
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x33333333,                                                  // 4 random values                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000000,                                                  // zero                                       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[256] __attribute__ ((aligned (32))) = {
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x7f800000,                                                  // inf                                        
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000000,                                                  // -zero                                      
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x55555555,                                                  // 4 random values                            
			 0x55555555,                                                  // 4 random values                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f800000,                                                  // inf                                        
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000000,                                                  // zero                                       
			 0x7f800000,                                                  // inf                                        
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[256] __attribute__ ((aligned (32))) = {
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000000,                                                  // -zero                                      
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_16[256] __attribute__ ((aligned (32))) = {
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff800000,                                                  // -inf                                       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x3f028f5c,                                                  // 0.51                                       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_17[256] __attribute__ ((aligned (32))) = {
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000000,                                                  // -zero                                      
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001,                                                  // signaling NaN                              
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_18[256] __attribute__ ((aligned (32))) = {
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f800000,                                                  // inf                                        
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff800000,                                                  // -inf                                       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_19[256] __attribute__ ((aligned (32))) = {
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f800000,                                                  // inf                                        
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x55555555,                                                  // 4 random values                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x33333333,                                                  // 4 random values                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x55555555,                                                  // 4 random values                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
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
		"fmul.s f20, f12, f10, dyn\n"                         // opcode=0x10a67a53
		"flt.s x27, f10, f16\n"                               // opcode=0xa1051dd3
		"ffrc.ps f21, f12\n"                                  // opcode=0x58260afb
		"fpackrepb.pi f23, f19\n"                             // opcode=0x26098bfb
		"flq2 f28, 0 (x14)\n"                                 // opcode=0x31c75e07
		//RTLMIN-5479"fsin.ps f23, f12\n"                                  // opcode=0x58660bfb
		"cubefaceidx.ps f20, f17, f10\n"                      // opcode=0x88a89a7b
		"fswizz.ps f31, f11, 98\n"                            // opcode=0xe785dffb
		"feqm.ps m3, f17, f13\n"                              // opcode=0xa0d8e1fb
		"flt.s x28, f18, f11\n"                               // opcode=0xa0b91e53
		"fsgnjx.s f30, f11, f16\n"                            // opcode=0x2105af53
		//RTLMIN-5479"frsq.ps f30, f10\n"                                  // opcode=0x58850f7b
		"fmulhu.pi f24, f15, f14\n"                           // opcode=0x16e79c7b
		"fsrl.pi f23, f11, f16\n"                             // opcode=0x0705dbfb
		"fle.s x30, f16, f18\n"                               // opcode=0xa1280f53
		"cubeface.ps f21, f16, f18\n"                         // opcode=0x89280afb
		"fsra.pi f21, f11, f11\n"                             // opcode=0x0eb5dafb
		"mov.m.x m7, x13, 118\n"                              // opcode=0x57d6c3fb
		"fmul.s f22, f12, f18, dyn\n"                         // opcode=0x11267b53
		"flw.ps f21, -1898(x11)\n"                            // opcode=0x8965aa8b
		//RTLMIN-5479"fsin.ps f28, f15\n"                                  // opcode=0x58678e7b
		"flw.ps f23, 393(x19)\n"                              // opcode=0x1899ab8b
		"fnot.pi f20, f14\n"                                  // opcode=0x06072a7b
		"cubesgnsc.ps f25, f13, f11\n"                        // opcode=0x88b6acfb
		"cubesgntc.ps f21, f19, f11\n"                        // opcode=0x88b9bafb
		"fadd.ps f0, f15, f17, dyn\n"                         // opcode=0x0117f07b
		"fsrai.pi f28, f17, 6\n"                              // opcode=0x4ed8fe7b
		"fle.pi f0, f11, f12\n"                               // opcode=0xa6c5807b
		"fbc.ps f20, 2046(x14)\n"                             // opcode=0x7fe70a0b
		"fmulh.pi f23, f15, f11\n"                            // opcode=0x16b7abfb
		"fand.pi f25, f11, f13\n"                             // opcode=0x06d5fcfb
		"fpackrepb.pi f28, f17\n"                             // opcode=0x26088e7b
		"fmadd.s f26, f11, f17, f10, dyn\n"                   // opcode=0x5115fd43
		//RTLMIN-5479"frsq.ps f20, f10\n"                                  // opcode=0x58850a7b
		"fadd.ps f24, f16, f13, dyn\n"                        // opcode=0x00d87c7b
		"fsq2 f17, 0 (x10)\n"                                 // opcode=0x1f1556a7
		"fle.pi f23, f15, f14\n"                              // opcode=0xa6e78bfb
		"fnmadd.s f30, f12, f17, f13, rdn\n"                  // opcode=0x69162f4f
		"feq.pi f29, f12, f19\n"                              // opcode=0xa7362efb
		"fpackrepb.pi f28, f13\n"                             // opcode=0x26068e7b
		"fslli.pi f26, f13, 13\n"                             // opcode=0x4fa69d7b
		"fandi.pi f26, f15, 283\n"                            // opcode=0x8d679d3f
		"mov.m.x m3, x12, 89\n"                               // opcode=0x576621fb

		"fsq2 f14, 0 (x17)\n"                                 // opcode=0x36e8d127
		"fsub.ps f20, f18, f12, dyn\n"                        // opcode=0x08c97a7b
		"fsgnjn.s f31, f13, f13\n"                            // opcode=0x20d69fd3
		"feq.ps f27, f10, f14\n"                              // opcode=0xa0e52dfb
		"fbcx.ps f28, x16\n"                                  // opcode=0x00083e0b
		"fmvz.x.ps x21, f16, 1\n"                             // opcode=0xe0180afb
		"fclass.ps f31, f17\n"                                // opcode=0xe0089ffb
		"fadd.pi f28, f11, f11\n"                             // opcode=0x06b58e7b
		"cubesgnsc.ps f25, f14, f13\n"                        // opcode=0x88d72cfb
		"fsgnj.ps f20, f19, f10\n"                            // opcode=0x20a98a7b
		"fsgnjn.s f0, f10, f11\n"                             // opcode=0x20b51053
		"fcmovm.ps f28, f11, f10\n"                           // opcode=0x00a58e77
		"fbcx.ps f22, x12\n"                                  // opcode=0x00063b0b
		"fnmadd.ps f21, f12, f10, f13, dyn\n"                 // opcode=0x6ea67adb
		"fandi.pi f23, f15, 66\n"                             // opcode=0x24479bbf
		"for.pi f0, f17, f18\n"                               // opcode=0x0728e07b
		"fsub.s f28, f15, f15, dyn\n"                         // opcode=0x08f7fe53
		"fmadd.s f29, f17, f15, f19, dyn\n"                   // opcode=0x98f8fec3
		"fsgnj.s f30, f10, f14\n"                             // opcode=0x20e50f53
		"fadd.pi f22, f10, f12\n"                             // opcode=0x06c50b7b
		"fsatu8.pi f23, f18\n"                                // opcode=0x06193bfb
		"fbci.pi f30, 456976\n"                               // opcode=0xdf220f5f
		"for.pi f23, f14, f18\n"                              // opcode=0x07276bfb
		"fmulh.pi f25, f11, f12\n"                            // opcode=0x16c5acfb
		"fnot.pi f22, f12\n"                                  // opcode=0x06062b7b
		"fand.pi f22, f16, f13\n"                             // opcode=0x06d87b7b
		//RTLMIN-5479"fsin.ps f31, f17\n"                                  // opcode=0x58688ffb
		"fmul.s f20, f14, f13, rtz\n"                         // opcode=0x10d71a53
		"fmul.pi f29, f11, f11\n"                             // opcode=0x16b58efb

		"fmul.ps f31, f15, f15, dyn\n"                        // opcode=0x10f7fffb
		"fsrl.pi f28, f14, f16\n"                             // opcode=0x07075e7b
		"fsw.ps f15, -1603(x10)\n"                            // opcode=0x9af56e8b
		"fmvs.x.ps x22, f17, 0\n"                             // opcode=0xe008ab7b
		"fsq2 f11, 0 (x15)\n"                                 // opcode=0xb8b7d8a7
		//RTLMIN-5479"frsq.ps f26, f15\n"                                  // opcode=0x58878d7b
		"flq2 f0, 0 (x16)\n"                                  // opcode=0xc5785007
		"fsw.ps f13, -1806(x12)\n"                            // opcode=0x8ed6690b
		"fmadd.s f0, f13, f19, f10, dyn\n"                    // opcode=0x5136f043
		"fsrai.pi f21, f15, 6\n"                              // opcode=0x4ec7fafb
		"fltm.pi m1, f14, f12\n"                              // opcode=0x3ec700fb

		"fmin.ps f0, f11, f15\n"                              // opcode=0x28f5807b
		"fsub.s f31, f15, f15, dyn\n"                         // opcode=0x08f7ffd3
		"flq2 f27, 0 (x18)\n"                                 // opcode=0x90195d87
		"fmin.s f22, f13, f19\n"                              // opcode=0x29368b53
		"fsw f19, -371(x13)\n"                                // opcode=0xe936a6a7
		"fmulhu.pi f20, f15, f12\n"                           // opcode=0x16c79a7b
		"fswizz.ps f22, f18, 114\n"                           // opcode=0xe7c94b7b
		"fnot.pi f26, f14\n"                                  // opcode=0x06072d7b
		"fsub.pi f30, f14, f11\n"                             // opcode=0x0eb70f7b
		"fxor.pi f23, f10, f17\n"                             // opcode=0x07154bfb
		"feq.s x26, f18, f15\n"                               // opcode=0xa0f92d53
		"fpackrepb.pi f30, f10\n"                             // opcode=0x26050f7b
		"fsub.ps f31, f17, f15, dyn\n"                        // opcode=0x08f8fffb
		"fswizz.ps f27, f11, 50\n"                            // opcode=0xe6c5ddfb
		"fslli.pi f30, f16, 4\n"                              // opcode=0x4e881f7b
		"fsra.pi f28, f17, f18\n"                             // opcode=0x0f28de7b
		"fswizz.ps f29, f17, 125\n"                           // opcode=0xe7f8befb
		"fslli.pi f27, f11, 9\n"                              // opcode=0x4f259dfb
		//RTLMIN-5479"fsin.ps f24, f10\n"                                  // opcode=0x58650c7b
		"fminu.pi f23, f14, f19\n"                            // opcode=0x2f372bfb
		"fmin.pi f28, f16, f16\n"                             // opcode=0x2f080e7b
		"fle.pi f25, f14, f13\n"                              // opcode=0xa6d70cfb
		"fsub.pi f28, f19, f16\n"                             // opcode=0x0f098e7b
		"fslli.pi f21, f11, 9\n"                              // opcode=0x4f359afb
		"fsrl.pi f29, f16, f19\n"                             // opcode=0x07385efb
		"fsatu8.pi f27, f11\n"                                // opcode=0x0615bdfb
		"fsrl.pi f27, f11, f13\n"                             // opcode=0x06d5ddfb
		"fmaxu.pi f0, f11, f18\n"                             // opcode=0x2f25b07b
		"fsgnjn.ps f30, f17, f17\n"                           // opcode=0x21189f7b
		"fsra.pi f24, f10, f17\n"                             // opcode=0x0f155c7b
		"fmul.pi f21, f10, f16\n"                             // opcode=0x17050afb
		"fbcx.ps f28, x19\n"                                  // opcode=0x0009be0b
		"fltu.pi f0, f11, f17\n"                              // opcode=0xa715b07b
		"fsgnjx.s f20, f15, f12\n"                            // opcode=0x20c7aa53
		"fsrai.pi f25, f12, 7\n"                              // opcode=0x4ee67cfb
		"flt.ps f21, f19, f16\n"                              // opcode=0xa1099afb
		"fmadd.s f25, f11, f14, f12, rmm\n"                   // opcode=0x60e5ccc3
		"flt.s x30, f18, f11\n"                               // opcode=0xa0b91f53
		"fcmovm.ps f20, f12, f14\n"                           // opcode=0x00e60a77
		"fxor.pi f26, f11, f19\n"                             // opcode=0x0735cd7b
		"cubesgnsc.ps f24, f15, f17\n"                        // opcode=0x8917ac7b
		"fltm.ps m1, f12, f10\n"                              // opcode=0xa0a650fb
		"cubesgnsc.ps f26, f17, f14\n"                        // opcode=0x88e8ad7b
		"fsw f14, -1530(x19)\n"                               // opcode=0xa0e9a327
		"fltu.pi f0, f19, f18\n"                              // opcode=0xa729b07b
		"flq2 f22, 0 (x18)\n"                                 // opcode=0x34695b07
		"fpackrepb.pi f21, f16\n"                             // opcode=0x26080afb
		"fbci.ps f20, 237342\n"                               // opcode=0x73e3da1f
		"fsrl.pi f30, f11, f17\n"                             // opcode=0x0715df7b
		"fcmov.ps f28, f12, f19, f19\n"                       // opcode=0x9d362e3f
		"fmul.ps f24, f15, f16, rtz\n"                        // opcode=0x11079c7b
		"ffrc.ps f21, f11\n"                                  // opcode=0x58258afb
		//RTLMIN-5479"fsin.ps f29, f11\n"                                  // opcode=0x58658efb
		"fnot.pi f25, f13\n"                                  // opcode=0x0606acfb
		"fsrli.pi f30, f10, 5\n"                              // opcode=0x4eb55f7b
		"fmsub.ps f23, f17, f17, f13, dyn\n"                  // opcode=0x6b18fbdb
		"fsq2 f12, 0 (x18)\n"                                 // opcode=0x0ac95827

		"flq2 f24, 0 (x18)\n"                                 // opcode=0xa4995c07
		"fmv.s.x f0, x14\n"                                   // opcode=0xf0070053
		"fmsub.s f30, f10, f16, f12, dyn\n"                   // opcode=0x61057f47
		"flt.pi f24, f12, f10\n"                              // opcode=0xa6a61c7b
		"fsw.ps f14, -607(x13)\n"                             // opcode=0xdae6e08b
		"cubeface.ps f20, f13, f10\n"                         // opcode=0x88a68a7b
		"mov.m.x m3, x12, 32\n"                               // opcode=0x568611fb
		"cubefaceidx.ps f29, f12, f17\n"                      // opcode=0x89161efb
		"fminu.pi f25, f14, f17\n"                            // opcode=0x2f172cfb
		"fmv.s.x f22, x10\n"                                  // opcode=0xf0050b53
		"fsgnjx.s f20, f13, f13\n"                            // opcode=0x20d6aa53
		"fsgnjx.ps f30, f19, f10\n"                           // opcode=0x20a9af7b
		"fmul.s f28, f10, f17, dyn\n"                         // opcode=0x11157e53
		"feq.pi f31, f16, f13\n"                              // opcode=0xa6d82ffb
		"fle.pi f29, f14, f11\n"                              // opcode=0xa6b70efb
		"flt.ps f22, f10, f16\n"                              // opcode=0xa1051b7b
		"fbc.ps f29, -544(x15)\n"                             // opcode=0xde078e8b
		"fmul.ps f23, f13, f19, dyn\n"                        // opcode=0x1136fbfb
		"fmvz.x.ps x30, f11, 0\n"                             // opcode=0xe0058f7b
		"ffrc.ps f27, f16\n"                                  // opcode=0x58280dfb
		"for.pi f22, f12, f12\n"                              // opcode=0x06c66b7b
		"fsub.ps f29, f11, f19, dyn\n"                        // opcode=0x0935fefb
		"fmadd.ps f27, f15, f16, f12, dyn\n"                  // opcode=0x6107fddb
		"fsgnj.s f20, f18, f13\n"                             // opcode=0x20d90a53
		"fsgnjn.s f0, f17, f15\n"                             // opcode=0x20f89053
		"fsrl.pi f31, f11, f17\n"                             // opcode=0x0715dffb
		"fbcx.ps f25, x14\n"                                  // opcode=0x00073c8b
		"fmadd.s f25, f16, f10, f17, dyn\n"                   // opcode=0x88a87cc3
		"fmax.ps f28, f13, f15\n"                             // opcode=0x28f69e7b
		"fmax.s f20, f13, f11\n"                              // opcode=0x28b69a53
		"fmv.s.x f26, x15\n"                                  // opcode=0xf0078d53
		"fswizz.ps f21, f13, 67\n"                            // opcode=0xe706fafb
		"fmadd.ps f23, f17, f19, f12, dyn\n"                  // opcode=0x6138fbdb
		"fle.s x28, f17, f14\n"                               // opcode=0xa0e88e53
		"flt.ps f21, f15, f16\n"                              // opcode=0xa1079afb
		"flt.pi f20, f18, f15\n"                              // opcode=0xa6f91a7b
		"fsetm.pi m4, f11\n"                                  // opcode=0xe005d27b
		"fsgnjn.s f27, f10, f18\n"                            // opcode=0x21251dd3
		"frcp.ps f29, f17\n"                                  // opcode=0x58788efb
		"fmulhu.pi f28, f17, f18\n"                           // opcode=0x17289e7b
		"fmv.x.s x31, f18\n"                                  // opcode=0xe0090fd3
		"cubefaceidx.ps f25, f17, f11\n"                      // opcode=0x88b89cfb
		"flem.ps m6, f17, f18\n"                              // opcode=0xa128c37b
		"cubeface.ps f26, f11, f11\n"                         // opcode=0x88b58d7b
		"fsra.pi f24, f12, f18\n"                             // opcode=0x0f265c7b
		"feq.pi f0, f19, f17\n"                               // opcode=0xa719a07b
		"fmsub.s f25, f19, f15, f13, rne\n"                   // opcode=0x68f98cc7
		"fadd.s f21, f10, f15, dyn\n"                         // opcode=0x00f57ad3
		"fsub.ps f0, f11, f18, dyn\n"                         // opcode=0x0925f07b
		"fswizz.ps f28, f10, 127\n"                           // opcode=0xe7f56e7b
		"faddi.pi f20, f16, 229\n"                            // opcode=0x74b80a3f
		"cubesgntc.ps f25, f14, f12\n"                        // opcode=0x88c73cfb
		"fle.s x22, f11, f11\n"                               // opcode=0xa0b58b53
		"fmax.pi f31, f13, f15\n"                             // opcode=0x2ef69ffb
		//RTLMIN-5479"fsin.ps f21, f14\n"                                  // opcode=0x58670afb
		"cubesgntc.ps f31, f19, f10\n"                        // opcode=0x88a9bffb
		"fsw f12, -1408(x17)\n"                               // opcode=0xa8c8a027
		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs
		"fmul.s f0, f10, f11, dyn\n"                          // opcode=0x10b57053
		"fxor.pi f22, f15, f13\n"                             // opcode=0x06d7cb7b
		"fle.pi f31, f10, f10\n"                              // opcode=0xa6a50ffb
		"fmsub.s f23, f19, f12, f17, dyn\n"                   // opcode=0x88c9fbc7
		"fle.s x22, f13, f16\n"                               // opcode=0xa1068b53
		"fsatu8.pi f29, f14\n"                                // opcode=0x06173efb
		"fclass.ps f24, f14\n"                                // opcode=0xe0071c7b

		"fmsub.s f25, f15, f11, f12, dyn\n"                   // opcode=0x60b7fcc7
		"fsrai.pi f30, f15, 12\n"                             // opcode=0x4f97ff7b
		"fsq2 f10, 0 (x19)\n"                                 // opcode=0xcaa9d6a7
		"fsrai.pi f31, f12, 5\n"                              // opcode=0x4ea67ffb
		"fsat8.pi f30, f15\n"                                 // opcode=0x0607bf7b
		"flq2 f26, 0 (x14)\n"                                 // opcode=0xd7275d07
		"fmvz.x.ps x25, f13, 4\n"                             // opcode=0xe0468cfb
		"fsq2 f10, 0 (x17)\n"                                 // opcode=0x6ea8d127
		"fle.s x20, f17, f19\n"                               // opcode=0xa1388a53
		"cubesgnsc.ps f20, f11, f17\n"                        // opcode=0x8915aa7b
		"fmax.ps f28, f10, f10\n"                             // opcode=0x28a51e7b
		"fxor.pi f21, f14, f19\n"                             // opcode=0x07374afb
		"fnmadd.ps f22, f10, f17, f12, dyn\n"                 // opcode=0x67157b5b
		"fnmadd.s f28, f18, f11, f18, dyn\n"                  // opcode=0x90b97e4f
		"faddi.pi f25, f12, 166\n"                            // opcode=0x54d60cbf
		"fmax.s f24, f18, f18\n"                              // opcode=0x29291c53
		"feq.ps f30, f15, f12\n"                              // opcode=0xa0c7af7b
		"fpackrepb.pi f25, f13\n"                             // opcode=0x26068cfb
		"fmulh.pi f23, f17, f10\n"                            // opcode=0x16a8abfb
		"fmvz.x.ps x20, f16, 3\n"                             // opcode=0xe0380a7b
		"flt.s x23, f13, f13\n"                               // opcode=0xa0d69bd3
		"fsgnjn.s f28, f11, f12\n"                            // opcode=0x20c59e53
		"fsq2 f10, 0 (x19)\n"                                 // opcode=0xf8a9dda7
		"flt.pi f22, f11, f11\n"                              // opcode=0xa6b59b7b

		"cubeface.ps f24, f14, f15\n"                         // opcode=0x88f70c7b
		"cubesgnsc.ps f25, f16, f19\n"                        // opcode=0x89382cfb
		"flq2 f24, 0 (x16)\n"                                 // opcode=0xfbe85c07
		"fmsub.ps f22, f19, f18, f12, dyn\n"                  // opcode=0x6329fb5b
		"fmadd.ps f30, f15, f13, f15, dyn\n"                  // opcode=0x78d7ff5b
		"cubefaceidx.ps f21, f12, f17\n"                      // opcode=0x89161afb
		//RTLMIN-5479"frsq.ps f30, f10\n"                                  // opcode=0x58850f7b
		"fexp.ps f26, f18\n"                                  // opcode=0x58490d7b
		"fminu.pi f26, f16, f19\n"                            // opcode=0x2f382d7b
		"fsgnjn.s f26, f12, f10\n"                            // opcode=0x20a61d53
		"fbc.ps f27, -1510(x10)\n"                            // opcode=0xa1a50d8b
		"cubeface.ps f21, f10, f18\n"                         // opcode=0x89250afb
		"feq.pi f20, f19, f15\n"                              // opcode=0xa6f9aa7b
		"fadd.s f28, f10, f17, dyn\n"                         // opcode=0x01157e53
		"fexp.ps f21, f18\n"                                  // opcode=0x58490afb
		"fsq2 f18, 0 (x16)\n"                                 // opcode=0x23285c27
		"fexp.ps f28, f18\n"                                  // opcode=0x58490e7b
		"fminu.pi f26, f14, f14\n"                            // opcode=0x2ee72d7b
		"fmv.s.x f24, x11\n"                                  // opcode=0xf0058c53
		"fswizz.ps f25, f19, 110\n"                           // opcode=0xe7b9dcfb
		"flog.ps f26, f16\n"                                  // opcode=0x58380d7b
		"fswizz.ps f26, f16, 66\n"                            // opcode=0xe7084d7b
		"fmin.s f20, f19, f11\n"                              // opcode=0x28b98a53
		"fround.ps f23, f17, dyn\n"                           // opcode=0x5818fbfb
		"flt.pi f0, f18, f11\n"                               // opcode=0xa6b9107b
		"fsgnjn.s f23, f13, f14\n"                            // opcode=0x20e69bd3
		"fsat8.pi f24, f12\n"                                 // opcode=0x06063c7b
		"fsatu8.pi f26, f12\n"                                // opcode=0x06163d7b
		"fmadd.ps f30, f16, f10, f13, dyn\n"                  // opcode=0x68a87f5b
		"fsub.s f20, f16, f13, dyn\n"                         // opcode=0x08d87a53
		"fle.s x23, f13, f11\n"                               // opcode=0xa0b68bd3
		"fmv.s.x f21, x14\n"                                  // opcode=0xf0070ad3
		"fbcx.ps f30, x10\n"                                  // opcode=0x00053f0b
		"fclass.s x26, f16\n"                                 // opcode=0xe0081d53
		"fmadd.s f29, f14, f15, f10, dyn\n"                   // opcode=0x50f77ec3
		"flt.ps f27, f14, f10\n"                              // opcode=0xa0a71dfb
		"fmvs.x.ps x29, f14, 7\n"                             // opcode=0xe0772efb
		"feqm.ps m4, f15, f16\n"                              // opcode=0xa107e27b
		"fmulhu.pi f0, f14, f17\n"                            // opcode=0x1717107b
		"fmaxu.pi f31, f14, f11\n"                            // opcode=0x2eb73ffb
		"fpackreph.pi f28, f15\n"                             // opcode=0x26079e7b
		"fle.s x24, f17, f15\n"                               // opcode=0xa0f88c53
		"fsw.ps f15, 1619(x11)\n"                             // opcode=0x64f5e98b
		"cubesgnsc.ps f24, f17, f16\n"                        // opcode=0x8908ac7b
		"fclass.ps f31, f15\n"                                // opcode=0xe0079ffb
		"fsll.pi f23, f18, f12\n"                             // opcode=0x06c91bfb
		"fsll.pi f22, f16, f17\n"                             // opcode=0x07181b7b
		"fmax.ps f0, f14, f18\n"                              // opcode=0x2927107b
		"fmadd.ps f30, f12, f10, f13, dyn\n"                  // opcode=0x68a67f5b
		"fswizz.ps f20, f16, 10\n"                            // opcode=0xe6284a7b
		"for.pi f26, f11, f14\n"                              // opcode=0x06e5ed7b
		"fltm.ps m3, f16, f12\n"                              // opcode=0xa0c851fb
		"fsub.ps f25, f15, f16, dyn\n"                        // opcode=0x0907fcfb
		"fmax.s f28, f19, f15\n"                              // opcode=0x28f99e53
		"fpackreph.pi f23, f15\n"                             // opcode=0x26079bfb

		"flw f30, 1245(x18)\n"                                // opcode=0x4dd92f07
		"fbc.ps f20, -1777(x10)\n"                            // opcode=0x90f50a0b
		"fsat8.pi f20, f19\n"                                 // opcode=0x0609ba7b
		"flt.ps f28, f13, f17\n"                              // opcode=0xa1169e7b
		"fltu.pi f27, f13, f11\n"                             // opcode=0xa6b6bdfb
		"fmul.pi f22, f14, f18\n"                             // opcode=0x17270b7b
		"fle.ps f21, f15, f13\n"                              // opcode=0xa0d78afb
		"fbcx.ps f27, x12\n"                                  // opcode=0x00063d8b
		"fand.pi f27, f17, f14\n"                             // opcode=0x06e8fdfb
		"fminu.pi f0, f18, f10\n"                             // opcode=0x2ea9207b
        "fence\n"
    //		"csrrs x0, 0x7D1,1\n"                                     // set txfma sleep 2to7 csr 
        "fence\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
		"fmadd.ps f22, f17, f18, f15, dyn\n"                  // opcode=0x7928fb5b
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        
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
