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
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x33333333,                                                  // 4 random values                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000,                                                  // -zero                                      
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff800000,                                                  // -inf                                       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x33333333,                                                  // 4 random values                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x33333333,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x33333333,                                                  // 4 random values                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f800000,                                                  // inf                                        
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c400000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00000000,                                                  // zero                                       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000000,                                                  // -zero                                      
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff800000,                                                  // -inf                                       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x55555555,                                                  // 4 random values                            
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000,                                                  // -zero                                      
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000,                                                  // -inf                                       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000000,                                                  // -zero                                      
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001,                                                  // -signaling NaN                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f800000,                                                  // inf                                        
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000,                                                  // inf                                        
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000000,                                                  // zero                                       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000000,                                                  // -zero                                      
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x33333333,                                                  // 4 random values                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000000,                                                  // zero                                       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000000,                                                  // zero                                       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x80000000,                                                  // -zero                                      
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x33333333,                                                  // 4 random values                            
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff800000,                                                  // -inf                                       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff800000,                                                  // -inf                                       
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf028f5c,                                                  // -0.51                                      
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7f800000,                                                  // inf                                        
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f028f5c,                                                  // 0.51                                       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x33333333,                                                  // 4 random values                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x55555555,                                                  // 4 random values                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x55555555,                                                  // 4 random values                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000000,                                                  // -zero                                      
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c600000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c600000,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x55555555,                                                  // 4 random values                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000,                                                  // inf                                        
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x33333333,                                                  // 4 random values                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000000,                                                  // -zero                                      
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000,                                                  // inf                                        
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x33333333,                                                  // 4 random values                            
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x7f800000,                                                  // inf                                        
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e000001,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000000,                                                  // zero                                       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x55555555,                                                  // 4 random values                            
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xbf028f5c,                                                  // -0.51                                      
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xffc00001,                                                  // -signaling NaN                             
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x33333333,                                                  // 4 random values                            
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c600000,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c400000,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x55555555,                                                  // 4 random values                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xff800000,                                                  // -inf                                       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x80000000,                                                  // -zero                                      
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x7f800000,                                                  // inf                                        
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0xff800000,                                                  // -inf                                       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x33333333,                                                  // 4 random values                            
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x55555555,                                                  // 4 random values                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00000000,                                                  // zero                                       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x3f028f5c,                                                  // 0.51                                       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xff800000,                                                  // -inf                                       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x33333333,                                                  // 4 random values                            
			 0x00000000,                                                  // zero                                       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x3f800000     // 1.0                   
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f800000,                                                  // inf                                        
			 0x33333333,                                                  // 4 random values                            
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x0c780000,                                                  // Leading 1s:                                
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x33333333,                                                  // 4 random values                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xff800000,                                                  // -inf                                       
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x7f800000,                                                  // inf                                        
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0c600000,                                                  // Leading 1s:                                
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x55555555,                                                  // 4 random values                            
			 0x00000000,                                                  // zero                                       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000000,                                                  // zero                                       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xff800000,                                                  // -inf                                       
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x4b000000,                                                  // 8388608.0                                  
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000,                                                  // inf                                        
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x0c600000,                                                  // Leading 1s:                                
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x55555555,                                                  // 4 random values                            
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x80800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x33333333,                                                  // 4 random values                            
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x33333333,                                                  // 4 random values                            
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00011111,                                                  // 9.7958E-41                                 
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c780000,                                                  // Leading 1s:                                
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x80800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             
			 0x0c780000,                                                  // Leading 1s:                                
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x33333333,                                                  // 4 random values                            
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7f800000,                                                  // inf                                        
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000003,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                             
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0c7e0000,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000000,                                                  // -zero                                      
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000001, // min subnorm                                   // SP +ve numbers                             
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0xff7fffff, // max norm                                      // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fe000,                                                  // Leading 1s:                                
			 0x00011111,                                                  // 9.7958E-41                                 
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x80800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x7f800000,                                                  // inf                                        
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x3f800000, // 1                                             // SP +ve numbers                             
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x00800000,                                                  // none of the mantissa set to +3ulp          
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x33333333,                                                  // 4 random values                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x55555555,                                                  // 4 random values                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000000,                                                  // zero                                       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x55555555,                                                  // 4 random values                            
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                            
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x0c7ff000,                                                  // Leading 1s:                                
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00800000, // min norm                                      // SP +ve numbers                             
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x7fc00001,                                                  // signaling NaN                              
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x0c7fc000,                                                  // Leading 1s:                                
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000fff,                                                  // Trailing 1s:                               
			 0x0e0000ff,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                               
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00003f,                                                  // Trailing 1s:                               
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000000,                                                  // zero                                       
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x7f7fffff, // max norm                                      // max norm +ve                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x0e0001ff,                                                  // Trailing 1s:                               
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e1fffff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00000001,                                                  // 1.4E-45 (+denorm)                          
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x0c7ffffe,                                                  // Leading 1s:                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c400000,                                                  // Leading 1s:                                
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // SP - ve numbers                            
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xAAAAAAAA,                                                  // 4 random values                            
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xcb000000,                                                  // -8388608.0                                 
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x55555555,                                                  // 4 random values                            
			 0x80000000, // 0                                             // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                             
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                             
			 0xffffffff, // QNan                                          // SP - ve numbers                            
			 0x80000000,                                                  // -zero                                      
			 0xff7fffff, // max norm                                      // max norm -ve                               
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x33333333,                                                  // 4 random values                            
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0x007fffff,                                                  // 1.1754942E-38                              
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0xbf028f5c,                                                  // -0.51                                      
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x0e00000f,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e03ffff,                                                  // Trailing 1s:                               
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x33333333,                                                  // 4 random values                            
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0e000003,                                                  // Trailing 1s:                               
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x0e07ffff,                                                  // Trailing 1s:                               
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000001,                                                  // -1.4E-45 (-denorm)                         
			 0x3f028f5c,                                                  // 0.51                                       
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                             
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7ff800,                                                  // Leading 1s:                                
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
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
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                               
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0x80800000, // min norm                                      // subnormals -ve                             
			 0x00000000, // 0                                             // SP +ve numbers                             
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f3
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x0e3fffff,                                                  // Trailing 1s:                               
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               

			 // vpu register f4
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0c7ffc00,                                                  // Leading 1s:                                

			 // vpu register f5
			 0x4b8c4b40,                                                  // 18388608.0                                 
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                             
			 0xffc00001,                                                  // -signaling NaN                             
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                            
			 0x0c7f8000,                                                  // Leading 1s:                                

			 // vpu register f6
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x4b000000,                                                  // 8388608.0                                  
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffc00001,                                                  // -signaling NaN                             

			 // vpu register f7
			 0x00800001,                                                  // none of the mantissa set to +3ulp          
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               
			 0xcb8c4b40,                                                  // -18388608.0                                
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               

			 // vpu register f8
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7fc00001,                                                  // signaling NaN                              
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0xcb000000,                                                  // -8388608.0                                 
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f7fffff, // max norm                                      // max norm +ve                               

			 // vpu register f9
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                               
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x7fc00000,                                                  // cquiet NaN                                 
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             

			 // vpu register f10
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0c7ffc00,                                                  // Leading 1s:                                
			 0xff800000,                                                  // -inf                                       
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f11
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0003ff,                                                  // Trailing 1s:                               
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                            
			 0x0c400000,                                                  // Leading 1s:                                

			 // vpu register f12
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                               
			 0x0c7c0000,                                                  // Leading 1s:                                
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x807fffff, // max subnorm                                   // SP - ve numbers                            
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            

			 // vpu register f13
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c7fff80,                                                  // Leading 1s:                                
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e0007ff,                                                  // Trailing 1s:                               
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f14
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x0d00fff0,                                                  // Set of 1s                                  
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e01ffff,                                                  // Trailing 1s:                               
			 0x00000000,                                                  // zero                                       
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                            

			 // vpu register f15
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                             
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x55555555,                                                  // 4 random values                            
			 0x0c7fffc0,                                                  // Leading 1s:                                
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                            
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0c400000,                                                  // Leading 1s:                                
			 0xbf800000, // 1                                             // SP - ve numbers                            

			 // vpu register f16
			 0xCCCCCCCC,                                                  // 4 random values                            
			 0x0c700000,                                                  // Leading 1s:                                
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                             
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xffc00001,                                                  // -signaling NaN                             
			 0x0c7fff00,                                                  // Leading 1s:                                
			 0x0c7fff80,                                                  // Leading 1s:                                

			 // vpu register f17
			 0x00800000, // min norm                                      // subnormals +ve                             
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e0fffff,                                                  // Trailing 1s:                               
			 0xbf028f5c,                                                  // -0.51                                      
			 0x3f028f5c,                                                  // 0.51                                       

			 // vpu register f18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                            
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7f800000, // infinity                                      // SP +ve numbers                             
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x0c7f0000,                                                  // Leading 1s:                                

			 // vpu register f19
			 0x0c400000,                                                  // Leading 1s:                                
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 

			 // vpu register f20
			 0x80000001, // min subnorm                                   // SP - ve numbers                            
			 0x0e7fffff,                                                  // Trailing 1s:                               
			 0x80000000,                                                  // -zero                                      
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fc00001,                                                  // signaling NaN                              
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         

			 // vpu register f21
			 0x80800003,                                                  // none of the mantissa set to +3ulp          
			 0xffc00000,                                                  // -cquiet NaN                                
			 0x0c780000,                                                  // Leading 1s:                                
			 0x7fc00001, // QNan                                          // SP +ve numbers                             
			 0x7fc00001,                                                  // signaling NaN                              
			 0x0c7fffff,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e001fff,                                                  // Trailing 1s:                               

			 // vpu register f22
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x0e00007f,                                                  // Trailing 1s:                               
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp         
			 0x4b000000,                                                  // 8388608.0                                  
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xcb000000,                                                  // -8388608.0                                 

			 // vpu register f23
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp         
			 0x0e007fff,                                                  // Trailing 1s:                               
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xff800000,                                                  // -inf                                       
			 0x0e000007,                                                  // Trailing 1s:                               
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                             
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                             
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x0c7fffe0,                                                  // Leading 1s:                                
			 0x0e00001f,                                                  // Trailing 1s:                               
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f25
			 0xbf800000, // 1                                             // SP - ve numbers                            
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0e00ffff,                                                  // Trailing 1s:                               
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                             
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve       

			 // vpu register f26
			 0x0c7ffffc,                                                  // Leading 1s:                                
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                             
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp         
			 0x7fffffff, // QNan                                          // SP +ve numbers                             
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve       

			 // vpu register f27
			 0x0e003fff,                                                  // Trailing 1s:                               
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                               
			 0x80011111,                                                  // -9.7958E-41                                
			 0xff800001, // SNaN                                          // SP - ve numbers                            
			 0x0c7ffe00,                                                  // Leading 1s:                                
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x0d000ff0,                                                  // Set of 1s                                  
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             

			 // vpu register f28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0x0e000001,                                                  // Trailing 1s:                               
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0xffc00000,                                                  // -cquiet NaN                                
			 0xffc00001,                                                  // -signaling NaN                             
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             
			 0x7f7fffff, // max norm                                      // SP +ve numbers                             

			 // vpu register f29
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve       
			 0xffbfffff, // SNaN                                          // SP - ve numbers                            
			 0xffc00001, // QNan                                          // SP - ve numbers                            
			 0x0c7f0000,                                                  // Leading 1s:                                
			 0x0c700000,                                                  // Leading 1s:                                
			 0x80011111,                                                  // -9.7958E-41                                
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                             

			 // vpu register f30
			 0x0c7ffff0,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                             
			 0x0c7ffff8,                                                  // Leading 1s:                                
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve       
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x00800002,                                                  // none of the mantissa set to +3ulp          
			 0xff800000, // infinity                                      // SP - ve numbers                            
			 0x0c7ffff8,                                                  // Leading 1s:                                

			 // vpu register f31
			 0x0c7f8000,                                                  // Leading 1s:                                
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                            
			 0x00800003,                                                  // none of the mantissa set to +3ulp          
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                               
			 0x3f028f5c,                                                  // 0.51                                       
			 0x7f800001, // SNaN                                          // SP +ve numbers                             
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                             
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
		"fbcx.ps f25, x11\n"                                  
		"fmv.x.s x27, f14\n"                                  
		"fmsub.s f1, f13, f15, f14, rup\n"                    
		"flw.ps f1,  32 (x4)\n"
		"maskand m0, m2, m3\n"                                
		"fmulhu.pi f17, f15, f15\n"                           
		"fcvt.s.w f21, x13, dyn\n"                            
		"fmvz.x.ps x1, f12, 6\n"                              




		"fsgnjx.s f29, f12, f15\n"                            
		"fnmadd.s f1, f0, f13, f12, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"


		"fsw f12, -739(x13)\n"                                
		"flem.ps m7, f14, f1\n"                               
		"fcmovm.ps f18, f12, f1\n"                            




		"fpackreph.pi f19, f14\n"                             
		"ffrc.ps f26, f15\n"                                  
		"fmulh.pi f27, f15, f15\n"                            




		"fbci.ps f16, 18045\n"                                
		"fmin.s f20, f15, f15\n"                              
		"fclass.s x24, f1\n"                                  


		"flw.ps f20, 1773(x11)\n"                             




		//RTLMIN-5479"fsin.ps f17, f1\n"                                   
		"fclass.s x18, f1\n"                                  
		"fsgnj.s f23, f1, f1\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsetm.pi m7, f12\n"                                  
		"fbci.ps f31, 51055\n"                                


		"fbc.ps f27, -1248(x12)\n"                            
		"fslli.pi f27, f12, 8\n"                              
		"fsatu8.pi f21, f0\n"                                 
		"fsatu8.pi f22, f12\n"                                


		"fswizz.ps f30, f13, 67\n"                            
		"fandi.pi f17, f1, 70\n"                              
		"flw.ps f1,  32 (x4)\n"


		"fswizz.ps f21, f13, 110\n"                           
		"fmvz.x.ps x29, f0, 6\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.s f18, f12, f0, rne\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmaxu.pi f26, f0, f0\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"flq2 f1, 32(x14)\n"                                  
		"flw.ps f1,  32 (x4)\n"
		"fmax.pi f17, f13, f13\n"                             
		"masknot m7, m2\n"                                    
		"fnmsub.s f26, f13, f15, f15, dyn\n"                  
		"fltu.pi f22, f0, f12\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmsub.ps f18, f1, f1, f14, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"


		"fbc.ps f27, 128(x15)\n"                              


		"fswizz.ps f17, f14, 110\n"                           
		"fnmsub.s f26, f1, f12, f1, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"feqm.ps m0, f13, f13\n"                              


		"fsw f13, -1749(x14)\n"                               
		"mov.m.x m7, x1, 115\n"                               
		"fsub.s f24, f0, f0, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f21, f14, dyn\n"                         
		"fsgnjx.ps f18, f1, f13\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x20, f13, dyn\n"                            


		"fsw.ps f13, -182(x14)\n"                             
		"fsll.pi f1, f15, f1\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.s f31, f12, f0\n"                             
		"fsrl.pi f27, f13, f13\n"                             
		"cubeface.ps f18, f12, f14\n"                         
		"fmvs.x.ps x18, f12, 7\n"                             
		"fcvt.ps.f16 f30, f12\n"                              
		"fsatu8.pi f1, f1\n"                                  
		"flw.ps f1,  32 (x4)\n"
		"fpackrepb.pi f0, f0\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m0, m2, m3\n"                                 
		"fmul.s f22, f15, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fmin.s f24, f13, f0\n"                               
		"cubesgntc.ps f25, f0, f15\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.pi f1, f13, f13\n"                              
		"flw.ps f1,  32 (x4)\n"
		"cubesgntc.ps f21, f0, f14\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fbc.ps f31, 1024(x12)\n"                             
		"fand.pi f21, f14, f12\n"                             
		"maskxor m1, m2, m0\n"                                
		"fandi.pi f21, f12, 353\n"                            
		"maskor m5, m2, m4\n"                                 
		"masknot m4, m7\n"                                    
		"fclass.ps f23, f1\n"                                 
		"fand.pi f24, f12, f15\n"                             
		"fnmadd.ps f27, f0, f13, f12, dyn\n"                  
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.ps f21, f15, f14, f0, dyn\n"                  
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f18, f14, dyn\n"                         
		"fmv.s.x f26, x14\n"                                  
		"fmin.ps f21, f1, f15\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pwu.ps f21, f14, dyn\n"                         
		"cubeface.ps f18, f1, f12\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fbci.ps f16, 382221\n"                               
		"fle.ps f20, f15, f12\n"                              
		"fmsub.ps f16, f1, f15, f1, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"fclass.ps f25, f0\n"                                 
		"fbci.ps f16, 358441\n"                               
		"fsub.pi f21, f14, f12\n"                             
		"fadd.ps f27, f1, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f22, f15, f15, dyn\n"                        
		"fcvt.pwu.ps f21, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fmadd.s f21, f13, f14, f0, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.ps f25, f0, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.s f17, f1, f0, f15, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmvs.x.ps x17, f1, 4\n"                              
		"flw.ps f1,  32 (x4)\n"
		"flt.s x16, f14, f15\n"                               
		"cubefaceidx.ps f1, f1, f13\n"                        
		"flw.ps f1,  32 (x4)\n"


		"fsq2 f12, -1760(x14)\n"                              


		"flw.ps f18, -590(x12)\n"                             
		"fsgnjx.ps f27, f13, f13\n"                           
		"flt.s x26, f0, f13\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f16, f13, f12\n"                             
		"cubeface.ps f23, f1, f0\n"                           
		"flw.ps f1,  32 (x4)\n"
		"fsrai.pi f17, f15, 8\n"                              
		"fsgnjn.s f24, f13, f1\n"                             
		"fsatu8.pi f0, f15\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.x.s x28, f0\n"                                   
		"fsgnj.ps f22, f12, f13\n"                            
		"fpackreph.pi f19, f15\n"                             
		"fround.ps f28, f14, dyn\n"                           
		"fcvt.pw.ps f19, f13, rtz\n"                          
		"masknot m4, m1\n"                                    
		"feqm.ps m0, f14, f13\n"                              
		"fnmsub.s f27, f14, f15, f15, rdn\n"                  
		//RTLMIN-5479"frsq.ps f31, f12\n"                                  
		"fmin.pi f22, f14, f1\n"                              
		"fmadd.ps f25, f13, f0, f0, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"faddi.pi f0, f12, 467\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.s f22, f1, f1\n"                              
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"fsin.ps f26, f12\n"                                  
		"fbci.pi f17, 473861\n"                               
		"fmaxu.pi f25, f12, f0\n"                             
		//RTLMIN-5479"frsq.ps f17, f12\n"                                  
		"fsrl.pi f27, f15, f12\n"                             
		"faddi.pi f22, f14, 271\n"                            


		"fsw f14, 1541(x13)\n"                                
		"maskor m4, m6, m4\n"                                 
		"fnmadd.ps f27, f12, f12, f15, dyn\n"                 
		"fcvt.pw.ps f25, f1, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"
		"fmul.pi f28, f1, f14\n"                              
		"flw.ps f1,  32 (x4)\n"
		"maskpopc x24, m7\n"                                  
		"fround.ps f24, f13, dyn\n"                           
		"fsat8.pi f31, f14\n"                                 
		"flem.ps m4, f13, f0\n"                               
		"fcvt.ps.f16 f0, f14\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.w.s x21, f15, rne\n"                            
		"fcvt.w.s x23, f0, rup\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.ps f27, f15, f0\n"                            
		"fnot.pi f25, f15\n"                                  
		"fmax.ps f16, f14, f12\n"                             


		"fbc.ps f16, -1856(x11)\n"                            
		"maskxor m4, m4, m0\n"                                
		"fsetm.pi m7, f1\n"                                   
		"fcvt.pw.ps f17, f12, dyn\n"                          
		"fsrai.pi f20, f13, 0\n"                              
		"maskor m0, m3, m4\n"                                 
		//RTLMIN-5479"fsin.ps f1, f1\n"                                    
		"flw.ps f1,  32 (x4)\n"
		"fmul.ps f28, f12, f1, dyn\n"                         
		"flw.ps f1,  32 (x4)\n"
		"fmvz.x.ps x22, f12, 1\n"                             
		"fnmadd.ps f1, f1, f1, f15, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"fmsub.ps f31, f14, f1, f15, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"




		"cubeface.ps f28, f15, f15\n"                         
		"fmvz.x.ps x16, f12, 0\n"                             
		"fbcx.ps f0, x0\n"                                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"for.pi f25, f0, f14\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubeface.ps f27, f15, f14\n"                         
		"fcvt.wu.s x30, f1, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"maskxor m5, m7, m7\n"                                
		"fmulhu.pi f28, f1, f13\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fsat8.pi f26, f13\n"                                 
		"flt.pi f22, f1, f1\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fmax.s f23, f0, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fcvt.s.wu f16, x15, dyn\n"                           
		"fcvt.ps.pw f25, f13, dyn\n"                          
		"fsra.pi f20, f13, f1\n"                              
		"fcmov.ps f24, f14, f0, f14\n"                        
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubeface.ps f31, f0, f15\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrli.pi f29, f15, 10\n"                             




		"ffrc.ps f30, f14\n"                                  
		"cubesgntc.ps f31, f12, f14\n"                        
		"cubefaceidx.ps f28, f14, f15\n"                      
		"fcvt.pwu.ps f31, f12, dyn\n"                         
		"mova.m.x x0\n"                                       
		"fmv.x.s x26, f13\n"                                  
		"fbcx.ps f19, x11\n"                                  
		"feqm.ps m4, f15, f0\n"                               


		"fsw.ps f12, -13(x15)\n"                              
		//RTLMIN-5479"frsq.ps f25, f15\n"                                  
		"fsat8.pi f21, f0\n"                                  
		"fcvt.pw.ps f25, f15, dyn\n"                          
		"frcp.ps f19, f13\n"                                  
		"fmv.x.s x16, f15\n"                                  
		"fbci.ps f22, 69897\n"                                
		"fand.pi f23, f0, f0\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmaxu.pi f31, f1, f1\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f1, x14, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"maskxor m3, m4, m2\n"                                


		"flw.ps f27, 495(x12)\n"                              








		"fmaxu.pi f31, f12, f13\n"                            
		"fmul.pi f16, f1, f1\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f26, f13, f15, dyn\n"                        
		"fmin.s f30, f0, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m3, m5, m5\n"                                 
		"maskand m7, m4, m7\n"                                
		"feq.ps f25, f14, f1\n"                               
		"flem.ps m0, f0, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f17, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fxor.pi f18, f14, f1\n"                              
		"fsll.pi f18, f14, f0\n"                              
		"fsat8.pi f27, f12\n"                                 
		"fcvt.pw.ps f27, f15, dyn\n"                          
		"fsgnjn.s f17, f12, f13\n"                            

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"faddi.pi f27, f14, 414\n"                            
		"fcvt.pw.ps f20, f15, dyn\n"                          
		"flog.ps f30, f0\n"                                   
		"cubesgntc.ps f1, f15, f12\n"                         
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f1, f1, f15\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsatu8.pi f19, f1\n"                                 




		"fmulhu.pi f1, f1, f1\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fsetm.pi m7, f15\n"                                  
		"fadd.pi f31, f12, f13\n"                             
		"cubeface.ps f26, f13, f1\n"                          
		"maskand m0, m4, m3\n"                                
		"fclass.ps f21, f15\n"                                
		"mova.x.m x20\n"                                      
		"fmvz.x.ps x28, f12, 0\n"                             
		"fclass.ps f1, f1\n"                                  
		"flw.ps f1,  32 (x4)\n"
		"flem.ps m2, f1, f0\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fandi.pi f25, f15, 174\n"                            
		"fcvt.wu.s x31, f13, dyn\n"                           
		//RTLMIN-5479"fsin.ps f16, f12\n"                                  
		"fcvt.s.wu f0, x12, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrl.pi f21, f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fmaxu.pi f17, f12, f15\n"                            
		"fcmovm.ps f24, f14, f1\n"                            


		"flw f29, 371(x11)\n"                                 


		"flw.ps f22, 1941(x11)\n"                             
		"mova.x.m x26\n"                                      


		"fbc.ps f30, -1728(x13)\n"                            
		"fmulh.pi f20, f14, f15\n"                            
		"maskxor m0, m5, m7\n"                                
		"mov.m.x m1, x15, 34\n"                               
		"fsgnj.s f24, f13, f0\n"                              
		"fcvt.s.wu f19, x13, dyn\n"                           
		"fpackreph.pi f29, f14\n"                             


		"fsw f12, 1779(x12)\n"                                
		"mov.m.x m5, x15, 1\n"                                
		"ffrc.ps f22, f12\n"                                  
		"fandi.pi f0, f15, 207\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcmovm.ps f24, f13, f1\n"                            
		"fltm.ps m4, f1, f15\n"                               
		"flw.ps f1,  32 (x4)\n"






		"flq2 f21, 1824(x14)\n"                               
		"fltm.pi m6, f13, f14\n"                              
		"cubefaceidx.ps f22, f12, f0\n"                       
		"fcvt.s.wu f28, x15, dyn\n"                           
		"fle.s x22, f13, f15\n"                               
		"maskand m5, m4, m5\n"                                


		"flw.ps f1, -214(x12)\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fmin.s f22, f15, f13\n"                              
		"cubesgntc.ps f26, f0, f14\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m4, f1\n"                                   
		"feq.ps f25, f0, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0x11f46b63fed4915d\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"ffrc.ps f27, f14\n"                                  
		"fcvt.f16.ps f28, f13\n"                              
		"fcmovm.ps f30, f15, f0\n"                            
		"fltm.ps m2, f12, f12\n"                              
		"fsgnjx.ps f20, f14, f12\n"                           
		"fnmsub.s f30, f0, f13, f14, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fmul.ps f18, f13, f15, dyn\n"                        
		"fandi.pi f21, f15, 135\n"                            




		"masknot m6, m3\n"                                    


		"fsw.ps f13, -2002(x12)\n"                            
		//RTLMIN-5479"fsin.ps f27, f12\n"                                  
		"mova.m.x x14\n"                                      
		"fle.ps f19, f13, f13\n"                              
		"fmin.s f0, f14, f1\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.ps f21, f13, f0\n"                               
		"fmul.ps f16, f15, f13, rtz\n"                        
		"fbci.pi f22, 37900\n"                                
		"fmulhu.pi f28, f13, f1\n"                            
		"fnmadd.s f17, f13, f12, f1, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.s f30, f13, f0\n"                             
		"fnot.pi f30, f12\n"                                  
		"fadd.pi f0, f14, f1\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.ps f21, f15\n"                                
		"fsub.s f0, f13, f15, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fandi.pi f26, f0, 94\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubesgnsc.ps f31, f0, f12\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.pi f30, 138862\n"                               
		"mova.x.m x19\n"                                      


		"fswizz.ps f23, f13, 6\n"                             
		"fnmadd.ps f0, f1, f15, f15, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f25, f15, f13\n"                             
		"fmul.ps f20, f1, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fle.s x0, f13, f0\n"                                 
		"flog.ps f31, f0\n"                                   
		"fnot.pi f26, f12\n"                                  
		"fmul.ps f19, f13, f12, dyn\n"                        
		"fcvt.pwu.ps f26, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"maskor m6, m5, m6\n"                                 


		"flq2 f28, 1472(x11)\n"                               
		"fbcx.ps f30, x15\n"                                  
		"maskand m4, m2, m2\n"                                
		"fsgnjx.s f31, f12, f12\n"                            
		"fle.s x19, f13, f13\n"                               
		"fcvt.s.w f16, x12, rup\n"                            
		"maskpopc x25, m7\n"                                  
		"flem.ps m3, f13, f1\n"                               
		"faddi.pi f30, f14, 79\n"                             




		"fsgnjn.ps f29, f15, f1\n"                            
		"fcvt.w.s x21, f13, dyn\n"                            
		"fxor.pi f0, f14, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.f16.ps f16, f1\n"                               
		"fnot.pi f24, f12\n"                                  
		"fnmsub.ps f28, f13, f1, f15, dyn\n"                  
		"flw.ps f1,  32 (x4)\n"
		"fsgnj.s f30, f14, f14\n"                             
		"maskand m2, m3, m7\n"                                
		"feq.s x25, f14, f0\n"                                
		"fnot.pi f19, f1\n"                                   
		"fbci.pi f27, 523078\n"                               
		"fsat8.pi f19, f1\n"                                  
		"fclass.ps f1, f1\n"                                  
		"flw.ps f1,  32 (x4)\n"


		"flw f21, -2015(x11)\n"                               


		"fsw f12, -1282(x15)\n"                               
		"flt.ps f19, f0, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvz.x.ps x25, f0, 6\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.ps f29, f15, f13\n"                             
		"fsrli.pi f21, f13, 4\n"                              
		"flem.ps m4, f15, f12\n"                              
		"cubesgntc.ps f31, f1, f1\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x17, f15, dyn\n"                            
		"maskpopc x23, m5\n"                                  
		"fsub.s f17, f13, f13, rne\n"                         
		"fcvt.pwu.ps f24, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.s f26, f1, f13\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fbci.pi f23, 240699\n"                               
		"fsat8.pi f17, f1\n"                                  
		"mova.x.m x1\n"                                       
		"fsgnjn.ps f19, f0, f13\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f24, f1, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"
		"masknot m7, m1\n"                                    
		"feq.ps f21, f15, f14\n"                              




		"frcp.ps f26, f0\n"                                   
		"fbci.pi f1, 349307\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fand.pi f16, f15, f12\n"                             
		"masknot m7, m4\n"                                    
		"fmvz.x.ps x31, f12, 0\n"                             
		"fmadd.ps f1, f12, f13, f1, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"


		"flq2 f31, 128(x15)\n"                                
		"fandi.pi f22, f14, 410\n"                            
		"fcvt.ps.pw f16, f12, dyn\n"                          
		"fsat8.pi f0, f1\n"                                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m4, m7, m6\n"                                 
		"mov.m.x m2, x15, 32\n"                               
		"flt.ps f31, f0, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f28, f13, f1\n"                              
		"fmaxu.pi f29, f1, f13\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fmin.ps f29, f13, f15\n"                             
		"fltu.pi f22, f15, f13\n"                             
		"mova.x.m x1\n"                                       
		"fmv.s.x f25, x12\n"                                  

		"li x5, 0x14caf8c68d567b99\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fnmadd.s f25, f12, f1, f15, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"cubeface.ps f27, f13, f0\n"                          


		"flq2 f27, 1056(x12)\n"                               
		"fcvt.pwu.ps f18, f15, dyn\n"                         
		"fle.s x20, f15, f15\n"                               


		"fswizz.ps f18, f13, 111\n"                           
		"fminu.pi f1, f14, f12\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fnot.pi f30, f14\n"                                  
		"frcp.ps f22, f0\n"                                   
		"fmv.x.s x26, f1\n"                                   
		"maskor m5, m1, m6\n"                                 
		"maskxor m4, m7, m6\n"                                
		"fnot.pi f19, f15\n"                                  
		"flt.ps f30, f0, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fbc.ps f30, 1184(x15)\n"                             
		"fmax.ps f29, f1, f14\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fmin.ps f28, f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fadd.pi f19, f15, f0\n"                              
		"fbci.ps f20, 276534\n"                               
		"flt.s x31, f14, f15\n"                               




		"fmin.pi f28, f0, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskand m4, m1, m3\n"                                


		"flw f20, -1640(x13)\n"                               
		"fcvt.w.s x16, f13, dyn\n"                            
		"fbci.ps f23, 183318\n"                               
		"fcvt.wu.s x25, f12, dyn\n"                           




		"cubeface.ps f30, f12, f14\n"                         


		"fsw f1, -595(x14)\n"                                 
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f22, f12, dyn\n"                          
		"maskpopc x28, m4\n"                                  
		"fmvz.x.ps x0, f14, 7\n"                              
		"fsll.pi f30, f14, f1\n"                              
		"masknot m3, m7\n"                                    
		"maskpopc x20, m3\n"                                  


		"fsw.ps f15, -1075(x14)\n"                            
		"fmsub.s f19, f14, f0, f13, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.s f18, f14, f1\n"                              
		"fmvz.x.ps x18, f0, 4\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopcz x0, m6\n"                                  


		"fsgnjx.s f30, f13, f13\n"                            
		"fcvt.pwu.ps f23, f15, dyn\n"                         
		"fmul.ps f18, f15, f13, dyn\n"                        
		"fsat8.pi f23, f13\n"                                 


		"flq2 f27, 1600(x15)\n"                               
		"fsgnj.ps f26, f12, f15\n"                            
		"fsgnjx.s f18, f0, f15\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.pi f28, f0, f12\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.pi f28, f13, f13\n"                              
		"fsat8.pi f21, f15\n"                                 
		"fbci.pi f0, 71707\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsatu8.pi f18, f1\n"                                 
		"flt.s x22, f1, f12\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fmvz.x.ps x20, f12, 5\n"                             
		"fsetm.pi m6, f12\n"                                  
		"fmsub.s f24, f12, f15, f0, rne\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubeface.ps f29, f1, f14\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fmsub.s f20, f0, f14, f0, dyn\n"                     
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.s f29, f1, f14, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"mov.m.x m0, x13, 88\n"                               
		"fadd.ps f25, f13, f1, dyn\n"                         
		"flw.ps f1,  32 (x4)\n"
		"flog.ps f18, f1\n"                                   
		"fle.ps f26, f12, f0\n"                               
		"fcvt.pwu.ps f20, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m5, f1\n"                                   
		"fcvt.ps.f16 f1, f0\n"                                
		"flw.ps f1,  32 (x4)\n"


		"fsub.ps f17, f14, f12, dyn\n"                        
		"fmadd.ps f21, f12, f12, f15, dyn\n"                  
		"fexp.ps f23, f13\n"                                  
		"cubesgntc.ps f27, f12, f0\n"                         
		"fsetm.pi m0, f15\n"                                  


		"flw.ps f24, -929(x11)\n"                             
		"frcp.ps f16, f15\n"                                  
		"fpackreph.pi f22, f0\n"                              


		"fsw f12, -556(x11)\n"                                
		"maskand m1, m1, m4\n"                                
		"fsrli.pi f30, f13, 6\n"                              




		"fltm.ps m5, f14, f1\n"                               
		"maskor m1, m2, m3\n"                                 
		"faddi.pi f1, f15, 343\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x26, f15, dyn\n"                            
		"fcvt.pw.ps f18, f0, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.pi f24, 472425\n"                               


		"flq2 f20, -480(x15)\n"                               


		"flw f21, -267(x12)\n"                                
		"for.pi f23, f12, f12\n"                              


		"fsq2 f12, 672(x11)\n"                                
		"fsub.ps f17, f14, f12, dyn\n"                        
		"fmsub.s f24, f15, f13, f14, dyn\n"                   
		"fsub.s f28, f15, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"




		"fcvt.w.s x1, f15, dyn\n"                             
		"fadd.s f18, f0, f1, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"mova.m.x x1\n"                                       


		"feq.ps f19, f1, f0\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fnmsub.s f26, f15, f0, f15, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"ffrc.ps f16, f15\n"                                  




		"fsll.pi f16, f0, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.s f21, f1, f12, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f0, f13, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.s f28, f1, f15\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fltm.pi m1, f0, f14\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmin.s f21, f14, f15\n"                              
		"fsub.ps f19, f15, f13, dyn\n"                        
		"fltm.pi m1, f13, f14\n"                              
		"maskpopcz x22, m5\n"                                 
		"fsgnjn.s f29, f14, f1\n"                             
		"fmadd.s f27, f13, f13, f12, rtz\n"                   
		"faddi.pi f1, f12, 271\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f26, f14, rmm\n"                          
		"feq.s x27, f0, f12\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw f16, -182(x13)\n"                                
		"fmvs.x.ps x25, f0, 6\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.ps f0, f1, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmadd.ps f18, f1, f1, f15, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"flt.ps f26, f12, f0\n"                               


		"fsq2 f0, 384(x14)\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m6, f0, f12\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw.ps f13, -361(x13)\n"                             
		"fle.pi f27, f14, f13\n"                              
		"feq.s x1, f1, f13\n"                                 
		"flw.ps f1,  32 (x4)\n"
		"cubefaceidx.ps f16, f15, f13\n"                      
		"fmv.x.s x22, f12\n"                                  
		"fnmadd.ps f24, f13, f1, f1, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"fcmov.ps f1, f13, f0, f1\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f29, f14, dyn\n"                          
		"fpackrepb.pi f21, f14\n"                             
		"fsgnjx.s f24, f0, f15\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.f16.ps f29, f15\n"                              
		"flt.s x1, f0, f13\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f21, f14, dyn\n"                          
		"fcmov.ps f0, f12, f1, f13\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fclass.ps f0, f12\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.s x29, f1\n"                                  
		"fround.ps f24, f12, rmm\n"                           
		"feq.s x19, f13, f12\n"                               
		"fmax.pi f26, f13, f1\n"                              
		"fminu.pi f18, f0, f13\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmsub.s f18, f13, f14, f13, dyn\n"                   
		"cubefaceidx.ps f29, f15, f12\n"                      
		"fmax.s f26, f14, f12\n"                              
		"fadd.pi f28, f1, f12\n"                              
		"flw.ps f1,  32 (x4)\n"


		"fbc.ps f19, 1728(x14)\n"                             
		"fmul.pi f21, f14, f12\n"                             
		"fltm.ps m2, f0, f14\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.s f29, f12, f13\n"                              
		"fmv.x.s x0, f15\n"                                   
		"fmv.s.x f29, x13\n"                                  
		"fnmadd.ps f24, f13, f14, f1, dyn\n"                  
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f1, f13, 57\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fsub.ps f1, f12, f15, dyn\n"                         
		"flw.ps f1,  32 (x4)\n"

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmv.s.x f26, x0\n"                                   
		"fcvt.pwu.ps f28, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m5, m0, m5\n"                                 


		"fclass.s x1, f14\n"                                  
		"maskor m0, m0, m4\n"                                 


		"fsw f0, -38(x12)\n"                                  
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"frsq.ps f25, f12\n"                                  
		"fandi.pi f26, f13, 148\n"                            
		"fbci.ps f0, 389785\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m2, m0, m5\n"                                 
		"fle.pi f0, f1, f15\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"maskor m1, m0, m2\n"                                 
		"fsgnjx.s f18, f0, f0\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f28, f0, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m2, x13, 52\n"                               
		"fle.ps f30, f1, f15\n"                               
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x0\n"                                       
		"fmvs.x.ps x19, f15, 1\n"                             
		"fmv.x.s x24, f1\n"                                   
		"fmvz.x.ps x26, f0, 4\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.ps f21, f1, f14\n"                            
		"flw.ps f1,  32 (x4)\n"


		"fswizz.ps f17, f13, 107\n"                           
		"fcvt.s.wu f17, x1, dyn\n"                            
		"flog.ps f16, f14\n"                                  


		"flq2 f28, -928(x14)\n"                               
		"fsetm.pi m4, f15\n"                                  
		"fcmov.ps f20, f12, f15, f13\n"                       
		"fandi.pi f29, f12, 0\n"                              
		"fcvt.f16.ps f23, f13\n"                              
		"fmul.ps f22, f12, f12, dyn\n"                        
		"feqm.ps m6, f13, f0\n"                               
		"fnot.pi f21, f15\n"                                  
		"fsgnjn.ps f0, f12, f13\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"frsq.ps f26, f13\n"                                  
		"fsrai.pi f16, f14, 8\n"                              
		"fcvt.s.w f23, x14, dyn\n"                            
		"feq.pi f30, f14, f12\n"                              
		"fmax.ps f20, f1, f15\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fle.ps f30, f12, f1\n"                               
		"fsrl.pi f18, f12, f0\n"                              
		"fsat8.pi f21, f15\n"                                 
		"fsgnjn.s f29, f0, f15\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.ps f17, f1, f1\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.w f0, x1, rmm\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubeface.ps f1, f15, f13\n"                          
		"flw.ps f1,  32 (x4)\n"
		"mova.m.x x13\n"                                      


		"fbc.ps f24, 1600(x13)\n"                             
		"mov.m.x m4, x12, 68\n"                               
		"fmvs.x.ps x17, f14, 7\n"                             

		"li x5, 0xfedb66a77a54dd52\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs



		"flw f19, -1772(x12)\n"                               
		"fmul.pi f16, f12, f13\n"                             
		"fpackreph.pi f30, f12\n"                             
		"frcp.ps f18, f12\n"                                  
		"fltm.pi m1, f14, f0\n"                               
		"fmul.pi f0, f15, f13\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"mov.m.x m1, x11, 21\n"                               
		"fltm.pi m4, f0, f1\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fmin.pi f26, f0, f13\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"mov.m.x m4, x12, 33\n"                               


		"fsq2 f15, -96(x11)\n"                                
		"maskpopc x23, m2\n"                                  


		"flw.ps f22, 1093(x13)\n"                             


		"fswizz.ps f16, f14, 62\n"                            
		"fmax.s f25, f13, f1\n"                               


		"fsw.ps f0, 269(x11)\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.s f17, f12, f13\n"                              
		"fpackrepb.pi f25, f14\n"                             
		"frcp.ps f26, f15\n"                                  




		"flt.s x0, f14, f12\n"                                
		"flem.ps m3, f15, f14\n"                              




		"fle.pi f31, f15, f13\n"                              


		"flq2 f19, 0(x13)\n"                                  
		"fmax.ps f30, f13, f12\n"                             
		"fsgnjx.s f31, f1, f0\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fle.ps f1, f13, f14\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f0, f1, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f0, f1, 3\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"maskpopc x20, m7\n"                                  
		"cubeface.ps f22, f13, f1\n"                          
		"fsetm.pi m0, f1\n"                                   
		"feq.pi f23, f14, f14\n"                              
		"fsll.pi f17, f15, f0\n"                              
		"fsgnjn.ps f1, f1, f13\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fmadd.s f19, f1, f13, f13, rdn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"maskpopcz x31, m2\n"                                 
		"fbcx.ps f25, x14\n"                                  
		//RTLMIN-5479"fsin.ps f31, f1\n"                                   
		"cubesgnsc.ps f22, f12, f0\n"                         
		"fbci.pi f22, 361927\n"                               
		"fmulh.pi f21, f15, f0\n"                             
		"for.pi f0, f0, f13\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw f13, -1649(x14)\n"                               
		"fsgnj.ps f23, f14, f12\n"                            
		"fcvt.pw.ps f1, f1, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x28, f1\n"                                  
		"fmaxu.pi f25, f14, f14\n"                            
		"feq.s x23, f13, f13\n"                               
		"fmul.ps f30, f12, f14, dyn\n"                        

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flt.pi f26, f14, f14\n"                              
		"faddi.pi f27, f15, 127\n"                            
		"flog.ps f27, f13\n"                                  
		"fcvt.pw.ps f29, f0, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fcmovm.ps f26, f15, f14\n"                           
		//RTLMIN-5479"frsq.ps f0, f0\n"                                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskxor m4, m4, m3\n"                                


		"fcvt.ps.pw f30, f1, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f0, f13, 5\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrai.pi f30, f13, 14\n"                             
		"feq.pi f27, f13, f1\n"                               
		"fround.ps f27, f13, dyn\n"                           
		"fbci.ps f22, 480841\n"                               
		"fmax.ps f22, f1, f15\n"                              
		"flw.ps f1,  32 (x4)\n"


		"fbc.ps f17, -1184(x11)\n"                            
		"fsatu8.pi f28, f14\n"                                
		"mova.m.x x15\n"                                      
		"cubesgnsc.ps f0, f14, f0\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feqm.ps m0, f0, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.pi f28, 152399\n"                               
		"fbci.ps f16, 66110\n"                                




		"fclass.s x1, f1\n"                                   
		//RTLMIN-5479"fsin.ps f25, f15\n"                                  
		"fmvz.x.ps x23, f15, 2\n"                             
		"fnot.pi f27, f14\n"                                  
		"fmax.s f0, f0, f0\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"faddi.pi f20, f13, 107\n"                            
		"fcvt.wu.s x27, f15, dyn\n"                           
		"fnmadd.ps f21, f0, f14, f12, dyn\n"                  
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.pi f16, f15, f1\n"                               
		"frcp.ps f18, f1\n"                                   
		"fmvs.x.ps x24, f1, 3\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f25, f12, f13, dyn\n"                        
		"fsrl.pi f30, f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f1, f1, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fmv.x.s x27, f0\n"                                   
		"faddi.pi f20, f13, 470\n"                            


		"fbc.ps f23, 960(x14)\n"                              
		"fmin.s f26, f13, f13\n"                              
		"cubeface.ps f30, f15, f1\n"                          
		"fmin.pi f16, f12, f0\n"                              
		"fexp.ps f19, f14\n"                                  
		"fsll.pi f29, f1, f13\n"                              
		"flw.ps f1,  32 (x4)\n"
		"feq.ps f20, f1, f12\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fclass.ps f22, f1\n"                                 
		"fmv.x.s x24, f15\n"                                  


		"fnmsub.s f1, f14, f15, f0, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcmovm.ps f19, f12, f15\n"                           
		"fnmsub.ps f28, f13, f14, f1, dyn\n"                  
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"fsin.ps f0, f15\n"                                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0x5eb3ba576ace3f30\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsrli.pi f28, f0, 9\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmaxu.pi f23, f1, f13\n"                             
		"flw.ps f1,  32 (x4)\n"




		"fle.pi f28, f14, f1\n"                               
		"fcvt.ps.pwu f30, f13, dyn\n"                         
		"fsll.pi f29, f12, f14\n"                             
		"cubefaceidx.ps f20, f12, f1\n"                       


		"fbc.ps f1, 1600(x13)\n"                              
		"flw.ps f1,  32 (x4)\n"
		"flt.s x21, f12, f14\n"                               




		"flt.pi f19, f15, f15\n"                              
		"fsgnj.s f1, f14, f13\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x1, f12, dyn\n"                             
		"fmax.pi f19, f15, f13\n"                             
		"fsrli.pi f17, f12, 14\n"                             
		"fxor.pi f29, f12, f12\n"                             


		"fswizz.ps f16, f14, 116\n"                           
		"fnmadd.ps f0, f14, f15, f13, dyn\n"                  
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"masknot m2, m4\n"                                    
		"flt.s x1, f0, f0\n"                                  
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f18, f12, dyn\n"                         
		"fcvt.s.w f26, x13, dyn\n"                            
		"feq.pi f31, f15, f15\n"                              
		"feq.ps f24, f14, f14\n"                              
		"flt.pi f25, f12, f0\n"                               
		"cubefaceidx.ps f31, f12, f0\n"                       
		"fxor.pi f26, f14, f14\n"                             
		"flog.ps f21, f14\n"                                  
		"fmsub.s f30, f15, f14, f1, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"fmv.x.s x17, f1\n"                                   
		"fcvt.f16.ps f27, f14\n"                              
		"fle.ps f20, f14, f12\n"                              
		"fsgnjx.s f19, f1, f14\n"                             
		"flw.ps f1,  32 (x4)\n"
		"maskand m7, m7, m2\n"                                
		"fsgnjx.ps f16, f14, f15\n"                           
		"flt.pi f19, f14, f15\n"                              


		//RTLMIN-5479"frsq.ps f29, f13\n"                                  
		"masknot m5, m3\n"                                    


		"flw.ps f20, -2007(x13)\n"                            
		"fle.ps f0, f12, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopcz x1, m5\n"                                  
		"fcmovm.ps f26, f1, f1\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fbci.pi f26, 224799\n"                               


		"fbc.ps f31, 1056(x11)\n"                             
		"fclass.s x31, f13\n"                                 
		"feqm.ps m7, f1, f15\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsll.pi f0, f1, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"




		"fmul.ps f28, f14, f15, rup\n"                        
		"maskpopc x19, m1\n"                                  


		"fadd.s f1, f12, f14, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f17, x0, dyn\n"                            


		"fbc.ps f0, -736(x12)\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"maskpopcz x16, m0\n"                                 




		"maskand m5, m0, m5\n"                                
		"flt.pi f24, f15, f13\n"                              


		"flw f24, 1748(x14)\n"                                


		"fsw f15, 1595(x14)\n"                                
		"fmsub.ps f31, f15, f1, f0, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"




		"fadd.ps f22, f13, f12, dyn\n"                        
		"fadd.pi f16, f12, f12\n"                             
		"fcvt.ps.pwu f27, f15, dyn\n"                         
		//RTLMIN-5479"frsq.ps f20, f15\n"                                  
		"fmulhu.pi f18, f13, f13\n"                           
		"fsgnjx.ps f29, f12, f1\n"                            
		"fclass.s x1, f15\n"                                  
		"mova.m.x x0\n"                                       
		"fpackreph.pi f1, f12\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.ps f30, f0, f15\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmadd.s f25, f0, f14, f1, dyn\n"                     
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsatu8.pi f28, f14\n"                                
		"fcvt.wu.s x17, f0, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m6, f12, f15\n"                              
		"fsgnj.ps f21, f15, f12\n"                            
		"fpackrepb.pi f22, f1\n"                              
		"cubesgntc.ps f17, f1, f0\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fnmsub.ps f31, f1, f15, f13, dyn\n"                  
		"flw.ps f1,  32 (x4)\n"
		"masknot m5, m0\n"                                    
		"fle.ps f17, f12, f0\n"                               
		"fandi.pi f19, f13, 224\n"                            
		"fminu.pi f29, f13, f14\n"                            
		"fltm.ps m5, f1, f1\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f31, x1, dyn\n"                            
		"fsat8.pi f26, f1\n"                                  
		"fsgnjn.ps f16, f15, f15\n"                           
		"fltu.pi f20, f0, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrli.pi f1, f0, 14\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pw f16, f15, dyn\n"                          
		"mova.x.m x26\n"                                      
		"frcp.ps f0, f13\n"                                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f22, f1, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"
		"fmaxu.pi f21, f1, f12\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fbcx.ps f1, x1\n"                                    
		"flw.ps f1,  32 (x4)\n"
		"fmadd.s f26, f13, f13, f12, dyn\n"                   


		"flw f20, 16(x15)\n"                                  
		"flog.ps f17, f1\n"                                   
		"fpackreph.pi f16, f12\n"                             
		"flem.ps m1, f0, f12\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskxor m7, m4, m5\n"                                
		"fsub.ps f18, f15, f12, dyn\n"                        
		"fmax.ps f24, f14, f15\n"                             
		"fcmov.ps f21, f14, f14, f14\n"                       

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fltm.pi m0, f15, f0\n"                               


		"fbc.ps f27, -896(x15)\n"                             




		"fsgnjn.ps f24, f1, f15\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fmvs.x.ps x20, f12, 4\n"                             
		"for.pi f24, f1, f12\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsetm.pi m0, f12\n"                                  
		"fsub.ps f1, f12, f12, dyn\n"                         
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.pwu f23, f15, dyn\n"                         
		"fle.ps f20, f1, f12\n"                               
		"flw.ps f1,  32 (x4)\n"


		"fsw.ps f12, 394(x11)\n"                              
		"cubesgnsc.ps f20, f1, f1\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fmsub.s f24, f15, f12, f0, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feqm.ps m0, f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"


		"fsw f0, -356(x14)\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopc x1, m0\n"                                   
		"fcvt.wu.s x0, f13, dyn\n"                            
		"fcvt.ps.f16 f19, f1\n"                               
		"flt.s x24, f12, f1\n"                                
		"fsrl.pi f27, f15, f1\n"                              
		"mova.x.m x20\n"                                      
		"fcvt.wu.s x0, f15, dyn\n"                            




		"fadd.pi f1, f12, f1\n"                               
		"flw.ps f1,  32 (x4)\n"
		"mov.m.x m0, x11, 126\n"                              
		"fmax.s f28, f1, f1\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fmv.s.x f24, x0\n"                                   


		"fsw f0, -520(x11)\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.wu.s x0, f0, dyn\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.s f16, f0, f15, f1, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"mov.m.x m4, x15, 4\n"                                
		"fmax.pi f16, f0, f14\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvz.x.ps x21, f0, 2\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubeface.ps f20, f14, f0\n"                          
		"fmv.x.s x31, f1\n"                                   
		"fmv.s.x f26, x0\n"                                   
		"fbci.pi f31, 477447\n"                               
		"fadd.pi f1, f1, f12\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fclass.ps f25, f0\n"                                 
		"fsatu8.pi f31, f15\n"                                
		"feq.s x27, f15, f12\n"                               
		"fsll.pi f28, f1, f15\n"                              
		"flw.ps f1,  32 (x4)\n"
		"flt.pi f18, f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"
		"cubeface.ps f27, f14, f12\n"                         








		"flt.pi f18, f14, f14\n"                              
		"fmulh.pi f17, f12, f12\n"                            
		"flog.ps f31, f14\n"                                  
		"fsatu8.pi f19, f13\n"                                
		"fsub.pi f23, f15, f13\n"                             
		"maskpopc x16, m2\n"                                  


		"fsq2 f15, -736(x11)\n"                               

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flt.pi f31, f12, f1\n"                               
		"fcmov.ps f29, f1, f13, f1\n"                         
		"flw.ps f1,  32 (x4)\n"
		"feq.ps f1, f14, f13\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f25, f14, 6\n"                              
		"fcvt.pwu.ps f20, f12, dyn\n"                         
		"maskand m5, m2, m4\n"                                
		"fnot.pi f24, f0\n"                                   
		"fmin.s f19, f12, f14\n"                              
		"fsatu8.pi f19, f1\n"                                 
		"fltm.ps m2, f12, f14\n"                              
		"maskand m7, m4, m1\n"                                
		"fmvs.x.ps x26, f0, 2\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjn.ps f30, f1, f13\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f29, f15, f0\n"                              
		"fandi.pi f27, f0, 109\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubesgntc.ps f0, f15, f15\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f16, f12, dyn\n"                          
		"fcvt.ps.f16 f1, f0\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fnmadd.s f29, f13, f14, f13, rtz\n"                  
		"fand.pi f17, f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"
		"ffrc.ps f16, f15\n"                                  
		"fmul.ps f24, f1, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f24, f15, f12, dyn\n"                        
		"fcvt.wu.s x25, f0, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw.ps f0, 411(x12)\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrai.pi f29, f13, 9\n"                              
		"fle.pi f20, f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x27, f12\n"                                 
		"flt.ps f26, f15, f0\n"                               




		"feq.ps f31, f15, f13\n"                              
		"maskand m6, m3, m6\n"                                
		"fcvt.wu.s x24, f13, dyn\n"                           
		"fsra.pi f21, f13, f15\n"                             
		"fsatu8.pi f28, f1\n"                                 
		//RTLMIN-5479"fsin.ps f22, f1\n"                                   
		"flog.ps f31, f1\n"                                   
		"fle.s x24, f12, f14\n"                               
		"fcvt.ps.f16 f22, f14\n"                              
		"for.pi f21, f0, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.ps f30, 383511\n"                               
		"fsrai.pi f26, f13, 0\n"                              
		"fslli.pi f20, f13, 13\n"                             




		"fand.pi f30, f14, f12\n"                             
		"fround.ps f29, f12, dyn\n"                           
		"fnmsub.s f24, f1, f13, f15, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"frcp.ps f18, f0\n"                                   
		"fmul.pi f0, f15, f14\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmsub.s f16, f13, f14, f14, dyn\n"                   
		"maskor m3, m2, m2\n"                                 


		"flw.ps f30, -228(x14)\n"                             

		"li x5, 0x6d4618539a0efbb8\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"cubefaceidx.ps f17, f14, f0\n"                       
		"fsrai.pi f17, f1, 11\n"                              
		"flw.ps f1,  32 (x4)\n"
		"for.pi f30, f0, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flq2 f25, -544(x12)\n"                               
		"fbci.pi f25, 120270\n"                               


		"fsq2 f13, -128(x15)\n"                               
		"fclass.ps f23, f15\n"                                
		"fcvt.s.w f16, x15, dyn\n"                            
		"fltm.pi m3, f15, f12\n"                              
		"cubeface.ps f28, f1, f0\n"                           
		"flw.ps f1,  32 (x4)\n"
		"fltm.ps m4, f0, f1\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.pi f18, f14, f12\n"                              
		"cubeface.ps f17, f12, f1\n"                          
		"flem.ps m5, f13, f0\n"                               
		"fmax.s f29, f0, f14\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw.ps f14, 949(x14)\n"                              
		"fexp.ps f19, f1\n"                                   
		"fsub.s f0, f15, f15, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flq2 f1, 1824(x12)\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fsll.pi f16, f14, f1\n"                              


		"fsw.ps f0, 1003(x14)\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fcvt.ps.pw f17, f1, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"
		"fsll.pi f21, f12, f13\n"                             
		"fandi.pi f30, f12, 215\n"                            
		"fmul.s f1, f12, f14, rup\n"                          
		"flw.ps f1,  32 (x4)\n"
		"feq.pi f19, f0, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f19, f12, f12\n"                             
		"fbci.ps f31, 127007\n"                               


		"flw.ps f18, 1876(x14)\n"                             
		"fsgnjx.ps f26, f13, f15\n"                           
		"fltm.pi m7, f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"


		"flt.ps f0, f15, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrli.pi f25, f12, 6\n"                              
		"fle.s x1, f12, f12\n"                                
		"fsub.ps f24, f1, f12, rne\n"                         
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.w f24, x1, rup\n"                             
		"fbci.pi f19, 70411\n"                                
		"fmadd.s f17, f12, f1, f0, dyn\n"                     
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsetm.pi m1, f13\n"                                  
		"fsgnj.ps f1, f14, f0\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fle.pi f21, f15, f1\n"                               
		"fltm.pi m5, f14, f0\n"                               
		"fcmov.ps f0, f12, f0, f0\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw f1, -1038(x14)\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.ps f21, f15, f13\n"                           
		"fmax.pi f16, f14, f14\n"                             
		"fpackrepb.pi f27, f0\n"                              
		"feqm.ps m0, f15, f13\n"                              
		"fand.pi f18, f1, f14\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fmsub.ps f20, f0, f12, f14, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fcvt.pw.ps f31, f1, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x26\n"                                      
		"fmul.ps f24, f12, f13, dyn\n"                        
		"for.pi f0, f14, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"fsin.ps f26, f12\n"                                  
		"fpackrepb.pi f0, f1\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmin.pi f17, f13, f12\n"                             
		"fmin.pi f18, f0, f14\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f16, f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsrl.pi f30, f1, f13\n"                              
		"flw.ps f1,  32 (x4)\n"
		"feq.pi f0, f14, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f31, f0, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.pi f16, f1, f0\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.s f31, f14, f12\n"                            
		"fsgnjx.s f19, f13, f1\n"                             
		"fmin.ps f21, f0, f12\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.w f25, x12, dyn\n"                            
		"feq.ps f21, f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"




		"maskor m5, m5, m3\n"                                 
		"fcvt.w.s x24, f12, dyn\n"                            
		"cubeface.ps f1, f13, f14\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fsll.pi f0, f13, f13\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fbc.ps f29, -800(x12)\n"                             
		"fsrai.pi f27, f0, 11\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.pi f17, f13, f15\n"                              
		"fbci.pi f16, 81726\n"                                
		"fcvt.ps.pwu f17, f15, dyn\n"                         
		"feq.pi f28, f12, f14\n"                              
		"faddi.pi f21, f12, 366\n"                            




		"fcvt.ps.f16 f16, f0\n"                               


		"flw.ps f20, -560(x11)\n"                             
		"fcvt.w.s x24, f13, dyn\n"                            
		"mova.m.x x0\n"                                       


		"fcvt.wu.s x28, f12, dyn\n"                           
		"flem.ps m7, f12, f1\n"                               
		"fsub.ps f1, f1, f0, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f27, f14, f15, dyn\n"                        
		"fbci.pi f18, 364379\n"                               
		"fsgnjn.s f27, f13, f0\n"                             
		"fbci.ps f22, 289949\n"                               
		"fslli.pi f16, f12, 13\n"                             
		"fsgnjn.ps f19, f12, f14\n"                           
		"fsgnjx.s f22, f13, f1\n"                             
		"fmax.pi f24, f15, f13\n"                             


		"fbc.ps f22, -544(x12)\n"                             
		"fmax.pi f25, f13, f12\n"                             
		"fsgnj.ps f25, f13, f13\n"                            
		"fmul.pi f18, f12, f13\n"                             
		"fadd.s f25, f12, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw f1, 1520(x12)\n"                                 
		"flw.ps f1,  32 (x4)\n"

		"li x5, 0xa6adbef8a9c5e9d0\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fbci.pi f22, 225875\n"                               
		"fxor.pi f0, f14, f0\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m4, m0, m4\n"                                 
		"frcp.ps f17, f1\n"                                   


		"flw f22, -1364(x15)\n"                               
		"fcvt.wu.s x18, f13, dyn\n"                           
		"ffrc.ps f24, f12\n"                                  
		"flt.s x28, f13, f14\n"                               
		"faddi.pi f25, f1, 62\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fmax.s f18, f14, f13\n"                              
		"fbcx.ps f29, x13\n"                                  
		"fcmovm.ps f0, f1, f14\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"


		"fnmsub.ps f22, f14, f14, f1, dyn\n"                  
		"flw.ps f1,  32 (x4)\n"
		"fnmadd.s f16, f12, f12, f0, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.ps f29, f14, f13\n"                              
		"mova.x.m x29\n"                                      
		"fmulh.pi f22, f12, f1\n"                             
		"fpackrepb.pi f20, f15\n"                             
		"fmadd.ps f27, f14, f12, f12, dyn\n"                  
		"fsgnjn.s f21, f14, f12\n"                            
		//RTLMIN-5479"fsin.ps f25, f12\n"                                  
		"fcmov.ps f28, f1, f0, f13\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmin.ps f20, f12, f12\n"                             
		"fxor.pi f30, f15, f1\n"                              
		"fsgnjx.ps f22, f15, f1\n"                            
		"fsat8.pi f22, f12\n"                                 
		"fmvs.x.ps x0, f14, 6\n"                              
		"fmul.s f24, f0, f13, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmsub.ps f29, f1, f1, f12, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"fcvt.ps.f16 f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"
		"cubesgnsc.ps f0, f1, f12\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f26, f15, f12\n"                             
		"fsatu8.pi f20, f14\n"                                
		"fcvt.pwu.ps f23, f13, dyn\n"                         
		"fsra.pi f20, f12, f0\n"                              
		"fcmovm.ps f21, f15, f0\n"                            
		"flog.ps f19, f12\n"                                  
		"fsgnjx.s f22, f13, f0\n"                             
		"fcvt.pw.ps f17, f13, dyn\n"                          
		"fsub.ps f17, f14, f12, dyn\n"                        
		"fbcx.ps f28, x13\n"                                  
		"mov.m.x m2, x15, 53\n"                               
		"flt.pi f0, f0, f1\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackrepb.pi f24, f1\n"                              
		"fcvt.pwu.ps f18, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulhu.pi f19, f14, f14\n"                           
		"fnmsub.s f25, f15, f0, f14, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltu.pi f31, f14, f0\n"                              
		"maskor m0, m1, m2\n"                                 
		"fsgnjn.s f29, f13, f14\n"                            

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmulh.pi f0, f1, f1\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsat8.pi f1, f0\n"                                   
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x16, f1\n"                                  
		"fsrl.pi f0, f14, f1\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f1, f0, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f19, f12, 3\n"                              








		"mova.m.x x0\n"                                       
		"fmvz.x.ps x22, f14, 7\n"                             
		"faddi.pi f26, f1, 244\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fsgnj.s f30, f12, f15\n"                             
		"fle.pi f17, f13, f0\n"                               
		"fclass.ps f16, f0\n"                                 
		"masknot m2, m6\n"                                    
		"fadd.s f18, f13, f15, dyn\n"                         
		"fle.pi f25, f0, f14\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.s f21, f15, f15\n"                             
		"fsgnjx.s f17, f12, f14\n"                            
		"fsrai.pi f20, f1, 11\n"                              
		"flw.ps f1,  32 (x4)\n"
		"cubefaceidx.ps f1, f12, f1\n"                        
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f31, x15, dyn\n"                           
		"fcmov.ps f31, f12, f0, f15\n"                        
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.ps f21, f14\n"                                
		"fadd.pi f30, f15, f15\n"                             
		"fmaxu.pi f17, f12, f1\n"                             
		"mova.m.x x11\n"                                      
		"fmax.s f22, f1, f12\n"                               
		"flw.ps f1,  32 (x4)\n"
		"flem.ps m3, f15, f12\n"                              
		"fnmadd.s f20, f14, f12, f12, rup\n"                  
		"fltu.pi f18, f1, f1\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fmsub.s f28, f14, f13, f1, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"fmadd.ps f0, f0, f1, f12, dyn\n"                     
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"cubefaceidx.ps f22, f14, f12\n"                      
		"faddi.pi f17, f12, 440\n"                            
		"fmvz.x.ps x1, f15, 2\n"                              
		"feq.s x17, f0, f15\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pwu.ps f17, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fmulhu.pi f24, f15, f13\n"                           


		"fbc.ps f17, 1024(x12)\n"                             
		"fpackreph.pi f26, f14\n"                             
		"fsatu8.pi f0, f14\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrai.pi f24, f15, 5\n"                              
		"fmul.pi f18, f12, f0\n"                              
		"fcvt.ps.pwu f27, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.ps f24, f12, f0\n"                             
		"flt.pi f16, f12, f15\n"                              
		"fsat8.pi f30, f15\n"                                 
		"fnmsub.ps f23, f1, f14, f12, rmm\n"                  
		"flw.ps f1,  32 (x4)\n"
		"cubefaceidx.ps f28, f13, f13\n"                      
		"fmadd.s f24, f14, f13, f15, dyn\n"                   
		"fsrai.pi f31, f0, 7\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsll.pi f16, f0, f1\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsetm.pi m6, f1\n"                                   
		"cubeface.ps f16, f15, f13\n"                         
		"fmsub.ps f0, f12, f14, f15, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.x.m x24\n"                                      


		"fsq2 f1, -320(x14)\n"                                
		"flw.ps f1,  32 (x4)\n"
		"feq.ps f29, f0, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw.ps f12, 796(x13)\n"                              




		"feq.ps f23, f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x29, f0\n"                                  
		//RTLMIN-5479"frsq.ps f1, f1\n"                                    
		"flw.ps f1,  32 (x4)\n"
		"mova.m.x x0\n"                                       
		"fsetm.pi m5, f0\n"                                   
		"fadd.ps f24, f1, f12, dyn\n"                         
		"flw.ps f1,  32 (x4)\n"








		"maskxor m7, m6, m1\n"                                
		"fxor.pi f23, f1, f15\n"                              
		"flw.ps f1,  32 (x4)\n"
		//RTLMIN-5479"frsq.ps f25, f0\n"                                   


		"flw f28, 1151(x12)\n"                                
		"fmadd.s f31, f15, f1, f12, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"fltm.pi m1, f13, f15\n"                              
		"for.pi f27, f15, f15\n"                              


		"cubesgntc.ps f22, f1, f12\n"                         
		"flw.ps f1,  32 (x4)\n"




		"fnot.pi f28, f15\n"                                  
		"fexp.ps f27, f14\n"                                  
		"mov.m.x m4, x14, 12\n"                               
		"fmvs.x.ps x18, f15, 7\n"                             
		"fsrli.pi f24, f13, 7\n"                              
		"mova.x.m x16\n"                                      
		"fmadd.s f1, f0, f15, f0, dyn\n"                      
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.f16.ps f18, f1\n"                               
		"fcvt.s.wu f0, x14, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.ps f19, f13, f13\n"                             


		"flq2 f24, -2016(x12)\n"                              
		"fcvt.w.s x27, f12, dyn\n"                            
		"fmsub.s f1, f1, f14, f13, dyn\n"                     
		"flw.ps f1,  32 (x4)\n"
		"fmulhu.pi f30, f15, f13\n"                           
		"fsgnjn.ps f21, f15, f1\n"                            
		"fcvt.ps.pw f18, f12, dyn\n"                          
		"flem.ps m0, f12, f0\n"                               
		"fsub.pi f24, f0, f14\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fbc.ps f16, 576(x11)\n"                              
		"fsgnjx.ps f28, f1, f0\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f1, f14, 11\n"                              
		"flw.ps f1,  32 (x4)\n"


		"flw f19, -563(x12)\n"                                
		"fmin.s f26, f15, f14\n"                              
		"fle.ps f30, f13, f0\n"                               
		"ffrc.ps f28, f12\n"                                  
		"fbci.ps f27, 161473\n"                               
		"fmul.pi f27, f14, f12\n"                             
		"fsatu8.pi f17, f12\n"                                

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmaxu.pi f19, f0, f15\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw.ps f1, 792(x11)\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fnmadd.ps f22, f13, f15, f1, dyn\n"                  
		"flw.ps f1,  32 (x4)\n"
		"maskor m1, m0, m2\n"                                 
		"flem.ps m1, f15, f0\n"                               
		"fcvt.ps.pwu f17, f14, dyn\n"                         
		"fle.ps f18, f13, f0\n"                               
		"ffrc.ps f30, f0\n"                                   
		"fmax.ps f29, f14, f0\n"                              


		"fswizz.ps f16, f1, 113\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fsat8.pi f31, f12\n"                                 
		"fmadd.s f18, f13, f14, f14, rne\n"                   
		"fmin.ps f22, f0, f1\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fadd.s f25, f15, f12, dyn\n"                         
		//RTLMIN-5479"fsin.ps f31, f15\n"                                  
		"fmsub.ps f29, f15, f12, f14, dyn\n"                  
		"fcmov.ps f31, f14, f15, f1\n"                        
		"fsub.pi f26, f14, f1\n"                              
		"flog.ps f18, f1\n"                                   
		"cubesgntc.ps f0, f14, f13\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.w f20, x0, dyn\n"                             
		"fcvt.ps.pw f20, f15, dyn\n"                          
		"fltu.pi f16, f15, f13\n"                             


		"fmvz.x.ps x24, f0, 2\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m1, x14, 38\n"                               
		"fmin.s f16, f1, f0\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fround.ps f23, f12, dyn\n"                           
		"fcvt.f16.ps f29, f14\n"                              
		"maskpopc x29, m0\n"                                  
		"fcvt.wu.s x30, f0, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubefaceidx.ps f18, f13, f12\n"                      
		"fmin.s f28, f15, f13\n"                              
		"feqm.ps m0, f0, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fandi.pi f25, f0, 45\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feqm.ps m7, f15, f0\n"                               
		"fnmadd.s f22, f0, f1, f15, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f23, f13, 0\n"                              
		"fmadd.s f20, f1, f1, f12, dyn\n"                     
		"flw.ps f1,  32 (x4)\n"
		"fmsub.ps f22, f12, f14, f15, dyn\n"                  
		"cubeface.ps f30, f15, f0\n"                          
		"fle.s x25, f12, f15\n"                               
		"fbci.pi f17, 446561\n"                               
		"fsgnjn.s f24, f15, f15\n"                            
		"fbci.ps f20, 329348\n"                               
		"fmaxu.pi f25, f0, f0\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.pi f29, f1, f13\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x25, f13\n"                                 
		"faddi.pi f29, f0, 468\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xd9674101ee756e43\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"mova.m.x x12\n"                                      
		"fcvt.wu.s x31, f1, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fsgnj.ps f1, f13, f0\n"                              
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x30\n"                                      
		"mov.m.x m4, x0, 125\n"                               
		"maskor m4, m5, m7\n"                                 




		"fltu.pi f0, f14, f14\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.pi f0, 506441\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmadd.ps f18, f12, f15, f13, dyn\n"                  




		"maskand m1, m5, m2\n"                                
		"fnmsub.ps f17, f13, f14, f14, dyn\n"                 
		"fcmov.ps f31, f13, f15, f13\n"                       
		"fbcx.ps f17, x0\n"                                   
		"feq.ps f26, f12, f13\n"                              




		"fmax.s f18, f0, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.s x18, f1, f14\n"                                
		"flw.ps f1,  32 (x4)\n"
		"feqm.ps m6, f13, f12\n"                              
		"maskxor m2, m0, m2\n"                                


		"flw f25, 896(x13)\n"                                 


		"fswizz.ps f19, f1, 15\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fpackreph.pi f29, f13\n"                             
		"maskpopc x16, m0\n"                                  


		"flw.ps f29, 1132(x11)\n"                             
		"fsgnjn.ps f23, f15, f0\n"                            
		"fcvt.s.w f16, x0, dyn\n"                             
		"fcvt.s.w f23, x11, dyn\n"                            
		"fadd.ps f20, f13, f15, rtz\n"                        
		"fmin.s f16, f14, f0\n"                               
		"fandi.pi f25, f14, 483\n"                            
		"fsrl.pi f17, f0, f13\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pw f31, f12, dyn\n"                          
		"fsatu8.pi f28, f0\n"                                 




		"fltm.ps m1, f13, f14\n"                              
		"fmadd.s f27, f1, f14, f12, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"fadd.pi f22, f15, f1\n"                              
		"fxor.pi f23, f12, f13\n"                             
		"fmvz.x.ps x0, f0, 4\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmsub.s f16, f14, f0, f12, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubefaceidx.ps f19, f12, f12\n"                      


		"flw f30, -653(x15)\n"                                




		"fadd.s f0, f13, f13, rup\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackreph.pi f24, f14\n"                             
		"fsrl.pi f29, f15, f12\n"                             
		"fcvt.s.wu f22, x14, dyn\n"                           
		"fltm.pi m7, f12, f14\n"                              
		"fslli.pi f22, f15, 10\n"                             






		"fbc.ps f17, 1888(x12)\n"                             
		"masknot m0, m7\n"                                    
		"fsgnjx.ps f22, f12, f12\n"                           
		"fnmsub.ps f19, f0, f0, f15, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsgnjn.ps f25, f0, f12\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.ps f22, f15, f14\n"                            


		"fbc.ps f20, -1216(x11)\n"                            
		"fnmsub.s f26, f13, f15, f0, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pw.ps f19, f15, dyn\n"                          
		"fltm.ps m5, f13, f0\n"                               
		"fsatu8.pi f22, f12\n"                                
		"fmsub.ps f22, f12, f15, f12, dyn\n"                  
		"fcvt.pw.ps f16, f15, dyn\n"                          
		"maskpopcz x31, m2\n"                                 
		"fpackreph.pi f19, f0\n"                              








		"fle.s x26, f14, f15\n"                               
		"fsra.pi f29, f12, f15\n"                             
		"fpackrepb.pi f31, f1\n"                              
		"ffrc.ps f24, f15\n"                                  






		"flq2 f16, -192(x14)\n"                               
		"fmv.s.x f27, x11\n"                                  
		"fsetm.pi m7, f12\n"                                  
		"masknot m6, m6\n"                                    
		"fmin.ps f0, f1, f12\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmulh.pi f31, f1, f12\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fcvt.f16.ps f19, f0\n"                               
		"fmul.pi f28, f13, f0\n"                              
		"fnot.pi f26, f0\n"                                   
		"fandi.pi f29, f15, 377\n"                            
		"fcvt.ps.f16 f29, f1\n"                               
		"fsatu8.pi f1, f1\n"                                  
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.s f20, f14, f0\n"                             
		"feq.ps f28, f14, f1\n"                               
		"fsatu8.pi f18, f0\n"                                 
		"fcvt.ps.f16 f17, f1\n"                               
		"fsetm.pi m2, f15\n"                                  
		"fnot.pi f20, f15\n"                                  
		"fmvz.x.ps x28, f15, 5\n"                             
		"feqm.ps m5, f1, f0\n"                                
		"flw.ps f1,  32 (x4)\n"
		"mova.m.x x13\n"                                      
		"cubeface.ps f22, f14, f13\n"                         
		"fminu.pi f0, f12, f1\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fmul.pi f29, f12, f13\n"                             
		"flog.ps f19, f0\n"                                   
		"fcvt.ps.pwu f30, f12, dyn\n"                         
		"mov.m.x m0, x14, 29\n"                               




		"fpackreph.pi f21, f14\n"                             
		"fsub.pi f29, f0, f12\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskor m2, m3, m7\n"                                 


		"fsw.ps f15, -1944(x11)\n"                            
		"fxor.pi f16, f0, f1\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f17, f15, f1\n"                              
		"fcvt.pwu.ps f22, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0x96b046a8dd0268b6\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"maskor m4, m2, m0\n"                                 
		"fmax.pi f24, f13, f0\n"                              
		"fmin.ps f1, f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f26, f0, 426\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.s x20, f12\n"                                 
		"feq.ps f23, f15, f15\n"                              
		"fltm.ps m7, f0, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.f16.ps f30, f15\n"                              
		"fsgnjx.s f25, f14, f15\n"                            
		"fadd.ps f1, f0, f0, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fle.pi f1, f0, f13\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fandi.pi f0, f12, 134\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopcz x18, m6\n"                                 
		"cubesgnsc.ps f17, f15, f1\n"                         
		"flt.pi f21, f12, f0\n"                               
		"fcvt.pw.ps f28, f12, dyn\n"                          
		"fadd.pi f30, f15, f14\n"                             


		"flq2 f18, 32(x13)\n"                                 
		"fsatu8.pi f0, f0\n"                                  
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.m.x x11\n"                                      
		"fsgnjn.s f28, f14, f0\n"                             
		"flt.s x24, f13, f13\n"                               
		"fclass.ps f30, f15\n"                                
		"fltm.ps m7, f1, f12\n"                               
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x27\n"                                      
		"fclass.ps f0, f14\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.ps f16, f1, f1, f12, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"cubesgnsc.ps f22, f12, f15\n"                        
		"ffrc.ps f30, f1\n"                                   
		"cubesgntc.ps f23, f1, f1\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fclass.s x30, f14\n"                                 
		"fmul.s f29, f14, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"mova.x.m x21\n"                                      
		"fcvt.s.w f26, x14, dyn\n"                            
		"feq.ps f26, f15, f14\n"                              
		"fslli.pi f23, f15, 11\n"                             
		"fltm.ps m7, f15, f1\n"                               
		"fcvt.ps.pw f30, f13, dyn\n"                          
		"fnmadd.s f20, f14, f1, f0, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fbci.ps f0, 210036\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"faddi.pi f28, f12, 304\n"                            
		"fmvz.x.ps x18, f12, 5\n"                             
		"fsrai.pi f22, f12, 12\n"                             


		"fswizz.ps f17, f13, 98\n"                            




		"maskxor m4, m1, m5\n"                                
		"fmsub.s f31, f15, f1, f14, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"


		"flw f21, 808(x11)\n"                                 


		"flw f24, 1783(x12)\n"                                
		"maskpopc x18, m0\n"                                  
		"fmv.x.s x27, f12\n"                                  

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"maskxor m0, m2, m7\n"                                
		"fmax.ps f19, f13, f0\n"                              
		"fround.ps f1, f13, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.ps f0, f15, f14\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f27, f15, f15\n"                             
		"fadd.ps f21, f15, f12, dyn\n"                        
		"fround.ps f19, f1, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"flt.pi f30, f15, f12\n"                              
		"feq.ps f30, f12, f15\n"                              
		"maskpopcz x31, m6\n"                                 
		"fcvt.pw.ps f17, f15, rne\n"                          
		"fsat8.pi f23, f1\n"                                  
		"fsgnj.ps f18, f0, f1\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"ffrc.ps f21, f0\n"                                   


		"fsw f15, -1956(x11)\n"                               
		"maskor m1, m3, m1\n"                                 
		"frcp.ps f20, f0\n"                                   
		"fmaxu.pi f20, f12, f14\n"                            
		"fsub.s f20, f14, f12, dyn\n"                         
		"feq.s x30, f0, f12\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		//RTLMIN-5479"frsq.ps f24, f1\n"                                   
		"fnot.pi f19, f13\n"                                  
		"fcvt.s.wu f22, x15, dyn\n"                           
		"feq.ps f19, f15, f12\n"                              
		"maskpopcz x0, m4\n"                                  


		"flw.ps f17, -628(x15)\n"                             


		"fcvt.f16.ps f20, f14\n"                              
		"fcvt.pwu.ps f29, f13, dyn\n"                         
		"maskpopcz x28, m7\n"                                 
		"fcvt.pw.ps f23, f0, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"faddi.pi f24, f13, 322\n"                            


		"fsw f0, -1978(x11)\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fpackreph.pi f19, f0\n"                              
		"fnot.pi f21, f15\n"                                  
		"fsgnjn.s f31, f15, f1\n"                             
		"fpackreph.pi f16, f1\n"                              
		"maskor m0, m3, m0\n"                                 
		"fcvt.ps.f16 f18, f1\n"                               




		"fclass.ps f26, f1\n"                                 
		"fle.pi f20, f15, f0\n"                               
		"for.pi f18, f14, f14\n"                              
		"faddi.pi f27, f13, 27\n"                             
		"fcvt.wu.s x24, f1, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fbci.ps f19, 228733\n"                               




		"flq2 f17, -1248(x11)\n"                              
		"fmin.ps f31, f1, f12\n"                              
		"flw.ps f1,  32 (x4)\n"
		"maskpopc x31, m1\n"                                  
		"feqm.ps m6, f1, f15\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fltm.ps m5, f13, f0\n"                               
		"maskand m1, m7, m5\n"                                

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fnmsub.s f20, f15, f14, f1, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"fsat8.pi f16, f12\n"                                 
		"ffrc.ps f19, f15\n"                                  
		"fcvt.pwu.ps f30, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fmin.ps f24, f12, f0\n"                              
		"flem.ps m6, f1, f15\n"                               
		"flw.ps f1,  32 (x4)\n"


		"fbc.ps f27, -1856(x15)\n"                            
		"fmul.ps f25, f0, f14, dyn\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.s f28, f15, f15\n"                             
		"fnmsub.ps f22, f12, f1, f0, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"flog.ps f27, f0\n"                                   
		"fmax.ps f16, f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"


		"flw f25, 1348(x13)\n"                                
		"fbcx.ps f27, x15\n"                                  
		"fcvt.s.w f26, x15, rup\n"                            
		"fsub.s f18, f12, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"frcp.ps f18, f0\n"                                   
		"fmsub.ps f19, f15, f13, f12, dyn\n"                  
		"fandi.pi f26, f0, 349\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.s x30, f14\n"                                 
		"maskxor m0, m1, m4\n"                                
		"for.pi f29, f0, f14\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.pi f24, f12, f15\n"                             
		"fnot.pi f21, f0\n"                                   
		"fmax.ps f26, f1, f14\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f23, f0, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopc x19, m4\n"                                  
		"mova.x.m x21\n"                                      
		"fsrai.pi f27, f1, 15\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.wu f17, x0, rne\n"                            
		"fmadd.ps f26, f15, f15, f12, dyn\n"                  
		"cubeface.ps f16, f1, f12\n"                          
		"flw.ps f1,  32 (x4)\n"


		"fmsub.s f23, f14, f15, f13, dyn\n"                   
		"cubesgntc.ps f18, f13, f13\n"                        






		"flw.ps f1, -1842(x15)\n"                             
		"flw.ps f1,  32 (x4)\n"


		"flw.ps f18, -825(x15)\n"                             
		"fbci.pi f26, 483140\n"                               
		"fnot.pi f17, f13\n"                                  
		"fcvt.pw.ps f23, f15, rtz\n"                          
		"fadd.s f21, f0, f13, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcmovm.ps f23, f14, f0\n"                            
		"fnmsub.ps f19, f13, f0, f12, dyn\n"                  
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsq2 f1, -2016(x11)\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fmaxu.pi f21, f1, f15\n"                             
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f25, f13, 62\n"                             
		"faddi.pi f23, f0, 315\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fbc.ps f1, 1792(x15)\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fltm.ps m6, f15, f13\n"                              
		"fpackrepb.pi f31, f1\n"                              
		"flt.s x22, f0, f12\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmin.ps f24, f14, f14\n"                             
		"mova.x.m x18\n"                                      
		"fcvt.ps.pw f18, f1, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"


		"fswizz.ps f0, f13, 35\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmadd.s f27, f1, f14, f12, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"fadd.s f0, f1, f1, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.f16.ps f28, f12\n"                              




		"fle.ps f17, f15, f0\n"                               
		"fsgnjn.ps f30, f13, f1\n"                            


		"flw.ps f18, 1046(x14)\n"                             
		"fcvt.ps.pwu f20, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"ffrc.ps f29, f12\n"                                  
		"mova.m.x x12\n"                                      
		"maskpopcz x26, m7\n"                                 
		"masknot m2, m3\n"                                    
		"fsrl.pi f22, f15, f13\n"                             
		"fsat8.pi f21, f0\n"                                  
		"fsgnjx.ps f29, f1, f12\n"                            
		"flw.ps f1,  32 (x4)\n"


		//RTLMIN-5479"fsin.ps f1, f12\n"                                   
		"flw.ps f1,  32 (x4)\n"
		"cubeface.ps f30, f14, f14\n"                         
		"fadd.ps f30, f15, f0, dyn\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pwu.ps f22, f13, dyn\n"                         
		"mova.x.m x28\n"                                      
		"fmin.ps f17, f14, f13\n"                             
		"fround.ps f17, f0, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"fcvt.pwu.ps f27, f13, dyn\n"                         
		"flt.ps f21, f15, f12\n"                              
		"fandi.pi f25, f12, 297\n"                            
		"fsgnj.s f30, f13, f14\n"                             
		"fcvt.f16.ps f20, f0\n"                               
		"fle.ps f23, f13, f14\n"                              
		"fpackreph.pi f27, f1\n"                              
		"fsub.ps f16, f12, f15, dyn\n"                        
		"fle.pi f25, f14, f14\n"                              
		"fsrl.pi f28, f12, f14\n"                             
		"fsgnj.s f16, f0, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.wu f0, x13, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltu.pi f25, f0, f13\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fmaxu.pi f19, f15, f0\n"                             
		"fcvt.ps.pwu f19, f15, dyn\n"                         
		"fsub.ps f20, f15, f15, dyn\n"                        
		"fsrai.pi f26, f13, 3\n"                              
		"fnmadd.ps f31, f13, f13, f12, dyn\n"                 


		"flw.ps f28, 0(x11)\n"                                
		"fround.ps f24, f1, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f25, f1, f15, dyn\n"                         
		"flw.ps f1,  32 (x4)\n"
		"fmul.ps f25, f15, f12, dyn\n"                        
		"fnot.pi f24, f14\n"                                  








		"fcvt.s.wu f29, x12, dyn\n"                           
		"fltm.ps m5, f15, f14\n"                              

		"li x5, 0x46010ff3c2d3568d\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmul.pi f19, f14, f0\n"                              


		"fswizz.ps f16, f12, 115\n"                           
		"fsgnj.ps f0, f1, f12\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fand.pi f30, f0, f13\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"faddi.pi f17, f15, 317\n"                            
		"fminu.pi f23, f0, f1\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubesgnsc.ps f30, f12, f13\n"                        
		"fcvt.ps.pwu f24, f13, rup\n"                         
		"fbcx.ps f31, x11\n"                                  
		"fcvt.ps.pw f18, f12, dyn\n"                          
		"fmsub.s f22, f13, f15, f14, rdn\n"                   
		"fadd.pi f22, f15, f13\n"                             
		"fnmsub.s f30, f14, f13, f15, dyn\n"                  




		//RTLMIN-5479"fsin.ps f26, f1\n"                                   
		"cubeface.ps f30, f0, f13\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmadd.ps f20, f14, f14, f13, dyn\n"                  
		"fbci.ps f17, 105393\n"                               
		"fsgnjn.s f31, f12, f13\n"                            
		"maskor m2, m0, m0\n"                                 
		"fltu.pi f19, f1, f13\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x20, f0, dyn\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmsub.ps f25, f14, f12, f1, dyn\n"                  
		"flw.ps f1,  32 (x4)\n"


		"flw f26, 1246(x15)\n"                                
		"fle.pi f29, f15, f1\n"                               
		"fmadd.ps f26, f14, f12, f14, dyn\n"                  
		"fsra.pi f28, f0, f12\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.wu.s x24, f13, dyn\n"                           
		"fmvz.x.ps x18, f13, 5\n"                             
		"fmin.s f17, f15, f15\n"                              
		"fltm.pi m1, f0, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fclass.ps f31, f0\n"                                 
		"fbci.pi f0, 52017\n"                                 
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fle.pi f22, f1, f12\n"                               
		"flw.ps f1,  32 (x4)\n"
		"feqm.ps m5, f13, f1\n"                               
		"fsrl.pi f26, f1, f15\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fltm.pi m0, f12, f0\n"                               
		"fxor.pi f27, f1, f13\n"                              
		"flw.ps f1,  32 (x4)\n"
		"flt.ps f28, f12, f14\n"                              
		"fmul.pi f0, f12, f13\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmulh.pi f24, f0, f15\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubesgnsc.ps f28, f14, f1\n"                         
		"fcvt.w.s x24, f12, dyn\n"                            
		"fcvt.ps.pw f1, f15, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"
		"faddi.pi f16, f13, 391\n"                            
		"cubefaceidx.ps f30, f14, f14\n"                      
		"fcmovm.ps f18, f1, f15\n"                            
		"flw.ps f1,  32 (x4)\n"
		"fand.pi f22, f0, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"cubesgntc.ps f1, f12, f0\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fsatu8.pi f0, f1\n"                                  
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"mova.x.m x22\n"                                      
		"fltm.pi m0, f15, f12\n"                              


		"flw f22, -415(x15)\n"                                
		"fsatu8.pi f19, f15\n"                                
		"fsrli.pi f20, f1, 5\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fnmadd.s f31, f13, f13, f0, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f17, f13, dyn\n"                         
		"fle.s x24, f13, f12\n"                               
		"fcvt.s.wu f24, x0, dyn\n"                            
		"fmax.s f30, f14, f12\n"                              
		"ffrc.ps f16, f15\n"                                  
		"maskpopcz x0, m4\n"                                  
		"flt.pi f29, f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"




		"fltm.pi m6, f0, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.wu.s x19, f0, dyn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.pi f17, f1, f14\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.s f18, f14, f15\n"                            
		"masknot m2, m3\n"                                    
		"flem.ps m4, f1, f13\n"                               
		"flw.ps f1,  32 (x4)\n"


		"feq.ps f18, f0, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.ps f24, f12, f14\n"                           
		"flem.ps m7, f15, f13\n"                              




		"fcvt.s.w f1, x14, rmm\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fmin.ps f0, f12, f0\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.ps f27, f1, f12, dyn\n"                         
		"flw.ps f1,  32 (x4)\n"


		"fltu.pi f29, f12, f13\n"                             
		"fmvz.x.ps x24, f12, 1\n"                             
		"fle.ps f29, f14, f15\n"                              
		"fcvt.ps.pw f23, f13, rne\n"                          
		//RTLMIN-5479"frsq.ps f22, f0\n"                                   
		"fmin.s f0, f12, f12\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.pwu f1, f12, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"masknot m2, m1\n"                                    
		"fsgnjx.s f23, f14, f13\n"                            
		"fand.pi f30, f14, f12\n"                             
		"fsatu8.pi f24, f15\n"                                
		"fmin.pi f30, f15, f15\n"                             
		"fsub.s f22, f1, f15, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"




		"fminu.pi f31, f0, f1\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvz.x.ps x1, f14, 5\n"                              
		"masknot m0, m7\n"                                    
		"fslli.pi f30, f0, 14\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.s f19, f13, f14, dyn\n"                         
		"fcvt.pwu.ps f20, f13, dyn\n"                         


		"fsw f1, -1965(x13)\n"                                
		"flw.ps f1,  32 (x4)\n"
		"maskpopc x22, m0\n"                                  
		"fmadd.s f23, f13, f14, f1, dyn\n"                    
		"flw.ps f1,  32 (x4)\n"
		"fmin.s f30, f15, f15\n"                              




		"fclass.ps f23, f12\n"                                

		"li x5, 0x9cb69c49e3a26061\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flt.s x24, f14, f12\n"                               
		"fsgnjn.s f18, f14, f12\n"                            
		"fclass.ps f24, f1\n"                                 
		//RTLMIN-5479"fsin.ps f23, f15\n"                                  
		"fadd.s f23, f1, f13, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fltu.pi f28, f13, f13\n"                             




		"flt.pi f0, f12, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m7, x14, 62\n"                               
		"maskor m1, m0, m0\n"                                 


		"flw.ps f24, -892(x12)\n"                             
		"feqm.ps m0, f15, f14\n"                              
		"mova.m.x x0\n"                                       


		"fswizz.ps f28, f15, 96\n"                            
		"fmaxu.pi f26, f0, f14\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m4, x12, 83\n"                               
		"fcvt.wu.s x22, f14, dyn\n"                           
		"fcvt.s.w f16, x15, dyn\n"                            


		"flw f31, -586(x11)\n"                                
		"fmadd.s f21, f13, f12, f0, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsra.pi f28, f12, f1\n"                              
		"fcvt.f16.ps f16, f15\n"                              
		"frcp.ps f29, f14\n"                                  
		"flt.pi f28, f15, f14\n"                              


		"fsw f14, 308(x15)\n"                                 
		"masknot m2, m2\n"                                    
		"fmul.pi f1, f15, f13\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fmul.s f28, f12, f15, dyn\n"                         
		"maskor m1, m4, m6\n"                                 
		"flem.ps m0, f1, f15\n"                               
		"flw.ps f1,  32 (x4)\n"


		"flw.ps f24, -1252(x11)\n"                            
		"fmadd.s f16, f14, f12, f14, dyn\n"                   


		"flw f29, 1825(x12)\n"                                
		"fslli.pi f18, f13, 1\n"                              
		"feq.ps f31, f0, f12\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsatu8.pi f22, f0\n"                                 
		//RTLMIN-5479"fsin.ps f0, f14\n"                                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsll.pi f31, f14, f13\n"                             
		"fpackrepb.pi f23, f1\n"                              
		"fmin.pi f31, f13, f15\n"                             
		"fsetm.pi m3, f12\n"                                  
		"fandi.pi f17, f14, 46\n"                             
		"fnmadd.s f17, f15, f1, f13, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"maskpopcz x26, m1\n"                                 




		"fpackreph.pi f23, f15\n"                             
		"fpackreph.pi f0, f12\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmax.pi f0, f1, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"


		"flq2 f31, -960(x11)\n"                               


		"fcvt.pwu.ps f19, f15, rtz\n"                         
		"fsrl.pi f18, f1, f1\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.s f30, f13, f13\n"                            

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fsgnjx.ps f24, f13, f0\n"                            
		"fmul.ps f28, f14, f15, dyn\n"                        
		"fnot.pi f17, f13\n"                                  
		"maskpopcz x16, m5\n"                                 
		"flt.ps f26, f1, f14\n"                               
		"flw.ps f1,  32 (x4)\n"
		"maskand m5, m4, m5\n"                                
		"maskpopc x17, m0\n"                                  
		"maskpopc x30, m2\n"                                  
		"fcvt.ps.pwu f22, f13, dyn\n"                         
		"maskpopcz x26, m4\n"                                 


		"feq.ps f31, f13, f1\n"                               
		"fcvt.f16.ps f27, f0\n"                               
		"fsrl.pi f23, f15, f0\n"                              
		"faddi.pi f29, f0, 304\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero




		"mov.m.x m6, x0, 40\n"                                








		"fmul.pi f0, f13, f1\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.f16 f17, f13\n"                              
		"fmsub.ps f24, f15, f0, f13, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.pi f30, f15, f13\n"                             
		"fmin.ps f22, f1, f14\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fsgnjn.s f1, f0, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f25, f13, f13\n"                             
		//RTLMIN-5479"frsq.ps f27, f13\n"                                  
		"masknot m2, m7\n"                                    
		"fmin.ps f17, f0, f1\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopcz x31, m1\n"                                 
		"fcvt.pw.ps f24, f1, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"
		"fandi.pi f16, f15, 485\n"                            
		//RTLMIN-5479"frsq.ps f30, f12\n"                                  
		"fmvz.x.ps x31, f12, 7\n"                             
		"feq.ps f1, f0, f12\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fmsub.s f1, f12, f1, f13, dyn\n"                     
		"flw.ps f1,  32 (x4)\n"








		"fcvt.wu.s x21, f1, dyn\n"                            
		"flw.ps f1,  32 (x4)\n"
		"mova.m.x x15\n"                                      
		"fand.pi f21, f13, f15\n"                             
		"fcvt.pw.ps f26, f13, dyn\n"                          


		"maskpopcz x20, m1\n"                                 
		"fmvs.x.ps x16, f12, 6\n"                             
		"masknot m1, m5\n"                                    
		"fslli.pi f26, f12, 4\n"                              
		//RTLMIN-5479"frsq.ps f19, f14\n"                                  
		"fsetm.pi m7, f15\n"                                  
		"feq.s x23, f0, f15\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.s.wu f30, x1, dyn\n"                            
		"fsat8.pi f26, f15\n"                                 
		"flog.ps f19, f1\n"                                   
		"fsetm.pi m6, f13\n"                                  
		"frcp.ps f30, f14\n"                                  
		"fmulh.pi f21, f13, f1\n"                             
		"masknot m5, m7\n"                                    

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"cubefaceidx.ps f29, f13, f14\n"                      
		"fcvt.ps.pwu f31, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.ps f0, f13, f15, f0, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw.ps f1, 1216(x13)\n"                              
		"flw.ps f1,  32 (x4)\n"




		"fsgnjx.ps f27, f0, f14\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero






		"fsq2 f13, -1952(x13)\n"                              
		"fexp.ps f20, f13\n"                                  
		"fbcx.ps f22, x12\n"                                  
		"fcvt.ps.pwu f1, f13, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fmv.x.s x0, f1\n"                                    
		"flog.ps f16, f1\n"                                   


		"fbc.ps f28, -1792(x15)\n"                            
		"faddi.pi f29, f15, 332\n"                            
		"masknot m5, m0\n"                                    
		"mova.m.x x12\n"                                      
		"fle.ps f31, f13, f15\n"                              
		"fmadd.s f16, f15, f12, f12, rup\n"                   
		"faddi.pi f24, f0, 440\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsw.ps f12, 1199(x14)\n"                             
		"fadd.s f22, f15, f14, dyn\n"                         
		"for.pi f30, f13, f13\n"                              


		"fsw f0, -1951(x15)\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbcx.ps f22, x13\n"                                  




		"fslli.pi f24, f13, 6\n"                              
		"fsub.s f19, f0, f1, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fltm.pi m3, f12, f14\n"                              
		"fsgnjn.ps f23, f12, f12\n"                           
		"fsrli.pi f19, f1, 14\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fminu.pi f29, f13, f1\n"                             
		//RTLMIN-5479"frsq.ps f31, f13\n"                                  
		"fand.pi f24, f1, f13\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fsatu8.pi f20, f15\n"                                
		"fxor.pi f16, f0, f12\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnot.pi f24, f12\n"                                  
		"fadd.pi f25, f15, f12\n"                             






		"fbc.ps f19, 768(x12)\n"                              
		//RTLMIN-5479"frsq.ps f22, f1\n"                                   
		"fmsub.ps f24, f14, f12, f12, dyn\n"                  
		"fbci.ps f16, 286590\n"                               
		"fsgnjx.s f21, f15, f14\n"                            


		"fbc.ps f17, -1536(x14)\n"                            
		"fsat8.pi f23, f13\n"                                 
		"fbcx.ps f24, x11\n"                                  
		"fpackrepb.pi f24, f13\n"                             


		"maskand m4, m1, m0\n"                                


		"fbc.ps f23, 256(x12)\n"                              
		"fmul.s f21, f12, f14, dyn\n"                         
		"fslli.pi f16, f15, 7\n"                              
		"fcmov.ps f27, f1, f0, f13\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fcvt.wu.s x17, f14, dyn\n"                           

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmv.s.x f18, x11\n"                                  
		"fltm.ps m3, f0, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsll.pi f19, f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"
		"maskand m0, m3, m3\n"                                
		"fcvt.pw.ps f22, f15, dyn\n"                          
		"fsrai.pi f24, f12, 11\n"                             
		"fnmadd.ps f20, f15, f13, f13, dyn\n"                 
		"fsub.pi f24, f12, f13\n"                             
		"fsra.pi f21, f13, f14\n"                             
		"mov.m.x m5, x14, 79\n"                               
		"fsetm.pi m3, f15\n"                                  


		"flq2 f18, -1344(x14)\n"                              
		"fcvt.w.s x31, f13, dyn\n"                            
		"fcvt.s.w f31, x0, dyn\n"                             
		"mov.m.x m3, x0, 102\n"                               
		"fmadd.ps f22, f13, f1, f15, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"fsub.ps f26, f14, f12, dyn\n"                        
		"cubeface.ps f21, f12, f12\n"                         




		"fcvt.s.w f28, x12, dyn\n"                            
		"fcvt.f16.ps f24, f13\n"                              
		"fmaxu.pi f29, f12, f12\n"                            
		"fslli.pi f20, f0, 8\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.x.m x27\n"                                      
		"fbcx.ps f29, x13\n"                                  


		"maskpopc x26, m7\n"                                  
		"fltu.pi f29, f0, f0\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.w.s x19, f0, dyn\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnjx.ps f30, f15, f0\n"                            
		"fmul.pi f31, f0, f13\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mova.m.x x12\n"                                      
		"frcp.ps f22, f12\n"                                  
		"maskpopcz x24, m6\n"                                 
		"fexp.ps f27, f0\n"                                   
		"fmv.x.s x19, f14\n"                                  
		"fmsub.ps f1, f1, f0, f0, dyn\n"                      
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fxor.pi f26, f14, f1\n"                              
		"cubesgntc.ps f24, f14, f15\n"                        
		"fbci.pi f29, 515786\n"                               
		"flt.pi f0, f0, f14\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flem.ps m6, f14, f15\n"                              
		"mova.m.x x12\n"                                      
		"fmul.ps f26, f13, f12, dyn\n"                        
		"maskxor m1, m2, m0\n"                                
		"fmax.ps f21, f15, f12\n"                             
		"fmv.s.x f24, x12\n"                                  
		"flt.s x31, f14, f13\n"                               
		"fsatu8.pi f31, f13\n"                                
		"maskxor m5, m2, m5\n"                                




		"fclass.ps f21, f12\n"                                
		"fsub.pi f26, f13, f1\n"                              

		"li x5, 0x236ffbc6916551a0\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fbci.ps f27, 448932\n"                               
		"fslli.pi f27, f1, 4\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fcvt.wu.s x0, f15, dyn\n"                            
		"mova.x.m x24\n"                                      
		"for.pi f24, f15, f1\n"                               
		"fsub.ps f22, f12, f14, dyn\n"                        




		"fminu.pi f24, f15, f12\n"                            
		"fcmov.ps f29, f0, f12, f0\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feq.ps f0, f1, f15\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.ps f27, f0, f15\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"feqm.ps m1, f15, f12\n"                              
		"maskpopcz x21, m3\n"                                 
		"for.pi f25, f15, f14\n"                              
		"fmvs.x.ps x25, f12, 2\n"                             
		"fcmovm.ps f16, f15, f12\n"                           
		"fmax.ps f27, f12, f15\n"                             
		"fmaxu.pi f18, f14, f13\n"                            
		"fpackrepb.pi f0, f15\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.s f23, f13, f15, dyn\n"                         
		"fcvt.wu.s x1, f0, dyn\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.ps f29, f1, f13, f0, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"flog.ps f26, f0\n"                                   
		"fsgnjx.ps f26, f14, f14\n"                           




		"fminu.pi f27, f12, f15\n"                            
		"fnot.pi f18, f13\n"                                  
		"fcvt.ps.f16 f29, f1\n"                               




		"masknot m7, m1\n"                                    


		"flq2 f1, -1760(x15)\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsatu8.pi f31, f0\n"                                 


		"fbc.ps f24, -1728(x11)\n"                            
		"cubesgntc.ps f23, f12, f13\n"                        
		"fle.s x23, f14, f0\n"                                
		"masknot m6, m4\n"                                    
		"fmvs.x.ps x28, f14, 0\n"                             
		"maskand m7, m3, m0\n"                                


		"fswizz.ps f17, f15, 39\n"                            
		"fbcx.ps f31, x11\n"                                  
		"fcmovm.ps f22, f15, f0\n"                            
		"for.pi f21, f1, f13\n"                               
		"flw.ps f1,  32 (x4)\n"
		"maskpopc x27, m2\n"                                  
		"fsrai.pi f23, f13, 8\n"                              


		"flw f17, 961(x14)\n"                                 
		"fsub.s f0, f1, f13, dyn\n"                           
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"




		"fsub.ps f1, f12, f14, dyn\n"                         
		"flw.ps f1,  32 (x4)\n"
		"fmsub.ps f0, f12, f12, f14, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.ps.f16 f26, f1\n"                               
		"fsatu8.pi f1, f12\n"                                 
		"flw.ps f1,  32 (x4)\n"
		"fltm.ps m5, f0, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fnmadd.ps f16, f15, f1, f0, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"




		"flog.ps f18, f0\n"                                   

		"li x5, 0xffffffffffffffff\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmsub.ps f24, f15, f15, f15, dyn\n"                  
		"flt.pi f17, f15, f12\n"                              


		"fsw f13, -1660(x15)\n"                               
		"fmul.ps f24, f12, f14, dyn\n"                        
		"fsat8.pi f16, f0\n"                                  
		"fmin.s f18, f12, f1\n"                               
		"fle.pi f23, f15, f15\n"                              
		"fsat8.pi f24, f14\n"                                 
		"fmin.pi f25, f13, f14\n"                             
		"flt.ps f26, f12, f13\n"                              
		"fsgnjx.s f28, f1, f13\n"                             
		"flw.ps f1,  32 (x4)\n"


		"fsw f1, -1182(x15)\n"                                
		"flw.ps f1,  32 (x4)\n"
		"fcvt.s.w f23, x11, dyn\n"                            
		"fmul.s f17, f1, f13, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"




		"fcvt.pwu.ps f20, f15, dyn\n"                         
		"fmulh.pi f21, f14, f14\n"                            
		"flem.ps m3, f0, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.ps m6, f0, f15\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fbci.ps f28, 83412\n"                                
		"flem.ps m2, f14, f14\n"                              
		"fcvt.pwu.ps f17, f15, rdn\n"                         
		"cubeface.ps f21, f15, f0\n"                          
		"fbci.ps f28, 140244\n"                               
		"masknot m7, m5\n"                                    
		"maskpopcz x23, m1\n"                                 
		"feq.ps f0, f14, f0\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.w.s x23, f12, dyn\n"                            
		"maskpopcz x31, m0\n"                                 




		"fsgnjn.ps f18, f12, f13\n"                           
		"fsub.pi f30, f12, f0\n"                              
		"maskxor m4, m5, m0\n"                                
		"fmin.pi f19, f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fcvt.pw.ps f21, f15, rtz\n"                          
		"ffrc.ps f27, f12\n"                                  
		"mova.x.m x25\n"                                      


		"fbc.ps f20, -1280(x13)\n"                            
		"fsub.pi f29, f12, f15\n"                             
		"faddi.pi f21, f13, 1\n"                              
		"fle.pi f23, f0, f13\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fcvt.pwu.ps f1, f14, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f22, f12, 6\n"                              
		"fand.pi f31, f14, f15\n"                             
		"mov.m.x m7, x13, 69\n"                               
		"fsgnjx.s f27, f13, f0\n"                             
		"fmadd.ps f17, f1, f0, f1, dyn\n"                     
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"flt.pi f17, f14, f14\n"                              
		"fpackreph.pi f31, f12\n"                             
		"fsatu8.pi f30, f12\n"                                
		"fle.ps f16, f13, f12\n"                              


		"flw.ps f17, -1900(x12)\n"                            

		"li x5, 0x942666865d4bcc8\n"                         // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fmulh.pi f22, f12, f13\n"                            
		"maskpopcz x19, m3\n"                                 
		"fsgnj.ps f0, f12, f13\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fltm.pi m6, f12, f14\n"                              
		"fnmsub.s f19, f13, f1, f13, dyn\n"                   
		"flw.ps f1,  32 (x4)\n"
		"maskxor m5, m0, m6\n"                                
		"flog.ps f16, f1\n"                                   




		"fmvz.x.ps x24, f1, 4\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fmulh.pi f28, f1, f15\n"                             
		"flw.ps f1,  32 (x4)\n"


		"fswizz.ps f24, f0, 102\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.s x24, f14, f0\n"                                




		"fmsub.ps f25, f14, f13, f0, dyn\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fmin.pi f27, f0, f14\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmul.s f30, f13, f1, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"fslli.pi f24, f0, 5\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flq2 f16, -1408(x15)\n"                              
		"fle.s x17, f14, f12\n"                               
		"mova.m.x x14\n"                                      
		"fsub.ps f30, f15, f1, dyn\n"                         
		"flw.ps f1,  32 (x4)\n"
		"fcvt.wu.s x26, f0, rdn\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskand m4, m0, m3\n"                                
		"fsetm.pi m4, f14\n"                                  
		"flt.ps f22, f0, f1\n"                                
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsub.s f0, f15, f14, rup\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"fsq2 f13, -1088(x13)\n"                              


		"fmulhu.pi f1, f13, f15\n"                            
		"flw.ps f1,  32 (x4)\n"








		"fmax.pi f21, f15, f1\n"                              
		//RTLMIN-5479"fsin.ps f26, f12\n"                                  
		"fmadd.ps f23, f15, f12, f12, dyn\n"                  
		"fpackrepb.pi f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsgnj.ps f28, f1, f12\n"                             
		"flw.ps f1,  32 (x4)\n"
		"fadd.ps f23, f12, f12, dyn\n"                        




		"maskxor m5, m6, m7\n"                                
		"fcvt.ps.pwu f16, f12, dyn\n"                         


		"flw.ps f22, 1150(x11)\n"                             
		"maskor m1, m4, m5\n"                                 
		"fbci.ps f29, 72770\n"                                
		"fsrli.pi f23, f0, 8\n"                               
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"maskpopc x27, m4\n"                                  
		"flt.s x1, f1, f0\n"                                  
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.ps f1, f0, f0\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"fclass.ps f1, f13\n"                                 
		"flw.ps f1,  32 (x4)\n"
		"fcvt.w.s x25, f0, dyn\n"                             
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmvz.x.ps x31, f1, 5\n"                              
		"flw.ps f1,  32 (x4)\n"
		"fadd.pi f20, f15, f1\n"                              
		"flog.ps f24, f15\n"                                  
		"fmaxu.pi f30, f14, f14\n"                            
		"fnmsub.ps f0, f15, f0, f0, dyn\n"                    
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsrli.pi f1, f15, 9\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fsgnjx.ps f29, f12, f1\n"                            

		"li x5, 0xf0f0\n"                                     // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"flt.ps f22, f14, f12\n"                              
		"fbcx.ps f24, x11\n"                                  
		"fcmovm.ps f25, f1, f1\n"                             
		"flw.ps f1,  32 (x4)\n"


		"fbc.ps f18, -832(x11)\n"                             
		//RTLMIN-5479"fsin.ps f17, f0\n"                                   
		"fsrl.pi f26, f14, f15\n"                             
		"cubeface.ps f16, f0, f12\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fadd.s f20, f13, f14, rtz\n"                         
		"fcvt.pwu.ps f18, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"frcp.ps f18, f1\n"                                   
		"fcvt.ps.pwu f23, f14, dyn\n"                         




		"fsll.pi f26, f13, f1\n"                              
		"feq.ps f28, f14, f0\n"                               
		"fslli.pi f23, f1, 10\n"                              
		"flw.ps f1,  32 (x4)\n"


		"fsw f13, -1307(x15)\n"                               
		"fltm.ps m2, f14, f1\n"                               
		"fcvt.f16.ps f23, f1\n"                               
		"fcvt.ps.pwu f1, f12, dyn\n"                          
		"flw.ps f1,  32 (x4)\n"
		"flt.s x30, f14, f12\n"                               
		"fadd.ps f25, f0, f15, dyn\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m6, x12, 61\n"                               
		"fandi.pi f23, f14, 337\n"                            
		"maskor m4, m4, m5\n"                                 




		"fle.ps f30, f1, f15\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fmin.s f25, f15, f14\n"                              
		"fsra.pi f25, f15, f13\n"                             
		"fsrai.pi f20, f13, 4\n"                              
		"fslli.pi f27, f14, 2\n"                              
		"fcvt.pwu.ps f21, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero


		"flw f19, -1286(x11)\n"                               
		"fmv.x.s x26, f12\n"                                  
		"fbci.ps f29, 60879\n"                                
		"fcvt.f16.ps f25, f15\n"                              
		"fmvs.x.ps x27, f12, 1\n"                             
		"fnmsub.ps f31, f13, f1, f0, rup\n"                   
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flw.ps f1,  32 (x4)\n"
		"cubesgntc.ps f31, f15, f14\n"                        
		"fandi.pi f29, f15, 335\n"                            
		"fmulh.pi f0, f15, f0\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"mov.m.x m5, x13, 100\n"                              
		"fandi.pi f1, f1, 260\n"                              
		"flw.ps f1,  32 (x4)\n"




		"fle.pi f18, f13, f0\n"                               
		"fsrai.pi f27, f0, 13\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsat8.pi f25, f13\n"                                 
		"maskpopcz x24, m6\n"                                 
		"fclass.ps f17, f15\n"                                
		"fadd.ps f27, f0, f14, rup\n"                         
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fsgnj.s f25, f14, f14\n"                             
		"masknot m1, m4\n"                                    
		"fnmsub.s f30, f12, f14, f12, dyn\n"                  


		"fswizz.ps f25, f0, 109\n"                            
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero

		"li x5, 0xffffffff\n"                                 // set mask regs
		"mova.m.x x5\n"                                       // set mask regs

		"fbci.pi f30, 364549\n"                               
		"fmul.s f0, f14, f12, rup\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"fmv.s.x f1, x0\n"                                    
		"flw.ps f1,  32 (x4)\n"
		"fsll.pi f20, f13, f12\n"                             


		"fswizz.ps f30, f14, 87\n"                            
		"fmaxu.pi f26, f14, f1\n"                             


		"flw.ps f19, 135(x13)\n"                              
		"fcvt.pwu.ps f17, f0, dyn\n"                          
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
		"flt.s x16, f13, f0\n"                                




		"mova.m.x x1\n"                                       
		"mov.m.x m5, x1, 124\n"                               
		"fcvt.pw.ps f20, f1, dyn\n"                           
		"flw.ps f1,  32 (x4)\n"
		"fle.ps f18, f1, f12\n"                               
		"flw.ps f1,  32 (x4)\n"
		"fmulhu.pi f28, f13, f13\n"                           
		"fandi.pi f22, f13, 251\n"                            
		"maskor m2, m7, m6\n"                                 
		"mov.m.x m1, x0, 56\n"                                
		"fclass.s x27, f13\n"                                 
		"fand.pi f18, f1, f0\n"                               
		"flw.ps f1,  32 (x4)\n"
		"maskor m3, m2, m5\n"                                 
		"fclass.ps f30, f0\n"                                 
		"fmax.pi f23, f0, f12\n"                              
		"fcvt.s.w f0, zero\n"                                 // reset f0 to zero
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
