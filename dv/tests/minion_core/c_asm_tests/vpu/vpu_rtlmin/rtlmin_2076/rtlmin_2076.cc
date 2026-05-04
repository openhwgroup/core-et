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
	volatile uint32_t m_11[1024] __attribute__ ((aligned (4096))) = {
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000000,                                                  // -zero                                      
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000000,                                                  // zero                                       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000000,                                                  // -zero                                      
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x55555555,                                                  // 4 random values                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000000,                                                  // zero                                       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f800000,                                                  // inf                                        
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f800000,                                                  // inf                                        
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000000,                                                  // zero                                       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x55555555,                                                  // 4 random values                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000,                                                  // inf                                        
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x33333333,                                                  // 4 random values                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x55555555,                                                  // 4 random values                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000000,                                                  // -zero                                      
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7f800000,                                                  // inf                                        
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x55555555,                                                  // 4 random values                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f800000,                                                  // inf                                        
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80011111,                                                  // -9.7958E-41                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x33333333,                                                  // 4 random values                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x33333333,                                                  // 4 random values                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000000,                                                  // zero                                       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000,                                                  // inf                                        
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x33333333,                                                  // 4 random values                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c700000,                                                  // Leading 1s:                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x55555555,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff800000,                                                  // -inf                                       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000,                                                  // inf                                        
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000000,                                                  // zero                                       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000000,                                                  // -zero                                      
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x55555555,                                                  // 4 random values                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x0c780000,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f800000,                                                  // inf                                        
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0xff800000,                                                  // -inf                                       
			 0x80000000,                                                  // -zero                                      
			 0x00000000,                                                  // zero                                       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xbf028f5c,                                                  // -0.51                                      
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7f800000,                                                  // inf                                        
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000000,                                                  // zero                                       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x3f028f5c,                                                  // 0.51                                       
			 0x55555555,                                                  // 4 random values                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000000,                                                  // -zero                                      
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000000,                                                  // zero                                       
			 0xbf028f5c,                                                  // -0.51                                      
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000000,                                                  // zero                                       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000000,                                                  // zero                                       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000000,                                                  // zero                                       
			 0xff800000,                                                  // -inf                                       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff800000,                                                  // -inf                                       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c700000,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000,                                                  // inf                                        
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f800000,                                                  // inf                                        
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000,                                                  // inf                                        
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000000,                                                  // zero                                       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f800000,                                                  // inf                                        
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000,                                                  // -inf                                       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f800000,                                                  // inf                                        
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x7f800000,                                                  // inf                                        
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000000,                                                  // -zero                                      
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000,                                                  // -zero                                      
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x33333333,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f800000,                                                  // inf                                        
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff800000,                                                  // -inf                                       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x4b000000,                                                  // 8388608.0                                  
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xff800000,                                                  // -inf                                       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xff800000,                                                  // -inf                                       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000,                                                  // -inf                                       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
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
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               

			 // vpu register f3
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f4
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             

			 // vpu register f5
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff800000, // infinity                                      // SP - ve numbers                            

			 // vpu register f6
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            

			 // vpu register f7
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e00001f,                                                  // Trailing 1s:                               

			 // vpu register f8
			 0x0c780000,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f9
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            

			 // vpu register f10
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             

			 // vpu register f11
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000000,                                                  // -zero                                      
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               

			 // vpu register f12
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         

			 // vpu register f13
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f14
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 

			 // vpu register f15
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            

			 // vpu register f16
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 

			 // vpu register f17
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00003f,                                                  // Trailing 1s:                               

			 // vpu register f18
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x33333333,                                                  // 4 random values                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7ffff0,                                                  // Leading 1s:                                

			 // vpu register f19
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e000007,                                                  // Trailing 1s:                               

			 // vpu register f20
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f21
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000000,                                                  // -zero                                      
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          

			 // vpu register f22
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             

			 // vpu register f23
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            

			 // vpu register f24
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               

			 // vpu register f25
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                

			 // vpu register f26
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f27
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               

			 // vpu register f28
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                

			 // vpu register f29
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               

			 // vpu register f30
			 0x55555555,                                                  // 4 random values                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                

			 // vpu register f31
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x3f800000     // 1.0                   
	};

	__asm__ __volatile__ (
		"j SET_UP_EXCEPTION_VECTOR\n"
		".align 12\n"
		"M_MODE_E_H:\n"
		"csrrw x0, 0x342,x0\n"      // mcause = 0
		"csrrw x0, 0x343,x0\n"      // mtval = 0
		"csrr	x5, 0x341\n"       // x5 = mepc
		"addi	x5,x5,4\n"        // x5+=4
		"csrw	 0x341,x5\n"       // mepc = x5
		"mret\n"                    // return
		".align 12\n"
		"S_MODE_E_H:\n"
		"csrrw x0, 0x142,x0\n"
		"csrrw x0, 0x143,x0\n"
		"csrr	x5, 0x141\n"
		"addi	x5,x5,4\n"
		"csrw	 0x141,x5\n"
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
		"addi x11, x11, 2047\n"
		"addi x11, x11, 1\n"
		"mv x12, %[m_12]\n"
		"addi x12, x12, 2047\n"
		"addi x12, x12, 1\n"
		"mv x13, %[m_13]\n"
		"addi x13, x13, 2047\n"
		"addi x13, x13, 1\n"
		"mv x14, %[m_14]\n"
		"addi x14, x14, 2047\n"
		"addi x14, x14, 1\n"
		"mv x15, %[m_15]\n"
		"addi x15, x15, 2047\n"
		"addi x15, x15, 1\n"
		"mv x4, %[operands]\n"
		"flw.ps f0,  0 (x4)\n"
		"flw.ps f1,  32 (x4)\n"
		"flw.ps f2,  64 (x4)\n"
		"flw.ps f3,  96 (x4)\n"
		"flw.ps f4,  128 (x4)\n"
		"flw.ps f5,  160 (x4)\n"
		"flw.ps f6,  192 (x4)\n"
		"flw.ps f7,  224 (x4)\n"
		"flw.ps f8,  256 (x4)\n"
		"flw.ps f9,  288 (x4)\n"
		"flw.ps f10,  320 (x4)\n"
		"flw.ps f11,  352 (x4)\n"
		"flw.ps f12,  384 (x4)\n"
		"flw.ps f13,  416 (x4)\n"
		"flw.ps f14,  448 (x4)\n"
		"flw.ps f15,  480 (x4)\n"
		"flw.ps f16,  512 (x4)\n"
		"flw.ps f17,  544 (x4)\n"
		"flw.ps f18,  576 (x4)\n"
		"flw.ps f19,  608 (x4)\n"
		"flw.ps f20,  640 (x4)\n"
		"flw.ps f21,  672 (x4)\n"
		"flw.ps f22,  704 (x4)\n"
		"flw.ps f23,  736 (x4)\n"
		"flw.ps f24,  768 (x4)\n"
		"flw.ps f25,  800 (x4)\n"
		"flw.ps f26,  832 (x4)\n"
		"flw.ps f27,  864 (x4)\n"
		"flw.ps f28,  896 (x4)\n"
		"flw.ps f29,  928 (x4)\n"
		"flw.ps f30,  960 (x4)\n"
		"flw.ps f31,  992 (x4)\n"
		"fence\n"
        "fsrli.pi f1, f13, 2\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fcmovm.ps f25, f1, f12\n"           
		"fcvt.wu.s x22, f1, dyn\n"        
        
	// Asm output results to C code
	:
	// Asm input data from C code
	: [operands] "r" (operands), [m_11] "r" (m_11), [m_12] "r" (m_12), [m_13] "r" (m_13), [m_14] "r" (m_14), [m_15] "r" (m_15)
	// Registers used in Asm so C knows they change
	: "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", "x11", "x12", "x13", "x14", "x15", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
	);
	C_TEST_PASS;
	return 0;
}
